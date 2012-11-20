OS := $(shell uname)

INCLUDE_FLAGS = -I. -I../ShivaVG/include -I/usr/local/include

ifeq ($(OS), Linux)
	# We are on Linux
	CC = gcc
	GL_FLAGS = -lglut -lGL -lGLU
endif
ifeq ($(OS), Darwin)
	# We are on OSX
	CC = clang
	GL_FLAGS = -g ../glfw-2.7.7/lib/cocoa/libglfw.a ../ShivaVG/src/libOpenVG.a -framework OpenGL -framework Cocoa -framework IOKit
endif

test_wrapper: wrapper.c
	$(CC) wrapper.c wrapper_caller.c $(INCLUDE_FLAGS) $(GL_FLAGS) -o test_wrapper