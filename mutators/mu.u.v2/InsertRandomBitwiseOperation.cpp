#include "Mutator.h"
#include "MutatorManager.h"

#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <random>

using namespace clang;

class InsertRandomBitwiseOperation : public Mutator,
                                     public clang::RecursiveASTVisitor<InsertRandomBitwiseOperation> {
public:
  using Mutator::Mutator;
  using VisitorTy = clang::RecursiveASTVisitor<InsertRandomBitwiseOperation>;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheNumericExprs.empty()) return false;

    Expr *expr = randElement(TheNumericExprs);
    SourceLocation insertionLoc = expr->getBeginLoc();

    // Generate random bitwise operation and compatible value
    std::vector<std::string> operations = {"&", "|", "^", "~"};
    std::string randomOperation = operations[randIndex(operations.size() - 1)]; // exclude NOT
    unsigned compatibleValue = randIndex(256); // use small values to avoid semantic issues

    // Perform mutation
    std::string newExpression = "(" + getSourceText(expr) + " " + randomOperation + " " + std::to_string(compatibleValue) + ")";
    getRewriter().ReplaceText(expr->getSourceRange(), newExpression);

    return true;
  }

  bool VisitExpr(clang::Expr *E) {
    if (E->getType()->isIntegerType()) {
      TheNumericExprs.push_back(E);
    }
    return true;
  }

private:
  std::vector<clang::Expr *> TheNumericExprs;
};

static RegisterMutator<InsertRandomBitwiseOperation> M(
    "u2.InsertRandomBitwiseOperation",
    "Inserts a random bitwise operation (AND, OR, XOR, or NOT) with a compatible value to a numeric variable or expression, potentially altering its value.");
