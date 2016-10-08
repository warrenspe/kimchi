/*
 *  long.c: A file containing functions used by kimchi.c to serialize longs.
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

#include "headers/kimchi.h"
#include "longintrepr.h"

// Function Prototypes
int serializeLong(PyObject *, unsigned char, char **, unsigned long long *);
PyObject *deserializeLong(UserBuffer *, unsigned char, unsigned long long);


int serializeLong(PyObject *longObj, unsigned char type, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Long into a string.
 *
 * Inputs: longObj - The PyLong to serialize.
 *         type    - A char containing the serialization type of longObj. (POS/NEG long).
 *         buffer  - A pointer to a string to initialize and serialize integer to.
 *         size    - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    PyLongObject *pyLong = (PyLongObject *) longObj;

    *size = Py_SIZE(pyLong) * sizeof(digit);

    if (type == NEG_LONG_TYPE) {
        *size = -(*size);
    }

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    memcpy((void *) *buffer, (void *) pyLong->ob_digit, *size);

    return 0;
}

PyObject *deserializeLong(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a string into a Python Long.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyLong.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the PyLong.
 *
 * Outputs: A Python Long, or NULL if an error occurs.
 */

    PyLongObject *pyLong = _PyLong_New((Py_ssize_t) size / sizeof(digit));

    if (type == NEG_LONG_TYPE) {
        Py_SIZE(pyLong) = -Py_SIZE(pyLong);
    }

    if (readBuffer(buf, (unsigned char *) (pyLong->ob_digit), size)) {
        Py_DECREF(pyLong);
        return NULL;
    }

    return (PyObject *) pyLong;
}
