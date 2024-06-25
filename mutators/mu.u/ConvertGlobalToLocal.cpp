#include "Mutator.h"
#include "MutatorManager.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>

using namespace clang;

class ConvertGlobalToLocal : public Mutator,
                             public clang::RecursiveASTVisitor<ConvertGlobalToLocal> {
public:
  ConvertGlobalToLocal(const char* name, const char* desc)
      : Mutator(name, desc) {}

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheGlobalVars.empty()) return false;

    VarDecl *globVar = randElement(TheGlobalVars);
    std::string varName = globVar->getNameAsString();
    std::string varType = globVar->getType().getAsString();
    std::string newDeclaration;

    // Get initial value of the variable
    APValue *initValue = globVar->evaluateValue();
    if (initValue && initValue->isInt()) {
      int value = initValue->getInt().getExtValue();

      // Traverse all function declarations
      for (auto func : TheFuncs) {
        if (isVarUsedInFunc(globVar, func)) {

          // Convert global variable to local
          newDeclaration = varType + " " + varName + " = " + std::to_string(value) + ";\n";

          // Insert new local variable declaration at the start of the function body
          getRewriter().InsertText(func->getBody()->getBeginLoc().getLocWithOffset(1), newDeclaration);
        }
      }

      // Remove global declaration
      getRewriter().RemoveText(globVar->getSourceRange());
    }
    return true;
  }

  bool VisitFunctionDecl(FunctionDecl *F) {
    TheFuncs.push_back(F);
    return true;
  }

  bool VisitVarDecl(VarDecl *VD) {
    if (VD->isFileVarDecl() && VD->hasInit() && VD->getInit()->isIntegerConstantExpr(getASTContext())) {
      TheGlobalVars.push_back(VD);
    }
    return true;
  }

private:
  std::vector<VarDecl *> TheGlobalVars;
  std::vector<FunctionDecl *> TheFuncs;

  bool isVarUsedInFunc(VarDecl* globVar, FunctionDecl* func) {
    for (auto stmt : func->getBody()->children()) {
      if (StmtVisitor(stmt, globVar)) {
        return true;
      }
    }
    return false;
  }

  bool StmtVisitor(Stmt *stmt, VarDecl* globalVar) {
    for (Stmt::child_iterator i = stmt->child_begin(), e = stmt->child_end(); i != e; ++i) {
      if (!*i) continue;
      if (isa<DeclRefExpr>(*i)) {
        DeclRefExpr *refExpr = cast<DeclRefExpr>(*i);
        if (refExpr->getDecl() == globalVar) {
          return true;
        }
      }
      if (StmtVisitor(*i, globalVar)) {
        return true;
      }
    }
    return false;
  }
};

static RegisterMutator<ConvertGlobalToLocal> M("u1.ConvertGlobalToLocal", "Converts a global variable to local variables in all functions where it is used");