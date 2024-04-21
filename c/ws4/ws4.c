/*
name: itay
status: finished
*/

#include <stdio.h>	/* printf, scanf */
#include <stdlib.h>	/* exit */
#include "ws4.h"

#define ESC 27
#define A 65
#define T 84
#define ASCII_SIZE 256

void LUTImplementation(void)
{
	char userch = '\0';
	typedef void (*Handlers_t)(void);
	size_t i = 0;
	Handlers_t funcs[ASCII_SIZE];
	
	for (i = 0; i < (sizeof(funcs) / sizeof(funcs[0])); ++i)
	{
		funcs[i] = FuncEmpty;
	}
	funcs[ESC] = FuncESCPressed;
	funcs[A] = FuncAPressed;
	funcs[T] = FuncTPressed;
	
	system("stty -icanon -echo");
	
	while(ESC != userch) /* 27 = ESC key*/
	{
		
		printf("enter a character: \n");
		scanf("%c", &userch);
		funcs[(unsigned int)userch]();
	}
	system("stty icanon echo");
}


void FuncEmpty(void)
{ /* does nothing */ }


void FuncAPressed(void)
{
	printf("A-pressed\n");
}
void FuncTPressed(void)
{
	printf("T-pressed\n");
}
void FuncESCPressed(void)
{
	system("stty icanon echo");
	return;
}



void IfImplementation(void)
{
	char userch = '\0';

	system("stty -icanon -echo");
	
	while(ESC != userch)
	{
		
		printf("enter a character: \n");
		scanf("%c", &userch);

		
		if (userch == ESC)
		{
			system("stty icanon echo");
			return;
		}
		else if (userch == 'A')
		{
			printf("A-pressed\n");
		}
		else if (userch == 'T')
		{
			printf("T-pressed\n");
		}
	}
	
	system("stty icanon echo");
}


void SwitchImplementation(void)
{
	char userch = '\0';
	
	system("stty -icanon -echo");
	
	while(ESC != userch)
	{
		
		printf("enter a character: \n");
		scanf("%c", &userch);
		
		switch(userch)
		{
			case 'A':
				printf("A-pressed\n");
				break;
			case 'T':
				printf("T-pressed\n");
				break;
			case ESC:
				system("stty icanon echo");
				return;
			default:
				break;
		}
	}
	
	system("stty icanon echo");
}


void TypedefEx()
{
	typedef int (*func)(int, int);
	
	typedef char string[5];
	string st = {'a', 'b', 'c', '\0'};
	
	func newfunc = Add;
	
	int x = 5;
	int *xp = &x;
	
	int y = newfunc(6, 10);

	xp++;
	printf("%d\n", y);
	printf("%s\n", st);
}

int Add(int a, int b)
{
	int x = 3 + 6;
	x++;
	return a + b;
}
