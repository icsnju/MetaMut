import numpy as np
from common import *

raw_buf = b"12345678"*20+b"ABCDEFG"
raw_bits = np.frombuffer(raw_buf, dtype=np.uint8)
raw_size = len(raw_bits) * 8 - 1

coverage = Coverage(raw_bits, raw_size)

# file
coverage.to_file("a.bin", True)
n = Coverage.from_file("a.bin")
os.remove("a.bin")
assert coverage == n

# file
coverage.to_file("b.bin", False)
n = Coverage.from_file("b.bin", coverage.nbrs)
os.remove("b.bin")
assert coverage == n

# shm
shm = ShmObj(size=len(raw_bits))
coverage.to_shm(shm)
n = Coverage.from_shm(shm, raw_size)
assert coverage == n

# shm
a = CoverageSampler(120)
b = CoverageSampler(1200)
c = CoverageSampler(1200)
if '__AFL_SHM_ID' in os.environ:
  del os.environ['__AFL_SHM_ID']
with a:
  assert str(a.shm.id) == os.environ['__AFL_SHM_ID']
  with b:
    assert str(b.shm.id) == os.environ['__AFL_SHM_ID']
    with c:
      assert str(c.shm.id) == os.environ['__AFL_SHM_ID']
    assert str(b.shm.id) == os.environ['__AFL_SHM_ID']
  assert str(a.shm.id) == os.environ['__AFL_SHM_ID']
assert '__AFL_SHM_ID' not in os.environ
