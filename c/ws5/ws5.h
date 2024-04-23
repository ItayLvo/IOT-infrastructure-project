#ifndef __WSFILES_H__
#define __WSFILES_H__
#define EQUAL 1

void AppendToFile(char *filename, char *str);

enum STATUS {FALIURE, SUCCESS};

enum STATUS OperationCount(char *filename, char *input);
enum STATUS OperationAppend(char *filename, char *input);
enum STATUS OperationRemove(char *filename, char *input);
enum STATUS OperationExit(char *filename, char *input);

int CompareAppend(char *input);
int CompareExit(char *input);
int CompareCount(char *input);
int CompareRemove(char *input);

typedef struct flag_t
{
    char *inputString;
    int (*CompareFunc)(char *);
    enum STATUS (*OperationFunc)(char *, char *);
} flag_t;


void Print(int num);
struct print_me
{
   	int data;
   	void (*Print)(int);
};

#endif /* __WSFILES_H__ */
