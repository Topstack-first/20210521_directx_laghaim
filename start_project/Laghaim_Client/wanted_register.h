#ifndef _WANTED_REGISTER_H_
#define _WANTED_REGISTER_H_

#include "Config.h"

#include "baseTgaWindow.h"
#include "TextBox.h"
#include "TextOutBox.h"
#include "ComboBox.h"

class CWantedRegister : public CUITgaBaseWindow
{
public:
	CWantedRegister();
	virtual ~CWantedRegister();

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

	void	SendMsg();
	void	SetRaceName(char* szRaceName);
	int	    GetTerm(int nNum);

private :
	CBasicButton	m_btWantedStart;	// 수배
	CBasicButton	m_btExistCheck;		// 존재여부확인
	CBasicButton	m_btBack;

	CTextBox		m_TextCharName;		// 캐릭터명 입력
	CTextOutBox		m_TextRace;			// 종족 표시
	CComboBox		m_ComboTerm;		// 수배기간
	CTextBox		m_TextWantedMoney;	// 현상금 입력

	char			m_szMsgBuffer[1024];

	DWORD			m_beforTiem;

};

#endif // _WANTED_REGISTER_H_