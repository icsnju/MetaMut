import os
import json
import random
import configs
import difflib
import pathlib
import filecmp
import subprocess
from enum import Enum
from utils.gdb import GDBController
from utils.ShellProcess import ShellProcess

class CheckRes:
  Normal = 1
  Hang = 2
  InvalidChange = 3
  NoChange = 4
  Crash = 5
  NotCompile = 6
  ApplyFail = 7
  def __init__(self, res):
    self.res_code = res
    for k, v in CheckRes.__dict__.items():
      if v == res:
        self.res_str = k
        break
  def format_rcode(self):
    if self.res_code == self.Normal: return "Normal"
    if self.res_code == self.Hang: return "Hang"
    if self.res_code == self.InvalidChange: return "InvalidChange"
    if self.res_code == self.NoChange: return "NoChange"
    if self.res_code == self.Crash: return "Crash"
    if self.res_code == self.NotCompile: return "NotCompile"
    if self.res_code == self.ApplyFail: return "ApplyFail"
    assert False
  def is_valid(self):
    return self.res_code == self.Normal
  def isa(self, code):
    return self.res_code == code
  @classmethod
  def from_apply_fail(cls, isrc):
    inst = cls(CheckRes.ApplyFail)
    inst.isrc = isrc
    return inst
  @classmethod
  def from_normal(cls):
    return cls(CheckRes.Normal)
  @classmethod
  def from_crash(cls, stderr, backtrace):
    inst = cls(CheckRes.Crash)
    inst.stderr = stderr
    inst.backtrace = backtrace
    return inst
  @classmethod
  def from_invalid(cls, isrc, osrc, diff, errors):
    inst = cls(CheckRes.InvalidChange)
    inst.isrc = isrc
    inst.osrc = osrc
    inst.diff = diff
    inst.errors = errors
    return inst
  @classmethod
  def from_hang(cls, isrc, backtrace):
    inst = cls(CheckRes.Hang)
    inst.isrc = isrc
    inst.backtrace = backtrace
    return inst
  @classmethod
  def from_nochange(cls, isrc):
    inst = cls(CheckRes.NoChange)
    inst.isrc = isrc
    return inst
  @classmethod
  def from_not_compile(cls, errors):
    errors = errors[:2000]
    inst = cls(CheckRes.NotCompile)
    inst.errors = errors
    return inst

class ExecUtils:
  max_exec_times = 3
  @classmethod
  def compile(cls, m):
    mut_cpp_file = f"lib/workbench/{m.name}.cpp"
    pathlib.Path(mut_cpp_file).write_text(m.code)
    p = ShellProcess(
        f"cd output; cmake ..; make -j{configs.jobs}",
        stdout=subprocess.DEVNULL,
        stderr=subprocess.PIPE, tlimit=300)
    retcode, isTimeout, stdout, stderr = p.communicate()
    if retcode == 0: return CheckRes.from_normal()
    try:
      os.remove(mut_cpp_file)
    except FileNotFoundError:
      pass
    return CheckRes.from_not_compile(stderr)
  @classmethod
  def checkValidity(cls, ifile, ofile):
    p = ShellProcess(f'clang -w {ifile} -c -o /dev/null')
    retcode, _, _, _ = p.communicate()
    if retcode == 0:
      p = ShellProcess(f'clang -w {ofile} -c -o /dev/null',
          stderr=subprocess.PIPE)
      retcode, _, _, stderr = p.communicate()
      if retcode == 1:
        isrc = pathlib.Path(ifile).read_text()
        osrc = pathlib.Path(ofile).read_text()
        diff = difflib.unified_diff(isrc.splitlines(True),
            osrc.splitlines(True),
            fromfile=ifile, tofile=ofile)
        return CheckRes.from_invalid(isrc, osrc, ''.join(diff),
            stderr)
      return CheckRes.from_normal()
    return None # no check res
  @staticmethod
  def format_backtrace(frames):
    ret = ''
    for frame in frames:
      level = frame.get('level', '?')
      addr =  frame.get('addr', '0x?')
      func =  frame.get('func', '?')
      line =  frame.get('line', '?')
      fname = frame.get('file', '?')
      fpath = frame.get('fullname', '?')
      ret += f'#{level} {addr} in {func} at {fname}:{line}\n'
      if line != '?' and fpath != '?':
        p = pathlib.Path(fpath)
        if p.is_file():
          line = int(line) - 1
          lines = p.read_text().splitlines()
          if 0 <= line and line < len(lines):
            ret += f'==> {lines[int(line)]}\n'
    return ret
  @classmethod
  def execute(cls, m, example):
    ifile = f"{configs.objdir}/input.c"
    ofile = f"{configs.objdir}/output.c"
    pathlib.Path(ifile).write_text(example)
    mut_bin_file = f"{configs.mutator_bin}"
    for i in range(cls.max_exec_times):
      seed = random.randint(0, 2**31-1)
      mut_cmd = f"{mut_bin_file} --mutator={m.name}" \
        f" -i {ifile} -o {ofile} --seed {seed}"
      p = ShellProcess(mut_cmd,
          stdout=subprocess.PIPE,
          stderr=subprocess.PIPE,
          tlimit=10)
      retcode, isTimeout, stdout, stderr = p.communicate()
      if isTimeout:
        pass
      elif retcode == 0:
        if filecmp.cmp(ifile, ofile): # no change
          continue
        else:
          valid = cls.checkValidity(ifile, ofile)
          if valid is not None: return valid
          continue
      elif retcode == 1:
        if i + 1 == cls.max_exec_times:
          return CheckRes.from_apply_fail(example)
        else:
          continue
      # crash or timeout
      g = GDBController(mut_cmd)
      timeout = not g.run(timeout=5)
      frames = g.traceback_frames()[:10]
      backtrace = cls.format_backtrace(frames)
      if timeout:
        return CheckRes.from_hang(example, backtrace)
      return CheckRes.from_crash(stdout + stderr, backtrace)
    return CheckRes.from_nochange(example)
  @classmethod
  def list_mutators(cls):
    existing_mutators = []
    p = ShellProcess(
        f'{configs.mutator_bin} --list-mutators',
        stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    _, _, stdout, _ = p.communicate()
    try:
      for name, desc in json.loads(stdout):
        existing_mutators.append([name, desc])
    except json.decoder.JSONDecodeError:
      pass
    return existing_mutators
