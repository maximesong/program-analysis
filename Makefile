vpath %.o build
vpath %.cpp src
vpath %.h include
vpath %.d build

CC = clang++
program = pa
sources = main.cpp
objects = $(addprefix build/,$(sources:.cpp=.o))
build_dir = build

all: $(program)

$(build_dir):
	mkdir $@

$(program): $(objects)
	$(CC) $^ -o $@

build/%.d: %.cpp $(build_dir)
	@set -e; rm -f $@; \
		$(CC) -MM $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ :,g' > $@
		
build/%.o: %.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $<

clean:
	rm -rf $(program) build

include $(addprefix build/,$(sources:.cpp=.d))
