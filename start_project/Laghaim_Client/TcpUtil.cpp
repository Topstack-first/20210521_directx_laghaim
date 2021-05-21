#include "stdafx.h"


#include <Config.h>
#include <Def.h>
#include "TcpipCon.h"

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

void SkipSpaces(char **string)
{
	for (; **string && is_space(**string); (*string)++);
}

char *AnyOneArg(char *argument, char *first_arg)
{
	SkipSpaces(&argument);

	while (*argument && *argument != '\n' && !is_space(*argument))
	{	
		*(first_arg++) = *argument;
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}

char *AnyTwoArg(char *argument, char *first_arg, char *second_arg)
{
	return (AnyOneArg(AnyOneArg(argument, first_arg), second_arg)); /* :-) */
}

char *GetSentence(char *dest, int n, FILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = fgets(dest, n, fp)) == NULL)
			return tmp;

		//while ((*tmp == 0x20 || (*tmp >= 0x09 && *tmp <= 0x0D)) && *tmp != 0)
		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	}
	while (*tmp == '#' || *tmp == 0);

	return tmp;
}


char *PGetSentence(char *dest, int n, PFILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = pfgets(dest, n, fp)) == NULL)
			return tmp;

		//while ((*tmp == 0x20 || (*tmp >= 0x09 && *tmp <= 0x0D)) && *tmp != 0)
		while (iswspace(*tmp) && *tmp != 0)
			tmp++;
	}
	while (*tmp == '#' || *tmp == 0);

	return tmp;
}


char *GetSentence(char *argument, char *first_sentence)
{
	SkipSpaces(&argument);
	// 따옴표가 있을테니깐.. 따옴표 건너띄자.
	if (*argument == '\'')
		argument++;

	while (*argument && *argument != '\'')
	{
		*(first_sentence++) = *argument;
		argument++;
	}
	if (*argument == '\'')
		argument++;

	*first_sentence = '\0';
	// 맨뒤에 이을 따옴표도 없애자.
	if (strlen(first_sentence) > 0 && *(first_sentence-1) == '\'')
		*(first_sentence-1) = '\0';

	return (argument);
}
