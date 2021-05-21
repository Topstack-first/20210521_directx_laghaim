#pragma once

#include "headers.h"
#include "BasicStatic.h"
#include "BasicButton.h"
#include "TextOutBox.h"
#include "MsgPopUp.h"

#include "Nk2DFrame.h"
#include "ControlBottom.h"

class CSheet;


typedef struct _UI_SLOT
{
	CBasicButton slot;
	CTextOutBox	namebox;
	CTextOutBox	levelbox;
	CTextOutBox	desc;

} UI_SLOT, * PUI_SLOT;


class LH_SelectCharacterPage
{
public:
	CSheet*			m_pSheet;


	CMsgPopUp		m_MsgPopUp;
	CMsgPopUp		m_MsgPopUpNew;


public:
	CBasicStatic	m_title;
	CBasicStatic	m_bg_list;
	CBasicStatic	m_blocked;
	CBasicButton	m_btn_start;

	CTextOutBox		m_servername;
	CTextOutBox		m_blockedDesc;
	CTextOutBox		m_blockedDesc2;

	// slot 5°³
	CTextOutBox		m_server_name;


	UI_SLOT			m_slot[SLOT_COUNT];




	CBasicButton	m_btn_new;
	CBasicButton	m_btn_delete;
	CBasicButton	m_btn_back;
	CBasicButton	m_btn_exit;



public:
	int			m_selected_slot;



public:
	DWORD		m_UserIndex;
	SYSTEMTIME	m_FlatRateDay;
	char		m_strFlatRateTime[20];
	int			m_MeterRateTime;
	CTextOutBox	*m_pFlatRateText;
	CTextOutBox	*m_pMeterRateText;


public:
	void Action( int i );
	void SetSheetPtr( CSheet* pSheet )
	{
		m_pSheet = pSheet;
	}
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	void DrawBlockedUI();
	int  GetSlotNum(int x, int y);
	void ChangeSlot(int slot_num);
	void SetCharNames(void);
	void SetCharLevel(void);
	void MsgProc( HWND, UINT, WPARAM, LPARAM );


	void UpdateButton(int slot_num);



public:
	void ResetAccountEnd();
	void GetAccountEndString( int UserIndex );
	void SetAccountEndInfo( char *strAccountEnd );
	void DrawAccountEnd();

	HRESULT Restore();

private:
	void CheckBlock(int select_slot);

public:
	LH_SelectCharacterPage();
	LH_SelectCharacterPage(CSheet* pSheet);
	~LH_SelectCharacterPage(void);
};
