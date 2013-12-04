#include "consumers.h"

#include <iostream>
#include "jsonxx.h"

using namespace std;
using namespace jsonxx;

string FunctionInfo::qualified_name()
{
	string qualified_name;
	qualified_name += name;
	qualified_name += "(";
	if (parameter_types.size() != 0) {
		qualified_name += parameter_types[0];
		for (int i = 1; i != parameter_types.size(); ++i) {
			qualified_name += ", " + parameter_types[i];
		}
	}
	qualified_name += ")";
	return qualified_name;
}

FunctionInfo getFunctionInfo(ASTContext &context, FunctionDecl *decl)
{
	FunctionInfo info;
	info.name = decl->getQualifiedNameAsString();
	info.return_type = decl->getResultType().getAsString();
	for (auto p = decl->param_begin(); p != decl->param_end(); ++p) {
		string type = (*p)->getOriginalType().getAsString();
		info.parameter_types.push_back(type);
	}
	if (isa<CXXMethodDecl>(decl)) {
		CXXMethodDecl *method = cast<CXXMethodDecl>(decl);
		//info.this_type = method->getParent()getThisType(context).getAsString();
		//info.this_type = decl->getQualifiedNameAsString();
	}
	return info;
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
				FunctionInfo caller = getFunctionInfo(Context, caller_decl);
				nodes << caller.qualified_name();
				//e.first->dump();
				/*
				for (auto i = e.second->begin(); i != e.second->end(); ++i) {
					i->dump();
				}
				*/
				Array callees;
				for (auto i : *e.second) {
					FunctionDecl *callee_decl = cast<FunctionDecl>(i->getDecl());
					FunctionInfo callee = getFunctionInfo(Context, callee_decl);
					callees << callee.qualified_name();
				}
				edges << Object(caller.qualified_name(), callees);
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
