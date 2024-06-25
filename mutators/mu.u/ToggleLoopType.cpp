// ToggleLoopType.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleLoopType : public Mutator,
                       public clang::RecursiveASTVisitor<ToggleLoopType> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLoops.empty()) return false;

    Stmt *loop = randElement(TheLoops);
    if (isa<ForStmt>(loop)) {
      return mutateForLoop(cast<ForStmt>(loop));
    } else {
      return mutateWhileLoop(cast<WhileStmt>(loop));
    }

    return false;
  }

  bool VisitForStmt(clang::ForStmt *FS) {
    TheLoops.push_back(FS);
    return true;
  }

  bool VisitWhileStmt(clang::WhileStmt *WS) {
    TheLoops.push_back(WS);
    return true;
  }

private:
  std::vector<clang::Stmt *> TheLoops;

  bool mutateForLoop(clang::ForStmt *FS) {
    // Extracts the initialization, condition, increment and body of the for loop
    std::string Init = getSourceText(FS->getInit());
    std::string Cond = getSourceText(FS->getCond());
    std::string Incr = getSourceText(FS->getInc());
    std::string Body = getSourceText(FS->getBody());

    std::string NewLoop = "{ " + Init + "; while (" + Cond + ") { " + Body + "; " + Incr + "; } }";
    
    getRewriter().ReplaceText(FS->getSourceRange(), NewLoop);

    return true;
  }

  bool mutateWhileLoop(clang::WhileStmt *WS) {
    // A while loop can only be converted to a for loop if the body is a compound statement with at least one statement.
    if (!isa<CompoundStmt>(WS->getBody()) || cast<CompoundStmt>(WS->getBody())->body_empty()) {
      return false;
    }

    // Extracts the first statement as the initialization, the condition of the while loop, and the last statement as the increment.
    CompoundStmt *Body = cast<CompoundStmt>(WS->getBody());
    std::string Init = getSourceText(Body->body_front());
    std::string Cond = getSourceText(WS->getCond());
    std::string Incr = getSourceText(Body->body_back());

    std::string NewBody;
    for (auto It = std::next(Body->body_begin()); It != std::prev(Body->body_end()); ++It) {
      NewBody += getSourceText(*It) + "; ";
    }

    std::string NewLoop = "for (" + Init + "; " + Cond + "; " + Incr + ") { " + NewBody + " }";

    getRewriter().ReplaceText(WS->getSourceRange(), NewLoop);

    return true;
  }
};

static RegisterMutator<ToggleLoopType> M("u1.ToggleLoopType", "Toggle between `for` and `while` loop types.");