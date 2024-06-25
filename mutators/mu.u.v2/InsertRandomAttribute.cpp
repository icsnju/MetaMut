#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomAttribute
    : public Mutator,
      public clang::RecursiveASTVisitor<InsertRandomAttribute> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheDecls.empty()) return false;

    auto *selectedDecl = randElement(TheDecls);

    std::string attribute = getRandomAttribute();
    std::string newDeclText = attribute + " " + getSourceText(selectedDecl);

    getRewriter().ReplaceText(selectedDecl->getSourceRange(), newDeclText);

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    TheDecls.push_back(VD);
    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheDecls.push_back(FD);
    return true;
  }

private:
  std::vector<clang::NamedDecl *> TheDecls;

  std::string getRandomAttribute() {
    static const std::vector<std::string> attributes = {
        "__attribute__((const))", "__attribute__((packed))",
        "__attribute__((aligned(16)))", "__attribute__((noreturn))"};

    return randElement(attributes);
  }
};

static RegisterMutator<InsertRandomAttribute> M(
    "u2.InsertRandomAttribute", "Insert a random supported attribute to a function or variable declaration.");
