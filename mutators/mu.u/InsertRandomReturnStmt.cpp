// InsertRandomReturnStmt.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomReturnStmt
    : public Mutator,
      public clang::RecursiveASTVisitor<InsertRandomReturnStmt> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFunctions.empty()) return false;

    FunctionDecl *func = randElement(TheFunctions);
    Stmt *body = func->getBody();
    
    if(!isa<CompoundStmt>(body)) return false;
    CompoundStmt *compoundStmt = cast<CompoundStmt>(body);
    
    std::vector<Stmt*> bodyStmts(compoundStmt->body_begin(), compoundStmt->body_end());
    if(bodyStmts.empty()) return false;
    
    Stmt* insertPosition = randElement(bodyStmts);
    
    // Skip if the selected statement is a ReturnStmt
    if(isa<ReturnStmt>(insertPosition)) return false;
    
    QualType returnType = func->getReturnType();
    std::string defaultValueStr = getDefaultValue(returnType);
    
    std::string newReturnStmtStr = "return " + defaultValueStr + ";";
    
    SourceLocation insertLoc = getLocForEndOfToken(insertPosition->getEndLoc());
    getRewriter().InsertTextAfter(insertLoc, "\n" + newReturnStmtStr);

    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    TheFunctions.push_back(FD);
    return true;
  }

private:
  std::vector<clang::FunctionDecl *> TheFunctions;

  std::string getDefaultValue(QualType returnType) {
    if(returnType->isIntegerType()) {
      return "0";
    } else if(returnType->isRealFloatingType()) {
      return "0.0";
    } else if(returnType->isBooleanType()) {
      return "false";
    } else if(returnType->isPointerType()) {
      return "NULL";
    } else {
      // For other types, just return an empty string.
      // This should be improved for complete support.
      return "";
    }
  }
};

static RegisterMutator<InsertRandomReturnStmt> M("u1.InsertRandomReturnStmt", "Insert a return statement at a random position in a function");