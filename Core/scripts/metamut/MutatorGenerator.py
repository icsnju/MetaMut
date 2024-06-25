import re
import os
import json
import random
import shutil
import configs
import pathlib
import datetime
import subprocess
from enum import Enum
from utils.ShellProcess import ShellProcess
from .MutatorExecutor import ExecUtils, CheckRes
from .ResponseParser import ParseUtils
from .Mutator import ExceedMaxQueryTimes, Mutator
from jinja2 import Environment, FileSystemLoader


class MutatorGenerator:
  def __init__(self, api, logfile='log.txt',
      existing_mutators=[], max_query_times=20, pass_ratio=1):
    self.api = api
    self.logfile = logfile
    self.existing_mutators = existing_mutators
    self.eval_ctx = {
        'existing_mutators': self.existing_mutators }
    self.query_times = 0
    self.pass_ratio = pass_ratio

    self.max_query_times = max_query_times
    self.stages = {
        "InventMutator": "description",
        "Example": "example",
        "ImplementMutator": "code",
        "RefineMutator": "code", }
  @staticmethod
  def format_timedelta(td):
    total_seconds = int(td.total_seconds())
    hours = total_seconds // 3600
    minutes = (total_seconds % 3600) // 60
    seconds = total_seconds % 60
    milliseconds = td.microseconds // 1000
    return f"{hours:02}:{minutes:02}:{seconds:02}.{milliseconds:03}"
  def jlog(self, **kwargs):
    now = datetime.datetime.now()
    if not hasattr(self, 'start_time'):
      self.start_time = now
    with open(self.logfile, 'a+') as fp:
      delta = now - self.start_time
      d = {"time":self.format_timedelta(delta),
          "rounds":self.query_times, **kwargs}
      s = json.dumps(d)
      print(f'>>>>>> {s}\n')
      fp.write(f'\n>>>>>> {s}\n')
  def tlog(self, text):
    with open(self.logfile, 'a+') as fp:
      fp.write(text)
  def query(self, stage):
    self.curr_stage = stage # for testing
    # fetch template
    templates = list(pathlib.Path(f'stages').glob(f'{stage}-*.txt'))
    t = templates[random.randint(0, len(templates) - 1)]
    # generate query
    env = Environment(loader=FileSystemLoader('stages'))
    template = env.get_template(str(t.name))
    output = template.render(**self.eval_ctx)
    output = re.sub(r'\n\n+', r'\n\n', output)
    self.query_times += 1
    # send query
    self.jlog(stage=stage, action="query", template=str(t.name))
    self.tlog(output)
    response = self.api.send(output)
    self.jlog(stage=stage, action="response")
    self.tlog(response)

    # parse response and return
    if self.stages[stage] == "description":
      return ParseUtils.parse_description(response)
    elif self.stages[stage] == "example":
      return ParseUtils.parse_example(response)
    elif self.stages[stage] == "code":
      return ParseUtils.parse_code(response)
    assert False, f"no output specification for {stage}"
  def check_mutator(self, mutator, example):
    # execute
    chkres = ExecUtils.execute(mutator, example)
    if not chkres.is_valid():
      # clean on execute fail
      mut_cpp_file = f"lib/workbench/{mutator.name}.cpp"
      shutil.rmtree(mut_cpp_file, ignore_errors=True)
    return chkres
  def request_fix(self, mutator, chkres):
    self.eval_ctx["chkres"] = chkres
    self.eval_ctx['mutator'] = mutator
    while self.query_times < self.max_query_times:
      refined_mutator = self.query('RefineMutator')
      if refined_mutator is not None:
        mutator.code = refined_mutator.code
        return mutator
    else:
      raise ExceedMaxQueryTimes(self.mutator.name, self.mutator.desc)
  def refine_mutator(self, mutator, examples):
    while True:
      check_results = []
      random.shuffle(examples)
      # compile
      chkres = ExecUtils.compile(mutator)
      while chkres.isa(CheckRes.NotCompile):
        mutator = self.request_fix(mutator, chkres)
        chkres = ExecUtils.compile(mutator)
        
      # execute
      num_of_passed = 0
      for example in examples:
        chkres = self.check_mutator(mutator, example)
        num_of_passed += int(chkres.isa(CheckRes.Normal))
        self.jlog(action="log",
            chkres=chkres.format_rcode(), example=example)
        check_results.append([example, chkres])
      self.jlog(action="log", num_of_passed=num_of_passed, total=len(examples))
      if num_of_passed >= max(1, int(len(examples) * self.pass_ratio)):
        return mutator

      # hint for passed testcases
      self.eval_ctx['num_of_passed'] = 0
      self.eval_ctx['passed_examples'] = []
      for example, chkres in check_results:
        if chkres.isa(CheckRes.Normal):
          self.eval_ctx['num_of_passed'] += 1
          self.eval_ctx['passed_examples'].append(example)

      # fix
      for example, chkres in check_results:
        if not chkres.isa(CheckRes.Normal):
          mutator = self.request_fix(mutator, chkres)
          break
  def query_examples(self):
    rl = []
    while len(rl) == 0:
      examples = self.query('Example')
      for example in examples:
        ifile = f"{configs.objdir}/input.c"
        pathlib.Path(ifile).write_text(example)
        p = ShellProcess(
            f'clang -xc -w {ifile} -c -o /dev/null',
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL)
        retcode, _, _, _ = p.communicate()
        self.jlog(action="log", subaction="check_example", retcode=retcode, example=example)
        if retcode == 0: rl.append(example)
    return rl
  def implement_mutator(self, mutator):
    while True:
      impled_mutator = self.query('ImplementMutator')
      if impled_mutator is not None:
        mutator.code = impled_mutator.code
        return mutator
      self.jlog(action="log", status="impl_is_none")
      if self.query_times >= self.max_query_times:
        raise ExceedMaxQueryTimes(self.mutator.name, self.mutator.desc)
    return mutator
  def run(self):
    # invent mutators
    mutators = self.query('InventMutator')
    assert len(mutators) > 0
    for name, desc in mutators:
      print(f'[MetaMut] Implementing {name}: {desc}')
      self.mutator = Mutator(name, desc, None)
      self.eval_ctx['mutator'] = self.mutator
      self.eval_ctx['num_of_passed'] = 0
      self.eval_ctx['passed_examples'] = []
      self.examples = self.query_examples()
      self.mutator = self.implement_mutator(self.mutator)
      self.mutator = self.refine_mutator(self.mutator, self.examples)
      self.eval_ctx['existing_mutators'].append((name, desc))
      self.jlog(action="log", status="finalize", name=name, desc=desc)
      return self.mutator
