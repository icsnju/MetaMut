#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AggregateMemberToScalarVariable
    : public Mutator,
      public clang::RecursiveASTVisitor<AggregateMemberToScalarVariable> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *ASE);

private:
  std::vector<clang::ArraySubscriptExpr *> TheArrays;
};

static RegisterMutator<AggregateMemberToScalarVariable> M("s.aggregate-member-to-scalar-variable",
      "Replace accesses to an aggregate member with a "
      "corresponding scalar variable.");

bool AggregateMemberToScalarVariable::VisitArraySubscriptExpr(
    ArraySubscriptExpr *ASE) {
  if (isMutationSite(ASE) && dyn_cast<DeclRefExpr>(ASE->getBase()->IgnoreImpCasts()))
    TheArrays.push_back(ASE);
  return true;
}

bool AggregateMemberToScalarVariable::mutate() {
  TraverseAST(getASTContext());
  if (TheArrays.empty())
    return false;

  ArraySubscriptExpr *arrayExpr = randElement(TheArrays);

  std::string baseName = getSourceText(arrayExpr->getBase());
  std::string indexName = getSourceText(arrayExpr->getIdx());

  // The scalar variable to replace the array access.
  std::string scalarVar = generateUniqueName(baseName);

  // Get the declaration of the array variable
  DeclRefExpr *arrayDeclRef =
      dyn_cast<DeclRefExpr>(arrayExpr->getBase()->IgnoreImpCasts());
  VarDecl *arrayDecl = dyn_cast<VarDecl>(arrayDeclRef->getDecl());

  // Get the location of the array variable declaration.
  SourceLocation locEnd = getDeclEndLoc(arrayDecl);

  // Get the type of the array variable's elements.
  QualType elementType = arrayExpr->getBase()->getType()->getPointeeType();

  std::string decl = formatAsDecl(elementType, scalarVar);

  // Check if the array variable is global
  if (arrayDecl->hasGlobalStorage() && !arrayDecl->isStaticLocal()) {
    // If the array is global, we can't initialize it with a non-constant expression.
    // We will just declare the scalar variable without initialization.
    decl += ";\n";
  } else {
    // If the array is not global, we can proceed with the initialization.
    decl += " = " + baseName + "[" + indexName + "];\n";
  }

  // Insert the scalar variable declaration at the location of the array declaration.
  getRewriter().InsertText(locEnd, decl);

  // Replace the array access with the scalar variable.
  getRewriter().ReplaceText(arrayExpr->getSourceRange(), scalarVar);

  return true;
}
