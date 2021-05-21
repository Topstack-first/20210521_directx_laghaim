#include "stdafx.h"
#include <string.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "gamble.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "AlphaPopup.h"
#include "textoutbox.h"
#include "SpecialItem.h"
#include "controlbottom.h"
#include "controlinven.h"
#include "msgpopup.h"
#include "mouse.h"
#include "LoadEffectSound.h"
#include "DirtSnd.h"
#include "alphapopup.h"
#include "Country.h"
#include "g_stringmanager.h"
#include "TcpUtil.h"
#include "CommonConfig.h"

#define _TYPE_EXCHANGE		  0
#define _TYPE_GAMBLE		  1
#define _TYPE_JEWELGAMBLE	  2
#define _TYPE_WEAPON_REFINE   3
#define _TYPE_EVENT_LAGHAIM   4
#define _TYPE_JEWELGAMBLE2    5	// [1/21/2008 반재승] 일본보석교환
#define _TYPE_EVENT_SONKRAN_TH 6	// [5/6/2008 반재승] 태국 송크란 이벤트

#define _TYPE_RULLET	  7

#define _TYPE_GAMBLE2		 8

extern int g_EffectSound[];
extern DWORD	g_dwLangType;
extern DWORD g_dwClientCountry;


#define TIME_MOVE		100.0F // 돌아가는 속도.(너무 빨라서 약간 조절했음.)(by 원석)
#define TIME_ALL_MOVE	4000.0F // 결과 나오는 속도.(너무 오래걸려서 조절했음.)(by 원석)
#define NONE_NUM		99
#define RESULT_SUCCESS		1
#define RESULT_DWON_GRADE	2
#define RESULT_LOST			0

//#define CHOOSUK_EVENT_2007

//-------------------------------------------------------------------
// name : 생성자
//-------------------------------------------------------------------

CGamble::CGamble()
	: m_hWnd(NULL)
	, m_bType(FALSE)
	, m_iBinggo_num(0)
	, m_fStartTime(0)
	, m_fEndTime(0)
	, m_pNkCha(NULL)
	, m_my_slotstart_x(0)
	, m_my_slotstart_y(0)
	, m_bImgRender(false)
{
	memset(m_szArg, 0, sizeof(m_szArg));

	int	i;

	m_bCooSuk = FALSE;
	m_pBack = NULL;
	m_pBtnCancel = NULL;	 // "cancle" 버튼
	m_pBtnExchange = NULL;	 // "교환" 버튼
	m_pBtnInfo = NULL;		 // "성명보기" 버튼
	m_pTextInfo = NULL;
	m_pBack_Yut = NULL;
	m_pAlphaPopUp = NULL;
	m_pBack_Title = NULL;

	for(i=0; i<NUM_PANNEL; i++)
	{
		m_pRect_Click[i] = NULL;
		m_pRect_Move[i] = NULL;
	}

	m_pBinggo = NULL;
	m_pFail   = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot  = NULL;
	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_bMainActive = false;
	m_bReceiveMsg = false;
	m_iResult		 = NONE_NUM;
	m_iGamble_vnum   = -1;
	m_iGamble_idx    = -1;
	m_iClick_Slot     = NONE_NUM;
	m_iLoop_Slot      = NONE_NUM;
	m_iOrder[0] = 0;
	m_iOrder[1] = 1;
	m_iOrder[2] = 2;
	m_iOrder[3] = 5;
	m_iOrder[4] = 4;
	m_iOrder[5] = 3;
	m_iNewBinggo_num = -1;
	m_iOverTime = TIME_MOVE;
	m_bClick = m_bMove = m_bMoveEnd = false;
	m_bExchange = false;
	m_nMoney = 0;
	m_bGambleStart = FALSE;

	m_BackupItemIndex = NULL;
	m_BackupItemCount = 0;

	m_psExecution[0]		= NULL;
	m_psExecution[1]		= NULL;

	for( i = 0; i < RSIMG_END; i++ )
		m_psResult[i] = NULL;

	m_nCnt					= 0;
	m_dwStartTime			= 0;
	m_dwCoolTime			= 0;

	m_dwMouseClickTickTime = timeGetTime();
	// [4/2/2008 반재승] 패킷 이름 저장 객체 초기화 해주자.
	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CGamble::~CGamble()
{
	DeleteRes();

	SAFE_DELETE(m_pAlphaPopUp);

	m_pNkCha = NULL;
}

#include <direct.h>
//-------------------------------------------------------------------
// naem : LoadRes
//-------------------------------------------------------------------
void CGamble::LoadRes(int Type, CItemRender *pItemRender)
{
	if( !g_pDisplay )
		return;

	m_pItemRender = pItemRender;
	m_bType = Type;			 // 교환용인가? 도박용인가?
	m_bMoveEnd = FALSE;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");

	if( Type == _TYPE_RULLET )		g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/gamble/rullet/bg_gamble.bmp");
	else							g_pDisplay->CreateSurfaceFromBitmap (&m_pBack, "interface/gamble/bg_gamble.bmp");


	if( !m_pBack )
		return;

	g_pDisplay->CreateSurfaceFromBitmap (&m_psExecution[0],				"interface/gamble/rullet/execution1.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psExecution[1],				"interface/gamble/rullet/execution2.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_UP1],		"interface/gamble/rullet/1CLASSUP.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_5],			"interface/gamble/rullet/X5.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_4],			"interface/gamble/rullet/X4.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_3],			"interface/gamble/rullet/X3.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_2],			"interface/gamble/rullet/X2.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_REEVENT],	"interface/gamble/rullet/NO EVENT.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_DOWN1],		"interface/gamble/rullet/1CLASSDOWN.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_DOWN2],		"interface/gamble/rullet/2CLASSDOWN.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_psResult[RSIMG_BLANK],		"interface/gamble/rullet/BLANK.bmp");

	m_pBack->SetColorKey( TRANS_COLOR );
	m_pBack->Xpos = g_pNk2DFrame->GetClientWidth() - 291 - m_pBack->GetWidth();
	m_pBack->Ypos = 0;

	if( Type == _TYPE_RULLET )
	{
		m_my_slotstart_x = m_pBack->Xpos + 35;
		m_my_slotstart_y = m_pBack->Ypos + 320;
	}
	else
	{
		m_my_slotstart_x = m_pBack->Xpos + 35;
		m_my_slotstart_y = m_pBack->Ypos + 251;
	}



	if(m_bType == _TYPE_WEAPON_REFINE || m_bType == _TYPE_EVENT_LAGHAIM)
		LoadRefine();
	else if(m_bType == _TYPE_EXCHANGE || m_bType == _TYPE_JEWELGAMBLE
			|| m_bType == _TYPE_JEWELGAMBLE2 // [1/21/2008 반재승] 일본보석교환
			|| m_bType == _TYPE_EVENT_SONKRAN_TH) // [5/6/2008 반재승] 태국 송크란 이벤트
		LoadExchange();

	else if ( m_bType == _TYPE_RULLET )
		LoadRullet();

	else
		LoadGamble();

	m_bMainActive = TRUE;
}


// 보석 교환시 로드 루틴
//----------------------------------------------------------------------------
//-- LoadRefine()
//----------------------------------------------------------------------------
//--  마석 정제 System / Event 에서 교환 버튼을 상요하는 System
void CGamble::LoadRefine()
{
	if( !g_pDisplay || !m_pBack )
		return;

	m_pBtnCancel = new CBasicButton();
	if( m_pBtnCancel )
		m_pBtnCancel->SetFileName("common/btn_close_01");	  // 취소버튼
	m_pBtnExchange = new CBasicButton();
	if( m_pBtnExchange )
		m_pBtnExchange->SetFileName("gamble/btn_compound"); // 교환버튼
	if ( m_bType == _TYPE_EVENT_LAGHAIM )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_re/info_event_cusuk.bmp");
	else
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_re/compound_interF.bmp");	// 교체 요망

	if( m_pBtnCancel )
		m_pBtnCancel->LoadRes();
	if( m_pBtnExchange )
		m_pBtnExchange->LoadRes();

	{
		if( m_pTextInfo )
		{
			m_pTextInfo->Xpos = m_pBack->Xpos+34;
			m_pTextInfo->Ypos = m_pBack->Ypos+80;
		}
		if( m_pBtnCancel )
			m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
		if( m_pBtnExchange )
			m_pBtnExchange->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+198);


		g_pDisplay->CreateSurfaceFromBitmap (&m_pBack_Title, "interface/gamble/title_refine.bmp");
		if( m_pBack_Title )
		{
			m_pBack_Title->SetColorKey( TRANS_COLOR );
			m_pBack_Title->Xpos = m_pBack->Xpos;
			m_pBack_Title->Ypos = m_pBack->Ypos;
		}
	}
}

