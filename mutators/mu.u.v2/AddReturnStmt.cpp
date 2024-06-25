#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddReturnStmt : public Mutator,
                      public clang::RecursiveASTVisitor<AddReturnStmt> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);
    if (func->getReturnType()->isVoidType()) return false;

    std::string returnType = formatAsDecl(func->getReturnType(), "");
    std::string defaultReturnValue = getDefaultReturnValue(returnType);

    std::string returnStmt = "return " + defaultReturnValue + ";\n";

    CompoundStmt *body = llvm::cast<CompoundStmt>(func->getBody());
    SourceLocation lastStmtLoc = body->getLBracLoc().getLocWithOffset(1);
    getRewriter().InsertTextAfter(lastStmtLoc, returnStmt);

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheFunctions.push_back(FD);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;

  std::string getDefaultReturnValue(const std::string &returnType) {
    if (returnType == "int" || returnType == "float" || returnType == "double") {
      return "0";
    } else if (returnType == "char") {
      return "'\\0'";
    } else if (returnType.find("*") != std::string::npos) {
      return "NULL";
    } else {
      return "{}";
    }
  }
};

static RegisterMutator<AddReturnStmt> M(
    "u2.AddReturnStmt", "Inserts a return statement with a default value for the function's return type.");
