#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddInitialization : public Mutator,
                          public clang::RecursiveASTVisitor<AddInitialization> {

  using VisitorTy = clang::RecursiveASTVisitor<AddInitialization>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool TraverseFunctionDecl(clang::FunctionDecl *FD);
  bool VisitVarDecl(clang::VarDecl *VD);

private:
  std::vector<clang::VarDecl *> TheVars;
};

static RegisterMutator<AddInitialization> M("s.add-initialization",
      "Randomly add initialization to an uninitialized VarDecl.");

bool AddInitialization::TraverseFunctionDecl(clang::FunctionDecl *FD) {
  return VisitorTy::TraverseStmt(FD->getBody()); // skip parm
}

bool AddInitialization::VisitVarDecl(VarDecl *VD) {
  if (isMutationSite(VD) && !VD->hasInit())
    TheVars.push_back(VD);
  return true;
}

bool AddInitialization::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty())
    return false;

  VarDecl *selectedVD = randElement(TheVars);
  int initValue = 0;

  // Get the end location of the VarDecl name
  SourceLocation varEndLoc = getLocForEndOfToken(selectedVD->getLocation());
  TypeSourceInfo *TSI = selectedVD->getTypeSourceInfo();

  if (TSI)
    varEndLoc = getLocForEndOfToken(TSI->getTypeLoc().getEndLoc());

  // Insert the initialization after the VarDecl name
  getRewriter().InsertTextAfter(varEndLoc,
                                " = {" + std::to_string(initValue) + "}");

  return true;
}
