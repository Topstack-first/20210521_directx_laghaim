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






// [5/6/2008 �����] ����� �� ��ũ�� UI ���� ó���� ���� ���� ��� ���� ��ü �߰�
extern DWORD				g_dwClientCountry;

// nate : ���� �κ� ��ǥ ���� ����
#define MYPET_FOOD_SLOT_X		88

#define MYPET_FOOD_SLOT_Y		77
#define MYPET_FOOD_SLOT_WIDTH	160
#define MYPET_FOOD_SLOT_HEIGHT	25
#define MYPET_FOOD_SLOT_GAP		27

// [5/6/2008 �����] ��ư ��ǥ ���̰� ���������� ����
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

// [5/6/2008 �����] ����� ���ҽ� ���̷� ���� X��ǥ ����
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
	m_pBtn6 = NULL; // ���� �߰� ��� 1
	m_pBtn7 = NULL; // PK ���Ÿ� �߰� ��� 2
	m_pBtn8 = NULL;	// �����(��ȭ ���븸 ��밡��)
	m_pBtn9 = NULL;	// ��ũ�� ���(��ȭ ���븸 ��밡��)
	m_pNameText = NULL;
	m_pMenuInfo = NULL;
	m_pMenuInfo = new CAlphaPopUp;
	ZeroMemory(&m_StrTemp, sizeof(m_StrTemp));
	/// ��ȣ�� lib����..
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

	//-- IDS_SUPPORT_MODE : ���� ���
	sprintf(m_StrTempSupp,G_STRING(IDS_SUPPORT_MODE));
	//-- IDS_LONG_RANGE_MODE : ���Ÿ��迭 �������
	sprintf(m_StrTempPk,G_STRING(IDS_LONG_RANGE_MODE));

	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pApple);
	SAFE_DELETE(m_pBtn1);
	SAFE_DELETE(m_pBtn2);
	SAFE_DELETE(m_pBtn3);
	SAFE_DELETE(m_pBtn4);
	SAFE_DELETE(m_pBtn5);
	SAFE_DELETE(m_pBtn6); // ���� �߰���� 1
	SAFE_DELETE(m_pBtn7); // PK ���Ÿ� �߰���� 2
	SAFE_DELETE(m_pBtn8);
	SAFE_DELETE(m_pBtn9);
	SAFE_DELETE(m_pNameText);

	m_pBtn1 = new CBasicButton();
	m_pBtn2 = new CBasicButton();
	m_pBtn3 = new CBasicButton();
	m_pBtn4 = new CBasicButton();
	m_pBtn5 = new CBasicButton();
	m_pBtn6 = new CBasicButton(); // ���� �߰���� 1
	m_pBtn7 = new CBasicButton(); // PK ���Ÿ� �߰���� 2
	m_pBtn8 = new CBasicButton();
	m_pBtn9 = new CBasicButton();

	m_pControlMacro = new CControlPetMacro();

	if( m_pControlMacro )
	{
		m_pControlMacro->LoadRes();
		m_pControlMacro->SetPos( 100, 100 );
		m_pControlMacro->LoadMacro(); // �����ص� ��ũ�θ� �ҷ��´�.
	}

	if( m_pBtn1 )
		m_pBtn1->SetFileName("lshiftmenu/btn_attack");// ���� ������ �߰�
	if( m_pBtn2 )
		m_pBtn2->SetFileName("lshiftmenu/btn_follow");// ����
	if( m_pBtn3 )
		m_pBtn3->SetFileName("lshiftmenu/btn_eat");// ����
	if( m_pBtn4 )
		m_pBtn4->SetFileName("lshiftmenu/btn_avoid");// ȸ��
	if( m_pBtn5 )
		m_pBtn5->SetFileName("common/btn_close_01");// �ݱ�
	if( m_pBtn6 )
		m_pBtn6->SetFileName("lshiftmenu/btn_supp");// ����
	if( m_pBtn7 )
		m_pBtn7->SetFileName("lshiftmenu/btn_rang");// ���Ÿ� ����
	if( m_pBtn8 )
		m_pBtn8->SetFileName("lshiftmenu/btn_guard");// ���
	if( m_pBtn9 )
		m_pBtn9->SetFileName("lshiftmenu/btn_macro");// ��ũ��

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
		m_pBtn6->LoadRes();// ���� �߰���� 1
	if( m_pBtn7 )
		m_pBtn7->LoadRes();// PK ���Ÿ� �߰���� 2
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
	SAFE_DELETE(m_pBtn6); // ���� �߰���� 1
	SAFE_DELETE(m_pBtn7); // PK ���Ÿ� �߰���� 2
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
		m_pBtn1->SetPosition(x+30, y+63); // ����
	if( m_pBtn2 )
		m_pBtn2->SetPosition(x+69, y+63); // ����
	if( m_pBtn3 )
		m_pBtn3->SetPosition(x+108, y+63); // ����
	if( m_pBtn4 )
		m_pBtn4->SetPosition(x+147, y+63); // ȸ��
	if( m_pBtn5 )
		m_pBtn5->SetPosition(x+352, y+5); // �ݱ�211,79
	if( m_pBtn6 )
		m_pBtn6->SetPosition(x+186, y+63); // ���� ��� 1
	if( m_pBtn7 )
		m_pBtn7->SetPosition(x+225, y+63); // PK ���Ÿ� �߰���� 2
	if( m_pBtn8 )
		m_pBtn8->SetPosition(x+264, y+63); // ���
	if( m_pBtn9 )
		m_pBtn9->SetPosition(x+303, y+63); // ��ũ��
}

