#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DuplicateFunctionDecl
    : public Mutator,
      public clang::RecursiveASTVisitor<DuplicateFunctionDecl> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);

    std::string funcName = func->getNameInfo().getName().getAsString();
    std::string newFuncName = generateUniqueName(funcName);
    std::string funcDeclStr = formatAsDecl(func->getReturnType(), newFuncName);
    std::string funcParams;
    
    for (auto param = func->param_begin(); param != func->param_end(); ++param) {
        funcParams += formatAsDecl((*param)->getOriginalType(), (*param)->getNameAsString()) + ",";
    }

    if (!funcParams.empty()) {
      funcParams.pop_back(); // Remove trailing comma
    }

    std::string functionBody = getSourceText(func->getBody());
    std::string newFunction = funcDeclStr + "(" + funcParams + ") " + functionBody;

    addStringBeforeFunctionDecl(func, newFunction);

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheFunctions.push_back(FD);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
};

static RegisterMutator<DuplicateFunctionDecl> M(
    "u2.DuplicateFunctionDecl", "It duplicates a selected function declaration, creating a new function with a similar body and adjusted function name.");
