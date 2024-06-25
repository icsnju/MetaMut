import os
import sys
import time
import openai
import configs
import pathlib
import argparse
from metamut.Driver import MetaMutDriver
from llm.ReplayAPI import Context as ReplayAPI
from llm.APISwitch import make_context, switch_llm_api

def parse_args():
  parser = argparse.ArgumentParser('MetaMut.driver')
  parser.add_argument('--replay-logs',
      dest='replay_logs', nargs='+',
      help='Replay the generation process from the specified log files.')
  parser.add_argument('--print-model-list',
      dest='print_model_list', action='store_true',
      help='Print a list of available models and exit.')
  parser.add_argument('--invent-only',
      dest='invent_only', action='store_true',
      help='Invent new mutators without generating them.')
  parser.add_argument('--num-mutators', dest='num_mutators',
      type=int, default=100,
      help='specify the number of mutators to be generated or invented (default: 100)')
  parser.add_argument('-j', dest='jobs',
      type=int, default=1,
      help='specify the number of jobs to compile mutators')
  parser.add_argument('--api', dest='api', default='python.openai',
      help='specify the api source, choices are "python.openai" and "https.openai"')
  return parser.parse_args()

def test_simple_dialogue():
  print(f'Testing simple diaglogue ...')
  request = "What's the answer for 10 + 1?"
  print(f'Q> {request}')
  api = make_context()
  response = api.send(request)
  print(f'A> {response}')
  is_right = '11' in response
  if is_right:
    print('It seems OK for this API setting')
  else:
    print('It seems not OK for this API setting')
  return is_right

def main(args):
  if args.print_model_list:
    print(openai.models.list())
    sys.exit(0)

  if args.api:
    switch_llm_api(args.api)
  
  if not test_simple_dialogue():
    sys.exit(0)

  configs.jobs = args.jobs

  driver = MetaMutDriver()
  if args.invent_only:
    if args.replay_logs:
      for logfile in args.replay_logs:
        driver.invent_mutators(n=1, api=ReplayAPI(logfile))
    else:
      driver.invent_mutators(n=args.num_mutators, api=make_context())
  elif args.replay_logs:
    driver.replay_logs(args.replay_logs)
  else:
    driver.generate_mutators(n=args.num_mutators,
        api=make_context())

if __name__ == "__main__":
  main(parse_args())