void CLShiftMenu::Draw()
{
	if( !m_pSurfaceBack || !g_pDisplay )
		return;

	if( m_bMacroEditing && m_pControlMacro ) // ��ũ�� ��� �����߿� ��ũ�� ��� â�� ó��.
	{
		m_pControlMacro->Draw();

		return;
	}

	// Ȥ�� ȭ������� ������ ����� ��ǥ ����
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

	if( PosChange ) // ��ǥ�� �ٲ���ٸ�..
		SetPos( m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos ); // ��ư�� ��ǥ�� �ٽ� ����.

	int x, y;
	x = m_pSurfaceBack->Xpos; //��üȭ�鿡�� back�� x��ǥ
	y = m_pSurfaceBack->Ypos; //��üȭ�鿡�� back�� y��ǥ
	g_pDisplay->Blt(x, y, m_pSurfaceBack);
///�̸�����.���⿡ �ֿϵ��� �̸� �޾Ƽ� ����.
	if( m_pNameText )
		m_pNameText->Draw(m_pSurfaceBack->Xpos+98 , m_pSurfaceBack->Ypos + 43, 170);
	if( m_pBtn1 )
		m_pBtn1->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_COMBAT ); // ����
	if( m_pBtn2 )
		m_pBtn2->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_FOLLOW ); // ����
	if( m_pBtn3 )
		m_pBtn3->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_PICKUP ); // ����
	if( m_pBtn4 )
		m_pBtn4->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_WIMPY ); // ȸ��
	if( m_pBtn5 )
		m_pBtn5->DrawToBlt(); // �ݱ�
	if( m_pBtn6 )
		m_pBtn6->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_SUPP ); // ���� ���� �߰���� 1
	if( m_pBtn7 )
		m_pBtn7->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_RANGECOMBAT ); //  PK ���� ���Ÿ� �߰���� 2
	if( m_pBtn8 )
		m_pBtn8->DrawToBlt( !g_MyPetStatus.m_bUseMacroMode && g_MyPetStatus.mode == MYPET_MODE_GUARD /*|| !g_MyPetStatus.m_Class*/ ); // ��� // �⺻ ������ ��������.
	if( m_pBtn9 )
		m_pBtn9->DrawToBlt( g_MyPetStatus.m_bUseMacroMode /*|| !g_MyPetStatus.m_Class*/ ); // ��ũ�� // �⺻ ������ ��������.


	for (int i = 0; i < g_MyPetStatus.food_count; i++)
		g_pDisplay->Blt(m_pSurfaceBack->Xpos+101+(28*i), m_pSurfaceBack->Ypos+94, m_pApple);

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);
	CMultiText multiText;

	if( m_pMenuInfo )
	{
		// �ֿϵ��� ��� ����
		if( m_pBtn1 && m_pBtn1->IsInside( point.x, point.y ) )
		{
			if (m_pMenuInfo->GetIndex() != -100)
			{
				//-- IDS_LSHIFT_MOOD_0 : �������
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
				//-- IDS_LSHIFT_MOOD_1 : ������
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
				//-- IDS_LSHIFT_MOOD_2 : ȹ����
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
				//-- IDS_LSHIFT_MOOD_3 : ȸ�Ǹ��
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
				//-- IDS_LSHIFT_MODE_6 : �����
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
				//-- IDS_LSHIFT_MODE_7 : ��ũ�θ��
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
	static int DistX = 0, DistY = 0; // �巡�׽� Ŭ���� ���� ���콺 ��ǥ�� ���� ��ġ.

	if( m_bMacroEditing && m_pControlMacro ) // ��ũ�� ��� �����߿� ��ũ�� ��� â�� ó��.
	{
		int Result;
		Result = m_pControlMacro->MsgProc( hWnd, msg, wParam, lParam );
		if( Result == 2 ) // ���ϰ��� 2�̸� ������ �����Ŵ�.
		{
			m_pControlMacro->SaveMacro(); // ��ũ�θ� �����Ѵ�.
			m_pControlMacro->StartMacroMode();

			m_pControlMacro->m_bNowInput = FALSE; // �̰� ��Ǯ���� ���� �־���. Ǯ������.

			m_pBtn9->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}
		else if( Result == 1 ) // �׳� ���� ���� ���. �ٸ� �޼��� �����ϱ� ���� �ʿ�.
			return 1;
		else
			return 0; // �ƹ��͵� �ȴ����� �Ʒ��� ����Ǹ� �ȵȴ�.
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
				 && !m_pBtn7->IsInside(x,y)) // �����ȿ��� �ٿ��̸�...
		{
			bLShiftMenu_Drag = TRUE;
			DistX = m_pSurfaceBack->Xpos - x;
			DistY = m_pSurfaceBack->Ypos - y;
		}
		else if( !m_pBtn8->IsInside(x,y) && !m_pBtn9->IsInside(x,y) ) // �ӽ÷� ����� ����. ���߿� ���� ��ġ��.
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
		if(m_pBtn6 && m_pBtn6->GetState() != BTN_DISABLE)// ����
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
			m_pBtn6->MsgProc(hWnd, msg, wParam, lParam);// ����
		if(m_pBtn7)
			m_pBtn7->MsgProc(hWnd, msg, wParam, lParam); // PK
		if(m_pBtn8 && m_pBtn9->GetState() != BTN_DISABLE)
			m_pBtn8->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn9 && m_pBtn9->GetState() != BTN_DISABLE)
			m_pBtn9->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn5 && m_pBtn5->GetState() == BTN_ACTION) 			//����
		{
			m_pBtn5->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}

		if( g_pNk2DFrame )
		{
			if(m_pBtn1 && m_pBtn1->GetState() == BTN_ACTION)
			{
				//���� �޽��� ����
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_COMBAT);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);

				m_pBtn1->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_COMBAT);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)

				return 1;
			}
			else if(m_pBtn2 && m_pBtn2->GetState() == BTN_ACTION)
			{
				//���� �޽��� ����
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_FOLLOW);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn2->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_FOLLOW);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)

				return 1;
			}
			else if(m_pBtn3 && m_pBtn3->GetState() == BTN_ACTION)
			{
				//���� �޽��� ����
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_PICKUP);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn3->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_PICKUP);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)

				return 1;
			}
			else if(m_pBtn4 && m_pBtn4->GetState() == BTN_ACTION)
			{
				//ȸ�� �޽��� ����
				sprintf(tmp, "p_mode %d\n", MYPET_MODE_WIMPY);
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);
				m_pBtn4->SetState(BTN_NORMAL);
				CloseMenu();
				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode(MYPET_MODE_WIMPY);

				g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)

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

				g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)

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

				g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)

				return 1;
			}
			else if(m_pBtn8 && m_pBtn8->GetState() == BTN_ACTION ) // �����
			{
				if( g_MyPetStatus.m_Class ) // �⺻ ������ ��������.
				{
					sprintf(tmp, "p_mode %d\n", MYPET_MODE_GUARD);
					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage(tmp);
					m_pBtn8->SetState(BTN_NORMAL);
					CloseMenu();
					if (pCMyApp->m_pMyPet)
						g_MyPetStatus.SetMode(MYPET_MODE_GUARD);

					g_MyPetStatus.m_bUseMacroMode = FALSE; // �ٸ� ���� ����� �� ��ũ�� ��带 ����.(���� ��ũ�ΰ� ������ �Ŷ�� �� ���� �ٽ� TRUE�� ����������Ѵ�.)
				}
				else
				{
					//-- IDS_ITCAN_EVOLPET : ��ȭ�� ���븸 �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITCAN_EVOLPET), TYPE_NOR_OK, 1 );
				}


				return 1;
			}
			else if(m_pBtn9 && m_pBtn9->GetState() == BTN_ACTION ) // ��ũ�θ��
			{
				if( g_MyPetStatus.m_Class ) // �⺻ ������ ��������.
				{
					m_bMacroEditing = TRUE; // ��ũ�� ��� ���� ���� �ٲ۴�.
				}
				else
				{
					//-- IDS_ITCAN_EVOLPET : ��ȭ�� ���븸 �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITCAN_EVOLPET), TYPE_NOR_OK, 1 );
				}
			}
		}

		break;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bLShiftMenu_Drag ) // �巡��
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

			if( PosChange ) // ��ǥ�� �ٲ���ٸ�..
				SetPos( m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos ); // ��ư�� ��ǥ�� �ٽ� ����.



