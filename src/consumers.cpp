#include "consumers.h"

#include <iostream>
#include <fstream>

#include "clang/Lex/Lexer.h"
#include "utils.h"

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
		if (method->isInstance()) {
			info.this_type = method->getThisType(context).getAsString();
		}
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
	visitor.setContext(&Context);
	visitor.TraverseDecl(Context.getTranslationUnitDecl());
}

bool CFGVisitor::VisitFunctionDecl(FunctionDecl *D) 
{
	//cout << "VISIT" << endl;
	Object function;
	Array blocks;


	SourceManager &manager = context->getSourceManager();
	StringRef ref = manager.getFilename(D->getSourceRange().getBegin());
	if (ref.empty()) {
		return true;
	}
	string source_file = ref.data();
	if (!D->hasBody() || !is_user_defined(source_file)) {
		//cout << "Skip: " << D->getNameInfo().getAsString() << endl;
		return true;
	}
	//cout << source_file << endl;
	function << "source_file" << source_file;
	AnalysisDeclContextManager *m = 
		new  AnalysisDeclContextManager();
	AnalysisDeclContext context(m, D);
	CFG *cfg = context.getCFG();
	LangOptions ops;
	for (auto block : *cfg) {
		//block->dump(cfg, ops, true);
		//cout << *block << endl;
		for (auto e : *block) {
			if (e.getKind() == CFGElement::Kind::Statement) {
				const Stmt *s = e.castAs<CFGStmt>().getStmt();
				//cout << s->getStmtClassName() << endl;
				cout << s->getLocStart().printToString(manager) << endl;
				cout << s->getLocEnd().printToString(manager) << endl;
				//s->dump();
				CharSourceRange cr = CharSourceRange::getTokenRange(
						s->getSourceRange());
				SourceCodeRange r = SourceCodeRange::parse(
						cr.getBegin().printToString(manager),
						cr.getEnd().printToString(manager));
				cout << "Code:" << r.getLineCode() << endl;
				llvm::StringRef sref = clang::Lexer::getSourceText(
						CharSourceRange::getTokenRange(
							s->getSourceRange()),
						manager, ops);
				//cout << "Stmt:" << sref.data() << ":END" << endl;
				//cout << *s << endl;
				//s->dump();
				//s->dump();
			} else {
				assert(false);
			}
			//cout << *b << endl;
		}
	}
	//cfg->dump(ops, true);
	return true;
}

void CFGVisitor::setContext(ASTContext *Context)
{
	context = Context;	
}