//----------------------------------------------------------------------------
//-- LoadExchange()
//----------------------------------------------------------------------------
//-- 교환 / 연계 /
void CGamble::LoadExchange()
{
	if( !g_pDisplay || !m_pBack )
		return;

	m_pBtnCancel = new CBasicButton();
	if( m_pBtnCancel )
		m_pBtnCancel->SetFileName("common/btn_close_01");	  // 취소버튼
	m_pBtnExchange = new CBasicButton();
	if( m_pBtnExchange )
		m_pBtnExchange->SetFileName("gamble/btn_trade"); // 교환버튼

	//-- 각 기능에 따른 설명창.
	if( m_bType == _TYPE_EXCHANGE )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_ex/info.bmp");
	else if( m_bType == _TYPE_JEWELGAMBLE )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_ex/info_jewel.bmp");	// 교체 요망
	else if( m_bType == _TYPE_JEWELGAMBLE2 )
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_ex/info_jewel2.bmp");  // [1/21/2008 반재승] 일본보석교환
	else if( m_bType == _TYPE_EVENT_SONKRAN_TH )	// [5/6/2008 반재승] 태국 송크란 이벤트
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_ex/info_event_cusuk(0)(0).bmp");	// 교체 요망

	if( m_pBtnCancel )
		m_pBtnCancel->LoadRes();
	if( m_pBtnExchange )
		m_pBtnExchange->LoadRes();

	{
		if( m_pTextInfo )
		{
			m_pTextInfo->Xpos = m_pBack->Xpos+34;
			m_pTextInfo->Ypos = m_pBack->Ypos+80;
		}
		if( m_pBtnCancel )
			m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
		if( m_pBtnExchange )
			m_pBtnExchange->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+198);

		g_pDisplay->CreateSurfaceFromBitmap (&m_pBack_Title, "interface/gamble/title_exchange.bmp");
		if( m_pBack_Title )
		{
			m_pBack_Title->SetColorKey( TRANS_COLOR );
			m_pBack_Title->Xpos = m_pBack->Xpos;
			m_pBack_Title->Ypos = m_pBack->Ypos;
		}
	}
}


// 겜블시 로드 루틴
void CGamble::LoadGamble()
{
	if( !g_pDisplay || !m_pBack )
		return;


	g_pDisplay->CreateSurfaceFromBitmap (&m_pBack_Title, "interface/gamble/title_gamble.bmp");
	if( m_pBack_Title )
	{
		m_pBack_Title->SetColorKey( TRANS_COLOR );
		m_pBack_Title->Xpos = m_pBack->Xpos;
		m_pBack_Title->Ypos = m_pBack->Ypos;
	}

	m_pBtnCancel = new CBasicButton();
	if( m_pBtnCancel )
		m_pBtnCancel->SetFileName("common/btn_close_01");		// 취소버튼

	if(m_bCooSuk)
	{
		m_pBtnInfo = new CBasicButton();
		if( m_pBtnInfo )
			m_pBtnInfo->SetFileName("gamble/btn_throw_yut");			// 설명보기 버튼

		char tmp[100];
		for(int i=0; i<NUM_PANNEL; i++)
		{
			sprintf(tmp, "interface/gamble/yut_%d.bmp", i);
			g_pDisplay->CreateSurfaceFromBitmap(&m_pRect_Move[i], tmp);
		}
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBinggo, "interface/gamble/Yut/yut_bingo.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pBack_Yut, "interface/gamble/Yut/yut_base.bmp");
		if( m_pBinggo )
			m_pBinggo->SetColorKey(RGB(0, 0, 0));
		if( m_pBack_Yut )
		{
			{
				m_pBack_Yut->Xpos = m_pBack->Xpos+33;
				m_pBack_Yut->Ypos = m_pBack->Ypos+79;
			}
		}
	}
	else
	{
		m_pAlphaPopUp = new CAlphaPopUp;
		if( m_pAlphaPopUp )
		{
			m_pAlphaPopUp->Init(14);
			m_pAlphaPopUp->LoadRes(m_ClientRc.right, m_ClientRc.bottom);
		}

		m_pBtnInfo = new CBasicButton();
		if( m_pBtnInfo )
			m_pBtnInfo->SetFileName("gamble/btn_help");			// 설명보기 버튼

		char tmp[100];
		for(int i=0; i<NUM_PANNEL; i++)
		{
			sprintf(tmp, "interface/gamble/jewel/gamble_select%d.bmp", i);
			g_pDisplay->CreateSurfaceFromBitmap(&m_pRect_Click[i], tmp);

			sprintf(tmp, "interface/gamble/jewel/gamble_light_%d.bmp", i);
			g_pDisplay->CreateSurfaceFromBitmap(&m_pRect_Move[i], tmp);
		}

		g_pDisplay->CreateSurfaceFromBitmap(&m_pBinggo, "interface/gamble/jewel/gamble_bosuk.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pFail, "interface/gamble/jewel/gamble_haegol.bmp");

		CMultiText multiText;
		if (m_pAlphaPopUp->GetIndex() != -100)
		{
			if( g_dwLangType == LANG_DEU || g_dwLangType == LANG_FR )
			{
				//-- IDS_GAMBLE_INFO01 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO01), 80, TRUE, _FCOLOR_GREEN);
				//-- IDS_GAMBLE_INFO02 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO02), 80, TRUE, _FCOLOR_GREEN);
				//-- IDS_GAMBLE_INFO03 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO03), 80, TRUE, _FCOLOR_GREEN);
				//-- IDS_GAMBLE_INFO04 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO04), 80, TRUE, _FCOLOR_GREEN);

				multiText.AddString(" ", _FCOLOR_GREEN);
				//-- IDS_GAMBLE_INFO05 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO05), 80, TRUE, _FCOLOR_RED);
				//-- IDS_GAMBLE_INFO06 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO06), 80, TRUE, _FCOLOR_RED);
				//-- IDS_GAMBLE_INFO07 :
				multiText.AddStringMulti((char *)G_STRING(IDS_GAMBLE_INFO07), 80, TRUE, _FCOLOR_RED);
			}
			else
			{
				if (m_bType == _TYPE_GAMBLE2)
				{
					if( CommonConfig::Instance()->GetUsingLP() == true )
					{
						multiText.AddString("One or more Magic Jewelery items of the same type (up to 12)", _FCOLOR_GREEN);
						multiText.AddString("If you select one of the buttons from 1 to 6,", _FCOLOR_GREEN);
						multiText.AddString("I will acquire. 10 LP is consumed", _FCOLOR_GREEN);
					}
					else
					{	
						multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_1), _FCOLOR_GREEN);						
						multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_2), _FCOLOR_GREEN);						
						multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_3), _FCOLOR_GREEN);
					}

					multiText.AddString(" ", _FCOLOR_GREEN);
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_4), _FCOLOR_RED);
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_5), _FCOLOR_RED);
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_6), _FCOLOR_RED);
					multiText.AddString(" ", _FCOLOR_RED);
				}
				else
				{
					//-- IDS_GAMBLE2_INFO_7 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_7), _FCOLOR_GREEN);
					//-- IDS_GAMBLE2_INFO_8 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_8), _FCOLOR_GREEN);
					//-- IDS_GAMBLE2_INFO_9 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_9), _FCOLOR_GREEN);
					//-- IDS_GAMBLE2_INFO_10 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE2_INFO_10), _FCOLOR_GREEN);
					multiText.AddString(" ", _FCOLOR_GREEN);
					//-- IDS_GAMBLE_INFO05 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE_INFO05), _FCOLOR_RED);
					//-- IDS_GAMBLE_INFO06 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE_INFO06), _FCOLOR_RED);
					//-- IDS_GAMBLE_INFO07 :
					multiText.AddString((char *)G_STRING(IDS_GAMBLE_INFO07), _FCOLOR_RED);
				}

			}
			if( m_pAlphaPopUp )
			{
				m_pAlphaPopUp->PutString(&multiText);
				m_pAlphaPopUp->SetIndex(-100);
			}
		}
	}

	if( m_pBtnCancel )
		m_pBtnCancel->LoadRes();
	if( m_pBtnInfo )
		m_pBtnInfo->LoadRes();

	int box_start_x, box_start_y;

	{
		if( m_pBtnCancel )
			m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
		if( m_pBtnInfo )
			m_pBtnInfo->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+198);

		box_start_x = m_pBack->Xpos + 34;
		box_start_y = m_pBack->Ypos + 80;

	}

	int cx, cy;
	cx = 53;
	cy = 56;
	int cnt = 0;
	for(int i=0; i<2; i++)
	{
		for (int ii=cnt; ii<3+cnt; ii++)
		{
			m_rtArray[ii].left = box_start_x+((ii-cnt)*cx);
			m_rtArray[ii].right = box_start_x+((ii-cnt)*cx)+cx;
			m_rtArray[ii].top = box_start_y+(i*cy);
			m_rtArray[ii].bottom = box_start_y+(i*cy)+cy;
		}
		cnt = 3;
	}
}


//-------------------------------------------------------------------
// naem : DeleteRes()
//-------------------------------------------------------------------
void CGamble::DeleteRes()
{
	SAFE_DELETE(m_pBack_Title);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);

	if(m_bType == _TYPE_EXCHANGE || m_bType == _TYPE_JEWELGAMBLE
			|| m_bType == _TYPE_JEWELGAMBLE2 // [1/21/2008 반재승] 일본보석교환
			|| m_bType == _TYPE_EVENT_SONKRAN_TH) // [5/6/2008 반재승] 태국 송크란 이벤트
		DeleteExchange();

	else if( m_bType == _TYPE_RULLET )
		DeleteRullet();

	else
		DeleteGamble();

	SAFE_DELETE( m_BackupItemIndex );
	SAFE_DELETE_RES( m_pAlphaPopUp );
}

