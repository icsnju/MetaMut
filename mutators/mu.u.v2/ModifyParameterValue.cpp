#include <clang/AST/RecursiveASTVisitor.h>
#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyParameterValue : public Mutator,
                             public clang::RecursiveASTVisitor<ModifyParameterValue> {
public:
    using Mutator::Mutator;

    bool VisitCallExpr(CallExpr *CE) {
        if (CE->getNumArgs() > 1) { // Only collect CallExpr with more than 1 argument
            TheCallExprs.push_back(CE);
        }
        return true;
    }

    bool mutate() override {
        // Step 1, Traverse the AST
        TranslationUnitDecl *TU = getCompilerInstance().getASTContext().getTranslationUnitDecl();
        TraverseDecl(TU);

        if (TheCallExprs.empty()) return false;

        // Step 3, Randomly select a mutation instance
        CallExpr *callExpr = randElement(TheCallExprs);
        unsigned numArgs = callExpr->getNumArgs();
        if (numArgs <= 1) return false;

        unsigned argIndex = randIndex(numArgs - 1) + 1; // Skip first argument (function pointer)
        Expr *argExpr = callExpr->getArg(argIndex);
        QualType argType = argExpr->getType();

        std::string newValue = generateRandomCompatibleValue(argType);
        if (newValue.empty()) return false;  // Skip if unable to generate compatible value

        SourceRange argRange = getExpansionRange(argExpr->getSourceRange());

        // Step 5, Perform mutation
        getRewriter().ReplaceText(argRange, newValue);

        // Step 6, Return true if changed
        return true;
    }

private:
    std::vector<CallExpr *> TheCallExprs;

    std::string generateRandomCompatibleValue(QualType argType) {
        if (argType->isIntegerType()) {
            int newValue = getManager().getRandomGenerator()();
            return std::to_string(newValue);
        } else {
            return ""; // Skip if unable to generate compatible value
        }
    }
};

static RegisterMutator<ModifyParameterValue> M(
    "u2.ModifyParameterValue", "Changes the value of a function parameter at the call site to a random compatible value.");
