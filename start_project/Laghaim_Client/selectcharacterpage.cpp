#include "stdafx.h"
#define D3D_OVERLOADS
#include <ddraw.h>
#include <d3d.h>
#include <direct.h>//1115
#include "SelectCharacterPage.h"
#include "headers.h"
#include "Sheet.h"
#include "createcharacterpage.h"

#include "ProgressBox.h"
#include "TextOutBox.h"
#include "Tcpipcon.h"
#include "main.h"
#include "direct.h"
#include "skill.h"
#include "FxSet.h"
#include "DirtSnd.h"
#include "Loginpage.h"
#include "Country.h"
#include "g_stringmanager.h"
#include "WebWorld.h"


#define	GOSTART		1
#define GOCREATE	2
#define DELETECHAR	3
#define GOLOGIN		4
#define STARTOBJECT_XPOS	49
#define STARTOBJECT_YPOS	114
#define OBJECT_WIDTH		108 // 케릭터가 그려햨E창의 넓이 / 케릭터 갯펯E
#define OBJECT_HEIGHT		155 // 

#define BACKGROUND_SETCHARACTERPAGE		"interface/SelectChar/back_setcharacterpage.bmp"
#define BACK_BACK						"interface/SelectChar/backback.bmp"  // 빈소켓에 구멍이 나는것때문에 땜빵이미지를 넣었다
#define EMPTY_VITAL						"interface/SelectChar/empty_vital.bmp"
#define EMPTY_MANA						"interface/SelectChar/empty_mana.bmp"
#define EMPTY_STAMINA					"interface/SelectChar/empty_stamina.bmp"
#define EMPTY_EPOWER					"interface/SelectChar/empty_epower.bmp"
#define NAME_ACTIVE_IMG					"interface/SelectChar/nameactive.bmp"
#define NAME_DEFAULT_IMG				"interface/SelectChar/namestic.bmp"

#define LEVEL_TEXT_COLOR	RGB(255, 255, 255)
#define LEVEL_TEXT_SIZE     14

extern BOOL		g_bTestServer;
extern BOOL		g_bFinishFxCharWave;
extern BOOL		g_MatrixSvr;
extern BOOL     g_bMatrixMatch;
extern DWORD	g_dwClientCountry;
extern DWORD	g_dwLangType;

//-------------------------------------------------------------------------------------------
CSelectCharacterPage::CSelectCharacterPage()
	: m_pBatangSur(NULL)
	, m_pNameActive(NULL)
	, m_pNameDefault(NULL)
	, m_pBtnBack(NULL)
	, m_bStartAni(FALSE)
	, m_dwStartTime(0)
{
	memset(m_pTmpStic, 0, sizeof(m_pTmpStic));

	m_pSheet = NULL;
	m_DownSlotNum = -1;
	m_pTitleBack = NULL;

	Init();


}

CSelectCharacterPage::CSelectCharacterPage( CSheet* pSheet )
	: m_pBatangSur(NULL)
	, m_pNameActive(NULL)
	, m_pNameDefault(NULL)
	, m_pBtnBack(NULL)
	, m_bStartAni(FALSE)
	, m_dwStartTime(0)
{
	memset(m_pTmpStic, 0, sizeof(m_pTmpStic));

	m_pSheet = NULL;
	SetSheetPtr( pSheet );
	m_DownSlotNum = -1;
	m_pTitleBack = NULL;

	m_pFlatRateText = NULL;  // 계정만료일 출력퓖E
	m_pMeterRateText = NULL;  // 계정만료일 출력퓖E


	Init();

	m_pSheet->m_bCreatedCha = FALSE;
}

CSelectCharacterPage::~CSelectCharacterPage()
{
	DeleteRes();
}


void CSelectCharacterPage::SetSheetPtr( CSheet* pSheet )
{
	if( !pSheet )
		return;

	m_pSheet = pSheet;
}


void CSelectCharacterPage::Init()
{
	m_StartBtn.SetFileName("SelectChar/sel_start");
	m_StartBtn.SetDisable( TRUE );

	m_NewBtn.SetFileName("SelectChar/sel_newcharacter");
	m_NewBtn.SetDisable( TRUE );
	
	m_DeleteBtn.SetFileName("SelectChar/sel_delete");
	m_DeleteBtn.SetDisable( TRUE );

	m_BackBtn.SetFileName("SelectChar/sel_back");
	m_BackBtn.SetDisable( false );

	m_ExitBtn.SetFileName("SelectChar/sel_exit");
	m_ExitBtn.SetDisable(true);

	m_VitalBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
	m_VitalBox.SetVisible(FALSE);

	m_ManaBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
	m_ManaBox.SetVisible(FALSE);

	m_StaminaBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
	m_StaminaBox.SetVisible(FALSE);

	m_EpowerBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
	m_EpowerBox.SetVisible(FALSE);

	m_StrBox.Init(SELCHAR_STAT_FSIZE, SELCHAR_STAT_FCOLOR, 460, 295 );
	m_StrBox.SetVisible(FALSE);

	m_IntBox.Init(SELCHAR_STAT_FSIZE, SELCHAR_STAT_FCOLOR, 460, 317 );
	m_IntBox.SetVisible(FALSE);

	m_DexBox.Init(SELCHAR_STAT_FSIZE, SELCHAR_STAT_FCOLOR, 460, 338 );
	m_DexBox.SetVisible(FALSE);

	m_ConBox.Init(SELCHAR_STAT_FSIZE, SELCHAR_STAT_FCOLOR, 460, 359 );
	m_ConBox.SetVisible(FALSE);

	m_ChaBox.Init(SELCHAR_STAT_FSIZE, SELCHAR_STAT_FCOLOR, 460, 380 );
	m_ChaBox.SetVisible(FALSE);



	m_Level[0].Init(LEVEL_TEXT_SIZE, LEVEL_TEXT_COLOR, 130, 204);
	m_Level[1].Init(LEVEL_TEXT_SIZE, LEVEL_TEXT_COLOR, 230, 204);
	m_Level[2].Init(LEVEL_TEXT_SIZE, LEVEL_TEXT_COLOR, 330, 204);
	m_Level[3].Init(LEVEL_TEXT_SIZE, LEVEL_TEXT_COLOR, 430, 204);
	m_Level[4].Init(LEVEL_TEXT_SIZE, LEVEL_TEXT_COLOR, 530, 204);


	for (int i = 0; i < SLOT_COUNT; i++)
	{
		m_SlotName[i].Init(SLOT_NAME_FSIZE, SLOT_NAME_FCOLOR,SLOT_NAME_X + (SLOT_GAP*i), -1);
		m_SlotName[i].SetVisible(FALSE);
	}


	m_MsgPopUp.Init(TYPE_DEL);
	m_MsgPopUpNew.Init(TYPE_NOR_OK);

	m_bNullSlot = FALSE;

	ResetAccountEnd(); // 계정만료일 표시 기능 초기화.
}

