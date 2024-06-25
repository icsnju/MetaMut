import time
import psutil
from multiprocessing import Process

class Scheduler:
  def __init__(self, worker, worker_args, jobs):
    self.worker = worker
    self.worker_args = worker_args
    self.jobs = jobs
    self.ps = []
    self.cores = list(range(self.jobs))

  def execute_jobs(self):
    jobs = iter(self.worker_args)
    while True:
      job = next(jobs, None)
      if job is None: break
      while len(self.cores) == 0:
        _ps = []
        for affinity, p, wa in self.ps:
          if p.is_alive():
            _ps.append([affinity, p, wa])
            continue
          self.cores.append(affinity)
        self.ps = _ps
        time.sleep(1)
      affinity = self.cores.pop()
      p = Process(target=self.worker, args=[job])
      p.start()
      self.ps.append([affinity, p, job])
      psutil.Process(p.pid).cpu_affinity([
        affinity, affinity + psutil.cpu_count() // 2])
    for _, p, _ in self.ps: p.join()
