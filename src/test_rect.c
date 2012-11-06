#include "test_rect.h"


void testCleanup(void)
{
  
  vgDestroyContextSH();

}


void create_window(int argc, char**argv, int w, int h, const char *title){
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | 
						GLUT_STENCIL | GLUT_MULTISAMPLE);
	
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow(title);
	
	//glutDisplayFunc(testDisplay);
	
	atexit(testCleanup);
	
	vgCreateContextSH(w,h);
	
}

int main(int argc, char **argv){
	create_window(argc, argv, 500,500, "HELLO");
	glutMainLoop();
	return 0;
	
	
	
	
}


