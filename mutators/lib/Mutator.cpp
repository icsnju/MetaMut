#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Lex/Lexer.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

unsigned Mutator::randIndex(unsigned n) {
  return getManager().randint(0, n - 1);
}

bool Mutator::randBool() { return getManager().randint(0, 1); }

clang::Rewriter &Mutator::getRewriter() const {
  return getManager().getRewriter();
}

clang::ASTContext &Mutator::getASTContext() const {
  return getManager().getASTContext();
}

clang::CompilerInstance &Mutator::getCompilerInstance() const {
  return getManager().getCompilerInstance();
}

MutationRange Mutator::getMutationRange() const {
  return getManager().getMutationRange();
}

class IdentifierCollector
    : public clang::RecursiveASTVisitor<IdentifierCollector> {
public:
  std::set<std::string> identifiers;
  IdentifierCollector() = default;

  explicit IdentifierCollector(std::set<std::string> &identifiers)
      : identifiers(identifiers) {}

  bool VisitNamedDecl(clang::NamedDecl *ND) {
    identifiers.insert(ND->getNameAsString());
    return true;
  }

  static std::set<std::string> collect(clang::ASTContext &Ctx) {
    IdentifierCollector collector;
    collector.TraverseAST(Ctx);
    return collector.identifiers;
  }
};

std::string Mutator::generateUniqueName(const std::string &baseName) {
  static std::set<std::string> allIdentifiers =
      IdentifierCollector::collect(getASTContext());
  clang::ASTContext &Context = getASTContext();

  int suffix = 1;
  std::string newName;
  while (true) {
    newName = baseName + "_" + std::to_string(suffix);
    if (allIdentifiers.find(newName) != allIdentifiers.end()) {
      ++suffix;
      continue;
    }
    allIdentifiers.insert(newName);
    break;
  }
  return newName;
}

clang::Decl *Mutator::getMostRecentTranslationUnitDecl(
    clang::FunctionDecl *FD) {
  for (clang::DeclContext *DC = FD; DC; DC = DC->getParent()) {
    if (DC->getParent() && DC->getParent()->isTranslationUnit()) {
      return clang::cast<clang::Decl>(DC);
    }
  }
  return nullptr;
}

clang::SourceLocation Mutator::findStringLocationFrom(
    clang::SourceLocation beginLoc, llvm::StringRef target) {
  // Get the SourceManager from the ASTContext
  clang::SourceManager &SM = getASTContext().getSourceManager();

  // Decompose the location to a file position
  std::pair<clang::FileID, unsigned> locInfo = SM.getDecomposedLoc(beginLoc);

  // Get the file buffer
  bool invalidTemp;
  llvm::StringRef fileBuffer = SM.getBufferData(locInfo.first, &invalidTemp);

  // Search for the string
  size_t foundPos = fileBuffer.find(target, locInfo.second);

  if (foundPos != llvm::StringRef::npos) {
    // String found. Create and return a new SourceLocation
    clang::SourceLocation foundLoc =
        SM.getLocForStartOfFile(locInfo.first).getLocWithOffset(foundPos);
    return foundLoc;
  } else {
    // String not found. Return an invalid SourceLocation
    return clang::SourceLocation();
  }
}

clang::SourceRange Mutator::findBracesRange(
    clang::SourceLocation scrhBeginLoc, char beginBrace, char endBrace) {
  // Get the SourceManager from the ASTContext
  clang::SourceManager &SM = getASTContext().getSourceManager();

  // Decompose the location to a file position
  std::pair<clang::FileID, unsigned> locInfo =
      SM.getDecomposedLoc(scrhBeginLoc);

  // Get the file buffer
  bool invalidTemp;
  llvm::StringRef fileBuffer = SM.getBufferData(locInfo.first, &invalidTemp);

  unsigned foundPos = locInfo.second, nBraces = 0;
  for (; foundPos < fileBuffer.size(); foundPos++)
    if (fileBuffer[foundPos] == beginBrace) break;

  auto beginLoc =
      SM.getLocForStartOfFile(locInfo.first).getLocWithOffset(foundPos);
  for (; foundPos < fileBuffer.size(); foundPos++) {
    if (fileBuffer[foundPos] == beginBrace)
      nBraces++;
    else if (fileBuffer[foundPos] == endBrace)
      nBraces--;
    if (nBraces == 0) break;
  }
  auto endLoc =
      SM.getLocForStartOfFile(locInfo.first).getLocWithOffset(foundPos);
  return clang::SourceRange(beginLoc, endLoc);
}

