import os
import time
import shutil
import psutil
import resource
import threading
from threading import Thread

def clean_shared_memory():
  os.system("for SHM_ID in $(ipcs -m | awk '$6 == 0 { print $2 }'); do ipcrm -m $SHM_ID; done")

def clean_processes(proc_names):
  # when a process escape from its parent, kill it
  for p in psutil.Process(1).children():
    try:
      if p.name() not in proc_names: continue
      p.kill()
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.Error):
      pass

def start_thread_to_clean_processes(ps):
  stop_thread = threading.Event()
  def task(l):
    while not stop_thread.is_set():
      try:
        clean_processes(l)
        time.sleep(5)
      except KeyboardInterrupt:
        break
  t = Thread(target=task, args=(ps,))
  t.start()
  return stop_thread

def set_unlimited_stack():
  try:
    # Get current stack limits
    soft, hard = resource.getrlimit(resource.RLIMIT_STACK)
    # print(f"Current Stack Limits: Soft={soft}, Hard={hard}")
    # Set the stack limit to unlimited, respecting the hard limit
    resource.setrlimit(resource.RLIMIT_STACK, (resource.RLIM_INFINITY, hard))
    # print("Stack size has been set to unlimited")
    # Verify the change
    new_soft, new_hard = resource.getrlimit(resource.RLIMIT_STACK)
    # print(f"New Stack Limits: Soft={new_soft}, Hard={new_hard}")
  except ValueError as e:
    print(f"Error setting stack limit: {e}")
  except resource.ResourceError as e:
    print(f"Failed to set stack limit: {e}")

def remove_files(directory):
  if not os.access(directory, os.X_OK | os.R_OK):
    return
  for filename in os.listdir(directory):
    file_path = os.path.join(directory, filename)
    try:
      if os.path.isfile(file_path) or os.path.islink(file_path):
        os.unlink(file_path)
      elif os.path.isdir(file_path):
        shutil.rmtree(file_path)
    except Exception as e:
      pass

def kill_proc_tree(pid, including_parent=True, timeout=5):
  try:
    parent = psutil.Process(pid)
  except psutil.NoSuchProcess:
    return

  children = parent.children(recursive=True)
  for child in children:
    try:
      child.suspend()
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.Error):
      pass

  time.sleep(1)

  children = parent.children(recursive=True)
  for child in children:
    try:
      child.kill()
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.Error):
      pass

  psutil.wait_procs(children, timeout=timeout)

  if including_parent:
    try:
      parent.kill()
      parent.wait(timeout)
    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.Error):
      pass
