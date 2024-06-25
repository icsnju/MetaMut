import re
import os
import time
import errno
import shlex
import random
import string
import psutil
import configs
import pathlib
import argparse
import datetime
import traceback
import itertools
import subprocess
from multiprocessing import Process
from lib import Utils
from lib.logger import *
from lib.CrashPattern import *
from lib.CompilerInstance import Testcase, CompilerStatus
from lib.Coverage import CoverageSampler
from lib.ControlledProcess import ControlledProcess, ControlledGDB

class CompilerTracer:
  def __init__(self, cmd, timeout=30, mlimit=4*2**30):
    self.cmd = cmd
    self.gdb = ControlledGDB(cmd, mlimit=mlimit)
    self.timeout = timeout
  def try_sample_pattern_trackbacks(self): # failed try
    timeout_count = 0
    tracebacks, last_traceback = [], None
    while timeout_count < 10:
      res = self.gdb.run(cmd='finish', timeout=1)
      timeout_count += not res
      # print(f'res is {res}, last_traceback is')
      # if last_traceback:
      #   for f, a in last_traceback:
      #     print(f"  {f} - {a}")
      if not res and last_traceback is not None:
        if last_traceback not in tracebacks:
          tracebacks.append(last_traceback)
      last_traceback = self.gdb.traceback_functions_and_addresses()
    return [list(t) for t in tracebacks]
  def traceback(self):
    self.gdb.communicate('set print inferior-events off')
    # self.gdb.communicate('set auto-solib-add off')
    self.gdb.communicate('set breakpoint pending on')
    self.gdb.communicate('set follow-fork-mode child')
    self.gdb.communicate('set disable-randomization on')
    self.gdb.communicate('b exit')
    self.gdb.communicate('b abort')
    self.gdb.communicate('b llvm::sys::PrintStackTrace')
    # True means normally exits, False means timeout
    res = self.gdb.run(timeout=self.timeout)
    # return (res, self.gdb.traceback_functions())
    # return (res, self.gdb.traceback_functions_and_addresses())
    return (res, self.gdb.traceback_functions_and_lines())

class FuzzArgs:
  def __init__(self, *, wdir, cc, seed, timeout, duration, testcases, coverage_only, mutator):
    self.wdir = wdir
    self.cc = cc
    self.seed = seed
    self.timeout = timeout
    self.duration = duration
    self.testcases = testcases
    self.coverage_only = coverage_only
    self.mutator = mutator

