/*
 *  bool.c: A file containing functions used by kimchi.c to serialize Python Booleans.
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
int serializeBool(char **, unsigned long long *);
PyObject *deserializeBool(unsigned char);


int serializeBool(char **buffer, unsigned long long *size) {
/* Function which serializes a Python Boolean into a string.
 *
 * Inputs: buffer - A pointer to a string where we would otherwise be serializing the Boolean to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0.
 */

    *buffer = NULL;
    *size = 0;
    return 0;
}


PyObject *deserializeBool(unsigned char type) {
/* Function which returns the appropriate Python Boolean given the type.
 *
 * Inputs: type - A char containing the type of object we're deserializing.
 *
 * Outputs: A Python Boolean.
 */

    if (type == BOOL_TRUE_TYPE) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}
