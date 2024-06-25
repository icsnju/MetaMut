#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InvertRelationalOperator 
    : public Mutator,
      public clang::RecursiveASTVisitor<InvertRelationalOperator> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    BinaryOperator *expr = randElement(TheOperators);

    // perform rewrite
    auto newOp = invertOperator(expr);
    if(newOp == expr->getOpcode()) return false; // Ensure the operator is actually inverted

    SourceLocation OpLoc = expr->getOperatorLoc();
    std::string NewOp = BinaryOperator::getOpcodeStr(newOp).str();
    getRewriter().ReplaceText(
        OpLoc, expr->getOpcodeStr(expr->getOpcode()).size(), NewOp);

    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if(BO->isRelationalOp()) {
      TheOperators.push_back(BO);
    }
    return true;
  }

private:
  std::vector<clang::BinaryOperator *> TheOperators;

  clang::BinaryOperatorKind invertOperator(clang::BinaryOperator *op) {
    switch(op->getOpcode()) {
      case BO_LT: return BO_GE;
      case BO_GT: return BO_LE;
      case BO_LE: return BO_GT;
      case BO_GE: return BO_LT;
      case BO_EQ: return BO_NE;
      case BO_NE: return BO_EQ;
      default: return op->getOpcode();
    }
  }
};

static RegisterMutator<InvertRelationalOperator> M("u1.InvertRelationalOperator", "Invert the relational operator in a BinaryOperator");
