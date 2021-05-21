#include "stdafx.h"



#include <string.h>
#include <TCHAR.h>
#include "main.h"
#include "headers.h"
#include "ddutil.h"
#include "AlphaPopUp.h"
#include "Nk2DFrame.h"
#include "ControlBottom.h"
#include "Scroll.h"
#include "Mouse.h"
#include "BasicButton.h"
#include "textoutbox.h"
#include "country.h"
#include "g_stringmanager.h"
#include "ImageDefine.h"

#include "ControlNotice.h"





extern int					g_EffectTextr[];

extern char g_strFontName[];
extern HFONT G_GetFont(int h, int w,const char  * const fontname);

///공지팝업창
CControlNotice::CControlNotice()
	: m_hWnd(NULL)
	, m_PosX(0)
	, m_PosY(0)
{
	m_pTitleBar = NULL;
	m_pTitleBarLight = NULL;
	m_pTitle = NULL;     // 창 타이틀 부분의 글자.(버택스 사이즈 조절에 영향을 안 받을수있도록...

	m_pContentsSurface = NULL; // 표시 내용의 텍스트를 서피스로 변환해놓은 것. 사이즈 조절시마다 다시 만들어 줘야한다.

	m_pSizeControl = NULL;     // 사이즈 조절을 위한 클릭용 이미지

	m_pNewMessage = NULL; // 뉴 메세지시의 효과음.


	Init(); // 변수들 초기화.
}

CControlNotice::~CControlNotice()
{
	DeleteRes();
}

void CControlNotice::Init()
{
	m_nType = NOTICE_TYPE_NORMAL; // 무슨 공지인가..
	m_nState = NOTICE_STATE_FULL; // 인터페이스의 상태 ( 0 : 전개 상태, 1 : 최소화 상태 )
	m_bMoveDrag = FALSE; // 위치이동 드래그 중인가.
	m_bSizeDrag = FALSE; // 사이즈 조절 드래그 중인가.
	m_bNewMessage = FALSE; // 새로운 메세지가 와있는지. (깜빡이자)
	m_SizeRect.left = 0; // 디폴트 사이즈
	m_SizeRect.top = 0; // 디폴트 사이즈 (다른 건 WindowChange에서 셋팅)
	m_ViewLineNum = 0;
	m_TotalLineNum = 0;

	ZeroMemory( &m_Contents, sizeof(CMultiText2) ); // 새로 생기면 깔끔히 지워줘야한다.

	m_MiniBtn.SetFileName("PopupNotice/btn_mini");
	m_MaxBtn.SetFileName("PopupNotice/btn_max");
	m_CloseBtn.SetFileName("PopupNotice/btn_close");
}

void CControlNotice::DeleteRes()
{
	SAFE_DELETE( m_pSizeControl );
	m_CloseBtn.DeleteRes();
	m_MaxBtn.DeleteRes();
	m_MiniBtn.DeleteRes();
	m_Scroll.DeleteRes();
	SAFE_DELETE( m_pContentsSurface );
	SAFE_DELETE( m_pTitle );
	SAFE_DELETE( m_pTitleBarLight );
	SAFE_DELETE( m_pTitleBar );
}

void CControlNotice::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap( &m_pTitleBar, "interface/PopupNotice/title_bar.bmp" );
	if( m_pTitleBar )
		m_pTitleBar->SetColorKey( RGB(0, 0, 0) );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pTitleBarLight, "interface/PopupNotice/title_bar2.bmp" );
	if( m_pTitleBarLight )
		m_pTitleBarLight->SetColorKey( RGB(0, 0, 0) );


	m_Scroll.LoadRes( "PopupNotice//scr_up", "PopupNotice//scr_down", "interface//PopupNotice//scr_bar.bmp" );
	m_Scroll.SetRange( 1 ); // 스크롤 레인지 셋팅은 별도로 초기화 해줄 필요가 있다.

	m_MiniBtn.LoadRes();
	m_MaxBtn.LoadRes();
	m_CloseBtn.LoadRes();


	g_pDisplay->CreateSurfaceFromBitmap( &m_pSizeControl, "interface/PopupNotice/sizecontroler.bmp" );
	if( m_pSizeControl )
		m_pSizeControl->SetColorKey( RGB(0, 0, 0) );

	WindowChange( 10, 100, 150, 143 ); // 초기위치와 사이즈 셋팅(버튼등 좌표 셋팅도 여기서 해준다.)
}

