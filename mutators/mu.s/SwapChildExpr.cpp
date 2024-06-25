#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapChildExpr : public Mutator,
                      public clang::RecursiveASTVisitor<SwapChildExpr> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitBinaryOperator(clang::BinaryOperator *BO);

private:
  std::vector<clang::BinaryOperator *> TheOps;
};

static RegisterMutator<SwapChildExpr> M("s.swap-child-expr",
      "Randomly swap two child expressions of a BinaryOperator.");

bool SwapChildExpr::VisitBinaryOperator(BinaryOperator *BO) {
  if (isMutationSite(BO) && BO->getLHS() && BO->getRHS())
    TheOps.push_back(BO);
  return true;
}

bool SwapChildExpr::mutate() {
  TraverseAST(getASTContext());
  if (TheOps.empty())
    return false;

  BinaryOperator *selectedBO = randElement(TheOps);

  // Create a new BinaryOperator with swapped operands
  auto newExpr = BinaryOperator::Create(
      getASTContext(),
      selectedBO->getRHS(), // Swapped
      selectedBO->getLHS(), // Swapped
      selectedBO->getOpcode(), selectedBO->getType(),
      selectedBO->getValueKind(), selectedBO->getObjectKind(),
      selectedBO->getOperatorLoc(), FPOptionsOverride{});

  // Get the token directly from the source location
  clang::SourceLocation loc = selectedBO->getOperatorLoc();
  clang::Token token;
  clang::Lexer::getRawToken(loc, token, getASTContext().getSourceManager(),
                            getASTContext().getLangOpts());

  // Check if the new BinaryOperator is valid
  clang::ExprResult result = getCompilerInstance().getSema().ActOnBinOp(
      getCompilerInstance().getSema().getCurScope(), loc, token.getKind(),
      selectedBO->getRHS(), selectedBO->getLHS());

  if (result.isInvalid()) {
    return false; // The swapped operator is not valid.
  }

  // Get the source ranges of the child expressions
  SourceRange lhsRange = selectedBO->getLHS()->getSourceRange();
  SourceRange rhsRange = selectedBO->getRHS()->getSourceRange();

  // Get the source texts of the child expressions
  std::string lhsText = getRewriter().getRewrittenText(lhsRange);
  std::string rhsText = getRewriter().getRewrittenText(rhsRange);

  // Swap the child expressions
  getRewriter().ReplaceText(lhsRange, rhsText);
  getRewriter().ReplaceText(rhsRange, lhsText);

  return true;
}
