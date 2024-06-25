#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class IfToGoto : public Mutator, public clang::RecursiveASTVisitor<IfToGoto> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitIfStmt(clang::IfStmt *IS);

private:
  std::vector<clang::IfStmt *> TheIfs;
};

static RegisterMutator<IfToGoto> M("s.if-to-goto", "Convert an if-else statement to goto statements.");

bool IfToGoto::VisitIfStmt(IfStmt *IS) {
  if (isMutationSite(IS))
    TheIfs.push_back(IS);
  return true;
}

bool IfToGoto::mutate() {
  TraverseAST(getASTContext());
  if (TheIfs.empty())
    return false;

  IfStmt *ifStmt = randElement(TheIfs);

  std::string elseLabel = generateUniqueName("else_label");
  std::string endLabel = generateUniqueName("end_label");

  std::string gotoText = "if (!(";
  gotoText += getSourceText(ifStmt->getCond());
  gotoText += ")) goto ";
  gotoText += elseLabel;
  gotoText += ";\n";
  gotoText += getSourceText(ifStmt->getThen());
  gotoText += "\ngoto ";
  gotoText += endLabel;
  gotoText += ";\n";
  gotoText += elseLabel;
  gotoText += ": \n";
  if (ifStmt->getElse()) {
    gotoText += getSourceText(ifStmt->getElse());
  }
  gotoText += "\n";
  gotoText += endLabel;
  gotoText += ": ;";

  getRewriter().ReplaceText(ifStmt->getSourceRange(), gotoText);

  return true;
}