clang::SourceLocation Mutator::getLocForEndOfToken(clang::SourceLocation loc) {
  return clang::Lexer::getLocForEndOfToken(loc, 0,
      getASTContext().getSourceManager(), getASTContext().getLangOpts());
}

clang::SourceLocation Mutator::getStmtEndLocWithSemi(clang::Stmt *S) {
  auto &SM = getASTContext().getSourceManager();
  auto &LangOpts = getASTContext().getLangOpts();
  clang::SourceLocation EndLoc = S->getEndLoc();
  if (EndLoc.isInvalid()) { return clang::SourceLocation(); }
  const char *CharData = SM.getCharacterData(EndLoc, nullptr);
  // The latter case `Curr == ';'` may occur when this stmt is an empty statement consists of only semicolon
  if (*CharData == '}' || *CharData == ';') return EndLoc;

  // Move to the end of the token
  EndLoc = clang::Lexer::getLocForEndOfToken(EndLoc, 0, SM, LangOpts);

  // Ensure the location is valid
  if (EndLoc.isInvalid()) { return clang::SourceLocation(); }

  // Skip whitespace and comments to find the semicolon
  while (true) {
    // Get the character at the current location
    const char *CharData = SM.getCharacterData(EndLoc, nullptr);
    if (*CharData == ';') {
      return EndLoc; // Found the semicolon
    }

    // Handle end of buffer
    if (EndLoc == SM.getLocForEndOfFile(SM.getFileID(EndLoc))) {
      return clang::SourceLocation(); // Reached end of file without finding semicolon
    }

    // Check for whitespace or comments
    if (!isspace(*CharData)) {
      return clang::SourceLocation(); // Found something other than whitespace or
                               // semicolon
    }

    // Advance to the next character
    EndLoc = EndLoc.getLocWithOffset(1);
  }

  return clang::SourceLocation(); // Return invalid location if semicolon not found
}

clang::SourceRange Mutator::getExpansionRange(clang::SourceRange range) {
  auto &SM = getASTContext().getSourceManager();
  clang::SourceLocation begin = range.getBegin();
  while (begin.isMacroID()) begin = SM.getExpansionLoc(begin);
  clang::SourceLocation end = range.getEnd();
  while (end.isMacroID()) end = SM.getExpansionLoc(end);
  return clang::SourceRange(begin, end);
}

clang::SourceLocation Mutator::getDeclEndLoc(clang::Decl *decl) {
  auto loc = findStringLocationFrom(decl->getEndLoc(), ";");
  return loc.getLocWithOffset(1);
}

clang::SourceLocation Mutator::balancedSearchUntil(
    clang::SourceLocation beginLoc, std::set<char> untils) {
  clang::SourceManager &SM = getASTContext().getSourceManager();
  std::pair<clang::FileID, unsigned> locInfo = SM.getDecomposedLoc(beginLoc);
  bool invalidTemp;
  llvm::StringRef fileBuffer = SM.getBufferData(locInfo.first, &invalidTemp);

  std::vector<char> balances;
  unsigned foundPos = locInfo.second;
  llvm::outs() << "foundPos = " << foundPos << "\n";
  for (; foundPos < fileBuffer.size(); foundPos++) {
    llvm::outs() << "encounter ch = '" << fileBuffer[foundPos] << "'\n";
    switch (fileBuffer[foundPos]) {
    case '(': balances.push_back('('); break;
    case ')':
      if (balances.empty() || balances.back() != '(') {
        // Handle the error: Unbalanced parentheses
      } else {
        balances.pop_back();
      }
      break;
    case '[': balances.push_back('['); break;
    case '{': balances.push_back('{'); break;
    case '}':
      if (balances.empty() || balances.back() != '{') {
        // Handle the error: Unbalanced braces
      } else {
        balances.pop_back();
      }
      break;
    default:
      if (balances.empty() && untils.find(fileBuffer[foundPos]) != untils.end())
        return SM.getLocForStartOfFile(locInfo.first)
            .getLocWithOffset(foundPos);
    }
  }
  llvm::outs() << "foundPos = " << foundPos << "\n";
  return SM.getLocForStartOfFile(locInfo.first).getLocWithOffset(foundPos);
}

