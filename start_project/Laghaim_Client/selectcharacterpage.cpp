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
#define OBJECT_WIDTH		108 // �ɸ��Ͱ� �׷���Eâ�� ���� / �ɸ��� ����E
#define OBJECT_HEIGHT		155 // 

#define BACKGROUND_SETCHARACTERPAGE		"interface/SelectChar/back_setcharacterpage.bmp"
#define BACK_BACK						"interface/SelectChar/backback.bmp"  // ����Ͽ� ������ ���°Ͷ����� �����̹����� �־���
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

	m_pFlatRateText = NULL;  // ���������� ��¿�E
	m_pMeterRateText = NULL;  // ���������� ��¿�E


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

	ResetAccountEnd(); // ���������� ǥ�� ��� �ʱ�ȭ.
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
			for (int i = 0; i < SLOT_COUNT; i++)   ///2007�� �ּ� ǰ.
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
		m_Level[0].Draw(47+6,  78, 73);  //����ѥ ������ ���ؼ�..
		m_Level[1].Draw(199+6, 78, 73);
		m_Level[2].Draw(360+6, 78, 73);
		m_Level[3].Draw(518+6, 78, 73);
		m_Level[4].Draw(673+6, 78, 73);


		m_SlotName[0].Draw(47+6,  60, 73);  //����ѥ ������ ���ؼ�..
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

	// ���� ������ ǥ�ñ��.
	if( g_dwClientCountry == CTRY_KOR ) // ������������ �ϴ� �ѱ��� ����E
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

	///2007�ε�����E
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

		// 2007 ���� ����E��ŸƮ��E�� ���̰�. ///2007�ε�����E
		m_StartBtn.SetDisable( FALSE );
		m_StartBtn.SetState(BTN_NORMAL);///2007�ε�����E

		m_NewBtn.SetDisable( TRUE );
		m_NewBtn.SetState(BTN_DISABLE);///2007�ε�����E

		// nate 2004 - 11 : new race
		if( g_dwClientCountry == CTRY_CN )	// �߱��� ���� ������ ���ݴ�. (by ����)
		{
			m_DeleteBtn.SetDisable(FALSE);
			m_DeleteBtn.SetState(BTN_NORMAL);
		}
		else if( g_dwClientCountry == CTRY_TH && m_pSheet->m_Slot[ slot_num ]->m_Level >= 250 )	// �±� ����ó�� // �±��� 250���� ��EE��������. 07-06-11
		{
			m_DeleteBtn.SetDisable(TRUE);
			m_DeleteBtn.SetState(BTN_DISABLE);
		}
		//250���� �̻��� �ɸ� ����E�E��ư(������ ��E����E? �¼��� ����E?�ؾ��Ѵ�.)
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
	// ���� �޼����� ����Դ°�??
	if(m_MsgPopUp.m_bActive == TRUE)
	{
		int ret = -1;

		// ĳ���ͻ��� ��ư�� �������濁E�޼����˾�â�� ���
		// ����E"OK" ��ư�� ���ȴٸ�E�ɸ��͸� ������
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
						//-- IDS_SELCHA_FAULT_PASS : �н����尡 Ʋ���ϴ�. �ٽ� �Է��ϼ���E
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
					//-- IDS_SELCHA_FAULT_PASS : �н����尡 Ʋ���ϴ�. �ٽ� �Է��ϼ���E
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

		// ����E��?��E������ ���������� "����"��ư�� ��Ȱ��ȭ�� �����
		if(m_DownSlotNum > -1)
		{
			if (m_pSheet->m_Slot[m_DownSlotNum] == NULL)
			{
				m_bNullSlot = TRUE;
				m_DeleteBtn.m_iBtnFlg = BTN_DISABLE;
				//====================================
				// nate 2005 - 01
				// opening2
				// ����Ʈ ���� �����Ѵ�. - ///2007�ε�����E�� ǰ
				m_pSheet->OpenNewGate(m_DownSlotNum);
				//====================================
			}
			else
			{
				m_StartBtn.m_iBtnFlg = BTN_NORMAL;

				m_DeleteBtn.m_iBtnFlg = BTN_NORMAL;
				m_bNullSlot = FALSE;

				// ��E����?������������E35���� �̻�Eĳ���� ���� �Ұ�ó��.(���� ��ưó���� ������Կ� ����.) ///��E������?
				if( g_dwClientCountry == CTRY_BRA && m_pSheet && m_pSheet->m_Slot[m_DownSlotNum] ) // ��E����ε?������ ���� ������E35���� �̻��� ĳ���ʹ� ��Ȱ�� ó��.
				{
					if( !m_pSheet->m_bPaidUser && m_pSheet->m_Slot[m_DownSlotNum]->m_Level >= 35 ) // ������ ���� ������ 35���� �̻�Eĳ�����̸�E...
					{
						m_StartBtn.m_iBtnFlg = BTN_DISABLE; // ��ŸƮ ���ϵ���.
						m_bNullSlot = TRUE;
						return; // ����Ʈ ��ŵ
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
				// ����Ʈ ���� �����Ѵ�. ///2007�ε�����E
				m_pSheet->OpenNewGate(5); // 5�� �ָ�Eĳ���� ���°� ��� ������ �س���.
			}
		}

	case WM_MOUSEMOVE:
		// ���� ���õ� �ɸ��Ͱ� ���ٸ�E.�޽����� ����E�ʰڴ�
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
			// ��E����?������������E35���� �̻�Eĳ���� ���� �Ұ�ó��. ///��E������?
			if( g_dwClientCountry == CTRY_BRA && m_pSheet && m_pSheet->m_Slot[slot_num] ) // ��E����ε?������ ���� ������E35���� �̻��� ĳ���ʹ� ��Ȱ�� ó��.
			{
				if( !m_pSheet->m_bPaidUser && m_pSheet->m_Slot[slot_num]->m_Level >= 35 ) // ������ ���� ������ 35���� �̻�Eĳ�����̸�E...
				{
					m_pSheet->m_GoHomePage.PutString( "Ap? o lvl 35, ?necess?io adquirir a conta Premium para continuar a usufruir do jogo, entre no site e saiba como" ); // ��E���E�α��� �氁E�޼���E ���������� �޼����� �׳� �ٷ� �ִ´�. �ٸ� ����� ����E��?������ �����ϱ�E.
					return; // ��ŵó��
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
				//-- IDS_CANT_CRE_MATRIX : ��Ʈ���� ���������� ĳ���͸� ���鼁E�����ϴ�
				sprintf(buf,(char*)G_STRING(IDS_CANT_CRE_MATRIX));
				if(g_bMatrixMatch)
					//-- IDS_CANT_CRE_GAME : ���Ӵ�E?���������� ĳ���͸� ���鼁E�����ϴ�.
					sprintf(buf,(char*)G_STRING(IDS_CANT_CRE_GAME));

				m_MsgPopUpNew.PutString(buf);
			}
			else
				Action( GOCREATE );
		}
		else if ( m_DeleteBtn.GetState() == BTN_ACTION )
		{
			m_DeleteBtn.m_iBtnFlg = BTN_NORMAL;

			// �ż��϶� ��尡 ���ԵǾ�E�ִٸ�E����E�流EŻ�� �����Ѵ�.
			if(
				m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_GuilMa > 0 && // 0 : �̰��� Ȥ�� ���� ������ ��ٸ��� ����.
				m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_GuildIndex > 0 ) // 0 : �̰��� �流E
			{
				// IDS_GUILD_PLZ_MEMBER_OUT "����E��带 Ż���Ͻʽÿ�E"
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
				//-- IDS_CANT_DEL_MATRIX : ��Ʈ���� ���������� ĳ���͸� ����E�E�����ϴ�
				sprintf(buf,(char*)G_STRING(IDS_CANT_DEL_MATRIX));
				if(g_bMatrixMatch)
					//-- IDS_CANT_DEL_GAME : ���Ӵ�E?���������� ĳ���͸� ����E�E�����ϴ�.
					sprintf(buf,(char*)G_STRING(IDS_CANT_DEL_GAME));

				m_MsgPopUpNew.PutString(buf);
			}
			else
// ������ư Ŭ���� �Ϻ��� ���� �ʵǵ���(�������� �����϶�� �޽���
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
		m_pSheet->m_pCrtCharPg->OutoSetValue(0);// ���� ��ĭ
		return ;

	case DELETECHAR:
		//-- IDS_SELCHA_CHA_DELETE : ĳ���͸� �����մϴ�. �н����带 �Է��ϼ���E
		m_MsgPopUp.PutString((char*)G_STRING(IDS_SELCHA_CHA_DELETE));
		m_StartBtn.SetState(BTN_DISABLE);
		m_NewBtn.SetState(BTN_DISABLE);
		m_DeleteBtn.SetState(BTN_DISABLE);
		m_BackBtn.SetState(BTN_DISABLE);
		m_ExitBtn.SetState(BTN_DISABLE);
		break;

	case GOLOGIN:
		// 1. "�ڷ�" ��ư�� �����濁E������ ���������� ���´�
		// 2. "�ڷ�" ��ư�� ������E������ ���õ� �ɸ��Ͱ� �����ǰ�E�ƹ��͵� ��������E���� ���·� �����
		// 3. "�ڷ�" ��ư�� ��������E"���ɸ���"��ư�� Ȱ��ȭ�� �Ȼ��¶�E"���ɸ���"��ư�� ��Ȱ��ȭ ��Ű��E"����"��ư�� Ȱ��ȭ��Ų��
		// 4. "�ڷ�" ��ư�� ������E���õ� �ɸ����� "������"�� ��������E�ʴ´�
		// 5. "new" / "delete" ��ư�� �׷��� ó�����·� �����
		// 6. ����������E������ "-1"�� �����

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


		// �� ���ⱁE..
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

		// ��Ʈ���� �������� ������ ���� ��E�����Ƿ� �÷��׸� FALSE�� ���´�.
		m_pSheet->m_pLoginPg->m_bShowMatrixList = FALSE;

		m_pSheet->GoPage( LOGINPAGE );
		return ;

	}
}

void CSelectCharacterPage::ResetAccountEnd() // ���������� ǥ�� ��� �ʱ�ȭ.
{
	m_UserIndex = 0; // ���� �������� �˱�E���� �ʿ��� ����E�ε���.
	ZeroMemory( &m_FlatRateDay, sizeof(SYSTEMTIME) ); // ���� ����� ��E��E���� �����ϱ�E���� �༮
	ZeroMemory( m_strFlatRateTime, sizeof(m_strFlatRateTime) ); // ���� ����� ��E��E���� �����ϱ�E���� �༮
	m_MeterRateTime = 0; // ���� ������� ���� ��.
	SAFE_DELETE( m_pFlatRateText ); // ��� ��E
	SAFE_DELETE( m_pMeterRateText ); // ��� ��E
}

void CSelectCharacterPage::GetAccountEndString( int UserIndex ) // �� �������� ���� �޾ƿ� ��Ʈ���� ���� ������ ������ �����.
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

void CSelectCharacterPage::SetAccountEndInfo( char *strAccountEnd ) // �� �������� ���� �޾ƿ� ��Ʈ���� ���� ������ ������ �����.
{
	ResetAccountEnd(); // �ʱ�ȭ.

	int i = 0;

	char strTemp1[256];
	char strTemp2[256];
	char msg[256];

	int OriginalStringLen = 0;
	int SectionPoint = -1; // �������� ǥ���� "||"�� ��ġ.

	if( !strAccountEnd ) // �߸�E�������E..
		return;

	OriginalStringLen = strlen(strAccountEnd);

	if( OriginalStringLen == 0 || OriginalStringLen <= 2 ) // ����E?���ų� 0 �̰ų� ���� ������ ������E"||"�� ������E
		return;

	// ���ڿ��� "||"�� �������� �ڸ���.
	for( i = 0 ; i < OriginalStringLen-2 ; ++i ) // ���ڿ� ���̸�ŭ ���鼭...
	{
		if( strncmp(&strAccountEnd[i],"||",2) == 0 ) // ���� ���� �߰ߵǾ�����E..
			SectionPoint = i; // �������� �⾁Eصд?
	}

	if( SectionPoint == -1 ) // �������� ������E..
		return;

	if( SectionPoint != 0 ) // OriginalStringLen�� 0�� �濁E?�������� �̿�E���E���� �濁E?
	{
		// ������ �κ� ��Ʈ�� ����.
		strncpy( strTemp1, strAccountEnd, SectionPoint );
		strTemp1[SectionPoint] = NULL;

		char temp1[10];
		char temp2[10];
		char temp3[10];

		sscanf( strTemp1, "%s %s %s %s", temp1, temp2, temp3, m_strFlatRateTime ); // ��������ح �о��´�.

		// ��Ʈ������ ����ح ��ȯ�ؾ��Ѵ�. 07�� <- �̷��濁E%d�δ� ����E?���о�������E
		m_FlatRateDay.wMonth = atoi(temp1);
		m_FlatRateDay.wDay = atoi(temp2);
		m_FlatRateDay.wYear = atoi(temp3);
	}

	if( SectionPoint+2 <= OriginalStringLen ) // �� �濁E?�������� �̿�Eϰ�E�ִ� �濁E
	{
		// ������ �κ� ��Ʈ�� ����
		strncpy( strTemp2, &strAccountEnd[SectionPoint+2], OriginalStringLen-SectionPoint+2 );
		strTemp2[OriginalStringLen-SectionPoint+2] = NULL;

		m_MeterRateTime = atoi( strTemp2 ); // ������ �ð� ����.
	}

	if( m_FlatRateDay.wMonth ) // �̰� 0�� �ƴϸ�E�������� �����ִ� �Ŵ�.
	{
		// 5�� �����϶��� ǥ��
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
				m_pFlatRateText->Init(font_size, RGB(0, 200, 0), 5, 7); // �� ���¿��� ���� ������̡ �׻�E800�̴�.
				m_pFlatRateText->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(220, 100, 135) ); // ���� : ��, ���ڻ�E �ܰ��� ��E
			}
		}
	}

	if( m_MeterRateTime && m_MeterRateTime < 60 ) // �̰� 0�� �ƴϸ�E������ �����ִ� �Ŵ�.(�ѽð� ���Ϸ� ������ ���� ǥ��)
	{
		int font_size = 14;

		sprintf( msg, G_STRING(IDS_LHSTRING1780), m_MeterRateTime );

		int len;
		if ((len = strlen(msg)) <= 0)
			return;

		m_pMeterRateText = new CTextOutBox;
		if( m_pMeterRateText )
		{
			m_pMeterRateText->Init(font_size, RGB(0, 200, 0), 5, 27 ); // �� ���¿��� ���� ������̡ �׻�E800�̴�.
			m_pMeterRateText->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(220, 100, 135) ); // ���� : ��, ���ڻ�E �ܰ��� ��E
		}
	}
}

void CSelectCharacterPage::DrawAccountEnd() // ������ ���.
{
//	if( (timeGetTime()%700) > 600 ) // �����̱�Eó��...
//		return;

	if( m_pFlatRateText && m_pMeterRateText ) // ������ ��Ʈ����E������ ��Ʈ���� �Ѵ� �ִ� �濁E
	{
		m_pFlatRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
		m_pMeterRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
	else if( m_pFlatRateText ) // �������� �ִ� �濁E
	{
		m_pFlatRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
	else if( m_pMeterRateText ) // �������� �մ� �濁E
	{
		m_pMeterRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
}
