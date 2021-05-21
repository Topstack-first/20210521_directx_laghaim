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

	m_pPartyCountSur = NULL; 	// [2008/2/28 Theodoric] 파티수 표시

	for( int i = 0 ; i < MAX_PARTY_NUM_BOSSRAID_FINAL ; i++ )
	{
		m_aPartyList[i].m_Name = NULL;
		m_aPartyList[i].m_pSurName = NULL;
	}
	Init();

	g_bUseVoiceChat = FALSE;
	m_bShowPartyVital = FALSE;
}

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
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
	m_RequestMode = -1; // -1로 초기화.

	for( int i = 0 ; i < MAX_PARTY_NUM_BOSSRAID_FINAL ; i++ )
	{
		m_aPartyList[i].m_Index = -1;
		m_aPartyList[i].m_Vital = 0;
		m_aPartyList[i].m_MaxVital = 0;
		SAFE_DELETE( m_aPartyList[i].m_Name );
		SAFE_DELETE( m_aPartyList[i].m_pSurName );
	}

	m_ViewMemberState.SetFileName( "party/btn_viewstate" );  // 파티상태 보기.
	m_RequestRejection.SetFileName( "party/btn_rejection" ); // 초대거부.

	m_AskjoinBtn.SetFileName("party/btn_askjoin");		// 가입신청.
	m_AskoutBtn.SetFileName("party/btn_askout");		// 탈퇴신청

	m_CloseBtn.SetFileName("common/btn_close_01");
	m_CloseBtn.SetDisable(false);

	m_DisbandBtn.SetFileName("party/btn_allout");		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)
	m_DumyMandateBtn.SetFileName("party/btn_mandate");		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)
	m_DumyAbsOutBtn.SetFileName("party/btn_out");		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)

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

	m_ExpGetRule.SetItemSize( 151, 16, 7, 4 ); // 경험치 분배 방식.
	m_ExpGetRule.LoadRes("party/btn_dropdown", "interface/party/comboback.bmp", "interface/party/ListBack1.bmp");

	m_ExpGetRule.DeleteAllItem(); // 이걸 꼭해줘야 리스토어시 등에 누적으로 안들어간다.
	//-- IDS_PARTY_EXP_RULE : 경험치 룰
	m_ExpGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_EXP_RULE), 13, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_EXP_RULE_DAMAGE : 데미지
	m_ExpGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_EXP_RULE_DAMAGE), 13, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_EXP_RULE_EQUAL : 균등
	m_ExpGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_EXP_RULE_EQUAL), 13, RGB(255, 210, 0), RGB(255,255,255) );

	m_ItemGetRule.SetItemSize( 151, 16, 7, 4 ); // 아이템 분배 방식.
	m_ItemGetRule.LoadRes("party/btn_dropdown", "interface/party/comboback.bmp", "interface/party/ListBack2.bmp");

	m_ItemGetRule.DeleteAllItem(); // 이걸 꼭해줘야 리스토어시 등에 누적으로 안들어간다.
	//-- IDS_PARTY_ITEM_RULE : 아이템 룰
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE), 12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_DAMAGE : 데미지 우선
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_DAMAGE), 12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_FIRSTGET : 입수 우선
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_FIRSTGET), 12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_RANDOM : 무작위
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_RANDOM),12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_LEADER : 파티장 우선
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_LEADER),12, RGB(255, 210, 0), RGB(255,255,255) );
	//-- IDS_PARTY_ITEM_RULE_TURN : 순차
	m_ItemGetRule.InsertItem( (char*)G_STRING(IDS_PARTY_ITEM_RULE_TURN),12, RGB(255, 210, 0), RGB(255,255,255) );

	m_ViewMemberState.LoadRes();  // 파티상태 보기.
	m_RequestRejection.LoadRes(); // 초대거부.
	m_AskjoinBtn.LoadRes();
	m_AskoutBtn.LoadRes();
	m_CloseBtn.LoadRes();
	m_DisbandBtn.LoadRes();		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)
	m_DumyMandateBtn.LoadRes();		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)
	m_DumyAbsOutBtn.LoadRes();		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)


	GetClientRect(g_pDisplay->GetHWnd(), &rcClient);



	m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth();
	m_pBackSur->Ypos = 0;

	m_ViewMemberNum = MEMBER_NUM;
	m_MemberScroll.LoadRes( "common/scroll_up", "common/scroll_down", "interface/common/scroll_bar.bmp" ); // 해상도 별로 바 크기를 다르게 로딩.
	m_MemberScroll.SetRange( MAX_PARTY_NUM + 1 - m_ViewMemberNum ); // +1 해줘야 맞더라.

	// scroll size = 9
	m_MemberScroll.SetDrawPos( m_pBackSur->Xpos+250, m_pBackSur->Ypos+71, 212 ); // 현재는 전체보기에서만 지원하니 전체보기만 좌표를 세로 셋팅해준다.

	m_ExpGetRule.SetPosition( m_pBackSur->Xpos+44, m_pBackSur->Ypos+310 ); // 경험치 분배 방식.
	m_ItemGetRule.SetPosition( m_pBackSur->Xpos+44, m_pBackSur->Ypos+342); // 아이템 분배 방식.

	m_ViewMemberState.SetPosition( m_pBackSur->Xpos+36, m_pBackSur->Ypos+382 );
	m_RequestRejection.SetPosition( m_pBackSur->Xpos+103, m_pBackSur->Ypos+382 );
	m_AskjoinBtn.SetPosition( m_pBackSur->Xpos+170, m_pBackSur->Ypos+382 );

	m_AskoutBtn.SetPosition( m_pBackSur->Xpos+113, m_pBackSur->Ypos+451 );

	m_CloseBtn.SetPosition(m_pBackSur->Xpos+257, m_pBackSur->Ypos+6);

	// 멤버 리스트 내에 위치하는 버튼들 위치지정.
	x = m_pBackSur->Xpos + PARTY_LIST_START_X;
	y = m_pBackSur->Ypos + PARTY_LIST_START_Y;

	m_DisbandBtn.SetPosition( x+m_pNameBox->GetWidth()+PARTY_LIST_LINE_X_GAB-4, y - 10 -10 );

	for( i = 0 ; i < MEMBER_NUM ; ++i ) // 리스트에 위치하는 버튼들의 위치를 정해준다.(멤버당 버튼이 아니라 위치당 버튼임에 유의)
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

	SAFE_DELETE(m_pPartyCountSur);	// [2008/2/28 Theodoric] 파티수 표시
	SAFE_DELETE(m_pBackSur);
	SAFE_DELETE(m_pNameBox);
	SAFE_DELETE(m_pMasterMark);

	m_MemberScroll.DeleteRes();
	m_ExpGetRule.DeleteRes();
	m_ItemGetRule.DeleteRes();
	m_ViewMemberState.DeleteRes();  // 파티상태 보기.
	m_RequestRejection.DeleteRes(); // 초대거부.
	m_AskjoinBtn.DeleteRes();
	m_AskoutBtn.DeleteRes();
	m_CloseBtn.DeleteRes();
	m_DisbandBtn.DeleteRes();

	for( i = 0 ; i < MEMBER_NUM ; i++ ) // 더미 버튼을 해제하기전에 더미 버튼의 이미지 참조들을 풀어야한다.(안그러면 이 버튼들 소멸자에서 뻑날지도?)
	{
		m_MandateBtn[i].DeleteRes();
		m_AbsOutBtn[i].DeleteRes();
	}

	m_DumyMandateBtn.DeleteRes();		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)
	m_DumyAbsOutBtn.DeleteRes();		// 더미 버튼.(메시지받기나 드로우를 하지 않고 이미지를 로드해서 보관하는 용도로 쓰인다.)
}

