#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ShiftArrayElementValues : public Mutator,
                                 public clang::RecursiveASTVisitor<ShiftArrayElementValues> {
  using Mutator::Mutator;

public:
  bool VisitDeclStmt(DeclStmt *DS) {
    for (Decl *decl : DS->decls()) {
      if (VarDecl *varDecl = dyn_cast<VarDecl>(decl)) {
        if (varDecl->getType()->isArrayType()) {
          ArrayDecls.push_back(varDecl);
        }
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (ArrayDecls.empty()) return false;

    VarDecl *targetArray = randElement(ArrayDecls);
    QualType arrayType = targetArray->getType();
    const ConstantArrayType *arraySize = getASTContext().getAsConstantArrayType(arrayType);

    if (!arraySize) return false;

    int shiftAmount = randIndex(arraySize->getSize().getLimitedValue());
    bool shiftRight = randBool();

    const InitListExpr *initList = dyn_cast<InitListExpr>(targetArray->getInit());
    std::vector<std::string> arrayElements;
    for (int i = 0; i < initList->getNumInits(); ++i) {
      arrayElements.push_back(getSourceText(initList->getInit(i)));
    }

    if (shiftRight) {
      for (int i = 0; i < shiftAmount; ++i) {
        arrayElements.insert(arrayElements.begin(), "0");
      }
    } else {
      for (int i = 0; i < shiftAmount; ++i) {
        arrayElements.push_back("0");
      }
    }

    std::string newArrayInit = "{";
    for (size_t i = 0; i < arrayElements.size(); ++i) {
      newArrayInit += arrayElements[i];
      if (i < arrayElements.size() - 1) {
        newArrayInit += ", ";
      }
    }
    newArrayInit += "}";

    getRewriter().ReplaceText(targetArray->getInit()->getSourceRange(), newArrayInit);
    return true;
  }

private:
  std::vector<VarDecl *> ArrayDecls;
};

static RegisterMutator<ShiftArrayElementValues> M(
    "u2.ShiftArrayElementValues", "Shifts array elements to left or right by a specified number of positions.");
