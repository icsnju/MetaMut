#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class FlipLogicalOperator : public Mutator,
                            public clang::RecursiveASTVisitor<FlipLogicalOperator> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheExprs.empty()) return false;

    BinaryOperator *expr = randElement(TheExprs);
    if (expr->getOpcode() == BO_LAnd) {
      // Flip && operator to ||
      SourceRange range(expr->getOperatorLoc(), expr->getOperatorLoc().getLocWithOffset(1));
      getRewriter().ReplaceText(range, "||");
    } else if (expr->getOpcode() == BO_LOr) {
      // Flip || operator to &&
      SourceRange range(expr->getOperatorLoc(), expr->getOperatorLoc().getLocWithOffset(1));
      getRewriter().ReplaceText(range, "&&");
    }

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->isLogicalOp())
      TheExprs.push_back(BO);
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheExprs;
};

static RegisterMutator<FlipLogicalOperator> M("u1.FlipLogicalOperator", "Flip the logical operator in a LogicalExpr.");