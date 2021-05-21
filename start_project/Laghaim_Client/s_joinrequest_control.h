#ifndef _JOIN_REQUEST_H_
#define _JOIN_REQUEST_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include <vector>
#include <algorithm>

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "TextOutBox.h"
#include "Scroll.h"
#include "guild_data.h"

using namespace std;

class CSurface;

class CBunch
{
public:
	static CBunch* Create(int nBaseX = 0, int nBaseY = 0, int nTopLine = 0, int nBottomLine = 0);
	~CBunch();
public:
	void Init();
	void LoadRes();

	void Draw(int nScrollPos);
	BOOL DrawToBlt(CSurface* pSurface, int nScrollPos);

	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition(int nPosX, int nPosY, int nNum);
	void SetRePosition(int nNum, int nPosY);
	void Restore();

	void SetCharName(char* pName);
	void SetTime(t_Info nTime);
	void SendMsg();
	void SetCharIdx(int nIdx)
	{
		m_nCharIdx = nIdx;
	}

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll);
public:
	CSurface* m_pInputBox;

	CTextOutBox nameBox;	// ĳ���� �̸�
	CTextOutBox	dateBox;	// ��¥

	CBasicButton btOk;
	CBasicButton btCancel;

	int		m_nBaseX;		// ���� UI�� �⺻ ��ǥ X
	int		m_nBaseY;		// ���� UI�� �⺻ ��ǥ Y
	int		m_nRequestNum;
	int		m_nTopLimit;	// �׷����� ����
	int		m_nBottomLimit; // �׷����� ����

	int		m_nSequence;	// ���� ����

	HWND	m_hWnd;
	RECT    m_ClientRc;

	char	m_MsgBuf[1024];

	int		m_nCharIdx;

private:
	CBunch(int nBaseX = 0, int nBaseY = 0, int nTopLine = 0, int nBottomLine = 0);
};

/**********************************************************************************************/

class CJoinRequestControl
{

public:
	HWND	m_hWnd;
	RECT    m_ClientRc;

	int		m_nBaseX;
	int		m_nBaseY;

	CScroll			m_R_Scroll;
	CScroll			m_G_Scroll;

	CSurface*	m_pTitle;
	CSurface*	m_pRecommanBox;
	CSurface*	m_pRequestBox;

	//////////////////////////////////////////////////////////////////////////////////////
	vector<CBunch*>	m_R_Waitlist;	// R = Recommand
	vector<CBunch*>	m_G_Waitlist;	// G = General

	int			m_nSelectType;		// 0�� : ��õ, 1�� : �Ϲ�
	int			m_nRSelectNum;		// ���õȰ�
	int			m_nGSelectNum;		// ���õȰ�

	int			m_nRCnt;
	int			m_nGCnt;


public:
	static CJoinRequestControl* Create(int nBaseX = 0, int nBaseY = 0);
	~CJoinRequestControl();

	void Init();
	void LoadRes();

	void Draw();
	BOOL DrawToBlt(CSurface* pSurface);

	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition();
	void Restore();

	void SetTotalNum(int nRecommnad, int nGeneral);

	void Insert_list();
	void Insert_list(guild_member& g_member);

	void EraseData(int charindex);
	void EraseData();
	void SetDataReset();

	int IsExist(int nCharIdx, int type);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private :
	CJoinRequestControl(int nBaseX = 0, int nBaseY = 0);
};
#endif