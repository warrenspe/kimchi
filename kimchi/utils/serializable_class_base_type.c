/*
 *  class_base_type.c: A file defining an abstract base class which can be subclassed in python to serialize classes.
 *  Copyright (C) 2017 Warren Spencer warrenspencer27@gmail.com

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

static int SerializableInit(PyObject *self, PyObject *args, PyObject *kwds) {
    // Do not allow instances of Serializable to be created
    if (Py_TYPE(self) == &SerializableType) {
        PyErr_Format(PyExc_TypeError, "Cannot create non-derived instances of kimchi.Serializable.");
        return -1;
    }

    return 0;
}

void SerializableDeInit(Serializable *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}
