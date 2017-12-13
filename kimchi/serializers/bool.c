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

// Function Prototypes
int serializeBool(char **, unsigned long long *);
PyObject *deserializeBool(unsigned char);


int serializeBool(char **buffer, unsigned long long *size) {
/* Function which serializes a Python Boolean into a string.
 *
 * Inputs: buffer - A pointer to a string where we would otherwise be serializing the Boolean to.
 *         size   - A pointer to a long long to fill with the number of bytes serialized to buffer.
 *
 * Outputs: 0.
 */

    *buffer = NULL;
    *size = 0;
    return 0;
}


PyObject *deserializeBool(unsigned char type) {
/* Function which returns the appropriate Python Boolean given the type.
 *
 * Inputs: type - A char containing the type of object we're deserializing.
 *
 * Outputs: A Python Boolean.
 */

    if (type == BOOL_TRUE_TYPE) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}
