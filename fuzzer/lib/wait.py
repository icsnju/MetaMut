import time
from .logger import wlog

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
    # wlog(f'update({hit_event}) {old_delay} -> {self.delay} -({self.remaining})')
  def wait(self):
    time.sleep(self.delay)
    self.update_delay()
