#ifndef _CONTROL_PET_STATUS_H_
#define _CONTROL_PET_STATUS_H_

#include "Config.h"
#include "ControlInven.h"

class CSurface;
class CTextOutBox;
class CBasicButton;
class CTcpIpConnection;
//class CNkMyPet;


#define NUM_MAX_INFO  13

#define SIZE_OF_SKILL_ICON 26 // 스킬아이콘의 한변 길이.

enum PetState_Info
{
	PetState_Info__Name,
	PetState_Info__Lv,
	PetState_Info__Exp,
	PetState_Info__x1,
	PetState_Info__x2,
	PetState_Info__Hp,
	PetState_Info__NA,
	PetState_Info__FA,
	PetState_Info__ND,
	PetState_Info__FD,
	PetState_Info__MixLv,
	PetState_Info__Speed,
	PetState_Info__GetLv,

	PetState_Info__EndOfEnum
};

struct INFO
{
	char  str[40]; // 빽업용이다.
	CTextOutBox * text_sur;
	int x, y; // 텍스트 x,y 좌표
	int cx;   // 이건 빡스좌표

};


class CControlPetStatus
{
public:

	INFO		   m_Info[NUM_MAX_INFO];
	CSurface     * m_pBack;
	CSurface	 * m_pBackNew;
	CBasicButton * m_pBtn_Close;
	int			m_ScreenCx, m_ScreenCy, m_iMainX, m_iMainY, m_ModeYGap;
	int			m_NeckXpos, m_NeckYpos;
	BOOL		m_bBagLock;
	DWORD	m_timeInvenLock;

	int m_DrawSkillIconX, m_DrawSkillIconY;
	CBasicButton *m_pSkillIcon[PET_SKILL_MAX*PET_EVOLUTION_TYPENUM]; // 스킬을 배웠는지의 아이콘. 스킬 갯수 만큼 가지고 있는다.
	CAlphaPopUp		*m_pSkillIconName; // 스킬 명을 표시해주기 위한 팝업.
	CEquipSlot		m_EquipSlot[PET_PIECE_NUM];

protected:
	int m_slot_index; // 퀵슬롯 사용시 설정
	int m_newpet_offset_y;

public:
	void SetSlot(const int slot)
	{
		m_slot_index = slot;
	}


public:
	CControlPetStatus();
	virtual ~CControlPetStatus();
	CItem*	Draw();
	CItem*	DrawEquip();
	void	LoadRes();
	void	DeleteRes();
	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInside(int x, int y);
	BOOL	IsBagSide(int x, int y);
	BOOL	IsNecklaceSide(int x, int y);
	void	RestoreSurfaces();
	void	SetPetName(char* name);
	void	SetPetStatus(int type, __int64 val, int val2 = 0 );
	void	LoadDLLStringTable();
	void	DeleteDLLStringTable();
	LRESULT NamePopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
	void	Reposition();
	LRESULT	ProcInven(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL	IsInsideEquip(int x, int y);
	void	SetEquipArea();
};

#endif // _CONTROL_PET_STATUS_H_
