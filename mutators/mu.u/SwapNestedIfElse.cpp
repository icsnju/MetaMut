#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapNestedIfElse
    : public Mutator,
      public clang::RecursiveASTVisitor<SwapNestedIfElse> {

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *stmt = randElement(TheIfStmts);

    // perform rewrite
    swapNestedIfElse(stmt);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    if(IS->getElse() != nullptr) {
        TheIfStmts.push_back(IS);
    }
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;

  void swapNestedIfElse(clang::IfStmt *stmt) {
    SourceLocation ifStart = stmt->getIfLoc();
    SourceLocation elseStart = stmt->getElseLoc();
    SourceLocation condEnd = getLocForEndOfToken(stmt->getCond()->getEndLoc());

    std::string newIf = "if (!" + getSourceText(stmt->getCond()) + ") ";
    std::string elseBody = getSourceText(stmt->getElse());
    std::string ifBody = getSourceText(stmt->getThen());

    getRewriter().ReplaceText(
        SourceRange(ifStart, condEnd), newIf);
    getRewriter().ReplaceText(
        stmt->getThen()->getSourceRange(), elseBody);
    getRewriter().ReplaceText(
        stmt->getElse()->getSourceRange(), ifBody);
  }
};

static RegisterMutator<SwapNestedIfElse> M("u1.SwapNestedIfElse", "Swap a nested if-else statement and inverse the condition.");