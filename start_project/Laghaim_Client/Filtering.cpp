#include "stdafx.h"

#include <assert.h>

#include <stdlib.h>
#include "main.h"
#include "TcpUtil.h"
#include "Filtering.h"
#include "country.h"

extern DWORD	g_dwClientCountry;
// ���͸� ����.
//-------�弳 ���͸��� ���õ� ������
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

	AnyOneArg(str, buf_head);	// ĳ������ �̸� �κ��� �����Ѵ�.
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
		if(!is_space(buf[i]))// ���鹮�ڿ��� �ƴϸ� ��ƶ�
			buf2[a++] = buf[i];
		else
			array_space[max_space_num++] = i; // ���鹮�ڿ��� �ִ� ��ġ���~ ��� ����..
	}
	// NULL ó��
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
			inc_byte = 2;      // �ѱ��� ���
		else
			inc_byte = 1;      // ������ ���

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
					// ��ü���� ũ�⸸ŭ ���縦 �����ϰ�
					buf2[ii] =m_sList_b[mid].strRep[ii-k];

				int ttaa = strlen(tmp);
				buf2[ii+ttaa] = NULL;  // buf�� ������ ���ڿ��ں���..�ʱ�ȭ�� �����Ѵ�

				for(int iii = ii; iii< ii+ttaa; iii++)
					buf2[iii] = tmp[iii-ii];

				is_changed = true;
			}
			if(Res<0)
				left = mid + 1;	  // check_buffer�� �� ũ��
			else
				right = mid - 1 ; //                �� ������
		}
		if(is_changed)
			k += (m_sList_b[mid].size2)-1; // ��ü�� ������ ������ ũ�⸸ŭ�� ���Ѵ�
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

	// ���ڿ��� ���̿��� "�������"�� ������ ���� ��ŭ ��Ǫ�� ����
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
	*p = '\0';	         // �ى��!! NULL ó�� ���ֶ�~

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


