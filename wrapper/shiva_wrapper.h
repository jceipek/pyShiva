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

// This structure will keep track of the order in which objects shall be drawn
typedef struct LayerNode {
	struct LayerNode *previous;
	struct Object *contents;
	struct LayerNode *next;
	struct LayerList *layer_list_ref;
} LayerNode;

// This structure will contain information about the layer list
typedef struct LayerList {
	int length;
	struct LayerNode *first;
	struct LayerNode *last;
} LayerList;

void layerNode_add_end_node (LayerList *list, LayerNode *node);
void layerNode_add_start_node (LayerList *list, LayerNode *node);
void layerNode_remove (LayerList *list, LayerNode *node);
LayerNode *make_layerNode();
void layerNode_dealloc(LayerNode *node);

LayerList *make_layerList();
void layerList_dealloc(LayerList *list);

// Color
typedef struct Color {
	VGPaint *paint;
	VGfloat *paint_array;

} Color;

Color *make_color(float r, float g, float b, float a);
void color_dealloc(Color *color);
Color *color_change (Color *color, float val, int color_elem);



// Object

enum OBJECT_TYPE {OBJECT_GROUP, OBJECT_RECT, OBJECT_GENERIC};
typedef struct Object {
	float x, y;
	enum OBJECT_TYPE type;
	union {
		struct LayerList *contains; // Set to NULL unless it is a Group
		struct {
			VGPath *path_data; // Set to NULL if it is a Group
			struct Color *fill_ref; // Set to NULL if it is a Group
		};
	};
	struct LayerNode *layer_node; // Set to NULL unless it is part of a Group or Window
} Object;

Object *make_object(float x, float y);
Object *make_group(float x, float y);
int group_add_object (Object *group, Object *object);
int group_remove_object (Object *group, Object *object);
Object *make_rect(float x, float y, float width, float height, Color *fill);
void object_dealloc(Object *object);
void object_draw (Object *object, float x, float y);

// Window 
typedef struct Window {
	int width, height;
	char *title;
	struct LayerList *contents; // stores objects that get added to the window 

	// TODO: add a high precision clock and the ability to keep track of:
	//	How much time passed since last refresh
	//  How long since the window opened
	double s_last_refresh_time;

	VGfloat bg_color[4];
} Window;

Window *make_window (char *title, int width, int height);
int GLFWCALL window_close_callback (void);
void GLFWCALL window_resize_callback(int width, int height);
void window_refresh (Window *window);
void window_dealloc (Window *window);
int window_isopen (Window *window);
//void window_set_pos (Window *window, int pos_x, int pos_y);
void window_set_size (Window *window, int width, int height);
void window_set_title (Window *window, char *title);
int window_add_object (Window *window, Object *object);
int window_remove_object (Window *window, Object *object);

void window_set_bg(Window *window, float r, float g, float b);

void get_mouse_pos(int *x, int *y);

void module_dealloc();

int demo();
