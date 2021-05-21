#include "stdafx.h"
#define D3D_OVERLOADS
#include <math.h>
#include <d3d.h>
#include "d3dutil.h"
#include "IndexedTexture.h"
// nate 2004 - 7
#include "ImageDefine.h"
#include <assert.h>
#include "scrollpopup.h"
#include "headers.h"
#include "main.h"

extern CMyD3DApplication *pCMyApp;
extern CDisplay* g_pDisplay;
extern IndexedTexture *g_pCapsyongTexture;
extern int g_EffectTextr[];


CScrollPopUp::CScrollPopUp(int type, LPDIRECT3DDEVICE7 pd3dDevice)
{

	m_CurType = type;

	Init();
	LoadRes(type, pd3dDevice);
}

CScrollPopUp::~CScrollPopUp()
{
	DeleteRes();
}


void CScrollPopUp::Init()
{
	m_pBack =	NULL;
	m_pInfoSur = NULL;
	m_pDragBar = NULL;


	m_bMainAct = FALSE;
	m_iArray = NULL;
	m_CurArrayCnt = 0;
	m_bBarClick = FALSE;

	m_StartLine = 0;		// 스크롤 (보여질 시작위치)
	m_EndLine = _INFO_CY;   // 스크롤 (보여질 마지막위치)		break;

}
void CScrollPopUp::LoadRes(int type, LPDIRECT3DDEVICE7 pd3dDevice)
{
	if( !pd3dDevice || !g_pDisplay )
		return;

	int TmpPos;
	RECT ClientRc;

	m_pd3dDevice = pd3dDevice;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &ClientRc);
	m_ScreenWidth = ClientRc.right;
	m_ScreenHeight = ClientRc.bottom;


	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, _IMG_BACK, _BACK_CX, _BACK_CY);
	m_pBack->SetColorKey(RGB(255, 0, 255));
	m_pBack->Xpos = (ClientRc.right/2) - (_BACK_CX/2); // 현재해상도의 중간에 위치하도록
	m_pBack->Ypos = (ClientRc.bottom/2) - (_BACK_CY/2);
	g_pDisplay->CreateSurfaceFromBitmap(&m_pInfoSur, _IMG_INFO);
	m_pInfoSur->SetColorKey(RGB(255, 0, 255));
	m_pInfoSur->Xpos = m_pBack->Xpos;
	m_pInfoSur->Ypos = m_pBack->Ypos+_BACK_TITLE_CY;
	m_Arrow_Up.SetFileName(_FN_ARROW_UP);
	m_Arrow_Down.SetFileName(_FN_ARROW_DOWN);
	m_Arrow_Up.LoadRes();
	m_Arrow_Down.LoadRes();
	m_Arrow_Up.PosX = m_pBack->Xpos+_BACK_CX - m_Arrow_Up.Width;
	m_Arrow_Up.PosY = m_pBack->Ypos+_BACK_TITLE_CY;
	m_Arrow_Down.PosX = m_Arrow_Up.PosX;
	m_Arrow_Down.PosY = m_pBack->Ypos+_BACK_CY - _BACK_BOTTOM_AREA_CY - m_Arrow_Down.Height;

	g_pDisplay->CreateSurfaceFromBitmap(&m_pDragBar, _IMG_DRAG_BAR, m_Arrow_Up.Width, 30);
	m_pDragBar->Xpos = m_Arrow_Up.PosX;
	m_pDragBar->Ypos = m_Arrow_Up.PosY+m_Arrow_Up.Height;


	if(type == _TYPE_SCROLL_OK || type == _TYPE_ONLY_INFO_OK)
	{
		m_Btn1.SetFileName(_FN_BTN1);
		m_Btn1.LoadRes();
		m_Btn1.PosX = m_pBack->Xpos+(_BACK_CX/2)-(m_Btn1.Width/2); // "ok"버튼 하나만 존재하므로 버튼의 위치는 중앙에 위치한다
		m_Btn1.PosY = m_pBack->Ypos+(_BACK_CY - m_Btn1.Height);
	}

	else if( type == _TYPE_SCROLL_OKCANCLE || type == _TYPE_ONLY_INFO_OKCANCLE)
	{
		TmpPos = _BACK_CX / 4; // "OK/CANCLE" 버트의 위치를위해서
		m_Btn1.SetFileName(_FN_BTN1);
		m_Btn1.LoadRes();
		m_Btn1.PosX = m_pBack->Xpos+(TmpPos)-(m_Btn1.Width/2); //
		m_Btn1.PosY = m_pBack->Ypos+(_BACK_CY - m_Btn1.Height);
		m_Btn2.SetFileName(_FN_BTN2);
		m_Btn2.LoadRes();
		m_Btn2.PosX = m_pBack->Xpos+(TmpPos*3)-(m_Btn2.Width/2); //
		m_Btn2.PosY = m_pBack->Ypos+(_BACK_CY - m_Btn2.Height);
	}

	CalBasic();
}