HRESULT CControlNotice::RestoreSurfaces()
{
	DeleteRes();
	LoadRes();
	MakeTextSurface();

	return S_OK;
}

void CControlNotice::SetType( int Type ) // 타이틀 변경.
{
	m_nType = Type;

	if( m_pTitle ) // 일단 지우고..
		SAFE_DELETE( m_pTitle );

	switch( m_nType ) // 타입에 따라 타이틀을 바꿔준다.
	{
	case NOTICE_TYPE_MOBIUS:
		g_pDisplay->CreateSurfaceFromBitmap( &m_pTitle, "interface/PopupNotice/mobiusarena.bmp" );
		break;
	}
}

void CControlNotice::SetContents( char *String ) // 스트링 삽입.
{
	if( !String )
		return;

	if( m_Contents.m_LineCount >= 150 ) // 일정량 이상 쌓이면..
		m_Contents.Reset(); // 지우고 다시 처음부터 넣는다.

	m_Contents.AddString( String, RGB(255,255,255) );
	m_Contents.AddString( " ", RGB(255,255,255) ); // 공백 한줄을 넣어준다.

	MakeTextSurface(); // 서피스를 새로 만든다.

	// 스크롤을 자동 조정 해준다.
	m_Scroll.SetPos( m_Scroll.RangeMax ); // 이렇게 하면 자동으로 아래로 선택될 듯.
}

void CControlNotice::Draw() // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
{
	// 실제 인터페이스 영역 드로우.
	RenderOj( m_PosX, m_PosY, m_SizeRect.right, m_SizeRect.bottom );

	// 타이틀 바 그리기.
	if( m_pTitleBar )
		g_pDisplay->Blt( m_PosX, m_PosY,  m_PosX+m_SizeRect.right, m_PosY+m_pTitleBar->GetHeight(), m_pTitleBar );
	if( m_bNewMessage && m_pTitleBarLight && (timeGetTime()%1000) > 500 )
		g_pDisplay->Blt( m_PosX, m_PosY,  m_PosX+m_SizeRect.right, m_PosY+m_pTitleBarLight->GetHeight(), m_pTitleBarLight );

	// 3D 창 위의 2D 출력들
	if( m_nState == NOTICE_STATE_FULL ) // 전개 상태
	{
		if( g_pDisplay && m_pContentsSurface )
		{
			RECT TempRect;
			TempRect.left = 0;
			TempRect.right = m_pContentsSurface->GetWidth();
			TempRect.top = m_Scroll.GetPos()*POPUP_NOTICE_TEXTLINE_HEIGHT;

			if( m_TotalLineNum > m_ViewLineNum )
				TempRect.bottom = (m_Scroll.GetPos()*POPUP_NOTICE_TEXTLINE_HEIGHT) + (m_SizeRect.bottom - POPUP_NOTICE_TEXTSUR_H_SPACE);
			else
				TempRect.bottom = m_pContentsSurface->GetHeight();

			g_pDisplay->Blt( m_PosX+10, m_PosY+25, m_pContentsSurface, &TempRect );
		}

		if( m_pTitle )
		{
			if( m_SizeRect.right >= 148 )
				g_pDisplay->Blt( m_PosX+5, m_PosY+1, m_pTitle );
			else // 사이즈가 작을땐 클리핑.
			{
				RECT TempRect;
				TempRect.left = 0;
				TempRect.right = m_pTitle->GetWidth() - (148-m_SizeRect.right);
				TempRect.top = 0;
				TempRect.bottom = m_pTitle->GetHeight();
				g_pDisplay->Blt( m_PosX+5, m_PosY+1, m_pTitle, &TempRect );
			}
		}

		if( m_Scroll.RangeMax != 1 )
			m_Scroll.Draw();
		m_MiniBtn.Draw(); // 최소화 버튼

		if( m_pSizeControl )
		{
			g_pDisplay->Blt( m_pSizeControl->Xpos, m_pSizeControl->Ypos, m_pSizeControl );
		}
	}
	else if( m_nState == NOTICE_STATE_MINI ) // 미니멈 상태
	{
		m_MaxBtn.Draw(); // 최대화 버튼

	}

	m_CloseBtn.Draw(); // 닫기 버튼
}

