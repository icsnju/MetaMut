#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomCompoundAssignWithSeparate : public Mutator,
                                                public clang::RecursiveASTVisitor<ReplaceRandomCompoundAssignWithSeparate> {
public:
    using Mutator::Mutator;
    bool mutate() override {
        TraverseAST(getASTContext());
        if (TheCompoundAssigns.empty()) return false;

        CompoundAssignOperator *expr = randElement(TheCompoundAssigns);

        // Perform rewrite
        BinaryOperatorKind newOpKind = expr->getOpcode();
        std::string newOp = BinaryOperator::getOpcodeStr(newOpKind).str();
        std::string replacement = getSourceText(expr->getLHS()) + " = " + getSourceText(expr->getLHS()) + ' ' + newOp + ' ' + getSourceText(expr->getRHS()) + ';';
        SourceRange range = getExpansionRange(expr->getSourceRange());

        getRewriter().ReplaceText(range, replacement);

        return true;
    }

    bool VisitCompoundAssignOperator(clang::CompoundAssignOperator *CAO) {
        TheCompoundAssigns.push_back(CAO);
        return true;
    }

private:
    std::vector<clang::CompoundAssignOperator *> TheCompoundAssigns;
};

static RegisterMutator<ReplaceRandomCompoundAssignWithSeparate> M("u1.ReplaceRandomCompoundAssignWithSeparate", "Randomly selects a CompoundAssignOperator (like +=, -=, *=, /=), breaks it down into a separate binary operation and an assignment, and replaces the original CompoundAssignOperator with the two new separate statements.");
