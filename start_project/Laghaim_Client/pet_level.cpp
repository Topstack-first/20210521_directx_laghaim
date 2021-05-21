#include "stdafx.h"


#include "headers.h"
#include "Nk2DFrame.h"
#include "ComboBox.h"
#include "pet_level.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "tcpipcon.h"
#include "NkCharacter.h"
#include "nkmob.h"
#include "nkpet.h"
#include "nkMypet.h"
#include "g_stringmanager.h"
#include "PacketSend.h"
#include "info_define.h"

int PetSkillNeed[3][4][3] =
{
	{
		{20,5000000, 5000000}, // �극�� (�ʿ䷹��/����/����ġ)
		{40,5000000,10000000}, // ��ȣ (�ʿ䷹��/����/����ġ)
		{80,5000000,15000000}, // ��ȣ (�ʿ䷹��/����/����ġ)
		{90,5000000,20000000}, // ���� (�ʿ䷹��/����/����ġ)
	}, // �����
	{
		{20,5000000, 5000000}, // ������ (�ʿ䷹��/����/����ġ)
		{40,5000000,10000000}, // ���� (�ʿ䷹��/����/����ġ)
		{80,5000000,15000000}, // �ݵ� (�ʿ䷹��/����/����ġ)
		{90,5000000,20000000}, // ��Ȧ (�ʿ䷹��/����/����ġ)
	},  // ������
	{
		{20,5000000, 5000000}, // ������ (�ʿ䷹��/����/����ġ)
		{40,5000000,10000000}, // ���� (�ʿ䷹��/����/����ġ)
		{80,5000000,15000000}, // ��ȣ (�ʿ䷹��/����/����ġ)
		{90,5000000,20000000}, // ���� (�ʿ䷹��/����/����ġ)
	}
}; // ���� ��ų�� ���µ� �ʿ��� ����/ ����ġ ���̺�


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPetLevel::CPetLevel()
	: m_money(0)
{
	memset(m_UserName, 0, sizeof(m_UserName));

	m_pSurfaceBack = NULL;
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;
	m_pMoneyText = NULL;
	m_pLevelText = NULL;
	m_pExpText = NULL;
	m_pNeedExpText = NULL;
	m_pNeedMoneyText = NULL;
}

CPetLevel::~CPetLevel()
{
	DeleteRes();

	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pBtn1);
	SAFE_DELETE(m_pBtn2);
	SAFE_DELETE(m_pMoneyText);
	SAFE_DELETE(m_pLevelText);
	SAFE_DELETE(m_pExpText);
	SAFE_DELETE(m_pNeedExpText);
	SAFE_DELETE(m_pNeedMoneyText);
}

void CPetLevel::Init()
{
	m_Combo1.SetState( 0 ); // ��Ȱ���� Ǭ��.
	m_Combo1.SetSelect( 0 ); // ����Ʈ ������..

	m_Combo2.SetState( -1 ); // �� �༮�� ó���� ��Ȱ��.
	m_Combo2.SetSelect( 0 ); // ����Ʈ ������..

	m_pBtn1->SetState( BTN_DISABLE ); // OK ��ư ��Ȱ�� �ʱ�ȭ.
}

