#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InsertCaseStmt : public Mutator,
                       public clang::RecursiveASTVisitor<InsertCaseStmt> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheSwitchStmts.empty()) return false;

    SwitchStmt *stmt = randElement(TheSwitchStmts);

    // Create a unique case value and a block of code
    int caseValue = generateUniqueCaseValue(stmt);
    std::string newCase = "case " + std::to_string(caseValue) + ":\n";
    newCase += "  printf(\"Inserted case for value: %d\\n\", " + std::to_string(caseValue) + ");\n";
    newCase += "  break;\n";

    // Find the appropriate location to insert the new case
    CompoundStmt *cs = llvm::cast<CompoundStmt>(stmt->getBody());
    Stmt *lastCaseStmt = nullptr;
    for (auto &childStmt : cs->body()) {
      if (CaseStmt *caseStmt = llvm::dyn_cast<CaseStmt>(childStmt)) {
        lastCaseStmt = childStmt;
      }
    }
    
    if (lastCaseStmt != nullptr) {
        SourceLocation insertLoc = getLocForEndOfToken(lastCaseStmt->getEndLoc());
        SourceLocation semicolonLoc = findStringLocationFrom(insertLoc, ";");
        getRewriter().InsertTextAfter(semicolonLoc.getLocWithOffset(1), "\n" + newCase);
    } else {
      SourceLocation insertLoc = stmt->getBody()->getBeginLoc();
      getRewriter().InsertTextBefore(insertLoc, "\n" + newCase);
    }

    return true;
  }

  bool VisitSwitchStmt(clang::SwitchStmt *SS) {
    TheSwitchStmts.push_back(SS);
    return true;
  }

private:
  std::vector<clang::SwitchStmt *> TheSwitchStmts;

  int generateUniqueCaseValue(SwitchStmt *stmt) {
    CompoundStmt *cs = llvm::cast<CompoundStmt>(stmt->getBody());
    std::set<int> caseValues;

    for (auto &childStmt : cs->body()) {
      if (CaseStmt *caseStmt = llvm::dyn_cast<CaseStmt>(childStmt)) {
        caseValues.insert(caseStmt->getLHS()->EvaluateKnownConstInt(getASTContext()).getZExtValue());
      }
    }

    int newCaseValue = rand() % 1024;
    while (caseValues.count(newCaseValue)) {
      newCaseValue = rand() % 1024;
    }

    return newCaseValue;
  }
};

static RegisterMutator<InsertCaseStmt> M(
    "u2.InsertCaseStmt", "Inserts a new case statement with a unique integer constant and an associated block of code in a switch statement.");
