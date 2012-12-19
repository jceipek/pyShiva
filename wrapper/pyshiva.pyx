cimport cpyshiva

def get_mouse_pos():
    '''Get the integer x,y position of the mouse cursor.

    The bottom left corner of the window is at (0,0). 
    Values increase upwards and to the right.
    '''
    cdef int x
    cdef int y
    cpyshiva.get_mouse_pos(&x, &y)
    return (x, y)

cdef Color make_color_if_needed(color):
    '''Create a color object as appropriate based on the input color.

    If the input color is already a color object, the object itself will be returned.
    If the input is None, a fully translucent color will be returned.
    '''
    if isinstance(color, Color):
        return color
    elif not color:
        return Color(0,0,0,0) #Transparent black
    else:
        return Color(*color)

cdef class Color:
    '''A Color in r,g,b,a space.

    Colors have an opacity associated with them via an alpha channel.
    '''
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
        '''The red color channel can range from 0 (no red content) to 1 (full red)

        '''
        def __get__(self):
            return self._r
        def __set__(self, float value):
            self._r = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 0)

    property g:
        '''The green color channel can range from 0 (no green content) to 1 (full green)

        '''
        def __get__(self):
            return self._g
        def __set__(self, float value):
            self._g = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 1)

    property b:
        '''The blue color channel can range from 0 (no blue content) to 1 (full blue)

        '''
        def __get__(self):
            return self._b
        def __set__(self, float value):
            self._b = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 2)

    property a:
        '''The alpha color channel can range from 0 (transparent) to 1 (opaque)

        '''
        def __get__(self):
            return self._a
        def __set__(self, float value):
            self._a = value
            self._c_color = cpyshiva.color_change(self._c_color, value, 3)

    property values:
        '''Use this property to modify the values of the color all at once.

        '''
        def __get__(self):
            return (self._r, self._g, self._b, self._a)

        def __set__(self, iter_value):
            for i,v in enumerate(iter_value):
                if i > 3:
                    raise IndexError("colors have 4 channels: r,g,b,a")
                self.__setitem__(self, i, <float>v)

    def __getitem__(self, int index):
        if index == 0:
            return self._r
        elif index == 1:
            return self._g
        elif index == 2:
            return self._b
        elif index == 3:
            return self._a
        raise IndexError("colors have 4 channels: r,g,b,a")

    def __setitem__(self, int index, float value):
        if index == 0:
            self._r = value
        elif index == 1:
            self._g = value
        elif index == 2:
            self._b = value
        elif index == 3:
            self._a = value
        else:
            raise IndexError("colors have 4 channels: r,g,b,a")    
        self._c_color = cpyshiva.color_change(self._c_color, value, index)

    def __repr__(self):
        return str((self._r, self._g, self._b, self._a))

    def __str__(self):
        return "Color with r:%f g:%f b:%f a:%f" % (self._r, self._g, self._b, self._a)

    def __dealloc__(self):
        cpyshiva.color_dealloc(self._c_color)

