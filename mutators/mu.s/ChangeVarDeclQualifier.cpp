#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <map>

#include "Mutator.h"
#include "MutatorManager.h"

class ChangeVarDeclQualifier
    : public Mutator,
      public clang::RecursiveASTVisitor<ChangeVarDeclQualifier> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);

private:
  std::vector<clang::VarDecl *> TheVars;
};

static RegisterMutator<ChangeVarDeclQualifier> M("s.change-vardecl-qualifier", "Change a variable's type qualifier.");

bool ChangeVarDeclQualifier::VisitVarDecl(clang::VarDecl *VD) {
  if (isMutationSite(VD)) TheVars.push_back(VD);
  return true;
}

bool ChangeVarDeclQualifier::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty()) return false;

  bool isMutated = false;
  clang::VarDecl *var = randElement(TheVars);
  clang::QualType oldType = var->getType();
  clang::Qualifiers oldQualifiers = oldType.getQualifiers();

  // If the type is const-qualified, remove the const qualifier.
  // If the type is not const-qualified, add the const qualifier.
  // Do the same for volatile and restrict qualifiers.
  clang::Qualifiers newQualifiers = oldQualifiers;
  if (randBool()) {
    isMutated = true;
    if (oldQualifiers.hasConst()) {
      newQualifiers.removeConst();
    } else {
      newQualifiers.addConst();
    }
  }

  if (randBool()) {
    isMutated = true;
    if (oldQualifiers.hasVolatile()) {
      newQualifiers.removeVolatile();
    } else {
      newQualifiers.addVolatile();
    }
  }

  if (randBool() && var->getType()->isPointerType()) {
    isMutated = true;
    if (oldQualifiers.hasRestrict()) {
      newQualifiers.removeRestrict();
    } else {
      newQualifiers.addRestrict();
    }
  }

  clang::QualType newType = getASTContext().getQualifiedType(
      oldType.getUnqualifiedType(), newQualifiers);
  std::string newVarDeclString;
  llvm::raw_string_ostream ostream(newVarDeclString);
  newType.print(
      ostream, getASTContext().getPrintingPolicy(), var->getNameAsString());
  ostream.flush();

  if (var->getInit())
    newVarDeclString =
        newVarDeclString + " = " + getSourceText(var->getInit());

  getRewriter().ReplaceText(var->getSourceRange(), newVarDeclString);

  return isMutated;
}
