#include <stdio.h> /* FILE * */
#include "serialize.h"


int main(int argc, char *argv[])
{
	student_t student1;
	student_t new_student;
	
	
	CreateStudent(&student1, "moshe", "hi", 1.1,2.2,-3.3,4,5);
	PrintStudent(&student1);
	MarshalStudent(&student1, argv[1]);
	ReadStudent(&new_student, argv[1]);
	PrintStudent(&new_student);
	
	FreeStudent(&student1);
	FreeStudent(&new_student);
	return 0;
}



