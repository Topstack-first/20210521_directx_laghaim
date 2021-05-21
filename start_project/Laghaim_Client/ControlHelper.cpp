#include "stdafx.h"
// ControlHelper.cpp: implementation of the CControlHelper class.
//
//////////////////////////////////////////////////////////////////////


#include <time.h> ///07-05-03 ���� ��¥ ǥ������ �߰�
#include "headers.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "UIMgr.h"
#include "main.h"
#include "NkCharacter.h"
#include "tcpipcon.h"
#include "ControlHelper.h"
#include <process.h>
#include "textbox.h"
#include "controlmap.h"
#include "controlbottom.h"
#include "textoutbox.h"
#include "LoadEffectSound.h"
#include "g_stringmanager.h"
#include "ControlLetter.h"
#include "window_resource.h"

#define HELPER_FONTSIZE 12
#define HELPER_FONTCOLOR RGB(255, 255, 0)

extern char					commOutBuf[512];





extern char					g_strFontName[];
extern DWORD				g_dwLangType;
extern BOOL					g_bUseVoiceChat;
extern DWORD	g_dwClientCountry; ///07-04-23

extern int		gMapWidth;
extern int		gMapHeight;
extern int		gMapExtention;

void VoiceChatThread(LPVOID lParam);


CControlHelper::CControlHelper()
	: m_reject(false)
	, m_nExplan(0)
	, m_PartyState(0)
	, m_bIsMaster(FALSE)
	, m_RequestMode(0)
{
	Init();
}

CControlHelper::~CControlHelper()
{
	DeleteRes();
}

void CControlHelper::Init()
{
	m_pBackSur = NULL;
	m_Font = NULL;

	m_CurClientMode = -1;
	m_MemberCount = 0;
	m_ViewMemberNum = 0;
	m_nListNum = -1;

	m_nHelper = -1;
	m_nMember = -1;
	m_nIndex = -1;

	m_pExplanHelperSur = NULL;
	m_pExplanTraineeSur = NULL;
	m_pExplanGiveupSur = NULL;
	m_pExplanFailSur = NULL;
	m_pExplanUnableSur = NULL;
	m_pInfoBoxSeleteSur = NULL;

	for( int i = 0 ; i < MAX_TRAINEE_NUM ; i++ )
	{
		m_pInfoBoxSur[i] = NULL;

		m_chName[i] = NULL;
		m_nLevel[i] = -1;
		m_nRace[i] = -1;
		m_nSex[i] = -1;
		m_nConnect[i] = -1;
		m_dwStartedTime[i] = -1; ///07-05-03 ���� ��¥�� ǥ��

		m_aTraineeList[i].m_Index = -1;
		m_aTraineeList[i].m_nLevel = -1;
		m_aTraineeList[i].m_nRace = -1;
		m_aTraineeList[i].m_nSex = -1;
		m_aTraineeList[i].m_nConnect = -1;
		m_aTraineeList[i].m_dwStartedTime = -1; ///07-05-03 ���� ��¥�� ǥ��
	}

	m_LetterSendBtn.SetFileName( "helper/btn_lettersend" );		// ���� ������
	m_HelperRejectBtn.SetFileName( "helper/btn_helperreject" );		// �İ��� �ź�
	m_TraineegiveupBtn.SetFileName( "helper/btn_traineegiveup" );	// �߽��� ����


	m_bHelperAccept = TRUE;
	m_bInfoClick = FALSE;
	m_nClickCount = 0;

	m_CloseBtn.SetFileName("common/btn_close_01");
	m_CloseBtn.SetDisable(false);

	SetFont(5, 12, g_strFontName);
}

