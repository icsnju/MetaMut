#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class CopyExpr : public Mutator, public clang::RecursiveASTVisitor<CopyExpr> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitExpr(clang::Expr *E);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DR);
  bool VisitIntegerLiteral(clang::IntegerLiteral *IL);

private:
  std::vector<clang::Expr *> TheExprs;
  std::vector<clang::Expr *> TheOldVars;
};

static RegisterMutator<CopyExpr> M("s.copy-expr", "Copy expression to a variable or constant literal.");

bool CopyExpr::VisitExpr(Expr *E) {
  if (isMutationSite(E))
    if (!isa<DeclRefExpr>(E) && !isa<IntegerLiteral>(E))
      TheExprs.push_back(dyn_cast<Expr>(E));
  return true;
}

bool CopyExpr::VisitDeclRefExpr(DeclRefExpr *DR) {
  if (isMutationSite(DR))
    TheOldVars.push_back(DR);
  return true;
}

bool CopyExpr::VisitIntegerLiteral(IntegerLiteral *IL) {
  if (isMutationSite(IL))
    TheOldVars.push_back(IL);
  return true;
}

bool CopyExpr::mutate() {
  TraverseAST(getASTContext());
  if (TheExprs.empty() || TheOldVars.empty())
    return false;

  Expr *expr = randElement(TheExprs);

  // Select a random variable or constant literal to replace the expression
  Expr *oldExpr = nullptr;
  if (!TheOldVars.empty()) {
    oldExpr = randElement(TheOldVars);
  } else {
    return false;
  }

  // Check type compatibility
  if (getCompilerInstance().getSema().CheckAssignmentConstraints(
          oldExpr->getBeginLoc(), oldExpr->getType(), expr->getType()) !=
      Sema::AssignConvertType::Compatible) {
    return false;
  }

  SourceLocation ExprLoc = oldExpr->getBeginLoc();
  auto &SM = getASTContext().getSourceManager();
  const auto &LO = getASTContext().getLangOpts();
  unsigned tokLen = Lexer::MeasureTokenLength(ExprLoc, SM, LO);
  if (DeclRefExpr *VD = dyn_cast<DeclRefExpr>(oldExpr))
    tokLen = VD->getDecl()->getName().size();
  getRewriter().ReplaceText(ExprLoc, tokLen, getSourceText(expr));
  return true;
}
