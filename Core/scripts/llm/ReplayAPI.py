import re
import sys
import json
import random
import pathlib
from .Exceptions import NetworkFailure

def parse_log(filename):
  rounds = []
  text = pathlib.Path(filename).read_text().strip()
  curr_round, curr_j = "", None
  r1 = re.compile(r'^>>>>>> (.*)$')
  first_try = True
  for line in text.splitlines():
    if line.startswith('>>>>>>'):
      q = r1.search(line)
      j = eval(q.group(1))
      if not first_try and curr_j.get('action', '') in ['query', 'response']:
        rounds.append([curr_j, curr_round])
      curr_round = ""
      curr_j = j
      first_try = False
    else:
      curr_round += line + "\n"
  if curr_j.get('action', '') in ['query', 'response']:
    rounds.append([curr_j, curr_round])
  # print(filename)
  # print(json.dumps(rounds, indent=2))
  # sys.exit(0)
  return rounds

class Context:
  def __init__(self, logfile):
    self.logfile = logfile
    self.round_idx = 0
    self.rounds = parse_log(logfile)
  def link(self, mg):
    self.mg = mg
  def send(self, text):
    self.round_idx += 1
    index = self.round_idx * 2 - 1
    if index < len(self.rounds):
      return self.rounds[index][1]
    raise NetworkFailure()
