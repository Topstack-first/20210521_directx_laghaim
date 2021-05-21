#ifndef _WEB_CTRL_H_
#define _WEB_CTRL_H_

#include "Config.h"

//////////////////////////////////////////////////////////////////
// * ���� [1] windows.h�� ���� ������ afxwin.h�� �ٲ۴�			//
//        [2] Poject->Settings->C/C++�� Project Option����		//
//			  (Debug Mode) MLD => MTD, (Release Mode) ML => MT	//
//////////////////////////////////////////////////////////////////
//#include <afxdtctl.h>
#include <exdisp.h>
//#include <atlconv.h>

class CWebCtrl
{
public:

//------------------------ �� �� ��  �� ------------------------//
	CWebCtrl();													// Constructor
	virtual ~CWebCtrl();										// Destructor

	////////////////////
	// Ŭ���� ���� �Լ�
	void SetParentHwnd(HWND hWnd);								// Set Parent Hwnd
	BOOL OpenWeb();												// Initialize Web Wrapper Class
	void CloseWeb();											// Destroy Web Wrapper Class
	BOOL ShowWeb();												// Show Web Window

	/////////////////////////
	// ��Ÿ �Լ�
	HWND GetWebHwnd();

	//////////////////
	// �� ��Ʈ�� �Լ�
	BOOL GoHome();
	BOOL Navigate(char* szAddr = NULL);
	void Refresh();

	void SetBrowserSize( int Width, int Height ); // ������ ����� �����ؼ� ��ũ�� ���ü��ֵ��� �� �༮. (���� �߰�)


protected:
	void SetBrowserStyle();
//------------------------ �� �� �� �� -------------------------//
protected:
	IWebBrowser2*			m_pWebBrowser;
	HWND					m_hWnd_Parent;
	HWND					m_hWnd_Explorer;
	bool					m_bComInitSuccess;
};


class CWebWnd // ���� ���ӻ� ��¿� ���Ǿ����� Ŭ����.
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

	CWebCtrl m_WebCtrl; // �� �������� ������ִ� Ŭ����.
	HWND m_WebHwnd; // ���� �������� ���� ������
//	HWND m_DummyHwnd; // Ŭ���� �������� ���� ������.

	POINT oldLaghaimPT; // â �̵��߳� üũ��.
};

#endif // _WEB_CTRL_H_
