import os
import re
import sys
import time
import shutil
import signal
import random
import select
import pathlib
import configs
import logging
import resource
import subprocess
from enum import Enum
from six.moves import reduce
from .wait import Waiter
from .ControlledProcess import ControlledProcess
from typing import Set, List, Dict, Tuple, Union, Any, Optional

class Testcase:
  def __init__(self, *ifiles, wdir=None):
    self.ifiles = ifiles
    self.wdir = wdir
  def __str__(self):
    return ' '.join(self.ifiles)
  def needs_link(self):
    return len(self.ifiles) > 1

class CompilerStatus(Enum):
  Timeout     = 1
  Crash       = 2
  OutOfMem    = 3
  DiagError   = 4
  DiagWarn    = 5
  DiagMain    = 6
  DiagNoSuchFile = 7
  DiagUnknown = 8
  Normal      = 9
  # Regular expression patterns as class-level attributes
  ERROR_PATTERN = re.compile(r': error: ')
  WARNING_PATTERN = re.compile(r': warning: ')
  NOFILE_PATTERN = re.compile(r'(error: no such file or directory|fatal error: [^:]+: No such file or directory)')
  LINK_PATTERN = re.compile(r"undefined reference to `main'")
  OOM_PATTERN = re.compile(r"out of mem", re.I)
  CLANG_CRASH_PATTERN = re.compile(r'PLEASE submit a bug report to')
  CLANG2_CRASH_PATTERN = re.compile(r'PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT')
  GCC_CRASH_PATTERN = re.compile(r'Please submit a full bug report')
  def is_bug(self):
    return self.value in [
        CompilerStatus.Timeout.value,
        CompilerStatus.Crash.value,
        CompilerStatus.OutOfMem.value]
  @staticmethod
  def analyze(retcode, output, is_tle, is_oom):
    if CompilerStatus.CLANG_CRASH_PATTERN.value.search(output) or \
      CompilerStatus.CLANG2_CRASH_PATTERN.value.search(output) or \
      CompilerStatus.GCC_CRASH_PATTERN.value.search(output):
      return CompilerStatus.Crash
    if is_tle:
      return CompilerStatus.Timeout
    elif is_oom:
      return CompilerStatus.OutOfMem
    elif retcode != 0 and retcode != 1:
      return CompilerStatus.Crash
    elif retcode != 0:
      if CompilerStatus.ERROR_PATTERN.value.search(output):
        return CompilerStatus.DiagError
      elif CompilerStatus.LINK_PATTERN.value.search(output):
        return CompilerStatus.DiagMain
      elif CompilerStatus.NOFILE_PATTERN.value.search(output):
        return CompilerStatus.DiagNoSuchFile
      return CompilerStatus.DiagUnknown
    elif CompilerStatus.WARNING_PATTERN.value.search(output):
      return CompilerStatus.DiagWarn
    return CompilerStatus.Normal
