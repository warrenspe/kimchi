/*
 *  buffer.c: File containing logic pertaining to reading from the buffer containing a serialized object.
 *            Prevents buffer overflow errors & attacks by limiting the number of bytes which can be read from the object.
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


UserBuffer *initBuffer(char *buffer, unsigned long long bytesRemaining) {
/* Initializes a UserBuffer, for the purposes of safely reading from.
 *
 * Inputs: buffer         - A pointer to the buffer to read from.
 *         bytesRemaining - The number of bytes in buffer to read.
 *
 * Outputs: An instance of UserBuffer to read from.
 */

    UserBuffer *buf = malloc(sizeof(UserBuffer));

    if (buf == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to acquire memory for UserBuffer");
        return NULL;
    }

    buf->buffer = buffer;
    buf->bytesRemaining = bytesRemaining;

    return buf;
}

int readBuffer(UserBuffer *buf, unsigned char *out, unsigned long long bytesToRead) {
/* Reads from a UserBuffer.  Ensures that we are not reading past the end of the buffer.
 *
 * Inputs: buf         - The UserBuffer to read from.
 *         out         - A char pointer to a string with sufficient memory (bytesToRead) to write to.
 *         bytesToRead - The number of bytes to read from `buf`.
 *
 * Outputs: 0 if successful. > 0 otherwise.
 */

    char *toWrite;

    if (buf->bytesRemaining < bytesToRead) {
        PyErr_SetString(PyExc_OverflowError, "Attempted to read past the end of UserBuffer");
        return 1;
    }

    toWrite = buf->buffer;

    buf->buffer += bytesToRead;

    memcpy(out, toWrite, bytesToRead);

    buf->bytesRemaining -= bytesToRead;

    return 0;
}
