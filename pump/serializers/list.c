/*
 *  list.c: A file containing functions used by pump.c to serialize Python Lists.
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

// Function Prototypes
static void _freeSerializeListBuffers(char **, Py_ssize_t *, unsigned long long);
int serializeList(PyObject *, char **, unsigned long long *);
PyObject *deserializeList(UserBuffer *, unsigned char, unsigned long long);


static void _freeSerializeListBuffers(char **serializations, Py_ssize_t *sizes, unsigned long long numItems) {
    unsigned long long i;

    for (i = 0; i < numItems; i++) {
        free(*(serializations + i));
    }
    free(serializations);
    free(sizes);
}

int serializeList(PyObject *list, char **buffer, unsigned long long *size) {
/* Function which serializes a Python list into a string.
 *
 * Inputs: list: The Python list to serialize.
 *         buffer: A pointer to a string to initialize and serialize `list` to.
 *         size: A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    Py_ssize_t *sizes = NULL;
    PyObject *item;
    char **serializations = NULL;
    unsigned long long bufferOffset,
                       i,
                       numItems;

    numItems = PyList_Size(list);
    *size = 0;

    // Create an array of strings containing serializations of the objects in the list
    if ((serializations = calloc(numItems, sizeof(char *))) == NULL) {
        return 1;
    }

    // Create an array of sizes of the objects to serialize
    if ((sizes = calloc(numItems, sizeof(unsigned long long *))) == NULL) {
        _freeSerializeListBuffers(serializations, sizes, 0);
        return 1;
    }

    // Serialize each item
    for (i = 0; i < numItems; i++) {
        item = PyList_GetItem(list, i);

        if (serialize(item, (serializations + i), (sizes + i))) {
            _freeSerializeListBuffers(serializations, sizes, i);
            return 1;
        }

        *size += *(sizes + i);
    }

    // Add the number of bytes required to create a serialized version of the number of items in the list
    bufferOffset = numSizeHeaderBytes(numItems);
    *size += bufferOffset;

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        _freeSerializeListBuffers(serializations, sizes, (*size));
        return 1;
    }

    // Write the num items to the buffer
    writeSizeHeader(*buffer, numItems);

    // Write each serialization to buffer
    for (i = 0; i < numItems; i++) {
        memcpy((void *) ((*buffer) + bufferOffset), *(serializations + i), *(sizes + i));
        bufferOffset += *(sizes + i);
    }

    _freeSerializeListBuffers(serializations, sizes, numItems);

    return 0;
}

PyObject *deserializeList(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a string into a Python List.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a list.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the items of the list.
 *
 * Outputs: A Python List.
 */

    PyObject *list;
    PyObject *item;
    unsigned long long numItems,
                       i;

    if (readSizeHeader(buf, &numItems)) {
        return NULL;
    }

    if ((list = PyList_New(numItems)) == NULL) {
        return NULL;
    }

    for (i = 0; i < numItems; i++) {
        if ((item = deserialize(buf)) == NULL) {
            Py_DECREF(list);
            return NULL;
        }
        PyList_SET_ITEM(list, i, item);
    }

    return list;
}
