#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddRandomAssignment : public Mutator,
                            public clang::RecursiveASTVisitor<AddRandomAssignment> {
  using VisitorTy = clang::RecursiveASTVisitor<AddRandomAssignment>;

public:
  using Mutator::Mutator;

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheFunctions.push_back(FD);
    return true;
  }

  bool mutate() override {
    // Step 1, Traverse the AST
    TraverseAST(getASTContext());

    if (TheFunctions.empty()) return false;

    // Step 3, Randomly select a mutation instance
    FunctionDecl *selectedFunc = randElement(TheFunctions);

    // Step 4, Checking mutation validity
    CompoundStmt *body = llvm::dyn_cast<CompoundStmt>(selectedFunc->getBody());
    if (!body) return false;

    // Collect struct variables
    std::vector<const VarDecl *> structVars;
    for (const auto &decl : selectedFunc->decls()) {
      if (const VarDecl *var = dyn_cast<VarDecl>(decl)) {
        if (var->getType()->isStructureType()) {
          structVars.push_back(var);
        }
      }
    }

    if (structVars.empty()) return false;

    // Pick a random struct variable
    const VarDecl *selectedVar = randElement(structVars);

    // Pick a random field from the struct
    const RecordDecl *structDecl = selectedVar->getType()->getAsStructureType()->getDecl();
    std::vector<FieldDecl *> fields(structDecl->field_begin(), structDecl->field_end());
    const FieldDecl *selectedField = randElement(fields);

    // Step 5, Perform mutation
    std::string fieldValue;
    QualType fieldType = selectedField->getType();
    if (fieldType->isIntegerType()) {
      fieldValue = "42";
    } else if (fieldType->isRealFloatingType()) {
      fieldValue = "42.0";
    } else if (fieldType->isCharType()) {
      fieldValue = "'A'";
    } else {
      fieldValue = "{0}";
    }
    std::string assignmentCode = selectedVar->getNameAsString() + "." + selectedField->getNameAsString() + " = " + fieldValue + ";";
    bool result = addStringAfterStmt(body->body_back(), assignmentCode);

    // Step 6, Return true if changed
    return result;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
};

static RegisterMutator<AddRandomAssignment> M(
    "u2.AddRandomAssignment", "Randomly assign a value to a struct field within a function.");
