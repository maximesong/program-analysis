#include "consumers.h"

#include <iostream>

using namespace std;

void CallGraphConsumer::HandleTranslationUnit(ASTContext &Context)
{
	CallGraph callGraph;
	callGraph.addToCallGraph(Context.getTranslationUnitDecl());
	SourceManager &manager = Context.getSourceManager();
	for (auto e : callGraph) {
		if (e.first != 0) {
			string source_file = 
				manager.getFilename(e.first->getSourceRange().getBegin()).data();
			if (source_file == "input.cc") {
				//e.first->dump();
				cout << "S" << endl;
				e.second->dump();
				cout << " : " << endl;
				/*
				for (auto i = e.second->begin(); i != e.second->end(); ++i) {
					i->dump();
				}
				*/
				for (auto i : *e.second) {
					cout << "EE: " << endl;
					i->dump();
					cout << "\t" << endl;
				}
				cout << endl;
			}

			//cout << i.first->getSourceRange() << endl;
		}
	}
}

void CFGConsumer::HandleTranslationUnit(ASTContext &Context)
{
	//v.TraverseDecl(Context.getTranslationUnitDecl());
}
