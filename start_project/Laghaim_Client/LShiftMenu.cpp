#include "stdafx.h"


#include "headers.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "ComboBox.h"
#include "LShiftMenu.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "tcpipcon.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "AlphaPopUp.h"
#include "g_stringmanager.h"
#include "ControlBottom.h"
#include "Country.h"






// [5/6/2008 반재승] 브라질 펫 매크로 UI 예외 처리를 위해 국가 언어 구분 객체 추가
extern DWORD				g_dwClientCountry;

// nate : 라비용 인벤 좌표 버그 수정
#define MYPET_FOOD_SLOT_X		88

#define MYPET_FOOD_SLOT_Y		77
#define MYPET_FOOD_SLOT_WIDTH	160
#define MYPET_FOOD_SLOT_HEIGHT	25
#define MYPET_FOOD_SLOT_GAP		27

// [5/6/2008 반재승] 버튼 좌표 차이값 디파인으로 정의
#define MYPET_X_GAP0			11
#define MYPET_X_GAP1			24
#define MYPET_X_GAP2			63
#define MYPET_X_GAP3			102
#define MYPET_X_GAP4			141
#define MYPET_X_GAP5			251
#define MYPET_X_GAP6			180
#define MYPET_X_GAP7			219
#define MYPET_X_GAP8			258
#define MYPET_X_GAP9			297

#define MYPET_Y_GAP0			83
#define MYPET_Y_GAP1			46
#define MYPET_Y_GAP2			79

// [5/6/2008 반재승] 브라질 리소스 차이로 인한 X좌표 차이
#define X_GAP_BRA				70

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLShiftMenu::CLShiftMenu()
{
	memset(m_UserName, 0, sizeof(m_UserName));

	m_bMacroEditing = FALSE;
	m_pControlMacro = NULL;

	m_pSurfaceBack = NULL;
	m_pApple = NULL;
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;
	m_pBtn3 = NULL;
	m_pBtn4 = NULL;
	m_pBtn5 = NULL;
	m_pBtn6 = NULL; // 생산 추가 모드 1
	m_pBtn7 = NULL; // PK 원거리 추가 모드 2
	m_pBtn8 = NULL;	// 경계모드(진화 라비용만 사용가능)
	m_pBtn9 = NULL;	// 매크로 모드(진화 라비용만 사용가능)
	m_pNameText = NULL;
	m_pMenuInfo = NULL;
	m_pMenuInfo = new CAlphaPopUp;
	ZeroMemory(&m_StrTemp, sizeof(m_StrTemp));
	/// 추호에 lib에서..
	ZeroMemory(m_StrTempSupp,sizeof(m_StrTempSupp));
	ZeroMemory(m_StrTempPk,sizeof(m_StrTempPk));
	Init();
}

CLShiftMenu::~CLShiftMenu()
{
	DeleteRes();
	SAFE_DELETE(m_pMenuInfo);
}

void CLShiftMenu::Init()
{
	if( m_pMenuInfo )
		m_pMenuInfo->Init(14);
}

void CLShiftMenu::LoadRes()
{
	if( !g_pDisplay )
		return;

	//-- IDS_SUPPORT_MODE : 지원 모드
	sprintf(m_StrTempSupp,G_STRING(IDS_SUPPORT_MODE));
	//-- IDS_LONG_RANGE_MODE : 원거리계열 해제모드
	sprintf(m_StrTempPk,G_STRING(IDS_LONG_RANGE_MODE));

	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pApple);
	SAFE_DELETE(m_pBtn1);
	SAFE_DELETE(m_pBtn2);
	SAFE_DELETE(m_pBtn3);
	SAFE_DELETE(m_pBtn4);
	SAFE_DELETE(m_pBtn5);
	SAFE_DELETE(m_pBtn6); // 생산 추가모드 1
	SAFE_DELETE(m_pBtn7); // PK 원거리 추가모드 2
	SAFE_DELETE(m_pBtn8);
	SAFE_DELETE(m_pBtn9);
	SAFE_DELETE(m_pNameText);

	m_pBtn1 = new CBasicButton();
	m_pBtn2 = new CBasicButton();
	m_pBtn3 = new CBasicButton();
	m_pBtn4 = new CBasicButton();
	m_pBtn5 = new CBasicButton();
	m_pBtn6 = new CBasicButton(); // 생산 추가모드 1
	m_pBtn7 = new CBasicButton(); // PK 원거리 추가모드 2
	m_pBtn8 = new CBasicButton();
	m_pBtn9 = new CBasicButton();

	m_pControlMacro = new CControlPetMacro();

	if( m_pControlMacro )
	{
		m_pControlMacro->LoadRes();
		m_pControlMacro->SetPos( 100, 100 );
		m_pControlMacro->LoadMacro(); // 저장해둔 매크로를 불러온다.
	}

	if( m_pBtn1 )
		m_pBtn1->SetFileName("lshiftmenu/btn_attack");// 전투 아이콘 추가
	if( m_pBtn2 )
		m_pBtn2->SetFileName("lshiftmenu/btn_follow");// 동행
	if( m_pBtn3 )
		m_pBtn3->SetFileName("lshiftmenu/btn_eat");// 먹자
	if( m_pBtn4 )
		m_pBtn4->SetFileName("lshiftmenu/btn_avoid");// 회피
	if( m_pBtn5 )
		m_pBtn5->SetFileName("common/btn_close_01");// 닫기
	if( m_pBtn6 )
		m_pBtn6->SetFileName("lshiftmenu/btn_supp");// 생산
	if( m_pBtn7 )
		m_pBtn7->SetFileName("lshiftmenu/btn_rang");// 원거리 해제
	if( m_pBtn8 )
		m_pBtn8->SetFileName("lshiftmenu/btn_guard");// 경계
	if( m_pBtn9 )
		m_pBtn9->SetFileName("lshiftmenu/btn_macro");// 매크로

	if( m_pBtn1 )
		m_pBtn1->LoadRes();
	if( m_pBtn2 )
		m_pBtn2->LoadRes();
	if( m_pBtn3 )
		m_pBtn3->LoadRes();
	if( m_pBtn4 )
		m_pBtn4->LoadRes();
	if( m_pBtn5 )
		m_pBtn5->LoadRes();
	if( m_pBtn6 )
		m_pBtn6->LoadRes();// 생산 추가모드 1
	if( m_pBtn7 )
		m_pBtn7->LoadRes();// PK 원거리 추가모드 2
	if( m_pBtn8 )
		m_pBtn8->LoadRes();
	if( m_pBtn9 )
		m_pBtn9->LoadRes();

	m_pNameText = new CTextOutBox();
	if( m_pNameText )
		m_pNameText->Init(12, RGB(255, 210, 0),0, 0);

	if( m_pMenuInfo )
		m_pMenuInfo->LoadRes(g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/lshiftmenu/pet_macro.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pApple, "interface/lshiftmenu/pet_apple01.bmp");

	m_pSurfaceBack->SetColorKey( TRANS_COLOR );

	CloseMenu();
}

