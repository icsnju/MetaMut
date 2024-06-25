#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveStmt : public Mutator,
                   public clang::RecursiveASTVisitor<RemoveStmt> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitStmt(clang::Stmt *S);

private:
  std::vector<clang::Stmt *> TheStatements;
};

static RegisterMutator<RemoveStmt> M("s.remove-stmt", "Remove a statement.");

bool RemoveStmt::VisitStmt(Stmt *S) {
  if (isMutationSite(S))
    TheStatements.push_back(S);
  return true;
}

bool RemoveStmt::mutate() {
  TraverseAST(getASTContext());
  if (TheStatements.empty())
    return false;

  Stmt *stmt = randElement(TheStatements);

  // perform rewrite
  SourceRange StmtRange = stmt->getSourceRange();
  getRewriter().ReplaceText(StmtRange, "");

  return true;
}
