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



	// 심의 등급 표시용으로 추가한 도움말 페이지 및 메뉴항목.
	BOOL m_bViewKrServiceLv; // 심의 등급이 보여지는 중인가...
	CSurface*  m_pSyntexKrServiceLv; // 도움말이 들어가는  이미지
	CBasicButton *m_pServiceLvCloseBtn;  // 심의등급 표시창 닫기 버튼. (포인터로 사용에 유의.
	CBasicButton	m_LinkKrServiceLv; // 링크 버튼들


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