#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "MutatorManager.h"

using namespace clang;
using namespace ysmut;

class Test : public Mutator,
                      public clang::RecursiveASTVisitor<Test> {

  using VisitorTy = clang::RecursiveASTVisitor<Test>;

public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    clang::VarDecl *VD = randElement(TheVars);
    clang::QualType originalType = VD->getType();
    clang::QualType newType = getASTContext().getPointerType(originalType);

    auto *P = randElement(TheFuncs);
    auto i = randIndex(P->getNumParams());
    removeParmFromFuncDecl(P->getParamDecl(i), P->getNumParams(), i);

    const clang::SourceManager& sm = getASTContext().getSourceManager();  // You get this from your AST context
    clang::SourceRange sr = VD->getSourceRange();  // This is your source range

    clang::PresumedLoc start = sm.getPresumedLoc(sr.getBegin());
    clang::PresumedLoc end = sm.getPresumedLoc(sr.getEnd());

    llvm::outs() << "var: " << VD->getNameAsString() << "\n";
    if (start.isValid() && end.isValid()) {
      llvm::outs() << "Begin: " << start.getLine() << ", " << start.getColumn() << "\n";
      llvm::outs() << "End: " << end.getLine() << ", " << end.getColumn() << "\n";
    } else {
      llvm::outs() << "Invalid source range!" << "\n";
    }

    addStringAfterVarDecl(VD, "={0}");
    replaceVarDeclarator(VD, newType);
    return true;
  }

  bool VisitStmt(clang::Stmt *S) {
    if (!clang::isa<clang::Expr>(S))
      addStringAfterStmt(S, "while (0) ;");
    return true;
  }

  bool VisitFunctionDecl(clang::FunctionDecl *VD) {
    addStringBeforeFunctionDecl(VD, "int xxx = 0;");
    TheFuncs.push_back(VD);
    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (isInMainFile(VD))
      TheVars.push_back(VD);
    return true;
  }

private:
  std::vector<clang::VarDecl *> TheVars;
  std::vector<clang::FunctionDecl *> TheFuncs;
};

// static RegisterMutator<Test> M("Test", "Just for test");
