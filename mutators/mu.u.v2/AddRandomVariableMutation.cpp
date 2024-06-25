#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomVariableMutation
    : public Mutator,
      public clang::RecursiveASTVisitor<AddRandomVariableMutation> {
public:
  using Mutator::Mutator;

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->isAssignmentOp() || BO->isCompoundAssignmentOp()) {
      Expr *rhs = BO->getRHS()->IgnoreParenImpCasts();
      if (rhs->getType()->isIntegerType()) {
        TheExpressions.push_back(rhs);
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());

    if (TheExpressions.empty()) return false;

    Expr *expr = randElement(TheExpressions);
    SourceRange exprRange = getExpansionRange(expr->getSourceRange());
    std::string exprText = getSourceText(expr);

    std::vector<std::string> operators = {"+", "-", "*", "/"};
    int randomValue = rand() % 10;
    std::string mutation = operators[randIndex(operators.size())] + std::to_string(randomValue);
    std::string mutatedExpr = "(" + exprText + " " + mutation + ")";

    getRewriter().ReplaceText(exprRange, mutatedExpr);
    return true;
  }

private:
  std::vector<clang::Expr *> TheExpressions;
};

static RegisterMutator<AddRandomVariableMutation> M(
    "u2.AddRandomVariableMutation", "Applies a random arithmetic operation (addition, subtraction, multiplication, or division) with a random value to a numeric variable or expression on the right-hand side of assignment or compound assignment operators.");
