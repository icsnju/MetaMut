// LiftNestedFunction.cpp
#include <clang/AST/AST.h>
#include <clang/AST/ParentMap.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Basic/SourceManager.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class LiftNestedFunction
    : public Mutator,
      public clang::RecursiveASTVisitor<LiftNestedFunction> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    auto TU = getASTContext().getTranslationUnitDecl();
    TraverseAST(getASTContext());
    if (TheCalls.empty()) return false;

    CallExpr *call = randElement(TheCalls);
    std::string varName = generateUniqueName("liftedFunctionVar");
    QualType returnType = call->getCallReturnType(getASTContext());

    // Add new variable declaration
    std::string declStr = formatAsDecl(returnType, varName) + " = " + getSourceText(call) + ";\n";

    // Locate parent statement and insert before it
    const Stmt *parentStmt = PM->getParent(call);
    getRewriter().InsertTextBefore(parentStmt->getBeginLoc(), declStr);

    // Replace the original function call with the new variable
    getRewriter().ReplaceText(call->getSourceRange(), varName);

    return true;
  }

  bool TraverseFunctionDecl(FunctionDecl *FD) {
    if (FD->hasBody()) {
      PM = std::make_unique<ParentMap>(FD->getBody());
      return clang::RecursiveASTVisitor<LiftNestedFunction>::TraverseFunctionDecl(FD);
    }
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    const FunctionDecl *FD = CE->getDirectCallee();
    if (FD && FD->hasBody() && !FD->isMain()) {
      TheCalls.push_back(CE);
    }
    return true;
  }

private:
  std::vector<clang::CallExpr *> TheCalls;
  std::unique_ptr<ParentMap> PM;
};

static RegisterMutator<LiftNestedFunction> M("u1.LiftNestedFunction", 
    "Lifts a function call to the outer level and replaces the original location with a new variable.");