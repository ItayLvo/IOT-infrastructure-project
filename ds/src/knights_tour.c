#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */

#include "knights_tour.h"
#include "bitarray.h"

#define N 8
#define ALL_VISITED 0xffffffffffffffff
#define NUM_POSSIBLE_MOVES 8
#define X 0
#define Y 1
#define INVALID_INDEX 0xff

static int IsValidMove(bit_array_t board, int x, int y);
static position_t NextKnightMove(bit_array_t board, position_t current_pos);
static unsigned char ParseX(position_t current_pos);
static unsigned char ParseY(position_t current_pos);


unsigned char possible_knight_moves[NUM_POSSIBLE_MOVES][2] =
{
	{1, 2},
	{1, -2},
	{-1, 2},
	{-1, -2},
	{2, 1},
	{2, -1},
	{-2, 1},
	{-2, -1}
};


knights_tour_status_e RunKnightsTour(position_t path[PATH_LENGTH], 
                                     position_t starting_position, 
                                     int use_heuristic, 
                                     unsigned int time_limit)
{
	
	/* bit_array_t is size_t => 8 bytes, 64 bits - represents 8x8 board */
	bit_array_t board = 0;
	
	return SUCCESS;
}


position_t BuildPosition(unsigned char row, unsigned char col)
{
	position_t pos = 0;	/* 0000 0000 */
	
	assert (row <= 7 && col <= 7);
	
	pos |= row;			/* 0000 xxxx */
	pos <<= 4;			/* xxxx 0000 */
	pos |= col;			/* xxxx yyyy  */
	
	return pos;
}


static int IsValidMove(bit_array_t board, int x, int y)
{
	return (x >= 0 && x < N &&
			y >= 0 && y < N &&
			GetVal(board, y * NUM_OF_ROWS + x) == 0);
}


static position_t NextKnightMove(bit_array_t board, position_t current_pos)
{
	size_t i = 0;
	unsigned char x = ParseX(current_pos);
	unsigned char y = ParseY(current_pos);
/*	printf("x = %d, y = %d\n", x, y);*/
	for (i = 0; i < NUM_POSSIBLE_MOVES; ++i)
	{
/*		printf("x = %d, y = %d\n", x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]);*/
		if (IsValidMove(board, x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]))
		{
/*			printf("x = %d, y = %d\n", x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]);*/
			return BuildPosition(x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]);
		}
	}
	
	return INVALID_INDEX;
}


static unsigned char ParseX(position_t current_pos)
{
	unsigned char x = (current_pos >> 4);
	
	return x;
}


static unsigned char ParseY(position_t current_pos)
{
	unsigned char y = (current_pos << 4);
	
	y >>= 4;
	
	return y;
}



int main()
{
	bit_array_t bitarray = 0;
	position_t pos = 36;
	char string[100];
	char new_string[100];
	
	printf("old: %s\n", ToString(pos, string));
	
	pos = NextKnightMove(bitarray, pos);
	
	printf("new: %s\n", ToString(pos, new_string));
	
	return 0;
}
