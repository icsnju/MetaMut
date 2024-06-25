#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveUnusedVariable
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveUnusedVariable> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);

private:
  std::map<clang::VarDecl *, bool> TheVars;
};

static RegisterMutator<RemoveUnusedVariable> M("s.remove-unused-variable",
                                               "Remove an unused variable.");

// RemoveUnusedVariable.cc
bool RemoveUnusedVariable::VisitVarDecl(VarDecl *VD) {
  // Initialize each variable as unused
  if (TheVars.find(VD) == TheVars.end())
    TheVars[VD] = false;
  return true;
}

bool RemoveUnusedVariable::VisitDeclRefExpr(DeclRefExpr *DRE) {
  // Mark variable as used if it's referred to
  if (VarDecl *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
    TheVars[VD] = true;
  }
  return true;
}

bool RemoveUnusedVariable::mutate() {
  TraverseAST(getASTContext());
  // Filter out used variables
  std::vector<VarDecl *> UnusedVars;
  for (auto it = TheVars.begin(); it != TheVars.end(); ++it) {
    if (it->first && !it->second)
      UnusedVars.push_back(it->first);
  }
  if (UnusedVars.empty())
    return false;

  VarDecl *unusedVar = randElement(UnusedVars);
  getRewriter().ReplaceText(unusedVar->getSourceRange(), "");

  return true;
}
