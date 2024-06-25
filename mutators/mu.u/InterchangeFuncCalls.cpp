// InterchangeFuncCalls.cpp
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include "MutatorManager.h"

using namespace clang;

class InterchangeFuncCalls : public Mutator,
                             public clang::RecursiveASTVisitor<InterchangeFuncCalls> {

  using VisitorTy = clang::RecursiveASTVisitor<InterchangeFuncCalls>;

public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCalls.size() < 2) return false;

    // Randomly select two distinct function calls
    size_t i = randIndex(TheCalls.size());
    size_t j = randIndex(TheCalls.size());
    while (j == i) { j = randIndex(TheCalls.size()); }
    
    CallExpr *call1 = TheCalls[i];
    CallExpr *call2 = TheCalls[j];

    // Get source texts
    std::string call1Text = getSourceText(call1);
    std::string call2Text = getSourceText(call2);

    // Perform swap
    getRewriter().ReplaceText(call1->getSourceRange(), call2Text);
    getRewriter().ReplaceText(call2->getSourceRange(), call1Text);

    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    if (isMutationSite(CE)) {
      TheCalls.push_back(CE);
    }
    return true;
  }

private:
  std::vector<clang::CallExpr *> TheCalls;
};

static RegisterMutator<InterchangeFuncCalls> M("u1.InterchangeFuncCalls", "Interchange positions of two function calls within a CompoundStmt");