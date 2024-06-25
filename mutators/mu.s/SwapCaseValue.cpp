#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class SwapCaseValue : public Mutator,
                      public clang::RecursiveASTVisitor<SwapCaseValue> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitSwitchStmt(clang::SwitchStmt *SS);

private:
  std::vector<clang::SwitchStmt *> TheSwitches;
};

static RegisterMutator<SwapCaseValue> M("s.swap-casevalue",
                                        "Swap two CaseStmt's values.");

bool SwapCaseValue::VisitSwitchStmt(SwitchStmt *SS) {
  if (isMutationSite(SS))
    TheSwitches.push_back(SS);
  return true;
}

bool SwapCaseValue::mutate() {
  TraverseAST(getASTContext());
  if (TheSwitches.empty())
    return false;

  // Randomly select a SwitchStmt
  SwitchStmt *selectedSwitch = randElement(TheSwitches);

  // Collect all CaseStmts within the selected SwitchStmt
  std::vector<CaseStmt *> caseStmts;
  for (SwitchCase *SC = selectedSwitch->getSwitchCaseList(); SC;
       SC = SC->getNextSwitchCase()) {
    if (CaseStmt *CS = dyn_cast<CaseStmt>(SC))
      caseStmts.push_back(CS);
  }

  // Ensure there are at least two cases to swap
  if (caseStmts.size() < 2)
    return false;

  // Randomly select two CaseStmts
  CaseStmt *case1 = randElement(caseStmts);
  CaseStmt *case2 = randElement(caseStmts);

  // Ensure that the two cases are not the same
  while (case1 == case2) {
    case2 = randElement(caseStmts);
  }

  // Get the original case values
  std::string val1 = getSourceText(case1->getLHS());
  std::string val2 = getSourceText(case2->getLHS());

  // Swap the case values
  getRewriter().ReplaceText(case1->getLHS()->getSourceRange(), val2);
  getRewriter().ReplaceText(case2->getLHS()->getSourceRange(), val1);

  return true;
}
