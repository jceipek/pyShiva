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

#include "../src/GL/glext.h"

#include <vg/openvg.h>
#include <vg/vgu.h>

void create_window(int argc, char**argv, int w, int h, int pos_x, int pos_y, const char *title);
void refresh(void);
int main2(int argc, char **argv);