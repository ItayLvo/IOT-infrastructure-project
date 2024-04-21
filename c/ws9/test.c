#include <stdio.h> /*printf*/
#include "serialize.h"

/*
typedef struct real_grade_t
{
	float math_grade;
	float physics_grade;
} real_grade_t;


typedef struct humanistic_grade_t
{
	float art_grade;
	float history_grade;
} humanistic_grade_t;


typedef struct grade_t
{
	humanistic_grade_t humanistic_grades;
	real_grade_t real_grades;
	float sports_grade;
} grade_t;


typedef struct student_t
{
	char *first_name;
	char *last_name;
	grade_t grade;
} student_t;
*/

int main(int argc, char *argv[])
{
	student_t student1;
	student_t new_student;
	
	
	CreateStudent(&student1, "moshe", "hi", 1,2,3,4,5);
	PrintStudent(&student1);
	MarshalStudent(&student1, argv[1]);
	ReadStudent(&new_student, argv[1]);
	printf("\n");
	PrintStudent(&new_student);
	
	return 0;
}