void CSelectCharacterPage::SetBackground()
{
}

void CSelectCharacterPage::LoadRes()
{
	if( !g_pDisplay )
		return;

	SetBackground();

	m_StartBtn.LoadRes(  );
	m_NewBtn.LoadRes(  );
	m_DeleteBtn.LoadRes(  );
	m_BackBtn.LoadRes(  );
	m_ExitBtn.LoadRes(  );
	m_VitalBox.LoadRes	(178, 295,  127, 18, PRO_BACK_IMG, PRO_VITAL_IMG);
	m_ManaBox.LoadRes	(178, 325,  127, 18, PRO_BACK_IMG, PRO_MANA_IMG);
	m_StaminaBox.LoadRes(178, 354,  127, 18, PRO_BACK_IMG, PRO_STAMINA_IMG);
	m_EpowerBox.LoadRes	(178, 381,  127, 18, PRO_BACK_IMG, PRO_EPOWER_IMG);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pNameDefault, NAME_DEFAULT_IMG);
	m_pNameDefault->SetColorKey(RGB(0, 0, 0));

	g_pDisplay->CreateSurfaceFromBitmap(&m_pTitleBack, "interface/SelectChar/sel_char_title.bmp");
	m_pTitleBack->SetColorKey(RGB(0, 0, 0));

	ChangeSlot(m_pSheet->m_SlotNum);

	m_MsgPopUp.LoadRes();
	m_MsgPopUpNew.LoadRes();

	SetCharNames();
}


void CSelectCharacterPage::DeleteRes()
{
	m_StartBtn.DeleteRes();
	m_NewBtn.DeleteRes();
	m_DeleteBtn.DeleteRes();
	m_BackBtn.DeleteRes();
	m_ExitBtn.DeleteRes();

	m_VitalBox.DeleteRes();
	m_ManaBox.DeleteRes();
	m_StaminaBox.DeleteRes();
	m_EpowerBox.DeleteRes();

	m_StrBox.DeleteRes();
	m_IntBox.DeleteRes();
	m_DexBox.DeleteRes();
	m_ConBox.DeleteRes();
	m_ChaBox.DeleteRes();

	for(int k=0; k<5; k++)
	{
		m_Level[k].DeleteRes();
	}

	for (int i = 0; i < SLOT_COUNT; i++)
	{
		m_SlotName[i].DeleteRes();
	}

	m_MsgPopUp.DeleteRes();
	m_MsgPopUpNew.DeleteRes();

	if(m_pNameDefault)
		SAFE_DELETE(m_pNameDefault);

	SAFE_DELETE(m_pTitleBack);
}

void CSelectCharacterPage::SetCharNames()
{
	for (int i = 0; i < SLOT_COUNT; i++)
	{
		if (m_pSheet->m_Slot[i])
		{
			m_SlotName[i].SetString(m_pSheet->m_Slot[i]->m_Name);
			m_SlotName[i].SetVisible(TRUE);
		}
		else
		{
			m_SlotName[i].SetVisible(FALSE);
		}
	}
}

void CSelectCharacterPage::SetCharLevel()
{
	for (int i = 0; i < SLOT_COUNT; i++)
	{
		if (m_pSheet->m_Slot[i])
		{
			m_Level[i].SetString(m_pSheet->m_Slot[i]->m_Level);
			m_Level[i].SetVisible(TRUE);
		}
		else
		{
			m_Level[i].SetVisible(FALSE);
		}
	}
}

void CSelectCharacterPage::SetStartAni(BOOL bStart)
{
	m_bStartAni = bStart;
}

