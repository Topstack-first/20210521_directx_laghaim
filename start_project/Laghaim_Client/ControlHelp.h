#ifndef _CONTROL_HELP_H_
#define _CONTROL_HELP_H_

#include "Config.h"

#include "Nk2dframe.h"
#include "BasicButton.h"
#include "BasicStatic.h"

#define  SYNTEX_ALL_NUM  19

class CSurface;

class CControlHelp
{
public:
	int  m_cur_index;

public:
	CBasicStatic	m_background;
	CBasicButton	m_btn_close;

	CBasicButton	m_tab_01;
	CBasicButton	m_tab_02;

	CBasicButton	m_btn_list;
	CBasicButton	m_btn_prev;
	CBasicButton	m_btn_next;

	CBasicStatic	m_help[SYNTEX_ALL_NUM];
	CBasicButton	m_btn_link[SYNTEX_ALL_NUM];



	// ���� ��� ǥ�ÿ����� �߰��� ���� ������ �� �޴��׸�.
	BOOL m_bViewKrServiceLv; // ���� ����� �������� ���ΰ�...
	CSurface*  m_pSyntexKrServiceLv; // ������ ����  �̹���
	CBasicButton *m_pServiceLvCloseBtn;  // ���ǵ�� ǥ��â �ݱ� ��ư. (�����ͷ� ��뿡 ����.
	CBasicButton	m_LinkKrServiceLv; // ��ũ ��ư��


public:
	CControlHelp();
	~CControlHelp();

	void LoadRes();
	void Draw();
	void DrawSyntex(int index);
	void DeleteRes();
	void SelectDeleteRes();
	BOOL IsInside(int, int);
	BOOL IsInsideServiceLv(int, int);

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


};

#endif // _CONTROL_HELP_H_