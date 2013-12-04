#include "consumers.h"

#include <iostream>

using namespace std;
using namespace jsonxx;

string FunctionInfo::id()
{
	string id;
	id += qualified_name;
	id += "(";
	if (parameter_types.size() != 0) {
		id += parameter_types[0];
		for (int i = 1; i != parameter_types.size(); ++i) {
			id += ", " + parameter_types[i];
		}
	}
	id += ")";
	return id;
}

Object FunctionInfo::to_json()
{
	Object func;
	func << "id" << id()
		<< "name" << name
		<< "qualified_name" << qualified_name
		<< "return_type" << return_type;
	Array parameters;
	for (auto p : parameter_types) {
		parameters << p;
	}
	func << "parameter_types" << parameters;
	return func;
}

FunctionInfo getFunctionInfo(ASTContext &context, FunctionDecl *decl)
{
	FunctionInfo info;
	info.name = decl->getNameAsString();
	info.qualified_name = decl->getQualifiedNameAsString();
	info.return_type = decl->getResultType().getAsString();
	for (auto p = decl->param_begin(); p != decl->param_end(); ++p) {
		string type = (*p)->getOriginalType().getAsString();
		info.parameter_types.push_back(type);
	}
	if (isa<CXXMethodDecl>(decl)) {
		CXXMethodDecl *method = cast<CXXMethodDecl>(decl);
		info.this_type = method->getThisType(context).getAsString();
		//cout << info.this_type << "\t" << info.qualified_name << endl;
	}
	return info;
}

bool is_user_defined(string filename) {
	//cout << filename << endl;
	if (filename.find("/usr") == string::npos) {
		return true;
	} else {
		return false;
	}
}

void CallGraphConsumer::HandleTranslationUnit(ASTContext &Context)
{
	CallGraph callGraph;
	callGraph.addToCallGraph(Context.getTranslationUnitDecl());
	SourceManager &manager = Context.getSourceManager();
	Object all;
	Object edges;
	Object nodes;
	for (auto e : callGraph) {
		if (e.first != 0) {
			string source_file = 
				manager.getFilename(e.first->getSourceRange().getBegin()).data();
			if (is_user_defined(source_file)) {
				FunctionDecl *caller_decl = cast<FunctionDecl>(e.second->getDecl());
				FunctionInfo caller = getFunctionInfo(Context, caller_decl);
				nodes << caller.id() << caller.to_json();
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
					callees << callee.id();
				}
				edges << caller.id() << callees;
			}
			//cout << i.first->getSourceRange() << endl;
		}
	}
	all << "functions" << nodes;
	all << "calls" << edges;
	cout << all.json() << endl;
}

void CFGConsumer::HandleTranslationUnit(ASTContext &Context)
{
	//v.TraverseDecl(Context.getTranslationUnitDecl());
}
