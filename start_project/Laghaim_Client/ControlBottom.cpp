#include "stdafx.h"
#include <assert.h>
#include <direct.h>
#include "ControlBottom.h"
#include "TextBox.h"
#include "qlistbox.h"
#include "tcpipcon.h"
#include "NkCharacter.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "AlphaPopUp.h"
#include "TcpUtil.h"
#include "Skill.h"
#include "Mouse.h"
#include "ControlSkill.h"
#include "main.h"
#include "Help.h"
#include "filtering.h"
#include "controlsocial.h"
#include "NoWhisper.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "ControlTop.h"
#include "RShiftMenu.h"
#include "Mp3Module.h"
#include "Country.h"
#include "UIMgr.h"
#include "Tcpipcon.h"
#include "AutoMouse.h"
#include "g_stringmanager.h"
#include "ControlGuildM.h"
#include "s_notice_control.h"
#include "AutoPlay_test.h"
#include "DevTestGmCommand.h"
#include "YutManager.h"
#include "FishingManager.h"
#include "ChatUtilMgr.h"
#include "CommonConfig.h"
#include "PacketSend.h"
#include "LottoManager.h"
#include "ControlLotto.h"
#include "BingoManager.h"
#include "ControlBingo.h"
#include "ControlLetterWindow.h"
#include "revision.h"
#include "BaseballManager.h"
#include "LotteryManager.h"
#include "DiceGameManager.h"

#define DISTANCE_SCRBTN		337

#define _HAN_		100
#define _ENG_		101
#define _LESS_		102
#define _IDEN_		103
#define _GREATER_	104
#define TEXTVIEW_NOR 12
#define TEXTVIEW_EXP 13
#define TEXTVIEW_DIS 14

#define SHOUT_LIST_OFFSET_Y	120
#define SHOUT_LIST_MAX_LINE	6

const DWORD CHAT_HIDE_DELAY = 30000;
const DWORD CHAT_HIDEALL_DELAY = 120000;

extern BOOL		g_bTimerHelp;
extern DWORD	g_dwLangType;
extern DWORD	g_dwClientCountry;
extern HINSTANCE  g_hDllInst;
extern BOOL g_bAdmin;
extern BOOL g_bAdmin2;
extern BOOL g_bNc;
extern char g_text[128];
extern BOOL g_bGhost;
extern BOOL	g_bFreeUser;
extern int	g_nServerIndex;		// 서버 인덱스 ( 쪽지 시스템 )
extern BOOL g_bNewLetter;			//새쪽지가 있는가
extern int	g_MatrixSvr;
extern BOOL g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.

BOOL	g_bSalesView;
BOOL	g_bSendSalesOff;

DWORD g_dwSkillQuickKeyDelay = 0; // 스킬 단축키 딜레이. 스킬 연타로 렉 버그 사용하는 유저가 있어서 체크 추가 (07-02-12 원석)

#define REGIST_CHAT_PROC(cmd, funcname) \
	m_mapChatProc.insert(std::make_pair(cmd, &CControlBottom::funcname))

//--------------------------------------------------------------------
//		CONSTRUCTOR / DESTRUCTOR
//--------------------------------------------------------------------
CControlBottom::CControlBottom()
	: m_iYellowCard(0)
	, m_fStretchX(0)
	, m_fStretchY(0)
	, m_CurClientMode(0)
	, m_CB_Xpos(0)
	, m_CB_Ypos(0)
	, m_IB_Xpos(0)
	, m_IB_Ypos(0)
	, m_VitalEg_Xpos(0)
	, m_VitalEg_Ypos(0)
	, m_ManaEg_Xpos(0)
	, m_ManaEg_Ypos(0)
	, m_StaminaEg_Xpos(0)
	, m_StaminaEg_Ypos(0)
	, m_Activebox_X(0)
	, m_Activebox_Y(0)
	, m_ModeState(0)
	, _StartCnt(0)
	, _LastCnt(0)
	, m_bMoadChange(FALSE)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_DownKey(0)
	, m_QuicSlot_X(0)
	, m_QuicSlot_Y(0)
	, m_bar_y(0)
	, m_helth_bar_x(0)
	, m_mana_bar_x(0)
	, m_stamina_bar_x(0)
	, m_epower_bar_x(0)
	, m_OldNeedExp(0)
	, m_ExpBar_Cx(0)
	, m_ExpBar_Cy(0)
	, m_NeedTotalExp(0)
{
	memset(m_StrTemp, 0, sizeof(m_StrTemp));
	memset(m_commOut, 0, sizeof(m_commOut));
	memset(m_strSalesCmd, 0, sizeof(m_strSalesCmd));

	m_lcurrent_view = GageView_Type__Vital;
	m_rcurrent_view = GageView_Type__Stamina;

	m_hide_tick = 0;

	m_pBottom  = NULL;
	m_pVitalEg = NULL;
	m_pEVitalEg = NULL;
	m_pManaEg = NULL;
	m_pStaminaEg = NULL;
	m_pEpowerEg = NULL;
	m_pNkCha = NULL;
	m_pChaMode_S = NULL; // 케릭터 평화모드
	m_pChaMode_C = NULL; // 케릭터 전투모드
	m_pNeedExpBar = NULL; // 레벨업까지 남은 포인트 bar
	m_pChatOutOpt_Back = NULL; // 채팅 아웃 옵션 버튼 배경.
	m_pEtcInfo = new CAlphaPopUp;
	m_chat_hidden_list = false;
	m_chat_hidden_bottom = false;

	Init();
	InitExp();

	history_cnt = 0;
	historydraw_cnt = 0;
	ZeroMemory(m_HistoryBuf, sizeof(m_HistoryBuf));
	ZeroMemory(m_szSenderLIstBuf, sizeof(m_szSenderLIstBuf));	// nate 2006-03-10 : Whisper List

	// 채팅 거부자 리스트
	m_pNoWhisper = new CNoWhisper;
	m_bRepSalesCmd = FALSE;

	m_bF1Pressed=FALSE;
	m_bF2Pressed=FALSE;
	m_bF3Pressed=FALSE;
	m_bF4Pressed=FALSE;
	m_bF5Pressed=FALSE;
	m_bF6Pressed=FALSE;
	m_bF7Pressed=FALSE;
	m_bF8Pressed=FALSE;

	m_bViewFunctionList = FALSE;

	m_nRepSales = 0;
	m_bSalesView = TRUE;
	g_bSalesView = TRUE;
	g_bSendSalesOff = FALSE;
	m_bDrawQuick = TRUE;

	RegistChatProc();
}



CControlBottom::~CControlBottom()
{
	DeleteRes();

	SAFE_DELETE( m_pNoWhisper );
	SAFE_DELETE( m_pEtcInfo );
	SAFE_DELETE( m_QlistBox );
	SAFE_DELETE( m_QlistBox_Shout );

	// 폴더 생성및 파일이름 만들기.
	char path[1024];
	char *strPath = pCMyApp->GetExePath();
	_chdir(strPath);
	sprintf(path, "%s/lag_lcg/", strPath);
	_mkdir(path);

	FILE *stream;
	int _list[30] = {0,};
	int _list2[30] = {0,};

	char strTemp[128] = {0,};
	char temp[50] = {0,};

	if(!pCMyApp->m_bSelectCha)//1125
		return;

	strcpy(strTemp, pCMyApp->m_SaveChaName);
	int i=0;
	while(strTemp[i]!='\0')
	{
		switch(strTemp[i])
		{
		case '\\':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"0",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '/':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"1",1);
			strcpy(strTemp+i+2,temp);
			break;
		case ':':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"2",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '*':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"3",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '?':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"4",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '"':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"5",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '<':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"6",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '>':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"7",1);
			strcpy(strTemp+i+2,temp);
			break;
		case '|':
			strncpy(strTemp+i,"'",1);
			strcpy(temp,&strTemp[i+1]);
			strncpy(strTemp+i+1,"8",1);
			strcpy(strTemp+i+2,temp);
			break;
		}
		i++;
	}

	strcat(strTemp, ".lcg");
	char tmp[800];
	sprintf(tmp, "%s%s", path, strTemp);
	if( (stream = fopen( tmp, "wb" )) != NULL )
		// text로 저장하니 문제가 발생하는 것같아서 binary로 저장하게 바꿨음.+도 필요없을터다.( 05-08-12 by 원석 )
	{
		for (int i = 0; i < FUNCTION_NUM; i++ )
		{
			_list[i] = m_QuickSkill[i].skill;
			_list2[i] = m_QuickSkill[i].type;
		}
		fwrite( _list, sizeof(int), 8, stream );
		fwrite( _list2, sizeof(int), 8, stream );

		fclose( stream );
	}
}

HRESULT CControlBottom::RestoreSurfaces()
{
	int i;
	m_InputBox.RestoreSurfaces();
	if( m_QlistBox )
	{
		m_QlistBox->RestoreSurfaces();
	}
	if( m_QlistBox_Shout )
	{
		m_QlistBox_Shout->RestoreSurfaces();
	}

	for (i=0; i < QUICK_SLOT_NUM; i++)
	{
		m_QuickCount[i].Restore();
		m_QuickCount2[i].Restore();
	}

	for (i=0; i<FUNCTION_NUM; i++)  // 스킬 단축아이콘 위에 그려질 택스트
		m_FunctionKey[i].Restore();

	for (i=0; i<FUNCTION_NUM; i++)  // 스킬 단축아이콘 위에 그려질 택스트
		m_FunctionList[i].Restore();

	return S_OK;
}

void CControlBottom::Init()
{
	int i;
	m_hWnd = NULL;
	m_chat_view_value = 10;
	m_QlistBox = new CQlistBox (0, 0, 0, 0, m_chat_view_value, 0);
	m_QlistBox_Shout = new CQlistBox(0, 0, 0, 0, SHOUT_LIST_MAX_LINE, 0);

	ChatOutOpt_PlusY = 0.0f;

	// 파티
	// 길드
	// 외침
	// Whisper List


	m_ChatInOpt_LoopBtn.SetFileName( "chat/btn_chat_general_2" ); // 채팅 입력시 옵션.(길드,파티 채팅등의 편의 버튼)
	m_ChatInOpt_LoopBtn.SetFileName( "chat/btn_chat_party_2" ); // 채팅 입력시 옵션.(길드,파티 채팅등의 편의 버튼)D
	m_ChatInOpt_LoopBtn.SetFileName( "chat/btn_chat_guild_2" ); // 채팅 입력시 옵션.(길드,파티 채팅등의 편의 버튼)
	m_ChatInOpt_LoopBtn.SetFileName( "chat/btn_chat_shout_2" ); // 채팅 입력시 옵션.(길드,파티 채팅등의 편의 버튼)
	m_ChatInOpt_LoopBtn.SetFileName( "chat/btn_chat_whisper_2" );	// nate 2006-03-10 : Whisper List

	m_ChatInOpt_LoopBtn.SetDisable( FALSE );


	ZeroMemory(m_TmpBuf, sizeof(m_TmpBuf));
	m_bAct = FALSE;

	m_pEtcInfo->Init(14);


	for (i = 0; i < QUICK_SLOT_NUM; i++)
	{
		m_QuickCount[i].Init(12, RGB(255, 255, 255), 0, 0);
		m_QuickCount2[i].Init(12, RGB(250,250,50), 0, 0);
	}

	for(i=0; i<FUNCTION_NUM; i++)
	{
		m_FunctionKey[i].Init(12, RGB(64, 255, 64), 0, 0);
		m_FunctionKey[i].m_Type = _TYPE_NONE;
		m_QuickSkill[i].skill = -99;
		m_QuickSkill[i].type  = _TYPE_NONE;
		m_FunctionList[i].Init(12, _FCOLOR_YELLOW, 0, 0);
	}

	m_bViewCntChange = FALSE;
	m_CurTextView    = TEXTVIEW_EXP;
	m_CallRskillNum  = -99;
	m_CallLskillNum  = -99;
	m_iCurQuickSlotNum = -1;
}

void CControlBottom::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	LoadRes_Bottom();
	LoadRes_Chatting();

	SetPosAll();

	if( m_pEtcInfo )
		m_pEtcInfo->LoadRes(m_ScreenWidth, m_ScreenHeight);

	DivLoadRes();
	SaveChaMotionStr();
	SetChangeVal(3);
}



void CControlBottom::LoadRes_Bottom()
{
	if( !g_pDisplay )
		return;

	std::string strBgFilename = "interface/bottom/bg_bottom2.bmp";	

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBottom, (char*)strBgFilename.c_str()) ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pVitalEg, "interface/bottom/bottom_HP.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pEVitalEg, "interface/bottom/bottom_epower.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pManaEg, "interface/bottom/bottom_Mana.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pEpowerEg, "interface/bottom/bottom_epower.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pStaminaEg, "interface/bottom/bottom_Stam.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pChaMode_S, "interface/bottom/peace.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pChaMode_C, "interface/bottom/combat.bmp") ) )
		return;
	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pNeedExpBar, "interface/bottom/bottom_Exp.BMP") ) )
		return;


	m_pBottom->SetColorKey(TRANS_COLOR);
	m_pVitalEg->SetColorKey(TRANS_COLOR);
	m_pEVitalEg->SetColorKey(TRANS_COLOR);
	m_pManaEg->SetColorKey(TRANS_COLOR);
	m_pEpowerEg->SetColorKey(TRANS_COLOR);
	m_pStaminaEg->SetColorKey(TRANS_COLOR);



	m_SalesOn.Init("bottom/btn_sale");
	m_SocialBtn.Init("bottom/btn_social");
	m_btn_Letter.Init("bottom/BTN_LETTER_02");
	m_btn_NewLetter.Init("bottom/btn_new_letter");
	m_btn_Yutnori.Init("bottom/btn_yut");
	m_btn_Fishing.Init("bottom/btn_fishing");
	m_btn_Lotto.Init("bottom/btn_lotto");
	m_btn_Bingo.Init("bottom/btn_bingo");
	m_btn_Baseball.Init("bottom/btn_baseball");
	m_btn_Lottery.Init("bottom/btn_lottery");
	m_btn_DiceGame.Init("bottom/btn_dice");

	m_imgLetterNew.Init("bottom/ICON_LETTER_NEW");
	NewLetter(false);

	m_R_Skill.LoadRes();
	m_L_Skill.LoadRes();

	for (int i = 0; i < QUICK_SLOT_NUM; i++)
	{
		if (m_pNkCha)
			m_QuickCount[i].SetString(m_pNkCha->m_QuickCount[i]);

		m_QuickCount2[i].SetString("");
	}

	m_SalesOn.SetDisable(false);
	m_SocialBtn.SetDisable(false);
	m_btn_Letter.SetDisable(false);
	m_btn_NewLetter.SetDisable(false);
	m_btn_Yutnori.SetDisable(false);
	m_btn_Fishing.SetDisable(false);
	m_btn_Lotto.SetDisable(false);
	m_btn_Bingo.SetDisable(false);
	m_btn_Baseball.SetDisable(false);
	m_btn_Lottery.SetDisable(false);
	m_btn_DiceGame.SetDisable(false);

	m_chat_size_big.Init( "common/arrowup" );
	m_chat_size_small.Init( "common/arrowdown" );
}

void CControlBottom::NewLetter(bool visible)
{
	m_imgLetterNew.SetVisible(visible);
}

void CControlBottom::DeleteRes_Bottom()
{
}

void CControlBottom::Draw_Bottom()
{
	if( !g_pDisplay )
		return;
}

void CControlBottom::LoadRes_Chatting()
{
	if( !g_pDisplay || !m_pBottom )
		return;

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pChatOutOpt_Back, "interface/chat/bg_chat.bmp") ) )
		return;

	m_pChatOutOpt_Back->SetColorKey(TRANS_COLOR);

	m_ChatOutOpt_All.Init( "chat/btn_chat_all" );   // 전체 채팅 보이기
	m_ChatOutOpt_Normal.Init( "chat/btn_chat_general" ); // 일반 채팅 보이기
	m_ChatOutOpt_Party.Init( "chat/btn_chat_party" ); // 파티 채팅 보이기
	m_ChatOutOpt_Guild.Init( "chat/btn_chat_guild" ); // 길드 채팅 보이기
	m_ChatOutOpt_Shout.Init( "chat/btn_chat_whisper" ); // 외침 채팅 보이기
	m_ChatOutOpt_MasterMsg.Init( "chat/btn_chat_info" ); // 공지 채팅 보이기
	m_ChatOutOpt_BalloonOnOff.Init( "chat/btn_chat_balloon" ); // 자신 머리위에 뜨는 풍선말 On/Off 토글 버튼.

	m_ChatOutOpt_All.SetDisable( FALSE );
	m_ChatOutOpt_All.SetToggleState( TRUE );
	m_ChatOutOpt_Normal.SetDisable( FALSE );
	m_ChatOutOpt_Party.SetDisable( FALSE );
	m_ChatOutOpt_Guild.SetDisable( FALSE );
	m_ChatOutOpt_Shout.SetDisable( FALSE );
	m_ChatOutOpt_MasterMsg.SetDisable( FALSE );
	m_ChatOutOpt_BalloonOnOff.SetDisable( FALSE );
	m_ChatOutOpt_BalloonOnOff.SetToggleState( TRUE );


	m_ChatInOpt_LoopBtn.LoadRes();
}

