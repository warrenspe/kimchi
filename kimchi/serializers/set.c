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
static void _freeSerializeSetBuffers(char **, unsigned long long *, unsigned long long, PyObject *);
int serializeSet(PyObject *, char **, unsigned long long *);
PyObject *deserializeSet(UserBuffer *, unsigned char);


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


PyObject *deserializeSet(UserBuffer *buf, unsigned char type) {
/* Function which deserializes a string into a Python Set.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a Set.
 *         type - A char containing the type of object we're deserializing.
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
