#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/Builtins.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Parse/ParseAST.h>

#include <random>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "ClangWrapper.h"
#include "MutatorManager.h"
#include "json.hpp"

using json = nlohmann::ordered_json;

class SyntaxOnlyConsumer : public clang::ASTConsumer {
public:
  SyntaxOnlyConsumer() = default;
};

MutatorManager *MutatorManager::getInstance() {
  static MutatorManager manager;
  return &manager;
}

void MutatorManager::srand(unsigned seed) {
  rndgen = std::make_unique<std::mt19937>(seed);
}

int MutatorManager::randint(int begin, int end) {
  std::uniform_int_distribution<int> distrib(begin, end);
  int ret = distrib(*rndgen);
  return ret;
}

float MutatorManager::randreal(float begin, float end) {
  std::uniform_real_distribution<float> distrib(begin, end);
  float ret = distrib(*rndgen);
  return ret;
}

Mutator *MutatorManager::createMutator(std::string name) {
  auto &info = getMutatorsMap()[name];
  currentMutator = info.create();
  assert(currentMutator && "invalid name to lookup Mutator");
  currentMutator->setManager(this);
  return &*currentMutator;
}

bool MutatorManager::hasMutator(std::string name) {
  const auto &map = getMutatorsMap();
  return map.find(name) != map.end();
}

std::vector<std::string> MutatorManager::getAllMutators() {
  std::vector<std::string> mutators;
  for (auto &kvpair : getMutatorsMap())
    mutators.push_back(kvpair.first);
  return mutators;
}

void MutatorManager::printMutators() {
  json j = json::array();
  for (auto &kvpair : getMutatorsMap())
    j.push_back(kvpair.first);
  llvm::outs() << j.dump(2) << "\n";
}

clang::CompilerInstance &MutatorManager::getCompilerInstance() const {
  assert(CW && CW->ErrorMsg.size() == 0);
  return CW->getCompilerInstance();
}

void MutatorManager::setOutputFile(std::string filename) {
  if (filename == "-") {
    outStream = &llvm::outs();
  } else {
    std::error_code ec;
    outStreamOwner.reset(new llvm::raw_fd_ostream(filename, ec));
    outStream = &*outStreamOwner;
  }
}

void MutatorManager::setOutStream(llvm::raw_ostream &os) { outStream = &os; }

llvm::raw_ostream &MutatorManager::outs() { return *outStream; }

bool MutatorManager::setSourceFile(std::string filename) {
  CW.reset(new ClangWrapper(ClangWrapper::fromSourceFile(filename)));
  return true;
}

bool MutatorManager::setSourceText(const std::string &sourceText,
                                   const std::string &fileName) {
  CW.reset(
      new ClangWrapper(ClangWrapper::fromSourceText(sourceText, fileName)));
  return true;
}

void MutatorManager::setMutationRange(MutationRange range) {
  this->mutationRange = range;
}

void MutatorManager::setMutator(std::string mutator) {
  this->mutatorName = mutator;
}

bool MutatorManager::mutate() {
  rewriter.reset(new clang::Rewriter);
  assert(outStream && "output file not initialized!");
  auto &SM = getASTContext().getSourceManager();
  auto &LO = getASTContext().getLangOpts();
  rewriter->setSourceMgr(SM, LO);
  auto *mutator = createMutator(mutatorName);
  bool mutRes = mutator->mutate();
  if (mutRes)
    mutator->post(outs());
  return mutRes;
}
