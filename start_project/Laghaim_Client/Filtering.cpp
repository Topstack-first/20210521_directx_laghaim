#include "stdafx.h"

#include <assert.h>

#include <stdlib.h>
#include "main.h"
#include "TcpUtil.h"
#include "Filtering.h"
#include "country.h"

extern DWORD	g_dwClientCountry;
// 필터링 관련.
//-------욕설 필터링에 관련된 변수들
char		m_DrawBuf[255];
char		m_MotionBuf[255];
FILTER*		m_sList_b;
MOTION_STR*	m_sList_m;
int			m_ListCnt;
int			index_cnt;
int			rep_pos;

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))


BOOL tag_INFO::GetHead(char* str)
{
	if(!str)
		return FALSE;

	AnyOneArg(str, buf_head);	// 캐릭터의 이름 부분을 보관한다.
	int len = strlen(buf_head);
	strcpy(buf, &str[len]);

	int emty_cnt = 0;
	while(is_space(buf[emty_cnt]))
		emty_cnt++;

	strcpy(buf, &buf[emty_cnt]);

	return TRUE;
}




BOOL tag_INFO::SkipEmptyStr()
{
	int a = 0;
	int len = strlen(buf);

	for(int i=0; i<len; i++)
	{
		if(!is_space(buf[i]))// 공백문자열이 아니면 담아라
			buf2[a++] = buf[i];
		else
			array_space[max_space_num++] = i; // 공백문자열이 있던 위치라네~ 언능 담자..
	}
	// NULL 처리
	buf2[len-max_space_num] = '\0';
	return TRUE;
}


BOOL tag_INFO::Bi_Search()
{
	int len = strlen(buf2);

	for(int k=0; k<len; k++)
	{
		int mid;
		int left = 0;
		int right;
		int inc_byte = 0;
		bool  is_changed = false;

		right =  m_ListCnt-1;

		if((buf2[k]) < 0x00)
			inc_byte = 2;      // 한글일 경우
		else
			inc_byte = 1;      // 영문일 경우

		while(right >=left)
		{
			mid = (right+left) /2;

			int Res = strncmp(m_sList_b[mid].str, &buf2[k], m_sList_b[mid].size);
			char tmp[100];

			if(Res == 0)
			{
				strcpy(tmp, &buf2[k+m_sList_b[mid].size]);
				int ii = 0;
				for(ii=k; ii<k+m_sList_b[mid].size2; ii++)
					// 대체어의 크기만큼 복사를 실행하고
					buf2[ii] =m_sList_b[mid].strRep[ii-k];

				int ttaa = strlen(tmp);
				buf2[ii+ttaa] = NULL;  // buf의 마지막 문자열뒤부터..초기화를 실행한다

				for(int iii = ii; iii< ii+ttaa; iii++)
					buf2[iii] = tmp[iii-ii];

				is_changed = true;
			}
			if(Res<0)
				left = mid + 1;	  // check_buffer이 더 크면
			else
				right = mid - 1 ; //                더 작으면
		}
		if(is_changed)
			k += (m_sList_b[mid].size2)-1; // 대체를 실행한 문자의 크기만큼을 더한다
		else
			k += inc_byte-1;
	}
	return TRUE;
}


BOOL tag_INFO::AssemStr(char*dest, int* array)
{
	if(!dest)
		return false;

	char * p = NULL;
	int cnt = 0;
	int t = 0;
	int len = strlen(dest);

	strcpy(buf3, dest);
	p = buf3;

	// 문자열의 길이에서 "공백분자"의 갯수를 더한 만큼 루푸를 돈다
	for(int i=0; i<len+max_space_num; i++)
	{
		if(cnt == array[t] && array[t] > 0)
		{
			*p = ' ';
			p++;
			t++;
			cnt++;
		}
		else
		{
			cnt++;
			*p = *dest;
			p++;
			dest++;
		}
	}
	*p = '\0';	         // 다됬네!! NULL 처리 해주라~

	return TRUE;
}

