import os
import signal
import subprocess

class ShellProcess(subprocess.Popen):
  print_exec = False
  def __init__(self, cmd, tlimit=30, mlimit=2**30,
      shell=True, text=True,
      stdin=None, stdout=None, stderr=None):
    self.cmd = cmd
    self.tlimit = tlimit
    self.mlimit = mlimit
    super().__init__(cmd, shell=shell,
        preexec_fn=os.setpgrp, stdin=stdin,
        stdout=stdout, stderr=stderr, text=text)
  def communicate(self, input=None):
    if self.print_exec:
      print(f"exec: {self.cmd}")
    # run the shell
    retcode, isTimeout = None, False
    stdout, stderr = None, None
    try:
      stdout, stderr = super().communicate(
          input=input, timeout=self.tlimit)
    except subprocess.TimeoutExpired:
      pgrp = os.getpgid(self.pid)
      os.killpg(pgrp, signal.SIGKILL)
      isTimeout=True
    self.wait()
    if self.print_exec:
      print(f"exec: result: {self.poll()}, {isTimeout}")
    return self.poll(), isTimeout, stdout, stderr
  def terminate(self):
    try:
      pgrp = os.getpgid(self.pid)
      os.killpg(pgrp, signal.SIGKILL)
    except ProcessLookupError:
      pass