void CGamble::DeleteExchange()
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnExchange);
	SAFE_DELETE(m_pTextInfo);
}

void CGamble::DeleteGamble()
{
	if(m_bCooSuk)
	{
		SAFE_DELETE(m_pBack_Yut);
		SAFE_DELETE(m_pBack);
		SAFE_DELETE(m_pBtnCancel);
		SAFE_DELETE(m_pBtnInfo);

		for(int i=0; i<NUM_PANNEL; i++)
			SAFE_DELETE(m_pRect_Move[i]);

		SAFE_DELETE(m_pBinggo);
	}
	else
	{
		SAFE_DELETE(m_pBack);
		SAFE_DELETE(m_pBtnCancel);
		SAFE_DELETE(m_pBtnInfo);

		for(int i=0; i<NUM_PANNEL; i++)
		{
			SAFE_DELETE(m_pRect_Click[i]);
			SAFE_DELETE(m_pRect_Move[i]);
		}

		SAFE_DELETE(m_pBinggo);
		SAFE_DELETE(m_pFail);
		SAFE_DELETE(m_pAlphaPopUp);
	}

	SAFE_DELETE( m_BackupItemIndex );
}


//-------------------------------------------------------------------
// name : Draw
//-------------------------------------------------------------------
CItem* CGamble::Draw()
{
	if(!m_bMainActive)
		return NULL;

	m_pOverItem = NULL;

	if(m_bType == _TYPE_WEAPON_REFINE || m_bType ==  _TYPE_EVENT_LAGHAIM)
		DrawRefine();
	else if(m_bType == _TYPE_EXCHANGE || m_bType == _TYPE_JEWELGAMBLE
			|| m_bType == _TYPE_JEWELGAMBLE2 // [1/21/2008 반재승] 일본보석교환
			|| m_bType == _TYPE_EVENT_SONKRAN_TH) // [5/6/2008 반재승] 태국 송크란 이벤트
		DrawExchange();
	else
	{
		if(m_bCooSuk)
			DrawGamble_CooSuk();

		else if ( m_bType == _TYPE_RULLET )
			DrawRullet();

		else
			DrawGamble();
	}

	DrawCommon();
	return m_pOverItem;
}
void CGamble::DrawRefine()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack_Title);
	g_pDisplay->Blt(m_pTextInfo->Xpos, m_pTextInfo->Ypos, m_pTextInfo);
	if( m_pBtnCancel )
		m_pBtnCancel->Draw();
	if( m_pBtnExchange )
		m_pBtnExchange->Draw();
	DrawMySlot();
}
void CGamble::DrawExchange()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack_Title);
	g_pDisplay->Blt(m_pTextInfo->Xpos, m_pTextInfo->Ypos, m_pTextInfo);
	if( m_pBtnCancel )
		m_pBtnCancel->Draw();
	if( m_pBtnExchange )
		m_pBtnExchange->Draw();
	DrawMySlot();

}

void CGamble::DrawGamble()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack_Title);
	if( m_pBtnCancel )
		m_pBtnCancel->Draw();
	if( m_pBtnInfo )
		m_pBtnInfo->Draw();
	DrawMySlot();



	if(m_iClick_Slot != NONE_NUM)
	{
		if(m_bClick)
			g_pDisplay->Blt(m_rtArray[m_iClick_Slot].left, m_rtArray[m_iClick_Slot].top, m_pRect_Click[m_iClick_Slot]);
		else if(!m_bClick && m_bMove)
		{
			m_iClick_Slot;
			DWORD curTime = timeGetTime();

			g_pDisplay->Blt(m_rtArray[m_iOrder[m_iClick_Slot]].left, m_rtArray[m_iOrder[m_iClick_Slot]].top, m_pRect_Move[m_iOrder[m_iClick_Slot]]);

			if(curTime - m_fStartTime >= m_iOverTime)
			{
				if(curTime - m_fEndTime > TIME_ALL_MOVE)
				{
					m_bGambleStart = FALSE;//0120
					m_iNewBinggo_num = m_iBinggo_num;
					if(m_iResult == RESULT_SUCCESS)
					{
						if(m_iClick_Slot == m_iBinggo_num)
						{
							m_bMove = false;
							m_bMoveEnd = true;
							m_iOverTime = TIME_MOVE;
							if( m_BackupItemCount > 1)
								ChangeToMulti();
							else
								ChangeTo(m_iGamble_vnum, m_iGamble_idx);
							m_pOverItem = NULL;
							m_iClick_Slot  = NONE_NUM;
							if (g_pDSound)
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_WINCHIP]);
							}
						}
					}
					else
					{
						if(m_iClick_Slot != m_iBinggo_num)
						{
							m_bMove = false;
							m_bMoveEnd = true;
							m_iOverTime = TIME_MOVE;
							m_pOverItem = NULL;

							if(m_iResult == RESULT_LOST)
							{
								RemoveAllMyInven();
								m_pOverItem = NULL;
								if (g_pDSound)
								{
									g_pDSound->Play(g_EffectSound[EFF_SND_KWANG]);
								}
							}
							else if(m_iResult == RESULT_DWON_GRADE)
							{
								if( m_BackupItemCount > 1)
									ChangeToMulti();
								else
									ChangeTo(m_iGamble_vnum, m_iGamble_idx);
								m_pOverItem = NULL;
								if (g_pDSound)
								{
									g_pDSound->Play(g_EffectSound[EFF_SND_KWANG]);
								}
							}
							m_iClick_Slot  = NONE_NUM;
						}
					}
				}
				m_fStartTime = curTime;
				m_iOverTime  += 10.0F;

				if (g_pDSound)
				{
					g_pDSound->Play(g_EffectSound[EFF_SND_BEEP1]);
				}

				m_iClick_Slot--;
				if(m_iClick_Slot<0) m_iClick_Slot = 5;
			}
		}
	}
	if(!m_bMove && m_bMoveEnd && m_iNewBinggo_num != -1)
	{
		for(int i=0; i<NUM_PANNEL; i++)
		{
			if(i == m_iNewBinggo_num)
				g_pDisplay->Blt(m_rtArray[i].left, m_rtArray[i].top, m_pBinggo);
			else
				g_pDisplay->Blt(m_rtArray[i].left, m_rtArray[i].top, m_pFail);
		}
	}

	if(m_pBtnInfo->GetState() == BTN_ON)
	{
		if( m_pAlphaPopUp )
			m_pAlphaPopUp->Draw(m_pBtnInfo->PosX, m_pBtnInfo->PosY);
	}
}

void CGamble::DrawGamble_CooSuk()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack_Title);
	g_pDisplay->Blt(m_pBack_Yut->Xpos, m_pBack_Yut->Ypos, m_pBack_Yut);
	if( m_pBtnCancel )
		m_pBtnCancel->Draw();
	if( m_pBtnInfo )
		m_pBtnInfo->Draw();
	DrawMySlot();

	static int pp;
	if(!m_bClick && m_bMove && m_bReceiveMsg)
	{
		DWORD curTime = timeGetTime();

		g_pDisplay->Blt(m_rtArray[m_iOrder[m_iLoop_Slot]].left, m_rtArray[m_iOrder[m_iLoop_Slot]].top, m_pRect_Move[m_iOrder[m_iLoop_Slot]]);

		//m_iOverTime = 900;

		if(curTime - m_fStartTime >= m_iOverTime)
		{

			if(curTime - m_fEndTime > TIME_ALL_MOVE)
			{
				if(m_iBinggo_num == m_iOrder[m_iLoop_Slot])
				{
					if(m_iResult == RESULT_SUCCESS)
					{
						m_bMove = false;
						m_bMoveEnd = true;
						m_iOverTime = TIME_MOVE;
						ChangeTo(m_iGamble_vnum, m_iGamble_idx);
						m_pOverItem = NULL;
						if (g_pDSound)
						{
							g_pDSound->Play(g_EffectSound[EFF_SND_WINCHIP]);
						}
						pp = m_iOrder[m_iLoop_Slot];

					}
					else
					{
						m_bMove = false;
						m_bMoveEnd = true;
						m_iOverTime = TIME_MOVE;
						m_pOverItem = NULL;

						if(m_iResult == RESULT_LOST)
						{
							RemoveAllMyInven();
							m_pOverItem = NULL;
							if (g_pDSound)
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_KWANG]);
							}
						}
						else if(m_iResult == RESULT_DWON_GRADE)
						{
							ChangeTo(m_iGamble_vnum, m_iGamble_idx);
							m_pOverItem = NULL;
							if (g_pDSound)
							{
								g_pDSound->Play(g_EffectSound[EFF_SND_KWANG]);
							}
						}
						pp = m_iOrder[m_iLoop_Slot];

					}
				}
			}


			m_fStartTime = curTime;
			m_iOverTime  += 10.0F;

			if (g_pDSound)
			{
				g_pDSound->Play(g_EffectSound[EFF_SND_BEEP1]);
			}

			m_iLoop_Slot--;
			if(m_iLoop_Slot<0) m_iLoop_Slot = 5;


		}
	}

	if(!m_bMove && m_bMoveEnd)
		g_pDisplay->Blt(m_rtArray[pp].left, m_rtArray[pp].top, m_pBinggo);
}

