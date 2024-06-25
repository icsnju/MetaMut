#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class GroupParameterAssignments : public Mutator,
                                  public clang::RecursiveASTVisitor<GroupParameterAssignments> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFuncs.empty()) return false;

    FunctionDecl *FD = randElement(TheFuncs);
    std::string new_func_name = generateUniqueName(FD->getNameAsString() + "_grouped");

    std::string new_func_signature = formatAsDecl(FD->getReturnType(), new_func_name) + "(";
    std::string new_func_body = "{";
    int i = 0;
    for (const auto &param : FD->parameters()) {
      if (i > 0) {
        new_func_signature += ", ";
      }
      std::string param_name = "p" + std::to_string(i);
      new_func_signature += formatAsDecl(param->getType(), param_name);
      new_func_body += " " + getSourceText(param) + " = " + param_name + ";";
      i++;
    }
    new_func_signature += ")";
    new_func_body += " return " + FD->getNameAsString() + "(";

    for (int j = 0; j < i; ++j) {
      if (j > 0) {
        new_func_body += ", ";
      }
      new_func_body += "p" + std::to_string(j);
    }
    new_func_body += "); }";

    std::string new_func_declaration = new_func_signature + ";";
    std::string new_func_definition = new_func_signature + new_func_body;

    addStringBeforeFunctionDecl(FD, new_func_declaration);
    addStringBeforeFunctionDecl(FD, new_func_definition);

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheFuncs.push_back(FD);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFuncs;
};

static RegisterMutator<GroupParameterAssignments> M(
    "u2.GroupParameterAssignments", "Groups multiple consecutive assignments to different function parameters into a single compound statement.");
