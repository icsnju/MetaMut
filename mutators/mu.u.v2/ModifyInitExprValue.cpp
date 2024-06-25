#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyInitExprValue : public Mutator,
                            public clang::RecursiveASTVisitor<ModifyInitExprValue> {
  using VisitorTy = clang::RecursiveASTVisitor<ModifyInitExprValue>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheVarInits.empty()) return false;

    VarDecl *varDecl = randElement(TheVarInits);
    Expr *initExpr = varDecl->getInit();

    if (!initExpr) return false;

    QualType varType = varDecl->getType();
    Expr::EvalResult result;
    if (initExpr->EvaluateAsRValue(result, getASTContext()) && result.Val.isInt()) {
        llvm::APSInt oldValue = result.Val.getInt();
      
        // Generate a random number within a reasonable range
        int offset = getManager().getRandomGenerator()() % 10 + 1;

        // Add or subtract the random number from the original value
        if (randBool()) {
            oldValue = oldValue + offset;
        } else {
            oldValue = oldValue - offset;
        }

        getRewriter().ReplaceText(initExpr->getSourceRange(), oldValue.toString(/*Radix=*/10));
        return true;
    } else if (auto *IL = dyn_cast<InitListExpr>(initExpr)) {
        unsigned index = randIndex(IL->getNumInits());
        Expr *selectedInit = IL->getInit(index);
        if (!selectedInit) return false;

        if (selectedInit->isIntegerConstantExpr(getASTContext())) {
            Expr::EvalResult initResult;
            if (selectedInit->EvaluateAsRValue(initResult, getASTContext()) && initResult.Val.isInt()) {
                llvm::APSInt oldValue = initResult.Val.getInt();

                // Generate a random number within a reasonable range
                int offset = getManager().getRandomGenerator()() % 10 + 1;

                // Add or subtract the random number from the original value
                if (randBool()) {
                    oldValue = oldValue + offset;
                } else {
                    oldValue = oldValue - offset;
                }

                getRewriter().ReplaceText(selectedInit->getSourceRange(), oldValue.toString(/*Radix=*/10));
                return true;
            }
        }
    }
    return false;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->hasInit()) {
      TheVarInits.push_back(VD);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheVarInits;
};

static RegisterMutator<ModifyInitExprValue> M(
    "u2.ModifyInitExprValue", "Modify a variable's initialization expression value.");