void CControlBottom::DeleteRes_Chatting()
{
}

void CControlBottom::Draw_Chatting()
{
	if( !g_pDisplay )
		return;

	if( m_InputBox.GetEnable() )
	{
		m_chat_hidden_bottom = false;
		m_chat_hidden_list	 = false;
	}
	else
	{
		DWORD dwTick = GetTickCount();
		if( m_hide_tick )
		{
			if( m_hide_tick + CHAT_HIDE_DELAY < dwTick )
				m_chat_hidden_bottom = true;
			if( m_hide_tick + CHAT_HIDEALL_DELAY < dwTick )
				m_chat_hidden_list = true;
		}
		else
		{
			m_hide_tick = dwTick;
		}
	}

	if( !m_chat_hidden_bottom )
	{
		if( m_pChatOutOpt_Back )
			g_pDisplay->Blt( m_pChatOutOpt_Back->Xpos , m_pChatOutOpt_Back->Ypos, m_pChatOutOpt_Back );

		m_ChatOutOpt_All.DrawToBlt();
		m_ChatOutOpt_Normal.DrawToBlt();
		m_ChatOutOpt_Party.DrawToBlt();
		m_ChatOutOpt_Guild.DrawToBlt();
		m_ChatOutOpt_Shout.DrawToBlt();
		m_ChatOutOpt_MasterMsg.DrawToBlt();
		m_ChatOutOpt_BalloonOnOff.DrawToBlt();


		m_ChatInOpt_LoopBtn.DrawToBlt();

		m_InputBox.Draw();
	}

//	if( !m_chat_hidden_list )
	{
		SetFramePos(3);
		if( m_QlistBox )
		{
			m_QlistBox->Draw();
		}
		if( m_QlistBox_Shout )
		{
			m_QlistBox_Shout->Draw();
		}

		m_chat_size_big.Draw();
		m_chat_size_small.Draw();
	}
}


void CControlBottom::DivLoadRes()
{
	m_FunctionKey[0].SetString("F2");
	m_FunctionKey[1].SetString("F3");
	m_FunctionKey[2].SetString("F4");
	m_FunctionKey[3].SetString("F5");
	m_FunctionKey[4].SetString("F6");
	m_FunctionKey[5].SetString("F7");
	m_FunctionKey[6].SetString("F8");
	m_FunctionKey[7].SetString("F1");

	m_FunctionList[0].SetString("F1"); //m_QuickSkill[i].skill:스킬번호가 들어감.
	m_FunctionList[1].SetString("F2"); //g_Skills[skill_idx].m_Name
	m_FunctionList[2].SetString("F3");//g_Skills[m_QuickSkill[0].skill].m_Name
	m_FunctionList[3].SetString("F4");
	m_FunctionList[4].SetString("F5");
	m_FunctionList[5].SetString("F6");
	m_FunctionList[6].SetString("F7");
	m_FunctionList[7].SetString("F8");

	_StartCnt = 0 ;
	_LastCnt = _StartCnt+12;
}


void CControlBottom::SetPosAll()
{
	int bgx = m_pBottom->Xpos = (g_pNk2DFrame->GetClientWidth() - m_pBottom->GetWidth() ) >> 1;
	int bgy = m_pBottom->Ypos = g_pNk2DFrame->GetClientHeight() - m_pBottom->GetHeight();

	m_L_Skill.SetPos( bgx + 96 , bgy + 44 );
	m_R_Skill.SetPos( bgx + 673, bgy + 44 );

	m_SocialBtn.SetPosition(bgx + 246 , bgy + 48);
	m_SalesOn.SetPosition(bgx + 214 , bgy + 52);
	m_btn_Letter.SetPosition( bgx + 182 , bgy + 53 );
	m_btn_NewLetter.SetPosition( bgx + 564 , bgy + 52 );
	m_btn_Fishing.SetPosition( bgx + 596, bgy + 52 );
	m_btn_Yutnori.SetPosition( bgx + 628, bgy + 52 );
	m_btn_Baseball.SetPosition( bgx + 596, bgy + 22 );
	m_btn_DiceGame.SetPosition( bgx + 564, bgy + 22 );

	m_btn_Lotto.SetPosition( bgx + 596, bgy + 52 );
	m_btn_Bingo.SetPosition( bgx + 596, bgy + 52 );
	
	m_btn_Lottery.SetPosition( bgx + 596, bgy + 52 );
	

	m_imgLetterNew.SetPos(bgx + 199, bgy + 41);

	m_VitalEg_Xpos		= bgx + 30;
	m_VitalEg_Ypos		= bgy + 0;

	m_ManaEg_Xpos		= bgx + 703;
	m_ManaEg_Ypos		= bgy + 0;

	m_EpowerEg_Xpos		= bgx + 703;
	m_EpowerEg_Ypos		= bgy + 0;

	m_StaminaEg_Xpos	= bgx + 703;
	m_StaminaEg_Ypos	= bgy + 0;

	m_QuicSlot_X		= bgx + 300;
	m_QuicSlot_Y		= bgy + 36;
	m_QuicSlot_Y		= bgy + 3;

	m_pVitalEg->Xpos	= bgx + 30;
	m_pVitalEg->Ypos	= bgy + 0;

	m_pEVitalEg->Xpos	= bgx + 30;
	m_pEVitalEg->Ypos	= bgy + 0;

	m_pManaEg->Xpos		= bgx + 703;
	m_pManaEg->Ypos		= bgy + 0;

	m_pEpowerEg->Xpos	= bgx + 703;
	m_pEpowerEg->Ypos	= bgy + 0;

	m_pStaminaEg->Xpos = bgx + 703;
	m_pStaminaEg->Ypos = bgy + 0;

	if( m_pNeedExpBar )
	{
		m_pNeedExpBar->Xpos = bgx + 282;
		m_pNeedExpBar->Ypos = bgy + 72;


		m_ExpBar_Cx = m_pNeedExpBar->GetWidth();
		m_ExpBar_Cy = m_pNeedExpBar->GetHeight();
	}

	m_pChaMode_S->Xpos  = bgx + 527;
	m_pChaMode_S->Ypos  = bgy + 48;
	m_pChaMode_C->Xpos  = bgx + 527;
	m_pChaMode_C->Ypos  = bgy + 48;


	m_bar_y			= m_ScreenHeight - 79;
	m_helth_bar_x   = 38;
	m_mana_bar_x	= 54;
	m_stamina_bar_x = m_ScreenWidth - 68;
	m_epower_bar_x  = m_ScreenWidth - 53;



	// 채팅창 포지션 지정하기
	m_pChatOutOpt_Back->Xpos = 0;
	m_pChatOutOpt_Back->Ypos = g_pNk2DFrame->GetClientHeight() - m_pBottom->GetHeight() - m_pChatOutOpt_Back->GetHeight() - 5;

	bgx = m_pChatOutOpt_Back->Xpos;
	bgy = m_pChatOutOpt_Back->Ypos;
	int bgyb = m_pChatOutOpt_Back->Ypos + m_pChatOutOpt_Back->GetHeight();

	m_InputBox.Init(bgx+5+48 , bgy-35 , 17*2, RGB(255, 255, 255), _FCOLOR_YELLOW);
	LostFocus();

	m_InputBox.SetMaxLen(20*2);//채팅에디트 수정
	m_InputBox.SetfontSize(14);
	if( m_QlistBox )
	{
		m_QlistBox->SetImage(14);
	}
	if( m_QlistBox_Shout )
	{
		m_QlistBox_Shout->SetImage(12);
	}


	int offset = 46;
	m_ChatOutOpt_All.SetPosition( bgx , bgyb - 18 );
	m_ChatOutOpt_Normal.SetPosition( bgx + offset , bgyb - 18 );
	m_ChatOutOpt_Party.SetPosition( bgx + offset*2 , bgyb - 18 );
	m_ChatOutOpt_Guild.SetPosition( bgx + offset*3 , bgyb - 18 );
	m_ChatOutOpt_Shout.SetPosition( bgx + offset*4 , bgyb - 18 );
	m_ChatOutOpt_MasterMsg.SetPosition( bgx + offset*5 , bgyb - 18 );
	m_ChatOutOpt_BalloonOnOff.SetPosition( bgx + offset*6 , bgyb - 18 );

	m_QlistBox->m_PosX = bgx + 20;
	m_QlistBox->m_PosY = bgy - 15;
	m_QlistBox_Shout->m_PosX = m_QlistBox->m_PosX;
	m_QlistBox_Shout->m_PosY = m_QlistBox->m_PosY - SHOUT_LIST_OFFSET_Y;

	m_InputBox.SetPos(bgx + 5 + 48 , bgyb - 35);
	m_InputBox.DrawText();

	m_ChatOutOpt_View.PosX = bgx + 1;
	m_ChatOutOpt_View.PosY = bgy - 10;

	m_ChatInOpt_LoopBtn.SetPosition( bgx , bgyb - 38 );

	m_chat_size_big.SetPosition( bgx + 2 , bgy + 237 );
	m_chat_size_small.SetPosition( bgx + 2 + 17 , bgy + 237 );
}

//--------------------------------------------------------------------
// GetRefHeight()
// Desc : Viewport 변환을 위한 참조점.
//--------------------------------------------------------------------
DWORD CControlBottom::GetRefHeight()
{
	if( m_pBottom )
		return m_pBottom->GetHeight();

	return 80;
}

void CControlBottom::DeleteRes()
{
	m_InputBox.DeleteRes();

	SAFE_DELETE(m_pBottom);
	SAFE_DELETE(m_pVitalEg);
	SAFE_DELETE(m_pEVitalEg);
	SAFE_DELETE(m_pManaEg);
	SAFE_DELETE(m_pStaminaEg);
	SAFE_DELETE(m_pEpowerEg);

	if( m_QlistBox )
	{
		m_QlistBox->DeleteRes();
	}
	if( m_QlistBox_Shout )
	{
		m_QlistBox_Shout->DeleteRes();
	}

	m_SalesOn.DeleteRes();
	m_SocialBtn.DeleteRes();
	m_btn_Letter.DeleteRes();
	m_btn_NewLetter.DeleteRes();
	m_btn_Yutnori.DeleteRes();
	m_btn_Fishing.DeleteRes();
	m_btn_Lotto.DeleteRes();
	m_btn_Bingo.DeleteRes();
	m_btn_Baseball.DeleteRes();
	m_btn_Lottery.DeleteRes();
	m_btn_DiceGame.DeleteRes();

	m_imgLetterNew.DeleteRes();

	SAFE_DELETE(m_pChaMode_S);
	SAFE_DELETE(m_pChaMode_C);
	SAFE_DELETE(m_pNeedExpBar);
	SAFE_DELETE(m_pChatOutOpt_Back);

	m_ChatOutOpt_All.DeleteRes();   // 전체 채팅 보이기
	m_ChatOutOpt_Normal.DeleteRes(); // 일반 채팅 보이기
	m_ChatOutOpt_Party.DeleteRes(); // 파티 채팅 보이기
	m_ChatOutOpt_Guild.DeleteRes(); // 길드 채팅 보이기
	m_ChatOutOpt_Shout.DeleteRes(); // 외침 채팅 보이기
	m_ChatOutOpt_MasterMsg.DeleteRes(); // 공지 채팅 보이기
	m_ChatOutOpt_BalloonOnOff.DeleteRes(); // 자신 머리위에 뜨는 풍선말 On/Off 토글 버튼.
	m_ChatInOpt_LoopBtn.DeleteRes(); // 채팅 입력 옵션 버튼.
	m_R_Skill.DeleteRes();
	m_L_Skill.DeleteRes();
	if( m_pEtcInfo )
		m_pEtcInfo->DeleteRes();

	m_chat_size_big.DeleteRes();
	m_chat_size_small.DeleteRes();

	for (int i = 0; i < QUICK_SLOT_NUM; i++)
	{
		m_QuickCount[i].DeleteRes();
		m_QuickCount2[i].DeleteRes();
	}

	for (int i=0; i<FUNCTION_NUM; i++)
		m_FunctionKey[i].DeleteRes();

	for (int i=0; i<FUNCTION_NUM; i++)
		m_FunctionList[i].DeleteRes();
}


int CControlBottom::Draw(CItem **ppOverItem)
{
	if( !g_pDisplay )
		return FALSE;

	if( m_pBottom )
		g_pDisplay->Blt(m_pBottom->Xpos , m_pBottom->Ypos, m_pBottom);		// BOTTOM 그리기

	DrawNeedExpBar();

	if( m_pNkCha )
	{
		if( m_pEVitalEg && g_pRoh && g_pRoh->m_Accumulate) // 누적데미지 그리기
			DrawAllEg(m_pEVitalEg, m_VitalEg_Xpos, m_VitalEg_Ypos, m_pNkCha->m_Vital, m_pNkCha->m_MaxVital);
		else if( m_pVitalEg )
			DrawAllEg(m_pVitalEg, m_VitalEg_Xpos, m_VitalEg_Ypos, m_pNkCha->m_Vital, m_pNkCha->m_MaxVital);

		switch( m_rcurrent_view )
		{
		case GageView_Type__Mana:
			{
				if( m_pManaEg )
					DrawAllEg(m_pManaEg, m_ManaEg_Xpos, m_ManaEg_Ypos, m_pNkCha->m_Mana, m_pNkCha->m_MaxMana);
			}
			break;
		case GageView_Type__Stamina:
			{
				if( m_pStaminaEg )
					DrawAllEg(m_pStaminaEg, m_StaminaEg_Xpos, m_StaminaEg_Ypos, m_pNkCha->m_Stamina, m_pNkCha->m_MaxStamina);
			}
			break;
		case GageView_Type__EPower:
			{
				if( m_pEpowerEg )
					DrawAllEg(m_pEpowerEg, m_EpowerEg_Xpos, m_EpowerEg_Ypos, m_pNkCha->m_Epower, m_pNkCha->m_MaxEpower);
			}
			break;
		}
	}


	m_SalesOn.DrawToBlt();
	m_SocialBtn.DrawToBlt();
	m_btn_Letter.DrawToBlt();
	m_btn_NewLetter.DrawToBlt();
	// Add 4 Game bottom
	m_btn_Yutnori.DrawToBlt();
	m_btn_Fishing.DrawToBlt();
	m_btn_Baseball.DrawToBlt();
	m_btn_DiceGame.DrawToBlt();

	m_imgLetterNew.Draw();
	
	if( LottoManager::GetInstance()->GetLottoOn() == true )
	{
		m_btn_Lotto.DrawToBlt();
	}
	if( BingoManager::GetInstance()->GetBingoOn() == true )
	{
		m_btn_Bingo.DrawToBlt();
	}
	
	if( LotteryManager::GetInstance()->GetLotteryOn() == true )
	{
		m_btn_Lottery.DrawToBlt();
	}
	

	if(m_pChaMode_S && g_pRoh && g_pRoh->GetCombat() == FALSE)
		g_pDisplay->Blt(m_pChaMode_S->Xpos , m_pChaMode_S->Ypos, m_pChaMode_S);
	else if( m_pChaMode_C )
		g_pDisplay->Blt(m_pChaMode_C->Xpos , m_pChaMode_C->Ypos, m_pChaMode_C);



	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	//	퀵슬롯!!
	DrawQuickItem(point, ppOverItem);

	DrawFunctionList();
	int over_skill1 = m_R_Skill.Draw();
	int over_skill2 = m_L_Skill.Draw();

	DrawEtcInfo(point);
	DrawFunctionKey();

	if (over_skill1 != SKILL_UNSELECTED)
		return over_skill1;

	return over_skill2;
}

