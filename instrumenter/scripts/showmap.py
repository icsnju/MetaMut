#!/usr/bin/env python3

import os
import sys
import json
import time
import ctypes
import random
import struct
import signal
import pathlib
import argparse
import datetime
import sysv_ipc
import subprocess
import numpy as np
from common import *

if __name__ == "__main__":
  parser = argparse.ArgumentParser('showmap')
  parser.add_argument('--bbmap-dir', dest='map_dir',
      type=str, required=True,
      help='map file for all basic blocks')
  parser.add_argument('--timeout', dest='timeout',
      type=int, default=30,
      help='specify timeout seconds')
  parser.add_argument('--show-bb', dest='show_bb',
      action='store_true',
      help='show basic block coverage')
  parser.add_argument('--show-fn', dest='show_fn',
      action='store_true',
      help='show function performance counter')
  parser.add_argument('cmdline', nargs='+', help='cmdline to execute')
  args = parser.parse_args()

  if args.show_bb:
    bbMap = BasicBlockMap.load(args.map_dir)
    cSampler = CoverageSampler(len(bbMap))
    cSampler.__enter__()
  if args.show_fn:
    fnMap = FunctionMap.load(args.map_dir)
    pSampler = PerformanceSampler(len(fnMap))
    pSampler.__enter__()

  p = subprocess.Popen(args.cmdline, preexec_fn=os.setpgrp)
  try:
    p.communicate(timeout=args.timeout)
  except subprocess.TimeoutExpired:
    pgrp = os.getpgid(p.pid)
    os.killpg(pgrp, signal.SIGKILL)
  p.wait()

  if args.show_bb:
    cSampler.__exit__()
    coverage = cSampler.sample()
    for idx in coverage.get_covered_branches():
      for filename, funcname, lineno in bbMap[idx]:
        print('bb:', idx, filename, funcname, lineno)

  if args.show_fn:
    pSampler.__exit__()
    perfdata = pSampler.sample() 
    total = perfdata.buffer.sum().item()
    output = []
    for i in range(len(perfdata.buffer)):
      counter = perfdata.buffer[i].item()
      output.append((counter, i, fnMap[i].strip()))

    output.sort(key=lambda x: x[0], reverse=True)
    for counter, i, fn in output[:10]:
      print(f'fn:\t{i}\t{counter}\t{counter/total}\t{fn}')
