/*
 *  string.c: A file containing functions used by pump.c to serialize strings.
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

// Function prototypes
int serializeString(PyObject *str, char **buffer, unsigned long long *size);
PyObject *deserializeString(UserBuffer *buf, unsigned char type, unsigned long long size);


int serializeString(PyObject *str, char **buffer, unsigned long long *size) {
/* Function which serializes a Python String or Unicode to a bytestring.
 *  *
 * Inputs: str: The PyStr or PyUnicode to serialize.
 *         buffer: A pointer to a string to initialize and serialize str to.
 *         size: A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on Failure.
 */

    char *tmpBuffer;

    // Retrieve the internal string from the PyStr / PyUnicode
    if (PyString_AsStringAndSize(str, &tmpBuffer, (Py_ssize_t *) size) == -1) {
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

PyObject *deserializeString(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a string into a PyStr or PyUnicode.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyStr / PyUnicode
 *         type - A char containing the type of the object we're deserializing.
 *         size - The number of bytes to use in constructing the PyStr / PyUnicode.
 *
 * Outputs: A Python String / Unicode object.
 */

    unsigned char *bytes;

    if ((bytes = malloc(size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for deserialization");
        return NULL;
    }

    if (readBuffer(buf, bytes, size)) {
        return NULL;
    }


    if (type == UNICODE_TYPE) {
        return PyUnicode_FromStringAndSize((char *) bytes, (Py_ssize_t) size);
    }

    return PyString_FromStringAndSize((char *) bytes, (Py_ssize_t) size);
}
