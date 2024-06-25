import os
import sys
import json
import datetime
import inspect
import logging
import pathlib

output_filename = None
disabled_module_logger = set([])
abort_on_failure = True
log_prefix_tag = None

def set_logfile(filename):
  global output_filename
  output_filename = filename

def disable_module(modulename):
  global disabled_module_logger
  disabled_module_logger.add(modulename)

def disable_this_module():
  global disabled_module_logger
  frame = inspect.currentframe().f_back
  modulename = inspect.getmodule(frame).__name__
  disabled_module_logger.add(modulename)

def handle_message(level, msg):
  global disabled_module_logger
  frame = inspect.currentframe().f_back.f_back
  filename = inspect.getframeinfo(frame).filename
  lineno = inspect.getframeinfo(frame).lineno
  modulename = inspect.getmodule(frame).__name__
  filename = pathlib.Path(filename).name
  if modulename in disabled_module_logger: return
  now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
  global log_prefix_tag
  prefix = f"[{now}] {level}:{filename}:{lineno}:"
  if log_prefix_tag:
    output = f"{prefix}:{log_prefix_tag}: {msg}"
  else:
    output = f"{prefix}: {msg}"
  print(output, file=sys.stderr)

  if output_filename:
    with open(output_filename, 'a+') as fp:
      fp.write(output + '\n')
      fp.flush()

def jlog(**kwargs):
  j = {
    'time': str(datetime.datetime.now()),
    **kwargs
  }
  output = json.dumps(j)
  if 'sync_with_stdout' in kwargs:
    del j['sync_with_stdout']
    print(output, file=sys.stderr)

  if output_filename:
    with open(output_filename, 'a+') as fp:
      fp.write(output + '\n')
      fp.flush()

def remove_last_line(filename, chunk_size=1024):
  with open(filename, 'r+') as file:
    file.seek(0, os.SEEK_END)
    end_position = file.tell() - 1 # skip ending newline

    while end_position > 0:
      chunk_position = max(0, end_position - chunk_size)
      file.seek(chunk_position)
      chunk = file.read(end_position - chunk_position)
      newline_position = chunk.rfind('\n')
      if newline_position != -1:
        file.seek(chunk_position + newline_position + 1)
        file.truncate()
        break
      end_position = chunk_position

def rollback_logs(nlogs=1):
  for i in range(nlogs):
    remove_last_line(output_filename)

def wlog(msg):
  handle_message('DEBUG', msg)

def werr(msg):
  global abort_on_failure
  handle_message('ERROR', msg)
  if abort_on_failure:
    raise RuntimeError(msg)

def wassert(cond, msg=None):
  global abort_on_failure
  if not cond:
    handle_message('Assertion Fail', msg)
    if abort_on_failure:
      raise AssertionError(f"Assertion Fail ({msg})")

__all__ = [
    'set_logfile',
    'disable_module',
    'disable_this_module',
    'wlog', 'werr', 'wassert', 'jlog'
]
