import os
import re
import ast
import json
import time
import pickle
import random
import select
import signal
import pathlib
import argparse
import datetime
import multiprocessing

def json_serializable(cls):
  def to_json(instance):
    return {
      attr: value.to_json() \
          if hasattr(value, 'to_json') and \
            callable(getattr(value, 'to_json')) \
          else json.loads(json.dumps(value))
      for attr, value in instance.__dict__.items()
    }
  cls.to_json = to_json
  return cls

@json_serializable
class CrashFrame:
  def __init__(self, file, func, line):
    self.file = file
    self.func = func
    self.line = line
  def to_tuple(self):
    return (self.file, self.func, self.line)
  def __hash__(self):
    return hash(self.to_tuple())
  def __str__(self):
    return f"{self.file}: {self.func}: {self.line}"
  def __eq__(self, that):
    return self.to_tuple() == that.to_tuple()
  def drop_line(self):
    return CrashFrame(self.file, self.func, None)
  @classmethod
  def from_json(cls, j):
    return cls(j['file'], j['func'], j['line'])
  @classmethod
  def from_string(cls, s):
    if ': ' in s:
      file, func, line = s.split(': ')
      file = file.split('/')[-1]
      return cls(file if file != "??" else None,
          func, line if line != "??" else None)
    else:
      return cls(None, s.strip(), None)

class CrashFrameTuple:
  def __init__(self, frames=[]):
    assert type(frames) in [tuple, list, set]
    self.frames = list(frames)
    for fr in frames:
      assert type(fr) == CrashFrame
  def to_tuple(self):
    return tuple(fr.to_tuple() for fr in self.frames)
  def drop_line(self):
    return CrashFrameTuple([fr.drop_line() for fr in self.frames])
  def __len__(self):
    return len(self.frames)
  def __getitem__(self, i):
    return self.frames[i]
  def __hash__(self):
    return hash(self.to_tuple())
  def __eq__(self, that):
    return self.to_tuple() == that.to_tuple()
  def __len__(self):
    return len(self.frames)
  def __iter__(self):
    return iter(self.frames)
  def append(self, fr):
    assert type(fr) == CrashFrame
    self.frames.append(fr)
  def to_json(self):
    return [fr.to_json() for fr in self.frames]
  @classmethod
  def from_json(cls, j):
    return cls([CrashFrame.from_json(e) for e in j])

@json_serializable
class RootSeed:
  def __init__(self, size, file):
    self.size = size
    self.file = file
  def to_tuple(self):
    return (self.size, self.file)
  def __hash__(self):
    return hash(self.to_tuple())
  def __eq__(self, that):
    return self.to_tuple() == that.to_tuple()
  @classmethod
  def from_json(cls, j):
    if j is None: return None
    return cls(j['size'], j['file'])

class CompilerInvocation:
  def __init__(self, compiler, options):
    self.compiler = compiler
    self.options = options
  def to_tuple(self):
    return (self.compiler, tuple(set(self.options)))
  def __hash__(self):
    return hash(self.to_tuple())
  def __str__(self):
    return f"{self.compiler} " + " ".join(self.options)
  def __eq__(self, that):
    return self.to_tuple() == that.to_tuple()
  @classmethod
  def from_string(cls, s):
    opts = s.split(' ')
    return cls(opts[0], opts[1:])

class BuggySrcfileMap(dict):
  def __init__(self, srcfiles_map):
    for k in srcfiles_map:
      assert type(k) == CompilerInvocation
      assert type(srcfiles_map[k]) in [list, set, tuple]
      self[k] = list(srcfiles_map[k])
  def merge(self, that):
    for k in that:
      self.setdefault(k, [])
      self[k].extend(that[k])
  def to_json(self):
    return {str(k):self[k] for k in self}
  @classmethod
  def from_json(cls, j):
    return cls({
      CompilerInvocation.from_string(k): j[k] for k in j})

@json_serializable
class BugInst:
  def __init__(self, bugid, bugtype, compiler,
      crash_frames=CrashFrameTuple([]), root_seed=None,
      srcfiles=BuggySrcfileMap({}), extra_fields={}):
    self.bugid = bugid
    self.bugtype = bugtype
    self.compiler = compiler
    self.info = extra_fields
    self.crash_frames = crash_frames
    self.root_seed = root_seed
    self.srcfiles = srcfiles
    assert type(crash_frames) == CrashFrameTuple
    assert root_seed is None or type(root_seed) == RootSeed
    assert type(srcfiles) == BuggySrcfileMap
  def get_identity(self):
    return (self.bugtype, self.compiler,
      self.crash_frames.to_tuple() if self.crash_frames is not None else None,
      self.root_seed.to_tuple() if self.root_seed is not None else None)
  def __hash__(self):
    return hash(self.get_identity())
  def merge(self, that):
    assert hash(self) == hash(that)
    self.srcfiles.merge(that.srcfiles)
    differ_fields = {}
    for k in set(self.info) & set(that.info):
      if self.info[k] == that.info[k]: continue
      differ_fields[k] = []
      if type(self.info[k]) in [list, tuple]:
        differ_fields[k].extend(self.info[k])
      else:
        differ_fields[k].append(self.info[k])
      if type(that.info[k]) in [list, tuple]:
        differ_fields[k].extend(that.info[k])
      else:
        differ_fields[k].append(that.info[k])
      differ_fields[k] = list(set(differ_fields[k]))
    self.info.update(that.info)
    self.info.update(differ_fields)
  @classmethod
  def from_json(cls, j):
    bug = cls(j.get('bugid', 100),
        j['bugtype'], j['compiler'],
        CrashFrameTuple.from_json(j.get('crash_frames', [])
),
        RootSeed.from_json(j.get('root_seed', None)),
        BuggySrcfileMap.from_json(j.get('srcfiles', {})),
        j.get('info', {}))
    return bug