// 			if( m_pNameText )
// 				m_pNameText->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+83);

			if( m_pBtn1 )
				m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+30, m_pSurfaceBack->Ypos+63); // ����
			if( m_pBtn2 )
				m_pBtn2->SetPosition(m_pSurfaceBack->Xpos+69, m_pSurfaceBack->Ypos+63); // ����
			if( m_pBtn3 )
				m_pBtn3->SetPosition(m_pSurfaceBack->Xpos+108, m_pSurfaceBack->Ypos+63); // ����
			if( m_pBtn4 )
				m_pBtn4->SetPosition(m_pSurfaceBack->Xpos+147, m_pSurfaceBack->Ypos+63); // ȸ��
			if( m_pBtn5 )
				m_pBtn5->SetPosition(m_pSurfaceBack->Xpos+352, m_pSurfaceBack->Ypos+5); // �ݱ�211,79
			if( m_pBtn6 )
				m_pBtn6->SetPosition(m_pSurfaceBack->Xpos+186, m_pSurfaceBack->Ypos+63); // ���� ��� 1
			if( m_pBtn7 )
				m_pBtn7->SetPosition(m_pSurfaceBack->Xpos+225, m_pSurfaceBack->Ypos+63); // PK ���Ÿ� �߰���� 2
			if( m_pBtn8 )
				m_pBtn8->SetPosition(m_pSurfaceBack->Xpos+264, m_pSurfaceBack->Ypos+63); // ���
			if( m_pBtn9 )
				m_pBtn9->SetPosition(m_pSurfaceBack->Xpos+303, m_pSurfaceBack->Ypos+63); // ��ũ��
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
		if(m_pBtn6 && m_pBtn6->GetState() != BTN_DISABLE)//����
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