clang::SourceRange Mutator::findDeclaratorRange(const clang::VarDecl *VD) {
  clang::SourceManager &SM = getSourceManager();
  std::pair<clang::FileID, unsigned> begLocInfo =
      SM.getDecomposedLoc(VD->getBeginLoc());
  std::pair<clang::FileID, unsigned> endLocInfo =
      SM.getDecomposedLoc(VD->getEndLoc());
  unsigned begOff = endLocInfo.second, endOff = endLocInfo.second;

  if (!VD->getTypeSourceInfo()) {
    // case: does not have type source info
    auto loc = VD->getLocation();
    return clang::SourceRange(loc, loc);
  }

  clang::TypeLoc nextTypeLoc = VD->getTypeSourceInfo()->getTypeLoc();
  if (nextTypeLoc && !nextTypeLoc.getNextTypeLoc()) {
    // case: has only specifier, doesn't has declarator
    auto loc = VD->getLocation();
    return clang::SourceRange(loc, loc);
  }

  // case: has declarator
  while (nextTypeLoc) {
    clang::SourceRange range = nextTypeLoc.getLocalSourceRange();
    begLocInfo = SM.getDecomposedLoc(range.getBegin());
    endLocInfo = SM.getDecomposedLoc(range.getEnd());
    if (begLocInfo.second < begOff) begOff = begLocInfo.second;
    if (endLocInfo.second > endOff) endOff = endLocInfo.second;
    nextTypeLoc = nextTypeLoc.getNextTypeLoc();
    if (nextTypeLoc && !nextTypeLoc.getNextTypeLoc()) break;
  }

  clang::SourceLocation begLoc =
      SM.getLocForStartOfFile(begLocInfo.first).getLocWithOffset(begOff);
  clang::SourceLocation endLoc =
      SM.getLocForStartOfFile(endLocInfo.first).getLocWithOffset(endOff);
  // llvm::outs() << "return range: " <<
  // formatSourceRange(clang::SourceRange(begLoc, endLoc))
  // << "\n";
  return clang::SourceRange(begLoc, endLoc);
}

std::string Mutator::formatAsDecl(clang::QualType ty,
    const std::string &placeholder, bool supressSpecifiers) {
  std::string declStr;
  llvm::raw_string_ostream ostream(declStr);
  auto policy = getASTContext().getPrintingPolicy();
  policy.SuppressSpecifiers = supressSpecifiers;
  ty.print(ostream, policy, placeholder);
  ostream.flush();
  return ostream.str();
}

unsigned Mutator::getSourceLocOffset(clang::SourceLocation loc) {
  clang::SourceManager &SM = getASTContext().getSourceManager();
  std::pair<clang::FileID, unsigned> locInfo = SM.getDecomposedLoc(loc);
  return locInfo.second;
}

clang::SourceLocation Mutator::getNextTokenLoc(clang::SourceLocation Loc) {
  clang::Token Tok;
  clang::SourceManager &SM = getSourceManager();
  clang::LangOptions LangOpts = getASTContext().getLangOpts();

  clang::SourceLocation LocEnd =
      clang::Lexer::getLocForEndOfToken(Loc, 0, SM, LangOpts);
  bool Invalid = false;
  llvm::StringRef File = SM.getBufferData(SM.getFileID(Loc), &Invalid).str();

  if (Invalid) { return clang::SourceLocation{}; }

  clang::Lexer Lex(LocEnd, LangOpts, File.begin(),
      getSourceManager().getCharacterData(LocEnd.getLocWithOffset(1)),
      File.end());
  Lex.LexFromRawLexer(Tok);
  return Tok.getLocation();
}

bool Mutator::addStringAfterStmt(
    clang::Stmt *AfterStmt, const std::string &Str) {
  std::string NewStr = ";\n" + Str + ";";
  return !getRewriter().InsertText(
      getLocForEndOfToken(AfterStmt->getEndLoc()), NewStr);
}

