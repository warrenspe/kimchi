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
int serializeUnicode(PyObject *, char **, unsigned long long *);
PyObject *deserializeUnicode(UserBuffer *, unsigned long long);


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

PyObject *deserializeUnicode(UserBuffer *buf, unsigned long long size) {
/* Function which deserializes a utf8 encoded bytestring into a PyUnicode.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyUnicode.
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