void CControlBottom::DrawQuickItem(POINT &point, CItem **ppOverItem)
{
	int quick_slot_num = CheckQuickSlotNumber(point.x, point.y);

	if( g_pNk2DFrame->GetItemRender() )
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();

	for (int j = 0; j < QUICK_SLOT_Y_NUM; ++j)
	{
		for (int i = 0; i < QUICK_SLOT_X_NUM; i++)
		{
			int n = i + (j * QUICK_SLOT_X_NUM);

			if (m_pNkCha && m_pNkCha->m_QuickSlot[n])
			{
				int x = m_QuicSlot_X + (i * QUICK_SLOT_X_SIZE) + ((QUICK_SLOT_X_SIZE - ITEM_RENDER_WIDTH) / 2);
				int y = m_QuicSlot_Y + (j * QUICK_SLOT_Y_SIZE) + ((QUICK_SLOT_Y_SIZE - ITEM_RENDER_HEIGHT) / 2);

				if (g_pNk2DFrame->GetItemRender() && n == quick_slot_num)
				{
					g_pNk2DFrame->RenderItemInUI( x, y , m_pNkCha->m_QuickSlot[n] , TRUE ,FALSE, TRUE);
					*ppOverItem = m_pNkCha->m_QuickSlot[n];
				}
				else
				{
					DWORD ambient = 0x00555555;
					//스크롤과 재분배권 ambient값 변경

					if( g_pNk2DFrame->IsScroll(m_pNkCha->m_QuickSlot[n])
							|| m_pNkCha->m_QuickSlot[n]->m_Vnum == 1251 )
						ambient = 0x00cccccc;

					g_pNk2DFrame->RenderItemInUI( x, y , m_pNkCha->m_QuickSlot[n] , FALSE ,FALSE, TRUE);

				}

				m_QuickCount[n].Draw( m_QuicSlot_X + (i * QUICK_SLOT_X_SIZE), m_QuicSlot_Y + (j * QUICK_SLOT_Y_SIZE));

				if( GET_TYPE(m_pNkCha->m_QuickSlot[n]) == ITYPE_ETC
						&& (   GET_SHAPE(m_pNkCha->m_QuickSlot[n]) == IETC_RECOVERY
							   || GET_SHAPE(m_pNkCha->m_QuickSlot[n]) == IETC_BUFF
							   || GET_SHAPE(m_pNkCha->m_QuickSlot[n]) == IETC_SUMMONNPC
						   )
				  )
				{
					if( m_pNkCha->m_QuickSlot[n]->m_Num9 )
					{
						char strTemp[10];
						sprintf( strTemp, "%d", m_pNkCha->m_QuickSlot[n]->m_Special );
						m_QuickCount2[n].SetString( strTemp, RGB(250,250,50), RGB(0,0,1) );
						m_QuickCount2[n].Draw(x + 10 , y + 58 );
					}
				}

			}
		}
	}

	if( g_pNk2DFrame->GetItemRender() )
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
}

void CControlBottom::DrawFunctionList()
{
	if(m_bViewFunctionList)
	{
		char tmp[40];
		int cur_skill_r = GetRSkill()->GetCurSkill();
		int cur_skill_l = GetLSkill()->GetCurSkill();

		sprintf(tmp, "F1 "); //F1이 배열 7에 들어있어서 부득이 빼서 먼저 찍음.
		if(m_QuickSkill[7].skill!=-99)
		{
			COLORREF font_color;
			if(m_QuickSkill[7].skill == cur_skill_r && m_QuickSkill[7].type == 1)
				font_color = _FCOLOR_RED;
			else if(m_QuickSkill[7].skill == cur_skill_l && m_QuickSkill[7].type == 2)
				font_color = _FCOLOR_GREEN;
			else
				font_color = _FCOLOR_YELLOW;

			if(m_QuickSkill[7].skill==-2)
			{
				// IDS_BOTTOM_ROT_CAM : 화면회전
				strcat(tmp, G_STRING(IDS_BOTTOM_ROT_CAM));
				m_FunctionList[7].SetString(tmp, font_color);
			}
			else if(m_QuickSkill[7].skill==-1)
			{
				// IDS_BOTTOM_ATT_NOR : 일반공격
				strcat(tmp, G_STRING(IDS_BOTTOM_ATT_NOR));
				m_FunctionList[7].SetString(tmp, font_color);
			}
			else
			{
				strcat(tmp, g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[7].skill]].GetName());
				m_FunctionList[7].SetString(tmp, font_color); //m_QuickSkill[i].skill:스킬번호가 들어감.
			}
		}
		else
		{
			m_FunctionList[7].SetString(tmp,_FCOLOR_YELLOW);
		}

		if (g_dwLangType == 0 && g_dwClientCountry == 0)
			m_FunctionList[7].Draw(5, 100+10);
		else
			m_FunctionList[7].Draw(5, 100+10);

		for(int i=0; i<FUNCTION_NUM-1; i++)
		{
			sprintf(tmp, "F%d ",i+2);

			if(m_QuickSkill[i].skill!=-99)
			{

				COLORREF font_color;
				if(m_QuickSkill[i].skill == cur_skill_r && m_QuickSkill[i].type == 1)
					font_color = _FCOLOR_RED;
				else if(m_QuickSkill[i].skill == cur_skill_l && m_QuickSkill[i].type == 2)
					font_color = _FCOLOR_GREEN;
				else
					font_color = _FCOLOR_YELLOW;

				if(m_QuickSkill[i].skill==-2)
				{
					// IDS_BOTTOM_ROT_CAM : 화면회전
					strcat(tmp, G_STRING(IDS_BOTTOM_ROT_CAM));
					m_FunctionList[i].SetString(tmp,font_color);
				}
				else if(m_QuickSkill[i].skill==-1)
				{
					// IDS_BOTTOM_ATT_NOR : 일반공격
					strcat(tmp, G_STRING(IDS_BOTTOM_ATT_NOR));
					m_FunctionList[i].SetString(tmp,font_color);
				}
				else
				{
					strcat(tmp, g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[i].skill]].GetName());
					m_FunctionList[i].SetString(tmp,font_color); //m_QuickSkill[i].skill:스킬번호가 들어감.
				}
			}
			else
			{
				//m_FunctionList[i].Init(12, _FCOLOR_YELLOW, 0, 0);
				m_FunctionList[i].SetString(tmp,_FCOLOR_YELLOW);
			}

			if( i < 3 )
			{
				if (g_dwLangType == 0 && g_dwClientCountry == 0)
					m_FunctionList[i].Draw(5, 100+10+(i+1)*15);
				else
					m_FunctionList[i].Draw(5, 100+10+(i+1)*15);
			}
			else
			{
				int k = i - 3;
				if (g_dwLangType == 0 && g_dwClientCountry == 0)
					m_FunctionList[i].Draw(5+140, 100+10+k*15);	// 97->140
				else
					m_FunctionList[i].Draw(5+97+80, 100+10+k*15);

			}
		}
	}
}

void CControlBottom::DrawEtcInfo(POINT &point)
{
	if( m_pEtcInfo )
	{
		// 풍선 도움말.
		CMultiText multiText;
		if( m_pVitalEg->IsIn( point.x , point.y ) )
		{
			if( m_lcurrent_view == GageView_Type__Vital )
			{
				if (m_pEtcInfo->GetIndex() != -100)
				{
					// IDS_BOTTOM_VATAL : 생명력
					multiText.AddString((char*)G_STRING(IDS_BOTTOM_VATAL), RGB(255, 50, 50)); // 생명력
					sprintf(m_StrTemp, "%d/%d", (int)m_pNkCha->m_Vital, (int)m_pNkCha->m_MaxVital);
					multiText.AddString(m_StrTemp, RGB(255, 255, 255));

					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-100);
				}

				m_pEtcInfo->Draw(point.x, point.y);
			}
		}
		else if( m_pManaEg->IsIn( point.x , point.y ) )
		{
			if( m_rcurrent_view == GageView_Type__Mana )
			{
				if (m_pEtcInfo->GetIndex() != -101)
				{
					// IDS_BOTTOM_MANA : 마법력
					multiText.AddString((char*)G_STRING(IDS_BOTTOM_MANA), RGB(120, 120, 255));
					sprintf(m_StrTemp, "%d/%d", (int)m_pNkCha->m_Mana, (int)m_pNkCha->m_MaxMana);
					multiText.AddString(m_StrTemp, RGB(255, 255, 255));

					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-101);
				}
			}
			else if( m_rcurrent_view == GageView_Type__Stamina )
			{
				if (m_pEtcInfo->GetIndex() != -102)
				{
					// IDS_BOTTOM_STAMINA : 스테미너
					multiText.AddString((char*)G_STRING(IDS_BOTTOM_STAMINA), RGB(255, 255, 50));
					sprintf(m_StrTemp, "%d/%d", (int)m_pNkCha->m_Stamina, (int)m_pNkCha->m_MaxStamina);
					multiText.AddString(m_StrTemp, RGB(255, 255, 255));

					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-102);
				}

			}
			else if( m_rcurrent_view == GageView_Type__EPower )
			{
				if (m_pEtcInfo->GetIndex() != -103)
				{
					// IDS_BOTTOM_EPOWER : 전기력
					multiText.AddString((char *)G_STRING(IDS_BOTTOM_EPOWER), RGB(50, 255, 50)); // 전기력
					sprintf(m_StrTemp, "%d/%d", (int)m_pNkCha->m_Epower, (int)m_pNkCha->m_MaxEpower);
					multiText.AddString(m_StrTemp, RGB(255, 255, 255));

					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-103);
				}
			}

			m_pEtcInfo->Draw(point.x, point.y);
		}

		// 풍선도움말 2
		else if( m_SocialBtn.IsInside(point.x , point.y ) )
		{
			if (m_pEtcInfo->GetIndex() != -104)
			{
				// IDS_BOTTOM_SOCAIL : 사교동작
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_SOCAIL), _FCOLOR_YELLOW); // 사교동작
				m_pEtcInfo->PutString(&multiText);
				m_pEtcInfo->SetIndex(-104);
			}
			m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_btn_NewLetter.IsInside(point.x, point.y) )
		{	
			if (m_pEtcInfo->GetIndex() != -106)
			{
				multiText.AddString((char *)G_STRING(IDS_FRIEND_SHORTCUT), _FCOLOR_YELLOW); // 사교동작
				m_pEtcInfo->PutString(&multiText);
				m_pEtcInfo->SetIndex(-106);
			}
			m_pEtcInfo->Draw(point.x, point.y);			
		}
		else if( m_btn_Yutnori.IsInside(point.x, point.y) )
		{
				if (m_pEtcInfo->GetIndex() != -108)
				{
					multiText.AddString((char *)G_STRING(IDS_YUTNORI_SHORTCUT), _FCOLOR_YELLOW); // 사교동작
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-108);
				}
				m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_btn_Fishing.IsInside(point.x, point.y) )
		{
				if( m_pEtcInfo->GetIndex() != -109 )
				{
					multiText.AddString((char *)G_STRING(IDS_FISHING_SHORTCUT), _FCOLOR_YELLOW); // 사교동작
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-109);
				}
				m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_btn_Lotto.IsInside(point.x, point.y) )
		{
			if( LottoManager::GetInstance()->GetLottoOn() == true )
			{
				if( m_pEtcInfo->GetIndex() != -110 )
				{
					multiText.AddString((char *)G_STRING(IDS_FISHING_SHORTCUT), _FCOLOR_YELLOW);
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-110);
				}
				m_pEtcInfo->Draw(point.x, point.y);
			}
		}
		else if( m_btn_Bingo.IsInside(point.x, point.y) )
		{
			if( BingoManager::GetInstance()->GetBingoOn() == true )
			{
				if( m_pEtcInfo->GetIndex() != -111 )
				{
					multiText.AddString((char *)G_STRING(IDS_BINGO_SHORTCUT), _FCOLOR_YELLOW);
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-111);
				}
				m_pEtcInfo->Draw(point.x, point.y);
			}
		}
		else if( m_btn_Baseball.IsInside(point.x, point.y) )
		{
				if( m_pEtcInfo->GetIndex() != -112 )
				{
					multiText.AddString((char *)G_STRING(IDS_FISHING_SHORTCUT), _FCOLOR_YELLOW);
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-112);
				}
				m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_btn_Lottery.IsInside(point.x, point.y) )
		{
			if( LotteryManager::GetInstance()->GetLotteryOn() == true )
			{
				if( m_pEtcInfo->GetIndex() != -113 )
				{
					multiText.AddString((char *)G_STRING(IDS_FISHING_SHORTCUT), _FCOLOR_YELLOW);
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-113);
				}
				m_pEtcInfo->Draw(point.x, point.y);
			}
		}
		else if( m_btn_DiceGame.IsInside(point.x, point.y) )
		{
				if( m_pEtcInfo->GetIndex() != -114 )
				{
					multiText.AddString("", _FCOLOR_YELLOW);
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-114);
				}
				m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_pChaMode_S->IsIn( point.x, point.y ) )
		{
			if (m_pEtcInfo->GetIndex() != -105)
			{
				// IDS_BOTTOM_MOTION_MODE : 평화 / 전투모드
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_MOTION_MODE), _FCOLOR_YELLOW); // 평화 전투모드
				m_pEtcInfo->PutString(&multiText);
				m_pEtcInfo->SetIndex(-105);
			}
			m_pEtcInfo->Draw(point.x, point.y);
		}
		else if( m_SalesOn.IsInside(point.x, point.y) )
		{
			if (m_pEtcInfo->GetIndex() != -107)
			{
				// IDS_BOTTOM_CHANNEL : 장사채널
				multiText.AddString((char *)G_STRING(IDS_BOTTOM_CHANNEL), _FCOLOR_YELLOW);
				m_pEtcInfo->PutString(&multiText);
				m_pEtcInfo->SetIndex(-107);
			}
			m_pEtcInfo->Draw(point.x, point.y);
		}




		//	탑으로 빼야되는데...일단은 ㄱㄱ싱...ㅋ


		///------

		if (g_dwLangType == 0 && g_dwClientCountry == 0)
		{
			if(IsInsideQuickSlot(point.x, point.y) && m_bDrawQuick)
			{
				if (m_pEtcInfo->GetIndex() != -119)
				{
					// IDS_BOTTOM_QUICKSLOT : 퀵슬롯
					multiText.AddString((char *)G_STRING(IDS_BOTTOM_QUICKSLOT), _FCOLOR_YELLOW);
					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-119);
				}
				m_pEtcInfo->Draw(point.x, point.y);
			}
		}

		if( m_pNeedExpBar )
		{
			// 경험치 표시
			if( m_pNeedExpBar->IsIn( point.x , point.y ) )
			{
				// nate 2004 - 6
				// 경험치 게이지에 데이터 업데이트 안되는 버그 수정
				if (m_pNkCha && m_pEtcInfo->GetIndex() != -120)
				{
					// by evilkiki 2009.09.014 :
// 					g_pRoh->GetNeedExp();
// 					g_pRoh->GetTotalExp();
// 					g_pRoh->GetLevelExp(g_pRoh->m_Level);
// 					g_pRoh->GetLevelExp(g_pRoh->m_Level+1);

					sprintf(m_StrTemp, "%I64d / %I64d (%.1f%%)" , g_pNk2DFrame->GetExpGet() , g_pNk2DFrame->GetExpRequire() , (float)g_pNk2DFrame->GetExpGet() / g_pNk2DFrame->GetExpRequire() * 100.0f );
					multiText.AddString(m_StrTemp, RGB(255, 255, 255));

					m_pEtcInfo->PutString(&multiText);
					m_pEtcInfo->SetIndex(-120);
				}
				m_pEtcInfo->Draw(point.x, point.y);

			}
		}
	}
}


void CControlBottom::ResetInfo(int info_num)
{
	if (m_pEtcInfo && m_pEtcInfo->GetIndex() == info_num)
		m_pEtcInfo->SetIndex(-1);
}

// 체팅박스의 영역을 체크한다
BOOL CControlBottom::IsInside(int x, int y)
{
	if ( !m_pBottom )
		return false;
	return m_pBottom->IsIn( x, y );
}

BOOL CControlBottom::IsInside_Prompt(int x, int y)
{
	return m_InputBox.IsInside( x, y );
}


// 전투 / 평호모드 체크를 위해만들었으나..다른 추후 다른 곳에쓰일지 몰라서.
BOOL CControlBottom::IsInside_Tmp(CSurface* surface, int x, int y)
{
	if( !surface )
		return false;

	return surface->IsIn( x, y );
}


BOOL CControlBottom::IsChatWide()
{
	return m_bAct;
}