void CLShiftMenu::SetCurRace(int race)  //�� �ʿ������.
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
	else if( m_pBtn8 && m_pBtn9 ) // �ӽ÷� ����� ����. ���߿� ���� ��ġ��.
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
		m_bUseCheck[i] = FALSE; // ��� ���� üũ.
		m_ModeComboBox[i].SetState( 0 ); // ��Ȱ���� Ǭ��.
		m_ModeComboBox[i].SetSelect( 0 ); // ����Ʈ ������..
		m_DelayInputText[i].ClearText();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
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
		// [5/6/2008 �����] �� ��ũ�� �޺��ڽ� ������ ���� - �����
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

		m_pSurfaceBack->SetWidth( size_Width ); // �� CreateSurface���� CSurface �� ���� �����صδ� m_dwWidth,m_dwHeight�� ���� ��������ִ� ���װ� �־ �׳� ���� ����.
		m_pSurfaceBack->SetHeight( 250 );

		m_pSurfaceBack->SetColorKey(TRANS_COLOR);


		m_RunBtn.SetFileName( "lshiftmenu/PetMacro/btn_run" );
		m_RunBtn.LoadRes();

		m_CheckBtn.SetFileName( "lshiftmenu/PetMacro/check_btn" );
		m_CheckBtn.LoadRes();

		for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			// [5/6/2008 �����] �� ��ũ�� �޺��ڽ� ������ ���� - �����

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

			m_ModeComboBox[i].SetItemSize( pos_X, 16, 7, 4 ); // ����ġ �й� ���.
			m_ModeComboBox[i].LoadRes("lshiftmenu/PetMacro/btn_drowdown_pm", "interface/lshiftmenu/PetMacro/ComboBack.bmp", "interface/lshiftmenu/PetMacro/ListBack.bmp");

			m_ModeComboBox[i].DeleteAllItem(); // �̰� ������� �������� � �������� �ȵ���.
			m_ModeComboBox[i].InsertItem( " ",10, RGB(255, 210, 0), RGB(255,255,255) );
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_0),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_0 : �������
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_1),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_1 : ������
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_2),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_2 : ȹ����
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MOOD_3),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MOOD_3 : ȸ�Ǹ��
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MODE_4),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MODE_4 : �������
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MODE_5),10, RGB(255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MODE_5 : �������
			m_ModeComboBox[i].InsertItem( (char *)G_STRING(IDS_LSHIFT_MODE_6),10, RGB(1255, 210, 0), RGB(255,255,255) ); // IDS_LSHIFT_MODE_6 : �����

			m_DelayInputText[i].Init( 0, 0, 6, RGB(255, 255, 255), RGB(255, 255, 0) );
			m_DelayInputText[i].SetfontSize(); // ����Ʈ ��Ʈ ������� �ʱ�ȭ.(�̰� ���ϸ� �ٸ��� �ʱ�ȭ �ȵǼ� �ȵȴ�.)
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
		m_DelayInputText[i].DeleteRes();     // ���������� ����� ��Ʈ ��¿� ���ǽ�.
	}
}

