#include "stdafx.h"

#include "headers.h"
#include "ddutil.h"
#include "Nk2DFrame.h"
#include "UIMgr.h"
#include "main.h"
#include "NkCharacter.h"
#include "tcpipcon.h"
#include "ControlParty.h"
#include <process.h>
#include "textbox.h"
#include "controlmap.h"
#include "controlbottom.h"
#include "textoutbox.h"
#include "LoadEffectSound.h"
#include "g_stringmanager.h"

extern char					commOutBuf[512];





extern char					g_strFontName[];
extern DWORD				g_dwLangType;
extern BOOL					g_bUseVoiceChat;
extern DWORD				g_dwClientCountry;

extern int		gMapWidth;
extern int		gMapHeight;
extern int		gMapExtention;

void VoiceChatThread(LPVOID lParam);


CControlParty::CControlParty()
	: m_fStatrTime(0)
{
	m_pBackSur = NULL;
	m_Font = NULL;
	m_pNameBox = NULL;
	m_pMasterMark = NULL;

	m_pPartyCountSur = NULL; 	// [2008/2/28 Theodoric] ��Ƽ�� ǥ��

	for( int i = 0 ; i < MAX_PARTY_NUM_BOSSRAID_FINAL ; i++ )
	{
		m_aPartyList[i].m_Name = NULL;
		m_aPartyList[i].m_pSurName = NULL;
	}
	Init();

	g_bUseVoiceChat = FALSE;
	m_bShowPartyVital = FALSE;
}

//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- m_Font �� handle �� �����´�. ���� �ı��� �ʿ䰡 ����.
CControlParty::~CControlParty()
{
	DeleteRes();
	Init();
}

void CControlParty::Init()
{
	m_CurClientMode = -1;
	m_bIsMaster = FALSE;
	m_MemberCount = 0;
	m_ViewMemberNum = 0;
	m_PartyState = PARTY_STATE_NONE;
	m_RequestMode = -1; // -1�� �ʱ�ȭ.

	for( int i = 0 ; i < MAX_PARTY_NUM_BOSSRAID_FINAL ; i++ )
	{
		m_aPartyList[i].m_Index = -1;
		m_aPartyList[i].m_Vital = 0;
		m_aPartyList[i].m_MaxVital = 0;
		SAFE_DELETE( m_aPartyList[i].m_Name );
		SAFE_DELETE( m_aPartyList[i].m_pSurName );
	}

	m_ViewMemberState.SetFileName( "party/btn_viewstate" );  // ��Ƽ���� ����.
	m_RequestRejection.SetFileName( "party/btn_rejection" ); // �ʴ�ź�.

	m_AskjoinBtn.SetFileName("party/btn_askjoin");		// ���Խ�û.
	m_AskoutBtn.SetFileName("party/btn_askout");		// Ż���û

	m_CloseBtn.SetFileName("common/btn_close_01");
	m_CloseBtn.SetDisable(false);

	m_DisbandBtn.SetFileName("party/btn_allout");		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)
	m_DumyMandateBtn.SetFileName("party/btn_mandate");		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)
	m_DumyAbsOutBtn.SetFileName("party/btn_out");		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)

	SetFont(5, 12, g_strFontName);
}