BOOL CControlBottom::IsInsideQuickSlot(int x, int y)
{
	if ( x >= m_QuicSlot_X && x < m_QuicSlot_X + QUICK_SLOT_WIDTH &&
			y >= m_QuicSlot_Y && y < m_QuicSlot_Y + QUICK_SLOT_HEIGHT )
		return TRUE;

	m_iCurQuickSlotNum = -1;			// 퀵슬롯 영역을 벗어나면 선택안됨으로 변경
	return FALSE;

}
int CControlBottom::CheckQuickSlotNumber(int x, int y)
{
	if (!IsInsideQuickSlot(x, y))
		return -1;

	m_iCurQuickSlotNum = -1;

	for(int j = 0; j < QUICK_SLOT_Y_NUM; ++j)
	{
		for (int i = 0; i < QUICK_SLOT_X_NUM; ++i)
		{
			if (y >= m_QuicSlot_Y+(j*QUICK_SLOT_Y_SIZE) && y < m_QuicSlot_Y+((j+1)*QUICK_SLOT_Y_SIZE))
			{
				if(x >= m_QuicSlot_X+(i*QUICK_SLOT_X_SIZE) && x < m_QuicSlot_X+((i+1)*QUICK_SLOT_Y_SIZE))
				{
					m_iCurQuickSlotNum = i + (j * QUICK_SLOT_X_NUM);
					return m_iCurQuickSlotNum;
				}
			}
		}
	}

	return m_iCurQuickSlotNum;
}

static char commOutBuf[512];	// 나중에 변수 다른 곳에 설정.
LRESULT CControlBottom::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x, y;

//	if( !m_chat_hidden_list )
	{
		if( m_QlistBox
				&& m_QlistBox->m_bScrollFlag
				&& m_QlistBox->m_Scroll.MsgProc( hWnd, msg, wParam, lParam ) == S_OK )
		{
			if( m_chat_hidden_bottom )
				m_chat_hidden_bottom = false;
			m_hide_tick = 0;
			return 1;
		}

		if( m_QlistBox && m_QlistBox->MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		{
			return 1;
		}

		if( m_QlistBox_Shout &&
			m_QlistBox_Shout->m_bScrollFlag &&
			m_QlistBox_Shout->m_Scroll.MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		{
			return 1;
		}

		if( m_QlistBox_Shout &&
			m_QlistBox_Shout->MsgProc(hWnd, msg, wParam, lParam) == S_OK )
		{
			return 1;
		}
	}

	if( !m_chat_hidden_bottom )
	{
		if( int chat_loop_state = m_ChatInOpt_LoopBtn.MsgProc(hWnd, msg, wParam, lParam) )
		{
			if( chat_loop_state == 2 )
			{
				historydraw_cnt = -1;
				m_InputBox.ClearText();

				// 채팅 인풋 옵션 버튼의 스테이트에 따라 다음 채팅말에 마커를 붙여준다.(길드, 파티, 외침 등.) (by 원석)
				switch( m_ChatInOpt_LoopBtn.GetLoopState() )
				{
				case 1: // 파티
					m_InputBox.SetText( "; ");
					break;

				case 2: // 길드
					m_InputBox.SetText( ": ");
					break;

				case 3: // 외침
					m_InputBox.SetText( "! ");
					break;

				case 4: // nate 2006-03-10 : Whisper List
					m_InputBox.SetText( "/");
					break;
				}

				m_InputBox.SetEnable(TRUE);
			}

			return 1;
		}
	}

	YutManager* yutMgr = YutManager::GetInstance();
	FishingManager* fishingMgr = FishingManager::GetInstance();
	LottoManager* lottoMgr = LottoManager::GetInstance();
	BingoManager* bingoMgr = BingoManager::GetInstance();
	BaseballManager* baseballMgr = BaseballManager::GetInstance();
	LotteryManager* lotteryMgr = LotteryManager::GetInstance();
	DiceGameManager* dicegameMgr = DiceGameManager::GetInstance();
	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if(!pCMyApp->IsSoccerZone())
		{
			if (IsInsideQuickSlot(x, y))
			{
				if( pCMyApp->m_pUIMgr->IsInvenLock() )
					return 1;

				if( timeGetTime() - pCMyApp->m_pUIMgr->m_dwUseQuickTick < 300 ) // 현재는 일본만 적용.
					return 1;

				int num = CheckQuickSlotNumber(x, y);

				if (m_pNkCha->ToQuickSlot(num))
				{
					pCMyApp->m_pUIMgr->m_dwUseQuickTick = timeGetTime();
					sprintf(commOutBuf, "quick %d\n", num);
					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage(commOutBuf);
				}
				return 1;
			}

			if(IsInside_Tmp(m_pChaMode_S, LOWORD (lParam), HIWORD (lParam)) == TRUE)
			{
				if (g_pRoh
						&& !(g_pRoh->IsDead())
						&& g_pRoh->GetMotionState() != CHA_SOCIAL
						&& !g_pNk2DFrame->IsUserShopVisible() // 개인상점이 열려있으면 평화모드 못바꿈. ///개인상점
						&& g_pRoh->IsShown())
				{
					g_pRoh->ToggleCombat();
					g_pRoh->ResetMotionState();
					sprintf(commOutBuf, "combat %d\n", g_pRoh->GetCombat());
					if (g_pTcpIp)
						g_pTcpIp->SendNetMessage(commOutBuf);
				}
				return 1;
			}

			if( m_pManaEg->IsIn( x, y ) )
			{
				switch( m_rcurrent_view )
				{
				case GageView_Type__Mana:
					m_rcurrent_view = GageView_Type__EPower;
					break;
				case GageView_Type__Stamina:
					m_rcurrent_view = GageView_Type__Mana;
					break;
				case GageView_Type__EPower:
					m_rcurrent_view = GageView_Type__Stamina;
					break;
				}

				return 1;
			}

			m_SalesOn.MsgProc( hWnd , msg , wParam , lParam );
			if( m_SalesOn.IsInside( x, y) )
				return 1;

			m_SocialBtn.MsgProc( hWnd , msg , wParam , lParam );
			if( m_SocialBtn.IsInside( x, y) )
				return 1;

			m_btn_NewLetter.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btn_NewLetter.IsInside( x, y ) )
				return 1;

			m_btn_Letter.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btn_Letter.IsInside(x, y) )
				return 1;

			m_btn_Yutnori.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btn_Yutnori.IsInside( x, y ) )
			return 1;
			
			m_btn_Fishing.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btn_Fishing.IsInside( x, y ) )
			return 1;
			
			m_btn_Baseball.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btn_Baseball.IsInside( x, y ) )
			return 1;
			
			m_btn_DiceGame.MsgProc( hWnd, msg, wParam, lParam );
			if( m_btn_DiceGame.IsInside( x, y ) )
			return 1;

			if( lottoMgr->GetLottoOn() == true )
			{
				m_btn_Lotto.MsgProc( hWnd, msg, wParam, lParam );
				if( m_btn_Lotto.IsInside( x, y ) )
					return 1;
			}
			if( bingoMgr->GetBingoOn() == true )
			{
				m_btn_Bingo.MsgProc( hWnd, msg, wParam, lParam );
				if( m_btn_Bingo.IsInside( x, y ) )
					return 1;
			}
				
			if( lotteryMgr->GetLotteryOn() == true )
			{
				m_btn_Lottery.MsgProc( hWnd, msg, wParam, lParam );
				if( m_btn_Lottery.IsInside( x, y ) )
					return 1;
			}
				
			

			if(m_R_Skill.MsgProc(hWnd, msg, wParam, lParam))
				return 1;

			if(m_L_Skill.MsgProc(hWnd, msg, wParam, lParam))
				return 1;


			if( m_chat_size_big.MsgProc( hWnd, msg, wParam , lParam ) == S_OK )
			{
				m_QlistBox->m_ViewCount = min( m_QlistBox->m_ViewCount + 3 , 12 );
				m_QlistBox->m_StartViewCount = _StartCnt;
				m_QlistBox->m_Scroll.SetRange( MAX_Q - m_QlistBox->m_ViewCount );
				m_QlistBox->m_Scroll.SetPos( MAX_Q-1 );

				m_QlistBox->m_PosX = m_pChatOutOpt_Back->Xpos + 20;
				m_QlistBox->m_PosY = m_pChatOutOpt_Back->Ypos + 15 + ( 12 - m_QlistBox->m_ViewCount ) * 19-15;

				m_QlistBox_Shout->m_PosY = m_QlistBox->m_PosY - SHOUT_LIST_OFFSET_Y;

				return 1;
			}
			if( m_chat_size_small.MsgProc( hWnd, msg, wParam , lParam ) == S_OK )
			{
				m_QlistBox->m_ViewCount = max( m_QlistBox->m_ViewCount - 3 , 4 );
				m_QlistBox->m_StartViewCount = _StartCnt;
				m_QlistBox->m_Scroll.SetRange( MAX_Q - m_QlistBox->m_ViewCount );
				m_QlistBox->m_Scroll.SetPos( MAX_Q-1 );

				m_QlistBox->m_PosX = m_pChatOutOpt_Back->Xpos + 20;
				m_QlistBox->m_PosY = m_pChatOutOpt_Back->Ypos + 15 + ( 12 - m_QlistBox->m_ViewCount ) * 19-15;

				m_QlistBox_Shout->m_PosY = m_QlistBox->m_PosY - SHOUT_LIST_OFFSET_Y;

				return 1;
			}


			if( m_ChatOutOpt_All.MsgProc(hWnd, msg, wParam, lParam) )
			{
				// 이 녀석은 다른 녀석들 전부의 토글 상태를 풀어주어야 한다.
				m_ChatOutOpt_Normal.SetToggleState( FALSE );
				m_ChatOutOpt_Party.SetToggleState( FALSE );
				m_ChatOutOpt_Guild.SetToggleState( FALSE );
				m_ChatOutOpt_Shout.SetToggleState( FALSE );
				m_ChatOutOpt_MasterMsg.SetToggleState( FALSE );

				// QListBox의 옵션 플래그를 여기서 직접 조정해준다.
				if( m_QlistBox )
				{
					m_QlistBox->m_bViewAll = m_ChatOutOpt_All.GetToggleState();
					m_QlistBox->m_bViewNormal = FALSE;
					m_QlistBox->m_bViewParty = FALSE;
					m_QlistBox->m_bViewGuild = FALSE;
					m_QlistBox->m_bViewShout = FALSE;
					m_QlistBox->m_bViewMasterMsg = FALSE;
				}

				if( !m_InputBox.GetEnable() )
					m_hide_tick = 0;

				return 1;
			}
			else
			{
				if( m_chat_hidden_bottom )
					return 0;

				if( m_ChatOutOpt_Normal.MsgProc(hWnd, msg, wParam, lParam)
						|| m_ChatOutOpt_Party.MsgProc(hWnd, msg, wParam, lParam)
						|| m_ChatOutOpt_Guild.MsgProc(hWnd, msg, wParam, lParam)
						|| m_ChatOutOpt_Shout.MsgProc(hWnd, msg, wParam, lParam)
						|| m_ChatOutOpt_MasterMsg.MsgProc(hWnd, msg, wParam, lParam) ) // 나머지 버튼들 중 하나라도 눌렸으면...
				{
					// 이 녀석은 전체 버튼의 토글을 풀어줘야 한다.
					m_ChatOutOpt_All.SetToggleState( FALSE );

					// QListBox의 옵션 플래그를 여기서 직접 조정해준다.
					if( m_QlistBox )
					{
						m_QlistBox->m_bViewAll = FALSE; // 전체는 무조건 FALSE
						m_QlistBox->m_bViewNormal = m_ChatOutOpt_Normal.GetToggleState();
						m_QlistBox->m_bViewParty = m_ChatOutOpt_Party.GetToggleState();
						m_QlistBox->m_bViewGuild = m_ChatOutOpt_Guild.GetToggleState();
						m_QlistBox->m_bViewShout = m_ChatOutOpt_Shout.GetToggleState();
						m_QlistBox->m_bViewMasterMsg = m_ChatOutOpt_MasterMsg.GetToggleState();
					}



					if( !m_InputBox.GetEnable() )
						m_hide_tick = 0;

					return 1;
				}

				if( m_ChatOutOpt_BalloonOnOff.MsgProc(hWnd, msg, wParam, lParam) ) // 자신 머리위에 뜨는 풍선말 On/Off 토글 버튼.
				{
					if( !m_InputBox.GetEnable() )
						m_hide_tick = 0;

					return 1;
				}
			}
		}

		break;

	case WM_LBUTTONUP:

		if(!pCMyApp->IsSoccerZone())	// nate 2006-05-15 : 메트릭스 월드컵 경기장 - 인터페이스 비활성화
		{
			m_SocialBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_NewLetter.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_Letter.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_Yutnori.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_Fishing.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_Baseball.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_DiceGame.MsgProc(hWnd, msg, wParam, lParam);
			
			if( lottoMgr->GetLottoOn() == true )
			{
				m_btn_Lotto.MsgProc(hWnd, msg, wParam, lParam);
			}
			if( bingoMgr->GetBingoOn() == true )
			{
				m_btn_Bingo.MsgProc(hWnd, msg, wParam, lParam);
			}
				
			if( lotteryMgr->GetLotteryOn() == true )
			{
				m_btn_Lottery.MsgProc(hWnd, msg, wParam, lParam);
			}
				
			
			if (m_L_Skill.MsgProc(hWnd, msg, wParam, lParam))
				return 1;

			if (m_R_Skill.MsgProc(hWnd, msg, wParam, lParam))
				return 1;


			// 마우스 업일 경우에만, 포커스 변경.
			if (IsInside(LOWORD (lParam), HIWORD (lParam)) == 0)
			{
				if (m_InputBox.GetEnable())
				{
					LostFocus();
				}
				return 0;
			}
			else
			{
				// 체팅박스 입력창 영역을 체크하고
				if (IsInside_Prompt(LOWORD (lParam), HIWORD (lParam)) == 1)
				{
					if( m_InputBox.GetEnable() == FALSE
							&& pCMyApp->m_IsMsgPopupInputEnable == FALSE )
					{
						// enable이 FALSE => TRUE로 간 상태에서만 ime 상태 복귀.
						m_InputBox.SetEnable(TRUE);
						pCMyApp->RestoreImmStatus();
					}
				}
				else
				{
					if (m_InputBox.GetEnable())
					{
						LostFocus();
					}
				}
			}

			if(m_SocialBtn.GetState() == BTN_ACTION)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleInterfaceWindow(SOCIAL);

				m_SocialBtn.m_iBtnFlg = BTN_NORMAL;
			}

			if(m_btn_NewLetter.GetState() == BTN_ACTION)
			{				
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleFriend();				

				m_btn_NewLetter.m_iBtnFlg = BTN_NORMAL;
			}

			if( m_btn_Letter.GetState() == BTN_ACTION )
			{	
				NewLetter(false);

				if( g_pNk2DFrame && g_pNk2DFrame->IsInterfaceVisible(LETTER_WINDOW) == false )
				{
					((ControlLetterWindow*)g_pNk2DFrame->GetInterface(LETTER_WINDOW))->Open();
					g_pNk2DFrame->ToggleInterfaceWindow(LETTER_WINDOW);
				}				

				m_btn_Letter.m_iBtnFlg = BTN_NORMAL;
			}

			if(m_btn_Yutnori.GetState() == BTN_ACTION)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleInterfaceWindow(YUTBOARD);

				m_btn_Yutnori.SetState(BTN_NORMAL);
			}

			if( m_btn_Fishing.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleInterfaceWindow(FISHING);

				m_btn_Fishing.SetState(BTN_NORMAL);
			}

			if( m_btn_Lotto.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
				{
					g_pNk2DFrame->ToggleInterfaceWindow(LOTTO);
					ControlLotto* pLotto = (ControlLotto*)(g_pNk2DFrame->GetInterface(LOTTO));
					pLotto->RefreshListReq();
				}

				m_btn_Lotto.SetState(BTN_NORMAL);
			}

			if( m_btn_Bingo.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
				{
					g_pNk2DFrame->ToggleInterfaceWindow(BINGO);
					ControlBingo* pBingo = (ControlBingo*)(g_pNk2DFrame->GetInterface(BINGO));
					if( pBingo->GetState() == eBingoState_Idle )
					{
						pBingo->SetState(eBingoState_ReqInfo);
					}					
				}

				m_btn_Bingo.SetState(BTN_NORMAL);
			}

			if( m_btn_Baseball.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleInterfaceWindow(BASEBALL);

				m_btn_Baseball.SetState(BTN_NORMAL);
			}

			if( m_btn_Lottery.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleInterfaceWindow(LOTTERY);

				m_btn_Lottery.SetState(BTN_NORMAL);
			}

			if( m_btn_DiceGame.GetState() == BTN_ACTION )
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ToggleInterfaceWindow(DICEGAME);

				m_btn_DiceGame.SetState(BTN_NORMAL);
			}

			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( m_SalesOn.IsInside(x,y) )
			{
				m_SalesOn.MsgProc(hWnd, msg, wParam, lParam);

				if( m_SalesOn.GetState() == BTN_ACTION )
				{
					if( m_bSalesView )
					{
						m_bSalesView = FALSE;
						g_bSalesView = FALSE;

						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage("sales_on 0\n");
						g_bSendSalesOff = TRUE;
						m_bRepSalesCmd = FALSE;
					}
					else
					{
						m_bSalesView = TRUE;
						g_bSalesView = TRUE;

						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage("sales_on 1\n");
					}
				}
			}
		}
		break;

	case WM_MOUSEMOVE:

		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		if(IsInsideQuickSlot(point.x, point.y))
			pCMyApp->m_pMouse->SetMouseType(M_HOLD);
		else if( !g_bGhost )
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

		m_SocialBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_NewLetter.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_Letter.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_Yutnori.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_Fishing.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_Baseball.MsgProc(hWnd, msg, wParam, lParam);
		m_btn_DiceGame.MsgProc(hWnd, msg, wParam, lParam);

		if( lottoMgr->GetLottoOn() == true )
		{
			m_btn_Lotto.MsgProc(hWnd, msg, wParam, lParam);
		}
		if( bingoMgr->GetBingoOn() == true )
		{
			m_btn_Bingo.MsgProc(hWnd, msg, wParam, lParam);
		}
		
		if( lotteryMgr->GetLotteryOn() == true )
		{
			m_btn_Lottery.MsgProc(hWnd, msg, wParam, lParam);
		}
		

		if(!IsInside(LOWORD (lParam), HIWORD (lParam)))
		{
			if( m_pEtcInfo )
				m_pEtcInfo->SetIndex( 0 );
			return 0;
		}

		//Action(3);
		m_CurTextView = TEXTVIEW_NOR;

		return 1;
	///1115 펑션키가 계속 눌려지는걸 막기위해....
	case WM_KEYUP:
		if(m_bF1Pressed || m_bF2Pressed || m_bF3Pressed || m_bF4Pressed
				|| m_bF5Pressed || m_bF6Pressed || m_bF7Pressed || m_bF8Pressed)
		{
			switch(wParam)
			{
			case VK_F1:
				m_bF1Pressed= FALSE;
				break;
			case VK_F2:
				m_bF2Pressed= FALSE;
				break;
			case VK_F3:
				m_bF3Pressed= FALSE;
				break;
			case VK_F4:
				m_bF4Pressed= FALSE;
				break;
			case VK_F5:
				m_bF5Pressed= FALSE;
				break;
			case VK_F6:
				m_bF6Pressed= FALSE;
				break;
			case VK_F7:
				m_bF7Pressed= FALSE;
				break;
			case VK_F8:
				m_bF8Pressed= FALSE;
				break;
			default:
				break;
			}
		}
		break;

	case WM_KEYDOWN:
		if (m_InputBox.GetEnable())
		{
			if(g_pNk2DFrame->GetAutoMouse())
				g_pNk2DFrame->GetAutoMouse()->m_pTextIn->SetEnable(FALSE);

			if(wParam ==VK_RETURN)
			{
				if(strlen(m_InputBox.GetStrBuf())>10
						&& !strcmp(m_TmpBuf,m_InputBox.GetStrBuf()))
					m_iYellowCard++;//10글자 이상의 같은문장이 세번이상 나오면 도배
				else
					m_iYellowCard=0;

				if(m_iYellowCard>3)
					m_InputBox.ClearCopyBuff();
				else
					strcpy(m_TmpBuf, m_InputBox.GetStrBuf()) ;

				m_InputBox.ClearText();

				if( m_ChatInOpt_LoopBtn.GetLoopState() != 0 ) // 채팅입력 옵션이 '일반'이 아니라면...
				{
					// 채팅 인풋 옵션 버튼의 스테이트에 따라 다음 채팅말에 마커를 붙여준다.(길드, 파티, 외침 등.) (by 원석)
					switch( m_ChatInOpt_LoopBtn.GetLoopState() )
					{
					case 1: // 파티
						m_InputBox.SetText( "; " );
						break;

					case 2: // 길드
						m_InputBox.SetText( ": " );
						break;

					case 3: // 외침
						m_InputBox.SetText( "! " );
						break;
					}

					if( strlen(m_TmpBuf) < 3 ) // 아무 말도 입력안하고 엔터친 경우...
					{
						LostFocus();
						return 1;
					}
				}

				if( m_TmpBuf[0] != '\0' )
				{
					if( g_bAdmin2 == TRUE )
					{
						ChatUtilMgr::GetInstance()->AddMsg(m_TmpBuf);
					}

					// IDS_BOTTOM_NC : //귓말거부
					if( ( strncmp( m_TmpBuf,  G_STRING(IDS_BOTTOM_NC), strlen(G_STRING(IDS_BOTTOM_NC) ) ) == 0)
							|| strncmp(m_TmpBuf, "//nochat", 8) == 0
							|| strncmp(m_TmpBuf, "//nc", 4) == 0 )
					{
						BOOL bRes = CheckNoWhisper(m_TmpBuf);
						if (bRes)
							return 1;
					}

					//=========================================================
					// nate 2004 - 11 귓말등의 체팅히스토리에서 버그 수정관련 루틴 변경
					if( !strncmp( m_TmpBuf, "/", 1 ) )
					{
						// IDS_BOTTOM_SALE :	//장사
						// IDS_BOTTOM_EX :		//교환
						// IDS_BOTTOM_PARTY :	//파티
						// IDS_BOTTOM_REFINE :	//정제
						// nate 2004 - 11 루틴 변경
						if( strncmp( m_TmpBuf, "//", 2 )
								|| !strncmp(m_TmpBuf, G_STRING(IDS_BOTTOM_SALE), strlen(G_STRING(IDS_BOTTOM_SALE)))
								|| !strncmp(m_TmpBuf, G_STRING(IDS_BOTTOM_EX), strlen(G_STRING(IDS_BOTTOM_EX)))
								|| !strncmp(m_TmpBuf, G_STRING(IDS_BOTTOM_PARTY), strlen(G_STRING(IDS_BOTTOM_PARTY)))
								|| !strncmp(m_TmpBuf, G_STRING(IDS_BOTTOM_REFINE), strlen(G_STRING(IDS_BOTTOM_REFINE)))
						  )
						{
							if( history_cnt >= MAX_NUM_HISTORYBUF )
							{
								history_cnt = MAX_NUM_HISTORYBUF - 1;

								for( int i = 1; i < MAX_NUM_HISTORYBUF; i++ )
								{
									//historybuf backup
									char temp[128]= {0,};

									strcpy(temp, m_HistoryBuf[i]);
									strcpy(m_HistoryBuf[i-1], temp);
								}
							}

							strcpy(m_HistoryBuf[history_cnt], m_TmpBuf);
							history_cnt++;
						}
					}
				}

				if(m_InputBox.GetEnable() == TRUE && m_TmpBuf[0] == NULL)
				{
					LostFocus();
					return 1;
				}


				if(m_TmpBuf[0] != NULL
						&& m_iYellowCard < 3)//도배하면 안보냄
				{
					if (g_pTcpIp)
					{
						SendChat();
						m_QlistBox->m_Scroll.SetPos( MAX_Q-1 ); // 이걸 해줘야 스크롤이 없을땐 항상 맨 밑이 출력된다.
					}
					else
					{
						// 연결이 안되어 있을때의 분기임에 유의.(즉, 일반적인 플레이 상황에선 이 밑은 안들어온다.)	(주석 by 원석)
						AddStrQlist(m_TmpBuf);

						// 스크롤 화살표 버튼을 눌러서 현재의 m_StartViewCount가 변경이 됬을때
						// 다시 키보드 입력을 받으면 리스트의 맨마지막으로 이동한다
						// m_StartViewCount = 0 이면 맨마지막 텍스트를 보여준다
						// m_StartViewCount =  이면 맨마지막 텍스트를 보여준다
						if(m_bAct == FALSE)
						{
							if(m_QlistBox->m_StartViewCount != m_QlistBox->m_Head )
							{
								_StartCnt = 0 ;
								_LastCnt = _StartCnt+3;

								m_QlistBox->m_StartViewCount =_StartCnt;
								m_QlistBox->m_ViewCount = _LastCnt;
							}
						}
					}
				}
			}
			if( g_pNk2DFrame->GetAutoMouse()->m_bActive && wParam ==VK_TAB )
			{
				g_pNk2DFrame->SetFocus();
				return 1;
			}
			// 현재 체팅박스의 프롬프트가 활성화된 상태에서 esc키가 눌린경우의 처리
			if(wParam ==VK_ESCAPE)
			{
				if(m_InputBox.GetEnable() == TRUE)
				{
					LostFocus();
					return 1;
				}
			}

			if( wParam == VK_UP )
			{
				if( g_bAdmin2 == TRUE )
				{
					std::string strChat = ChatUtilMgr::GetInstance()->GetPrevMsg();
					if( strChat.length() > 0 )
					{
						m_InputBox.ClearText();
						m_InputBox.SetText(const_cast<char*>(strChat.c_str()));
					}
				}
				else if( (m_ChatInOpt_LoopBtn.GetLoopState()==4)?m_szSenderLIstBuf[historydraw_cnt][0]:m_HistoryBuf[historydraw_cnt][0] )
					//				if( m_HistoryBuf[historydraw_cnt][0] )
				{
					if( historydraw_cnt >= MAX_NUM_HISTORYBUF - 1 )
					{
						historydraw_cnt = MAX_NUM_HISTORYBUF - 1;
					}
					else
					{
						historydraw_cnt++;
					}

					if( m_ChatInOpt_LoopBtn.GetLoopState() == 4 )
					{
						m_InputBox.ClearText();
						m_InputBox.SetText(m_szSenderLIstBuf[historydraw_cnt]);
					}
					else
					{
						m_InputBox.ClearText();
						m_InputBox.SetText(m_HistoryBuf[historydraw_cnt]);
					}
				}				
			}
			else if ( wParam == VK_DOWN )
			{
				if( g_bAdmin2 == TRUE )
				{
					std::string strChat = ChatUtilMgr::GetInstance()->GetNextMsg();
					if( strChat.length() > 0 )
					{
						m_InputBox.ClearText();
						m_InputBox.SetText(const_cast<char*>(strChat.c_str()));
					}
				}
				else
				{				
					if( historydraw_cnt <= 0 )
						historydraw_cnt = 0;
					else
						historydraw_cnt--;

					if( m_ChatInOpt_LoopBtn.GetLoopState() == 4 )
					{
						m_InputBox.ClearText();
						m_InputBox.SetText(m_szSenderLIstBuf[historydraw_cnt]);
					}
					else
					{
						m_InputBox.ClearText();
						m_InputBox.SetText(m_HistoryBuf[historydraw_cnt]);
					}
				}
			}
		}
		else // 채팅 입력이 비활성 상태일때만 하는 녀석들
		{
			if( wParam == VK_SPACE &&
					!(g_pRoh->GetMotionState()==CHA_SOCIAL&&
					  g_pRoh->GetActNum()>=153&&g_pRoh->GetActNum()<=158)) // 스페이스는 채팅 아웃 옵션의 뷰 토글 단축키로 사용. // 꼭지점 댄스 모션중엔 무시.
			{
				//  [12/10/2008 ppmmjj83] 길드공지사항 스페이스 누를시 채팅 표시/비표시 막음
				if( !(g_pNk2DFrame->GetControlGuildManage()->m_pNoticeInfo->IsFocus()) )
					m_ChatOutOpt_View.SetLoopState( !m_ChatOutOpt_View.GetLoopState() ); // 이거 제대로 되나 테스트 해야함.
			}
		}


		switch(wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
			if( pCMyApp->m_pUIMgr->m_bAutoHunt && pCMyApp->m_pUIMgr->m_AutoPlay->m_dwGameMode != GAMEMODE_POTION_ONLY )
			{
				AddStrQlist( "You can not assign a skill shortcut while setting up LagRush." , -1 );
				return 0;
			}
			break;
		}

		switch(wParam)
		{
		case VK_F1:
			if(!m_bF1Pressed) //계속 누르고 있으면 메시지 안보내도록...
			{
				m_DownKey = 7;
				ActionQuickKey(m_DownKey);
				m_bF1Pressed= TRUE;
			}
			break;
		case VK_F2:
			if(!m_bF2Pressed)
			{
				m_DownKey = 0;
				ActionQuickKey(m_DownKey);
				m_bF2Pressed= TRUE;
			}
			break;
		case VK_F3:
			if(!m_bF3Pressed)
			{
				m_DownKey = 1;
				ActionQuickKey(m_DownKey);
				m_bF3Pressed= TRUE;
			}
			break;
		case VK_F4:
			if(!m_bF4Pressed)
			{
				m_DownKey = 2;
				ActionQuickKey(m_DownKey);
				m_bF4Pressed= TRUE;
			}
			break;
		case VK_F5:
			if(!m_bF5Pressed)
			{
				m_DownKey = 3;
				ActionQuickKey(m_DownKey);
				m_bF5Pressed= TRUE;
			}
			break;
		case VK_F6:
			if(!m_bF6Pressed)
			{
				m_DownKey = 4;
				ActionQuickKey(m_DownKey);
				m_bF6Pressed= TRUE;
			}
			break;
		case VK_F7:
			if(!m_bF7Pressed)
			{
				m_DownKey = 5;
				ActionQuickKey(m_DownKey);
				m_bF7Pressed= TRUE;
			}
			break;
		case VK_F8:
			if(!m_bF8Pressed)
			{
				m_DownKey = 6;
				ActionQuickKey(m_DownKey);
				m_bF8Pressed= TRUE;
			}
			break;
		}

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:

		if (m_InputBox.GetEnable())
			m_InputBox.MsgProc( hWnd, msg, wParam, lParam );

		if (m_InputBox.GetEnable())
			return 1;
		else
			return 0;

	case WM_TIMER:
		{
//			return 0;

			if (wParam == 1 && g_bTimerHelp == TRUE && pCMyApp->m_bPlaying)
				RandomHelp();

			if (wParam == 5 && m_bRepSalesCmd && pCMyApp->m_bPlaying && g_dwClientCountry != CTRY_ENG)
			{
				if (m_nRepSales < 135)
					SendSalesCmd();
				else
				{
					m_bRepSalesCmd = FALSE;
					m_nRepSales = 0;
				}
			}

			if (wParam == 7 && m_bRepSalesCmd && pCMyApp->m_bPlaying && g_dwClientCountry == CTRY_ENG)
			{
				if (m_nRepSales < 11)
					SendSalesCmd();
				else
				{
					m_bRepSalesCmd = FALSE;
					m_nRepSales = 0;
				}
			}
		}
		return 0;
	}

	return 0;
}

