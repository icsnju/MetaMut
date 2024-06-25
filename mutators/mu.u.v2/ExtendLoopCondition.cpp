#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ExtendLoopCondition
    : public Mutator,
      public clang::RecursiveASTVisitor<ExtendLoopCondition> {
public:
  using Mutator::Mutator;

  bool VisitWhileStmt(clang::WhileStmt *WS) {
    TheLoops.push_back(WS);
    return true;
  }

  bool VisitForStmt(clang::ForStmt *FS) {
    TheLoops.push_back(FS);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLoops.empty()) return false;

    Stmt *loop = randElement(TheLoops);
    Expr *condition = nullptr;
    SourceLocation loc;
    if (auto whileStmt = dyn_cast<WhileStmt>(loop)) {
      condition = whileStmt->getCond();
      loc = whileStmt->getCond()->getBeginLoc();
    } else if (auto forStmt = dyn_cast<ForStmt>(loop)) {
      condition = forStmt->getCond();
      loc = forStmt->getCond()->getBeginLoc();
    }
    if (!condition) return false;

    auto &sema = getCompilerInstance().getSema();
    std::string newConditionStr;
    if (randBool()) { // Choose AND or OR
      // Add a new AND condition
      ExprResult newCondition = sema.CreateBuiltinBinOp(
          loc, BO_LAnd, condition, condition->IgnoreParens());
      if (!newCondition.isInvalid()) {
        newConditionStr =
            getSourceText(condition) + " && " + getSourceText(condition);
      } else {
        return false;
      }
    } else {
      // Add a new OR condition
      ExprResult newCondition = sema.CreateBuiltinBinOp(
          loc, BO_LOr, condition, condition->IgnoreParens());
      if (!newCondition.isInvalid()) {
        newConditionStr =
            getSourceText(condition) + " || " + getSourceText(condition);
      } else {
        return false;
      }
    }

    getRewriter().ReplaceText(condition->getSourceRange(), newConditionStr);

    return true;
  }

private:
  std::vector<clang::Stmt *> TheLoops;
};

static RegisterMutator<ExtendLoopCondition> M(
    "u2.ExtendLoopCondition",
    "Extend loop condition by adding a compatible condition using a logical operator (AND or OR).");
