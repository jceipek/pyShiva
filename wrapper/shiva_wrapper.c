#include "shiva_wrapper.h"

Window *main_window = NULL; // GLFW only supports one window!

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
Window *make_window (char *title, int width, int height) {
	if (main_window != NULL) {
		printf("%s\n", "There can only be one window at any time!");
		return main_window;
	}

	if( !glfwInit() )
	{
		return NULL; // Couldn't initialize GLFW
	}

	Window *window = check_malloc (sizeof(Window));
	window->title = strdup(title);
	window->width = width;
	window->height = height;
	window->contents = make_layerList();
	window->s_last_refresh_time = 0.0;
	window->bg_color[0] = 0;
	window->bg_color[1] = 0;
	window->bg_color[2] = 0;
	window->bg_color[3] = 1;

	int n = glfwOpenWindow(width, height, 0,0,0,0,0,0, GLFW_WINDOW);
	if (!n) {
		glfwTerminate(); // Cleanup GLFW
		return NULL; // Couldn't create a window
	}

	glfwSetWindowSizeCallback(window_resize_callback);
	glfwSetWindowCloseCallback(window_close_callback);

	glfwSetWindowTitle(title);

	vgCreateContextSH(width, height); // XXX: TODO: handle errors!!!!

	main_window = window;

	return window;
}

int GLFWCALL window_close_callback (void) {
	// This is the function that gets called when the window gets closed by the user
	// TODO: add a python function here

	return GL_TRUE;
}

void GLFWCALL window_resize_callback (int width, int height) {
	if (main_window != NULL) {
		main_window->width = width;
		main_window->height = height;
		vgResizeSurfaceSH(width, height);
	}
}

void window_set_bg(Window *window, float r, float g, float b) {
	window->bg_color[0] = r;
	window->bg_color[1] = g;
	window->bg_color[2] = b;
}

void window_refresh (Window *window) {
	// TODO: make this loop through every element of the variable size
	// datastructure the window will have.

	window->s_last_refresh_time = glfwGetTime();

	vgSetfv(VG_CLEAR_COLOR, 4, window->bg_color);
	vgClear(0, 0, window->width, window->height);
	
	LayerNode *curr;
	curr = window->contents->first;
	while (curr != NULL) {
		vgLoadIdentity();
		object_draw(curr->contents, curr->contents->x, curr->contents->y);
		curr = curr->next;
	}

	glfwSwapBuffers();	
}

int window_add_object (Window *window, Object *object) {
	if (object->layer_node == NULL) {
		LayerNode *node = make_layerNode();
		node->contents = object;
		object->layer_node = node;
		layerNode_add_end_node(window->contents, node);
		return 1; //Succeeded
	}
	return 0; // Failed; object is already part of something else
}

int window_remove_object (Window *window, Object *object) { //TODO: Check object owner properly.
	if (object->layer_node->layer_list_ref != window->contents) { //if object is in window:
		layerNode_remove(window->contents, object->layer_node);
		object->layer_node = NULL;
		return 1; //Succeeded
	}
	return 0; // Failed; object is not part of window->contents
}


void window_set_size (Window *window, int width, int height) {
	glfwSetWindowSize( width, height );
}

void window_set_title (Window *window, char *title) {
	char* new_title = strdup(title);
	glfwSetWindowTitle(new_title);
	window->title = new_title;
}

int window_isopen (Window *window) {
	return glfwGetWindowParam(GLFW_OPENED);
}

void window_dealloc (Window *window) { // XXX: TODO: return deallocation success as a flag
	if (main_window != NULL) {
		

		free(window->title);
        layerList_dealloc(window->contents); //TODO: Make sure implemented
		free(window); // TODO: Check to see if deallocs are success!
		window = NULL;
		main_window = NULL;
        
        // Clean up the ShivaVG context
		vgDestroyContextSH();

		// Close window and terminate GLFW
		glfwTerminate();
	} else {
		printf("%s\n", "WINDOW ALREADY DEALLOCATED!");
	}
}
// END WINDOW
//

void get_mouse_pos(int *x, int *y) {
	if (main_window != NULL) {
		glfwGetMousePos(x, y);
		*y = main_window->height - *y;
	} else { // Window not allocated!
		*x = 0;
		*y = 0;
	}
}

//
// START LAYER_NODE
void layerNode_add_end_node (LayerList *list, LayerNode *node) {
	if (list->first == NULL) {
		list->first = node;
		list->last = node;
		list->length++;
	} else if (list->last != NULL) {
		list->last->next = node;
		node->previous = list->last;
		list->last = node;
		list->length++;
	}
	node->layer_list_ref = list;
	// TODO: should anything else go here?
}

void layerNode_add_start_node (LayerList *list, LayerNode *node) {
	if (list->first == NULL) {
		list->first = node;
		list->last = node;
		list->length++;
	} else {
		node->next = list->first;
		list->first->previous = node;
		list->first = node;
		list->length++;
	}
	node->layer_list_ref = list;
}

