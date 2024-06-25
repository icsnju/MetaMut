#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleArrayElementSign
    : public Mutator,
      public clang::RecursiveASTVisitor<ToggleArrayElementSign> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheInitExprs.empty()) return false;

    auto initExpr = randElement(TheInitExprs);
    unsigned index = randIndex(initExpr->getNumInits());
    Expr *element = initExpr->getInit(index);

    SourceRange elementRange = element->getSourceRange();
    Expr::EvalResult result;
    if (element->EvaluateAsInt(result, getASTContext())) {
      llvm::APSInt val = result.Val.getInt();
      int64_t newVal = -val.getSExtValue();
      getRewriter().ReplaceText(elementRange, std::to_string(newVal));
      return true;
    }
    return false;
  }

  bool VisitInitListExpr(InitListExpr *ILE) {
    for (Expr *init : ILE->inits()) {
      if (isa<IntegerLiteral>(init)) {
        TheInitExprs.push_back(ILE);
        break; // one push for the entire InitListExpr is enough
      }
    }
    return true;
  }

private:
  std::vector<clang::InitListExpr *> TheInitExprs;
};

static RegisterMutator<ToggleArrayElementSign> M(
    "u2.ToggleArrayElementSign",
    "Toggle the sign of an array element initialization value.");
