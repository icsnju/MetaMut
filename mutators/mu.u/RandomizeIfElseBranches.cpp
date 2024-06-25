#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RandomizeIfElseBranches 
    : public Mutator,
      public clang::RecursiveASTVisitor<RandomizeIfElseBranches> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *stmt = randElement(TheIfStmts);

    // perform rewrite
    invertCondition(stmt);
    swapThenElse(stmt);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;

  void swapThenElse(clang::IfStmt *stmt) {
    std::string thenBranch = getSourceText(stmt->getThen());
    std::string elseBranch = stmt->getElse() ? getSourceText(stmt->getElse()) : "";

    SourceRange thenRange = getExpansionRange(stmt->getThen()->getSourceRange());
    SourceRange elseRange = stmt->getElse() ? getExpansionRange(stmt->getElse()->getSourceRange()) : SourceRange();

    if (stmt->getElse()) {
      getRewriter().ReplaceText(thenRange, elseBranch);
      getRewriter().ReplaceText(elseRange, thenBranch);
    } else {
      getRewriter().InsertTextAfter(getLocForEndOfToken(thenRange.getEnd()), " else " + thenBranch);
      getRewriter().ReplaceText(thenRange, "");
    }
  }

  void invertCondition(clang::IfStmt *stmt) {
    std::string condition = getSourceText(stmt->getCond());
    std::string invertedCondition = "!(" + condition + ")";
    SourceRange conditionRange = getExpansionRange(stmt->getCond()->getSourceRange());
    getRewriter().ReplaceText(conditionRange, invertedCondition);
  }
};

static RegisterMutator<RandomizeIfElseBranches> M("u1.RandomizeIfElseBranches", 
    "Randomly selects an IfStmt, inverts the condition, and swaps the then and else branches.");