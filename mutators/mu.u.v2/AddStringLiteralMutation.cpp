#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class AddStringLiteralMutation
    : public Mutator,
      public clang::RecursiveASTVisitor<AddStringLiteralMutation> {
public:
  using Mutator::Mutator;
  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheStringLiterals.empty()) return false;

    StringLiteral *stringLiteral = randElement(TheStringLiterals);

    std::string newText = mutateStringLiteral(stringLiteral->getString().str());
    if (newText.empty()) return false;

    getRewriter().ReplaceText(stringLiteral->getSourceRange(), "\"" + newText + "\"");

    return true;
  }

  bool VisitStringLiteral(clang::StringLiteral *SL) {
    TheStringLiterals.push_back(SL);
    return true;
  }

private:
  std::vector<clang::StringLiteral *> TheStringLiterals;

  std::string mutateStringLiteral(const std::string &stringLiteral) {
    if (stringLiteral.find("%s") != std::string::npos || stringLiteral.find("%d") != std::string::npos) {
        return "";
    }
    std::string randomString = generateRandomString();
    int position = randIndex(stringLiteral.size() + 1);
    std::string mutatedString = stringLiteral.substr(0, position) + randomString + stringLiteral.substr(position);

    return mutatedString;
  }

  std::string generateRandomString() {
    static const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int length = randIndex(10) + 1; // Generate random string length between 1 and 10
    std::string randomString;

    for (int i = 0; i < length; ++i) {
      randomString += alphanum[randIndex(sizeof(alphanum) - 1)];
    }

    return randomString;
  }
};

static RegisterMutator<AddStringLiteralMutation> M(
    "u2.AddStringLiteralMutation", "Modifies the content of a string literal by appending, prepending, or inserting a random string.");
