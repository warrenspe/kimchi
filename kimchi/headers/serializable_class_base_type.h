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
