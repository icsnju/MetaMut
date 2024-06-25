#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <map>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DecaySmallStruct : public Mutator,
                         public clang::RecursiveASTVisitor<DecaySmallStruct> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitMemberExpr(clang::MemberExpr *ME);

private:
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, std::vector<clang::MemberExpr *>> VarUses;
};

static RegisterMutator<DecaySmallStruct> M("s.decay-small-struct",
                                           "Decay small struct to long long.");

bool DecaySmallStruct::VisitVarDecl(clang::VarDecl *VD) {
  if (isMutationSite(VD)) {
    auto type = VD->getType();
    if (!type->isIncompleteType() && type->isRecordType() &&
        getASTContext().getTypeSize(type) <= 64) {
      TheVars.push_back(VD);
    }
  }
  return true;
}

bool DecaySmallStruct::VisitMemberExpr(clang::MemberExpr *ME) {
  if (isMutationSite(ME)) {
    clang::ValueDecl *VD = ME->getMemberDecl();
    if (VD && isa<clang::FieldDecl>(VD)) {
      clang::Expr *base = ME->getBase();
      if (base && isa<clang::DeclRefExpr>(base)) {
        clang::Decl *decl = cast<clang::DeclRefExpr>(base)->getDecl();
        if (decl && isa<clang::VarDecl>(decl)) {
          VarUses[cast<clang::VarDecl>(decl)].push_back(ME);
        }
      }
    }
  }
  return true;
}

bool DecaySmallStruct::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty())
    return false;

  clang::VarDecl *var = randElement(TheVars);
  std::string newName = generateUniqueName(var->getNameAsString());
  std::string newVarDeclString = "long long " + newName + ";";

  getRewriter().ReplaceText(var->getSourceRange(), newVarDeclString);

  for (clang::MemberExpr *ME : VarUses[var]) {
    clang::FieldDecl *field = cast<clang::FieldDecl>(ME->getMemberDecl());
    const clang::RecordDecl *record = field->getParent();
    unsigned offsetInBits = getASTContext().getFieldOffset(field);
    unsigned offsetInBytes = offsetInBits / 8;

    std::string fieldString = "(*(" + field->getType().getAsString() +
                              "*) ((char*) &" + newName + " + " +
                              std::to_string(offsetInBytes) + "))";
    getRewriter().ReplaceText(ME->getSourceRange(), fieldString);
  }

  return true;
}
