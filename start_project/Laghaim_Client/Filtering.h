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

	char  buf_head[MAX_HEAD_NUM]; // �ɸ��� ���̵� ��´�.
	char  buf[MAX_STR_NUM]; // head �κ��� �� ���ڿ�
	char  buf2[MAX_STR_NUM]; // ���鹮�ڿ��� ���� �ѱ������� ���� ���ڿ��� �����Ѵ�.
	char  buf3[MAX_STR_NUM]; // head �κа� ���鹮�ڿ��� ������ ���� ����
	int   array_space[MAX_SPACE_ARRAY_NUM]; // ���鹮�ڿ��� ��ġ�� ���� �迭
	int   max_space_num; // ���鹮�ڰ� ����� ����

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


char*		CheckFilterListToString(char* str ); // ���͸���Ʈ�� ���ؼ� ���Ե� ���ڱ� ������ TRUE���� �����ش�.
void		CheckAndRepair(char* syntax);
void		ReadFilterList(char *file_name, BOOL bAutoRep = FALSE);	// �弳��Ʈ�� ���� �а� ���ۿ� �����ϱ�
//char*		CheckAndRepair2(char* syntax);// �����κ��� ���� ��Ʈ������ ���ϱ�
//char*		CheckAndRepair(char* syntax);
//int		CheckSpace(char* String);	   // ��Ʈ���߿� ������ �κ��� ī��Ʈ�� �����Ѵ�
//bool		CheckSpace(char* String);
void		MySort_ForFilter(FILTER *ft, int n);
void		DeleteFilterList();

void		SaveChaMotionStr();
int			CheckChaMotionStr(char* syntax);
char*		GetStr();
void		MySort_ForMotion(MOTION_STR *ft, int n);
int			comp(const void* s1, const void* s2);

#endif // _FILTERING_H_
