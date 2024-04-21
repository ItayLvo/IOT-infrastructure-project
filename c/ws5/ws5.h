#ifndef __WSFILES_H__
#define __WSFILES_H__

void Print(int num);
void AppendToFile(char *filename, char *str);

enum STATUS {FALSE, TRUE};

enum STATUS OperationCount(char *filename, char *input);
enum STATUS OperationAppend(char *filename, char *input);
enum STATUS OperationRemove(char *filename, char *input);
enum STATUS OperationExit(char *filename, char *input);

int CompareAppend(char *input);
int CompareExit(char *input);
int CompareCount(char *input);
int CompareRemove(char *input);


struct print_me
{
   	int data;
   	void (*Print)(int);
};


struct flag_t
{
    char *inputString;
    int (*CompareFunc)(char *);
    enum STATUS (*OperationFunc)(char *, char *);
};



#endif /* __WSFILES_H__ */
