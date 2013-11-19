#include <iostream>

#include <stdint.h>
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace std;
using namespace clang::tooling;
using namespace llvm;

int main(int argc, const char **argv) {
	cout << "Hello" << endl;
	clang::tooling::CommonOptionsParser OptionsParser(argc, argv);
	ClangTool Tool(OptionsParser.getCompilations(),
			OptionsParser.getSourcePathList());
	return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>());
}
