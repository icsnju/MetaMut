#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>
#include <iomanip>
#include <sstream>
#include <string>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

std::string escape_string(const std::string &unescaped_string) {
  std::stringstream ss;

  for (char ch : unescaped_string) {
    switch (ch) {
    case '\n':
      ss << "\\n";
      break;
    case '\v':
      ss << "\\v";
      break;
    case '\f':
      ss << "\\f";
      break;
    case '\b':
      ss << "\\b";
      break;
    default:
      if (std::isprint(ch)) {
        ss << ch;
      } else {
        ss << "\\x" << std::setw(2) << std::setfill('0') << std::hex
           << static_cast<int>(ch);
      }
      break;
    }
  }

  return ss.str();
}

class CorrectNoreturn : public Mutator,
                        public clang::RecursiveASTVisitor<CorrectNoreturn> {
public:
  using Mutator::Mutator;

  bool VisitFunctionDecl(clang::FunctionDecl *FD) {
    /* find unreturned return function */
    if (FD->getBody() && !FD->getReturnType()->isVoidType() &&
        FD->getDeclName().getAsString() != "main") {
      Stmt *body = FD->getBody();
      if (body && isa<clang::CompoundStmt>(body)) {
        Stmt *LastStmt = clang::dyn_cast<CompoundStmt>(body)->body_back();
        if (!LastStmt || !isa<ReturnStmt>(LastStmt)) {
          TheFuncs.insert(FD);
        }
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheFuncs.empty())
      return false;

    for (FunctionDecl *FD : TheFuncs) {
      // Format the return statement
      std::string ReturnStmt;
      if (FD->getReturnType()->isIntegerType() ||
          FD->getReturnType()->isFloatingType() ||
          FD->getReturnType()->isPointerType())
        ReturnStmt = "return 0;";
      else
        ReturnStmt = "return {0};";

      // Insert the return statement at the end of the
      // function body
      SourceLocation InsertLoc = FD->getBody()->getEndLoc();
      getRewriter().InsertTextAfter(InsertLoc, "\n" + ReturnStmt);
    }

    return true;
  }

  bool hasMutationSite() const { return TheFuncs.size(); }

private:
  std::set<clang::FunctionDecl *> TheFuncs;
};

class CorrectNoinit : public Mutator,
                      public clang::RecursiveASTVisitor<CorrectNoinit> {
  using VisitorTy = clang::RecursiveASTVisitor<CorrectNoinit>;

public:
  using Mutator::Mutator;

  bool hasAssign(clang::VarDecl *VD) {
    auto &SM = getASTContext().getSourceManager();
    clang::SourceLocation bloc = VD->getBeginLoc();
    const char *beg = SM.getCharacterData(bloc);
    const char *end =
        SM.getCharacterData(SM.getLocForEndOfFile(SM.getFileID(bloc)));
    for (const char *p = beg; p < end; ++p)
      if (std::isalnum(*p) || std::isspace(*p) || *p == '_' || *p == '.')
        continue;
      else if (*p == '=')
        return true;
      else
        break;
    return false;
  }

  bool TraverseFunctionDecl(clang::FunctionDecl *FD) {
    VisitorTy::TraverseStmt(FD->getBody());
    return true;
  }

  bool VisitVarDecl(clang::VarDecl *VD) {
    if (!VD->hasInit() && !hasAssign(VD) &&
        !VD->getDeclContext()->isTranslationUnit() &&
        VD->getStorageClass() != clang::SC_Extern &&
        !clang::isa<ParmVarDecl>(VD) && !VD->getType()->isVariableArrayType()) {
      TheVars.insert(VD);
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheVars.empty())
      return false;

    for (VarDecl *VD : TheVars) {
      std::string InitValue = "0";
      if (!VD->getType()->isIntegerType() && !VD->getType()->isFloatingType() &&
          !VD->getType()->isPointerType())
        InitValue = "{0}";

      SourceLocation EndLoc = getLocForEndOfToken(VD->getEndLoc());
      SourceLocation fileEndLoc = getSourceManager().getLocForEndOfFile(
          getSourceManager().getFileID(EndLoc));
      CharSourceRange Range =
          clang::CharSourceRange::getCharRange(EndLoc, fileEndLoc);

      bool InvalidTemp = false;
      llvm::StringRef Text = clang::Lexer::getSourceText(
          Range, getSourceManager(), getASTContext().getLangOpts(),
          &InvalidTemp);
      if (Text.trim().startswith("__attribute__")) {
        int i = 0, depth = 0;
        for (; i < Text.size(); i++) {
          if (Text[i] == '(')
            depth++;
          else if (Text[i] == ')') {
            depth--;
            if (depth == 0)
              break;
          }
        }
        EndLoc = EndLoc.getLocWithOffset(i + 1);
      }

      getRewriter().InsertTextAfter(EndLoc, " = " + InitValue);
    }

    return true;
  }

  bool hasMutationSite() const { return TheVars.size(); }

private:
  std::set<clang::VarDecl *> TheVars;
};

class CorrectMain : public Mutator,
                    public clang::RecursiveASTVisitor<CorrectMain> {
  using VisitorTy = clang::RecursiveASTVisitor<CorrectMain>;

public:
  using Mutator::Mutator;

  bool TraverseFunctionDecl(clang::FunctionDecl *FD) {
    /* find unreturned return function */
    if (FD->getBody() && FD->getDeclName().getAsString() == "main") {
      CurrentMainFD = FD;
      TheMainReturns[FD] = std::vector<ReturnStmt *>{}; // insert
      VisitorTy::TraverseFunctionDecl(FD);
      CurrentMainFD = nullptr;
    }
    return true;
  }

  bool VisitReturnStmt(clang::ReturnStmt *Ret) {
    if (CurrentMainFD)
      TheMainReturns[CurrentMainFD].push_back(Ret);
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheMainReturns.empty())
      return false;
    for (auto &FuncAndRets : TheMainReturns) {
      auto *FD = FuncAndRets.first;
      if (!FD->getReturnType()->isIntegerType()) {
        SourceRange ReturnTypeRange = FD->getReturnTypeSourceRange();
        getRewriter().ReplaceText(ReturnTypeRange, "int ");
      }

      for (ReturnStmt *Ret : FuncAndRets.second) {
        if (!Ret->getRetValue()) {
          getRewriter().ReplaceText(Ret->getSourceRange(), "return 0;");
        } else if (!Ret->getRetValue()->getType()->isIntegerType()) {
          getRewriter().ReplaceText(
              Ret->getRetValue()->getSourceRange(),
              "(int) (" + getSourceText(Ret->getRetValue()) + ")");
        }
      }

      Stmt *body = FD->getBody();
      if (body && clang::isa<CompoundStmt>(body)) {
        Stmt *LastStmt = clang::dyn_cast<CompoundStmt>(body)->body_back();
        if (!LastStmt || !isa<ReturnStmt>(LastStmt)) {
          SourceLocation InsertLoc = FD->getBody()->getEndLoc();
          getRewriter().InsertTextAfter(InsertLoc, "\nreturn 0;");
        }
      }
    }

    return true;
  }

  bool hasMutationSite() const { return TheMainReturns.size(); }

private:
  clang::FunctionDecl *CurrentMainFD;
  std::map<clang::FunctionDecl *, std::vector<ReturnStmt *>> TheMainReturns;
};