void CControlPetMacro::SaveMacro() // ������ ���� ����
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
void CControlPetMacro::LoadMacro() // ����ص� ���� ��ũ�θ� �ε�.
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

void CControlPetMacro::StartMacroMode() // ó�� ������ ��ũ�θ� ã�Ƽ� ��Ŷ �����ְ� ��ũ�� ��� �������ִ� �༮.
{
	char tmp[256];

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		if( m_bUseCheck[i] ) // ���� ���� üũ �Ǿ��ִ� �༮�� ó���� �ٷ� ������.
		{
			sprintf(tmp, "p_mode %d\n", m_ModeComboBox[i].GetSelect()+11 ); // ���õǾ��ִ� ��带 ������.
			if (g_pTcpIp)
				g_pTcpIp->SendNetMessage(tmp);

			if (pCMyApp->m_pMyPet)
				g_MyPetStatus.SetMode( m_ModeComboBox[i].GetSelect()+11 );

			g_MyPetStatus.m_bUseMacroMode = TRUE; // ���� üũ �Ǿ��ִ� �༮�� ������ ��ũ�� ���� ��ȯ���� �ʴ´�.

			m_NowUse = i; // ���� �����ϴ� ��ũ�� ��ȣ�� ���.
			m_dwPrevChangeTick = timeGetTime();

			break; // ���� ����.
		}
	}
}

