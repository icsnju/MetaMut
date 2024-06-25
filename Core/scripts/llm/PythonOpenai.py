import re
import time
import tiktoken
import openai
import traceback
import random
from .OpenaiUtils import *

class Context:
  def __init__(self, model='gpt-4-0613', top_p=0.95, temperature=1.0):
    self.system_prompt = {"role": "system",
        "content": "You are an AI programming assistant."}
    self.conversation = [ ]
    self.model = model
    self.top_p = top_p
    self.temperature = temperature
    self.curr_completion = None
  def send(self, text):
    while True:
      try:
        return self.send_internal(text)
      except KeyboardInterrupt:
        print("Interrupted by Ctrl+C. stopping the program...")
        raise
      except (openai.RateLimitError, openai.BadRequestError) as e:
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
      remain_tokens = num_max_tokens(self.model) - \
          num_tokens_from_messages(messages, self.model)
      if remain_tokens >= 1024: break
      # print(f'drop history {self.conversation[0]}')
      self.conversation.pop(0)

    self.completion = openai.chat.completions.create(
      model=self.model,
      max_tokens=remain_tokens,
      messages=messages,
      temperature=self.temperature,
      top_p=self.top_p,
      frequency_penalty=0,
      presence_penalty=0,
      stop=None)

    self.conversation.append(request)
    response = self.completion.choices[0].message.to_dict()
    self.conversation.append(response)
    # print(self.conversation)
    return response['content']
