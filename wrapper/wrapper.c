#include "wrapper.h"

#define WIDTH 500
#define HEIGHT 500
VGPath rect;

void testCleanup(void)
{
  printf("Cleaning up vgContext before quitting!\n");
  vgDestroyContextSH();

}

VGPath testCreatePath()
{
  return vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                      1,0,0,0, VG_PATH_CAPABILITY_ALL);
}

void keyboardHandler(unsigned char key, int x, int y)
{
	// If the ESC key is pressed
	if (key == 27)
		close_window();		
}

void refresh(void)
{  
	VGfloat magenta[] = {1,0,1,1};
	vgSetfv(VG_CLEAR_COLOR, 4, magenta);
	vgClear(0,0,WIDTH, HEIGHT);
	 
	vgLoadIdentity();
	vgTranslate(100,100);
	vgDrawPath(rect, VG_FILL_PATH);
	/* Swap */
	glutSwapBuffers();
}

void close_window() {
	printf("Close Called\n");
	glutDestroyWindow(glutGetWindow());
	exit(0);
}

void create_window(int argc, char**argv, int w, int h, int pos_x, int pos_y, const char *title) {
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | 
						GLUT_STENCIL | GLUT_MULTISAMPLE);

	glutInitWindowPosition(pos_x, pos_y);
	glutInitWindowSize(w, h);
	glutCreateWindow(title);
	
	glutDisplayFunc(refresh);
	glutKeyboardFunc(keyboardHandler);
	//glutCloseFunc(close_window);
	
	//atexit(testCleanup);
	
	vgCreateContextSH(w,h);
}

int main2(int argc, char **argv){
	VGPaint fill; //declare an object that is filled
	VGfloat white[] = {1,1,1,1}; //declare an object to represents the collor white

	create_window(argc, argv, WIDTH, HEIGHT, 0, 0, "HELLO"); //make a window
	//hello is the header text
	
	fill = vgCreatePaint();// actually make the fill object
    vgSetParameterfv(fill, VG_PAINT_COLOR, 4, white); //change fill to be white
    vgSetPaint(fill, VG_FILL_PATH);
	
	rect = testCreatePath();
	vguRect(rect, -50,-30, 100,60);
	
	glutMainLoop();
	return 0;
}


