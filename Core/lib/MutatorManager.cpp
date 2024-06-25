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

#include "MutatorManager.h"
#include "json.hpp"

using namespace ysmut;
using json = nlohmann::ordered_json;

class SyntaxOnlyConsumer : public clang::ASTConsumer {
public:
  SyntaxOnlyConsumer() = default;
};

MutatorManager::MutatorManager() {}
MutatorManager::~MutatorManager() {
  if (CI) CI->getDiagnosticClient().EndSourceFile();
}

void MutatorManager::srand(unsigned seed) {
  ::srand(seed);
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

MutatorManager *MutatorManager::getInstance() {
  static MutatorManager manager;
  return &manager;
}

void MutatorManager::registerMutator(
    const char *mutatorName, Mutator *mutatorImpl) {
  MutatorManager *manager = getInstance();
  manager->mutatorsMap[std::string(mutatorName)] = mutatorImpl;
  mutatorImpl->setManager(manager);
}

Mutator *MutatorManager::getMutator(std::string name) {
  Mutator *impl = getInstance()->mutatorsMap[name];
  assert(impl && "invalid name to lookup Mutator");
  return impl;
}

bool MutatorManager::hasMutator(std::string name) {
  const auto &map = getInstance()->mutatorsMap;
  return map.find(name) != map.end();
}

std::vector<std::string> MutatorManager::getAllMutators() {
  std::vector<std::string> mutators;
  for (auto &kvpair : mutatorsMap) mutators.push_back(kvpair.first);
  return mutators;
}

void MutatorManager::printMutators() {
  json j = json::array();
  for (auto &kvpair : mutatorsMap)
    j.push_back(json::array({kvpair.first, kvpair.second->getDescription()}));
  llvm::outs() << j.dump(2) << "\n";
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
  this->inputFile = filename;

  using namespace clang;
  if (CI) {
    ErrorMsg = "CompilerInstance has been initialized!";
    return false;
  }

  std::vector<std::string> Includes;
  CI.reset(new CompilerInstance());
  assert(CI);

  CI->createDiagnostics();

  TargetOptions &TargetOpts = CI->getTargetOpts();
  PreprocessorOptions &PPOpts = CI->getPreprocessorOpts();
  TargetOpts.Triple = LLVM_DEFAULT_TARGET_TRIPLE;
  llvm::Triple T(TargetOpts.Triple);
  CompilerInvocation &Invocation = CI->getInvocation();
  InputKind IK = FrontendOptions::getInputKindForExtension(
      StringRef(filename).rsplit('.').second);
  if (IK.getLanguage() == Language::C) {
    Invocation.setLangDefaults(CI->getLangOpts(), Language::C, T, Includes);
  } else if (IK.getLanguage() == Language::CXX) {
    // ISSUE: it might cause some problems when building AST
    // for a function which has a non-declared callee, e.g.,
    // It results an empty AST for the caller.
    Invocation.setLangDefaults(CI->getLangOpts(), Language::CXX, T, Includes);
  } else if (IK.getLanguage() == Language::OpenCL) {
    // Commandline parameters
    std::vector<const char *> Args;
    Args.push_back("-x");
    Args.push_back("cl");
    Args.push_back("-Dcl_clang_storage_class_specifiers");

    const char *CLCPath = getenv("CREDUCE_LIBCLC_INCLUDE_PATH");

    CI->createFileManager();

    if (CLCPath != NULL && CI->hasFileManager() &&
        bool(CI->getFileManager().getDirectory(CLCPath, false))) {
      Args.push_back("-I");
      Args.push_back(CLCPath);
    }

    Args.push_back("-include");
    Args.push_back("clc/clc.h");
    Args.push_back("-fno-builtin");

    llvm::ArrayRef<const char *> ArgsRef(Args.data(), Args.size());
    CompilerInvocation::CreateFromArgs(
        Invocation, ArgsRef, CI->getDiagnostics());
    Invocation.setLangDefaults(
        CI->getLangOpts(), Language::OpenCL, T, Includes);
  } else {
    ErrorMsg = "Unsupported file type!";
    return false;
  }

  TargetInfo *Target = TargetInfo::CreateTargetInfo(
      CI->getDiagnostics(), CI->getInvocation().TargetOpts);
  CI->setTarget(Target);

  if (const char *env = getenv("EXTRA_INCLUDE_PATH")) {
    HeaderSearchOptions &HeaderSearchOpts = CI->getHeaderSearchOpts();

    const std::size_t npos = std::string::npos;
    std::string text = env;

    std::size_t now = 0, next = 0;
    do {
      next = text.find(':', now);
      std::size_t len = (next == npos) ? npos : (next - now);
      HeaderSearchOpts.AddPath(
          text.substr(now, len), clang::frontend::Angled, false, false);
      now = next + 1;
    } while (next != npos);
  }

  CI->createFileManager();
  CI->createSourceManager(CI->getFileManager());
  CI->createPreprocessor(TU_Complete);

  DiagnosticConsumer &DgClient = CI->getDiagnosticClient();
  DgClient.BeginSourceFile(CI->getLangOpts(), &CI->getPreprocessor());
  CI->createASTContext();

  CI->setASTConsumer(std::make_unique<SyntaxOnlyConsumer>());
  Preprocessor &PP = CI->getPreprocessor();
  PP.getBuiltinInfo().initializeBuiltins(
      PP.getIdentifierTable(), PP.getLangOpts());

  if (!CI->InitializeSourceManager(FrontendInputFile(filename, IK))) {
    return false;
  }

  CI->createSema(TU_Complete, 0);
  DiagnosticsEngine &Diag = CI->getDiagnostics();
  Diag.setSuppressAllDiagnostics(true);
  Diag.setIgnoreAllWarnings(true);

  ParseAST(CI->getSema());

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
  assert(CI && "CompilerInstance not initialized");
  auto &SM = getASTContext().getSourceManager();
  auto &LO = getASTContext().getLangOpts();
  rewriter->setSourceMgr(SM, LO);
  bool mutRes = getMutator(mutatorName)->mutate();
  if (mutRes) getRewriter().getEditBuffer(SM.getMainFileID()).write(outs());
  return mutRes;
}
