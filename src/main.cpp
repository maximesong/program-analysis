#include <iostream>
#include <vector>

#include <stdint.h>
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Analysis/CallGraph.h"
#include "clang/Analysis/CFG.h"
#include "clang/Analysis/AnalysisContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/LangOptions.h"

using namespace std;
using namespace clang::tooling;
using namespace clang;
using namespace llvm;

class FindNamedClassVisitor
: public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
	explicit FindNamedClassVisitor() {}

	bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
		cout << "C Plus Plus Class"<< endl;
		return true;
	}

	bool VisitFunctionDecl(FunctionDecl *D) {
		AnalysisDeclContextManager *m = new  AnalysisDeclContextManager();
		AnalysisDeclContext context(m, D);
		CFG *cfg = context.getCFG();
		LangOptions ops;
		cfg->dump(ops, true);
		return true;
	}

private:
};

/*
class FindNamedClassVisitor : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
	bool visitDecl(Decl *decl) {
		cout << "decl" << endl;
		return true;
	}
};
*/

class FindNamedClassConsumer : public ASTConsumer {
public:
	explicit FindNamedClassConsumer(ASTContext *Context){}

	virtual void Initialize(ASTContext &Context) {
		cout << "init" << endl;
	}

	virtual void HandleTranslationUnit(ASTContext &Context) {
		CallGraph callGraph;
		callGraph.addToCallGraph(Context.getTranslationUnitDecl());
		callGraph.dump();
		FindNamedClassVisitor v;
		v.TraverseDecl(Context.getTranslationUnitDecl());
		/*
		AnalysisDeclContextManager *m = new  AnalysisDeclContextManager();
		AnalysisDeclContext context(m, Context.getTranslationUnitDecl());
		CFG *cfg = context.getCFG();
		LangOptions ops;
		cfg->dump(ops, true);
		*/
	}
};

class AnalysisAction : public ASTFrontendAction {
public:
	virtual ASTConsumer *CreateASTConsumer(
			CompilerInstance &Compiler, StringRef InFile) {
		cout << "create" << endl;
		return new FindNamedClassConsumer(&Compiler.getASTContext());
	}
};

int main(int argc, const char **argv) {
	cout << "Hello" << endl;
	CommonOptionsParser OptionsParser(argc, argv);
	CallGraph call;
	ClangTool Tool(OptionsParser.getCompilations(),
			OptionsParser.getSourcePathList());
	Tool.run(newFrontendActionFactory<AnalysisAction>());
	/*
	runToolOnCode(new AnalysisAction, argv[1]);
	*/
	return 0;
}
