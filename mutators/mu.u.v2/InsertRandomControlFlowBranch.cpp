#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomControlFlowBranch
    : public Mutator,
      public clang::RecursiveASTVisitor<InsertRandomControlFlowBranch> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheCompoundStmts.empty()) return false;

    CompoundStmt *cs = randElement(TheCompoundStmts);

    // Generate random if statement
    std::string newControlFlowBranch = generateRandomControlFlowBranch();
    addStringAfterStmt(cs->body_front(), newControlFlowBranch);

    return true;
  }

  bool VisitCompoundStmt(clang::CompoundStmt *CS) {
    TheCompoundStmts.push_back(CS);
    return true;
  }

private:
  std::vector<clang::CompoundStmt *> TheCompoundStmts;

  std::string generateRandomControlFlowBranch() {
    // Generate a random condition and associated code block
    std::string condition = "rand() % 2";
    std::string codeBlock = "{\n  printf(\"Random control flow branch executed\\n\");\n}";

    // Randomly select if statement or switch statement
    if (randBool()) {
      return "if (" + condition + ") " + codeBlock;
    } else {
      return "switch (" + condition + ") {\n  case 1:\n  " + codeBlock + "\n  break;\n}";
    }
  }
};

static RegisterMutator<InsertRandomControlFlowBranch> M(
    "u2.InsertRandomControlFlowBranch", "Insert a random control flow branch into a compound statement.");
