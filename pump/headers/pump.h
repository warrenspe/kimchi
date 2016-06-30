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

#ifndef __PUMP_H
#define __PUMP_H 1

    ///
    // Serializable Type Definitions
    ///
    #define INT_TYPE 1
    #define NEG_INT_TYPE 2
    #define LONG_TYPE 3
    #define NEG_LONG_TYPE 4
    #define STRING_TYPE 5
    #define UNICODE_TYPE 6
    #define LIST_TYPE 7
    #define TUPLE_TYPE 8
    #define DICT_TYPE 9


    // Standard Library Includes
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>
    #include <Python.h>

    // Util Includes
    #include "headers/buffer.h"
    #include "headers/headers.h"
    #include "headers/types.h"
    // Include the definition of serialize and deserialize for use in serializers
    #define __SERIALIZE_HEADERS_ONLY 1
    #include "headers/serialize.h"
    #undef __SERIALIZE_HEADERS_ONLY

    // Serializer Includes
    #include "serializers/int.c"
    #include "serializers/long.c"
    #include "serializers/string.c"
    #include "serializers/sequence.c"

    // Main C API Include
    #include "headers/serialize.h"


    // pump.c Function Prototypes
    PyObject *deflate(PyObject *self, PyObject *args);
    PyObject *inflate(PyObject *self, PyObject *args);
    void initpump(void);

#endif
