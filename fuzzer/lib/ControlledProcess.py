import os
import re
import ast
import pwd
import sys
import time
import shutil
import base64
import psutil
import random
import select
import signal
import string
import getpass
import pathlib
import subprocess
from .wait import Waiter
from .psutils import Psutil
from .logger import jlog, wlog

class ExecutionResult:
  def __init__(self, is_tle, is_oom, retcode, stdout, stderr):
    self.is_tle = is_tle
    self.is_oom = is_oom
    self.retcode = retcode
    self.stdout = stdout
    self.stderr = stderr
  def to_tuple(self):
    return (self.is_tle, self.is_oom,
      self.retcode, self.stdout, self.stderr)
  def __iter__(self):
    for e in self.to_tuple(): yield e
  def __str__(self):
    return str(self.__dict__)
  def __hash__(self):
    return hash(self.to_tuple())
  def __eq__(self, that):
    if not isinstance(that, ExecutionResult):
      return False
    return self.to_tuple() == that.to_tuple()
  def to_json(self):
    return {**self.__dict__}

class PsutilMonitor:
  def __init__(self, ctrlpfx):
    self.ctrlpfx = ctrlpfx
    self.pids = set([])
    self.cpu_usage_map = {}
  def add_process(self, pid):
    self.pids.add(pid)
  def set_memory_max(self, mem, *, allow_unimplemented):
    assert allow_unimplemented
  def get_cpu_usage(self):
    lookuped_pids = set([])
    for pid in self.pids:
      for ch in [pid] + Psutil.get_descendant_pids(pid):
        if ch in lookuped_pids: continue
        lookuped_pids.add(ch)
        user_time, system_time = Psutil.get_cpu_usage(ch)
        total_time = user_time + system_time
        if total_time > self.cpu_usage_map.get(ch, 0):
          self.cpu_usage_map[ch] = total_time
    return sum(self.cpu_usage_map.values())
  def get_mem_usage(self):
    mem_usages = []
    lookuped_pids = set([])
    for pid in self.pids:
      for ch in [pid] + Psutil.get_descendant_pids(pid):
        if ch in lookuped_pids: continue
        lookuped_pids.add(ch)
        mem_usages.append(Psutil.get_mem_usage(ch))
    return sum(mem_usages)
  def kill_procs(self):
    for pid in self.pids:
      for ch in [pid] + Psutil.get_descendant_pids(pid):
        try:
          os.kill(ch, signal.SIGKILL)
        except ProcessLookupError:
          pass

ProcessMonitor = PsutilMonitor

def set_process_monitor(monitor):
  global ProcessMonitor
  ProcessMonitor = monitor

class ControlledProcess:
  def __init__(self, cmd, tlimit=30, mlimit=2**30,
      shell=True, stdin=None, stdout=None, stderr=None, text=True):
    self.cmd = cmd
    self.tlimit = tlimit
    self.mlimit = mlimit

    self.pmon = ProcessMonitor('pmon')
    self.pmon.set_memory_max(int(1.2 * self.mlimit),
        allow_unimplemented=True)

    self.check_existence(cmd)

    self.p = subprocess.Popen(cmd, shell=shell,
        stdin=stdin, stdout=stdout, stderr=stderr, text=text)
    self.stdin = self.p.stdin
    self.stdout = self.p.stdout
    self.stderr = self.p.stderr

    self.pmon.add_process(self.p.pid)

  @staticmethod
  def check_existence(cmd):
    if type(cmd) == list: cmd = ' '.join(cmd)
    exec_file = cmd.split(' ', 1)[0]
    if exec_file == 'exec' or exec_file == '.':
      exec_file = cmd.split(' ', 2)[1]
    msgpfx = "run_shell failed"
    if os.access(exec_file, os.F_OK | os.X_OK) or \
      shutil.which(exec_file) or \
      exec_file in ['cd', '[', '[[']: # builtin commands
      pass
    else:
      raise RuntimeError(f"{msgpfx}: {exec_file} cannot be execute")

  def communicate(self):
    is_tle, is_oom = False, False
    rlist = []
    outs, errs = [], []
    if self.stdout is not None:
      rlist.append(self.stdout.fileno())
    if self.stderr is not None:
      rlist.append(self.stderr.fileno())
    waiter = Waiter(self.tlimit, self.pmon.get_cpu_usage)
    # not supported now
    if self.stdin: self.stdin.close()
    while not is_oom and not is_tle:
      # output
      ready_fds, _, _ = select.select(rlist, [], [], waiter.delay)
      has_output = len(ready_fds) > 0
      waiter.update_delay(has_output)
      if self.stdout and self.stdout.fileno() in ready_fds:
        outs.append(os.read(self.stdout.fileno(), 32768))
      if self.stderr and self.stderr.fileno() in ready_fds:
        errs.append(os.read(self.stderr.fileno(), 32768))
      # check
      self.cpu_usage = waiter.timer()
      self.mem_usage = self.pmon.get_mem_usage()
      is_tle = self.cpu_usage > self.tlimit
      is_oom = self.mem_usage > self.mlimit
      if is_oom or is_tle: break
      if self.p.poll() is not None: break
    # concat outputs
    if self.stdout:
      stdout = b''.join(outs).decode(self.stdout.encoding, errors='ignore')
    else:
      stdout = None
    if self.stderr:
      stderr = b''.join(errs).decode(self.stderr.encoding, errors='ignore')
    else:
      stderr = None
    self.pmon.kill_procs()
    retcode = self.p.poll()
    # if retcode not in [None, 0, 1]: wlog(f'{retcode}, {self.cmd}')
    return ExecutionResult(
        is_tle, is_oom, retcode, stdout, stderr)
  def wait(self):
    self.communicate()