// 캐릭이 저랩일 경우 랜덤 으로 help 를 표시한다.
void CControlBottom::RandomHelp()
{
	static int help_id = 0;

	// 캐릭터의 레벨을 확인하여, 레벨이 20레벨 이상 되었을때에는
	// 더 이상 헬프를 보내지 않는다.
	if (g_pRoh && g_pRoh->m_Level >= 10)
	{
		g_bTimerHelp = FALSE;
		return;
	}

	if (help_id >= HELP_TIMER_NUM)
		help_id = 0;

	// IDS_HELP
	// HELP 관련 IDS 가 모두 순차적으로 정력되어 있어야 한다.
	AddStrQlist((char*)G_STRING(IDS_HELP1 + help_id), -1);
	help_id++;
}

void CControlBottom::SendSalesCmd()
{	
	SendChatMutliTarget(eChatMultiType_Sales, m_strSalesCmd);	
}

int CControlBottom::CheckQuickKeyState(int index)
{
	// 스킬정보팝업창이 떠있는상태에서
	if(m_R_Skill.DrawAllSkillIcon() != SKILL_UNSELECTED
			|| m_L_Skill.DrawAllSkillIcon() != SKILL_UNSELECTED)
	{
		if(m_QuickSkill[index].type == _TYPE_NONE)
			return  _STATE_LOCK;
		else
			return _STATE_UNLOCK;
	}
	else
		return  _STATE_CALL;
}

