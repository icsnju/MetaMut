#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomExprWithNullptr
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceRandomExprWithNullptr> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());

    if (PointerExpressions.empty()) return false;

    Expr *expr = randElement(PointerExpressions);

    getRewriter().ReplaceText(
        expr->getSourceRange(), "NULL");

    return true;
  }

  bool VisitUnaryOperator(clang::UnaryOperator *UO) {
    if (UO->getSubExpr()->getType()->isPointerType()) {
      PointerExpressions.push_back(UO->getSubExpr());
    }
    return true;
  }

  bool VisitBinaryOperator(clang::BinaryOperator *BO) {
    if (BO->getLHS()->getType()->isPointerType()) {
      PointerExpressions.push_back(BO->getLHS());
    }
    if (BO->getRHS()->getType()->isPointerType()) {
      PointerExpressions.push_back(BO->getRHS());
    }
    return true;
  }

private:
  std::vector<clang::Expr *> PointerExpressions;
};

static RegisterMutator<ReplaceRandomExprWithNullptr> M("u1.ReplaceRandomExprWithNullptr", "Replace a random pointer expression with NULL.");