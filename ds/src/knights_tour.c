/*
Date: 08/07
Status: ready for CR
Reviwer:
*/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <time.h>	/* time */

#include "knights_tour.h"
#include "bitarray.h"

#define NUM_POSSIBLE_MOVES 8
#define X 0
#define Y 1
#define INVALID_INDEX 0xff
#define NOT_HEURISTIC 0
#define HEURISTIC 1

#define FULL_BOARD 0xffffffffffffffff		/* 8x8 */
/* #define FULL_BOARD 0x1FFFFFF */		 	/* 5x5 */
/* #define FULL_BOARD 0xFFFFFFFFF */		/* 6x6 */
/* #define FULL_BOARD 0x1FFFFFFFFFFFF */	/* 7x7 */

static int IsValidMove(int x, int y);
static position_t NextKnightMove(position_t current_pos, size_t knight_movement);
static unsigned char ParseX(position_t current_pos);
static unsigned char ParseY(position_t current_pos);
static size_t ConvertPositionToIndex(position_t pos);
static position_t FindStepWithMinimalMoves(position_t pos);
static size_t CountPossibleMoves(position_t pos);
static knights_tour_status_e RunKnightsTourRecursion(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number);


/* global static variables */
static bit_array_t g_board = 0; /* global bitarray to represent chess board
								64 bits, represents 8x8 board */
static time_t g_end_time = 0;
static int g_use_heuristic = NOT_HEURISTIC;

/* knight valid moves LUT */
int possible_knight_moves[NUM_POSSIBLE_MOVES][2] =
{
	{2, 1},		/* 0 */
	{1, 2},		/* 1 */
	{-1, 2},	/* 2 */
	{-2, 1},	/* 3 */
	{-2, -1},	/* 4 */
	{-1, -2},	/* 5 */
	{1, -2},	/* 6 */
	{2, -1}		/* 7 */
};



knights_tour_status_e RunKnightsTour(position_t path[PATH_LENGTH], 
                                     position_t starting_position, 
                                     int use_heuristic, 
                                     unsigned int time_limit)
{
	g_end_time = time(NULL);
	g_end_time += time_limit;
	
	g_use_heuristic = use_heuristic;
	
	return RunKnightsTourRecursion(path, starting_position, 0);
}



static knights_tour_status_e RunKnightsTourRecursion(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number)
{
	size_t current_index = 0;
	position_t next_pos = 0;
	size_t i = 0;
	
	if (time(NULL) >= g_end_time)
	{
		return TIME_EXCEEDED;
	}
	
	current_index = ConvertPositionToIndex(current_pos);
	g_board = SetOn(g_board, current_index);
	path[move_number] = current_pos;
	
	if (g_board == FULL_BOARD)
	{
		return SUCCESS;
	}
	
	if (g_use_heuristic == HEURISTIC)	/* use heuristic variation */
	{
		next_pos = FindStepWithMinimalMoves(current_pos);
		return RunKnightsTourRecursion(path, next_pos, move_number + 1);
	}
	else	/* use backtracking, non-heuristic variation */
	{
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
		
		g_board = SetOff(g_board, current_index);
		return BAD_PATH;
	}
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



static position_t FindStepWithMinimalMoves(position_t pos)
{
	position_t move_to_return = 0;
	size_t current_num_possible_moves = 0;
	size_t min_num_possible_moves = NUM_POSSIBLE_MOVES + 1;		/* init min to be larger than possible max (8) */
	
	size_t i = 0;	/* index of move in LUT */
	
	unsigned char x = ParseX(pos);
	unsigned char y = ParseY(pos);
	
	unsigned char next_x = 0;
	unsigned char next_y = 0;
	
	position_t next_move = 0;
	
	for(i = 0; i < NUM_POSSIBLE_MOVES; ++i)
	{
		next_x = x + possible_knight_moves[i][X];
		next_y = y + possible_knight_moves[i][Y];
		
		if (IsValidMove(next_x, next_y))
		{
			next_move = BuildPosition(next_x, next_y);
			current_num_possible_moves = CountPossibleMoves(next_move);
			
			if (current_num_possible_moves < min_num_possible_moves)
			{
				min_num_possible_moves = current_num_possible_moves;
				move_to_return = next_move;
			}
		}
	}
	
	return move_to_return;
}



static size_t CountPossibleMoves(position_t pos)
{
	size_t count_valid_moves = 0;
	size_t i = 0;
	unsigned char x = ParseX(pos);
	unsigned char y = ParseY(pos);
	unsigned char next_x = 0;
	unsigned char next_y = 0;
	
	for (i = 0; i < NUM_POSSIBLE_MOVES; ++i)
	{
		next_x = x + possible_knight_moves[i][X];
		next_y = y + possible_knight_moves[i][Y];
		
		if (IsValidMove(next_x, next_y))
		{
			++count_valid_moves;
		}
	}
	
	return count_valid_moves;
}



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
			GetVal(g_board, x * NUM_OF_ROWS + y) == 0);
}



static position_t NextKnightMove(position_t current_pos, size_t knight_movement)
{
	unsigned char x = ParseX(current_pos);
	unsigned char y = ParseY(current_pos);
	
	unsigned char next_x = x + possible_knight_moves[knight_movement][X];
	unsigned char next_y = y + possible_knight_moves[knight_movement][Y];
	
	if (IsValidMove(next_x, next_y))
	{
		return BuildPosition(next_x, next_y);
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


