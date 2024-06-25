#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class InlineFunctionReversal
    : public Mutator,
      public clang::RecursiveASTVisitor<InlineFunctionReversal> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFunctionDecl(clang::FunctionDecl *FD);

private:
  std::vector<clang::FunctionDecl *> TheFunctions;
};

static RegisterMutator<InlineFunctionReversal> M("s.inverse-inline", "Toggle inline attributes of function declarations.");

bool InlineFunctionReversal::VisitFunctionDecl(clang::FunctionDecl *FD) {
  if (isMutationSite(FD))
    TheFunctions.push_back(clang::dyn_cast<clang::FunctionDecl>(FD));
  return true;
}

bool InlineFunctionReversal::mutate() {
  TraverseAST(getASTContext());
  if (TheFunctions.empty())
    return false;

  clang::FunctionDecl *func = randElement(TheFunctions);
  clang::Decl *TUD = getMostRecentTranslationUnitDecl(func);

  auto &SM = getASTContext().getSourceManager();
  if (func->isInlineSpecified()) {
    auto &LO = getASTContext().getLangOpts();
    SourceLocation InlineStart =
        findStringLocationFrom(TUD->getBeginLoc(), "inline");
    clang::PresumedLoc PLoc = SM.getPresumedLoc(InlineStart);
    SourceLocation InlineEnd = InlineStart.getLocWithOffset(strlen("inline"));
    // Replace the 'inline' keyword with an empty string
    getRewriter().ReplaceText(SourceRange(InlineStart, InlineEnd), "");
  } else if (func->hasAttr<clang::AlwaysInlineAttr>()) {
    SourceLocation AttrStart =
        findStringLocationFrom(TUD->getBeginLoc(), "__attribute__");
    SourceLocation AttrEnd = findBracesRange(AttrStart).getEnd();
    if (AttrStart.isValid() && AttrEnd.isValid())
      getRewriter().ReplaceText(SourceRange(AttrStart, AttrEnd), "");
  } else if (func->hasAttr<clang::NoInlineAttr>()) {
    SourceLocation AttrStart =
        findStringLocationFrom(TUD->getBeginLoc(), "__attribute__");
    SourceLocation AttrEnd = findBracesRange(AttrStart).getEnd();
    if (AttrStart.isValid() && AttrEnd.isValid())
      getRewriter().ReplaceText(SourceRange(AttrStart, AttrEnd), "");
  } else {
    switch (randIndex(3)) {
    case 0:
      getRewriter().InsertText(func->getBeginLoc(), "inline ");
      break;
    case 1:
      getRewriter().InsertText(func->getBeginLoc(),
                               " __attribute__((always_inline)) ");
      break;
    case 2:
      getRewriter().InsertText(func->getBeginLoc(),
                               " __attribute__((noinline)) ");
      break;
    }
  }

  return true;
}
