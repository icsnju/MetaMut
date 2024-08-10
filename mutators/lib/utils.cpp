#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <random>
#include <semaphore.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#include "fmt/core.h"
#include "fmt/fmtlog.h"
#include "utils.h"

void disable_core_dump() {
  struct rlimit limit = {.rlim_cur = 0, .rlim_max = 0};
  fmtlog_assert(setrlimit(RLIMIT_CORE, &limit) == 0);
}

std::string get_timestamp() {
  struct timeval tv;
  struct tm *timeinfo;
  char buffer[128] = {0};

  // Get the current time including microseconds
  gettimeofday(&tv, NULL);
  timeinfo = localtime(&tv.tv_sec);

  // Format the time, up to seconds
  strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S", timeinfo);

  // Calculate milliseconds from microseconds
  int milliseconds = tv.tv_usec / 1000;

  // Create the final timestamp string with milliseconds
  char finalTimestamp[256] = {0};
  snprintf(
      finalTimestamp, sizeof(finalTimestamp), "%s.%03d]", buffer, milliseconds);

  // Return the timestamp string
  return std::string(finalTimestamp);
}

std::mt19937 &get_rndgen() {
  // auto *manager = SemanticMutatorManager::getInstance();
  // return manager->getRandomGenerator();
  static std::mt19937 rndgen(time(NULL) ^ getpid());
  return rndgen;
}

int weighted_sample(const std::vector<unsigned> &weights) {
  std::discrete_distribution<> dist(weights.begin(), weights.end());
  int index = dist(get_rndgen());
  return index;
}

void set_memory_limit(uint64_t limit) {
  struct rlimit rl;
  fmtlog_assert(getrlimit(RLIMIT_AS, &rl) == 0);
  static_assert(sizeof(rl.rlim_cur) >= sizeof(1ull));
  rl.rlim_cur = limit;
  fmtlog_assert(setrlimit(RLIMIT_AS, &rl) == 0);
}

void clean_semaphore(const char *name, sem_t *sem) {
  if (sem_close(sem) == -1) { logi("Failed to close semaphore {}\n", name); }

  if (sem_unlink(name) == -1) { logi("Failed to unlink semaphore {}\n", name); }
}

sem_t *reset_semaphore(const char *name, int init_value) {
  sem_t *sem = sem_open(name, O_CREAT | O_EXCL, 0644, init_value);
  if (sem == SEM_FAILED) {
    if (errno == EEXIST) {
      // Semaphore already exists, so unlink and try to
      // create again
      logi(
          "Semaphore {} already exists. Unlinking and "
          "recreating...",
          name);

      if (sem_unlink(name) == -1) {
        logi("Failed to unlink existing semaphore {}", name);
        return NULL;
      }

      sem = sem_open(name, O_CREAT | O_EXCL, 0644, init_value);
      if (sem == SEM_FAILED) {
        logi(
            "Failed to create semaphore {} after "
            "unlinking",
            name);
        return NULL;
      }
    } else {
      logi("Failed to create semaphore {}", name);
      return NULL;
    }
  }

  return sem;
}

bool sem_timedwait_milliseconds(sem_t *sem, unsigned milliseconds) {
  struct timespec ts;

  // Get the current time
  if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
    perror("clock_gettime failed");
    return false;
  }

  // Calculate time in seconds and nanoseconds for the
  // specified milliseconds
  long secs = milliseconds / 1000;
  long nanosecs = (milliseconds % 1000) * 1000000L;

  // Add the time to the current time
  ts.tv_sec += secs;
  ts.tv_nsec += nanosecs;

  // Handle the case where nanoseconds overflow
  if (ts.tv_nsec >= 1000000000L) {
    ts.tv_sec += ts.tv_nsec / 1000000000L;
    ts.tv_nsec %= 1000000000L;
  }

  // Wait on the semaphore with a timeout
  while (sem_timedwait(sem, &ts) == -1) {
    if (errno == EINTR) {
      continue;
    } else if (errno == ETIMEDOUT) {
      return false; // wait failed
    } else {
      return false; // wait failed
    }
  }
  return true;
}
