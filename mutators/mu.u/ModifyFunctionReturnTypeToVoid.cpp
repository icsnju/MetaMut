#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyFunctionReturnTypeToVoid
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyFunctionReturnTypeToVoid> {
public:
  using Mutator::Mutator;

  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (!FD->getReturnType().getTypePtr()->isVoidType()) {
        TheFunctions.push_back(FD);
    }
    return true;
  }

  bool VisitReturnStmt(ReturnStmt *RS) {
    if (auto FD = TheFunctions.back()) {
        FuncReturns[FD].push_back(RS);
    }
    return true;
  }

  bool VisitCallExpr(CallExpr *CE) {
    if (auto FD = CE->getDirectCallee()) {
        FuncCalls[FD].push_back(CE);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());

    if (TheFunctions.empty()) {
        return false;
    }

    FunctionDecl *func = randElement(TheFunctions);

    // Change the return type to void
    QualType voidType = getASTContext().VoidTy;
    std::string voidTypeStr = formatAsDecl(voidType, "");

    SourceRange typeRange = func->getReturnTypeSourceRange();
    getRewriter().ReplaceText(typeRange, voidTypeStr);

    // Remove all return statements related to the function
    for (ReturnStmt *retStmt : FuncReturns[func]) {
        getRewriter().ReplaceText(retStmt->getSourceRange(), "");
    }

    // Replace all uses of the function's result with a default value
    for (CallExpr *call : FuncCalls[func]) {
        QualType returnType = func->getReturnType();
        std::string replaceText = returnType->isFloatingType() ? "0.0" : "0";
        getRewriter().ReplaceText(call->getSourceRange(), replaceText);
    }

    return true;
  }

private:
  std::vector<FunctionDecl *> TheFunctions;
  std::map<FunctionDecl *, std::vector<ReturnStmt *>> FuncReturns;
  std::map<FunctionDecl *, std::vector<CallExpr *>> FuncCalls;
};

static RegisterMutator<ModifyFunctionReturnTypeToVoid> M("u1.ModifyFunctionReturnTypeToVoid", "Change a function's return type to void, remove all return statements, and replace all uses of the function's result with a default value.");
