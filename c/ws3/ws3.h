#ifndef __WSARRAYS_H__
#define __WSARRAYS_H__

int *MatrixSum(int size, int mat[2][2], int *res);

int josephus(int arr[], int size);

int NextAliveIndex(int arr[], int size, int index);

char **CopyEnv(char **envp, char **newstr);

char *ToLowerString(char *str);


void TestJosephus();
void TestMatrix();
void TestEnvp(char *envp[]);

#endif /*__WSARRAYS_H__ */