class ControlledGDB:
  def __init__(self, cmd:str, mlimit=1*2**30):
    gdb_preex = []
    argv = ['gdb', '--interpreter=mi3',
        '-q', *gdb_preex, '--args', *cmd.split(' ')]
    self.p = subprocess.Popen(argv,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=open('/dev/null', 'w'),
        text=True)
    self.mlimit = mlimit

    self.pmon = ProcessMonitor('wiz.gdb.')
    self.pmon.add_process(self.p.pid)
    self.pmon.set_memory_max(int(1.2 * mlimit),
        allow_unimplemented=True)
    # consume initial response
    self.recv_until(['(gdb)'])
  def is_oom(self):
    return self.pmon.get_mem_usage() > self.mlimit
  def send(self, data): # internal API
    if '\n' not in data: data += '\n'
    self.p.stdin.write(data)
    self.p.stdin.flush()
  def recv_until(self, until=['(gdb)']): # internal API
    line, response, stop = '', '', False
    while not stop:
      line = self.p.stdout.readline()
      response += line
      stop = any(line.startswith(u) for u in until)
    return response
  def recv(self): # internal API
    return self.recv_until()
  def communicate(self, data):
    self.send(data)
    response = self.recv_until(['^done', '*stopped'])
    response += self.recv_until(['(gdb)'])
    return response
  def interrupt(self):
    self.p.send_signal(subprocess.signal.SIGINT)
    return self.recv()
  def run(self, cmd='r', timeout=30):
    self.send(cmd), self.recv() # until (gdb)
    st_time = self.pmon.get_cpu_usage()
    fd_list = [self.p.stdout.fileno()]
    line = ''
    waiter = Waiter(timeout, self.pmon.get_cpu_usage)
    while not waiter.finish():
      ready_fds, _, _ = select.select(fd_list, [], [], waiter.delay)
      has_output = self.p.stdout.fileno() in ready_fds
      waiter.update_delay(has_output)
      if has_output:
        piece = os.read(self.p.stdout.fileno(), 2**16)
        line += piece.decode('ascii', errors='ignore')
        if '(gdb)' in line: break
        if '\n' in line:
          line = line.rsplit('\n', 1)[1]
      if self.pmon.get_mem_usage() > self.mlimit:
        break
    if '(gdb)' not in line:
      self.interrupt()
      return False # means timeout
    return True
  def sample_terminal_functions(self, depth=10, step=0.1, duration=30):
    last_time = time.time()
    counter = {}
    while time.time() < last_time + duration:
      self.run(cmd='c', timeout=step)
      response = self.communicate('-stack-list-frames 0 {depth}')
      funcs = re.findall(r'func="[^"]*"', response)
      for func in funcs:
        func = re.sub(r'func="([^"]*)"', r'\1', func)
        counter.setdefault(func, 0)
        counter[func] += step
    return counter
  def traceback_frames(self):
    response = self.communicate('-stack-list-frames')
    response = re.sub(r'^\^done,stack=\[frame=(.*)\]$', r'\1', response)
    frame_data = re.split(r',frame=', response)
    frames = []
    for data in frame_data:
      frame_dict = {}
      kv_dat = re.findall(r'\b([-\w]+)="([^"]+)"', data)
      for k, v in kv_dat:
        frame_dict[k] = v
      frames.append(frame_dict)
    return tuple(frames)
  def traceback_functions(self):
    functions = []
    for frame in self.traceback_frames():
      func = frame.get('func', '??')
      if func != '??': functions.append(func)
    return tuple(functions)
  def traceback_functions_and_addresses(self):
    functions = []
    for frame in self.traceback_frames():
      func = frame.get('func', '??')
      addr = frame.get('addr', '??')
      if func != '??': functions.append((func, addr))
    return tuple(functions)
  def traceback_functions_and_lines(self):
    functions = []
    for frame in self.traceback_frames():
      func = frame.get('func', '??')
      file = frame.get('fullname', '??')
      line = frame.get('line', '??')
      info = f"{file}: {func}: {line}"
      if func != '??': functions.append(info)
    return tuple(functions)
  def response(self):
    output, _ = self.p.communicate()
    return output
  def __del__(self):
    self.send('q')
    time.sleep(0.1)
    self.p.terminate()
    self.p.wait()
