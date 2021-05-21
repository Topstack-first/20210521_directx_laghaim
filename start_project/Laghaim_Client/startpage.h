#ifndef _START_PAGE_H_
#define _START_PAGE_H_

#include "Config.h"

#include "headers.h"
#include "BasicButton.h"


#define START_BTN_X				260
#define START_BTN_Y				138
#define START_BTN_WIDTH			96
#define START_BTN_HEIGHT		39
#define START_BTN_GAP			60

class CSheet;


class CStartPage
{
public:
	CSheet*			m_pSheet;
	CSurface*		m_pBack;        // ���ȭ��
	CBasicButton	m_LoginBtn;		// ���� ��ư
	CBasicButton	m_MakeAcntBtn;	// ��������� ��ư
	CBasicButton	m_SetOptBtn;	// ��� ��ư
	//CBasicButton	m_TutorBtn;		// ���� ��ư
	CBasicButton	m_ExitBtn;		// ���� ��ư




	CStartPage();
	CStartPage( CSheet *pSheet );
	~CStartPage();
	void SetSheetPtr( CSheet *pSheet );
	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	void MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Action( int i );

	// Ȩ�������� �̵� ó��
	char m_AddRes[256];		// Ȩ������ URL �ּҸ� ������ ����
	void Read_URL_Add(char *filename);  // ������ �а� m_AddRes�� ��Ʈ���� �����Ѵ�

};

#endif // _START_PAGE_H_