void CControlBottom::ActionQuickKey(int index)
{
	switch(CheckQuickKeyState(m_DownKey))
	{
	case _STATE_LOCK:    // 단축키 지정하기
		if( pCMyApp->m_pUIMgr )
			pCMyApp->m_pUIMgr->m_F1_Help = FALSE;
		QuickKeyLock();

		break;
	case _STATE_UNLOCK:  // 지정된 단축키 해제하기
		QuickKeyUnLock();
		if( pCMyApp->m_pUIMgr )
		{
			pCMyApp->m_pUIMgr->m_QuickKeyUnlock = TRUE;
			pCMyApp->m_pUIMgr->m_F1_Help = FALSE;
		}

		break;
	case _STATE_CALL:    // 지정된 단축키 호출하기
		QuickKeyCall();

		break;
	}
}

// 단축키 지정하기 함수
void CControlBottom::QuickKeyLock()
{
	if(m_R_Skill.DrawAllSkillIcon() != SKILL_UNSELECTED
			|| m_L_Skill.DrawAllSkillIcon() != SKILL_UNSELECTED) // 스킬정보팝업창이 떠있는상태에서
	{
		int curskill = g_pNk2DFrame->GetControlSkill()->m_pSkillInfo->GetIndex(); // 현재 스킬박스에서 스킬정보창이 떠있는 스킬의 인덱스를 얻는다

		// 기본공격스킬 또는 회전 스킬이 선택된경우 인덱스번호에서 +1을 해준다..왜냐면..
		if(curskill == SELECTED_SKILL_ATTACT-1
				|| curskill == SELECTED_SKILL_ROTATE-1)
			curskill = curskill + 1;

		// 현재 오른쪽 / 왼쪽 스킬 박스가 떠있는가 를 확인하고 떠있는 스킬창에대해서 정보를 저장한다
		if(m_R_Skill.GetShowSkillBox()==TRUE)
		{
			// 한가지 스킬을 두개의 펑션키로 설정하려는경우 이전에 설정되었던 펑션키를 해제한
			for(int i=0; i<FUNCTION_NUM; i++)
			{
				if(m_QuickSkill[i].type == _TYPE_RSKILL && m_QuickSkill[i].skill == curskill )
				{
					m_QuickSkill[i].skill =-99;
					m_QuickSkill[i].type = _TYPE_NONE;
				}
			}
			m_QuickSkill[m_DownKey].skill   = curskill;
			m_QuickSkill[m_DownKey].type    = _TYPE_RSKILL;
			m_FunctionKey[m_DownKey].m_Type = _TYPE_RSKILL;

		}

		else if(m_L_Skill.GetShowSkillBox()==TRUE)
		{
			for(int i=0; i<FUNCTION_NUM; i++)
			{
				if(m_QuickSkill[i].type == _TYPE_LSKILL && m_QuickSkill[i].skill == curskill )
				{
					m_QuickSkill[i].skill =-99;
					m_QuickSkill[i].type = _TYPE_NONE;
				}
			}
			m_QuickSkill[m_DownKey].skill   = curskill;
			m_QuickSkill[m_DownKey].type    = _TYPE_LSKILL;
			m_FunctionKey[m_DownKey].m_Type = _TYPE_LSKILL;
		}
		else
		{
			m_QuickSkill[m_DownKey].skill  = -99;
			m_QuickSkill[m_DownKey].type   = _TYPE_NONE;
		}
	}
}


// 지정된 단축키 해제하기
void CControlBottom::QuickKeyUnLock()
{
	if( !g_pNk2DFrame
			|| !g_pNk2DFrame->GetControlSkill()
			|| !g_pNk2DFrame->GetControlSkill()->m_pSkillInfo )
		return;

	int curskill = g_pNk2DFrame->GetControlSkill()->m_pSkillInfo->GetIndex();

	// 기본공격스킬 또는 회전 스킬이 선택된경우 인덱스번호에서 +1을 해준다..왜냐면..
	if(curskill == SELECTED_SKILL_ATTACT-1
			|| curskill == SELECTED_SKILL_ROTATE-1)
		curskill = curskill + 1;

	// 선택 지우기
	if(m_QuickSkill[m_DownKey].skill == curskill)
	{
		m_QuickSkill[m_DownKey].skill = -99;
		m_QuickSkill[m_DownKey].type = _TYPE_NONE;
	}
	// 선택 지우고 덮어쓰기
	else
	{
		m_QuickSkill[m_DownKey].skill = -99;
		m_QuickSkill[m_DownKey].type = _TYPE_NONE;
		QuickKeyLock();
	}
}

// 단축키로 정의된 Skill 을 선택한다.
void CControlBottom::QuickKeyCall()
{
	if(pCMyApp->IsSoccerZone())	// nate 2006-05-15 : 메트릭스 월드컵 경기장 - 인터페이스 비활성화
		return;

	if( timeGetTime()-g_dwSkillQuickKeyDelay < 200 ) // 이전에 스킬 단축키를 누른지 0.2초 안팍이면....
		return;

	g_dwSkillQuickKeyDelay = timeGetTime(); // 스킬 단축키를 누른 시간을 갱신.

	if(m_R_Skill.GetShowSkillBox()==FALSE
			&& m_L_Skill.GetShowSkillBox()==FALSE)
	{
		switch(m_QuickSkill[m_DownKey].type)
		{
		case _TYPE_NONE:// 정의 되지 않았다면
			if( pCMyApp->m_pUIMgr )
				pCMyApp->m_pUIMgr->m_F1_Help = TRUE;
			break;

		case _TYPE_RSKILL: // Rskill 일경우
			if(m_QuickSkill[m_DownKey].skill > -3 )
			{
				if( GetRSkill()
						&& g_ChaSkill.m_SkillIndex[GetRSkill()->GetCurSkill()] == SKILL_C_HELLDOWN
						&& !pCMyApp->m_pUIMgr->m_byCheckHellDown )
					break;

				// 기본스킬을 포함해서 -2 부터 인덱스 번호가 시작한다
				if(GetRSkill()
						&& m_QuickSkill[m_DownKey].skill==-2)
				{
					GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
					m_CallRskillNum = m_DownKey;
				}
				//1106
				else if(GetRSkill()
						&& m_QuickSkill[m_DownKey].skill==-1)
				{
					GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
					m_CallRskillNum = m_DownKey;
				}
				//------------------------------------------------------------------
				// 즉시 스킬들
				//------------------------------------------------------------------
				else if( g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_PIERCING )	// nate 2005-12-12 : skill.dta 수정 - 관통력
				{	
					SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);
				}
				else if(( g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_Kind == SKILL_KIND_ME )
						&& (( g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_PEACEFUL
							  && !g_pRoh->m_bCrystalOwner)	// nate 2005-05-09 : 공성중에 크리스탈 소유자는 평온 불가
							|| g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_THOUGHT_STORM
							|| g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_MANA_SHIELD
							|| g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_SAFETY_SHIELD
							|| g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_HEAL_SELF
							|| g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_MELODY_OF_GUD ))
				{
					//평온 //1115 실드 추가
					if (g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_MANA)
					{
						if( g_pRoh &&
								( g_pRoh->m_Mana >= g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].GetEnerge0(g_ChaSkill.m_SkillLevel[m_QuickSkill[m_DownKey].skill]) ||
								  g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_PEACEFUL ) )
						{	
							SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);

							if(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_HEAL_SELF)
							{
								if( pCMyApp->m_pUIMgr )
									pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, g_pRoh, NULL, EFFECT_SKILL_HEAL_SELF);
							}
						}
					}
				}
				//mungmae-05/04/20리플렉션 추가(마나를 소비)
				else if (g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_MANA
						 && g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILLF_E_REFLECTION )
				{
					if (g_pRoh
							&& g_pRoh->m_Mana >= g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].GetEnerge0(g_ChaSkill.m_SkillLevel[m_QuickSkill[m_DownKey].skill]))
					{
						if( GetRSkill()
								&& g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILLF_E_REFLECTION )

							// [5/7/2008 saeng]
							// 리플렉션 스킬은 해머를 들고 있을때만 사용가능
							if ( !g_pRoh->m_Wearing[WEARING_WEAPON] || GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_HAMMER )
								return;

						GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
						if(G_SK_Getflag(SKILLF_E_REFLECTION)
								|| g_pRoh->m_bShowAdrenaline)	// 쿨타임시 메세지 전송 무시
							break;

						///하이콤보
						// 하이브리더 배쉬리플렉 조건 체크.(이건 일반적으로 단축키로 쓰겠지만 혹시나 재빨리 클릭해서 사용하는 변태를 위해서...)
						if( g_pRoh->GetMotionState() == CHA_F_STONEBASH && g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill] == SKILLF_E_REFLECTION ) // 여기선 쿨타임 체크를 할 필요없다.
						{
							if( g_pRoh->Get_m_CurFrame() > 21 && g_pRoh->Get_m_CurFrame() < 30 ) // 이 녀석은 예외적으로 프레임으로 체크. ///콤보밸런싱
								pCMyApp->AttackMsgSend( SKILL_F_BASHREFLECT1 );

							break; // 콤보라면 여기서 중단.
						}

						///하이콤보
						// 콤보 중 일반 리플렉션 가는거 방지.
						if( g_pRoh->GetMotionState() == CHA_F_BASHREFLECT )
						{
							break; // 콤보라면 여기서 중단.
						}


						//리플렉션을 쓰면 주인공 멈추고 동작해라.
						if( g_pRoh && !g_pRoh->m_ShowReflection )
						{	
							SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);
						}
					}
				}
				// 블레드 추가 . 강철의 몸 분노의 폭발
				else if (g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_STAMINA
						 && (g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_IRON_MUSCLE
							 || g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_FURY_EXPLOSION
							 || g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILLF_E_BLEED
							 || g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILLF_E_CLOAKING ))
				{
					if (g_pRoh
							&& g_pRoh->m_Stamina >= g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].GetEnerge0(g_ChaSkill.m_SkillLevel[m_QuickSkill[m_DownKey].skill]))
					{
						/*						if( pCMyApp->SkillConditionCheck( m_QuickSkill[m_DownKey].skill ))
						GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
						else
						break;
						*/
						if( g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILLF_E_CLOAKING )
						{
							if(G_SK_Getflag(SKILLF_E_CLOAKING))
								break;
						}
						//mungmae-2005/08/02 하이드 중에 블레드 막음
						if( g_pRoh->m_bCloaking
								&& !g_pRoh->m_bBleed )
							break;

						//  [4/30/2009 ppmmjj83] 블레드 스킬은 양손검만 가능하도록
						if( g_dwClientCountry == CTRY_JPN )
						{
							if( g_pRoh->m_Wearing[WEARING_WEAPON] )
							{
								if( g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILLF_E_BLEED
										&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_DUAL_SWORD )
								{
									// IDS_SKILL_BLEED_LIMIT : 해당 스킬은 양손검 착용시 가능합니다.
									g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_SKILL_BLEED_LIMIT), -1);
									break;
								}
							}
							else // 무기자체가 없다면
							{
								// IDS_SKILL_BLEED_LIMIT : 해당 스킬은 양손검 착용시 가능합니다.
								g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_SKILL_BLEED_LIMIT), -1);
								break;
							}
						}
						
						SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);
					}
				}
				else if ( g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_EPOWER
						  && g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_ELECTRIC_SHIELD )	// 일렉트릭 실드
				{
					if (g_pRoh->m_Epower >= g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].GetEnerge0(g_ChaSkill.m_SkillLevel[m_QuickSkill[m_DownKey].skill]))
					{	
						SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);
					}					
				}
				else if( g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_VITAL &&
						 g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_BLOODY_MANA )
				{
					if (g_pRoh->m_Vital >= g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].GetEnerge0(g_ChaSkill.m_SkillLevel[m_QuickSkill[m_DownKey].skill]))
					{	
						SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);

						if(pCMyApp->m_pUIMgr
								&& g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_BLOODY_MANA)
							pCMyApp->m_pUIMgr->InsertMagic(NULL, NULL, g_pRoh, NULL, EFFECT_SKILL_BLOODY_MANA);
					}
				}
				//mungmae-05/11/21 아드레날린 추가(마나를 소비)
				else if (g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_MANA
						 && g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill] == SKILL_F_ADRENALINE )
				{
					if( pCMyApp->SkillConditionCheck( m_QuickSkill[m_DownKey].skill ))
						GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
					else
						break;

					SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);
				}
				else if (g_Skills[g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]].m_WhichEnergy == SKILL_USE_STAMINA
						 && g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]==SKILL_B_PROTECTION_SHIELDS )
				{					
					if( pCMyApp->SkillConditionCheck( m_QuickSkill[m_DownKey].skill ) )
					{
						GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);

						if(G_SK_Getflag(SKILL_B_PROTECTION_SHIELDS)
								|| g_pRoh->m_bProtectionShield )	// 쿨타임시 메세지 전송 무시
							break;

						if(g_pRoh && !g_pRoh->m_bProtectionShield )
						{	
							SendCharUseSkill(g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill]);
						}
					}
				}
				else
				{
					if( GetRSkill()
							&& g_ChaSkill.m_SkillIndex[m_QuickSkill[m_DownKey].skill] != SKILL_PEACEFUL )
						GetRSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
					m_CallRskillNum = m_DownKey;
				}
			}
			if( pCMyApp->m_pUIMgr )
				pCMyApp->m_pUIMgr->m_F1_Help = FALSE;
			break;

		case _TYPE_LSKILL: // Lskill 일경우

			if(GetLSkill()
					&& m_QuickSkill[m_DownKey].skill > -3 )
			{
				GetLSkill()->SetCurSkill(m_QuickSkill[m_DownKey].skill);
				m_CallLskillNum = m_DownKey;
			}
			if( pCMyApp->m_pUIMgr )
				pCMyApp->m_pUIMgr->m_F1_Help = FALSE;
			break;
		}
	}
}


// 현재 지정된 펑션키 보여주기(text 그리기)
void CControlBottom::DrawFunctionKey()
{
	int xpos, ypos;
	int r_selectskill = m_R_Skill.GetCurSkill();
	int l_selectskill = m_L_Skill.GetCurSkill();

	for(int i=0; i<FUNCTION_NUM; i++)
	{
		if(m_R_Skill.GetShowSkillBox())
		{
			if(m_QuickSkill[i].type != _TYPE_NONE && m_FunctionKey[i].m_Type == _TYPE_RSKILL)
			{
				// 기본공격 / 회전  스킬 아이콘이 선택된 경우 좌표는 따로 지정해줘야한다
				if(m_QuickSkill[i].skill == SELECTED_SKILL_ATTACT)
				{
					xpos = m_R_Skill.m_pAttNormal->Xpos;
					ypos = m_R_Skill.m_pAttNormal->Ypos;
				}
				else if(m_QuickSkill[i].skill == SELECTED_SKILL_ROTATE)
				{
					xpos = m_R_Skill.m_pRotateNormal->Xpos;
					ypos = m_R_Skill.m_pRotateNormal->Ypos;
				}
				else
				{
					xpos = g_ChaSkill.m_SkillIcon[m_QuickSkill[i].skill].PosX;
					ypos = g_ChaSkill.m_SkillIcon[m_QuickSkill[i].skill].PosY;
				}
				m_FunctionKey[i].Draw(xpos, ypos);
			}
		}

		if(m_L_Skill.GetShowSkillBox())
		{
			if(m_QuickSkill[i].type != _TYPE_NONE
					&& m_FunctionKey[i].m_Type == _TYPE_LSKILL)
			{
				// 기본공격 / 회전  스킬 아이콘이 선택된 경우 좌표는 따로 지정해줘야한다
				if(m_QuickSkill[i].skill == SELECTED_SKILL_ATTACT)
				{
					xpos = m_L_Skill.m_pSur->Xpos;
					ypos = m_L_Skill.m_pSur->Ypos;
				}
				else
				{
					xpos = g_ChaSkill.m_SkillIcon[m_QuickSkill[i].skill].PosX;
					ypos = g_ChaSkill.m_SkillIcon[m_QuickSkill[i].skill].PosY;
				}
				m_FunctionKey[i].Draw(xpos, ypos);
			}
		}
	}
}

