#include <algorithm>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <random>
#include <string>
#include <vector>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DuplicateParameter
    : public Mutator,
      public clang::RecursiveASTVisitor<DuplicateParameter> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitCallExpr(clang::CallExpr *CE);

private:
  std::vector<clang::FunctionDecl *> TheFuncs;
  std::map<clang::FunctionDecl *, std::vector<clang::CallExpr *>> FuncToCalls;
};

static RegisterMutator<DuplicateParameter> M("s.duplicate-parameter", "Duplicate a function's parameter.");

bool DuplicateParameter::VisitFunctionDecl(FunctionDecl *FD) {
  if (FD && FD->getNumParams() > 0)
    TheFuncs.push_back(FD);
  return true;
}

bool DuplicateParameter::VisitCallExpr(CallExpr *CE) {
  if (FunctionDecl *FD = CE->getDirectCallee()) {
    if (FD->getNumParams() > 0)
      FuncToCalls[FD].push_back(CE);
  }
  return true;
}

bool DuplicateParameter::mutate() {
  TraverseAST(getASTContext());
  if (TheFuncs.empty())
    return false;

  FunctionDecl *func = randElement(TheFuncs);

  unsigned index = rand() % func->getNumParams();
  ParmVarDecl *param = func->getParamDecl(index);
  if (!param->getType().getTypePtr()) return false;

  // Generate unique name for the duplicated parameter
  std::string newName = generateUniqueName(param->getNameAsString());

  // Create the new parameter declaration
  std::string newParamDecl;
  llvm::raw_string_ostream ostream(newParamDecl);
  param->getType().print(ostream, getASTContext().getPrintingPolicy(), newName);
  ostream.flush();

  // Add it to the function declaration
  clang::SourceLocation endLoc =
      param->getEndLoc().getLocWithOffset(param->getNameAsString().size());
  getRewriter().InsertTextAfter(endLoc, ", " + newParamDecl);

  // Duplicate corresponding CallExpr argument
  for (auto *CE : FuncToCalls[func]) {
    Expr *arg = CE->getArg(index);
    std::string argStr = getSourceText(arg);
    getRewriter().ReplaceText(arg->getSourceRange(), argStr + ", " + argStr);
  }

  return true;
}
