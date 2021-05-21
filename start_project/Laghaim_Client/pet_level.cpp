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
		{20,5000000, 5000000}, // 브레스 (필요레벨/라임/경험치)
		{40,5000000,10000000}, // 수호 (필요레벨/라임/경험치)
		{80,5000000,15000000}, // 보호 (필요레벨/라임/경험치)
		{90,5000000,20000000}, // 생명 (필요레벨/라임/경험치)
	}, // 방어형
	{
		{20,5000000, 5000000}, // 포이즌 (필요레벨/라임/경험치)
		{40,5000000,10000000}, // 마비 (필요레벨/라임/경험치)
		{80,5000000,15000000}, // 콜드 (필요레벨/라임/경험치)
		{90,5000000,20000000}, // 블랙홀 (필요레벨/라임/경험치)
	},  // 공격형
	{
		{20,5000000, 5000000}, // 포이즌 (필요레벨/라임/경험치)
		{40,5000000,10000000}, // 마비 (필요레벨/라임/경험치)
		{80,5000000,15000000}, // 보호 (필요레벨/라임/경험치)
		{90,5000000,20000000}, // 생명 (필요레벨/라임/경험치)
	}
}; // 펫의 스킬을 배우는데 필요한 라임/ 경험치 테이블


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
	m_Combo1.SetState( 0 ); // 비활성을 푼다.
	m_Combo1.SetSelect( 0 ); // 디폴트 값으로..

	m_Combo2.SetState( -1 ); // 이 녀석은 처음엔 비활성.
	m_Combo2.SetSelect( 0 ); // 디폴트 값으로..

	m_pBtn1->SetState( BTN_DISABLE ); // OK 버튼 디활성 초기화.
}

