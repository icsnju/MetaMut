#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceConditionalExprWithTernary
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceConditionalExprWithTernary> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *ifStmt = randElement(TheIfStmts);

    Stmt *trueBranch = ifStmt->getThen();
    Stmt *falseBranch = ifStmt->getElse();

    if (!trueBranch || !falseBranch) {
      return false;
    }

    auto *trueExpr = dyn_cast<Expr>(trueBranch);
    auto *falseExpr = dyn_cast<Expr>(falseBranch);

    if (trueExpr && trueExpr->getType()->isFunctionPointerType() && falseExpr && falseExpr->getType()->isFunctionPointerType()) {
      return false;
    }

    Expr *condition = ifStmt->getCond();
    if (!condition) return false;

    std::string conditionText = getSourceText(condition);
    std::string trueBranchText = getSourceText(trueBranch);
    std::string falseBranchText = getSourceText(falseBranch);
    std::string ternaryExpression =
        "(" + conditionText + ") ? (" + trueBranchText + ") : (" + falseBranchText + ");";

    getRewriter().ReplaceText(ifStmt->getSourceRange(), ternaryExpression);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<ReplaceConditionalExprWithTernary> M(
    "u2.ReplaceConditionalExprWithTernary",
    "Replace a simple if-else statement that assigns a value to a single variable with a ternary conditional expression.");
