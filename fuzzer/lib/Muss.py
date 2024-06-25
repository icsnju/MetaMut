import os
import time
import random
import shutil
import psutil
import pathlib
import configs
import datetime
import threading
import subprocess
from .ControlledProcess import ControlledProcess
from .CompilerInstance import Testcase
from .Utils import kill_proc_tree
from .logger import wlog

def wait(file_or_dir, mode=os.F_OK, timeout=1):
  st = time.time()
  ed = st + (timeout_seconds := timeout)
  delay = 0.01
  while time.time() < ed:
    if os.access(file_or_dir, mode): return True
    time.sleep(delay)
    remaining = ed - time.time()
    delay = min(delay * 2, remaining, .05)
  return False

class Mutator:
  def __init__(self, name, mutator_bin):
    self.name = name
    self.mutator_bin = mutator_bin
  def setup_workdir(self, wdir):
    self.wdir = wdir
  def mutate(self, ifile, opfx):
    raise NotImplemented("api mutate is not implemented")
  def is_generator(self):
    return False
  def clean(self, testcase):
    for f in testcase.ifiles:
      try:
        os.unlink(f)
      except OSError:
        pass
  def get_current_mutator_invoke_cmdline(self):
    return None

class Muss(Mutator):
  def mutate(self, ifile, opfx):
    ofile = f"{self.wdir}/{opfx}.c"
    seed = random.randint(0, 2**31 - 1)
    cmd  = f'{self.mutator_bin} --seed {seed}'
    cmd += f' --randomly-try-all-mutators'
    cmd += f' -i {ifile} -o {ofile}'
    self.invoke_cmd = cmd
    p = ControlledProcess(cmd,
       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    pRes = p.communicate()
    if pRes.retcode != 0: return None
    return Testcase(ofile)
  def get_current_mutator_invoke_cmdline(self):
    return getattr(self, 'invoke_cmd', None)
