#ifndef CONSUMERS_H
#define CONSUMERS_H

#include <vector>

#include "clang/Analysis/CallGraph.h"
#include "clang/Analysis/CFG.h"
#include "clang/Analysis/AnalysisContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"

#include "jsonxx.h"

using namespace clang;
using namespace std;
using namespace jsonxx;

class FunctionInfo {
public:
	string name;
	string return_type;
	vector<string> parameter_types;
	string qualified_name;
	string this_type;
	string id();
	Object to_json();
};

class CallGraphConsumer : public ASTConsumer {
public:
	virtual void HandleTranslationUnit(ASTContext &context);
};

namespace {

class CFGVisitor : public RecursiveASTVisitor<CFGVisitor> {
public:
	virtual bool VisitFunctionDecl(FunctionDecl *D) {
		if (!D->hasBody()) {
			return false;
		}
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