HRESULT CControlParty::RestoreSurfaces()
{
	for( int i = 0 ; i < MAX_PARTY_NUM_BOSSRAID_FINAL; i++ )
	{
		if( m_aPartyList[i].m_Name ) // 이름이 있는 녀석만 복구.
		{
			SAFE_DELETE( m_aPartyList[i].m_pSurName );
			g_pDisplay->CreateSurfaceFromText( &m_aPartyList[i].m_pSurName
											   , m_Font, m_aPartyList[i].m_Name, RGB(0, 0, 0), RGB(255,255,0) );
			if( m_aPartyList[i].m_pSurName )
				m_aPartyList[i].m_pSurName->SetColorKey(RGB(0, 0, 0));
		}
	}

	// 나머지는 무식하게 다 지웠다가 로딩.
	DeleteRes();
	LoadRes();

	return S_OK;
}

void CControlParty::Draw()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur);


	// [2008/2/28 Theodoric] 파티수
	if( m_pPartyCountSur && m_MemberCount > 0)
	{
		g_pDisplay->Blt(m_pPartyCountSur->Xpos, m_pPartyCountSur->Ypos, m_pPartyCountSur);
	}

	m_MemberScroll.Draw();
	DrawMembers();
	m_ViewMemberState.Draw();  // 파티상태 보기.
	m_RequestRejection.Draw(); // 초대거부.
	m_AskjoinBtn.Draw();
	m_CloseBtn.Draw();

	if( m_ExpGetRule.GetState() > 0 ) // 리스트 전개중이면 아래꺼 먼저그린다.
	{
		m_ItemGetRule.Draw();
		m_ExpGetRule.Draw(); // 이녀석들을 뒤에 출력해주는게 좋다.(위에 출력되도록)
	}
	else
	{
		m_ExpGetRule.Draw(); // 이녀석들을 뒤에 출력해주는게 좋다.(위에 출력되도록)
		m_ItemGetRule.Draw();
	}

	switch (m_PartyState)
	{
	case PARTY_STATE_NONE:
		break;
	case PARTY_STATE_REQUEST:
		m_AskoutBtn.Draw(); // 가입 취소 버튼
		break;
	case PARTY_STATE_MEMBER:
		m_AskoutBtn.Draw(); // 가입 취소 버튼
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
		if( m_aPartyList[i].m_pSurName ) // 해당 위치의 멤버가 있을때만 출력
		{
			z = i - m_MemberScroll.GetPos(); // 출력 높이용.
			y = m_MandateBtn[z].PosY+ 5; // 버튼의 y좌표를 사용해서 계산.

			if( i == 0 ) // 제일 처음이 파장.
			{
				// 파장 마크 출력
				g_pDisplay->Blt( x-m_pMasterMark->GetWidth()-10, y-2, m_pMasterMark ); // 이름 배경.

				if( m_bIsMaster ) // 자신이 파티장이면 해산 버튼도 출력
					m_DisbandBtn.Draw(); // 미리 지정된 위치에 출력.
			}

			g_pDisplay->Blt( x, y, m_pNameBox ); // 이름 배경.
			g_pDisplay->Blt( x+2, y+2, m_aPartyList[i].m_pSurName ); // 실제 이름 출력.


			if( m_bIsMaster && i != 0 ) // 파티장이면 버튼들도 출력해준다.(자기 자신 제외)
			{
				// 위임, 강퇴 버튼 출력.
				m_MandateBtn[z].Draw(); // 미리 지정된 위치에 출력.
				m_AbsOutBtn[z].Draw(); // 미리 지정된 위치에 출력.
			}
		}
	}
}



