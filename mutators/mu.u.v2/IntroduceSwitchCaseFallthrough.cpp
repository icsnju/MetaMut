#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class IntroduceSwitchCaseFallthrough
    : public Mutator,
      public clang::RecursiveASTVisitor<IntroduceSwitchCaseFallthrough> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCases.empty()) return false;

    CaseStmt *caseStmt = randElement(TheCases);
    SourceLocation caseEnd = caseStmt->getSubStmt()->getEndLoc();

    getRewriter().InsertText(getLocForEndOfToken(caseEnd), ";\n/* fallthrough */");

    return true;
  }

  bool VisitCaseStmt(clang::CaseStmt *CS) {
    TheCases.push_back(CS);
    return true;
  }

private:
  std::vector<clang::CaseStmt *> TheCases;
};

static RegisterMutator<IntroduceSwitchCaseFallthrough> M(
    "u2.IntroduceSwitchCaseFallthrough", "It adds a fallthrough comment to a random case block within a switch statement, indicating the execution to continue into the subsequent case block.");
