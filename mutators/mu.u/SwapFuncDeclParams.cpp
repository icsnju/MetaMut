#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;

class SwapFuncDeclParams: public Mutator, public clang::RecursiveASTVisitor<SwapFuncDeclParams> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFuncDecls.empty()) return false;

    FunctionDecl *funcDecl = randElement(TheFuncDecls);

    // Ensure function has at least two parameters
    if (funcDecl->param_size() < 2)
      return false;

    // Choose two different parameters to swap
    unsigned i = randIndex(funcDecl->param_size());
    unsigned j;
    do {
      j = randIndex(funcDecl->param_size());
    } while (j == i);

    // Swap parameters in function declaration
    ParmVarDecl* paramI = funcDecl->getParamDecl(i);
    ParmVarDecl* paramJ = funcDecl->getParamDecl(j);
    
    std::string declI = getSourceText(paramI);
    std::string declJ = getSourceText(paramJ);
    
    getRewriter().ReplaceText(paramI->getSourceRange(), declJ);
    getRewriter().ReplaceText(paramJ->getSourceRange(), declI);

    // Swap parameters in each call site
    for (auto callExpr : CallExprMap[funcDecl]) {
      if (callExpr->getNumArgs() < 2)
        continue;
      
      Expr* argI = callExpr->getArg(i);
      Expr* argJ = callExpr->getArg(j);
      
      std::string argIstr = getSourceText(argI);
      std::string argJstr = getSourceText(argJ);
      
      getRewriter().ReplaceText(argI->getSourceRange(), argJstr);
      getRewriter().ReplaceText(argJ->getSourceRange(), argIstr);
    }

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheFuncDecls.push_back(FD);
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    if (FunctionDecl* FD = CE->getDirectCallee())
      CallExprMap[FD].push_back(CE);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFuncDecls;
  std::map<clang::FunctionDecl *, std::vector<clang::CallExpr *>> CallExprMap;
};

static RegisterMutator<SwapFuncDeclParams> M("u1.SwapFuncDeclParams", "Swap two parameters in a function declaration and its call sites");