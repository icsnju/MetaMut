#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleStaticFunction : public Mutator,
                             public clang::RecursiveASTVisitor<ToggleStaticFunction> {
public:
  using Mutator::Mutator;

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->isDefined()) {
      TheFunctions.push_back(FD);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);

    bool isStatic = func->getStorageClass() == SC_Static;

    if (!isStatic && func->isUsed() && !func->isInAnonymousNamespace()) {
      return false;
    }

    if (isStatic) {
      getRewriter().RemoveText(func->getInnerLocStart(), strlen("static "));
    } else {
      SourceLocation beforeLoc = getMostRecentTranslationUnitDecl(func)->getBeginLoc();
      getRewriter().InsertTextBefore(beforeLoc, "static ");
    }

    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
};

static RegisterMutator<ToggleStaticFunction> M(
    "u2.ToggleStaticFunction", "Toggle static keyword for a function declaration.");