void CPetLevel::LoadRes()
{
	if( !g_pDisplay )
		return;

	DeleteRes();

	m_pBtn1 = new CBasicButton();
	m_pBtn2 = new CBasicButton();

	if( m_pBtn1 )	m_pBtn1->Init("petlevel/btn_training");// ����
	if( m_pBtn2 )	m_pBtn2->Init("common/btn_close_01");// �ݱ�

	if( m_pBtn1 )	m_pBtn1->LoadRes();
	if( m_pBtn2 )	m_pBtn2->LoadRes();

	m_Combo1.SetItemSize( 100, 16, 7, 4 );
	m_Combo1.LoadRes("PetLevel/btn_dropdown", "interface/PetLevel/but01.bmp", "interface/PetLevel/ListBack_4.bmp");

	m_Combo1.DeleteAllItem(); // �̰� ������� �������� � �������� �ȵ���.
	m_Combo1.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_STUDY : ����
	m_Combo1.InsertItem( (char*)G_STRING(IDS_STUDY),13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_EVOLUTION : ��ȭ
	m_Combo1.InsertItem( (char*)G_STRING(IDS_EVOLUTION),13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_SKILL : ��ų
	m_Combo1.InsertItem( (char*)G_STRING(IDS_SKILL),13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_PET_SKILL_INIT : ��ų �ʱ�ȭ
	m_Combo1.InsertItem( (char*)G_STRING(IDS_PET_SKILL_INIT), 13, RGB(255, 210, 0), RGB(255,255,255) );


	m_Combo2.SetItemSize( 95, 16, 7, 4 );
	m_Combo2.LoadRes("PetLevel/btn_dropdown", "interface/PetLevel/but01.bmp", "interface/PetLevel/ListBack_4.bmp");


	m_pMoneyText = new CTextOutBox();
	if( m_pMoneyText )
		m_pMoneyText->Init(12, RGB(255, 210, 0),0, 0);

	m_pLevelText = new CTextOutBox();
	if( m_pLevelText )
		m_pLevelText->Init(12, RGB(255, 210, 0),0, 0);

	m_pExpText = new CTextOutBox();
	if( m_pExpText )
		m_pExpText->Init(12, RGB(255, 210, 0),0, 0);

	m_pNeedExpText = new CTextOutBox();
	if( m_pNeedExpText )
		m_pNeedExpText->Init(12, RGB(255, 210, 0),0, 0);

	m_pNeedMoneyText = new CTextOutBox();
	if( m_pNeedMoneyText )
		m_pNeedMoneyText->Init(12, RGB(255, 210, 0),0, 0);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/petlevel/pet_level.BMP");
	m_pSurfaceBack->SetColorKey( TRANS_COLOR );

	CloseMenu();
}

void CPetLevel::DeleteRes()
{
	SAFE_SURFACE_DESTROY(m_pSurfaceBack);
	SAFE_DELETE_RES(m_pBtn1);
	SAFE_DELETE_RES(m_pBtn2);
	SAFE_DELETE_RES(m_pMoneyText);
	SAFE_DELETE_RES(m_pLevelText);
	SAFE_DELETE_RES(m_pExpText);
	SAFE_DELETE_RES(m_pNeedExpText);
	SAFE_DELETE_RES(m_pNeedMoneyText);

	m_Combo1.DeleteRes();
	m_Combo2.DeleteRes();
}

HRESULT CPetLevel::Restore()
{
	DeleteRes();
	LoadRes();

	return S_OK;
}

int CPetLevel::GetWidth()
{
	if (m_pSurfaceBack)
		return m_pSurfaceBack->GetWidth();

	return 0;
}

int CPetLevel::GetHeight()
{
	if (m_pSurfaceBack)
		return m_pSurfaceBack->GetHeight();

	return 0;
}

void CPetLevel::SetXPos(int x)
{
	if (m_pSurfaceBack)
		m_pSurfaceBack->Xpos = x;
}

void CPetLevel::SetYPos(int y)
{
	if (m_pSurfaceBack)
		m_pSurfaceBack->Ypos = y;
}


void CPetLevel::Draw()
{
	if( !m_pSurfaceBack || !g_pDisplay )
		return;

	int x, y;
	x = m_pSurfaceBack->Xpos; //��üȭ�鿡�� back�� x��ǥ
	y = m_pSurfaceBack->Ypos; //��üȭ�鿡�� back�� y��ǥ

	g_pDisplay->Blt(x, y, m_pSurfaceBack);

	if(g_MyPetStatus.mode>10)
	{
		// ������ ���� ����ġ�� �׻� ���.
		if( m_pLevelText )		m_pLevelText->SetString(g_MyPetStatus.level,TRUE);
		if( m_pExpText )		m_pExpText->SetString(g_MyPetStatus.exp,TRUE);

		__int64 NeedExp = 0, NeedLaim = 0;

		switch( m_Combo1.GetSelect() ) // ���Ϸ��Ŀ� ���� ǥ�õ� Ʋ������.
		{
		case 1: // ����.
			if( m_pNeedExpText )	m_pNeedExpText->SetString(g_MyPetStatus.need_exp,TRUE);
			if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString(g_MyPetStatus.need_money,TRUE);
			break;

		case 2: // ��ȭ.
			if( m_pNeedExpText )	m_pNeedExpText->SetString( "-" );
			if( m_pNeedMoneyText )
			{
				if( g_MyPetStatus.m_Class == 0 )
					m_pNeedMoneyText->SetString(10000000, TRUE);
				else if( g_MyPetStatus.m_Class == 1 || g_MyPetStatus.m_Class == 2 )
					m_pNeedMoneyText->SetString(100000000, TRUE);
				else
					m_pNeedMoneyText->SetString("-");
			}
			break;

		case 3: // ��ų

			if( m_Combo2.GetSelect() != 0 )
			{
				if( g_MyPetStatus.m_Class > 0 )
					NeedExp = PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][2];

				if( g_MyPetStatus.m_Class > 0 )
					NeedLaim = PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][1];

				if( m_pNeedExpText )	m_pNeedExpText->SetString( NeedExp,TRUE );
				if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString( NeedLaim,TRUE );
			}
			else // ���� ���õ� ��ų�� ������...
			{
				if( m_pNeedExpText )	m_pNeedExpText->SetString( "-" );
				if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString( "-" );
			}

			break;
		case 4: // ��ų �ʱ�ȭ
			if( m_pNeedExpText )	m_pNeedExpText->SetString( "-" );
			if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString( PETSKILL_INIT_MONEY, TRUE );
			break;

		default:
			if( m_pNeedExpText )	m_pNeedExpText->SetString( "-" );
			if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString( "-" );
			break;
		}
	}

	if( m_pBtn1 )	m_pBtn1->Draw(x+142, y+238);
	if( m_pBtn2 )	m_pBtn2->Draw(x+346, y+5);

	m_Combo1.SetPosition( x+50,y+148 );
	m_Combo1.Draw();
	m_Combo2.SetPosition( x+200,y+148 );
	m_Combo2.Draw();

	if( m_pLevelText )		m_pLevelText->Draw(x+100,y+60,78);  //����
	if( m_pNeedExpText )	m_pNeedExpText->Draw(x+100,y+92,78);  //�ʿ� ����ġ
	if( m_pExpText )		m_pExpText->Draw(x+100,y+123,78);  //ȹ�� ����ġ
	if( m_pNeedMoneyText )	m_pNeedMoneyText->Draw(x+253,y+92,78);  //�ʿ� �ݾ�
	if( m_pMoneyText )		m_pMoneyText->Draw(x+253,y+123,78);  //���� �����ݾ�.


	// by evilkiki...draw pet
	/// ���� �׸��÷ƴϱ�?
	// CSMRender * rendertarget;
}

BOOL CPetLevel::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return FALSE;

	return m_pSurfaceBack->IsIn(x ,y);
}

