// ControlStatus.h: interface for the CControlStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLSTATUS_H__6D0CC5E3_4CB2_4BB1_8266_C39C76544F33__INCLUDED_)
#define AFX_CONTROLSTATUS_H__6D0CC5E3_4CB2_4BB1_8266_C39C76544F33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicStatic.h"
#include "TextOutBox.h"

class CNkCharacter;

class CControlStatus
{
public:
	CBasicStatic	m_background;
	CBasicStatic	m_face;
	CBasicStatic	m_hp;
	CBasicStatic	m_mp;
	CBasicStatic	m_ep;
	CBasicStatic	m_sp;
	CTextOutBox		m_textout[3];


public:
	bool	m_face_load;

	void	LoadRes_Char();

	int GetWidth()
	{
		return m_background.m_w;
	}
	int GetHeight()
	{
		return m_background.m_h;
	}


public:
	bool IsIn(int x, int y)
	{
		return m_background.IsIn(x,y);
	}
	void SetNkChar();
	void SetName();


public:
	void Init();
	void LoadRes();
	void DeleteRes();


public:
	void Draw();
	void Draw_Bar(CBasicStatic* ui , int base_value, int max_value);
	void Draw_Percent( CTextOutBox* textout , int base_value, int max_value);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	HRESULT RestoreSurfaces();

public:
	CControlStatus();
	virtual ~CControlStatus();

};

#endif // !defined(AFX_CONTROLSTATUS_H__6D0CC5E3_4CB2_4BB1_8266_C39C76544F33__INCLUDED_)
