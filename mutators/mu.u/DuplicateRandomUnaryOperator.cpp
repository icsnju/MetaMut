// DuplicateRandomUnaryOperator.cpp
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;

class DuplicateRandomUnaryOperator : public Mutator,
                                    public clang::RecursiveASTVisitor<DuplicateRandomUnaryOperator> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheUnaryOperators.empty()) return false;

    UnaryOperator *expr = randElement(TheUnaryOperators);
    SourceRange range = expr->getSourceRange();

    // Get the source text for the unary operator and its operand
    std::string sourceText = getSourceText(expr);

    // Duplicate the unary operator expression by inserting it after the original place
    getRewriter().InsertTextAfter(getLocForEndOfToken(range.getEnd()), sourceText);

    return true;
  }

  bool VisitUnaryOperator(clang::UnaryOperator *UO) {
    TheUnaryOperators.push_back(UO);
    return true;
  }

private:
  std::vector<clang::UnaryOperator *> TheUnaryOperators;
};

static RegisterMutator<DuplicateRandomUnaryOperator> M("u1.DuplicateRandomUnaryOperator", "Duplicate randomly selected UnaryOperator.");