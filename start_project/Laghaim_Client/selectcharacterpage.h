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
	CSurface*		m_pTmpStic[SLOT_COUNT]; // �ɸ��� �̸��Ʒ��� ���� �׸�

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

	// ���� ���� ������ ǥ�� ��
	DWORD m_UserIndex; // ���� �������� �˱� ���� �ʿ��� ���� �ε���.
	SYSTEMTIME m_FlatRateDay; // ���� ����� ��/��/���� �����ϱ� ���� �༮
	char m_strFlatRateTime[20]; // ���� ����� ��/���� �����ϱ� ���� �༮.
	int m_MeterRateTime; // ���� ������� ���� ��.
	CTextOutBox		*m_pFlatRateText; // ��� ��
	CTextOutBox		*m_pMeterRateText; // ��� ��.

	void ResetAccountEnd(); // ���������� ǥ�� ��� �ʱ�ȭ.
	void GetAccountEndString( int UserIndex ); // �� �������� ���� �޾ƿ� ��Ʈ���� ���� ������ ������ �����.
	void SetAccountEndInfo( char *strAccountEnd ); // �� �������� ���� �޾ƿ� ��Ʈ���� ���� ������ ������ �����.
	void DrawAccountEnd(); // ������ ���.


};

#endif // _SELECT_CHARACTER_PAGE_H_