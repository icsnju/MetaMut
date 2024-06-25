#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyPointerTarget : public Mutator,
                      public clang::RecursiveASTVisitor<ModifyPointerTarget> {
public:
  using Mutator::Mutator;
  
  bool mutate() override {
    TraverseAST(getASTContext());
    if (PointerDecls.empty() || IntDecls.empty()) return false;

    VarDecl *ptr = randElement(PointerDecls);
    VarDecl *newTarget = randElement(IntDecls);
    if (ptr->getAnyInitializer()->getBeginLoc() == newTarget->getEndLoc()) return false;
    
    std::string replacement = "&" + newTarget->getName().str();
    getRewriter().ReplaceText(ptr->getAnyInitializer()->getSourceRange(), replacement);

    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (VD->getType()->isPointerType())
      PointerDecls.push_back(VD);
    else if (VD->getType()->isIntegerType())
      IntDecls.push_back(VD);
    return true;
  }

private:
  std::vector<clang::VarDecl *> PointerDecls;
  std::vector<clang::VarDecl *> IntDecls;
};

static RegisterMutator<ModifyPointerTarget> M("u1.ModifyPointerTarget", "Modifies the target of a random PointerType variable to another integer variable available in the current scope.");
