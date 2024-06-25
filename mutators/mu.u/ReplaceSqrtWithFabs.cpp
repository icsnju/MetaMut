#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceSqrtWithFabs
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceSqrtWithFabs> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (SqrtCalls.empty()) return false;

    CallExpr *call = randElement(SqrtCalls);

    SourceRange callRange = call->getSourceRange();
    std::string replacement = "fabs(";
    for (unsigned i = 0; i < call->getNumArgs(); ++i) {
      if (i != 0) replacement += ", ";
      replacement += getSourceText(call->getArg(i));
    }
    replacement += ")";
    getRewriter().ReplaceText(callRange, replacement);

    return true;
  }

  bool VisitCallExpr(CallExpr *CE) {
    if (CE->getDirectCallee() && CE->getDirectCallee()->getNameAsString() == "sqrt")
      SqrtCalls.push_back(CE);
    return true;
  }

private:
  std::vector<CallExpr *> SqrtCalls;
};

static RegisterMutator<ReplaceSqrtWithFabs> M("u1.ReplaceSqrtWithFabs", "Replace calls to the sqrt function with calls to the fabs function.");