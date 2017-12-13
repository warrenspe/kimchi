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
static void _freeSerializeTupleBuffers(char **, unsigned long long *, unsigned long long);
int serializeTuple(PyObject *, char **, unsigned long long *);
PyObject *deserializeTuple(UserBuffer *);


static void _freeSerializeTupleBuffers(char **serializations, unsigned long long *sizes, unsigned long long numItems){
/* Function which frees several buffers used when serializing a Python Tuple.
 *
 * Inputs: serializations - A pointer to a block of memory used to hold pointers to buffers recording each item's
 *                          serializations.
 *         sizes          - A pointer to an block of memory used to record the sizes of each item's serialization.
 *         numItems       - A value containing the number of items in serializations to free.
 */

    unsigned long long i;

    for (i = 0; i < numItems; i++) {
        free(*(serializations + i));
    }
    free(serializations);
    free(sizes);
}


int serializeTuple(PyObject *tuple, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Tuple into a string.
 *
 * Inputs: tuple  - The Python Tuple to serialize.
 *         buffer - A pointer to a string to initialize and serialize `tuple` to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    PyObject *item;
    char **serializations = NULL;
    unsigned long long *sizes = NULL,
                       bufferOffset,
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

        if (serialize(item, (serializations + i), (Py_ssize_t *) (sizes + i))) {
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

PyObject *deserializeTuple(UserBuffer *buf) {
/* Function which deserializes a string into a Python Tuple.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a tuple.
 *
 * Outputs: A Python Tuple, or NULL if an error occurs.
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
        if ((item = deserialize(buf)) == NULL) {
            Py_DECREF(tuple);
            return NULL;
        }
        PyTuple_SET_ITEM(tuple, i, item);
    }

    return tuple;
}