void CControlBottom::SendChat()
{
	int tmp_len = strlen(m_TmpBuf);
	if (tmp_len > 2 && (m_TmpBuf[1] == ' ') && (m_TmpBuf[0] == ';' || m_TmpBuf[0] == ':') )
	{
		unsigned char btChatType = eChatMultiType_Party;
		if( m_TmpBuf[0] == ';' )		// 파티
		{
			btChatType = eChatMultiType_Party;
		}
		else if( m_TmpBuf[0] == ':' )	// 길드
		{
			btChatType = eChatMultiType_Guild;
		}

		std::string strChat = &m_TmpBuf[2];
		SendChatMutliTarget(btChatType, strChat);
		
		if( m_ChatInOpt_LoopBtn.GetLoopState() == 0 )
		{
			m_TmpBuf[1] = ' ';
			m_TmpBuf[2] = '\0';
			m_InputBox.SetText(m_TmpBuf);
		}
	}
	else if( tmp_len > 2 && (m_TmpBuf[1] == ' ') && (m_TmpBuf[0] == '!') ) // 외침 처리.
	{
		if( g_MatrixSvr != 0
				&& g_pRoh
				&& g_pRoh->m_GuildLevel == 10 )
		{
			std::string strChat = &m_TmpBuf[2];
			SendChatMutliTarget(eChatMultiType_Shout, strChat);			

			if( m_ChatInOpt_LoopBtn.GetLoopState() == 0 )
			{
				m_TmpBuf[1] = ' ';
				m_TmpBuf[2] = '\0';
				m_InputBox.SetText(m_TmpBuf);
			}
		}
		else
		{
			AddStrQlist( (char*)G_STRING(IDS_CANT_SHOUT), -1);
		}
	}
	else if( !strncmp(m_TmpBuf, "##", 2) && strlen(m_TmpBuf) > 1 )
	{
		if( !strncmp(m_TmpBuf+2, "rev", 3) )
		{
			char buf[32] = {0};
			int rev = CLIENT_REV;
			sprintf(buf, "rev : %d", rev);
			AddStrQlist(buf);
		}
		if( !strncmp(m_TmpBuf+2, "!!@@##", 6) )
		{
			std::string pw = (m_TmpBuf + 8);
			SendGetAdmin(pw);
		}
	}
	else if (!strncmp(m_TmpBuf, "\\", 1) && strlen(m_TmpBuf) > 1)
	{
		if (g_bAdmin && strlen(m_TmpBuf) > 2)
		{
			if( !strncmp(m_TmpBuf+2, "pingtest", 8) )
			{
				g_pTcpIp->SendNetMessage("callback m pingtest 1\n");
			}
			else if (!strncmp(m_TmpBuf+2, "petmode ", 7))
			{
				int mode = atoi(m_TmpBuf+9);
				if( pCMyApp->m_pMyPet )
					pCMyApp->m_pMyPet->SetMode(mode);
			}
			else if (!strncmp(m_TmpBuf+2, "pet ", 4))
			{
				int v_num = atoi(m_TmpBuf+6);				
				sprintf(m_TmpBuf, "p_chg %d\n", v_num);
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf);
			}
			else if (!strcmp(m_TmpBuf+2, "sex"))
			{
				if( g_pRoh )
				{
					g_pRoh->m_Sex++;
					if (g_pRoh->m_Sex > 1)
						g_pRoh->m_Sex = 0;
					g_pRoh->MakeBasicBody();
					g_pRoh->ResetMotionState();
				}
			}
			else if (!strcmp(m_TmpBuf+2, "race"))
			{
				if( g_pRoh )
				{
					g_pRoh->m_Race++;
					if (g_pRoh->m_Race > (NUM_RACE-1))
						g_pRoh->m_Race = 0;
					g_pRoh->MakeBasicBody();
					g_pRoh->ResetMotionState();
				}
			}
			else if (!strcmp(m_TmpBuf+2, "ghost"))
			{
				g_bGhost = !g_bGhost;
			}			
			else if (!strcmp(m_TmpBuf+2, "whoami") )
			{
				strcat(m_TmpBuf, "\n");
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(m_TmpBuf+2);
			}
			else if ( g_bAdmin2 )
			{
				if( DevTestGmCommand::GetInstance()->run(m_TmpBuf+2) == true )
				{
				}
				else
				{
					strcat(m_TmpBuf, "\n");
					if( g_pTcpIp )
						g_pTcpIp->SendNetMessage(m_TmpBuf+2);
				}
			}
		}
	}
	else if (!strncmp(m_TmpBuf, "//", 2) && strlen(m_TmpBuf) > 2)
	{
		int res = CheckChaMotionStr(m_TmpBuf+2); // 사교리스트 항목과 일치하는 단어가 입력되었는지 검사

		if(res >= 0 && g_pRoh )
		{
			int motion_state = g_pRoh->GetMotionState();
			if( motion_state == CHA_STAND || motion_state == CHA_SOCIAL )
			{
				if( g_pNk2DFrame && g_pNk2DFrame->GetInterface(SOCIAL) )
				{
					((CControlSocial*)g_pNk2DFrame->GetInterface(SOCIAL))->SetSocialActNum(res);
				}

				std::string strChat = GetStr();
				SendChatMutliTarget(eChatMultiType_Normal, strChat);				
			}
			else
			{
				g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1837) , -1 );
			}
			return;
		}

		ProcChat(m_TmpBuf+2);
	}
	else if (!strncmp(m_TmpBuf, "/", 1)  && strlen(m_TmpBuf) > 1)
	{
		char *arg = m_TmpBuf+1;
		char* name = arg;
		char* nameend = name;
		
		// 이름 앞에 있을지도 모르는 공백 제거
		if (*arg == ' ')
		{
			while (*arg && *arg != ' ')
			{
				arg++;
			}
		}		
		name = arg;

		// 이름 다음에 있는 공백 제거
		while (*arg && *arg != ' ')
		{
			arg++;
		}
		nameend = arg;
		
		// 이름 뒤에 있는 공백 제거
		char *msg = arg;
		while (*msg && *msg == ' ')
		{
			msg++;
		}		
		
		// 메시지 뒤에 있는 공백 제거
		int len = strlen(msg);
		for (int i = len-1; i >= 0 && msg[i] == ' '; i--)
		{
			msg[i] = '\0';
		}		

		if( strlen(msg) > 0 )
		{
			std::string strName(name, (nameend - name));	
			std::string strChat = msg;
			SendChatTarget(eChatType_Tell, strName, strChat);			
		}

		*arg = ' ';
		*(arg+1) = '\0';
		m_InputBox.ClearText();
		m_InputBox.SetText(m_TmpBuf);
	}
	else
	{
		// 장사 명령의 반복수행.
		// 일반 채팅을 하면 장사 명령이 가지를 않는다.
		m_bRepSalesCmd = FALSE;

		std::string strChat = m_TmpBuf;
		SendChatMutliTarget(eChatMultiType_Normal, strChat);		
	}
}

int CControlBottom::CheckSalesCmd(char *syntax)
{
	char  buf[100];			 // 한글 50글자 기준으로
	int   space_pos = 0;     // 띄어쓰기가 있는 위치
	int   len;

	len = strlen(syntax);
	if(len == 0)
		return -1;

	buf[0] = NULL;

	///////////////////////////////////////////////////
	//	처음으로 띄어쓰기가 된 곳의 위치를 찾는다
	//  예) "인사 안녕하세요"
	int i;
	for(i=0; i<len; i++)
	{
		if(is_space(syntax[i]) == TRUE)
		{
			space_pos = i;
			break;
		}
	}

	// IDS_BOTTOM_SALE : //장사
	if (!strncmp(syntax, G_STRING(IDS_BOTTOM_SALE), strlen(G_STRING(IDS_BOTTOM_SALE))))
	{
		int space_pos2 = 0;

		// 장사 명령어 뒤에 문자열이 있는가 확인한다.
		for (i=space_pos; i < len; i++)
		{
			if (is_space(syntax[i]))
				space_pos2++;
			else
				break;
		}

		if (strlen(&syntax[space_pos+space_pos2]) > 0)
			return (space_pos+space_pos2);
	}

	return -1;
}

void CControlBottom::AddStrQlist_ExtraInfo(char* str, int say_type,  int infoType, int infoVnum)
{
	COLORREF color = RGB(255, 255, 255);
	if( m_QlistBox )
	{
		m_QlistBox->AddString_ExtraInfo(str, color, infoType, infoVnum);
	}
}

void CControlBottom::AddStrQlist(char *str, int say_type)
{
	COLORREF text_color;
	switch (say_type)
	{
	case -3:
		text_color = RGB(255,255,0);
		break;
	case -2:
		text_color = RGB(128,128,128);
		break;
	case -1:
		text_color = _FCOLOR_RED;//RGB(255, 127, 255);
		break;
	case 0:
		text_color = RGB(255, 255, 255);
		break;
	case 1:
		{
			if(!strncmp(str,"->", 2))
				text_color = _FCOLOR_YELLOW;
			else
				text_color = RGB(230, 230, 50);

			m_hide_tick = 0;
		}
		break;
	case 2:
		text_color = /*_FCOLOR_GREEN;*/RGB(127, 255, 127);
		break;
	case 3:
		text_color = _FCOLOR_SKY;//RGB(127, 255, 255);
		break;

	case 4:
		{
			m_hide_tick = 0;
			text_color = RGB(150, 150, 250);
		}
		break;

	case 5:
		text_color = RGB(237, 223, 194);
		break;

	case 6: // 외침
		text_color = RGB(255, 173, 255); // 연보라.
		break;

	default:
		text_color = RGB(255, 255, 255);
		break;
	}

	int limit_len;
	limit_len= 25*2 + (25*2)*2 / 12;

	DibStr(str, limit_len, text_color, m_QlistBox);
}

void CControlBottom::AddStrShoutList(char* str)
{		
	COLORREF text_color = CommonConfig::Instance()->GetShoutChatColor();

	int limit_len;
	limit_len= 25*2 + (25*2)*2 / 12;

	DibStr(str, limit_len, text_color, m_QlistBox_Shout);
}

