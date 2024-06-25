// SwapRandomBinaryOperands.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapRandomBinaryOperands
    : public Mutator,
      public clang::RecursiveASTVisitor<SwapRandomBinaryOperands> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator* expr = randElement(TheOperators);

    // perform rewrite
    Expr* lhs = expr->getLHS();
    Expr* rhs = expr->getRHS();

    std::string lhsStr = getSourceText(lhs);
    std::string rhsStr = getSourceText(rhs);

    SourceRange lhsRange = getExpansionRange(lhs->getSourceRange());
    SourceRange rhsRange = getExpansionRange(rhs->getSourceRange());

    getRewriter().ReplaceText(lhsRange, rhsStr);
    getRewriter().ReplaceText(rhsRange, lhsStr);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator* BO) {
    TheOperators.push_back(BO);
    return true;
  }

private:
  std::vector<clang::BinaryOperator*> TheOperators;
};

static RegisterMutator<SwapRandomBinaryOperands> M("u1.SwapRandomBinaryOperands", "Swap operands of a random BinaryOperator's expression.");