void CControlParty::LoadRes()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/party/party.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pNameBox, "interface/party/namebox.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pMasterMark, "interface/party/master_mark.bmp");

	m_pBackSur->SetColorKey( TRANS_COLOR );
	m_pMasterMark->SetColorKey( TRANS_COLOR );

	int i = 0;
	int x = 0, y = 0;
	RECT rcClient;

	m_ExpGetRule.SetItemSize( 151, 16, 7, 4 ); // ����ġ �й� ���.
	m_ExpGetRule.LoadRes("party/btn_dropdown", "interface/party/comboback.bmp", "interface/party/ListBack1.bmp");

	m_ExpGetRule.DeleteAllItem(); // �̰� ������� �������� � �������� �ȵ���.
	//-- IDS_PARTY_EXP_RULE : ����ġ ��
	m_ExpGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_EXP_RULE), 13, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_EXP_RULE_DAMAGE : ������
	m_ExpGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_EXP_RULE_DAMAGE), 13, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_EXP_RULE_EQUAL : �յ�
	m_ExpGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_EXP_RULE_EQUAL), 13, RGB(255, 210, 0), RGB(255,255,255) );

	m_ItemGetRule.SetItemSize( 151, 16, 7, 4 ); // ������ �й� ���.
	m_ItemGetRule.LoadRes("party/btn_dropdown", "interface/party/comboback.bmp", "interface/party/ListBack2.bmp");

	m_ItemGetRule.DeleteAllItem(); // �̰� ������� �������� � �������� �ȵ���.
	//-- IDS_PARTY_ITEM_RULE : ������ ��
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE), 12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_DAMAGE : ������ �켱
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_DAMAGE), 12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_FIRSTGET : �Լ� �켱
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_FIRSTGET), 12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_RANDOM : ������
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_RANDOM),12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_LEADER : ��Ƽ�� �켱
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_LEADER),12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_TURN : ����
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_TURN),12, RGB(255, 210, 0), RGB(255,255,255) );

	m_ViewMemberState.LoadRes();  // ��Ƽ���� ����.
	m_RequestRejection.LoadRes(); // �ʴ�ź�.
	m_AskjoinBtn.LoadRes();
	m_AskoutBtn.LoadRes();
	m_CloseBtn.LoadRes();
	m_DisbandBtn.LoadRes();		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)
	m_DumyMandateBtn.LoadRes();		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)
	m_DumyAbsOutBtn.LoadRes();		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)


	GetClientRect(g_pDisplay->GetHWnd(), &rcClient);



	m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth();
	m_pBackSur->Ypos = 0;

	m_ViewMemberNum = MEMBER_NUM;
	m_MemberScroll.LoadRes( "common/scroll_up", "common/scroll_down", "interface/common/scroll_bar.bmp" ); // �ػ� ���� �� ũ�⸦ �ٸ��� �ε�.
	m_MemberScroll.SetRange( MAX_PARTY_NUM + 1 - m_ViewMemberNum ); // +1 ����� �´���.

	// scroll size = 9
	m_MemberScroll.SetDrawPos( m_pBackSur->Xpos+250, m_pBackSur->Ypos+71, 212 ); // ����� ��ü���⿡���� �����ϴ� ��ü���⸸ ��ǥ�� ���� �������ش�.

	m_ExpGetRule.SetPosition( m_pBackSur->Xpos+44, m_pBackSur->Ypos+310 ); // ����ġ �й� ���.
	m_ItemGetRule.SetPosition( m_pBackSur->Xpos+44, m_pBackSur->Ypos+342); // ������ �й� ���.

	m_ViewMemberState.SetPosition( m_pBackSur->Xpos+36, m_pBackSur->Ypos+382 );
	m_RequestRejection.SetPosition( m_pBackSur->Xpos+103, m_pBackSur->Ypos+382 );
	m_AskjoinBtn.SetPosition( m_pBackSur->Xpos+170, m_pBackSur->Ypos+382 );

	m_AskoutBtn.SetPosition( m_pBackSur->Xpos+113, m_pBackSur->Ypos+451 );

	m_CloseBtn.SetPosition(m_pBackSur->Xpos+257, m_pBackSur->Ypos+6);

	// ��� ����Ʈ ���� ��ġ�ϴ� ��ư�� ��ġ����.
	x = m_pBackSur->Xpos + PARTY_LIST_START_X;
	y = m_pBackSur->Ypos + PARTY_LIST_START_Y;

	m_DisbandBtn.SetPosition( x+m_pNameBox->GetWidth()+PARTY_LIST_LINE_X_GAB-4, y - 10 -10 );

	for( i = 0 ; i < MEMBER_NUM ; ++i ) // ����Ʈ�� ��ġ�ϴ� ��ư���� ��ġ�� �����ش�.(����� ��ư�� �ƴ϶� ��ġ�� ��ư�ӿ� ����)
	{
		y = m_pBackSur->Ypos + PARTY_LIST_START_Y + (i*PARTY_LIST_LINE_Y_GAB);

		m_MandateBtn[i].Init( "party/btn_mandate" );
		m_AbsOutBtn[i].Init( "party/btn_out" );
		m_MandateBtn[i].SetPosition(x+m_pNameBox->GetWidth()+PARTY_LIST_LINE_X_GAB-4, y - 10 -10 );
		m_AbsOutBtn[i].SetPosition( x+m_pNameBox->GetWidth()+m_MandateBtn[i].Width+PARTY_LIST_LINE_X_GAB-4, y -10 -10 );
	}
}

