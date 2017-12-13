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

#include "headers/kimchi.h"
#include "longintrepr.h"


unsigned char getType(PyObject *obj) {
/* Function which determines what type a given python object is (see kimchi.h for type definitions).
 *
 * Inputs: obj - The python object to determine the type of.
 *
 * Outputs: A positive integer (in a char) on successful detection of the type of obj.
 *          0 if unable to determine the type of obj.
 */

    // Python 2 specific types
    #if PY_MAJOR_VERSION == 2
        if (PyInt_CheckExact(obj)) {
            if (PyLong_AsLongLong(obj) < 0)
                return NEG_INT_TYPE;
            return INT_TYPE;
        }

    #endif

    if (PyLong_CheckExact(obj)) {
        if (Py_SIZE(obj) < 0)
            return NEG_LONG_TYPE;
        return LONG_TYPE;
    }

    if (PyFloat_CheckExact(obj)) {
        return FLOAT_TYPE;
    }

    if (PyBytes_Check(obj)) {
        return BYTES_TYPE;
    }

    if (PyUnicode_Check(obj)) {
        return UNICODE_TYPE;
    }

    if (PyList_CheckExact(obj)) {
        return LIST_TYPE;
    }

    if (PyTuple_CheckExact(obj)) {
        return TUPLE_TYPE;
    }

    if (PyDict_CheckExact(obj)) {
        return DICT_TYPE;
    }

    if (obj == Py_None) {
        return NONE_TYPE;
    }

    if (PyBool_Check(obj)) {
        if (Py_True == obj)
            return BOOL_TRUE_TYPE;
        return BOOL_FALSE_TYPE;
    }

    if (PyAnySet_CheckExact(obj)) {
        if (PyFrozenSet_CheckExact(obj))
            return FROZEN_SET_TYPE;
        return SET_TYPE;
    }

    PyErr_Format(PyExc_TypeError, "Unknown object type \"%.400s\"", Py_TYPE(obj)->tp_name);
    return 0;
}
