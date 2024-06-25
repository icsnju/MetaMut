#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveRandomParameter
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveRandomParameter> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *targetFunc = randElement(TheFunctions);
    int paramCount = targetFunc->getNumParams();

    if (paramCount <= 0) return false;

    int removedParam = randIndex(paramCount);

    // Replace all uses of the removed parameter with a default value in the function body
    Stmt *funcBody = targetFunc->getBody();
    for (auto &use : TheUses[targetFunc][removedParam]) {
      QualType paramType = targetFunc->getParamDecl(removedParam)->getOriginalType();
      std::string defaultValue = getDefaultInitValue(paramType);
      getRewriter().ReplaceText(use, defaultValue);
    }

    return true;
  }

  std::string getDefaultInitValue(QualType paramType) {
    if (paramType->isIntegerType()) {
      return "0";
    } else if (paramType->isRealFloatingType()) {
      return "0.0";
    } else if (paramType->isPointerType()) {
      return "nullptr";
    } else if (paramType->isUnionType() || paramType->isStructureType()) {
      const RecordType *recordType = paramType->getAs<RecordType>();
      RecordDecl *recordDecl = recordType->getDecl();
      if (!recordDecl) return "{}";
      return "(" + paramType.getAsString() + "){}";
    } else {
      return "{}";
    }
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->isThisDeclarationADefinition() && FD->getNumParams() > 0) {
      TheFunctions.push_back(FD);
      unsigned numParams = FD->getNumParams();
      TheUses[FD].resize(numParams);
    }
    return true;
  }

  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
    if (ParmVarDecl *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl())) {
      TheUses[PVD->getDeclContext()][PVD->getFunctionScopeIndex()].push_back(DRE->getSourceRange());
    }
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
  std::map<clang::DeclContext *, std::vector<std::vector<clang::SourceRange>>> TheUses;
};

static RegisterMutator<RemoveRandomParameter> M(
    "u2.RemoveRandomParameter", "Replaces the value of a randomly chosen parameter with a default value in a function definition.");
