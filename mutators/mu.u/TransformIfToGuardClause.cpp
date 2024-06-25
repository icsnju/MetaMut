#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceManager.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class TransformIfToGuardClause : public Mutator,
                                 public clang::RecursiveASTVisitor<TransformIfToGuardClause> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfStmts.empty()) return false;

    IfStmt *ifStmt = randElement(TheIfStmts);

    // Invert the condition
    std::string invertedCondition = "!(" + getSourceText(ifStmt->getCond()) + ")";
    
    // Generate the guard clause
    std::string guardClause = "if (" + invertedCondition + ") ";

    // If there's an else branch, make it the main body of the function 
    Stmt* elseStmt = ifStmt->getElse();
    if (elseStmt) {
      guardClause += getSourceText(elseStmt);
    } else {
      guardClause += "return;";
    }

    // Prepend the original 'then' branch as an early return
    std::string thenStmt = getSourceText(ifStmt->getThen());
    guardClause = thenStmt + " return; " + guardClause;

    // Replace the original IfStmt with the guard clause
    getRewriter().ReplaceText(ifStmt->getSourceRange(), guardClause);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<TransformIfToGuardClause> M("u1.TransformIfToGuardClause", "Transform IfStmt into a guard clause by inverting the condition and making the 'then' branch an early return.");