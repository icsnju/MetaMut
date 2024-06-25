#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomPointerArithmetic
    : public Mutator,
      public clang::RecursiveASTVisitor<AddRandomPointerArithmetic> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (ThePointerVars.empty()) return false;

    Expr *pointerExpr = randElement(ThePointerVars);

    // Perform rewrite
    std::string newArithmeticOperation = (randBool()) ? " += " : " -= ";
    int randomValue = getManager().getRandomGenerator()() % 5 + 1;

    SourceManager &SM = getASTContext().getSourceManager();
    SourceLocation exprEndLocation = getLocForEndOfToken(pointerExpr->getEndLoc());
    SourceLocation exprEndExpansionLocation = SM.getExpansionLoc(exprEndLocation);
    unsigned lineNumber = SM.getExpansionLineNumber(exprEndExpansionLocation);
    SourceLocation insertionLocation = SM.translateLineCol(SM.getFileID(exprEndExpansionLocation), lineNumber + 1, 1);

    std::string insertionText = getSourceText(pointerExpr) + newArithmeticOperation + std::to_string(randomValue) + ";\n";
    getRewriter().InsertText(insertionLocation, insertionText);

    return true;
  }

  bool VisitExpr(clang::Expr *E) {
    if (isa<clang::DeclRefExpr>(E) && E->getType()->isPointerType()) {
      ThePointerVars.push_back(E);
    }
    return true;
  }

private:
  std::vector<clang::Expr *> ThePointerVars;
};

static RegisterMutator<AddRandomPointerArithmetic> M(
    "u2.AddRandomPointerArithmetic", "Add a random pointer arithmetic operation (addition or subtraction) with a random value to a pointer variable or expression, potentially changing its memory address.");
