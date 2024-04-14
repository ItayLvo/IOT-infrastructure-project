/* Currently implementing part 2 and part 3 of the exercies */

#ifndef __WSFILES_H__
#define __WSFILES_H__

void Print(int num);
void AppendToFile(char *filename, char *str);

void OperationCount(char *filename, char *input);
void OperationAppend(char *filename, char *input);
void OperationRemove(char *filename, char *input);
void OperationExit(char *filename, char *input);

int CompareAppend(char *input);
int CompareExit(char *input);
int CompareCount(char *input);
int CompareRemove(char *input);

#endif /* __WSFILES_H__ */
