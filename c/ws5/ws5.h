#ifndef __WSFILES_H__
#define __WSFILES_H__

int CompareAndOperate(char *user_input, char *file_name);

void AppendToFile(char *filename, char *str);

typedef enum STATUS {FALIURE, SUCCESS} STATUS;
STATUS OperationCount(char *filename, char *input);
STATUS OperationAppend(char *filename, char *input);
STATUS OperationRemove(char *filename, char *input);
STATUS OperationExit(char *filename, char *input);

int CompareAppend(char *input);
int CompareExit(char *input);
int CompareCount(char *input);
int CompareRemove(char *input);

typedef struct flag_t
{
    char *inputString;
    int (*CompareFunc)(char *);
    STATUS (*OperationFunc)(char *, char *);
} flag_t;


void Print(int num);
struct print_me
{
   	int data;
   	void (*Print)(int);
};

#endif /* __WSFILES_H__ */
