vpath %.o build
vpath %.cpp src
vpath %.h include
vpath %.d build

CC = clang++
program = pa
sources = main.cpp analyzer.cpp
objects = $(addprefix build/,$(sources:.cpp=.o))
build_dir = build
CXXFLAGS += -Iinclude

all: $(program)

$(program): $(objects)
	$(CC) $^ -o $@

build/%.d: %.cpp
	mkdir -p build
	@set -e; rm -f $@; \
		$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ :,g' < $@.$$$$ > $@; \
		cat $@ ;\
		rm -f $@.$$$$
		
build/%.o: %.cpp
	$(CC) -c -o $@ $(CXXFLAGS) $<

clean:
	rm -rf $(program) build

include $(addprefix build/,$(sources:.cpp=.d))
