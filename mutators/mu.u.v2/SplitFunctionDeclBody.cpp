#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SplitFunctionDeclBody : public Mutator,
                              public clang::RecursiveASTVisitor<SplitFunctionDeclBody> {
  using Mutator::Mutator;

public:
  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    if (FD->isMain()) return true;  // Skip the main function
    TheFunctions.push_back(FD);
    return true;
  }

  bool VisitCallExpr(clang::CallExpr *CE) {
    const auto *callee = CE->getDirectCallee();
    if (callee) {
      TheCallExprs[callee].push_back(CE);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);
    CompoundStmt *funcBody = dyn_cast<CompoundStmt>(func->getBody());

    if (!funcBody || funcBody->size() < 2) return false;

    size_t midPoint = funcBody->size() / 2;
    std::string funcBodyPart1 = "{", funcBodyPart2 = "{";
    size_t i = 0;
    std::vector<DeclStmt *> localVarDecls;

    for (const auto &stmt : funcBody->body()) {
      if (DeclStmt *DS = dyn_cast<DeclStmt>(stmt)) {
        localVarDecls.push_back(DS);
      }
      if (i < midPoint || localVarDecls.size()) {
        funcBodyPart1 += getSourceText(stmt) + ";";
      } else {
        funcBodyPart2 += getSourceText(stmt) + ";";
      }
      i++;
      if (i == midPoint) localVarDecls.clear();  // Clear declarations when reaching the midpoint
    }
    funcBodyPart1 += "}";
    funcBodyPart2 += "}";

    std::string newFuncName = generateUniqueName(func->getNameAsString() + "_split");
    QualType returnType = func->getReturnType();
    std::string newFuncDecl = formatAsDecl(returnType, newFuncName) + "(";

    for (unsigned i = 0; i < func->getNumParams(); ++i) {
      if (i > 0) newFuncDecl += ", ";
      newFuncDecl += formatAsDecl(func->getParamDecl(i)->getType(),
                                  func->getParamDecl(i)->getNameAsString());
    }

    newFuncDecl += ") " + funcBodyPart2;

    if (!addStringBeforeFunctionDecl(func, newFuncDecl)) return false;

    // Replace function body
    SourceRange bodyRange = funcBody->getSourceRange();
    getRewriter().ReplaceText(bodyRange, funcBodyPart1);

    // Replace function calls
    auto it = TheCallExprs.find(func);
    if (it != TheCallExprs.end()) {
      for (const auto &call : it->second) {
        std::string newCall = getSourceText(call->getCallee()) + "(";
        for (unsigned i = 0; i < call->getNumArgs(); ++i) {
          if (i > 0) newCall += ", ";
          newCall += getSourceText(call->getArg(i));
        }
        newCall += ")";
        newCall += "; " + newFuncName + "(";
        for (unsigned i = 0; i < call->getNumArgs(); ++i) {
          if (i > 0) newCall += ", ";
          newCall += getSourceText(call->getArg(i));
        }
        newCall += ")";
        getRewriter().ReplaceText(call->getSourceRange(), newCall);
      }
    }

    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
  std::map<const FunctionDecl *, std::vector<const CallExpr *>> TheCallExprs;
};

static RegisterMutator<SplitFunctionDeclBody> M(
    "u2.SplitFunctionDeclBody",
    "It takes a function declaration and divides its body into two separate "
    "functions, with one function containing half of the statements and the "
    "other containing the remaining half, updating all existing calls to the "
    "original function with the new function calls in sequence.");
