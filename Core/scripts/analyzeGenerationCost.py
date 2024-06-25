import re
import json
import pathlib
import tiktoken
import statistics
import datetime
import numpy as np
import argparse
from statistics import median, mean
from llm.OpenaiUtils import num_tokens_from_messages

def parse_args():
  parser = argparse.ArgumentParser('analyzer')
  parser.add_argument('--logs', dest='logs',
    nargs='+', required=True,
    help='specify log files')
  return parser.parse_args()

def parse_timestamp(timestamp):
  return datetime.datetime.strptime(timestamp, "%H:%M:%S.%f")

def parse_log(filename):
  rounds, dates = [], []
  text = pathlib.Path(filename).read_text().strip()
  curr_round = ""
  r = re.compile(r'^>>>>>> (.*)$')
  first_try = True
  for line in text.splitlines():
    if line.startswith('>>>>>>'):
      rtag = r.search(line)
      if rtag:
        rtag = json.loads(rtag.group(1))
        dates.append(parse_timestamp(rtag['time']))
      if not first_try:
        rounds.append(curr_round)
        curr_round = ""
      first_try = False
    else:
      curr_round += line
  rounds.append(curr_round)
  # print(rounds, dates)
  return rounds, dates
        
def num_tokens_from_messages(messages, model="gpt-3.5-turbo-0613"):
  if model == "gpt-35-turbo-16k": model = "gpt-3.5-turbo-16k-0613"
  """Return the number of tokens used by a list of messages."""
  try:
    encoding = tiktoken.encoding_for_model(model)
  except KeyError:
    # print("Warning: model not found. Using cl100k_base encoding.")
    encoding = tiktoken.get_encoding("cl100k_base")
  if model in {
    "gpt-3.5-turbo-0613",
    "gpt-3.5-turbo-16k-0613",
    "gpt-4-0314",
    "gpt-4-32k-0314",
    "gpt-4-0613",
    "gpt-4-32k-0613", }:
    tokens_per_message = 3
    tokens_per_name = 1
  elif model == "gpt-3.5-turbo-0301":
    tokens_per_message = 4  # every message follows <|start|>{role/name}\n{content}<|end|>\n
    tokens_per_name = -1  # if there's a name, the role is omitted
  elif "gpt-3.5-turbo" in model:
    # print("Warning: gpt-3.5-turbo may update over time. Returning num tokens assuming gpt-3.5-turbo-0613.")
    return num_tokens_from_messages(messages, model="gpt-3.5-turbo-0613")
  elif "gpt-4" in model:
    # print("Warning: gpt-4 may update over time. Returning num tokens assuming gpt-4-0613.")
    return num_tokens_from_messages(messages, model="gpt-4-0613")
  else:
    raise NotImplementedError(
      f"""num_tokens_from_messages() is not implemented for model {model}. See https://github.com/openai/openai-python/blob/main/chatml.md for information on how messages are converted to tokens."""
    )
  num_tokens = 0
  for message in messages:
    num_tokens += tokens_per_message
    for key, value in message.items():
      num_tokens += len(encoding.encode(value))
      if key == "name":
        num_tokens += tokens_per_name
  num_tokens += 3  # every reply is primed with <|start|>assistant<|message|>
  return num_tokens

encoding = tiktoken.encoding_for_model("gpt-4")
nTokens = {"Invention":[], "Implementation":[], "Bug-Fixing":[], "Total":[]}
QARounds = {"Invention":[], "Implementation":[], "Bug-Fixing":[], "Total":[]}
QATime = {"Invention":[], "Implementation":[], "Bug-Fixing":[], "Total":[]}
QTime = []
ATime = []
args = parse_args()
for f in args.logs:
  if "network" in str(f): continue
  rounds, dates = parse_log(f)
  # print(f, len(rounds), len(dates))
  nTokens["Invention"].append(len(encoding.encode(rounds[0])) + len(encoding.encode(rounds[1])))
  nTokens["Implementation"].append(len(encoding.encode(rounds[4])) + len(encoding.encode(rounds[5])))
  nTokens["Bug-Fixing"].append(
      len(encoding.encode(rounds[2])) + len(encoding.encode(rounds[3])) +
      sum([len(encoding.encode(r)) for r in rounds[6:]]))
  nTokens["Total"].append(sum([len(encoding.encode(r)) for r in rounds]))
  # QA
  # if len(rounds[6:]) == 44: print(f)
  QARounds["Bug-Fixing"].append(1 + len(rounds[6:])//2)
  # time
  QATime["Invention"].append((dates[1]-dates[0]).total_seconds())
  QATime["Implementation"].append((dates[5]-dates[4]).total_seconds())
  QATime["Bug-Fixing"].append(
      (dates[3]-dates[2]).total_seconds() +
      (dates[-1]-dates[5]).total_seconds())
  QATime["Total"].append((dates[-1]-dates[0]).total_seconds())
  # time
  for i in range(1, len(rounds), 2):
    ATime.append((dates[i]-dates[i-1]).total_seconds())
  for i in range(2, len(rounds), 2):
    QTime.append((dates[i]-dates[i-1]).total_seconds())

print("tokens:")
for stage in nTokens:
  print(stage+":",
       "min:", min(nTokens[stage]),
       "max:", max(nTokens[stage]),
       "median:", median(nTokens[stage]),
       "mean:", mean(nTokens[stage]))
print("")

print("rounds:")
for stage in ["Bug-Fixing"]:
  print(stage+":",
       "min:", min(QARounds[stage]),
       "max:", max(QARounds[stage]),
       "median:", median(QARounds[stage]),
       "mean:", mean(QARounds[stage]))
print("Total"+":",
     "min:", min(QARounds[stage])+2,
     "max:", max(QARounds[stage])+2,
     "median:", median(QARounds[stage])+2,
     "mean:", mean(QARounds[stage])+2)
print("")

print("time")
for stage in QATime:
  print(stage+":",
       "min:%.0f"%(min(QATime[stage])),
       "max:%.0f"%(max(QATime[stage])),
       "median:%.0f"%(median(QATime[stage])),
       "mean:%.0f"%(mean(QATime[stage])))
print("")

print("Wait for Response:", end=' ')
print("min:%.0f"%(min(ATime)),
     "max:%.0f"%(max(ATime)),
     "median:%.0f"%(median(ATime)),
     "mean:%.0f"%(mean(ATime)))
print("Prepare for Request:", end=' ')
print("min:%.0f"%(min(QTime)),
     "max:%.0f"%(max(QTime)),
     "median:%.0f"%(median(QTime)),
     "mean:%.0f"%(mean(QTime)))
