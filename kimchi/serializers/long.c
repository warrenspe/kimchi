/*
 * MIT License
 *
 * Copyright (c) 2017 Warren Spencer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
