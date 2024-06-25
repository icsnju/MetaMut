#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class MutateSuffixUnaryOp
    : public Mutator,
      public clang::RecursiveASTVisitor<MutateSuffixUnaryOp> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitUnaryOperator(clang::UnaryOperator *UO);

private:
  std::vector<clang::UnaryOperator *> TheUnaryOps;
};

static RegisterMutator<MutateSuffixUnaryOp> M("s.mutate-suffix-unaryop", "Change a UnaryOperator's suffix operator.");

bool MutateSuffixUnaryOp::VisitUnaryOperator(UnaryOperator *UO) {
  if (UO->isPostfix() && isMutationSite(UO))
    TheUnaryOps.push_back(UO);
  return true;
}

bool MutateSuffixUnaryOp::mutate() {
  TraverseAST(getASTContext());
  if (TheUnaryOps.empty())
    return false;

  UnaryOperator *unaryOp = randElement(TheUnaryOps);
  UnaryOperatorKind oldOp = unaryOp->getOpcode();

  // Only handle postfix increment and decrement
  if (oldOp != UO_PostInc && oldOp != UO_PostDec)
    return false;

  // Change the operator
  UnaryOperatorKind newOp = (oldOp == UO_PostInc) ? UO_PostDec : UO_PostInc;
  std::string newOpStr = (newOp == UO_PostInc) ? "++" : "--";

  getRewriter().ReplaceText(unaryOp->getSourceRange(),
                            getSourceText(unaryOp->getSubExpr()) +
                                newOpStr);

  return true;
}
