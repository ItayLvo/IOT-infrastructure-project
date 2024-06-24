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
#define NUMBER_OF_INPUT_TYPES 7

static int count_parenthesis_open = 0;

/* forward declerations */
static void InitializeCharLUT(void);
static int InitializeStacks(size_t stack_size);
static void DestroyStacks(void);
static void InitializeOperatorsLUT(void);

static double Add(double a, double b);
static double Subtract(double a, double b);
static double Multiply(double a, double b);
static double Divide(double a, double b);
static double Power(double a, double b);
static double Parenthesis(double a, double b);

static int HandleNumber(const char **);
static int HandleOperator(const char **);
static int HandleEnd(const char **);
static int HandleWhitespace(const char **);
static int HandleError(const char **);
static int HandleOpenParenthesis(const char **);
static int HandleClosedParenthesis(const char **);

static void CalculateTemporary(void);


/* math calculation function pointer */
typedef double (*math_operation_t)(double, double);

/* function pointer to a function that receives pointer to string (input) and executes transition */
typedef int (*transition_handler_t)(const char **input);



/* typedfef for different input options */
typedef enum
{
	CHAR_NUMBER,
	CHAR_OPERATOR,
	CHAR_INVALID,
	CHAR_WHITESPACE,
	CHAR_END,
	CHAR_PARENTHESIS_OPEN,
	CHAR_PARENTHESIS_CLOSED
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



transition_t transition_table[NUMBER_OF_STATES][NUMBER_OF_INPUT_TYPES] =
{
	{
		{STATE_WAIT_FOR_NUMBER, CHAR_NUMBER, STATE_WAIT_FOR_OPERATOR, HandleNumber},
		{STATE_WAIT_FOR_NUMBER, CHAR_OPERATOR, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_NUMBER, CHAR_INVALID, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_NUMBER, CHAR_WHITESPACE, STATE_WAIT_FOR_NUMBER,HandleWhitespace},
		{STATE_WAIT_FOR_NUMBER, CHAR_END, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_NUMBER, CHAR_PARENTHESIS_OPEN, STATE_WAIT_FOR_NUMBER, HandleOpenParenthesis},
		{STATE_WAIT_FOR_NUMBER, CHAR_PARENTHESIS_CLOSED, STATE_ERROR, HandleError}
	},
	{
		{STATE_WAIT_FOR_OPERATOR, CHAR_NUMBER, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_OPERATOR, CHAR_OPERATOR, STATE_WAIT_FOR_NUMBER, HandleOperator},
		{STATE_WAIT_FOR_OPERATOR, CHAR_INVALID, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_OPERATOR, CHAR_WHITESPACE, STATE_WAIT_FOR_OPERATOR,HandleWhitespace},
		{STATE_WAIT_FOR_OPERATOR, CHAR_END, STATE_SUCCESS, HandleEnd},
		{STATE_WAIT_FOR_OPERATOR, CHAR_PARENTHESIS_OPEN, STATE_ERROR, HandleError},
		{STATE_WAIT_FOR_OPERATOR, CHAR_PARENTHESIS_CLOSED, STATE_WAIT_FOR_OPERATOR, HandleClosedParenthesis}
	}
};



/* operator struct */
typedef struct
{
    int priority;
    math_operation_t math_function;
} operator_t;


/* LUT for operators */
operator_t operators_lut[126] = {0};

static void InitializeOperatorsLUT(void)
{
	operators_lut[0].priority = 0;
	operators_lut[0].math_function = NULL;
	
	operators_lut['+'].priority = 1;
	operators_lut['+'].math_function = Add;
	
	operators_lut['-'].priority = 1;
	operators_lut['-'].math_function = Subtract;
	
	operators_lut['*'].priority = 2;
	operators_lut['*'].math_function = Multiply;
	
	operators_lut['/'].priority = 2;
	operators_lut['/'].math_function = Divide;
	
	operators_lut['^'].priority = 3;
	operators_lut['^'].math_function = Power;
}



/* static ASCII LUT */
static char_type_t char_lut[128];


static void InitializeCharLUT(void)
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

	
	char_lut['+'] = CHAR_OPERATOR;
	char_lut['-'] = CHAR_OPERATOR;
	char_lut['*'] = CHAR_OPERATOR;
	char_lut['/'] = CHAR_OPERATOR;
	char_lut['^'] = CHAR_OPERATOR;
	
	char_lut['('] = CHAR_PARENTHESIS_OPEN;
	char_lut[')'] = CHAR_PARENTHESIS_CLOSED;
	
	char_lut['.'] = CHAR_NUMBER;
	
	char_lut[' '] = CHAR_WHITESPACE;
	
	char_lut['\0'] = CHAR_END;
}



static stack_t *operator_stack = NULL;
static stack_t *operand_stack = NULL;
static int InitializeStacks(size_t stack_size)
{
	operator_stack = StackCreateOneMalloc(stack_size, sizeof(char));
	if (NULL == operator_stack)
	{
		return 1;
	}
	
	operand_stack = StackCreateOneMalloc(stack_size, sizeof(double));
	if (NULL == operand_stack)
	{
		free(operator_stack);
		return 1;
	}
	
	return 0;
}


