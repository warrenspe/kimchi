/*
 *  headers.c: A file used to create the serialization headers for serialized values.
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

#include "headers/kimchi.h"


// Header constructing functions

unsigned long long numSizeHeaderBytes(unsigned long long size) {
/* Calculates the length of the header generated for the size field for a representation of the value `size`.
 *
 * Inputs: size - The number of bytes required to create a size header representing `size` bytes.
 *
 * Outputs: An integer representing the number of bytes needed for the size header desired,
 *          or 0 if the size is greater than can be fit into an unsigned long long.
 */

    unsigned long long upper = 128,
              length = 1;

    while (upper < size) {
        upper <<= 7; // Multiply by 128 (shift by 7; as we only use 7 out of the 8 bits)
        length += 1;

        if (length > MAX_SIZE_HEADER_LEN) {
            return 0;
        }
    }

    return length;
}


void writeSizeHeader(char *out, unsigned long long size) {
/* Function which writes size headers to a buffer.
 *
 * Note: No validation of arguments or buffer length is done.
 *
 * Inputs: out  - The buffer to write to.
 *         size - The value to serialize in the size header.
 */

    unsigned long long i = 0;

    do {
        *(out + i++) = ((size >> 7 == 0) << 7) | (size & 127);
        size >>= 7;
    } while (size);
}


int readSizeHeader(UserBuffer *buf, unsigned long long *size) {
/* Function which reads size headers from a UserBuffer.
 *
 * Inputs: buf  - The UserBuffer to read from.
 *         size - The value to initialize with the size from `buf`.
 *
 * Outputs: 0 on Success. > 0 on failure.
 */

    int shift = 0,
        i;
    unsigned char sizeByte;

    *size = 0;

    // We never serialize more than MAX_SIZE_HEADER_LEN bytes
    for (i = 0; i <= MAX_SIZE_HEADER_LEN; i++) {
        if (readBuffer(buf, &sizeByte, 1)) {
            return 1;
        }

        *size += (sizeByte & 127) << shift;

        // Check if this was the last byte of the size headers
        if (sizeByte & 128) {
            break;
        }

        shift += 7;
    }

    // We should never receive a size header with more than MAX_SIZE_HEADER_LEN bytes
    // If we do, something has corrupted our buffer.
    if (i > MAX_SIZE_HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "Size header corrupted");
        return 1;
    }

    return 0;
}

int constructHeaders(char **out, unsigned long long *headerLength, Py_ssize_t size, unsigned char type) {
/* Function which initializes and populates a buffer with headers for a serialized object.
 *
 * Inputs: out          - A pointer to a string to initialize and populate with our headers.
 *         headerLength - A pointer to an integer to populate with the number of bytes in our headers.
 *         size         - A value containing the size of the serialized body.
 *         type         - A char containing the type of value which is being serialized.
 *
 * Outputs: 0 on Success. > 0 on failure.
 */

    // Calculate how many bytes we need for the 'size' portion of the headers
    unsigned long long sizeBytes = numSizeHeaderBytes((unsigned long long) size);

    if (!sizeBytes) {
        PyErr_SetString(PyExc_OverflowError, "Size field overflow.");
        return 1;
    }

    // Update the long long passed to us, taking into account the Type byte
    *headerLength = sizeBytes + 1;

    *out = malloc(*headerLength);

    if (*out == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for parsed headers");
        return 1;
    }

    memset((void *) *out, '\0', *headerLength);

    // Write the type byte to the headers
    **out = type;

    // Write the size header to the buffer
    writeSizeHeader((*out) + 1, size);

    return 0;
}

// Header parsing functions
int parseHeaders(UserBuffer *buf, unsigned char *type, unsigned long long *size) {
/* Function which parses headers out of a UserBuffer.
 *
 * Inputs: buf  - A UserBuffer struct to parse headers out of.
 *         type - A pointer to a char to initialize with the type read from the buffer.
 *         size - A pointer to a long long to initialize with the size of the data to deserialize.
 *
 * Outputs: 0 on Success. > 0 on failure.
 */

    // Get the type; which is the first character of the headers
    if (readBuffer(buf, type, 1)) {
        return 1;
    }

    if (readSizeHeader(buf, size)) {
        return 1;
    }

    return 0;
}
