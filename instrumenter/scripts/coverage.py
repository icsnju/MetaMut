#!/usr/bin/env python3

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
from common import *

if __name__ == "__main__":
  parser = argparse.ArgumentParser('coverage')
  parser.add_argument('--bbmap-dir', dest='bbmap_dir',
      type=str, help='map file for all basic blocks')
  parser.add_argument('coverage_bins',
      nargs='+', help="specify total branches")
  args = parser.parse_args()

  c = None
  for binfile in args.coverage_bins:
    new_c = Coverage.from_file(binfile)
    if not c: c = new_c
    else: c.merge(new_c)

  n_cover = c.get_covered_branches_count()
  n_total = c.get_total_branches_count()
  r_rate = c.get_coverage_rate()
  print(f"coverage: {n_cover} / {n_total} = {r_rate}")
