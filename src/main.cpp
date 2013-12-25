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

class ProgramAnalysisAction : public ASTFrontendAction {
public:
	virtual ASTConsumer *CreateASTConsumer(
			CompilerInstance &Compiler, StringRef InFile) {
		return new CFGConsumer;
	}
};

void analyze_code(string source) {
	runToolOnCode(new ProgramAnalysisAction, source);
}

void analyze_file(string filename) {
	std::ifstream t(filename.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();
	analyze_code(buffer.str());
}

int main(int argc, const char **argv) {
	if (argc == 2) {
		//analyze_file(argv[1]);
		CommonOptionsParser OptionsParser(argc, argv);
		ClangTool Tool(OptionsParser.getCompilations(),
				OptionsParser.getSourcePathList());
		Tool.run(newFrontendActionFactory<ProgramAnalysisAction>());
	} else if (argc == 3) {
		int my_argc = 2;
		const char *my_argv[2];
		my_argv[0] = argv[0];
		my_argv[1] = argv[2];
		CommonOptionsParser OptionsParser(my_argc, my_argv);
		ClangTool Tool(OptionsParser.getCompilations(),
				OptionsParser.getSourcePathList());
		Tool.run(newFrontendActionFactory<ProgramAnalysisAction>());
	} else {
		cout << "Usage: " << argv[0] << " <filename>" << endl;
	}
	return 0;
}
