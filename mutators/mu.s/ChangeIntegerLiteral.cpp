#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <llvm/ADT/APSInt.h>

#include "Mutator.h"
#include "MutatorManager.h"

class ChangeIntegerLiteral
    : public Mutator,
      public clang::RecursiveASTVisitor<ChangeIntegerLiteral> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitIntegerLiteral(clang::IntegerLiteral *IL);

private:
  std::vector<clang::IntegerLiteral *> TheLiterals;
};

static RegisterMutator<ChangeIntegerLiteral> M("s.change-integer-literal",
                                               "Change an integer literal.");

bool ChangeIntegerLiteral::VisitIntegerLiteral(clang::IntegerLiteral *IL) {
  if (isMutationSite(IL))
    TheLiterals.push_back(IL);
  return true;
}

bool ChangeIntegerLiteral::mutate() {
  TraverseAST(getASTContext());
  if (TheLiterals.empty())
    return false;

  clang::IntegerLiteral *literal = randElement(TheLiterals);
  llvm::APInt value = literal->getValue();

  // Add a random integer to the existing value
  value += getManager().randint(-100, 100);

  getRewriter().ReplaceText(literal->getSourceRange(),
                            value.toString(10, true));

  return true;
}
