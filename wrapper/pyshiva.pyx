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
            self._r = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 0)


    property g:
        def __get__(self):
            return self._g
        def __set__(self, float value):
            self._g = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 1)

    property b:
        def __get__(self):
            return self._b
        def __set__(self, float value):
            self._b = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 2)

    property a:
        def __get__(self):
            return self._a
        def __set__(self, float value):
            self._a = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 3)

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

    def __dealloc__(self):
        cpyshiva.color_dealloc(self._c_color)

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

    def __dealloc__(self):
    # TODO: Add this back in after making it so that closing the window does not delete the objects in it.
        cpyshiva.object_dealloc(self._c_object)

cdef class Group(Entity):
    """A Group

    """
    def __init__(self, float x=0, float y=0):
        if not self._inited:
            self._inited = True
            self._c_object = cpyshiva.make_group(x, y)

    def add(self, Entity obj):
        cpyshiva.group_add_object(self._c_object, obj._c_object)

    def remove(self, Entity obj):
        cpyshiva.group_remove_object(self._c_object, obj._c_object)

cdef class Rect(Entity):
    """A Rect that can be added to groups and the window

    """
    cdef float _width
    cdef float _height
    cdef Color _color

    def __init__(self, float x=0, float y=0, float width=20, float height=10, color=(1,1,1,1)):
        if not self._inited:
            self._inited = True
            self._width = width
            self._height = height
            if isinstance(color, Color):
                self._color = color
                self._init_with_color_object(x, y, width, height, self._color)
            else:
                self._color = Color(*color)
                self._init_with_color_object(x, y, width, height, self._color)
    
    cdef _init_with_color_object(self, float x, float y, float width, float height, Color color):
        self._c_object = cpyshiva.make_rect(x, y, width, height, color._c_color)

    property width:
        def __get__(self):
            return self._width
        def __set__(self, value):
            cpyshiva.resize_rect(value, self.height, self._c_object)

    property height:
        def __get__(self):
            return self._height
        def __set__(self, value):
            cpyshiva.resize_rect(self.width, value, self._c_object)

    property color:
        def __get__(self):
            return self._color
        def __set__(self, value):
            if isinstance(value, Color):
                self._color = value
                self._set_color_to_color_object(value)
            else:
                self._color = Color(*value)
                self._set_color_to_color_object(self._color)

    cdef _set_color_to_color_object(self, Color color):
        cpyshiva.recolor_rect(self._c_object, color._c_color)

    def __repr__(self):
        return str((self.x, self.y, self.width, self.height))

    def __str__(self):
        return "Rect at (%f, %f) with size (%f,%f)" % (self.x, self.y, self.width, self.height)

    # Dealloc inherited from Entity

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

    property bg_color:
        def __set__(self, color):
            cpyshiva.window_set_bg(self._c_window, <float>color[0], <float>color[1], <float>color[2])

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
        cpyshiva.window_dealloc(self._c_window)

cdef class __global_context__:
    def __dealloc__(self):
        cpyshiva.module_dealloc()

ctx = __global_context__()
