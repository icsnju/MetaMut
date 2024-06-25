#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DuplicateIfStmt : public Mutator,
                        public clang::RecursiveASTVisitor<DuplicateIfStmt> {
  using Mutator::Mutator;

public:
  bool VisitIfStmt(IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *selectedIfStmt = randElement(TheIfStmts);
    Stmt *innerStmt = selectedIfStmt->getThen();
    std::string innerStmtText = getSourceText(innerStmt);
    std::string newIfStmtText =
        "if (" + getSourceText(selectedIfStmt->getCond()) + ") { " +
        innerStmtText + " }";

    getRewriter().ReplaceText(innerStmt->getSourceRange(), newIfStmtText);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<DuplicateIfStmt> M(
    "u2.DuplicateIfStmt", "Duplicates an IfStmt, creating a nested IfStmt with the same condition and body.");
