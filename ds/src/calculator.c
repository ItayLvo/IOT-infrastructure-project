#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <math.h>	/* pow */
#include <stdlib.h>	/* strtod */
#include <string.h>	/* strlen */


#include "calculator.h"		/* calculator functions */
#include "stack.h"		/* stack data structure and functions */


#define NUMBER_OF_STATES 2
#define NUMBER_OF_INPUTS 5


/* forward declerations */
static char *RemoveWhiteSpacesFromStart(char *string);	/* delete? */

static void InitializeCharLUT(void);

static double Add(double a, double b);
static double Subtract(double a, double b);
static double Multiply(double a, double b);
static double Divide(double a, double b);
static double Power(double a, double b);

static void HandleNumber(const char **);
static void HandleOperator(const char **);
/*
static void HandleOpenParen(const char **);
static void HandleCloseParen(const char **);
*/
static void HandleEnd(const char **);
static void HandleWhitespace(const char **);
static void HandleError(const char **);


/* math calculation function pointer */
typedef double (*math_operation_t)(double, double);

/* function pointer to a function that receives pointer to string (input) and executes transition */
typedef void (*transition_handler_t)(const char **input);



/* typedfef for different input options */
typedef enum
{
	CHAR_INVALID,
	CHAR_NUMBER,
	CHAR_OPERATOR,
	CHAR_WHITESPACE,
	CHAR_END
} char_type_t;



/* FSM states */
typedef enum
{
	STATE_WAIT_FOR_NUMBER,
	STATE_WAIT_FOR_OPERATOR,
	STATE_ERROR,
	STATE_SUCCESS
} state_t;



/* FSM transition struct (start state -> input -> handler -> end state) */
typedef struct
{
	state_t start_state;
	char_type_t input;
	state_t next_state;
	transition_handler_t transition_handler;
} transition_t;


/* [2][5] */
transition_t transition_table[NUMBER_OF_STATES][NUMBER_OF_INPUTS] =
{
	{
		{STATE_WAIT_FOR_NUMBER, CHAR_NUMBER, STATE_WAIT_FOR_OPERATOR, HandleNumber},
		{STATE_WAIT_FOR_NUMBER, CHAR_OPERATOR, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_NUMBER, CHAR_INVALID, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_NUMBER, CHAR_WHITESPACE, STATE_WAIT_FOR_NUMBER,HandleWhitespace},
		{STATE_WAIT_FOR_NUMBER, CHAR_END, STATE_ERROR, HandleError}
	},
	{
		{STATE_WAIT_FOR_OPERATOR, CHAR_NUMBER, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_OPERATOR, CHAR_OPERATOR, STATE_WAIT_FOR_NUMBER, HandleOperator},
		{STATE_WAIT_FOR_OPERATOR, CHAR_INVALID, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_OPERATOR, CHAR_WHITESPACE, STATE_WAIT_FOR_OPERATOR,HandleWhitespace},
		{STATE_WAIT_FOR_OPERATOR, CHAR_END, STATE_SUCCESS, HandleEnd}
	}
};



/* operator struct */
typedef struct
{
    char symbol;
    int priority;
    math_operation_t math_function;
} operator_t;



/* LUT for operators */
operator_t operators_arr[] = {
	{'+', 1, Add},
	{'-', 1, Subtract},
	{'*', 2, Multiply},
	{'/', 2, Divide},
	{'^', 3, Power},
	{'\0', 0, NULL}
};


/* static ASCII LUT */
char_type_t char_lut[128];


void InitializeCharLUT(void)
{
	int i = 0;
	
	for (i = 0; i < 128; ++i)
	{
		char_lut[i] = CHAR_INVALID;
	}


	for (i = '0'; i <= '9'; ++i)
	{
		char_lut[i] = CHAR_NUMBER;
	}

	
	/* loop through operators and assign the relevant LUT symbol with the right char type */
	for (i = 0; operators_arr[i].symbol != '\0'; ++i)
	{
		char_lut[(int)operators_arr[i].symbol] = CHAR_OPERATOR;
	}
	
	
	char_lut['.'] = CHAR_NUMBER;
	char_lut[' '] = CHAR_WHITESPACE;
	char_lut['\0'] = CHAR_END;
}



e_status_t Calculate(const char *input, double *result)
{
	char *runner = (char *)input;
	char *end_of_current_num = NULL;
	char *str_remainder = NULL;
	double operand = 0.0;
	double *p_operator = NULL;
	double tmp_result = 0;
	
	stack_t *operator_stack = StackCreateOneMalloc(strlen(input), sizeof(char));
	stack_t *operand_stack = StackCreateOneMalloc(strlen(input), sizeof(double));
	
	char ch;
	double num1;
	double num2;
	
	
	operand = strtod(runner, &str_remainder);
	
	while ('\0' != *runner)
	{
		str_remainder = RemoveWhiteSpacesFromStart(str_remainder);
		
		printf("operand = %f\n", operand);
		printf("operator = %c\n", str_remainder[0]);
		printf("remainder string = %s\n", str_remainder);
		
		StackPush(operand_stack, &operand);
		
		ch = str_remainder[0];
		if (ch == '-')
		{
			ch = '+';
		}
			
		StackPush(operator_stack, &str_remainder[0]);

		if (*(str_remainder + 1) == ' ')
		{
			++str_remainder;
			str_remainder = RemoveWhiteSpacesFromStart(str_remainder);
		}
		
		
		
		runner = str_remainder;
		operand = strtod(str_remainder, &str_remainder);
	}
	
	/* pop '\0' */
	StackPop(operator_stack);
	
	while (!StackIsEmpty(operator_stack))
	{

		ch = *(char *)StackPeek(operator_stack);
		StackPop(operator_stack);
		
		if (!StackIsEmpty(operand_stack))
		{
			num1 = *(double *)StackPeek(operand_stack);
			StackPop(operand_stack);
		}
		
		tmp_result += num1;
		
		printf("curr sum = %f \n", tmp_result);
	}
	
	if (!StackIsEmpty(operand_stack))
	{
		num1 = *(double *)StackPeek(operand_stack);
		StackPop(operand_stack);
		tmp_result += num1;
	}
			
	printf("end sum = %f \n", tmp_result);
	
	StackDestroy(operator_stack);
	StackDestroy(operand_stack);
	
	return CALC_SUCCESS;
}



/* operator functions */
double Add(double a, double b) { return a + b; }
double Subtract(double a, double b) { return a - b; }
double Multiply(double a, double b) { return a * b; }
double Divide(double a, double b) { return b == 0 ? 0 : a / b; }
double Power(double a, double b) { return pow(a, b); }



static void HandleNumber(const char **input)
{

}

static void HandleOperator(const char **input)
{

}
/*
static void HandleOpenParen(const char **input);
static void HandleCloseParen(const char **input);
*/


static void HandleEnd(const char **input)
{

}
static void HandleWhitespace(const char **input)
{

}
static void HandleError(const char **input)
{

}



static char *RemoveWhiteSpacesFromStart(char *string)
{
	char *runner = string;
	while (' ' == *runner)
	{
		++runner;
	}
	
	return runner;
}

