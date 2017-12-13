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

#ifndef __CLASS_BASE_TYPE_H
#define __CLASS_BASE_TYPE_H 1

    typedef struct {
        PyObject_HEAD

    } Serializable;

    // Function Prototypes
    static int SerializableInit(PyObject *, PyObject *, PyObject *);
    void SerializableDeInit(Serializable *);

    // Class Definition
    static PyMemberDef SerializableMembers[] = {
        {NULL} // Sentinel
    };

    static PyMethodDef SerializableMethods[] = {
        {NULL} // Sentinel
    };

    static PyTypeObject SerializableType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "kimchi.Serializable",
        sizeof(Serializable),
        0,                         /*tp_itemsize*/
        (destructor)SerializableDeInit,/*tp_dealloc*/
        0,                         /*tp_print*/
        0,                         /*tp_getattr*/
        0,                         /*tp_setattr*/
        0,                         /*tp_compare*/
        0,                         /*tp_repr*/
        0,                         /*tp_as_number*/
        0,                         /*tp_as_sequence*/
        0,                         /*tp_as_mapping*/
        0,                         /*tp_hash */
        0,                         /*tp_call*/
        0,                         /*tp_str*/
        0,                         /*tp_getattro*/
        0,                         /*tp_setattro*/
        0,                         /*tp_as_buffer*/
        Py_TPFLAGS_HAVE_CLASS | Py_TPFLAGS_BASETYPE,       /*tp_flags*/
        "Base Class for kimchi serializable types.",
        0,                         /* tp_traverse */
        0,                         /* tp_clear */
        0,                         /* tp_richcompare */
        0,                         /* tp_weaklistoffset */
        0,                         /* tp_iter */
        0,                         /* tp_iternext */
        SerializableMethods,       /* tp_methods */
        SerializableMembers,       /* tp_members */
        0,                         /* tp_getset */
        0,                         /* tp_base */
        0,                         /* tp_dict */
        0,                         /* tp_descr_get */
        0,                         /* tp_descr_set */
        0,                         /* tp_dictoffset */
        (initproc)SerializableInit,/* tp_init */
        0,                         /* tp_alloc */
        PyType_GenericNew,         /* tp_new */
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    };

    // Includes
    #include "utils/serializable_class_base_type.c"

#endif