void layerNode_remove (LayerList *list, LayerNode *node) {
	// Assumes that node is in list
	if (list->first == node) {
		list->first = node->next;
	}
	if (list->last == node) {
		list->last = node->previous;
	}
	list->length--;
	if (node->previous != NULL) {
		node->previous->next = node->next;
	}
	if (node->next != NULL) {
		node->next->previous = node->previous;
	}
	// TODO: figure out if we should dealloc here!
}

LayerNode *make_layerNode() {
	LayerNode *node = check_malloc(sizeof(LayerNode));
	node->previous = NULL;
	node->contents = NULL; // TODO: make this be the object we add
	node->next = NULL;
	node->layer_list_ref = NULL;

	return node;
}

void layerNode_dealloc(LayerNode *node) {
	// TODO: Test this!!!
    object_dealloc(node->contents);
    free(node);
}
// END LAYER_NODE
//

//
// START LAYER_LIST
LayerList *make_layerList() {
	LayerList *list = check_malloc(sizeof(LayerList));
	list->length = 0;
	list->first = NULL;
	list->last = NULL;

	return list;
}

void layerList_dealloc(LayerList *list) {
	// TODO: test this!!!
    LayerNode *node = list->first;
    while(node != NULL) {
        LayerNode *next = node->next; 
        layerNode_dealloc(node);
        node = next;
    }
	free(list);
}
// END LAYER_LIST
//

//
// START OBJECT
Object *make_object(float x, float y) {
	Object *object = check_malloc(sizeof(Object));
	object->x = x;
	object->y = y;
	object->contains = NULL;
	object->layer_node = NULL;
	object->path_data = NULL;
	object->fill_ref = NULL;
	return object;
}

Object *make_rect(float x, float y, float width, float height, Color *fill) {
	Object *object = make_object(x, y);
	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
								1,0,0,0, VG_PATH_CAPABILITY_ALL);
	vguRect(path, 0, 0, width, height);

	object->path_data = path;
	object->fill_ref = fill;

	return object;
}

void object_dealloc(Object *object) {
	// TODO: Implement this!
    if (object->contains != NULL) {
        layerList_dealloc(object->contains);
        free(object);
    }
    /*
    if (object->path_data != NULL) {
    	vgDestroyPath(object->path_data);
        //VGPath free(object->path_data); //TODO: Look up syntax, implement
    }
    */
    // NOTE: the color ref'd by color_ref needs to get deallocated manually elsewhere
    //free(object);
}

void object_draw (Object *object, float x, float y) {
	vgTranslate(x, y);
	if (object->contains == NULL) {
		vgSetPaint(object->fill_ref->paint, VG_FILL_PATH);
		vgDrawPath(object->path_data, VG_FILL_PATH); // TODO: Make this not just be a rect!
	} else {
		LayerNode *curr;
		curr = object->contains->first;
		while (curr != NULL) {
			curr = curr->next;
			vgLoadIdentity();
			object_draw (curr->contents, x+curr->contents->x, y+curr->contents->y);
		}
	}
}
// END OBJECT
//

Color *make_color (float r, float g, float b, float a) {
	// TODO: More checking during allocation
	Color *color = check_malloc(sizeof(Color));
	VGPaint paint;
	VGfloat paint_array[] = {r, g, b, a};
	paint = vgCreatePaint();
    vgSetParameterfv(paint, VG_PAINT_COLOR, 4, paint_array);
    color->paint = paint;

    return color;
}

void color_dealloc (Color *color) {
	// TODO: XXX: Actually free the data in the struct
    vgDestroyPaint(color->paint);
    //VGPaint dealloc(color->paint); //TODO: Look up syntax, implement
	free(color);
}

int demo() {
	int running = GL_TRUE;

	// Initialize GLFW, create a ShivaVG context, open a window
	Window *win = make_window("HELLO", 640, 480);

	Color *color = make_color(1,1,1,1);

	int i;
	for (i = 0; i < 3; i++) {
		Object *object = make_rect(i*120, 0, 50, 50, color);
		window_add_object(win, object);
	}
	Object *demo_object = make_rect(100, 300, 100, 50, color);
	window_add_object(win, demo_object);
	for (i = 0; i < 3; i++) {
		Object *object = make_rect(i*120, 200, 50, 20, color);
		window_add_object(win, object);
	}

	window_remove_object(win, demo_object);
	window_add_object(win, demo_object);

	while (running) {
		window_refresh(win);

		// Terminate when ESC is pressed or the window is closed
		running = !glfwGetKey(GLFW_KEY_ESC) && window_isopen(win);
	}

	color_dealloc(color);
	// Close the window, clean up the ShivaVG context, and clean up GLFW
	window_dealloc(win);
	//does not deallocate objects
	
	return 0;
}