void CScrollPopUp::DeleteRes()
{
	SAFE_DELETE(m_pBack);
	m_Btn1.DeleteRes();
	m_Arrow_Up.DeleteRes();
	m_Arrow_Down.DeleteRes();
	SAFE_DELETE(m_pInfoSur);
	SAFE_DELETE(m_pDragBar);
	for(int i=0; i<m_MaxArrayCnt; i++)
	{
		if(m_iArray)
		{
			delete m_iArray;
			m_iArray = NULL;
		}
	}
}


void CScrollPopUp::Draw()
{
	if(!m_bMainAct) return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	RenderOj(m_pInfoSur->Xpos, m_pInfoSur->Ypos, _BACK_CX, _INFO_CY);
	ScrollTextDraw();

	switch(m_CurType)
	{
	case _TYPE_SCROLL_OK:
		m_Btn1.Draw();
		m_Arrow_Up.Draw();
		m_Arrow_Down.Draw();
		g_pDisplay->Blt(m_pDragBar->Xpos, m_pDragBar->Ypos,	m_pDragBar);
		break;
	case _TYPE_SCROLL_OKCANCLE:
		m_Btn1.Draw();
		m_Btn2.Draw();
		m_Arrow_Up.Draw();
		m_Arrow_Down.Draw();
		g_pDisplay->Blt(m_pDragBar->Xpos, m_pDragBar->Ypos,	m_pDragBar);
		break;

	case _TYPE_ONLY_INFO_OK:
		m_Btn1.Draw();
		break;

	case _TYPE_ONLY_INFO_OKCANCLE:
		m_Btn1.Draw();
		m_Btn2.Draw();
		break;
	}
}



LRESULT CScrollPopUp::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD (lParam);
	int y = HIWORD (lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		m_Btn1.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn2.MsgProc(hWnd, msg, wParam, lParam);
		m_Arrow_Up.MsgProc(hWnd, msg, wParam, lParam);
		m_Arrow_Down.MsgProc(hWnd, msg, wParam, lParam);
		if(m_Arrow_Up.GetState() == BTN_DOWN)
		{
			ArrowUpBtn_Click();
		}
		if(m_Arrow_Down.GetState() == BTN_DOWN)
		{
			ArrowDownBtn_Click();
		}

		// 스크롤기능이 없을때에는 메시지를 받지 않아도 좋다!!
		if(m_CurType == _TYPE_SCROLL_OK || m_CurType == _TYPE_ONLY_INFO_OK)
		{
			if(IsInside(x, y))
			{
				DragAndClick(x, y);
			}
			m_bBarClick = IsInsideSurface(m_pDragBar, x, y) ? TRUE : FALSE;
		}
		break;

	case WM_LBUTTONUP:
		m_Btn1.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn2.MsgProc(hWnd, msg, wParam, lParam);
		m_Arrow_Up.MsgProc(hWnd, msg, wParam, lParam);
		m_Arrow_Down.MsgProc(hWnd, msg, wParam, lParam);
		if(m_Btn1.GetState() == BTN_ACTION)
		{
			Action(0);
			m_Btn1.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_Btn2.GetState() == BTN_ACTION)
		{
			Action(0);
			m_Btn2.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_Arrow_Down.GetState() == BTN_DOWN)
		{
			m_Arrow_Down.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_Arrow_Up.GetState() == BTN_DOWN)
		{
			m_Arrow_Up.m_iBtnFlg = BTN_NORMAL;
		}

		if(m_bBarClick)
			m_bBarClick = FALSE;
		break;

	case WM_MOUSEMOVE:
		m_Btn1.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn2.MsgProc(hWnd, msg, wParam, lParam);
		m_Arrow_Up.MsgProc(hWnd, msg, wParam, lParam);
		m_Arrow_Down.MsgProc(hWnd, msg, wParam, lParam);

		if(m_bBarClick)
			DragAndClick(x, y);
		break;

	case WM_KEYDOWN:
		if(wParam == 'y' || wParam =='Y')
		{
			m_bMainAct =! m_bMainAct;
			return 1;
		}
		break;
	}
	if(IsInsideSurface(m_pBack, x, y)) // 메세지.처리를 두번 하지 않겠다
		return 1;
	else
		return 0;
}

