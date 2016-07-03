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
int serializeSet(PyObject *, char **, unsigned long long *);
PyObject *deserializeSet(UserBuffer *, unsigned char, unsigned long long);


int serializeSet(PyObject *set, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Set into a string.
 *
 * Inputs: dict   - The Python Set to serialize.
 *         buffer - A pointer to a string to initialize and serialize `set` to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

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

    return NULL;
}
