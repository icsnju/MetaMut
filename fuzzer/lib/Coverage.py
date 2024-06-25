import os
import sys
import json
import time
import ctypes
import random
import struct
import pathlib
import argparse
import datetime
import sysv_ipc
import subprocess
import numpy as np
from multiprocessing import shared_memory

class ShmObj(sysv_ipc.SharedMemory):
  def __init__(self, size=1024, private=True):
    super().__init__(
      sysv_ipc.IPC_PRIVATE if private else sysv_ipc.IPC_CREAT,
      size=size,
      flags=sysv_ipc.IPC_CREX,
      init_character=b'\0')
  def read_counter(self):
    int_bytes = self.read(4)
    int_value = struct.unpack('i', int_bytes)[0]
    return int_value
  def close(self):
    self.detach()
    self.remove()
  def write_zeros(self):
    zero_bytes = bytes(self.size)
    self.write(zero_bytes)
  def to_numpy(self):
    return np.frombuffer(self.read(),
        dtype=np.uint8, count=self.size)
  def __del__(self):
    self.close()

class Coverage:
  def __init__(self, bits, nbrs):
    assert type(bits) == np.ndarray
    assert len(bits) == (nbrs + 7) // 8
    self.bits = bits # coverage map bits
    self.nbrs = nbrs # number of total branches
  def __eq__(self, that):
    return np.all(self.bits == that.bits) and self.nbrs == that.nbrs
  def __ne__(self, that):
    return not (self == that)
  def merge(self, that):
    assert self.nbrs == that.nbrs
    self.bits = self.bits | that.bits
  @classmethod
  def from_shm(cls, shm, nbrs):
    return cls(shm.to_numpy(), nbrs)
  @classmethod
  def from_file(cls, filename, nbrs=None):
    buffer = None
    with open(filename, 'rb') as fp:
      buffer = fp.read()
    assert buffer
    return cls.from_buffer(buffer, nbrs)
  @classmethod
  def from_buffer(cls, buffer, nbrs=None):
    if buffer[:4] == b"CVRt":
      header_struct = struct.Struct("4s I 14I")
      hdr_sz = header_struct.size
      _, size, *_ = header_struct.unpack(buffer[:hdr_sz])
      # size -> buffer[hdr_sz:hdr_sz+size/8+1]
      # nbrs -> buffer[hdr_sz:hdr_sz+nbrs/8+1]
      #      -> buffer[hdr_sz:hdr_sz+len(buffer)-hdr_sz]
      if nbrs:
        nbrs = min(size, nbrs, (len(buffer) - hdr_sz)*8)
      else:
        nbrs = min(size, (len(buffer) - hdr_sz)*8)
      bits = np.frombuffer(buffer[hdr_sz:],
          dtype=np.uint8, count=(nbrs+7)//8)
      return cls(bits, nbrs)
    else:
      if nbrs:
        nbrs = min(nbrs, len(buffer)*8)
      else:
        nbrs = len(buffer)*8
      bits = np.frombuffer(buffer,
          dtype=np.uint8, count=(nbrs+7)//8)
      return cls(bits, nbrs)
  def to_buffer(self, header=True):
    '''
    struct header {
      char ident[4] = "CVRt"; // coverage trace
      unsigned size = 0x12345678;
      unsigned extension[14];
    }
    '''
    if not header: return self.bits.tobytes()
    header_struct = struct.Struct("4s I 14I")
    ident = b"CVRt"
    size = self.nbrs
    extension = [0] * 14
    header_data = header_struct.pack(ident, size, *extension)
    return header_data + self.bits.tobytes()
  def to_file(self, filename, header=True):
    with open(filename, "wb") as fp:
      fp.write(self.to_buffer(header))
  def to_shm(self, shm):
    shm.write(self.to_buffer(False))
  def is_superior(self, that):
    # all bits set in that.bits is also set in self.bits
    return np.all((self.bits | that.bits) == self.bits)
  def outperforms(self, that):
    t = self.bits & ~that.bits
    return np.any(t & 0xFF)
  def underperforms(self, that):
    return that.outperforms(self)
  def get_improved_coverage_count(self, that):
    t = self.bits & ~that.bits
    bits_array = np.unpackbits(self.bits, bitorder='little')
    return np.nonzero(bits_array)
  def get_covered_branches_count(self):
    bits_array = np.unpackbits(self.bits)
    return bits_array.sum()
  def get_total_branches_count(self):
    return self.nbrs
  def get_coverage_rate(self):
    return float(self.get_covered_branches_count()) \
        / self.get_total_branches_count()
  def get_covered_branches(self):
    bits_array = np.unpackbits(self.bits, bitorder='little')
    return np.nonzero(bits_array)[0]

class CoverageSampler:
  env_var = '__AFL_BB_SHM_ID'
  def __init__(self, total_branches):
    self.total_branches = total_branches
    self.shm = ShmObj(int(total_branches / 8 + 1))
    self.last_shmid = None
  def __enter__(self):
    if self.env_var in os.environ:
      self.last_shmid = os.environ[self.env_var]
    os.environ[self.env_var] = str(self.shm.id)
  def __exit__(self, exc_type, exc_val, exc_tb):
    del os.environ[self.env_var]
    if self.last_shmid:
      os.environ[self.env_var] = self.last_shmid
  def clear(self):
    self.shm.write_zeros()
  def sample(self):
    return Coverage.from_shm(self.shm, self.total_branches)
  def write(self, filename):
    coverage = self.sample()
    coverage.to_file(filename)

class BasicBlockMap:
  @staticmethod
  def load(bbmap_dir):
    m = {}
    for filename in pathlib.Path(bbmap_dir).glob('*'):
      with open(filename) as fp:
        for line in fp:
          idx, locs = line.split(':', 1)
          m[int(idx)] = json.loads(locs)
    return m
  @staticmethod
  def count_totals(bbmap_dir):
    count = 0
    files = pathlib.Path(bbmap_dir).glob('*')
    for filename in files:
      with open(filename, 'r') as fp:
        for line in fp:
          count += 1
    return count
