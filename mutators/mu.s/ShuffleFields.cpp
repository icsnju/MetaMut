#include <algorithm>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <random>
#include <vector>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ShuffleFields : public Mutator,
                      public clang::RecursiveASTVisitor<ShuffleFields> {
  using VisitorTy = clang::RecursiveASTVisitor<ShuffleFields>;

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitRecordDecl(clang::RecordDecl *D);

private:
  std::vector<clang::RecordDecl *> Records;
};

static RegisterMutator<ShuffleFields> M("s.shuffle-fields",
                                        "Shuffle fields of a record.");

bool ShuffleFields::VisitRecordDecl(RecordDecl *D) {
  Records.push_back(D);
  return true;
}

bool ShuffleFields::mutate() {
  TraverseAST(getASTContext());

  if (Records.empty())
    return false;

  // Select a random RecordDecl
  RecordDecl *D = randElement(Records);

  // Store fields and their original source code
  std::vector<FieldDecl *> Fields;
  std::vector<std::string> originalSources;
  for (FieldDecl *FD : D->fields()) {
    Fields.push_back(FD);
    originalSources.push_back(
        getRewriter().getRewrittenText(FD->getSourceRange()));
  }

  // Shuffle the fields
  std::shuffle(Fields.begin(), Fields.end(), getManager().getRandomGenerator());

  // Replace each field with its new source code
  for (std::size_t i = 0; i < Fields.size(); ++i) {
    getRewriter().ReplaceText(Fields[i]->getSourceRange(), originalSources[i]);
  }

  return true;
}