void CPetLevel::LoadRes()
{
	if( !g_pDisplay )
		return;

	DeleteRes();

	m_pBtn1 = new CBasicButton();
	m_pBtn2 = new CBasicButton();

	if( m_pBtn1 )	m_pBtn1->Init("petlevel/btn_training");// 교육
	if( m_pBtn2 )	m_pBtn2->Init("common/btn_close_01");// 닫기

	if( m_pBtn1 )	m_pBtn1->LoadRes();
	if( m_pBtn2 )	m_pBtn2->LoadRes();

	m_Combo1.SetItemSize( 100, 16, 7, 4 );
	m_Combo1.LoadRes("PetLevel/btn_dropdown", "interface/PetLevel/but01.bmp", "interface/PetLevel/ListBack_4.bmp");

	m_Combo1.DeleteAllItem(); // 이걸 꼭해줘야 리스토어시 등에 누적으로 안들어간다.
	m_Combo1.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_STUDY : 교육
	m_Combo1.InsertItem( (char*)G_STRING(IDS_STUDY),13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_EVOLUTION : 진화
	m_Combo1.InsertItem( (char*)G_STRING(IDS_EVOLUTION),13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_SKILL : 스킬
	m_Combo1.InsertItem( (char*)G_STRING(IDS_SKILL),13, RGB(255, 210, 0), RGB(255,255,255) );
	// IDS_PET_SKILL_INIT : 스킬 초기화
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
	x = m_pSurfaceBack->Xpos; //전체화면에서 back의 x좌표
	y = m_pSurfaceBack->Ypos; //전체화면에서 back의 y좌표

	g_pDisplay->Blt(x, y, m_pSurfaceBack);

	if(g_MyPetStatus.mode>10)
	{
		// 레벨과 현재 경험치는 항상 출력.
		if( m_pLevelText )		m_pLevelText->SetString(g_MyPetStatus.level,TRUE);
		if( m_pExpText )		m_pExpText->SetString(g_MyPetStatus.exp,TRUE);

		__int64 NeedExp = 0, NeedLaim = 0;

		switch( m_Combo1.GetSelect() ) // 뭘하려냐에 따라 표시도 틀려진다.
		{
		case 1: // 교육.
			if( m_pNeedExpText )	m_pNeedExpText->SetString(g_MyPetStatus.need_exp,TRUE);
			if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString(g_MyPetStatus.need_money,TRUE);
			break;

		case 2: // 진화.
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

		case 3: // 스킬

			if( m_Combo2.GetSelect() != 0 )
			{
				if( g_MyPetStatus.m_Class > 0 )
					NeedExp = PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][2];

				if( g_MyPetStatus.m_Class > 0 )
					NeedLaim = PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][1];

				if( m_pNeedExpText )	m_pNeedExpText->SetString( NeedExp,TRUE );
				if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString( NeedLaim,TRUE );
			}
			else // 아직 선택된 스킬이 없으면...
			{
				if( m_pNeedExpText )	m_pNeedExpText->SetString( "-" );
				if( m_pNeedMoneyText )	m_pNeedMoneyText->SetString( "-" );
			}

			break;
		case 4: // 스킬 초기화
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

	if( m_pLevelText )		m_pLevelText->Draw(x+100,y+60,78);  //레벨
	if( m_pNeedExpText )	m_pNeedExpText->Draw(x+100,y+92,78);  //필요 경험치
	if( m_pExpText )		m_pExpText->Draw(x+100,y+123,78);  //획득 경험치
	if( m_pNeedMoneyText )	m_pNeedMoneyText->Draw(x+253,y+92,78);  //필요 금액
	if( m_pMoneyText )		m_pMoneyText->Draw(x+253,y+123,78);  //현재 보유금액.


	// by evilkiki...draw pet
	/// 펫을 그리시렵니까?
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
			m_Combo2.MsgProc(hWnd, msg, wParam, lParam); // 여기서 이걸 해주는 이유는 두 콤보박스에 우선순위(밑의 리턴때문에 다른 콤보박스가 무시될수있으므로.)때문이다. 예를 들어 select중 다른 데를 찍으면 select모드가 취소되야 함으로..

			// 이 녀석은 선택이 바뀌었으면 m_Combo2에 영향을 준다.
			if( m_Combo1.GetSelect() == 1 || // 교육
				m_Combo1.GetSelect() == 4 )  // 스킬 초기화
			{
				m_Combo2.DeleteAllItem(); // 전부 지우고.
				m_Combo2.SetSelect( 0 );
				m_Combo2.SetState( -1 ); // 비활성 시킨다.

				m_pBtn1->SetState( BTN_NORMAL ); // OK 버튼 활성
			}
			else if( m_Combo1.GetSelect() == 2 ) // 진화
			{
				m_Combo2.DeleteAllItem(); // 전부 지우고.
				m_Combo2.SetSelect( 0 );
				m_Combo2.SetState( 0 ); // 활성 시킨다.

				if( g_MyPetStatus.m_Class == 0 )
				{
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_DEFENCE_TYPE : 방어형
					m_Combo2.InsertItem( (char*)G_STRING(IDS_DEFENCE_TYPE),13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_ATTACK_TYPE : 공격형
					m_Combo2.InsertItem( (char*)G_STRING(IDS_ATTACK_TYPE),13, RGB(255, 210, 0), RGB(255,255,255) );
				}
				else
				{
					m_Combo2.SetState( -1 ); // 도로 비활성 시킨다.
				}

				if( g_MyPetStatus.m_Class == 0 || g_MyPetStatus.m_Class == 3 )
				{
					m_pBtn1->SetState( BTN_DISABLE ); // OK 버튼 디활성 초기화.
				}
				else
				{
					m_pBtn1->SetState( BTN_NORMAL );
				}
			}
			else if( m_Combo1.GetSelect() == 3 ) // 스킬
			{
				m_Combo2.DeleteAllItem(); // 전부 지우고.
				m_Combo2.SetSelect( 0 );
				m_Combo2.SetState( 0 ); // 활성 시킨다.

				// 자신 펫 타입따라 리스트가 달라진다.
				switch( g_MyPetStatus.m_Class )
				{
				case 1: // 방어형
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL4_BREATH : 브레스
					sprintf( tmp, "%s(20Lv)", (char*)G_STRING(IDS_PETSKILL4_BREATH) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL5_GUARD : 수호
					sprintf( tmp, "%s(40Lv)", (char*)G_STRING(IDS_PETSKILL5_GUARD) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL6_DEFENCE : 보호
					sprintf( tmp, "%s(80Lv)", (char*)G_STRING(IDS_PETSKILL6_DEFENCE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL7_LIFE : 생명
					sprintf( tmp, "%s(90Lv)", (char*)G_STRING(IDS_PETSKILL7_LIFE) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					break;
				case 2: // 공격형
					m_Combo2.InsertItem( "",13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL0_POISON : 포이즌
					sprintf( tmp, "%s(20Lv)", (char*)G_STRING(IDS_PETSKILL0_POISON) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL1_PARALYSIS : 마비
					sprintf( tmp, "%s(40Lv)", (char*)G_STRING(IDS_PETSKILL1_PARALYSIS) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL2_COLD : 콜드
					sprintf( tmp, "%s(80Lv)", (char*)G_STRING(IDS_PETSKILL2_COLD) );
					m_Combo2.InsertItem( tmp,13, RGB(255, 210, 0), RGB(255,255,255) );
					// IDS_PETSKILL3_BLACKHOLE : 블랙홀
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
					m_Combo2.SetState( -1 ); // 도로 비활성 시킨다.
					break;
				}

				m_pBtn1->SetState( BTN_DISABLE ); // OK 버튼 디활성 초기화.
			}

			// 1이 리턴되었으면 다른 프로시져 무시. (콤보박스는 리스트가 다른 컨트롤들의 위에 놓여질수있으므로 이 처리가 필요하다.)
			return 1;
		}
		if( m_Combo2.MsgProc(hWnd, msg, wParam, lParam) )
		{
			m_Combo1.MsgProc(hWnd, msg, wParam, lParam); // 여기서 이걸 해주는 이유는 두 콤보박스에 우선순위(밑의 리턴때문에 다른 콤보박스가 무시될수있으므로.)때문이다. 예를 들어 select중 다른 데를 찍으면 select모드가 취소되야 함으로..

			if( m_Combo2.GetSelect() != 0 ) // 무언가 선택되어 졌다면...
				m_pBtn1->SetState( BTN_NORMAL ); // OK 버튼 활성

			// 1이 리턴되었으면 다른 프로시져 무시. (콤보박스는 리스트가 다른 컨트롤들의 위에 놓여질수있으므로 이 처리가 필요하다.)
			return 1;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pBtn2 && m_pBtn2->GetState() != BTN_DISABLE)
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn2 && m_pBtn2->GetState() == BTN_ACTION)
		{
			//닫기
			m_pBtn2->SetState(BTN_NORMAL);
			CloseMenu();
			return 1;
		}

		if(m_pBtn1 && m_pBtn1->GetState() == BTN_ACTION) // 승인
		{
			if( m_Combo1.GetSelect() == 1 ) // 교육.
			{
				//교육 메시지.

				if(g_MyPetStatus.mode>10)
				{
					if(m_money < g_MyPetStatus.need_money) //돈이 부족할때
					{
						//-- IDS_PETLEV_NOTENOUGH_MONEY : 돈이 부족하여 레벨업을 할수가 없습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_NOTENOUGH_MONEY), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if( g_MyPetStatus.level >= 100 ) // 레벨제한 추가 (05-10-28 원석)
					{
						//-- IDS_INVEN_REQUILE_MORE_LEVEL : 레벨이 맞지 않습니다
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), TYPE_NOR_OK, 1 );
						return 1;
					}
					else if(g_MyPetStatus.exp < g_MyPetStatus.need_exp)
					{
						//-- IDS_PETLEV_NOTENOUGH_LEVEL : 경험치가 부족하여 레벨업을 할수가 없습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_NOTENOUGH_LEVEL), TYPE_NOR_OK, 1 );
						return 1;
					}
					else
					{
						sprintf(tmp, "p_level up\n");
						if (g_pTcpIp)	g_pTcpIp->SendNetMessage(tmp);
						//-- IDS_PETLEV_COMPLETE : 성공적으로 레벨업이 되었습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PETLEV_COMPLETE), TYPE_NOR_OK, 1 );
						return 1;
					}
				}
				else if(g_MyPetStatus.mode==MYPET_MODE_NONE)
				{
					//-- IDS_PETLEV_PRE_BUY : 먼저 애완동물을 사셔야 합니다.
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
			else if( m_Combo1.GetSelect() == 2 ) // 진화
			{
				if( g_MyPetStatus.m_Class == 0 && m_Combo2.GetSelect() == 0 ) // 진화 종류가 설정되어있지않으면...
				{
					//-- IDS_CHOOSE_EVOL_TYPE : 진화 타입을 골라주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHOOSE_EVOL_TYPE), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_Class >= 3 )
				{
					//-- IDS_CANT_MORE_EVOL : 더 이상 진화할 수 없습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_MORE_EVOL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.level != 100 ) // 100레벨 라비용만 진화가 가능하다.
				{
					//-- IDS_INVEN_REQUILE_MORE_LEVEL : 레벨이 맞지 않습니다
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_Class == 0 && m_money < 10000000) // 현재 고정 1천만라임.
				{
					//-- IDS_EXCHANGE_SCARCE_MONEY : 금액이 부족합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( (g_MyPetStatus.m_Class == 1 || g_MyPetStatus.m_Class == 2) &&
						  m_money < 100000000)
				{
					//-- IDS_EXCHANGE_SCARCE_MONEY : 금액이 부족합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}

				if(g_MyPetStatus.m_Class == 0)
				{
					// 여의주 체크. 교호나창에 여의주 하나만 있어야한다.
					CItem* pItem = NULL;
					if( g_pRoh )
						pItem = g_pRoh->m_ExgInven;

					if( !pItem || pItem->m_Vnum != 1454 || pItem->m_Next ) // 여의주가 아니면..
					{
						//-- IDS_NEED_ONE_DRAGONBALL : 교환창에 여의주 아이템 하나만 놓아주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEED_ONE_DRAGONBALL), TYPE_NOR_OK, 1 );

						return 1;
					}

					if( m_Combo2.GetSelect() == 1 ) // 방어형.
						//-- IDS_DEFENCE_TYPE : 방어형
						sprintf( tmp2, (char*)G_STRING(IDS_DEFENCE_TYPE) );
					else if( m_Combo2.GetSelect() == 2 ) // 공격형.
						//-- IDS_ATTACK_TYPE : 공격형
						sprintf( tmp2, (char*)G_STRING(IDS_ATTACK_TYPE) );

					//-- IDS_WANT_PET_EVOL : 라비용을 %s으로 진화시키시겠습니까?
					sprintf(tmp, (char*)G_STRING(IDS_WANT_PET_EVOL), tmp2 );
					g_pNk2DFrame->InsertPopup( tmp, TYPE_NOR_OKCANCLE, 31 );
				}
				else
				{
					// 신석
					int cnt = g_pRoh->GetCountInExch(1740);
					if( cnt < 1 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEWPET_MSG0), TYPE_NOR_OK, 1 );
						return 1;
					}
					// 빛나는 신석
					cnt = g_pRoh->GetCountInExch(10021);
					if( cnt < 10 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEWPET_MSG0), TYPE_NOR_OK, 1 );
						return 1;
					}
					// 오염된 사막 상자
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
			else if( m_Combo1.GetSelect() == 3 ) // 스킬
			{
				if( m_Combo2.GetSelect() == 0 ) // 스킬 종류가 설정되어있지않으면...
				{
					//-- IDS_CHOOSE_SKILL : 스킬을 골라주세요.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHOOSE_SKILL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_Class == 0 ) // 기본 형은 안됨
				{
					//-- IDS_ITCAN_EVOLPET : 진화된 라비용만 가능합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITCAN_EVOLPET), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.level < PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][0] ) // 필요 레벨 체크.
				{
					//-- IDS_INVEN_REQUILE_MORE_LEVEL : 레벨이 맞지 않습니다
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( m_money < PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][1] ) //돈이 부족할때
				{
					//-- IDS_EXCHANGE_SCARCE_MONEY : 금액이 부족합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EXCHANGE_SCARCE_MONEY), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.exp < PetSkillNeed[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1][2] ) // 경험치가 부족할때...
				{
					//-- IDS_MORE_NEED_EXP : 경험치가 부족합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_MORE_NEED_EXP), TYPE_NOR_OK, 1 );
					return 1;
				}
				else if( g_MyPetStatus.m_SkillLevel[m_Combo2.GetSelect()-1] ) // 이미 배운 스킬 체크.
				{
					//-- IDS_ALREADY_STUDY_SKILL : 이미 배운 스킬입니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ALREADY_STUDY_SKILL), TYPE_NOR_OK, 1 );
					return 1;
				}

				if( g_MyPetStatus.m_Class == 1 ) // 방어형
				{	
					GetPetSkillName( (m_Combo2.GetSelect()-1)+PET_SKILL_MAX , tmp2 );					
				}
				else if( g_MyPetStatus.m_Class == 2 ) // 공격형
				{	
					GetPetSkillName( (m_Combo2.GetSelect()-1) , tmp2 ) ;					
				}
				else if( g_MyPetStatus.m_Class == 3 ) // 공격형
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

				//-- IDS_WANT_STUDY_PETSKILL :  "%s" 스킬을 배우시겠습니까?
				sprintf(tmp, (char*)G_STRING(IDS_WANT_STUDY_PETSKILL), tmp2 );
				g_pNk2DFrame->InsertPopup( tmp, TYPE_NOR_OKCANCLE, 31 );

				return 1;
			}
			else if( m_Combo1.GetSelect() == 4 ) // 스킬 초기화
			{
				if( g_MyPetStatus.m_Class < 3 )
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_PET_SKILL_INIT_CONFIRM), TYPE_NOR_OKCANCLE, POPUP_PET_UPGRADE);
				else
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_NEWPET_SKILL_INIT_CONFIRM), TYPE_NOR_OKCANCLE, POPUP_PET_UPGRADE);

				return 1;
			}

			return 1;
		}

		if( msg == WM_LBUTTONDOWN && IsInside(x, y) ) // 인터페이스 안쪽 빈공간 클릭시엔 땅클릭은 안보내도록..
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
		if( m_Combo1.GetSelect() == 2 ) // 진화
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
		else if( m_Combo1.GetSelect() == 3 ) // 스킬
		{
			sprintf(tmp, "p_skillup %d\n", g_PetSkillGroup[g_MyPetStatus.m_Class-1][m_Combo2.GetSelect()-1] );
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(tmp);
		}
		else if( m_Combo1.GetSelect() == 4 ) // 스킬 초기화
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