//-------------------------------------------------------------------
// name : RenderOj
// desc : 3d 기능을 사용하여 팝업창의 반투명한 바탕(BACK)를 그린다.
//-------------------------------------------------------------------
void CControlNotice::RenderOj(int xpos, int ypos, int cx, int cy)
{
	if( m_nState == NOTICE_STATE_MINI ) // 미니 모드일땐 이걸 안그린다.
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)

		// 메인 창
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , TRUE);

		m_TLVec[0] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos,     0), 1, 0x88000000, 0, 0, 0);
		m_TLVec[1] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos,     0), 1, 0x88000000, 0, 1, 0);
		m_TLVec[2] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos+cy,  0), 1, 0x88000000, 0, 0, 1);
		m_TLVec[3] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos+cy,  0), 1, 0x88000000, 0, 1, 1);

		GET_D3DDEVICE()->SetTexture(0, lpTexture );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_TLVec, 4, 0);

		GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}

LRESULT CControlNotice::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static int DistX = 0, DistY = 0; // 드래그시 클릭한 곳과 마우스 좌표의 차이 수치.
	static int OldSizeX = 0, OldSizeY = 0; // 사이즈 조절시 이전 사이즈도 기억하기 위한녀석
	int x = 0, y = 0;

	if( m_nState == NOTICE_STATE_FULL && m_Scroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
		return 1;

	switch (msg)
	{
	case 0x020A: // 휠마우스 메세지(리스트 영역안에서의 휠 메세지를 강제로 스크롤에 연동시켜준다.)

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_nState == NOTICE_STATE_FULL && IsInside(x,y-45) ) // 리스트 영역에서만 처리 (이 메세지에서의 y값이 이상해서 약간 보정)
		{
			if( (short)HIWORD(wParam) < 0 )
			{
				m_Scroll.NowPos += 1;

				if( m_Scroll.NowPos > m_Scroll.RangeMax-1 )
					m_Scroll.NowPos = m_Scroll.RangeMax-1;
			}
			else if( (short)HIWORD(wParam) > 0 )
			{
				m_Scroll.NowPos -= 1;

				if( m_Scroll.NowPos < 0 )
					m_Scroll.NowPos = 0;
			}
			return 1;
		}

		break;

	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y)) //영역을 벗어남
			return 0;

		if( m_nState == NOTICE_STATE_FULL && m_bNewMessage ) // 전개 시 깜빡이는 중이면 클릭만으로도 해제
			m_bNewMessage = FALSE;

		if( !m_MiniBtn.IsInside(x,y) && !m_MaxBtn.IsInside(x,y) && !m_CloseBtn.IsInside(x,y) ) // 버튼 영역에서가 아니면...
		{
			// 드래깅 시작 체크.
			if( IsSizeControlInside(x,y) && m_nState == NOTICE_STATE_FULL ) // 사이즈 조절 컨트롤 영역이면..(전개상태에서만 가능)
			{
				m_bSizeDrag = TRUE; // 사이즈 드래깅임을 표시
				DistX = x; // 이 녀석은 집은 위치를 기억
				DistY = y;
				OldSizeX = m_SizeRect.right; // 이전 사이즈도 기억
				OldSizeY = m_SizeRect.bottom;
			}
			else if( IsTitleBarInside(x,y) )
			{
				m_bMoveDrag = TRUE; // 그 외엔 이동 드래그.
				DistX = m_PosX - x; // 이녀석은 좌표와 집은 곳의 차이를 기억.
				DistY = m_PosY - y;
			}
		}

		if( m_MiniBtn.m_bVisible ) // 보일때만 처리
			m_MiniBtn.MsgProc(hWnd, msg, wParam, lParam);
		if( m_MaxBtn.m_bVisible ) // 보일때만 처리
			m_MaxBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 마우스 때면 드래그는 무조건 취소.
		m_bSizeDrag = FALSE; // 사이즈 드래깅임을 표시
		m_bMoveDrag = FALSE; // 그 외엔 이동 드래그.

		if(!IsInside(x, y))//영역을 벗어남
			return 0;

		if( m_bSizeDrag || m_bMoveDrag ) // 드래그 중에 버튼 반응 안하게..
			return 0;

		if( m_MiniBtn.m_bVisible ) // 보일때만 처리
		{
			m_MiniBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_MiniBtn.GetState() == BTN_ACTION )
			{
				m_nState = NOTICE_STATE_MINI;
				// 최소화 하기전 크기를 기억.
				OldSizeX = m_SizeRect.right;
				OldSizeY = m_SizeRect.bottom;

				// 크기와 위치를 보정.
				WindowChange( m_PosX+(OldSizeX-POPUP_NOTICE_MINIMIZE_SIZE_X), m_PosY, POPUP_NOTICE_MINIMIZE_SIZE_X, POPUP_NOTICE_MINIMIZE_SIZE_Y ); // 사이즈 변경

				m_MiniBtn.SetState(BTN_NORMAL);

				m_MiniBtn.SetVisible( FALSE );
				m_MaxBtn.SetVisible( TRUE );

				return 1;
			}
		}

		if( m_MaxBtn.m_bVisible ) // 보일때만 처리
		{
			m_MaxBtn.MsgProc(hWnd, msg, wParam, lParam);
			if( m_MaxBtn.GetState() == BTN_ACTION )
			{
				m_nState = NOTICE_STATE_FULL;


				WindowChange( m_PosX-(OldSizeX-POPUP_NOTICE_MINIMIZE_SIZE_X), m_PosY, OldSizeX, OldSizeY ); // 사이즈 변경

				if( m_bNewMessage )
				{
					MakeTextSurface(); // 최소화 상태에서 새 메세지가 들어왔었다면 전개시 서피스를 다시 만들어 줘야한다.
					m_bNewMessage = FALSE; // 전개시에 무조건 풀어준다
				}

				m_MaxBtn.SetState(BTN_NORMAL);

				m_MiniBtn.SetVisible( TRUE );
				m_MaxBtn.SetVisible( FALSE );

				return 1;
			}
		}

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if( m_CloseBtn.GetState() == BTN_ACTION )
		{
			m_CloseBtn.SetState(BTN_NORMAL);

			g_pNk2DFrame->ShowControlNoticeWindow( FALSE ); // 창을 끈다.(메모리에서 제거)

			return 1;
		}

		return 1;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_bSizeDrag ) // 사이즈 조절 중.
		{
			WindowChange( m_PosX, m_PosY, OldSizeX+(x-DistX), OldSizeY+(y-DistY) ); // 사이즈 변경

			// 사이즈 조절되었으면 스크롤및 내용 서피스를 다시 만들어 줘야한다.
			MakeTextSurface(); // 텍스트 서피스도 다시 만든다.
		}
		else if( m_bMoveDrag ) // 위치 이동중.
		{
			WindowChange( x + DistX, y + DistY, m_SizeRect.right, m_SizeRect.bottom ); // 좌표 변경
		}

		break;

	}

	return 0;
}

