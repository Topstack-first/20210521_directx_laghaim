#include "stdafx.h"
// QlistBox.cpp: implementation of the CQlistBox class.
//
//////////////////////////////////////////////////////////////////////

#include "QlistBox.h"
#include "ddutil.h"
#include "headers.h"
#include "ListBox.h"
#include "dxutil.h"

#include "textbox.h"
#include "main.h"
#include "LoadEffectSound.h"
#include "Nk2DFrame.h"


extern char g_strFontName[];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQlistBox :: CQlistBox ()
	: m_PosX(0)
	, m_PosY(0)
	, m_Width(0)
	, m_Height(0)
	, m_ImgWidth(0)
	, m_BtnHeight(0)
	, m_SbarHeight(0)
	, m_Head(0)
	, m_ViewCount(0)
	, m_Tem(0)
	, m_DecBtnSurf(NULL)
	, m_IncBtnSurf(NULL)
	, m_SbarSurf(NULL)
	, m_Font(NULL)
	, m_FontSize(0)
	, m_bScrollFlag(FALSE)
	, m_StartViewCount(0)
	, m_bViewAll(FALSE)
	, m_bViewNormal(FALSE)
	, m_bViewParty(FALSE)
	, m_bViewGuild(FALSE)
	, m_bViewShout(FALSE)
	, m_bViewMasterMsg(FALSE)
{
	memset(m_TextQ, 0, sizeof(m_TextQ));
	memset(m_TextColor, 0, sizeof(m_TextColor));
	memset(m_OutputText, 0, sizeof(m_OutputText));
	memset(m_OutputTextColor, 0, sizeof(m_OutputTextColor));
	memset(m_TextSurf, 0, sizeof(m_TextSurf));
	memset(m_ExtraInfo, 0, sizeof(m_ExtraInfo));
	memset(m_OutputExtraInfo, 0, sizeof(m_OutputExtraInfo));
	memset(m_DrawExtraInfo, 0, sizeof(m_DrawExtraInfo));

	//		  X		Y	Width	Height	View count fontsize
	CQlistBox(100, 100, 200,	200,	3,			16);//FontSize 16
}


CQlistBox :: CQlistBox (int x, int y, int w, int h, int view_count, int font_size)
	: m_ImgWidth(0)
	, m_BtnHeight(0)
	, m_SbarHeight(0)
	, m_Font(NULL)
	, m_FontSize(0)
{
	memset(m_TextColor, 0, sizeof(m_TextColor));
	memset(m_OutputTextColor, 0, sizeof(m_OutputTextColor));

	SetValue(x, y, w, h, view_count);

	m_DecBtnSurf = NULL;
	m_IncBtnSurf = NULL;
	m_SbarSurf = NULL;
	ZeroMemory( m_TextQ, sizeof(char) * MAX_Q * MAX_CHAT_STRING_LENGTH );
	ZeroMemory( m_OutputText, sizeof(char) * MAX_Q * MAX_CHAT_STRING_LENGTH );
	memset(m_ExtraInfo, 0, sizeof(m_ExtraInfo));
	memset(m_OutputExtraInfo, 0, sizeof(m_OutputExtraInfo));
	memset(m_DrawExtraInfo, 0, sizeof(m_DrawExtraInfo));

	for (int i = 0; i < MAX_DRAW_Q; i++)
		m_TextSurf[i] = NULL;

	m_Head = 0;

	// 필터링 옵션 플래그.
	m_bViewAll = TRUE;
	m_bViewNormal = FALSE;
	m_bViewParty = FALSE;
	m_bViewGuild = FALSE;
	m_bViewShout = FALSE;
	m_bViewMasterMsg = FALSE;

	m_bScrollFlag = FALSE;

}

void CQlistBox::SetValue(int x, int y, int w, int h, int view_count)
{
	m_PosX = x;
	m_PosY = y;
	m_Width = w;
	m_Height = h;
	m_Tem = 0;

	m_ViewCount = view_count;
	m_StartViewCount = 0;

}