void CLShiftMenu::DeleteRes()
{
	SAFE_DELETE( m_pControlMacro );

	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pApple);
	SAFE_DELETE(m_pBtn1);
	SAFE_DELETE(m_pBtn2);
	SAFE_DELETE(m_pBtn3);
	SAFE_DELETE(m_pBtn4);
	SAFE_DELETE(m_pBtn5);
	SAFE_DELETE(m_pBtn6); // 생산 추가모드 1
	SAFE_DELETE(m_pBtn7); // PK 원거리 추가모드 2
	SAFE_DELETE(m_pBtn8);
	SAFE_DELETE(m_pBtn9);
	SAFE_DELETE(m_pNameText);
}

int CLShiftMenu::GetWidth()
{
	if (m_pSurfaceBack)
		return m_pSurfaceBack->GetWidth();

	return 0;
}

int CLShiftMenu::GetHeight()
{
	if (m_pSurfaceBack)
		return m_pSurfaceBack->GetHeight();

	return 0;
}

void CLShiftMenu::SetXPos(int x)
{
	if (m_pSurfaceBack)
		m_pSurfaceBack->Xpos = x;
}

void CLShiftMenu::SetYPos(int y)
{
	if (m_pSurfaceBack)
		m_pSurfaceBack->Ypos = y;
}

void CLShiftMenu::SetPos(int x, int y)
{
	SetXPos(x);
	SetYPos(y);
	if( m_pBtn1 )
		m_pBtn1->SetPosition(x+30, y+63); // 전투
	if( m_pBtn2 )
		m_pBtn2->SetPosition(x+69, y+63); // 동행
	if( m_pBtn3 )
		m_pBtn3->SetPosition(x+108, y+63); // 먹자
	if( m_pBtn4 )
		m_pBtn4->SetPosition(x+147, y+63); // 회피
	if( m_pBtn5 )
		m_pBtn5->SetPosition(x+352, y+5); // 닫기211,79
	if( m_pBtn6 )
		m_pBtn6->SetPosition(x+186, y+63); // 지원 모드 1
	if( m_pBtn7 )
		m_pBtn7->SetPosition(x+225, y+63); // PK 원거리 추가모드 2
	if( m_pBtn8 )
		m_pBtn8->SetPosition(x+264, y+63); // 경계
	if( m_pBtn9 )
		m_pBtn9->SetPosition(x+303, y+63); // 매크로
}

