#ifndef _LHUI_OpEvent_H_
#define _LHUI_OpEvent_H_

#include "BasicStatic.h"
#include "TextOutBox.h"

enum OpEvent_Type
{
	OpEvent_Type__Pc,
	OpEvent_Type__Pre,
	OpEvent_Type__Drop,
	OpEvent_Type__Att,
	OpEvent_Type__Def,
	OpEvent_Type__Hp,
	OpEvent_Type__Exp,
	OpEvent_Type__Rush,
	OpEvent_Type__Normal,	// 일반결제자
	OpEvent_Type__LuckyTime,
	OpEvent_Type__Less_Exp,
	OpEvent_Type__Fever_Time,

	OpEvent_Type__EndOfEnum
};



enum OpEvent_Ex_Type
{
	OpEvent_Ex_Type__Att,
	OpEvent_Ex_Type__Def,
	OpEvent_Ex_Type__Exp,

	OpEvent_Ex_Type__EndOfEnum
};






struct OPEVENT
{
	int				value;
	CBasicStaticEx	image;
};

struct OPEVENT_EX
{
	int				value;
	CBasicStaticEx	image;
};

struct OPEVENT_PRIVATE
{
	int				value;
	int				tick;
	CBasicStaticEx	image;
};

class CAlphaPopUp;

class LHUI_OpEvent
{
	DWORD			m_dwTime;
	bool			m_bImgDraw;
	float			m_fPcLpTime;

public:
	bool			m_activate;
	OPEVENT			m_op_event[OpEvent_Type__EndOfEnum];


	bool			m_activate_ex;
	OPEVENT_EX		m_op_event_ex[OpEvent_Ex_Type__EndOfEnum];

	CAlphaPopUp *	m_over_info;


public:
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	bool IsInside( int x, int y );
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


public:
	bool IsActivate();
	void SetEvent( int type , int value );

	void SetEvent_Ex( int type , int value );


	void SetPcLpTime(float time)
	{
		m_fPcLpTime = time;
	}


public:
	LHUI_OpEvent(void);
	~LHUI_OpEvent(void);
};
#endif // _LHUI_OpEvent_H_