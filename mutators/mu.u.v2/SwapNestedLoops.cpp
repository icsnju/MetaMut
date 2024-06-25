#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapNestedLoops
    : public Mutator,
      public clang::RecursiveASTVisitor<SwapNestedLoops> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheOuterLoops.empty()) return false;

    const LoopPair &selectedPair = randElement(TheOuterLoops);
    ForStmt *outerLoop = selectedPair.first;
    ForStmt *innerLoop = selectedPair.second;

    Stmt *outerBody = outerLoop->getBody();
    SourceRange outerBodyRange = outerBody->getSourceRange();

    Stmt *innerBody = innerLoop->getBody();
    SourceRange innerBodyRange = innerBody->getSourceRange();

    // Store the original source text for inner and outer loops
    std::string originalInner = getSourceText(innerLoop);
    std::string originalOuter = getSourceText(outerLoop);

    // Find the location of the inner loop in the outer loop
    SourceLocation innerLoopLocation = outerBody->getBeginLoc();

    // Insert the inner loop's body at the beginning of the outer loop's body
    getRewriter().InsertText(innerLoopLocation, getSourceText(innerBody) + " ");

    // Replace the inner loop with the entire outer loop
    getRewriter().ReplaceText(innerLoop->getSourceRange(), originalOuter);

    return true;
  }

  bool VisitForStmt(clang::ForStmt *FS) {
    Stmt *body = FS->getBody();
    if (ForStmt *innerLoop = dyn_cast<ForStmt>(body)) {
      TheOuterLoops.push_back(std::make_pair(FS, innerLoop));
    } else if (CompoundStmt *CS = dyn_cast<CompoundStmt>(body)) {
      for (Stmt *childStmt : CS->children()) {
        if (ForStmt *innerLoop = dyn_cast<ForStmt>(childStmt)) {
          TheOuterLoops.push_back(std::make_pair(FS, innerLoop));
          break;
        }
      }
    }
    return true;
  }

private:
  using LoopPair = std::pair<clang::ForStmt *, clang::ForStmt *>;
  std::vector<LoopPair> TheOuterLoops;
};

static RegisterMutator<SwapNestedLoops> M(
    "u2.SwapNestedLoops", "Swap the positions of two nested loops within a function");
