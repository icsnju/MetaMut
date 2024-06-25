#!/usr/bin/env python3

import re
import os
import sys
import pathlib
import subprocess

class EmptyClass:
  pass

def parse_args():
  args = EmptyClass()
  args.ifiles = []
  args.ifiles_idxs = []
  args.mode = '-c-l' # compile and link
  i = 1
  while i < len(sys.argv):
    if sys.argv[i] == '-o' and i + 1 < len(sys.argv):
      args.ofile = sys.argv[i + 1]
      args.ofile_idx = i + 1
      i += 1
    elif sys.argv[i] == '-x' and i + 1 < len(sys.argv):
      args.ftype = sys.argv[i + 1]
      i += 1
    elif sys.argv[i].startswith('-x'):
      args.ftype = sys.argv[i][2:]
    elif sys.argv[i] == '-c':
      args.mode = '-c'
    elif sys.argv[i] == '-S':
      args.mode = '-S'
    elif sys.argv[i] == '-E':
      args.mode = '-E'
    elif sys.argv[i] == '-I' or \
         sys.argv[i] == '-Xclang' or \
         sys.argv[i] == '-mllvm' or \
         sys.argv[i] == '-Xpreprocessor' or \
         sys.argv[i] == '-Xlinker' or \
         sys.argv[i] == '-Xassembler':
      i += 1
    elif sys.argv[i] == '--':
      if i + 1 < len(sys.argv):
        args.ifiles = sys.argv[i + 1:]
        args.ifiles_idxs.extend(range(i + 1,
          len(sys.argv)))
      break
    elif sys.argv[i].startswith('-'):
      pass
    else:
      args.ifiles_idxs.append(i)
      args.ifiles.append(sys.argv[i])
    i += 1
  if 'ofile' not in args.__dict__ and len(args.ifiles) == 1:
    ifile = pathlib.Path(args.ifiles[0])
    if args.mode == '-c':
      args.ofile = str(ifile.with_suffix('.o'))
    elif args.mode == '-S':
      args.ofile = str(ifile.with_suffix('.s'))
    elif args.mode == '-c-l':
      args.ofile = 'a.out'
  return args

def edit_argv(args):
  if args.mode == '-E': return None
  argv = [e for e in sys.argv]
  if 'ofile_idx' in args.__dict__:
    argv[args.ofile_idx] = args.ofile + '.bc'
  else:
    argv.extend(['-o', args.ofile + '.bc'])
  if len(args.ifiles) == 0: return None
  suffix = pathlib.Path(args.ifiles[0]).suffix
  ifile_is_source = False
  if 'ftype' in args.__dict__ or \
      suffix.lower() not in ['.o', '.so', '.a']:
    ifile_is_source = True
  if ifile_is_source:
    if len(args.ifiles) == 1:
      ftype = None
      if 'ftype' in args.__dict__:
        ftype = args.ftype
      else:
        if suffix.lower() in ['.c', '.h']:
          argv[0] = 'clang'
        elif suffix.lower() in ['.cc', '.cpp', '.cxx', '.hpp']:
          argv[0] = 'clang++'
        else:
          argv[0] = 'clang'
      argv.append('-emit-llvm')
      if '-c' not in argv:
        argv.append('-c')
    else:
      assert False
  else:
    argv[0] = 'llvm-link'
    for i in args.ifiles_idxs:
      argv[i] += '.bc'
    i = 1
    while i < len(argv):
      if argv[i] == '-fPIC' or \
         argv[i] == '-g' or \
         argv[i] == '-shared' or \
         argv[i] == '-pedantic' or \
         argv[i].startswith('-l') or \
         argv[i].startswith('-f') or \
         argv[i].startswith('-W'):
        argv.pop(i)
        continue
      i += 1
  return argv

app = str(pathlib.Path(sys.argv[0]).name)

args = parse_args()
# print(' '.join(new_argv), file=sys.stderr)
# normal execution
p = subprocess.Popen([app, *sys.argv[1:]])
p.communicate()

if 'ofile' in args.__dict__:
  # hacked execution
  new_argv = edit_argv(args)
  fake_p = subprocess.Popen(new_argv)
  fake_p.communicate()
  if new_argv[0] == 'llvm-link':
    r = re.match(r'(.*)\.so\.(\d+)$', args.ofile)
    if r:
      ofile_p = pathlib.Path(args.ofile).absolute()
      odir = str(ofile_p.parent)
      old_bc = str(ofile_p.name) + '.bc'
      new_bc = str(ofile_p.with_suffix('.bc'))
      os.system(f'cd {odir}; ln -s {old_bc} {new_bc}')

sys.exit(p.returncode)
