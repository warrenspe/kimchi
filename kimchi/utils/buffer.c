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
