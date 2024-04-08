#ifndef STRFUNCS_H
#define STRFUNCS_H

size_t StrLen(const char *str);

int StrCmp(const char *str1, const char *str2);

char *StrCpy(char* destination, const char* source);

char *StrnCpy(char *dest, const char *src, size_t n);

int StrnCmp(const char *str1, const char *str2, size_t n);

 int StrCaseCmp(const char *s1, const char *s2);
 
 char *StrChr(const char *str, int c);

#endif