void CGamble::DrawCommon()
{}

//-------------------------------------------------------------------
// name : MsgProc
//-------------------------------------------------------------------
LRESULT CGamble::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int ret = 0;
	int ren = -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);
	if(m_bType == _TYPE_GAMBLE && m_bMove)
		return 1;

	switch ( msg )
	{
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:

		if(m_bType == _TYPE_EXCHANGE || m_bType == _TYPE_WEAPON_REFINE || m_bType == _TYPE_EVENT_LAGHAIM)
			ret = ExgMessage(  hWnd,  msg,  wParam,  lParam);
		else if(m_bType == _TYPE_JEWELGAMBLE
				|| m_bType == _TYPE_JEWELGAMBLE2 // [1/21/2008 반재승] 일본보석교환
				|| m_bType == _TYPE_EVENT_SONKRAN_TH) // [5/6/2008 반재승] 태국 송크란 이벤트
			ret = JewelExgMessage(  hWnd,  msg,  wParam,  lParam);

		else if ( m_bType == _TYPE_RULLET )
			ret = RulletMessage( hWnd, msg, wParam, lParam);

		else
		{
			if(m_bCooSuk)
				ret = YutMessage( hWnd, msg, wParam, lParam);
			else if (m_bType == _TYPE_GAMBLE)
				ret = GamMessage( hWnd, msg, wParam, lParam);
			else if (m_bType == _TYPE_GAMBLE2)
				ret = GamMessage2( hWnd, msg, wParam, lParam);
		}
		break;
	}
	return ret;
}

// 칩에서 보석으로의 교환시에 메세지루틴..
LRESULT CGamble::ExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int chip = -1;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);


		if(m_bExchange) // 칩을 교환하는순간 extra 하는 것을 막기 위함이다.
			return 1;

		if (IsInside(x, y))
		{
			if (m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_pBtnExchange && m_pBtnExchange->IsInside(x, y))
				m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					 y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else if( m_pNkCha )
				{
					// 아이템을 빼는 것.
					if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowGambleWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnExchange )
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowGambleWindow(FALSE);

		if (m_pBtnExchange && m_pBtnExchange->GetState() == BTN_ACTION)
		{
			//-- 에이디아 무기조합 NOC
			if(m_bType == _TYPE_WEAPON_REFINE)
			{
				sprintf(m_MsgBuf, "ms mix_item \n");
				SendMsg();
			}
			//-- "Laghaim" 추석 이벤트
			else if (m_bType == _TYPE_EVENT_LAGHAIM)
			{
				sprintf(m_MsgBuf, "ex_item\n");
				SendMsg();
			}
			else
			{
				if((chip = CheckMyslot_Exg()) !=-1)  // 정상적으로 5개의 집으로 교환을 할경우
				{
					sprintf(m_MsgBuf, "chip_exchange 0 %d\n", chip);
					SendMsg();
					m_bExchange = TRUE;
				}
			}
		}
		if( m_pBtnExchange )
			m_pBtnExchange->SetState(BTN_NORMAL);
		if( m_pBtnCancel )
			m_pBtnCancel->SetState(BTN_NORMAL);
		return 1;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;
	}
	return 0;
}

LRESULT CGamble::JewelExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int chip = -1;

	m_bExchange = FALSE;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // 칩을 교환하는순간 extra 하는 것을 막기 위함이다.
			return 1;

		if (IsInside(x, y))
		{
			if (m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			else if(m_pBtnExchange->IsInside(x, y))
				m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);
			else if (x >= m_my_slotstart_x
					 && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					 && y >= m_my_slotstart_y
					 && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (m_pNkCha && m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowGambleWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnExchange )
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel->GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowGambleWindow(FALSE);

		}
		if (m_pBtnExchange->GetState() == BTN_ACTION)
		{
			// [5/6/2008 반재승] 태국 송크란 이벤트 : 교환 아이템 체크 리턴 값이 송크란 이벤트 디파인 값이 되면 안됨.
			if( ( (chip = CheckMyslot_JewelExg()) !=-1 ) && ( CheckMyslot_JewelExg() != _TYPE_EVENT_SONKRAN_TH ) )
			{
				// 정상적으로 보석 교환을 할경우
				if( m_bType == _TYPE_JEWELGAMBLE )
				{
					sprintf(m_MsgBuf, "gi_ex\n");

					SendMsg();
				}

				else if( m_bType == _TYPE_JEWELGAMBLE2 ) // [1/21/2008 반재승] 일본보석교환
				{
					sprintf(m_MsgBuf, "jexch\n");

					SendMsg();
				}

				// [5/6/2008 반재승] 태국 송크란 이벤트
				else if( ( CheckMyslot_JewelExg() == _TYPE_EVENT_SONKRAN_TH ) && ( m_bType == _TYPE_EVENT_SONKRAN_TH ) )
				{
					sprintf(m_MsgBuf, "skrxch\n");

					SendMsg();
				}

				m_bExchange = TRUE;
			}
		}
		if( m_pBtnExchange )
			m_pBtnExchange->SetState(BTN_NORMAL);
		if( m_pBtnCancel )
			m_pBtnCancel->SetState(BTN_NORMAL);
		return 1;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;

		break;
	}
	return 0;
}

LRESULT CGamble::GamMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int cur_pannel;
	int chip_vnum;

	//int nGem_vnum;

	switch (msg)
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnInfo )
			m_pBtnInfo->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtnInfo && m_pBtnInfo->IsInside(x, y))
			m_pBtnInfo->m_iBtnFlg = BTN_ON;
		else if( m_pBtnInfo )
			m_pBtnInfo->SetState(BTN_NORMAL);


		if (m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowGambleWindow(FALSE);

		if( m_pBtnCancel )
			m_pBtnCancel->SetState(BTN_NORMAL);

		if(m_bClick)
		{
			m_bMove = true;
			m_bClick = false;
		}

		return 1;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;
		break;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if (m_pBtnCancel && m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);

			else if((cur_pannel = CheckPannel(x, y)) != -1)
			{
				// 겜블시 유저가 숫자를 클릭했을경우


				//connie test
				if((chip_vnum = CheckMyslot_Gam()) != -1 )
				{
					// 정상적인 겜블 시작

					m_bGambleStart = TRUE;//0117 겜블도중에 ESC로 빠져나가지 못하게 하기 위함.
					m_iClick_Slot = cur_pannel;
					sprintf(m_MsgBuf, "chip_exchange 1 %d %d\n", chip_vnum, m_iClick_Slot);
					SendMsg();
					m_fEndTime = m_fStartTime = timeGetTime();
					m_bClick = true;
					m_iGamble_vnum = -1;
					m_iGamble_idx  = -1;
					m_iBinggo_num = -1;
					m_iNewBinggo_num = -1;
					m_bReceiveMsg = false;
				}
			}
			else if (x >= m_my_slotstart_x
					 && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					 && y >= m_my_slotstart_y
					 && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
						m_bMoveEnd = false;
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (m_pNkCha && m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
					m_bMoveEnd = false;
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowGambleWindow(FALSE);
		else
			return 0;

		return 1;
	}
	return 0;
}

LRESULT CGamble::GamMessage2( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int cur_pannel;
	int chip_vnum;

	//int nGem_vnum;

	switch (msg)
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnInfo )
			m_pBtnInfo->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtnInfo && m_pBtnInfo->IsInside(x, y))
			m_pBtnInfo->m_iBtnFlg = BTN_ON;
		else if( m_pBtnInfo )
			m_pBtnInfo->SetState(BTN_NORMAL);


		if (m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowGambleWindow(FALSE);

		if( m_pBtnCancel )
			m_pBtnCancel->SetState(BTN_NORMAL);

		if(m_bClick)
		{
			m_bMove = true;
			m_bClick = false;
		}

		return 1;

	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;
		break;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( m_bGambleStart ) return 1;

		if (IsInside(x, y))
		{
			if (m_pBtnCancel && m_pBtnCancel->IsInside(x, y))
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);

			else if((cur_pannel = CheckPannel(x, y)) != -1)
			{
				if( CommonConfig::Instance()->GetUsingLP() == true )
				{
					if( g_pRoh->m_nLaghaimPoint < 10 )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_RULLET_MSG), TYPE_NOR_OK, 1);
						return 1;
					}
				}

				if ((chip_vnum = CheckMyslot_Gam2()) != -1  )
				{
					m_bGambleStart = TRUE;//0117 겜블도중에 ESC로 빠져나가지 못하게 하기 위함.
					m_iClick_Slot = cur_pannel;
					sprintf(m_MsgBuf, "chip_exchange 2 %d %d\n", chip_vnum, m_iClick_Slot);
					SendMsg();
					m_fEndTime = m_fStartTime = timeGetTime();
					m_bClick = true;
					m_iGamble_vnum = -1;
					m_iGamble_idx  = -1;
					m_iBinggo_num = -1;
					m_iNewBinggo_num = -1;
					m_bReceiveMsg = false;
				}

				return 1;
			}
			else if (x >= m_my_slotstart_x
					 && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					 && y >= m_my_slotstart_y
					 && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
						m_bMoveEnd = false;
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (m_pNkCha && m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
					m_bMoveEnd = false;
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowGambleWindow(FALSE);
		else
			return 0;

		return 1;
	}
	return 0;
}


