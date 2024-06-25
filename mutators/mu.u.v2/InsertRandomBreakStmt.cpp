#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomBreakStmt : public Mutator,
                              public clang::RecursiveASTVisitor<InsertRandomBreakStmt> {
  using Mutator::Mutator;

public:
  bool VisitForStmt(clang::ForStmt *FS) {
    LoopAndSwitchStmts.push_back(FS);
    return true;
  }

  bool VisitWhileStmt(clang::WhileStmt *WS) {
    LoopAndSwitchStmts.push_back(WS);
    return true;
  }

  bool VisitDoStmt(clang::DoStmt *DS) {
    LoopAndSwitchStmts.push_back(DS);
    return true;
  }

  bool VisitSwitchStmt(clang::SwitchStmt *SS) {
    LoopAndSwitchStmts.push_back(SS);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (LoopAndSwitchStmts.empty()) return false;

    Stmt *selectedStmt = randElement(LoopAndSwitchStmts);
    CompoundStmt *bodyStmt = nullptr;

    if (ForStmt *FS = dyn_cast<ForStmt>(selectedStmt)) {
      bodyStmt = dyn_cast<CompoundStmt>(FS->getBody());
    } else if (WhileStmt *WS = dyn_cast<WhileStmt>(selectedStmt)) {
      bodyStmt = dyn_cast<CompoundStmt>(WS->getBody());
    } else if (DoStmt *DS = dyn_cast<DoStmt>(selectedStmt)) {
      bodyStmt = dyn_cast<CompoundStmt>(DS->getBody());
    } else if (SwitchStmt *SS = dyn_cast<SwitchStmt>(selectedStmt)) {
      bodyStmt = dyn_cast<CompoundStmt>(SS->getBody());
    }

    if (!bodyStmt || bodyStmt->size() == 0) return false;

    std::vector<Stmt*> bodyVec(bodyStmt->body_begin(), bodyStmt->body_end());
    Stmt *insertPoint = randElement(bodyVec);
    SourceLocation loc = insertPoint->getBeginLoc();
    getRewriter().InsertTextBefore(loc, "break;\n");

    return true;
  }

private:
  std::vector<clang::Stmt *> LoopAndSwitchStmts;
};

static RegisterMutator<InsertRandomBreakStmt> M(
    "u2.InsertRandomBreakStmt",
    "It takes a loop or switch statement and inserts a random break statement within its body, altering the control flow by causing an early exit from the loop or switch.");
