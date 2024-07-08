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
#define FULL_BOARD 0xffffffffffffffff  	/* 8x8 */
/*#define FULL_BOARD 0x1FFFFFF  */	 /* 5x5 */
/*#define FULL_BOARD 0xFFFFFFFFF*/	/* 6x6 */
/*#define FULL_BOARD 0x1FFFFFFFFFFFF */	/* 7x7 */

static int IsValidMove(int x, int y);
static position_t NextKnightMove(position_t current_pos, size_t knight_movement);
static unsigned char ParseX(position_t current_pos);
static unsigned char ParseY(position_t current_pos);
static size_t ConvertPositionToIndex(position_t pos);
static knights_tour_status_e RunKnightsTourRecursion(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number);
static position_t FindStepWithMinimalMoves(position_t pos);
static size_t CountPossibleMoves(position_t pos);


static knights_tour_status_e RunKnightsTourHeuristic(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number);


/* global bitarray to represent chess board: 64 bits, represents 8x8 board */
static bit_array_t board = 0;	/*TODO: _g */

static time_t end_time = 0;

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
	end_time = time(NULL);
	end_time += time_limit;
	
	
	(void)use_heuristic;
	return RunKnightsTourHeuristic(path, starting_position, 0);
}

/*******************************/

static knights_tour_status_e RunKnightsTourRecursion(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number)
{
	size_t current_index = 0;
	position_t next_pos = 0;
	size_t i = 0;
	char st[100] = {0};
	
	printf("move number = %lu\n", move_number);
/*	printf("%s\n", ToString(board, st));*/
	
	if (time(NULL) >= end_time)
	{
		return TIME_EXCEEDED;
	}
	
	if (move_number > PATH_LENGTH)
	{
		return BAD_PATH;
	}
	
	current_index = ConvertPositionToIndex(current_pos);
/*	printf("current index = %lu\n", current_index);*/
	board = SetOn(board, current_index);
	path[move_number] = current_pos;
	printf("%s\n", ToString(board, st));
	
	if (board == FULL_BOARD)
	{
		return SUCCESS;
	}
	
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


/******************valid option above!!******************/

/**********experiment option below***********************/
static knights_tour_status_e RunKnightsTourHeuristic(position_t path[PATH_LENGTH],
													 position_t current_pos,
													 size_t move_number)
{
	size_t current_index = 0;
	position_t next_pos = 0;
	char st[100] = {0};
	
	printf("move number = %lu\n", move_number);
/*	printf("%s\n", ToString(board, st));*/
	
	if (time(NULL) >= end_time)
	{
		return TIME_EXCEEDED;
	}
	
	if (move_number > PATH_LENGTH)
	{
		return BAD_PATH;
	}
	
	current_index = ConvertPositionToIndex(current_pos);
/*	printf("current index = %lu\n", current_index);*/
	board = SetOn(board, current_index);
	path[move_number] = current_pos;
	printf("%s\n", ToString(board, st));
	
	if (board == FULL_BOARD)
	{
		return SUCCESS;
	}
	
	next_pos = FindStepWithMinimalMoves(current_pos);
	
	return RunKnightsTourHeuristic(path, next_pos, move_number + 1);
}

/*****************experiment option above*****************/


static position_t FindStepWithMinimalMoves(position_t pos)
{
	position_t move_to_return = 0;	/* TODO: invalid move */
	size_t current_num_possible_moves = 0;
	size_t min_num_possible_moves = NUM_POSSIBLE_MOVES + 1;		/* init min to be larger than possible max (8) */
	
	size_t i = 0;							/* index of move in LUT */
	
	unsigned char x = ParseX(pos);
	unsigned char y = ParseY(pos);
	
	position_t next_move = 0;
	
	for(i = 0; i < NUM_POSSIBLE_MOVES; ++i)
	{
		if (IsValidMove(x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]))
		{
			next_move = BuildPosition(x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]);
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
	
	for (i = 0; i < NUM_POSSIBLE_MOVES; ++i)
	{
		if (IsValidMove(x + possible_knight_moves[i][X], y + possible_knight_moves[i][Y]))
		{
			++count_valid_moves;
		}
	}
	
	return count_valid_moves;
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
	
/*	printf("current: i = %lu, x = %d, y = %d\n", knight_movement, x,y);*/
	
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




