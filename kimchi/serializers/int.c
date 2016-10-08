/*
 *  int.c: A file containing functions used by kimchi.c to serialize integers.
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
static unsigned long long _numBytesToSerialize(unsigned long long);
static void _writeBytesToBuffer(unsigned long long, char *, unsigned long long);
int serializeInt(PyObject *, unsigned char, char **, unsigned long long *);
PyObject *deserializeInt(UserBuffer *, unsigned char, unsigned long long);


static unsigned long long _numBytesToSerialize(unsigned long long toSerialize) {
/* Function which calculates the number of bytes required to serialize this value.
 *
 * Inputs: toSerialize - The value we will be determining the size of the serialization of.
 *
 * Outputs: The number of bytes required to serialize `toSerialize`.
 */

    long long counter = 0;

    while (toSerialize) {
        toSerialize >>= 8;
        counter++;
    }

    return counter;
}

static void _writeBytesToBuffer(unsigned long long toSerialize, char *buffer, unsigned long long size) {
/* Function which writes `size` bytes to `buffer`, from `toSerialize`.
 *
 * Inputs: toSerialize - The value we're serializing to buffer.
 *         buffer      - The buffer we're serializing toSerialize to.
 *         size        - The number of bytes we need to serialize into buffer.
 */

    unsigned long long i;

    for (i = 0; i < size; i++) {
        *(buffer + i) = toSerialize & 255;
        toSerialize >>= 8;
    }
}


int serializeInt(PyObject *integer, unsigned char type, char **buffer, unsigned long long *size) {
/* Function which serializes a Python int into a string.
 *
 * Inputs: integer - The PyInt to serialize.
 *         type    - The type of integer to serialize (positive / negative).
 *         buffer  - A pointer to a string to initialize and serialize integer to.
 *         size    - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    unsigned long long toSerialize = (unsigned long long) PyLong_AsLongLong(integer);

    // If we've detected that we're serializing a negative integer, reverse the polarity on it (as we work unsigned)
    if (type == NEG_INT_TYPE) {
        toSerialize = -toSerialize;
    }

    if (PyErr_Occurred() != NULL) {
        return 1;
    }

    *size = _numBytesToSerialize(toSerialize);

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    memset((void *) *buffer, '\0', *size);
    _writeBytesToBuffer(toSerialize, *buffer, *size);

    return 0;
}

PyObject *deserializeInt(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a string into a Python int.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyInt.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the PyInt.
 *
 * Outputs: A Python int, or NULL if an error occurs.
 */

    unsigned long long val = 0;
    unsigned int shift = 0;
    unsigned char byte;
    unsigned long long i;

    for (i = 0; i < size; i++) {
        if (readBuffer(buf, &byte, 1)) {
            return NULL;
        }

        val |= (unsigned long long) byte << shift;

        shift += 8;
    }

    if (type == NEG_INT_TYPE) {
        val = -val;
    }

    return PyInt_FromLong((long) val);
}