int CControlPetMacro::CheckMacroTime() // ��ũ�� ��� �ٲ��ֱ� üũ.
{
	char tmp[256];

	int Delay = atoi( m_DelayInputText[m_NowUse].GetStrBuf() );

	if( timeGetTime()-m_dwPrevChangeTick > Delay*1000 ) // ��带 �ٲ� �ð��� �Ǿ��ٸ�...
	{
		for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			++m_NowUse; // ��ũ�θ� �Ѵܰ� ������Ű��.
			if( m_NowUse >= PET_MACRO_MAX )
				m_NowUse = 0; // ����

			if( m_bUseCheck[m_NowUse] && atoi(m_DelayInputText[m_NowUse].GetStrBuf()) ) // ��� üũ�� �Ǿ��ִ� �༮�̶��...(�����̵� 0�ʸ� ����)
			{
				sprintf(tmp, "p_mode %d\n", m_ModeComboBox[m_NowUse].GetSelect()-1+11 ); // ���õǾ��ִ� ��带 ������.
				if (g_pTcpIp)
					g_pTcpIp->SendNetMessage(tmp);

				if (pCMyApp->m_pMyPet)
					g_MyPetStatus.SetMode( m_ModeComboBox[m_NowUse].GetSelect()-1+11 );

				g_MyPetStatus.m_bUseMacroMode = TRUE; // ���� üũ �Ǿ��ִ� �༮�� ������ ��ũ�� ���� ��ȯ���� �ʴ´�.

				m_dwPrevChangeTick = timeGetTime();

				return m_NowUse; // ���������.
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

		// [5/6/2008 �����] �� ��ũ�� �޺��ڽ� ������ ���� - �����
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

		if( m_ModeComboBox[i].GetState() > 0 ) // ����Ʈ ���������� �༮�� ������...
			SelectedCombo = i; // ��¾��ϰ� �ε����� ���.
		else
			m_ModeComboBox[i].Draw(); // ���� ���°� �ƴ� �༮�� ���.

		m_DelayInputText[i].Draw();
	}

	if( SelectedCombo != -1 ) // ���������� �޺��ڽ��� �־��ٸ�...
		m_ModeComboBox[SelectedCombo].Draw(); // ���� ������ �༮�� �������� ���.
}

void CControlPetMacro::SetPos(int x, int y)
{
	if( m_pSurfaceBack )
	{
		m_pSurfaceBack->Xpos = x;
		m_pSurfaceBack->Ypos = y;
	}

	// [5/6/2008 �����] OK ��ư ��ġ ���� - �����

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

	m_RunBtn.SetPosition(m_pSurfaceBack->Xpos+83 , m_pSurfaceBack->Ypos+206); // ��ũ��

	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		m_ModeComboBox[i].SetPosition( m_pSurfaceBack->Xpos+35, m_pSurfaceBack->Ypos+39+(i*33) ); // ����ġ �й� ���.
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
		// [5/6/2008 �����] ������ �Է�â ��ġ ���� �ڷ� - �����
		m_DelayInputText[i].SetPos( m_pSurfaceBack->Xpos+pos_X2, m_pSurfaceBack->Ypos+47+(i*33) );
	}
}

