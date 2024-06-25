#include <algorithm>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <random>
#include <vector>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ShuffleExprStmts : public Mutator,
                         public clang::RecursiveASTVisitor<ShuffleExprStmts> {
  using VisitorTy = clang::RecursiveASTVisitor<ShuffleExprStmts>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitCompoundStmt(clang::CompoundStmt *S);

private:
  std::vector<std::vector<clang::Stmt *>> StmtGroups;
  std::vector<clang::Stmt *> CurrentGroup;
};

static RegisterMutator<ShuffleExprStmts> M("s.shuffle-exprstmts", "Shuffle continuous non-declaration statements.");

bool ShuffleExprStmts::VisitCompoundStmt(CompoundStmt *CS) {
  CurrentGroup.clear();
  for (Stmt *S : CS->body()) {
    if (!isa<Expr>(S)) {
      if (CurrentGroup.size() > 1)
        StmtGroups.push_back(std::move(CurrentGroup));
      CurrentGroup.clear();
    } else {
      CurrentGroup.push_back(S);
    }
  }
  if (CurrentGroup.size() > 1)
    StmtGroups.push_back(std::move(CurrentGroup));
  return true;
}

bool ShuffleExprStmts::mutate() {
  TraverseAST(getASTContext());

  if (StmtGroups.empty())
    return false;

  auto &group = randElement(StmtGroups);

  // Store the original source code
  std::vector<std::string> originalSources;
  for (auto stmt : group) {
    originalSources.push_back(
        getRewriter().getRewrittenText(stmt->getSourceRange()) + ";");
  }

  // Shuffle the statements
  std::shuffle(group.begin(), group.end(), getManager().getRandomGenerator());

  // Replace each statement with its new source code
  for (std::size_t i = 0; i < group.size(); ++i) {
    getRewriter().ReplaceText(group[i]->getSourceRange(), originalSources[i]);
  }

  return true;
}