char* CheckFilterListToString(char* str )
{
	char syntax[512] = {0,};

	memcpy( syntax, str, strlen(str));

	switch( g_dwClientCountry )
	{
	case CTRY_KOR:
		{
			int len = strlen(syntax);
			int pos = 0;

			while( len > pos )
			{
				for(int i=0; i<m_ListCnt; i++ )
				{
					int res = strnicmp( m_sList_b[i].str, &syntax[pos], 1 );

					if( res == 0)
					{
						int strFlen = strlen(m_sList_b[i].str);
						res = strnicmp( m_sList_b[i].str, &syntax[pos], strFlen );

						if( res == 0)
						{
							return m_sList_b[i].str;
						}
					}
				}
				pos ++;
			}
		}
		break;
	}

	return NULL;
}


// 실제 필터링 단어를 치환시키는 부분.
void CheckAndRepair(char* syntax)
{
	switch( g_dwClientCountry )
	{
	case CTRY_BRA: //- 단어의 첫 부분부터만 검사한다.
		{
			int len = strlen(syntax);
			int i = 0;
			int WordSize = 0;

			while(1)
			{
				if( i >= len ) // 끝까지 검색을 다했다면 종료 하자.
					break;

				WordSize = 0;

				if( is_space(syntax[i]) || i == 0 ) // 공백을 찾으면 첫번째 문자부터 필터링을 시도한다.
				{
					if( i != 0 )
						i++;

					int mid		= 0;
					int left	= 0;
					int right	= m_ListCnt-1;
					int temp	= i;

					while(1) // 한 단어의 길이를 알아 본다.
					{
						if( is_space(syntax[temp]) || syntax[temp] == NULL )
						{
							WordSize = temp - i;
							break;
						}
						temp ++;
					}

					while(right >= left) // 한단어를 필터링 한다.
					{
						mid = (right+left) /2;
						int Res = strnicmp(m_sList_b[mid].str, &syntax[i], m_sList_b[mid].size); ///국가대항전 시에 영문욕설 대소문자 상관없이 체크되도록 strnicmp 로 바꿈. strncmp 로 하면 대소문자 구분해서 체크한다.( 07-03-08 원석)

						if(Res < 0)
							left = mid + 1;	  // check_buffer이 더 크면
						else if (Res > 0)
							right = mid - 1 ; //                더 작으면
						else
						{
							if(m_sList_b[mid].size != WordSize  )
							{
								WordSize = 0;
								break;
							}

							memcpy(&syntax[i], m_sList_b[mid].strRep, m_sList_b[mid].size);
							break;
						}
					} // end while(right >= left)
				} // end if( is_space(syntax[i]) || i == 0 )

				if( WordSize == 0)
					i++;
				else
					i += WordSize;

			} // end while(1)
		}
		break;

	default: // 공백을 없앤 후 검사한다.
		{
			int   space_array[250] = {0,};	 // 띄어쓰기가 있는 위치를 담을 배열
			int   space_cnt = 0;     // 띄어쓰기가 있는 위치
			int i = 0, pos = 0;

			int len = strlen(syntax);
			if(len <= 0)
				return;

			// space 를 모두 없애고, space 가 있던 위치를 저장한다.
			for (i = 0; i < len; i++)
			{
				if (is_space(syntax[i]))
				{
					for (pos = i; pos < len; pos++)
						syntax[pos] = syntax[pos+1];

					space_array[space_cnt++] = i;
					len--;
					i--;
				}
			}

			// 필터링
			i = 0;
			while (i < len)
			{
				BOOL found = FALSE;
				int mid = 0;
				int left = 0;
				int right = 0;
				right =  m_ListCnt-1;

				while (right >= left)
				{
					mid = (right+left) /2;
					int Res = strnicmp(m_sList_b[mid].str, &syntax[i], m_sList_b[mid].size); ///국가대항전 시에 영문욕설 대소문자 상관없이 체크되도록 strnicmp 로 바꿈. strncmp 로 하면 대소문자 구분해서 체크한다.( 07-03-08 원석)

					if(Res < 0)
						left = mid + 1;	  // check_buffer이 더 크면
					else if (Res > 0)
						right = mid - 1 ; //                더 작으면
					else
					{
						memcpy(&syntax[i], m_sList_b[mid].strRep, m_sList_b[mid].size);
						found = TRUE;
						break;
					}
				}
				if (found)
					i += m_sList_b[mid].size;
				else
				{
					if (syntax[i] < 0 || syntax[i] > 127) //2바이트 코드
						i += 2;
					else
						i++;
				}
			}

			// 원상 복귀
			for (i = space_cnt-1; i >= 0; i--)
			{
				for (pos = len; pos >= space_array[i]; pos--)
					syntax[pos+1] = syntax[pos];

				syntax[space_array[i]] = ' ';
				len++;
			}
		}
		break;
	}
}

