#include "Mutator.h"
#include "MutatorManager.h"

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

using namespace clang;

class DecayRandomFunctionToPointer : public Mutator,
                                     public clang::RecursiveASTVisitor<DecayRandomFunctionToPointer> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);

    std::string newFuncName = func->getNameAsString() + "_ptr";
    std::string decl = formatAsDecl(func->getReturnType(), "(*" + newFuncName + ")");
    decl += "(";
    for (unsigned i = 0; i < func->getNumParams(); ++i) {
        if (i != 0) decl += ", ";
        decl += func->getParamDecl(i)->getType().getAsString();
    }
    decl += ") = " + func->getNameAsString() + ";\n";

    getRewriter().InsertTextAfter(getLocForEndOfToken(func->getEndLoc()), decl);

    for (auto call : TheCalls) {
      if (call->getDirectCallee() == func) {
        std::string callText = "(*" + newFuncName + ")(";
        for (unsigned i = 0; i < call->getNumArgs(); ++i) {
          if (i != 0) callText += ", ";
          callText += Lexer::getSourceText(CharSourceRange::getTokenRange(
                        call->getArg(i)->getExprLoc(),
                        getLocForEndOfToken(call->getArg(i)->getEndLoc())),
                        getRewriter().getSourceMgr(), getASTContext().getLangOpts());
        }
        callText += ")";
        getRewriter().ReplaceText(call->getSourceRange(), callText);
      }
    }

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->getNameAsString() != "main") {
        TheFunctions.push_back(FD);
    }
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    TheCalls.push_back(CE);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
  std::vector<clang::CallExpr *> TheCalls;
};

static RegisterMutator<DecayRandomFunctionToPointer> M("u1.DecayRandomFunctionToPointer", "Decays random functions to pointers and adjusts call sites accordingly.");