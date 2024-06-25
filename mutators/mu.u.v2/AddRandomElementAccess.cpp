#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomElementAccess
    : public Mutator,
      public clang::RecursiveASTVisitor<AddRandomElementAccess> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheArrays.empty() || TheStmts.empty()) return false;

    VarDecl *arrayVar = randElement(TheArrays);
    if (!arrayVar->getType()->isArrayType()) return false;

    QualType elementType = arrayVar->getType()->getAsArrayTypeUnsafe()->getElementType();
    std::string arrayName = arrayVar->getNameAsString();
    std::string newIndex = generateUniqueName("index");
    std::string accessExpr = arrayName + "[" + newIndex + "]";

    int arraySize = getArraySize(arrayVar->getType());

    std::string newIndexDecl = "int " + newIndex + " = " + std::to_string(randIndex(arraySize)) + ";";
    std::string insertCode = newIndexDecl + "\n" + accessExpr + ";\n";

    Stmt *stmtAfter = randElement(TheStmts);
    SourceLocation insertionLocation = stmtAfter->getBeginLoc().getLocWithOffset(-1);
    getRewriter().InsertText(insertionLocation, insertCode + "\n");

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    QualType varType = VD->getType();
    if (varType->isArrayType() || varType->isPointerType()) {
      TheArrays.push_back(VD);
    }
    return true;
  }

  bool VisitCompoundStmt(clang::CompoundStmt *S) {
    for (auto *child : S->body()) {
      if (isa<Expr>(child) || isa<DeclStmt>(child)) continue;
      TheStmts.push_back(child);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheArrays;
  std::vector<clang::Stmt *> TheStmts;

  int getArraySize(QualType arrayType) {
    if (const ConstantArrayType *constArrayType = dyn_cast<ConstantArrayType>(arrayType)) {
      return constArrayType->getSize().getLimitedValue();
    }
    return 0;
  }
};

static RegisterMutator<AddRandomElementAccess> M(
    "u2.AddRandomElementAccess", "Inserts a random element access expression for a selected array variable.");
