#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class LiftLogicalSubExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<LiftLogicalSubExpr> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitBinaryOperator(clang::BinaryOperator *BO);

private:
  std::vector<clang::BinaryOperator *> BinaryOps;
};

static RegisterMutator<LiftLogicalSubExpr> M("s.lift-logical-subexpr",
      "Lift a subexpression from a logical expression.");

bool LiftLogicalSubExpr::VisitBinaryOperator(BinaryOperator *BO) {
  if ((BO->getOpcode() == BO_LAnd || BO->getOpcode() == BO_LOr) &&
      isMutationSite(BO)) {
    BinaryOps.push_back(BO);
  }
  return true;
}

bool LiftLogicalSubExpr::mutate() {
  TraverseAST(getASTContext());
  if (BinaryOps.empty())
    return false;

  BinaryOperator *op = randElement(BinaryOps);
  Expr *lhs = op->getLHS();
  Expr *rhs = op->getRHS();

  // Randomly choose to lift the left or right operand
  Expr *lifted = randBool() ? lhs : rhs;

  // Replace the entire logical expression with the lifted operand
  getRewriter().ReplaceText(getExpansionRange(op->getSourceRange()),
                            getSourceText(lifted));

  return true;
}
