#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

volatile uint32_t __afl_num_of_branches = 1 << 23;
volatile char __afl_bbmap_dir[256] = ".";

uint8_t *__afl_bbmap_ptr = NULL;

/* Uninspired gcc plugin instrumentation */
void __afl_trace_bb(const uint32_t x) {
  if (__afl_bbmap_ptr)
    __afl_bbmap_ptr[x / 8] |= 1 << (x & 7);
}

__attribute__((constructor())) void __afl_auto_init(void) {
  if (getenv("PRINT_NBRS")) {
    printf("%d", __afl_num_of_branches);
    fflush(stdout);
    exit(0);
  }

  if (getenv("PRINT_BBMAP_DIR")) {
    printf("%s", __afl_bbmap_dir);
    fflush(stdout);
    exit(0);
  }

  const char *bb_shmid_ptr = getenv("__AFL_BB_SHM_ID");
  if (bb_shmid_ptr && !__afl_bbmap_ptr) {
    int shmid = atoi(bb_shmid_ptr);
    __afl_bbmap_ptr = shmat(shmid, NULL, 0);
  }
}
