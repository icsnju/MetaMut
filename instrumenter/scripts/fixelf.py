#!/usr/bin/env python3
import json
import struct
import pathlib
import argparse
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from elftools.elf.constants import SH_FLAGS

class ElfModifier:
  def __init__(self):
    pass

  def modify_int32(self, filename, objname, newvalue):
    self._modify_integer(filename, objname, newvalue, 4)
  
  def modify_int64(self, filename, objname, newvalue):
    self._modify_integer(filename, objname, newvalue, 8)
  
  def modify_string(self, filename, objname, newvalue):
    with open(filename, 'r+b') as f:
      elf = ELFFile(f)
      symbol, section, offset = self._find_symbol(elf, objname)
      f.seek(section['sh_offset'] + offset)
      # Write the new null-terminated string value
      new_string_value = newvalue.encode('utf-8') + b'\x00'
      f.write(new_string_value)

  def _modify_integer(self, filename, objname, newvalue, size):
    with open(filename, 'r+b') as f:
      elf = ELFFile(f)
      symbol, section, offset = self._find_symbol(elf, objname)
      f.seek(section['sh_offset'] + offset)
      # Write the new integer value
      new_integer_value = newvalue.to_bytes(size, byteorder='little')
      f.write(new_integer_value)

  def _find_symbol(self, elf, name):
    symbol = None
    symtab = None

    # Iterate over all sections and search for a SymbolTableSection
    for section in elf.iter_sections():
      if not isinstance(section, SymbolTableSection): continue
      symbol = section.get_symbol_by_name(name)
      if symbol:
        symtab = section
        break  # Break if we find the symbol

    if not symbol:
      raise ValueError(f"Symbol {name} not found in any symbol table section.")

    symbol = symbol[0]  # get_symbol_by_name returns a list

    # Get the section containing the symbol
    section_index = symbol['st_shndx']
    if section_index == 'SHN_UNDEF' or section_index >= elf.num_sections():
      raise ValueError(f"Invalid section index {section_index} for symbol {name}.")

    containing_section = elf.get_section(section_index)
    offset = symbol['st_value'] - containing_section['sh_addr']
    return symbol, containing_section, offset

if __name__ == "__main__":
  parser = argparse.ArgumentParser('tracer')
  parser.add_argument('elffile',
      help='specify elf file to fix')
  parser.add_argument('-i', dest='ifile',
      type=str, default='configs.json',
      help='specify json file to store the compilation database')
  args = parser.parse_args()

  j = json.loads(pathlib.Path(args.ifile).read_text())

  mod = ElfModifier()
  mod.modify_int32(args.elffile, "__afl_num_of_branches", j['nbrs'])
  assert len(j['bbmap.dir']) < 256 - 1
  mod.modify_string(args.elffile, "__afl_bbmap_dir", j['bbmap.dir'])
