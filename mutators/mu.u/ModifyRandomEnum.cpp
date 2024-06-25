#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyRandomEnum : public Mutator,
                         public clang::RecursiveASTVisitor<ModifyRandomEnum> {

  using VisitorTy = clang::RecursiveASTVisitor<ModifyRandomEnum>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    // Traverse the AST to find EnumDecls and EnumConstantDecls
    TraverseAST(getASTContext());
    
    if (TheEnums.empty()) return false;
    
    // Pick a random EnumDecl
    EnumDecl *enumDecl = randElement(TheEnums);

    // Get the list of constants for this EnumDecl
    auto constantsRange = enumDecl->enumerators();

    // Convert the range to a vector
    std::vector<EnumConstantDecl*> constants(constantsRange.begin(), constantsRange.end());

    if (constants.empty()) return false;

    // Pick a random constant
    EnumConstantDecl *constant = randElement(constants);

    // Generate a new value for the constant
    int newValue = getManager().getRandomGenerator()(); // get a random int

    // Replace the old value with the new one
    getRewriter().ReplaceText(constant->getSourceRange(),
                              constant->getNameAsString() + " = " + std::to_string(newValue));

    return true;
  }

  bool VisitEnumDecl(clang::EnumDecl *ED) {
    TheEnums.push_back(ED);
    return true;
  }

private:
  std::vector<clang::EnumDecl *> TheEnums;
};

static RegisterMutator<ModifyRandomEnum> M("u1.ModifyRandomEnum", "Randomly select an EnumDecl and change the value of one of its EnumConstantDecls to a different integer value.");