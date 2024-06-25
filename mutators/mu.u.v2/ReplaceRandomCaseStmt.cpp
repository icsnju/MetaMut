#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomCaseStmt
    : public Mutator,
      public clang::RecursiveASTVisitor<ReplaceRandomCaseStmt> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheSwitchStmts.empty()) return false;

    SwitchStmt *selectedSwitch = randElement(TheSwitchStmts);

    std::vector<CaseStmt *> caseStmts;
    CompoundStmt *switchBody = dyn_cast<CompoundStmt>(selectedSwitch->getBody());
    for (auto &stmt : switchBody->body()) {
      if (CaseStmt *caseStmt = dyn_cast<CaseStmt>(stmt)) {
        caseStmts.push_back(caseStmt);
      }
    }
    if (caseStmts.empty()) return false;

    CaseStmt *randomCase = randElement(caseStmts);
    int newConstant = generateNewConstant(caseStmts);
    std::string newCaseCode = std::to_string(newConstant);

    getRewriter().ReplaceText(randomCase->getLHS()->getSourceRange(), newCaseCode);
    return true;
  }

  bool VisitSwitchStmt(clang::SwitchStmt *SS) {
    TheSwitchStmts.push_back(SS);
    return true;
  }

private:
  std::vector<clang::SwitchStmt *> TheSwitchStmts;

  int generateNewConstant(const std::vector<CaseStmt *> &caseStmts) {
    int newConstant = 0;
    bool unique;
    do {
      unique = true;
      newConstant = randIndex(100);
      for (const auto &caseStmt : caseStmts) {
        if (auto *ICE = dyn_cast<IntegerLiteral>(caseStmt->getLHS())) {
          if (newConstant == ICE->getValue().getSExtValue()) {
            unique = false;
            break;
          }
        }
      }
    } while (!unique);

    return newConstant;
  }
};

static RegisterMutator<ReplaceRandomCaseStmt> M(
    "u2.ReplaceRandomCaseStmt", "Replaces a random case statement with a new unique integer constant and an associated block of code.");
