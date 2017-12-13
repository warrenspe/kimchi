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
