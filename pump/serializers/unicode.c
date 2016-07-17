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

    PyObject *utf8String;

    // Encode our unicode object as UTF8, then perform string (bytes) serialization on it
    utf8String = PyUnicode_EncodeUTF8((const Py_UNICODE *) PyUnicode_AS_DATA(unicode),
                                      (Py_ssize_t) PyUnicode_GET_SIZE(unicode),
                                      NULL);
    if (utf8String == NULL) {
        return 1;
    }

    return serializeBytes(utf8String, buffer, size);
}

PyObject *deserializeUnicode(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a utf8 encoded bytestring into a PyUnicode.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyUnicode.
 *         type - A char containing the type of the object we're deserializing.
 *         size - The number of bytes to use in constructing the PyUnicode.
 *
 * Outputs: A Python Unicode object, or NULL if an error occurs.
 */

    PyObject *unicode;
    unsigned char *str;

    if ((str = malloc(size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for deserialization");
        return NULL;
    }

    if (readBuffer(buf, str, size)) {
        free(str);
        return NULL;
    }

    // Decode our UTF8 encoded string into its unicode object representation
    if ((unicode = PyUnicode_DecodeUTF8((const char *) str, size, NULL)) == NULL) {
        free(str);
        return NULL;
    }

    return unicode;
}
