#include "stdafx.h"
#define D3D_OVERLOADS
#include "baseTgaWindow.h"
#include "headers.h"
#include "basicbutton.h"
#include "D3DMath.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "IndexedTexture.h"
#include "window_resource.h"

CUITgaBaseWindow::CUITgaBaseWindow()
	: m_nWidth(0)
	, m_nHeight(0)
{
	m_pBtExit		= NULL;
	m_eResolution	= R800;
	m_bMove			= true;
	m_nPosX			= 0;
	m_nPosY			= 0;
	m_nPosFX		= 0;
	m_nPosFY		= 0;
	m_bMouseDown	= false;

	m_pIndexedTexture	= NULL;

	m_nWindowResNum	= -1;

	m_WndId	= -1;

	ZeroMemory(&m_MoveBarRange, sizeof(RECT));
}

CUITgaBaseWindow::~CUITgaBaseWindow()
{
	DeleteRes();
	SAFE_DELETE(m_pBtExit);
// 	if(m_pIndexedTexture)
// 		m_pIndexedTexture->InvalidateAllTextures();
}

void CUITgaBaseWindow::SetBackground(int nResNum)
{
	m_nWindowResNum = nResNum;
}

void CUITgaBaseWindow::MakeCloseBtn(char *szName)
{
	SAFE_DELETE(m_pBtExit);

	if(m_pBtExit == NULL)
	{
		m_pBtExit = new CBasicButton;
		m_pBtExit->SetFileName(szName);
		m_pBtExit->LoadRes();
	}
}

void CUITgaBaseWindow::SetExitBtnPostion(int nX, int nY)
{
	if(m_pBtExit)
	{
		m_pBtExit->SetPosition(m_nPosX + nX, m_nPosY + nY);
	}
}


void CUITgaBaseWindow::SetWindomMoveBarRange(int left, int right, int top, int bottom)
{
	m_MoveBarRange.left		= left;
	m_MoveBarRange.right	= right;
	m_MoveBarRange.top		= top;
	m_MoveBarRange.bottom   = bottom;
}

void CUITgaBaseWindow::DeleteRes()
{
	if(m_pBtExit) m_pBtExit->DeleteRes();
// 	if(m_pIndexedTexture)
// 		m_pIndexedTexture->InvalidateAllTextures();
}

int	CUITgaBaseWindow::Render()
{
	if(m_nWindowResNum < 0 || !pCMyApp->GetDevice() || !m_pIndexedTexture)	return 0;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	pCMyApp->GetDevice()->GetViewport(&vpOld);

	RECT rect;

	pCMyApp->GetDevice()->GetViewport(&vpNew);
	GetClientRect(pCMyApp->Get_hWnd(), &rect);
	vpNew.dwWidth  = rect.right - rect.left ;

	int nHeight;
	nHeight = vpNew.dwWidth;
	if ( (nHeight - (rect.bottom-rect.top)) > 0)
		nHeight = rect.bottom - rect.top;
	vpNew.dwY = ( (rect.bottom - rect.top) - nHeight ) / 2;
	vpNew.dwHeight = nHeight;

	pCMyApp->GetDevice()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);


	TLVec[0] = D3DTLVERTEX( D3DVECTOR((FLOAT)m_nPosX		   -0.5f, (FLOAT)m_nPosY			-0.5f, 0.0f), 1, 0x00ffffff, 0, 0.0f, 0.0f);
	TLVec[1] = D3DTLVERTEX( D3DVECTOR((FLOAT)(m_nPosX+m_nWidth)-0.5f, (FLOAT)m_nPosY			-0.5f, 0.0f), 1, 0x00ffffff, 0, 1.0f, 0.0f);
	TLVec[2] = D3DTLVERTEX( D3DVECTOR((FLOAT)m_nPosX		   -0.5f, (FLOAT)(m_nPosY+m_nHeight)-0.5f, 0.0f), 1, 0x00ffffff, 0, 0.0f, 1.0f);
	TLVec[3] = D3DTLVERTEX( D3DVECTOR((FLOAT)(m_nPosX+m_nWidth)-0.5f, (FLOAT)(m_nPosY+m_nHeight)-0.5f, 0.0f), 1, 0x00ffffff, 0, 1.0f, 1.0f);


	// 라이팅
	pCMyApp->GetDevice()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( pCMyApp->GetDevice()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		LPDIRECTDRAWSURFACE7 lpTexture = m_pIndexedTexture->GetSurfaceByIdx(m_nWindowResNum);

		pCMyApp->GetDevice()->SetTexture(0, lpTexture);

		pCMyApp->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		pCMyApp->GetDevice()->EndScene();
	}

	// 원상복귀.
	pCMyApp->GetDevice()->SetViewport(&vpOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);


	if(m_pBtExit) m_pBtExit->Draw();


	return 1;
}

BOOL CUITgaBaseWindow::MoveWindow(UINT msg, int x, int y)
{
	if( !m_bMove )
		return 0;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		if( !m_bMouseDown && IsMoveBarInSide(x, y) )
		{
			m_bMouseDown = true;
			m_nPosFX = x - m_nPosX;
			m_nPosFY = y - m_nPosY;
			return 1;
		}
		break;

	case WM_LBUTTONUP:
		m_nPosFX = m_nPosFY = 0;
		m_bMouseDown = false;
		break;

	case WM_MOUSEMOVE:
		if( m_bMouseDown )
		{
			int backX = m_nPosX;
			int backY = m_nPosY;

			m_nPosX = x - m_nPosFX;
			m_nPosY = y - m_nPosFY;

			int cw = g_pNk2DFrame->GetClientWidth() -  m_nWidth -1;
			int ch = g_pNk2DFrame->GetClientHeight() - m_nHeight-1;

			if( m_nPosX < 1 )	m_nPosX = 1;
			if( m_nPosX > cw )	m_nPosX = cw;
			if( m_nPosY < 1 )	m_nPosY = 1;
			if( m_nPosY > ch )	m_nPosY = ch;

			SetPosition();

			return 1;
		}

		m_nPosFX = m_nPosFY = 0;
	}

	return 0;
}


BOOL CUITgaBaseWindow::IsInside(int x, int y)
{
	if(m_nWindowResNum < 0)	return FALSE;

	if (x >= m_nPosX && x < m_nPosX + m_nWidth  &&
			y >= m_nPosY && y < m_nPosY + m_nHeight )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CUITgaBaseWindow::IsMoveBarInSide(int x, int y)
{
	if (x >= m_nPosX+m_MoveBarRange.left && x < m_nPosX+m_MoveBarRange.right &&
			y >= m_nPosY+m_MoveBarRange.top  && y < m_nPosY+m_MoveBarRange.bottom )
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT CUITgaBaseWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_pBtExit)	m_pBtExit->MsgProc(hWnd, msg, wParam, lParam);

	switch(msg)
	{
	case WM_LBUTTONDOWN:
		if( m_pBtExit && m_pBtExit->GetState() == BTN_DOWN )
			return IM_TGA_WIN_INPUT;

		break;

	case WM_LBUTTONUP :
		if(m_pBtExit && m_pBtExit->GetState() == BTN_ACTION)
			return IM_TGA_WIN_CLOSE;

		break;
	}

	if( MoveWindow( msg, LOWORD(lParam), HIWORD(lParam) )	)
		return IM_TGA_WIN_INPUT;

	return IM_TGA_WIN_NONE;
}