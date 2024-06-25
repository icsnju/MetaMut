import re
import sys
import json
import pathlib

def format_timedelta(td):
  total_seconds = int(td.total_seconds())
  hours = total_seconds // 3600
  minutes = (total_seconds % 3600) // 60
  seconds = total_seconds % 60
  milliseconds = td.microseconds // 1000
  return f"{hours:02}:{minutes:02}:{seconds:02}.{milliseconds:03}"

def parse_log(filename):
  rounds, dates = [], []
  text = pathlib.Path(filename).read_text().strip()
  curr_round = ""
  r1 = re.compile(r'^Q>>>>>>>>(.*)$')
  r2 = re.compile(r'^A<<<<<<<<(.*)$')
  first_try = True
  for line in text.splitlines():
    if line.startswith('Q>>>>>>>>') or \
        line.startswith('A<<<<<<<<'):
      q = r1.search(line)
      if q: dates.append(q.group(1))
      a = r2.search(line)
      if a: dates.append(a.group(1))
      if not first_try:
        rounds.append(curr_round)
        curr_round = ""
      first_try = False
    else:
      curr_round += line + '\n'
  rounds.append(curr_round)
  # print(filename)
  # print(json.dumps(rounds, indent=2))
  # print(json.dumps(list(map(str, dates)), indent=2))
  # sys.exit(0)
  return rounds, dates

for f in sys.argv[1:]:
  rounds, dates = parse_log(f)
  assert len(rounds) == len(dates)
  outs = ''
  for i in range(len(rounds)):
    if i < 2: stage = 'InventMutator'
    elif i < 4: stage = 'Example'
    elif i < 6: stage = 'ImplementMutator'
    else: stage = 'RefineMutator'
    if i % 2 == 0: action = 'query'
    else: action = 'response'

    j = {'time': dates[i], 'rounds': i//2,
        'stage':stage, 'action':action}
    s = json.dumps(j)
    outs += f'\n>>>>>> {s}\n'
    outs += rounds[i].strip()
  pathlib.Path(f).write_text(outs)
