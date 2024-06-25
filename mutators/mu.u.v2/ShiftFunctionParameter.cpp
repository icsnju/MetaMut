#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ShiftFunctionParameter : public Mutator,
                               public clang::RecursiveASTVisitor<ShiftFunctionParameter> {
  using Mutator::Mutator;

public:
  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (FD->getNumParams() > 1) {
      TheFunctions.push_back(FD);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *targetFunc = randElement(TheFunctions);
    unsigned int numParams = targetFunc->getNumParams();

    int idxA = randIndex(numParams);
    int idxB;
    do {
      idxB = randIndex(numParams);
    } while (idxA == idxB);

    ParmVarDecl *paramA = targetFunc->getParamDecl(idxA);
    ParmVarDecl *paramB = targetFunc->getParamDecl(idxB);

    std::string paramAStr = getSourceText(paramA);
    std::string paramBStr = getSourceText(paramB);

    getRewriter().ReplaceText(paramA->getSourceRange(), paramBStr);
    getRewriter().ReplaceText(paramB->getSourceRange(), paramAStr);

    CallCollector collector(targetFunc);
    collector.TraverseAST(getASTContext());
    for (auto call : collector.getCalls()) {
      Expr *argA = call->getArg(idxA);
      Expr *argB = call->getArg(idxB);

      std::string argAStr = getSourceText(argA);
      std::string argBStr = getSourceText(argB);

      getRewriter().ReplaceText(argA->getSourceRange(), argBStr);
      getRewriter().ReplaceText(argB->getSourceRange(), argAStr);
    }

    return true;
  }

private:
  std::vector<FunctionDecl *> TheFunctions;

  class CallCollector : public RecursiveASTVisitor<CallCollector> {
  public:
    CallCollector(FunctionDecl *FD) : targetFunc(FD) {}

    bool VisitCallExpr(CallExpr *CE) {
      if (CE->getDirectCallee() == targetFunc) {
        TheCalls.push_back(CE);
      }
      return true;
    }

    const std::vector<CallExpr *> &getCalls() const { return TheCalls; }

  private:
    FunctionDecl *targetFunc;
    std::vector<CallExpr *> TheCalls;
  };
};

static RegisterMutator<ShiftFunctionParameter> M(
    "u2.ShiftFunctionParameter", "Shift the order of function parameters and update all function calls with the new parameter order.");
