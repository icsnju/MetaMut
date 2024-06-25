#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomConditionalExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<AddRandomConditionalExpr> {
public:
  using Mutator::Mutator;

  bool VisitReturnStmt(clang::ReturnStmt *RS) {
    Expr *retExpr = RS->getRetValue();
    if (retExpr && retExpr->getType()->isIntegerType()) {
      TheReturnStmts.push_back(RS);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheReturnStmts.empty()) return false;

    ReturnStmt *returnStmt = randElement(TheReturnStmts);
    Expr *retExpr = returnStmt->getRetValue();

    std::string randomCondition = generateRandomBooleanCondition();
    std::string randomValue = generateRandomIntegerValue();
    std::string conditionalExpression = "return " + randomCondition + " ? " + getSourceText(retExpr) + " : " + randomValue;

    getRewriter().ReplaceText(returnStmt->getSourceRange(), conditionalExpression);

    return true;
  }

private:
  std::vector<clang::ReturnStmt *> TheReturnStmts;

  std::string generateRandomBooleanCondition() {
    return randBool() ? "1" : "0";
  }

  std::string generateRandomIntegerValue() {
    int randomValue = getManager().getRandomGenerator()() % 100;
    return std::to_string(randomValue);
  }
};

static RegisterMutator<AddRandomConditionalExpr> M(
    "u2.AddRandomConditionalExpr",
    "It takes an integer return statement and inserts a random "
    "conditional expression, modifying the returned value based on a boolean condition.");
