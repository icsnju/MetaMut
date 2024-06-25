#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwitchToGoto : public Mutator,
                     public clang::RecursiveASTVisitor<SwitchToGoto> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitSwitchStmt(clang::SwitchStmt *SS);

private:
  std::vector<clang::SwitchStmt *> TheSwitches;
  std::string getSourceTextRemovingBreak(clang::Stmt *stmt,
                                         const std::string &endLabel);
};

static RegisterMutator<SwitchToGoto> M("s.switch-to-goto", "Convert a switch statement to goto statements.");

bool SwitchToGoto::VisitSwitchStmt(SwitchStmt *SS) {
  if (isMutationSite(SS))
    TheSwitches.push_back(SS);
  return true;
}

std::string
SwitchToGoto::getSourceTextRemovingBreak(clang::Stmt *stmt,
                                         const std::string &endLabel) {
  std::string stmtText = getSourceText(stmt);

  // Replace "break;" with "goto end_label;"
  size_t index = 0;
  std::string searchStr = "break";
  std::string replaceStr = "goto " + endLabel;
  while (true) {
    /* Locate the substring to replace. */
    index = stmtText.find(searchStr, index);
    if (index == std::string::npos)
      break;

    /* Make the replacement. */
    stmtText.replace(index, searchStr.size(), replaceStr);

    /* Advance index forward so the next iteration doesn't pick it up as well.
     */
    index += replaceStr.size();
  }

  return stmtText;
}

bool SwitchToGoto::mutate() {
  TraverseAST(getASTContext());
  if (TheSwitches.empty())
    return false;

  SwitchStmt *switchStmt = randElement(TheSwitches);

  std::string gotoText;
  std::string endLabel = generateUniqueName("end_label");

  for (SwitchCase *SC = switchStmt->getSwitchCaseList(); SC;
       SC = SC->getNextSwitchCase()) {
    std::string caseLabel = generateUniqueName("case_label");

    if (isa<CaseStmt>(SC)) {
      gotoText += "if (";
      gotoText += getSourceText(switchStmt->getCond());
      gotoText += " == ";
      gotoText += getSourceText(cast<CaseStmt>(SC)->getLHS());
      gotoText += ") goto ";
      gotoText += caseLabel;
      gotoText += ";\n";
    } else { // DefaultStmt
      gotoText += "goto ";
      gotoText += caseLabel;
      gotoText += ";\n";
    }

    gotoText += caseLabel;
    gotoText += ": \n";
    gotoText += getSourceTextRemovingBreak(SC->getSubStmt(), endLabel);
    gotoText += ";\n";
  }

  gotoText += endLabel;
  gotoText += ": ;";

  getRewriter().ReplaceText(switchStmt->getSourceRange(), gotoText);

  return true;
}
