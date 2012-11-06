#include "test_rect.h"

#define WIDTH 500
#define HEIGHT 500
VGPath rect;

void testCleanup(void)
{
  
  vgDestroyContextSH();

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

VGPath testCreatePath()
{
  return vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                      1,0,0,0, VG_PATH_CAPABILITY_ALL);
}


void create_window(int argc, char**argv, int w, int h, const char *title){
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | 
						GLUT_STENCIL | GLUT_MULTISAMPLE);
	
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow(title);
	
	glutDisplayFunc(refresh);
	
	atexit(testCleanup);
	
	vgCreateContextSH(w,h);
	
}

int main(int argc, char **argv){
	VGPaint fill;
	VGfloat white[] = {1,1,1,1};
	
	create_window(argc, argv, WIDTH, HEIGHT, "HELLO");
	
	fill = vgCreatePaint();
    vgSetParameterfv(fill, VG_PAINT_COLOR, 4, white);
    vgSetPaint(fill, VG_FILL_PATH);
	
	rect = testCreatePath();
	vguRect(rect, -50,-30, 100,60);
	
	glutMainLoop();
	return 0;
	
	
	
	
}


