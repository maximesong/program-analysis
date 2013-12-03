#ifndef CONSUMERS_H
#define CONSUMERS_H

#include <vector>

#include "clang/Analysis/CallGraph.h"
#include "clang/Analysis/CFG.h"
#include "clang/Analysis/AnalysisContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"

using namespace clang;
using namespace std;

class CallGraphConsumer : public ASTConsumer {
public:
	virtual void HandleTranslationUnit(ASTContext &context);
};

namespace {

class CFGVisitor : public RecursiveASTVisitor<CFGVisitor> {
public:
	virtual bool VisitFunctionDecl(FunctionDecl *D) {
		AnalysisDeclContextManager *m = 
			new  AnalysisDeclContextManager();
		AnalysisDeclContext context(m, D);
		CFG *cfg = context.getCFG();
		LangOptions ops;
		cfg->dump(ops, true);
		return true;
	}
};

};

class CFGConsumer : public ASTConsumer {
public:
	virtual void HandleTranslationUnit(ASTContext &context);
private:
	CFGVisitor visitor;
};

#endif
