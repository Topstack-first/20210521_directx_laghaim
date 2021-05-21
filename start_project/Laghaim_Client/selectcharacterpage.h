#ifndef _SELECT_CHARACTER_PAGE_H_
#define _SELECT_CHARACTER_PAGE_H_

#include "headers.h"
#include "BasicButton.h"
#include "ProgressBox.h"
#include "TextOutBox.h"
#include "Sheet.h"
#include "MsgPopUp.h"

//1114
#include "Nk2DFrame.h"
#include "ControlBottom.h"

#define SELCHAR_VAL_X		108
#define SELCHAR_VAL_Y		271
#define SELCHAR_VAL_WIDTH	150
#define SELCHAR_VAL_HEIGHT	23
#define SELCHAR_VAL_GAP		37
#define PRO_VITAL_IMG	"interface/pro_vital.bmp"
#define PRO_MANA_IMG	"interface/pro_mana.bmp"
#define PRO_EPOWER_IMG	"interface/pro_epower.bmp"
#define PRO_STAMINA_IMG	"interface/pro_stamina.bmp"
#define PRO_BACK_IMG	"interface/pro_Back.bmp"
#define SELCHAR_VAL_COLOR	RGB(205, 205, 205)

#define SELCHAR_BTN_X		471
#define SELCHAR_BTN_Y		270
#define SELCHAR_BTN_WIDTH	96
#define SELCHAR_BTN_HEIGHT	39
#define SELCHAR_BTN_GAP		65

#define SELCHAR_STAT_X		458
#define SELCHAR_STAT_Y		295
#define SELCHAR_STAT_GAP	23
#define SELCHAR_STAT_FSIZE	16
#define SELCHAR_STAT_FCOLOR	RGB(255, 255, 0)

#define SLOT_X			49
#define SLOT_Y			114

#define _SLOT_WIDTH		530
#define _SLOT_HEIGHT	155

#define SLOT_GAP		(_SLOT_WIDTH/SLOT_COUNT)

#define SLOT_NAME_X			80
#define SLOT_NAME_Y			160
#define SLOT_NAME_FSIZE		12
#define	SLOT_NAME_FCOLOR	RGB(255, 180, 0)

#define SELCHAR_POPUP_NUM	1

class CSheet;
class CSelectCharacterPage;
class CMsgPopUp;

class CSelectCharacterPage
{
public:
	CSheet*			m_pSheet;
	CSurface*		m_pBatangSur;
	CSurface*		m_pNameActive;
	CSurface*       m_pNameDefault;
	CSurface*		m_pTitleBack;
	CSurface*		m_pBtnBack;

	CBasicButton	m_StartBtn;
	CBasicButton	m_NewBtn;
	CBasicButton	m_DeleteBtn;
	CBasicButton	m_BackBtn;
	CBasicButton	m_ExitBtn;

	CProgressBox	m_VitalBox;
	CProgressBox	m_ManaBox;
	CProgressBox	m_StaminaBox;
	CProgressBox	m_EpowerBox;

	CTextOutBox		m_StrBox;
	CTextOutBox		m_IntBox;
	CTextOutBox		m_DexBox;
	CTextOutBox		m_ConBox;
	CTextOutBox		m_ChaBox;

	CTextOutBox		m_Level[SLOT_COUNT];

	CTextOutBox		m_SlotName[SLOT_COUNT];

	CMsgPopUp		m_MsgPopUp;
	CMsgPopUp		m_MsgPopUpNew;
	CSurface*		m_pTmpStic[SLOT_COUNT]; // 케릭터 이름아래의 바탕 그림

	int m_DownSlotNum;
	BOOL m_bNullSlot;

	CSelectCharacterPage();
	CSelectCharacterPage( CSheet* pSheet );
	~CSelectCharacterPage();
	void Action( int i );
	void SetSheetPtr( CSheet* pSheet );
	void Init();
	void SetBackground();
	void LoadRes();
	void DeleteRes();
	void Draw();
	int  GetSlotNum(int x, int y);
	void ChangeSlot(int slot_num);
	void SetBoxesVisible(BOOL bVisible);
	void SetCharNames(void);
	void SetCharLevel(void);
	void MsgProc( HWND, UINT, WPARAM, LPARAM );


	////////////////////////////////////////////////////////////
	// Animation Page
	BOOL	m_bStartAni;
	DWORD	m_dwStartTime;
	void	SetStartAni(BOOL bStart);

	// 계정 결제 만료일 표시 용
	DWORD m_UserIndex; // 계정 만료일을 알기 위해 필요한 유저 인덱스.
	SYSTEMTIME m_FlatRateDay; // 정액 요금의 년/월/일을 저장하기 위한 녀석
	char m_strFlatRateTime[20]; // 정액 요금의 시/분을 저장하기 위한 녀석.
	int m_MeterRateTime; // 종량 요금제의 남은 분.
	CTextOutBox		*m_pFlatRateText; // 출력 용
	CTextOutBox		*m_pMeterRateText; // 출력 용.

	void ResetAccountEnd(); // 계정만료일 표시 기능 초기화.
	void GetAccountEndString( int UserIndex ); // 웹 페이지로 부터 받아온 스트링을 계정 만료일 정보로 만든다.
	void SetAccountEndInfo( char *strAccountEnd ); // 웹 페이지로 부터 받아온 스트링을 계정 만료일 정보로 만든다.
	void DrawAccountEnd(); // 만료일 출력.


};

#endif // _SELECT_CHARACTER_PAGE_H_