#ifndef _MEMBER_CHANGE_H_
#define _MEMBER_CHANGE_H_

#include "Config.h"

#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "TextOutBox.h"
#include "Scroll.h"
#include "guild_data.h"

class CSurface;
class CCheckbutton;


class CMemberControl
{
public:
	class CBunch
	{
	public:
		static CBunch* Create(int nBaseX = 0, int nBaseY = 0);
		~CBunch();
	public:
		void Init();
		void LoadRes();

		void Draw(int nScrollPos);
		BOOL DrawToBlt(CSurface* pSurface, int nScrollPos);

		void DeleteRes();
		BOOL IsInside(int, int);

		void SetPosition(int nPosX, int nPosY, int nNum);
		void SetRePosition(int nNum);
		void SetCharIdx(int nIdx);
		void Restore();


		void SetCharName(char* pName);
		void SendMsg();

		LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll);
	public:

		CTextOutBox m_TCharName;

		CBasicButton m_BtFire;
		CCheckbutton *m_pCheckBt;

		int		m_nBaseX;		// 공통 UI의 기본 좌표 X
		int		m_nBaseY;		// 공통 UI의 기본 좌표 Y

		int		m_nSequence;	// 나의 순서
		int		m_nCharidx;

		HWND	m_hWnd;
		RECT    m_ClientRc;

		char	m_MsgBuf[1024];

	private:
		CBunch(int nBaseX = 0, int nBaseY = 0);
	};

	vector<CBunch*>	m_listMember;

public:
	HWND	m_hWnd;
	RECT    m_ClientRc;

	CSurface*  m_pTitle;
	CSurface*  m_pBBlack;

	int		m_nBaseX;
	int		m_nBaseY;

	CScroll m_Scroll;

	int		m_nSelectNum;
	int		m_nTotalNum;

public:

	static CMemberControl* Create(int nBaseX = 0, int nBaseY = 0);


	~CMemberControl();

	void Init();
	void LoadRes();
	void Draw();
	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition();

	void SetTotalNum(int nGeneral);

	void Insert_list();
	void Insert_list(guild_member& g_member);
	void Delete_list(guild_member& g_member);
	void Refresh_list(guild_member& g_member);

	void Restore();

	void EraseData();
	void SetDataReset();
	void SetListAllReset();


	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	CMemberControl(int nBaseX = 0, int nBaseY = 0);
};
#endif