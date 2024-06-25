#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapStmtOrder : public Mutator,
                      public clang::RecursiveASTVisitor<SwapStmtOrder> {
public:
  using Mutator::Mutator;

  bool VisitCompoundStmt(clang::CompoundStmt *CS) {
    TheCompoundStmts.push_back(CS);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCompoundStmts.empty()) return false;

    CompoundStmt *selectedCS = randElement(TheCompoundStmts);
    auto stmts = selectedCS->body();
    if (std::distance(stmts.begin(), stmts.end()) < 2) return false;

    unsigned idx = randIndex(std::distance(stmts.begin(), stmts.end()) - 1);
    Stmt *stmt1 = *std::next(stmts.begin(), idx);
    Stmt *stmt2 = *std::next(stmts.begin(), idx + 1);

    // Check if both statements are non-declaration statements
    if (isa<DeclStmt>(stmt1) || isa<DeclStmt>(stmt2)) {
      return false;
    }

    // Swap adjacent statements and add a semicolon
    SourceLocation endStmt1 = getLocForEndOfToken(stmt1->getEndLoc());
    SourceLocation beginStmt2 = stmt2->getBeginLoc();
    getRewriter().ReplaceText(
        SourceRange(stmt1->getBeginLoc(), endStmt1),
        getSourceText(stmt2) + ";");
    getRewriter().ReplaceText(
        SourceRange(beginStmt2, getLocForEndOfToken(stmt2->getEndLoc())),
        getSourceText(stmt1) + ";");

    return true;
  }

private:
  std::vector<clang::CompoundStmt *> TheCompoundStmts;
};

static RegisterMutator<SwapStmtOrder> M(
    "u2.SwapStmtOrder", "Swaps the order of execution for two adjacent non-declaration statements.");