void CControlHelper::LoadRes()
{
	if( !g_pDisplay )
		return;

	int x = 0, y = 0;
	RECT rcClient;

	m_LetterSendBtn.LoadRes();
	m_HelperRejectBtn.LoadRes();
	m_TraineegiveupBtn.LoadRes();
	m_CloseBtn.LoadRes();

	GetClientRect(g_pDisplay->GetHWnd(), &rcClient);

	for( int i = 0 ; i < MAX_TRAINEE_NUM ; i++ )
	{
		m_NameTex[i].Init(HELPER_FONTSIZE, HELPER_FONTCOLOR, 0, 0, -1, -1, TRUE);
		m_NameTex[i].SetVisible(TRUE);
		m_LevelTex[i].Init(HELPER_FONTSIZE, HELPER_FONTCOLOR, 0, 0, -1, -1, TRUE);
		m_LevelTex[i].SetVisible(TRUE);
		m_RaceTex[i].Init(HELPER_FONTSIZE, HELPER_FONTCOLOR, 0, 0, -1, -1, TRUE);
		m_RaceTex[i].SetVisible(TRUE);
		m_SexTex[i].Init(HELPER_FONTSIZE, HELPER_FONTCOLOR, 0, 0, -1, -1, TRUE);
		m_SexTex[i].SetVisible(TRUE);
		m_ConnectTex[i].Init(HELPER_FONTSIZE, HELPER_FONTCOLOR, 0, 0, -1, -1, TRUE);
		m_ConnectTex[i].SetVisible(TRUE);
		m_DayLimitTex[i].Init(HELPER_FONTSIZE, HELPER_FONTCOLOR, 0, 0, -1, -1, TRUE); ///07-05-03 ���� ��¥�� ���� ��� �߰�
		m_DayLimitTex[i].SetVisible(TRUE);

		g_pDisplay->CreateSurfaceFromBitmap( &m_pInfoBoxSur[i], "interface/helper/menu_list_trainee.bmp");
		m_pInfoBoxSur[i]->SetColorKey(TRANS_COLOR);
	}

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pExplanHelperSur, "interface/description/helper/helperexplan.bmp") ) )
		return;   //�İ��ο� ���� ����
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pExplanTraineeSur, "interface/description/helper/tranieeexplan.bmp") ) )
		return; //�߽����� ���� ����
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pExplanGiveupSur, "interface/description/helper/traineegiveup.bmp") ) )
		return;  //�߽��� ���⿡ ���� ����
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pExplanFailSur, "interface/description/helper/helperfail.bmp") ) )
		return;	   //�İ��� ���� ����
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pExplanUnableSur, "interface/description/helper/helperunable.bmp") ) )
		return;   //�İ��� �ý��� ��� �Ұ� ����
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pInfoBoxSeleteSur, "interface/helper/menu_selete_trainee.bmp") ) )
		return;   //�İ���/�߽��� ���� ����

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/helper/menu_helper.bmp") ) )
		return;

	m_reject_helper[0].Init( "helper/helper_reject_on" );
	m_reject_helper[1].Init( "helper/helper_reject_off" );
	m_reject = false;

	if( m_pBackSur )
	{
		m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth();
		m_pBackSur->Ypos = 0;
		m_pBackSur->SetColorKey( TRANS_COLOR );

		m_pExplanHelperSur->SetColorKey( TRANS_COLOR );
		m_pExplanTraineeSur->SetColorKey( TRANS_COLOR );
		m_pExplanGiveupSur->SetColorKey( TRANS_COLOR );
		m_pExplanFailSur->SetColorKey( TRANS_COLOR );
		m_pExplanUnableSur->SetColorKey( TRANS_COLOR );
		m_pInfoBoxSeleteSur->SetColorKey( TRANS_COLOR );

		for( int i = 0 ; i < MAX_TRAINEE_NUM ; i++ )
		{
			if( m_pInfoBoxSur[i] )
			{
				m_pInfoBoxSur[i]->Xpos = m_pBackSur->Xpos + 31;
				m_pInfoBoxSur[i]->Ypos = m_pBackSur->Ypos + 70 + m_pInfoBoxSur[i]->GetHeight() * i;
			}
		}

		m_ViewMemberNum = TRAINEE_NUM;
		m_MemberScroll.LoadRes( "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // �ػ� ���� �� ũ�⸦ �ٸ��� �ε�.
		m_MemberScroll.SetRange( 1 );
		m_MemberScroll.SetDrawPos( m_pBackSur->Xpos+235, m_pBackSur->Ypos+72, 275+24 ); // ����� ��ü���⿡���� �����ϴ� ��ü���⸸ ��ǥ�� ���� �������ش�.
		m_ExplanScroll.LoadRes( "common/scroll_up_t01", "common/scroll_dn_t01", "interface/common/scroll_thumb.bmp" ); // �ػ� ���� �� ũ�⸦ �ٸ��� �ε�.
		m_ExplanScroll.SetRange( 1 );
		m_ExplanScroll.SetDrawPos( m_pBackSur->Xpos+235, m_pBackSur->Ypos+72, 275+24 );  // ����� ��ü���⿡���� �����ϴ� ��ü���⸸ ��ǥ�� ���� �������ش�.

		m_LetterSendBtn.SetPosition( m_pBackSur->Xpos+150, m_pBackSur->Ypos+355+24 );
		m_HelperRejectBtn.SetPosition( m_pBackSur->Xpos+48, m_pBackSur->Ypos+355+24 );
		m_TraineegiveupBtn.SetPosition( m_pBackSur->Xpos+48, m_pBackSur->Ypos+355+24 );
		m_CloseBtn.SetPosition(m_pBackSur->Xpos+255, m_pBackSur->Ypos+5);


		m_reject_helper[0].SetPos( m_pBackSur->Xpos+40 , m_pBackSur->Ypos+44 );
		m_reject_helper[1].SetPos( m_pBackSur->Xpos+40 , m_pBackSur->Ypos+44 );

		m_pExplanHelperSur->Xpos = m_pBackSur->Xpos + 18;
		m_pExplanHelperSur->Ypos = m_pBackSur->Ypos + 70;
		m_pExplanTraineeSur->Xpos = m_pBackSur->Xpos + 30;
		m_pExplanTraineeSur->Ypos = m_pBackSur->Ypos + 68;
		m_pExplanGiveupSur->Xpos = m_pBackSur->Xpos + 30;
		m_pExplanGiveupSur->Ypos = m_pBackSur->Ypos + 68;
		m_pExplanFailSur->Xpos = m_pBackSur->Xpos + 30;
		m_pExplanFailSur->Ypos = m_pBackSur->Ypos + 68;
		m_pExplanUnableSur->Xpos = m_pBackSur->Xpos + 30;
		m_pExplanUnableSur->Ypos = m_pBackSur->Ypos + 68;
	}

	m_MemberScroll.SetPos(0);
	m_ExplanScroll.SetPos(0);
}