BOOL CControlParty::IsInside(int x, int y)
{
	// 아이텀 박스의 영역또한 마우스체크를 체크했다..
	if( !m_pBackSur )
		return false;

	return m_pBackSur->IsIn(x, y);
}

BOOL CControlParty::IsMemberListInside(int x, int y) // 멤버 리스트 영역(멤버이름, 파티장 전용 버튼들) 체크용.
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

int CControlParty::IsMemberNameInside(int x, int y) // 멤버 리스트 영역안의 멤버이름 체크용. // 멤버 인덱스를 +1 된 값으로 리턴함에 유의.
{
	if( !m_pBackSur )
		return 0;

	int cx = 0, cy = 0, cz = 0;

	cx = m_pBackSur->Xpos + PARTY_LIST_START_X;

	// 몇번째 아이템이 선택되었는지 를 계산한다.
	for( int i = 0 ; i < m_ViewMemberNum ; i++ ) // 0번은 제외함에 유의.
	{
		cz = i + m_MemberScroll.GetPos(); // 출력 높이용.
		cy = m_MandateBtn[i].PosY; // 버튼의 y좌표를 사용해서 계산.

		if( m_aPartyList[cz].m_pSurName )
		{
			if(   x > cx && x < cx+m_pNameBox->GetWidth() && y > cy && y < cy+m_pNameBox->GetHeight()  )
			{
				return cz+1; // +1된 값을 리턴!
			}
		}
	}

	return 0; // 리스트 영역은 맞는데 안에 아이템의 안은 아닐경우.( 다른 메세지 무시를 위해 필요하다 )
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

		///튜토리얼 예외처리
		if( pCMyApp->m_pUIMgr->IsEventScenePlaying() == 5 )
		{
			return 0;
		}

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 마우스가 CControlScore 패널 위에 놓여 있지 않으면
		// 메시지를 처리하지 않는다.
		if (!IsInside(x, y))
			return 0;

		if( m_ExpGetRule.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_ItemGetRule.MsgProc(hWnd, msg, wParam, lParam); // 여기서 이걸 해주는 이유는 두 콤보박스에 우선순위(밑의 리턴때문에 다른 콤보박스가 무시될수있으므로.)때문이다. 예를 들어 select중 다른 데를 찍으면 select모드가 취소되야 함으로..

			// 1이 리턴되었으면 다른 프로시져 무시. (콤보박스는 리스트가 다른 컨트롤들의 위에 놓여질수있으므로 이 처리가 필요하다.)
			return 1;
		}

		if( m_ItemGetRule.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_ExpGetRule.MsgProc(hWnd, msg, wParam, lParam); // 위와 마찬가지로 다른 콤보박스의 메시지 프로시져도 체크해줘야한다.

			// 1이 리턴되었으면 다른 프로시져 무시.(콤보박스는 리스트가 다른 컨트롤들의 위에 놓여질수있으므로 이 처리가 필요하다.)
			return 1;
		}

		if( g_pRoh && m_ViewMemberState.MsgProc(hWnd, msg, wParam, lParam) )  // 파티상태 보기.
			g_pRoh->m_bShowPartysVital = m_ViewMemberState.GetToggleState(); // 이건 리턴까진 말고 동기화만 처리...

		m_RequestRejection.MsgProc(hWnd, msg, wParam, lParam); // 초대거부.
		m_AskjoinBtn.MsgProc(hWnd, msg, wParam, lParam);

		switch (m_PartyState)
		{
		case PARTY_STATE_NONE:
//			m_AskjoinBtn.MsgProc(hWnd, msg, wParam, lParam); // 가입 신청 버튼
			break;
		case PARTY_STATE_REQUEST:
//			m_AskoutBtn.MsgProc(hWnd, msg, wParam, lParam); // 가입 취소 버튼
			break;
		case PARTY_STATE_MEMBER:
			m_AskoutBtn.MsgProc(hWnd, msg, wParam, lParam); // 탈퇴 버튼

			if( IsMemberListInside(x,y) ) // 멤버 리스트 영역.(멤버이름, 파티장 전용 버튼들)
			{
				if( msg == WM_LBUTTONDOWN ) // 귓말 셋팅은 클릭만 처리
				{
					if( i = IsMemberNameInside(x,y) ) // 파티원 이름 클릭했을 시.
					{
						// i-1 번째 파티원의 이름을 귓말로 셋팅.
						if( g_pNk2DFrame
								&& g_pNk2DFrame->GetControlBottom()
								&& m_aPartyList[i-1].m_Name
								&& m_aPartyList[i-1].m_Index != g_pRoh->m_nCharIndex ) // 자기 자신이 아닐 때만 처리
						{
							sprintf( strTemp,"/%s ", m_aPartyList[i-1].m_Name );
							g_pNk2DFrame->GetControlBottom()->SetFocus();
							g_pNk2DFrame->GetControlBottom()->m_InputBox.ClearText();
							g_pNk2DFrame->GetControlBottom()->m_InputBox.SetText( strTemp );
						}
					}
				}

				if( m_bIsMaster ) // 파티장 전용 버튼.
				{
					// 해산 버튼은 스크롤이 0 위치일때만 보인다.
					if( m_MemberScroll.GetPos() == 0 ) // 스크롤이 0이면...
					{
						m_DisbandBtn.MsgProc(hWnd, msg, wParam, lParam); // 해산 버튼 체크.

						if( m_DisbandBtn.GetState() == BTN_ACTION )
						{
							sprintf( commOutBuf, "party disband\n" );
							if( g_pTcpIp )
								g_pTcpIp->SendNetMessage( commOutBuf );

							return 1;
						}
					}

					// 각 버튼마다 영역 체크.
					for( i = 0 ; i < m_ViewMemberNum ; i++ ) // 리스트에 위치하는 버튼들의 위치를 정해준다.(멤버당 버튼이 아니라 위치당 버튼임에 유의)
					{
						z = m_MemberScroll.GetPos() + i; // 파티멤버 배열에서의 위치.

						if( m_aPartyList[z].m_pSurName ) // 파티멤버가 있을때만 체크.
						{
							m_MandateBtn[i].MsgProc(hWnd, msg, wParam, lParam);

							if( m_MandateBtn[i].GetState() == BTN_ACTION )
							{
								sprintf( commOutBuf, "party entrust %d\n", m_aPartyList[z].m_Index ); // 위임.
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
				//-- IDS_PARTY : 상대방 이름을 적어주세요
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PARTY), TYPE_EXCHANGE, 17 );
				pCMyApp->m_pUIMgr->SetModal(TRUE);

				m_AskjoinBtn.m_iBtnFlg = BTN_NORMAL;
			}
			else if( m_AskoutBtn.GetState() == BTN_ACTION ) // 탈퇴 신청.
			{
				if( g_pTcpIp )
				{
					g_pTcpIp->SendNetMessage("party secession\n");
					// 서버에서 응답이 오면 처리하자
					//g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
					//SetParty( FALSE ); // 이거 혹시 서버쪽에선 처리가 안되었을 경우 문제 생길수있다.
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
		// 처음 리스트가 자기 자신이면...
		if( char_idx == g_pRoh->m_nCharIndex )
			m_bIsMaster = TRUE;
		else
			m_bIsMaster = FALSE;
	}

	m_aPartyList[m_MemberCount].m_Index = char_idx;

	CNkCharacter *pCha = NULL;
	if( pCha = pCMyApp->m_pUIMgr->FindChaById(char_idx, FALSE) ) // 근처에 있으면 셋 파티 설정.
		pCha->SetMyParty(TRUE);

	SAFE_DELETE( m_aPartyList[m_MemberCount].m_Name );

	m_aPartyList[m_MemberCount].m_Name = new char[strlen(name)+1];
	m_aPartyList[m_MemberCount].m_Name[strlen(name)] = NULL;
	strcpy( m_aPartyList[m_MemberCount].m_Name, name );

	m_aPartyList[m_MemberCount].m_Vital = 100; // 임의로 초기화.
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
	// [2008/2/28 Theodoric] 파티수
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

		//  [9/14/2009 ppmmjj83] 브라질은 길이가 길어서 좀 앞으로 땡겨줌
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
		if( m_aPartyList[i].m_pSurName ) // 파티 멤버가 있는 녀석만 체크.
		{
			if( m_aPartyList[i].m_Index >= 0 && m_aPartyList[i].m_Index == char_idx)
				return TRUE;
		}
	}

	return FALSE;
}

//-- Global Font 중 하나를 받아온다.
//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경
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
		if( m_aPartyList[i].m_pSurName ) // 파티 멤버가 있는 녀석만 체크.
		{
			if( m_aPartyList[i].m_Index >= 0 && m_aPartyList[i].m_Index == char_idx)
				return m_aPartyList[i].m_Name;
		}
	}

	return NULL;
}

void CControlParty::AskParty( int RequestMode, char *subscriber_name, int leader_index, char *leader_name, int ExpGetMode, int ItemGetMode ) ///파티개편
{
	m_RequestMode = RequestMode; // 무슨 모드로 신청했는지를 기억해둔다.(consist 응낙 패킷에 서버로 다시 돌려보내주기 위해서.)

	char strTemp[256] = "";
	char strTemp4[256] = "";
	char strTemp2[32] = ""; // 모드값 스트링 결정용.
	char strTemp3[32] = ""; // 모드값 스트링 결정용.

	switch( ExpGetMode )
	{
	case 1:
		//-- IDS_PARTY_EXP_RULE_DAMAGE : 데미지
		sprintf( strTemp2, "%s", G_STRING(IDS_PARTY_EXP_RULE_DAMAGE) );
		break;
	case 2:
		//-- IDS_PARTY_EXP_RULE_EQUAL : 균등
		sprintf( strTemp2, "%s", G_STRING(IDS_PARTY_EXP_RULE_EQUAL) );
		break;
	default:
		sprintf( strTemp2, "%s", "????" );
		break;
	}

	switch( ItemGetMode )
	{
	case 1:
		//-- IDS_PARTY_ITEM_RULE_DAMAGE : 데미지 우선
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_DAMAGE) );
		break;
	case 2:
		//-- IDS_PARTY_ITEM_RULE_FIRSTGET : 입수 우선
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_FIRSTGET) );
		break;
	case 3:
		//-- IDS_PARTY_ITEM_RULE_RANDOM : 무작위
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_RANDOM) );
		break;
	case 4:
		//-- IDS_PARTY_ITEM_RULE_LEADER : 파티장 우선
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_LEADER) );
		break;
	case 5:
		//-- IDS_PARTY_ITEM_RULE_TURN : 순차
		sprintf( strTemp3, "%s", G_STRING(IDS_PARTY_ITEM_RULE_TURN) );
		break;
	default:
		sprintf( strTemp3, "%s", "????" );
		break;
	}


	switch( RequestMode ) // 리퀘스트 모드에 따라 다르게 메세지를 출력.
	{
	case 0: // 무소속이 무소속2에게 파티를 요청하였을때.(무소속2에게 보이는 메세지)
	case 1: // 파티장이 무소속에게 파티를 요청하였을때.(무소속에게 보이는 메세지)
		sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PARTY), subscriber_name); //-- IDS_NK2D_REQUIRE_PARTY : %s 님이 파티요청을 했습니다
		sprintf( strTemp4, " [Exp:%s/Item:%s]", strTemp2, strTemp3 ); // 새로 파티에 권유받은 경우만 방식을 보여준다.
		strcat( strTemp, strTemp4 );
		break;
	case 2: // 파티원이 무소속에게 파티를 요청하였을때.(무소속에게 보이는 메세지)
		sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PARTY), subscriber_name); //-- IDS_NK2D_REQUIRE_PARTY : %s 님이 파티요청을 했습니다
		sprintf( strTemp4, " [Exp:%s/Item:%s]", strTemp2, strTemp3 ); // 새로 파티에 권유받은 경우만 방식을 보여준다.
		strcat( strTemp, strTemp4 );
		break;
	case 3: // 무소속이 파티원에게 파티를 요청하였을때.(파티장에게 보이는 메세지) - 현재 안쓰임.(나중에 다른 모드가 생기면 메세지 바꿔주자)
		//-- IDS_PARTY_ASK_MESSAGE1 : 파티원 %s님이 %s님으로부터 파티요청을 받았습니다.
		sprintf(strTemp, G_STRING(IDS_PARTY_ASK_MESSAGE1), leader_name, subscriber_name);
		break;
	case 4: // 무소속이 파티장에게 파티를 요청하였을때.(파티장에게 보이는 메세지)
		sprintf(strTemp, G_STRING(IDS_NK2D_REQUIRE_PARTY), subscriber_name); //-- IDS_NK2D_REQUIRE_PARTY : %s 님이 파티요청을 했습니다
		break;
	case 5: // 파티원이 무소속에게 파티를 요청했을때.(파티장에게 보이는 메세지)
		//-- IDS_PARTY_ASK_MESSAGE2 : 파티원 %s님이 %s님을 파티참가를 권유했습니다.
		sprintf(strTemp, G_STRING(IDS_PARTY_ASK_MESSAGE2), leader_name, subscriber_name);
		break;
	case 6: // 무소속이 파티원에게 파티를 요청하였을때.(파티장에게 보이는 메세지) - 현재는 3번과 동일
		//-- IDS_PARTY_ASK_MESSAGE1 : 파티원 %s님이 %s님으로부터 파티요청을 받았습니다.
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
	if( nMsg == 1) // 확인버튼이 눌린경우
		// 파티 가입 요청을 승낙하는 메시지를 보낸다.
		sprintf(commOutBuf, "party consist %d %d\n", m_RequestMode, g_pNk2DFrame->m_PartyReadyIndex); ///파티개편.
	else if(nMsg == -1) // 취소버튼이 눌린경우
	{
		// 파티 가입 요청을 거절하는 메시지를 보낸다. ///파티개편.
		if( m_bIsMaster ) // 파티장만 dismiss
			sprintf( commOutBuf, "party dismiss %d 0\n", g_pNk2DFrame->m_PartyReadyIndex ); // 자동 취소가 아니면 뒤에 0
		else
			sprintf( commOutBuf, "party secession %d 0\n", g_pNk2DFrame->m_PartyReadyIndex ); // 자동 취소가 아니면 뒤에 0
	}

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(commOutBuf);

	return 0L;
}

LRESULT CControlParty::ReQuestPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney)
{
	// "확인" 버튼이 눌린경우
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

			if( ExpRule == 0 ) //  0일땐 기본설정값으로 셋팅.
				ExpRule = 1;

			if( ItemRule == 0 ) //  0일땐 기본설정값으로 셋팅.
				ItemRule = 1;

			sprintf(commOutBuf, "party request %s %d %d\n", strTemp, ExpRule, ItemRule ); ///파티개편.

			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(commOutBuf);
		}
	}

	if( pCMyApp->m_pUIMgr )
		pCMyApp->m_pUIMgr->SetModal(FALSE);

	return 0L;
}
