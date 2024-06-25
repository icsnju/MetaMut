#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InvertAssignmentOperator
    : public Mutator,
      public clang::RecursiveASTVisitor<InvertAssignmentOperator> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    // Perform rewrite
    BinaryOperatorKind newOpKind = getInverseOperatorKind(expr->getOpcode());
    if (newOpKind == expr->getOpcode()) {
      // No inverse operator for the current one, select a random compound assignment operator
      newOpKind = getRandomCompoundAssignmentOp();
    }
    std::string NewOp = BinaryOperator::getOpcodeStr(newOpKind).str();
    getRewriter().ReplaceText(expr->getOperatorLoc(), expr->getOpcodeStr().size(), NewOp);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->isCompoundAssignmentOp()) {
      TheOperators.push_back(BO);
    }
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;

  BinaryOperatorKind getInverseOperatorKind(BinaryOperatorKind op) {
    switch (op) {
      case BO_AddAssign: return BO_SubAssign;
      case BO_SubAssign: return BO_AddAssign;
      case BO_MulAssign: return BO_DivAssign;
      case BO_DivAssign: return BO_MulAssign;
      case BO_ShlAssign: return BO_ShrAssign;
      case BO_ShrAssign: return BO_ShlAssign;
      default: return op; // In case of unsupported operators, return the same operator
    }
  }

  BinaryOperatorKind getRandomCompoundAssignmentOp() {
    static const std::vector<BinaryOperatorKind> compoundAssignOps = {
      BO_AddAssign, BO_SubAssign, BO_MulAssign, BO_DivAssign,
      BO_ShlAssign, BO_ShrAssign, BO_AndAssign, BO_XorAssign, BO_OrAssign
    };
    return compoundAssignOps[randIndex(compoundAssignOps.size())];
  }
};

static RegisterMutator<InvertAssignmentOperator> M(
    "u2.InvertAssignmentOperator", "Invert assignment operator within an expression");
