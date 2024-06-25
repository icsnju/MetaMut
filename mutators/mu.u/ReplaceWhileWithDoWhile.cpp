#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceWhileWithDoWhile : public Mutator,
                                public clang::RecursiveASTVisitor<ReplaceWhileWithDoWhile> {
public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheWhileStmts.empty()) return false;

    WhileStmt *stmt = randElement(TheWhileStmts);

    std::string stmtText = getSourceText(stmt);
    std::size_t whilePos = stmtText.find("while");
    std::size_t conditionBegin = stmtText.find("(", whilePos);
    std::size_t conditionEnd = stmtText.find(")", conditionBegin);
    std::string condition = stmtText.substr(conditionBegin, conditionEnd - conditionBegin + 1);
    std::string body = stmtText.substr(conditionEnd + 1);

    std::string newStmt = "do" + body + "while" + condition + ";";

    getRewriter().ReplaceText(stmt->getSourceRange(), newStmt);

    return true;
  }

  bool VisitWhileStmt(clang::WhileStmt *WS) {
    TheWhileStmts.push_back(WS);
    return true;
  }

private:
  std::vector<clang::WhileStmt *> TheWhileStmts;
};

static RegisterMutator<ReplaceWhileWithDoWhile> M("u1.ReplaceWhileWithDoWhile", "Replace a WhileStmt with a DoWhileStmt, retaining the condition and body.");