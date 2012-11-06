OS := $(shell uname)

INCLUDE_FLAGS = -I. -I../include -I/usr/local/include -L/usr/local/lib

ifeq ($(OS), Linux)
	# We are on Linux
	CC = gcc
	GL_FLAGS := "-lglut -lGL -lGLU"
endif
ifeq ($(OS), Darwin)
	# We are on OSX
	CC = clang
	GL_FLAGS = -framework GLUT -framework OpenGL -framework Cocoa
endif

test_rect: test_rect.c
	CC test_rect.c *.o $(GL_FLAGS) $(INCLUDE_FLAGS) -o test_rect