#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomFunctionParam
    : public Mutator,
      public clang::RecursiveASTVisitor<AddRandomFunctionParam> {

  using VisitorTy = clang::RecursiveASTVisitor<AddRandomFunctionParam>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);

    // Generate new parameter type and name
    QualType newParamType = getASTContext().IntTy;
    std::string newParamName = generateUniqueName("new_param");

    // Update function declaration
    SourceRange paramRange = findBracesRange(func->getSourceRange().getBegin(), '(', ')');
    std::string updatedDecl = ", " + formatAsDecl(newParamType, newParamName);
    getRewriter().InsertTextBefore(paramRange.getEnd(), updatedDecl);

    // Update function calls
    for (CallExpr *call : TheFunctionCalls[func]) {
      SourceRange argRange = findBracesRange(call->getBeginLoc(), '(', ')');
      std::string updatedCall = ", 0";
      getRewriter().InsertTextBefore(argRange.getEnd(), updatedCall);
    }

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->isThisDeclarationADefinition() && FD->hasBody() && FD->getNameAsString() != "main") {
      TheFunctions.push_back(FD);
    }
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    FunctionDecl *callee = CE->getDirectCallee();
    if (callee) {
      TheFunctionCalls[callee].push_back(CE);
    }
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
  std::map<clang::FunctionDecl *, std::vector<clang::CallExpr *>> TheFunctionCalls;
};

static RegisterMutator<AddRandomFunctionParam> M(
    "u2.AddRandomFunctionParam",
    "Adds a new function parameter with a compatible type and initializes it with a random default value, updating all existing calls to the function with the new parameter.");
