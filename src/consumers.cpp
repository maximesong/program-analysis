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
			cout << manager.getFilename(e.first->getSourceRange().getBegin()).data() << endl;
			e.first->dump();

			//cout << i.first->getSourceRange() << endl;
		}
	}
}

void CFGConsumer::HandleTranslationUnit(ASTContext &Context)
{
	//v.TraverseDecl(Context.getTranslationUnitDecl());
}