void CSelectCharacterPage::Draw()
{
	static int init = 0;
	int nChaNum = 0;

	if (m_bStartAni)
	{
		if (!init)
		{
			m_dwStartTime = timeGetTime();
			init = 1;
		}
		m_pSheet->m_vEye.z = m_pSheet->m_vEyeSelStart.z + ( (timeGetTime() - m_dwStartTime)/(float) 1500*(m_pSheet->m_vEyeSelEnd.z - m_pSheet->m_vEyeSelStart.z)) ;

		if (m_pSheet->m_vEye.z < m_pSheet->m_vEyeSelEnd.z)
		{
			m_pSheet->m_vEye.z = m_pSheet->m_vEyeSelEnd.z;

			init = 0;
			m_bStartAni = FALSE;

			//====================================
			// nate 2005 - 01
			// opening2
			// gate animation
			for (int i = 0; i < SLOT_COUNT; i++)   ///2007때 주석 품.
			{
				if (m_pSheet->m_Slot[i] != NULL)
				{
					m_pSheet->AnimateGate(i, SEL_GATE_DOWN_ING);
				}
			}
			//====================================
		}
	}

	m_pSheet->TestSceneRender2();

	if (m_bStartAni)
		return;

	for (int i = 0; i < SLOT_COUNT; i++)
	{
		if (m_pSheet->m_Slot[i] != NULL)
		{
			m_pSheet->SetBasicCha(m_pSheet->m_Slot[i]->m_Race, m_pSheet->m_Slot[i]->m_Sex, m_pSheet->m_Slot[i]->m_Hair);

			if (i == m_pSheet->m_SlotNum)
			{
				m_pSheet->RenderSlotCha(i, STARTOBJECT_XPOS+i*OBJECT_WIDTH, STARTOBJECT_YPOS, OBJECT_WIDTH, OBJECT_HEIGHT, TRUE, FALSE);
			}
			else
			{
				m_pSheet->RenderSlotCha(i, STARTOBJECT_XPOS+i*OBJECT_WIDTH, STARTOBJECT_YPOS, OBJECT_WIDTH, OBJECT_HEIGHT, FALSE, FALSE);
			}
		}
	}

	if (pCMyApp->m_pFxSet)
	{
		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			pCMyApp->m_pFxSet->Render();
			GET_D3DDEVICE()->EndScene();
		}
	}

	g_pDisplay->Blt(262, 0, m_pTitleBack, NULL);


	m_StartBtn.Draw( 246, 507 );
	m_NewBtn.Draw( 400, 507 );
	m_DeleteBtn.Draw(554, 507);
	m_BackBtn.Draw( 112, 507);

	if (m_pSheet->m_Slot[0] != NULL && !m_bStartAni)
		g_pDisplay->Blt(47, 57, m_pNameDefault, NULL);
	if (m_pSheet->m_Slot[1] != NULL && !m_bStartAni)
		g_pDisplay->Blt(199, 57, m_pNameDefault, NULL);
	if (m_pSheet->m_Slot[2] != NULL && !m_bStartAni)
		g_pDisplay->Blt(360, 57, m_pNameDefault, NULL);
	if (m_pSheet->m_Slot[3] != NULL && !m_bStartAni)
		g_pDisplay->Blt(518, 57, m_pNameDefault, NULL);
	if (m_pSheet->m_Slot[4] != NULL && !m_bStartAni)
		g_pDisplay->Blt(673, 57, m_pNameDefault, NULL);

	if (!m_bStartAni)
	{
		m_Level[0].Draw(47+6,  78, 73);  //가웝棄 정렬을 위해서..
		m_Level[1].Draw(199+6, 78, 73);
		m_Level[2].Draw(360+6, 78, 73);
		m_Level[3].Draw(518+6, 78, 73);
		m_Level[4].Draw(673+6, 78, 73);


		m_SlotName[0].Draw(47+6,  60, 73);  //가웝棄 정렬을 위해서..
		m_SlotName[1].Draw(199+6, 60, 73);
		m_SlotName[2].Draw(360+6, 60, 73);
		m_SlotName[3].Draw(518+6, 60, 73);
		m_SlotName[4].Draw(673+6, 60, 73);
	}

	m_MsgPopUpNew.Draw();
	m_MsgPopUp.Draw();


	if (timeGetTime() - m_pSheet->m_dwDummyBaseTime > 120000)
		m_pSheet->SendDummyMsg();

	Sleep(1);

	// 계정 만료일 표시기능.
	if( g_dwClientCountry == CTRY_KOR ) // 계정만료일은 일단 한국만 지퓖E
		DrawAccountEnd();
}

void CSelectCharacterPage::SetBoxesVisible(BOOL bVisible)
{
	m_VitalBox.SetVisible(bVisible);
	m_ManaBox.SetVisible(bVisible);
	m_StaminaBox.SetVisible(bVisible);
	m_EpowerBox.SetVisible(bVisible);

	m_StrBox.SetVisible(bVisible);
	m_IntBox.SetVisible(bVisible);
	m_DexBox.SetVisible(bVisible);
	m_ConBox.SetVisible(bVisible);
	m_ChaBox.SetVisible(bVisible);
}

int CSelectCharacterPage::GetSlotNum(int x, int y)
{

	///2007로딩변컖E
	if (x >= 43 && x <= 43+122 && y >= 129 && y <= 133+199)
		return 0;
	else if (x >= 196 && x <= 196+104 && y >= 133 && y <= 133+170)
		return 1;
	else if (x >= 357 && x <= 357+93 && y >= 133 && y <= 133+157)
		return 2;
	else if (x >= 516 && x <= 516+93 && y >= 133 && y <= 133+166)
		return 3;
	else if (x >= 654 && x <= 654+99 && y >= 130 && y <= 130+201)
		return 4;
	else
		return -1;
}

