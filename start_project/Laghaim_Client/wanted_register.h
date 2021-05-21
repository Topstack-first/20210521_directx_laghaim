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
	CBasicButton	m_btWantedStart;	// ����
	CBasicButton	m_btExistCheck;		// ���翩��Ȯ��
	CBasicButton	m_btBack;

	CTextBox		m_TextCharName;		// ĳ���͸� �Է�
	CTextOutBox		m_TextRace;			// ���� ǥ��
	CComboBox		m_ComboTerm;		// ����Ⱓ
	CTextBox		m_TextWantedMoney;	// ����� �Է�

	char			m_szMsgBuffer[1024];

	DWORD			m_beforTiem;

};

#endif // _WANTED_REGISTER_H_