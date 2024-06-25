import os
import datetime
import traceback
import errno

class ExceptionRecorder:
  def __init__(self, logfile):
    self.logfile = logfile
    self.log_directory = os.path.dirname(logfile)
    
    if not os.path.exists(self.log_directory):
      os.makedirs(self.log_directory)
    
    # Check if the logfile can be opened for writing
    try:
      with open(self.logfile, 'a'):
        pass
    except IOError as e:
      raise IOError(f"Cannot open logfile '{self.logfile}' for writing: {str(e)}")

  def __enter__(self):
    return self

  def __exit__(self, exc_type, exc_value, traceback):
    if exc_type is not None:
      if exc_type == KeyboardInterrupt:
        return False
      elif exc_type == OSError and exc_value.errno == errno.ENOSPC:
        return False
      else:
        self.record_exception(exc_value)
        return True

  def record_exception(self, exception):
    s = f"======= {datetime.datetime.now()} =======\n"
    s += ''.join(traceback.format_exception(
      type(exception), exception, exception.__traceback__))
    with open(self.logfile, 'a', errors='ignore') as fp:
      fp.write(s)
    print(s)
