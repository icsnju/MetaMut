import os
import sys
import pwd
import time
import base64
import string
import random
import signal
import psutil
import select
import getpass
import pathlib
import subprocess

class Psutil:
  @staticmethod
  def get_cpu_usage(pid):
    try:
      with open(f"/proc/{pid}/stat", "r") as file:
        contents = file.readline()
        values = contents.split()
        # print(contents, values[13], values[14])
        utime = int(values[13])
        stime = int(values[14])
      
      clk_tck = os.sysconf(os.sysconf_names['SC_CLK_TCK'])
      
      user_time = utime / clk_tck
      system_time = stime / clk_tck
      
      return user_time, system_time
    except OSError as e:
      return 0., 0.

  @staticmethod
  def get_mem_usage(pid):
    try:
      with open(f"/proc/{pid}/statm", "r") as file:
        contents = file.readline()
        values = contents.split()
        # Multiply by page size (in bytes) to get RSS in bytes
        rss = int(values[1]) * os.sysconf('SC_PAGESIZE')
      
      return rss
    except OSError as e:
      return 0

  @staticmethod
  def get_children_pids(pid):
    children_pids = []
    tasks_dir = f'/proc/{pid}/task'

    # Check if the tasks directory exists for the given PID
    try:
      # List all entries in the tasks directory
      for task in os.listdir(tasks_dir):
        task_dir = os.path.join(tasks_dir, task, "children")
        with open(task_dir, 'r') as children_file:
          # Read the children PIDs from the file
          children = children_file.read().strip()
          # Split the string by whitespace to get individual PIDs
          if children:
            children_pids.extend(children.split(' '))
    except (FileNotFoundError, ProcessLookupError):
      pass
    return [int(pid) for pid in children_pids if pid.isdigit()]

  @staticmethod
  def get_descendant_pids(pid):
    descendant_pids = []
    children_pids = Psutil.get_children_pids(pid)
    for child_pid in children_pids:
      descendant_pids.append(child_pid)
      descendant_pids.extend(Psutil.get_descendant_pids(child_pid))
    return descendant_pids
