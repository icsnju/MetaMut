#ifndef YSMUT_MUTATORMANAGER_H
#define YSMUT_MUTATORMANAGER_H

#include <random>
#include <cassert>
#include <map>
#include <string>

#include <llvm/Support/raw_ostream.h>

#include "Mutator.h"

namespace ysmut {

class Mutator;

class MutatorManager {
public:
  static MutatorManager *getInstance();

  void srand(unsigned seed);
  int randint(int begin, int end);
  float randreal(float begin, float end);
  std::mt19937 &getRandomGenerator() { return *rndgen; }

  static void registerMutator(const char *mutatorName, Mutator *mutatorImpl);

  void printMutators();
  Mutator *getMutator(std::string name);
  bool hasMutator(std::string name);
  std::vector<std::string> getAllMutators();

  void setOutputFile(std::string filename);
  void setOutStream(llvm::raw_ostream &os);
  bool setSourceFile(std::string filename);
  void setMutationRange(MutationRange range);
  void setMutator(std::string mutator);

  bool mutate();

  clang::Rewriter &getRewriter() { return *rewriter; }
  clang::ASTContext &getASTContext() const {
    return getCompilerInstance().getASTContext();
  }
  clang::CompilerInstance &getCompilerInstance() const {
    assert(CI);
    return *CI;
  }

  MutationRange getMutationRange() const { return mutationRange; }

  llvm::raw_ostream &outs();

private:
  MutatorManager();
  ~MutatorManager();

  std::string ErrorMsg;

  unsigned seed = 0;
  std::unique_ptr<std::mt19937> rndgen;
  std::string mutatorName;
  std::string inputFile;
  MutationRange mutationRange;
  std::unique_ptr<llvm::raw_ostream> outStreamOwner;
  llvm::raw_ostream *outStream = nullptr;

  std::unique_ptr<clang::CompilerInstance> CI;
  std::unique_ptr<clang::Rewriter> rewriter;

  /* transformation instances */
  std::map<std::string, Mutator *> mutatorsMap;
};

template <typename MutatorClass>
class RegisterMutator {
public:
  RegisterMutator(const char *name, const char *desc) {
    Mutator *impl = new MutatorClass(name, desc);
    assert(impl && "Fail to create MutatorClass");
    MutatorManager::registerMutator(name, impl);
  }
};

} // namespace ysmut

#endif