//----------------------------------------------------------------------------
//-- Destructor
//----------------------------------------------------------------------------
//-- Destructor

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CQlistBox :: ~CQlistBox ()
{
	DeleteRes();
}

HRESULT CQlistBox::RestoreSurfaces()
{
	if (g_pDisplay)
		SetImage(m_FontSize);

	return S_OK;
}

void CQlistBox::DeleteRes()
{
	m_Scroll.DeleteRes(); // 파티개편 시 추가.

	DeleteImg();
	DeleteTextSurf();
}

void CQlistBox::DeleteImg()
{
	SAFE_DELETE (m_DecBtnSurf);
	SAFE_DELETE (m_IncBtnSurf);
	SAFE_DELETE (m_SbarSurf);
}


void CQlistBox::DeleteTextSurf()
{
	for (int i = 0; i < MAX_DRAW_Q; i++)
		SAFE_DELETE(m_TextSurf[i]);
}

void CQlistBox::SetImage (int FontSize)//,  char *DecBtn, char *IncBtn, char *ScrollBar,
//int img_width, int btn_height, int sbar_height)
{
	if( !g_pDisplay )
		return;

	DeleteRes();

	m_FontSize = FontSize;

	SetFont (m_FontSize/2, m_FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	m_FontSize = sizeFont.cy;

	for (int i = 0; i < MAX_DRAW_Q; i++)
	{
		g_pDisplay->CreateSurface(&m_TextSurf[i], sizeFont.cx*MAX_TEXT_LEN, sizeFont.cy);

		if( m_TextSurf[i] == NULL )
			continue;

		m_TextSurf[i]->Clear();
		m_TextSurf[i]->SetColorKey(0);
	}

	m_Scroll.LoadRes( "common/scroll_up", "common/scroll_down", "interface/common/scroll_thumb_chat.bmp" );
	m_Scroll.SetRange( MAX_Q - m_ViewCount );
	m_Scroll.SetPos( MAX_Q-1 );
}

void CQlistBox::DrawText(int idx) // 현재 안쓰임.(05-03-04 by 원석) (스크롤 기능 추가하면서 안씀.)
{
	if (!m_TextSurf[idx] || !m_Font)
		return;

	if (m_TextQ[idx])
	{
		m_TextSurf[idx]->Clear();
		//-- Lyul -- 수정 -- shadow
		m_TextSurf[idx]->DrawText(m_Font, m_TextQ[idx], 0, 0, 0 , m_TextColor[idx] ,1, RGB(10, 10, 10));

		m_TextSurf[idx]->SetColorKey(0);
	}
}

int CQlistBox::AddString(char *str, COLORREF textColor)
{
	if( str == NULL )
		return 0;

	memcpy( m_TextQ[0], m_TextQ[1], sizeof(char) * (MAX_Q-1) * MAX_CHAT_STRING_LENGTH );
	strcpy( m_TextQ[MAX_Q-1], str);
	memcpy( &m_TextColor[0], &m_TextColor[1], sizeof(COLORREF) * (MAX_Q-1) ); // 색상도 땡겨줘야할듯.
	m_TextColor[MAX_Q-1] = textColor;

	memcpy( &m_ExtraInfo[0], &m_ExtraInfo[1], sizeof(QlistBox_ExtraInfo) * (MAX_Q-1) );
	m_ExtraInfo[MAX_Q-1].nType = 0;
	m_ExtraInfo[MAX_Q-1].nVnum = 0;
	m_ExtraInfo[MAX_Q-1].nStrLen = 0;

	return 1;
}

int CQlistBox::AddString_ExtraInfo(char* str, COLORREF textColor, int infoType, int infoVnum)
{
	if( str == NULL )
		return 0;

	memcpy( m_TextQ[0], m_TextQ[1], sizeof(char) * (MAX_Q-1) * MAX_CHAT_STRING_LENGTH );
	strcpy( m_TextQ[MAX_Q-1], str);
	memcpy( &m_TextColor[0], &m_TextColor[1], sizeof(COLORREF) * (MAX_Q-1) ); // 색상도 땡겨줘야할듯.
	m_TextColor[MAX_Q-1] = textColor;

	memcpy( &m_ExtraInfo[0], &m_ExtraInfo[1], sizeof(QlistBox_ExtraInfo) * (MAX_Q-1) );
	m_ExtraInfo[MAX_Q-1].nType = infoType;
	m_ExtraInfo[MAX_Q-1].nVnum = infoVnum;
	m_ExtraInfo[MAX_Q-1].nStrLen = strlen(str);

	return 1;
}

HRESULT CQlistBox::Clear(CSurface *surf)
{
	if (!surf || !m_Font)
		return E_FAIL;

	char *blankText = "                                                                                                                                           "; // 42
	surf->DrawText(m_Font, blankText, 0, 0, TRANS_COLOR, TRANS_COLOR,0);

	return S_OK;
}

void CQlistBox::MakeFillterdStr() // 옵션에 따라서 필터링 된 스트링을 아웃풋 버퍼에 넣어준다.
{
	int Count = MAX_Q-1;

	ZeroMemory( m_OutputText, sizeof(char)*MAX_Q*MAX_CHAT_STRING_LENGTH );

	for( int i= MAX_Q-1 ; i >= 0 ; --i )
	{
		if( Count < 0 )
			break;

		if( m_bViewAll )
		{
			strcpy( m_OutputText[Count], m_TextQ[i] );
			m_OutputTextColor[Count] = m_TextColor[i];
			memcpy( &m_OutputExtraInfo[Count], &m_ExtraInfo[i], sizeof(QlistBox_ExtraInfo) );
			Count--;
		}
		else
		{
			if( m_TextColor[i] == RGB(255, 255, 255) && !m_bViewNormal ) // 일반 채팅. say_type 0
			{
				continue; // 스킵.
			}
			else if( m_TextColor[i] == _FCOLOR_SKY && !m_bViewParty ) // 파티 채팅. say_type 3
			{
				continue; // 스킵.
			}
			else if( m_TextColor[i] == RGB(127, 255, 127) && !m_bViewGuild ) // 길드 채팅. say_type 2
			{
				continue; // 스킵.
			}
			else if( (m_TextColor[i] == _FCOLOR_YELLOW || m_TextColor[i] == RGB(230, 230, 50)) && !m_bViewShout ) // 외침 채팅. say_type 6
			{
				continue; // 스킵.
			}
			else if( (m_TextColor[i]==RGB(150, 150, 250)||m_TextColor[i]==_FCOLOR_RED) && !m_bViewMasterMsg ) // 공지 채팅. say_type 4 (빨간색 상태 메세지(say_type -1)도 이쪽에 넣는다.)
			{
				continue; // 스킵.
			}
			else // 필터링에서 살아남은 텍스트만 넣어준다.
			{
				strcpy( m_OutputText[Count], m_TextQ[i] ); // 필터링 조건에 맞는 스트링은 넣고
				m_OutputTextColor[Count] = m_TextColor[i]; // 색도 저장.
				memcpy( &m_OutputExtraInfo[Count], &m_ExtraInfo[i], sizeof(QlistBox_ExtraInfo) );
				Count--; // 카운트 증가. 이렇게 해야 m_OutputText에 비어있는 줄 없이 차곡차곡 쌓인다.
			}
		}
	}

}

void CQlistBox::UpdateSurface() // 출력할 서피스 셋트를 작성한다.
{
	MakeFillterdStr();

	// 여기서 각종 출력 옵션들을 체크. 적용시킨다. 현재는 옵션이 없음.
	for( int i = 0 ; i < m_ViewCount ; ++i ) // 보여야 할 갯수 만큼만 처리.
	{
		m_TextSurf[i]->Clear(); // 일단 지우고.
		m_TextSurf[i]->DrawText(m_Font, m_OutputText[ m_Scroll.GetPos()+1+i ], 0, 0, 0 , m_OutputTextColor[ m_Scroll.GetPos()+1+i ] ,1, RGB(10, 10, 10));
		memcpy(&m_DrawExtraInfo[i], &m_OutputExtraInfo[m_Scroll.GetPos()+1+i], sizeof(QlistBox_ExtraInfo));
	}
}

// by evilkiki 2009.09.22
void CQlistBox::Draw()
{
	UpdateSurface();

	for( int i= 0; i < m_ViewCount; ++i )
	{
		g_pDisplay->Blt(m_PosX, m_PosY+((m_FontSize+m_Tem)*i) + pCMyApp->Font_AddPosY, m_TextSurf[i]);
	}

	if( m_bScrollFlag )
	{
		m_Scroll.Draw();
	}
}

extern BOOL g_bAdmin2;
LRESULT CQlistBox::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int		x = LOWORD(lParam);
	int		y = HIWORD(lParam);

	switch( msg )
	{
	case WM_LBUTTONDOWN :
		{
			if( g_bAdmin2 == FALSE )
				return E_FAIL;

			SIZE	sizeFont;
			getFontSize(&sizeFont, m_Font, "A", 1);

			int width = 0;
			int heightStart = 0;
			int heightEnd = 0;
			for( int i = 0; i < m_ViewCount; ++i )
			{
				if( m_DrawExtraInfo[i].nType == 0 )
					continue;

				width = m_DrawExtraInfo[i].nStrLen * sizeFont.cx;

				if( m_PosX > x || m_PosX + width < x )
					continue;

				heightEnd = m_PosY + ((m_FontSize + m_Tem) * i);
				heightStart = m_PosY + ((m_FontSize + m_Tem) * (i + 1));

				if( heightStart < y || heightEnd > y )
					continue;

				char buf[MAX_PATH] = {0,};
				if( m_DrawExtraInfo[i].nType == 1 )
				{
					if( GetKeyState(VK_CONTROL) & 0x80 )
					{
						sprintf(buf, "load %d 10 ?", m_DrawExtraInfo[i].nVnum);
						g_pNk2DFrame->InsertPopup(buf, TYPE_NOR_OKCANCLE, POPUP_GMCOMMAND_LOADITEM_10, 30, 0, (void*)m_DrawExtraInfo[i].nVnum);
					}
					else
					{
						sprintf(buf, "load %d ?", m_DrawExtraInfo[i].nVnum);
						g_pNk2DFrame->InsertPopup(buf, TYPE_NOR_OKCANCLE, POPUP_GMCOMMAND_LOADITEM, 30, 0, (void*)m_DrawExtraInfo[i].nVnum);
					}
				}
				else if( m_DrawExtraInfo[i].nType == 2 )
				{
					if( GetKeyState(VK_CONTROL) & 0x80 )
					{
						sprintf(buf, "gonpc %d ?", m_DrawExtraInfo[i].nVnum);
						g_pNk2DFrame->InsertPopup(buf, TYPE_NOR_OKCANCLE, POPUP_GMCOMMAND_GONPC, 30, 0, (void*)m_DrawExtraInfo[i].nVnum);
					}
					else
					{
						sprintf(buf, "gomob %d ?", m_DrawExtraInfo[i].nVnum);
						g_pNk2DFrame->InsertPopup(buf, TYPE_NOR_OKCANCLE, POPUP_GMCOMMAND_GOMOB, 30, 0, (void*)m_DrawExtraInfo[i].nVnum);
					}
				}
				else
				{
					return E_FAIL;
				}

				return S_OK;
			}
		}
		break;
	}

	return E_FAIL;
}



//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font 중 하나를 받아온다.

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경
void CQlistBox :: SetFont (int f_w, int f_h, char *fontname)
{
	if( fontname == NULL )
		return;
	// Font 생성
	m_Font = G_GetFont(f_h,0,fontname);
}



