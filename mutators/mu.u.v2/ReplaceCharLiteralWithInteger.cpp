#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceCharLiteralWithInteger
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceCharLiteralWithInteger> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCharLiterals.empty()) return false;

    CharacterLiteral *charLit = randElement(TheCharLiterals);

    // perform rewrite
    int intValue = charLit->getValue();
    SourceLocation charLoc = charLit->getExprLoc();
    std::string intStr = std::to_string(intValue);
    getRewriter().ReplaceText(charLoc, intStr);

    return true;
  }

  bool VisitCharacterLiteral(clang::CharacterLiteral *CL) {
    TheCharLiterals.push_back(CL);
    return true;
  }

private:
  std::vector<clang::CharacterLiteral *> TheCharLiterals;
};

static RegisterMutator<ReplaceCharLiteralWithInteger> M(
    "u2.ReplaceCharLiteralWithInteger", "Replace a character literal in an expression with its corresponding ASCII integer value.");
