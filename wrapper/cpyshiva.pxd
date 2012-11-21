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
    int window_isopen (Window *window)
    void window_set_pos (Window *window, int pos_x, int pos_y)

    int main2()