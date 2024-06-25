#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <map>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeVarDeclScope
    : public Mutator,
      public clang::RecursiveASTVisitor<ChangeVarDeclScope> {
  using VisitorTy = clang::RecursiveASTVisitor<ChangeVarDeclScope>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DR);
  bool TraverseFunctionDecl(clang::FunctionDecl *FD);

private:
  clang::FunctionDecl *CurrFD = nullptr;
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, clang::FunctionDecl *> VarLocs;
  std::map<clang::DeclRefExpr *, clang::FunctionDecl *> DRLocs;
  std::map<clang::VarDecl *, std::vector<clang::DeclRefExpr *>> VarUses;
};

static RegisterMutator<ChangeVarDeclScope> M("s.change-vardecl-scope",
                                             "Change a variable's scope.");

bool ChangeVarDeclScope::VisitVarDecl(clang::VarDecl *VD) {
  VarLocs[VD] = CurrFD;
  if (isMutationSite(VD))
    if (!isa<ParmVarDecl>(VD))
      TheVars.push_back(VD);
  return true;
}

bool ChangeVarDeclScope::VisitDeclRefExpr(clang::DeclRefExpr *DR) {
  DRLocs[DR] = CurrFD;
  if (isMutationSite(DR)) {
    VarDecl *VD = dyn_cast<VarDecl>(DR->getDecl());
    if (VD)
      VarUses[VD].push_back(DR);
  }
  return true;
}

bool ChangeVarDeclScope::TraverseFunctionDecl(clang::FunctionDecl *FD) {
  CurrFD = FD;
  VisitorTy::TraverseFunctionDecl(FD);
  CurrFD = nullptr;
  return true;
}

bool ChangeVarDeclScope::mutate() {
  TraverseAST(getASTContext());
  if (TheVars.empty())
    return false;

  clang::VarDecl *VD = randElement(TheVars);
  std::string newName = generateUniqueName(VD->getNameAsString());
  std::string newVarDeclString;
  llvm::raw_string_ostream ostream(newVarDeclString);
  VD->getType().print(ostream, getASTContext().getPrintingPolicy(), newName);
  ostream.flush();

  bool replaceSuccess = false;

  // Move variable declaration to a different scope
  if (VD->isLocalVarDecl() || VD->isLocalVarDeclOrParm()) {
    // Move local variable to global scope
    // Find the first function and insert declaration before it
    auto *TU = getMostRecentTranslationUnitDecl(VarLocs[VD]);
    if (TU) {
      clang::SourceLocation SL = TU->getBeginLoc();
      getRewriter().InsertTextBefore(SL, newVarDeclString + ";\n");
      replaceSuccess = true;
    }
  } else if (VD->isFileVarDecl()) {
    // Move global variable to local scope
    // Insert a new variable declaration to functions that use this variable
    std::set<FunctionDecl *> MeetFDs;
    for (DeclRefExpr *DR : VarUses[VD]) {
      auto FD = DRLocs[DR];
      if (MeetFDs.find(FD) != MeetFDs.end())
        continue;
      MeetFDs.insert(FD);
      if (FD && FD->getBody()) {
        auto ILoc = dyn_cast<CompoundStmt>(FD->getBody())
                        ->getLBracLoc()
                        .getLocWithOffset(1);
        getRewriter().InsertTextBefore(ILoc, newVarDeclString + ";\n");
        replaceSuccess = true;
      }
    }
  }

  for (DeclRefExpr *DR : VarUses[VD]) {
    getRewriter().ReplaceText(DR->getSourceRange(), newName);
  }

  // Remove old variable declaration
  // auto &SM = getASTContext().getSourceManager();
  // getRewriter().RemoveText(VD->getSourceRange());
  return true;
}
