#include "shiva_wrapper.h"

#define WIDTH 500
#define HEIGHT 500
VGPath rect;
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
Window *make_window (char *title, int width, int height, int pos_x, int pos_y) {
	if (main_window != NULL) {
		printf("%s\n", "There can only be one window at any time!");
		return main_window;
	}

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

	int n = glfwOpenWindow(width, height, 0,0,0,0,0,0, GLFW_WINDOW);
	if (!n) {
		glfwTerminate(); // Cleanup GLFW
		return NULL; // Couldn't create a window
	}

	glfwSetWindowSizeCallback(window_resize_callback);
	glfwSetWindowCloseCallback(window_close_callback);

	glfwSetWindowTitle(title);
	glfwSetWindowPos(pos_x, pos_y);

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
	}
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

void window_set_pos (Window *window, int pos_x, int pos_y) {
	window->pos_x = pos_x;
	window->pos_y = pos_y;
	glfwSetWindowPos(pos_x, pos_y);
}

void window_set_size (Window *window, int width, int height) {
	glfwSetWindowSize( width, height );
}

int window_isopen (Window *window) {
	return glfwGetWindowParam(GLFW_OPENED);
}

void window_dealloc (Window *window) { // XXX: TODO: return deallocation success as a flag
	if (main_window != NULL) {
		// Clean up the ShivaVG context
		vgDestroyContextSH();

		// Close window and terminate GLFW
		glfwTerminate();

		// XXX: Clean up the contents of the struct
		free(window); // TODO: Check to see if success!
		window = NULL;
		main_window = NULL;
	} else {
		printf("%s\n", "WINDOW ALREADY DEALLOCATED!");
	}
}
// END WINDOW
//

//
// START LAYER_NODE
void layerNode_add_end_node (LayerList *list, LayerNode *node) {
	// TODO: Implement!
}

void layerNode_add_start_node (LayerList *list, LayerNode *node) {
	// TODO: Implement!
}

void layerNode_remove (LayerList *list, LayerNode *node) {
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

	return node;
}

void layerNode_dealloc(LayerNode *node) {
	// TODO: Implement this!!!
	free(node);
}
// END LAYER_NODE
//

//
// START LAYER_LIST
LayerList *make_layerList() {
	LayerList *list = check_malloc(sizeof(LayerList));
	list->identifier = ID_LAYER_LIST;
	list->length = 0;
	list->first = NULL;
	list->last = NULL;

	return list;
}

void layerList_dealloc(LayerList *list) {
	// TODO: Implement this!!!
	free(list);
}
// END LAYER_LIST
//

int demo() {
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