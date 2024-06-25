#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddBitwiseOperator
    : public Mutator,
      public clang::RecursiveASTVisitor<AddBitwiseOperator> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheExprs.empty()) return false;

    Expr *expr = randElement(TheExprs);

    BinaryOperatorKind bitwiseOpKind = randBool() ? BO_And : BO_Or;
    std::string NewOp = BinaryOperator::getOpcodeStr(bitwiseOpKind).str();
    int operand = randIndex(8);  // Random integer between 0 and 7

    SourceLocation ExprEnd = Lexer::getLocForEndOfToken(expr->getEndLoc(), 0,
                                              getRewriter().getSourceMgr(), getASTContext().getLangOpts());
    getRewriter().InsertTextAfter(ExprEnd, " " + NewOp + " " + std::to_string(operand));

    return true;
  }

  bool VisitExpr(clang::Expr *E) {
    if (E->getType()->isIntegerType()) {
      TheExprs.push_back(E);
    }
    return true;
  }

private:
  std::vector<clang::Expr *> TheExprs;
};

static RegisterMutator<AddBitwiseOperator> M(
    "u2.AddBitwiseOperator", "It adds a bitwise operator with an operand to a numeric variable or expression, potentially altering its value.");
