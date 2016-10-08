/*
 *  float.c: A file containing functions used by kimchi.c to serialize floats.
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
int serializeFloat(PyObject *, char **, unsigned long long *);
PyObject *deserializeFloat(UserBuffer *);


int serializeFloat(PyObject *floatObj, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Float into a string.
 *
 * Inputs: floatObj - The PyFloat to serialize.
 *         buffer  - A pointer to a string to initialize and serialize integer to.
 *         size    - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    double val = PyFloat_AsDouble(floatObj);

    if ((*buffer = malloc(sizeof(val))) == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for serialization");
        return 1;
    }

    memcpy((void *) *buffer, (void *) &val, sizeof(val));
    *size = sizeof(val);

    return 0;
}

PyObject *deserializeFloat(UserBuffer *buf) {
/* Function which deserializes a string into a Python Float.
 *
 * Inputs: buf  - A UserBuffer containing the data to convert into a PyFloat.
 *
 * Outputs: A Python Float, or NULL if an error occurs.
 */

    double val = 0;

    readBuffer(buf, (unsigned char *) &val, sizeof(val));

    return PyFloat_FromDouble(val);
}
