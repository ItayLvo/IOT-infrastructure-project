#ifndef __WSARRAYS_H__
#define __WSARRAYS_H__

#define COL 2
#define ROW 2

int *MatrixSum(int mat[ROW][COL], int *res);

int josephus(int arr[], int size);

int NextAliveIndex(int arr[], int size, int index);

char **CopyEnv(char **envp, char **newstr);

char *ToLowerString(char *str);

#endif /*__WSARRAYS_H__ */