void CLShiftMenu::Draw()
{
	if( !m_pSurfaceBack || !g_pDisplay )
		return;

	if( m_bMacroEditing && m_pControlMacro ) // 매크로 모드 편집중엔 매크로 모드 창만 처리.
	{
		m_pControlMacro->Draw();

		return;
	}

	// 혹시 화면밖으로 나가는 경우의 좌표 보정
	BOOL PosChange = FALSE;

	if( m_pSurfaceBack->Xpos < 0 )
	{
		m_pSurfaceBack->Xpos = 0;
		PosChange = TRUE;
	}
	if( m_pSurfaceBack->Ypos < 0 )
	{
		m_pSurfaceBack->Ypos = 0;
		PosChange = TRUE;
	}
	if( m_pSurfaceBack->Xpos > g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth() )
	{
		m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth();
		PosChange = TRUE;
	}
	if( m_pSurfaceBack->Ypos > g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
	{
		m_pSurfaceBack->Ypos = g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
		PosChange = TRUE;
	}

	if( PosChange ) // 좌표가 바뀌었다면..
		SetPos( m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos ); // 버튼들 좌표도 다시 보정.

	int x, y;
	x = m_pSurfaceBack->Xpos; //전체화면에서 back의 x좌표
	y = m_pSurfaceBack->Ypos; //전체화면에서 back의 y좌표
	g_pDisplay->Blt(x, y, m_pSurfaceBack);
///이름띄우기.여기에 애완동물 이름 받아서 찍음.
	if( m_pNameText )
		m_pNameText->Draw(m_pSurfaceBack->Xpos+98 , m_pSurfaceBack->Ypos + 43, 170);
	if( m_pBtn1 )
		m_pBtn1->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_COMBAT ); // 전투
	if( m_pBtn2 )
		m_pBtn2->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_FOLLOW ); // 동행
	if( m_pBtn3 )
		m_pBtn3->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_PICKUP ); // 먹자
	if( m_pBtn4 )
		m_pBtn4->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_WIMPY ); // 회피
	if( m_pBtn5 )
		m_pBtn5->DrawToBlt(); // 닫기
	if( m_pBtn6 )
		m_pBtn6->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_SUPP ); // 생산 관련 추가모드 1
	if( m_pBtn7 )
		m_pBtn7->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_RANGECOMBAT ); //  PK 관련 원거리 추가모드 2
	if( m_pBtn8 )
		m_pBtn8->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_GUARD /*|| !g_MyPetStatus.m_Class*/ ); // 경계 // 기본 라비용은 쓸수없다.
	if( m_pBtn9 )
		m_pBtn9->DrawToBlt( g_MyPetStatus.m_bUseMacroMode /*|| !g_MyPetStatus.m_Class*/ ); // 매크로 // 기본 라비용은 쓸수없다.


	for (int i = 0; i < g_MyPetStatus.food_count; i++)
		g_pDisplay->Blt(m_pSurfaceBack->Xpos+101+(28*i), m_pSurfaceBack->Ypos+94, m_pApple);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);
	CMultiText multiText;

	if( m_pMenuInfo )
	{
		// 애완동물 모드 도움말
		if( m_pBtn1 && m_pBtn1->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -100)
			{
				//-- IDS_LSHIFT_MOOD_0 : 전투모드
				sprintf(m_StrTemp, G_STRING(IDS_LSHIFT_MOOD_0));
				multiText.AddString(m_StrTemp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-100);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn2 && m_pBtn2->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -101)
			{
				//-- IDS_LSHIFT_MOOD_1 : 동행모드
				sprintf(m_StrTemp, G_STRING(IDS_LSHIFT_MOOD_1));
				multiText.AddString(m_StrTemp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-101);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn3 && m_pBtn3->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -102)
			{
				//-- IDS_LSHIFT_MOOD_2 : 획득모드
				sprintf(m_StrTemp, G_STRING(IDS_LSHIFT_MOOD_2));
				multiText.AddString(m_StrTemp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-102);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn4 && m_pBtn4->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -103)
			{
				//-- IDS_LSHIFT_MOOD_3 : 회피모드
				sprintf(m_StrTemp, G_STRING(IDS_LSHIFT_MOOD_3));
				multiText.AddString(m_StrTemp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-103);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn6 && m_pBtn6->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -104)
			{
				multiText.AddString(m_StrTempSupp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-104);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn7 && m_pBtn7->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -105)
			{
				multiText.AddString(m_StrTempPk, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-105);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn8 && m_pBtn8->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -106)
			{
				//-- IDS_LSHIFT_MODE_6 : 경계모드
				sprintf(m_StrTemp, G_STRING(IDS_LSHIFT_MODE_6));
				multiText.AddString(m_StrTemp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-106);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}
		else if( m_pBtn9 && m_pBtn9->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -107)
			{
				//-- IDS_LSHIFT_MODE_7 : 매크로모드
				sprintf(m_StrTemp, G_STRING(IDS_LSHIFT_MODE_7));
				multiText.AddString(m_StrTemp, RGB(255, 255, 255));
				m_pMenuInfo->PutString(&multiText);
				m_pMenuInfo->SetIndex(-107);
			}
			m_pMenuInfo->Draw(point.x, point.y);
		}

	}
}

BOOL CLShiftMenu::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn( x, y );
}


BOOL CLShiftMenu::IsFoodSlot(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	if (x >= m_pSurfaceBack->Xpos + 101 &&
			x < m_pSurfaceBack->Xpos + 101 + MYPET_FOOD_SLOT_WIDTH &&
			y >= m_pSurfaceBack->Ypos + 94 &&
			y < m_pSurfaceBack->Ypos + 94 + MYPET_FOOD_SLOT_HEIGHT)
	{
		return TRUE;
	}
	return FALSE;
}