LRESULT CGamble::YutMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int cur_pannel;
	int chip_vnum;

	switch (msg)
	{
	case WM_RBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;
		break;


	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if( m_pBtnCancel )
				m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pBtnInfo )
				m_pBtnInfo->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pBtnInfo && m_pBtnInfo->m_iBtnFlg == BTN_DOWN)
			{
				m_bClick = true;
				return 1;
			}

			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha->m_ExtraSlot)
				{
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0
							&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0
							&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
						m_bMoveEnd = false;
					}
				}
				else
				{
					// 아이템을 빼는 것.
					if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
					m_bMoveEnd = false;
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowGambleWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnInfo )
			m_pBtnInfo->MsgProc(hWnd, msg, wParam, lParam);

		if (m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
			g_pNk2DFrame->ShowGambleWindow(FALSE);

		if( m_pBtnCancel )
			m_pBtnCancel->SetState(BTN_NORMAL);


		if(m_bClick)
		{
			if(m_pBtnInfo && m_pBtnInfo->m_iBtnFlg == BTN_ACTION)
			{

				cur_pannel = rand()%5;// 난수 발생
				if((chip_vnum = CheckMyslot_Gam()) != -1)  // 정상적인 겜블 시작
				{
					m_iLoop_Slot = m_iClick_Slot = cur_pannel;
					sprintf(m_MsgBuf, "yut 1 %d %d\n", chip_vnum, m_iClick_Slot);
					SendMsg();
					m_fEndTime = m_fStartTime = timeGetTime();

					m_iGamble_vnum = -1;
					m_iGamble_idx  = -1;
					m_iBinggo_num = -1;
					m_bReceiveMsg = false;

					m_bMove = true;
					m_bClick = false;
				}
				if( m_pBtnInfo )
					m_pBtnInfo->m_iBtnFlg  = BTN_NORMAL;
				return 1;
			}
		}
		return 1;


	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		if( m_pBtnInfo )
			m_pBtnInfo->MsgProc(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}

//-------------------------------------------------------------------
// name : DrawMySlot
//-------------------------------------------------------------------

#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)

void CGamble::DrawMySlot()
{

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	// 아이템이 위치한 슬롯 표시
	CItem *pItem = m_pNkCha->m_ExgInven;
	while (pItem)
	{
		rcRect.left = 0;
		rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top = 0;
		rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
		if( g_pDisplay )
			g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX)
							 , m_my_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY)
							 , m_pExistSlot
							 , &rcRect);
		pItem = pItem->m_Next;
	}

	// Extra Slot에 있는 아이템이 놓일 곳 표시
	int slot_x_num, slot_y_num;
	if (m_pNkCha->m_ExtraSlot)
	{
		if (point.x >= m_my_slotstart_x
				&& point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM )
				&& point.y >= m_my_slotstart_y
				&& point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
				slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
			else
				slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

			if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
				slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
			else
				slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

			if (slot_x_num >= 0
					&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
					&& slot_y_num >= 0
					&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot);

				if (m_pNkCha->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
													GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot), GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)) >= -1)
				{
					if( g_pDisplay )
						g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
										 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
										 m_pEnableSlot, &rcRect);
				}
				else
				{
					if( g_pDisplay )
						g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
										 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
										 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// 내 교환품 그리기..
	if (point.x >= m_my_slotstart_x && point.y >= m_my_slotstart_y)
	{
		slot_x_num = GET_MY_SLOT_X(point.x);
		slot_y_num = GET_MY_SLOT_Y(point.y);
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}

	int index = m_pNkCha->GetExgSlotIndex(slot_x_num, slot_y_num);

	if( m_pNkCha )
		pItem = m_pNkCha->m_ExgInven;
	else
		pItem = NULL;

	if( !m_pItemRender )
		return;

	m_pItemRender->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.

	while (pItem)
	{
		if (pItem->m_Index == index)
		{
			g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
										 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
										 pItem , TRUE , FALSE , FALSE, TRUE );
			m_pOverItem = pItem;
		}
		else
		{
			DWORD ambient = 0x00555555;

			if(g_pNk2DFrame->IsScroll(pItem))
				ambient = 0x00cccccc;
			else if (pItem->m_PlusNum > 0)
				ambient = UPGRADE_ITEM_COLOR;
			g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
										 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
										 pItem , FALSE , FALSE , FALSE, TRUE );
		}
		pItem = pItem->m_Next;
	}
	m_pItemRender->EndRenderLolo();

}

int CGamble::CheckMyslot_Exg()
{
	int  vnum = -1;
	int chip_idx[24];
	int index = 0;

	for(int i=0; i<24; i++)
		chip_idx[i] = -1;

	CItem* pItem = m_pNkCha->m_ExgInven;

	while (pItem)
	{
		vnum = pItem->m_Vnum;

		if((vnum >= 521 && vnum <= 541)// 칩 아이템인지 확인하구..아니면 에러 메세지..
				|| (vnum >= 2237 && vnum <=2239)
				|| (vnum >= 11236 && vnum <=11238)// [2009/4/28 Theodoric] 리젠트 다이아 칩 추가
		  )
		{
			chip_idx[index] = vnum;
			if(index>0)
			{
				if(chip_idx[index-1] != chip_idx[index])
				{
					//-- IDS_GAMBLE_INCLUDE_DIFF_CHIP : 다른종류의 칩이 섞여 있습니다
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_DIFF_CHIP), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			index++;
		}
		else
		{
			//-- IDS_GAMBLE_INCLUDE_NOTCHIP : 다른종류의 아이템입니다
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_NOTCHIP), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}


	if(index != 5)
	{
		//-- IDS_GAMBLE_NEED_5CHIP : 칩의 개수는 5개이어야 합니다
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_NEED_5CHIP), TYPE_NOR_OK, 1 );
		return -1;
	}

	int tmp = chip_idx[0];
	if(chip_idx[1] == tmp && chip_idx[2] == tmp && chip_idx[3] == tmp && chip_idx[4] ==tmp)
		return tmp;

	return -1;
}

#define MAX_GEM_COUNT 7

