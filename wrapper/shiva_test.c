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
	int x, y;
	VGPaint paint;
	VGPath path;
} Rect;

// Window 
typedef struct {
	int width, height;
	char *title;
	Rect rect;
} Window;

Rect *make_rect(float x, float y, float w, float h, int r, int g, int b, int a);
int GLFWCALL window_close_callback (void);
int window_isopen (Window *window);
void window_refresh (Window *window);
void window_dealloc (Window *window);

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



Rect *make_rect(float x, float y, float w, float h, int r, int g, int b, int a){
	Rect *rect = check_malloc (sizeof(Rect));
	rect->x = x;
	rect->y = y;
	
	//make color
	VGPaint paint;
	VGfloat paint_array[] = {r,g,b,a};
	paint = vgCreatePaint();
    vgSetParameterfv(paint, VG_PAINT_COLOR, 4, paint_array);
    
    rect->paint = paint;
	
	//make rect

	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
								1,0,0,0, VG_PATH_CAPABILITY_ALL);
	vguRect(path, 0, 0, w, h);
	
	rect->path = path;
	
	return rect;
}

int GLFWCALL window_close_callback (void) {
	return GL_TRUE;
}

int window_isopen (Window *window) {
	return glfwGetWindowParam(GLFW_OPENED);
}

void window_refresh (Window *window) {

	VGfloat magenta[] = {0.9,0,0,1};
	vgSetfv(VG_CLEAR_COLOR, 4, magenta);
	vgClear(0, 0, window->width, window->height);
	

	vgLoadIdentity();
	
	//vgTranslate(window->rect->x, window->rect->y);
	vgSetPaint(window->rect->paint, VG_FILL_PATH);
	vgDrawPath(window->rect->path, VG_FILL_PATH);

	glfwSwapBuffers();	
}

void window_dealloc (Window *window) { // XXX: TODO: return deallocation success as a flag
	// Clean up the ShivaVG context
	vgDestroyContextSH();

	// Close window and terminate GLFW
	glfwTerminate();

	// XXX: Clean up the contents of the struct
	free(window); // TODO: Check to see if success!
	window = NULL;
}

int main() {
	int running = GL_TRUE;
	
	Window *win = check_malloc (sizeof(Window));
	win->width = 640;
	win->height = 480;

	// Initialize GLFW, create a ShivaVG context, open a window

	if( !glfwInit() )
	{
		return NULL; // Couldn't initialize GLFW
	}

	int n = glfwOpenWindow(win->width, win->height, 0,0,0,0,0,0, GLFW_WINDOW);
	if (!n) {
		glfwTerminate(); // Cleanup GLFW
		return NULL; // Couldn't create a window
	}

	glfwSetWindowCloseCallback(window_close_callback);

	glfwSetWindowTitle("Shiva test");

	vgCreateContextSH(win->width, win->height); // XXX: TODO: handle errors!!!!
	
	
	Rect *r = make_rect(0,0,100,100,0,1,1,1);
	win->rect = r;


	

	while (running) {
		window_refresh(win);

		// Terminate when ESC is pressed or the window is closed
		running = !glfwGetKey(GLFW_KEY_ESC) && window_isopen(win);
	}

	// Close the window, clean up the ShivaVG context, and clean up GLFW
	window_dealloc(win);
	
	return 0;
}
