#ifndef _CREATE_CHARACTER_PAGE_H_
#define _CREATE_CHARACTER_PAGE_H_

#include "Config.h"

#include "headers.h"
#include "BasicButton.h"
#include "TextBox.h"
#include "SpinBox.h"
#include "MsgPopUp.h"

#include "ChaAct.h"

char *ExamSpChar(char *str);		//2바이트 특수문자 블럭
char *ExamSpCharOne(char *str);		//1바이트 특수문자 블럭
bool ExamNumOnly(char *str);		//숫자로만 구성된 이름 검사
bool strinc(char *str, const char *inc);
bool IsTowByte(char str);
char *ExamSpChar_for_jpn(char *str, bool bKata);
char *ExamEscChar_for_jpn(char *str);
BOOL Str2Str(char * str1, char * str2 );

#define CRECHAR_BTN_X			179
#define CRECHAR_BTN_Y			HORIZ_BTN_Y
#define CRECHAR_BTN_WIDTH		HORIZ_BTN_WIDTH
#define CRECHAR_BTN_HEIGHT		HORIZ_BTN_HEIGHT
#define CRECHAR_BTN_GAP			140

#define CRECHAR_EDIT_X		160
#define CRECHAR_EDIT_Y		74
#define CRECHAR_EDIT_FSIZE	12
#define CRECHAR_EDIT_FCOLOR	RGB(255, 255, 255)

//==============================================================
// nate 2004 - 12
// interface
// 좌표값 수정
#define CRECHAR_SPIN_WIDTH	177

#define CRECHAR_SPIN_IMGWIDTH	20
#define CRECHAR_SPIN_IMGHEIGHT	26
#define CRECHAR_SPIN_FSIZE	14
#define CRECHAR_SPIN_FCOLOR	RGB(255, 255, 255)


#define CRECHAR_RACE_X		84
#define CRECHAR_RACE_Y		176
#define CRECHAR_SEX_X		84
#define CRECHAR_SEX_Y		203
#define CRECHAR_HAIR_X		84
#define CRECHAR_HAIR_Y		230

#define CRECHAR_CHA_X		290
#define CRECHAR_CHA_Y		135

#define RACE_INFO_X			34
#define RACE_INFO_Y			265

#define DENY_STRING_NUM	10
//==============================================================

#define CRECHAR_POPUP_NUM	2
class CSheet;
class CCreateCharacterPage;
class CSetValuePage;
class CMsgPopUp;
class LHUI_Captcha;

class CCreateCharacterPage
{
public:
	CSheet*			m_pSheet;
	CSetValuePage * m_pSVPage;
	CSurface*		m_pBatangSur;
	CBasicButton	m_NextBtn;
	CBasicButton	m_BackBtn;

	CTextBox		m_NameBox;
	CSpinBox		m_RaceBox;
	CSpinBox		m_SexBox;
	CSpinBox		m_HairBox;

	CSurface*		m_pInfo[ NUM_RACE ];
	CMsgPopUp		m_MsgPopUp;
	int				m_iOldRaceNum;
	char*			m_Deny_String[DENY_STRING_NUM];

	LHUI_Captcha *	m_captcha;

	CCreateCharacterPage();
	CCreateCharacterPage( CSheet* pSheet );
	~CCreateCharacterPage();

	void SetSheetPtr( CSheet* pSheet );

	void Init();
	void SetBackground();


	void LoadRes();

	void DeleteRes();

	void Draw();

	void MsgProc( HWND, UINT, WPARAM, LPARAM );
	void Action( int );
	BOOL CheckSpace(char* String);
	void OutoSetValue(int Race);
	// nate 2004 - 12 : new race
	void SetRaceBox( BYTE byCanCharCreate );
	void ReadProperList(char* szFileName);	// nate 2006-03-17 : Proper List
	char**	m_pszProperList;		// 캐릭명 필터링 리스트
	int		m_nProperListCount;		// 캐릭명 필터링 갯수

	HRESULT Restore();
};

#endif // _CREATE_CHARACTER_PAGE_H_