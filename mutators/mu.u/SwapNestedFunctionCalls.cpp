#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapNestedFunctionCalls : public Mutator,
                                public clang::RecursiveASTVisitor<SwapNestedFunctionCalls> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCalls.empty() || TheCalls.size() < 2) return false;

    // Select two random function calls
    CallExpr *call1 = randElement(TheCalls);
    CallExpr *call2 = randElement(TheCalls);
    while (call1 == call2) {
      call2 = randElement(TheCalls);
    }

    // Get the source text for the two function calls
    std::string call1Text = getSourceText(call1);
    std::string call2Text = getSourceText(call2);

    // Swap the function calls
    getRewriter().ReplaceText(call1->getSourceRange(), call2Text);
    getRewriter().ReplaceText(call2->getSourceRange(), call1Text);

    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    TheCalls.push_back(CE);
    return true;
  }

private:
  std::vector<clang::CallExpr *> TheCalls;
};

static RegisterMutator<SwapNestedFunctionCalls> M("u1.SwapNestedFunctionCalls", "Swap the positions of two nested function calls.");