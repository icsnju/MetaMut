#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <random>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyForLoopIncrement
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyForLoopIncrement> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitForStmt(clang::ForStmt *FS);

private:
  std::vector<clang::ForStmt *> TheLoops;
};

static RegisterMutator<ModifyForLoopIncrement> M("s.modify-forloopincrement", "Modify the increment step in a For loop.");

bool ModifyForLoopIncrement::VisitForStmt(ForStmt *FS) {
  if (isMutationSite(FS))
    TheLoops.push_back(FS);
  return true;
}

bool ModifyForLoopIncrement::mutate() {
  TraverseAST(getASTContext());
  if (TheLoops.empty())
    return false;

  ForStmt *fs = randElement(TheLoops);

  Expr *incExpr = fs->getInc();
  if (!incExpr)
    return false;

  // Handle CompoundAssignOperator (i += step, i -= step)
  if (auto *compoundAssign =
          dyn_cast_or_null<CompoundAssignOperator>(incExpr)) {
    auto *rhs =
        dyn_cast<IntegerLiteral>(compoundAssign->getRHS()->IgnoreImpCasts());
    if (!rhs)
      return false;

    int oldIncr = rhs->getValue().getLimitedValue();
    int newIncr;

    do {
      newIncr = randIndex(10);
    } while (newIncr == oldIncr);

    getRewriter().ReplaceText(rhs->getSourceRange(), std::to_string(newIncr));

    return true;
  }

  // Handle UnaryOperator (i++, i--)
  if (auto *unaryOp = dyn_cast_or_null<UnaryOperator>(incExpr)) {
    UnaryOperatorKind opKind = unaryOp->getOpcode();
    std::string subExprStr = getSourceText(unaryOp->getSubExpr());
    if (opKind == UO_PostInc || opKind == UO_PreInc) { // i++
      std::string newIncr = std::to_string(randIndex(10) + 2);
      getRewriter().ReplaceText(unaryOp->getSourceRange(),
                                subExprStr + " += " + newIncr);
    } else if (opKind == UO_PostDec || opKind == UO_PreDec) { // i--
      std::string newIncr = std::to_string(randIndex(10) + 2);
      getRewriter().ReplaceText(unaryOp->getSourceRange(),
                                subExprStr + " -= " + newIncr);
    }

    return true;
  }

  return false;
}
