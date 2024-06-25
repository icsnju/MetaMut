#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddNestedLoop
    : public Mutator,
      public clang::RecursiveASTVisitor<AddNestedLoop> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLoops.empty()) return false;

    Stmt *loop = randElement(TheLoops);
    std::string newVarName = generateUniqueName("i");

    std::string loopBody = getSourceText(loop);
    std::string newLoop = "for (int " + newVarName + " = 0; " + newVarName + " < 3; ++" + newVarName + ") {" + loopBody + "}";
    getRewriter().ReplaceText(loop->getSourceRange(), newLoop);

    return true;
  }

  bool VisitStmt(clang::Stmt *S) {
    if (isa<ForStmt>(S) || isa<WhileStmt>(S) || isa<DoStmt>(S)) {
      TheLoops.push_back(S);
    }
    return true;
  }

private:
  std::vector<clang::Stmt *> TheLoops;
};

static RegisterMutator<AddNestedLoop> M(
    "u2.AddNestedLoop", "Add another loop inside an existing loop, copying the inner loop's body from the outer loop and adjusting loop variables.");
