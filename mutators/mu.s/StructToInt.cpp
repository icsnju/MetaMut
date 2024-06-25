#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/TypeLoc.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class StructToInt : public Mutator,
                    public clang::RecursiveASTVisitor<StructToInt> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitElaboratedTypeLoc(clang::ElaboratedTypeLoc ETL);

private:
  std::vector<const clang::TagDecl *> StructDecls;
  std::map<const clang::TagDecl *, std::vector<clang::TypeLoc>> StructsMap;
};

static RegisterMutator<StructToInt> M("s.struct-to-int",
                                      "Change a struct type to int type.");

bool StructToInt::VisitElaboratedTypeLoc(clang::ElaboratedTypeLoc ETL) {
  if (ETL.getNamedTypeLoc().getType()->isStructureType()) {
    const RecordDecl *RD = ETL.getNamedTypeLoc()
                               .getType()
                               ->getAsStructureType()
                               ->getDecl()
                               ->getDefinition();
    if (RD && RD->getCanonicalDecl()) {
      StructDecls.push_back(RD->getCanonicalDecl());
      StructsMap[RD->getCanonicalDecl()].push_back(ETL);
    }
  }
  return true;
}

bool StructToInt::mutate() {
  TraverseAST(getASTContext());
  if (StructsMap.empty())
    return false;

  auto *selectedStruct = randElement(StructDecls);

  for (auto &loc : StructsMap[selectedStruct]) {
    getRewriter().ReplaceText(loc.getSourceRange(), "int");
  }

  return true;
}