LRESULT CLShiftMenu::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmp[200];
	int x, y;
	static BOOL bLShiftMenu_Drag = FALSE;
	static int DistX = 0, DistY = 0; // 드래그시 클릭한 곳과 마우스 좌표의 차이 수치.

	if( m_bMacroEditing && m_pControlMacro ) // 매크로 모드 편집중엔 매크로 모드 창만 처리.
	{
		int Result;
		Result = m_pControlMacro->MsgProc( hWnd, msg, wParam, lParam );
		if( Result == 2 ) // 리턴값이 2이면 실행을 누른거다.
		{
			m_pControlMacro->SaveMacro(); // 매크로를 저장한다.
			m_pControlMacro->StartMacroMode();

			m_pControlMacro->m_bNowInput = FALSE; // 이거 안풀리는 버그 있었음. 풀어주자.

			m_pBtn9->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if( Result == 1 ) // 그냥 뭔가 눌린 경우. 다른 메세지 무시하기 위해 필요.
			return 1;
		else
			return 0; // 아무것도 안눌려도 아래께 실행되면 안된다.
	}

	switch (msg)
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))
			return 0;
		else if( !m_pBtn1->IsInside(x,y) && !m_pBtn2->IsInside(x,y)
				 && !m_pBtn3->IsInside(x,y) && !m_pBtn4->IsInside(x,y)
				 && !m_pBtn5->IsInside(x,y) && !m_pBtn6->IsInside(x,y)
				 && !m_pBtn7->IsInside(x,y)) // 영역안에서 다운이면...
		{
			bLShiftMenu_Drag = TRUE;
			DistX = m_pSurfaceBack->Xpos - x;
			DistY = m_pSurfaceBack->Ypos - y;
		}
		else if( !m_pBtn8->IsInside(x,y) && !m_pBtn9->IsInside(x,y) ) // 임시로 여기로 뺀다. 나중엔 위에 함치자.
		{
			bLShiftMenu_Drag = TRUE;
			DistX = m_pSurfaceBack->Xpos - x;
			DistY = m_pSurfaceBack->Ypos - y;
		}

		if (IsFoodSlot(x, y))
		{
			if (g_pRoh->m_ExtraSlot && g_pRoh->m_ExtraSlot->m_Vnum == 618)
			{
				if (g_MyPetStatus.IncFoodCount())
				{
					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage("p_bag put\n");

					SAFE_DELETE(g_pRoh->m_ExtraSlot);
				}
			}
			return 1;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn2 && m_pBtn2->GetState() != BTN_DISABLE)
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn3 && m_pBtn3->GetState() != BTN_DISABLE)
			m_pBtn3->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn4 && m_pBtn4->GetState() != BTN_DISABLE)
			m_pBtn4->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn5 && m_pBtn5->GetState() != BTN_DISABLE)
			m_pBtn5->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn6 && m_pBtn6->GetState() != BTN_DISABLE)// 생산
			m_pBtn6->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn7 && m_pBtn7->GetState() != BTN_DISABLE)// PK
			m_pBtn7->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn8 && m_pBtn8->GetState() != BTN_DISABLE)
			m_pBtn8->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn9 && m_pBtn9->GetState() != BTN_DISABLE)
			m_pBtn9->MsgProc(hWnd, msg, wParam, lParam);

		return 1;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		bLShiftMenu_Drag = FALSE;

		if(m_pBtn1)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn2)
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn3)
			m_pBtn3->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn4)
			m_pBtn4->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn5)
			m_pBtn5->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn6)
			m_pBtn6->MsgProc(hWnd, msg, wParam, lParam);// 생산
		if(m_pBtn7)
			m_pBtn7->MsgProc(hWnd, msg, wParam, lParam); // PK
		if(m_pBtn8 && m_pBtn9->GetState() != BTN_DISABLE)
			m_pBtn8->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn9 && m_pBtn9->GetState() != BTN_DISABLE)
			m_pBtn9->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn5 && m_pBtn5->GetState() == BTN_ACTION) 			//종료
		{
			m_pBtn5->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}

		if( g_pNk2DFrame )
		{
			if(m_pBtn1 && m_pBtn1->GetState() == BTN_ACTION)
			{
				//전투 메시지 넣음
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_COMBAT);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);

				m_pBtn1->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_COMBAT);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)

				return 1;
			}
			else if(m_pBtn2 && m_pBtn2->GetState() == BTN_ACTION)
			{
				//동행 메시지 넣음
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_FOLLOW);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn2->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_FOLLOW);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)

				return 1;
			}
			else if(m_pBtn3 && m_pBtn3->GetState() == BTN_ACTION)
			{
				//먹자 메시지 넣음
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_PICKUP);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn3->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_PICKUP);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)

				return 1;
			}
			else if(m_pBtn4 && m_pBtn4->GetState() == BTN_ACTION)
			{
				//회피 메시지 넣음
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_WIMPY);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn4->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_WIMPY);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)

				return 1;
			}
			else if(m_pBtn6 && m_pBtn6->GetState() == BTN_ACTION)
			{
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_SUPP);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn6->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_SUPP);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)

				return 1;
			}
			else if(m_pBtn7 && m_pBtn7->GetState() == BTN_ACTION)
			{
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_RANGECOMBAT );
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn7->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_RANGECOMBAT );

				g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)

				return 1;
			}
			else if(m_pBtn8 && m_pBtn8->GetState() == BTN_ACTION ) // 경계모드
			{
				if( g_MyPetStatus.m_Class ) // 기본 라비용은 쓸수없다.
				{
					sprintf(tmp, "p_mode %d\n", MYPET_MODE_GUARD);
					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage(tmp);
					m_pBtn8->SetState(BTN_NORMAL);
					CloseMenu();
					if (pCMyApp->m_pMyPet)
						g_MyPetStatus.SetMode(MYPET_MODE_GUARD);

					g_MyPetStatus.m_bUseMacroMode = FALSE; // 다른 모드로 변경될 때 매크로 모드를 끈다.(만약 매크로가 변경한 거라면 이 이후 다시 TRUE를 설정해줘야한다.)
				}
				else
				{
					//-- IDS_ITCAN_EVOLPET : 진화된 라비용만 가능합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITCAN_EVOLPET), TYPE_NOR_OK, 1 );
				}


				return 1;
			}
			else if(m_pBtn9 && m_pBtn9->GetState() == BTN_ACTION ) // 매크로모드
			{
				if( g_MyPetStatus.m_Class ) // 기본 라비용은 쓸수없다.
				{
					m_bMacroEditing = TRUE; // 매크로 모드 편집 모드로 바꾼다.
				}
				else
				{
					//-- IDS_ITCAN_EVOLPET : 진화된 라비용만 가능합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITCAN_EVOLPET), TYPE_NOR_OK, 1 );
				}
			}
		}

		break;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bLShiftMenu_Drag ) // 드래그
		{
			m_pSurfaceBack->Xpos = x + DistX;
			m_pSurfaceBack->Ypos = y + DistY;



			BOOL PosChange = FALSE;

			if( m_pSurfaceBack->Xpos < 0 )
			{
				m_pSurfaceBack->Xpos = 0;
				PosChange = TRUE;
			}
			if( m_pSurfaceBack->Ypos < 0 )
			{
				m_pSurfaceBack->Ypos = 0;
				PosChange = TRUE;
			}
			if( m_pSurfaceBack->Xpos > g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth() )
			{
				m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth();
				PosChange = TRUE;
			}
			if( m_pSurfaceBack->Ypos > g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
			{
				m_pSurfaceBack->Ypos = g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();
				PosChange = TRUE;
			}

			if( PosChange ) // 좌표가 바뀌었다면..
				SetPos( m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos ); // 버튼들 좌표도 다시 보정.



// 			if( m_pNameText )
// 				m_pNameText->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+83);

			if( m_pBtn1 )
				m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+30, m_pSurfaceBack->Ypos+63); // 전투
			if( m_pBtn2 )
				m_pBtn2->SetPosition(m_pSurfaceBack->Xpos+69, m_pSurfaceBack->Ypos+63); // 동행
			if( m_pBtn3 )
				m_pBtn3->SetPosition(m_pSurfaceBack->Xpos+108, m_pSurfaceBack->Ypos+63); // 먹자
			if( m_pBtn4 )
				m_pBtn4->SetPosition(m_pSurfaceBack->Xpos+147, m_pSurfaceBack->Ypos+63); // 회피
			if( m_pBtn5 )
				m_pBtn5->SetPosition(m_pSurfaceBack->Xpos+352, m_pSurfaceBack->Ypos+5); // 닫기211,79
			if( m_pBtn6 )
				m_pBtn6->SetPosition(m_pSurfaceBack->Xpos+186, m_pSurfaceBack->Ypos+63); // 지원 모드 1
			if( m_pBtn7 )
				m_pBtn7->SetPosition(m_pSurfaceBack->Xpos+225, m_pSurfaceBack->Ypos+63); // PK 원거리 추가모드 2
			if( m_pBtn8 )
				m_pBtn8->SetPosition(m_pSurfaceBack->Xpos+264, m_pSurfaceBack->Ypos+63); // 경계
			if( m_pBtn9 )
				m_pBtn9->SetPosition(m_pSurfaceBack->Xpos+303, m_pSurfaceBack->Ypos+63); // 매크로
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn2 && m_pBtn2->GetState() != BTN_DISABLE)
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn3 && m_pBtn3->GetState() != BTN_DISABLE)
			m_pBtn3->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn4 && m_pBtn4->GetState() != BTN_DISABLE)
			m_pBtn4->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn5 && m_pBtn5->GetState() != BTN_DISABLE)
			m_pBtn5->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn6 && m_pBtn6->GetState() != BTN_DISABLE)//생산
			m_pBtn6->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn7 && m_pBtn7->GetState() != BTN_DISABLE)//PK
			m_pBtn7->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn8 && m_pBtn4->GetState() != BTN_DISABLE)
			m_pBtn8->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn9 && m_pBtn5->GetState() != BTN_DISABLE)
			m_pBtn9->MsgProc(hWnd, msg, wParam, lParam);

		break;
	}
	return 0;
}

