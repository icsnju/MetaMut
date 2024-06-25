#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveRandomParameter : public Mutator,
                            public clang::RecursiveASTVisitor<RemoveRandomParameter> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());

    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);

    if (func->param_size() == 0) return false;

    unsigned paramToRemoveIndex = randIndex(func->param_size());
    ParmVarDecl *paramToRemove = func->getParamDecl(paramToRemoveIndex);

    // Remove parameter from function declaration
    std::string newFuncDeclStr = generateNewFuncDecl(func, paramToRemoveIndex);
    getRewriter().ReplaceText(func->getSourceRange(), newFuncDeclStr);

    // Remove parameter usage in function body
    Stmt *funcBody = func->getBody();
    for (auto it = funcBody->child_begin(); it != funcBody->child_end(); ++it) {
      if (DeclRefExpr *ref = dyn_cast<DeclRefExpr>(*it)) {
        if (ref->getDecl() == paramToRemove) {
          getRewriter().RemoveText(ref->getSourceRange());
        }
      }
    }

    // Update all call sites
    for (CallExpr *call : TheCalls[func]) {
      std::string newCallStr = generateNewCall(call, paramToRemoveIndex);
      getRewriter().ReplaceText(call->getSourceRange(), newCallStr);
    }

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (isMutationSite(FD))
      TheFunctions.push_back(FD);
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    if (FunctionDecl *FD = CE->getDirectCallee())
      TheCalls[FD].push_back(CE);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
  std::map<clang::FunctionDecl *, std::vector<clang::CallExpr *>> TheCalls;

  std::string generateNewFuncDecl(FunctionDecl *func, unsigned paramToRemoveIndex) {
    std::string newFuncDeclStr;
    llvm::raw_string_ostream ss(newFuncDeclStr);

    ss << func->getReturnType().getAsString() << " " << func->getNameAsString() << "(";

    for (unsigned i = 0; i < func->param_size(); ++i) {
      if (i != paramToRemoveIndex) {
        ss << func->getParamDecl(i)->getOriginalType().getAsString();
        ss << " ";
        ss << func->getParamDecl(i)->getNameAsString();
        if (i < func->param_size() - 1 && !(i == func->param_size() - 2 && paramToRemoveIndex == func->param_size() - 1)) 
          ss << ", ";
      }
    }
    ss << ") {}"; // Add empty function body

    return ss.str();
  }

  std::string generateNewCall(CallExpr *call, unsigned paramToRemoveIndex) {
    std::string newCallStr;
    llvm::raw_string_ostream ss(newCallStr);

    ss << call->getDirectCallee()->getNameAsString() << "(";

    for (unsigned i = 0; i < call->getNumArgs(); ++i) {
      if (i != paramToRemoveIndex) {
        ss << getSourceText(call->getArg(i));
        if (i < call->getNumArgs() - 1 && !(i == call->getNumArgs() - 2 && paramToRemoveIndex == call->getNumArgs() - 1)) 
          ss << ", ";
      }
    }
    ss << ")";

    return ss.str();
  }
};

static RegisterMutator<RemoveRandomParameter> M("u1.RemoveRandomParameter", "Randomly remove a non-essential parameter from a function declaration and adjust all call sites accordingly.");
