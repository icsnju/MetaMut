// ReplaceWithEquivalentUnaryExpr.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceWithEquivalentUnaryExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceWithEquivalentUnaryExpr> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    UnaryOperator *expr = randElement(TheOperators);
    SourceLocation startLoc = expr->getOperatorLoc();
    SourceLocation endLoc = expr->getSubExpr()->getEndLoc();
    std::string oldExpr = getRewriter().getRewrittenText(SourceRange(startLoc, endLoc));
    std::string newOp = computeValidAlternatives(expr, oldExpr);
    if (newOp.empty()) return false;

    getRewriter().ReplaceText(SourceRange(startLoc, endLoc), newOp);

    return true;
  }

  bool VisitUnaryOperator(clang::UnaryOperator *UO) {
    if (UO->isArithmeticOp() || UO->getOpcode() == UO_LNot) {
      TheOperators.push_back(UO);
    }
    return true;
  }

private:
  std::vector<clang::UnaryOperator *> TheOperators;

  std::string computeValidAlternatives(clang::UnaryOperator *op, std::string oldExpr) {
    UnaryOperatorKind kind = op->getOpcode();

    if (kind == UO_Minus) {
      // Replace unary minus with equivalent expression 0 - x
      return "0 - " + oldExpr;
    } else if (kind == UO_Not) {
      // Replace bitwise not with equivalent expression -x - 1
      return "-(" + oldExpr + ") - 1";
    } else if (kind == UO_LNot) {
      // Replace logical not with equivalent expression x == 0
      return oldExpr + " == 0";
    } else {
      return "";
    }
  }
};

static RegisterMutator<ReplaceWithEquivalentUnaryExpr> M("u1.ReplaceWithEquivalentUnaryExpr", "Replace UnaryExpr with an equivalent UnaryExpr that uses a different operator.");