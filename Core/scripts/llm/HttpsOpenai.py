import re
import time
import tiktoken
import openai
import requests
import traceback
import random
import configs
from .OpenaiUtils import *

class Context:
  def __init__(self, engine='gpt-4', top_p=0.95, temperature=1.0):
    self.system_prompt = {"role": "system",
        "content": "You are an AI programming assistant."}
    self.conversation = [ ]
    self.engine = engine
    self.top_p = top_p
    self.temperature = temperature
    self.curr_completion = None
    self.headers = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer {openai.api_key}',
    }

    if 'chat/completions' not in openai.base_url:
      if openai.base_url.endswith('/'):
        self.base_url = f"{openai.base_url}chat/completions"
      else:
        self.base_url = f"{openai.base_url}/chat/completions"
    else:
      self.base_url = openai.base_url
    print(f'use {self.base_url} as https.openai interface')
  def send(self, text):
    while True:
      try:
        return self.send_internal(text)
      except KeyboardInterrupt:
        print("Interrupted by Ctrl+C. stopping the program...")
        raise
      except Exception as e:
        s = ''.join(traceback.format_exception(
            type(e), e, e.__traceback__))
        print(s)
        time.sleep(21 + random.random() * 8)
        continue
      else:
        break
  def send_internal(self, text):
    request = { "role": "user", "content": text }

    # drop previous tokens
    while True:
      messages = [ self.system_prompt,
          *self.conversation, request ]
      remain_tokens = num_max_tokens(self.engine) - \
          num_tokens_from_messages(messages, self.engine)
      if remain_tokens >= 1024: break
      # print(f'drop history {self.conversation[0]}')
      self.conversation.pop(0)

    response = requests.post(self.base_url,
        headers=self.headers,
        json={'model':self.engine, 'messages':messages})

    self.conversation.append(request)
    response = response.json()['choices'][0]['message']
    self.conversation.append(response)
    # print(self.conversation)
    return response['content']
