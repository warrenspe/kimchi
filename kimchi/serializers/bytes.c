/*
 *  bytes.c: A file containing functions used by kimchi.c to serialize `bytes` objects and Python2 strings.
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