void CControlParty::DeleteRes()
{
	int i = 0;

	SAFE_DELETE(m_pPartyCountSur);	// [2008/2/28 Theodoric] ��Ƽ�� ǥ��
	SAFE_DELETE(m_pBackSur);
	SAFE_DELETE(m_pNameBox);
	SAFE_DELETE(m_pMasterMark);

	m_MemberScroll.DeleteRes();
	m_ExpGetRule.DeleteRes();
	m_ItemGetRule.DeleteRes();
	m_ViewMemberState.DeleteRes();  // ��Ƽ���� ����.
	m_RequestRejection.DeleteRes(); // �ʴ�ź�.
	m_AskjoinBtn.DeleteRes();
	m_AskoutBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_DisbandBtn.DeleteRes();

	for( i = 0 ; i < MEMBER_NUM ; i++ ) // ���� ��ư�� �����ϱ����� ���� ��ư�� �̹��� �������� Ǯ����Ѵ�.(�ȱ׷��� �� ��ư�� �Ҹ��ڿ��� ��������?)
	{
		m_MandateBtn[i].DeleteRes();
		m_AbsOutBtn[i].DeleteRes();
	}

	m_DumyMandateBtn.DeleteRes();		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)
	m_DumyAbsOutBtn.DeleteRes();		// ���� ��ư.(�޽����ޱ⳪ ��ο츦 ���� �ʰ� �̹����� �ε��ؼ� �����ϴ� �뵵�� ���δ�.)
}

HRESULT CControlParty::RestoreSurfaces()
{
	for( int i = 0 ; i < MAX_PARTY_NUM_BOSSRAID_FINAL; i++ )
	{
		if( m_aPartyList[i].m_Name ) // �̸��� �ִ� �༮�� ����.
		{
			SAFE_DELETE( m_aPartyList[i].m_pSurName );
			g_pDisplay->CreateSurfaceFromText( &m_aPartyList[i].m_pSurName
											   , m_Font, m_aPartyList[i].m_Name, RGB(0, 0, 0), RGB(255,255,0) );
			if( m_aPartyList[i].m_pSurName )
				m_aPartyList[i].m_pSurName->SetColorKey(RGB(0, 0, 0));
		}
	}

	// �������� �����ϰ� �� �����ٰ� �ε�.
	DeleteRes();
	LoadRes();

	return S_OK;
}

void CControlParty::Draw()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur);


	// [2008/2/28 Theodoric] ��Ƽ��
	if( m_pPartyCountSur && m_MemberCount > 0)
	{
		g_pDisplay->Blt(m_pPartyCountSur->Xpos, m_pPartyCountSur->Ypos, m_pPartyCountSur);
	}

	m_MemberScroll.Draw();
	DrawMembers();
	m_ViewMemberState.Draw();  // ��Ƽ���� ����.
	m_RequestRejection.Draw(); // �ʴ�ź�.
	m_AskjoinBtn.Draw();
	m_CloseBtn.Draw();

	if( m_ExpGetRule.GetState() > 0 ) // ����Ʈ �������̸� �Ʒ��� �����׸���.
	{
		m_ItemGetRule.Draw();
		m_ExpGetRule.Draw(); // �̳༮���� �ڿ� ������ִ°� ����.(���� ��µǵ���)
	}
	else
	{
		m_ExpGetRule.Draw(); // �̳༮���� �ڿ� ������ִ°� ����.(���� ��µǵ���)
		m_ItemGetRule.Draw();
	}

	switch (m_PartyState)
	{
	case PARTY_STATE_NONE:
		break;
	case PARTY_STATE_REQUEST:
		m_AskoutBtn.Draw(); // ���� ��� ��ư
		break;
	case PARTY_STATE_MEMBER:
		m_AskoutBtn.Draw(); // ���� ��� ��ư
		break;
	}
}

void CControlParty::DrawMembers()
{
	if( !g_pDisplay )
		return;

	int i = 0;
	int x, y, z;

	x = m_pBackSur->Xpos + PARTY_LIST_START_X;

	for( i = m_MemberScroll.GetPos() ; i < m_MemberScroll.GetPos()+m_ViewMemberNum ; i++ )
	{
		if( m_aPartyList[i].m_pSurName ) // �ش� ��ġ�� ����� �������� ���
		{
			z = i - m_MemberScroll.GetPos(); // ��� ���̿�.
			y = m_MandateBtn[z].PosY+ 5; // ��ư�� y��ǥ�� ����ؼ� ���.

			if( i == 0 ) // ���� ó���� ����.
			{
				// ���� ��ũ ���
				g_pDisplay->Blt( x-m_pMasterMark->GetWidth()-10, y-2, m_pMasterMark ); // �̸� ���.

				if( m_bIsMaster ) // �ڽ��� ��Ƽ���̸� �ػ� ��ư�� ���
					m_DisbandBtn.Draw(); // �̸� ������ ��ġ�� ���.
			}

			g_pDisplay->Blt( x, y, m_pNameBox ); // �̸� ���.
			g_pDisplay->Blt( x+2, y+2, m_aPartyList[i].m_pSurName ); // ���� �̸� ���.


			if( m_bIsMaster && i != 0 ) // ��Ƽ���̸� ��ư�鵵 ������ش�.(�ڱ� �ڽ� ����)
			{
				// ����, ���� ��ư ���.
				m_MandateBtn[z].Draw(); // �̸� ������ ��ġ�� ���.
				m_AbsOutBtn[z].Draw(); // �̸� ������ ��ġ�� ���.
			}
		}
	}
}



