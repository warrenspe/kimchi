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

#ifndef __HEADERS_H
#define __HEADERS_H 1

    // Function Prototypes
    static long long _determineSizeBytes(Py_ssize_t size);
    static void _writeHeaders(char *out, Py_ssize_t size, int sizeBytes, unsigned char type);
    int constructHeaders(char **out, unsigned long long *headerLength, Py_ssize_t size, unsigned char type);
    int parseHeaders(UserBuffer *buffer, unsigned char *type, unsigned long long *size);

    // Includes
    #include "utils/headers.c"

#endif
