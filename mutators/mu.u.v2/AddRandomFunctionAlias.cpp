#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomFunctionAlias
    : public Mutator,
      public clang::RecursiveASTVisitor<AddRandomFunctionAlias> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *selectedFunc = randElement(TheFunctions);
    std::string aliasName = generateUniqueName(selectedFunc->getNameAsString() + "_alias");

    QualType canonicalFuncType = selectedFunc->getType().getCanonicalType();
    std::string aliasDeclaration = "typedef " + formatAsDecl(canonicalFuncType, "(*" + aliasName + "_type)") + ";\n";
    aliasDeclaration += aliasName + "_type " + aliasName + " = &" + selectedFunc->getNameAsString() + ";\n";

    if (!addStringAfterStmt(selectedFunc->getBody(), "\n" + aliasDeclaration)) return false;

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (!FD->isMain() && FD->hasExternalFormalLinkage()) {
      TheFunctions.push_back(FD);
    }
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
};

static RegisterMutator<AddRandomFunctionAlias> M(
    "u2.AddRandomFunctionAlias", "Create a function alias for a randomly selected existing function.");
