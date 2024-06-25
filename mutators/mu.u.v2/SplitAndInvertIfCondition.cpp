#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SplitAndInvertIfCondition
    : public Mutator,
      public clang::RecursiveASTVisitor<SplitAndInvertIfCondition> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *ifStmt = randElement(TheIfStmts);

    // perform rewrite
    Expr *condition = ifStmt->getCond();
    SourceLocation start = condition->getBeginLoc();
    SourceLocation end = condition->getEndLoc();

    Rewriter &rewriter = getRewriter();
    std::string newCondition = "!(" + getSourceText(condition) + ")";

    rewriter.ReplaceText(SourceRange(start, end), newCondition);
    std::string trueBranch = getSourceText(ifStmt->getThen());
    std::string falseBranch = getSourceText(ifStmt->getElse());

    rewriter.ReplaceText(ifStmt->getThen()->getSourceRange(), falseBranch);
    rewriter.ReplaceText(ifStmt->getElse()->getSourceRange(), trueBranch);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IFS) {
    TheIfStmts.push_back(IFS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<SplitAndInvertIfCondition> M(
    "u2.SplitAndInvertIfCondition", "Split IfStmt condition and invert branches.");
