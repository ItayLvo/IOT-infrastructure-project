#ifndef __KNIGHTS_TOUR_H__
#define __KNIGHTS_TOUR_H__

#define NUM_OF_ROWS 8
#define PATH_LENGTH NUM_OF_ROWS*NUM_OF_ROWS

typedef enum knights_tour_status_e {
    SUCCESS, 
    BAD_PATH, 
    TIME_EXCEEDED
} knights_tour_status_e;

typedef unsigned char position_t;

/*
RunKnightsTour
description: A function that finds a knight tour on a 8x8 board and returns the path of the order of square visits.
input: Points on 8x8 board , time limit in seconds.
return: status and the path of the order of square visits as out-param
time complexity: O(1)
space complexity: O(1)
*/
knights_tour_status_e RunKnightsTour(position_t path[PATH_LENGTH], 
                                     position_t starting_position, 
                                     int use_heuristic, 
                                     unsigned int time_limit);

/*
BuildPosition
description: build position 
input: row , column
return: pointer to position_t
time complexity: O(1)
space complexity: O(1)
*/
position_t BuildPosition(unsigned char row, unsigned char col);

#endif /* __KNIGHTS_TOUR_H__ */
