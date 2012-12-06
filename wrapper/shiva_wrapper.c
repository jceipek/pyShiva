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
	if (object->layer_node->layer_list_ref == window->contents) { //if object is in window:
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

	//deallocate node without affecting contents
	node->contents = NULL;
	layerNode_dealloc(node);
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
	//frees node, preserves groups and objects
	// TODO: Test this!!!
	
	if (node->contents != NULL)
	{
		//object_dealloc(node->contents);
		node->contents->layer_node = NULL;
	}
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
    LayerNode *next;
    while(node != NULL) {
        next = node->next;
        layerNode_dealloc(node);
        node = next;
    }
	free(list);
}

int check_layerlist(LayerList *list) {
	if (list->first == NULL && list->last == NULL){
		printf("empty list\n");
		return 1; //valid; empty list
	}
	if (list->first == NULL || list->last == NULL){
		printf("undefined ends\n");
		return 0; //invalid
	}
	LayerNode *node = list->first;
	LayerNode *next = node -> next;
	while (next != NULL){
		if (node != next->previous){
			printf("next/previous mismatch\n");
			return 0; //invalid
		}
		node = next;
		next = node->next;
	}
	if (list->last != node){
		printf("last does not reflect actual last\n");
		return 0; //invalid
	}
	return 1; //valid
}
// END LAYER_LIST
//

//
// START OBJECT
Object *make_object(float x, float y) {
	Object *object = check_malloc(sizeof(Object));
	object->x = x;
	object->y = y;
	object->type = OBJECT_GENERIC;
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
	object->type = OBJECT_RECT;

	return object;
}

void resize_rect(float width, float height, Object *rect){
	vguRect(rect->path_data, 0, 0, width, height);
}


void recolor_rect(Object *rect, Color *fill){
	rect->fill_ref = fill;
}

void object_dealloc(Object *object) {

	if (object->layer_node != NULL){
		LayerList *list = object->layer_node->layer_list_ref;
		if(list != NULL){
			layerNode_remove(list, object->layer_node);
		}
		object->layer_node = NULL;
    }
    if (object->type == OBJECT_GROUP) {
        layerList_dealloc(object->contains);
    }
    
    else if (object->path_data != NULL) {
    	vgDestroyPath(object->path_data);
        //VGPath free(object->path_data); //TODO: Look up syntax, implement
    }
    // NOTE: the color ref'd by color_ref needs to get deallocated manually elsewhere

    free(object);
}

void object_draw (Object *object, float x, float y) {
	vgTranslate(x, y);
	if (object->type != OBJECT_GROUP) {
		vgSetPaint(object->fill_ref->paint, VG_FILL_PATH);
		vgDrawPath(object->path_data, VG_FILL_PATH); // TODO: Make this not just be a rect!
	} else {
		LayerNode *curr;
		curr = object->contains->first;
		while (curr != NULL) {
			vgLoadIdentity();
			object_draw (curr->contents, x+curr->contents->x, y+curr->contents->y);
			curr = curr->next;
		}
	}
}
// END OBJECT
//

//
// START GROUP

Object *make_group(float x, float y) {
	Object *group = make_object(x, y);
	group->contains = make_layerList();
	group->layer_node = NULL;
	group->type = OBJECT_GROUP;
	return group;
}

int group_add_object (Object *group, Object *object) {
	if (object->layer_node == NULL) {
		LayerNode *node = make_layerNode();
		node->contents = object;
		object->layer_node = node;
		layerNode_add_end_node(group->contains, node);
		return 1; //Succeeded
	}
	return 0; // Failed; object is already part of something else
}

int group_remove_object (Object *group, Object *object) {
	if (group->type != OBJECT_GROUP){
		printf("Not a valid group\n");
		return 0; //Failed; group not a group
	}
	if (object->layer_node->layer_list_ref == group->contains) { //if object is in group:
		layerNode_remove(group->contains, object->layer_node);
		object->layer_node = NULL;
		if (!check_layerlist(group->contains)){
			printf("layerlist bad\n");
		}
		return 1; //Succeeded
	}
	return 0; // Failed; object is not part of group->contains
}

// END GROUP
//

Color *make_color (float r, float g, float b, float a) {
	// TODO: More checking during allocation
	Color *color = check_malloc(sizeof(Color));
	VGPaint paint;
	VGfloat *paint_array = check_malloc(sizeof(VGfloat)*4);
	paint_array[0] = r;
	paint_array[1] = g;
	paint_array[2] = b;
	paint_array[3] = a;
	paint = vgCreatePaint();
    vgSetParameterfv(paint, VG_PAINT_COLOR, 4, paint_array);
    color->paint = paint;
    color->paint_array = paint_array;

    return color;
}

void color_dealloc (Color *color) {
    vgDestroyPaint(color->paint);
    free(color->paint_array);
	free(color);
}


Color *color_change (Color *color, float val, int color_elem) {
	vgDestroyPaint(color->paint);
	color->paint_array[color_elem] = val;
	VGPaint newPaint = vgCreatePaint();
	vgSetParameterfv(newPaint, VG_PAINT_COLOR, 4, color->paint_array);
    color->paint = newPaint;

    return color;
}

void module_dealloc() {
    // Clean up the ShivaVG context
	vgDestroyContextSH();

	// Close window and terminate GLFW
	glfwTerminate();
}

int demo() {
	int running = GL_TRUE;

	// Initialize GLFW, create a ShivaVG context, open a window
	Window *win = make_window("HELLO", 640, 480);

	Color *color = make_color(1,1,1,1);
	Object *group = make_group(0,0);
	Object *group2 = make_group(0,0);

	int i;
	int n = 6;
	Object *objects[n];
	for (i = 0; i < 3; i++) {
		objects[i] = make_rect(i*120, 0, 50, 50, color);
		group_add_object(group, objects[i]);

		//window_add_object(win, objects[i]);
	}
	Object *demo_object = make_rect(100, 300, 100, 50, color);
	Object *demo_object2 = make_rect(100, 300, 100, 50, color);
	Object *demo_object3 = make_rect(100, 300, 100, 50, color);

	window_add_object(win, demo_object);
	window_add_object(win, demo_object2);
	window_add_object(win, demo_object3);

	object_dealloc(demo_object);
	object_dealloc(demo_object2);
	object_dealloc(demo_object3);

	
	for (i = 3; i < n; i++) {
		objects[i] = make_rect((i-3)*120, 200, 50, 20, color);
		group_add_object(group2, objects[i]);
		//if (i<6)
	//		window_add_object(win, objects[i]);
	}
	/*

	window_remove_object(win, demo_object);
	window_add_object(win, demo_object);
*/
	group_add_object(group, group2);

	window_add_object(win, group);

	//group_remove_object(group, group2);

	
	object_dealloc(group2);
	printf("check group list\n");
	if (!check_layerlist(group->contains))
		printf("group layerlist corrupt\n");
	object_dealloc(group);
	printf("check win list\n");
	if (!check_layerlist(win->contents))
		printf("window layerlist corrupt\n");




	while (running) {
		window_refresh(win);

		// Terminate when ESC is pressed or the window is closed
		running = !glfwGetKey(GLFW_KEY_ESC) && window_isopen(win);
	}

	for (i = 0; i < n; i++) {
		object_dealloc(objects[i]);
	}
	//object_dealloc(demo_object);
	//free(objects);


	color_dealloc(color);
	printf("deallocated color\n");
	// Close the window, clean up the ShivaVG context, and clean up GLFW
	window_dealloc(win);
	//does not deallocate objects
	printf("deallocated win\n");

	module_dealloc();
	
	return 0;
}
