#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if defined(WIN32)
#  define vsnprintf _vsnprintf
#  include <windows.h>
#endif

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include "../glfw-2.7.7/include/GL/glfw.h"
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include "../glfw-2.7.7/include/GL/glfw.h"
#endif

#include "../ShivaVG/src/GL/glext.h"

#include <vg/openvg.h>
#include <vg/vgu.h>

void perror_exit(char *s);
void *check_malloc(int size);

typedef struct {
	int width, height, pos_x, pos_y;
	char *title;
	// TODO: add a data structure here for storing objects that get added to the window 
	// (structure needs to have an ordering, the ability for objects to be efficiently inserted 
	// and removed by hash)

	// TODO: add a high precision clock and the ability to keep track of:
	//	How much time passed since last refresh
	//  How long since the window opened
} Window;

Window *make_window (char *title, int width, int height, int pos_x, int pos_y);
void window_refresh (Window *window);
void window_dealloc (Window *window);
int window_isopen (Window *window);
void window_set_pos (Window *window, int pos_x, int pos_y);

int main2();