bool Mutator::replaceVarDeclarator(
    const clang::VarDecl *VD, clang::QualType ty) {
  clang::SourceRange range = findDeclaratorRange(VD);
  std::string NewStr = formatAsDecl(ty, VD->getNameAsString());
  return !getRewriter().ReplaceText(range, NewStr);
}

bool Mutator::addStringBeforeFunctionDecl(
    const clang::FunctionDecl *FD, const std::string &Str) {
  clang::SourceRange FuncRange;
  if (clang::FunctionTemplateDecl *TmplD = FD->getDescribedFunctionTemplate()) {
    FuncRange = TmplD->getSourceRange();
  } else {
    FuncRange = FD->getSourceRange();
  }
  clang::SourceLocation StartLoc = FuncRange.getBegin();
  return !getRewriter().InsertTextBefore(StartLoc, Str);
}

bool Mutator::addStringBeforeVarDecl(
    const clang::VarDecl *VD, const std::string &Str) {
  clang::SourceRange range = findDeclaratorRange(VD);
  return !getRewriter().InsertText(range.getBegin(), Str);
}

bool Mutator::addStringAfterVarDecl(
    const clang::VarDecl *VD, const std::string &Str) {
  clang::SourceRange range = findDeclaratorRange(VD);
  return !getRewriter().InsertTextAfter(
      getLocForEndOfToken(range.getEnd()), Str);
}

bool Mutator::removeParmFromFuncDecl(const clang::ParmVarDecl *PV,
    unsigned int TotalNumOfFuncParams, int ParamPos) {
  SourceRange ParamLocRange = PV->getSourceRange();
  int RangeSize;

  SourceLocation StartLoc = ParamLocRange.getBegin();
  if (StartLoc.isMacroID())
    StartLoc = getSourceManager().getExpansionLoc(StartLoc);
  SourceLocation EndLoc = ParamLocRange.getEnd();
  if (EndLoc.isMacroID()) EndLoc = getSourceManager().getExpansionLoc(EndLoc);
  if (StartLoc.isInvalid() && EndLoc.isInvalid()) {
    return false;
  } else if (StartLoc.isInvalid()) {
    StartLoc = EndLoc;
    RangeSize = PV->getNameAsString().size();
  } else if (EndLoc.isInvalid()) {
    const char *Buf = getSourceManager().getCharacterData(StartLoc);
    if ((ParamPos == 0) && (TotalNumOfFuncParams == 1)) {
      RangeSize = getOffsetUntil(Buf, ')');
    } else {
      RangeSize = getOffsetUntil(Buf, ',');
    }
  } else {
    RangeSize = getRewriter().getRangeSize(SourceRange(StartLoc, EndLoc));
    if (RangeSize == -1) return false;
  }

  // The param is the only parameter of the function
  // declaration. Replace it with void
  if ((ParamPos == 0) && (TotalNumOfFuncParams == 1)) {
    return !(getRewriter().ReplaceText(StartLoc, RangeSize, "void"));
  }

  // The param is the last parameter
  if (ParamPos == static_cast<int>(TotalNumOfFuncParams - 1)) {
    int Offset = 0;
    const char *StartBuf = getSourceManager().getCharacterData(StartLoc);

    assert(StartBuf && "Invalid start buffer!");
    while (*StartBuf != ',') {
      StartBuf--;
      Offset--;
    }

    SourceLocation NewStartLoc = StartLoc.getLocWithOffset(Offset);

    return !(getRewriter().RemoveText(NewStartLoc, RangeSize - Offset));
  }

  // We cannot use the code below:
  //   SourceLocation EndLoc = ParamLocRange.getEnd();
  //   const char *EndBuf =
  //     ConsumerInstance->getSourceManager().getCharacterData(EndLoc);
  // Because getEnd() returns the start of the last token if
  // this is a token range. For example, in the above
  // example, getEnd() points to the start of "x" See the
  // comments on getRangeSize in
  // clang/lib/Rewriter/Rewriter.cpp
  int NewRangeSize = 0;
  const char *StartBuf = getSourceManager().getCharacterData(StartLoc);

  while (NewRangeSize < RangeSize) {
    StartBuf++;
    NewRangeSize++;
  }

  assert(StartBuf && "Invalid start buffer!");
  // FIXME: This isn't really correct for processing
  // old-style function declarations, but just let's live
  // with it for now.
  while (*StartBuf != ',' && *StartBuf != ';') {
    StartBuf++;
    NewRangeSize++;
  }

  return !(getRewriter().RemoveText(StartLoc, NewRangeSize + 1));
}

