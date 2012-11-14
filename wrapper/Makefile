OS := $(shell uname)

INCLUDE_FLAGS = -I. -I../include -I/usr/local/include

ifeq ($(OS), Linux)
	# We are on Linux
	CC = gcc
	GL_FLAGS = -lglut -lGL -lGLU
endif
ifeq ($(OS), Darwin)
	# We are on OSX
	CC = clang
	GL_FLAGS = -g -framework GLUT -framework OpenGL -framework Cocoa
endif

test_wrapper: wrapper.c
	$(CC) wrapper.c wrapper_caller.c ../src/*.o $(INCLUDE_FLAGS) $(GL_FLAGS) -o test_wrapper