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

    property values:
        def __get__(self):
            return (self._r, self._g, self._b, self._a)

        def __set__(self, iter_value):
            for i,v in enumerate(iter_value):
                if i > 3:
                    raise IndexError("colors have 4 channels: r,g,b,a")
                self._c_color = cpyshiva.color_change(self._c_color, <float>v, i)

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
    """A Entity that can be added to groups and the window

    """
    cdef bint _inited
    cdef cpyshiva.Object *_c_object

    def __cinit__(self):
        self._inited = False

    def __init__(self, float x=0, float y=0):
        if not self._inited:
            self._inited = True
            self._c_object = cpyshiva.make_object(x, y)
            print 'entity_init'

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
        cpyshiva.object_dealloc(self._c_object)

cdef class Group(Entity):
    """A Group

    """
    cdef dict _map_c_to_python

    def __init__(self, float x=0, float y=0):
        if not self._inited:
            self._map_c_to_python = dict()
            self._inited = True
            self._c_object = cpyshiva.make_group(x, y)

    def add(self, Entity obj):
        self._map_c_to_python[<int>obj._c_object] = obj
        cpyshiva.group_add_object(self._c_object, obj._c_object)

    def remove(self, Entity obj):
        del self._map_c_to_python[<int>obj._c_object]
        cpyshiva.group_remove_object(self._c_object, obj._c_object)

    def __len__(self):
        return self._c_object.contains.length

    def __getitem__(self, int index):
        obj = cpyshiva.group_get_item(self._c_object, index)
        if obj:
            return self._map_c_to_python[<int>obj]
        else:
            return None

    def __iter__(self):
        def gen_for_objects():
            curr_node = cpyshiva.group_get_first_node(self._c_object)
            while curr_node:
                yield self._map_c_to_python[<int>curr_node.contents]
                curr_node = curr_node.next
        
        return gen_for_objects()

cdef class Shape(Entity):
    cdef Color _color
    cdef Color _stroke_color

    def __init__(self, float x=0, float y=0, color=(1,1,1,1), stroke_color=None, stroke_width=1.0):
        if not self._inited:
            self._inited = True

            if isinstance(color, Color):
                if isinstance(stroke_color, Color):
                    self.__init_with_color_object__(x, y, color, stroke_color, stroke_width)
                elif not stroke_color is None:
                    self.__init_with_color_object__(x, y, color, Color(*stroke_color), stroke_width)
                else:
                    self.__init_with_color_object__(x, y, color, None, stroke_width)
            else:
                if isinstance(stroke_color, Color):
                    self.__init_with_color_object__(x, y, Color(*color), stroke_color, stroke_width)
                elif not stroke_color is None:
                    self.__init_with_color_object__(x, y, Color(*color), Color(*stroke_color), stroke_width)
                else:
                    self.__init_with_color_object__(x, y, Color(*color), None, stroke_width)

    cdef __init_with_color_object__(self, float x, float y, Color fill_color, Color stroke_color, float stroke_width):
        if stroke_color:
            self._c_object = cpyshiva.make_shape(x, y, fill_color._c_color, stroke_width, stroke_color._c_color)
        else:
            self._c_object = cpyshiva.make_shape(x, y, fill_color._c_color, stroke_width, NULL)
        self._color = fill_color
        self._stroke_color = stroke_color

    property stroke_width:
        def __get__(self):
            return self._c_object.stroke_width
        def __set__(self, float value):
            self._c_object.stroke_width = value

    property color:
        def __get__(self):
            return self._color
        def __set__(self, value):
            if isinstance(value, Color):
                self._set_color_to_color_object(value)
            else:
                self._set_color_to_color_object(Color(*value))

    property stroke_color:
        def __get__(self):
            return self._stroke_color
        def __set__(self, value):
            if isinstance(value, Color):
                self._set_stroke_color_to_color_object(value)
            else:
                self._set_stroke_color_to_color_object(Color(*value))

    cdef _set_color_to_color_object(self, Color color):
        self._color = color
        cpyshiva.shape_recolor(self._c_object, color._c_color)

    cdef _set_stroke_color_to_color_object(self, Color color):
        self._stroke_color = color
        cpyshiva.shape_recolor(self._c_object, color._c_color)    
    
    def __repr__(self):
        return str((self.x, self.y, self.color))

    def __str__(self):
        return "Shape at (%f, %f) with color %s" % (self.x, self.y, self.color)
     
    # Dealloc inherited from Entity



