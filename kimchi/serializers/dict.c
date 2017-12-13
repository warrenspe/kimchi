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
int serializeDict(PyObject *, char **, unsigned long long *);
PyObject *deserializeDict(UserBuffer *);


int serializeDict(PyObject *dict, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Dict into a string.
 *
 * Inputs: dict   - The Python Dict to serialize.
 *         buffer - A pointer to a string to initialize and serialize `dict` to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    PyObject *items = PyDict_Items(dict);

    return serializeList(items, buffer, size);
}


PyObject *deserializeDict(UserBuffer *buf) {
/* Function which deserializes a string into a Python Dict.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a Dict.
 *
 * Outputs: A Python Dict, or NULL if an error occurs.
 */

    PyObject *dict,
             *items = deserializeList(buf);

    if (items == NULL) {
        return NULL;
    }

    dict = PyDict_New();

    if (PyDict_MergeFromSeq2(dict, items, 0) == -1) {
        Py_DECREF(items);
        Py_DECREF(dict);
        return NULL;
    }

    return dict;
}
