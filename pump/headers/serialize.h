/*
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

#ifndef __SERIALIZE_H

    // Function Prototypes
    int serialize(PyObject *object, char **out, Py_ssize_t *outSize);
    PyObject *deserialize(UserBuffer *buf);

    // Includes

    // The first time we are included, we just want the function definitions.
    // This allows for serializers to use our serialize/deserialize functions.
    // The next time we are included include the definitions for these functions
    #ifndef __SERIALIZE_HEADERS_ONLY
        #define __SERIALIZE_H 1
        #include "utils/serialize.c"
    #endif

#endif
