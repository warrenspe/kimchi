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

// Function Prototypes
int serialize(PyObject *, char **, Py_ssize_t *);
PyObject *deserialize(UserBuffer *);

// Note, we do not include utils/serialize.c here because it requires all of our serializers to be defined, while
// this file is included before them as they could in turn require access to the serialize and deserialize functions
// Thusly, utils/serialize.c is included near the end of headers/kimchi.h
