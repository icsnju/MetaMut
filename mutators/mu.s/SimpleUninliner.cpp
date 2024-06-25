#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

class SimpleUninliner : public Mutator,
                        public clang::RecursiveASTVisitor<SimpleUninliner> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);
  bool VisitCompoundStmt(clang::CompoundStmt *CS);

private:
  std::vector<clang::CompoundStmt *> TheStmts;
  clang::FunctionDecl *FirstFD = nullptr;
};

static RegisterMutator<SimpleUninliner> M("s.simple-uninliner", "Turn a block of code into a function call.");

bool SimpleUninliner::VisitFunctionDecl(clang::FunctionDecl *FD) {
  if (!FirstFD)
    FirstFD = FD;
  return true;
}

bool SimpleUninliner::VisitCompoundStmt(clang::CompoundStmt *CS) {
  if (isMutationSite(CS))
    TheStmts.push_back(CS);
  return true;
}

class VariableCollector : public clang::RecursiveASTVisitor<VariableCollector> {
public:
  bool VisitDeclRefExpr(clang::DeclRefExpr *DRE) {
    if (auto *VD = clang::dyn_cast<clang::VarDecl>(DRE->getDecl())) {
      varsUsed.insert(VD);
    }
    return true;
  }

  std::set<clang::VarDecl *> varsUsed;
};

bool SimpleUninliner::mutate() {
  TraverseAST(getASTContext());
  if (TheStmts.empty())
    return false;

  clang::CompoundStmt *stmt = randElement(TheStmts);

  // Use VariableCollector to find the variables used in the stmt block
  VariableCollector VC;
  VC.TraverseStmt(stmt);

  // Extract the block of code into a string
  std::string stmtBody = getSourceText(stmt);

  // Generate a unique name for the new function
  std::string funcName = generateUniqueName("uninliner");

  // Generate the function argument list
  std::string funcArgs;
  std::string callArgs;
  for (const auto &var : VC.varsUsed) {
    funcArgs += formatAsDecl(var->getType(), var->getNameAsString()) + ", ";
    callArgs += var->getNameAsString() + ", ";
  }

  // Remove trailing commas from the argument lists
  if (!funcArgs.empty())
    funcArgs.erase(funcArgs.length() - 2);
  if (!callArgs.empty())
    callArgs.erase(callArgs.length() - 2);

  // Create the new function definition
  std::string funcDef =
      "void " + funcName + "(" + funcArgs + ") {\n" + stmtBody + "}\n";

  // Replace the block of code with a call to the new function
  getRewriter().ReplaceText(
      clang::SourceRange(stmt->getLBracLoc().getLocWithOffset(1),
                         stmt->getRBracLoc().getLocWithOffset(-1)),
      funcName + "(" + callArgs + ");");

  // Insert the function definition at the top of the file
  getRewriter().InsertText(
      getMostRecentTranslationUnitDecl(FirstFD)->getBeginLoc(), funcDef);
  return true;
}