bool Mutator::removeArgFromExpr(const clang::Expr *E, int ParamPos) {

  if (ParamPos >= static_cast<int>(getNumArgsWrapper(E))) return true;

  const Expr *Arg = getArgWrapper(E, ParamPos);
  assert(Arg && "Null arg!");
  if (dyn_cast<CXXDefaultArgExpr>(Arg->IgnoreParenCasts())) return true;

  SourceRange ArgRange = Arg->getSourceRange();
  int RangeSize = getRewriter().getRangeSize(ArgRange);

  if (RangeSize == -1) return false;

  SourceLocation StartLoc = ArgRange.getBegin();
  unsigned int NumArgs = getNumArgsWrapper(E);

  if ((ParamPos == 0) &&
      ((NumArgs == 1) ||
          ((NumArgs > 1) && dyn_cast<CXXDefaultArgExpr>(
                                getArgWrapper(E, 1)->IgnoreParenCasts())))) {
    // Note that ')' is included in ParamLocRange
    return !(getRewriter().RemoveText(ArgRange));
  }

  int LastArgPos = static_cast<int>(NumArgs - 1);
  // The param is the last non-default parameter
  if ((ParamPos == LastArgPos) ||
      ((ParamPos < LastArgPos) &&
          dyn_cast<CXXDefaultArgExpr>(
              getArgWrapper(E, ParamPos + 1)->IgnoreParenCasts()))) {
    int Offset = 0;
    const char *StartBuf = getSourceManager().getCharacterData(StartLoc);

    assert(StartBuf && "Invalid start buffer!");
    while (*StartBuf != ',') {
      StartBuf--;
      Offset--;
    }

    SourceLocation NewStartLoc = StartLoc.getLocWithOffset(Offset);
    return !(getRewriter().RemoveText(NewStartLoc, RangeSize - Offset));
  }

  // We cannot use
  // getSourceManager().getCharacterData(StartLoc) to get
  // the buffer, because it returns the unmodified string.
  // I've tried to use getEndlocationUntil(ArgRange,
  // ",", ...) call, but still failed. Seems in some cases,
  // it returns bad results for a complex case like:
  //  foo(...foo(...), ...)
  // So I ended up with this ugly way - get the end loc from
  // the next arg.
  const Expr *NextArg = getArgWrapper(E, ParamPos + 1);
  SourceRange NextArgRange = NextArg->getSourceRange();
  SourceLocation NextStartLoc = NextArgRange.getBegin();
  const char *NextStartBuf = getSourceManager().getCharacterData(NextStartLoc);
  int Offset = 0;
  while (*NextStartBuf != ',') {
    NextStartBuf--;
    Offset--;
  }

  SourceLocation NewEndLoc = NextStartLoc.getLocWithOffset(Offset);
  return !getRewriter().RemoveText(SourceRange(StartLoc, NewEndLoc));
}

int Mutator::getOffsetUntil(const char *Buf, char Symbol) {
  int Offset = 0;
  while (*Buf != Symbol) {
    Buf++;
    if (*Buf == '\0') break;
    Offset++;
  }
  return Offset;
}

unsigned Mutator::getNumArgsWrapper(const Expr *E) {
  const CXXConstructExpr *CtorE = dyn_cast<CXXConstructExpr>(E);
  if (CtorE) return CtorE->getNumArgs();

  const CallExpr *CE = dyn_cast<CallExpr>(E);
  if (CE) return CE->getNumArgs();

  assert(0 && "Invalid Expr!");
  return 0;
}

const Expr *Mutator::getArgWrapper(const Expr *E, int ParamPos) {
  const CXXConstructExpr *CtorE = dyn_cast<CXXConstructExpr>(E);
  if (CtorE) return CtorE->getArg(ParamPos);

  const CallExpr *CE = dyn_cast<CallExpr>(E);
  if (CE) return CE->getArg(ParamPos);

  assert(0 && "Invalid Expr!");
  return NULL;
}
