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

#include "headers/pump.h"


// Header constructing functions

static long long _determineSizeBytes(Py_ssize_t size) {
/* Calculates the length of the header field for a given serialization requiring size bytes.
 *
 * Inputs: size - The size of the header for the serialization in bytes.
 *
 * Outputs: An integer representing the number of bytes needed for the size prefix of the serialization,
 *          or 0 if the size of the serialized object is greater than we can fit into a long long.
 */

    long long upper = 128,
              length = 1;

    while (upper < size) {
        upper <<= 7; // Multiply by 128 (shift by 7; as we only use 7 out of the 8 bits)
        length += 1;

        // If length is >= 10 we've hit an integer overflow; as long long's are at minimum 64 bits wide;
        // and 128**10 > 2**64
        if (length >= 10) {
            return 0;
        }
    }

    return length;
}


static void _writeHeaders(char *out, Py_ssize_t size, int sizeBytes, unsigned char type) {
/* Function which writes the type and size headers to a buffer.
 *
 * Inputs: out: The buffer to write to.
 *         size: The value to write for the size portion of the headers; detailing the size of the serialized body.
 *         sizeBytes: The number of bytes to serialize from size.
 *         type: A char, containing the value to serialize for the type portion of the headers.
 */

    // Write the type byte to the headers
    *out = type;

    // Write the size bytes to the header
    for (int i = 1; i <= sizeBytes; i++) {
        *(out + i) = ((i == sizeBytes) << 7) | (size & 127);
        size >>= 7;
    }
}


int constructHeaders(char **out, long long *headerLength, Py_ssize_t size, unsigned char type) {
/* Function which initializes and populates a buffer with headers for a serialized object.
 *
 * Inputs: out: A pointer to a string to initialize and populate with our headers.
 *         headerLength: A pointer to an integer to populate with the number of bytes in our headers.
 *         size: A value containing the size of the serialized body.
 *         type: A char containing the type of value which is being serialized.
 *
 * Outputs: 0 on Success. > 0 on failure.
 */

    // Calculate how many bytes we need for the 'size' portion of the headers
    long long sizeBytes = _determineSizeBytes(size);

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

    // Write the headers to the buffer
    _writeHeaders(*out, size, sizeBytes, type);

    return 0;
}

// Header parsing functions
int parseHeaders(UserBuffer *buffer, unsigned char *type, long long *size) {
/* Function which parses headers out of a UserBuffer
 *
 * Inputs: buffer - A UserBuffer struct to parse headers out of.
 *         type   - A pointer to a char to initialize with the type read from the buffer.
 *         size   - A pointer to a long long to initialize with the size of the data to deserialize.
 *
 * Outputs: 0 on Success. > 0 on failure.
 */

    int shift = 0;
    *size = 0;
    unsigned char sizeByte;

    // Get the type; which is the first character of the headers
    if (readBuffer(buffer, type, 1)) {
        return 1;
    }

    // Get the size of the serialized body
    for (int i = 0; i < 10; i++) {
        if (readBuffer(buffer, &sizeByte, 1)) {
            return 1;
        }

        *size += (sizeByte & 127) << shift;

        // Check if this was the last byte of the size headers
        if (sizeByte & 128) {
            break;
        }

        shift += 7;
    }

    // Check if we exited the loop due to maximum number of bytes, or due to a last-byte-in-size flag.
    // If it's the prior, raise an exception. As we expect to see a last-byte-in-size flag.
    if (!(sizeByte & 128)) {
        PyErr_SetString(PyExc_ValueError, "Size header corrupted");
        return 1;
    }

    return 0;
}
