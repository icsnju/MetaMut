#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class UnwrapIfElseBlock
    : public Mutator,
      public clang::RecursiveASTVisitor<UnwrapIfElseBlock> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIfElseBlocks.empty()) return false;

    IfStmt *ifElseBlock = randElement(TheIfElseBlocks);

    Expr *cond = ifElseBlock->getCond();
    Stmt *thenBlock = ifElseBlock->getThen();
    Stmt *elseBlock = ifElseBlock->getElse();
    SourceLocation elseLoc = ifElseBlock->getElseLoc();

    // Generate new inverted condition and new if statements
    std::string invertedCond = "!(" + getSourceText(cond) + ")";
    std::string newIfBlock = "if (" + getSourceText(cond) + ") " + getSourceText(thenBlock) + " else if (" + invertedCond + ") " + getSourceText(elseBlock);

    // Perform the mutation
    getRewriter().ReplaceText(ifElseBlock->getSourceRange(), newIfBlock);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    if (IS->getElse()) {
      TheIfElseBlocks.push_back(IS);
    }
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfElseBlocks;
};

static RegisterMutator<UnwrapIfElseBlock> M(
    "u2.UnwrapIfElseBlock", "Unwrap if-else block into separate if blocks with inverted conditions.");
