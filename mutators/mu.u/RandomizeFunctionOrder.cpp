#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RandomizeFunctionOrder
    : public Mutator,
      public clang::RecursiveASTVisitor<RandomizeFunctionOrder> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.size() < 2) return false;

    // Randomly select two functions
    FunctionDecl *firstFunc = randElement(TheFunctions);
    FunctionDecl *secondFunc;
    do {
      secondFunc = randElement(TheFunctions);
    } while(firstFunc == secondFunc);

    // Get source text for the two functions
    std::string firstFuncText = getSourceText(firstFunc);
    std::string secondFuncText = getSourceText(secondFunc);

    // Replace the function definitions in the source code
    getRewriter().ReplaceText(firstFunc->getSourceRange(), secondFuncText);
    getRewriter().ReplaceText(secondFunc->getSourceRange(), firstFuncText);

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->isThisDeclarationADefinition()) {
      TheFunctions.push_back(FD);
    }
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
};

static RegisterMutator<RandomizeFunctionOrder> M("u1.RandomizeFunctionOrder", "Randomly swap two function definitions in a program.");