// 필터링 리스트를 파일로 부터 읽어오는 함수. 게임 시작시 읽어서 저장해 둔다.
void ReadFilterList(char* fName, BOOL bAutoRep) // bAutoRep 는 자동으로 XXXX <-로 대치할 것인가의 여부. 이게 TRUE가 아닌 경우는 파일에 대치어가 따로 들어가 있어야한다.
{
	FILE* fp;
	int i, ii, j, len;
	char buf[100], buf2[100];
	fp = fopen(fName, "rt");

	assert(fp != NULL);

	fscanf(fp, "%s", buf);
	//fgets(buf, 100, fp);
	len = strlen(buf);
	if(g_dwClientCountry==CTRY_KOR)
	{
		//한국의 필터링  .ftr 파일만
		for(i=0; i<len; i++)
			buf[i] = buf[i]-1;
	}
	m_ListCnt = atoi(buf);

	// 구조체를 리스트 개수만큼 생성한다
	m_sList_b = new FILTER[m_ListCnt];

	for (i=0; i<m_ListCnt; i++)
	{
		m_sList_b[i].str = new char[256];
		m_sList_b[i].strRep = new char[256];

		if(bAutoRep)
		{
			// 자동으로 대체어를 생성한다.
			fscanf(fp, "%s", m_sList_b[i].str);
			m_sList_b[i].size = strlen(m_sList_b[i].str);
			m_sList_b[i].size2 = m_sList_b[i].size;


			for(ii=0; ii<m_sList_b[i].size; ii++)
			{
				if(g_dwClientCountry==CTRY_KOR)//한국의 필터링은 암호화 되어있어서 암호화를 풀어야한다. (by 원석)
					m_sList_b[i].str[ii] = m_sList_b[i].str[ii]-1; // 암호화 된 녀석만 푼다.

				m_sList_b[i].strRep[ii] = 'x';
			}
			m_sList_b[i].strRep[m_sList_b[i].size] = '\0';

		}
		else
		{
			fscanf(fp, "%s %s", buf, buf2);	  // 제한단어  대치단어		ex) 씨발놈 좋은놈
			len = strlen(buf);

			if(g_dwClientCountry==CTRY_KOR)//한국의 필터링은 암호화 되어있어서 암호화를 풀어야한다. (by 원석)
			{
				for(j=0; j<len; j++)
				{
					buf[j] = buf[j]-1;
					buf2[j] = buf2[j]-1;
				}
			}

			strcpy(m_sList_b[i].str, buf);
			strcpy(m_sList_b[i].strRep, buf2);

			m_sList_b[i].size = strlen(m_sList_b[i].str);
			m_sList_b[i].size2 = m_sList_b[i].size;
		}

	}
	fclose(fp);

	MySort_ForFilter(m_sList_b, m_ListCnt);
}

// 게임 종료시 필터링 리스트 메모리 해제
void DeleteFilterList()
{
	int i;
	for(i = 0; i<m_ListCnt; i++)
	{
		if(m_sList_b[i].str != NULL)
		{
			SAFE_DELETE_ARRAY(m_sList_b[i].str);
			SAFE_DELETE_ARRAY(m_sList_b[i].strRep);
		}
	}
	SAFE_DELETE_ARRAY( m_sList_b );

	for(i=0; i<index_cnt; i++)
	{
		SAFE_DELETE_ARRAY(m_sList_m[i].str);
	}
	SAFE_DELETE_ARRAY(m_sList_m);
}


