#include <algorithm>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DuplicateWithBinaryOp
    : public Mutator,
      public clang::RecursiveASTVisitor<DuplicateWithBinaryOp> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitExpr(clang::Expr *E);

private:
  std::vector<clang::Expr *> TheExprs;
};

static RegisterMutator<DuplicateWithBinaryOp> M("s.duplicate-with-binop",
      "Duplicate an expression with a binary operator.");

bool DuplicateWithBinaryOp::VisitExpr(Expr *E) {
  if (isMutationSite(E) &&
      !E->IgnoreParenImpCasts()->getType()->isFunctionType())
    TheExprs.push_back(E);
  return true;
}

bool DuplicateWithBinaryOp::mutate() {
  TraverseAST(getASTContext());
  if (TheExprs.empty())
    return false;

  Expr *expr = randElement(TheExprs); // Choose an expression randomly

  // List of binary operators to try
  std::vector<BinaryOperatorKind> ops = {
      BO_Add, BO_Sub, BO_Mul, BO_Div, BO_Rem, BO_And, BO_Or, BO_LAnd, BO_LOr};

  // Randomly shuffle the operators
  std::shuffle(ops.begin(), ops.end(), getManager().getRandomGenerator());

  for (const auto &op : ops) {
    Expr *lhs = expr->IgnoreImpCasts();
    Expr *rhs = expr->IgnoreImpCasts();
    BinaryOperator *newExpr = BinaryOperator::Create(
        getASTContext(), lhs, rhs, op, lhs->getType(), VK_RValue, OK_Ordinary,
        expr->getExprLoc(), FPOptionsOverride());

    // Check if the new expression is valid
    if (getCompilerInstance()
            .getSema()
            .CorrectDelayedTyposInExpr(newExpr)
            .isInvalid()) {
      continue; // If not valid, try the next operator
    }

    std::string exprStr =
        getRewriter().getRewrittenText(expr->getSourceRange());
    std::string newExprStr = "((" + exprStr + ")" + " " +
                             newExpr->getOpcodeStr().str() + " " + "(" +
                             exprStr + "))";

    // If the new expression is valid, replace the old one
    getRewriter().ReplaceText(expr->getSourceRange(), newExprStr);
    return true;
  }

  // No suitable binary operator found
  return false;
}
