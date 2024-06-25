#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Decl.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomMemberFunction : public Mutator,
                                 public clang::RecursiveASTVisitor<AddRandomMemberFunction> {
  using Mutator::Mutator;

public:
  bool VisitRecordDecl(RecordDecl *RD) {
    if (!RD->isStruct()) return true;

    TheStructs.push_back(RD);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());

    if (TheStructs.empty()) return false;

    RecordDecl *targetStruct = randElement(TheStructs);
    std::string funcName = generateUniqueName("randomFunc");
    std::string structName = targetStruct->getNameAsString();

    // Add a return type and function signature for the new member function
    std::string returnType = "int";
    std::string funcSignature = returnType + " (*" + funcName + ")(struct " + structName + "* this)";

    // Add the new member function inside the struct
    std::string newFuncStr = "\n  " + funcSignature + ";\n";
    getRewriter().InsertTextBefore(targetStruct->getBraceRange().getEnd(), newFuncStr);

    return true;
  }

private:
  std::vector<RecordDecl *> TheStructs;
};

static RegisterMutator<AddRandomMemberFunction> M(
    "u2.AddRandomMemberFunction", "Insert a random member function in a struct or class with a compatible return type and parameters, providing additional functionality to the object instances.");