void CSelectCharacterPage::ChangeSlot(int slot_num)
{
	if (slot_num < 0 || slot_num >= SLOT_COUNT)
		return;

	m_pSheet->m_SlotNum = slot_num;

	if (m_pSheet->m_Slot[slot_num])
	{
		m_VitalBox.SetValues(m_pSheet->m_Slot[slot_num]->m_MaxVital,
							 m_pSheet->m_Slot[slot_num]->m_Vital, SELCHAR_VAL_COLOR);
		m_ManaBox.SetValues(m_pSheet->m_Slot[slot_num]->m_MaxMana,
							m_pSheet->m_Slot[slot_num]->m_Mana, SELCHAR_VAL_COLOR);
		m_StaminaBox.SetValues(m_pSheet->m_Slot[slot_num]->m_MaxStamina,
							   m_pSheet->m_Slot[slot_num]->m_Stamina, SELCHAR_VAL_COLOR);
		m_EpowerBox.SetValues(m_pSheet->m_Slot[slot_num]->m_MaxEpower,
							  m_pSheet->m_Slot[slot_num]->m_Epower, SELCHAR_VAL_COLOR);

		m_StrBox.SetString(m_pSheet->m_Slot[slot_num]->m_Str);
		m_IntBox.SetString(m_pSheet->m_Slot[slot_num]->m_Int);
		m_DexBox.SetString(m_pSheet->m_Slot[slot_num]->m_Dex);
		m_ConBox.SetString(m_pSheet->m_Slot[slot_num]->m_Con);
		m_ChaBox.SetString(m_pSheet->m_Slot[slot_num]->m_Cha);

		// 2007 부턴 뉴턿E스타트턿E다 보이게. ///2007로딩변컖E
		m_StartBtn.SetDisable( FALSE );
		m_StartBtn.SetState(BTN_NORMAL);///2007로딩변컖E

		m_NewBtn.SetDisable( TRUE );
		m_NewBtn.SetState(BTN_DISABLE);///2007로딩변컖E

		// nate 2004 - 11 : new race
		if( g_dwClientCountry == CTRY_CN )	// 중국은 삭제 제한을 없앴다. (by 원석)
		{
			m_DeleteBtn.SetDisable(FALSE);
			m_DeleteBtn.SetState(BTN_NORMAL);
		}
		else if( g_dwClientCountry == CTRY_TH && m_pSheet->m_Slot[ slot_num ]->m_Level >= 250 )	// 태국 예외처리 // 태국은 250레벨 콅EE삭제가능. 07-06-11
		{
			m_DeleteBtn.SetDisable(TRUE);
			m_DeleteBtn.SetState(BTN_DISABLE);
		}
		//250레벨 이상의 케릭 지퓖E갋버튼(본섭은 툈E지퓖E? 태섭은 지퓖E?해야한다.)
		else if( !g_bTestServer
				 && (( m_pSheet->m_Slot[ slot_num ]->m_Race == RACE_FREAK	&& m_pSheet->m_Slot[ slot_num ]->m_Level >= 250 )
					 || ( m_pSheet->m_Slot[ slot_num ]->m_Race != RACE_FREAK && m_pSheet->m_Slot[ slot_num ]->m_Level >= 200 )))
		{
			m_DeleteBtn.SetDisable(TRUE);
			m_DeleteBtn.SetState(BTN_DISABLE);
		}
		else
		{
			m_DeleteBtn.SetDisable( FALSE );
			m_DeleteBtn.SetState(BTN_NORMAL);
		}

		SetBoxesVisible(TRUE);

		m_SlotName[slot_num].SetString(m_pSheet->m_Slot[slot_num]->m_Name);
		m_SlotName[slot_num].SetVisible(TRUE);
	}
	else
	{
		m_VitalBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
		m_ManaBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
		m_StaminaBox.SetValues(1, 0, SELCHAR_VAL_COLOR);
		m_EpowerBox.SetValues(1, 0, SELCHAR_VAL_COLOR);

		if(m_pSheet->m_Slot[slot_num] != NULL)
		{
			m_StartBtn.SetDisable( TRUE );
			m_StartBtn.SetState(BTN_DISABLE);

			m_NewBtn.SetDisable( FALSE );
			m_NewBtn.SetState(BTN_NORMAL);
		}
		else
		{
			m_DeleteBtn.SetDisable( FALSE );
			m_DeleteBtn.SetVisible( TRUE );

			m_StartBtn.SetDisable( TRUE );
			m_StartBtn.SetState(BTN_DISABLE);

			m_NewBtn.SetDisable( FALSE );
			m_NewBtn.SetState(BTN_NORMAL);
		}

		SetBoxesVisible(FALSE);

		m_SlotName[slot_num].SetVisible(FALSE);
	}
}



void CSelectCharacterPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// 현제 메세지가 들엉輩는가??
	if(m_MsgPopUp.m_bActive == TRUE)
	{
		int ret = -1;

		// 캐릭터삭제 버튼이 눌렸을경퓖E메세지팝업창이 뜬다
		// 만푳E"OK" 버튼이 눌렸다툈E케릭터를 지웝芩
		if ((ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{
			if( g_dwLangType == LANG_KOR)
			{
				m_StartBtn.SetState(BTN_NORMAL);
				m_NewBtn.SetState(BTN_NORMAL);
				m_DeleteBtn.SetState(BTN_DISABLE);
				m_BackBtn.SetState(BTN_NORMAL);
				m_ExitBtn.SetState(BTN_NORMAL);
				m_bNullSlot = TRUE;

				char *msg;
				bool isFail = false;
				if (msg = m_pSheet->SendDelChar(isFail, m_MsgPopUp.GetCheckPW()))
				{
					if( !strncmp( msg, "not_success", 11) )
					{
						m_MsgPopUp.m_InputLine.ClearText();
						//-- IDS_SELCHA_FAULT_PASS : 패스워드가 틀립니다. 다시 입력하세퓖E
						m_MsgPopUp.PutString((char*)G_STRING(IDS_SELCHA_FAULT_PASS));
						m_MsgPopUp.m_InputLine.ClearText();
					}
				}
				else
				{
					m_pSheet->UpdateChars();
					ChangeSlot(m_pSheet->m_SlotNum);
				}


				m_MsgPopUp.m_InputLine.ClearText();
			}
			else
			{
				if(strcmp(m_pSheet->m_sUserPW, m_MsgPopUp.GetCheckPW()) == 0)
				{
					m_StartBtn.SetState(BTN_NORMAL);
					m_NewBtn.SetState(BTN_NORMAL);
					m_DeleteBtn.SetState(BTN_DISABLE);
					m_BackBtn.SetState(BTN_NORMAL);
					m_ExitBtn.SetState(BTN_NORMAL);
					m_bNullSlot = TRUE;

					char *msg;
					bool isFail = false;
					if (msg = m_pSheet->SendDelChar(isFail))
					{
					}
					else
					{
						m_pSheet->UpdateChars();
						ChangeSlot(m_pSheet->m_SlotNum);
					}
					m_MsgPopUp.m_InputLine.ClearText();
				}
				else
				{
					//-- IDS_SELCHA_FAULT_PASS : 패스워드가 틀립니다. 다시 입력하세퓖E
					m_MsgPopUp.PutString((char*)G_STRING(IDS_SELCHA_FAULT_PASS));
					m_MsgPopUp.m_InputLine.ClearText();
				}
			}
		}
		else if (ret == -1)
		{
			m_StartBtn.SetState(BTN_NORMAL);
			m_NewBtn.SetState(BTN_NORMAL);
			m_DeleteBtn.SetState(BTN_NORMAL);
			m_BackBtn.SetState(BTN_NORMAL);
			m_ExitBtn.SetState(BTN_NORMAL);
			m_MsgPopUp.m_InputLine.ClearText();
			m_bNullSlot = FALSE;
		}
		return ;
	}

	if(m_MsgPopUpNew.m_bActive == TRUE)
	{
		int ret = -1;
		if ((ret = m_MsgPopUpNew.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{
			if(m_NewBtn.GetState() == BTN_ACTION)
			{
				m_StartBtn.SetState(BTN_NORMAL);
				m_NewBtn.SetState(BTN_NORMAL);
				m_DeleteBtn.SetState(BTN_DISABLE);
				m_BackBtn.SetState(BTN_NORMAL);
				m_ExitBtn.SetState(BTN_NORMAL);
				m_bNullSlot = TRUE;
			}
		}
		else if (ret == -1)
		{
			m_StartBtn.SetState(BTN_NORMAL);
			m_NewBtn.SetState(BTN_NORMAL);
			m_DeleteBtn.SetState(BTN_NORMAL);
			m_BackBtn.SetState(BTN_NORMAL);
			m_ExitBtn.SetState(BTN_NORMAL);
			m_MsgPopUpNew.m_InputLine.ClearText();
		}
		return ;
	}


	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		m_DownSlotNum = GetSlotNum(LOWORD(lParam), HIWORD(lParam));

		// 마퓖E볜?틒E슬롯을 선택했을때 "삭제"버튼을 비활성화로 만든다
		if(m_DownSlotNum > -1)
		{
			if (m_pSheet->m_Slot[m_DownSlotNum] == NULL)
			{
				m_bNullSlot = TRUE;
				m_DeleteBtn.m_iBtnFlg = BTN_DISABLE;
				//====================================
				// nate 2005 - 01
				// opening2
				// 게이트 문을 조절한다. - ///2007로딩변컖E때 품
				m_pSheet->OpenNewGate(m_DownSlotNum);
				//====================================
			}
			else
			{
				m_StartBtn.m_iBtnFlg = BTN_NORMAL;

				m_DeleteBtn.m_iBtnFlg = BTN_NORMAL;
				m_bNullSlot = FALSE;

				// 틒E竄珦?결제안했으툈E35레벨 이퍊E캐릭은 선택 불가처리.(위의 버튼처리는 해줘야함에 유의.) ///틒E竄解甦?
				if( g_dwClientCountry == CTRY_BRA && m_pSheet && m_pSheet->m_Slot[m_DownSlotNum] ) // 틒E竄珦琯?결제를 안한 유저라툈E35레벨 이상의 캐릭터는 비활성 처리.
				{
					if( !m_pSheet->m_bPaidUser && m_pSheet->m_Slot[m_DownSlotNum]->m_Level >= 35 ) // 결제를 안한 유저의 35레벨 이퍊E캐릭터이툈E...
					{
						m_StartBtn.m_iBtnFlg = BTN_DISABLE; // 스타트 못하도록.
						m_bNullSlot = TRUE;
						return; // 이펙트 스킵
					}
				}

				EffectSort ef_sort;
				ef_sort.nType = FX_SKILL_LIGHT;
				int i = 0;
				for( i = 0; i < 15; i++ )
				{
					ef_sort.nCurFrame = -i;
					ef_sort.vPos = D3DVECTOR(m_pSheet->m_vSelCharPos.x + RandomNum(-5.0f, 5.0f), m_pSheet->m_vSelCharPos.y, m_pSheet->m_vSelCharPos.z + RandomNum(-5.0f, 5.0f));
					pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
				}
				for( i = 0; i < 15; i++ )
				{
					ef_sort.nCurFrame = -i;
					ef_sort.vPos = D3DVECTOR(m_pSheet->m_vSelCharPos.x + RandomNum(-5.0f, 5.0f), m_pSheet->m_vSelCharPos.y, m_pSheet->m_vSelCharPos.z + RandomNum(-5.0f, 5.0f));
					pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
				}
				if (g_pDSound)
				{
					g_pDSound->Play(g_SheetSound[SHEET_SND_APP]);
				}
				// 게이트 문을 조절한다. ///2007로딩변컖E
				m_pSheet->OpenNewGate(5); // 5를 주툈E캐릭터 업는건 모두 닫히게 해놨다.
			}
		}

	case WM_MOUSEMOVE:
		// 현제 선택된 케릭터가 없다툈E.메시지를 받햨E않겠다
		if(m_bNullSlot == FALSE)
		{
			m_StartBtn.MsgProc( hWnd, msg, wParam, lParam );
			m_DeleteBtn.MsgProc( hWnd, msg, wParam, lParam );
		}
		else
		{
			m_NewBtn.MsgProc( hWnd, msg, wParam, lParam );
		}
		m_BackBtn.MsgProc( hWnd, msg, wParam, lParam );

		break;

	case WM_LBUTTONUP:
		int slot_num;
		slot_num = GetSlotNum(LOWORD(lParam), HIWORD(lParam));
		if (slot_num >= 0 && slot_num == m_DownSlotNum)
		{
			// 틒E竄珦?결제안했으툈E35레벨 이퍊E캐릭은 선택 불가처리. ///틒E竄解甦?
			if( g_dwClientCountry == CTRY_BRA && m_pSheet && m_pSheet->m_Slot[slot_num] ) // 틒E竄珦琯?결제를 안한 유저라툈E35레벨 이상의 캐릭터는 비활성 처리.
			{
				if( !m_pSheet->m_bPaidUser && m_pSheet->m_Slot[slot_num]->m_Level >= 35 ) // 결제를 안한 유저의 35레벨 이퍊E캐릭터이툈E...
				{
					m_pSheet->m_GoHomePage.PutString( "Ap? o lvl 35, ?necess?io adquirir a conta Premium para continuar a usufruir do jogo, entre no site e saiba como" ); // 틒E竄갋로그인 경컖E메세햨E 예외적으로 메세지를 그냥 바로 넣는다. 다른 나라와 공퓖E막?쓰일일 없으니콅E.
					return; // 스킵처리
				}
			}

			ChangeSlot(slot_num);
		}

		if(m_bNullSlot == FALSE)
		{
			m_StartBtn.MsgProc( hWnd, msg, wParam, lParam );
			m_DeleteBtn.MsgProc( hWnd, msg, wParam, lParam );
		}
		m_NewBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_BackBtn.MsgProc( hWnd, msg, wParam, lParam );

		if ( m_StartBtn.GetState() == BTN_ACTION )
		{
			m_StartBtn.m_iBtnFlg = BTN_NORMAL;
			g_bFinishFxCharWave = TRUE;
			Action( GOSTART );
		}
		else if ( m_NewBtn.GetState() == BTN_ACTION )
		{
			m_NewBtn.m_iBtnFlg = BTN_NORMAL;
			if (m_pSheet->m_bEmptySloat == TRUE)
			{
				m_pSheet->m_SlotNum = 2;
			}

			if(g_MatrixSvr != 0)
			{
				char buf[256];
				//-- IDS_CANT_CRE_MATRIX : 매트릭스 서버에서는 캐릭터를 만들펯E없습니다
				sprintf(buf,(char*)G_STRING(IDS_CANT_CRE_MATRIX));
				if(g_bMatrixMatch)
					//-- IDS_CANT_CRE_GAME : 게임큱E?서버에서는 캐릭터를 만들펯E없습니다.
					sprintf(buf,(char*)G_STRING(IDS_CANT_CRE_GAME));

				m_MsgPopUpNew.PutString(buf);
			}
			else
				Action( GOCREATE );
		}
		else if ( m_DeleteBtn.GetState() == BTN_ACTION )
		{
			m_DeleteBtn.m_iBtnFlg = BTN_NORMAL;

			// 신섭일때 길드가 가입되푳E있다툈E먼픸E길탛E탈퇴를 유도한다.
			if(
				m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_GuilMa > 0 && // 0 : 미가입 혹은 가입 승인을 기다리는 상태.
				m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_GuildIndex > 0 ) // 0 : 미가입 길탛E
			{
				// IDS_GUILD_PLZ_MEMBER_OUT "먼픸E길드를 탈퇴하십시퓖E"
				m_MsgPopUpNew.PutString((char*)G_STRING(IDS_GUILD_PLZ_MEMBER_OUT));/// msg_kor

				break;
			}
			else if( m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_Support > 0 )
			{
				m_MsgPopUpNew.PutString((char*)G_STRING(IDS_PLZ_SUPPORT_OUT));

				break;
			}
			else if(g_MatrixSvr != 0 )
			{
				char buf[256];
				//-- IDS_CANT_DEL_MATRIX : 매트릭스 서버에서는 캐릭터를 지퓖E갋없습니다
				sprintf(buf,(char*)G_STRING(IDS_CANT_DEL_MATRIX));
				if(g_bMatrixMatch)
					//-- IDS_CANT_DEL_GAME : 게임큱E?서버에서는 캐릭터를 지퓖E갋없습니다.
					sprintf(buf,(char*)G_STRING(IDS_CANT_DEL_GAME));

				m_MsgPopUpNew.PutString(buf);
			}
			else
// 삭제버튼 클릭시 일본은 삭제 않되도록(웝팀에게 문의하라는 메시지
				Action( DELETECHAR );
		}
		else if ( m_BackBtn.GetState() == BTN_ACTION )
		{
			m_BackBtn.m_iBtnFlg = BTN_NORMAL;
			Action( GOLOGIN );
		}
		else if ( m_ExitBtn.GetState() == BTN_ACTION )
		{
			m_ExitBtn.m_iBtnFlg = BTN_NORMAL;
			g_pTcpIp->SetTerminate();
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	}
}


void CSelectCharacterPage::Action( int i )
{
	if (g_pDSound)
	{
		g_pDSound->Play(g_SheetSound[SHEET_SND_CLICK]);
	}
	switch ( i )
	{
	case GOSTART:
		m_pSheet->StartGame(START_GAME);
		strcpy(pCMyApp->m_SaveChaName, m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_Name);
//////////////
//1114
		pCMyApp->m_bSelectCha=TRUE;

		char path[1024];
		char *strPath;
		strPath = pCMyApp->GetExePath();
		_chdir(strPath);
		sprintf(path, "%s/lag_lcg/", strPath);


		FILE *stream;
		int list[30];
		int list2[30];
		char temp[50];
		char strTemp[128];//
		strcpy(strTemp, pCMyApp->m_SaveChaName);		//
		int count;

		count=0;

		while(strTemp[count]!='\0')
		{

			switch(strTemp[count])
			{
			case '\\':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"0",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '/':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"1",1);
				strcpy(strTemp+count+2,temp);
				break;
			case ':':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"2",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '*':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"3",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '?':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"4",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '"':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"5",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '<':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"6",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '>':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"7",1);
				strcpy(strTemp+count+2,temp);
				break;
			case '|':
				strncpy(strTemp+count,"'",1);
				strcpy(temp,&strTemp[count+1]);
				strncpy(strTemp+count+1,"8",1);
				strcpy(strTemp+count+2,temp);
				break;
			default:
				break;
			}
			count++;
		}
		strcat(strTemp, ".lcg");

///1115
		char tmp[800];
		sprintf(tmp, "%s%s", path, strTemp);

		if( (stream = fopen( tmp, "rb" )) != NULL )
		{
			fread( list, sizeof( int ), 8, stream );
			fread( list2, sizeof( int ), 8, stream );

			for (count = 0; count < FUNCTION_NUM; count++ )
			{
				g_pNk2DFrame->GetControlBottom()->m_QuickSkill[count].skill = list[count];
				g_pNk2DFrame->GetControlBottom()->m_QuickSkill[count].type = list2[count];
				g_pNk2DFrame->GetControlBottom()->m_FunctionKey[count].m_Type = list2[count];
			}
			fclose( stream );
		}
/////
////////////////
		return;

	case GOCREATE: // NEW btn click
		m_StartBtn.m_iBtnFlg = BTN_DISABLE;
		m_DeleteBtn.m_iBtnFlg = BTN_DISABLE;
		m_pSheet->GoPage( CRTCHARPAGE );
		m_pSheet->GoCreateChaPage();
		m_StartBtn.SetDisable( FALSE );
		m_NewBtn.SetDisable( TRUE );
		m_pSheet->m_pCrtCharPg->OutoSetValue(0);// 최초 불칸
		return ;

	case DELETECHAR:
		//-- IDS_SELCHA_CHA_DELETE : 캐릭터를 삭제합니다. 패스워드를 입력하세퓖E
		m_MsgPopUp.PutString((char*)G_STRING(IDS_SELCHA_CHA_DELETE));
		m_StartBtn.SetState(BTN_DISABLE);
		m_NewBtn.SetState(BTN_DISABLE);
		m_DeleteBtn.SetState(BTN_DISABLE);
		m_BackBtn.SetState(BTN_DISABLE);
		m_ExitBtn.SetState(BTN_DISABLE);
		break;

	case GOLOGIN:
		// 1. "뒤로" 버튼이 눌릴경퓖E기존의 서버연결을 끊는다
		// 2. "뒤로" 버튼이 눌리툈E기존에 선택된 케릭터가 해제되컖E아무것도 선택하햨E않은 상태로 만든다
		// 3. "뒤로" 버튼이 눌리기픸E"새케릭터"버튼이 활성화가 된상태라툈E"새케릭터"버튼을 비활성화 시키컖E"시작"버튼을 활성화시킨다
		// 4. "뒤로" 버튼이 눌리툈E선택된 케릭터의 "상세정보"를 보여주햨E않는다
		// 5. "new" / "delete" 버튼을 그레이 처리상태로 만든다
		// 6. 현제눌려햨E슬롯을 "-1"로 맞춘다

		if(m_NewBtn.m_bDisable == FALSE)
		{
			m_StartBtn.SetDisable( FALSE );
			m_NewBtn.SetDisable( TRUE );
		}
		m_StartBtn.m_iBtnFlg = BTN_DISABLE;
		m_DeleteBtn.m_iBtnFlg = BTN_DISABLE;

		SetBoxesVisible(FALSE);

		m_bNullSlot = TRUE;
		m_DownSlotNum = -1;
		m_pSheet->m_SlotNum = -1;
		g_pTcpIp->SetTerminate();


		// 옷 벗기콅E..
		for(int j=0; j<5; j++)
		{
			if(m_pSheet->m_Slot[j] != NULL)
			{
				for (i = 0; i < WEARING_SHOW_NUM; i++)
				{
					if (m_pSheet->m_Slot[j]->m_Wearing[i] >= 0)
					{
						m_pSheet->m_Slot[j]->m_Wearing[i] = -1;
						//m_pSheet->m_pSlotCha[m_DownSlotNum]->Wear(i, -1, 0);
					}
				}
			}
		}

		// 매트릭스 서버로의 접속이 있을 펯E있으므로 플래그를 FALSE로 놓는다.
		m_pSheet->m_pLoginPg->m_bShowMatrixList = FALSE;

		m_pSheet->GoPage( LOGINPAGE );
		return ;

	}
}

void CSelectCharacterPage::ResetAccountEnd() // 계정만료일 표시 기능 초기화.
{
	m_UserIndex = 0; // 계정 만료일을 알콅E위해 필요한 유픸E인덱스.
	ZeroMemory( &m_FlatRateDay, sizeof(SYSTEMTIME) ); // 정액 요금의 퀋E퓖E일을 저장하콅E위한 녀석
	ZeroMemory( m_strFlatRateTime, sizeof(m_strFlatRateTime) ); // 정액 요금의 퀋E퓖E일을 저장하콅E위한 녀석
	m_MeterRateTime = 0; // 종량 요금제의 남은 분.
	SAFE_DELETE( m_pFlatRateText ); // 출력 퓖E
	SAFE_DELETE( m_pMeterRateText ); // 출력 퓖E
}

void CSelectCharacterPage::GetAccountEndString( int UserIndex ) // 웹 페이지로 부터 받아온 스트링을 계정 만료일 정보로 만든다.
{
	m_UserIndex = UserIndex;

	CWebWorld	webworld;

	char address[256] = "";
	char temp[256] = "";

	sprintf(address, "http://payment.laghaim.com/ForGame/getUserGameTimeInfo.asp?");
	sprintf(temp, "userId=%d", (int)m_UserIndex);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		if( webworld.m_UrlData )
			SetAccountEndInfo( webworld.m_UrlData );
	}
}

