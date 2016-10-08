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

#ifndef __BUFFER_H
#define __BUFFER_H 1

    // Struct to hold a serialized object; ensuring we do not read past its length
    typedef struct UserBuffer {
        char *buffer;
        unsigned long long bytesRemaining;
    } UserBuffer;

    // Function Prototypes
    UserBuffer *initBuffer(char *, unsigned long long);
    int readBuffer(UserBuffer *, unsigned char *, unsigned long long);

    // Includes
    #include "utils/buffer.c"

#endif
