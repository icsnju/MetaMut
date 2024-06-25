#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class StripReturn : public Mutator,
                    public clang::RecursiveASTVisitor<StripReturn> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitReturnStmt(clang::ReturnStmt *RS);

private:
  std::vector<clang::FunctionDecl *> TheFuncs;
  std::vector<clang::ReturnStmt *> TheReturns;
};

static RegisterMutator<StripReturn> M("s.strip-return",
      "Change a function's return type to void and strip return keyword.");

bool StripReturn::VisitFunctionDecl(FunctionDecl *FD) {
  if (isMutationSite(FD) && !FD->getReturnType()->isVoidType()) {
    TheFuncs.push_back(FD);
  }
  return true;
}

bool StripReturn::VisitReturnStmt(ReturnStmt *RS) {
  if (isMutationSite(RS))
    TheReturns.push_back(RS);
  return true;
}

bool StripReturn::mutate() {
  TraverseAST(getASTContext());
  if (TheFuncs.empty())
    return false;

  // Change random function's return type to void
  FunctionDecl *funcDecl = randElement(TheFuncs);
  SourceRange returnTypeRange = funcDecl->getReturnTypeSourceRange();
  getRewriter().ReplaceText(returnTypeRange, "void");

  // Traverse the function and find return statements
  TheReturns.clear();
  TraverseDecl(funcDecl);
  for (ReturnStmt *returnStmt : TheReturns) {
    SourceRange returnRange(returnStmt->getReturnLoc(),
                            returnStmt->getReturnLoc());
    getRewriter().ReplaceText(returnRange, "");
  }

  return true;
}