BOOL CControlParty::IsInside(int x, int y)
{
	// ������ �ڽ��� �������� ���콺üũ�� üũ�ߴ�..
	if( !m_pBackSur )
		return false;

	return m_pBackSur->IsIn(x, y);
}

BOOL CControlParty::IsMemberListInside(int x, int y) // ��� ����Ʈ ����(����̸�, ��Ƽ�� ���� ��ư��) üũ��.
{
	if( !m_pBackSur )
		return FALSE;

	for( int i=0 ; i<m_ViewMemberNum ; i++ )
	{
		if( m_MandateBtn[i].IsInside(x, y) || m_AbsOutBtn[i].IsInside(x, y) )
		{
			return TRUE;
		}
	}

	return FALSE;

// 	RECT rc;
// 	rc.left = m_pBackSur->Xpos + 15;
// 	rc.right = m_pBackSur->Xpos + 246;
// 	rc.top = m_pBackSur->Ypos + 70;
// 	rc.bottom = m_pBackSur->Ypos + 214;
//
// 	POINT pt = {x,y};
//
// 	return PtInRect(&rc, pt);
}

int CControlParty::IsMemberNameInside(int x, int y) // ��� ����Ʈ �������� ����̸� üũ��. // ��� �ε����� +1 �� ������ �����Կ� ����.
{
	if( !m_pBackSur )
		return 0;

	int cx = 0, cy = 0, cz = 0;

	cx = m_pBackSur->Xpos + PARTY_LIST_START_X;

	// ���° �������� ���õǾ����� �� ����Ѵ�.
	for( int i = 0 ; i < m_ViewMemberNum ; i++ ) // 0���� �����Կ� ����.
	{
		cz = i + m_MemberScroll.GetPos(); // ��� ���̿�.
		cy = m_MandateBtn[i].PosY; // ��ư�� y��ǥ�� ����ؼ� ���.

		if( m_aPartyList[cz].m_pSurName )
		{
			if(   x > cx && x < cx+m_pNameBox->GetWidth() && y > cy && y < cy+m_pNameBox->GetHeight()  )
			{
				return cz+1; // +1�� ���� ����!
			}
		}
	}

	return 0; // ����Ʈ ������ �´µ� �ȿ� �������� ���� �ƴҰ��.( �ٸ� �޼��� ���ø� ���� �ʿ��ϴ� )
}


#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

