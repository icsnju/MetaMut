#!/usr/bin/env python3

import os
import pathlib
import argparse
from common import *

if __name__ == "__main__":
  parser = argparse.ArgumentParser('update')
  parser.add_argument('--branches', dest='branches',
      type=int, required=True, help="specify total branches")
  parser.add_argument('coverage_bins',
      nargs='+', help="specify total branches")
  parser.add_argument('--degrade', dest='degrade',
      action='store_true', help="degrade coverage binaries")
  args = parser.parse_args()

  for binfile in args.coverage_bins:
    c = Coverage.from_file(binfile, args.branches)
    c.to_file(binfile, header=not args.degrade)
