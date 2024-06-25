#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class NegateBranchCondition
    : public Mutator,
      public clang::RecursiveASTVisitor<NegateBranchCondition> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitIfStmt(clang::IfStmt *IS);

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<NegateBranchCondition> M("s.negate-branch-cond", "Negate the condition of a branch statement.");

bool NegateBranchCondition::VisitIfStmt(IfStmt *IS) {
  if (isMutationSite(IS))
    TheIfStmts.push_back(IS);
  return true;
}

bool NegateBranchCondition::mutate() {
  TraverseAST(getASTContext());
  if (TheIfStmts.empty())
    return false;

  IfStmt *stmt = randElement(TheIfStmts);

  // perform rewrite
  SourceRange CondRange = stmt->getCond()->getSourceRange();
  std::string NewCond =
      "!(" +
      Lexer::getSourceText(CharSourceRange::getTokenRange(CondRange),
                           getASTContext().getSourceManager(),
                           getASTContext().getLangOpts()).str()
           +
      ")";

  getRewriter().ReplaceText(CondRange, NewCond);

  return true;
}
