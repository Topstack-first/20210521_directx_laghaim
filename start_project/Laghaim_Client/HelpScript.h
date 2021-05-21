#ifndef _HELP_SCRIPT_
#define _HELP_SCRIPT_

#include "config.h"
#include <vector>
#include <list>
#include <iterator>
//#include <algorithm>
using namespace std;

#pragma warning(disable : 4995)

#define HELPSCRIPT_FRONT_DUMMY_SIZE 91 // ���� ��ȣȭ�� ���ʿ� �ִ� ���� ������
#define HELPSCRIPT_LAST_DUMMY_SIZE 23 // ���� ��ȣȭ�� ���ʿ� �ִ� ���� ������
#define HELPSCRIPT_ENCODE_BYTE 0x64     // ��ȣȭ ��Ʈ. ���� �����ͺο� ^= �����ؼ� ����Ѵ�. ���ڴ� �� ���Ѱ����� �����͸� �о���� �� �ҽ������� ���� ��������Ѵ�.

enum { HELP_CON_TYPE_F_LEVELUP = 0, HELP_CON_TYPE_A_LEVEL, HELP_CON_TYPE_F_ITEM, HELP_CON_TYPE_F_SERVER
	   , HELP_CON_TYPE_F_NPC_IN, MAX_HELP_CON_TYPE_NUM
	 }; // ���� Ÿ��. F�� ���� ���� ����, A�� ���� üũ ����.
enum { HELP_EXCEP_CODE_ONLY_ACHERON = 1, HELP_EXCEP_CODE_SERVER, MAX_EXCEP_CODE_NUM }; // ���� ���� Ÿ��.(�ߺ��� �ȵǴϱ� ���� �ߺ� ������ ����� �ڵ带 �ϳ� �߰�����.

struct sHelpData
{
	int m_Index; // ������ �ε���(���������θ� ������ �ִ´�.)
	int m_ConditionType; // ������ ���� ����
	char *m_strConArg; // ������ ���� ����.
	int m_CoolTime; // ���� ���� �ƴҶ� �ݺ��ϴ� ƽ.
	int m_ExceptionCode; // ���� ����ó���� �ʿ��� ��쿡 ���. (��: ���ɷ� ���������� ����,)

	char *m_strSearchTitle; // �˻� ��� ǥ�� ����.
	char *m_strSearchWord1; // �˻� Ű����1
	char *m_strSearchWord2; // �˻� Ű����2
	char *m_strSearchWord3; // �˻� Ű����3

	char *m_strNormalTalk; // �Ϲ� ������ ���.
	char *m_strChaoticTalk; // ī�� ������ ���.

	vector<char *>		m_listImage; // �� ���� �̹����� ����Ʈ.

	DWORD m_ActivateTick; // �ߵ��� ƽ.

	sHelpData()
	{
		m_strConArg = NULL; // ������ ���� ����.
		m_strSearchTitle = NULL; // �˻� ��� ǥ�� ����.
		m_strSearchWord1 = NULL; // �˻� Ű����1
		m_strSearchWord2 = NULL; // �˻� Ű����2
		m_strSearchWord3 = NULL; // �˻� Ű����3
		m_strNormalTalk = NULL; // �Ϲ� ������ ���.
		m_strChaoticTalk = NULL; // ī�� ������ ���.

		Init();
	}

	~sHelpData()
	{
		Init();
	}

	void Init()
	{
		m_Index = -1; // ������ �ε���(���������θ� ������ �ִ´�.)
		m_ConditionType = -1; // ������ ���� ����
		m_CoolTime = -1; // ���� ���� �ƴҶ� �ݺ��ϴ� ƽ.
		m_ExceptionCode = -1;

		m_ActivateTick = 0; // �ߵ��� ƽ.

		if( m_strConArg )
			SAFE_DELETE_ARRAY( m_strConArg );
		if( m_strSearchTitle )
			SAFE_DELETE_ARRAY( m_strSearchTitle );
		if( m_strSearchWord1 )
			SAFE_DELETE_ARRAY( m_strSearchWord1 );
		if( m_strSearchWord2 )
			SAFE_DELETE_ARRAY( m_strSearchWord2 );
		if( m_strSearchWord3 )
			SAFE_DELETE_ARRAY( m_strSearchWord3 );
		if( m_strNormalTalk )
			SAFE_DELETE_ARRAY( m_strNormalTalk );
		if( m_strChaoticTalk )
			SAFE_DELETE_ARRAY( m_strChaoticTalk );

		char *pImage;
		vector<char*>::iterator prImage;
		for( prImage = m_listImage.begin(); prImage != m_listImage.end(); ++prImage )
		{
			pImage = *prImage;
			SAFE_DELETE( pImage );
		}

		m_listImage.clear();
	}

