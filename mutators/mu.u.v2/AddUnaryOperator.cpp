#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddUnaryOperator
    : public Mutator,
      public clang::RecursiveASTVisitor<AddUnaryOperator> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheExpressions.empty()) return false;

    Expr *expr = randElement(TheExpressions);
    SourceLocation ExprLoc = expr->getExprLoc();

    auto &sema = getCompilerInstance().getSema();
    bool foundValidUnary = false;

    for (int i = UO_Minus; i <= UO_Not; ++i) {
      UnaryOperatorKind kind = static_cast<UnaryOperatorKind>(i);
      ExprResult result = sema.CreateBuiltinUnaryOp(ExprLoc, kind, expr);

      if (!result.isInvalid()) {
        foundValidUnary = true;
        UnaryOperatorKind newOpKind = kind;
        SourceRange exprRange = getExpansionRange(expr->getSourceRange());
        std::string newText = UnaryOperator::getOpcodeStr(newOpKind).str() + " " + getRewriter().getRewrittenText(exprRange);
        getRewriter().ReplaceText(exprRange, newText);
        break;
      }
    }

    return foundValidUnary;
  }

  bool VisitExpr(clang::Expr *E) {
    if (E->isIntegerConstantExpr(getASTContext()) || isa<DeclRefExpr>(E)) {
      TheExpressions.push_back(E);
    }
    return true;
  }

private:
  std::vector<clang::Expr *> TheExpressions;
};

static RegisterMutator<AddUnaryOperator> M(
    "u2.AddUnaryOperator", "It adds a unary operator to a numeric variable or expression, potentially changing its value.");
