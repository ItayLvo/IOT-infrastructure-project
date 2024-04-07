/*
**
**
**

Exercises - C Expressions worksheet


a.
Expression: a;
Type: int(l-value)
value: the value of a (initizliard or assigned)
side effect: none


b.
Expression: foo;
type: function
value: r-value
side effect: none


c.
Expression: &foo;
type: function pointer
value: r-value (address of a function is not a modifiable l-value, it's simply a value that represents the memory address of the function's entry point).
side effeft: none


d.
Expression: foo();
type: int r-value
value: the outcome of the function (returned value)
side effets: the side effects of calling the function


e.
Expression: a+ foo();
type: int(non l-value)
value - a + outcome of foo();
side effect: same as question d

f.
expression: &a
type: int*
value: l-value of the type pointer to by the pointer (int)
side effect: accessing illegal address may cause a crash
**
**
**
*/


#include <stdio.h>
int main()
{
char st[] = {0x22, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x21,  0x22, '\0'};
printf("%s\n", st);
}
