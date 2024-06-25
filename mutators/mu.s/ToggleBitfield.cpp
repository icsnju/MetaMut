#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ToggleBitfield : public Mutator,
                       public clang::RecursiveASTVisitor<ToggleBitfield> {
public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitFieldDecl(clang::FieldDecl *FD);
  bool VisitBinaryOperator(clang::BinaryOperator *BO);

private:
  std::vector<clang::FieldDecl *> TheFields;
  std::set<clang::FieldDecl *> FieldUsedAsLValue;
};

static RegisterMutator<ToggleBitfield> M("s.toggle-bitfield", "Toggle bitfield of a struct or union field.");

bool ToggleBitfield::VisitFieldDecl(FieldDecl *FD) {
  if (isMutationSite(FD))
    TheFields.push_back(FD);
  return true;
}

bool ToggleBitfield::VisitBinaryOperator(clang::BinaryOperator *BO) {
  if (BO->isAssignmentOp()) {
    if (auto declRefExpr = llvm::dyn_cast<clang::DeclRefExpr>(
            BO->getLHS()->IgnoreParenImpCasts())) {
      if (auto FD = llvm::dyn_cast<FieldDecl>(declRefExpr->getDecl())) {
        FieldUsedAsLValue.insert(FD);
      }
    }
  }
  return true;
}

bool ToggleBitfield::mutate() {
  TraverseAST(getASTContext());
  if (TheFields.empty())
    return false;

  // Separate the fields into bitfield fields and non-bitfield fields
  std::vector<FieldDecl *> bitfieldFields;
  std::vector<FieldDecl *> nonBitfieldFields;
  for (auto FD : TheFields) {
    if (FD->isBitField()) {
      bitfieldFields.push_back(FD);
    } else if (FieldUsedAsLValue.find(FD) == FieldUsedAsLValue.end()) {
      nonBitfieldFields.push_back(FD);
    }
  }

  // If there are no eligible fields to mutate, return false
  if (bitfieldFields.empty() && nonBitfieldFields.empty()) {
    return false;
  }

  // Choose a random eligible field to mutate
  FieldDecl *field;
  if (!bitfieldFields.empty() && !nonBitfieldFields.empty()) {
    // If there are both bitfield fields and non-bitfield fields, choose
    // randomly between them
    if (randBool()) {
      field = randElement(bitfieldFields);
    } else {
      field = randElement(nonBitfieldFields);
    }
  } else if (!bitfieldFields.empty()) {
    // If there are only bitfield fields, choose one of them
    field = randElement(bitfieldFields);
  } else {
    // If there are only non-bitfield fields, choose one of them
    field = randElement(nonBitfieldFields);
  }

  std::string newFieldDecl;
  if (field->isBitField()) {
    // Remove the bitfield
    newFieldDecl =
        field->getType().getAsString() + " " + field->getName().str() + ";";
  } else {
    // Add a new bitfield
    int bits = randIndex(64); // Generate a random bitfield from 0 to 64
    newFieldDecl = field->getType().getAsString() + " " +
                   field->getName().str() + " : " + std::to_string(bits) + ";";
  }

  // Replace the original field with the new field declaration
  SourceLocation start = field->getBeginLoc();
  SourceLocation end = Lexer::getLocForEndOfToken(
      field->getEndLoc(), 0, getASTContext().getSourceManager(),
      getASTContext().getLangOpts());
  getRewriter().ReplaceText(clang::SourceRange(start, end), newFieldDecl);
  return true;
}