void CLShiftMenu::SetCurRace(int race)  //별 필요없을듯.
{
	switch(race)
	{
	case 0: //
		break;
	case 1: //
		break;
	case 2: //
		break;
	case 3: //
		break;
	}
}

void CLShiftMenu::CloseMenu()
{
	if( g_pNk2DFrame )
		g_pNk2DFrame->SetShowLShiftFlag(false);
	if(m_pBtn1 && m_pBtn2 && m_pBtn3 && m_pBtn4 && m_pBtn5 && m_pBtn6 && m_pBtn7)
	{
		m_pBtn1->SetState(BTN_NORMAL);
		m_pBtn2->SetState(BTN_NORMAL);
		m_pBtn3->SetState(BTN_NORMAL);
		m_pBtn4->SetState(BTN_NORMAL);
		m_pBtn5->SetState(BTN_NORMAL);
		m_pBtn6->SetState(BTN_NORMAL);
		m_pBtn7->SetState(BTN_NORMAL);
	}
	else if( m_pBtn8 && m_pBtn9 ) // 임시로 여기로 뺀다. 나중엔 위에 함치자.
	{
		m_pBtn8->SetState(BTN_NORMAL);
		m_pBtn9->SetState(BTN_NORMAL);
	}
}

void CLShiftMenu::SetUserName(char* name)
{
	if(!m_pNameText)
		return;

	m_pNameText->SetString(name);
	strcpy(m_UserName, name);
}








