#include <stdio.h> /*printf*/
#include <string.h> /*strlen*/
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

void CreateStudent(student_t *student, char *f_name, char *l_name, float art_grade, float history_grade, float math_grade, float physics_grade, float sports_grade)
{
	student->first_name = f_name;
	student->last_name = l_name;
	student->grade.humanistic_grades.art_grade = art_grade;
	student->grade.humanistic_grades.history_grade = history_grade;
	student->grade.real_grades.math_grade = math_grade;
	student->grade.real_grades.physics_grade = physics_grade;
	student->grade.sports_grade = sports_grade;
}


void MarshalStudent(student_t *student, char *file_name)
{
	size_t student_size = sizeof(*student);
	FILE *file_ptr;
	
	if (NULL == file_name)
	{
		return;
	}
	
	
	file_ptr = fopen(file_name, "ab");
	if (NULL == file_ptr)
	{
		return;
	}
	
	
	file_ptr = WriteString(student->first_name, file_ptr);
	file_ptr = WriteString(student->last_name, file_ptr);
	file_ptr = WriteGrade(student->grade, file_ptr);
	
	/*fwrite(student, student_size, 1, file_ptr);*/
	
	fclose(file_ptr);
	
}


FILE *WriteString(char *str, FILE *file_ptr)
{
	size_t string_len = 0;
	
	if (NULL == file_ptr)
	{
		return NULL;
	}
	
	string_len = strlen(str) + 1;
	
	printf("sizeof str is: %ld\n", string_len);
	
	fwrite(str, string_len, 1, file_ptr);

	return file_ptr;
}


FILE *WriteGrade(grade_t grade, FILE *file_ptr)
{
	if (NULL == file_ptr)
	{
		return NULL;
	}
	
	file_ptr = WriteHumanisticGrade(grade.humanistic_grades, file_ptr);
	file_ptr = WriteRealGrade(grade.real_grades, file_ptr);
	file_ptr = WriteFloat(&(grade.sports_grade), file_ptr);

	return file_ptr;
}


FILE *WriteHumanisticGrade(humanistic_grade_t grades, FILE *file_ptr)
{
	WriteFloat(&(grades.art_grade), file_ptr);
	WriteFloat(&(grades.history_grade), file_ptr);
	
	return file_ptr;
}


FILE *WriteRealGrade(real_grade_t grades, FILE *file_ptr)
{
	WriteFloat(&(grades.math_grade), file_ptr);
	WriteFloat(&(grades.physics_grade), file_ptr);
	
	return file_ptr;
}


FILE *WriteFloat(float *grade, FILE *file_ptr)
{
	
	if (NULL == file_ptr)
	{
		return NULL;
	}
	
	fwrite(grade, sizeof(float), 1, file_ptr);
	
	return file_ptr;
}


void ReadStudent(student_t *student, char *file_name)
{
	int file_size = 0;
	FILE *file_ptr;
	
	if (NULL == file_name)
	{
		return;
	}
	
	
	file_ptr = fopen(file_name, "rb");
	if (NULL == file_ptr)
	{
		return;
	}
	
	fseek(file_ptr, 0, SEEK_END);
	file_size = ftell(file_ptr); 
	rewind(file_ptr);
	
	fread(student, file_size, 1, file_ptr);
	fclose(file_ptr);
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






