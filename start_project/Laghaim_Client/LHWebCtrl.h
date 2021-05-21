#pragma once

class LHWebCtrl
{
public:
	IWebBrowser2 * m_pWebBrowser;

	HWND m_hWnd_Parent;
	HWND m_hWnd_Explorer;


public:
	bool Init(HWND hParent);
	void Open();
	void Close();


public:
	LHWebCtrl(void);
	virtual ~LHWebCtrl(void);
};