class CorrectBuiltins : public Mutator,
                        public clang::RecursiveASTVisitor<CorrectBuiltins> {
public:
  using Mutator::Mutator;

  bool VisitDeclRefExpr(DeclRefExpr *DRE) {
    if (DRE->getDecl()->isFunctionOrFunctionTemplate()) {
      if (FunctionDecl *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
        if (FD->getBuiltinID()) {
          std::string functionName = FD->getNameInfo().getName().getAsString();
          if (functionName == "__builtin_object_size" ||
              functionName == "__builtin_constant_p")
            TheDREs.push_back(DRE);
        }
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (TheDREs.empty())
      return false;
    for (DeclRefExpr *DRE : TheDREs) {
      getRewriter().ReplaceText(DRE->getSourceRange(), "sizeof");
    }
    return true;
  }

private:
  std::vector<DeclRefExpr *> TheDREs;
};

class CorrectPointerFormatter
    : public Mutator,
      public clang::RecursiveASTVisitor<CorrectPointerFormatter> {
public:
  using Mutator::Mutator;

  bool VisitCallExpr(CallExpr *CE) {
    if (FunctionDecl *FD = CE->getDirectCallee()) {
      StringRef FuncName = FD->getNameInfo().getAsString();
      if (FuncName == "printf" || FuncName == "sprintf" ||
          FuncName == "vsprintf" || FuncName == "vsnprintf" ||
          FuncName == "snprintf" || FuncName == "__builtin_printf" ||
          FuncName == "__builtin_sprintf" || FuncName == "__builtin_vsprintf" ||
          FuncName == "__builtin_vsnprintf" ||
          FuncName == "__builtin_snprintf") {
        FormatStringCalls.push_back(CE);
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    if (FormatStringCalls.empty())
      return false;

    for (CallExpr *CE : FormatStringCalls) {
      StringLiteral *FormatLiteral = nullptr;
      unsigned FormatArgIndex = 0;

      for (const auto &Arg : CE->arguments()) {
        if ((FormatLiteral =
                 dyn_cast<StringLiteral>(Arg->IgnoreParenCasts()))) {
          break;
        }
        ++FormatArgIndex;
      }

      if (!FormatLiteral)
        return false;

      StringRef FormatString = FormatLiteral->getString();
      if (!FormatString.contains("%p"))
        return false;

      std::string NewFormatString = std::string(FormatString);
      size_t pos = 0;
      while ((pos = NewFormatString.find("%p", pos)) != std::string::npos) {
        NewFormatString.replace(pos, 2, "%d");
        pos += 2;
      }

      SourceLocation BeginLoc = FormatLiteral->getBeginLoc();
      SourceLocation EndLoc = FormatLiteral->getEndLoc();
      getRewriter().ReplaceText(SourceRange(BeginLoc, EndLoc),
                                "\"" + escape_string(NewFormatString) + "\"");

      unsigned ArgIndex = FormatArgIndex + 1;
      for (unsigned i = ArgIndex; i < CE->getNumArgs(); ++i) {
        const Expr *Arg = CE->getArg(i);
        if (Arg->getType()->isPointerType()) {
          std::string CastExpr = "(int)!!(" + getSourceText(Arg) + ")";
          getRewriter().ReplaceText(Arg->getSourceRange(), CastExpr);
        }
      }
    }

    return true;
  }

private:
  std::vector<CallExpr *> FormatStringCalls;
};

class CorrectForLoopInc : public Mutator,
                          public clang::RecursiveASTVisitor<CorrectForLoopInc> {
  bool mutated = false;

public:
  using Mutator::Mutator;

  bool VisitForStmt(ForStmt *FS) { return true; }

  bool mutate() override {
    TraverseAST(getASTContext());
    return mutated;
  }
};

class CForgeInjector : public Mutator,
                       public clang::RecursiveASTVisitor<CForgeInjector> {

  std::vector<VarDecl *> gblVars;
  std::vector<VarDecl *> argVars;
  bool currFuncIsMain = false;
  FunctionDecl *currFuncDecl = nullptr;
  using VisitorTy = clang::RecursiveASTVisitor<CForgeInjector>;

  std::vector<std::pair<std::string, std::string>> replacements = {
      {"__builtin_abort", "CFORGE_ABORT"},
      {"abort", "CFORGE_ABORT"},
      {"assert", "CFORGE_ASSERT"},
      {"__DATE__", "CFORGE_DATE"},
      {"__TIME__", "CFORGE_TIME"},
      {"__builtin_printf", "CFORGE_IGNORE"},
      {"__builtin_fprintf", "CFORGE_IGNORE"},
      {"fprintf", "CFORGE_IGNORE"},
      {"printf", "CFORGE_IGNORE"},
      {"sleep", "CFORGE_IGNORE"},
      {"usleep", "CFORGE_IGNORE"},
      {"scanf", "CFORGE_IGNORE"},
      {"pthread_create", "CFORGE_PTHREAD_CREATE"},
      {"pthread_join", "CFORGE_PTHREAD_JOIN"},
      {"__builtin_scanf", "CFORGE_IGNORE"},
  };

public:
  using Mutator::Mutator;

  llvm::raw_ostream &indentSpaces(int indent, llvm::raw_ostream &os) {
    for (int i = 0; i < indent; i++)
      os << "  ";
    return os;
  }

  bool TraverseSizeOfPackExpr(SizeOfPackExpr *E) { return true; }
  bool TraverseFieldDecl(FieldDecl *FD) { return true; }
  bool TraverseRecordDecl(RecordDecl *FD) { return true; }
  bool TraverseStmtExpr(StmtExpr *SE) { return true; }

  bool isInvokableType(clang::QualType T) {
    // Remove any qualifiers like const, volatile, etc.
    T = T.getCanonicalType();

    // Check if the type is a function type or a pointer to a function type
    if (T->isFunctionPointerType() || T->isFunctionType() ||
        T->isFunctionProtoType()) {
      return true;
    }

    // Check if the type is a record type (class, struct, or union) and has
    // operator()
    if (const clang::RecordType *RT = T->getAs<clang::RecordType>()) {
      clang::RecordDecl *RD = RT->getDecl();
      if (RD) {
        for (auto D : RD->decls()) {
          if (clang::CXXMethodDecl *MD =
                  clang::dyn_cast<clang::CXXMethodDecl>(D)) {
            if (MD->isOverloadedOperator() &&
                MD->getOverloadedOperator() == clang::OO_Call) {
              // Found an operator() - this is a callable object
              return true;
            }
          }
        }
      }
    }

    // Check if the type is a reference to an invokable type
    if (T->isReferenceType()) {
      return isInvokableType(T->getPointeeType());
    }

    // Check if the type is a class template specialization with operator()
#if 0
    if (const clang::ClassTemplateSpecializationDecl *Spec =
            clang::dyn_cast_or_null<clang::ClassTemplateSpecializationDecl>(
                RT->getDecl())) {
      const clang::TemplateArgumentList &TemplateArgs = Spec->getTemplateArgs();
      for (unsigned I = 0, E = TemplateArgs.size(); I != E; ++I) {
        if (TemplateArgs[I].getKind() == clang::TemplateArgument::Type) {
          if (isInvokableType(TemplateArgs[I].getAsType())) {
            return true;
          }
        }
      }
    }
#endif
    return false;
  }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    if (!UO || !currFuncDecl)
      return true;
    Expr *E = UO->getSubExpr();
    if (UO->getOpcode() == clang::UO_Deref) {
      if ((!isInvokableType(E->getType()))) {
        DeclRefExpr *ch =
            dyn_cast_or_null<DeclRefExpr>(E->IgnoreParenImpCasts());
        if (ch && ch->getDecl() &&
            ch->getDecl()->getType().getCanonicalType()->isArrayType()) {
          ; // do nothing
        } else {
          getRewriter().InsertTextAfter(E->getBeginLoc(),
                                        "CFORGE_SAFE_POINTER(");
          auto endLoc = clang::Lexer::getLocForEndOfToken(
              E->getEndLoc(), 0, getSourceManager(),
              getASTContext().getLangOpts());
          getRewriter().InsertTextBefore(endLoc, ")");
        }
      }
    }
    return true;
  }

  bool VisitMemberExpr(MemberExpr *ME) {
    if (!ME || !currFuncDecl)
      return true;
    Expr *E = ME->getBase();
    if (ME->isArrow()) {
      DeclRefExpr *ch = dyn_cast_or_null<DeclRefExpr>(E->IgnoreParenImpCasts());
      if (ch && ch->getDecl() &&
          ch->getDecl()->getType().getCanonicalType()->isArrayType()) {
        ; // do nothing
      } else {
        getRewriter().InsertText(E->getBeginLoc(), "CFORGE_SAFE_POINTER(");
        auto endLoc = clang::Lexer::getLocForEndOfToken(
            E->getEndLoc(), 0, getSourceManager(),
            getASTContext().getLangOpts());
        getRewriter().InsertText(endLoc, ")");
      }
    }
    return true;
  }

  bool TraverseFunctionDecl(FunctionDecl *FD) {
    if (!FD)
      return true;
    currFuncDecl = FD;
    argVars.clear();
    currFuncIsMain = FD->getDeclName().getAsString() == "main";
    for (auto param : FD->parameters())
      argVars.push_back(param);
    VisitorTy::TraverseFunctionDecl(FD);
    currFuncIsMain = false;
    argVars.clear();
    currFuncDecl = nullptr;
    return true;
  }

  bool VisitCompoundStmt(CompoundStmt *CS) {
    if (!CS)
      return true;

    bool alreadyMeetReturn = false;
    std::vector<VarDecl *> vars;
    for (Stmt *sub : CS->body()) {
      if (auto *DS = dyn_cast<DeclStmt>(sub)) {
        for (Decl *decl : DS->decls()) {
          if (auto *VD = dyn_cast<VarDecl>(decl)) {
            vars.push_back(VD);
          }
        }
      } else if (isa<ReturnStmt>(sub)) {
        alreadyMeetReturn = true;
        std::string osstr;
        llvm::raw_string_ostream oss(osstr);
        for (VarDecl *d : vars)
          insertObserver(d, d->getNameAsString(), d->getType(), oss, 0, 0);
        if (currFuncIsMain)
          for (VarDecl *d : gblVars)
            insertObserver(d, d->getNameAsString(), d->getType(), oss, 0, 0);
        else
          for (VarDecl *d : argVars)
            insertObserver(d, d->getNameAsString(), d->getType(), oss, 0, 0);
        getRewriter().InsertText(sub->getBeginLoc(), oss.str());
      }
    }
    if (alreadyMeetReturn)
      return true;

    std::string osstr;
    llvm::raw_string_ostream oss(osstr);
    if (currFuncIsMain)
      for (VarDecl *d : gblVars)
        insertObserver(d, d->getNameAsString(), d->getType(), oss, 0, 0);
    else
      for (VarDecl *d : argVars)
        insertObserver(d, d->getNameAsString(), d->getType(), oss, 0, 0);
    getRewriter().InsertTextBefore(CS->getEndLoc(), oss.str());
    return true;
  }

  bool insertObserver(VarDecl *VD, const std::string &var, QualType ty,
                      llvm::raw_ostream &os, int indent = 0,
                      int arrIterDepth = 0) {
    if (ty->isIntegralOrEnumerationType()) {
      indentSpaces(indent, os) << "CFORGE_OBSERVE(" << var << ");\n";
    } else if (ty->isFloatingType()) {
      indentSpaces(indent, os) << "CFORGE_OBSERVE((int64_t)" << var << ");\n";
    } else if (ty->isPointerType()) {
      indentSpaces(indent, os) << "CFORGE_OBSERVE(!!" << var << ");\n";
    } else if (ty->isRecordType()) {
      // Iterate through each field in the record type
      RecordDecl *RD = ty->getAsRecordDecl();
      for (auto field : RD->fields()) {
        std::string fieldName = field->getNameAsString();
        if (fieldName.size() > 0) {
          std::string fieldVar = var + "." + fieldName;
          insertObserver(VD, fieldVar, field->getType(), os, indent,
                         arrIterDepth);
        } else if (field->getType()->isRecordType()) {
          insertObserver(VD, var, field->getType(), os, indent, arrIterDepth);
        }
      }
    } else if (ty->isConstantArrayType() && isa<ConstantArrayType>(ty)) {
      const ConstantArrayType *arrayType = dyn_cast<ConstantArrayType>(ty);
      auto arraySize = getASTContext().getConstantArrayElementCount(arrayType);
      QualType eleTy = arrayType->getElementType();

      static const char *iterVars[] = {"i",  "j",  "k",  "ii", "ij",
                                       "ik", "ji", "jj", "jk"};
      std::string iterVar;
      if (arrIterDepth < sizeof(iterVars) / sizeof(*iterVars)) {
        iterVar = iterVars[arrIterDepth];
        if (iterVar == VD->getName())
          iterVar = std::string("i") + std::to_string(arrIterDepth);
      } else {
        iterVar = std::string("i") + std::to_string(arrIterDepth);
      }

      indentSpaces(indent, os)
          << "for (int " << iterVar << " = 0; " << iterVar << " < " << arraySize
          << "; ++" << iterVar << ") {\n";
      insertObserver(VD, var + "[" + iterVar + "]", eleTy, os, indent + 1,
                     arrIterDepth + 1);
      indentSpaces(indent, os) << "}\n";
    } else {
      // For types that are not handled, you might want to do something else
      os << "// Unhandled type for " << var << "\n";
    }
    return true;
  }

  bool insertLoopGuard(Stmt *body) {
    if (isa<CompoundStmt>(body)) {
      CompoundStmt *CS = dyn_cast<CompoundStmt>(body);
      getRewriter().InsertText(CS->getLBracLoc().getLocWithOffset(1),
                               "CFORGE_LOOP_GUARD();");
    } else {
      getRewriter().InsertTextBefore(body->getBeginLoc(),
                                     "{CFORGE_LOOP_GUARD();");
      auto stmtEndLoc = getStmtEndLocWithSemi(body);
      getRewriter().InsertTextAfter(getLocForEndOfToken(stmtEndLoc), "}");
    }
    return true;
  }

  bool VisitForStmt(ForStmt *FS) {
    insertLoopGuard(FS->getBody());
    return true;
  }

  bool VisitWhileStmt(WhileStmt *FS) {
    insertLoopGuard(FS->getBody());
    return true;
  }

  bool VisitDoStmt(DoStmt *FS) {
    insertLoopGuard(FS->getBody());
    return true;
  }

  bool VisitVarDecl(VarDecl *VD) {
    /* case 1 */
    if (VD && VD->hasGlobalStorage() && !VD->isStaticLocal() &&
        !getASTContext().getSourceManager().isInSystemHeader(
            VD->getLocation())) {
      gblVars.push_back(VD);
    }
    /* case 2 */
    StringRef VarName = VD->getName();
    static std::vector<StringRef> shadow_variables = {
        "true",
        "false",
        "bool",
    };
    for (auto &f : shadow_variables) {
      if (f == VarName) {
        replacements.emplace_back(f.str(), f.str() + "_rep");
      }
    }
    return true;
  }

  bool VisitEnumConstantDecl(EnumConstantDecl *ECD) {
    StringRef VarName = ECD->getName();
    static std::vector<StringRef> shadow_variables = {
        "true",
        "false",
        "bool",
    };
    for (auto &f : shadow_variables) {
      if (f == VarName) {
        replacements.emplace_back(f.str(), f.str() + "_rep");
      }
    }
    return true;
  }

  bool VisitTypedefDecl(TypedefDecl *TD) {
    StringRef VarName = TD->getName();
    static std::vector<StringRef> shadow_variables = {
        "bool",
    };
    for (auto &f : shadow_variables) {
      if (f == VarName) {
        replacements.emplace_back(f.str(), f.str() + "_rep");
      }
    }
    return true;
  }

  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (FD && !FD->hasBody()) {
      StringRef FuncName = FD->getNameInfo().getAsString();
      if (FuncName == "abort" || FuncName == "fprintf" ||
          FuncName == "printf" || FuncName == "sleep" || FuncName == "usleep" ||
          FuncName == "scanf")
        getRewriter().RemoveText(FD->getSourceRange());
    }
    if (FD && FD->hasBody()) {
      // these functions will be replaced if and only if they have a definition
      static std::vector<StringRef> shadow_functions = {
          "true",      "false",    "isfinite",  "isinf",  "isnan",
          "isnormal",  "signbit",  "isfinitef", "isinff", "isnanf",
          "isnormalf", "signbitf", "isfinitel", "isinfl", "isnanl",
          "isnormall", "signbitl",
      };
      for (auto &f : shadow_functions) {
        StringRef FuncName = FD->getNameInfo().getAsString();
        if (f == FuncName) {
          replacements.emplace_back(f.str(), f.str() + "_rep");
        }
      }
    }
    return true;
  }

  bool mutate() override {
    TraverseAST(getASTContext());
    return true;
  }

  bool post(llvm::raw_ostream &os) override {
    /* post process */
    std::string instr;
    {
      llvm::raw_string_ostream oss(instr);
      auto &SM = getASTContext().getSourceManager();
      auto ID = SM.getMainFileID();
      getRewriter().getEditBuffer(ID).write(oss);
      oss.flush();
    }

    auto is_A0_ = [](char ch) { return std::isalnum(ch) || ch == '_'; };
    size_t index = 0;
    while (index < instr.size()) {
      size_t index_b = index;
      if (is_A0_(instr[index]))
        while (index < instr.size() && is_A0_(instr[index]))
          index++;
      // instr[index_b : index]
      bool is_replaced = false;
      size_t tl = index - index_b;
      if (tl > 0) {
        // is a simple identifier/number token
        for (auto &kv : replacements) {
          if (!instr.compare(index_b, tl, kv.first)) {
            os << kv.second;
            is_replaced = true;
            break;
          }
        }
        if (!is_replaced)
          os.write(&instr[index_b], tl);
      } else if (instr[index] == '#') {
        // skip preprocessing directive
        while (index < instr.size() && instr[index] != '\n')
          os << instr[index++];
#if 0
      } else if (!instr.compare(index, 3, ";};")) {
        // replacements: `;}; while` ==> `;} while`
        int index_e = index + 3;
        while (index_e < instr.size() && std::isspace(instr[index_e]))
          index_e++;
        if (!instr.compare(index_e, 5, "while")) {
          os << ";}";
          index += 2;
        } else {
          os << ";};";
          index += 3;
        }
#endif
      } else {
        os << instr[index++];
      }
    }
    return true;
  }
};

class CorrectUndefinedBehavior
    : public Mutator,
      public clang::RecursiveASTVisitor<CorrectUndefinedBehavior> {
public:
  using Mutator::Mutator;

  CForgeInjector cforge_injector{"cforge-injector", "cforge-injector"};

  template <class T, class... Args> bool for_each_type_perform_mutation() {
    T mutator("<template T>", "<template ...>");
    bool mutated = false;
    mutator.setManager(&getManager());
    mutated = mutator.mutate() || mutated;

    if constexpr (sizeof...(Args) > 0)
      mutated = for_each_type_perform_mutation<Args...>() || mutated;
    return mutated;
  }

  bool mutate() override {
    bool mutated =
        for_each_type_perform_mutation<CorrectNoreturn, CorrectNoinit,
                                       CorrectMain, CorrectPointerFormatter,
                                       CorrectBuiltins>();
    cforge_injector.setManager(&getManager());
    mutated = cforge_injector.mutate() || mutated;
    return mutated;
  }

  bool post(llvm::raw_ostream &os) override { return cforge_injector.post(os); }
};

static RegisterMutator<CorrectUndefinedBehavior>
    X("CorrectUndefinedBehavior", "correct static undefined behavior");
