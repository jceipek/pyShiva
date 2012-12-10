cdef extern from "shiva_wrapper.h":
    double glfwGetTime()

#Basically a header file for pyshiva.pyx
cdef extern from "shiva_wrapper.h":

    ctypedef struct Window:
        int width
        int height
        char *title
        double s_last_refresh_time

    Window *make_window (char *title, int width, int height)
    void window_refresh (Window *window)
    void window_dealloc (Window *window)
    bint window_isopen (Window *window)
    void window_set_size (Window *window, int width, int height)
    void window_set_title (Window *window, char *title)

    ctypedef struct Color:
        pass
    Color *make_color(float r, float g, float b, float a)
    void *color_dealloc(Color *color)
    Color *color_change(Color *color, float val, int color_elem)


    ctypedef struct Object:
        float x
        float y
        #enum OBJECT_TYPE type
        #cdef union WAT:
        #    LayerList *contains # Set to NULL unless it is a Group
        #    cdef struct:
        #VGPath *path_data # Set to NULL if it is a Group
        Color *fill_ref # Set to NULL if it is a Group    
        #LayerNode *layer_node # Set to NULL unless it is part of a Group or Window

    Object *make_object(float x, float y)
    Object *make_shape(float x, float y, Color *fill)
    Object *make_rect_from_shape(Object *shape, float width, float height)
    Object *make_ellipse_from_shape(Object *shape, float width, float height)


    void recolor_rect(Object *rect, Color *fill)
    void resize_rect(float width, float height, Object *rect)
    void *object_dealloc(Object *object)
    int window_add_object (Window *window, Object *object)
    int window_remove_object (Window *window, Object *object)
    void window_set_bg(Window *window, float r, float g, float b)
    void get_mouse_pos(int *x, int *y)

    Object *make_group(float x, float y)
    int group_add_object (Object *group, Object *object)
    int group_remove_object (Object *group, Object *object)
    void *group_dealloc(Object *group)

    ctypedef struct LayerNode:
        LayerNode *previous
        Object *contents
        LayerNode *next
        #struct LayerList *layer_list_ref
    #LayerNode *layerList_get_first_node(LayerList *list)
    Object *window_get_item(Window *window, int index)
    LayerNode *window_get_first_node(Window *window)
    Object *group_get_item(Object *group, int index)
    LayerNode *group_get_first_node(Object *group)

    void module_dealloc()

    int demo()