cimport cpyshiva

cdef test():
    cpyshiva.main2()

cdef class Window:
    """A Window that can be created with pyshiva

    """
    cdef cpyshiva.Window *_c_window
    cdef public int x
    cdef public int y
    cdef public int width
    cdef public int height
    def __cinit__(self, title="pyshiva", width=640, height=480, x=0, y=0):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self._c_window = cpyshiva.make_window(<char *>title, <int>width, <int>height, <int>x, <int>y)

    def set_pos(self, x, y):
        self.x = x
        self.y = y
        cpyshiva.window_set_pos(self._c_window, <int>x, <int>y)

    def refresh(self):
        cpyshiva.window_refresh(self._c_window)

    def __dealloc__(self):
        # This is where we want to clean up memory.
        cpyshiva.window_dealloc(self._c_window)
        print "Deallocatin'"