#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DecomposeCompoundAssignment
    : public Mutator,
      public clang::RecursiveASTVisitor<DecomposeCompoundAssignment> {
public:
  using Mutator::Mutator;

  bool VisitCompoundAssignOperator(clang::CompoundAssignOperator *CAO) {
    TheAssignments.push_back(CAO);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheAssignments.empty()) return false;

    CompoundAssignOperator *CAO = randElement(TheAssignments);

    // Decompose the compound assignment
    std::string LHS = getSourceText(CAO->getLHS());
    std::string RHS = getSourceText(CAO->getRHS());
    BinaryOperatorKind decomposedOp = CAO->getOpcode();
    std::string decomposedOpStr = BinaryOperator::getOpcodeStr(decomposedOp).str();

    std::string newAssignment = LHS + " = " + LHS + " " + decomposedOpStr + " " + RHS + ";";
    getRewriter().ReplaceText(CAO->getSourceRange(), newAssignment);

    return true;
  }

private:
  std::vector<clang::CompoundAssignOperator *> TheAssignments;
};

static RegisterMutator<DecomposeCompoundAssignment> M(
    "u2.DecomposeCompoundAssignment",
    "Decompose compound assignment into separate assignment and arithmetic statements.");