// ���� ���͸� �ܾ ġȯ��Ű�� �κ�.
void CheckAndRepair(char* syntax)
{
	switch( g_dwClientCountry )
	{
	case CTRY_BRA: //- �ܾ��� ù �κк��͸� �˻��Ѵ�.
		{
			int len = strlen(syntax);
			int i = 0;
			int WordSize = 0;

			while(1)
			{
				if( i >= len ) // ������ �˻��� ���ߴٸ� ���� ����.
					break;

				WordSize = 0;

				if( is_space(syntax[i]) || i == 0 ) // ������ ã���� ù��° ���ں��� ���͸��� �õ��Ѵ�.
				{
					if( i != 0 )
						i++;

					int mid		= 0;
					int left	= 0;
					int right	= m_ListCnt-1;
					int temp	= i;

					while(1) // �� �ܾ��� ���̸� �˾� ����.
					{
						if( is_space(syntax[temp]) || syntax[temp] == NULL )
						{
							WordSize = temp - i;
							break;
						}
						temp ++;
					}

					while(right >= left) // �Ѵܾ ���͸� �Ѵ�.
					{
						mid = (right+left) /2;
						int Res = strnicmp(m_sList_b[mid].str, &syntax[i], m_sList_b[mid].size); ///���������� �ÿ� �����弳 ��ҹ��� ������� üũ�ǵ��� strnicmp �� �ٲ�. strncmp �� �ϸ� ��ҹ��� �����ؼ� üũ�Ѵ�.( 07-03-08 ����)

						if(Res < 0)
							left = mid + 1;	  // check_buffer�� �� ũ��
						else if (Res > 0)
							right = mid - 1 ; //                �� ������
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

	default: // ������ ���� �� �˻��Ѵ�.
		{
			int   space_array[250] = {0,};	 // ���Ⱑ �ִ� ��ġ�� ���� �迭
			int   space_cnt = 0;     // ���Ⱑ �ִ� ��ġ
			int i = 0, pos = 0;

			int len = strlen(syntax);
			if(len <= 0)
				return;

			// space �� ��� ���ְ�, space �� �ִ� ��ġ�� �����Ѵ�.
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

			// ���͸�
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
					int Res = strnicmp(m_sList_b[mid].str, &syntax[i], m_sList_b[mid].size); ///���������� �ÿ� �����弳 ��ҹ��� ������� üũ�ǵ��� strnicmp �� �ٲ�. strncmp �� �ϸ� ��ҹ��� �����ؼ� üũ�Ѵ�.( 07-03-08 ����)

					if(Res < 0)
						left = mid + 1;	  // check_buffer�� �� ũ��
					else if (Res > 0)
						right = mid - 1 ; //                �� ������
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
					if (syntax[i] < 0 || syntax[i] > 127) //2����Ʈ �ڵ�
						i += 2;
					else
						i++;
				}
			}

			// ���� ����
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

// ���͸� ����Ʈ�� ���Ϸ� ���� �о���� �Լ�. ���� ���۽� �о ������ �д�.
void ReadFilterList(char* fName, BOOL bAutoRep) // bAutoRep �� �ڵ����� XXXX <-�� ��ġ�� ���ΰ��� ����. �̰� TRUE�� �ƴ� ���� ���Ͽ� ��ġ� ���� �� �־���Ѵ�.
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
		//�ѱ��� ���͸�  .ftr ���ϸ�
		for(i=0; i<len; i++)
			buf[i] = buf[i]-1;
	}
	m_ListCnt = atoi(buf);

	// ����ü�� ����Ʈ ������ŭ �����Ѵ�
	m_sList_b = new FILTER[m_ListCnt];

	for (i=0; i<m_ListCnt; i++)
	{
		m_sList_b[i].str = new char[256];
		m_sList_b[i].strRep = new char[256];

		if(bAutoRep)
		{
			// �ڵ����� ��ü� �����Ѵ�.
			fscanf(fp, "%s", m_sList_b[i].str);
			m_sList_b[i].size = strlen(m_sList_b[i].str);
			m_sList_b[i].size2 = m_sList_b[i].size;


			for(ii=0; ii<m_sList_b[i].size; ii++)
			{
				if(g_dwClientCountry==CTRY_KOR)//�ѱ��� ���͸��� ��ȣȭ �Ǿ��־ ��ȣȭ�� Ǯ����Ѵ�. (by ����)
					m_sList_b[i].str[ii] = m_sList_b[i].str[ii]-1; // ��ȣȭ �� �༮�� Ǭ��.

				m_sList_b[i].strRep[ii] = 'x';
			}
			m_sList_b[i].strRep[m_sList_b[i].size] = '\0';

		}
		else
		{
			fscanf(fp, "%s %s", buf, buf2);	  // ���Ѵܾ�  ��ġ�ܾ�		ex) ���߳� ������
			len = strlen(buf);

			if(g_dwClientCountry==CTRY_KOR)//�ѱ��� ���͸��� ��ȣȭ �Ǿ��־ ��ȣȭ�� Ǯ����Ѵ�. (by ����)
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

// ���� ����� ���͸� ����Ʈ �޸� ����
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
//	���⼭�����ؽ�Ʈ�Է½� �米���� ó�� �κ�
// ����ü���ڿ� ��Ʈ���� ���� �Է��ϰ� ������Ų��
// ControlBottom�� LoadRes �κп��� �����Ѵ�
void SaveChaMotionStr()
{
	int i = 0;
	index_cnt = 28;
	// [12/22/2008 D.K ] : memory leak.
	char* strTemp[28] =//> char strTemp[28][12]
		//char strTemp[28][12] =
	{
		"���ѹα�", "��", "�λ�", "Ű��", "��", "�Ż����λ�", "�뼭", "����", "����", "�ڼ�", "���", "����", "��������"
		, "��", "������", "�޷�", "��ħ", "�Ӹ�", "���", "������", "��ü��", "����", "�̸�", "����", "����", "����", "�ɱ�"
		, "������"	// nate 2006-05-23 ������ �� �߰�
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

// Ű����κ��� ������ �Է¹޾Ҵٸ� �Է¹��� ���ڰ� ������ �迭����߿� ��ġ�ϴ°� �˻��Ѵ�
// �˻��� �����ߴٸ� �迭����� �������� �����Ѵ�
// ControlBottom �� MsgProc�κп��� ó���Ѵ�
int CheckChaMotionStr(char* syntax)
{
	char  buf[100] = "";			 // �ѱ� 50���� ��������
	int   space_pos = 0;     // ���Ⱑ �ִ� ��ġ
	int   len = 0;

	len = strlen(syntax);

	if(len == 0)
		return -1;

	buf[0] = NULL;

	///////////////////////////////////////////////////
	//	ó������ ���Ⱑ �� ���� ��ġ�� ã�´�
	//  ��) "�λ� �ȳ��ϼ���"
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
		// "�λ�" �� �ԷµȰ��
		space_pos = len;
		strcpy(buf, syntax);
		m_MotionBuf[0]='\0'; // ȭ�鿡 ������� �ʴ´�
	}
	else
	{
		// "�λ� �ȳ��ϼ���" �� �ԷµȰ��
		strncpy(buf, syntax, space_pos);
		buf[space_pos] = '\0';
		strcpy(m_MotionBuf, &syntax[space_pos]);
	}


	// �˻�����
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
			left = mid + 1;	  // check_buffer�� �� ũ��
		else
			right = mid - 1 ; //                �� ������
	}

	return -1;
}





// �ؽ�Ʈ�Է� �米���� ó���ÿ� �迭 ������ ���� �Լ�
void MySort_ForMotion(MOTION_STR *ft, int n)
{
	qsort(ft, n, sizeof(ft[0]), comp);
}

// �弳 ���͸��� ����
void MySort_ForFilter(FILTER *ft, int n)
{
	qsort(ft, n, sizeof(ft[0]), comp);
}

// ���ڿ� ���Լ�
int comp(const void* s1, const void* s2)
{
	return (strcmp(*(char**)s1, *(char**)s2));
}


// "�λ� �ȳ��ϼ���" �߿� "�λ�" �κ��� �� "�ȳ��ϼ���" �κи��� ���Ͻ��Ѽ� ����� �� �ֵ��� �Ѵ�
char* GetStr()
{
	return m_MotionBuf;
}


