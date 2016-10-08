/*
 *  dict.c: A file containing functions used by kimchi.c to serialize Python Dictionaries.
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
