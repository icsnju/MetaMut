#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class IntroduceTypeConversion
    : public Mutator,
      public clang::RecursiveASTVisitor<IntroduceTypeConversion> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheExpressions.empty()) return false;

    Expr *expr = randElement(TheExpressions);

    auto newTypes = computeValidConversionTypes(expr);
    if (newTypes.empty()) return false;

    QualType newType = newTypes[randIndex(newTypes.size())];
    SourceLocation exprLoc = expr->getExprLoc();
    std::string typeAsString = formatAsDecl(newType, "");
    std::string newExprText = "(" + typeAsString + ")(" + getSourceText(expr) + ")";
    getRewriter().ReplaceText(expr->getSourceRange(), newExprText);

    return true;
  }

  bool VisitExpr(clang::Expr *E) {
    TheExpressions.push_back(E);
    return true;
  }

private:
  std::vector<clang::Expr *> TheExpressions;

  std::vector<clang::QualType> computeValidConversionTypes(clang::Expr *expr) {
    std::vector<QualType> validConversionTypes;

    auto &context = getASTContext();
    auto &sema = getCompilerInstance().getSema();
    QualType originalType = expr->getType();

    for (const auto &entry : context.getTypes()) {
      QualType newType = QualType(entry, 0);

      if (newType == originalType) continue;

      ExprResult exprWithNewType = sema.BuildUnaryOp(nullptr, expr->getBeginLoc(), UO_AddrOf, expr);

      if (exprWithNewType.isInvalid() ||
          sema.CheckSingleAssignmentConstraints(newType, exprWithNewType) != Sema::Compatible) {
        continue;
      }

      validConversionTypes.push_back(newType);
    }

    return validConversionTypes;
  }
};

static RegisterMutator<IntroduceTypeConversion> M(
    "u2.IntroduceTypeConversion", "Introduce type conversion for an expression by explicitly casting it to a compatible type.");
