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

    ctypedef struct Object:
        float x
        float y
        #LayerList *contains
        #LayerNode *layer_node
        #VGPath *path_data

    Object *make_object(float x, float y)
    Object *make_rect(float x, float y, float width, float height, Color *color)
    int window_add_object (Window *window, Object *object)
    int window_remove_object (Window *window, Object *object)

    void get_mouse_pos(int *x, int *y)

    int demo()