void CControlHelper::DeleteRes()
{
	SAFE_DELETE(m_pBackSur);
	SAFE_DELETE(m_pExplanHelperSur);
	SAFE_DELETE(m_pExplanTraineeSur);
	SAFE_DELETE(m_pExplanGiveupSur);
	SAFE_DELETE(m_pExplanFailSur);
	SAFE_DELETE(m_pExplanUnableSur);
	SAFE_DELETE(m_pInfoBoxSeleteSur);

	m_MemberScroll.DeleteRes();
	m_ExplanScroll.DeleteRes();
	m_LetterSendBtn.DeleteRes();
	m_HelperRejectBtn.DeleteRes();
	m_TraineegiveupBtn.DeleteRes();
	m_CloseBtn.DeleteRes();

	for( int i = 0; i < MAX_TRAINEE_NUM; i++)
	{
		SAFE_DELETE(m_pInfoBoxSur[i]);

		m_NameTex[i].DeleteRes();
		m_LevelTex[i].DeleteRes();
		m_RaceTex[i].DeleteRes();
		m_SexTex[i].DeleteRes();
		m_ConnectTex[i].DeleteRes();
		m_DayLimitTex[i].DeleteRes(); ///07-05-03 ���� ��¥�� ���� ��� �߰�
	}
}

