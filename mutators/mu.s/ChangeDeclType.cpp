#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <llvm/ADT/DenseSet.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeDeclType : public Mutator,
                       public clang::RecursiveASTVisitor<ChangeDeclType> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitDecl(clang::Decl *D);

private:
  std::vector<clang::VarDecl *> TheVars;
  llvm::DenseSet<clang::QualType, llvm::DenseMapInfo<clang::QualType>> TheTypes;
};

static RegisterMutator<ChangeDeclType> M("s.change-decltype", "Change a VarDecl's type to a compliant one.");

bool ChangeDeclType::VisitVarDecl(VarDecl *VD) {
  if (isMutationSite(VD))
    TheVars.push_back(VD);
  return true;
}

bool ChangeDeclType::VisitDecl(Decl *D) {
  if (ValueDecl *V = dyn_cast<ValueDecl>(D))
    TheTypes.insert(V->getType());
  return true;
}

bool ChangeDeclType::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty())
    return false;

  VarDecl *oldVar = randElement(TheVars);
  QualType oldType = oldVar->getType();

  // Collect all compatible types
  std::vector<QualType> compatibleTypes;
  for (const auto &type : TheTypes) {
    if (type == oldType)
      continue;
    if (getCompilerInstance().getSema().CheckAssignmentConstraints(
            oldVar->getLocation(), type, oldType) ==
        Sema::AssignConvertType::Compatible) {
      compatibleTypes.push_back(type);
    }
  }

  if (compatibleTypes.empty())
    return false; // no suitable replacement found

  // Randomly select a new type
  QualType newType = randElement(compatibleTypes);

  // Replace the old type with the new one
  getRewriter().ReplaceText(
      oldVar->getTypeSourceInfo()->getTypeLoc().getSourceRange(),
      newType.getAsString());

  return true;
}
