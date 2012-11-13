#include <Python.h>
#include "test_rect.h"


/*static PyObject *
pyShiva_rect(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return Py_BuildValue("i", sts);
}
*/
static PyObject *pyShiva_test_rect(PyObject *self, PyObject *args)
{
	int sts;
	char *argv[1];
	argv[0] = "test_rect";
	sts = main2(1, argv);
	return Py_BuildValue("i", sts);
	
}


static PyMethodDef PyShivaMethods[] = {
	
    {"test_rect",  pyShiva_test_rect, METH_VARARGS,
     "Make a rectangle"},
     
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC

initrect(void)
{
	(void) Py_InitModule("rect", PyShivaMethods);
}
