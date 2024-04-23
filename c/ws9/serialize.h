#ifndef __SERIALIZE_H__
#define __SERIALIZE_H__


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


void CreateStudent(student_t *student, char *f_name, char *l_name, float art_grade, float history_grade, float math_grade, float physics_grade, float sports_grade);

void MarshalStudent(student_t *student, char *file_name);

void ReadStudent(student_t *student, char *file_name);

void PrintStudent(student_t *student);

FILE *WriteString(char *str, FILE *file_ptr);

FILE *WriteGrade(grade_t grade, FILE *file_ptr);

FILE *WriteFloat(float *grade, FILE *file_ptr);

FILE *WriteHumanisticGrade(humanistic_grade_t grades, FILE *file_ptr);

FILE *WriteRealGrade(real_grade_t grades, FILE *file_ptr);

FILE *ReadStrings(student_t *student, FILE *file_ptr);

FILE *ReadGrades(student_t *student, FILE *file_ptr);

void FreeStudent(student_t *student);

#endif /* __SERIALIZE_H__ */


