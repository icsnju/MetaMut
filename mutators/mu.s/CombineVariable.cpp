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

class CombineVariable : public Mutator,
                        public clang::RecursiveASTVisitor<CombineVariable> {
public:
  using VisitorTy = clang::RecursiveASTVisitor<CombineVariable>;
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitVarDecl(clang::VarDecl *VD);
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);

private:
  clang::SourceLocation firstFunctionLoc;
  std::vector<clang::VarDecl *> TheVars;
  std::map<clang::VarDecl *, std::vector<clang::DeclRefExpr *>> varDeclToRefs;
};

static RegisterMutator<CombineVariable> M("s.combine-variable",
                                          "Combine variables into a struct.");

bool CombineVariable::VisitVarDecl(VarDecl *VD) {
  if (isMutationSite(VD))
    TheVars.push_back(VD);
  return true;
}

bool CombineVariable::VisitFunctionDecl(FunctionDecl *FD) {
  SourceManager &SM = getASTContext().getSourceManager();
  if (firstFunctionLoc.isInvalid() && SM.isInMainFile(FD->getLocation())) {
    auto *D = getMostRecentTranslationUnitDecl(FD);
    if (D)
      firstFunctionLoc = D->getBeginLoc();
  }
  return true;
}

bool CombineVariable::VisitDeclRefExpr(DeclRefExpr *DRE) {
  if (VarDecl *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
    varDeclToRefs[VD].push_back(DRE);
  }
  return true;
}

bool CombineVariable::mutate() {
  TraverseAST(getASTContext());

  if (TheVars.size() < 1)
    return false;

  // Shuffle and get a random number of variables
  std::random_shuffle(TheVars.begin(), TheVars.end());
  int n = getManager().randint(1, TheVars.size()); // Generate a random number
  std::vector<VarDecl *> varsToCombine(TheVars.begin(), TheVars.begin() + n);

  // Generate the combined struct
  std::string structName = generateUniqueName("CombinedStruct");
  std::string structDef = "struct " + structName + " {\n";
  for (VarDecl *var : varsToCombine) {
    structDef += formatAsDecl(var->getType(), var->getNameAsString()) + ";\n";
  }
  structDef += "};\n";

  // Generate the new variable declaration
  std::string newVarName = generateUniqueName("combinedVar");
  std::string newVarDef = "struct " + structName + " " + newVarName + ";\n";

  // Insert the struct definition and the new variable declaration
  getRewriter().InsertTextBefore(firstFunctionLoc, structDef + newVarDef);

  // Replace references to the original variables with references to the fields
  // of the new struct
  for (VarDecl *var : varsToCombine) {
    std::string oldVarName = var->getNameAsString();
    std::string newVarRef = newVarName + "." + oldVarName;

    // Replace the variable references
    for (DeclRefExpr *expr : varDeclToRefs[var]) {
      getRewriter().ReplaceText(expr->getSourceRange(), newVarRef);
    }
  }

  return true;
}
