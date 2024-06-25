#include <unordered_set>    // Include the required header
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceIntWithEquivalentBitwiseExpr
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceIntWithEquivalentBitwiseExpr> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheIntegers.empty()) return false;

    IntegerLiteral *expr = randElement(TheIntegers);

    // compute a bitwise equivalent
    int val = expr->getValue().getLimitedValue();
    std::string newExpr = computeBitwiseEquivalent(val);

    // perform rewrite
    getRewriter().ReplaceText(
        expr->getSourceRange(), newExpr);

    return true;
  }

  bool VisitIntegerLiteral(clang::IntegerLiteral *IL) {
    if (ReturnIntegers.find(IL) == ReturnIntegers.end()) {
      TheIntegers.push_back(IL);
    }
    return true;
  }

  bool VisitReturnStmt(ReturnStmt *RS) {
    if (auto *IL = dyn_cast_or_null<IntegerLiteral>(RS->getRetValue())) {
      ReturnIntegers.insert(IL);
    }
    return true;
  }

private:
  std::vector<clang::IntegerLiteral *> TheIntegers;
  std::unordered_set<clang::IntegerLiteral *> ReturnIntegers;

  std::string computeBitwiseEquivalent(int val) {
    std::string expr = "";
    int bitPos = 0;
    while (val > 0) {
        if (val & 1) {
            if (!expr.empty()) {
                expr += " | ";
            }
            expr += "(1 << " + std::to_string(bitPos) + ")";
        }
        val >>= 1;
        bitPos++;
    }
    return expr;
  }
};

static RegisterMutator<ReplaceIntWithEquivalentBitwiseExpr> M("u1.ReplaceIntWithEquivalentBitwiseExpr", "Replace integer with equivalent bitwise expression, excluding return statements.");