#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class RandomizeStringLiteral
    : public Mutator,
      public clang::RecursiveASTVisitor<RandomizeStringLiteral> {

public:
  using Mutator::Mutator;
  bool mutate() override;
  bool VisitStringLiteral(clang::StringLiteral *SL);

private:
  std::vector<clang::StringLiteral *> TheStringLiterals;
  std::string mutateString(const std::string &oldString);
  std::string escape(const std::string &s);
};

static RegisterMutator<RandomizeStringLiteral> M("s.randomize-string-literal",
      "Randomly add/delete/duplicate characters in a string literal.");

bool RandomizeStringLiteral::VisitStringLiteral(StringLiteral *SL) {
  if (isMutationSite(SL))
    TheStringLiterals.push_back(SL);
  return true;
}

bool RandomizeStringLiteral::mutate() {
  TraverseAST(getASTContext());
  if (TheStringLiterals.empty())
    return false;

  StringLiteral *SL = randElement(TheStringLiterals);
  std::string oldString = SL->getBytes().str();
  std::string newString = mutateString(oldString);

  // Replace the old string literal with the new one
  getRewriter().ReplaceText(SL->getSourceRange(), "\"" + newString + "\"");

  return true;
}

std::string to_hex(char val) {
  assert(val < 16);
  val += (val < 10) ? '0' : ('a' - 10);
  return std::string(&val, 1);
}

std::string RandomizeStringLiteral::escape(const std::string &s) {
  std::string res;
  for (char c : s) {
    switch (c) {
    case '\\':
      res += "\\\\";
      break;
    case '\n':
      res += "\\n";
      break;
    case '\t':
      res += "\\t";
      break;
    case '"':
      res += "\\\"";
      break;
    case '\'':
      res += "\\\'";
      break;
    default:
      if (isprint(c))
        res += c;
      else
        res += "\\x" + to_hex((c & 0xF0) >> 4) + to_hex(c & 0xF);
      break;
    }
  }
  return res;
}

std::string RandomizeStringLiteral::mutateString(const std::string &oldString) {
  std::string newString = oldString;
  // Randomly choose between add (0), delete (1), duplicate (2)
  int operation = randIndex(3);

  if (operation == 0 && newString.length() > 0) { // Add
    char c = randIndex(128);
    newString.insert(newString.begin() + randIndex(newString.length()), c);
  } else if (operation == 1 && newString.length() > 1) { // Delete
    newString.erase(newString.begin() + randIndex(newString.length()));
  } else if (operation == 2 && newString.length() > 0) { // Duplicate
    char c = newString[randIndex(newString.length())];
    newString.insert(newString.begin() + randIndex(newString.length()), c);
  }

  return escape(newString);
}
