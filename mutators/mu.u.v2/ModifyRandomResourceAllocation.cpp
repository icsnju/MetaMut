#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyRandomResourceAllocation
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyRandomResourceAllocation> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheAllocations.empty()) return false;

    CStyleCastExpr *expr = randElement(TheAllocations);
    Expr *origSizeExpr = expr->getSubExprAsWritten();
    SourceLocation startLoc = origSizeExpr->getBeginLoc();
    SourceLocation endLoc = origSizeExpr->getEndLoc();

    std::string currentSize = getSourceText(origSizeExpr);
    std::string newSize = std::to_string(rand() % 128);

    getRewriter().ReplaceText(SourceRange(startLoc, endLoc), newSize);

    return true;
  }

  bool VisitCStyleCastExpr(clang::CStyleCastExpr *CCE) {
    if (CCE->getCastKind() == CK_BitCast &&
        isa<CallExpr>(CCE->getSubExpr()) &&
        isa<FunctionDecl>(
            cast<CallExpr>(CCE->getSubExpr())->getCalleeDecl())) {
      FunctionDecl *callee =
          cast<FunctionDecl>(cast<CallExpr>(CCE->getSubExpr())->getCalleeDecl());
      if (callee->getNameAsString() == "malloc") {
        TheAllocations.push_back(CCE);
      }
    }
    return true;
  }

private:
  std::vector<clang::CStyleCastExpr *> TheAllocations;
};

static RegisterMutator<ModifyRandomResourceAllocation> M(
    "u2.ModifyRandomResourceAllocation",
    "It takes a resource allocation statement and modifies the allocated size "
    "or count, potentially changing the amount of memory or resources "
    "allocated.");
