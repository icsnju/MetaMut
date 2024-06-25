#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyBuiltinFunctionArguments
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyBuiltinFunctionArguments> {

  using VisitorTy = clang::RecursiveASTVisitor<ModifyBuiltinFunctionArguments>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctionCalls.empty()) return false;

    CallExpr *funcCall = randElement(TheFunctionCalls);
    unsigned numArgs = funcCall->getNumArgs();
    
    if (numArgs == 0) return false;

    // Randomly select an argument to modify
    unsigned randArgIndex = randIndex(numArgs);
    Expr *randArg = funcCall->getArg(randArgIndex);
    SourceRange argRange = randArg->getSourceRange();

    // Generate a new value for the selected argument
    std::string newValue;
    QualType argType = randArg->getType();
    if (argType->isIntegerType()) {
      newValue = std::to_string(rand() % 100);
    } else if (argType->isRealFloatingType()) {
      newValue = std::to_string(static_cast<double>(rand() % 10000) / 100);
    } else if (argType->isPointerType()) {
      newValue = "NULL";
    } else {
      return false; // Unsupported argument type
    }

    getRewriter().ReplaceText(argRange, newValue);
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    FunctionDecl *callee = CE->getDirectCallee();
    if (!callee) return true; // Ignore indirect function calls

    if (callee->isOverloadedOperator())
      return true; // Ignore overloaded operators

    if (callee->getBuiltinID() != 0 || callee->isInStdNamespace()) {
      TheFunctionCalls.push_back(CE);
    }

    return true;
  }

private:
  std::vector<clang::CallExpr *> TheFunctionCalls;
};

static RegisterMutator<ModifyBuiltinFunctionArguments> M(
    "u2.ModifyBuiltinFunctionArguments",
    "It takes a call to a builtin function and modifies one or more of its arguments, while maintaining argument compatibility.");
