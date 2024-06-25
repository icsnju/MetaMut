#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

class RemoveUnaryExprOpcode
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveUnaryExprOpcode> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitUnaryOperator(clang::UnaryOperator *UO);

private:
  std::vector<const clang::UnaryOperator *> TheOperators;
};

static RegisterMutator<RemoveUnaryExprOpcode> M("s.remove-unop", "Remove unary expression's operator.");

bool RemoveUnaryExprOpcode::VisitUnaryOperator(clang::UnaryOperator *UO) {
  if (isMutationSite(UO))
    TheOperators.push_back(clang::dyn_cast<clang::UnaryOperator>(UO));
  return true;
}

bool RemoveUnaryExprOpcode::mutate() {
  TraverseAST(getASTContext());
  if (TheOperators.empty())
    return false;

  const clang::UnaryOperator *expr = randElement(TheOperators);
  clang::SourceLocation OpLoc = expr->getOperatorLoc();
  getRewriter().RemoveText(OpLoc, expr->getOpcodeStr(expr->getOpcode()).size());
  return true;
}
