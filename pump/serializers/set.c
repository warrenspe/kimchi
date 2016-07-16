/*
 *  set.c: A file containing functions used by pump.c to serialize Python Sets and FrozenSets.
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
static void _freeSerializeSetBuffers(char **, unsigned long long *, unsigned long long, PyObject *);
int serializeSet(PyObject *, char **, unsigned long long *);
PyObject *deserializeSet(UserBuffer *, unsigned char, unsigned long long);


static void _freeSerializeSetBuffers(char **serializations, unsigned long long *sizes,
                                     unsigned long long numItems, PyObject *iter) {
/* Function which frees several buffers used when serializing a Python Set.
 *
 * Inputs: serializations - A pointer to a block of memory used to hold pointers to buffers recording each item's
 *                          serializations.
 *         sizes          - A pointer to an block of memory used to record the sizes of each item's serialization.
 *         numItems       - A value containing the number of items in serializations to free.
 *         iter           - A pointer to a PyObject iterator over our set.
 */

    unsigned long long i;

    for (i = 0; i < numItems; i++) {
        free(*(serializations + i));
    }
    free(serializations);
    free(sizes);
    Py_DECREF(iter);
}


int serializeSet(PyObject *set, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Set into a string.
 *
 * Inputs: dict   - The Python Set to serialize.
 *         buffer - A pointer to a string to initialize and serialize `set` to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    PyObject *iter = NULL,
             *item;
    unsigned long long numItems = PySet_GET_SIZE(set),
                       bufferOffset,
                       i = 0,
                       *sizes = NULL;
    char **serializations = NULL;

    *size = 0;

    // Get an iterator over the items in the set
    if ((iter = PyObject_GetIter(set)) == NULL) {
        return 1;
    }

    // Create an array of strings containing serializations of the objects in the set
    if ((serializations = calloc(numItems, sizeof(char *))) == NULL) {
        _freeSerializeSetBuffers(serializations, sizes, numItems, iter);
        return 1;
    }

    // Create an array of sizes of the objects to serialize
    if ((sizes = calloc(numItems, sizeof(unsigned long long *))) == NULL) {
        _freeSerializeSetBuffers(serializations, sizes, numItems, iter);
        return 1;
    }

    // Serialize each item
    while ((item = PyIter_Next(iter)) != NULL) {
        if (serialize(item, (serializations + i), (Py_ssize_t *) (sizes + i))) {
            _freeSerializeSetBuffers(serializations, sizes, numItems, iter);
            return 1;
        }

        *size += *(sizes + i);
        i += 1;
    }

    // Add the number of bytes required to create a serialized version of the number of items in the set
    bufferOffset = numSizeHeaderBytes(numItems);
    *size += bufferOffset;

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        _freeSerializeSetBuffers(serializations, sizes, numItems, iter);
        return 1;
    }

    // Write the num items to the buffer
    writeSizeHeader(*buffer, numItems);

    // Write each serialization to buffer
    for (i = 0; i < numItems; i++) {
        memcpy((void *) ((*buffer) + bufferOffset), *(serializations + i), *(sizes + i));
        bufferOffset += *(sizes + i);
    }

    _freeSerializeSetBuffers(serializations, sizes, numItems, iter);

    return 0;
}


PyObject *deserializeSet(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a string into a Python Set.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a Set.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the items of the Set.
 *
 * Outputs: A Python Set, or NULL if an error occurs.
 */

    PyObject *tuple, // We will construct our set by first constructing a tuple of objects
             *set = NULL,
             *item;
    unsigned long long numItems,
                       i;

    // Get the number of items in our iterable
    if (readSizeHeader(buf, &numItems)) {
        return NULL;
    }

    // Construct a tuple of the serialized objects
    if ((tuple = PyTuple_New(numItems)) == NULL) {
        return NULL;
    }
    for (i = 0; i < numItems; i++) {
        if ((item = deserialize(buf)) == NULL) {
            Py_DECREF(tuple);
            return NULL;
        }
        PyTuple_SET_ITEM(tuple, i, item);
    }

    // Construct our set or frozenset
    if (type == SET_TYPE) {
        set = PySet_New(tuple);
    } else if (type == FROZEN_SET_TYPE) {
        set = PyFrozenSet_New(tuple);
    }

    if (set == NULL) {
        Py_DECREF(tuple);
        return NULL;
    }

    return set;
}
