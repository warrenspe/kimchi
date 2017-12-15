/*
 * MIT License
 *
 * Copyright (c) 2017 Warren Spencer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef __KIMCHI_H
#define __KIMCHI_H 1

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
    #define BOOL_TRUE_TYPE 14
    #define BOOL_FALSE_TYPE 15
    #define INSTANCE_TYPE 16


    // Standard Library Includes
    #include <Python.h>
    #include "structmember.h"
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>

    // Util Includes
    #include "headers/buffer.h"
    #include "headers/headers.h"
    #include "headers/types.h"
    #include "headers/serializable_class_base_type.h"
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
    #include "serializers/bool.c"
    #include "serializers/float.c"

    // Include the definitions of serialize and deserialize. Done seperately as this requires the serializers
    // above to be defined, which in turn can use the functions this file offers
    #include "utils/serialize.c"


    // kimchi.c Function Prototypes
    static PyObject *dumps(PyObject *, PyObject *);
    static PyObject *loads(PyObject *, PyObject *);
    #if PY_MAJOR_VERSION == 2
    void initkimchi(void);
    #else
    PyMODINIT_FUNC PyInit_kimchi(void);
    #endif

#endif
