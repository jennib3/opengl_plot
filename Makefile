LDLIBS=-lm -lglut -lGLEW -lGL

all: example_1 example_object

clean:
	rm -f *.o example_1 example_object

example_1: ./shader_utils/shader_utils.o graph.cpp

example_object: ./shader_utils/shader_utils.o graph.cpp

.PHONY: all clean
