#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomOperatorWithInverse : public Mutator, public clang::RecursiveASTVisitor<ReplaceRandomOperatorWithInverse> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    // perform rewrite
    BinaryOperatorKind newOpKind = getInverseOp(expr->getOpcode());
    SourceLocation OpLoc = expr->getOperatorLoc();
    std::string NewOp = BinaryOperator::getOpcodeStr(newOpKind).str();
    getRewriter().ReplaceText(
        OpLoc, expr->getOpcodeStr(expr->getOpcode()).size(), NewOp);

    // Adjust the right hand side operand to maintain the original result
    Expr *rhs = expr->getRHS();
    getRewriter().InsertTextBefore(rhs->getBeginLoc(), "(1 / ");
    getRewriter().InsertTextAfter(getLocForEndOfToken(rhs->getEndLoc()), ")");

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->isAdditiveOp() || BO->isMultiplicativeOp()) {
      TheOperators.push_back(BO);
    }
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;

  BinaryOperatorKind getInverseOp(BinaryOperatorKind op) {
    switch (op) {
      case BO_Add: return BO_Sub;
      case BO_Sub: return BO_Add;
      case BO_Mul: return BO_Div;
      case BO_Div: return BO_Mul;
      default: return op;  // should not reach here
    }
  }
};

static RegisterMutator<ReplaceRandomOperatorWithInverse> M("u1.ReplaceRandomOperatorWithInverse", "Replace a binary operator with its inverse and adjust the right hand side operand to maintain the original result.");
