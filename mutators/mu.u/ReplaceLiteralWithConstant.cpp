#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceLiteralWithConstant
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceLiteralWithConstant> {

public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLiterals.empty()) return false;

    Expr *Literal = randElement(TheLiterals);

    // Get the literal's type and value
    QualType LiteralType = Literal->getType();
    std::string LiteralValue = getSourceText(Literal);

    // Generate a unique name for the new global constant
    std::string NewConstantName = generateUniqueName("const");

    // Create the declaration for the new global constant
    std::string NewConstantDeclaration = formatAsDecl(LiteralType, NewConstantName) + " = " + LiteralValue + ";\n";

    // Insert the declaration at the beginning of the translation unit
    EnclosingFunctionFinder Finder(Literal);
    Finder.TraverseDecl(getASTContext().getTranslationUnitDecl());
    FunctionDecl* EnclosingFunction = Finder.getEnclosingFunction();
    if (!EnclosingFunction) return false;
    SourceLocation InsertLoc = getMostRecentTranslationUnitDecl(EnclosingFunction)->getBeginLoc();
    getRewriter().InsertTextBefore(InsertLoc, NewConstantDeclaration);

    // Replace the literal with the new constant
    getRewriter().ReplaceText(Literal->getSourceRange(), NewConstantName);

    return true;
  }

  bool VisitIntegerLiteral(clang::IntegerLiteral *IL) {
    TheLiterals.push_back(IL);
    return true;
  }

  bool VisitFloatingLiteral(clang::FloatingLiteral *FL) {
    TheLiterals.push_back(FL);
    return true;
  }

private:
  std::vector<clang::Expr *> TheLiterals;

  class EnclosingFunctionFinder : public RecursiveASTVisitor<EnclosingFunctionFinder> {
  public:
    EnclosingFunctionFinder(Expr* Target) : TargetExpr(Target), EnclosingFunc(nullptr) {}

    bool VisitFunctionDecl(FunctionDecl* FD) {
      ExprFinder Finder(TargetExpr);
      Finder.TraverseStmt(FD->getBody());
      if (Finder.found()) {
        EnclosingFunc = FD;
        return false; // stop traversal when found
      }
      return true;
    }

    FunctionDecl* getEnclosingFunction() const { return EnclosingFunc; }

  private:
    Expr* TargetExpr;
    FunctionDecl* EnclosingFunc;

    class ExprFinder : public RecursiveASTVisitor<ExprFinder> {
    public:
      ExprFinder(Expr* Target) : TargetExpr(Target), Found(false) {}

      bool VisitExpr(Expr* E) {
        if (E == TargetExpr) {
          Found = true;
          return false; // stop traversal when found
        }
        return true;
      }

      bool found() const { return Found; }

    private:
      Expr* TargetExpr;
      bool Found;
    };
  };
};

static RegisterMutator<ReplaceLiteralWithConstant> M("u1.ReplaceLiteralWithConstant", "Replace a literal with a global constant.");