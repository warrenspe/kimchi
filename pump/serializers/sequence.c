/*
 *  sequence.c: A file containing functions used by pump.c to serialize sequential values, like lists and tuples.
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
void _freeSerializeBuffers(char **serializations, Py_ssize_t *sizes, unsigned long long numItems);
int serializeSequence(PyObject *sequence, char **buffer, unsigned long long *size);
PyObject *deserializeSequence(UserBuffer *buf, unsigned char type, unsigned long long size);


void _freeSerializeBuffers(char **serializations, Py_ssize_t *sizes, unsigned long long numItems){
    unsigned long long i;

    for (i = 0; i < numItems; i++) {
        free(*(serializations + i));
    }
    free(serializations);
    free(sizes);
}

int serializeSequence(PyObject *sequence, char **buffer, unsigned long long *size) {
/* Function which serializes a Python list or tuple into a string.
 *
 * Inputs: sequence: The Python list or tuple to serialize.
 *         buffer: A pointer to a string to initialize and serialize sequence to.
 *         size: A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on Failure.
 */

    unsigned long long numItems,
                        tmp,
                        i;
    unsigned char type;
    char **serializations = NULL;
    Py_ssize_t *sizes = NULL;
    PyObject *item;

    type = getType(sequence);
    *size = 0;

    if (type == TUPLE_TYPE) {
        numItems = PyTuple_Size(sequence);
    } else {
        numItems = PyList_Size(sequence);
    }

    // Create an array of strings containing serializations of the objets in sequence
    if ((serializations = calloc(numItems, sizeof(char *))) == NULL) {
        return 1;
    }

    // Create an array of sizes of the objects to serialize
    if ((sizes = calloc(numItems, sizeof(unsigned long long *))) == NULL) {
        _freeSerializeBuffers(serializations, sizes, 0);
        return 1;
    }

    // Serialize each item
    for (i = 0; i < numItems; i++) {
        if (type == TUPLE_TYPE) {
            item = PyTuple_GET_ITEM(sequence, i);
        } else {
            item = PyList_GET_ITEM(sequence, i);
        }

        if (serialize(item, (serializations + i), (sizes + i))) {
            _freeSerializeBuffers(serializations, sizes, i);
            return 1;
        }

        *size += *(sizes + i);
    }

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        _freeSerializeBuffers(serializations, sizes, numItems);
        return 1;
    }
    memset((void *) *buffer, '\0', *size);

    // Write each serialization to buffer
    tmp = 0;
    for (i = 0; i < numItems; i++) {
        memcpy((void *) ((*buffer) + tmp), serializations + i, *(sizes + i));
        tmp += *(sizes + i);
    }

    _freeSerializeBuffers(serializations, sizes, numItems);

    return 0;
}

PyObject *deserializeSequence(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a sequence into a Python List or Tuple.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a List / Tuple.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the items of the List / Tuple.
 *
 * Outputs: A Python List / Tuple.
 */

    return NULL;
}
