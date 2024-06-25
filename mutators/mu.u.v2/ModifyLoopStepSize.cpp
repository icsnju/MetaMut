#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyLoopStepSize
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyLoopStepSize> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLoops.empty()) return false;

    ForStmt *loop = randElement(TheLoops);
    Stmt *loopUpdate = loop->getInc();

    if (!isa<UnaryOperator>(loopUpdate) && !isa<BinaryOperator>(loopUpdate))
      return false;

    // Alter the step size
    int stepSize = randBool() ? 2 : 3;
    std::string newStep;

    if (auto *unaryOp = dyn_cast<UnaryOperator>(loopUpdate)) {
      newStep = getSourceText(unaryOp->getSubExpr()) +
                (unaryOp->isIncrementOp() ? " += " : " -= ") + std::to_string(stepSize);
    } else if (auto *binaryOp = dyn_cast<BinaryOperator>(loopUpdate)) {
      newStep = getSourceText(binaryOp->getLHS()) + " " +
                BinaryOperator::getOpcodeStr(binaryOp->getOpcode()).str() +
                "= " + std::to_string(stepSize);
    }

    getRewriter().ReplaceText(loopUpdate->getSourceRange(), newStep);
    return true;
  }

  bool VisitForStmt(clang::ForStmt *FS) {
    TheLoops.push_back(FS);
    return true;
  }

private:
  std::vector<clang::ForStmt *> TheLoops;
};

static RegisterMutator<ModifyLoopStepSize> M(
    "u2.ModifyLoopStepSize", "It takes a loop control expression and alters the step size, changing the increment or decrement value.");