void CControlHelper::Draw()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur);

	if( m_nHelper == 1 || g_pRoh->m_Level >= 360 )
	{
		m_HelperRejectBtn.Draw();
		if( m_bHelperAccept )
			m_reject_helper[1].Draw();
		else
			m_reject_helper[0].Draw();
	}
	else if( m_nHelper == 0 || g_pRoh->m_Level < 360 )
		m_TraineegiveupBtn.Draw();

	m_LetterSendBtn.Draw();
	m_CloseBtn.Draw();

	if( ( m_nHelper == 0 || m_nHelper == 1 ) && m_nMember > 0)
	{
		m_MemberScroll.Draw();
		DrawMembers();
	}
	else
	{
		m_ExplanScroll.Draw();

		if( m_nExplan == 1 )
		{
			g_pDisplay->Blt(m_pExplanGiveupSur->Xpos, m_pExplanGiveupSur->Ypos, m_pExplanGiveupSur);
		}
		else if( m_nExplan == 2 )
		{
			g_pDisplay->Blt(m_pExplanFailSur->Xpos, m_pExplanFailSur->Ypos, m_pExplanFailSur);
		}
		else if( m_nExplan == 3 )
		{
			g_pDisplay->Blt(m_pExplanUnableSur->Xpos, m_pExplanUnableSur->Ypos, m_pExplanUnableSur);
		}
		else
		{
			if( g_pRoh->m_Level >= 360 )
			{
				RECT TempRect;
				{
					m_ExplanScroll.SetRange( 312 + 1 - 272 ); // +1 ����� �´���.

					int StrDrawHeight = m_ExplanScroll.GetPos() + 272 + 24; // ���� ����� ������ ���� ũ�⺸�� ũ�� �ƿ� ����� ���ع�������. �׷��� ������ �ʿ��ϴ�.

					if( StrDrawHeight > 312 ) // m_TextSurHeight ���Ϸ� �ٿ����Ѵ�.
						StrDrawHeight = 312;

					TempRect.left = 0;
					TempRect.right = 199; // ���� ���δ� Ǯ��...
					TempRect.top = m_ExplanScroll.GetPos(); // ��ũ���� �ȼ����� ��ũ�� �߱⶧���� �ٷ� GetPos()�� �ᵵ �ȴ�.
					TempRect.bottom = StrDrawHeight;
					g_pDisplay->Blt( m_pExplanHelperSur->Xpos, m_pExplanHelperSur->Ypos+5, m_pExplanHelperSur, &TempRect );
				}
			}
			else
				g_pDisplay->Blt(m_pExplanTraineeSur->Xpos, m_pExplanTraineeSur->Ypos+5, m_pExplanTraineeSur);
		}
	}

	char strLoc[30] = {0,}, strLoc2[30] = {0,}, strLoc3[30] = {0,}, strLoc4[30] = {0,}, strLoc5[30] = {0,};

	for(int i = 0; i < m_nMember; i++)
	{
		sprintf(strLoc, "%s", m_chName[i]);
		sprintf(strLoc2, "%d", m_nLevel[i]);

		if( m_nRace[i] ==RACE_BULKAN )
			sprintf(strLoc3,  G_STRING(IDS_CRECHA_BULKAN));
		else if( m_nRace[i] ==RACE_KAILIPTON )
			sprintf(strLoc3,  G_STRING(IDS_CRECHA_KAI));
		else if( m_nRace[i] ==RACE_AIDIA )
			sprintf(strLoc3,  G_STRING(IDS_CRECHA_ADI));
		else if( m_nRace[i] ==RACE_HUMAN )
			sprintf(strLoc3,  G_STRING(IDS_CRECHA_HUNMAN));
		else if( m_nRace[i] ==RACE_FREAK )
			sprintf(strLoc3,  G_STRING(IDS_CHARACTER_HIBRIDER));
		else if( m_nRace[i] ==RACE_PEROM )
			sprintf(strLoc3,  G_STRING(IDS_LHSTRING1893));

		if( m_nSex[i] == 0 )
			sprintf(strLoc4, G_STRING(IDS_CRECHA_MALE));
		else if( m_nSex[i] == 1 )
			sprintf(strLoc4, G_STRING(IDS_CRECHA_FEMALE));

		if( m_nConnect[i] == -1 )
		{
			sprintf(strLoc5, G_STRING(IDS_NOT_CONNECT_GAME));
		}
		else
		{
			sprintf(strLoc5, g_pNk2DFrame->GetWindowRes()->GetZoneName(m_nConnect[i]));
		}

		if( m_chName[i] != NULL && m_nLevel[i] > -1
				&& m_nRace[i] > -1 && m_nSex[i] > -1 )
		{
			m_NameTex[i].SetString(strLoc,RGB(231, 203, 102),RGB(0,0,0));
			m_LevelTex[i].SetString(strLoc2,RGB(231, 203, 102),RGB(0,0,0));
			m_RaceTex[i].SetString(strLoc3,RGB(231, 203, 102),RGB(0,0,0));
			m_SexTex[i].SetString(strLoc4,RGB(231, 203, 102),RGB(0,0,0));
			m_ConnectTex[i].SetString(strLoc5,RGB(231, 203, 102),RGB(0,0,0));

			///07-05-03 ���� ��¥�� ǥ��. �ణ ����� �ʿ��ϴ�.
			time_t end_ct = m_dwStartedTime[i]+(120*24*60*60); // [2009/4/24 Theodoric] 30�Ͽ��� 120�Ϸ� �ٲ�
			time_t now_ct = time(0); // ���� ��¥.(�ʴ���)
			time_t remainder_ct = end_ct-now_ct; // ���� �ʸ� ����.
			int _day = remainder_ct / ( 24*60*60 );
			int _day2 = remainder_ct % ( 24*60*60 );
			if( _day2 )
				_day++;

//			struct tm ti;
// 			memcpy (&ti, localtime (&remainder_ct), sizeof(struct tm)); // ȯ��.
//			sprintf(strLoc, "%d %s", ti.tm_mday, G_STRING(IDS_DAY));
			sprintf(strLoc, "%d %s", _day, G_STRING(IDS_DAY));
			m_DayLimitTex[i].SetString(strLoc,RGB(231, 203, 102),RGB(0,0,0));
		}
	}
}