LRESULT CControlParty::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;
	int i, z;
	char strTemp[100];

	if( m_MemberScroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK)
		return 1;

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		///Ʃ�丮�� ����ó��
		if( pCMyApp->m_pUIMgr->IsEventScenePlaying() == 5 )
		{
			return 0;
		}

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// ���콺�� CControlScore �г� ���� ���� ���� ������
		// �޽����� ó������ �ʴ´�.
		if (!IsInside(x, y))
			return 0;

		if( m_ExpGetRule.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_ItemGetRule.MsgProc(hWnd, msg, wParam, lParam); // ���⼭ �̰� ���ִ� ������ �� �޺��ڽ��� �켱����(���� ���϶����� �ٸ� �޺��ڽ��� ���õɼ������Ƿ�.)�����̴�. ���� ��� select�� �ٸ� ���� ������ select��尡 ��ҵǾ� ������..

			// 1�� ���ϵǾ����� �ٸ� ���ν��� ����. (�޺��ڽ��� ����Ʈ�� �ٸ� ��Ʈ�ѵ��� ���� �������������Ƿ� �� ó���� �ʿ��ϴ�.)
			return 1;
		}

		if( m_ItemGetRule.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_ExpGetRule.MsgProc(hWnd, msg, wParam, lParam); // ���� ���������� �ٸ� �޺��ڽ��� �޽��� ���ν����� üũ������Ѵ�.

			// 1�� ���ϵǾ����� �ٸ� ���ν��� ����.(�޺��ڽ��� ����Ʈ�� �ٸ� ��Ʈ�ѵ��� ���� �������������Ƿ� �� ó���� �ʿ��ϴ�.)
			return 1;
		}

		if( g_pRoh && m_ViewMemberState.MsgProc(hWnd, msg, wParam, lParam) )  // ��Ƽ���� ����.
			g_pRoh->m_bShowPartysVital = m_ViewMemberState.GetToggleState(); // �̰� ���ϱ��� ���� ����ȭ�� ó��...

		m_RequestRejection.MsgProc(hWnd, msg, wParam, lParam); // �ʴ�ź�.
		m_AskjoinBtn.MsgProc(hWnd, msg, wParam, lParam);

		switch (m_PartyState)
		{
		case PARTY_STATE_NONE:
//			m_AskjoinBtn.MsgProc(hWnd, msg, wParam, lParam); // ���� ��û ��ư
			break;
		case PARTY_STATE_REQUEST:
//			m_AskoutBtn.MsgProc(hWnd, msg, wParam, lParam); // ���� ��� ��ư
			break;
		case PARTY_STATE_MEMBER:
			m_AskoutBtn.MsgProc(hWnd, msg, wParam, lParam); // Ż�� ��ư

			if( IsMemberListInside(x,y) ) // ��� ����Ʈ ����.(����̸�, ��Ƽ�� ���� ��ư��)
			{
				if( msg == WM_LBUTTONDOWN ) // �Ӹ� ������ Ŭ���� ó��
				{
					if( i = IsMemberNameInside(x,y) ) // ��Ƽ�� �̸� Ŭ������ ��.
					{
						// i-1 ��° ��Ƽ���� �̸��� �Ӹ��� ����.
						if( g_pNk2DFrame
								&& g_pNk2DFrame->GetControlBottom()
								&& m_aPartyList[i-1].m_Name
								&& m_aPartyList[i-1].m_Index != g_pRoh->m_nCharIndex ) // �ڱ� �ڽ��� �ƴ� ���� ó��
						{
							sprintf( strTemp,"/%s ", m_aPartyList[i-1].m_Name );
							g_pNk2DFrame->GetControlBottom()->SetFocus();
							g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
							g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText( strTemp );
						}
					}
				}

				if( m_bIsMaster ) // ��Ƽ�� ���� ��ư.
				{
					// �ػ� ��ư�� ��ũ���� 0 ��ġ�϶��� ���δ�.
					if( m_MemberScroll.GetPos() == 0 ) // ��ũ���� 0�̸�...
					{
						m_DisbandBtn.MsgProc(hWnd, msg, wParam, lParam); // �ػ� ��ư üũ.

						if( m_DisbandBtn.GetState() == BTN_ACTION )
						{
							sprintf( commOutBuf, "party disband\n" );
							if( g_pTcpIp )
								g_pTcpIp->SendNetMessage( commOutBuf );

							return 1;
						}
					}

					// �� ��ư���� ���� üũ.
					for( i = 0 ; i < m_ViewMemberNum ; i++ ) // ����Ʈ�� ��ġ�ϴ� ��ư���� ��ġ�� �����ش�.(����� ��ư�� �ƴ϶� ��ġ�� ��ư�ӿ� ����)
					{
						z = m_MemberScroll.GetPos() + i; // ��Ƽ��� �迭������ ��ġ.

						if( m_aPartyList[z].m_pSurName ) // ��Ƽ����� �������� üũ.
						{
							m_MandateBtn[i].MsgProc(hWnd, msg, wParam, lParam);

							if( m_MandateBtn[i].GetState() == BTN_ACTION )
							{
								sprintf( commOutBuf, "party entrust %d\n", m_aPartyList[z].m_Index ); // ����.
								if( g_pTcpIp )
									g_pTcpIp->SendNetMessage( commOutBuf );

								return 1;
							}

							m_AbsOutBtn[i].MsgProc(hWnd, msg, wParam, lParam);

							if( m_AbsOutBtn[i].GetState() == BTN_ACTION )
							{
								sprintf( commOutBuf, "party dismiss %d\n", m_aPartyList[z].m_Index );
								if( g_pTcpIp )
									g_pTcpIp->SendNetMessage( commOutBuf );
								return 1;
							}
						}
					}
				}
			}

			break;
		}

		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);

		if( g_pNk2DFrame )
		{
			if( m_CloseBtn.GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->ShowPartyWindow(FALSE);
				m_CloseBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if( m_AskjoinBtn.GetState() == BTN_ACTION )
			{
				//-- IDS_PARTY : ���� �̸��� �����ּ���
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PARTY), TYPE_EXCHANGE, 17 );
				pCMyApp->m_pUIMgr->SetModal(TRUE);

				m_AskjoinBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if( m_AskoutBtn.GetState() == BTN_ACTION ) // Ż�� ��û.
			{
				if( g_pTcpIp )
				{
					g_pTcpIp->SendNetMessage("party secession\n");
					// �������� ������ ���� ó������
					//g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
					//SetParty( FALSE ); // �̰� Ȥ�� �����ʿ��� ó���� �ȵǾ��� ��� ���� ������ִ�.
				}

				m_AskoutBtn.m_iBtnFlg = BTN_NORMAL;
			}
		}

		return 1;

	case WM_RBUTTONDOWN:
		break;
	}
	return 0;
}

