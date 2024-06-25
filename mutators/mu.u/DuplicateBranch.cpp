#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;

class DuplicateBranch : public Mutator,
                        public clang::RecursiveASTVisitor<DuplicateBranch> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    // Randomly select an IfStmt
    IfStmt *ifStmt = randElement(TheIfStmts);

    // Choose which branch to duplicate (then or else)
    bool duplicateThenBranch = randBool();
    Stmt *sourceBranch = duplicateThenBranch ? ifStmt->getThen() : ifStmt->getElse();
    Stmt *targetBranch = duplicateThenBranch ? ifStmt->getElse() : ifStmt->getThen();

    if (!sourceBranch || !targetBranch) return false;

    // Get the source code for the branch to duplicate
    std::string sourceCode = getSourceText(sourceBranch);

    // Replace the target branch with the duplicated branch
    SourceRange targetRange = getExpansionRange(targetBranch->getSourceRange());
    getRewriter().ReplaceText(targetRange, sourceCode);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<DuplicateBranch> M("u1.DuplicateBranch", "Duplicates one branch of an if statement");