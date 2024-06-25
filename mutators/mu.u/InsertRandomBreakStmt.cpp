#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomBreakStmt : public Mutator,
                              public clang::RecursiveASTVisitor<InsertRandomBreakStmt> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLoops.empty()) return false;

    Stmt *loop = randElement(TheLoops);
    CompoundStmt *body = nullptr;

    if (auto forStmt = dyn_cast<ForStmt>(loop))
        body = dyn_cast<CompoundStmt>(forStmt->getBody());
    else if (auto whileStmt = dyn_cast<WhileStmt>(loop))
        body = dyn_cast<CompoundStmt>(whileStmt->getBody());
    else if (auto doStmt = dyn_cast<DoStmt>(loop))
        body = dyn_cast<CompoundStmt>(doStmt->getBody());

    if (!body || body->body_empty()) return false;

    std::vector<clang::Stmt*> bodyVector(body->body().begin(), body->body().end());
    Stmt *stmtToBreakAfter = randElement(bodyVector);

    getRewriter().InsertTextAfter(getLocForEndOfToken(stmtToBreakAfter->getEndLoc()), "; break;");

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

static RegisterMutator<InsertRandomBreakStmt> M("u1.InsertRandomBreakStmt", "Randomly inserts a break statement in a loop body.");