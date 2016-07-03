/*
 *  pump.c: A library used to serialize/deserialize certain python objects.
 *  Copyright (C) 2016 Warren Spencer warrenspencer27@gmail.com

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "headers/pump.h"


PyObject *deflate(PyObject *self, PyObject *args) {
/* API function which serializes a python object.
 *
 * Inputs: self - Unused. The required `self` parameter of any python-visible function.
 *         args - Positional arguments passed to the function.
 *
 * Outputs: A Python string containing the serialized value of the passed object, or NULL if an error occurs.
 */

    PyObject *obj;
    Py_ssize_t size;
    char *outputBuffer;

    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }

    if (serialize(obj, &outputBuffer, &size)) {
        return NULL;
    }

    obj = PyString_FromStringAndSize(outputBuffer, (Py_ssize_t) size);
    free(outputBuffer);

    return obj;
}

PyObject *inflate(PyObject *self, PyObject *args) {
/* API function which deserializes a python object.
 *
 * Inputs: self - Unused. The required `self` parameter of any python-visible function.
 *         args - Positional arguments passed to the function.
 *
 * Outputs: A copy of the original Python object serialized by the deflate function, or NULL if an error occurs.
 */

    PyObject *obj;
    int size;
    char *serializedBuffer;
    UserBuffer *buf;

    if (!PyArg_ParseTuple(args, "s#", &serializedBuffer, &size)) {
        return NULL;
    }

    if ((buf = initBuffer(serializedBuffer, (long long) size)) == NULL) {
        return NULL;
    }

    obj = deserialize(buf);
    free(buf);

    return obj;
}

static PyMethodDef pumpMethods[] = {
    {"deflate", (PyCFunction) deflate, METH_VARARGS, "Serialize a python object as a string."},
    {"inflate", (PyCFunction) inflate, METH_VARARGS, "Deserialize a string as a python object."},
    {NULL, NULL, 0, NULL}   /* sentinel */
};

void initpump(void) {
    Py_InitModule("pump", pumpMethods);
}
