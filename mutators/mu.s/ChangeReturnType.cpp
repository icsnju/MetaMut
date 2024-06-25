#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <llvm/ADT/DenseSet.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeReturnType : public Mutator,
                         public clang::RecursiveASTVisitor<ChangeReturnType> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitDecl(clang::Decl *D);

private:
  std::vector<clang::FunctionDecl *> TheFuncs;
  llvm::DenseSet<clang::QualType, llvm::DenseMapInfo<clang::QualType>> TheTypes;
};

static RegisterMutator<ChangeReturnType> M("s.change-returntype",
      "Change a FunctionDecl's return type to a compliant one.");

bool ChangeReturnType::VisitFunctionDecl(FunctionDecl *FD) {
  if (isMutationSite(FD))
    TheFuncs.push_back(FD);
  return true;
}

bool ChangeReturnType::VisitDecl(Decl *D) {
  if (ValueDecl *VD = dyn_cast<ValueDecl>(D))
    TheTypes.insert(VD->getType());
  return true;
}

bool ChangeReturnType::mutate() {
  TraverseAST(getASTContext());
  if (TheFuncs.empty())
    return false;

  FunctionDecl *oldFunc = randElement(TheFuncs);
  QualType oldType = oldFunc->getReturnType();

  // Collect all compatible types
  std::vector<QualType> compatibleTypes;
  for (const auto &type : TheTypes) {
    if (type == oldType)
      continue;
    if (getCompilerInstance().getSema().CheckAssignmentConstraints(
            oldFunc->getLocation(), type, oldType) ==
        Sema::AssignConvertType::Compatible) {
      compatibleTypes.push_back(type);
    }
  }

  if (compatibleTypes.empty())
    return false; // no suitable replacement found

  // Randomly select a new type
  QualType newType = randElement(compatibleTypes);

  // Replace the old type with the new one
  getRewriter().ReplaceText(oldFunc->getReturnTypeSourceRange(),
                            newType.getAsString());

  return true;
}
