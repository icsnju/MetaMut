#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <map>
#include <string>
#include <vector>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DestructRecord : public Mutator,
                       public clang::RecursiveASTVisitor<DestructRecord> {
public:
  using VisitorTy = clang::RecursiveASTVisitor<DestructRecord>;
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitDeclStmt(clang::DeclStmt *DS);
  bool TraverseFunctionDecl(clang::FunctionDecl *FD);
  bool VisitMemberExpr(clang::MemberExpr *ME);

private:
  std::vector<clang::VarDecl *> TheVars;
  std::vector<clang::MemberExpr *> TheMembers;
  std::map<clang::Decl *, clang::DeclStmt *> declToStmt;
};

static RegisterMutator<DestructRecord> M("s.destruct-record",
      "Break struct/union variables into smaller variables.");

bool DestructRecord::TraverseFunctionDecl(clang::FunctionDecl *FD) {
  VisitorTy::TraverseFunctionDecl(FD);
  return true;
}

bool DestructRecord::VisitVarDecl(VarDecl *VD) {
  if (isMutationSite(VD) && VD->getType()->isRecordType()) {
    TheVars.push_back(VD);
  }
  return true;
}

bool DestructRecord::VisitDeclStmt(DeclStmt *DS) {
  for (auto it = DS->decl_begin(); it != DS->decl_end(); ++it) {
    declToStmt[*it] = DS;
  }
  return true;
}

bool DestructRecord::VisitMemberExpr(MemberExpr *ME) {
  if (isMutationSite(ME)) {
    TheMembers.push_back(ME);
  }
  return true;
}

bool DestructRecord::mutate() {
  TraverseAST(getASTContext());

  if (TheVars.empty() || TheMembers.empty()) {
    return false;
  }

  std::set<Decl *> TheVarSet;
  for (VarDecl *V : TheVars)
    TheVarSet.insert(V);

  for (auto it = declToStmt.begin(); it != declToStmt.end();)
    if (TheVarSet.find(it->first) == TheVarSet.end())
      it = declToStmt.erase(it);
    else
      ++it;

  VarDecl *var = randElement(TheVars);

  // Create new variables for each field in the record
  RecordDecl *record = var->getType()->getAs<RecordType>()->getDecl();
  std::string varName = var->getNameAsString();
  std::map<std::string, std::string> fieldToVar;
  std::string declStr;
  for (auto field : record->fields()) {
    std::string newVarName =
        generateUniqueName(varName + "_" + field->getNameAsString());

    // Generate and insert new variable declaration
    declStr += formatAsDecl(field->getType(), newVarName) + " = " + varName +
               "." + field->getNameAsString() + ";\n";

    fieldToVar[field->getNameAsString()] = newVarName;
  }

  // Insert at the correct location
  clang::DeclStmt *DS = declToStmt[var];
  getRewriter().ReplaceText(DS->getSourceRange(),
                            getSourceText(DS) + declStr);

  // Replace references to the record's fields with the corresponding smaller
  // variables
  for (auto ME : TheMembers) {
    if (DeclRefExpr *DRE =
            dyn_cast<DeclRefExpr>(ME->getBase()->IgnoreImpCasts())) {
      if (VarDecl *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
        if (VD == var) {
          std::string fieldName = ME->getMemberDecl()->getNameAsString();
          getRewriter().ReplaceText(ME->getSourceRange(),
                                    fieldToVar[fieldName]);
        }
      }
    }
  }

  return true;
}
