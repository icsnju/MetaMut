#include <algorithm>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <random>
#include <vector>

#include "Mutator.h"
#include "MutatorManager.h"

class DuplicateStatement
    : public Mutator,
      public clang::RecursiveASTVisitor<DuplicateStatement> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitCompoundStmt(clang::CompoundStmt *S);

private:
  std::vector<clang::Stmt *> Stmts;
};

static RegisterMutator<DuplicateStatement> M("s.duplicate-stmt", "Duplicate a random non-declaration statement.");

bool DuplicateStatement::VisitCompoundStmt(clang::CompoundStmt *CS) {
  for (clang::Stmt *S : CS->body()) {
    if (isMutationSite(S))
      if (!clang::isa<clang::DeclStmt>(S)) {
        Stmts.push_back(S);
      }
  }
  return true;
}

bool DuplicateStatement::mutate() {
  TraverseAST(getASTContext());

  if (Stmts.empty())
    return false;

  auto stmt = randElement(Stmts);

  // Store the original source code
  std::string originalSource =
      getRewriter().getRewrittenText(stmt->getSourceRange());

  // Duplicate the statement
  std::string duplicateSource = originalSource + ";" + originalSource;

  // Replace the statement with its duplicate
  getRewriter().ReplaceText(stmt->getSourceRange(), duplicateSource);

  return true;
}
