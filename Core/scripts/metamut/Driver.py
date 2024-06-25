import os
import random
import configs
import pathlib
import datetime
import traceback
from llm.APISwitch import make_context
from llm.ReplayAPI import Context as ReplayAPI
from .MutatorGenerator import MutatorGenerator
from .MutatorExecutor import ExecUtils
from .Mutator import ExceedMaxQueryTimes

class MetaMutDriver:
  def __init__(self, existing_mutators=[]):
    # configurations
    self.objs_dir = pathlib.Path("output")
    self.logs_dir = pathlib.Path("logs")
    self.workbench_dir = pathlib.Path("lib/workbench")
    self.mutators_dir = pathlib.Path("lib/mutators")
    self.mutator_bin = pathlib.Path("output/LLMut")
    self.logs_dir.mkdir(parents=True, exist_ok=True)
    self.objs_dir.mkdir(parents=True, exist_ok=True)
    self.workbench_dir.mkdir(parents=True, exist_ok=True)
    self.mutators_dir.mkdir(parents=True, exist_ok=True)
    self.existing_logfiles = set(map(str, self.logs_dir.glob('*')))
    # preparations
    self.compile_project()
    # --------------------
    self.existing_mutators = existing_mutators
    self.existing_mutators.extend(ExecUtils.list_mutators())
  def compile_project(self):
    os.system(f'cd {self.objs_dir}; cmake ..; make -j{configs.jobs}')
  def get_randomized_mutators(self):
    mutators = [*self.existing_mutators]
    random.shuffle(mutators)
    return mutators
  def next_logfile(self, base='mutator'):
    index = 0
    while True:
      f = f'logs/{base}-{index}.txt'
      if f not in self.existing_logfiles:
        self.existing_logfiles.add(f)
        return f
      index += 1
    return None
  def clean_workbench(self):
    for filename in os.listdir(self.workbench_dir):
      file_path = os.path.join(self.workbench_dir, filename)
      try:
        if os.path.isfile(file_path) or os.path.islink(file_path):
          os.unlink(file_path)
        elif os.path.isdir(file_path):
          shutil.rmtree(file_path)
      except Exception as e:
        print(f'Failed to delete {file_path}. Reason: {e}')
  def save_mutator(self, mutator):
    if mutator is None: return
    srcfile = self.mutators_dir / f"{mutator.name}.cpp"
    print(f"[MetaMut] Saving {mutator.name} to {srcfile}")
    srcfile.write_text(mutator.code)
    self.existing_mutators.append([mutator.name, mutator.desc])
  def record_exception(self, e, logfile):
    # from exception
    if type(e) == ExceedMaxQueryTimes:
      self.existing_mutators.append([e.name, e.desc])
    # dump exception
    s = '\n'
    s += f"======= {datetime.datetime.now()} =======\n"
    s += ''.join(traceback.format_exception(
        type(e), e, e.__traceback__))
    with open(logfile, 'a+', errors='ignore') as fp:
      fp.write(s)
    print(s)
  def generate_single_mutator(self, api, logfile):
    self.clean_workbench()
    mg = MutatorGenerator(api, logfile,
        self.get_randomized_mutators())
    try:
      self.save_mutator(mg.run())
    except KeyboardInterrupt as e:
      raise
    except Exception as e:
      self.record_exception(e, logfile)
  def generate_mutators(self, n=100, api=make_context()):
    for i in range(n):
      self.generate_single_mutator(api,
          self.next_logfile())
  def replay_logs(self, logfiles):
    for logfile in logfiles:
      api = ReplayAPI(logfile)
      print(f'[MetaMut] Replaying {logfile}')
      self.generate_single_mutator(
          api, self.next_logfile())
  def invent_mutators(self, n=100, api=make_context()):
    invention_logfile = self.next_logfile('invent')
    for i in range(n):
      mg = MutatorGenerator(api, self.next_logfile(),
          self.get_randomized_mutators())
      mutators = mg.query('InventMutator')
      self.existing_mutators.extend(mutators)
      with open(invention_logfile, "a+") as fp:
        for name, desc in mutators:
          print(f"# {name}: {desc.strip()}\n")
          fp.write(f"# {name}: {desc.strip()}\n")