int CGamble::CheckMyslot_JewelExg()
{
	int  vnum = -1;
	int jewel_idx[24];
	int index = 0;
	int platinum_index = 0;
	int jewel_index = 0;

	int i = 0;

	for(i=0; i<24; i++)
		jewel_idx[i] = -1;

	CItem* pItem = m_pNkCha->m_ExgInven;

	JewelExg_Info jewel_info[MAX_GEM_COUNT];
	ZeroMemory(jewel_info, sizeof(jewel_info));

	// 자수정 -> 토파즈
	jewel_info[0].vnum = 222;
	jewel_info[0].count = 3;
	jewel_info[0].money = 2500;
	jewel_info[0].platinum = 0;

	// 토파즈 -> 오팔
	jewel_info[1].vnum = 223;
	jewel_info[1].count = 3;
	jewel_info[1].money = 10000;
	jewel_info[1].platinum = 0;

	// 오팔 -> 사파이어
	jewel_info[2].vnum = 224;
	jewel_info[2].count = 3;
	jewel_info[2].money = 40000;
	jewel_info[2].platinum = 0;

	// 사파이어 -> 루비

	jewel_info[3].vnum = 225;
	jewel_info[3].count = 3;
	jewel_info[3].money = 160000;
	jewel_info[3].platinum = 1;

	// 루비 -> 에메랄드
	jewel_info[4].vnum = 226;
	jewel_info[4].count = 3;
	jewel_info[4].money = 640000;
	jewel_info[4].platinum = 2;

	// 에메랄드 -> 다이아몬드
	jewel_info[5].vnum = 227;
	jewel_info[5].count = 3;
	jewel_info[5].money = 2560000;
	jewel_info[5].platinum = 3;

	//  [7/9/2009 ppmmjj83] 일본 요청에 의해 갯수를 국내와 다르게 조정
	if( g_dwClientCountry == CTRY_JPN )
	{
		// 에메랄드 -> 다이아몬드 (일본)
		jewel_info[6].vnum = 228;
		jewel_info[6].count = 10;
		jewel_info[6].money = 81920000;
		jewel_info[6].platinum = 10;
	}
	else
	{
		// 에메랄드 -> 다이아몬드 (국내)
		jewel_info[6].vnum = 228;
		jewel_info[6].count = 5;
		jewel_info[6].money = 10240000;
		jewel_info[6].platinum = 5;
	}
//222	매자수
//223	매토파
//224	매오팔
//225	매사파
//226	매루비
//227	매에메
//228	매직 다이아
//978	매플래티늄

	// 맨첨엔 같은 보석인지 확인
	// 예외 플래티늄이 있을때 예외 처리
	while (pItem)
	{
		vnum = pItem->m_Vnum;
		bool isGem = FALSE;

		// [5/6/2008 반재승] 태국 송크란 이벤트 관련 교환 아이템 인덱스 체크
		// (파우더 덩어리 : 1904, 물병 : 1905, 은그릇 : 1906, 금잔 : 1536, 은잔 : 1537)
		if( ( vnum >= 1536 && vnum <= 1537 ) || ( vnum >= 1904 && vnum <= 1906 ) )
			return _TYPE_EVENT_SONKRAN_TH;

		for( i = 0; i < MAX_GEM_COUNT; i++ )
		{
			if( jewel_info[i].vnum == vnum || vnum == 978 )
			{

				if( vnum != 978 ) // 플래티늄 예외
					jewel_index = vnum;

				isGem = TRUE;
				break;
			}
		}
		if( !isGem )
		{
			// 암튼 보석이나 플래티늄이 없으면 에러
			//-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요
			if( m_bType != _TYPE_EVENT_SONKRAN_TH ) // [5/6/2008 반재승] 태국 송크란 이벤트
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WATCH_ITEM), TYPE_NOR_OK, 1 );
			return -1;
		}

		jewel_idx[index] = vnum;

		if( vnum == 978 ) platinum_index++;

		if( index > 0 )
		{
			if( ( jewel_idx[index-1] != jewel_idx[index] ) && m_bType == _TYPE_JEWELGAMBLE )
			{
				if( jewel_idx[index] == 978 || jewel_idx[index-1] == 978 )
				{
					// 플래티늄 예외
				}
				else
				{
					//-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WATCH_ITEM), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			else
			{
				if( vnum != 978 ) // 플래티늄 예외
					jewel_index = vnum;
			}
		}
		index++;

		pItem = pItem->m_Next;
	}

	// [1/21/2008 반재승] 일본보석교환 때문에 이전 보석 교환 시스템은 검색에 제한을 둠.
	for( i = 0; i < MAX_GEM_COUNT; i++ )
	{
		if( jewel_info[i].vnum == jewel_index )
		{
			if( m_bType == _TYPE_JEWELGAMBLE )
			{
				// 개수가 맞는지 확인
				if( (jewel_info[i].count + jewel_info[i].platinum) != index )
				{
					//-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WATCH_ITEM), TYPE_NOR_OK, 1 );
					return -1;
				}
				else
				{
					if( jewel_info[i].platinum != platinum_index )
					{
						// 총개수는 맞아도 플래티늄이 틀리면 다 틀리단 야그
						//-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WATCH_ITEM), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				// 해당 보석에 드는 라임이 있는지 확인
				if( jewel_info[i].money > g_pRoh->m_Money)// 현재의 소지금과 비교해야함
				{
					//-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WATCH_ITEM), TYPE_NOR_OK, 1 );
					return -1;
				}
			}

			return jewel_index;
		}
	}
	return -1;
}

int CGamble::CheckMyslot_Gam2()
{
	int  vnum = -1;
	int nGem_idx[24];
	int index = 0;

	for(int i = 0; i < 24; i++)
		nGem_idx[i] = -1;

	CItem* pItem = m_pNkCha->m_ExgInven;

	while (pItem)
	{
		vnum = pItem->m_Vnum;

		if( (vnum >= 222 && vnum <= 228 || vnum == 1068) )// 보석 아이템인지 확인하구..아니면 에러 메세지
		{
			nGem_idx[index] = vnum;
			if(index > 0)
			{
				if(nGem_idx[index-1] != nGem_idx[index])
				{
					// connie [2009/5/14] - 일단 스트링은 칩으로 테스트
					//-- IDS_GAMBLE_INCLUDE_DIFF_CHIP : 다른종류의 칩이 섞여 있습니다
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_DIFF_CHIP), TYPE_NOR_OK, 1 );
					return -1;
				}
			}

			index++;
		}
		else
		{
			//-- IDS_GAMBLE_INCLUDE_NOTCHIP : 다른종류의 아이템입니다
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_NOTCHIP), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}

	if(index > 12 )
	{
		//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
		return -1;
	}
	int tmp = nGem_idx[0];
	return tmp;
}


int CGamble::CheckMyslot_Gam()
{
	int  vnum = -1;
	int chip_idx[24];
	int index = 0;

	for(int i=0; i<24; i++)
		chip_idx[i] = -1;

	CItem* pItem = m_pNkCha->m_ExgInven;

	while (pItem)
	{
		vnum = pItem->m_Vnum;

		// connie [2009/5/14] -
		//if(m_bType == )
		if( (vnum >= 521 && vnum <= 541)// 칩 아이템인지 확인하구..아니면 에러 메세지..
				|| (vnum >= 2237 && vnum <= 2239 ) // [2009/4/28 Theodoric] 리젠트 다이아 칩 추가
				|| (vnum >= 11236 && vnum <=11238)
		  )
		{
			chip_idx[index] = vnum;
			if(index>0)
			{
				if(chip_idx[index-1] != chip_idx[index])
				{
					//-- IDS_GAMBLE_INCLUDE_DIFF_CHIP : 다른종류의 칩이 섞여 있습니다
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_DIFF_CHIP), TYPE_NOR_OK, 1 );
					return -1;
				}
			}
			index++;
		}
		else
		{
			//-- IDS_GAMBLE_INCLUDE_NOTCHIP : 다른종류의 아이템입니다
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GAMBLE_INCLUDE_NOTCHIP), TYPE_NOR_OK, 1 );
			return -1;
		}
		pItem = pItem->m_Next;
	}
	{
		if(index <= 0 || index > 24 )
		{
			//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), TYPE_NOR_OK, 1 );
			return -1;
		}
	}

	int tmp = chip_idx[0];
	return tmp;
}




BOOL CGamble::IsInside(int x, int y)
{
	if( !m_pBack )
		return FALSE;

	return m_pBack->IsIn( x, y);
}

BOOL CGamble::IsPlayArea(int x, int y)
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return FALSE;
}

void CGamble::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}


void CGamble::ChipExchangeMulti(int type, BOOL bResult, int vnum, int count, int binggo_num, char * arg)
{
	if(bResult)
	{
		m_iResult = RESULT_SUCCESS;
		m_iLoop_Slot = m_iClick_Slot = binggo_num;
	}
	else
	{
		if(vnum == -1)
			m_iResult = RESULT_LOST;
		else
			m_iResult = RESULT_DWON_GRADE;
	}

	m_BackupItemCount = count;

	SAFE_DELETE(m_BackupItemIndex);
	m_BackupItemIndex = new int[count];

	char buf[8];

	for(int i=0; i<count; i++)
	{
		arg = AnyOneArg(arg, buf);
		m_BackupItemIndex[i] = atoi(buf);
	}
	m_iGamble_vnum = vnum;
	m_iBinggo_num = binggo_num;
	m_bReceiveMsg = true;

}


void CGamble::ChangeToMulti()
{
	if( !m_pNkCha )
		return;

	int x_idx, y_idx;
	// 인벤의 칩들을 없에고..
	RemoveAllMyInven();

	if (m_iGamble_vnum == -1)
		return;

	// 상위칩이나 보석으로 바꾸어준다
	//m_pNkCha->m_ExgInven = new CItem

	// Clear
	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
			m_pNkCha->m_ExgSlot[x_idx][y_idx] = -1;

	CItem * pNextItem = NULL;

	for( int i=0; i<m_BackupItemCount; i++)
	{
		CItem * pItem = new CItem;

		if( pItem)
		{
			pItem->m_Index	= m_BackupItemIndex[i];
			pItem->m_Vnum	= m_iGamble_vnum;
			pItem->m_ProtoNum = g_ItemList.FindItem(m_iGamble_vnum);
			pItem->m_Num0	= GET_ITEM_NUM0(pItem);
			pItem->m_Num1	= GET_ITEM_NUM1(pItem);
			pItem->m_Num2	= GET_ITEM_NUM2(pItem);
			pItem->m_Num3	= GET_ITEM_NUM3(pItem);
			pItem->m_Num4	= GET_ITEM_NUM4(pItem);
			pItem->m_Num5	= GET_ITEM_NUM5(pItem);
			pItem->m_Num6	= GET_ITEM_NUM6(pItem);
			pItem->m_Num7	= GET_ITEM_NUM7(pItem);
			pItem->m_Num8	= GET_ITEM_NUM8(pItem);
			pItem->m_Num9	= GET_ITEM_NUM9(pItem);

			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int	= GET_ITEM_INT(pItem);
			pItem->m_Dex	= GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);
			pItem->m_Next	= NULL;

			int x, y;

			if( m_pNkCha->FindExgSlotBlankSlot(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem) ) )
			{
				pItem->m_SlotX = x;
				pItem->m_SlotY = y;
				// Add
				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
						m_pNkCha->m_ExgSlot[x+x_idx][y+y_idx] = pItem->m_Index;

				if(m_pNkCha->m_ExgInven == NULL )
				{
					m_pNkCha->m_ExgInven = pItem;
					pNextItem = m_pNkCha->m_ExgInven;
				}
				else
				{
					pNextItem->m_Next = pItem;
					pNextItem = pItem;
				}
			}
			else
			{
				// 뭔가 문제가 있는 것.
			}
		}
	}

	m_bExchange = FALSE;
	m_BackupItemCount = -1;
	SAFE_DELETE(m_BackupItemIndex);
}


