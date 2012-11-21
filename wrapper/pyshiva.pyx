cimport cpyshiva

cdef test():
    cpyshiva.main2()

cdef class Window:
    """A Window that can be created with pyshiva

    """
    cdef cpyshiva.Window *_c_window
    cdef int _x
    cdef int _y
    cdef public int width
    cdef public int height
    def __cinit__(self, title="pyshiva", width=640, height=480, x=0, y=0):
        self._x = x
        self._y = y
        self.width = width
        self.height = height
        self._c_window = cpyshiva.make_window(<char *>title, <int>width, <int>height, <int>x, <int>y)

    property x:
        def __get__(self):
            return self._x
        def __set__(self, value):
            self._x = value
            cpyshiva.window_set_pos(self._c_window, <int>value, self._y)

    property y:
        def __get__(self):
            return self._y
        def __set__(self, value):
            self._y = value
            cpyshiva.window_set_pos(self._c_window, self._x, <int>value)

    def refresh(self):
        cpyshiva.window_refresh(self._c_window)

    def __dealloc__(self):
        # This is where we want to clean up memory.
        cpyshiva.window_dealloc(self._c_window)
        print "Deallocatin'"