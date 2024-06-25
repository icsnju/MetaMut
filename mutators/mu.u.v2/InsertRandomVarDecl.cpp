#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomVarDecl
    : public Mutator,
      public clang::RecursiveASTVisitor<InsertRandomVarDecl> {
public:
  using Mutator::Mutator;

  bool VisitStmt(Stmt *S) {
    if (isa<CompoundStmt>(S)) {
      CompoundStmt *CS = cast<CompoundStmt>(S);
      for (Stmt *Child : CS->body()) {
        InsertionPoints.push_back(Child);
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (InsertionPoints.empty()) return false;

    Stmt *target = randElement(InsertionPoints);
    SourceLocation loc = target->getBeginLoc();

    auto &sema = getCompilerInstance().getSema();
    QualType randomVarType = sema.getASTContext().IntTy;
    std::string varName = generateUniqueName("randomVar");

    std::string newVarDecl = formatAsDecl(randomVarType, varName);
    newVarDecl += " = " + std::to_string(randIndex(100)) + "; ";

    getRewriter().InsertTextBefore(loc, newVarDecl);

    return true;
  }

private:
  std::vector<clang::Stmt *> InsertionPoints;
};

static RegisterMutator<InsertRandomVarDecl> M(
    "u2.InsertRandomVarDecl", "Adds a random variable declaration with an appropriate initialization expression at a selected point in the code scope.");
