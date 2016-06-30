/*
 *  long.c: A file containing functions used by pump.c to serialize longs.
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
#include "longintrepr.h"

// Function Prototypes
int serializeLong(PyObject *longObj, char **buffer, unsigned long long *size);
PyObject *deserializeLong(UserBuffer *buf, unsigned char type, unsigned long long size);


int serializeLong(PyObject *longObj, char **buffer, unsigned long long *size) {
/* Function which serializes a Python long into a string.
 *
 * Inputs: longObj: The PyLong to serialize.
 *         buffer: A pointer to a string to initialize and serialize integer to.
 *         size: A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on Failure.
 */

    PyLongObject *pyLong = (PyLongObject *) longObj;
    unsigned int i;

    *size = pyLong->ob_size * sizeof(digit);

    if ((*buffer = malloc(*size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    // Write each data byte to the buffer
    for (i = 0; i * 4 < *size; i++) {
        memcpy((void *) (*buffer + (i * sizeof(digit))), (void *) (pyLong->ob_digit + i), sizeof(digit));
    }

    return 0;
}

PyObject *deserializeLong(UserBuffer *buf, unsigned char type, unsigned long long size) {
/* Function which deserializes a string into a Python long.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyLong.
 *         type - A char containing the type of object we're deserializing.
 *         size - The number of bytes to use in constructing the PyLong.
 *
 * Outputs: A Python long.
 */

    PyLongObject *pyLong = _PyLong_New((Py_ssize_t) size / sizeof(digit));
    unsigned char *bytes;

    if ((bytes = malloc(size)) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for deserialization");
        return NULL;
    }

    if (readBuffer(buf, (unsigned char *) (pyLong->ob_digit), size)) {
        return NULL;
    }

    return (PyObject *) pyLong;
}
