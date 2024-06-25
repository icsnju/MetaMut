#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeBinop
    : public Mutator,
      public clang::RecursiveASTVisitor<ChangeBinop> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    // perform rewrite
    auto newOps = computeValidAlternatives(expr);
    BinaryOperatorKind newOpKind = newOps[randIndex(newOps.size())];
    SourceLocation OpLoc = expr->getOperatorLoc();
    std::string NewOp = BinaryOperator::getOpcodeStr(newOpKind).str();
    getRewriter().ReplaceText(
        OpLoc, expr->getOpcodeStr(expr->getOpcode()).size(), NewOp);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    TheOperators.push_back(BO);
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;

  std::vector<clang::BinaryOperatorKind> computeValidAlternatives(
      clang::BinaryOperator *op) {
    std::vector<BinaryOperatorKind> validAlternatives;

    Expr *lhs = op->getLHS();
    Expr *rhs = op->getRHS();
    SourceLocation loc = op->getExprLoc();

    auto &sema = getCompilerInstance().getSema();
    for (int i = BO_PtrMemD; i <= BO_Comma; ++i) {
      if (i == BO_PtrMemD || i == BO_PtrMemI || i == BO_Cmp ||
          i == op->getOpcode())
        continue;
      BinaryOperatorKind kind = static_cast<BinaryOperatorKind>(i);
      ExprResult result = sema.CreateBuiltinBinOp(loc, kind, lhs, rhs);

      if (!result.isInvalid()) validAlternatives.push_back(kind);
    }

    return validAlternatives;
  }
};

static RegisterMutator<ChangeBinop> M(
    "ChangeBinop", "Change binary expression's operator.");
