#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

// this is mutator
class MutateVariableInitializationToRandomFunctionCall : public Mutator,
                      public clang::RecursiveASTVisitor<MutateVariableInitializationToRandomFunctionCall> {
  
  using VisitorTy = clang::RecursiveASTVisitor<MutateVariableInitializationToRandomFunctionCall>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheVars.empty() || TheFuncs.empty()) return false;

    VarDecl *var = randElement(TheVars);
    FunctionDecl *func = randElement(TheFuncs);

    // Check if the function's return type is compatible with the variable's type
    if (!getCompilerInstance().getSema().Context.hasSameType(var->getType(), func->getReturnType())) return false;

    // Replace the variable initialization with a function call
    std::string funcCall = func->getNameAsString() + "()";
    getRewriter().ReplaceText(var->getInit()->getSourceRange(), funcCall);

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->hasInit()) {
      TheVars.push_back(VD);
    }
    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->doesThisDeclarationHaveABody()) {
      TheFuncs.push_back(FD);
    }
    return true;
  }
  
private:
  std::vector<clang::VarDecl *> TheVars;
  std::vector<clang::FunctionDecl *> TheFuncs;
};

static RegisterMutator<MutateVariableInitializationToRandomFunctionCall> M("u1.MutateVariableInitializationToRandomFunctionCall", "Replace variable initialization with a random function call.");