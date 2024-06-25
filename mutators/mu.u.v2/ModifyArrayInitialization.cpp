#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Sema/Sema.h>
#include <clang/Lex/Lexer.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyArrayInitialization
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyArrayInitialization> {
public:
  using Mutator::Mutator;

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (auto InitExpr = dyn_cast_or_null<InitListExpr>(VD->getInit())) {
      ArrayInitExprs.push_back(InitExpr);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (ArrayInitExprs.empty()) return false;

    InitListExpr *initExpr = randElement(ArrayInitExprs);

    // Modify a single element of the array initialization
    unsigned int index = rand() % initExpr->getNumInits();
    Expr *element = initExpr->getInit(index);
    SourceLocation startLoc = element->getBeginLoc();
    SourceLocation endLoc = element->getEndLoc();

    auto &Sema = getCompilerInstance().getSema();
    ExprResult newValue = Sema.ActOnIntegerConstant(
        startLoc, rand() % 100); // Generate a new random integer value
    if (newValue.isInvalid()) return false;

    // Replace the old value with the new one
    std::string newText = std::to_string(rand() % 100);
    getRewriter().ReplaceText(SourceRange(startLoc, endLoc), newText);

    return true;
  }

private:
  std::vector<clang::InitListExpr *> ArrayInitExprs;
};

static RegisterMutator<ModifyArrayInitialization> M(
    "u2.ModifyArrayInitialization", "Modify the values of array elements during initialization.");
