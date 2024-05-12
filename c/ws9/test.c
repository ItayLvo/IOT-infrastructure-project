#include <stdio.h>	/*FILE **/
#include <stdlib.h>	/*malloc*/
#include <string.h>	/*strlen*/

#include "serialize.h"

void CreateStudent(student_t *student, char *f_name, char *l_name,
		float art_grade, float history_grade,
		float math_grade, float physics_grade,
		float sports_grade);
		
void PrintStudent(student_t *student);

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


void CreateStudent(student_t *student, char *f_name, char *l_name,
			float art_grade, float history_grade,
			float math_grade, float physics_grade,
			float sports_grade)
{
	student->first_name = (char *)malloc(strlen(f_name) + 1);
	if (NULL != student->first_name)
	{
	strcpy(student->first_name, f_name);
	}
	student->last_name = (char *)malloc(strlen(l_name) + 1);
	if (NULL != student->last_name)
	{
	strcpy(student->last_name, l_name);
	}
	
	student->grade.humanistic_grades.art_grade = art_grade;
	student->grade.humanistic_grades.history_grade = history_grade;
	student->grade.real_grades.math_grade = math_grade;
	student->grade.real_grades.physics_grade = physics_grade;
	student->grade.sports_grade = sports_grade;
}


void PrintStudent(student_t *student)
{
	printf("%s\n", student->first_name);
	printf("%s\n", student->last_name);
	printf("%f\n", student->grade.humanistic_grades.art_grade);
	printf("%f\n", student->grade.humanistic_grades.history_grade);
	printf("%f\n", student->grade.real_grades.math_grade);
	printf("%f\n", student->grade.real_grades.physics_grade);
	printf("%f\n", student->grade.sports_grade);
}