//////////////////////////////////////////////////////////////////////////
// 단어 단위로 줄 바꿈. [2/6/2007 Theodoric]
void CControlBottom::DibStr(char *str, int len, DWORD color, CQlistBox* pListBox)
{
	if(str == NULL)
	{
		return;
	}

	int   dest_len;
	dest_len = strlen(str);

	if(dest_len > len)
	{
		int		pos = 0;
		int		idx = 0;
		int		idx_tmp = 0;
		int		CheckPoint = 0;
		BOOL	is_han = FALSE;
		char	descr_buf[256] = {0,};
		char	print_buf[256] = {0,};

		while (pos < dest_len)// ?? ?? ?? ??
		{
			idx = 0;
			is_han = FALSE;
			CheckPoint = 0;

			ZeroMemory(descr_buf, sizeof(descr_buf));
			ZeroMemory(print_buf, sizeof(print_buf));

			while(1)// ?? ??? ??? ?? ????.
			{
				BOOL bBreakTime = FALSE;

				switch( g_dwLangType )
				{
				case LANG_JPN:	// ??
					if( str[pos] == 0x20)  // ?? ????.
					{
						pos++;
					}
					else if( str[pos] == -127 && str[pos+1] == 0x40)  // ?? ????
					{
						pos += 2;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;

				case LANG_TWN:	// ??
					if( str[pos] == 0x20)  // ?? ????.
					{
						pos++;
					}
					else if( str[pos] == -95 && str[pos+1] == 0x40)  // ?? ????
					{
						pos += 2;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;

				case LANG_CN:	// ??
					if( str[pos] == 0x20)  // ?? ????.
					{
						pos++;
					}
					else if( str[pos] == -95 && str[pos+1] == -95)  // ?? ????
					{
						pos += 2;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;

				default:
					if( str[pos] == 0x20)
					{
						pos++;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;
				}
				if(bBreakTime)
				{
					break;
				}
			}

			while( (idx < len || is_han) && pos < dest_len) // ??? ?? ?? ?? ? ? ??.
			{
				// ?? ?? ??? ?? ??.
				switch( g_dwLangType)
				{
				case LANG_JPN:	// ??
					if((str[pos-1] != 0x20  && str[pos] == 0x20)		// ????.
							|| ((str[pos-2] != -127 && str[pos-1] != 0x40)	// ????.
								&& (str[pos] == -127 && str[pos+1] == 0x40)))
					{
						CheckPoint = idx;
					}
					break;

				case LANG_TWN:	// ??
					if((str[pos-1] != 0x20  && str[pos] == 0x20)		// ????.
							|| ((str[pos-2] != -95 && str[pos-1] != 0x40)		// ????.
								&& (str[pos] == -95 && str[pos+1] == 0x40)))
					{
						CheckPoint = idx;
					}
					break;

				case LANG_CN:  // ??
					if((str[pos-1] != 0x20  && str[pos] == 0x20)		// ????.
							|| (str[pos-1] != -95 && str[pos] == -95))		// ????.
					{
						CheckPoint = idx;
					}
					break;

				default:
					if(str[pos-1] != 0x20  && str[pos] == 0x20)
					{
						CheckPoint = idx;
					}
					break;
				}

				if (str[pos] < 0 || str[pos] > 127)
				{
					if (is_han) // ???? ??
					{
						is_han = FALSE;
					}
					else
					{
						is_han = TRUE;
					}
				}
				else
				{
					is_han = FALSE;
				}

				// ??? ??, ??? ??
				descr_buf[idx++] = str[pos++];
			}

			if( CheckPoint > 0 && pos < dest_len )  // ? ??? ??? ??? ?? ??? ?? ??.
			{
				memcpy(print_buf, descr_buf, CheckPoint);
				pos = pos - (idx - CheckPoint) +1;
				switch( g_dwLangType )
				{
				case LANG_JPN:
					if( str[pos-1] == -127 )
					{
						pos ++;
					}
					break;	// ??
				case LANG_TWN:
					if( str[pos-1] == -95  )
					{
						pos ++;
					}
					break;	// ??
				case LANG_CN:
					if( str[pos-1] == -95  )
					{
						pos ++;
					}
					break;	// ??
				}
				idx = CheckPoint;
			}
			else
			{
				memcpy(print_buf, descr_buf, idx);
			}

			print_buf[strlen(print_buf)+1] = '\0';

			if( pListBox )
			{
				pListBox->AddString(print_buf, color);
			}
		}
	}
	else if(dest_len<=len)
	{
		if( pListBox )
		{
			pListBox->AddString(str, color);
		}
	}
}

void CControlBottom::SetFocus()
{
	if (m_InputBox.GetEnable() == FALSE)
	{
		m_InputBox.SetEnable(TRUE);
		pCMyApp->RestoreImmStatus();
	}
}

void CControlBottom::SetNkCha(CNkCharacter *pNkCha)
{
	m_pNkCha = pNkCha;

	if (m_pNkCha && m_pNkCha->m_Level > 249)
	{
		m_NeedTotalExp = m_pNkCha->m_Exp + m_pNkCha->m_NeedExp;
	}
	else
	{
		if(m_pNkCha && m_bLevelUp)
		{
			int cur_exp = GetLevelupExp(m_pNkCha->m_Level);
			int prev_exp = GetLevelupExp(m_pNkCha->m_Level-1);


			m_OldNeedExp = m_pNkCha->m_NeedExp;
			m_NeedTotalExp = cur_exp - prev_exp;
			m_bLevelUp = FALSE;
		}
		if(m_pNkCha && m_OldNeedExp < m_pNkCha->m_NeedExp)
		{
			m_OldNeedExp = m_pNkCha->m_NeedExp;
			m_bLevelUp = TRUE;
		}
	}

	switch( pNkCha->m_Race )
	{
	case RACE_BULKAN: // 불칸
		m_rcurrent_view = GageView_Type__Stamina;
		break;
	case RACE_KAILIPTON: // 카이립톤
		m_rcurrent_view = GageView_Type__Mana;
		break;
	case RACE_AIDIA: // 에이디아
		m_rcurrent_view = GageView_Type__Mana;
		break;
	case RACE_HUMAN: // 휴먼
		m_rcurrent_view = GageView_Type__EPower;
		break;
	case RACE_FREAK: // 하이브리더
		m_rcurrent_view = GageView_Type__Stamina;
		break;
	case RACE_PEROM: // 하이브리더
		m_rcurrent_view = GageView_Type__EPower;
		break;
	}
}

void CControlBottom::DrawInputBox(CTextBox* surface, int xpos, int ypos)
{
}

void CControlBottom::SetFramePos( int Case )
{
	if( m_QlistBox )
	{
		m_QlistBox->m_Scroll.SetDrawPos( 5, m_QlistBox->m_PosY - 5, (m_QlistBox->m_FontSize+m_QlistBox->m_Tem) * m_QlistBox->m_ViewCount );
	}
	if( m_QlistBox_Shout )
	{
		m_QlistBox_Shout->m_Scroll.SetDrawPos( 5, m_QlistBox_Shout->m_PosY - 5, (m_QlistBox_Shout->m_FontSize+m_QlistBox_Shout->m_Tem) * m_QlistBox_Shout->m_ViewCount );
	}
}

void CControlBottom::SetChangeVal(int Case)
{
	if(m_QlistBox)
	{
		m_QlistBox->m_StartViewCount = _StartCnt;
		m_QlistBox->m_ViewCount = _LastCnt;
		m_QlistBox->SetTem(7);
		m_QlistBox->m_bScrollFlag = TRUE;
		m_QlistBox->m_Scroll.SetRange( MAX_Q - m_QlistBox->m_ViewCount );
		m_QlistBox->m_Scroll.SetPos( MAX_Q-1 );
	}
	if(m_QlistBox_Shout)
	{
		m_QlistBox_Shout->m_StartViewCount = 0;
		m_QlistBox_Shout->m_ViewCount = 6;
		m_QlistBox_Shout->SetTem(7);
		m_QlistBox_Shout->m_bScrollFlag = TRUE;
		m_QlistBox_Shout->m_Scroll.SetRange( MAX_Q - m_QlistBox_Shout->m_ViewCount );
		m_QlistBox_Shout->m_Scroll.SetPos( MAX_Q-1 );
		m_QlistBox_Shout->m_Scroll.MoveGap = 1;
	}
}

void CControlBottom::DrawQlistBox(CQlistBox* surface, int xpos, int ypos)
{
}

void CControlBottom::DrawChatBox(int xpos, int ypos)
{

}

void CControlBottom::DrawAllBtn(CBasicButton* surface, int xpos, int ypos )
{
}

//---------------------------------------------------------------------------
//		 에너지 바 이미지 계산해서 그리기
//---------------------------------------------------------------------------
void CControlBottom::DrawAllEg(CSurface* surface, int xpos, int ypos, int cur_energy, int max_energy)
{
	DDBLTFX  ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.ddckSrcColorkey ;

	RECT rc, m_ClientRc, DestRc ;
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	int EnergyPercent;
	int ImagePercent;
	int Image_Width;
	int Image_Height;
	int Image_Xpos;
	int Image_Ypos;

	if( cur_energy > max_energy )
	{
		cur_energy = max_energy;
	}

	EnergyPercent = (int)(((double)cur_energy * max_energy) / (max_energy));
	ImagePercent  = (EnergyPercent * surface->GetHeight()) / (max_energy);

	Image_Width  = surface->GetWidth();
	Image_Xpos	 = (xpos);

	Image_Height = surface->GetHeight();
	Image_Ypos	 = (ypos);
	int ppp = ((ImagePercent));

	rc.left		= 0;
	rc.top		= surface->GetHeight()-(ImagePercent);
	rc.right	= surface->GetWidth();
	rc.bottom	= surface->GetHeight();

	DestRc.left   = Image_Xpos;
	DestRc.right  = Image_Xpos+Image_Width;
	DestRc.top    = Image_Ypos+Image_Height-(ppp);
	DestRc.bottom = Image_Ypos+Image_Height;

	// 여기서 만약 에너지 파워의 그림이 <= 0 이면 그리지 않는다. 왜? 사각형은 면적을 가져야 하니까..
	if (DestRc.top >= DestRc.bottom)
		return;

	if( g_pDisplay && g_pDisplay->GetBackBuffer() )
		g_pDisplay->GetBackBuffer()->Blt(&DestRc, surface->GetDDrawSurface(), &rc, DDBLT_KEYSRC | DDBLT_WAIT, &ddbltfx);
}


void CControlBottom::SetQuickCount(int slot_num)
{
	m_QuickCount[slot_num].SetString(m_pNkCha->m_QuickCount[slot_num]);
}

void CControlBottom::SetQuickNoCount(int slot_num)
{
	m_QuickCount[slot_num].SetString("");
}

void CControlBottom::InitExp()
{
	if (g_dwClientCountry == 1)
	{
		m_bLevelUp = TRUE;
		a_LevelupExp[0] = 80;

		for( int i=1; i<MAX_LEVEL; i++)
		{
			a_LevelupExp[i] = (int)((double)i*i*i*i/3)+2+(i*i*i)+(200*i*i)-(3*i)+200;
			assert(a_LevelupExp[i] > a_LevelupExp[i-1]);
		}
	}
	else if (g_dwClientCountry == CTRY_DEU
			 || g_dwClientCountry == CTRY_DENG
			 || g_dwClientCountry == CTRY_TH
			 || g_dwClientCountry == CTRY_FR
			 || g_dwClientCountry == CTRY_BRA )
	{
		m_bLevelUp = TRUE;
		a_LevelupExp[0] = 80;

		for( int i=1; i<MAX_LEVEL; i++)
		{
			a_LevelupExp[i] = ((int)((double)i*i*i*i/2)+2+(i*i*i)+(250*i*i)-(3*i)+350) / 2;
			assert(a_LevelupExp[i] > a_LevelupExp[i-1]);
		}
	}
	//  07-03-16 브라질 경험치 공식 태국하고 똑같이 바꿈.
	else if( g_dwClientCountry == CTRY_BRA )
	{
		m_bLevelUp = TRUE;
		a_LevelupExp[0] = 105;

		for( int i=1; i<MAX_LEVEL; i++)
		{
			a_LevelupExp[i] = (int)((double)i*i*i*i/2)+2+(i*i*i)+(250*i*i)-(3*i)+350;
			_int64 TempI64 = (_int64)a_LevelupExp[i]; // 브라질은 한국의 70%로 필요경험치를 낮춤.(변수 오버플로우 때문에 형변환을 거침.)
			a_LevelupExp[i] -= (int)(TempI64*30/100); // 브라질은 한국의 70%로 필요경험치를 낮춤.(변수 오버플로우 때문에 형변환을 거침.)
			assert(a_LevelupExp[i] > a_LevelupExp[i-1]);
		}
	}
	else
	{
		m_bLevelUp = TRUE;
		a_LevelupExp[0] = 150;

		for( int i=1; i<MAX_LEVEL; i++)
		{
			a_LevelupExp[i] = (int)((double)i*i*i*i/2)+2+(i*i*i)+(250*i*i)-(3*i)+350;
			assert(a_LevelupExp[i] > a_LevelupExp[i-1]);
		}
	}
}


int CControlBottom::GetLevelupExp(int level)
{
	//	level--;
	if(level <= 0)
		return 0;

	if(level>=MAX_LEVEL)
		level = MAX_LEVEL -1;

	return a_LevelupExp[level-1];
}



void CControlBottom::DrawNeedExpBar()
{
	// ratio = 현재 획득 경험치 / 필요경험치
	// by evilkiki 2009.09.014 :
	RECT rc;
	double percent = (double)g_pNk2DFrame->GetExpGet() / g_pNk2DFrame->GetExpRequire();
	int cx2 = (m_ExpBar_Cx * percent);

	SetRect(&rc, 0, 0, cx2, m_ExpBar_Cy);

	if(g_pDisplay && cx2>1 && m_pNeedExpBar )
		g_pDisplay->Blt(m_pNeedExpBar->Xpos, m_pNeedExpBar->Ypos, m_pNeedExpBar, &rc);
}


BOOL CControlBottom::CheckNoWhisper(char *msg)
{
	char buf[255];
	char tmp[200];
	// 앞단어는 귓말 거부라고 인식을 한다.

	if (strlen(msg) > 254)
		return FALSE;

	msg = AnyOneArg(msg, buf);

	// IDS_BOTTOM_NC : //귓말거부
	if (strcmp(buf, G_STRING(IDS_BOTTOM_NC)) != 0 && strcmp(buf, "//nochat") != 0 && strcmp(buf, "//nc") != 0)
		return FALSE;

	AnyOneArg(msg, buf);

	if(strlen(buf) <= 0)
		return FALSE;

	if(m_pNoWhisper && m_pNoWhisper->IsIntheList(buf))
	{
		// 리스트에서 제거한다.
		m_pNoWhisper->DeleteFromList(buf);
		// IDS_BOTTOM_WHISPER_OFF : %s 님의 귓말거부가 해제되었습니다.
		sprintf(tmp, G_STRING(IDS_BOTTOM_WHISPER_OFF), buf);
		if( g_pNk2DFrame )
			g_pNk2DFrame->AddChatStr(tmp, -1);
		return TRUE;
	}
	else
	{
		if( m_pNoWhisper )
			m_pNoWhisper->AddToList(buf);
		// IDS_BOTTOM_WHISPER_ON : %s 님의 귓말을 거부합니다.
		sprintf(tmp, G_STRING(IDS_BOTTOM_WHISPER_ON), buf);
		if( g_pNk2DFrame )
			g_pNk2DFrame->AddChatStr(tmp, -1);
		return TRUE;
	}

	return FALSE;
}

// nate 2006-03-10 : Insert Whisper List
void CControlBottom::InsertSender( char* szSender )
{
	if( !strcmp(g_pRoh->m_pstrName, szSender))	// 본캐 필터링
		return;

	char szTemp[64] = "";
	sprintf(szTemp,"/%s ", szSender);

	int i = 0;
	for( i = 0 ; i < MAX_NUM_HISTORYBUF ; i ++ )
	{
		if(!strcmp( m_szSenderLIstBuf[i], szTemp ))
			return;

		if( !m_szSenderLIstBuf[i][0] )
		{
			strcpy(m_szSenderLIstBuf[i], szTemp);
			return;
		}
	}

	for( i = 1 ; i < MAX_NUM_HISTORYBUF ; i ++ )
	{
		char temp[128] = "";
		strcpy(temp, m_szSenderLIstBuf[i]);
		strcpy(m_szSenderLIstBuf[i-1], temp);
	}

	strcpy(m_szSenderLIstBuf[MAX_NUM_HISTORYBUF-1], szTemp);
}


void CControlBottom::LostFocus()
{
	m_hide_tick = GetTickCount();

	m_InputBox.SetEnable(FALSE);
	pCMyApp->ConvertImmStatus();
}

bool CControlBottom::CheckAndMsg( char * buf, const char * const msg, const char * const check )
{
	if(strncmp(buf, check, strlen(check)))
		return false;

	char strBuf[256];
	char *arg;
	arg = AnyOneArg(buf, strBuf);

	sprintf(strBuf, msg, arg);

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(strBuf);

	return true;
}

bool CControlBottom::CheckAndMsg( char * buf, const char * const msg, const int num, const char ** check )
{
	for (int i = 0; i < num; ++i)
	{
		if (strncmp(buf, check[i], strlen(check[i])))
			continue;

		return CheckAndMsg(buf, msg, check[i]);
	}

	return false;
}

void CControlBottom::RegistChatProc()
{
	REGIST_CHAT_PROC("포인트",		ProcChat_Point);
	REGIST_CHAT_PROC("point",		ProcChat_Point);
	REGIST_CHAT_PROC("교환",			ProcChat_Exchange);
	REGIST_CHAT_PROC("exchange",	ProcChat_Exchange);
	REGIST_CHAT_PROC("ex",			ProcChat_Exchange);
	REGIST_CHAT_PROC("귓말거부",		ProcChat_NoChat);
	REGIST_CHAT_PROC("nochat",		ProcChat_NoChat);
	REGIST_CHAT_PROC("nc",			ProcChat_NoChat);
	REGIST_CHAT_PROC("장사",			ProcChat_Sales);
	REGIST_CHAT_PROC("trade",			ProcChat_Sales);
	REGIST_CHAT_PROC("pk",			ProcChat_FreePk);
	REGIST_CHAT_PROC("정제",			ProcChat_Refine);
	REGIST_CHAT_PROC("refine",		ProcChat_Refine);
	REGIST_CHAT_PROC("평화",			ProcChat_Peace);     //DISABLE PEACE COMMAND
	REGIST_CHAT_PROC("peace",			ProcChat_Peace);
	REGIST_CHAT_PROC("구경",			ProcChat_Watching);
	REGIST_CHAT_PROC("caliber",			ProcChat_Watching);
	REGIST_CHAT_PROC("친구",			ProcChat_AddFriend);
	REGIST_CHAT_PROC("친구추가",		ProcChat_AddFriend);
	REGIST_CHAT_PROC("addfriend",	ProcChat_AddFriend);
	REGIST_CHAT_PROC("친구삭제",		ProcChat_DelFriend);
	REGIST_CHAT_PROC("delfriend",	ProcChat_DelFriend);
	REGIST_CHAT_PROC("status",		ProcChat_MultiPoint);
	REGIST_CHAT_PROC("능력치",		ProcChat_MultiPoint);
	REGIST_CHAT_PROC("제어기",		ProcChat_ControlTower);
	REGIST_CHAT_PROC("ct",		ProcChat_ControlTower);
	REGIST_CHAT_PROC("셀프정제",		ProcChat_SelfRefine);
	REGIST_CHAT_PROC("selfrefine",		ProcChat_SelfRefine);
	REGIST_CHAT_PROC("현재시간",		ProcChat_CurrentTime);
	REGIST_CHAT_PROC("timecheck",		ProcChat_CurrentTime);
	REGIST_CHAT_PROC("서버확인",		ProcChat_ServerName);
	REGIST_CHAT_PROC("version",		ProcChat_ServerName);
	REGIST_CHAT_PROC("천공",			ProcChat_SkyMap);
	REGIST_CHAT_PROC("sky",			ProcChat_SkyMap);
	REGIST_CHAT_PROC("convert",		ProcChat_GoldConvert);
}

void CControlBottom::ProcChat(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	MAP_CHAT_PROC::iterator itFind = m_mapChatProc.find(buf);
	if( itFind != m_mapChatProc.end() )
	{
		(this->*((*itFind).second))(chat);		
	}
}

void CControlBottom::ProcChat_Point(char* chat)
{
	if( g_pRoh->IsEquiped() )
	{
		g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_LHSTRING1838), -1 );
		return;
	}

	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);

	sprintf(buf, "point%s\n", arg);

	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(buf);
}

void CControlBottom::ProcChat_Exchange(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	sprintf(buf, "ex%s\n", arg);

	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(buf);
}

void CControlBottom::ProcChat_NoChat(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	if( strlen(arg) == 0 )
	{
		g_bNc == 0 ? g_bNc = 1 : g_bNc = 0;

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage("nc\n");
	}
	else
	{
		sprintf(buf, "nc%s\n", arg);

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage(buf);
	}
	
}

void CControlBottom::ProcChat_Sales(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd
	SkipSpaces(&arg);

	if( strlen(arg) == 0 )
		return;

	if( g_pRoh && g_pRoh->m_Level > 10 )
	{
		m_bRepSalesCmd = TRUE;
	}
	else
	{
		m_bRepSalesCmd = FALSE;
	}

	m_nRepSales = 0;
	strcpy(m_strSalesCmd, arg);

	std::string strChat = arg;
	SendChatMutliTarget(eChatMultiType_Sales, strChat);
}

void CControlBottom::ProcChat_FreePk(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("pk\n");
}

void CControlBottom::ProcChat_Refine(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	sprintf(buf, "refine%s\n", arg);

	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(buf);
}

void CControlBottom::ProcChat_Peace(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("peace\n");
}

void CControlBottom::ProcChat_Watching(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("caliber\n");
}

void CControlBottom::ProcChat_AddFriend(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	sprintf(buf, "addfriend 1%s\n", arg);

	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(buf);
}

void CControlBottom::ProcChat_DelFriend(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	sprintf(buf, "delfriend 1%s\n", arg);

	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(buf);
}

void CControlBottom::ProcChat_MultiPoint(char* chat)
{
	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// main_cmd

	sprintf(buf, "m_point%s\n", arg);
	
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage(buf);
}

void CControlBottom::ProcChat_ControlTower(char* chat)
{
	if( g_pTcpIp == NULL )
		return;

	char buf[MAX_PATH] = {0};
	char* arg = chat;
	arg = AnyOneArg(arg, buf);	// "//제어기"
	arg = AnyOneArg(arg, buf);	// sub_cmd

	if( strcmp("info", buf) == 0 )
	{
		g_pTcpIp->SendNetMessage( "controltower info\n" );
	}
	else if( strcmp("on", buf) == 0 )
	{
		g_pTcpIp->SendNetMessage( "controltower on\n" );
	}
	else if( strcmp("off", buf) == 0 )
	{
		g_pTcpIp->SendNetMessage( "controltower off\n" );
	}
}

void CControlBottom::ProcChat_SelfRefine(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("selfrefine\n");
}

void CControlBottom::ProcChat_CurrentTime(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("timecheck\n");
}

void CControlBottom::ProcChat_ServerName(char* chat)
{
	char buf[MAX_PATH] = {0};
	sprintf(buf, (char*)G_STRING(IDS_CONN_SERVER_NAME), g_pNk2DFrame->GetConnectServerName());
	g_pNk2DFrame->AddChatStr(buf, -1);
}

void CControlBottom::ProcChat_SkyMap(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("go_world 6\n");
}

void CControlBottom::ProcChat_GoldConvert(char* chat)
{
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage("comm_eve\n");
}
