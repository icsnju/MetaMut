#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InvertArrayInitOrder
    : public Mutator,
      public clang::RecursiveASTVisitor<InvertArrayInitOrder> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheArrayInits.empty()) return false;

    InitListExpr *expr = randElement(TheArrayInits);

    // Perform the mutation
    std::vector<Expr *> invertedExprs;
    for (int i = expr->getNumInits() - 1; i >= 0; --i) {
      invertedExprs.push_back(expr->getInit(i));
    }

    std::string InvertedStr = "{";
    for (size_t i = 0; i < invertedExprs.size(); ++i) {
      InvertedStr += getSourceText(invertedExprs[i]);
      if (i < invertedExprs.size() - 1) {
        InvertedStr += ", ";
      }
    }
    InvertedStr += "}";

    getRewriter().ReplaceText(expr->getSourceRange(), InvertedStr);
    return true;
  }

  bool VisitInitListExpr(clang::InitListExpr *ILE) {
    TheArrayInits.push_back(ILE);
    return true;
  }

private:
  std::vector<clang::InitListExpr *> TheArrayInits;
};

static RegisterMutator<InvertArrayInitOrder> M(
    "u2.InvertArrayInitOrder", "Reverse the order of elements during the initialization of an array.");