cdef class Entity:
    '''A Entity that can be added to groups and the window

    '''
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
    '''A Group

    '''
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
    '''Shapes are core graphics primitives that can be added to windows and groups.

    Making a shape directly is currently senseless, because it doesn't have a visual representation.
    In the future, shapes in pyshiva will support arbitrary paths. For now, we strongly suggest
    that you use the shape subclasses (Rect, Ellipse, and Circle) and subclasses thereof to create objects you want to display.
    
    All shapes have an x,y position in window coordinates as well as a color, a stroke thickness, and a stroke color.
    '''
    cdef Color _color
    cdef Color _stroke_color

    def __init__(self, float x=0, float y=0, color=(1,1,1,1), stroke_color=None, stroke_thickness=1.0):
        if not self._inited:
            self._inited = True
            self.__init_with_color_object__(x, y, make_color_if_needed(color), make_color_if_needed(stroke_color), stroke_thickness)

    cdef __init_with_color_object__(self, float x, float y, Color fill_color, Color stroke_color, float stroke_thickness):
        if stroke_color:
            self._c_object = cpyshiva.make_shape(x, y, fill_color._c_color, stroke_thickness, stroke_color._c_color)
        else:
            self._c_object = cpyshiva.make_shape(x, y, fill_color._c_color, stroke_thickness, NULL)
        self._color = fill_color
        self._stroke_color = stroke_color

    property stroke_thickness:
        def __get__(self):
            return self._c_object.stroke_thickness
        def __set__(self, float value):
            self._c_object.stroke_thickness = value

    property color:
        def __get__(self):
            return self._color
        def __set__(self, value):
            self._set_color_to_color_object(make_color_if_needed(value))

    property stroke_color:
        def __get__(self):
            return self._stroke_color
        def __set__(self, value):
            self._set_stroke_color_to_color_object(make_color_if_needed(value))

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
    '''Rectangles are graphics primitives that can be added to windows and groups.
    
    Their x,y position specifies the position of their bottom left corner, which is
    consistent with the cartesian coordinate system that pyshiva uses.
    The size of a rectangle is specified using width and height.
    '''
    cdef float _width
    cdef float _height

    def __init__(self, float x=0, float y=0, float width=20, float height=10, color=(1,1,1,1), stroke_color=None, stroke_thickness=1.0):
        if not self._inited:
            Shape.__init__(self, x, y, color, stroke_color, stroke_thickness)
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
    '''Ellipses are graphics primitives that can be added to windows and groups.
    
    Unlike rectangles, their x,y position specifies their center rather than their corner.
    Unlike circles, ellipses have no radius. Instead, their size is specified by a width and height.
    '''
    cdef float _width
    cdef float _height

    def __init__(self, float x=0, float y=0, float width=20, float height=10, color=(1,1,1,1), stroke_color=None, stroke_thickness=1.0):
        if not self._inited:
            Shape.__init__(self, x, y, color, stroke_color, stroke_thickness)
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
    '''Circles are graphics primitives that can be added to windows and groups.
    
    Unlike rectangles, their x,y position specifies their center rather than their corner.
    Circles have a radius that specifies their size.
    '''
    cdef float _radius

    def __init__(self, float x=0, float y=0, float radius=5, color=(1,1,1,1), stroke_color=None, stroke_thickness=1.0):
        if not self._inited:
            Shape.__init__(self, x, y, color, stroke_color, stroke_thickness)
            self._radius = radius
            cpyshiva.make_ellipse_from_shape(self._c_object, radius*2, radius*2)
    
    property radius:
        '''The radius of the circle specifies the size of the circle.

        Note: the radius does not include the stroke stroke thickness.
        '''
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
    '''A Window that will contain any vector graphics objects that it contains.

    To maintain input responsiveness, we suggest the following usage:

        while window.is_open():
            # move objects and add new objects as desired
            window.refresh()

    The coordinate system of the window is the same as everywhere else in pyshiva;
    the bottom left corner of the window is at (0,0) and values increase upwards and to the right.

    Note: Due to limitations of ShivaVG and GLFW, once the window is closed, no
    new window can currently be created after an existing window is closed.
    Furthermore, a window must exist before any pyshiva objects can be created.
    '''
    cdef _bg_color
    cdef dict _map_c_to_python
    cdef cpyshiva.Window *_c_window
    def __cinit__(self, char *title="pyshiva", int width=640, int height=480, bg_color=(0,0,0,1)):
        self._map_c_to_python = dict()
        self._c_window = cpyshiva.make_window(title, width, height)
        self._set_bg_color_to_color_object(make_color_if_needed(bg_color))

    property title:
        '''The text to display in the window's titlebar

        '''
        def __get__(self):
            return self._c_window.title
        def __set__(self, char *value):
            cpyshiva.window_set_title (self._c_window, value)

    property width:
        '''The width of the window, in pixels.

        '''
        def __get__(self):
            return self._c_window.width
        def __set__(self, int value):
            cpyshiva.window_set_size(self._c_window, value, self._c_window.height)

    property height:
        '''The height of the window, in pixels.

        '''
        def __get__(self):
            return self._c_window.height
        def __set__(self, int value):
            cpyshiva.window_set_size(self._c_window, self._c_window.width, value)

    property bg_color:
        '''The background color of the window.

        Note that the alpha channel in this color has no effect.
        '''
        def __get__(self):
            return self._bg_color
        def __set__(self, color):
            self._set_bg_color_to_color_object(make_color_if_needed(color)) 

    cdef _set_bg_color_to_color_object(self, Color color):
        self._bg_color = color
        cpyshiva.window_set_bg_color(self._c_window, color._c_color)

    def is_open(self):
        '''Is the window currently open?

        Note: if the window is not currently open, it will no longer be possible
        to create or manipulate vector graphics objects due to the limitations
        of GLFW and ShivaVG
        '''
        return cpyshiva.window_isopen(self._c_window)

    def refresh(self):
        '''Update the contents of the window and handle input.

        We suggest using this function as part of a main loop to maintain responsiveness:

            while window.is_open():
                # move objects and add new objects as desired
                window.refresh()
        '''
        cpyshiva.window_refresh(self._c_window)

    def add(self, Entity obj):
        '''Add an entity to the window. 

        It will be added above any other objects already in the window.
        The entity could be a group, a pyshiva vector graphics
        primitive, or a user-created subclass of one of these things.
        The next time `window.refresh()` is called, the entity will be drawn on the screen
        (if it is within the window bounds).
        '''
        self._map_c_to_python[<int>obj._c_object] = obj
        cpyshiva.window_add_object(self._c_window, obj._c_object)

    def remove(self, Entity obj):
        '''Remove an entity from the window.

        '''
        del self._map_c_to_python[<int>obj._c_object]
        cpyshiva.window_remove_object(self._c_window, obj._c_object)

    def s_since_open(self):
        '''The amount of time that the window has been open, in seconds.
        
        '''
        return cpyshiva.glfwGetTime()

    def s_since_refresh(self):
        '''The amount of time since the window has refreshed, in seconds.

        This is useful for computing the distance that an object should move in a
        discrete-timestep simulation such as a game.
        '''
        return cpyshiva.glfwGetTime() - self._c_window.s_last_refresh_time

    def __len__(self):
        '''The amount of entities contained directly in the window.

        Note: this number is not recursive; if the window contains a group,
        with 100 objects, the objects inside the group do not contribute to
        the amount.
        '''
        return self._c_window.contents.length

    def __getitem__(self, int index):
        '''Indexing into the window like a list returns the entity on the layer provided as a key.

        Layer 0 is the lowest layer.
        '''
        obj = cpyshiva.window_get_item(self._c_window, index)
        if obj:
            return self._map_c_to_python[<int>obj]
        else:
            return None

    def __iter__(self):
        '''The window supports iteration of the form:
            for entity in window:
                #do something with the entity

        Entities are in order from lowest object to highest object in the layer stack.

        This provides a simple way to filter objects.
        NOTE: Removing objects while iterating through them is unsafe!
        '''
        def gen_for_objects():
            curr_node = cpyshiva.window_get_first_node(self._c_window)
            while curr_node:
                yield self._map_c_to_python[<int>curr_node.contents]
                curr_node = curr_node.next
        
        return gen_for_objects()

    def __dealloc__(self):
        cpyshiva.window_dealloc(self._c_window)

cdef class __global_context__:
    '''The pyshiva global context provides an internal way for pyshiva to ensure that the 
    OpenGL and ShivaVG graphics contexts are deallocated after every other object. This avoids
    memory leaks and segfaults.

    '''
    def __dealloc__(self):
        cpyshiva.module_dealloc()

ctx = __global_context__()