void CControlParty::AddMember(int char_idx, char *name)
{

	int PartyMax = MAX_PARTY_NUM;

	if( pCMyApp->GetCurWorldID() == 18) // WORLD_BOSSRAID_FINALBOSS
		PartyMax = MAX_PARTY_NUM_BOSSRAID_FINAL;
	else if( pCMyApp->IsCurWorldID_BossraidZone() )
		PartyMax = MAX_PARTY_NUM_BOSSRAID;

	if( char_idx < 0 || strlen(name) <= 0 || m_MemberCount >= PartyMax  )
		return;

	if( m_MemberCount == 0 )
	{
		// ó�� ����Ʈ�� �ڱ� �ڽ��̸�...
		if( char_idx == g_pRoh->m_nCharIndex )
			m_bIsMaster = TRUE;
		else
			m_bIsMaster = FALSE;
	}

	m_aPartyList[m_MemberCount].m_Index = char_idx;

	CNkCharacter *pCha = NULL;
	if( pCha = pCMyApp->m_pUIMgr->FindChaById(char_idx, FALSE) ) // ��ó�� ������ �� ��Ƽ ����.
		pCha->SetMyParty(TRUE);

	SAFE_DELETE( m_aPartyList[m_MemberCount].m_Name );

	m_aPartyList[m_MemberCount].m_Name = new char[strlen(name)+1];
	m_aPartyList[m_MemberCount].m_Name[strlen(name)] = NULL;
	strcpy( m_aPartyList[m_MemberCount].m_Name, name );

	m_aPartyList[m_MemberCount].m_Vital = 100; // ���Ƿ� �ʱ�ȭ.
	m_aPartyList[m_MemberCount].m_MaxVital = 100;

	SAFE_DELETE( m_aPartyList[m_MemberCount].m_pSurName );

	g_pDisplay->CreateSurfaceFromText( &m_aPartyList[m_MemberCount].m_pSurName
									   , m_Font, m_aPartyList[m_MemberCount].m_Name, RGB(0, 0, 0), RGB(255,255,0) );
	if( m_aPartyList[m_MemberCount].m_pSurName )
		m_aPartyList[m_MemberCount].m_pSurName->SetColorKey(RGB(0, 0, 0));

	m_MemberCount++;
}

void CControlParty::SetPartyCount()
{
	// [2008/2/28 Theodoric] ��Ƽ��
	if( m_MemberCount > 0 )

	{
		char str[128] = {0,};
		int totalcount = MAX_PARTY_NUM;

		if( pCMyApp->GetCurWorldID() == 18 ) // WORLD_BOSSRAID_FINALBOSS
			totalcount = MAX_PARTY_NUM_BOSSRAID_FINAL;
		else if( pCMyApp->IsCurWorldID_BossraidZone() )
			totalcount = MAX_PARTY_NUM_BOSSRAID;

		sprintf( str, G_STRING(IDS_PARTY_COUNT), m_MemberCount, totalcount);

		SAFE_DELETE(m_pPartyCountSur);
		//m_pPartyCountSur->Clear();
		g_pDisplay->CreateSurfaceFromText( &m_pPartyCountSur, m_Font, str, RGB(0, 0, 0), RGB(255,255,255) );
		m_pPartyCountSur->SetColorKey(RGB(0, 0, 0));

		//  [9/14/2009 ppmmjj83] ������� ���̰� �� �� ������ ������
		if( g_dwClientCountry == CTRY_BRA )
			m_pPartyCountSur->Xpos = m_pBackSur->Xpos+40;
		else
			m_pPartyCountSur->Xpos = m_pBackSur->Xpos+80;

		m_pPartyCountSur->Ypos = m_pBackSur->Ypos + 75 - 20;
	}
}



void CControlParty::RemoveMember(int char_idx)
{}



void CControlParty::RemoveMemberAll()
{
	CNkCharacter *pCha = NULL;

	for (int i = 0; i < MAX_PARTY_NUM_BOSSRAID_FINAL; i++)
	{
		if( m_aPartyList[i].m_Index >= 0 )
		{
			if( pCha = pCMyApp->m_pUIMgr->FindChaById(m_aPartyList[i].m_Index, FALSE))
			{
				pCha->SetMyParty(FALSE);
			}
		}

		m_aPartyList[i].Init();
	}

	m_MemberCount = 0;
}

