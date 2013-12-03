#include "consumers.h"

#include <iostream>
#include "jsonxx.h"

using namespace std;
using namespace jsonxx;

string getFunctionName(FunctionDecl *decl) 
{
	return decl->getNameInfo().getName().getAsString();
}

void CallGraphConsumer::HandleTranslationUnit(ASTContext &Context)
{
	CallGraph callGraph;
	callGraph.addToCallGraph(Context.getTranslationUnitDecl());
	SourceManager &manager = Context.getSourceManager();
	Object all;
	Array edges;
	Array nodes;
	for (auto e : callGraph) {
		if (e.first != 0) {
			string source_file = 
				manager.getFilename(e.first->getSourceRange().getBegin()).data();
			if (source_file == "input.cc") {
				FunctionDecl *caller_decl = cast<FunctionDecl>(e.second->getDecl());
				string caller = getFunctionName(caller_decl);
				nodes << caller;
				//e.first->dump();
				/*
				for (auto i = e.second->begin(); i != e.second->end(); ++i) {
					i->dump();
				}
				*/
				Array callees;
				for (auto i : *e.second) {
					FunctionDecl *callee_decl = cast<FunctionDecl>(i->getDecl());
					callees << getFunctionName(callee_decl);
					edges << Object(caller, callees);
				}
			}
			//cout << i.first->getSourceRange() << endl;
		}
	}
	all << "nodes" << nodes;
	all << "edges" << edges;
	cout << all.json() << endl;
}

void CFGConsumer::HandleTranslationUnit(ASTContext &Context)
{
	//v.TraverseDecl(Context.getTranslationUnitDecl());
}
