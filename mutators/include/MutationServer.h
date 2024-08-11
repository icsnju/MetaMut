#ifndef LAYMUT_MUTATORFORKSERVER_H
#define LAYMUT_MUTATORFORKSERVER_H
#pragma once

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
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Mutator.h"
#include "MutatorManager.h"
#include "backward.h"
#include "fmt/fmtlog.h"
#include "utils.h"

#include <llvm/Support/CommandLine.h>

namespace opt {
extern llvm::cl::opt<bool> verbose;
}

struct shared_data_t {
  bool is_crash;

  int task_req_mop;
  uint64_t task_req_seed;

  bool task_res_success;
  char task_res_mop[64];

  unsigned osrc_sz;
  char osrc[];
};

struct shared_isrc_t {
  unsigned isrc_sz;
  char isrc[];
};

enum class MutationError {
  Success,
  Timeout,
  Crash,
  Fail,
};

const char *formatMutationError(MutationError err) {
  switch (err) {
  case MutationError::Success: return "success";
  case MutationError::Timeout: return "timeout";
  case MutationError::Fail: return "fail";
  }
  return "<null>";
}

class raw_buffer_ostream : public llvm::raw_ostream {
  char *buffer;
  int bufsize;
  int pos;

public:
  void write_impl(const char *ptr, size_t size) override {
    if (pos < bufsize) {
      if (pos + size <= bufsize)
        memcpy(&buffer[pos], ptr, size);
      else
        memcpy(&buffer[pos], ptr, bufsize - pos);
    }
    pos += size;
  }

  uint64_t current_pos() const override { return pos; }

  uint64_t get_used_buffer_size() const {
    return pos > bufsize ? bufsize : pos;
  }

public:
  explicit raw_buffer_ostream(char *buffer, unsigned bufsize)
      : buffer(buffer), bufsize(bufsize), pos(0) {
    SetUnbuffered();
  }
};

class MutationServer {
  char task_req_name[128] = {0};
  char task_res_name[128] = {0};
  sem_t *task_req = nullptr;
  sem_t *task_res = nullptr;
  shared_data_t *shdata = nullptr;
  shared_isrc_t *shisrc = nullptr;

  int client_pid = -1;
  static constexpr unsigned shm_size = 1 << 20;

  std::vector<std::string> mutators;

  static MutationServer *&getMutationInstance() {
    static MutationServer *inst;
    return inst;
  }

public:
  MutationServer(std::vector<std::string> mutators)
      : mutators(std::move(mutators)) {
    shdata = (shared_data_t *)mmap(NULL, shm_size, PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    fmtlog_assert(shdata, "failed to create shared memory");

    shisrc = (shared_isrc_t *)mmap(NULL, shm_size, PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    fmtlog_assert(shisrc, "failed to create shared memory");

    reset_mutation_server();
  }

  ~MutationServer() {
    if (task_req) clean_semaphore(task_req_name, task_req);
    if (task_res) clean_semaphore(task_res_name, task_res);
  }

  void reset_mutation_server() {
    if (task_req) clean_semaphore(task_req_name, task_req);
    if (task_res) clean_semaphore(task_res_name, task_res);

    snprintf(task_req_name, sizeof(task_req_name) - 1, "/semaafl.task_req.%ld",
        get_rndgen()());
    snprintf(task_res_name, sizeof(task_res_name) - 1, "/semaafl.task_res.%ld",
        get_rndgen()());
    logi("resetting mutation server req:{} and res:{}", task_req_name,
        task_res_name);

    task_req = reset_semaphore(task_req_name, 0);
    task_res = reset_semaphore(task_res_name, 0);

    kill_client();
    start_client();
  }

  void start_client() {
    int pid = fork();
    if (pid == 0) {
      client();
    } else {
      client_pid = pid;
    }
  }

  void kill_client() {
    if (client_pid > 0) kill(client_pid, SIGKILL);

    int status;
    pid_t pid;

    // Call waitpid with WNOHANG until no more dead children
    // are found
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      logi("kill childs, {} terminated", pid);
    }
  }

  void setup_isrc(llvm::StringRef isrc) {
    auto sz = std::min<unsigned>(isrc.size(), shm_size - sizeof(shared_isrc_t));
    memcpy(shisrc->isrc, isrc.data(), sz);
    shisrc->isrc_sz = sz;
  }

  static void signalHandler(int signo, siginfo_t *info, void *_ctx) {
    logi("Crash, inst={}", (void *)getMutationInstance());
    // FIXME: inform the parent here is a crash
    if (getMutationInstance()) {
      if (getMutationInstance()->task_res)
        sem_post(getMutationInstance()->task_res);
      getMutationInstance()->shdata->is_crash = true;
    }
    if (opt::verbose) backward::SignalHandling::handleSignal(signo, info, _ctx);
    _exit(signo);
  }

  void client() {
    MutatorManager manager;
    getMutationInstance() = this;
    if (!opt::verbose) close(2);

    struct sigaction sa = {0};
    sa.sa_flags =
        static_cast<int>(SA_SIGINFO | SA_ONSTACK | SA_NODEFER | SA_RESETHAND);
    sa.sa_sigaction = &signalHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // Handle segmentation fault and abort signals
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);

    while (true) {
      sem_wait(task_req);
      manager.srand(shdata->task_req_seed);

      raw_buffer_ostream oss(shdata->osrc, shm_size - sizeof(shared_data_t));

      manager.setSourceText(std::string(shisrc->isrc, shisrc->isrc_sz));
      manager.setOutStream(oss);
      manager.setMutator(mutators[shdata->task_req_mop]);
      bool success = manager.mutate();
      oss.flush();
      shdata->osrc_sz = oss.get_used_buffer_size();
      shdata->task_res_success = success;
      sem_post(task_res);
    }
  }

  bool wait_mutation(sem_t *sem, unsigned milliseconds) {
    uint64_t begin = getTimeInMS();

    while (sem_trywait(sem) != 0) {
      int status = -1;
      waitpid(client_pid, &status, WNOHANG);
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
        logi("Child {} is terminated", client_pid);
        return false;
      }

      if (getTimeInMS() > begin + milliseconds) {
        logi("Wait timeout {} vs {}", begin, getTimeInMS());
        return false;
      }
    }
    return true;
  }

  std::pair<MutationError, llvm::StringRef> apply_mutation(
      int mop, unsigned seed) {
    shdata->is_crash = false;
    shdata->task_req_seed = seed;
    shdata->task_req_mop = mop;
    shdata->task_res_success = false;
    sem_post(task_req);
    if (wait_mutation(task_res, 1000)) {
      if (shdata->is_crash) {
        logi("Crash with {}", mutators[mop]);
        return std::pair<MutationError, llvm::StringRef>{
            MutationError::Crash, llvm::StringRef()};
      } else if (shdata->task_res_success) {
        logi("Succeeds for {}", mutators[mop]);
        return std::pair<MutationError, llvm::StringRef>{MutationError::Success,
            llvm::StringRef(shdata->osrc, shdata->osrc_sz)};
      } else {
        logi("Fails for {}", mutators[mop]);
        return std::pair<MutationError, llvm::StringRef>{MutationError::Fail,
            llvm::StringRef(shdata->osrc, shdata->osrc_sz)};
      }
    }

    // fails
    reset_mutation_server();
    assert (!shdata->is_crash);
    return std::pair<MutationError, llvm::StringRef>{
        MutationError::Timeout, llvm::StringRef()};
  }
};

#endif
