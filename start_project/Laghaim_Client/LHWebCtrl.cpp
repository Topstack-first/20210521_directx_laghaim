#include "stdafx.h"
#include "LHWebCtrl.h"

LHWebCtrl::LHWebCtrl(void)
	:	m_hWnd_Parent(NULL)
	,	m_hWnd_Explorer(NULL)
	,	m_pWebBrowser(NULL)
{
}

LHWebCtrl::~LHWebCtrl(void)
{
}

bool LHWebCtrl::Init(HWND hParent)
{
	if( FAILED( CoInitialize(NULL) ) )
		return false;

	if( FAILED( CoCreateInstance(CLSID_InternetExplorer , NULL , CLSCTX_LOCAL_SERVER , IID_IWebBrowser2 , (LPVOID*)&m_pWebBrowser) ) )
		return false;


	if( ( m_hWnd_Explorer = FindWindow( "IEFrame" , "Microsoft Internet Explorer"  ) ) == NULL )
		return false;

	::SetParent(m_hWnd_Explorer , m_hWnd_Parent);

	return true;
}

void LHWebCtrl::Open()
{
}

void LHWebCtrl::Close()
{
}
