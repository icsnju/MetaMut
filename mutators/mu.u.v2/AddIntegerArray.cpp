#include "Mutator.h"
#include "MutatorManager.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>

using namespace clang;

class AddIntegerArray : public Mutator,
                        public clang::RecursiveASTVisitor<AddIntegerArray> {
public:
  using Mutator::Mutator;

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->getNameAsString() == "main") {
      TheMain = FD;
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (!TheMain) return false;

    std::string arrayDecl = "int new_array[3] = {1, 2, 3};\n";
    SourceLocation insertLoc = getMostRecentTranslationUnitDecl(TheMain)->getBeginLoc();
    getRewriter().InsertTextBefore(insertLoc, arrayDecl);

    return true;
  }

private:
  clang::FunctionDecl *TheMain = nullptr;
};

static RegisterMutator<AddIntegerArray> M(
    "u2.AddIntegerArray", "Adds an integer array to the main function in the input program.");
