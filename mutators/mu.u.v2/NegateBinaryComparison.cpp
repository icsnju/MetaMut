#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class NegateBinaryComparison
    : public Mutator,
      public clang::RecursiveASTVisitor<NegateBinaryComparison> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheComparisons.empty()) return false;

    BinaryOperator *expr = randElement(TheComparisons);
    BinaryOperatorKind newOpKind = getNegatedComparison(expr->getOpcode());
    SourceLocation OpLoc = expr->getOperatorLoc();
    std::string NewOp = BinaryOperator::getOpcodeStr(newOpKind).str();
    getRewriter().ReplaceText(
        OpLoc, expr->getOpcodeStr(expr->getOpcode()).size(), NewOp);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->isComparisonOp()) {
      TheComparisons.push_back(BO);
    }
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheComparisons;

  BinaryOperatorKind getNegatedComparison(BinaryOperatorKind kind) {
    switch (kind) {
      case BO_EQ: return BO_NE;
      case BO_NE: return BO_EQ;
      case BO_LT: return BO_GE;
      case BO_LE: return BO_GT;
      case BO_GT: return BO_LE;
      case BO_GE: return BO_LT;
      default: return kind;
    }
  }
};

static RegisterMutator<NegateBinaryComparison> M(
    "u2.NegateBinaryComparison", "Negate binary comparison operator.");
