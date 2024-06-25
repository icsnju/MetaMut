#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveUnusedFields
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveUnusedFields> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFieldDecl(clang::FieldDecl *FD);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);

private:
  std::vector<clang::FieldDecl *> UnusedFields;
  std::set<const clang::FieldDecl *> UsedFields;
};

static RegisterMutator<RemoveUnusedFields> M("s.remove-unused-fields",
      "Remove unused fields of a struct or union type.");

bool RemoveUnusedFields::VisitFieldDecl(FieldDecl *FD) {
  if (isMutationSite(FD))
    UnusedFields.push_back(FD);
  return true;
}

bool RemoveUnusedFields::VisitDeclRefExpr(DeclRefExpr *DRE) {
  if (auto *FD = dyn_cast<FieldDecl>(DRE->getDecl())) {
    UsedFields.insert(FD);
  }
  return true;
}

bool RemoveUnusedFields::mutate() {
  TraverseAST(getASTContext());
  if (UnusedFields.empty())
    return false;

  // Filter out used fields
  UnusedFields.erase(std::remove_if(UnusedFields.begin(), UnusedFields.end(),
                                    [this](const FieldDecl *FD) {
                                      return UsedFields.count(FD) > 0;
                                    }),
                     UnusedFields.end());

  if (UnusedFields.empty())
    return false;

  FieldDecl *field = randElement(UnusedFields);

  // Rewrite the FieldDecl to remove it
  SourceRange fieldRange = field->getSourceRange();
  getRewriter().RemoveText(fieldRange);

  return true;
}
