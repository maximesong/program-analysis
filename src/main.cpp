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
#include "utils.h"

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
	char cwd[256];
	getcwd(cwd, 255);
	DirSettings::setWorkingDir(cwd);

	if (argc > 2) {
		if(argc ==3) argc--;
		CommonOptionsParser OptionsParser(argc, argv);
		ClangTool Tool(OptionsParser.getCompilations(),
				OptionsParser.getSourcePathList());
		if (strncmp(argv[2],"-cfg", 4) == 0 ){
			Tool.run(newFrontendActionFactory<CFGAnalysisAction>());
		} else if(strncmp(argv[2],"-cg", 3) == 0 ){
			Tool.run(newFrontendActionFactory<CGAnalysisAction>());
		} else{
			cout<< "Usage: " << argv[2] << " <analysis type>" << endl;
		}
	} else {
		cout << "Usage: " << argv[0] << " <filename> <type>" << endl;
	}
	return 0;
}
