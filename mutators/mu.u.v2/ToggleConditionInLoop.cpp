#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleConditionInLoop
    : public Mutator,
      public clang::RecursiveASTVisitor<ToggleConditionInLoop> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLoops.empty()) return false;

    Stmt *loopStmt = randElement(TheLoops);
    Expr *loopCond = nullptr;

    if (ForStmt *forLoop = dyn_cast<ForStmt>(loopStmt)) {
      loopCond = forLoop->getCond();
    } else if (WhileStmt *whileLoop = dyn_cast<WhileStmt>(loopStmt)) {
      loopCond = whileLoop->getCond();
    } else if (DoStmt *doWhileLoop = dyn_cast<DoStmt>(loopStmt)) {
      loopCond = doWhileLoop->getCond();
    }

    if (!loopCond) return false;

    SourceLocation startLoc = loopCond->getBeginLoc();
    getRewriter().InsertText(startLoc, "!");
    
    return true;
  }

  bool VisitForStmt(clang::ForStmt *FS) {
    TheLoops.push_back(FS);
    return true;
  }

  bool VisitWhileStmt(clang::WhileStmt *WS) {
    TheLoops.push_back(WS);
    return true;
  }

  bool VisitDoStmt(clang::DoStmt *DS) {
    TheLoops.push_back(DS);
    return true;
  }

private:
  std::vector<clang::Stmt *> TheLoops;
};

static RegisterMutator<ToggleConditionInLoop> M(
    "u2.ToggleConditionInLoop", "Toggle a loop condition in for, while, or do-while loops.");