BOOL CControlPetMacro::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;
	return m_pSurfaceBack->IsIn(x, y);
}

int CControlPetMacro::IsInsideContents(int x, int y) // �������̽� �� �۵��ϴ� ������ ��ġ�ΰ� üũ.
{
	for( int i = 0 ; i < PET_MACRO_MAX ; ++i )
	{
		// [5/6/2008 �����] ����� ��ũ�� ������ ���� ������ üũ�ڽ� ��ġ�� ����.
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

		// üũ�ڽ�
		if( x >= pos_X && x < pos_X + m_CheckBtn.Width
				&& y >= m_DelayInputText[i].m_Y-1 && y < m_DelayInputText[i].m_Y-1 + m_CheckBtn.Height )
		{
			return (i*3)+1;
		}

		// �޺��ڽ�
		if( x >= m_ModeComboBox[i].m_DrawPosX && x < m_ModeComboBox[i].m_DrawPosX + m_ModeComboBox[i].m_pSelectedBackGround->GetWidth() + m_ModeComboBox[i].m_pListViewBtn->Width
				&& y >= m_ModeComboBox[i].m_DrawPosY && y < m_ModeComboBox[i].m_DrawPosY + m_ModeComboBox[i].m_pSelectedBackGround->GetHeight() ) // ���ο� ��ư ���� �÷��� ������Ѵ�.
		{
			return (i*3)+2;
		}

		// ������ �Է¹ڽ�
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
	static int DistX = 0, DistY = 0; // �巡�׽� Ŭ���� ���� ���콺 ��ǥ�� ���� ��ġ.

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
			m_RunBtn.SetState(BTN_NORMAL); // ���¸� ���� �����ش�.

			return 2; // ������ ���������� ���ϰ��� Ʋ���� ����.
		}

		for( i = 0 ; i < PET_MACRO_MAX ; ++i )
		{
			if( m_ModeComboBox[i].MsgProc(hWnd, msg, wParam, lParam) )
			{
				// �޺� �ڽ��� �� ����Ʈ ������ �ٸ� �༮�� ħ���Ҽ� �ֱ⿡ �ڽ��� ������ ���Ͻ�Ų��.(�ٸ� �޺� �ڽ��鿡�� �ݱ� ó���� �ʿ��ҵ�.)

				return 1;
			}

			m_DelayInputText[i].MsgProc(hWnd, msg, wParam, lParam);
		}


		i = IsInsideContents( x, y );

		// �巡���� ó���� ������ �ٸ� ���ϵ��� ������ ó���Ѵ�.
		if( msg == WM_LBUTTONDOWN )
		{
			if(!IsInside(x, y))
				return 0;
			else if( !m_RunBtn.IsInside(x,y) && i == 0 ) // �����ȿ��� �ٿ��̸�...// i�� 0�� �ƴϸ� �巡�� �ϸ� �ȵȴ�.(i���� �ٸ��� ���� ������ ���ִ´�.)
			{
				bDrag = TRUE;
				DistX = m_pSurfaceBack->Xpos - x;
				DistY = m_pSurfaceBack->Ypos - y;
			}

			return 1; // ���� �ȿ���... ���콺 ������ ���
		}
		else if( msg == WM_LBUTTONUP )
		{
			bDrag = FALSE;

			if( i != 0 ) // ���� ��������...
			{
				if( i % 3 == 1 ) // üũ ��ư�� ���� ���.
				{
					m_bUseCheck[i/3] ^= 1; // ��� ��Ų��.
				}
			}

			return 1; // ���� �ȿ���... ���콺 ������ ���
		}
		else if( msg == WM_MOUSEMOVE )
		{
			if( bDrag ) // �巡��
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

