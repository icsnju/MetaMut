#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyRandomLiteralInExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyRandomLiteralInExpr> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLiterals.empty()) return false;

    Expr *expr = randElement(TheLiterals);

    if (IntegerLiteral *intLiteral = dyn_cast<IntegerLiteral>(expr)) {
      // Generate random integer
      int randomInteger = getManager().getRandomGenerator()() % 100;
      getRewriter().ReplaceText(
          intLiteral->getSourceRange(), std::to_string(randomInteger));
    } else if (CharacterLiteral *charLiteral = dyn_cast<CharacterLiteral>(expr)) {
      // Generate random character
      char randomChar = 'a' + (getManager().getRandomGenerator()() % 26);
      getRewriter().ReplaceText(
          charLiteral->getSourceRange(), "'" + std::string(1, randomChar) + "'");
    }

    return true;
  }

  bool VisitIntegerLiteral(clang::IntegerLiteral *IL) {
    TheLiterals.push_back(IL);
    return true;
  }

  bool VisitCharacterLiteral(clang::CharacterLiteral *CL) {
    TheLiterals.push_back(CL);
    return true;
  }

private:
  std::vector<clang::Expr *> TheLiterals;
};

static RegisterMutator<ModifyRandomLiteralInExpr> M("u1.ModifyRandomLiteralInExpr", "Modify a random literal in an expression statement to another random value of the same type.");