class Fuzzer:
  def __init__(self, fuzz_args):
    self.fuzz_args = fuzz_args
    ### some configs ###
    self.tmpdir = f"{fuzz_args.wdir}/tmpdir"
    self.coverage_logfile = f"{fuzz_args.wdir}/coverage-trend.txt"
    self.logfile = f"{fuzz_args.wdir}/log.txt"
    nbrs = fuzz_args.cc.instrum_nbrs
    self.sampler = CoverageSampler(nbrs)
    self.crashes = {}
    self.nullfile = '/dev/null'
    self.start_time = time.time()
    self.optlevel = "-O2"
    self.last_save_time = time.time() - 24*3600
    set_logfile(self.logfile)
  @staticmethod
  def clean_file(filename):
    os.system(f"rm -rf {filename}")
  @staticmethod
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

  def mark_current_state(self, state):
    with open(f"{self.fuzz_args.wdir}/state.txt", 'w+') as fp:
      fp.write(state + "\n")

  def query_current_state(self):
    p = pathlib.Path(f"{self.fuzz_args.wdir}/state.txt")
    if p.exists():
      return p.read_text().strip()
    return None

  def next_ifile(self):
    return random.choice(self.fuzz_args.testcases)

  def next_opfx(self):
    chars = string.ascii_letters
    s = ''.join(random.choice(chars) for _ in range(12))
    # return f"{self.fuzz_args.wdir}/{s}.c"
    return s

  def prologue(self):
    os.system(f'rm -rf {self.fuzz_args.wdir}')
    os.system(f"mkdir -p {self.fuzz_args.wdir}")
    os.system(f"mkdir -p {self.tmpdir}")
    os.environ['TMPDIR'] = self.tmpdir
    random.seed(self.fuzz_args.seed)
    p = psutil.Process(os.getpid())
    wlog(f'start fuzzing, seed={self.fuzz_args.seed}, parent={os.getpid()}, affinity={p.cpu_affinity()}')

  def save_coverage(self, coverage):
    m = coverage.get_covered_branches_count()
    n = coverage.get_total_branches_count()
    with open(self.coverage_logfile, 'a+') as fp:
      now = time.time() - self.start_time
      fp.write(f'{now},{m},{n},{m / n}\n')

  def get_cc_cmdline(self, cc, testcase, options=[], tolist=False):
    cmd = f"{cc} {testcase} -o {self.nullfile} -w "
    cmd += ' '.join(options)
    if tolist:
      return shlex.split(cmd)
    else:
      return cmd

  def compile(self, cc, testcase, options=[], capture_stderr=False):
    cmd = self.get_cc_cmdline(cc, testcase, options=options, tolist=True)
    p = ControlledProcess(cmd, shell=False,
          stdout=subprocess.DEVNULL,
          stderr=subprocess.PIPE if capture_stderr \
              else subprocess.DEVNULL,
          tlimit=self.fuzz_args.timeout)
    return p.communicate()

  def compile_and_check(self, cc, testcase):
    pRes = self.compile(cc, testcase, capture_stderr=True)
    return CompilerStatus.analyze(pRes.retcode,
        pRes.stderr, pRes.is_tle, pRes.is_oom)

  def run_for_crash(self, testcase):
    for cc in [configs.gcc_bin, configs.clang_bin]:
      chkRes = self.compile_and_check(cc, testcase)
      if chkRes == CompilerStatus.Crash:
        cmd = self.get_cc_cmdline(cc, testcase, tolist=True)
        jlog(crash=cmd, invoke=self.fuzz_args.mutator.get_current_mutator_invoke_cmdline())
        return cc
    return None

  def check_crash(self, testcase):
    cc = self.run_for_crash(testcase)
    if cc is None: return False
    # crash is found, now extract the crash stacks
    for opt_str in configs.options:
      options = shlex.split(opt_str)
      for cfile in testcase.ifiles:
        t = Testcase(cfile, wdir=testcase.wdir)
        cmd = self.get_cc_cmdline(cc, t, options=options, tolist=False)
        tracer = CompilerTracer(cmd, self.fuzz_args.timeout)
        res, traceback = tracer.traceback()
        if not res: continue
        crash_pattern = CrashPattern(cc, options, traceback)
        self.crashes.setdefault(crash_pattern,
            CompilerCrash(crash_pattern))
        self.crashes[crash_pattern].add_bugfile(str(t),
            time.time() - self.start_time)
        with open(f"{self.fuzz_args.wdir}/crashes.json", 'w+') as fp:
          j = [v.to_json() for v in self.crashes.values()]
          fp.write(json.dumps(j, indent=2))
    return True

  def fuzz(self):
    if self.query_current_state() == 'done':
      wlog('done detected, quit now')
      return

    self.prologue()
    st_time = datetime.datetime.now()
    ed_time = st_time + self.fuzz_args.duration
    while datetime.datetime.now() < ed_time:
      self.mark_current_state('running')
      # perform mutation
      ifile = self.next_ifile()
      opfx = self.next_opfx()
      ot = self.fuzz_args.mutator.mutate(ifile, opfx)
      if not ot: continue
      if self.fuzz_args.coverage_only:
        has_crash = False
      else:
        has_crash = self.check_crash(ot)
      # check coverage
      prev_coverage = self.sampler.sample()
      with self.sampler:
        for opt_str in configs.options:
          self.compile(self.fuzz_args.cc.instrum_path,
              ot, options=shlex.split(opt_str))
      curr_coverage = self.sampler.sample()
      if curr_coverage.outperforms(prev_coverage):
        # wlog(f"new branch: {ot}")
        self.save_coverage(curr_coverage)
        if self.fuzz_args.mutator.is_generator():
          if not has_crash:
            self.fuzz_args.mutator.clean(ot)
        else:
          assert len(ot.ifiles) == 1
          self.fuzz_args.testcases.append(ot.ifiles[0])
        self.last_save_time = time.time()
      else:
        if not has_crash:
          # wlog(f"bored testcase: {ot}")
          self.fuzz_args.mutator.clean(ot)
        if time.time() >= self.last_save_time + 600:
          self.last_save_time = time.time()
          self.save_coverage(curr_coverage)
      # clean tmpdir
      Utils.remove_files(self.tmpdir)
    self.mark_current_state('done')
