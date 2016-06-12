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

#include "headers/pump.h"


UserBuffer *initBuffer(char *buffer, long long bytesRemaining) {
/* Initializes a UserBuffer, for the purposes of safely reading from.
 *
 * Inputs: buffer - A pointer to the buffer to read from.
 *         bytesRemaining - The number of bytes in buffer to read.
 *
 * Outputs: An instance of UserBuffer to read from.
 */

    UserBuffer *buf = malloc(sizeof(UserBuffer));

    if (buf == NULL) {
        return NULL; // TODO error
    }

    buf->buffer = buffer;
    buf->bytesRemaining = bytesRemaining;

    return buf;
}

char *readBuffer(UserBuffer *buf, long long bytesToRead) {
/* Reads from a UserBuffer.  Ensures that we are not reading past the end of the buffer.
 *
 * Inputs: buf - The UserBuffer to read from.
 *         bytesToRead - The number of bytes to read from `buf`.
 *
 * Outputs: A char pointer to the data to read, which `bytesToRead` bytes can be safely read from.
 */

    char *toReturn;

    if (buf->bytesRemaining < bytesToRead) {
        return NULL; // TODO error
    }

    toReturn = buf->buffer;

    buf->buffer += bytesToRead;

    return toReturn;
}
