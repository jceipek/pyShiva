cimport cpyshiva

def test():
    cpyshiva.demo()

cdef class Entity:
    cdef bint _inited
    cdef cpyshiva.Object *_c_object
    """A Entity that can be added to groups and the window

    """
    def __cinit__(self):
        self._inited = False

    def __init__(self, x=0, y=0):
        if not self._inited:
            self._inited = True
            self._c_object = cpyshiva.make_object(<float>x, <float>y)

    property x:
        def __get__(self):
            return self._c_object.x
        def __set__(self, value):
            self._c_object.x = <float>value

    property y:
        def __get__(self):
            return self._c_object.y
        def __set__(self, value):
            self._c_object.y = <float>value

cdef class Rect(Entity):
    """A Rect that can be added to groups and the window

    """
    #def __cinit__(self, x=0, y=0, width=20, height=10):
    #    self._c_object = cpyshiva.make_rect(<float>x, <float>y, <float>width, <float>height)
    def __init__(self, x=0, y=0, width=20, height=10):
        if not self._inited:
            self._inited = True
            self._c_object = cpyshiva.make_rect(<float>x, <float>y, <float>width, <float>height)       

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

    def add(self, Entity obj):
        cpyshiva.window_add_object(self._c_window, obj._c_object)

    def remove(self, Entity obj):
        cpyshiva.window_remove_object(self._c_window, obj._c_object)

    def __dealloc__(self):
        # This is where we want to clean up memory.
        cpyshiva.window_dealloc(self._c_window)
        print "Deallocatin'"


