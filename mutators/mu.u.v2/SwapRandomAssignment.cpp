#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapRandomAssignment : public Mutator,
                             public clang::RecursiveASTVisitor<SwapRandomAssignment> {
  using Mutator::Mutator;

public:
  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->getOpcode() == BO_Assign) {
      TheAssignments.push_back(BO);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheAssignments.empty()) return false;

    auto *firstAssignment = randElement(TheAssignments);
    auto *secondAssignment = randElement(TheAssignments);

    if (firstAssignment == secondAssignment) {
      return false;
    }

    // Swap RHS of assignments
    std::string firstRHS = getRewriter().getRewrittenText(firstAssignment->getRHS()->getSourceRange());
    std::string secondRHS = getRewriter().getRewrittenText(secondAssignment->getRHS()->getSourceRange());

    getRewriter().ReplaceText(firstAssignment->getRHS()->getSourceRange(), secondRHS);
    getRewriter().ReplaceText(secondAssignment->getRHS()->getSourceRange(), firstRHS);

    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheAssignments;
};

static RegisterMutator<SwapRandomAssignment> M(
    "u2.SwapRandomAssignment", "Swaps the right-hand sides of two random assignment statements within a function.");
