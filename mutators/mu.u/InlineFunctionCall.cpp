#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InlineFunctionCall
    : public Mutator,
      public RecursiveASTVisitor<InlineFunctionCall> {

public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (FunctionCalls.empty()) return false;

    CallExpr *call = randElement(FunctionCalls);

    // Get the called function
    FunctionDecl *f = call->getDirectCallee();
    if (!f || !f->doesThisDeclarationHaveABody()) return false;

    // Prepare the mapping from function parameters to actual arguments
    std::map<std::string, std::string> paramToArg;
    for (unsigned i = 0; i < call->getNumArgs(); ++i) {
      ParmVarDecl *param = f->getParamDecl(i);
      Expr *arg = call->getArg(i);
      paramToArg[param->getNameAsString()] = getSourceText(arg);
    }

    // Get the function body and replace parameters with arguments
    Stmt *body = f->getBody();
    std::string newBody = getSourceText(body);
    for (const auto &p : paramToArg) {
      size_t pos = newBody.find(p.first);
      while(pos != std::string::npos) {
        newBody.replace(pos, p.first.length(), p.second);
        pos = newBody.find(p.first, pos + p.second.length());
      }
    }

    // Remove 'return ' from the newBody
    size_t returnPos = newBody.find("return ");
    if (returnPos != std::string::npos) {
        newBody.erase(returnPos, 7);
    }

    // Remove '{' and '}' from the newBody
    newBody.erase(remove(newBody.begin(), newBody.end(), '{'), newBody.end());
    newBody.erase(remove(newBody.begin(), newBody.end(), '}'), newBody.end());
    
    // Perform replacement
    getRewriter().ReplaceText(call->getSourceRange(), newBody);

    return true;
  }

  bool VisitCallExpr(CallExpr *CE) {
    FunctionCalls.push_back(CE);
    return true;
  }

private:
  std::vector<CallExpr *> FunctionCalls;
};

static RegisterMutator<InlineFunctionCall> M("u1.InlineFunctionCall", "Inline a function call with its body.");