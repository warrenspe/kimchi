# pump
C Library for serializing certain python types

NOTE: Not ready for use



/* Note: The memory format this library uses to serialize objects is as follows:
 *
 * [Type][Size][Serialization]
 *
 * Type: 1 byte.
 *     Bitflag representing what type was serialized here. See pump.h for definitions.
 *
 * Size: X bytes.
 *     The number of bytes in the size is determined by the first byte which has its first bit set to 1.
 *     Example: [00001111][00000000][11111111] <- size is 5 bits + 2 bytes; first byte with a 1 in the first position
 *              indicates that it is the final byte.
 *              It was implemented in this manner to be more frugal regarding the # of stored bytes; as using only
 *              7 bits per byte is more efficient than dedicating an entire byte to represent the length of the size
 *              field for all serialized objects up to 72057594037927936 (2**56) bytes in size. (The maximum we're able
 *              to serialize being 2**63).  Since we expect the vast majority of serialized objects to be of a size
 *              smaller than 2**56 it was implemented in this fashion.
 *
 * Serialization: {Size} bytes.
 *     The serialization of the object.  Certain objects (ex, iterables) have their own substructures in this section.
 */


// TODO make these a bit more explicit
/* NOTE: To extend this library with the ability to serialize new types the following steps should be taken:
 *
 * 1. Add a new entry to pump/pump.h: Type Definitions.  Follow the conventions existing in the file.
 *    Ensure that the Python Object returned from your deserialization has a ref count of 1.
 * 2. Add an entry to if block chain in pump/utils/types.c discerning whether a generic python object is of your new type.
 * 3. Add a new file to pump/serializers/, to (de)serialize your type.  Follow the conventions of other files in that directory.
 * 4. Add a case to the switches in pump/utils/serialize.c calling the functions in your new /serializers/ file.
 */

