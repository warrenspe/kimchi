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


int serialize(PyObject *object, char **out, Py_ssize_t *outSize) {
/* Function which can accept various objects and convert them into a serialized string version.
 * 
 * Inputs: object  - A Python object to be serialized.
 *         out     - A pointer to a string pointer to initialize and fill with object's serialization.
 *         outSize - A pointer to an int recording the size of the string placed into out.
 *
 * Outputs: 0 on success. > 0 on failure.
 */

    char *body = NULL,
         *headers = NULL;
    unsigned char type;
    unsigned long long bodySize = 0,
                       headerSize = 0;

    if (!(type = getType(object))) {
        return 1;
    }

    switch (type) {
    // Python 2 specific serializers
    #if PY_MAJOR_VERSION == 2
        case INT_TYPE:
        case NEG_INT_TYPE:
            if (serializeInt(object, type, &body, &bodySize))
                return 1;
            break;
    #endif

        case LONG_TYPE:
        case NEG_LONG_TYPE:
            if (serializeLong(object, type, &body, &bodySize))
                return 1;
            break;

        case FLOAT_TYPE:
            if (serializeFloat(object, &body, &bodySize))
                return 1;
            break;

        case BYTES_TYPE:
            if (serializeBytes(object, &body, &bodySize))
                return 1;
            break;

        case UNICODE_TYPE:
            if (serializeUnicode(object, &body, &bodySize))
                return 1;
            break;

        case LIST_TYPE:
            if (serializeList(object, &body, &bodySize))
                return 1;
            break;

        case TUPLE_TYPE:
            if (serializeTuple(object, &body, &bodySize))
                return 1;
            break;

        case DICT_TYPE:
            if (serializeDict(object, &body, &bodySize))
                return 1;
            break;

        case SET_TYPE:
        case FROZEN_SET_TYPE:
            if (serializeSet(object, &body, &bodySize))
                return 1;
            break;

        case BOOL_TRUE_TYPE:
        case BOOL_FALSE_TYPE:
            if (serializeBool(&body, &bodySize))
                return 1;
            break;

        case NONE_TYPE:
            if (serializeNone(&body, &bodySize))
                return 1;
            break;

        case INSTANCE_TYPE:
            if (serializeClassInstance(object, &body, &bodySize))
                return 1;
            break;

        default:
            PyErr_Format(PyExc_TypeError,
                         "Cannot serialize object; unserializable type \"%.400s\"",
                         Py_TYPE(object)->tp_name);
            return 1;
    }

    // Construct headers for this serialization
    constructHeaders(&headers, &headerSize, bodySize, type);

    *outSize = headerSize + bodySize;

    // Create a string, concatenating headers and body
    (*out) = malloc(bodySize + headerSize + 1);
    memset((void *) *out, '\0', bodySize + headerSize + 1);

    memcpy((void *) *out, (void *) headers, headerSize);
    memcpy((void *) ((*out) + headerSize), (void *) body, bodySize);

    // Free body and headers
    free(body);
    free(headers);

    return 0;
}

PyObject *deserialize(UserBuffer *buf) {
/* Function which takes a byte string in the form of a UserBuffer and deserializes it into a Python object.
 *
 * Inputs: buf - A UserBuffer struct containing the buffer to deserialize.
 *
 * Outputs: The deserialized Python object, or NULL if an error occurs.
 */

    unsigned char type; // Type of object being deserialized
    unsigned long long size;

    if (parseHeaders(buf, &type, &size)) {
        return NULL;
    }

    switch (type) {
    // Python 2 specific deserializers
    #if PY_MAJOR_VERSION == 2
        case INT_TYPE:
        case NEG_INT_TYPE:
            return deserializeInt(buf, type, size);
    #endif

        case LONG_TYPE:
        case NEG_LONG_TYPE:
            return deserializeLong(buf, type, size);

        case FLOAT_TYPE:
            return deserializeFloat(buf);

        case UNICODE_TYPE:
            return deserializeUnicode(buf, size);

        case BYTES_TYPE:
            return deserializeBytes(buf, size);

        case LIST_TYPE:
            return deserializeList(buf);

        case TUPLE_TYPE:
            return deserializeTuple(buf);

        case DICT_TYPE:
            return deserializeDict(buf);

        case SET_TYPE:
        case FROZEN_SET_TYPE:
            return deserializeSet(buf, type);

        case BOOL_TRUE_TYPE:
        case BOOL_FALSE_TYPE:
            return deserializeBool(type);

        case NONE_TYPE:
            return deserializeNone();

        case INSTANCE_TYPE:
            return deserializeClassInstance(buf);
    }

    PyErr_Format(PyExc_TypeError, "Cannot deserialize object; unrecognized type flag: %d", type);

    return NULL;
}
