#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleBreakContinue
    : public Mutator,
      public clang::RecursiveASTVisitor<ToggleBreakContinue> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitBreakStmt(clang::BreakStmt *BS);
  bool VisitContinueStmt(clang::ContinueStmt *CS);

private:
  std::vector<clang::Stmt *> TheStatements;
};

static RegisterMutator<ToggleBreakContinue> M("s.toggle-break-continue", "Change break to continue or vice versa.");

bool ToggleBreakContinue::VisitBreakStmt(BreakStmt *BS) {
  if (isMutationSite(BS))
    TheStatements.push_back(BS);
  return true;
}

bool ToggleBreakContinue::VisitContinueStmt(ContinueStmt *CS) {
  if (isMutationSite(CS))
    TheStatements.push_back(CS);
  return true;
}

bool ToggleBreakContinue::mutate() {
  TraverseAST(getASTContext());
  if (TheStatements.empty())
    return false;

  Stmt *stmt = randElement(TheStatements);

  // perform rewrite
  std::string newStmt;
  if (isa<BreakStmt>(stmt)) {
    newStmt = "continue;";
  } else {
    newStmt = "break;";
  }

  SourceLocation start = stmt->getBeginLoc();
  SourceLocation end = Lexer::getLocForEndOfToken(
      stmt->getEndLoc(), 0, getASTContext().getSourceManager(),
      getASTContext().getLangOpts());
  getRewriter().ReplaceText(clang::SourceRange(start, end), newStmt);

  return true;
}
