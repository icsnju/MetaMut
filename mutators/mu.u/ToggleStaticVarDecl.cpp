#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleStaticVarDecl
    : public Mutator,
      public clang::RecursiveASTVisitor<ToggleStaticVarDecl> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheVars.empty()) return false;

    VarDecl *varDecl = randElement(TheVars);
    if(varDecl->isStaticLocal()) {
      // remove static keyword
      SourceLocation startLoc = varDecl->getBeginLoc();
      SourceLocation staticLoc = findStringLocationFrom(startLoc, "static");
      getRewriter().RemoveText(SourceRange(staticLoc, getLocForEndOfToken(staticLoc)));
    } else {
      // add static keyword
      SourceLocation startLoc = varDecl->getBeginLoc();
      getRewriter().InsertTextBefore(startLoc, "static ");
    }

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->isLocalVarDecl())
      TheVars.push_back(VD);
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheVars;
};

static RegisterMutator<ToggleStaticVarDecl> M("u1.ToggleStaticVarDecl", "Toggle static and non-static for VarDecl");