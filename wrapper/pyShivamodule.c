#include <Python.h>
#include <structmember.h>
#include "wrapper.h"

// WINDOW

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    PyObject *title;
    int width;
    int height;
    int pos_x;
    int pos_y;
} Window;

static void
Window_dealloc(Window* self)
{
    Py_XDECREF(self->title);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Window_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Window *self;

    self = (Window *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->title = PyString_FromString("pyShiva Window");
        if (self->title == NULL)
          {
            Py_DECREF(self);
            return NULL;
          }

        self->width = 640;
        self->height = 480;
        self->pos_x = 0;
        self->pos_y = 0;
    }

    return (PyObject *)self;
}

static int
Window_init(Window *self, PyObject *args, PyObject *kwds)
{
    PyObject *title=NULL, *tmp;

    static char *kwlist[] = {"title", "width", "height", "pos_x", "pos_y", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|Oiiii", kwlist, 
                                      &title, 
                                      &self->width,
                                      &self->height,
                                      &self->pos_x,
                                      &self->pos_y))
        return -1; 

    if (title) {
        tmp = self->title;
        Py_INCREF(title);
        self->title = title;
        Py_XDECREF(tmp);
    }

    // Now actually create the window!
    char *argv[1];
    argv[0] = "";
    create_window(1, argv, self->width, self->height, PyString_AS_STRING(self->title));

    glutMainLoop();

    return 0;
}

static PyMemberDef Window_members[] = {
    {"title", T_OBJECT_EX, offsetof(Window, title), 0,
     "window title"},
    {"width", T_INT, offsetof(Window, width), 0,
     "window width"},
    {"height", T_INT, offsetof(Window, height), 0,
     "window height"},
    {"pos_x", T_INT, offsetof(Window, pos_x), 0,
     "window x position"},
    {"pos_y", T_INT, offsetof(Window, pos_y), 0,
     "window y position"},
    {NULL}  /* Sentinel */
};

// This is a silly method to see how methods are implemented
static PyObject *
Window_info(Window* self)
{
    static PyObject *format = NULL;
    PyObject *args, *result;

    if (format == NULL) {
        format = PyString_FromString("%s (%d %d) at (%d %d)");
        if (format == NULL)
            return NULL;
    }

    if (self->title == NULL) {
        PyErr_SetString(PyExc_AttributeError, "title");
        return NULL;
    }

    // XXX: stuff missing????

    args = Py_BuildValue("Oiiii", self->title, self->width, self->height, self->pos_x, self->pos_y);
    if (args == NULL)
        return NULL;

    result = PyString_Format(format, args);
    Py_DECREF(args);
    
    return result;
}

static PyMethodDef Window_methods[] = {
    {"info", (PyCFunction)Window_info, METH_NOARGS,
     "Return some information about the window as a string (title, size, and position)"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject WindowType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size This needs to be here for historical reasons.*/
    "pyshiva.Window",             /*tp_name*/
    sizeof(Window),             /*tp_basicsize How Python knows how much memory to allocate.*/
    0,                         /*tp_itemsize For objects of variable size.*/ //TODO: use this so we can add objects
    (destructor)Window_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Window objects",           /* tp_doc */ //TODO: Make this meaningful
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    Window_methods,             /* tp_methods */
    Window_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Window_init,      /* tp_init */
    0,                         /* tp_alloc */
    Window_new,                 /* tp_new */
};

static PyMethodDef pyshiva_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initpyshiva(void)
{
    PyObject* m;

    if (PyType_Ready(&WindowType) < 0)
            return;


    m = Py_InitModule3("pyshiva", pyshiva_methods,
                       "Example module that creates an extension type.");

    if (m == NULL)
      return;

    Py_INCREF(&WindowType);
    PyModule_AddObject(m, "Window", (PyObject *)&WindowType);
}