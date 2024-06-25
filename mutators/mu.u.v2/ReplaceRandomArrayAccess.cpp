#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomArrayAccess
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceRandomArrayAccess> {
public:
  using Mutator::Mutator;

  bool VisitDeclStmt(clang::DeclStmt *DS) {
    for (const auto *decl : DS->decls()) {
      if (const auto *VD = dyn_cast<VarDecl>(decl)) {
        if (VD->getName() == "index") {
          TheVarDecls.push_back(VD);
        }
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheVarDecls.empty()) return false;

    const VarDecl *VD = randElement(TheVarDecls);
    const Expr *InitExpr = VD->getInit();

    if (InitExpr && isa<IntegerLiteral>(InitExpr)) {
      SourceLocation beginLoc = InitExpr->getBeginLoc();
      SourceLocation endLoc = InitExpr->getEndLoc();

      unsigned newIndex = randIndex(5); // Assuming the array size is 5
      std::string newIndexStr = std::to_string(newIndex);
      getRewriter().ReplaceText(SourceRange(beginLoc, endLoc), newIndexStr);

      return true;
    }

    return false;
  }

private:
  std::vector<const clang::VarDecl *> TheVarDecls;
};

static RegisterMutator<ReplaceRandomArrayAccess> M(
    "u2.ReplaceRandomArrayAccess",
    "It takes an assignment statement of an 'index' variable and replaces the value with a random compatible index, potentially accessing a different element.");
