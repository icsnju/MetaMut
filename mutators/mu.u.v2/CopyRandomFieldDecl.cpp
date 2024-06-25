#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Sema/Sema.h>

#include "Mutator.h"
#include "MutatorManager.h"

using namespace clang;

class CopyRandomFieldDecl : public Mutator,
                            public clang::RecursiveASTVisitor<CopyRandomFieldDecl> {
public:
    using Mutator::Mutator;

    bool mutate() override {
        TraverseAST(getASTContext());

        if (FieldDecls.empty()) return false;

        FieldDecl *field = randElement(FieldDecls);

        std::string newFieldName = generateUniqueName(field->getNameAsString());

        std::string typeString;
        llvm::raw_string_ostream ss(typeString);
        PrintingPolicy policy = getCompilerInstance().getASTContext().getPrintingPolicy();
        field->getType().print(ss, policy);
        ss.flush();

        std::string newFieldDeclaration = "\n" + typeString + " " + newFieldName;

        if (field->hasInClassInitializer()) {
            newFieldDeclaration += " = " + getSourceText(field->getInClassInitializer()) + ";";
        } else {
            newFieldDeclaration += ";";
        }

        SourceLocation endLoc = field->getEndLoc();
        SourceLocation insertLoc = getRewriter().getSourceMgr().translateLineCol(
            getRewriter().getSourceMgr().getFileID(endLoc),
            getRewriter().getSourceMgr().getPresumedLineNumber(endLoc) + 1,
            1);
        getRewriter().InsertTextBefore(insertLoc, newFieldDeclaration);

        return true;
    }

    bool VisitFieldDecl(clang::FieldDecl *FD) {
        FieldDecls.push_back(FD);
        return true;
    }

private:
    std::vector<clang::FieldDecl *> FieldDecls;
};

static RegisterMutator<CopyRandomFieldDecl> M(
    "u2.CopyRandomFieldDecl", "Duplicates a random FieldDecl from a struct or class, assigning it a unique name.");