cdef class Rect(Shape):
    """A Rect that can be added to groups and the window
    """
    cdef float _width
    cdef float _height

    def __init__(self, float x=0, float y=0, float width=20, float height=10, color=(1,1,1,1), stroke_color=None, stroke_width=1.0):
        if not self._inited:
            Shape.__init__(self, x, y, color, stroke_color, stroke_width)
            self._width = width
            self._height = height
            cpyshiva.make_rect_from_shape(self._c_object, width, height)
    property width:
        def __get__(self):
            return self._width
        def __set__(self, float value):
            self._width = value
            cpyshiva.rect_resize(self._c_object, value, self.height)

    property height:
        def __get__(self):
            return self._height
        def __set__(self, float value):
            self._height = value
            cpyshiva.rect_resize(self._c_object, self.width, value)
    
    def __repr__(self):
        return str((self.x, self.y, self.width, self.height, self.color))

    def __str__(self):
        return "Rect at (%f, %f) with size (%f,%f) and color %s" % (self.x, self.y, self.width, self.height, self.color)

    # Dealloc inherited from Entity

cdef class Ellipse(Shape):
    """A Ellipse that can be added to groups and the window
    """
    cdef float _width
    cdef float _height

    def __init__(self, float x=0, float y=0, float width=20, float height=10, color=(1,1,1,1), stroke_color=None, stroke_width=1.0):
        if not self._inited:
            Shape.__init__(self, x, y, color, stroke_color, stroke_width)
            self._width = width
            self._height = height
            cpyshiva.make_ellipse_from_shape(self._c_object, width, height)
    property width:
        def __get__(self):
            return self._width
        def __set__(self, float value):
            self._width = value
            cpyshiva.ellipse_resize(self._c_object, value, self.height)

    property height:
        def __get__(self):
            return self._height
        def __set__(self, float value):
            self._height = value
            cpyshiva.ellipse_resize(self._c_object, self.width, value)

    def __repr__(self):
        return str((self.x, self.y, self.width, self.height, self.color))
        
    def __str__(self):
        return "Ellipse at (%f, %f) with size (%f,%f) and color %s" % (self.x, self.y, self.width, self.height, self.color)
    # Dealloc inherited from Entity

cdef class Circle(Shape):
    """A Circle that can be added to groups and the window
    """
    cdef float _radius

    def __init__(self, float x=0, float y=0, float radius=5, color=(1,1,1,1), stroke_color=None, stroke_width=1.0):
        if not self._inited:
            Shape.__init__(self, x, y, color, stroke_color, stroke_width)
            self._radius = radius
            cpyshiva.make_ellipse_from_shape(self._c_object, radius*2, radius*2)
    
    property radius:
        def __get__(self):
            return self._radius
        def __set__(self, float value):
            self._radius = value
            cpyshiva.ellipse_resize(self._c_object, value*2, value*2)

    def __repr__(self):
        return str((self.x, self.y, self.radius, self.color))
        
    def __str__(self):
        return "Circle at (%f, %f) with radius %f and color %s" % (self.x, self.y, self.radius, self.color)
    # Dealloc inherited from Entity

cdef class Window:
    """A Window that can be created with pyshiva

    """
    cdef dict _map_c_to_python
    cdef cpyshiva.Window *_c_window
    def __cinit__(self, char *title="pyshiva", int width=640, int height=480):
        self._map_c_to_python = dict()
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
        self._map_c_to_python[<int>obj._c_object] = obj
        cpyshiva.window_add_object(self._c_window, obj._c_object)

    def remove(self, Entity obj):
        del self._map_c_to_python[<int>obj._c_object]
        cpyshiva.window_remove_object(self._c_window, obj._c_object)

    def s_since_open(self):
        return cpyshiva.glfwGetTime()

    def s_since_refresh(self):
        return cpyshiva.glfwGetTime() - self._c_window.s_last_refresh_time

    def __len__(self):
        return self._c_window.contents.length

    def __getitem__(self, int index):
        obj = cpyshiva.window_get_item(self._c_window, index)
        if obj:
            return self._map_c_to_python[<int>obj]
        else:
            return None

    def __iter__(self):
        def gen_for_objects():
            curr_node = cpyshiva.window_get_first_node(self._c_window)
            while curr_node:
                yield self._map_c_to_python[<int>curr_node.contents]
                curr_node = curr_node.next
        
        return gen_for_objects()

    def __dealloc__(self):
        cpyshiva.window_dealloc(self._c_window)

cdef class __global_context__:
    def __dealloc__(self):
        cpyshiva.module_dealloc()

ctx = __global_context__()
