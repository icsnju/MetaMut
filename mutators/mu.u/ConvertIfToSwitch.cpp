#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Tooling/Core/Replacement.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ConvertIfToSwitch : public Mutator,
                          public clang::RecursiveASTVisitor<ConvertIfToSwitch> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());

    if (TheIfStmts.empty()) 
        return false;

    IfStmt *stmt = randElement(TheIfStmts);
    IfStmt *lastStmt = stmt;
    std::string switchStr = "switch (";
    std::string casesStr = "";

    while(stmt) {
        BinaryOperator *binOp = dyn_cast<BinaryOperator>(stmt->getCond()->IgnoreParenImpCasts());
        if (!binOp || binOp->getOpcode() != BO_EQ) 
            break;

        Expr *lhs = binOp->getLHS();
        Expr *rhs = binOp->getRHS();
        casesStr += "case " + getSourceText(rhs) + ":\n" + getSourceText(stmt->getThen()) + "\nbreak;\n";
        lastStmt = stmt;
        stmt = dyn_cast<IfStmt>(stmt->getElse());
    }

    switchStr += getSourceText(dyn_cast<BinaryOperator>(lastStmt->getCond())->getLHS());
    switchStr += ") {\n" + casesStr + "}\n";

    getRewriter().ReplaceText(lastStmt->getSourceRange(), switchStr);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *IS) {
    TheIfStmts.push_back(IS);
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheIfStmts;
};

static RegisterMutator<ConvertIfToSwitch> M("u1.ConvertIfToSwitch", "Converts simple if-else-if ladders into a switch-case construct");