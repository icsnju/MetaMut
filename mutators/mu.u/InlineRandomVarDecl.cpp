// InlineRandomVarDecl.cpp
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;

class InlineRandomVarDecl : public Mutator,
                            public clang::RecursiveASTVisitor<InlineRandomVarDecl> {
  using VisitorTy = clang::RecursiveASTVisitor<InlineRandomVarDecl>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheVars.empty()) return false;

    VarDecl *var = randElement(TheVars);

    // Ensure the VarDecl has an initializer
    if (!var->hasInit()) return false;

    // Get the source text of the initializer
    std::string initSrc = getSourceText(var->getInit());

    // Replace all occurrences of the variable with its initializer
    for (DeclRefExpr *ref : VarRefsMap[var]) {
      getRewriter().ReplaceText(ref->getSourceRange(), initSrc);
    }

    // Remove the VarDecl
    getRewriter().RemoveText(var->getSourceRange());

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    TheVars.push_back(VD);
    return true;
  }

  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
    if (isa<VarDecl>(DRE->getDecl())) {
      VarRefsMap[cast<VarDecl>(DRE->getDecl())].push_back(DRE);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, std::vector<clang::DeclRefExpr *>> VarRefsMap;
};

static RegisterMutator<InlineRandomVarDecl> M("u1.InlineRandomVarDecl", "Inline a random VarDecl's initializer and remove the declaration.");