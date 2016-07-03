/*
 *  unicode.c: A file containing functions used by pump.c to serialize unicode strings.
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
int serializeUnicode(PyObject *, char **, unsigned long long *);
PyObject *deserializeUnicode(UserBuffer *, unsigned char, unsigned long long);


int serializeUnicode(PyObject *unicode, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Unicode object to a bytestring.
 *  *
 * Inputs: unicode - The PyUnicode to serialize.
 *         buffer  - A pointer to a string to initialize and serialize str to.
 *         size    - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    *size = (unsigned long long) PyUnicode_GET_SIZE(unicode) * sizeof(Py_UNICODE);

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    // Copy the string to our internal buffer which can later be freed. (the string in tmpbuffer cannot be)
    memcpy((void *) *buffer, (void *) PyUnicode_AS_DATA(unicode), *size);

    return 0;
}

PyObject *deserializeUnicode(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a bytestring into a PyUnicode.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyUnicode.
 *         type - A char containing the type of the object we're deserializing.
 *         size - The number of bytes to use in constructing the PyUnicode.
 *
 * Outputs: A Python Unicode object, or NULL if an error occurs.
 */

    PyObject *unicode;

    // Because _PyUnicode_New is static; we have to use a bit of a roundabout way of getting our hands
    // on a PyUnicode object.
    unicode = PyUnicode_FromUnicode(NULL, (Py_ssize_t) size / sizeof(Py_UNICODE));

    if (unicode == NULL) {
        return NULL;
    }

    if (readBuffer(buf, (unsigned char *) PyUnicode_AS_DATA(unicode), size)) {
        Py_DECREF(unicode);
        return NULL;
    }

    return unicode;
}
