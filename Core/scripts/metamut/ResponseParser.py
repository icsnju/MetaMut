import re
from .Mutator import Mutator

class ParseUtils:
  codepat = re.compile(
      r'RegisterMutator<(\w+)>\s*\w+\s*\(\s*"([^"]+)"\s*,\s*"([^"]+)"\s*',
      re.I | re.M)
  @classmethod
  def fetch_codeboxes(cls, output):
    codeboxes = []
    isCodeBoxBegin, codeLines = False, []
    for line in output.split('\n'):
      if line.startswith('```'):
        if isCodeBoxBegin:
          if len(line.strip()) == 3:
            codeboxes.append('\n'.join(codeLines))
            isCodeBoxBegin = False
        else:
          codeLines = []
          isCodeBoxBegin = True
      elif isCodeBoxBegin:
        codeLines.append(line)
    return codeboxes
  @classmethod
  def parse_description(cls, text):
    mutators = []
    res = re.search(r'^\W*(\w+)\W*:', text)
    if res:
      text = re.sub(r'^\W*(\w+)\W*:', '', text)
      text = text.replace('```', '')
      return [(res.group(1), text.strip())]
    return []
  @classmethod
  def parse_example(cls, text):
    return cls.fetch_codeboxes(text)
  @classmethod
  def parse_code(cls, code):
    for codebox in cls.fetch_codeboxes(code):
      res = ParseUtils.codepat.search(codebox)
      if res:
        name = res.group(1)
        desc = res.group(3)
        return Mutator(name, desc, codebox)
    return None
