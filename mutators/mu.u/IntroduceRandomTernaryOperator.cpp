#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class IntroduceRandomTernaryOperator : public Mutator,
                                       public clang::RecursiveASTVisitor<IntroduceRandomTernaryOperator>{

  using VisitorTy = clang::RecursiveASTVisitor<IntroduceRandomTernaryOperator>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    // perform rewrite
    std::string newExpr = "((" + getSourceText(expr->getLHS()) + ") ? "
                          + getSourceText(expr->getRHS()) + " : "
                          + getSourceText(expr->getLHS()) + ")";
    getRewriter().ReplaceText(
        expr->getSourceRange(), newExpr);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->isComparisonOp() || BO->isLogicalOp()) {
      TheOperators.push_back(BO);
    }
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;
};

static RegisterMutator<IntroduceRandomTernaryOperator> M("u1.IntroduceRandomTernaryOperator",
    "Randomly selects a BinaryOperator in a Boolean condition and converts it into a ternary operator using an equivalent logical expression.");