#ifndef YSMUT_MUTATOR_H
#define YSMUT_MUTATOR_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Lexer.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Sema/Sema.h>
#include <llvm/ADT/SmallPtrSet.h>

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string>

#include "json.hpp"
using json = nlohmann::ordered_json;

namespace ysmut {

class MutatorManager;

template <typename, typename = void>
struct has_getSourceRange : std::false_type {};
template <typename T>
struct has_getSourceRange<T,
    std::void_t<
        decltype(std::declval<T>().getSourceRange())>>
    : std::true_type {};

struct MutationRange {
  enum : unsigned { npos = UINT_MAX };
  std::string filename;
  unsigned beginOffset = 0;
  unsigned endOffset = npos; // [0, UINT_MAX]

  bool isValid() const { return true; }

  static MutationRange from_json(json j) {
    MutationRange range;
    range.beginOffset = j[0];
    range.endOffset = j[1];
    assert(range.beginOffset <= range.endOffset);
    return range;
  }

  json to_json() const {
    json j = json::object();
    j["filename"] = filename;
    j["location"] = json::array({beginOffset, endOffset});
    return j;
  }

  clang::SourceRange toClangSourceRange(
      clang::SourceManager &SM) const {
    auto fileEntry = SM.getFileManager().getFile(filename);
    if (!fileEntry) return clang::SourceRange();
    unsigned beginOffset = this->beginOffset;
    unsigned endOffset = this->endOffset;

    clang::FileID file = SM.translateFile(fileEntry.get());

    // Check if endOffset exceeds file size.
    unsigned fileSize = SM.getFileIDSize(file);
    if (beginOffset >= fileSize) {
      beginOffset = fileSize - 1;
    }
    if (endOffset >= fileSize) { endOffset = fileSize - 1; }

    clang::SourceLocation beginLoc =
        SM.getComposedLoc(file, beginOffset);
    clang::SourceLocation endLoc =
        SM.getComposedLoc(file, endOffset);

    return clang::SourceRange(beginLoc, endLoc);
  }
};

inline bool rangesIntersect(clang::SourceManager &SM,
    clang::SourceRange r1, clang::SourceRange r2) {
  unsigned r1Begin = SM.getFileOffset(r1.getBegin());
  unsigned r1End = SM.getFileOffset(r1.getEnd());
  unsigned r2Begin = SM.getFileOffset(r2.getBegin());
  unsigned r2End = SM.getFileOffset(r2.getEnd());
  return (r1Begin <= r2End) && (r2Begin <= r1End);
}

class Mutator {
  const char *name;
  const char *desc;
  MutatorManager *manager = nullptr;

public:
  Mutator(const char *name, const char *desc)
      : name(name), desc(desc) {}
  Mutator(const Mutator &) = delete;
  Mutator(Mutator &&) = default;
  Mutator &operator=(const Mutator &) = delete;
  Mutator &operator=(Mutator &&) = default;

  const char *getDescription() const { return desc; }

  clang::ASTContext &getASTContext() const;
  clang::SourceManager &getSourceManager() const {
    return getASTContext().getSourceManager();
  }
  clang::CompilerInstance &getCompilerInstance() const;
  MutationRange getMutationRange() const;

  clang::SourceRange getMutationSourceRange() const {
    auto &SM = getASTContext().getSourceManager();
    return getMutationRange().toClangSourceRange(SM);
  }

  void setManager(MutatorManager *manager) {
    this->manager = manager;
  }
  MutatorManager &getManager() const {
    assert(this->manager);
    return *(this->manager);
  }

  clang::Decl *getMostRecentTranslationUnitDecl(
      clang::FunctionDecl *FD);

  std::string formatAsDecl(clang::QualType ty,
      const std::string &placeholder,
      bool supressSpecifiers = false);
  std::string formatSourceLocation(
      clang::SourceLocation Loc) {
    if (Loc.isInvalid()) { return std::string("(X,X)"); }
    auto &SM = getSourceManager();
    Loc = SM.getExpansionLoc(Loc);
    unsigned Line = SM.getExpansionLineNumber(Loc);
    unsigned Column = SM.getExpansionColumnNumber(Loc);
    return "(" + std::to_string(Line) + "," +
           std::to_string(Column) + ")";
  }

  std::string formatSourceRange(clang::SourceRange Range) {
    std::string Start =
        formatSourceLocation(Range.getBegin());
    std::string End = formatSourceLocation(Range.getEnd());
    return Start + "-" + End;
  }