void CGamble::ChipExchange(int type, BOOL bResult, int vnum, int svr_idx, int binggo_num)
{
	if (type == 0)
	{
		if(bResult)
			ChangeTo(vnum, svr_idx);
		m_bExchange = FALSE;
	}
	else if (type == 1 || type == 2)
	{
		if(bResult)
		{
			m_iResult = RESULT_SUCCESS;
			m_iLoop_Slot = m_iClick_Slot = binggo_num;
		}
		else
		{
			if(vnum == -1)
				m_iResult = RESULT_LOST;
			else
				m_iResult = RESULT_DWON_GRADE;
		}
		m_iGamble_vnum = vnum;
		m_iGamble_idx  = svr_idx;
		m_iBinggo_num = binggo_num;
		m_bReceiveMsg = true;
	}
}

void CGamble::JewelExchange(BOOL bResult, int vnum, int svr_idx)
{
	if( bResult == 0 )
	{
		//-- IDS_WATCH_ITEM : 아이템 혹은 소지금을 확인해주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WATCH_ITEM), TYPE_NOR_OK, 1 );
		m_bExchange = FALSE;
	}
	else if( bResult == 1 )
		ChangeTo(vnum, svr_idx);
}


void CGamble::ChangeTo(int vnum, int svr_idx)
{
	if( !m_pNkCha )
		return;

	int x_idx, y_idx;
	// 인벤의 칩들을 없에고..
	RemoveAllMyInven();

	if (vnum == -1)
		return;

	// 상위칩이나 보석으로 바꾸어준다
	m_pNkCha->m_ExgInven = new CItem;
	if( m_pNkCha->m_ExgInven )
	{
		m_pNkCha->m_ExgInven->m_Index = svr_idx;
		m_pNkCha->m_ExgInven->m_SlotX = 0;
		m_pNkCha->m_ExgInven->m_SlotY = 0;
		m_pNkCha->m_ExgInven->m_Vnum = vnum;
		m_pNkCha->m_ExgInven->m_ProtoNum = g_ItemList.FindItem(vnum);
		m_pNkCha->m_ExgInven->m_Num0 = GET_ITEM_NUM0(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num1 = GET_ITEM_NUM1(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num2 = GET_ITEM_NUM2(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num3 = GET_ITEM_NUM3(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num4 = GET_ITEM_NUM4(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num5 = GET_ITEM_NUM5(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num6 = GET_ITEM_NUM6(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num7 = GET_ITEM_NUM7(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num8 = GET_ITEM_NUM8(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Num9 = GET_ITEM_NUM9(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_MinLevel = GET_ITEM_LEVEL(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_MinLevel_Org = m_pNkCha->m_ExgInven->m_MinLevel;
		m_pNkCha->m_ExgInven->m_Int = GET_ITEM_INT(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Dex = GET_ITEM_DEX(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Weight = GET_WEIGHT(m_pNkCha->m_ExgInven);
		m_pNkCha->m_ExgInven->m_Next= NULL;
	}

	// Clear
	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
		{
			m_pNkCha->m_ExgSlot[x_idx][y_idx] = -1;
		}
	}
	// Add
	for (x_idx = 0; x_idx < GET_ITEM_WIDTH(m_pNkCha->m_ExgInven); x_idx++)
	{
		for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(m_pNkCha->m_ExgInven); y_idx++)
		{
			m_pNkCha->m_ExgSlot[x_idx][y_idx] = svr_idx;
		}
	}
	m_bExchange = FALSE;
}

void CGamble::RemoveAllMyInven()
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	g_pRoh->m_ExgInven = NULL;
}


int CGamble::CheckPannel(int x, int y)
{
	for(int i=0; i<NUM_PANNEL; i++)
	{
		if(x > m_rtArray[i].left && x < m_rtArray[i].right
				&& y > m_rtArray[i].top && y < m_rtArray[i].bottom)
		{
			return i;
		}
	}
	return -1;
}

void CGamble::ForcingCloseGamble()
{
	if(m_bReceiveMsg)
	{
		ChangeTo(m_iGamble_vnum, m_iGamble_idx);
		m_bReceiveMsg = false;
		m_iResult        = NONE_NUM;
		m_iGamble_vnum   = -1;
		m_iGamble_idx    = -1;
		m_iClick_Slot    = NONE_NUM;
		m_bClick = m_bMove = m_bMoveEnd = false;
		m_fEndTime = m_fStartTime = timeGetTime();
		m_iOverTime = TIME_MOVE;
		m_bGambleStart = FALSE; //0121
	}
}

void CGamble::LoadRullet(void)
{
	if( !g_pDisplay || !m_pBack )
		return;

	m_pBtnCancel	= new CBasicButton();
	m_pBtnExchange	= new CBasicButton();

	if( m_pBtnCancel )		m_pBtnCancel->SetFileName("common/btn_close_01");	  // 취소버튼
	if( m_pBtnExchange )	m_pBtnExchange->SetFileName("gamble/rullet/btn_stat");		// 시작버튼

	if( CommonConfig::Instance()->GetUsingLP() == true )
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/description/gamble_ex/info_rullet_lp.bmp");	// 이미지 아직이미로 일단 주석
	}
	else
	{
		g_pDisplay->CreateSurfaceFromBitmap(&m_pTextInfo, "interface/gamble/rullet/text.bmp");	// 이미지 아직이미로 일단 주석
	}

	if( m_pBtnCancel )		m_pBtnCancel->LoadRes();
	if( m_pBtnExchange )	m_pBtnExchange->LoadRes();

	if( m_pTextInfo )
	{
		m_pTextInfo->Xpos = m_pBack->Xpos+26;
		m_pTextInfo->Ypos = m_pBack->Ypos+184;
	}

	if( m_pBtnCancel )
		m_pBtnCancel->SetPosition(m_pBack->Xpos+214, m_pBack->Ypos+6);
	if( m_pBtnExchange )
		m_pBtnExchange->SetPosition(m_pBack->Xpos+47, m_pBack->Ypos+259);

	m_iResult = RSIMG_END;
}

void CGamble::DeleteRullet(void)
{
	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pBtnCancel);
	SAFE_DELETE(m_pBtnExchange);
}

void CGamble::DrawRullet(void)
{
	g_pDisplay->Blt(m_pBack->Xpos , m_pBack->Ypos, m_pBack);
	g_pDisplay->Blt(m_pTextInfo->Xpos, m_pTextInfo->Ypos, m_pTextInfo);

	if( m_pBtnCancel )		m_pBtnCancel->Draw();
	if( m_pBtnExchange )	m_pBtnExchange->Draw();

	DWORD dwTemp = GetTickCount();

	if( m_bExchange )
	{
		if( CommonConfig::Instance()->GetUsingLP() == true )
		{
			if (g_pDSound)
				g_pDSound->Play(g_EffectSound[EFF_SND_ROLLET_ROLL]);
		}

		if( m_dwCoolTime != 0 && m_dwCoolTime + 50 < dwTemp )
		{
			if( m_nCnt == 0 )	m_nCnt = 1;
			else				m_nCnt = 0;

			m_dwCoolTime = dwTemp;
		}

		if( m_dwStartTime != 0 && m_dwStartTime + 3000 < dwTemp )
		{
// 			if (g_pDSound)
// 				g_pDSound->Play(g_EffectSound[EFF_SND_ROLLET_ONEMORE]);

			int		g_ii[4];
			char	g_Buf[52];

			int nlen = strlen(m_szArg);
			m_szArg[nlen+1] = '\0';
			char* orgPtr = NULL;
			char* pszTemp = new char[nlen+1];
			strncpy( pszTemp, m_szArg, nlen+1);

			// input original pointer.
			orgPtr = pszTemp;


			pszTemp = AnyOneArg(pszTemp, g_Buf);
			g_ii[0] = atoi( g_Buf );		// 1 : 성공,   -1 : 실패

			pszTemp = AnyOneArg(pszTemp, g_Buf);
			g_ii[1] = atoi( g_Buf );		// 조건실패[ 오류번호 ], 결과[ 0:꽝, 1:한번더, 2:1단계up, 3:1단계down, 4:2단계down
			// 5:2배, 6:3배, 7:4배, 8:5배

			pszTemp = AnyOneArg(pszTemp, g_Buf);
			g_ii[2] = atoi( g_Buf );		// 보상 받을 아이템 vnum

			pszTemp = AnyOneArg(pszTemp, g_Buf);
			g_ii[3] = atoi( g_Buf );		// 보상 받을 보석 개수

			// 서버에서 아이템체크후 이상이 없을때 1을 줌
			if( g_ii[0] == 1 )
			{
				switch( g_ii[1] )
				{
				case 0:		// 꽝
					{
						if( CommonConfig::Instance()->GetUsingLP() == true )
						{
							if (g_pDSound)
								g_pDSound->Play(g_EffectSound[EFF_SND_ROLLET_BAD]);
						}

						RulletResult(g_ii[1]);
					}
					break;

				case 1:		// 한번더
					{
						if( CommonConfig::Instance()->GetUsingLP() == true )
						{
							if (g_pDSound)
								g_pDSound->Play(g_EffectSound[EFF_SND_ROLLET_ONEMORE]);
						}

						RulletResult(g_ii[1]);
					}
					break;

				case 3:		// 1단계 down
				case 4:		// 2단계 down
					{
						if( CommonConfig::Instance()->GetUsingLP() == true )
						{
							if (g_pDSound)
								g_pDSound->Play(g_EffectSound[EFF_SND_ROLLET_DOWN]);
						}						

						RulletExchangeMulti( g_ii[1], g_ii[2], g_ii[3], pszTemp);
					}
					break;

				case 2:		// 1단계 up
				case 5:		// 2배
				case 6:		// 3배
				case 7:		// 4배
				case 8:		// 5배
					{
						if( CommonConfig::Instance()->GetUsingLP() == true )
						{
							if (g_pDSound)
								g_pDSound->Play(g_EffectSound[EFF_SND_ROLLET_GOOD]);
						}

						RulletExchangeMulti( g_ii[1], g_ii[2], g_ii[3], pszTemp);
					}
					break;

				default:
					break;
				}
			}
			
			m_bExchange = FALSE;

			SAFE_DELETE_ARRAY(orgPtr);
		}

		g_pDisplay->Blt(m_pBack->Xpos + 35, m_pBack->Ypos + 76, m_psExecution[m_nCnt]);
	}

	else
	{
		if( m_iResult < RSIMG_END)
			g_pDisplay->Blt(m_pBack->Xpos + 35, m_pBack->Ypos + 76, m_psResult[m_iResult]);
	}

	DrawMySlot();
}
LRESULT CGamble::RulletMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
	int x, y;
	int hr = -1;
	int chip = -1;

	x = LOWORD (lParam);
	y = HIWORD (lParam);
	if (IsInside(x, y))
	{
		if( m_pBtnCancel )
			m_pBtnCancel->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pBtnExchange )
			m_pBtnExchange->MsgProc(hWnd, msg, wParam, lParam);
	}

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(m_bExchange) // 칩을 교환하는순간 extra 하는 것을 막기 위함이다.
			return 1;

		if (IsInside(x, y))
		{
			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM) &&
					y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
			{
				if( timeGetTime() - m_dwMouseClickTickTime < 300)  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				// 엑스트라 슬롯에 있는 경우, 교환창 슬롯에 놓는다.
				if (m_pNkCha && m_pNkCha->m_ExtraSlot)
				{
					//if (m_bExch)
					//	return 1;
					if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

					if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

					if (slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM &&
							slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
					{
						if (m_pNkCha->AddToExgInven(slot_x_num, slot_y_num))
						{
							sprintf(m_MsgBuf, "exch inven %d %d\n", slot_x_num, slot_y_num);
							SendMsg();
						}
					}
				}
				else if( m_pNkCha )
				{
					// 아이템을 빼는 것.
					if (m_pNkCha->AddToExgInven((x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT))
					{
						sprintf(m_MsgBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT);
						SendMsg();
					}
				}
			}
		}
		else if (g_pNk2DFrame && IsPlayArea(x, y))
			g_pNk2DFrame->ShowGambleWindow(FALSE);
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (!IsInside(x, y))
			return 0;

		if( m_bExchange ) return 1;

		if (m_pBtnCancel && m_pBtnCancel->GetState() == BTN_ACTION)
		{
			g_pNk2DFrame->ShowGambleWindow(FALSE);
			m_bExchange = false;
			m_dwCoolTime = 0;
		}

		if (m_pBtnExchange && m_pBtnExchange->GetState() == BTN_ACTION)
		{
			if( CommonConfig::Instance()->GetUsingLP() == true )
			{
				if( g_pRoh->m_nLaghaimPoint < 5)
				{	
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_RULLET_MSG), TYPE_NOR_OK, 1);
					return 1;
				}
			}			

			// 사용가능 아이템 : 매직자수정, 매직 토파즈, 매직오팔, 매직 사파이어, 매직 루비, 매직 에메랄드, 매직 다이아몬드, 매직 리젠트 다이아
			int nItemIndx[8] = { 222, 223, 224, 225, 226, 227, 228, 1068 };
			int	nCnt = 0;
			int	nItemIndex = 0;

			CItem* pItem = g_pRoh->m_ExgInven;

			if( pItem == NULL )
			{
				// IDS_GUILD_INVASION_FAIL : 조건이 맞지 않습니다.
				g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL),TYPE_NOR_OK, 1);
				return 1;
			}

			while (pItem)
			{
				nCnt++;

				if( nCnt > 1 )
				{
					// IDS_GUILD_INVASION_FAIL : 조건이 맞지 않습니다.
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL),TYPE_NOR_OK, 1);
					return 1;
				}

				if( !((pItem->m_Vnum >= 222 && pItem->m_Vnum <=228) || pItem->m_Vnum == 1068) )
				{
					// IDS_GUILD_INVASION_FAIL : 조건이 맞지 않습니다.
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_GUILD_INVASION_FAIL),TYPE_NOR_OK, 1);
					return 1;
				}

				nItemIndex = pItem->m_Vnum;

				pItem = pItem->m_Next;
			}

			if( CommonConfig::Instance()->GetUsingLP() == true )
			{
				if( g_pRoh->m_nLaghaimPoint < 10 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_RULLET_MSG), TYPE_NOR_OK, 1);
					return 1;
				}

				if (g_pDSound)
					g_pDSound->Play(g_EffectSound[EFF_SND_BEEP1]);
			}

			// m_bExchange 값을 true주어 인벤움직임을 막는다
			m_bExchange = TRUE;
			m_dwStartTime = m_dwCoolTime = GetTickCount();

			sprintf(m_MsgBuf, "chip_roulette %d\n", nItemIndex);
			printf("Client -> Server Msg : %s\n", m_MsgBuf);
			SendMsg();
		}
		return 1;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;
	}

	return 0;
}

