/* itay
reviewed by: shahar
status: needs adjustments after CR
*/


#include <stdio.h> /*printf*/
#include <string.h> /*strlen*/
#include <stdlib.h> /*malloc*/

#include "serialize.h"

static FILE *WriteHumanisticGrade(humanistic_grade_t grades, FILE *file_ptr);
static FILE *WriteRealGrade(real_grade_t grades, FILE *file_ptr);

static size_t g_first_name_size = 0;
static size_t g_last_name_size = 0;

void CreateStudent(student_t *student, char *f_name, char *l_name, float art_grade, float history_grade,
			float math_grade, float physics_grade, float sports_grade)
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


void MarshalStudent(student_t *student, char *file_name)
{
	FILE *file_ptr = NULL;
	
	if (NULL == file_name)
	{
		return;
	}
	
	
	file_ptr = fopen(file_name, "wb");
	if (NULL == file_ptr)
	{
		return;
	}
	
	g_first_name_size = strlen(student->first_name) + 1;
	g_last_name_size = strlen(student->last_name) + 1;
	
	file_ptr = WriteString(student->first_name, file_ptr);
	file_ptr = WriteString(student->last_name, file_ptr);
	file_ptr = WriteGrade(student->grade, file_ptr);
	
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


static FILE *WriteHumanisticGrade(humanistic_grade_t grades, FILE *file_ptr)
{
	WriteFloat(&(grades.art_grade), file_ptr);
	WriteFloat(&(grades.history_grade), file_ptr);
	
	return file_ptr;
}


static FILE *WriteRealGrade(real_grade_t grades, FILE *file_ptr)
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
	FILE *file_ptr = NULL;
	
	if (NULL == file_name)
	{
		return;
	}
	
	
	file_ptr = fopen(file_name, "rb");
	if (NULL == file_ptr)
	{
		return;
	}
	file_ptr = ReadStrings(student, file_ptr);
	file_ptr = ReadGrades(student, file_ptr);
	
	fclose(file_ptr);
}


FILE *ReadStrings(student_t *student, FILE *file_ptr)
{
	if (NULL == file_ptr)
	{
		return NULL;
	}
	
	student->first_name = (char *)malloc(g_first_name_size * sizeof(char));
	if (NULL != student->first_name)
	{
		fread(student->first_name, g_first_name_size, 1, file_ptr);
	}
	
	student->last_name = (char *)malloc(g_last_name_size * sizeof(char));
	
	if (NULL != student->last_name)
	{
		fread(student->last_name, g_last_name_size, 1, file_ptr);
	}
	
	return file_ptr;
}



FILE *ReadGrades(student_t *student, FILE *file_ptr)
{
	if (NULL == file_ptr)
	{
		return NULL;
	}
	
	fread(&(student->grade.humanistic_grades.art_grade), sizeof(float), 1, file_ptr);
	fread(&(student->grade.humanistic_grades.history_grade), sizeof(float), 1, file_ptr);
	fread(&(student->grade.real_grades.math_grade), sizeof(float), 1, file_ptr);
	fread(&(student->grade.real_grades.physics_grade), sizeof(float), 1, file_ptr);
	fread(&(student->grade.sports_grade), sizeof(float), 1, file_ptr);

	return file_ptr;
}


void FreeStudent(student_t *student)
{
	free(student->first_name);
	free(student->last_name);
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




