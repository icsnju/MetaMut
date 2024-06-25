#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <map>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeParamScope : public Mutator,
                         public clang::RecursiveASTVisitor<ChangeParamScope> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DR);

private:
  std::vector<clang::ParmVarDecl *> TheParams;
  std::map<clang::ParmVarDecl *, clang::FunctionDecl *> ParamLocs;
  std::map<clang::ParmVarDecl *, std::vector<clang::DeclRefExpr *>> ParamUses;
};

static RegisterMutator<ChangeParamScope> M("s.change-param-scope",
                                           "Change a parameter's scope.");

bool ChangeParamScope::VisitFunctionDecl(clang::FunctionDecl *FD) {
  if (isMutationSite(FD)) {
    for (auto P : FD->parameters()) {
      TheParams.push_back(P);
      ParamLocs[P] = FD;
    }
  }
  return true;
}

bool ChangeParamScope::VisitDeclRefExpr(clang::DeclRefExpr *DR) {
  if (isMutationSite(DR)) {
    ParmVarDecl *PVD = dyn_cast<ParmVarDecl>(DR->getDecl());
    if (PVD)
      ParamUses[PVD].push_back(DR);
  }
  return true;
}

bool ChangeParamScope::mutate() {
  TraverseAST(getASTContext());
  if (TheParams.empty())
    return false;

  clang::ParmVarDecl *param = randElement(TheParams);
  std::string newName = generateUniqueName(param->getNameAsString());
  std::string newParamDeclString;
  llvm::raw_string_ostream ostream(newParamDeclString);
  param->getType().print(ostream, getASTContext().getPrintingPolicy(), newName);
  ostream.flush();

  if (randBool()) {
    // Move parameter declaration to global scope
    Decl *D = getMostRecentTranslationUnitDecl(ParamLocs[param]);
    if (D) {
      clang::SourceLocation SL = cast<Decl>(D)->getBeginLoc();
      getRewriter().InsertTextBefore(SL, newParamDeclString + " = {0};\n");
    }
  } else {
    // Move parameter declaration to local scope
    if (FunctionDecl *FD =
            param->getDeclContext()->getNonClosureAncestor()->getAsFunction()) {
      if (FD->getBody()) {
        CompoundStmt *body = dyn_cast<CompoundStmt>(FD->getBody());
        if (body) {
          clang::SourceLocation SL = body->getLBracLoc();
          getRewriter().InsertTextAfter(SL.getLocWithOffset(1),
                                        newParamDeclString + " = {0};\n");
        }
      }
    }
  }

  for (DeclRefExpr *DR : ParamUses[param]) {
    getRewriter().ReplaceText(DR->getSourceRange(), newName);
  }

  return true;
}
