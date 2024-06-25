// InverseLogicalExpr.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InverseLogicalExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<InverseLogicalExpr> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    // perform rewrite
    std::string newOp;
    if (expr->getOpcode() == BO_LAnd) {
        newOp = "||";
    } else if (expr->getOpcode() == BO_LOr) {
        newOp = "&&";
    } else {
        return false;
    }

    SourceLocation OpLoc = expr->getOperatorLoc();
    getRewriter().ReplaceText(OpLoc, expr->getOpcodeStr().size(), newOp);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->getOpcode() == BO_LAnd || BO->getOpcode() == BO_LOr) {
        TheOperators.push_back(BO);
    }
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;
};

static RegisterMutator<InverseLogicalExpr> M("u1.InverseLogicalExpr", "Inverse logical expressions.");