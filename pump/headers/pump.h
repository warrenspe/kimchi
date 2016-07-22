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
    #define BYTES_TYPE 5
    #define UNICODE_TYPE 6
    #define LIST_TYPE 7
    #define TUPLE_TYPE 8
    #define DICT_TYPE 9
    #define SET_TYPE 10
    #define FROZEN_SET_TYPE 11
    #define NONE_TYPE 12
    #define FLOAT_TYPE 13


    // Standard Library Includes
    #include <Python.h>
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>

    // Util Includes
    #include "headers/buffer.h"
    #include "headers/headers.h"
    #include "headers/types.h"
    // Include the prototype of serialize and deserialize for use in serializers but not the actual function definitions
    #include "headers/serialize.h"

    // Serializer Includes

    // Python 2 specific serializers
    #if PY_MAJOR_VERSION == 2
        #include "serializers/int.c"
    #endif

    #include "serializers/long.c"
    #include "serializers/bytes.c"
    #include "serializers/unicode.c"
    #include "serializers/tuple.c"
    #include "serializers/list.c"
    #include "serializers/dict.c"
    #include "serializers/set.c"
    #include "serializers/none.c"
    #include "serializers/float.c"

    // Include the definitions of serialize and deserialize. Done seperately as this requires the serializers
    // above to be defined, which in turn can use the functions this file offers
    #include "utils/serialize.c"


    // pump.c Function Prototypes
    static PyObject *deflate(PyObject *, PyObject *);
    static PyObject *inflate(PyObject *, PyObject *);
    #if PY_MAJOR_VERSION == 2
    void initpump(void);
    #else
    PyMODINIT_FUNC PyInit_pump(void);
    #endif

#endif
