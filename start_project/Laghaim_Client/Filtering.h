#ifndef _FILTERING_H_
#define _FILTERING_H_

#include "Config.h"

#define MAX_SPACE_ARRAY_NUM   80
#define MAX_STR_NUM           80
#define MAX_HEAD_NUM		  40


typedef struct
{
	char* str;
	char* strRep;
	int   size;
	int   size2;
} FILTER;

typedef struct tag_INFO
{

	char  buf_head[MAX_HEAD_NUM]; // 케릭터 아이디를 담는다.
	char  buf[MAX_STR_NUM]; // head 부분을 뺀 문자열
	char  buf2[MAX_STR_NUM]; // 공백문자열을 빼고 한구문으로 만든 문자열을 보관한다.
	char  buf3[MAX_STR_NUM]; // head 부분과 공백문자열의 복구를 위한 버퍼
	int   array_space[MAX_SPACE_ARRAY_NUM]; // 공백문자열의 위치를 담을 배열
	int   max_space_num; // 공백문자가 검출된 개수

	tag_INFO()
	{
		buf_head[0]='\0';
		buf[0]='\0';
		buf2[0]='\0';
		buf3[0]='\0';
		max_space_num = 0;
		memset(array_space, -1, MAX_SPACE_ARRAY_NUM);
	}

	BOOL GetHead(char* str);
	BOOL SkipEmptyStr();
	BOOL Bi_Search();
	BOOL AssemStr(char*dest, int* array);
} tag_INFO;

typedef struct
{
	char* str;
	int	  index;
	int	  size;
} MOTION_STR;


char*		CheckFilterListToString(char* str ); // 필터리스트와 비교해서 포함된 문자기 있으면 TRUE값을 보내준다.
void		CheckAndRepair(char* syntax);
void		ReadFilterList(char *file_name, BOOL bAutoRep = FALSE);	// 욕설리트스 파일 읽고 버퍼에 저장하기
//char*		CheckAndRepair2(char* syntax);// 서버로부터 받은 스트링과의 비교하기
//char*		CheckAndRepair(char* syntax);
//int		CheckSpace(char* String);	   // 스트링중에 띄어쓰기한 부분의 카운트를 리턴한다
//bool		CheckSpace(char* String);
void		MySort_ForFilter(FILTER *ft, int n);
void		DeleteFilterList();

void		SaveChaMotionStr();
int			CheckChaMotionStr(char* syntax);
char*		GetStr();
void		MySort_ForMotion(MOTION_STR *ft, int n);
int			comp(const void* s1, const void* s2);

#endif // _FILTERING_H_
