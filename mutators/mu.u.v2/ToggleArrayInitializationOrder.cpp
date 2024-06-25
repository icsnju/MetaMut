#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleArrayInitializationOrder
    : public Mutator,
      public clang::RecursiveASTVisitor<ToggleArrayInitializationOrder> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheArrays.empty()) return false;

    InitListExpr *array = randElement(TheArrays);

    // Perform rewrite
    toggleInitListExprOrder(array);

    return true;
  }

  bool VisitInitListExpr(clang::InitListExpr *ILE) {
    TheArrays.push_back(ILE);
    return true;
  }

private:
  std::vector<clang::InitListExpr *> TheArrays;

  void toggleInitListExprOrder(clang::InitListExpr *ILE) {
    std::vector<Expr *> elements;
    for (unsigned i = ILE->getNumInits(); i > 0; --i) {
      elements.push_back(ILE->getInit(i - 1));
    }

    SourceRange range = getExpansionRange(ILE->getSourceRange());
    std::string newText = "{";
    for (unsigned i = 0; i < elements.size(); ++i) {
      newText += getSourceText(elements[i]);
      if (i < elements.size() - 1) newText += ", ";
    }
    newText += "}";

    getRewriter().ReplaceText(range, newText);
  }
};

static RegisterMutator<ToggleArrayInitializationOrder> M(
    "u2.ToggleArrayInitializationOrder", "Reverses the order of elements during array initialization.");
