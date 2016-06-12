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
    #define LONG_TYPE 2
    #define STRING_TYPE 3
    #define UNICODE_TYPE 4
    #define LIST_TYPE 5
    #define TUPLE_TYPE 6
    #define DICT_TYPE 7


    // Standard Library Includes
    #include <math.h>
    #include <string.h>
    #include <stdlib.h>
    #include <Python.h>

    // Util Includes
    #include "headers/buffer.h"
    #include "headers/headers.h"
    #include "headers/types.h"

    // Serializer Includes
    #include "serializers/numeric.c"

    // Main C API Include
    #include "headers/serialize.h"


    // pump.c Function Prototypes
    PyObject *deflate(PyObject *self, PyObject *args);
    PyObject *inflate(PyObject *self, PyObject *args);
    void initpump(void);

#endif
