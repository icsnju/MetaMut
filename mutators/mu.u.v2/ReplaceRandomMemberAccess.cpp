#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomMemberAccess
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceRandomMemberAccess> {

  using VisitorTy = clang::RecursiveASTVisitor<ReplaceRandomMemberAccess>;

public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());

    if (TheMemberExprs.empty()) return false;

    MemberExpr *memberExpr = randElement(TheMemberExprs);
    ValueDecl *oldMember = memberExpr->getMemberDecl();

    RecordDecl *record = dyn_cast<RecordDecl>(oldMember->getDeclContext());
    std::vector<ValueDecl *> validMembers;

    for (auto *decl : record->decls()) {
      if (FieldDecl *fieldDecl = dyn_cast<FieldDecl>(decl)) {
        if (fieldDecl != oldMember) {
          validMembers.push_back(fieldDecl);
        }
      }
    }

    if (validMembers.empty()) return false;

    ValueDecl *newMember = randElement(validMembers);
    getRewriter().ReplaceText(memberExpr->getMemberLoc(), oldMember->getNameAsString().length(), newMember->getNameAsString());

    return true;
  }

  bool VisitMemberExpr(clang::MemberExpr *ME) {
    TheMemberExprs.push_back(ME);
    return true;
  }

private:
  std::vector<clang::MemberExpr *> TheMemberExprs;
};

static RegisterMutator<ReplaceRandomMemberAccess> M(
    "u2.ReplaceRandomMemberAccess", "Modify a member access expression for a selected struct or class variable, replacing the accessed member with another valid member of the same instance.");
