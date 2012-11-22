cimport cpyshiva

def test():
    cpyshiva.demo()

cdef class Window:
    """A Window that can be created with pyshiva

    """
    cdef cpyshiva.Window *_c_window
    def __cinit__(self, title="pyshiva", width=640, height=480, x=0, y=0):
        self._c_window = cpyshiva.make_window(<char *>title, <int>width, <int>height, <int>x, <int>y)

    property x:
        def __get__(self):
            return self._c_window.pos_x
        def __set__(self, value):
            cpyshiva.window_set_pos(self._c_window, <int>value, self._c_window.pos_y)

    property y:
        def __get__(self):
            return self._c_window.pos_y
        def __set__(self, value):
            cpyshiva.window_set_pos(self._c_window, self._c_window.pos_x, <int>value)

    property width:
        def __get__(self):
            return self._c_window.width
        def __set__(self, value):
            cpyshiva.window_set_size(self._c_window, <int>value, self._c_window.height)

    property height:
        def __get__(self):
            return self._c_window.height
        def __set__(self, value):
            cpyshiva.window_set_size(self._c_window, self._c_window.width, <int>value)

    def is_open(self):
        return cpyshiva.window_isopen(self._c_window)

    def refresh(self):
        cpyshiva.window_refresh(self._c_window)

    def __dealloc__(self):
        # This is where we want to clean up memory.
        cpyshiva.window_dealloc(self._c_window)
        print "Deallocatin'"