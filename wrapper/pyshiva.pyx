cimport cpyshiva

cdef class Window:
    """A Window that can be created with pyshiva

    """
    def __cinit__(self, title="pyshiva", width=640, height=480, pos_x=0, pos_y=0):
        #print cpyshiva.create_window(<int>width, <int>height, <int>pos_x, <int>pos_y, <char *>title)
        cpyshiva.testr()
        cpyshiva.main2()
        print "Hello"

    def __dealloc__(self):
        # This is where we want to clean up memory.
        print "Deallocatin'"