// NegateLiteral.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class NegateLiteral
    : public Mutator,
      public clang::RecursiveASTVisitor<NegateLiteral> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLiterals.empty()) return false;

    Expr *expr = randElement(TheLiterals);

    // perform rewrite
    std::string newText = "-" + getSourceText(expr);
    getRewriter().ReplaceText(expr->getSourceRange(), newText);

    return true;
  }

  bool VisitIntegerLiteral(clang::IntegerLiteral *IL) {
    TheLiterals.push_back(IL);
    return true;
  }

  bool VisitFloatingLiteral(clang::FloatingLiteral *FL) {
    TheLiterals.push_back(FL);
    return true;
  }

private:
  std::vector<clang::Expr *> TheLiterals;
};

static RegisterMutator<NegateLiteral> M("u1.NegateLiteral", "Negate a selected Integer or Floating point Literal.");