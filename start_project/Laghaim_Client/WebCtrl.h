#ifndef _WEB_CTRL_H_
#define _WEB_CTRL_H_

#include "Config.h"

//////////////////////////////////////////////////////////////////
// * 주의 [1] windows.h를 쓰고 있으면 afxwin.h로 바꾼다			//
//        [2] Poject->Settings->C/C++의 Project Option에서		//
//			  (Debug Mode) MLD => MTD, (Release Mode) ML => MT	//
//////////////////////////////////////////////////////////////////
//#include <afxdtctl.h>
#include <exdisp.h>
//#include <atlconv.h>

class CWebCtrl
{
public:

//------------------------ 멤 버 함  수 ------------------------//
	CWebCtrl();													// Constructor
	virtual ~CWebCtrl();										// Destructor

	////////////////////
	// 클래스 세팅 함수
	void SetParentHwnd(HWND hWnd);								// Set Parent Hwnd
	BOOL OpenWeb();												// Initialize Web Wrapper Class
	void CloseWeb();											// Destroy Web Wrapper Class
	BOOL ShowWeb();												// Show Web Window

	/////////////////////////
	// 기타 함수
	HWND GetWebHwnd();

	//////////////////
	// 웹 콘트롤 함수
	BOOL GoHome();
	BOOL Navigate(char* szAddr = NULL);
	void Refresh();

	void SetBrowserSize( int Width, int Height ); // 브라우져 사이즈를 조절해서 스크롤 나올수있도록 한 녀석. (원석 추가)


protected:
	void SetBrowserStyle();
//------------------------ 멤 버 변 수 -------------------------//
protected:
	IWebBrowser2*			m_pWebBrowser;
	HWND					m_hWnd_Parent;
	HWND					m_hWnd_Explorer;
	bool					m_bComInitSuccess;
};


class CWebWnd // 실제 게임상에 출력에 사용되어지는 클래스.
{
public:
	CWebWnd();
	~CWebWnd();

	void Init();

	void LoadRes(int type = 0);
	void DeleteRes();

	void Open( char *Url, int x, int y, int Width, int Height );
	void Draw();
	void Close();

	RECT m_WebRect;

	CWebCtrl m_WebCtrl; // 웹 브라우져를 만들어주는 클래스.
	HWND m_WebHwnd; // 실제 브라우져를 찍을 윈도우
//	HWND m_DummyHwnd; // 클릭을 막기위한 더미 윈도우.

	POINT oldLaghaimPT; // 창 이동했나 체크용.
};

#endif // _WEB_CTRL_H_
