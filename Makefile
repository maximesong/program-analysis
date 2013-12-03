vpath %.o build
vpath %.cpp src
vpath %.h include
vpath %.d build

CC = clang++
program = pa
sources = main.cpp consumers.cpp jsonxx.cpp
objects = $(addprefix build/,$(sources:.cpp=.o))
build_dir = build
LINKFLAGS = --std=c++11 -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangARCMigrate -lclangRewriteFrontend -lclangRewriteCore -lclangAnalysis -lclangEdit -lclangAST -lclangBasic -lclangLex `llvm-build/Release+Asserts/bin/llvm-config --cxxflags --ldflags --libs`


CXXFLAGS += --std=c++11 -Iinclude `llvm-build/Release+Asserts/bin/llvm-config --cxxflags`

# -Lllvm-build/Release+Asserts/lib/ -lclangTooling -lclang -lLLVMCore -lLTO -L/usr/lib64/llvm -lLLVMSupport

all: $(program)

$(program): $(objects)
	$(CC) $^ -o $@ $(LINKFLAGS)

build/%.d: %.cpp
	mkdir -p build
	@set -e; rm -f $@; \
		$(CC) -MM $< $(CXXFLAGS) >  $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ :,g' < $@.$$$$ > $@; \
		cat $@ ;\
		rm -f $@.$$$$
		
build/%.o: %.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS)

clean:
	rm -rf $(program) build

try: $(program)
	@./$(program) samples/main.cpp

runserver:
	@cd web && python server.py 8000

include $(addprefix build/,$(sources:.cpp=.d))