BOOL CControlParty::IsMyParty(int char_idx)
{
	for( int i = 0; i < m_MemberCount ; i++ )
	{
		if( m_aPartyList[i].m_pSurName ) // ��Ƽ ����� �ִ� �༮�� üũ.
		{
			if( m_aPartyList[i].m_Index >= 0 && m_aPartyList[i].m_Index == char_idx)
				return TRUE;
		}
	}

	return FALSE;
}

//-- Global Font �� �ϳ��� �޾ƿ´�.
//-- 2004.03.15 - �����Ϸ�
//-- Lyul
//-- ��ü �������� Global ������� ����
void CControlParty::SetFont (int f_w, int f_h, char *fontname)
{
	m_Font = G_GetFont(f_h,0,fontname);
}

void CControlParty::ShowMyPartysPos(int idx, int xpos, int ypos)
{}

char* CControlParty::GetMemberName( int char_idx )
{
	for( int i = 0 ; i < m_MemberCount ; i++ )
	{
		if( m_aPartyList[i].m_pSurName ) // ��Ƽ ����� �ִ� �༮�� üũ.
		{
			if( m_aPartyList[i].m_Index >= 0 && m_aPartyList[i].m_Index == char_idx)
				return m_aPartyList[i].m_Name;
		}
	}

	return NULL;
}

void CControlParty::AskParty( int RequestMode, char *subscriber_name, int leader_index, char *leader_name, int ExpGetMode, int ItemGetMode ) ///��Ƽ����
{
	m_RequestMode = RequestMode; // ���� ���� ��û�ߴ����� ����صд�.(consist ���� ��Ŷ�� ������ �ٽ� ���������ֱ� ���ؼ�.)

	char strTemp[256] = "";
	char strTemp4[256] = "";
	char strTemp2[32] = ""; // ��尪 ��Ʈ�� ������.
	char strTemp3[32] = ""; // ��尪 ��Ʈ�� ������.

	switch( ExpGetMode )
	{
	case 1:
		//-- IDS_PARTY_EXP_RULE_DAMAGE : ������
		sprintf( strTemp2, "%s", G_STRING(IDS_PARTY_EXP_RULE_DAMAGE) );
		break;
	case 2:
		//-- IDS_PARTY_EXP_RULE_EQUAL : �յ�
		sprintf( strTemp2, "%s", G_STRING(IDS_PARTY_EXP_RULE_EQUAL) );
		break;
	default:
		sprintf( strTemp2, "%s", "????" );
		break;
	}

	switch( ItemGetMode )
	{
	case 1:
		//-- IDS_PARTY_ITEM_RULE_DAMAGE : ������ �켱
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_DAMAGE) );
		break;
	case 2:
		//-- IDS_PARTY_ITEM_RULE_FIRSTGET : �Լ� �켱
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_FIRSTGET) );
		break;
	case 3:
		//-- IDS_PARTY_ITEM_RULE_RANDOM : ������
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_RANDOM) );
		break;
	case 4:
		//-- IDS_PARTY_ITEM_RULE_LEADER : ��Ƽ�� �켱
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_LEADER) );
		break;
	case 5:
		//-- IDS_PARTY_ITEM_RULE_TURN : ����
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_TURN) );
		break;
	default:
		sprintf( strTemp3, "%s", "????" );
		break;
	}


	switch( RequestMode ) // ������Ʈ ��忡 ���� �ٸ��� �޼����� ���.
	{
	case 0: // ���Ҽ��� ���Ҽ�2���� ��Ƽ�� ��û�Ͽ�����.(���Ҽ�2���� ���̴� �޼���)
	case 1: // ��Ƽ���� ���Ҽӿ��� ��Ƽ�� ��û�Ͽ�����.(���Ҽӿ��� ���̴� �޼���)
		sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PARTY), subscriber_name); //-- IDS_NK2D_REQUIRE_PARTY : %s ���� ��Ƽ��û�� �߽��ϴ�
		sprintf( strTemp4, " [Exp:%s/Item:%s]", strTemp2, strTemp3 ); // ���� ��Ƽ�� �������� ��츸 ����� �����ش�.
		strcat( strTemp, strTemp4 );
		break;
	case 2: // ��Ƽ���� ���Ҽӿ��� ��Ƽ�� ��û�Ͽ�����.(���Ҽӿ��� ���̴� �޼���)
		sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PARTY), subscriber_name); //-- IDS_NK2D_REQUIRE_PARTY : %s ���� ��Ƽ��û�� �߽��ϴ�
		sprintf( strTemp4, " [Exp:%s/Item:%s]", strTemp2, strTemp3 ); // ���� ��Ƽ�� �������� ��츸 ����� �����ش�.
		strcat( strTemp, strTemp4 );
		break;
	case 3: // ���Ҽ��� ��Ƽ������ ��Ƽ�� ��û�Ͽ�����.(��Ƽ�忡�� ���̴� �޼���) - ���� �Ⱦ���.(���߿� �ٸ� ��尡 ����� �޼��� �ٲ�����)
		//-- IDS_PARTY_ASK_MESSAGE1 : ��Ƽ�� %s���� %s�����κ��� ��Ƽ��û�� �޾ҽ��ϴ�.
		sprintf(strTemp, G_STRING(IDS_PARTY_ASK_MESSAGE1), leader_name, subscriber_name);
		break;
	case 4: // ���Ҽ��� ��Ƽ�忡�� ��Ƽ�� ��û�Ͽ�����.(��Ƽ�忡�� ���̴� �޼���)
		sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PARTY), subscriber_name); //-- IDS_NK2D_REQUIRE_PARTY : %s ���� ��Ƽ��û�� �߽��ϴ�
		break;
	case 5: // ��Ƽ���� ���Ҽӿ��� ��Ƽ�� ��û������.(��Ƽ�忡�� ���̴� �޼���)
		//-- IDS_PARTY_ASK_MESSAGE2 : ��Ƽ�� %s���� %s���� ��Ƽ������ �����߽��ϴ�.
		sprintf(strTemp, G_STRING(IDS_PARTY_ASK_MESSAGE2), leader_name, subscriber_name);
		break;
	case 6: // ���Ҽ��� ��Ƽ������ ��Ƽ�� ��û�Ͽ�����.(��Ƽ�忡�� ���̴� �޼���) - ����� 3���� ����
		//-- IDS_PARTY_ASK_MESSAGE1 : ��Ƽ�� %s���� %s�����κ��� ��Ƽ��û�� �޾ҽ��ϴ�.
		sprintf(strTemp, G_STRING(IDS_PARTY_ASK_MESSAGE1), leader_name, subscriber_name);
		break;
	default:
		sprintf(strTemp, "party request mode error!" );
		break;
	}

	g_pNk2DFrame->InsertPopup(strTemp,TYPE_NOR_OKCANCLE, 11);
}

