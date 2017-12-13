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

// Function prototypes
int serializeBytes(PyObject *, char **, unsigned long long *);
PyObject *deserializeBytes(UserBuffer *, unsigned long long);


int serializeBytes(PyObject *bytes, char **buffer, unsigned long long *size) {
/* Function which serializes a Python2 String or bytes object to a string.
 *
 * Inputs: bytes  - The Py2Str or bytes to serialize.
 *         buffer - A pointer to a string to initialize and serialize bytes to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    char *tmpBuffer;

    // Retrieve the internal string from the Py2Str/bytes
    if (PyBytes_AsStringAndSize(bytes, &tmpBuffer, (Py_ssize_t *) size) == -1) {
        return 1;
    }

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    // Copy the string to our internal buffer which can later be freed. (the string in tmpbuffer cannot be)
    memcpy((void *) *buffer, (void *) tmpBuffer, *size);

    return 0;
}

PyObject *deserializeBytes(UserBuffer *buf, unsigned long long size) {
/* Function which deserializes a string into a bytes object.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a bytes object.
 *         size - The number of bytes to use in constructing the bytes object.
 *
 * Outputs: A bytes object, or NULL if an error occurs.
 */

    unsigned char *bytes;
    PyObject *pyBytes;

    if ((bytes = malloc(size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for deserialization");
        return NULL;
    }

    if (readBuffer(buf, bytes, size)) {
        free(bytes);
        return NULL;
    }

    pyBytes = PyBytes_FromStringAndSize((char *) bytes, (Py_ssize_t) size);

    if (pyBytes == NULL) {
        free(bytes);
    }

    return pyBytes;
}
