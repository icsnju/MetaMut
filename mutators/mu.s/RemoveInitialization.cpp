#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveInitialization
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveInitialization> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);

private:
  std::vector<clang::VarDecl *> TheVars;
};

static RegisterMutator<RemoveInitialization> M("s.remove-initialization",
      "Randomly remove the initialization of a VarDecl.");

bool RemoveInitialization::VisitVarDecl(VarDecl *VD) {
  if (isMutationSite(VD) && VD->hasInit())
    TheVars.push_back(VD);
  return true;
}

bool RemoveInitialization::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty())
    return false;

  VarDecl *selectedVD = randElement(TheVars);

  // Retrieve the source range of the initialization
  SourceRange initRange = selectedVD->getInit()->getSourceRange();

  // Get the end location of the VarDecl name and the start location of the
  // initialization
  SourceLocation varEndLoc = getLocForEndOfToken(selectedVD->getLocation());
  SourceLocation assignLoc = findStringLocationFrom(varEndLoc, "=");

  // Calculate the range to be deleted, including the equals sign
  SourceRange deleteRange = SourceRange(assignLoc, initRange.getEnd());

  getRewriter().RemoveText(deleteRange);

  return true;
}
