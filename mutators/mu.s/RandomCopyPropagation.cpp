#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RandomCopyPropagation
    : public Mutator,
      public clang::RecursiveASTVisitor<RandomCopyPropagation> {
  using VisitorTy = clang::RecursiveASTVisitor<RandomCopyPropagation>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool TraverseBinaryOperator(clang::BinaryOperator *BO);
  bool VisitDeclRefExpr(clang::DeclRefExpr *DR);

private:
  std::map<clang::VarDecl *, std::vector<clang::Expr *>> choices;
  std::map<clang::VarDecl *, std::vector<clang::DeclRefExpr *>> refExprs;
};

static RegisterMutator<RandomCopyPropagation> M("s.random-copy-propagation", "Propagate the RHS expression of assignment");

bool RandomCopyPropagation::TraverseBinaryOperator(clang::BinaryOperator *BO) {
  if (BO->getOpcode() != clang::BO_Assign)
    return VisitorTy::TraverseBinaryOperator(BO);

  auto LHS =
      clang::dyn_cast<clang::DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts());
  auto RHS = BO->getRHS()->IgnoreParenImpCasts();

  if (LHS && RHS) {
    auto LHSVarDecl = clang::dyn_cast<clang::VarDecl>(LHS->getDecl());

    if (LHSVarDecl) {
      choices[LHSVarDecl].push_back(RHS);
    }
  }

  VisitorTy::TraverseStmt(BO->getRHS());
  return true;
}

bool RandomCopyPropagation::VisitDeclRefExpr(clang::DeclRefExpr *DR) {
  auto *varDecl = clang::dyn_cast<clang::VarDecl>(DR->getDecl());
  if (varDecl && isMutationSite(DR))
    refExprs[varDecl].push_back(DR);
  return true;
}

bool RandomCopyPropagation::mutate() {
  TraverseAST(getASTContext());

  std::vector<clang::VarDecl *> vars;
  for (auto &kv : choices)
    if (refExprs.find(kv.first) != refExprs.end())
      vars.push_back(kv.first);

  if (vars.empty())
    return false;

  clang::VarDecl *varDecl = randElement(vars);
  auto exprs = refExprs[varDecl];
  std::shuffle(exprs.begin(), exprs.end(), getManager().getRandomGenerator());
  for (int i = 0, n = randIndex(exprs.size()); i < n + 1; i++) {
    clang::DeclRefExpr *refExpr = exprs[i];
    clang::Expr *choice = randElement(choices[varDecl]);
    std::string replacement = "(" + getSourceText(choice) + ")";
    getRewriter().ReplaceText(refExpr->getSourceRange(), replacement);
  }

  return true;
}
