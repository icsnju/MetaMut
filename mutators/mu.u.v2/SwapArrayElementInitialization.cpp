#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapArrayElementInitialization : public Mutator,
                                       public clang::RecursiveASTVisitor<SwapArrayElementInitialization> {
  using VisitorTy = clang::RecursiveASTVisitor<SwapArrayElementInitialization>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheInitLists.empty()) return false;

    InitListExpr *initList = randElement(TheInitLists);
    if (initList->getNumInits() < 2) return false;

    // Randomly select two indices to swap
    unsigned idx1 = randIndex(initList->getNumInits());
    unsigned idx2 = idx1;
    while (idx2 == idx1) {
      idx2 = randIndex(initList->getNumInits());
    }

    Expr *init1 = initList->getInit(idx1);
    Expr *init2 = initList->getInit(idx2);
    SourceRange init1Range = init1->getSourceRange();
    SourceRange init2Range = init2->getSourceRange();

    // Swap the initializations
    getRewriter().ReplaceText(init1Range, getSourceText(init2));
    getRewriter().ReplaceText(init2Range, getSourceText(init1));

    return true;
  }

  bool VisitInitListExpr(clang::InitListExpr *ILE) {
    TheInitLists.push_back(ILE);
    return true;
  }

private:
  std::vector<clang::InitListExpr *> TheInitLists;
};

static RegisterMutator<SwapArrayElementInitialization> M(
    "u2.SwapArrayElementInitialization", "Swaps the values of two random elements within an array initialization.");
