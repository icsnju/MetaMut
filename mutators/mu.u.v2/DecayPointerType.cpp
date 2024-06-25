#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DecayPointerType
    : public Mutator,
      public clang::RecursiveASTVisitor<DecayPointerType> {
public:
  using Mutator::Mutator;

  bool VisitUnaryOperator(clang::UnaryOperator *UO) {
    if (UO->getOpcode() == UO_AddrOf) {
      TheOperators.push_back(UO);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOperators.empty()) return false;

    UnaryOperator *expr = randElement(TheOperators);

    // Perform mutation
    SourceLocation opLoc = expr->getOperatorLoc();
    getRewriter().RemoveText(opLoc, 1);

    return true;
  }

private:
  std::vector<clang::UnaryOperator *> TheOperators;
};

static RegisterMutator<DecayPointerType> M(
    "u2.DecayPointerType",
    "It takes a pointer variable or expression and decreases its pointer level by one, converting a pointer-to-pointer variable to a single-level pointer.");