class BugInstMgr:
  def __init__(self, bugs=[]):
    self.bugs = {hash(bug): bug for bug in bugs}
    self.max_bugid = max([bug.bugid for bug in bugs], default=100)
  def __iter__(self):
    return iter(self.bugs.values())
  def add_bug(self, bug):
    assert type(bug) == BugInst
    h = hash(bug)
    if h in self.bugs:
      self.bugs[h].merge(bug)
    else:
      self.max_bugid += 1
      bug.bugid = self.max_bugid
      self.bugs[h] = bug
  def merge(self, that):
    for bug in that.bugs:
      self.add_bug(that.bugs[bug])
  def to_json(self):
    return [self.bugs[h].to_json() for h in self.bugs]
  @classmethod
  def from_json(cls, j):
    bugs = []
    for bug in j:
      bugs.append(BugInst.from_json(bug))
    return cls(bugs)

class FromBugTracks:
  @staticmethod
  def is_irrelevant_function(func):
    return func in [
      "__GI_abort",
      "__assert_fail_base",
      "__GI___assert_fail",
      "diagnostic_action_after_output",
      "diagnostic_report_diagnostic",
      "diagnostic_impl",
      "internal_error",
      "fancy_abort",
      "__GI_exit",
      "__libc_start_call_main",
      "__libc_start_main_impl",
      "_start",
      "llvm::sys::Process::Exit(int, bool)",
      "LLVMErrorHandler(void*, char const*, bool)",
      "llvm::report_fatal_error(llvm::Twine const&, bool)",
      "llvm::report_fatal_error(char const*, bool)",
      "llvm::llvm_unreachable_internal(char const*, char const*, unsigned int)",
      "tree_class_check",
      "llvm::CastInst::Create(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&, llvm::Instruction*)",
      "llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&)",
    ]
  @staticmethod
  def get_identity_frames(tracebacks):
    need_3rd_frame = False
    identity_frames = CrashFrameTuple()
    for tr in tracebacks:
      if ': ' in tr:
        file, func, line = tr.split(': ')
        file = file.split('/')[-1]
      else:
        file, func, line = None, tr, None
      if len(identity_frames) < 2:
        if not FromBugTracks.is_irrelevant_function(func):
          identity_frames.append(CrashFrame.from_string(tr))
    return identity_frames
  @staticmethod
  def update_info_field(newbug, bug):
    newbug.info = {
      k: bug[k] for k in bug if \
          k not in newbug.__dict__ and \
          k not in ['pattern', 'srcfiles']
    }
  @staticmethod
  def make_bug_pattern(bug, cpat):
    crash_frames, root_seed = CrashFrameTuple(), None
    bugtype = bug['pattern']['bugtype']
    if bugtype == str(CompilerStatus.Crash):
      traceback = cpat.get('traceback', [])
      if len(traceback) > 0:
        crash_frames = FromBugTracks.get_identity_frames(traceback)
    else:
      profpat = cpat.get('profpat', {})
      if len(profpat) > 0:
        root_seed = RootSeed(profpat.get('size', None),
            profpat.get('orig', None))
    return crash_frames, root_seed
  @staticmethod
  def from_json(j):
    bug_mgr = BugInstMgr()
    bugs = []
    for bug in j:
      bugid = bug['bugid']
      bugtype = bug['pattern']['bugtype']
      for cpat in bug['pattern']['compile_patterns']:
        compiler = cpat['compiler']
        options = cpat['options'].split(' ')
        crash_frames, root_seed = FromBugTracks.make_bug_pattern(bug, cpat)
        if len(crash_frames) == 0 and root_seed is None: continue
        invocation = CompilerInvocation(compiler, options)
        newbug = BugInst(bugid, bugtype, compiler.split('-')[0],
            crash_frames, root_seed,
            srcfiles=BuggySrcfileMap({invocation:bug['srcfiles']}))
        FromBugTracks.update_info_field(newbug, bug)
        bug_mgr.add_bug(newbug)
    return bug_mgr
