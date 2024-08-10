#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InlineArrayInitExpr : public Mutator,
                            public clang::RecursiveASTVisitor<InlineArrayInitExpr> {
  using VisitorTy = clang::RecursiveASTVisitor<InlineArrayInitExpr>;

public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheArrayInitExprs.empty()) {
      // llvm::errs() << "InlineArrayInitExpr: No suitable array initialization found.\n";
      return false;
    }

    DeclRefExpr *arrayInitExpr = randElement(TheArrayInitExprs);

    VarDecl *arrayVar = dyn_cast_or_null<VarDecl>(arrayInitExpr->getDecl());
    if (!arrayVar || !arrayVar->getInit()) return false;

    const auto *initExpr = dyn_cast<InitListExpr>(arrayVar->getInit());
    if (!initExpr) return false;

    std::vector<std::string> initElements;
    for (unsigned i = 0; i < initExpr->getNumInits(); ++i) {
      initElements.push_back(getSourceText(initExpr->getInit(i)));
    }

    for (auto *usage : TheArrayInitExprUsages[arrayInitExpr]) {
      std::string inlineElements = "(" + arrayVar->getType().getAsString() + "){" + llvm::join(initElements, ", ") + "}";
      getRewriter().ReplaceText(usage->getSourceRange(), inlineElements + "[" + getSourceText(usage->getIdx()) + "]");
    }

    return true;
  }

  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *ASE) {
    if (auto *DRE = dyn_cast<DeclRefExpr>(ASE->getBase()->IgnoreParenCasts())) {
      if (VarDecl *arrayVar = dyn_cast_or_null<VarDecl>(DRE->getDecl())) {
        const auto *initExpr = dyn_cast_or_null<InitListExpr>(arrayVar->getInit());
        if (initExpr) {
          TheArrayInitExprs.push_back(DRE);
          TheArrayInitExprUsages[DRE].push_back(ASE);
        }
      }
    }
    return true;
  }

private:
  std::vector<clang::DeclRefExpr *> TheArrayInitExprs;
  llvm::DenseMap<DeclRefExpr *, std::vector<ArraySubscriptExpr *>> TheArrayInitExprUsages;
};

static RegisterMutator<InlineArrayInitExpr> M(
    "u2.InlineArrayInitExpr", "Inline array initialization expression at usage locations.");
