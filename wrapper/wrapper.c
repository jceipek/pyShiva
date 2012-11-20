#include "wrapper.h"

#define WIDTH 500
#define HEIGHT 500
VGPath rect;

VGPath createRectPath()
{
  return vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                      1,0,0,0, VG_PATH_CAPABILITY_ALL);
}

void refresh(void)
{  
	VGfloat magenta[] = {1,0,1,1};
	vgSetfv(VG_CLEAR_COLOR, 4, magenta);
	vgClear(0,0,WIDTH, HEIGHT);
	 
	vgLoadIdentity();
	vgTranslate(100,100);
	vgDrawPath(rect, VG_FILL_PATH);

	glfwSwapBuffers();
}

void create_window(int argc, char**argv, int w, int h, int pos_x, int pos_y, const char *title) {

	// Open an OpenGL window
	if( !glfwOpenWindow( w, h, 0,0,0,0,0,0, GLFW_WINDOW ) )
	{
		// Clean up glfw
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetWindowTitle(title);
	glfwSetWindowPos(pos_x, pos_y);

	vgCreateContextSH(w,h);
}

int main2(int argc, char **argv){
	VGPaint fill; //declare an object that is filled
	VGfloat white[] = {1,1,1,1}; //declare an object to represent the color white

	int running = GL_TRUE;

	// Initialize GLFW
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
	create_window(argc, argv, WIDTH, HEIGHT, 0, 0, "HELLO"); //make a window

	fill = vgCreatePaint(); // actually make the fill object
    vgSetParameterfv(fill, VG_PAINT_COLOR, 4, white); //change fill to be white
    vgSetPaint(fill, VG_FILL_PATH); // set the active fill color to be the white fill color we just defined
	
	rect = createRectPath();
	vguRect(rect, -50,-30, 100,60);

	while (running) {
		refresh();

		// Terminate when ESC is pressed or the window is closed
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

	// Clean up the ShivaVG context
	vgDestroyContextSH();

	// Close window and terminate GLFW
	glfwTerminate();
	
	return 0;
}


