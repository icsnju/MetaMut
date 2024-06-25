import re
import os
import json
import random
import configs
import pathlib
import datetime
import subprocess

class ExceedMaxQueryTimes(Exception):
  def __init__(self, name, desc):
    self.name = name
    self.desc = desc

class Mutator:
  def __init__(self, name, desc, code):
    self.name = name
    self.desc = desc
    self.code = code
