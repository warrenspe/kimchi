/*
 *  numeric.c: A file containing functions used by pump.c to serialize numeric values.
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
static long long _numBytesToSerialize(unsigned long long toSerialize);
static void _writeBytesToBuffer(unsigned long long toSerialize, char *buffer, long long size);
int serializeNumeric(PyObject *numeric, char **buffer, long long *size);
PyObject *deserializeNumeric(UserBuffer *buf, unsigned char type, long long size);


static long long _numBytesToSerialize(unsigned long long toSerialize) {
/* Function which calculates the number of bytes required to serialize this value.
 *
 * Inputs: toSerialize: The value we will be determining the size of the serialization of.
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

static void _writeBytesToBuffer(unsigned long long toSerialize, char *buffer, long long size) {
/* Function which writes `size` bytes to `buffer`, from `toSerialize`.
 *
 * Inputs: toSerialize: The value we're serializing to buffer.
 *         buffer: The buffer we're serializing toSerialize to.
 *         size: The number of bytes we need to serialize into buffer.
 */

    for (int i = 0; i < size; i++) {
        *(buffer + i) = toSerialize & 255;
        toSerialize >>= 8;
    }
}


int serializeNumeric(PyObject *numeric, char **buffer, long long *size) {
/* Function which serializes a Python INT or LONG into a string.
 *
 * Inputs: numeric: The PyInt or PyLong to serialize.
 *         buffer: A pointer to a string to initialize and serialize numeric to.
 *         size: A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on Failure.
 */

    unsigned long long toSerialize = (unsigned long long) PyLong_AsLongLong(numeric);

    if (PyErr_Occurred() != NULL) {
        return 1;
    }

    *size = _numBytesToSerialize(toSerialize);

    *buffer = malloc(*size);
    memset((void *) *buffer, '\0', *size);
    _writeBytesToBuffer(toSerialize, *buffer, *size);

    return 0;
}

PyObject *deserializeNumeric(UserBuffer *buf, unsigned char type, long long size) {
/* Function which deserializes a string into a Python INT or Long.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyInt / PyLong.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the PyInt / PyLong.
 *
 * Outputs: A Python INT / LONG.
 */

    unsigned long long val = 0;
    unsigned int shift = 0;
    unsigned char *byte;

    for (long long i = 0; i < size; i++) {
        byte = (unsigned char *) readBuffer(buf, 1);

        if (byte == NULL) {
            return NULL; //TODO error?
        }

        val |= (unsigned long long) (*byte) << shift;

        shift += 8;
    }

    if (type == LONG_TYPE) {
        return PyLong_FromLongLong((long long) val);
    }

    return PyInt_FromLong((long) val);
}
