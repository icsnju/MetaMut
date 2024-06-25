import sys
import llm.PythonOpenai
import llm.HttpsOpenai

OpenaiContextCtor = llm.PythonOpenai.Context

def make_context(*args, **kwargs):
  return OpenaiContextCtor(*args, **kwargs)

def switch_llm_api(name):
  global OpenaiContextCtor
  if name == 'python.openai':
    OpenaiContextCtor = llm.PythonOpenai.Context
    print('use official python.openai api')
  elif name == 'https.openai':
    OpenaiContextCtor = llm.HttpsOpenai.Context
    print('use official https.openai api')
  else:
    print(f'Error: unknown api source {name}')
    sys.exit(0)
