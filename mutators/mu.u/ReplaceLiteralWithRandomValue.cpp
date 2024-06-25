#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <cstdlib>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceLiteralWithRandomValue : public Mutator,
                                      public clang::RecursiveASTVisitor<ReplaceLiteralWithRandomValue> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLiterals.empty()) return false;

    auto literal = randElement(TheLiterals);

    if (auto intLiteral = dyn_cast<IntegerLiteral>(literal)) {
      int value = std::rand();
      getRewriter().ReplaceText(intLiteral->getSourceRange(), std::to_string(value));
      return true;
    } 
    else if (auto floatLiteral = dyn_cast<FloatingLiteral>(literal)) {
      float value = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      getRewriter().ReplaceText(floatLiteral->getSourceRange(), std::to_string(value));
      return true;
    } 
    else if (auto charLiteral = dyn_cast<CharacterLiteral>(literal)) {
      char value = 'a' + std::rand() % 26;
      // Add quotes around the new character literal
      getRewriter().ReplaceText(charLiteral->getSourceRange(), "'" + std::string(1, value) + "'");
      return true;
    }

    return false;
  }

  bool VisitIntegerLiteral(clang::IntegerLiteral *IL) {
    TheLiterals.push_back(IL);
    return true;
  }

  bool VisitFloatingLiteral(clang::FloatingLiteral *FL) {
    TheLiterals.push_back(FL);
    return true;
  }

  bool VisitCharacterLiteral(clang::CharacterLiteral *CL) {
    TheLiterals.push_back(CL);
    return true;
  }

private:
  std::vector<clang::Expr *> TheLiterals;
};

static RegisterMutator<ReplaceLiteralWithRandomValue> M("u1.ReplaceLiteralWithRandomValue", "Replace a literal with a random value of the same type.");