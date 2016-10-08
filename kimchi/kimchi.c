/*
 *  kimchi.c: A library used to serialize/deserialize certain python objects.
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


/* DevNote: The memory format this library uses to serialize objects is as follows:
 *
 * [Type][Size][Serialization]
 *
 * Type: 1 byte.
 *     Bitflag representing what type was serialized here. See headers/kimchi.h for type definitions.
 *
 * Size: X bytes.
 *     The number of bytes the size field consumes is determined by the first byte which has its first bit set to 1.
 *     Example: [00001111][00000000][11111111] <- size is 3 bytes long; first byte with a 1 in the first position
 *              indicates that it is the final byte.  Note that only the first 7 bits of each byte contribute towards
 *              the value that is serialized in the size field (the first of each is just a continuation flag).
 *              It was implemented in this manner to be more frugal regarding the # of stored bytes; as using only
 *              7 bits per byte is more efficient than dedicating an entire byte to represent the length of the size
 *              field for all serialized objects up to 72057594037927936 (2**56) bytes in size. (The maximum we're able
 *              to serialize being 2**63).  Since we expect the vast majority of serialized objects to be of a size
 *              smaller than 2**56 it was implemented in this fashion.
 *
 * Serialization: {Size} bytes.
 *     The serialization of the object.  Certain objects (ex, iterables) have their own substructures in this section.
 */

/* DevNote: To extend this library with the capability to serialize further objects, the following steps must be taken.
 *     Follow the existing conventions in place in each of the files when making changes.
 *
 *     1. Add a new entry to kimchi/headers/kimchi.h: Type Definitions.
 *     2. Add an entry to if block chain in kimchi/utils/types.c discerning whether a generic python object is of your type.
 *     3. Add cases to the switches in kimchi/utils/serialize.c to serialize and deserialize objects of your type.
 *     4. Add a new file to kimchi/serializers/ to serialize & deserialize your type.
 *        Ensure that the Python Object returned from your deserialization has a ref count of 1.
 *     5. Add a new unit test for your type to the kimchi/tests/ directory.
 *        It will be automatically included in the unit tests.
 */

#include "headers/kimchi.h"

static PyObject *dumps(PyObject *self, PyObject *args) {
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

    obj = PyBytes_FromStringAndSize(outputBuffer, (Py_ssize_t) size);
    free(outputBuffer);

    return obj;
}

static PyObject *loads(PyObject *self, PyObject *args) {
/* API function which deserializes a python object.
 *
 * Inputs: self - Unused. The required `self` parameter of any python-visible function.
 *         args - Positional arguments passed to the function.
 *
 * Outputs: A copy of the original Python object serialized by the dumps function, or NULL if an error occurs.
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

static PyMethodDef kimchiMethods[] = {
    {"dumps", (PyCFunction) dumps, METH_VARARGS, "Serialize a python object as a string."},
    {"loads", (PyCFunction) loads, METH_VARARGS, "Deserialize a string as a python object."},
    {NULL, NULL, 0, NULL}   /* sentinel */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef kimchiDef = {
        PyModuleDef_HEAD_INIT,
        "kimchi",
        "Provides serialization capabilities for most built in objects",
        -1,
        kimchiMethods,
        NULL,
        NULL,
        NULL,
        NULL
};

PyMODINIT_FUNC PyInit_kimchi(void) {
    return PyModule_Create(&kimchiDef);
}

#else
void initkimchi(void) {
    Py_InitModule("kimchi", kimchiMethods);
}
#endif