void CSelectCharacterPage::SetAccountEndInfo( char *strAccountEnd ) // 웹 페이지로 부터 받아온 스트링을 계정 만료일 정보로 만든다.
{
	ResetAccountEnd(); // 초기화.

	int i = 0;

	char strTemp1[256];
	char strTemp2[256];
	char msg[256];

	int OriginalStringLen = 0;
	int SectionPoint = -1; // 갈라지는 표시인 "||"의 위치.

	if( !strAccountEnd ) // 잘툈E들엉邦툈E..
		return;

	OriginalStringLen = strlen(strAccountEnd);

	if( OriginalStringLen == 0 || OriginalStringLen <= 2 ) // 내퓖E?없거나 0 이거나 결제 정보가 없으툈E"||"만 있으툈E
		return;

	// 문자열을 "||"을 기준으로 자른다.
	for( i = 0 ; i < OriginalStringLen-2 ; ++i ) // 문자열 길이만큼 돌면서...
	{
		if( strncmp(&strAccountEnd[i],"||",2) == 0 ) // 구분 점이 발견되었으툈E..
			SectionPoint = i; // 구분점을 기푳E巒畇?
	}

	if( SectionPoint == -1 ) // 구분점이 업으툈E..
		return;

	if( SectionPoint != 0 ) // OriginalStringLen이 0인 경퓖E?정량제는 이퓖E舊갋않은 경퓖E?
	{
		// 정량제 부분 스트링 셋팅.
		strncpy( strTemp1, strAccountEnd, SectionPoint );
		strTemp1[SectionPoint] = NULL;

		char temp1[10];
		char temp2[10];
		char temp3[10];

		sscanf( strTemp1, "%s %s %s %s", temp1, temp2, temp3, m_strFlatRateTime ); // 구분지엉幕 읽엉龐다.

		// 스트링으로 끊엉幕 변환해야한다. 07일 <- 이런경퓖E%d로는 제큱E?못읽엉邦더턿E
		m_FlatRateDay.wMonth = atoi(temp1);
		m_FlatRateDay.wDay = atoi(temp2);
		m_FlatRateDay.wYear = atoi(temp3);
	}

	if( SectionPoint+2 <= OriginalStringLen ) // 이 경퓖E?종량제를 이퓖E構갋있는 경퓖E
	{
		// 종량제 부분 스트링 셋팅
		strncpy( strTemp2, &strAccountEnd[SectionPoint+2], OriginalStringLen-SectionPoint+2 );
		strTemp2[OriginalStringLen-SectionPoint+2] = NULL;

		m_MeterRateTime = atoi( strTemp2 ); // 종량제 시간 셋팅.
	}

	if( m_FlatRateDay.wMonth ) // 이게 0이 아니툈E정량제를 쓰과復는 거다.
	{
		// 5일 이하일때만 표시
		SYSTEMTIME NowTime;
		GetSystemTime( &NowTime );
//		if( NowTime.wYear == m_FlatRateDay.wYear && NowTime.wMonth == m_FlatRateDay.wMonth && NowTime.wDay >= m_FlatRateDay.wDay-5 )
		{
			int font_size = 14;

			sprintf( msg, G_STRING(IDS_LHSTRING1779), m_FlatRateDay.wYear, m_FlatRateDay.wMonth, m_FlatRateDay.wDay, m_strFlatRateTime );

			int len;
			if ((len = strlen(msg)) <= 0)
				return;

			m_pFlatRateText = new CTextOutBox;
			if( m_pFlatRateText )
			{
				m_pFlatRateText->Init(font_size, RGB(0, 200, 0), 5, 7); // 이 상태에선 가로 사이짊瞼 항퍊E800이다.
				m_pFlatRateText->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(220, 100, 135) ); // 인자 : 글, 글자퍊E 외곽선 퍊E
			}
		}
	}

	if( m_MeterRateTime && m_MeterRateTime < 60 ) // 이게 0이 아니툈E종제를 쓰과復는 거다.(한시간 이하로 남았을 때만 표시)
	{
		int font_size = 14;

		sprintf( msg, G_STRING(IDS_LHSTRING1780), m_MeterRateTime );

		int len;
		if ((len = strlen(msg)) <= 0)
			return;

		m_pMeterRateText = new CTextOutBox;
		if( m_pMeterRateText )
		{
			m_pMeterRateText->Init(font_size, RGB(0, 200, 0), 5, 27 ); // 이 상태에선 가로 사이짊瞼 항퍊E800이다.
			m_pMeterRateText->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(220, 100, 135) ); // 인자 : 글, 글자퍊E 외곽선 퍊E
		}
	}
}

void CSelectCharacterPage::DrawAccountEnd() // 만료일 출력.
{
//	if( (timeGetTime()%700) > 600 ) // 깜빡이콅E처리...
//		return;

	if( m_pFlatRateText && m_pMeterRateText ) // 정량제 스트링컖E종량제 스트링이 둘다 있는 경퓖E
	{
		m_pFlatRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
		m_pMeterRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
	else if( m_pFlatRateText ) // 정량제만 있는 경퓖E
	{
		m_pFlatRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
	else if( m_pMeterRateText ) // 종량제만 잇는 경퓖E
	{
		m_pMeterRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
}
