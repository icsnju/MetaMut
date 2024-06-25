#ifndef LAYMUT_SEMANTICMUTATORMANAGER_H
#define LAYMUT_SEMANTICMUTATORMANAGER_H

#include <cassert>
#include <map>
#include <random>
#include <string>

#include <llvm/Support/raw_ostream.h>

#include "ClangWrapper.h"
#include "Mutator.h"

namespace llvm {
class MemoryBuffer;
};

class Mutator;

struct MutatorInfo {
  const char *name;
  const char *desc;

  using CtorTy =
      std::function<std::unique_ptr<Mutator>(
          const char *, const char *)>;
  CtorTy ctor;

  std::unique_ptr<Mutator> create() {
    return ctor(name, desc);
  }
};

class MutatorManager {
public:
  static MutatorManager *getInstance();

  void srand(unsigned seed);
  int randint(int begin, int end);
  float randreal(float begin, float end);
  std::mt19937 &getRandomGenerator() { return *rndgen; }

  void printMutators();
  Mutator *createMutator(std::string name);
  bool hasMutator(std::string name);

  void setOutputFile(std::string filename);
  void setOutStream(llvm::raw_ostream &os);
  bool setSourceFile(std::string filename);
  bool setSourceText(const std::string &sourceText,
      const std::string &filename = "input.c");
  void setMutationRange(MutationRange range);
  void setMutator(std::string mutator);

  bool mutate();

  clang::Rewriter &getRewriter() { return *rewriter; }
  clang::ASTContext &getASTContext() const {
    return getCompilerInstance().getASTContext();
  }
  clang::CompilerInstance &getCompilerInstance() const;

  MutationRange getMutationRange() const {
    return mutationRange;
  }

  llvm::raw_ostream &outs();

  MutatorManager() = default;

private:
  std::string ErrorMsg;

  unsigned seed = 0;
  std::unique_ptr<std::mt19937> rndgen;
  std::string mutatorName;
  std::string inputFile;
  MutationRange mutationRange;
  std::unique_ptr<llvm::raw_ostream> outStreamOwner;
  llvm::raw_ostream *outStream = nullptr;

  std::unique_ptr<Mutator> currentMutator;
  std::unique_ptr<clang::Rewriter> rewriter;
  std::unique_ptr<ClangWrapper> CW;

public:
  /* transformation instances */
  static std::map<std::string, MutatorInfo> &
  getMutatorsMap() {
    static std::map<std::string, MutatorInfo>
        mutatorsMap;
    return mutatorsMap;
  }
  static std::vector<std::string> getAllMutators();
};

template <typename MutatorClass>
class RegisterMutator {
public:
  static std::unique_ptr<Mutator> Ctor(
      const char *name, const char *desc) {
    return std::unique_ptr<Mutator>(
        new MutatorClass(name, desc));
  }

  RegisterMutator(
      const char *name, const char *desc) {
    auto &mutatorsMap =
        MutatorManager::getMutatorsMap();
    mutatorsMap[std::string(name)] =
        MutatorInfo{name, desc, Ctor};
  }
};

#endif
