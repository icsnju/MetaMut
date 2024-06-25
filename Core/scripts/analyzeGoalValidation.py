import re
import json
import pathlib
import argparse
import tiktoken
import statistics
import subprocess
from llm.OpenaiUtils import num_tokens_from_messages

def parse_args():
  parser = argparse.ArgumentParser('analyzer')
  parser.add_argument('--logs', dest='logs',
    nargs='+', required=True,
    help='specify log files')
  parser.add_argument('--binary', dest='binary',
    default='/bin/mu.u', help='specify mutator binary')
  return parser.parse_args()

args = parse_args()
r1 = "The mutator is causing the program to hang."
r2 = "The mutator is transforming a semantically valid program into an invalid one"
r3 = "The output from the mutator is identical to the input"
r4 = "The mutator itself crashes."
r5 = "The implemented mutator is not compiling."
r6 = "The implemented mutator output nothing."

ri = "Assist me in implementing the mutator named (\w+)"

mutators = {}
p = subprocess.Popen(f'{args.binary} --list-mutators',
    shell=True, stdout=subprocess.PIPE)
stdout, _ = p.communicate()
for name, desc in json.loads(stdout):
  mutators[name] = desc

usable_mutators = []

n_usable = 0
n_unusable = 0
n_hang = 0
n_invalid = 0
n_nochange = 0
n_crash = 0
n_notcompile = 0
n_nooutput = 0
for f in map(pathlib.Path, args.logs):
  text = f.read_text()
  mutator = re.search(ri, text).group(1)
  if mutator not in mutators:
    n_unusable += 1
  else:
    usable_mutators.append(mutator)
    n_usable += 1
  if re.search(r1, text): n_hang += 1
  if re.search(r2, text): n_invalid += 1
  if re.search(r3, text): n_nochange += 1
  if re.search(r4, text): n_crash += 1
  if re.search(r5, text): n_notcompile += 1
  if re.search(r6, text): n_nooutput += 1

# for m in usable_mutators:
#   print(f"{m}:", mutators[m])

# print("n_usable:", n_usable)
# print("n_unusable:", n_unusable)
print("m not compile:", n_notcompile)
print("m hangs:", n_hang)
print("m crashes:", n_crash)
print("m outputs nothing:", n_nooutput)
print("m does not rewrite:", n_nochange)
print("m creates compile-error mutant:", n_invalid)
