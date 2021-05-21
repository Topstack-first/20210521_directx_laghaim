#ifndef _WANTED_LIST_H_
#define _WANTED_LIST_H_

#include "Config.h"

#include "wanted_data.h"
#include "baseTgaWindow.h"
#include "TextOutBox.h"
#include "Scroll.h"

class CWantedList : public CUITgaBaseWindow
{
public:
	enum
	{
		TEXT_REG_NUM,
		TEXT_WANTED_NAME,
		TEXT_WANTED_MONEY,
		TEXT_WANTED_RACE,
		TEXT_WANTED_CONNECT,
		TEXT_WANTED_REMAINTIME,
		TEXT_TOTAL
	};
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

		void SetBasePosition(int nPosX, int nPosY);
		void SetPosition(int nPosX, int nPosY, int nNum);
		void SetRePosition(int nNum);
		void SetCharIdx(int nIdx)
		{
			m_nCharidx = nIdx;
		}
		void Restore();

		void SetRegisterNum(char* pNum);
		void SetWantedName(char* pName);
		void SetWantedMoney(char* pName);
		void SetWantedRace(char* pName);
		void SetConnect(char* pName);
		void SetRemainTime(t_Info time_remain);

		void SendMsg();

		LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll);
	public:
		CTextOutBox  m_WantedText[TEXT_TOTAL];

		int		m_nBackPosX;		// 공통 UI의 기본 좌표 X
		int		m_nBackPosY;		// 공통 UI의 기본 좌표 Y

		int		m_nSequence;		// 나의 순서

		int		m_nCharidx;

		HWND	m_hWnd;
		RECT    m_ClientRc;

		char	m_MsgBuf[1024];

	private:
		CBunch(int nBaseX = 0, int nBaseY = 0);
	};

	vector<CBunch*> m_register_list;

public:
	CWantedList();
	virtual ~CWantedList();

public:
	/*********************************************************************/
	int		Render();
	void	Init();
	void	RestoreSurfaces() {};
	void	DeleteRes();
	BOOL    IsInside(int x, int y);
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	/*********************************************************************/
	BOOL	InitMode();
	void	LoadRes();

	BOOL	IsMouseUpperWindow(int x, int y)
	{
		return TRUE;
	}
	BOOL	MoveWindow(UINT msg, int x, int y);
	void	SetPosition();
	/*********************************************************************/

	void	Insert_list(wanted_info& info);
	void	list_all_reset();
	void	Delete_list();
	void	SendMsg();
private :
	CBasicButton	m_btRefrsh;	// 새로고침
	CBasicButton	m_btBack;
	CScroll			m_Scroll;

	int				m_nTotalCnt;
	int				m_nSelect;

	char			m_szMsg[1024];
};

#endif // _WANTED_LIST_H_