void CScrollPopUp::Action(int act)
{
	switch(act)
	{
	case 0: // 로드한 메모리를 지우고 팝업창을 종료한다
		m_bMainAct = FALSE;
		DeleteRes();
		break;
	}
}

void CScrollPopUp::ScrollTextDraw()
{
	RECT rc;
	SetRect(&rc, 0, m_StartLine, _INFO_CX, m_EndLine);
	g_pDisplay->Blt(m_pInfoSur->Xpos, m_pInfoSur->Ypos, m_pInfoSur, &rc);
}

BOOL CScrollPopUp::CalBasic()
{
	int   a, b;
	float c;

	a = (m_pInfoSur->GetHeight() - _INFO_CY) / 10;
	if(a<=0)
		return FALSE;

	b = _INFO_CY -  (m_Arrow_Up.Height*2) -m_pDragBar->GetHeight();
	c = (float)b/(float)a;

	if(c<=0)
		return FALSE;

	m_iArray = new float[a];
	for(int i=0; i<a; i++)
	{
		m_iArray[i] = m_Arrow_Up.PosY+m_Arrow_Up.Height+(i*c);
	}
	m_MaxArrayCnt = a; // 존재하는 라인의 모든 개수를 저장한다
	return TRUE;
}

void CScrollPopUp::ArrowDownBtn_Click()
{
	// 스크롤 조건을 검사한다
	if(m_EndLine < m_pInfoSur->GetHeight())
	{
		m_StartLine += 10;
		m_EndLine += 10;
		m_pDragBar->Ypos = m_iArray[m_CurArrayCnt++];
	}
	else
		m_EndLine = m_pInfoSur->GetHeight();
}

void CScrollPopUp::ArrowUpBtn_Click()
{
	if(m_StartLine > 0)
	{
		m_StartLine -= 10;
		m_EndLine -= 10;
		m_pDragBar->Ypos = m_iArray[m_CurArrayCnt--];
	}
	else
		m_StartLine = 0;
}

void CScrollPopUp::DragAndClick(int pos_x, int pos_y)
{
	m_pDragBar->Ypos = pos_y;
	if(m_pDragBar->Ypos+m_pDragBar->GetHeight() > m_Arrow_Down.PosY)
		m_pDragBar->Ypos = m_Arrow_Down.PosY-m_pDragBar->GetHeight();
	if(m_pDragBar->Ypos< m_Arrow_Up.PosY+m_Arrow_Up.Height)
		m_pDragBar->Ypos = m_Arrow_Up.PosY+m_Arrow_Up.Height;

	for(int i=0; i<m_MaxArrayCnt; i++)
	{
		if(m_pDragBar->Ypos >= m_iArray[i] && m_pDragBar->Ypos <= m_iArray[i]+3)
		{
			m_StartLine = 0+((i)*10);
			m_EndLine = 280+((i)*10);
			m_CurArrayCnt = i;
			break;
		}
	}
}




BOOL CScrollPopUp::IsInside(int x, int y)
{
	if(!m_bMainAct)
		return FALSE;
	if (x >= m_Arrow_Up.PosX && x <= m_Arrow_Up.PosX + m_Arrow_Up.Width &&
			y >= m_Arrow_Up.PosY+m_Arrow_Up.Height && y < m_Arrow_Down.PosY)
	{
		return TRUE;
	}
	return FALSE;
}


BOOL CScrollPopUp::IsInsideSurface(CSurface* surface, int x, int y)
{
	if(!m_bMainAct || !surface)
		return false;

	if ( ( ( x >= surface->Xpos ) && ( x <= (surface->Xpos + surface->GetWidth())))
			&& ( ( y >= surface->Ypos ) && ( y <= (surface->Ypos + surface->GetHeight()))))
		return true;

	else
		return false;

}

void CScrollPopUp::RenderOj(int xpos, int ypos, int cx, int cy)
{
	//xpos = X1, ypos = Y1; // TextSur의 위치를 알파 배경 위치와 같게 한다
	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	//D3DVECTOR vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	m_pd3dDevice->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	m_pd3dDevice->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos,      0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos,     0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos+cy,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos+cy,  0), 1, 0x00ffffff, 0, 1, 1);


	// 라이팅
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)

		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_NKCHARACTER_LINE ]);
		m_pd3dDevice->SetTexture(0, lpTexture);

		//m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		//m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
		//m_pd3dDevice->SetTexture(0, NULL);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		m_pd3dDevice->EndScene();
	}

	// 원상복귀.
	m_pd3dDevice->SetViewport(&vpOld);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}