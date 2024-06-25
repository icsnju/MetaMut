// EmbedCompoundStmt.cpp
#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class EmbedCompoundStmt : public Mutator {
public:
  EmbedCompoundStmt(const char *name, const char *desc) : Mutator(name, desc) {}
  bool mutate() override;

private:
  std::vector<CompoundStmt *> CompoundStmts;

  class StmtVisitor : public RecursiveASTVisitor<StmtVisitor> {
  public:
    StmtVisitor(EmbedCompoundStmt *Instance) : Instance(Instance) {}
    bool VisitCompoundStmt(CompoundStmt *CS) {
      Instance->CompoundStmts.push_back(CS);
      return true;
    }

  private:
    EmbedCompoundStmt *Instance;
  };
};

bool EmbedCompoundStmt::mutate() {
  StmtVisitor Visitor(this);
  Visitor.TraverseDecl(getASTContext().getTranslationUnitDecl());
  if (CompoundStmts.empty()) return false;

  CompoundStmt *stmt = randElement(CompoundStmts);

  // perform rewrite
  SourceLocation stmtStart = stmt->getBeginLoc();
  SourceLocation stmtEnd = getLocForEndOfToken(stmt->getEndLoc());

  getRewriter().InsertTextBefore(stmtStart, "{ ");
  getRewriter().InsertTextAfter(stmtEnd, " }");

  return true;
}

static RegisterMutator<EmbedCompoundStmt> M("u1.EmbedCompoundStmt", "Embed a CompoundStmt inside another CompoundStmt");