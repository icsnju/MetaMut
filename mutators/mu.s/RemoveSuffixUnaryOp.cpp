#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveSuffixUnaryOp
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveSuffixUnaryOp> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitUnaryOperator(clang::UnaryOperator *UO);

private:
  std::vector<clang::UnaryOperator *> TheUnaryOps;
};

static RegisterMutator<RemoveSuffixUnaryOp> M("s.remove-suffix-unaryop", "Remove a UnaryOperator's suffix operator.");

bool RemoveSuffixUnaryOp::VisitUnaryOperator(UnaryOperator *UO) {
  if (UO->isPostfix() && isMutationSite(UO))
    TheUnaryOps.push_back(UO);
  return true;
}

bool RemoveSuffixUnaryOp::mutate() {
  TraverseAST(getASTContext());
  if (TheUnaryOps.empty())
    return false;

  UnaryOperator *unaryOp = randElement(TheUnaryOps);
  UnaryOperatorKind oldOp = unaryOp->getOpcode();

  // Only handle postfix increment and decrement
  if (oldOp != UO_PostInc && oldOp != UO_PostDec)
    return false;

  // Remove the operator
  getRewriter().ReplaceText(unaryOp->getSourceRange(),
                            getSourceText(unaryOp->getSubExpr()));

  return true;
}