  clang::SourceRange getExpansionRange(
      clang::SourceRange range);
  clang::SourceLocation getLocForEndOfToken(
      clang::SourceLocation loc);
  clang::SourceLocation getNextTokenLoc(
      clang::SourceLocation loc);
  clang::SourceLocation findStringLocationFrom(
      clang::SourceLocation beginLoc,
      llvm::StringRef target);
  clang::SourceRange findBracesRange(
      clang::SourceLocation beginLoc, char beginBrace = '(',
      char endBrace = ')');
  clang::SourceLocation balancedSearchUntil(
      clang::SourceLocation beginLoc, std::set<char> utils);

  clang::SourceRange findDeclaratorRange(
      const clang::VarDecl *VD);
  const char *getSourceBuffer(clang::SourceLocation loc) {
    auto &SM = getASTContext().getSourceManager();
    auto &LO = getASTContext().getLangOpts();
    return SM.getCharacterData(loc);
  }

  template <class NodeType>
  std::string getSourceText(const NodeType *node) {
    clang::SourceRange range = node->getSourceRange();
    auto &SM = getASTContext().getSourceManager();
    auto &LO = getASTContext().getLangOpts();
    range = SM.getExpansionRange(range).getAsRange();
    const char *beg = SM.getCharacterData(range.getBegin());
    clang::SourceLocation E = range.getEnd();
    E = clang::Lexer::getLocForEndOfToken(E, 0, SM, LO);
    const char *end = SM.getCharacterData(E);
    return llvm::StringRef(beg, end - beg).str();
  }

  clang::Rewriter &getRewriter() const;

  template <class NodeType>
  bool isInMainFile(const NodeType *node) {
    auto &SM = getASTContext().getSourceManager();
    auto loc = node->getBeginLoc();
    return SM.isInMainFile(loc) && !loc.isMacroID();
  }

  template <class NodeType>
  bool isMutationSite(const NodeType *node) {
    auto &SM = getASTContext().getSourceManager();
    // auto sourceRange = getMutationSourceRange();
    // if (!getMutationRange().isValid()) return true;
    return SM.isInMainFile(node->getBeginLoc()) &&
           rangesIntersect(SM, getMutationSourceRange(),
               node->getSourceRange());
  }

  llvm::raw_ostream &outs();

  // for random
  int randIndex(int n);
  bool randBool();

  template <typename Container>
  auto &randElement(Container &container) {
    assert(!container.empty());
    return container[randIndex(container.size())];
  }

  std::string generateUniqueName(
      const std::string &baseName);

  // for semantic checking
  bool checkAssignment(clang::SourceLocation loc,
      clang::QualType lhsType, clang::QualType rhsType) {
    return getCompilerInstance()
               .getSema()
               .CheckAssignmentConstraints(
                   loc, lhsType, rhsType) ==
           clang::Sema::AssignConvertType::Compatible;
  }

  bool checkBinOp(clang::BinaryOperatorKind kind,
      clang::Expr *lhs, clang::Expr *rhs) {
    clang::ExprResult result =
        getCompilerInstance().getSema().CreateBuiltinBinOp(
            lhs->getBeginLoc(), kind, lhs, rhs);
    return !result.isInvalid();
  }

  bool checkUnaryOp(
      clang::UnaryOperatorKind kind, clang::Expr *sub) {
    clang::ExprResult result =
        getCompilerInstance()
            .getSema()
            .CreateBuiltinUnaryOp(
                sub->getBeginLoc(), kind, sub);
    return !result.isInvalid();
  }

  // return true if successfully mutate
  virtual bool mutate() = 0;

  bool addStringBeforeFunctionDecl(
      const clang::FunctionDecl *FD,
      const std::string &Str);
  bool addStringBeforeVarDecl(
      const clang::VarDecl *VD, const std::string &Str);
  bool addStringAfterVarDecl(
      const clang::VarDecl *VD, const std::string &Str);
  bool addStringAfterStmt(
      clang::Stmt *AfterStmt, const std::string &Str);
  bool replaceVarDeclarator(const clang::VarDecl *VD,
      clang::QualType ty); // eg. replace the `*a[10]` part
                           // of `int *a[10]`
  bool removeParmFromFuncDecl(const clang::ParmVarDecl *PV,
      unsigned int TotalNumOfFuncParams, int ParamPos);
  bool removeArgFromExpr(
      const clang::Expr *E, int ParamPos);

  virtual ~Mutator() = default;

private:
  int getOffsetUntil(const char *Buf, char Symbol);
  unsigned getSourceLocOffset(clang::SourceLocation loc);
  unsigned getNumArgsWrapper(const clang::Expr *E);
  const clang::Expr *getArgWrapper(const clang::Expr *E, int ParamPos);
};

} // namespace ysmut

#endif
