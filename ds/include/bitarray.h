#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h> /* size_t */

typedef size_t bit_array_t;

/*
Description: Set all bits to 1
Params: bit_array_t
Return Value: bit_array_t
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t SetAll(bit_array_t bit_arr);

/*
Description: Set all bits to 0
Params: bit_array_t
Return Value: bit_array_t
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t ResetAll(bit_array_t bit_arr);

/*
Description: Set bit on
Params: Bit array and an index
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t SetOn(bit_array_t bit_arr, size_t index);

/*
Description: Set bit off
Params: Bit array and an index
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t SetOff(bit_array_t bit_arr, size_t index);

/*
Description: Set the bit to the on/off depends on user given parameter bool_value.
Params: Bit array, index and boolean value
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t SetBit(bit_array_t bit_arr, size_t index, int bool_value);

/*
Description: Get the value of a bit of specific index
Params: Bit array, index
Return Value: int (bool)
Time Complexity: O(1)
Space: O(1)
*/
int GetVal(bit_array_t bit_arr, size_t index);


/*
Description: Flips bit at specific index.
Params: Bit array, index
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t FlipBit(bit_array_t bit_arr, size_t index);


/*
Description: Flip all the bits of the array
Params: Bit array
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t FlipAll(bit_array_t bit_arr);

/*
Description: Mirroring the bits in the array
Params: Bit array
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t Mirror(bit_array_t bit_arr);

/*
Description: Rotates n bits to right
Params: Bit array, n to rotate
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t RotateRight(bit_array_t bit_arr,size_t n);

/*
Description: Rotates n bits to left
Params: Bit array, n to rotate
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t RotateLeft(bit_array_t bit_arr,size_t n);

/*
Description: Counts the bits that are on in the array
Params: Bit array
Return Value: size_t
Time Complexity: O(1)
Space: O(1)
*/
size_t CountOnLut(bit_array_t bit_arr);

/*O(n) implementation: */
size_t CountOn(bit_array_t bit_arr);

/*
Description: Counts the bits that are off in the array
Params: Bit array
Return Value: size_t
Time Complexity: O(1)
Space: O(1)
*/
size_t CountOff(bit_array_t bit_arr);

/*
Description: Translate the bits to be represented as string
Params: Bit array, buffer
Return Value: size_t
Time Complexity: O(n)
Space: O(n)
*/
char *ToString(bit_array_t bit_arr, char *buffer);

/*
Description: Mirroring the bits in the array using LUT
Params: Bit array
Return Value: Bit array
Time Complexity: O(1)
Space: O(1)
*/
bit_array_t MirrorLut(bit_array_t bit_arr);



#endif /*__BIT_ARRAY_H__*/
