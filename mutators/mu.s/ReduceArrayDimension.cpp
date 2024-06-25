#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReduceArrayDimension
    : public Mutator,
      public clang::RecursiveASTVisitor<ReduceArrayDimension> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *ASE);
  bool VisitVarDecl(clang::VarDecl *VD);

private:
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, std::vector<clang::ArraySubscriptExpr *>>
      VarToExprs;
};

static RegisterMutator<ReduceArrayDimension> M("s.reduce-array-dimension",
      "Change a dimension n array variable to dimension n-1 array.");

bool ReduceArrayDimension::VisitArraySubscriptExpr(ArraySubscriptExpr *ASE) {
  if (isMutationSite(ASE)) {
    auto *DRE = llvm::dyn_cast<DeclRefExpr>(ASE->getBase()->IgnoreImpCasts());
    if (!DRE)
      return true;

    auto *VD = llvm::dyn_cast<VarDecl>(DRE->getDecl());
    if (!VD || !VD->getType()->isArrayType())
      return true;

    VarToExprs[VD].push_back(ASE);
  }
  return true;
}

bool ReduceArrayDimension::VisitVarDecl(VarDecl *VD) {
  if (VD->getType()->isArrayType())
    TheVars.push_back(VD);
  return true;
}

bool ReduceArrayDimension::mutate() {
  TraverseAST(getASTContext());
  if (VarToExprs.empty() || TheVars.empty())
    return false;

  VarDecl *oldVar = randElement(TheVars);
  if (!oldVar->getType()->isArrayType())
    return false;

  auto it = VarToExprs.find(oldVar);
  if (it == VarToExprs.end())
    return false;

  bool isMutationSuccessful = false;
  for (auto *ref : it->second) {
    // Replace the array with the base variable
    getRewriter().ReplaceText(ref->getSourceRange(), oldVar->getNameAsString());
    isMutationSuccessful = true;
  }

  clang::SourceRange firstArrayRange =
      findBracesRange(oldVar->getLocation(), '[', ']');
  if (firstArrayRange.getBegin().isValid() &&
      firstArrayRange.getEnd().isValid()) {
    getRewriter().ReplaceText(firstArrayRange, "");
    isMutationSuccessful = true;
  }

  return isMutationSuccessful;
}
