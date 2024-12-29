import re
import os
import sys
import time
import errno
import random
import psutil
import configs
import pathlib
import resource
import argparse
import datetime
import traceback
import itertools
import subprocess
from multiprocessing import Process
from lib.Coverage import CoverageSampler
from lib.Muss import *
from lib.Utils import *
from lib.Fuzzer import FuzzArgs, Fuzzer
from lib.Compilers import make_compilers
from lib.Schedule import Scheduler
from lib.ExceptionRecorder import ExceptionRecorder
from concurrent.futures import ProcessPoolExecutor
from concurrent.futures import as_completed

def worker(fuzz_args):
  exlogf = f"{fuzz_args.wdir}/exceptions.txt"
  with ExceptionRecorder(exlogf):
    clean_processes(['cc1', 'as'])
    fuzzer = Fuzzer(fuzz_args)
    fuzzer.fuzz()

def load_testcases(args):
  testcases = []
  for cfile in pathlib.Path(args.seeds_dir).glob('**/*.c'):
    testcases.append(str(cfile))
  random.shuffle(testcases)
  return testcases

def make_worker_list(args):
  worker_args_list = []
  for i in range(args.repeat_times):
    for cc in make_compilers():
      muss = Muss('muss', configs.muss_bin)
      wdir = f"{args.wdir}/{muss.name},{cc},{i}"
      muss.setup_workdir(wdir)
      worker_args_list.append(FuzzArgs(
        wdir=wdir,
        cc=cc,
        seed=random.randint(0, 2**31-1),
        timeout=args.timeout,
        duration=args.duration,
        testcases=args.testcases,
        coverage_only=False,
        mutator=muss))
  return worker_args_list

def check_context(args):
  if not os.access(configs.muss_bin, os.X_OK|os.F_OK):
    print(f"The muss binary is not compiled, please refer to README to compile it")
    sys.exit(0)

  if len(list(pathlib.Path(args.seeds_dir).glob('**/*.c'))) == 0:
    print(f"There are no seeds found in specified {args.seeds_dir}")
    sys.exit(0)

  for b in [configs.gcc_bin, configs.clang_bin,
      configs.instrum_gcc_bin, configs.instrum_clang_bin]:
    if os.access(b, os.X_OK|os.F_OK): continue
    print(f"The compiler binary {b} is not found, please refer to README to compile or obtain it")
    sys.exit(0)

def parse_args():
  parser = argparse.ArgumentParser('driver')
  parser.add_argument('-j', dest='jobs',
    default=(psutil.cpu_count() // 2), type=int,
    help='specify number of jobs')
  parser.add_argument('--repeat-times', dest='repeat_times',
    default=60, type=int, help='specify repeat times')
  parser.add_argument('--max-frames', dest='max_frames',
    default=50, type=int, help='specify max frames')
  parser.add_argument('--wdir', dest='wdir',
    default=str(os.getcwd()),
    help='specify number of jobs')
  parser.add_argument('--cc-opt', dest='options',
    nargs='+', default=["-O2"],
    help='specify compiler options to fuzz (default: ["-O2"])')
  parser.add_argument('--seed', dest='seed',
    type=int, default=12345678, help='specify seed')
  parser.add_argument('--duration', dest='duration',
    type=int, default=24*3600,
    help='specify duration of each fuzzing instance, default=86400 (s)')
  parser.add_argument('--timeout', dest='timeout',
    default=10, type=int,
    help='specify timeout seconds')
  parser.add_argument('--seeds-dir', dest='seeds_dir',
    required=True, type=str,
    help='specify where to store the seeds')
  return parser.parse_args()

if __name__ == "__main__":
  args = parse_args()
  if psutil.cpu_count() < 2:
    print(f'Error: at least 1 physical core (2 logical cores) required')
    sys.exit(0)

  args.duration = datetime.timedelta(seconds=args.duration)
  random.seed(args.seed)
  configs.options = args.options
  configs.max_frames = args.max_frames
  args.testcases = load_testcases(args)
  worker_args_list = make_worker_list(args)

  check_context(args)

  # strategies to enhance stability
  clean_shared_memory()
  # compiler's child process cc1/as often escape from kill even timeout
  evt = start_thread_to_clean_processes(['cc1', 'as'])

  try:
    scher = Scheduler(worker, worker_args_list, args.jobs)
    scher.execute_jobs()
  except KeyboardInterrupt:
    kill_proc_tree(os.getpid(), False)
    evt.set()
    raise

  evt.set()
