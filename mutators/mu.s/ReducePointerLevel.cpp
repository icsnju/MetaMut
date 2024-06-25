#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <map>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReducePointerLevel
    : public Mutator,
      public clang::RecursiveASTVisitor<ReducePointerLevel> {

  using VisitorTy = clang::RecursiveASTVisitor<ReducePointerLevel>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
  bool TraverseUnaryOperator(clang::UnaryOperator *UO);

private:
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, std::vector<clang::DeclRefExpr *>> VarToExprs;
  std::map<clang::VarDecl *, std::vector<clang::UnaryOperator *>> VarToUos;
};

static RegisterMutator<ReducePointerLevel> M("s.reduce-pointer-level", "Reduce a variable's pointer level.");

bool ReducePointerLevel::VisitVarDecl(VarDecl *VD) {
  if (isMutationSite(VD) && VD->getType()->isPointerType())
    TheVars.push_back(VD);
  return true;
}

bool ReducePointerLevel::VisitDeclRefExpr(DeclRefExpr *DRE) {
  if (isMutationSite(DRE)) {
    VarDecl *var = dyn_cast_or_null<VarDecl>(DRE->getDecl());
    if (var) {
      VarToExprs[var].push_back(DRE);
    }
  }
  return true;
}

bool ReducePointerLevel::TraverseUnaryOperator(UnaryOperator *UO) {
  if (UO->getOpcode() == UO_Deref) {
    auto *expr = dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreParenImpCasts());
    if (expr) {
      VarDecl *VD = dyn_cast<VarDecl>(expr->getDecl());
      if (VD) {
        VarToUos[VD].push_back(UO);
        return true;
      }
    }
  }

  return VisitorTy::TraverseUnaryOperator(UO);
}

bool ReducePointerLevel::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty() || VarToExprs.empty())
    return false;

  VarDecl *var = randElement(TheVars);

  // Ensure the selected variable has corresponding DeclRefExprs
  if (VarToExprs.find(var) == VarToExprs.end())
    return false;

  // Reduce the pointer level of the variable
  if (var->getType()->isPointerType()) {
    clang::TypeLoc TL =
        var->getTypeSourceInfo()->getTypeLoc().getUnqualifiedLoc();
    clang::PointerTypeLoc PTL = TL.getAs<clang::PointerTypeLoc>();
    if (PTL) {
      clang::SourceLocation starLoc = PTL.getStarLoc();
      getRewriter().ReplaceText(starLoc, 1, ""); // remove '*'
    }
    if (var->getInit()) {
      std::string initExprStr = getSourceText(var->getInit());
      getRewriter().ReplaceText(var->getInit()->getSourceRange(),
                                "*" + initExprStr);
    }
  }

  // Replace all corresponding DeclRefExprs
  for (auto *expr : VarToExprs[var]) {
    SourceLocation startExprLoc = expr->getBeginLoc();
    getRewriter().InsertTextBefore(startExprLoc, "&"); // add '&'
  }

  // Replace all corresponding UnaryOperators
  for (auto *uo : VarToUos[var]) {
    getRewriter().ReplaceText(uo->getOperatorLoc(), 1, ""); // remove '*'
  }

  return true;
}
