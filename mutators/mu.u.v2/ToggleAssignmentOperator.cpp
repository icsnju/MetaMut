#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleAssignmentOperator : public Mutator,
                                  public clang::RecursiveASTVisitor<ToggleAssignmentOperator> {
public:
  using Mutator::Mutator;

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->isCompoundAssignmentOp()) {
      TheOperators.push_back(BO);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    auto newOpKind = computeToggleOperator(expr->getOpcode());
    SourceLocation OpLoc = expr->getOperatorLoc();
    std::string NewOp = BinaryOperator::getOpcodeStr(newOpKind).str();
    getRewriter().ReplaceText(
        OpLoc, expr->getOpcodeStr(expr->getOpcode()).size(), NewOp);

    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;

  clang::BinaryOperatorKind computeToggleOperator(clang::BinaryOperatorKind opKind) {
    switch (opKind) {
      case BO_AddAssign:
        return BO_SubAssign;
      case BO_SubAssign:
        return BO_AddAssign;
      case BO_MulAssign:
        return BO_DivAssign;
      case BO_DivAssign:
        return BO_MulAssign;
      case BO_AndAssign:
        return BO_OrAssign;
      case BO_OrAssign:
        return BO_AndAssign;
      default:
        return opKind;
    }
  }
};

static RegisterMutator<ToggleAssignmentOperator> M(
    "u2.ToggleAssignmentOperator", "Change compound assignment operator to another compatible assignment operator.");
