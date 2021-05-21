#pragma once

#include "LHUI_Base.h"

class CBasicStatic;
class CBasicButton;
class CListBox;

#define ZONE_MAX 29

struct ZONE_LIST
{
	int zone_no;
};

class LHUI_Gate : public LHUI_Base
{
public:
	CBasicStatic * m_background;
	CBasicButton * m_btn_close;
	CListBox * m_lst_zone;

	int m_slot_index;
	int m_num_zonelist;
	int m_zone_num[ZONE_MAX]; // 일단은 존개수


public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:
	bool IsActivate()
	{
		return (_GetDisalbe() && !_GetVisible())?false:true;
	}
	BOOL IsInside(int x, int y);

	void SetSlot(int slot_index)
	{
		m_slot_index = slot_index;
	}
	void SetZoneList(int value , bool direct=false);
	bool AddList(int zone_no);
	const char * GetZoneName(int zone_no);

	void ExceptionItem();
	void ResetAll();


	bool CheckRestrict(int zone_index);

public:
	LHUI_Gate(void);
	virtual ~LHUI_Gate(void);
};
