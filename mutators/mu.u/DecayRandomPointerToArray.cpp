// DecayRandomPointerToArray.cpp
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;

class DecayRandomPointerToArray : public Mutator,
                      public clang::RecursiveASTVisitor<DecayRandomPointerToArray> {

  using VisitorTy = clang::RecursiveASTVisitor<DecayRandomPointerToArray>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (ThePointers.empty()) return false;

    VarDecl *ptr = randElement(ThePointers);

    // Replace the pointer with an array
    QualType type = ptr->getType();
    if (!type->isPointerType()) return false;

    QualType baseType = type->getPointeeType();
    std::string newArrayDecl = formatAsDecl(baseType, ptr->getNameAsString()) + "[10]"; // size of array is 10
    getRewriter().ReplaceText(ptr->getSourceRange(), newArrayDecl);

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->getType()->isPointerType()) {
      ThePointers.push_back(VD);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> ThePointers;
};

static RegisterMutator<DecayRandomPointerToArray> M("u1.DecayRandomPointerToArray", "Change a pointer declaration to an array declaration.");