cdef extern from "shiva_wrapper.h":

    ctypedef struct Window:
        int width
        int height
        int pos_x
        int pos_y
        char *title

    Window *make_window (char *title, int width, int height, int pos_x, int pos_y)
    void window_refresh (Window *window)
    void window_dealloc (Window *window)
    bint window_isopen (Window *window)
    void window_set_pos (Window *window, int pos_x, int pos_y)
    void window_set_size (Window *window, int width, int height)

    ctypedef struct Object:
        float x
        float y
        #LayerList *contains
        #LayerNode *layer_node
        #VGPath *path_data

    Object *make_rect(float x, float y, float width, float height)
    int window_add_object (Window *window, Object *object)

    int demo()