#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Type.h>
#include <clang/Basic/SourceManager.h>
#include <map>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class FlattenArray : public Mutator,
                     public clang::RecursiveASTVisitor<FlattenArray> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *ASE);

private:
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, std::vector<clang::ArraySubscriptExpr *>> VarUses;
  std::map<clang::VarDecl *, unsigned long long> ArraySizes;
};

static RegisterMutator<FlattenArray> M("s.flatten-array", "Flatten multi-dimensional arrays to one dimension.");

bool FlattenArray::VisitVarDecl(clang::VarDecl *VD) {
  if (isMutationSite(VD)) {
    const clang::ArrayType *AT = VD->getType()->getAsArrayTypeUnsafe();
    if (AT && AT->getElementType()->isConstantArrayType()) {
      // Calculate size of array
      unsigned long long totalSize = 1;
      const clang::ArrayType *currentType = AT;
      while (const clang::ConstantArrayType *CAT =
                 clang::dyn_cast<clang::ConstantArrayType>(currentType)) {
        totalSize *= CAT->getSize().getZExtValue();
        currentType = CAT->getElementType()->getAsArrayTypeUnsafe();
        if (!currentType)
          break;
      }
      ArraySizes[VD] = totalSize;
      TheVars.push_back(VD);
    }
  }
  return true;
}

bool FlattenArray::VisitArraySubscriptExpr(clang::ArraySubscriptExpr *ASE) {
  if (isMutationSite(ASE)) {
    clang::Expr *base = ASE->getBase();
    if (base && isa<clang::DeclRefExpr>(base)) {
      clang::Decl *decl = cast<clang::DeclRefExpr>(base)->getDecl();
      if (decl && isa<clang::VarDecl>(decl)) {
        VarUses[cast<clang::VarDecl>(decl)].push_back(ASE);
      }
    }
  }
  return true;
}

bool FlattenArray::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty())
    return false;

  clang::VarDecl *var = randElement(TheVars);
  std::string newName = generateUniqueName(var->getNameAsString());
  std::string newVarDeclString =
      var->getType().getAsString() + " " + newName +
      "[N];"; // Assuming N is the total size, you may need to calculate it

  getRewriter().ReplaceText(var->getSourceRange(), newVarDeclString);

  for (clang::ArraySubscriptExpr *ASE : VarUses[var]) {
    std::string subscriptExprString =
        newName + "[" + getSourceText(ASE->getIdx()) + " + " +
        getSourceText(ASE->getRHS()) + " + " +
        getSourceText(ASE->getLHS()) + "]";
    getRewriter().ReplaceText(ASE->getSourceRange(), subscriptExprString);
  }

  return true;
}
