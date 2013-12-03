#include "consumers.h"

#include <iostream>
#include "jsonxx.h"

using namespace std;
using namespace jsonxx;

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
				string caller = to_string((long) e.second);
				nodes << caller;
				//e.first->dump();
				/*
				for (auto i = e.second->begin(); i != e.second->end(); ++i) {
					i->dump();
				}
				*/
				Array callees;
				for (auto i : *e.second) {
					callees << to_string((long) i);
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
