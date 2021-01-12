LDLIBS=-lm -lglut -lGLEW -lGL
all: example_1
clean:
	rm -f *.o example_1
example_1: ./shader_utils/shader_utils.o graph.cpp
.PHONY: all clean
