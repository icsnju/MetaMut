#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InvertFunctionCondition
    : public Mutator,
      public clang::RecursiveASTVisitor<InvertFunctionCondition> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheReturns.empty()) return false;

    ReturnStmt *stmt = randElement(TheReturns);
    Expr *exp = stmt->getRetValue();

    if (exp && isa<IntegerLiteral>(exp)) {
        IntegerLiteral *literal = cast<IntegerLiteral>(exp);
        if (literal->getValue() == 0 || literal->getValue() == 1) {
            std::string newExpr = literal->getValue() == 1 ? "0" : "1";
            SourceRange range = exp->getSourceRange();
            getRewriter().ReplaceText(range, newExpr);
            return true;
        }
    }

    return false;
  }

  bool VisitReturnStmt(clang::ReturnStmt *RS) {
    TheReturns.push_back(RS);
    return true;
  }

private:
  std::vector<clang::ReturnStmt *> TheReturns;
};

static RegisterMutator<InvertFunctionCondition> M("u1.InvertFunctionCondition", "Inverts the condition of a boolean function.");