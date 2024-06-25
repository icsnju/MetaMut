#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddSuffixUnaryOp : public Mutator,
                         public clang::RecursiveASTVisitor<AddSuffixUnaryOp> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);

private:
  std::vector<clang::DeclRefExpr *> TheRefs;
};

static RegisterMutator<AddSuffixUnaryOp> M("s.add-suffix-unaryop", "Randomly add a suffix unary operator.");

bool AddSuffixUnaryOp::VisitDeclRefExpr(DeclRefExpr *DRE) {
  if (isMutationSite(DRE) && DRE->isLValue())
    TheRefs.push_back(DRE);
  return true;
}

bool AddSuffixUnaryOp::mutate() {
  TraverseAST(getASTContext());
  if (TheRefs.empty())
    return false;

  DeclRefExpr *ref = randElement(TheRefs);
  if (!isa<VarDecl>(ref->getDecl()))
    return false; // Ensure the Decl is actually a VarDecl

  UnaryOperatorKind OpKind =
      randBool() ? UO_PostInc : UO_PostDec; // Randomly select a suffix
  ExprResult result = getCompilerInstance().getSema().CreateBuiltinUnaryOp(
      ref->getExprLoc(), OpKind, ref);

  if (result.isInvalid())
    return false; // The operation was semantically invalid

  // Add the suffix to the variable
  getRewriter().ReplaceText(ref->getSourceRange(),
                            getSourceText(ref) +
                                (OpKind == UO_PostInc ? "++" : "--"));

  return true;
}
