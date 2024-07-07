#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */
#include <time.h>	/* time */

#include "knights_tour.h"
#include "bitarray.h"

#define NUM_POSSIBLE_MOVES 8
#define X 0
#define Y 1
#define INVALID_INDEX 0xff
/*#define FULL_BOARD 0xffffffffffffffff*/
#define FULL_BOARD 0x1FFFFFF

static int IsValidMove(int x, int y);
static position_t NextKnightMove(position_t current_pos, size_t knight_movement);
static unsigned char ParseX(position_t current_pos);
static unsigned char ParseY(position_t current_pos);
static size_t ConvertPositionToIndex(position_t pos);
static knights_tour_status_e RunKnightsTourRecursion(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number);

/* global bitarray to represent chess board: 64 bits, represents 8x8 board */
static bit_array_t board = 0;

static time_t end_time = 0;

int possible_knight_moves[NUM_POSSIBLE_MOVES][2] =
{
	{1, 2},		/* 0 */
	{1, -2},	/* 1 */
	{-1, 2},	/* 2 */
	{-1, -2},	/* 3 */
	{2, 1},		/* 4 */
	{2, -1},	/* 5 */
	{-2, 1},	/* 6 */
	{-2, -1}	/* 7 */
};



knights_tour_status_e RunKnightsTour(position_t path[PATH_LENGTH], 
                                     position_t starting_position, 
                                     int use_heuristic, 
                                     unsigned int time_limit)
{
	end_time = time(NULL);
	end_time += time_limit;
	
	
	(void)use_heuristic;
	return RunKnightsTourRecursion(path, starting_position, 0);
}



static knights_tour_status_e RunKnightsTourRecursion(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number)
{
	size_t current_index = 0;
	position_t next_pos = 0;
	size_t i = 0;
	char st[100] = {0};
	
	printf("move number = %lu\n", move_number);
	printf("%s\n", ToString(board, st));
	
	if (time(NULL) >= end_time)
	{
		return TIME_EXCEEDED;
	}
	
	current_index = ConvertPositionToIndex(current_pos);
	printf("current index = %lu\n", current_index);
	board = SetOn(board, current_index);
	path[move_number] = current_pos;
	printf("%s\n", ToString(board, st));
	
	if (board == FULL_BOARD)
	{
		return SUCCESS;
	}
	
/*	if (move_number > 25)*/
/*	{*/
/*		return BAD_PATH;*/
/*	}*/
	
	for (i = 0; i < NUM_POSSIBLE_MOVES; ++i)
	{
		next_pos = NextKnightMove(current_pos, i);
		
		if (next_pos != INVALID_INDEX)
		{
			if (RunKnightsTourRecursion(path, next_pos, move_number + 1) == SUCCESS)
			{
				return SUCCESS;
			}
		}
		
	}
	
	board = SetOff(board, current_index);
	return BAD_PATH;
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





/*************** static helper functions ***************/


static size_t ConvertPositionToIndex(position_t pos)
{
	unsigned char x = ParseX(pos);
	unsigned char y = ParseY(pos);
	
	size_t index = x * NUM_OF_ROWS + y;
	
	return index;
}


static int IsValidMove(int x, int y)
{
	return (x >= 0 && x < NUM_OF_ROWS &&
			y >= 0 && y < NUM_OF_ROWS &&
			GetVal(board, x * NUM_OF_ROWS + y) == 0);
}


static position_t NextKnightMove(position_t current_pos, size_t knight_movement)
{
	unsigned char x = ParseX(current_pos);
	unsigned char y = ParseY(current_pos);
	
	printf("current: i = %lu, x = %d, y = %d\n", knight_movement, x,y);
	
	if (IsValidMove(x + possible_knight_moves[knight_movement][X], y + possible_knight_moves[knight_movement][Y]))
	{
		printf("SELECTED: i = %lu, x = %d, y = %d\n\n", knight_movement, x + possible_knight_moves[knight_movement][X], y + possible_knight_moves[knight_movement][Y]);
		return BuildPosition(x + possible_knight_moves[knight_movement][X], y + possible_knight_moves[knight_movement][Y]);
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




