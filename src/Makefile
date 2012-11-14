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
	GL_FLAGS = -framework GLUT -framework OpenGL -framework Cocoa
endif

test_rect: test_rect_caller.c
	$(CC) test_rect.c test_rect_caller.c *.o $(INCLUDE_FLAGS) -o test_rect $(GL_FLAGS)
