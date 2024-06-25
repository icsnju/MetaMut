#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ModifyRandomLabelStmt
    : public Mutator,
      public clang::RecursiveASTVisitor<ModifyRandomLabelStmt> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLabels.empty() || TheGotos.empty()) return false;

    LabelStmt *label = randElement(TheLabels);
    std::string newLabelName = generateUniqueName(label->getName());
    
    getRewriter().ReplaceText(label->getIdentLoc(), label->getName());
    getRewriter().InsertTextAfter(label->getIdentLoc(), newLabelName);

    for (auto *gotoStmt : TheGotos) {
      if (gotoStmt->getLabel() && gotoStmt->getLabel()->getStmt() == label) {
        getRewriter().ReplaceText(gotoStmt->getLabelLoc(), label->getName());
        getRewriter().InsertTextAfter(gotoStmt->getLabelLoc(), newLabelName);
      }
    }

    return true;
  }

  bool VisitLabelStmt(clang::LabelStmt *LS) {
    TheLabels.push_back(LS);
    return true;
  }

  bool VisitGotoStmt(clang::GotoStmt *GS) {
    TheGotos.push_back(GS);
    return true;
  }

private:
  std::vector<clang::LabelStmt *> TheLabels;
  std::vector<clang::GotoStmt *> TheGotos;
};

static RegisterMutator<ModifyRandomLabelStmt> M(
    "u2.ModifyRandomLabelStmt", "Change a label statement's identifier and update corresponding goto statements.");