CControlPetMacro::CControlPetMacro()
	: m_pSurfaceBack(NULL)
{
	Init();
}

CControlPetMacro::~CControlPetMacro()
{
	DeleteRes();
}

void CControlPetMacro::Init()
{
	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		m_bUseCheck[i] = FALSE; // 사용 유무 체크.
		m_ModeComboBox[i].SetState( 0 ); // 비활성을 푼다.
		m_ModeComboBox[i].SetSelect( 0 ); // 디폴트 값으로..
		m_DelayInputText[i].ClearText();     // 상점주인이 써놓은 멘트 출력용 서피스.
	}

	m_bNowInput = FALSE;

	m_NowUse = 0;
	m_dwPrevChangeTick = 0;
}

void CControlPetMacro::LoadRes()
{
	int pos_X =0;
	g_pDisplay->CreateSurfaceFromBitmap( &m_pSurfaceBack, "interface/lshiftmenu/PetMacro/MacroBack.bmp" );

	if( m_pSurfaceBack )
	{
		// [5/6/2008 반재승] 펫 매크로 콤보박스 사이즈 조정 - 브라질
		int size_Width = 0;

		switch( g_dwClientCountry )
		{
		case CTRY_BRA:
			{
				size_Width = 322;
			}
			break;

		default:
			{
				size_Width = 250;
			}
			break;
		}

		m_pSurfaceBack->SetWidth( size_Width ); // 위 CreateSurface에서 CSurface 에 따로 저장해두는 m_dwWidth,m_dwHeight에 값을 저장안해주는 버그가 있어서 그냥 직접 셋팅.
		m_pSurfaceBack->SetHeight( 250 );

		m_pSurfaceBack->SetColorKey(TRANS_COLOR);


		m_RunBtn.SetFileName( "lshiftmenu/PetMacro/btn_run" );
		m_RunBtn.LoadRes();

		m_CheckBtn.SetFileName( "lshiftmenu/PetMacro/check_btn" );
		m_CheckBtn.LoadRes();

		for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			// [5/6/2008 반재승] 펫 매크로 콤보박스 사이즈 조정 - 브라질

			switch( g_dwClientCountry )
			{
			case CTRY_BRA:
				{
					pos_X = 72 + X_GAP_BRA;
				}
				break;

			default:
				{
					pos_X = 72;
				}
				break;

			}

			m_ModeComboBox[i].SetItemSize( pos_X, 16, 7, 4 ); // 경험치 분배 방식.
			m_ModeComboBox[i].LoadRes("lshiftmenu/PetMacro/btn_drowdown_pm", "interface/lshiftmenu/PetMacro/ComboBack.bmp", "interface/lshiftmenu/PetMacro/ListBack.bmp");

			m_ModeComboBox[i].DeleteAllItem(); // 이걸 꼭해줘야 리스토어시 등에 누적으로 안들어간다.
			m_ModeComboBox[i].InsertItem( " ",10, RGB(255, 210, 0), RGB(255,255,255) );
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_0),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_0 : 전투모드
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_1),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_1 : 동행모드
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_2),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_2 : 획득모드
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_3),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_3 : 회피모드
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MODE_4),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MODE_4 : 지원모드
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MODE_5),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MODE_5 : 해제모드
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MODE_6),10, RGB(1255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MODE_6 : 경계모드

			m_DelayInputText[i].Init( 0, 0, 6, RGB(255, 255, 255), RGB(255, 255, 0) );
			m_DelayInputText[i].SetfontSize(); // 디폴트 폰트 사이즈로 초기화.(이거 안하면 다른게 초기화 안되서 안된다.)
		}
	}

	SetPos( 100, 100 );
}
void CControlPetMacro::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);
	m_RunBtn.DeleteRes();
	m_CheckBtn.DeleteRes();

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		m_ModeComboBox[i].DeleteRes();
		m_DelayInputText[i].DeleteRes();     // 상점주인이 써놓은 멘트 출력용 서피스.
	}
}

void CControlPetMacro::SaveMacro() // 간단한 파일 저장
{
	FILE *fp;

	fp = fopen("data/etc/PetMacro.txt", "wt");

	if( !fp )
		return;

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		fprintf( fp, "%d\t%d\t%d\n", m_bUseCheck[i], m_ModeComboBox[i].GetSelect(), atoi(m_DelayInputText[i].GetStrBuf()) );
	}

	fclose( fp );
}
void CControlPetMacro::LoadMacro() // 기억해둔 이전 매크로를 로드.
{
	FILE *fp;

	fp = fopen("data/etc/PetMacro.txt", "rt");

	if( !fp )
		return;

	int Temp1 = 0, Temp2 = 0;
	char strTemp[50];

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		fscanf( fp, "%d\t%d\t%d\n", &m_bUseCheck[i], &Temp1, &Temp2 );
		m_ModeComboBox[i].SetSelect( Temp1 );

		m_DelayInputText[i].ClearText();
		itoa( Temp2, strTemp, 10 );
		m_DelayInputText[i].SetText( strTemp );
	}

	fclose( fp );
}

