LDLIBS=-lm -lglut -lGLEW -lGL
all: graph
clean:
	rm -f *.o graph
graph: ./shader_utils/shader_utils.o
.PHONY: all clean
