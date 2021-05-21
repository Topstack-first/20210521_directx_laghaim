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
	CSurface*		m_pBack;        // 배경화면
	CBasicButton	m_LoginBtn;		// 접속 버튼
	CBasicButton	m_MakeAcntBtn;	// 계정만들기 버튼
	CBasicButton	m_SetOptBtn;	// 욥션 버튼
	//CBasicButton	m_TutorBtn;		// 연습 버튼
	CBasicButton	m_ExitBtn;		// 종료 버튼




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

	// 홈페이지로 이동 처리
	char m_AddRes[256];		// 홈페이지 URL 주소를 저장할 공간
	void Read_URL_Add(char *filename);  // 파일을 읽고 m_AddRes에 스트링을 복사한다

};

#endif // _START_PAGE_H_