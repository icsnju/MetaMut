#include <algorithm>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <clang/Sema/Sema.h>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RemoveUnusedParameter
    : public Mutator,
      public clang::RecursiveASTVisitor<RemoveUnusedParameter> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitCallExpr(clang::CallExpr *CE);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE);

private:
  std::vector<clang::FunctionDecl *> TheFuncs;
  std::map<clang::FunctionDecl *, std::vector<clang::CallExpr *>> FuncToCalls;
  std::map<clang::ParmVarDecl *, int> ParamUsageCount;
};

static RegisterMutator<RemoveUnusedParameter> M("s.remove-unused-parameter", "Remove a function's unused parameter.");

bool RemoveUnusedParameter::VisitFunctionDecl(FunctionDecl *FD) {
  if (FD->getNumParams() > 0)
    TheFuncs.push_back(FD);
  return true;
}

bool RemoveUnusedParameter::VisitCallExpr(CallExpr *CE) {
  if (FunctionDecl *FD = CE->getDirectCallee()) {
    if (FD->getNumParams() > 0)
      FuncToCalls[FD].push_back(CE);
  }
  return true;
}

bool RemoveUnusedParameter::VisitDeclRefExpr(DeclRefExpr *DRE) {
  if (ParmVarDecl *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl())) {
    ParamUsageCount[PVD]++;
  }
  return true;
}

bool RemoveUnusedParameter::mutate() {
  TraverseAST(getASTContext());
  if (TheFuncs.empty())
    return false;

  std::shuffle(TheFuncs.begin(), TheFuncs.end(),
               getManager().getRandomGenerator());
  for (FunctionDecl *func : TheFuncs) {
    std::vector<unsigned> indices(func->getNumParams());
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, ..., n-1
    std::shuffle(indices.begin(), indices.end(),
                 getManager().getRandomGenerator());
    for (unsigned i : indices) {
      ParmVarDecl *param = func->getParamDecl(i);
      if (ParamUsageCount[param] == 0) { // Unused parameter found
        // Remove the parameter from function declaration
        SourceLocation Start =
            (i > 0 && i + 1 == func->getNumParams())
                ? getLocForEndOfToken(func->getParamDecl(i - 1)->getEndLoc())
                : param->getBeginLoc();
        SourceLocation End = i + 1 < func->getNumParams()
                                 ? getLocForEndOfToken(param->getEndLoc())
                                 : param->getEndLoc();
        getRewriter().RemoveText(SourceRange(Start, End));

        // Remove the corresponding argument from each call expression
        for (CallExpr *CE : FuncToCalls[func]) {
          Expr *arg = CE->getArg(i);
          SourceLocation argStart =
              (i > 0 && i + 1 == CE->getNumArgs())
                  ? getLocForEndOfToken(CE->getArg(i - 1)->getEndLoc())
                  : arg->getBeginLoc();
          SourceLocation argEnd = i + 1 < CE->getNumArgs()
                                      ? getLocForEndOfToken(arg->getEndLoc())
                                      : arg->getEndLoc();
          getRewriter().RemoveText(SourceRange(argStart, argEnd));
        }

        return true; // Stop after removing one unused parameter
      }
    }
  }

  return false;
}
