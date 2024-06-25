#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DecayIntegerLiteral
    : public Mutator,
      public clang::RecursiveASTVisitor<DecayIntegerLiteral> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (IntegerLiterals.empty()) return false;

    IntegerLiteral *literal = randElement(IntegerLiterals);
    
    int currentValue = literal->getValue().getLimitedValue();
    int newValue = currentValue - 1;
    std::string newValueString = std::to_string(newValue);
    getRewriter().ReplaceText(literal->getSourceRange(), newValueString);
    
    return true;
  }

  bool VisitIntegerLiteral(clang::IntegerLiteral *IL) {
    IntegerLiterals.push_back(IL);
    return true;
  }

private:
  std::vector<clang::IntegerLiteral *> IntegerLiterals;
};

static RegisterMutator<DecayIntegerLiteral> M(
    "u2.DecayIntegerLiteral", "Decrement integer literal value by one.");
