// ToggleReturnExpr.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleReturnExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<ToggleReturnExpr> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheReturns.empty()) return false;

    ReturnStmt *retStmt = randElement(TheReturns);
    Expr *retExpr = retStmt->getRetValue();
    if (!retExpr) return false;

    QualType retType = retExpr->getType();
    std::string defaultRetValue = getDefaultReturnValue(retType);

    getRewriter().ReplaceText(retExpr->getSourceRange(), defaultRetValue);

    return true;
  }

  bool VisitReturnStmt(clang::ReturnStmt *RS) {
    TheReturns.push_back(RS);
    return true;
  }

private:
  std::vector<clang::ReturnStmt *> TheReturns;

  std::string getDefaultReturnValue(QualType ty) {
    if (ty->isPointerType()) {
      return "NULL";
    } else if (ty->isBooleanType()) {
      return "false";
    } else if (ty->isIntegralType(getASTContext())) {
      return "0";
    } else if (ty->isRealFloatingType()) {
      return "0.0";
    } else {
      return "{}"; // for structs, classes or arrays
    }
  }
};

static RegisterMutator<ToggleReturnExpr> M("u1.ToggleReturnExpr", "Toggle return expression to default value.");