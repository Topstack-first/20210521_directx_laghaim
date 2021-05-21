#ifndef _GRADE_CHANGE_H_
#define _GRADE_CHANGE_H_

#include "Config.h"

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "TextBox.h"

class CSurface;
// 길드 계급명 변경
class CGradeChangeControl
{
public:
	enum
	{
		GRADE_TITLE = 0,
		GRADE_MASTER,
		GRADE_ASSISTMASTER,
		GRADE_KNIGHT,
		GRADE_MEMBER,
		GRADE_TOTAL
	};

	enum
	{
		CHANGE_MASTER = 0,
		CHANGE_ASSISTMASTER,
		CHANGE_KNIGHT,
		CHANGE_MEMBER,
		CHANGE_TOTAL
	};
public:
	HWND	m_hWnd;
	RECT    m_ClientRc;

//	CBasicButton	m_BtSave;4
//	CBasicButton	m_BtCancel;
	CBasicButton	m_BtChange[CHANGE_TOTAL];

	int				m_BasePosX;
	int				m_BasePosY;

	CTextBox		m_Textbox[CHANGE_TOTAL];

	CSurface* m_pPiece[GRADE_TOTAL];

	char			m_MsgBuf[1024];

public:
	static CGradeChangeControl* Create(int nBaseX = 0, int nBaseY = 0);

	~CGradeChangeControl();

	BOOL IsFocus();

	void Init();
	void LoadRes();
	void Draw();
	void DeleteRes();
	BOOL IsInside(int, int);

	void SetPosition();

	void Restore();
	void SendMsg();

	void SetGradeName(char* pszname, int nLevel);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL CheckSpace(char* String);
	bool CheckGuldGradeName(int grade);


private:
	CGradeChangeControl(int nBaseX = 0, int nBaseY = 0);
};
#endif