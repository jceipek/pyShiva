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

enum IDENTIFIER { 
	ID_LAYER_LIST,
	ID_OBJECT
 };

// This structure will keep track of the order in which objects shall be drawn
typedef struct LayerNode {
	struct LayerNode *previous;
	struct Object *contents;
	struct LayerNode *next;
} LayerNode;

// This structure will contain information about the layer list
typedef struct {
	enum IDENTIFIER identifier; // Must be set equal to ID_LAYER_LIST
	int length;
	LayerNode *first;
	LayerNode *last;
} LayerList;

void layerNode_add_end_node (LayerList *list, LayerNode *node);
void layerNode_add_start_node (LayerList *list, LayerNode *node);
void layerNode_remove (LayerList *list, LayerNode *node);
LayerNode *make_layerNode();
void layerNode_dealloc(LayerNode *node);

LayerList *make_layerList();
void layerList_dealloc(LayerList *list);

// Object
typedef struct Object {
	enum IDENTIFIER identifier; // Must be set equal to ID OBJECT
	LayerList *contains; // Set to NULL unless it is a Group
	LayerNode *layer_node; // Set to NULL unless it is part of a Group or Window
	// TODO: add data fields
} Object;

// Window 
typedef struct {
	int width, height, pos_x, pos_y;
	char *title;
	LayerList *contents; // stores objects that get added to the window 

	// TODO: add a high precision clock and the ability to keep track of:
	//	How much time passed since last refresh
	//  How long since the window opened
} Window;

Window *make_window (char *title, int width, int height, int pos_x, int pos_y);
int GLFWCALL window_close_callback (void);
void GLFWCALL window_resize_callback(int width, int height);
void window_refresh (Window *window);
void window_dealloc (Window *window);
int window_isopen (Window *window);
void window_set_pos (Window *window, int pos_x, int pos_y);
void window_set_size (Window *window, int width, int height);
int window_add_object (Window *window, Object *object);

int demo();