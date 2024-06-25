#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class ChangeCharLiteral : public Mutator,
                          public clang::RecursiveASTVisitor<ChangeCharLiteral> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitCharacterLiteral(clang::CharacterLiteral *CL);

private:
  std::vector<clang::CharacterLiteral *> TheLiterals;
};

static RegisterMutator<ChangeCharLiteral> M("s.change-charliteral", "Change a CharacterLiteral's value.");

bool ChangeCharLiteral::VisitCharacterLiteral(CharacterLiteral *CL) {
  if (isMutationSite(CL))
    TheLiterals.push_back(CL);
  return true;
}

bool ChangeCharLiteral::mutate() {
  TraverseAST(getASTContext());
  if (TheLiterals.empty())
    return false;

  CharacterLiteral *literal = randElement(TheLiterals);

  // Generate a new random character
  char newChar = static_cast<char>(rand() % 256);

  // Convert the character to its escaped form if necessary
  std::string newCharStr;
  switch (newChar) {
  case '\0':
    newCharStr = "\\0";
    break;
  case '\'':
    newCharStr = "\\\'";
    break;
  case '\"':
    newCharStr = "\\\"";
    break;
  case '\a':
    newCharStr = "\\a";
    break;
  case '\b':
    newCharStr = "\\b";
    break;
  case '\f':
    newCharStr = "\\f";
    break;
  case '\n':
    newCharStr = "\\n";
    break;
  case '\r':
    newCharStr = "\\r";
    break;
  case '\t':
    newCharStr = "\\t";
    break;
  case '\v':
    newCharStr = "\\v";
    break;
  case '\\':
    newCharStr = "\\\\";
    break;
  default:
    if (newChar < 0x20 || newChar > 0x7E) {
      // Non-printable ASCII characters are output as octal escape sequences
      char octalStr[5];
      sprintf(octalStr, "\\%03o", static_cast<unsigned char>(newChar));
      newCharStr = octalStr;
    } else {
      newCharStr = newChar;
    }
  }

  // Replace the old literal with the new one
  getRewriter().ReplaceText(literal->getSourceRange(), "'" + newCharStr + "'");

  return true;
}
