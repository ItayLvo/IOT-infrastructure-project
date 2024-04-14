#ifndef __STRFUNCS_H__
#define __STRFUNCS_H__

size_t StrLen(const char *str);

int StrCmp(const char *str1, const char *str2);

char *StrCpy(char* destination, const char* source);

char *StrnCpy(char *dest, const char *src, size_t n);

int StrnCmp(const char *str1, const char *str2, size_t n);

int StrCaseCmp(const char *s1, const char *s2);
 
char *StrChr(const char *str, int c);
 
char *StrDup(const char *s); 

char *StrCat(char *s1, const char *s2);

char *StrnCat(char *s1, const char *s2, size_t n);

char *StrStr(const char *haystack, const char *needle);

size_t StrSpn(const char *str1, const char *str2);

char *StrTok(char *str, const char *delim);

int IsPalindrome(char *str);

char *DeleteSpace(char *str);

void SevenBoom(int from, int to);

#endif
