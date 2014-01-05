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
        visitor.printFuncList();
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
	AnalysisDeclContextManager *m = 
		new  AnalysisDeclContextManager();
	AnalysisDeclContext analysis_context(m, D);
	CFG *cfg = analysis_context.getCFG();
	LangOptions ops;
	for (auto block : *cfg) {
		Object blockObject;
		//block->dump(cfg, ops);

		Array blockElements;
		Array predArray;
		Array succArray;
		//block->dump(cfg, ops, true);
		for (auto I = block->pred_begin(); 
				I != block->pred_end(); ++I) {
			assert(*I);
			predArray << (*I)->getBlockID();
		}
		for (auto I = block->succ_begin(); 
				I != block->succ_end(); ++I) {
			if (!*I) { // FIXME: A Bug of Clang here ?
				// do nothing
			} else {
				succArray << (*I)->getBlockID();
			}
		}
		for (auto e : *block) {
			Object blockElement;
			if (e.getKind() == CFGElement::Kind::Statement) {
				const Stmt *s = e.castAs<CFGStmt>().getStmt();
				CharSourceRange cr = CharSourceRange::getTokenRange(
						s->getSourceRange());
				SourceCodeRange r = SourceCodeRange::parse(
						cr.getBegin().printToString(manager),
						cr.getEnd().printToString(manager));
				blockElement << "start_line" << r.start.line
					<< "start_column" << r.start.column
					<< "end_line" << r.end.line
					<< "end_column" << r.end.column
					<< "code" << r.getLineCode();
				blockElements << blockElement;
			} else {
				assert(false);
			}
		}
		blockObject<< "block_id" << block->getBlockID()
			 << "elements" << blockElements
			 << "pred_id_list" << predArray
			 << "succ_id_list" << succArray;
		blocks << blockObject;
	}
	FunctionInfo function_info = getFunctionInfo(*context, D);
	function << "source_file" << source_file
		<< "id" << function_info.id()
		<< "entry_id" << cfg->getEntry().getBlockID()
		<< "exit_id" << cfg->getExit().getBlockID()
		<< "blocks" << blocks;
	cout << function << endl;
    func_list << function;
	return true;
}

void CFGVisitor::setContext(ASTContext *Context)
{
	context = Context;	
}

void CFGVisitor::printFuncList()
{
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    string json_file(homedir);
    json_file = json_file + "/program-analysis/tmpjson/funclist.json";
    //cout << json_file << endl;
    fstream file;
    file.open(json_file,fstream::trunc|fstream::out);
    Object output;
    output << "funclist" << func_list;
    file << output;
    file.close();
}
