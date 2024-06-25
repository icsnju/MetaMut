import os
import re
import sys
import ast
import time
import select
import subprocess

class Waiter:
  def __init__(self, timeout, timer=time.time):
    self.min_delay = 0.01
    self.max_delay = 5
    self.delay = self.min_delay
    self.timeout = timeout
    self.st_time = timer()
    self.ed_time = self.st_time + timeout
    self.remaining = self.ed_time - timer()
    self.timer = timer
  def finish(self):
    return self.timer() >= self.ed_time
  def update_delay(self, hit_event=False):
    # old_delay = self.delay
    self.remaining = self.ed_time - self.timer()
    if hit_event:
      # self.delay = max(self.delay / 1.2, self.min_delay)
      self.delay = self.min_delay
    else:
      self.delay = min(self.delay * 2, self.remaining, self.max_delay)
  def wait(self):
    time.sleep(self.delay)
    self.update_delay()

class GDBController:
  def __init__(self, cmd:str, mlimit=1*2**30):
    gdb_preex = []
    argv = ['gdb', '--interpreter=mi3',
        '-q', *gdb_preex, '--args', *cmd.split(' ')]
    self.p = subprocess.Popen(argv,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=open('/dev/null', 'w'),
        text=True)
    # consume initial response
    self.recv_until(['(gdb)'])
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
    fd_list = [self.p.stdout.fileno()]
    line = ''
    waiter = Waiter(timeout)
    while not waiter.finish():
      ready_fds, _, _ = select.select(fd_list, [], [], waiter.delay)
      has_output = self.p.stdout.fileno() in ready_fds
      waiter.update_delay(has_output)
      if has_output:
        piece = os.read(self.p.stdout.fileno(), 2**15)
        line += piece.decode('ascii', errors='ignore')
        if '(gdb)' in line: break
        if '\n' in line:
          line = line.rsplit('\n', 1)[1]
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
  def response(self):
    output, _ = self.p.communicate()
    return output
  def __del__(self):
    self.send('q')
    time.sleep(0.1)
    self.p.terminate()
    self.p.wait()

