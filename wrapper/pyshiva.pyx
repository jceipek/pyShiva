cimport cpyshiva

def test():
    cpyshiva.demo()

def get_mouse_pos():
    cdef int x
    cdef int y
    cpyshiva.get_mouse_pos(&x, &y)
    return (x, y)

cdef class Color:
    cdef cpyshiva.Color *_c_color
    cdef float _r
    cdef float _g
    cdef float _b
    cdef float _a
    def __cinit__(self, float r, float g, float b, float a=1.0):
        # TODO: Make colors mutable!
        self._r = r
        self._g = g
        self._b = b
        self._a = a
        self._c_color = cpyshiva.make_color(r, g, b, a)

    property r:
        def __get__(self):
            return self._r
        def __set__(self, float value):
            print "Colors are currently immutable (you can't change them)!"

    property g:
        def __get__(self):
            return self._g
        def __set__(self, float value):
            print "Colors are currently immutable (you can't change them)!"

    property b:
        def __get__(self):
            return self._b
        def __set__(self, float value):
            print "Colors are currently immutable (you can't change them)!"

    property a:
        def __get__(self):
            return self._a
        def __set__(self, float value):
            print "Colors are currently immutable (you can't change them)!"

    def __getitem__(self, int index):
        if index == 0:
            return self._r
        if index == 1:
            return self._g
        if index == 2:
            return self._b
        if index == 3:
            return self._a
        raise IndexError("colors have 4 channels: r,g,b,a")

    def __repr__(self):
        return str((self._r, self._g, self._b, self._a))

    def __str__(self):
        return "Color with r:%f g:%f b:%f a:%f" % (self._r, self._g, self._b, self._a)

cdef class Entity:
    cdef bint _inited
    cdef cpyshiva.Object *_c_object
    """A Entity that can be added to groups and the window

    """
    def __cinit__(self):
        self._inited = False

    def __init__(self, float x=0, float y=0):
        if not self._inited:
            self._inited = True
            self._c_object = cpyshiva.make_object(x, y)

    property x:
        def __get__(self):
            return self._c_object.x
        def __set__(self, float value):
            self._c_object.x = value

    property y:
        def __get__(self):
            return self._c_object.y
        def __set__(self, float value):
            self._c_object.y = value

cdef class Rect(Entity):
    """A Rect that can be added to groups and the window

    """
    cdef float _width
    cdef float _height
    def __init__(self, float x=0, float y=0, float width=20, float height=10, color=(1,1,1,1)):
        if not self._inited:
            self._inited = True
            self._width = width
            self._height = height
            self._c_object = cpyshiva.make_rect(x, y,
                                                width, height, 
                                                cpyshiva.make_color(color[0], color[1], color[2], color[3]))

    property width:
        def __get__(self):
            return self._width
        def __set__(self, value):
            # TODO: Implement!
            print "width cannot be set yet!"

    property height:
        def __get__(self):
            return self._height
        def __set__(self, value):
            # TODO: Implement!
            print "height cannot be set yet!"

    def __repr__(self):
        return str((self.x, self.y, self.width, self.height))

    def __str__(self):
        return "Rect at (%f, %f) with size (%f,%f)" % (self.x, self.y, self.width, self.height)

cdef class Window:
    """A Window that can be created with pyshiva

    """
    cdef cpyshiva.Window *_c_window
    def __cinit__(self, char *title="pyshiva", int width=640, int height=480):
        self._c_window = cpyshiva.make_window(title, width, height)

    property title:
        def __get__(self):
            return self._c_window.title
        def __set__(self, char *value):
            cpyshiva.window_set_title (self._c_window, value)

    property width:
        def __get__(self):
            return self._c_window.width
        def __set__(self, int value):
            cpyshiva.window_set_size(self._c_window, value, self._c_window.height)

    property height:
        def __get__(self):
            return self._c_window.height
        def __set__(self, int value):
            cpyshiva.window_set_size(self._c_window, self._c_window.width, value)

    def is_open(self):
        return cpyshiva.window_isopen(self._c_window)

    def refresh(self):
        cpyshiva.window_refresh(self._c_window)

    def add(self, Entity obj):
        cpyshiva.window_add_object(self._c_window, obj._c_object)

    def remove(self, Entity obj):
        cpyshiva.window_remove_object(self._c_window, obj._c_object)

    def s_since_open(self):
        return cpyshiva.glfwGetTime()

    def s_since_refresh(self):
        return cpyshiva.glfwGetTime() - self._c_window.s_last_refresh_time

    def __dealloc__(self):
        # This is where we want to clean up memory.
        cpyshiva.window_dealloc(self._c_window)
        print "Deallocatin'"


