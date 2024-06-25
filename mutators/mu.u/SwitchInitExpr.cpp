#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Decl.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;

class SwitchInitExpr : public Mutator,
                       public clang::RecursiveASTVisitor<SwitchInitExpr> {

  using VisitorTy = clang::RecursiveASTVisitor<SwitchInitExpr>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TheVars.clear();
    TraverseAST(getASTContext());
    if (TheVars.size() < 2) return false;

    // Randomly select two VarDecls with the same type
    VarDecl *var1 = randElement(TheVars);
    VarDecl *var2 = nullptr;
    std::vector<VarDecl *> candidates;

    for (auto var : TheVars) {
      if (var != var1 && var->getType().getCanonicalType() == var1->getType().getCanonicalType()) {
        candidates.push_back(var);
      }
    }

    if (candidates.empty()) return false;
    var2 = randElement(candidates);

    // Switch their init expressions
    std::string init1 = getSourceText(var1->getInit());
    std::string init2 = getSourceText(var2->getInit());
    SourceLocation loc1 = getLocForEndOfToken(var1->getEndLoc());
    SourceLocation loc2 = getLocForEndOfToken(var2->getEndLoc());

    getRewriter().ReplaceText(SourceRange(var1->getInit()->getBeginLoc(), loc1), init2 + ";");
    getRewriter().ReplaceText(SourceRange(var2->getInit()->getBeginLoc(), loc2), init1 + ";");

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->hasInit()) {
      TheVars.push_back(VD);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheVars;
};

static RegisterMutator<SwitchInitExpr> M("u1.SwitchInitExpr", "Switch initialization expressions of two VarDecls.");