void CControlPetMacro::StartMacroMode() // 처음 실행할 매크로를 찾아서 패킷 보내주고 매크로 모드 셋팅해주는 녀석.
{
	char tmp[256];

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		if( m_bUseCheck[i] ) // 가장 먼저 체크 되어있는 녀석을 처음에 바로 보낸다.
		{
			sprintf(tmp, "p_mode %d\n", m_ModeComboBox[i].GetSelect()+11 ); // 선택되어있는 모드를 보낸다.
			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);

			if (pCMyApp->m_pMyPet)
				g_MyPetStatus.SetMode( m_ModeComboBox[i].GetSelect()+11 );

			g_MyPetStatus.m_bUseMacroMode = TRUE; // 만약 체크 되어있는 녀석이 없으면 매크로 모드로 전환되지 않는다.

			m_NowUse = i; // 현재 실행하는 매크로 번호를 기억.
			m_dwPrevChangeTick = timeGetTime();

			break; // 루프 종료.
		}
	}
}

int CControlPetMacro::CheckMacroTime() // 매크로 모드 바꿔주기 체크.
{
	char tmp[256];

	int Delay = atoi( m_DelayInputText[m_NowUse].GetStrBuf() );

	if( timeGetTime()-m_dwPrevChangeTick > Delay*1000 ) // 모드를 바꿀 시간이 되었다면...
	{
		for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			++m_NowUse; // 매크로를 한단계 증가시키고.
			if( m_NowUse >= PET_MACRO_MAX )
				m_NowUse = 0; // 루프

			if( m_bUseCheck[m_NowUse] && atoi(m_DelayInputText[m_NowUse].GetStrBuf()) ) // 사용 체크가 되어있는 녀석이라면...(딜레이도 0초면 무시)
			{
				sprintf(tmp, "p_mode %d\n", m_ModeComboBox[m_NowUse].GetSelect()-1+11 ); // 선택되어있는 모드를 보낸다.
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);

				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode( m_ModeComboBox[m_NowUse].GetSelect()-1+11 );

				g_MyPetStatus.m_bUseMacroMode = TRUE; // 만약 체크 되어있는 녀석이 없으면 매크로 모드로 전환되지 않는다.

				m_dwPrevChangeTick = timeGetTime();

				return m_NowUse; // 끊어버린다.
			}
		}
	}

	return -1;
}

void CControlPetMacro::Draw()
{
	if( m_pSurfaceBack )
		g_pDisplay->Blt( m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack );

	m_RunBtn.Draw();

	int SelectedCombo = -1;

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		if( m_bUseCheck[i] )
			m_CheckBtn.SetState(BTN_DOWN);
		else
			m_CheckBtn.SetState(BTN_NORMAL);

		int pos_X = m_DelayInputText[i].m_X-143;

		// [5/6/2008 반재승] 펫 매크로 콤보박스 사이즈 조정 - 브라질
		switch( g_dwClientCountry )
		{
		case CTRY_BRA:
			{
				pos_X = m_DelayInputText[i].m_X-143 - X_GAP_BRA;
			}
			break;

		default:
			{
				pos_X = m_DelayInputText[i].m_X-143;
			}
			break;

		}

		m_CheckBtn.SetPosition( pos_X, m_DelayInputText[i].m_Y+1 );
		m_CheckBtn.Draw();

		if( m_ModeComboBox[i].GetState() > 0 ) // 리스트 전개상태인 녀석이 있으면...
			SelectedCombo = i; // 출력안하고 인덱스만 기억.
		else
			m_ModeComboBox[i].Draw(); // 전개 상태가 아닌 녀석만 출력.

		m_DelayInputText[i].Draw();
	}

	if( SelectedCombo != -1 ) // 전개상태인 콤보박스가 있었다면...
		m_ModeComboBox[SelectedCombo].Draw(); // 전개 상태인 녀석을 마지막에 출력.
}

void CControlPetMacro::SetPos(int x, int y)
{
	if( m_pSurfaceBack )
	{
		m_pSurfaceBack->Xpos = x;
		m_pSurfaceBack->Ypos = y;
	}

	// [5/6/2008 반재승] OK 버튼 위치 조정 - 브라질

	int pos_X = 0;

	switch( g_dwClientCountry )
	{
	case CTRY_BRA:
		{
			pos_X = 150 + X_GAP_BRA;
		}
		break;

	default:
		{
			pos_X = 150;
		}
		break;
	}

	m_RunBtn.SetPosition(m_pSurfaceBack->Xpos+83 , m_pSurfaceBack->Ypos+206); // 매크로

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		m_ModeComboBox[i].SetPosition( m_pSurfaceBack->Xpos+35, m_pSurfaceBack->Ypos+39+(i*33) ); // 경험치 분배 방식.
		int pos_X2= 0;

		switch( g_dwClientCountry )
		{
		case CTRY_BRA:
			{
				pos_X2 = 160 + X_GAP_BRA;
			}
			break;

		default:
			{
				pos_X2 = 160;
			}
			break;
		}
		// [5/6/2008 반재승] 딜레이 입력창 위치 좀더 뒤로 - 브라질
		m_DelayInputText[i].SetPos( m_pSurfaceBack->Xpos+pos_X2, m_pSurfaceBack->Ypos+47+(i*33) );
	}
}

