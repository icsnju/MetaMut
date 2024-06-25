#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwitchIfAndElseBlock
    : public Mutator,
      public clang::RecursiveASTVisitor<SwitchIfAndElseBlock> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *ifStmt = randElement(TheIfStmts);

    // Swap true and false branches
    Stmt *trueBranch = ifStmt->getThen();
    Stmt *falseBranch = ifStmt->getElse();

    if (falseBranch != nullptr) {
      std::string trueSource = getSourceText(trueBranch);
      std::string falseSource = getSourceText(falseBranch);

      getRewriter().ReplaceText(trueBranch->getSourceRange(), falseSource);
      getRewriter().ReplaceText(falseBranch->getSourceRange(), trueSource);
    } else {
      std::string trueSource = getSourceText(trueBranch);

      getRewriter().ReplaceText(trueBranch->getSourceRange(), "{}");
      getRewriter().InsertTextAfter(
          Lexer::getLocForEndOfToken(ifStmt->getElseLoc(), 0, getCompilerInstance().getSourceManager(), getCompilerInstance().getLangOpts()),
          " else " + trueSource);
    }

    // Invert the condition
    Expr *condition = ifStmt->getCond();
    std::string invertedCondition = "!(" + getSourceText(condition) + ")";

    getRewriter().ReplaceText(condition->getSourceRange(), invertedCondition);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<SwitchIfAndElseBlock> M(
    "u2.SwitchIfAndElseBlock", "Swaps the true and false branches of an IfStmt, inverting the condition in the process.");
