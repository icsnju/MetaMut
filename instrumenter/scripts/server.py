#!/usr/bin/env python3

import argparse
import json
import numpy as np
import os
import struct
import subprocess
import sys
import sysv_ipc
import threading
import pathlib

class ShmObj(sysv_ipc.SharedMemory):
  def __init__(self, size=1024, private=True):
    super().__init__(
      sysv_ipc.IPC_PRIVATE if private else sysv_ipc.IPC_CREAT,
      size=size,
      flags=sysv_ipc.IPC_CREX,
      init_character=b'\0')
  def read_counter(self, index=0):
    int_bytes = self.read(4, index*4)
    int_value = struct.unpack('i', int_bytes)[0]
    return int_value
  def write_counter(self, value, index=0):
    int_bytes = struct.pack('i', value)
    self.write(int_bytes, index*4)
  def close(self):
    self.detach()
    self.remove()
  def write_zeros(self):
    zero_bytes = bytes(self.size)
    self.write(zero_bytes)
  def to_numpy(self):
    return np.frombuffer(self.read(),
        dtype=np.uint8, count=self.size)

def count_lines(map_dir):
  total_lines, total_funcs = 0, 0
  for root, _, files in os.walk(map_dir):
    for file in files:
      file_path = os.path.join(root, file)
      if not os.path.isfile(file_path): continue
      if file.startswith('bb.'):
        with open(file_path, 'r') as f:
          total_lines += sum(1 for _ in f)
      elif file.startswith('fn.'):
        with open(file_path, 'r') as f:
          total_funcs += sum(1 for _ in f)
  return total_lines, total_funcs

if __name__ == "__main__":
  parser = argparse.ArgumentParser('tracer')
  parser.add_argument('--bbmap-dir', dest='map_dir',
      type=str, default='basic-block-maps',
      help='map file for all basic blocks')
  parser.add_argument('-o', dest='ofile',
      type=str, default='configs.json',
      help='specify json file to store the compilation database')
  parser.add_argument('cmdline', nargs='+', help='cmdline to execute')
  args = parser.parse_args()

  shm = ShmObj(64, True)
  map_dir = pathlib.Path(args.map_dir).absolute()
  map_dir.mkdir(parents=True, exist_ok=True)

  bb_idx, fn_idx = count_lines(map_dir)
  shm.write_counter(bb_idx, 0)
  shm.write_counter(fn_idx, 1)

  # the cmdline process
  print(f"[#] shmid = {shm.id}, begin = {shm.read_counter()}")
  os.environ[f"__AFL_COUNTER_SHMID"] = str(shm.id)
  os.environ[f"__AFL_COUNTER_MAP_DIR"] = str(map_dir)
  p = subprocess.Popen(args.cmdline)
  p.communicate()

  print(f"[#] num of branches: {shm.read_counter()}, "
    + f"num of functions: {shm.read_counter(1)}")

  pathlib.Path(args.ofile).write_text(json.dumps({
    "nbrs": shm.read_counter(),
    "nfns": shm.read_counter(1),
    "bbmap.dir": str(map_dir),
    }, indent=2))

  shm.close()