e_status_t Calculate(const char *input, double *result)
{
	state_t current_state = STATE_WAIT_FOR_NUMBER;
	transition_handler_t current_handler_function = NULL;
	state_t next_state = {0};
	char_type_t current_char_type = {0};
	const char *runner = input;
	int transition_function_status = 0;
	
	count_parenthesis_open = 0;
	InitializeCharLUT();
	InitializeOperatorsLUT();
	
	if (0 != InitializeStacks(strlen(input)))
	{
		return CALC_SYSTEM_ERROR;
	}
	
	
	while (current_state != STATE_ERROR && current_state != STATE_SUCCESS)
	{
		current_char_type = char_lut[*runner];
		next_state = transition_table[current_state][current_char_type].next_state;
		
		if (next_state == STATE_ERROR)
		{
			DestroyStacks();
			return CALC_SYNTAX_ERROR;
		}
		
		/* exctract function from FSM LUT: */
		current_handler_function = transition_table[current_state][current_char_type].transition_handler;
		/* execute transition function: */
		transition_function_status = current_handler_function(&runner);
		
		if (transition_function_status)
		{
			return CALC_MATH_ERROR;
		}
		
		
		current_state = next_state;
		++runner;
	}
	
	if (current_state == STATE_SUCCESS)
	{
		*result = *(double *)StackPeek(operand_stack);
		DestroyStacks();
		return CALC_SUCCESS;

	}
	
	
	return CALC_SYNTAX_ERROR;
}



/*
e_status_t Calculate(const char *input, double *result)
{
	char *runner = (char *)input;
	char *end_of_current_num = NULL;
	char *str_remainder = NULL;
	double operand = 0.0;
	double *p_operator = NULL;
	double tmp_result = 0;
	
	
	
	
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



	StackPop(operator_stack);
	
	while (!StackIsEmpty(operator_stack))
	{

		ch = *(char *)StackPeek(operator_stack);
		StackPop(operator_stack);
		
		if (!StackIsEmpty(operand_stack))
		static void DestroyStacks(void){
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
	
	
	
	return CALC_SUCCESS;
}
*/



/* operator functions */
static double Add(double a, double b) { return a + b; }
static double Subtract(double a, double b) { return a - b; }
static double Multiply(double a, double b) { return a * b; }
static double Divide(double a, double b) { return b == 0 ? 0 : a / b; }
static double Power(double a, double b) { return pow(a, b); }
static double Parenthesis(double a, double b) { return 0; }



/* transition handlers implementation */
static int HandleNumber(const char **input)
{
	double operand = 0.0;
	char *str_remainder = NULL;
	
	operand = strtod(*input, &str_remainder);
	StackPush(operand_stack, &operand);
	
	/* *input = str_remainder; */	/* no need, because im advancing runner in the Calculate() while loop */
	
	return 0;
}


static int HandleOperator(const char **input)
{
	const char new_char = **input;
	char prev_char = 0;
	operator_t prev_operator = {0};
	operator_t new_operator = {0};
	int i = 0;

	if (StackIsEmpty(operator_stack))
	{
		StackPush(operator_stack, &new_char);
		return 0;
	}
	
	prev_char = *(char *)StackPeek(operator_stack);
	prev_operator = operators_lut[prev_char];
	new_operator = operators_lut[new_char];
	
	if (new_operator.priority <= prev_operator.priority)
	{
		CalculateTemporary();
	}

	StackPush(operator_stack, &new_char);
	return 0;
}



static void CalculateTemporary(void)
{
	char operator_symbol = *(char *)StackPeek(operator_stack);
	operator_t operator = operators_lut[operator_symbol];
	double num1 = 0.0;
	double num2 = 0.0;
	double result = 0.0;
	
	StackPop(operator_stack);
	
	num1 = *(double *)StackPeek(operand_stack);
	StackPop(operand_stack);
	
	num2 = *(double *)StackPeek(operand_stack);
	StackPop(operand_stack);
	
	result = operator.math_function(num1, num2);
	
	StackPush(operand_stack, &result);
}


static int HandleEnd(const char **input)
{
	while (!StackIsEmpty(operator_stack))
	{
		CalculateTemporary();
	}
	
	return 0;
}


static int HandleWhitespace(const char **input)
{
	/* ++(*input); */
	
	/* do nothing... /*
	/* Calculate() while loop will advance the runner to the next char of the input */

	return 0;
}


static int HandleError(const char **input)
{
	/* do nothing */

	return 0;
}


static int HandleOpenParenthesis(const char **input)
{
	++count_parenthesis_open;
	
	
	
	return 0;
}


static int HandleClosedParenthesis(const char **input)
{
	if (count_parenthesis_open == 0)
	{
		return 1;
	}
	
	
	--count_parenthesis_open;
	
	return 0;
}


static void DestroyStacks(void)
{
	StackDestroy(operator_stack);
	StackDestroy(operand_stack);
}

