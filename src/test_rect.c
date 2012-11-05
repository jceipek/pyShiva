#include "test_rect.h"





void create_window(int w, int h, const char *title){
	glutInit(NULL, NULL);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | 
						GLUT_STENCIL | GLUT_MULTISAMPLE);
	
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow(title);
	
	vgCreateContextSH(w,h);
	
}

int main(int argc, char **argv){
	create_window(500,500, "HELLO");
	return 0;
	
	
	
	
}