BOOL CControlNotice::IsInside( int x, int y )
{
	if ( ( ( x > m_PosX) && ( x < (m_PosX+m_SizeRect.right)) )
			&& ( ( y > m_PosY) && ( y < (m_PosY+m_SizeRect.bottom)) ) )
		return TRUE;

	return FALSE;
}

BOOL CControlNotice::IsTitleBarInside( int x, int y ) // 무빙을 위한 타이틀 바.
{
	if ( ( ( x > m_PosX) && ( x < (m_PosX+m_SizeRect.right)) )
			&& ( ( y > m_PosY) && ( y < (m_PosY+POPUP_NOTICE_TITLEBAR_SIZE_Y)) ) )
		return TRUE;

	return FALSE;
}

BOOL CControlNotice::IsSizeControlInside( int x, int y ) // 사이즈 조절을 위한 가장자리를 클릭했는가.
{
	if( !m_pSizeControl )
		return FALSE;

	if ( ( x > m_pSizeControl->Xpos ) && ( x < m_pSizeControl->Xpos+m_pSizeControl->GetWidth() )
			&& ( y > m_pSizeControl->Ypos ) && ( y < m_pSizeControl->Ypos+m_pSizeControl->GetHeight() ) )
		return TRUE;

	return FALSE;
}

void CControlNotice::WindowChange( int x, int y, int size_right, int size_bottom ) // 창 위치나 사이즈가 바뀔때 해줘야하는 것들
{
	m_PosX = x;
	m_PosY = y;

	m_SizeRect.right = size_right;
	m_SizeRect.bottom = size_bottom;

	if( m_bSizeDrag ) // 사이즈 변경 중이라면...
	{
		// 최대 최소 사이즈 제한
		if( m_nState == NOTICE_STATE_FULL ) // 전개 상태에서만 제한한다.
		{
			if( m_SizeRect.right < 120 ) // 최소 사이즈
				m_SizeRect.right = 120;
			if( m_SizeRect.bottom < 143 )
				m_SizeRect.bottom = 143;
			if( m_SizeRect.right > 500 ) // 최대 사이즈
				m_SizeRect.right = 500;
			if( m_SizeRect.bottom > 300 )
				m_SizeRect.bottom = 300;
		}

		// 화면 넘어가면 보정.
		if( m_PosX+m_SizeRect.right > m_ClientRc.right )
			m_SizeRect.right = m_ClientRc.right - m_PosX;
		if( m_PosY+m_SizeRect.bottom > m_ClientRc.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			m_SizeRect.bottom = m_ClientRc.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() - m_PosY;
	}
	else if( m_bMoveDrag ) // 화면 넘어가면 보정. (위치 이동 보정인데 사이즈쪽 보정이 먼저되어야함에 유의.)
	{
		if( m_PosX < 0 )
			m_PosX = 0;
		if(	m_PosY < 0 )
			m_PosY = 0;
		if( m_PosX > m_ClientRc.right - m_SizeRect.right )
			m_PosX = m_ClientRc.right - m_SizeRect.right;
		if( m_PosY > m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			m_PosY = m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
	}
	else // 이 경우는 아마도 전개 축소 일거다.
	{
		// 사이즈를 유지한채 좌표를 보정
		if( m_PosX < 0 )
			m_PosX = 0;
		if( m_PosY > m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			m_PosY = m_ClientRc.bottom - m_SizeRect.bottom - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
	}


	// 위치 재조정.
	if( m_pSizeControl )
	{
		m_pSizeControl->Xpos = m_PosX+m_SizeRect.right-m_pSizeControl->GetWidth()-2;
		m_pSizeControl->Ypos = m_PosY+m_SizeRect.bottom-m_pSizeControl->GetHeight()-2;
	}
	m_MiniBtn.SetPosition( m_PosX+m_SizeRect.right-39, m_PosY+1 );	// 버튼
	m_MaxBtn.SetPosition( m_PosX+m_SizeRect.right-39, m_PosY+1 );	// 버튼
	m_CloseBtn.SetPosition( m_PosX+m_SizeRect.right-20, m_PosY+1 );	// 버튼
	m_Scroll.SetDrawPos( m_PosX+m_SizeRect.right-20, m_PosY+25, m_SizeRect.bottom-25-20 ); // 스크롤
}

void CControlNotice::MakeTextSurface() // CMultiText 를 서피스로 변환해주기 위한 녀석.
{
	int OldSurfaceHeight = 0;

	if( m_pContentsSurface )
	{
		OldSurfaceHeight = m_pContentsSurface->GetHeight();

		SAFE_DELETE( m_pContentsSurface ); // 깔끔히 지우고...
	}

	if( !g_pDisplay  )	// m_Font 는 핸들이다
		return;

	if( m_Contents.m_LineCount == 0 ) // 텍스트가 없어도 그냥 리턴
		return;


	CMultiText2 TempMulti; // 사이즈에 따른 개행용.
	int NextLinePos = 0; // 문자열 짜를 바이트 수.

	int FontHeight = 14;
	HFONT hFont = G_GetFont( FontHeight,0, g_strFontName );

	HDC hdc;
	hdc = GetDC(pCMyApp->Get_hWnd());
	HFONT hOldFont = (HFONT) SelectObject( hdc, hFont );

	// 문자열 짜를 바이트 수 계산.(멀티 바이트를 기준으로 짤라주기 위해 2의 배수로 구하자)(멀티바이트 구분은 아래 AddStringMulti에서 자동으로 해줄껄?
	SIZE	sizeFont;
	getFontSize( &sizeFont, hFont, "A", 1 );
	int FontWidth = sizeFont.cx; // 줄 짜를때 사용할 폰트 가로크기.임의로 셋팅. 조금 크게 잡자.


	NextLinePos = (m_SizeRect.right-50) / FontWidth;

	if( NextLinePos % 2 )
		NextLinePos++; // 2의 배수로 만들기.

	int i = 0;
	for( i = 0 ; i < m_Contents.m_LineCount ; ++i ) // 라인 갯수만큼.
	{
		TempMulti.AddStringMulti( m_Contents.m_Text[i], NextLinePos, TRUE, m_Contents.m_TextColor[i], m_Contents.m_TextBackColor[i] );
	}

	// 보여지는 줄수와 총줄수를 기억.
	m_ViewLineNum = (m_SizeRect.bottom-POPUP_NOTICE_TEXTSUR_H_SPACE) / POPUP_NOTICE_TEXTLINE_HEIGHT;
	m_TotalLineNum = TempMulti.m_LineCount;

	int TextSurWidth = NextLinePos * FontWidth;
	int TextSurHeight = TempMulti.m_LineCount * FontHeight;

	// 계산된 크기로 서피스 생성

	g_pDisplay->CreateSurface( &m_pContentsSurface, TextSurWidth, TextSurHeight );
	if( !m_pContentsSurface )
		return;

	m_pContentsSurface->SetWidth( TextSurWidth );
	m_pContentsSurface->SetHeight( TextSurHeight );


	// fillcolor를 이용한 textsurface 클리어하기
	m_pContentsSurface->Clear();

	int Ypos = 0;
	for( i = 0 ; i < TempMulti.m_LineCount ; ++i )
	{
		m_pContentsSurface->DrawText( hFont, TempMulti.m_Text[i], 0, Ypos,
									  TempMulti.m_TextBackColor[i], TempMulti.m_TextColor[i], 0);

		Ypos+=FontHeight;
	}
	m_pContentsSurface->SetColorKey(TEXT_BACK_COLOR);

	SelectObject(hdc, hOldFont);
	ReleaseDC(pCMyApp->Get_hWnd(), hdc);

	// 서피스에 맞춰 스크롤을 다시 조정해준다.
	if( m_TotalLineNum > m_ViewLineNum )
	{
		m_Scroll.SetRange( m_TotalLineNum-m_ViewLineNum ); // 픽셀단위가 아닌 라인단위로 하자.
	}
	else // 스크롤이 필요없는 길이 일때.
	{
		m_Scroll.SetRange( 1 ); // 스크롤
	}

	m_Scroll.SetPos( m_Scroll.RangeMax ); // 이렇게 하면 자동으로 아래로 선택될 듯. (원래 스크롤 위치로 돌려주긴 계산이 힘드므로 그냥 맨 밑으로 옮겨준다.)
}