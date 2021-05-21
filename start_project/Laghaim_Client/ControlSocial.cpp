#include "stdafx.h"


#include "headers.h"
#include "main.h"
#include "ControlSocial.h"
#include "ddutil.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "ListBox.h"
#include "g_stringmanager.h"






extern HINSTANCE			g_hDllInst;

CControlSocial::CControlSocial()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	m_pSurfaceBack = NULL;
	m_SocialList = NULL;
	Init();
}

CControlSocial::~CControlSocial()
{
	DeleteRes();
	SAFE_DELETE( m_SocialList );
}

void CControlSocial::Init()
{
	m_CloseBtn.SetFileName("common/btn_close_01");
	m_SocialList = new CListBox(" ", SOCIAL_LIST_X, SOCIAL_LIST_Y,
								SOCIAL_LIST_WIDTH, SOCIAL_LIST_HEIGHT);
	if( m_SocialList )
	{
		m_SocialList->SetImage("common/scroll_dn_t01", "common/scroll_up_t01", "interface/common/scroll_thumb_01.bmp");
		m_SocialList->SetSelectedLine(-1);
		m_SocialList->SetTextColor(SOCIAL_LIST_FCOLOR);
	}

	// IDS_SOCAIL_CLAP3 : 대한민국 (대한민국)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_CLAP3));
	m_SocialActNum[0] = 105;
	// IDS_SOCAIL_HAND : 손 흔들기 (손)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_HAND));	// 0
	m_SocialActNum[1] = 71;
	// IDS_SOCAI_GREETING : 인사 (인사)
	m_SocialList->AddString(G_STRING(IDS_SOCAI_GREETING));		// 1
	m_SocialActNum[2] = 72;
	// IDS_SOCAIL_KISS : 키스 (키스)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_KISS));
	m_SocialActNum[3] = 73;
	// IDS_SOCAIL_BOW : 절하기 (절)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_BOW));
	m_SocialActNum[4] = 74;
	// IDS_SOCAIL_GENTLE_GREETING : 신사의 인사 (신사의인사)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_GENTLE_GREETING));
	m_SocialActNum[5] = 75;
	// IDS_SOCAIL_PARDON : 용서해주세요 (용서)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_PARDON));
	m_SocialActNum[6] = 76;
	// IDS_SOCAIL_DISCO : 디스코 (디스코)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_DISCO));
	m_SocialActNum[7] = 77;
	// IDS_SOCAIL_JUMP : 점프 (점프)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_JUMP));
	m_SocialActNum[8] = 78;
	// IDS_SOCAIL_CLAP : 박수 (박수)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_CLAP));
	m_SocialActNum[9] = 79;
	// IDS_SOCAIL_DELIGHT_CLAP : 기쁨의 박수 (기쁨)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_DELIGHT_CLAP));
	m_SocialActNum[10] = 80;
	// IDS_SOCAIL_LAUGH : 웃기 (웃기)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_LAUGH));
	m_SocialActNum[11] = 81;
	// IDS_SOCAIL_AIR_SHOW : 공중제비 (공중제비)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_AIR_SHOW));
	m_SocialActNum[12] = 82;
	// IDS_SOCAIL_CHEEK_HIT : 뺨 때리기 (뺨)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_CHEEK_HIT));
	m_SocialActNum[13] = 83;
	// IDS_SOCAIL_KICK : 발차기 (발차기)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_KICK));
	m_SocialActNum[14] = 84;
	// IDS_SOCAIL_MELONG : 메롱 (메롱)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_MELONG));
	m_SocialActNum[15] = 85;
	// IDS_SOCAIL_TTONG : 똥침 (똥침)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_TTONG));
	m_SocialActNum[16] = 86;
	// IDS_SOCAIL_PUT_HEAD : 머리박기 (머리)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_PUT_HEAD));
	m_SocialActNum[17] = 87;
	// IDS_SOCAIL_CRYING : 울기 (울기)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_CRYING));
	m_SocialActNum[18] = 90;
	// IDS_SOCAIL_STRETCHING : 기지개 하품 (기지개)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_STRETCHING));
	m_SocialActNum[19] = 91;
	// IDS_SOCAIL_SNEEZE : 재채기 (재채기)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_SNEEZE));
	m_SocialActNum[20] = 92;
	// IDS_SOCAIL_WHIT_ME : 나를 따르라 (나를)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_WHIT_ME));
	m_SocialActNum[21] = 93;
	// IDS_SOCAIL_COME_HERE : 이리 와보세요 (이리)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_COME_HERE));
	m_SocialActNum[22] = 94;
	// IDS_SOCAIL_OK : 오케바리 (오케)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_OK));
	m_SocialActNum[23] = 95;
	// IDS_SOCAIL_DEEPLY : 곰곰히 생각 (곰곰)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_DEEPLY));
	m_SocialActNum[24] = 96;
	// IDS_SOCAIL_THAT_WAY : 저쪽입니다 (저쪽)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_THAT_WAY));
	m_SocialActNum[25] = 97;
	// IDS_SOCAIL_SIT_DOWN : 앉기 (앉기)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_SIT_DOWN));
	m_SocialActNum[26] = 67;
	// IDS_SOCAIL_WORLDCUP_DANCE : 꼭지점댄스 (꼭지점)
	m_SocialList->AddString(G_STRING(IDS_SOCAIL_WORLDCUP_DANCE));
	m_SocialActNum[27] = 153;
}

