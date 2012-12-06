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

int GLFWCALL window_close_callback (void);



// end header

void perror_exit(char *s)
{
  perror (s);
  exit (-1);
}

void *check_malloc(int size)
{
  void *p = malloc (size);
  if (p == NULL) perror_exit ("malloc failed");
  return p;
}



int GLFWCALL window_close_callback (void) {
	return GL_TRUE;
}



int main() {
	int running = GL_TRUE;
	int width = 640;
	int height = 480;
	int x = 50;
	int y = 50;
	int w = 10;
	int h = 10;
	/*
	Window *win = check_malloc (sizeof(Window));
	win->width = 640;
	win->height = 480;
	*/

	// Initialize GLFW, create a ShivaVG context, open a window

	if( !glfwInit() )
	{
		return 1; // Couldn't initialize GLFW
	}

	int n = glfwOpenWindow(width, height, 0,0,0,0,0,0, GLFW_WINDOW);
	if (!n) {
		glfwTerminate(); // Cleanup GLFW
		return 1; // Couldn't create a window
	}

	glfwSetWindowCloseCallback(window_close_callback);

	glfwSetWindowTitle("Shiva test");

	vgCreateContextSH(width, height); // XXX: TODO: handle errors!!!!
	
	/*
	Rect *r = make_rect(0,0,100,100,0,1,1,1);
	win->rect = r;
	*/
	
	//make color
	VGPaint paint;
	VGfloat paint_array[] = {1,0,1,1};
	paint = vgCreatePaint();
    vgSetParameterfv(paint, VG_PAINT_COLOR, 4, paint_array);
	
	//make rect

	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
								1,0,0,0, VG_PATH_CAPABILITY_ALL);
	vguRect(path, 0, 0, w, h);


	VGfloat magenta[] = {0.9,0,0,1};
	

	while (running) {
			
		vgSetfv(VG_CLEAR_COLOR, 4, magenta);
		vgClear(0, 0, width, height);
		
		vgLoadIdentity();

		vgTranslate(x, y);
		vgSetPaint(paint, VG_FILL_PATH);
		vgDrawPath(path, VG_FILL_PATH);

		glfwSwapBuffers();	

		// Terminate when ESC is pressed or the window is closed
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// Close the window, clean up the ShivaVG context, and clean up GLFW
	vgDestroyContextSH();

	// Close window and terminate GLFW
	glfwTerminate();

	
	return 0;
}
