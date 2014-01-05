#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

#include <stdint.h>
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/LangOptions.h"

#include "consumers.h"

using namespace std;
using namespace clang::tooling;
using namespace clang;
using namespace llvm;

class CFGAnalysisAction : public ASTFrontendAction {
public:
	virtual ASTConsumer *CreateASTConsumer(
			CompilerInstance &Compiler, StringRef InFile) {
		return new CFGConsumer;
	}
};

class CGAnalysisAction : public ASTFrontendAction {
public:
        virtual ASTConsumer *CreateASTConsumer(
                        CompilerInstance &Compiler, StringRef InFile) {
                return new CallGraphConsumer;
        }
};

void analyze_code(string source) {
	runToolOnCode(new CFGAnalysisAction, source);
}

void analyze_file(string filename) {
	std::ifstream t(filename.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();
	analyze_code(buffer.str());
}

int main(int argc, const char **argv) {
	if (argc > 1) {
		//analyze_file(argv[1]);
                //cout << argv[2] <<endl;
                if(argc ==3) argc--;
		CommonOptionsParser OptionsParser(argc, argv);
		ClangTool Tool(OptionsParser.getCompilations(),
				OptionsParser.getSourcePathList());
                if ( strncmp(argv[2],"-cfg",4) == 0 ){
			Tool.run(newFrontendActionFactory<CFGAnalysisAction>());
                 }else if(strncmp(argv[2],"-cg",3) == 0 ){
			Tool.run(newFrontendActionFactory<CGAnalysisAction>());
		}else{
			cout<< "Usage: " << argv[2] << " <analysis type>" << endl;
		}
	} else if (argc == 3) {
		int my_argc = 2;
		const char *my_argv[2];
		my_argv[0] = argv[0];
		my_argv[1] = argv[2];
		CommonOptionsParser OptionsParser(my_argc, my_argv);
		ClangTool Tool(OptionsParser.getCompilations(),
				OptionsParser.getSourcePathList());
		Tool.run(newFrontendActionFactory<CFGAnalysisAction>());
	} else {
		cout << "Usage: " << argv[0] << " <filename>" << endl;
	}
	return 0;
}