BOOL CControlPetMacro::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;
	return m_pSurfaceBack->IsIn(x, y);
}

int CControlPetMacro::IsInsideContents(int x, int y) // 인터페이스 상에 작동하는 컨텐츠 위치인가 체크.
{
	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		// [5/6/2008 반재승] 브라질 매크로 사이즈 증가 때문에 체크박스 위치도 변경.
		int pos_X = 0;

		switch( g_dwClientCountry )
		{
		case CTRY_BRA:
			{
				pos_X = m_DelayInputText[i].m_X-145 - X_GAP_BRA;
			}
			break;

		default:
			{
				pos_X = m_DelayInputText[i].m_X-145;
			}
			break;
		}

		// 체크박스
		if( x >= pos_X && x < pos_X + m_CheckBtn.Width
				&& y >= m_DelayInputText[i].m_Y-1 && y < m_DelayInputText[i].m_Y-1 + m_CheckBtn.Height )
		{
			return (i*3)+1;
		}

		// 콤보박스
		if( x >= m_ModeComboBox[i].m_DrawPosX && x < m_ModeComboBox[i].m_DrawPosX + m_ModeComboBox[i].m_pSelectedBackGround->GetWidth() + m_ModeComboBox[i].m_pListViewBtn->Width
				&& y >= m_ModeComboBox[i].m_DrawPosY && y < m_ModeComboBox[i].m_DrawPosY + m_ModeComboBox[i].m_pSelectedBackGround->GetHeight() ) // 가로에 버튼 길이 플러스 해줘야한다.
		{
			return (i*3)+2;
		}

		// 딜레이 입력박스
		if( m_DelayInputText[i].IsInside(x,y) )
		{
			return (i*3)+3;
		}
	}


	return 0;
}

LRESULT CControlPetMacro::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i;
	int x, y;
	static BOOL bDrag = FALSE;
	static int DistX = 0, DistY = 0; // 드래그시 클릭한 곳과 마우스 좌표의 차이 수치.

	switch (msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		m_RunBtn.MsgProc(hWnd, msg, wParam, lParam);
		if( m_RunBtn.GetState() == BTN_ACTION )
		{
			m_RunBtn.SetState(BTN_NORMAL); // 상태를 직접 돌려준다.

			return 2; // 실행을 눌렀을때의 리턴값이 틀림에 유의.
		}

		for( i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			if( m_ModeComboBox[i].MsgProc(hWnd, msg, wParam, lParam) )
			{
				// 콤보 박스는 그 리스트 영역이 다른 녀석을 침범할수 있기에 자신이 눌리면 리턴시킨다.(다른 콤보 박스들에는 닫기 처리가 필요할듯.)

				return 1;
			}

			m_DelayInputText[i].MsgProc(hWnd, msg, wParam, lParam);
		}


		i = IsInsideContents( x, y );

		// 드래그의 처리는 위쪽의 다른 리턴들이 없을때 처리한다.
		if( msg == WM_LBUTTONDOWN )
		{
			if(!IsInside(x, y))
				return 0;
			else if( !m_RunBtn.IsInside(x,y) && i == 0 ) // 영역안에서 다운이면...// i가 0이 아니면 드래그 하면 안된다.(i에는 다른거 눌린 정보가 들어가있는다.)
			{
				bDrag = TRUE;
				DistX = m_pSurfaceBack->Xpos - x;
				DistY = m_pSurfaceBack->Ypos - y;
			}

			return 1; // 영역 안에서... 마우스 눌렀을 경우
		}
		else if( msg == WM_LBUTTONUP )
		{
			bDrag = FALSE;

			if( i != 0 ) // 무언가 눌렸으면...
			{
				if( i % 3 == 1 ) // 체크 버튼이 눌린 경우.
				{
					m_bUseCheck[i/3] ^= 1; // 토글 시킨다.
				}
			}

			return 1; // 영역 안에서... 마우스 눌렀을 경우
		}
		else if( msg == WM_MOUSEMOVE )
		{
			if( bDrag ) // 드래그
			{
				m_pSurfaceBack->Xpos = x + DistX;
				m_pSurfaceBack->Ypos = y + DistY;

				if( m_pSurfaceBack->Xpos < 0 )
					m_pSurfaceBack->Xpos = 0;
				if( m_pSurfaceBack->Ypos < 0 )
					m_pSurfaceBack->Ypos = 0;
				if( m_pSurfaceBack->Xpos > g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth() )
					m_pSurfaceBack->Xpos = g_pNk2DFrame->GetClientWidth() - m_pSurfaceBack->GetWidth();
				if( m_pSurfaceBack->Ypos > g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
					m_pSurfaceBack->Ypos = g_pNk2DFrame->GetClientHeight() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();

				SetPos( m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos );
			}
		}

		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:

		m_bNowInput = FALSE;

		for( i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			if( m_DelayInputText[i].GetEnable() )
			{
				m_DelayInputText[i].MsgProc( hWnd, msg, wParam, lParam );
				m_bNowInput = TRUE;
				return 1;
			}
		}

		break;
	}

	return 0;
}

