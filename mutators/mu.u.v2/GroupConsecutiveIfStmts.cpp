#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class GroupConsecutiveIfStmts
    : public Mutator,
      public clang::RecursiveASTVisitor<GroupConsecutiveIfStmts> {
public:
  using Mutator::Mutator;

  bool VisitIfStmt(clang::IfStmt *IfS) {
    IfStmts.push_back(IfS);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (IfStmts.size() < 2) return false;

    clang::IfStmt *firstIf = randElement(IfStmts);
    clang::IfStmt *secondIf = nullptr;
    for (auto it = IfStmts.begin(); it != IfStmts.end(); ++it) {
      if ((*it) == firstIf) {
        ++it;
        if (it != IfStmts.end()) {
          secondIf = (*it);
          break;
        }
      }
    }

    if (secondIf == nullptr) return false;

    std::string firstIfSource = getSourceText(firstIf);
    std::string secondIfSource = getSourceText(secondIf);
    std::string combinedIfSource = firstIfSource + " else " + secondIfSource;

    SourceRange firstToEndRange(firstIf->getBeginLoc(), getLocForEndOfToken(secondIf->getEndLoc()));
    getRewriter().ReplaceText(firstToEndRange, combinedIfSource);

    return true;
  }

private:
  std::vector<clang::IfStmt *> IfStmts;
};

static RegisterMutator<GroupConsecutiveIfStmts> M(
    "u2.GroupConsecutiveIfStmts",
    "It takes consecutive IfStmts with unrelated conditions and combines them into a single nested IfStmt, improving code organization and readability.");
