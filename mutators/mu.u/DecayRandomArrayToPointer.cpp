// DecayRandomArrayToPointer.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class DecayRandomArrayToPointer
    : public Mutator,
      public clang::RecursiveASTVisitor<DecayRandomArrayToPointer> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseDecl(getASTContext().getTranslationUnitDecl());
    if (TheArrays.empty()) return false;

    VarDecl *arrayVar = randElement(TheArrays);

    // perform rewrite
    auto arrayType = arrayVar->getType();
    if (!arrayType->isArrayType()) return false;

    auto pointerType = getASTContext().getPointerType(arrayType->getAsArrayTypeUnsafe()->getElementType());
    std::string newDecl = formatAsDecl(pointerType, arrayVar->getNameAsString());

    getRewriter().ReplaceText(
        arrayVar->getSourceRange(), newDecl);

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if(VD->getType()->isArrayType()) {
      TheArrays.push_back(VD);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheArrays;
};

static RegisterMutator<DecayRandomArrayToPointer> M("u1.DecayRandomArrayToPointer", "Decay a randomly selected array variable to a pointer variable.");