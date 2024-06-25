import os
import sys
import json
import time
import ctypes
import random
import struct
import configs
import pathlib
import argparse
import datetime
import sysv_ipc
import subprocess
import numpy as np

class CrashPattern:
  def __init__(self, cc, options, traceback):
    self.cc = cc
    assert type(options) in [list, tuple]
    assert type(traceback) in [list, tuple]
    self.options = tuple(options)
    self.traceback = tuple(traceback)
  def to_tuple(self):
    return (self.cc, self.options, self.traceback)
  def to_json(self):
    return (self.cc, self.options, self.traceback[:configs.max_frames])
  def __hash__(self):
    return hash(self.to_tuple())
  def __eq__(self, that):
    if not isinstance(that, CrashPattern): return False
    return self.to_tuple() == that.to_tuple()

class CompilerCrash:
  def __init__(self, crash_pattern):
    self.crash_pattern = crash_pattern
    self.bugfiles = []
  def add_bugfile(self, bugfile, cur_time):
    self.bugfiles.append([bugfile, cur_time])
  def to_json(self):
    return {
        "compiler": self.crash_pattern.cc,
        "options": ' '.join(self.crash_pattern.options),
        "traceback": self.crash_pattern.traceback[:configs.max_frames],
        "srcfiles": self.bugfiles,
        }
