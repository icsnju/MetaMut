#include <algorithm>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DuplicateWithUnaryOp
    : public Mutator,
      public clang::RecursiveASTVisitor<DuplicateWithUnaryOp> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitExpr(clang::Expr *E);

private:
  std::vector<clang::Expr *> TheExprs;
};

static RegisterMutator<DuplicateWithUnaryOp> M("s.duplicate-with-unop", "Duplicate an expression with a unary operator.");

bool DuplicateWithUnaryOp::VisitExpr(Expr *E) {
  if (isMutationSite(E) &&
      !E->IgnoreParenImpCasts()->getType()->isFunctionType()) {
    TheExprs.push_back(E);
  }
  return true;
}

bool DuplicateWithUnaryOp::mutate() {
  TraverseAST(getASTContext());
  if (TheExprs.empty())
    return false;

  Expr *expr = randElement(TheExprs); // Choose an expression randomly

  // List of unary operators to try
  std::vector<UnaryOperatorKind> ops = {UO_Plus, UO_Minus, UO_Not, UO_LNot};

  // Randomly shuffle the operators
  std::shuffle(ops.begin(), ops.end(), getManager().getRandomGenerator());

  for (const auto &op : ops) {
    Expr *operand = expr->IgnoreImpCasts();
    UnaryOperator *newExpr = UnaryOperator::Create(
        getASTContext(), operand, op, operand->getType(), VK_RValue,
        OK_Ordinary, expr->getExprLoc(), false, FPOptionsOverride{});

    // Check if the new expression is valid
    if (getCompilerInstance()
            .getSema()
            .CorrectDelayedTyposInExpr(newExpr)
            .isInvalid()) {
      continue; // If not valid, try the next operator
    }

    // If the new expression is valid, replace the old one
    std::string exprStr =
        getRewriter().getRewrittenText(expr->getSourceRange());
    std::string newExprStr =
        "(" + UnaryOperator::getOpcodeStr(op).str() + " " + exprStr + ")";
    getRewriter().ReplaceText(expr->getSourceRange(), newExprStr);
    return true;
  }

  // No suitable unary operator found
  return false;
}