	BOOL CopyData( sHelpData *Dest )
	{
		if( !Dest )
			return FALSE;

		Dest->Init(); // �ʱ�ȭ�ϰ�.

		int Len = 0;

		Dest->m_Index = this->m_Index;
		Dest->m_ConditionType = this->m_ConditionType;

		if( this->m_strConArg )
		{
			Len = strlen( this->m_strConArg );
			Dest->m_strConArg = new char[Len+1];
			if( Dest->m_strConArg )
			{
				strcpy( Dest->m_strConArg, this->m_strConArg );
				Dest->m_strConArg[Len] = '\0';
			}
		}

		Dest->m_CoolTime = this->m_CoolTime;
		Dest->m_ExceptionCode = this->m_ExceptionCode;


		if( this->m_strSearchTitle )
		{
			Len = strlen( this->m_strSearchTitle );
			Dest->m_strSearchTitle = new char[Len+1];
			if( Dest->m_strSearchTitle )
			{
				strcpy( Dest->m_strSearchTitle, this->m_strSearchTitle );
				Dest->m_strSearchTitle[Len] = '\0';
			}
		}

		if( this->m_strSearchWord1 )
		{
			Len = strlen( this->m_strSearchWord1 );
			Dest->m_strSearchWord1 = new char[Len+1];
			if( Dest->m_strSearchWord1 )
			{
				strcpy( Dest->m_strSearchWord1, this->m_strSearchWord1 );
				Dest->m_strSearchWord1[Len] = '\0';
			}
		}

		if( this->m_strSearchWord2 )
		{
			Len = strlen( this->m_strSearchWord2 );
			Dest->m_strSearchWord2 = new char[Len+1];
			if( Dest->m_strSearchWord2 )
			{
				strcpy( Dest->m_strSearchWord2, this->m_strSearchWord2 );
				Dest->m_strSearchWord2[Len] = '\0';
			}
		}

		if( this->m_strSearchWord3 )
		{
			Len = strlen( this->m_strSearchWord3 );
			Dest->m_strSearchWord3 = new char[Len+1];
			if( Dest->m_strSearchWord3 )
			{
				strcpy( Dest->m_strSearchWord3, this->m_strSearchWord3 );
				Dest->m_strSearchWord3[Len] = '\0';
			}
		}


		if( this->m_strNormalTalk )
		{
			Len = strlen( this->m_strNormalTalk );
			Dest->m_strNormalTalk = new char[Len+1];
			if( Dest->m_strNormalTalk )
			{
				strcpy( Dest->m_strNormalTalk, this->m_strNormalTalk );
				Dest->m_strNormalTalk[Len] = '\0';
			}
		}


		if( this->m_strChaoticTalk )
		{
			Len = strlen( this->m_strChaoticTalk );
			Dest->m_strChaoticTalk = new char[Len+1];
			if( Dest->m_strChaoticTalk )
			{
				strcpy( Dest->m_strChaoticTalk, this->m_strChaoticTalk );
				Dest->m_strChaoticTalk[Len] = '\0';
			}
		}

		char *pImageName;
		char *pImageNameDest;
		vector<char*>::iterator prImage;
		for( prImage = m_listImage.begin(); prImage != m_listImage.end(); ++prImage )
		{
			pImageName = *prImage;

			Len = strlen( pImageName );
			pImageNameDest = new char[Len+1];
			if( pImageNameDest )
			{
				strcpy( pImageNameDest, pImageName );
				pImageNameDest[Len] = '\0';

				Dest->m_listImage.push_back( pImageNameDest );
			}
		}

		Dest->m_ActivateTick = this->m_ActivateTick; // ��� �̰� �ʿ������...


		return TRUE;
	}
};

class CHelpDataManager
{
public:
	CHelpDataManager()
	{
		Init();
	}

	~CHelpDataManager()
	{
		Init();
	}

	void Init()
	{
		m_TotalHelpNum = 0;

		sHelpData *pHelp;
		list<sHelpData*>::iterator prHelp;
		for( prHelp = m_listHelp.begin(); prHelp != m_listHelp.end(); ++prHelp )
		{
			pHelp = *prHelp;

			pHelp->Init(); // �̰� ������ ���༭ �޸𸮸� �������ش�.
			SAFE_DELETE( pHelp );
		}
		m_listHelp.clear();
	}

	BOOL LoadDta( const char* FileName );
	void SaveDta( const char* FileName ); // .dta �� ����

	void ExportScriptToText( const char* FileName ); // �ؽ�Ʈ�� ����
	void ImportScriptFromText( const char* FileName ); // �ؽ�Ʈ�κ��� �о�ͼ� ����

	int m_TotalHelpNum;
	list<sHelpData*>		m_listHelp; // ���� ����.
};


#endif