LRESULT CPetLevel::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmp[100] = "";
	char tmp2[100] = "";
	int x = 0, y = 0;
	int ret = -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		if(!IsInside(x, y))
		{
			CloseMenu();
			return 1;
		}
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:


		if( m_Combo1.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_Combo2.MsgProc(hWnd, msg, wParam, lParam); // ���⼭ �̰� ���ִ� ������ �� �޺��ڽ��� �켱����(���� ���϶����� �ٸ� �޺��ڽ��� ���õɼ������Ƿ�.)�����̴�. ���� ��� select�� �ٸ� ���� ������ select��尡 ��ҵǾ� ������..

			// �� �༮�� ������ �ٲ������ m_Combo2�� ������ �ش�.
			if( m_Combo1.GetSelect() == 1 || // ����
				m_Combo1.GetSelect() == 4 )  // ��ų �ʱ�ȭ
			{
				m_Combo2.DeleteAllItem(); // ���� �����.
				m_Combo2.SetSelect( 0 );
				m_Combo2.SetState( -1 ); // ��Ȱ�� ��Ų��.

				m_pBtn1->SetState( BTN_NORMAL ); // OK ��ư Ȱ��
			}
			else if( m_Combo1.GetSelect() == 2 ) // ��ȭ
			{
				m_Combo2.DeleteAllItem(); // ���� �����.
				m_Combo2.SetSelect( 0 );
				m_Combo2.SetState( 0 ); // Ȱ�� ��Ų��.

				if( g_MyPetStatus.m_Class == 0 )
				{
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_DEFENCE_TYPE : �����
					m_Combo2.InsertItem( (char*)G_STRING(IDS_DEFENCE_TYPE),13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_ATTACK_TYPE : ������
					m_Combo2.InsertItem( (char*)G_STRING(IDS_ATTACK_TYPE),13, RGB(255, 210, 0), RGB(255,255,255) );
				}
				else
				{
					m_Combo2.SetState( -1 ); // ���� ��Ȱ�� ��Ų��.
				}

				if( g_MyPetStatus.m_Class == 0 || g_MyPetStatus.m_Class == 3 )
				{
					m_pBtn1->SetState( BTN_DISABLE ); // OK ��ư ��Ȱ�� �ʱ�ȭ.
				}
				else
				{
					m_pBtn1->SetState( BTN_NORMAL );
				}
			}
			else if( m_Combo1.GetSelect() == 3 ) // ��ų
			{
				m_Combo2.DeleteAllItem(); // ���� �����.
				m_Combo2.SetSelect( 0 );
				m_Combo2.SetState( 0 ); // Ȱ�� ��Ų��.

				// �ڽ� �� Ÿ�Ե��� ����Ʈ�� �޶�����.
				switch( g_MyPetStatus.m_Class )
				{
				case 1: // �����
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL4_BREATH : �극��
					sprintf( tmp, "%s(20Lv)", (char*)G_STRING(IDS_PETSKILL4_BREATH) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL5_GUARD : ��ȣ
					sprintf( tmp, "%s(40Lv)", (char*)G_STRING(IDS_PETSKILL5_GUARD) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL6_DEFENCE : ��ȣ
					sprintf( tmp, "%s(80Lv)", (char*)G_STRING(IDS_PETSKILL6_DEFENCE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL7_LIFE : ����
					sprintf( tmp, "%s(90Lv)", (char*)G_STRING(IDS_PETSKILL7_LIFE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					break;
				case 2: // ������
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL0_POISON : ������
					sprintf( tmp, "%s(20Lv)", (char*)G_STRING(IDS_PETSKILL0_POISON) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL1_PARALYSIS : ����
					sprintf( tmp, "%s(40Lv)", (char*)G_STRING(IDS_PETSKILL1_PARALYSIS) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL2_COLD : �ݵ�
					sprintf( tmp, "%s(80Lv)", (char*)G_STRING(IDS_PETSKILL2_COLD) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL3_BLACKHOLE : ��Ȧ
					sprintf( tmp, "%s(90Lv)", (char*)G_STRING(IDS_PETSKILL3_BLACKHOLE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					break;
				case 3:
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					sprintf( tmp, "%s(20Lv)", (char*)G_STRING(IDS_PETSKILL0_POISON) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					sprintf( tmp, "%s(40Lv)", (char*)G_STRING(IDS_PETSKILL1_PARALYSIS) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					sprintf( tmp, "%s(80Lv)", (char*)G_STRING(IDS_PETSKILL6_DEFENCE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					sprintf( tmp, "%s(90Lv)", (char*)G_STRING(IDS_PETSKILL7_LIFE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					break;
				default:
					m_Combo2.SetState( -1 ); // ���� ��Ȱ�� ��Ų��.
					break;
				}

				m_pBtn1->SetState( BTN_DISABLE ); // OK ��ư ��Ȱ�� �ʱ�ȭ.
			}

			// 1�� ���ϵǾ����� �ٸ� ���ν��� ����. (�޺��ڽ��� ����Ʈ�� �ٸ� ��Ʈ�ѵ��� ���� �������������Ƿ� �� ó���� �ʿ��ϴ�.)
			return 1;
		}
		if( m_Combo2.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_Combo1.MsgProc(hWnd, msg, wParam, lParam); // ���⼭ �̰� ���ִ� ������ �� �޺��ڽ��� �켱����(���� ���϶����� �ٸ� �޺��ڽ��� ���õɼ������Ƿ�.)�����̴�. ���� ��� select�� �ٸ� ���� ������ select��尡 ��ҵǾ� ������..

			if( m_Combo2.GetSelect() != 0 ) // ���� ���õǾ� ���ٸ�...
				m_pBtn1->SetState( BTN_NORMAL ); // OK ��ư Ȱ��

			// 1�� ���ϵǾ����� �ٸ� ���ν��� ����. (�޺��ڽ��� ����Ʈ�� �ٸ� ��Ʈ�ѵ��� ���� �������������Ƿ� �� ó���� �ʿ��ϴ�.)
			return 1;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn2 && m_pBtn2->GetState() != BTN_DISABLE)
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn2 && m_pBtn2->GetState() == BTN_ACTION)
		{
			//�ݱ�
			m_pBtn2->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}

		if(m_pBtn1 && m_pBtn1->GetState() == BTN_ACTION) // ����
		{
			if( m_Combo1.GetSelect() == 1 ) // ����.
			{
				//���� �޽���.

				if(g_MyPetStatus.mode>10)
				{
					if(m_money < g_MyPetStatus.need_money) //���� �����Ҷ�
					{
						//-- IDS_PETLEV_NOTENOUGH_MONEY : ���� �����Ͽ� �������� �Ҽ��� �����ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_NOTENOUGH_MONEY), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if( g_MyPetStatus.level >= 100 ) // �������� �߰� (05-10-28 ����)
					{
						//-- IDS_INVEN_REQUILE_MORE_LEVEL : ������ ���� �ʽ��ϴ�
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if(g_MyPetStatus.exp < g_MyPetStatus.need_exp)
					{
						//-- IDS_PETLEV_NOTENOUGH_LEVEL : ����ġ�� �����Ͽ� �������� �Ҽ��� �����ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_NOTENOUGH_LEVEL), TYPE_NOR_OK, 1 );
						return 1;
					}
					else
					{
						sprintf(tmp, "p_level up\n");
						if (g_pTcpIp)	g_pTcpIp->SendNetMessage(tmp);
						//-- IDS_PETLEV_COMPLETE : ���������� �������� �Ǿ����ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_COMPLETE), TYPE_NOR_OK, 1 );
						return 1;
					}
				}
				else if(g_MyPetStatus.mode==MYPET_MODE_NONE)
				{
					//-- IDS_PETLEV_PRE_BUY : ���� �ֿϵ����� ��ž� �մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_PRE_BUY), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if(g_MyPetStatus.mode==MYPET_MODE_EGG)
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_PRE_REVIVE), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if(g_MyPetStatus.mode==MYPET_MODE_KEEP || g_MyPetStatus.mode==MYPET_MODE_HOLD)
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_PRE_TAKE), TYPE_NOR_OK, 1 );
					return 1;
				}

				if( m_pBtn1 )
					m_pBtn1->SetState(BTN_NORMAL);
				CloseMenu();

				return 1;
			}
			else if( m_Combo1.GetSelect() == 2 ) // ��ȭ
			{
				if( g_MyPetStatus.m_Class == 0 && m_Combo2.GetSelect() == 0 ) // ��ȭ ������ �����Ǿ�����������...
				{
					//-- IDS_CHOOSE_EVOL_TYPE : ��ȭ Ÿ���� ����ּ���.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHOOSE_EVOL_TYPE), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_Class >= 3 )
				{
					//-- IDS_CANT_MORE_EVOL : �� �̻� ��ȭ�� �� �����ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_MORE_EVOL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.level != 100 ) // 100���� ���븸 ��ȭ�� �����ϴ�.
				{
					//-- IDS_INVEN_REQUILE_MORE_LEVEL : ������ ���� �ʽ��ϴ�
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_Class == 0 && m_money < 10000000) // ���� ���� 1õ������.
				{
					//-- IDS_EXCHANGE_SCARCE_MONEY : �ݾ��� �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( (g_MyPetStatus.m_Class == 1 || g_MyPetStatus.m_Class == 2) &&
						  m_money < 100000000)
				{
					//-- IDS_EXCHANGE_SCARCE_MONEY : �ݾ��� �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}

				if(g_MyPetStatus.m_Class == 0)
				{
					// ������ üũ. ��ȣ��â�� ������ �ϳ��� �־���Ѵ�.
					CItem* pItem = NULL;
					if( g_pRoh )
						pItem = g_pRoh->m_ExgInven;

					if( !pItem || pItem->m_Vnum != 1454 || pItem->m_Next ) // �����ְ� �ƴϸ�..
					{
						//-- IDS_NEED_ONE_DRAGONBALL : ��ȯâ�� ������ ������ �ϳ��� �����ּ���.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEED_ONE_DRAGONBALL), TYPE_NOR_OK, 1 );

						return 1;
					}

					if( m_Combo2.GetSelect() == 1 ) // �����.
						//-- IDS_DEFENCE_TYPE : �����
						sprintf( tmp2, (char*)G_STRING(IDS_DEFENCE_TYPE) );
					else if( m_Combo2.GetSelect() == 2 ) // ������.
						//-- IDS_ATTACK_TYPE : ������
						sprintf( tmp2, (char*)G_STRING(IDS_ATTACK_TYPE) );

					//-- IDS_WANT_PET_EVOL : ������ %s���� ��ȭ��Ű�ðڽ��ϱ�?
					sprintf(tmp, (char*)G_STRING(IDS_WANT_PET_EVOL), tmp2 );
					g_pNk2DFrame->InsertPopup( tmp, TYPE_NOR_OKCANCLE, 31 );
				}
				else
				{
					// �ż�
					int cnt = g_pRoh->GetCountInExch(1740);
					if( cnt < 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEWPET_MSG0), TYPE_NOR_OK, 1 );
						return 1;
					}
					// ������ �ż�
					cnt = g_pRoh->GetCountInExch(10021);
					if( cnt < 10 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEWPET_MSG0), TYPE_NOR_OK, 1 );
						return 1;
					}
					// ������ �縷 ����
					cnt = g_pRoh->GetCountInExch(1527);
					if( cnt < 4 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEWPET_MSG0), TYPE_NOR_OK, 1 );
						return 1;
					}
					cnt = g_pRoh->GetCountInExch(10022);
					if( cnt < 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEWPET_MSG0), TYPE_NOR_OK, 1 );
						return 1;
					}

					sprintf(tmp, (char*)G_STRING(IDS_NEWPET_MSG1));
					g_pNk2DFrame->InsertPopup(tmp, TYPE_NOR_OKCANCLE, 31);					
				}				

				return 1;
			}
			else if( m_Combo1.GetSelect() == 3 ) // ��ų
			{
				if( m_Combo2.GetSelect() == 0 ) // ��ų ������ �����Ǿ�����������...
				{
					//-- IDS_CHOOSE_SKILL : ��ų�� ����ּ���.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHOOSE_SKILL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_Class == 0 ) // �⺻ ���� �ȵ�
				{
					//-- IDS_ITCAN_EVOLPET : ��ȭ�� ���븸 �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITCAN_EVOLPET), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.level < PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][0] ) // �ʿ� ���� üũ.
				{
					//-- IDS_INVEN_REQUILE_MORE_LEVEL : ������ ���� �ʽ��ϴ�
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( m_money < PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][1] ) //���� �����Ҷ�
				{
					//-- IDS_EXCHANGE_SCARCE_MONEY : �ݾ��� �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.exp < PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][2] ) // ����ġ�� �����Ҷ�...
				{
					//-- IDS_MORE_NEED_EXP : ����ġ�� �����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_MORE_NEED_EXP), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_SkillLevel[m_Combo2.GetSelect()-1] ) // �̹� ��� ��ų üũ.
				{
					//-- IDS_ALREADY_STUDY_SKILL : �̹� ��� ��ų�Դϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ALREADY_STUDY_SKILL), TYPE_NOR_OK, 1 );
					return 1;
				}

				if( g_MyPetStatus.m_Class == 1 ) // �����
				{	
					GetPetSkillName( (m_Combo2.GetSelect()-1)+PET_SKILL_MAX , tmp2 );					
				}
				else if( g_MyPetStatus.m_Class == 2 ) // ������
				{	
					GetPetSkillName( (m_Combo2.GetSelect()-1) , tmp2 ) ;					
				}
				else if( g_MyPetStatus.m_Class == 3 ) // ������
				{
					int sel = m_Combo2.GetSelect()-1;
					switch(sel)
					{
					case 0: GetPetSkillName(0, tmp2); break;
					case 1: GetPetSkillName(1, tmp2); break;
					case 2: GetPetSkillName(6, tmp2); break;
					case 3: GetPetSkillName(7, tmp2); break;
					default: GetPetSkillName(-1, tmp2); break;
					}
				}

				//-- IDS_WANT_STUDY_PETSKILL :  "%s" ��ų�� ���ðڽ��ϱ�?
				sprintf(tmp, (char*)G_STRING(IDS_WANT_STUDY_PETSKILL), tmp2 );
				g_pNk2DFrame->InsertPopup( tmp, TYPE_NOR_OKCANCLE, 31 );

				return 1;
			}
			else if( m_Combo1.GetSelect() == 4 ) // ��ų �ʱ�ȭ
			{
				if( g_MyPetStatus.m_Class < 3 )
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PET_SKILL_INIT_CONFIRM), TYPE_NOR_OKCANCLE, POPUP_PET_UPGRADE);
				else
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_NEWPET_SKILL_INIT_CONFIRM), TYPE_NOR_OKCANCLE, POPUP_PET_UPGRADE);

				return 1;
			}

			return 1;
		}

		if( msg == WM_LBUTTONDOWN && IsInside(x, y) ) // �������̽� ���� ����� Ŭ���ÿ� ��Ŭ���� �Ⱥ�������..
			return 1;

		break;
	}
	return 0;
}

HRESULT CPetLevel::PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char tmp[100] = "";

	if(nMsg == 1)
	{
		if( m_Combo1.GetSelect() == 2 ) // ��ȭ
		{
			if(g_MyPetStatus.m_Class == 0)
			{
				sprintf(tmp, "p_upgrade %d\n", m_Combo2.GetSelect() );
			}
			else
			{
				sprintf(tmp, "p_drake_upgrade\n");
			}
			
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(tmp);
		}
		else if( m_Combo1.GetSelect() == 3 ) // ��ų
		{
			sprintf(tmp, "p_skillup %d\n", g_PetSkillGroup[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1] );
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(tmp);
		}
		else if( m_Combo1.GetSelect() == 4 ) // ��ų �ʱ�ȭ
		{
			SendPetSkillInit();
		}
	}

	return 0L;
}

void CPetLevel::CloseMenu()
{
	if( g_pNk2DFrame )	g_pNk2DFrame->SetShowPetLevelFlag(false);

	if(m_pBtn1 && m_pBtn2 )
	{
		m_pBtn1->SetState(BTN_NORMAL);
		m_pBtn2->SetState(BTN_NORMAL);
	}
}

void CPetLevel::SetMoney(__int64 money)
{
	if(!m_pMoneyText)
		return;

	m_money = money;
	m_pMoneyText->SetString(money,TRUE);
}
