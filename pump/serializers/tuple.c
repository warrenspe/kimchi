/*
 *  tuple.c: A file containing functions used by pump.c to serialize Python Tuples.
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
static void _freeSerializeTupleBuffers(char **serializations, Py_ssize_t *sizes, unsigned long long numItems);
int serializeSequence(PyObject *sequence, char **buffer, unsigned long long *size);
PyObject *deserializeSequence(UserBuffer *buf, unsigned char type, unsigned long long size);


static void _freeSerializeTupleBuffers(char **serializations, Py_ssize_t *sizes, unsigned long long numItems){
    unsigned long long i;

    for (i = 0; i < numItems; i++) {
        free(*(serializations + i));
    }
    free(serializations);
    free(sizes);
}

int serializeTuple(PyObject *tuple, char **buffer, unsigned long long *size) {
/* Function which serializes a Python tuple into a string.
 *
 * Inputs: tuple: The Python tuple to serialize.
 *         buffer: A pointer to a string to initialize and serialize tuple to.
 *         size: A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on Failure.
 */

    Py_ssize_t *sizes = NULL;
    PyObject *item;
    char **serializations = NULL;
    unsigned long long bufferOffset,
                       i,
                       numItems;

    numItems = PyTuple_Size(tuple);
    *size = 0;

    // Create an array of strings containing serializations of the objects in the tuple
    if ((serializations = calloc(numItems, sizeof(char *))) == NULL) {
        return 1;
    }

    // Create an array of sizes of the objects to serialize
    if ((sizes = calloc(numItems, sizeof(unsigned long long *))) == NULL) {
        _freeSerializeTupleBuffers(serializations, sizes, 0);
        return 1;
    }

    // Serialize each item
    for (i = 0; i < numItems; i++) {
        item = PyTuple_GET_ITEM(tuple, i);

        if (serialize(item, (serializations + i), (sizes + i))) {
            _freeSerializeTupleBuffers(serializations, sizes, i);
            return 1;
        }

        *size += *(sizes + i);
    }

    // Add the number of bytes required to create a serialized version of the number of items in the tuple
    bufferOffset = numSizeHeaderBytes(numItems);
    *size += bufferOffset;

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        _freeSerializeTupleBuffers(serializations, sizes, (*size));
        return 1;
    }

    // Write the num items to the buffer
    writeSizeHeader(*buffer, numItems);

    // Write each serialization to buffer
    for (i = 0; i < numItems; i++) {
        memcpy((void *) ((*buffer) + bufferOffset), *(serializations + i), *(sizes + i));
        bufferOffset += *(sizes + i);
    }

    _freeSerializeTupleBuffers(serializations, sizes, numItems);

    return 0;
}

PyObject *deserializeTuple(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a tuple into a Python Tuple.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a Tuple.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the items of the Tuple.
 *
 * Outputs: A Python Tuple.
 */

    PyObject *tuple;
    PyObject *item;
    unsigned long long numItems,
                       i;

    if (readSizeHeader(buf, &numItems)) {
        return NULL;
    }

    if ((tuple = PyTuple_New(numItems)) == NULL) {
        return NULL;
    }

    for (i = 0; i < numItems; i++) {
        item = deserialize(buf);
        if (item == NULL || PyTuple_SetItem(tuple, i, item) == -1) {
            _PyTuple_Resize(&tuple, 0);
            Py_DECREF(tuple);
            return NULL;
        }
        Py_INCREF(item); // TODO check refcount of item here, see if we need dis
    }

    return tuple;
}