//////////////////////////////////////////////////////////////////////////////////////////
//	여기서부터텍스트입력시 사교동작 처리 부분
// 구조체인자에 스트링을 직접 입력하고 정열시킨다
// ControlBottom의 LoadRes 부분에서 실행한다
void SaveChaMotionStr()
{
	int i = 0;
	index_cnt = 28;
	// [12/22/2008 D.K ] : memory leak.
	char* strTemp[28] =//> char strTemp[28][12]
		//char strTemp[28][12] =
	{
		"대한민국", "손", "인사", "키스", "절", "신사의인사", "용서", "디스코", "점프", "박수", "기쁨", "웃기", "공중제비"
		, "뺨", "발차기", "메롱", "똥침", "머리", "울기", "기지개", "재체기", "나를", "이리", "오케", "곰곰", "저쪽", "앉기"
		, "꼭지점"	// nate 2006-05-23 꼭지점 댄스 추가
	};

	m_sList_m = new MOTION_STR[index_cnt];

	for(i = 0 ; i < index_cnt ; i ++ )
	{
		m_sList_m[i].str = new char[strlen(strTemp[i])+1];
		strcpy(m_sList_m[i].str, strTemp[i]);
		m_sList_m[i].index = i;
	}

	for (i=0; i<index_cnt; i++)
		m_sList_m[i].size = strlen(m_sList_m[i].str);

	MySort_ForMotion(m_sList_m, index_cnt);
}

// 키보드로부터 문자을 입력받았다면 입력받은 문자가 정열된 배열요소중에 일치하는가 검색한다
// 검색이 성공했다면 배열요소의 정수값을 리턴한다
// ControlBottom 의 MsgProc부분에서 처리한다
int CheckChaMotionStr(char* syntax)
{
	char  buf[100] = "";			 // 한글 50글자 기준으로
	int   space_pos = 0;     // 띄어쓰기가 있는 위치
	int   len = 0;

	len = strlen(syntax);

	if(len == 0)
		return -1;

	buf[0] = NULL;

	///////////////////////////////////////////////////
	//	처음으로 띄어쓰기가 된 곳의 위치를 찾는다
	//  예) "인사 안녕하세요"
	for(int i=0; i<len; i++)
	{
		if(is_space(syntax[i]) == TRUE)
		{
			rep_pos = space_pos = i;
			break;
		}
	}
	if(space_pos ==0)
	{
		// "인사" 만 입력된경우
		space_pos = len;
		strcpy(buf, syntax);
		m_MotionBuf[0]='\0'; // 화면에 출력하지 않는다
	}
	else
	{
		// "인사 안녕하세요" 가 입력된경우
		strncpy(buf, syntax, space_pos);
		buf[space_pos] = '\0';
		strcpy(m_MotionBuf, &syntax[space_pos]);
	}


	// 검색시작
	int mid = 0;
	int left = 0;
	int right = 0;
	right =  index_cnt-1;

	while(right >=left)
	{
		mid = (right+left) /2;

		int Res = strcmp(m_sList_m[mid].str, buf);

		if(Res == 0)
			return m_sList_m[mid].index;

		if(Res<0)
			left = mid + 1;	  // check_buffer이 더 크면
		else
			right = mid - 1 ; //                더 작으면
	}

	return -1;
}





// 텍스트입력 사교동작 처리시에 배열 정열을 위한 함수
void MySort_ForMotion(MOTION_STR *ft, int n)
{
	qsort(ft, n, sizeof(ft[0]), comp);
}

// 욕설 필터링을 위한
void MySort_ForFilter(FILTER *ft, int n)
{
	qsort(ft, n, sizeof(ft[0]), comp);
}

// 문자열 비교함수
int comp(const void* s1, const void* s2)
{
	return (strcmp(*(char**)s1, *(char**)s2));
}


// "인사 안녕하세요" 중에 "인사" 부분을 뺀 "안녕하세요" 부분만을 리턴시켜서 출력할 수 있도록 한다
char* GetStr()
{
	return m_MotionBuf;
}


