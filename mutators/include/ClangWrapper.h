#ifndef CLANG_WRAPPER_H
#define CLANG_WRAPPER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/Builtins.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/ToolChain.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Lex/Lexer.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Parse/ParseAST.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/VirtualFileSystem.h>

class ClangWrapper {
  bool useFileInput = true;
  std::string srctext;
  std::string filename;
  std::unique_ptr<clang::CompilerInstance> CI;
  std::unique_ptr<llvm::MemoryBuffer> bufferOwner;

public:
  std::string ErrorMsg;

  class SyntaxOnlyConsumer : public clang::ASTConsumer {
  public:
    SyntaxOnlyConsumer() = default;
  };

  ClangWrapper(ClangWrapper &&) = default;
  ClangWrapper &operator=(ClangWrapper &&) = default;

  ~ClangWrapper() {
    if (CI) CI->getDiagnosticClient().EndSourceFile();
  }

public:
  ClangWrapper(const std::string &filename,
      const std::string &srctext, bool useFileInput)
      : useFileInput(useFileInput), srctext(srctext),
        filename(filename) {
    using namespace clang;
    if (CI) {
      ErrorMsg = "CompilerInstance has been initialized!";
      return;
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

    // WIERTON: handle InputKind C/CXX/OpenCL
    InputKind IK =
        FrontendOptions::getInputKindForExtension(
            StringRef(filename).rsplit('.').second);
    if (IK.getLanguage() == Language::C) {
      Invocation.setLangDefaults(
          CI->getLangOpts(), Language::C, T, Includes);
    } else if (IK.getLanguage() == Language::CXX) {
      // ISSUE: it might cause some problems when building
      // AST for a function which has a non-declared callee,
      // e.g., It results an empty AST for the caller.
      Invocation.setLangDefaults(
          CI->getLangOpts(), Language::CXX, T, Includes);
    } else if (IK.getLanguage() == Language::OpenCL) {
      // Commandline parameters
      std::vector<const char *> Args;
      Args.push_back("-x");
      Args.push_back("cl");
      Args.push_back("-Dcl_clang_storage_class_specifiers");

      CI->createFileManager();

      Args.push_back("-fno-builtin");

      llvm::ArrayRef<const char *> ArgsRef(
          Args.data(), Args.size());
      CompilerInvocation::CreateFromArgs(
          Invocation, ArgsRef, CI->getDiagnostics());
      Invocation.setLangDefaults(
          CI->getLangOpts(), Language::OpenCL, T, Includes);
    } else {
      ErrorMsg = "Unsupported file type!";
      return;
    }

    Invocation.setLangDefaults(
        CI->getLangOpts(), IK.getLanguage(), T, Includes);

    TargetInfo *Target =
        TargetInfo::CreateTargetInfo(CI->getDiagnostics(),
            CI->getInvocation().TargetOpts);
    CI->setTarget(Target);

    // WIERTON: add system include headers
    HeaderSearchOptions &HeaderSearchOpts =
        CI->getHeaderSearchOpts();
    for (const std::string &inc : getSystemIncludes())
      HeaderSearchOpts.AddPath(
          inc, clang::frontend::System, false, true);

    CI->createFileManager();
    CI->createSourceManager(CI->getFileManager());
    CI->createPreprocessor(TU_Complete);

    DiagnosticConsumer &DgClient =
        CI->getDiagnosticClient();
    DgClient.BeginSourceFile(
        CI->getLangOpts(), &CI->getPreprocessor());
    CI->createASTContext();

    CI->setASTConsumer(
        std::make_unique<SyntaxOnlyConsumer>());
    Preprocessor &PP = CI->getPreprocessor();
    PP.getBuiltinInfo().initializeBuiltins(
        PP.getIdentifierTable(), PP.getLangOpts());

    // WIERTON: set input to memory buffer of file
    if (useFileInput) {
      if (!CI->InitializeSourceManager(
              FrontendInputFile(filename, IK)))
        return;
    } else {
      bufferOwner = llvm::MemoryBuffer::getMemBufferCopy(
          srctext, filename);
      llvm::MemoryBufferRef BufferRef =
          bufferOwner->getMemBufferRef();

      if (!CI->InitializeSourceManager(
              FrontendInputFile(BufferRef, IK)))
        return;
    }

    CI->createSema(TU_Complete, 0);
    DiagnosticsEngine &Diag = CI->getDiagnostics();
    Diag.setSuppressAllDiagnostics(true);
    Diag.setIgnoreAllWarnings(true);

    ParseAST(CI->getSema());
  }

  // WIERTON: this compute system includes via ToolChain
  std::vector<std::string> computeSystemIncludes() {
    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions>
        diagOpts = new clang::DiagnosticOptions();
    clang::DiagnosticsEngine diag_engine(
        llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(
            new clang::DiagnosticIDs()),
        diagOpts.get(),
        new clang::TextDiagnosticPrinter(
            llvm::nulls() /* WIERTON: this ostream controls
                             where to output diagnostic
                             messages */
            ,
            diagOpts.get()),
        true // ShouldOwnClient
    );

    clang::driver::Driver driver("clang",
        llvm::sys::getDefaultTargetTriple(), diag_engine);

    const char *args[] = {"clang", "-fsyntax-only"};
    unsigned numArgs = 2;

    std::unique_ptr<clang::driver::Compilation> C(
        driver.BuildCompilation(
            llvm::ArrayRef<const char *>(args, numArgs)));

    if (!C) return std::vector<std::string>{};

    const clang::driver::ToolChain &toolChain =
        C->getDefaultToolChain();

    // Use the arguments from the driver's compilation
    // instead of creating new ones
    const llvm::opt::ArgList &driverArgs = C->getArgs();

    // This list will store the system include paths
    llvm::opt::ArgStringList cc1Args;
    toolChain.AddClangSystemIncludeArgs(
        driverArgs, cc1Args);

    // Iterate over the system include directories and print
    // them
    std::vector<std::string> systemIncludes;
    for (int i = 0; i < cc1Args.size(); i++) {
      std::string arg = cc1Args[i];
      if (arg == "-internal-isystem" ||
          arg == "-internal-externc-isystem")
        systemIncludes.push_back(cc1Args[i + 1]);
    }
    return systemIncludes;
  }

  std::vector<std::string> getSystemIncludes() {
    static bool inited = false;
    static std::vector<std::string> systemIncludes;
    if (!inited) {
      inited = true;
      systemIncludes = computeSystemIncludes();
    }
    return systemIncludes;
  }

  clang::CompilerInstance &getCompilerInstance() {
    return *CI;
  }

  static ClangWrapper fromSourceFile(
      const std::string &filename) {
    return ClangWrapper(filename, "", true);
  }

  static ClangWrapper fromSourceText(
      const std::string &srctext,
      const std::string &filename) {
    return ClangWrapper(filename, srctext, false);
  }
};

#endif

