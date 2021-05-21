#ifndef _HELP_SCRIPT_
#define _HELP_SCRIPT_

#include "config.h"
#include <vector>
#include <list>
#include <iterator>
//#include <algorithm>
using namespace std;

#pragma warning(disable : 4995)

#define HELPSCRIPT_FRONT_DUMMY_SIZE 91 // 파일 암호화시 앞쪽에 넣는 더미 싸이즈
#define HELPSCRIPT_LAST_DUMMY_SIZE 23 // 파일 암호화시 뒤쪽에 넣는 더미 싸이즈
#define HELPSCRIPT_ENCODE_BYTE 0x64     // 암호화 비트. 실제 데이터부에 ^= 연산해서 사용한다. 숫자는 막 정한거지만 데이터를 읽어오는 쪽 소스에서도 같게 맞춰줘야한다.

enum { HELP_CON_TYPE_F_LEVELUP = 0, HELP_CON_TYPE_A_LEVEL, HELP_CON_TYPE_F_ITEM, HELP_CON_TYPE_F_SERVER
	   , HELP_CON_TYPE_F_NPC_IN, MAX_HELP_CON_TYPE_NUM
	 }; // 조건 타입. F는 강제 실행 조건, A는 수시 체크 조건.
enum { HELP_EXCEP_CODE_ONLY_ACHERON = 1, HELP_EXCEP_CODE_SERVER, MAX_EXCEP_CODE_NUM }; // 예외 사항 타입.(중복이 안되니까 만약 중복 조건이 생기면 코드를 하나 추가하자.

struct sHelpData
{
	int m_Index; // 헬프의 인덱스(내부적으로만 가지고 있는다.)
	int m_ConditionType; // 헬프의 조건 종류
	char *m_strConArg; // 헬프의 조건 인자.
	int m_CoolTime; // 직접 끈게 아닐때 반복하는 틱.
	int m_ExceptionCode; // 뭔가 예외처리가 필요한 경우에 사용. (예: 아케론 서버에서만 동작,)

	char *m_strSearchTitle; // 검색 결과 표시 제목.
	char *m_strSearchWord1; // 검색 키워드1
	char *m_strSearchWord2; // 검색 키워드2
	char *m_strSearchWord3; // 검색 키워드3

	char *m_strNormalTalk; // 일반 상태의 대사.
	char *m_strChaoticTalk; // 카오 상태의 대사.

	vector<char *>		m_listImage; // 상세 설명 이미지의 리스트.

	DWORD m_ActivateTick; // 발동된 틱.

	sHelpData()
	{
		m_strConArg = NULL; // 헬프의 조건 인자.
		m_strSearchTitle = NULL; // 검색 결과 표시 제목.
		m_strSearchWord1 = NULL; // 검색 키워드1
		m_strSearchWord2 = NULL; // 검색 키워드2
		m_strSearchWord3 = NULL; // 검색 키워드3
		m_strNormalTalk = NULL; // 일반 상태의 대사.
		m_strChaoticTalk = NULL; // 카오 상태의 대사.

		Init();
	}

	~sHelpData()
	{
		Init();
	}

	void Init()
	{
		m_Index = -1; // 헬프의 인덱스(내부적으로만 가지고 있는다.)
		m_ConditionType = -1; // 헬프의 조건 종류
		m_CoolTime = -1; // 직접 끈게 아닐때 반복하는 틱.
		m_ExceptionCode = -1;

		m_ActivateTick = 0; // 발동된 틱.

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

		Dest->Init(); // 초기화하고.

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

		Dest->m_ActivateTick = this->m_ActivateTick; // 사실 이건 필요없지만...


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

			pHelp->Init(); // 이걸 일일이 해줘서 메모리를 해제해준다.
			SAFE_DELETE( pHelp );
		}
		m_listHelp.clear();
	}

	BOOL LoadDta( const char* FileName );
	void SaveDta( const char* FileName ); // .dta 로 저장

	void ExportScriptToText( const char* FileName ); // 텍스트로 저장
	void ImportScriptFromText( const char* FileName ); // 텍스트로부터 읽어와서 구성

	int m_TotalHelpNum;
	list<sHelpData*>		m_listHelp; // 씬들 정보.
};


#endif