void CGamble::RulletExchangeMulti( int bResult, int vnum, int count, char * arg)
{
	int		i;
	int		x_idx, y_idx;
	char	buf[8];

	m_iResult			= bResult;				// 룰렛 결과
	m_BackupItemCount	= count;				// 받을 아이템 개수
	m_iGamble_vnum		= vnum;					// 아이템 load시 고정된 인덱스

	SAFE_DELETE(m_BackupItemIndex);
	m_BackupItemIndex	= new int[count];

	for( i=0; i<count; i++)
	{
		arg = AnyOneArg(arg, buf);
		m_BackupItemIndex[i] = atoi(buf);		// 아이템 마다 다른 고유 인덱스를 저장
	}

	RemoveAllMyInven();		// 익스트라 인벤의 아이템을 모두 지움

	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;

	CItem * pNextItem = NULL;

	for( i=0; i<m_BackupItemCount; i++)
	{
		CItem * pItem = new CItem;

		if( pItem)
		{
			pItem->m_Index	= m_BackupItemIndex[i];
			pItem->m_Vnum	= m_iGamble_vnum;
			pItem->m_ProtoNum = g_ItemList.FindItem(m_iGamble_vnum);
			pItem->m_Num0	= GET_ITEM_NUM0(pItem);
			pItem->m_Num1	= GET_ITEM_NUM1(pItem);
			pItem->m_Num2	= GET_ITEM_NUM2(pItem);
			pItem->m_Num3	= GET_ITEM_NUM3(pItem);
			pItem->m_Num4	= GET_ITEM_NUM4(pItem);
			pItem->m_Num5	= GET_ITEM_NUM5(pItem);
			pItem->m_Num6	= GET_ITEM_NUM6(pItem);
			pItem->m_Num7	= GET_ITEM_NUM7(pItem);
			pItem->m_Num8	= GET_ITEM_NUM8(pItem);
			pItem->m_Num9	= GET_ITEM_NUM9(pItem);

			pItem->m_MinLevel = GET_ITEM_LEVEL(pItem);
			pItem->m_MinLevel_Org = pItem->m_MinLevel;
			pItem->m_Int	= GET_ITEM_INT(pItem);
			pItem->m_Dex	= GET_ITEM_DEX(pItem);
			pItem->m_Weight = GET_WEIGHT(pItem);
			pItem->m_Next	= NULL;

			int x, y;

			if( g_pRoh->FindExgSlotBlankSlot(x, y, GET_ITEM_WIDTH(pItem), GET_ITEM_HEIGHT(pItem) ) )
			{
				pItem->m_SlotX = x;
				pItem->m_SlotY = y;
				// Add
				for (x_idx = 0; x_idx < GET_ITEM_WIDTH(pItem); x_idx++)
					for (y_idx = 0; y_idx < GET_ITEM_HEIGHT(pItem); y_idx++)
						g_pRoh->m_ExgSlot[x+x_idx][y+y_idx] = pItem->m_Index;

				if(g_pRoh->m_ExgInven == NULL )
				{
					g_pRoh->m_ExgInven = pItem;
					pNextItem = g_pRoh->m_ExgInven;
				}
				else
				{
					pNextItem->m_Next = pItem;
					pNextItem = pItem;
				}
			}
			else
			{
				// 뭔가 문제가 있는 것.
			}
		}
	}

	m_bExchange = FALSE;
	m_BackupItemCount = -1;
	SAFE_DELETE(m_BackupItemIndex);
}

void CGamble::RulletResult(int result )
{
	int		x_idx, y_idx;

	m_iResult = result;

	if( m_iResult == 0 )
	{
		RemoveAllMyInven();		// 익스트라 인벤의 아이템을 모두 지움

		for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
			for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
				g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
	}
}
