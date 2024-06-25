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

class InsertRandomScopedBlock
    : public Mutator,
      public clang::RecursiveASTVisitor<InsertRandomScopedBlock> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCompoundStmts.empty()) return false;

    CompoundStmt *cs = randElement(TheCompoundStmts);

    // Generate a random scoped block with its own declarations and statements
    std::string scopedBlock = "{\n";
    scopedBlock += "  int x = " + std::to_string(rand() % 100) + ";\n";
    scopedBlock += "  printf(\"Inside scoped block: x = %d\\n\", x);\n";
    scopedBlock += "}\n";

    // Insert the scoped block after the first statement in the compound statement
    if (cs->size() > 0) {
      Stmt *firstStmt = cs->body_begin()[0];
      getRewriter().InsertTextAfter(getLocForEndOfToken(firstStmt->getEndLoc()), scopedBlock);
    } else {
      getRewriter().InsertTextBefore(cs->getRBracLoc(), scopedBlock);
    }

    return true;
  }

  bool VisitCompoundStmt(clang::CompoundStmt *CS) {
    TheCompoundStmts.push_back(CS);
    return true;
  }

private:
  std::vector<clang::CompoundStmt *> TheCompoundStmts;
};

static RegisterMutator<InsertRandomScopedBlock> M(
    "u2.InsertRandomScopedBlock", "It takes a compound statement and inserts a random scoped block with its own declarations and statements, preserving the original control flow.");