void CControlHelper::DrawMembers()
{
	if( !g_pDisplay )
		return;

	if( m_nListNum > 0 )
	{
		g_pDisplay->Blt( m_pInfoBoxSeleteSur->Xpos, m_pInfoBoxSeleteSur->Ypos, m_pInfoBoxSeleteSur ); // ���� �ڽ� ����.
	}
	int i = 0, z;

	int memberscrollrange = m_nMember + 1 - m_ViewMemberNum;

	m_MemberScroll.SetRange( memberscrollrange );

	if( m_nMember > m_ViewMemberNum )
	{
		for( i = m_MemberScroll.GetPos() ; i < m_MemberScroll.GetPos() + m_ViewMemberNum ; ++i )
		{
			z = i - m_MemberScroll.GetPos(); // ��� ���̿�.
			g_pDisplay->Blt( m_pInfoBoxSur[z]->Xpos, m_pInfoBoxSur[z]->Ypos, m_pInfoBoxSur[z] ); // ���� �ڽ� ���

			m_NameTex[i].Draw(m_pInfoBoxSur[z]->Xpos + 29, m_pInfoBoxSur[z]->Ypos + 14 , 100 );
			m_LevelTex[i].Draw(m_pInfoBoxSur[z]->Xpos + 167, m_pInfoBoxSur[z]->Ypos + 14 , 20 );
			m_RaceTex[i].Draw(m_pInfoBoxSur[z]->Xpos + 29, m_pInfoBoxSur[z]->Ypos + 33 , 64 );
			m_SexTex[i].Draw(m_pInfoBoxSur[z]->Xpos + 137, m_pInfoBoxSur[z]->Ypos + 33 , 52 );
			m_ConnectTex[i].Draw(m_pInfoBoxSur[z]->Xpos + 29, m_pInfoBoxSur[z]->Ypos + 52 , 105 );
			m_DayLimitTex[i].Draw( m_pInfoBoxSur[z]->Xpos + 160, m_pInfoBoxSur[z]->Ypos + 52 , 24 );
		}
	}
	else
	{
		for( i = 0; i < m_nMember; ++i )
		{
			g_pDisplay->Blt( m_pInfoBoxSur[i]->Xpos, m_pInfoBoxSur[i]->Ypos, m_pInfoBoxSur[i] ); // ���� �ڽ� ���

			m_NameTex[i].Draw(m_pInfoBoxSur[i]->Xpos + 29, m_pInfoBoxSur[i]->Ypos + 14 , 100 );
			m_LevelTex[i].Draw(m_pInfoBoxSur[i]->Xpos + 167, m_pInfoBoxSur[i]->Ypos + 14 , 20 );
			m_RaceTex[i].Draw(m_pInfoBoxSur[i]->Xpos + 29, m_pInfoBoxSur[i]->Ypos + 33 , 64 );
			m_SexTex[i].Draw(m_pInfoBoxSur[i]->Xpos + 137, m_pInfoBoxSur[i]->Ypos + 33 , 52 );
			m_ConnectTex[i].Draw(m_pInfoBoxSur[i]->Xpos + 29, m_pInfoBoxSur[i]->Ypos + 52 , 105 );
			m_DayLimitTex[i].Draw( m_pInfoBoxSur[i]->Xpos + 160, m_pInfoBoxSur[i]->Ypos + 52 , 24 );
		}
	}
}



BOOL CControlHelper::IsInside(int x, int y)
{
	if (m_pBackSur == NULL)
		return FALSE;

	return m_pBackSur->IsIn( x, y );
}

