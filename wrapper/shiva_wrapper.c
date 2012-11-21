#include "shiva_wrapper.h"

#define WIDTH 500
#define HEIGHT 500
VGPath rect;

//
// BEGIN HELPERS
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
// END HELPERS
//

//
// BEGIN WINDOW
Window *make_window (char *title, int width, int height, int pos_x, int pos_y) {
	if( !glfwInit() )
	{
		return NULL; // Couldn't initialize GLFW
	}

	Window *window = check_malloc (sizeof(Window));
	window->title = title; // XXX: TODO: Should copy; not point!
	window->width = width;
	window->height = height;
	window->pos_x = pos_x;
	window->pos_y = pos_y;

	int n = glfwOpenWindow( width, height, 0,0,0,0,0,0, GLFW_WINDOW );
	if (!n) {
		glfwTerminate(); // Cleanup GLFW
		return NULL; // Couldn't create a window
	}

	glfwSetWindowTitle(title);
	glfwSetWindowPos(pos_x, pos_y);

	vgCreateContextSH(width, height); // XXX: TODO: handle errors!!!!

	return window;
}

void window_refresh (Window *window) {
	// TODO: make this loop through every element of the variable size
	// datastructure the window will have.

	VGfloat magenta[] = {1,0,1,1};
	vgSetfv(VG_CLEAR_COLOR, 4, magenta);
	vgClear(0, 0, window->width, window->height);
	 
	vgLoadIdentity();
	vgTranslate(100,100);
	vgDrawPath(rect, VG_FILL_PATH);

	glfwSwapBuffers();	
}

void window_set_pos (Window *window, int pos_x, int pos_y) {
	window->pos_x = pos_x;
	window->pos_y = pos_y;
	glfwSetWindowPos(pos_x, pos_y);
}

int window_isopen (Window *window) {
	return glfwGetWindowParam(GLFW_OPENED);
}

void window_dealloc (Window *window) { // XXX: TODO: return deallocation success as a flag
	// Clean up the ShivaVG context
	vgDestroyContextSH();

	// Close window and terminate GLFW
	glfwTerminate();

	// XXX: Clean up the contents of the struct
	free(window);
}
// END WINDOW
//

int main2(){
	VGPaint fill; //declare an object that is filled
	VGfloat white[] = {1,1,1,1}; //declare an object to represent the color white

	int running = GL_TRUE;

	// Initialize GLFW, create a ShivaVG context, open a window
	Window *win = make_window("HELLO", WIDTH, HEIGHT, 0, 0);

	fill = vgCreatePaint(); // actually make the fill object
    vgSetParameterfv(fill, VG_PAINT_COLOR, 4, white); //change fill to be white
    vgSetPaint(fill, VG_FILL_PATH); // set the active fill color to be the white fill color we just defined
	
	rect = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                      1,0,0,0, VG_PATH_CAPABILITY_ALL);
	vguRect(rect, -50,-30, 100,60);

	while (running) {
		window_refresh(win);

		// Terminate when ESC is pressed or the window is closed
		running = !glfwGetKey(GLFW_KEY_ESC) && window_isopen(win);
	}

	// Close the window, clean up the ShivaVG context, and clean up GLFW
	window_dealloc(win);
	
	return 0;
}