LRESULT CControlParty::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char commOutBuf[256] = "";
	if( nMsg == 1) // Ȯ�ι�ư�� �������
		// ��Ƽ ���� ��û�� �³��ϴ� �޽����� ������.
		sprintf(commOutBuf, "party consist %d %d\n", m_RequestMode, g_pNk2DFrame->m_PartyReadyIndex); ///��Ƽ����.
	else if(nMsg == -1) // ��ҹ�ư�� �������
	{
		// ��Ƽ ���� ��û�� �����ϴ� �޽����� ������. ///��Ƽ����.
		if( m_bIsMaster ) // ��Ƽ�常 dismiss
			sprintf( commOutBuf, "party dismiss %d 0\n", g_pNk2DFrame->m_PartyReadyIndex ); // �ڵ� ��Ұ� �ƴϸ� �ڿ� 0
		else
			sprintf( commOutBuf, "party secession %d 0\n", g_pNk2DFrame->m_PartyReadyIndex ); // �ڵ� ��Ұ� �ƴϸ� �ڿ� 0
	}

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(commOutBuf);

	return 0L;
}

LRESULT CControlParty::ReQuestPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "Ȯ��" ��ư�� �������
	if(nMsg == 1)
	{
		char strTemp[128] = "";
		strcpy(strTemp, szMoney);
		int len = strlen(strTemp);
		int count = 0;

		for (int idx = 0; idx < len; idx++)
		{
			if (!is_space(strTemp[idx]))
				count++;
		}

		if (g_pNk2DFrame && count > 2)
		{
			int ExpRule = m_ExpGetRule.GetSelect();
			int ItemRule = m_ItemGetRule.GetSelect();

			if( ExpRule == 0 ) //  0�϶� �⺻���������� ����.
				ExpRule = 1;

			if( ItemRule == 0 ) //  0�϶� �⺻���������� ����.
				ItemRule = 1;

			sprintf(commOutBuf, "party request %s %d %d\n", strTemp, ExpRule, ItemRule ); ///��Ƽ����.

			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(commOutBuf);
		}
	}

	if( pCMyApp->m_pUIMgr )
		pCMyApp->m_pUIMgr->SetModal(FALSE);

	return 0L;
}