LRESULT CControlHelper::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;
	char strTemp[100];

	if( ( m_nHelper == 0 || m_nHelper == 1 ) && m_nMember > 0 )
	{
		if( m_MemberScroll.MsgProc(hWnd, msg, wParam, lParam)  == S_OK)
			return 1;
	}
	else
	{
		if( m_ExplanScroll.MsgProc(hWnd, msg, wParam, lParam)  == S_OK)
			return 1;
	}

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	if (!IsInside(x, y))
		return 0;

	m_TraineegiveupBtn.MsgProc(hWnd, msg, wParam, lParam);
	m_HelperRejectBtn.MsgProc(hWnd, msg, wParam, lParam);

	m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		if( m_nHelper == 1 || g_pRoh->m_Level >= 360 )
		{
			if( m_reject_helper[0].IsIn( x, y ) )
			{
				m_bHelperAccept = !m_bHelperAccept;
				return 1;
			}
		}
		if( int i = IsMemberIndexInside(x, y) )
		{
			m_pInfoBoxSeleteSur->Xpos = m_pBackSur->Xpos + 29;
			int scroll = m_MemberScroll.GetPos() ;
			if( scroll < 0) scroll = 0;
			m_pInfoBoxSeleteSur->Ypos = m_pBackSur->Ypos + 70 + 2 + m_pInfoBoxSur[0]->GetHeight() * ( i - scroll -1);

			m_nListNum = i;
		}
	case WM_LBUTTONUP:
		if( g_pRoh->m_Level < 360 )
		{
			if( m_TraineegiveupBtn.GetState() == BTN_ACTION
					&& m_nHelper == 0 && m_nMember > 0 )	//�߽��� ����
			{
				char strTemp[128] = "";
				sprintf(strTemp, (char*)G_STRING(IDS_REALY_GIVEUP_TRAINEE) );
				g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 34);
			}
		}
		else if( g_pRoh->m_Level >= 360 )
		{
			if( m_HelperRejectBtn.GetState() == BTN_DOWN )
			{
				// popup â
				// m_nListNum
				if(m_nListNum > 0)
				{
					char strTemp[128] = "";
					sprintf(strTemp, G_STRING(IDS_LHSTRING1741) , m_chName[m_nListNum-1] );
					g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, POPUP_HELPER_REJECT);
				}
				else
				{
					// ���� �ȵǾ� ����
				}
			}
		}

		if( g_pNk2DFrame )
		{
			if( m_CloseBtn.GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->ShowHelperWindow(FALSE);
				m_CloseBtn.m_iBtnFlg = BTN_NORMAL;
			}
		}
		return 1;

	case WM_LBUTTONDBLCLK:
		if( int i = IsMemberIndexInside(x, y) ) // ��Ƽ�� �̸� Ŭ������ ��.
		{
			// i-1 ��° �߽����� �̸��� �Ӹ��� ����.
			if( g_pNk2DFrame
					&& g_pNk2DFrame->GetControlBottom() )
			{
				sprintf( strTemp,"/%s ", m_chName[i-1] );
				g_pNk2DFrame->GetControlBottom()->SetFocus();
				g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
				g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText( strTemp );
			}
		}
		return 1;
	}
	return 0;
}

//-- Global Font �� �ϳ��� �޾ƿ´�.
//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- ��ü �������� Global ������� ����
void CControlHelper::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,0,fontname);
}

int CControlHelper::GetMemberlist( int nIndex )
{
	for( int i = 0; i < m_nMember; ++i )
	{
		if( m_aTraineeList[i].m_Index >= 0 && m_aTraineeList[i].m_Index == nIndex )
			return i;
	}
	return -1;
}

void CControlHelper::SetMember(int nHelper, int nMember )
{
	m_nHelper = nHelper;
	m_nMember =	nMember;
}

void CControlHelper::Information( int nIndex , int count, char *ch_name, int nLevel, int nSex, int nRace, int nConnect, DWORD dwStartedTime ) //�İ���/�߽��� ���� ///07-05-03 ���� ��¥�� ���� ���� �߰�
{
	for(int i = 0; i < count; ++i )
	{
		SAFE_DELETE( m_chName[count] );
	}

//	ResetAll();

	if( ch_name )
	{
		int len = strlen(ch_name);
		if (len > 0 )
		{
			m_chName[count] = new char[len+1];
			strcpy(m_chName[count], ch_name);
		}
	}
	m_nLevel[count] = nLevel;
	m_nSex[count] = nSex;
	m_nRace[count] = nRace;
	m_nConnect[count] = nConnect;
	m_dwStartedTime[count] = dwStartedTime; ///07-05-03 ���� ��¥�� ǥ���ϱ� ���� ���� ������

	m_aTraineeList[count].m_Index = nIndex;
}

