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

// Function Prototypes
int serializeFloat(PyObject *, char **, unsigned long long *);
PyObject *deserializeFloat(UserBuffer *);


int serializeFloat(PyObject *floatObj, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Float into a string.
 *
 * Inputs: floatObj - The PyFloat to serialize.
 *         buffer  - A pointer to a string to initialize and serialize integer to.
 *         size    - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    double val = PyFloat_AsDouble(floatObj);

    if ((*buffer = malloc(sizeof(val))) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    memcpy((void *) *buffer, (void *) &val, sizeof(val));
    *size = sizeof(val);

    return 0;
}

PyObject *deserializeFloat(UserBuffer *buf) {
/* Function which deserializes a string into a Python Float.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyFloat.
 *
 * Outputs: A Python Float, or NULL if an error occurs.
 */

    double val = 0;

    readBuffer(buf, (unsigned char *) &val, sizeof(val));

    return PyFloat_FromDouble(val);
}
