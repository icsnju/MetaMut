#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RandomArrayElementAssign
    : public Mutator,
      public clang::RecursiveASTVisitor<RandomArrayElementAssign> {

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheArrays.empty()) return false;

    // Select a random array
    VarDecl *array = randElement(TheArrays);

    // Get the array type and cast it to a ConstantArrayType
    const ArrayType *arrayType = array->getType()->getAsArrayTypeUnsafe();
    const ConstantArrayType *constArrayType = dyn_cast<ConstantArrayType>(arrayType);
    if (!constArrayType) return false; // The array is not a constant array

    // Select a random index within the array
    unsigned index = randIndex(constArrayType->getSize().getLimitedValue());

    // Generate a new assignment statement
    std::string NewStmt = ";\n" + array->getNameAsString() + "[" + std::to_string(index) + "]";
    QualType elementType = constArrayType->getElementType();
    while (const ConstantArrayType *subArrayType = dyn_cast<ConstantArrayType>(elementType))
    {
        NewStmt += "[" + std::to_string(randIndex(subArrayType->getSize().getLimitedValue())) + "]";
        elementType = subArrayType->getElementType();
    }
    NewStmt += " = " + generateNewValue(elementType) + ";\n";

    // Insert the new statement at the end of the parent compound statement
    SourceLocation InsertLoc = getLocForEndOfToken(array->getEndLoc());
    getRewriter().InsertText(InsertLoc, NewStmt, true, true);

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->getType()->isArrayType()) {
      TheArrays.push_back(VD);
    }
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheArrays;

  // Generate a new value based on the type of the array elements
  std::string generateNewValue(clang::QualType type) {
    if (type->isIntegerType()) {
      return std::to_string(randIndex(100)); // Random integer between 0 and 99
    } else if (type->isCharType()) {
      return "'" + std::string(1, 'A' + randIndex(26)) + "'"; // Random char between 'A' and 'Z'
    } else if (type->isFloatingType()) {
      return std::to_string(static_cast<double>(randIndex(100)) / 10); // Random float between 0.0 and 9.9
    }
    // If the type is not supported, return a default value
    return "0";
  }
};

static RegisterMutator<RandomArrayElementAssign> M("u1.RandomArrayElementAssign", "Randomly selects an ArrayType variable and assigns a random element to a new value of the same type.");