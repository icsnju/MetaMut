import os
import time
import random
import shutil
import configs
import pathlib
import threading
import subprocess

class Compiler:
  def __init__(self, name, cc_path,
      instrum_path, instrum_nbrs):
    self.name = name
    self.cc_path = cc_path
    self.instrum_path = instrum_path
    self.instrum_nbrs = instrum_nbrs
  def __str__(self):
    return self.name

def make_compilers():
  return [
      Compiler('clang',
        configs.clang_bin,
        configs.instrum_clang_bin,
        configs.instrum_clang_nbrs),
      Compiler('gcc',
        configs.gcc_bin,
        configs.instrum_gcc_bin,
        configs.instrum_gcc_nbrs),
  ]
