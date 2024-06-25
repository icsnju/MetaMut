#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertRandomSwitchStmt : public Mutator,
                               public clang::RecursiveASTVisitor<InsertRandomSwitchStmt> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheLadders.empty()) return false;

    // Select a random if-else-if ladder
    IfStmt *stmt = randElement(TheLadders);

    // Construct the switch-case statement
    std::string switchStmt = "switch (x) {\n";
    for (const auto &caseStmt : getCaseStatements(stmt)) {
      switchStmt += "case " + caseStmt.first + ":\n" + caseStmt.second + ";\nbreak;\n";
    }
    switchStmt += "}\n";

    // Replace the if-else-if ladder with the switch-case statement
    getRewriter().ReplaceText(stmt->getSourceRange(), switchStmt);

    return true;
  }

  bool VisitIfStmt(clang::IfStmt *stmt) {
    if (isMutationSite(stmt)) {
      TheLadders.push_back(stmt);
    }
    return true;
  }

private:
  std::vector<clang::IfStmt *> TheLadders;

  std::vector<std::pair<std::string, std::string>> getCaseStatements(clang::IfStmt *stmt) {
    std::vector<std::pair<std::string, std::string>> caseStmts;

    while (stmt != nullptr) {
      auto *cond = dyn_cast<BinaryOperator>(stmt->getCond());
      if (cond == nullptr || cond->getOpcode() != BO_EQ) {
        break;
      }

      std::string caseValue = getSourceText(cond->getRHS());
      std::string caseBody = getSourceText(stmt->getThen());
      caseStmts.emplace_back(caseValue, caseBody);

      stmt = dyn_cast<IfStmt>(stmt->getElse());
    }

    return caseStmts;
  }
};

static RegisterMutator<InsertRandomSwitchStmt> M("u1.InsertRandomSwitchStmt", "Replace if-else-if ladder with a switch-case construct.");