#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeLoopType : public Mutator,
                       public clang::RecursiveASTVisitor<ChangeLoopType> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitForStmt(clang::ForStmt *FS);
  bool VisitWhileStmt(clang::WhileStmt *WS);

private:
  std::vector<clang::ForStmt *> TheForLoops;
  std::vector<clang::WhileStmt *> TheWhileLoops;
};

static RegisterMutator<ChangeLoopType> M("s.change-looptype",
      "Change a loop type ('for' to 'while' and vice versa).");

bool ChangeLoopType::VisitForStmt(ForStmt *FS) {
  if (isMutationSite(FS))
    TheForLoops.push_back(FS);
  return true;
}

bool ChangeLoopType::VisitWhileStmt(WhileStmt *WS) {
  if (isMutationSite(WS))
    TheWhileLoops.push_back(WS);
  return true;
}

bool ChangeLoopType::mutate() {
  TraverseAST(getASTContext());
  if (TheForLoops.empty() && TheWhileLoops.empty())
    return false;

  if (!TheForLoops.empty()) {
    // Change a 'for' loop into a 'while' loop
    ForStmt *FS = randElement(TheForLoops);
    std::string whileLoop;
    if (FS->getInit())
      whileLoop += getSourceText(FS->getInit()) + ";\n";
    if (FS->getCond())
      whileLoop += "while(" + getSourceText(FS->getCond()) + ") {\n";
    else
      whileLoop += "while(1) {\n";
    if (FS->getBody())
      whileLoop += getSourceText(FS->getBody());
    if (FS->getInc())
      whileLoop += getSourceText(FS->getInc()) + ";\n";
    whileLoop += "}\n";
    getRewriter().ReplaceText(FS->getSourceRange(), whileLoop);
  } else {
    // Change a 'while' loop into a 'for' loop
    WhileStmt *WS = randElement(TheWhileLoops);
    std::string forLoop =
        "for (; " +
        Lexer::getSourceText(
            CharSourceRange::getTokenRange(WS->getCond()->getSourceRange()),
            getASTContext().getSourceManager(), getASTContext().getLangOpts()).str()
             +
        ";) " + getSourceText(WS->getBody());
    getRewriter().ReplaceText(WS->getSourceRange(), forLoop);
  }

  return true;
}
