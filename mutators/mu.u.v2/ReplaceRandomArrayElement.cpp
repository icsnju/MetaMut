#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomArrayElement
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceRandomArrayElement> {
public:
  using Mutator::Mutator;

  bool VisitVarDecl(clang::VarDecl *VD) {
    Expr *initExpr = VD->getInit();
    if (initExpr && isa<InitListExpr>(initExpr)) {
      InitListExpr *ILE = cast<InitListExpr>(initExpr);
      if (isApplicableArray(ILE)) {
        TheInitListExprs.push_back(ILE);
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheInitListExprs.empty()) return false;

    InitListExpr *expr = randElement(TheInitListExprs);

    unsigned numInitExprs = expr->getNumInits();
    if (numInitExprs <= 1) return false;

    unsigned indexToReplace = randIndex(numInitExprs);
    SourceRange replacedElemRange = expr->getInit(indexToReplace)->getSourceRange();
    SourceLocation startLoc = replacedElemRange.getBegin();
    SourceLocation endLoc = replacedElemRange.getEnd();
    std::string defaultValue = getDefaultReplacementValue(expr->getInit(indexToReplace)->getType());

    getRewriter().ReplaceText(SourceRange(startLoc, endLoc), defaultValue);

    return true;
  }

private:
  std::vector<clang::InitListExpr *> TheInitListExprs;

  bool isApplicableArray(InitListExpr *ILE) {
    if (ILE->getNumInits() == 0) return false;
    return true;
  }

  std::string getDefaultReplacementValue(QualType elementType) {
    if (elementType->isIntegerType()) {
      return "0";
    } else if (elementType->isFloatingType()) {
      return "0.0";
    } else if (elementType->isPointerType()) {
      return "NULL";
    } else {
      return "{0}";
    }
  }
};

static RegisterMutator<ReplaceRandomArrayElement> M(
    "u2.ReplaceRandomArrayElement", "Replaces a random element with a default value in an initialized array.");
