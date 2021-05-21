#include "stdafx.h"

#include <mmsystem.h>
#include <d3d.h>
#include "Mouse.h"
#include "ddutil.h"
#include "headers.h"

//extern	SVR_TYPE	g_SvrType;

#define IMG_ATTACT			"interface/mouse/attact.bmp"
#define IMG_TALK			"interface/mouse/talk.bmp"
#define IMG_UNABLE_AREA		"interface/mouse/unablearea.bmp"
#define IMG_HOLD			"interface/mouse/hold.bmp"
#define IMG_NORMAL	 		"interface/mouse/normal.bmp"
#define IMG_NORMAL_CLICK	"interface/mouse/normalclick.bmp"
#define IMG_ATTACT_GUN		"interface/mouse/attact_gun.bmp"
#define IMG_ZOOM_IN			"interface/mouse/zoom_in.bmp"
#define IMG_ZOOM_OUT		"interface/mouse/zoom_out.bmp"
#define IMG_DOOR			"interface/mouse/door.bmp"
#define IMG_COMPAS_INOUT	"interface/mouse/in_out.bmp"
#define IMG_MAGIC_BOOK		"interface/mouse/book_spell.bmp"
#define IMG_CHAKRAM			"interface/mouse/chakram.bmp"
#define IMG_GHOST			"interface/mouse/ghost.bmp"
#define IMG_REPAIR			"interface/mouse/repair.bmp"	// [5/29/2007 Theodoric] 내구도 수리 :: 마우스


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMouse::CMouse()
	: m_FrameCnt(0)
	, m_dwDelay(0)
	, m_dwBaseTime(0)
	, m_pDisplay(NULL)
{
	for(int i= 0; i <MOUSE_KIND_CNT; i++)
		m_pMouseSurf[i]= NULL;

	m_nKind = M_NORMAL;
	m_nCurFrame = 0;
}

CMouse::~CMouse()
{
	DeleteRes();
}


void CMouse::LoadRes(int client_width, int client_height)
{
	// 클라이언트의 해상도 및 정보를 알아보자.
	m_ptCliSize.x = client_width;
	m_ptCliSize.y = client_height;

	DeleteRes();

	if( g_pDisplay )
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[0],	IMG_ATTACT,			42*6,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[1],	IMG_TALK,			32*4,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[2],	IMG_UNABLE_AREA,	32*6,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[3],	IMG_HOLD,			32*6,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[4],	IMG_NORMAL,			32,		32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[5],	IMG_NORMAL_CLICK,	32,		32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[6],	IMG_ATTACT_GUN,		31*6,	31);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[7],	IMG_ZOOM_IN,		32*3,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[8],	IMG_ZOOM_OUT,		32*3,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[9],	IMG_DOOR,			32*6,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[10],	IMG_COMPAS_INOUT,	46*4,	46);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[11],	IMG_MAGIC_BOOK,		42*6,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[12],	IMG_CHAKRAM,		42*6,	32);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[13],	IMG_GHOST,			3,		3);
		g_pDisplay->CreateSurfaceFromBitmap( &m_pMouseSurf[14],	IMG_REPAIR,			32,		32);	// [5/29/2007 Theodoric] 내구도 수리 :: 마우스
	}

	for(int i=0; i<MOUSE_KIND_CNT; i++)
	{
		if( m_pMouseSurf[i] )
			m_pMouseSurf[i]->SetColorKey(RGB(0, 0, 0));
	}

	m_dwBaseTime = timeGetTime();
	m_nCurFrame = 0;

	m_nKind = M_NORMAL;
}

void CMouse::DeleteRes()
{
	for(int i=0; i<MOUSE_KIND_CNT; i++)
		SAFE_DELETE(m_pMouseSurf[i]);
}

void CMouse::SetMouseType(int type)
{
	if( m_nKind == M_REPAIR )
	{
		if( type == M_REPAIR )
		{
			m_nKind = M_NORMAL;
		}
	}
	else
	{
		m_nKind = type;
		m_nCurFrame = 0;
	}
}

void CMouse::Draw(POINT point)
{
	if (m_nKind == M_NON)
		return;

	RECT rc;

	DWORD	dwCurTime;
	int		Cx = 0;
	int		Cy = 0;
	int		tmp = 0;

	if(m_nKind == M_NORMAL || m_nKind == M_NORMAL_CLICK || m_nKind == M_GHOST)
		m_FrameCnt = 1;
	if(m_nKind == M_TALK )
		m_FrameCnt = 4;
	if(m_nKind == M_ATTACT || m_nKind == M_HOLD || m_nKind == M_UNABLE_AREA || m_nKind == M_ATTACT_GUN ||
			m_nKind ==M_DOOR || m_nKind == M_MAGIC_BOOK || m_nKind == M_CHAKRAM)
		m_FrameCnt = 6;
	if(m_nKind == M_ZOOM_IN || m_nKind == M_ZOOM_OUT)
		m_FrameCnt = 3;
	if(m_nKind == M_COMPAS_INOUT)
		m_FrameCnt = 4;

	dwCurTime = timeGetTime();

	if(dwCurTime - m_dwBaseTime >= 100)
	{
		m_dwBaseTime = dwCurTime;

		m_nCurFrame++;
		if(m_nCurFrame > m_FrameCnt-1)
			m_nCurFrame = 0;
	}

	if( m_pMouseSurf[m_nKind] )
	{
		Cx = m_pMouseSurf[m_nKind]->GetWidth() / m_FrameCnt;
		Cy = m_pMouseSurf[m_nKind]->GetHeight();
	}
	SetRect(&rc, Cx*m_nCurFrame, 0, Cx*m_nCurFrame+Cx, Cy);

	if(m_nKind == M_NON)
		return;

//-----------클리핑 처리
// by evilkiki 2009.06.23 왜? 포인트를 뺄까?
// 	if( m_nKind == M_HOLD
// 		|| m_nKind == M_TALK
// 		|| m_nKind == M_ATTACT_GUN
// 		|| m_nKind == M_ZOOM_IN
// 		|| m_nKind == M_ZOOM_OUT)
// 	{
// 		point.x -= 16;
// 		point.y -= 16;
// 	}

	if(point.x <0 )
		point.x = 0;

	if (point.x >= m_ptCliSize.x)
		point.x = m_ptCliSize.x - 1;

	if(point.y<0)
		point.y = 0;

	if (point.y >= m_ptCliSize.y)
		point.y = m_ptCliSize.y - 1;

	if(point.x+(rc.right - rc.left) >= m_ptCliSize.x)
		rc.right = rc.right - ((point.x +(rc.right-rc.left)) - m_ptCliSize.x);

	if(point.y+(rc.bottom-rc.top)>=m_ptCliSize.y)
		rc.bottom -=((point.y+(rc.bottom-rc.top))-m_ptCliSize.y);

	if( g_pDisplay && m_pMouseSurf[m_nKind] )
		g_pDisplay->Blt(point.x, point.y, m_pMouseSurf[m_nKind]->GetDDrawSurface(), &rc , DDBLTFAST_SRCCOLORKEY);
}









