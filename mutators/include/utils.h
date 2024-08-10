#ifndef LAYMUT_UTILS_H
#define LAYMUT_UTILS_H
#pragma once

#include "fmt/core.h"

#include <random>
#include <semaphore.h>
#include <string>

template <class... Args>
int shexec(Args &&...args) {
  std::string s = fmt::format(std::forward<Args>(args)...);
  return system(s.c_str());
}

void disable_core_dump();
void set_memory_limit(uint64_t limit = 1ull << 32);

std::string get_timestamp();
int weighted_sample(const std::vector<unsigned> &weights);

std::mt19937 &get_rndgen();

inline uint64_t getTimeInMS() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

void clean_semaphore(const char *name, sem_t *sem);
sem_t *reset_semaphore(const char *name, int init_value);
bool sem_timedwait_milliseconds(
    sem_t *sem, unsigned milliseconds);

#endif
