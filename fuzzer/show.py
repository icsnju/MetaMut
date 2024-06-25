import os
import json
import pathlib
import datetime
import argparse
from lib.BugInst import FromBugTracks

class Bug:
  def __init__(self, cc, options, traceback):
    self.cc = cc
    self.options = tuple(options)
    frames = FromBugTracks.get_identity_frames(traceback)
    self.traceback = tuple([fr.to_tuple() for fr in frames])
  def to_tuple(self):
    return tuple([self.cc, self.options, self.traceback])
  def to_json(self):
    return {
        "compiler": self.cc,
        "options": ' '.join(self.options),
        "traceback": tuple(self.traceback),}
  def __hash__(self):
    return hash(self.to_tuple())
  def __eq__(self, that):
    if not isinstance(that, Bug):
      return False
    return self.to_tuple() == that.to_tuple()

def parse_start_date(logfile):
  with open(logfile) as fp:
    line = fp.readline()
    date_string = line.split(']')[0].strip("[]")
    date_object = datetime.datetime.strptime(date_string, "%Y-%m-%d %H:%M:%S.%f")
    return date_object

def load_bugs(dirs):
  bugmap = {}
  for d in dirs:
    d = pathlib.Path(d)
    st_date = parse_start_date(d/'log.txt')
    if not d.is_dir(): continue
    if ',' not in str(d): continue
    mutator, cc, num = str(d).split(',')
    jfile = d/'crashes.json'
    if os.access(jfile, os.R_OK):
      try:
        jobj = json.loads(jfile.read_text())
      except:
        continue
      for bug in jobj:
        bobj = Bug(bug['compiler'], [bug['options']], bug['traceback'])
        if len(bobj.traceback) == 0:
          print(f'drop invalid {jfile}')
          continue
        bugmap.setdefault(bobj, [])
        for fpath, secs in bug['srcfiles']:
          bugmap[bobj].append({
            "mutator": mutator,
            "date": secs,
            "file": fpath,})
  # convert to json
  retj = []
  for bug in bugmap:
    retj.append(bug.to_json())
    retj[-1]["mutators"] = list(set([e["mutator"] for e in bugmap[bug]]))
    retj[-1]["srcfiles"] = bugmap[bug]
  return retj

if __name__ == "__main__":
  parser = argparse.ArgumentParser('driver')
  parser.add_argument('dirs', nargs="+", help="specify working dirs")
  parser.add_argument('--summary', action='store_true',
      help="print summary")
  args = parser.parse_args()

  bugs = load_bugs(args.dirs)
  print(json.dumps(bugs, indent=2))
  if args.summary:
    print('')
    scores = {}
    for bug in bugs:
      for m in set(bug['mutators']):
        scores.setdefault(m, 0)
        scores[m] += 1
    print(json.dumps(scores, indent=2))
