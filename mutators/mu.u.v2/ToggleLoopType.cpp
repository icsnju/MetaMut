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

    if (isa<WhileStmt>(loop)) {
      WhileStmt *ws = cast<WhileStmt>(loop);
      Expr *cond = ws->getCond();
      Stmt *body = ws->getBody();

      std::string condText = getSourceText(cond);
      std::string bodyText = getSourceText(body);
      std::string replacement = "do " + bodyText + " while(" + condText + ");";

      getRewriter().ReplaceText(loop->getSourceRange(), replacement);
    } else if (isa<DoStmt>(loop)) {
      DoStmt *ds = cast<DoStmt>(loop);
      Expr *cond = ds->getCond();
      Stmt *body = ds->getBody();

      std::string condText = getSourceText(cond);
      std::string bodyText = getSourceText(body);
      std::string replacement = "while(" + condText + ") " + bodyText;

      getRewriter().ReplaceText(loop->getSourceRange(), replacement);
    }

    return true;
  }

  bool VisitWhileStmt(clang::WhileStmt *WS) {
    TheLoops.push_back(WS);
    return true;
  }

  bool VisitDoStmt(clang::DoStmt *DS) {
    TheLoops.push_back(DS);
    return true;
  }

private:
  std::vector<clang::Stmt *> TheLoops;
};

static RegisterMutator<ToggleLoopType> M(
    "u2.ToggleLoopType", "Change a while loop to a do-while loop and vice versa, maintaining the same condition and loop body.");
