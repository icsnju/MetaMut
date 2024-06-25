#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class MutateUnaryExprOpcode
    : public Mutator,
      public clang::RecursiveASTVisitor<MutateUnaryExprOpcode> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitUnaryOperator(clang::UnaryOperator *UO);

private:
  std::vector<clang::UnaryOperator *> TheOperators;

  std::vector<clang::UnaryOperatorKind>
  computeValidAlternatives(clang::UnaryOperator *op);
};

static RegisterMutator<MutateUnaryExprOpcode> M("s.mutate-unop", "Change unary expression's operator.");

bool MutateUnaryExprOpcode::VisitUnaryOperator(UnaryOperator *UO) {
  if (isMutationSite(UO) && UO->getOpcode() != UO_PostInc &&
      UO->getOpcode() != UO_PostDec)
    TheOperators.push_back(UO);
  return true;
}

bool MutateUnaryExprOpcode::mutate() {
  TraverseAST(getASTContext());
  if (TheOperators.empty())
    return false;

  UnaryOperator *expr = randElement(TheOperators);

  // perform rewrite
  auto newOps = computeValidAlternatives(expr);
  if (newOps.size() == 0)
    return false;
  UnaryOperatorKind newOpKind = newOps[randIndex(newOps.size())];
  SourceLocation OpLoc = expr->getOperatorLoc();

  std::string NewOp = UnaryOperator::getOpcodeStr(newOpKind).str();
  getRewriter().ReplaceText(OpLoc, expr->getOpcodeStr(expr->getOpcode()).size(),
                            NewOp);

  return true;
}

std::vector<UnaryOperatorKind>
MutateUnaryExprOpcode::computeValidAlternatives(UnaryOperator *op) {
  std::vector<UnaryOperatorKind> validAlternatives;
  Expr *subExpr = op->getSubExpr();
  SourceLocation loc = op->getExprLoc();
  auto &sema = getCompilerInstance().getSema();
  for (int i = UO_AddrOf; i <= UO_LNot; ++i) {
    UnaryOperatorKind kind = static_cast<UnaryOperatorKind>(i);
    ExprResult result = sema.CreateBuiltinUnaryOp(loc, kind, subExpr);
    if (!result.isInvalid() && op->getOpcode() != i &&
        result.get()->isLValue() == op->isLValue())
      validAlternatives.push_back(kind);
  }
  return validAlternatives;
}
