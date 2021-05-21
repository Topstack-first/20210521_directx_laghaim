#ifndef _TCP_UTIL_H_
#define _TCP_UTIL_H_

#include "Config.h"

char *AnyOneArg(char *argument, char *first_arg);
char *AnyTwoArg(char *argument, char *first_arg, char *second_arg);
void SkipSpaces(char **string);
char *GetSentence(char *dest, int n, FILE *fp);
char *PGetSentence(char *dest, int n, PFILE *fp);
char *GetSentence(char *argument, char *first_sentence);

#endif // TCP_UTIL_H