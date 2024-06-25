#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ReplaceRandomTypeWithTypedef : public Mutator,
                                     public clang::RecursiveASTVisitor<ReplaceRandomTypeWithTypedef> {

public:
  using Mutator::Mutator;

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheTypes.empty()) return false;

    QualType type = randElement(TheTypes);
    
    // Generate a new name for the typedef
    std::string newTypedefName = generateUniqueName("typedef_t");

    // Format the typedef as a declaration
    std::string typedefDecl = "typedef " + type.getAsString() + " " + newTypedefName + ";";

    // Insert the typedef at the beginning of the main file
    SourceLocation fileStart = getRewriter().getSourceMgr().getLocForStartOfFile(getRewriter().getSourceMgr().getMainFileID());
    getRewriter().InsertTextBefore(fileStart, typedefDecl + "\n");

    // Replace all occurrences of the type with the new typedef
    for (const auto &typeRange : TypeRanges[type]) {
      getRewriter().ReplaceText(typeRange, newTypedefName);
    }

    return true;
  }

  bool VisitTypeLoc(TypeLoc TL) {
    QualType type = TL.getType();
    if (!type->isBuiltinType()) return true;

    TheTypes.push_back(type);
    TypeRanges[type].push_back(TL.getSourceRange());

    return true;
  }

private:
  std::map<QualType, std::vector<SourceRange>> TypeRanges;
  std::vector<QualType> TheTypes;
};

static RegisterMutator<ReplaceRandomTypeWithTypedef> M("u1.ReplaceRandomTypeWithTypedef", 
    "Randomly selects a Type and replaces it with a typedef of the same underlying type.");