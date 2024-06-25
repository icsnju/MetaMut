#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SplitCompoundStmt
    : public Mutator,
      public clang::RecursiveASTVisitor<SplitCompoundStmt> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheBlocks.empty()) return false;

    CompoundStmt *block = randElement(TheBlocks);

    // perform rewrite
    int numStmts = block->size();
    if(numStmts < 2) return false;  // cannot split a block with less than 2 statements

    int splitIndex = randIndex(numStmts - 1) + 1;  // to ensure the split is not at the beginning or the end
    Stmt** stmts = block->body_begin(); 
    std::string firstBlock, secondBlock;

    for(int i = 0; i < numStmts; ++i) {
        std::string stmtText = getSourceText(stmts[i]);
        if(!llvm::StringRef(stmtText).endswith(";")) {
            stmtText += ";";
        }
        if(i < splitIndex) {
            firstBlock += stmtText + "\n";
        } else {
            secondBlock += stmtText + "\n";
        }
    }

    std::string newBlocks = "{\n" + firstBlock + "{\n" + secondBlock + "}\n}\n";
    getRewriter().ReplaceText(block->getSourceRange(), newBlocks);

    return true;
  }

  bool VisitCompoundStmt(clang::CompoundStmt *CS) {
    TheBlocks.push_back(CS);
    return true;
  }

  bool VisitForStmt(clang::ForStmt *FS) {
    if (auto *CS = llvm::dyn_cast<CompoundStmt>(FS->getBody())) {
      TheBlocks.push_back(CS);
    }
    return true;
  }

private:
  std::vector<clang::CompoundStmt *> TheBlocks;
};

static RegisterMutator<SplitCompoundStmt> M("u1.SplitCompoundStmt", "Randomly split a CompoundStmt into two nested CompoundStmts.");