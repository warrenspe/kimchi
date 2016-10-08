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

    // We assume we will never have to serialize an object which requires more storage than an unsigned long long can
    // provide.  Since an unsigned long long can represent values up to 2**64, and 128**9 < 2**64 < 128**10, 9 bytes is
    // the maximum length of the header.  (Note 128 instead of 256, as we only use 7 bits of a byte).
    #define MAX_SIZE_HEADER_LEN 9

    // Function Prototypes
    unsigned long long numSizeHeaderBytes(unsigned long long);
    void writeSizeHeader(char *, unsigned long long);
    int readSizeHeader(UserBuffer *, unsigned long long *);
    int constructHeaders(char **, unsigned long long *, Py_ssize_t, unsigned char);
    int parseHeaders(UserBuffer *, unsigned char *, unsigned long long *);

    // Includes
    #include "utils/headers.c"

#endif
