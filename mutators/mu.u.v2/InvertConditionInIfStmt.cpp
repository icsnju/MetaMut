#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InvertConditionInIfStmt : public Mutator,
                                public clang::RecursiveASTVisitor<InvertConditionInIfStmt> {
  using Mutator::Mutator;

public:
  bool VisitIfStmt(IfStmt *I) {
    TheIfStmts.push_back(I);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *ifStmt = randElement(TheIfStmts);

    // Invert the condition
    SourceRange condRange = ifStmt->getCond()->getSourceRange();
    std::string invertedCond = "!(" + getSourceText(ifStmt->getCond()) + ")";
    getRewriter().ReplaceText(condRange, invertedCond);

    // Swap the true and false branches
    Stmt *trueBranch = ifStmt->getThen();
    Stmt *falseBranch = ifStmt->getElse();

    if (falseBranch) {
      std::string trueSource = getSourceText(trueBranch);
      std::string falseSource = getSourceText(falseBranch);
      getRewriter().ReplaceText(trueBranch->getSourceRange(), falseSource);
      getRewriter().ReplaceText(falseBranch->getSourceRange(), trueSource);
    } else {
      std::string trueSource = getSourceText(trueBranch);
      getRewriter().InsertTextBefore(
          ifStmt->getThen()->getBeginLoc(),
          " else " + trueSource);
      getRewriter().RemoveText(trueBranch->getSourceRange());
    }

    return true;
  }

private:
  std::vector<IfStmt *> TheIfStmts;
};

static RegisterMutator<InvertConditionInIfStmt> M(
    "u2.InvertConditionInIfStmt",
    "Inverts the condition in an IfStmt, swapping the true and false branches in the body.");
