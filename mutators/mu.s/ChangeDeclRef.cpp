#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <set>
#include <stack>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeDeclRef : public Mutator,
                      public clang::RecursiveASTVisitor<ChangeDeclRef> {

  using VisitorTy = clang::RecursiveASTVisitor<ChangeDeclRef>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);
  bool VisitVarDecl(clang::VarDecl *VD);
  bool TraverseCompoundStmt(clang::CompoundStmt *CS);
  bool TraverseFunctionDecl(clang::FunctionDecl *FD);

private:
  std::map<clang::DeclRefExpr *, std::vector<clang::VarDecl *>> ExprVarsMap;
  std::vector<clang::DeclRefExpr *> TheRefs;
  std::vector<std::vector<clang::VarDecl *>> TheVars;
};

static RegisterMutator<ChangeDeclRef> M("s.change-declref", "Change a DeclRefExpr's referenced variable.");

bool ChangeDeclRef::TraverseFunctionDecl(clang::FunctionDecl *FD) {
  TheVars.push_back(TheVars.back());
  VisitorTy::TraverseFunctionDecl(FD);
  TheVars.pop_back();
  return true;
}

bool ChangeDeclRef::TraverseCompoundStmt(CompoundStmt *CS) {
  if (!TheVars.empty()) {
    // Push a copy of the current set of variables
    TheVars.push_back(TheVars.back());
  } else {
    // Create the first set of variables
    TheVars.push_back({});
  }

  VisitorTy::TraverseCompoundStmt(CS);
  TheVars.pop_back();
  return true;
}

bool ChangeDeclRef::VisitDeclRefExpr(DeclRefExpr *DRE) {
  if (isMutationSite(DRE)) {
    TheRefs.push_back(DRE);
    ExprVarsMap[DRE] = TheVars.back();
  }
  return true;
}

bool ChangeDeclRef::VisitVarDecl(VarDecl *VD) {
  if (!TheVars.empty()) {
    TheVars.back().push_back(VD);
  }
  return true;
}

bool ChangeDeclRef::mutate() {
  TheVars.push_back({});
  TraverseAST(getASTContext());
  if (TheRefs.empty() || TheVars.empty())
    return false;

  DeclRefExpr *ref = randElement(TheRefs);
  if (!isa<VarDecl>(ref->getDecl()))
    return false; // Ensure the Decl is actually a VarDecl

  VarDecl *oldVar = cast<VarDecl>(ref->getDecl());
  if (ExprVarsMap[ref].size() == 0)
    return false;

  VarDecl *newVar = randElement(ExprVarsMap[ref]);
  if (oldVar == newVar)
    return false;

  // Replace the old variable with the new one
  getRewriter().ReplaceText(ref->getSourceRange(), newVar->getNameAsString());

  return true;
}