void CControlSocial::LoadRes()
{
	if( g_pNk2DFrame )
	{
		m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SAFE_DELETE(m_pSurfaceBack);

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/social/social_back.bmp") ) )
		return;

	m_pSurfaceBack->SetColorKey(TRANS_COLOR);
	m_pSurfaceBack->Xpos = SOCIAL_START_X;
	m_pSurfaceBack->Ypos = SOCIAL_START_Y;


	m_pSurfaceBack->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth())>>1;
	m_pSurfaceBack->Ypos = (g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight())>>1;

	m_CloseBtn.PosX = m_pSurfaceBack->Xpos+221;
	m_CloseBtn.PosY = m_pSurfaceBack->Ypos+6;

	m_CloseBtn.LoadRes();
	if( m_SocialList )
	{
		m_SocialList->SetPosDim(m_pSurfaceBack->Xpos+25, m_pSurfaceBack->Ypos+81, 191, 226);
		m_SocialList->LoadRes();
	}
}

void CControlSocial::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);

	m_CloseBtn.DeleteRes();
	m_SocialList->DeleteRes();
}

void CControlSocial::Draw()
{
	if (!g_pDisplay || !m_pSurfaceBack)
		return;

	g_pDisplay->Blt(m_pSurfaceBack->Xpos,m_pSurfaceBack->Ypos, m_pSurfaceBack);
	m_CloseBtn.Draw();

	if( m_SocialList )
		m_SocialList->Draw();
}

BOOL CControlSocial::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn( x, y );
}

LRESULT CControlSocial::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y, sel;

	switch ( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (m_CloseBtn.GetState() == BTN_ACTION)
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterfaceWindow(FALSE,SOCIAL);
			m_CloseBtn.SetState(BTN_NORMAL);
		}
		if( m_SocialList )
			m_SocialList->GetMessage(hWnd, msg, wParam, lParam );

		if (!IsInside(x, y))
			return 0;

		return 1;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if (m_CloseBtn.IsInside(x, y))
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			if (m_SocialList
					&& (sel = m_SocialList->GetMessage(hWnd, msg, wParam, lParam )) >= 0)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE,SOCIAL);
				m_CloseBtn.SetState(BTN_NORMAL);
				if (g_pRoh
						&& !(g_pRoh->IsCantMove())
						&& !(pCMyApp->GetHeroSwimming())
						&& g_pRoh->IsShown())
				{
					int motion_state = g_pRoh->GetMotionState();
					if( motion_state == CHA_STAND || motion_state == CHA_SOCIAL )
					{
						g_pRoh->SetMotionState(CHA_SOCIAL, m_SocialActNum[sel]);
						pCMyApp->StopHero();
						if (g_pTcpIp)
						{
							sprintf(m_MsgBuf, "social %d\n", m_SocialActNum[sel]);
							g_pTcpIp->SendNetMessage(m_MsgBuf);
						}
					}
					else
					{
						g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1837) , -1 );
					}
				}
			}
		}
		else
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterfaceWindow(FALSE,SOCIAL);
			return 0;
		}
		return 1;
	}
	return 0;
}

void CControlSocial::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

// 주어진 인덱스에 의해서 사교동작 action !!
void CControlSocial::SetSocialActNum(int sel)
{
	if (g_pRoh
			&& !(g_pRoh->IsCantMove())
			&& !(pCMyApp->GetHeroSwimming())
			&& g_pRoh->IsShown())
	{
		g_pRoh->SetMotionState(CHA_SOCIAL, m_SocialActNum[sel]);
		pCMyApp->StopHero();

		if (g_pTcpIp)
		{
			sprintf(m_MsgBuf, "social %d\n", m_SocialActNum[sel]);
			g_pTcpIp->SendNetMessage(m_MsgBuf);
		}
	}
}