LRESULT CControlHelper::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char commOutBuf[256] = "";
	if( nMsg == 1) // ������ư�� �������
		sprintf(commOutBuf, "support ans 1\n");
	else if(nMsg == -1) // �źι�ư�� �������
		sprintf(commOutBuf, "support ans 0\n");

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(commOutBuf);

	return 0L;
}

LRESULT CControlHelper::PopupMsgProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	//
	char commOutBuf[256] = "";
	if( nMsg == 1 && m_nListNum > 0 ) // OK ��ư�� �������
	{
		sprintf(commOutBuf, "support sup_del %s\n" , m_chName[m_nListNum-1] );
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(commOutBuf);

		m_nListNum = 0;
	}

	return 0L;
}


void CControlHelper::AskHelperRequest(char *player_name)
{
	char strTemp[128] = "";
	sprintf(strTemp, (char*)G_STRING(IDS_REQUEST_HELPER), player_name);
	g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_ACCEPTREJECT, 33);
}

void CControlHelper::ConnectState(int nIndex, int nState) //���� ���� ����
{
	int listNum = GetMemberlist(nIndex);
	m_nConnect[listNum] = nState;
}

void CControlHelper::HelperExplan( int nExplan ) //�İ��� �ؽ�Ʈ ����
{
	m_nExplan = nExplan;
}

int CControlHelper::IsMemberIndexInside(int x, int y) // ��� ����Ʈ �������� ����̸� üũ��. // ��� �ε����� +1 �� ������ �����Կ� ����.
{
	if( !m_pBackSur )
		return 0;

	int cx = 0, cy = 0, cz = 0;

	cx = m_pBackSur->Xpos + 31;

	if( m_nMember > m_ViewMemberNum )
	{
		for( int i = 0 ; i < m_ViewMemberNum ; ++i ) // 0���� �����Կ� ����.
		{
			cz = i + m_MemberScroll.GetPos(); // ��� ���̿�.
			cy = m_pInfoBoxSur[i]->Ypos; // ��ư�� y��ǥ�� ����ؼ� ���.

			if( m_pInfoBoxSur[i] )
			{
				if( x > cx && x < cx + m_pInfoBoxSur[i]->GetWidth() && y > cy && y < cy + m_pInfoBoxSur[i]->GetHeight()  )
				{
					return cz+1; // +1�� ���� ����!
				}
			}
		}
	}
	else
	{
		for( int i = 0 ; i < m_nMember ; ++i ) // 0���� �����Կ� ����.
		{
			cy = m_pInfoBoxSur[i]->Ypos; // ��ư�� y��ǥ�� ����ؼ� ���.

			if( m_pInfoBoxSur[i] )
			{
				if( x > cx && x < cx + m_pInfoBoxSur[i]->GetWidth() && y > cy && y < cy + m_pInfoBoxSur[i]->GetHeight()  )
				{
					return i+1; // +1�� ���� ����!
				}
			}
		}
	}

	return 0; // ����Ʈ ������ �´µ� �ȿ� �������� ���� �ƴҰ��.( �ٸ� �޼��� ���ø� ���� �ʿ��ϴ� )
}

void CControlHelper::ResetAll()
{
	m_MemberCount = 0;
	m_ViewMemberNum = 0;
	m_nMember = -1;
	m_nIndex = -1;

	m_pInfoBoxSeleteSur = NULL;

	for( int i = 0 ; i < MAX_TRAINEE_NUM ; i++ )
	{
		m_pInfoBoxSur[i] = NULL;

		m_chName[i] = NULL;
		m_nLevel[i] = -1;
		m_nRace[i] = -1;
		m_nSex[i] = -1;
		m_nConnect[i] = -1;
		m_dwStartedTime[i] = -1; ///07-05-03 ���� ��¥�� ǥ��

		m_aTraineeList[i].m_Index = -1;
		m_aTraineeList[i].m_nLevel = -1;
		m_aTraineeList[i].m_nRace = -1;
		m_aTraineeList[i].m_nSex = -1;
		m_aTraineeList[i].m_nConnect = -1;
		m_aTraineeList[i].m_nConnect = -1;
		m_aTraineeList[i].m_dwStartedTime = -1; ///07-05-03 ���� ��¥�� ǥ��
	}
}
