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

// Function Prototypes
static PyObject *_loadClass(PyObject *, PyObject *);




static PyObject *_loadClass(PyObject *moduleName, PyObject *className)
{
    PyObject *global = 0, *module;

    module = PySys_GetObject("modules");
    if (module == NULL)
        return NULL;

    module = PyDict_GetItem(module, py_module_name);
    if (module == NULL) {
        module = PyImport_Import(py_module_name);
        if (!module)
            return NULL;
        global = PyObject_GetAttr(module, py_global_name);
        Py_DECREF(module);
    }
    else
        global = PyObject_GetAttr(module, py_global_name);
    return global;
}


PyImport_ImportModule



int serializeClassInstance(PyObject *instance, char **buffer, unsigned long long *size) {
/* Function which serializes a Python Class instance into a string.
 *
 * Inputs: instance - The Python Class instance to serialize.
 *         buffer   - A pointer to a string to initialize and serialize `instance` to.
 *         size     - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    
    // get module name
    // get class name
    // create tuple (module, class, dict) (maybe just put strstrdict into a string)
    // create dictionary
    // iterate through members of instance's _serialize attribute

    
}

PyObject *deserializeClassInstance(UserBuffer *buf) {
/* Function which deserializes a string into a Python Class instance
 *
 * Inputs: buf - A UserBuffer containing the data to convert into an instance.
 *
 */

    // get module name
    // get class name
    // create instance of class
    // iterate through dictionary, setting attributes from dict items
}
