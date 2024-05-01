#ifndef __BITARRAY_H__
#define __BITARRAY_H__

#include <stdio.h>	/* size_t */


typedef unsigned long bitarray_t;

typedef enum
{
	BIT_OFF = 0,
	BIT_ON = 1;
}	bit_mode_t;

typedef enum
{
	SUCCESS = 0,
	BM_PTR_ERROR = 1,
	RANGE_ERROR = 2,
	DEST_ERROR = 3
}	status_t;

typedef struct bitarray
{
    unsigned long data;
} bitarray;


void SetAll(bitarray *bitarr);

void ResetAll(bitarray *bitarr);

void SetOn(bitarray *bitarr, size_t index, size_t n);

void SetOff(bitarray *bitarr, size_t index, size_t n);

void SetBit(bitarray *bitarr, size_t index, bit_mode_t bit_mode);

bit_mode_t GetVal(bitarray *bitarr, size_t index);

void Flip(bitarray *bitarr);

void Mirror(bitarray *bitarr);

void RotateRight(bitarray *bitarr, size_t n);

void RotateLeft(bitarray *bitarr, size_t n);

void CountOn(bitarray *bitarr);

void CountOff(bitarray *bitarr);

void ToString(bitarray *bitarr);

#endif	/*__BITARRAY_H__*/
