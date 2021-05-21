#include "stdafx.h"
#define D3D_OVERLOADS
#include <list>
#include <iterator>
#include <winuser.h>
#include <assert.h>
#include "main.h"
#include "ddutil.h"
#include "Pages.h"
#include "Sheet.h"
#include "headers.h"
#include "SMRender.h"
#include "IndexedTexture.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "Tcpipcon.h"
#include "d3dutil.h"
#include "D3DMath.h"
#include "ItemProto.h"
#include "Item.h"
#include <time.h>
#include "DirtSnd.h"
#include "LandDemo.h"
#include "LdDecoObj.h"
#include "FxCharWave.h"
#include "WeatherMgr.h"
#include "Country.h"
#include "LH_SelectCharacterPage.h"
#include "LHUI_TakeBox.h"
#include "LHParam_Manager.h"
#include "g_stringmanager.h"
#include "g_devlogbox.h"
#include "packet_header.h"
#include "RecvPacketMgr.h"
#include "CommonConfig.h"

using namespace std;

extern BOOL g_bAidia;

extern int g_ClickSound;
extern BOOL g_bCsyMipmap;


// 맵을 셀로 나눈 것에 대한 정의.
extern int g_nLdMapCellWidth;						// 한 셀의 크기.
extern int g_nLdMapCellHeight;
extern int g_nLdMapCellWidthCount;
extern int g_nLdMapCellHeightCount;

extern int gLdMapWidth;
extern int gLdMapHeight;
extern int gLdMapExtention;

extern BOOL g_bSafari;

extern list<CLdDecoObj *> g_listLdDecoObj[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];
extern list<CLdDecoObj *> g_listLdDecoObj2[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];

extern void LdDecoObjSet(char *strFilePath);
extern void LdDecoObjInitialLoading(char *strFilePath, int nStage);
extern BOOL InsertLdDecoObjToMaplist(CLdDecoObj *pLdDecoObj) ;
extern void DeleteLdDecoObjData(int nStage);
extern void DeleteLdDecoObjSet();

extern DWORD	g_dwLangType;
extern DWORD	g_dwClientCountry;
extern int		g_MatrixSvr;
extern BOOL g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.
extern BOOL g_bSnow;

extern DWORD g_PhaseEndTick[4]; ///2007로딩변경

extern bool g_bWndNetMessage;

IndexedTexture *g_pLdDecoTexture;
int g_SheetSound[SHEET_SND_NUM];

const int CREATE_HYBRIDER_LEVEL = 200;
// const int CREATE_PEROM_LEVEL = 250;

const int CREATE_PEROM_LEVEL = 345;			// by evilkiki 2011.09.07. 퍼롬생성제한 345


extern void SendLogin(const int& type, const bool& web, const bool& flag, const bool& dev, const char* id, const char* pw);

CSlotChar::CSlotChar()
	: m_Race(0)
	, m_Sex(0)
	, m_Hair(0)
	, m_Level(0)
	, m_MaxVital(0)
	, m_MaxMana(0)
	, m_MaxStamina(0)
	, m_MaxEpower(0)
	, m_Vital(0)
	, m_Mana(0)
	, m_Stamina(0)
	, m_Epower(0)
	, m_Str(0)
	, m_Int(0)
	, m_Dex(0)
	, m_Con(0)
	, m_Cha(0)
	, m_BlockedTime(0)
	, m_GuilMa(0)
	, m_GuildIndex(0)
	, m_Support(0)
{
	m_Name = NULL;
	for (int i = 0; i < WEARING_SHOW_NUM; i++)
	{
		m_Wearing[i] = -1;
	}
}

CSlotChar::~CSlotChar()
{
	for (int i=0; i < WEARING_SHOW_NUM; i++)
	{
		m_Wearing[i] = -1;
	}
	SAFE_DELETE_ARRAY(m_Name);
}

void CSlotChar::Clone(CSlotChar* pClone)
{
	SAFE_DELETE_ARRAY(pClone->m_Name);

	if( m_Name != NULL )
	{
		pClone->m_Name = new char[strlen(m_Name) + 1];
		strcpy(pClone->m_Name, m_Name);
	}
	pClone->m_Race = m_Race;
	pClone->m_Sex = m_Sex;
	pClone->m_Hair = m_Hair;
	pClone->m_Level = m_Level;
	pClone->m_MaxVital = m_MaxVital;
	pClone->m_MaxMana = m_MaxMana;
	pClone->m_MaxStamina = m_MaxStamina;
	pClone->m_MaxEpower = m_MaxEpower;
	pClone->m_Vital = m_Vital;
	pClone->m_Mana = m_Mana;
	pClone->m_Stamina = m_Stamina;
	pClone->m_Epower = m_Epower;
	pClone->m_Str = m_Str;
	pClone->m_Int = m_Int;
	pClone->m_Dex = m_Dex;
	pClone->m_Con = m_Con;
	pClone->m_Cha = m_Cha;
	pClone->m_BlockedTime = m_BlockedTime;
	pClone->m_GuilMa = m_GuilMa;
	pClone->m_GuildIndex = m_GuildIndex;
	pClone->m_Support = m_Support;
	for(int i = 0; i < WEARING_SHOW_NUM; i++)
	{
		pClone->m_Wearing[i] = m_Wearing[i];
	}
}

CSheet::CSheet()
	: m_testui(NULL)
	, m_nMatrixChaSvrIdx(0)
	, m_bEmptySloat(FALSE)
	, m_bCreatedCha(FALSE)
	, m_dwDummyBaseTime(0)
	, m_dwAmbientLogin(0)
	, m_dwAmbientSel(0)
	, m_dwFogSkyColor(0)
	, m_dwFogLandColor(0)
	, m_dwFogSkyLoginColor(0)
	, m_dwFogLandLoginColor(0)
	, m_dwFogSkySelColor(0)
	, m_dwFogLandSelColor(0)
	, m_wBlok(0)
	, m_ChaCount(0)
{
	memset(m_sUserId, 0, sizeof(m_sUserId));
	memset(m_sUserPW, 0, sizeof(m_sUserPW));
	memset(m_Buf, 0, sizeof(m_Buf));
	memset(m_Buf2, 0, sizeof(m_Buf2));
	memset(m_dwSlotAniStart, 0, sizeof(m_dwSlotAniStart));
	memset(m_EmptySlotCheck, 0, sizeof(m_EmptySlotCheck));

	m_bNeedAcct = FALSE;
	m_bPaidUser = TRUE;

	int i;

	m_iPageState = LOGINPAGE;

	m_SlotNum = -1;
	m_SelRace = 0;
	m_SelSex = 0;
	m_eStartOption = GAME_MENU;

	m_pLoginPg = new CLoginPage(this);

	m_pSelCharPg = NULL;
	m_pSelCharPg2= NULL;
	m_pSelCharPg2 = new LH_SelectCharacterPage(this);

	m_pCrtCharPg = new CCreateCharacterPage(this);
	m_pOptionPg = new COptionPg(this);

	SetSvrAddr(NULL, 0);

	m_pIndexedTexture = NULL;

	for (i=0; i < LOLO_INDEX; i++)
	{
		m_pLolos[i] = NULL;
	}

	for (i = 0; i < SLOT_COUNT; i++)
	{
		m_Slot[i] = NULL;
		m_CharsIndex[i] = -1;
	}

	m_pSMRender = new CSMRender;
	m_pCha = new CNkCharacter;
	m_pCha->m_wx = m_pCha->m_wz = m_pCha->m_wy = 0.0f;
	m_pCha->m_dir = g_PI+g_PI_DIV_2;

	for (i=0; i < SLOT_COUNT; i++)
	{
		m_pSlotCha[i] = new CNkCharacter;
		m_pSlotCha[i]->m_wx = m_pSlotCha[i]->m_wy = m_pSlotCha[i]->m_wz = 0.0f;
		m_pSlotCha[i]->m_dir = g_PI+g_PI_DIV_2;
	}
	for( i = 0 ; i < 6 ; i ++ )
	{
		m_pOpeningCha[ i ] = NULL;
		for( int j = 0 ; j < 6 ; j ++ )
		{
			m_nOpeningWear[ i * j ] = 0;
		}
	}

	m_dpChaCapsyong = NULL;
	for( i = 0 ; i < NUM_RACE*NUM_SEX ; i ++ )
	{
		m_dpBipedList[ i ] = NULL;
	}
	srand( (unsigned)time( NULL ) );

	m_pLandDemo = NULL;
	m_pLandSelChar = NULL;

	///2007로딩변경
	m_vEye    = D3DVECTOR( 1710.98f, 360.0f, 912.181f );
	m_vLookAt = D3DVECTOR( 1641.48f, 390.0f, 987.077f );
	m_vEndEye = D3DVECTOR( 1036.18f, 390.0f, 1610.20f );		// 캐릭터 선택에서 BAck 눌렀을때 화면 시점.
	m_vLoginLookAt = D3DVECTOR( 1029.54f, 392.934f, 1617.07f ); // 캐릭터 선택에서 BAck 눌렀을때 화면 시점.

	m_vEyeSelStart = D3DVECTOR(300, 130, 500);
	m_vEyeSelEnd = D3DVECTOR(300, 130, 35);
	m_vSelLookAt = D3DVECTOR(300, 0, 650);

	m_vUpVec    = D3DVECTOR(0, 1, 0);


	g_pLdDecoTexture = new IndexedTexture(100);

	m_pddsBackBuffer = NULL;

	m_FovLogin = 0.9250f;
	m_FovSel = 0.6450f;

	sky = NULL;
	sky = new CSphere;
	sky->SkyCreate(HALF_OF_WIDTH_SPHERE, 3.0f, 3.0f);

	sky_cloud = NULL;
	sky_cloud = new CSphere;
	sky_cloud->SkyCreate(HALF_OF_WIDTH_SPHERE, 3.0f, 3.0f);

	m_wSky = 0;
	m_wSky_Cloud = 0;

	m_LogoTextrIndex = 0;

	m_bTwoPassSky = TRUE;

	m_dwRL = 0x5;
	m_dwGL = 0x5;
	m_dwBL = 0x10;
	m_dwRS = m_dwGS = m_dwBS = 0xAA;

	m_zNear = 10.0f;
	m_zFar = 3000.0f;

	m_fGoldRate = 1.618f;

	LdDecoObjSet("data/back/roomlogin/");

	m_vCharPos[0] = D3DVECTOR(253.267181f, 107.000000f, 146.117401f);
	m_vCharPos[1] = D3DVECTOR(271.175598f, 107.000000f, 170.610992f);
	m_vCharPos[2] = D3DVECTOR(300.019806f, 107.000000f, 180.012680f);
	m_vCharPos[3] = D3DVECTOR(328.847137f, 107.000000f, 170.735672f);
	m_vCharPos[4] = D3DVECTOR(346.833954f, 107.000000f, 146.308151f);

	m_vSelCharPos = D3DVECTOR(300, 105, 108);

	m_zSkyNear = 0.29f;
	m_zSkyFar = 1.03f;
	m_zLandNear = 0.05f;
	m_zLandFar = 0.12f;

	m_dwFogSkyR= 0x75;
	m_dwFogSkyG= 0x97;
	m_dwFogSkyB = 0xad;

	m_dwFogLandR = 0x29;
	m_dwFogLandG = 0x2e;
	m_dwFogLandB = 0x2a;

	m_zSkyNearLogin = m_zSkyNear;
	m_zSkyFarLogin = m_zSkyFar;
	m_zLandNearLogin = m_zLandNear;
	m_zLandFarLogin = m_zLandFar;

	m_dwFogSkyLoginR= m_dwFogSkyR;
	m_dwFogSkyLoginG= m_dwFogSkyG;
	m_dwFogSkyLoginB = m_dwFogSkyB;

	m_dwFogLandLoginR = m_dwFogLandR;
	m_dwFogLandLoginG = m_dwFogLandG;
	m_dwFogLandLoginB = m_dwFogLandB;

	m_zSkyNearSel = 0.38f;
	m_zSkyFarSel = 1.46f;
	m_zLandNearSel = 0.01f;
	m_zLandFarSel = 0.14f;

	m_dwFogSkySelR= 0x26;
	m_dwFogSkySelG= 0x44;
	m_dwFogSkySelB = 0x85;

	m_dwFogLandSelR = 0x11;
	m_dwFogLandSelG = 0x0f;
	m_dwFogLandSelB = 0x19;

	m_pWeatherMgr = new CWeatherMgr;

	for (int j=0; j < SLOT_COUNT; j++)
		m_bSlotAniIng[j] = m_bSlotOpenAni[j] = FALSE;

	if( g_dwClientCountry == CTRY_BRA )
		m_GoHomePage.Init(TYPE_NOR_OK);
	else
		m_GoHomePage.Init(TYPE_NOR_OKCANCLE);

	m_bIsChangeInfo = false;
}

CSheet::~CSheet()
{
	DeleteRes();

	SAFE_DELETE(m_pLoginPg);
	SAFE_DELETE(m_pSelCharPg);
	SAFE_DELETE(m_pSelCharPg2);
	SAFE_DELETE(m_pCrtCharPg);
	SAFE_DELETE(m_pOptionPg);

	SAFE_DELETE( m_pSMRender )
	SAFE_DELETE(m_pCha);

	for (int i = 0; i < SLOT_COUNT; i++)
	{
		if (m_Slot[i])
		{
			delete m_Slot[i];
		}
		if (m_pSlotCha[i])
			delete m_pSlotCha[i];
	}
	for( int i = 0 ; i < NUM_RACE ; i ++ )
	{
		if( m_pOpeningCha[ i ] )
		{
			if( m_pOpeningCha[ i ]->GetBiped() )
				delete m_pOpeningCha[ i ]->GetBiped();

			delete m_pOpeningCha[ i ];
		}
	}

	for (int i = 0; i < NUM_RACE * NUM_SEX; i++)
	{
		SAFE_DELETE( m_dpBipedList[i] );
	}

	SAFE_DELETE(sky);
	_asm nop;
	SAFE_DELETE(sky_cloud);

	SAFE_DELETE(m_pWeatherMgr);

	DeleteLdDecoObjSet();

	SAFE_DELETE(m_pLandDemo);
	SAFE_DELETE(m_pLandSelChar);
	SAFE_DELETE(g_pLdDecoTexture);
	SAFE_DELETE(m_pIndexedTexture);

	// 시작과 동시에 눈내림 설정(4번존)
	if( pCMyApp->m_param_manager )
	{
		const LHParam_MapInfo * info = pCMyApp->m_param_manager->GetMapInfo(4);
		if( info )
			g_bSnow = info->m_snow;
	}
}

void CSheet::LoadRes()
{
	static BOOL bOnce = TRUE;


	if( LHGlobal_String::GetSingletonPtr() )
		LHGlobal_String::GetSingletonPtr()->Open_All();

	m_pLoginPg->LoadRes();
	m_pSelCharPg2->LoadRes();

	m_pCrtCharPg->LoadRes();
	m_pOptionPg->LoadRes();

	if (pCMyApp->GetCurGameMode() == NKGRAPHIC2D && bOnce)
		pCMyApp->InitProgress(0);

	m_pSMRender->Init();

	LoadBipedList();

	LoadTexture();
	if (pCMyApp->GetCurGameMode() == NKGRAPHIC2D && bOnce)
	{
		pCMyApp->InitProgress(1);
		bOnce = FALSE;
	}
	LoadTexture2();

	m_pddsBackBuffer = g_pDisplay->GetBackBuffer();

	g_bSnow = TRUE; // 시작시 바로 눈 내리는 설정 하기


	m_pWeatherMgr->SetLogin(TRUE);
	m_pWeatherMgr->SetWeather(4);


	m_GoHomePage.LoadRes();
}

void CSheet::LoadSheetWav()
{
	if (g_pDSound)
	{
		g_pDSound->SetWaveFilePath("data/sound/");
		g_SheetSound[SHEET_SND_STONE] = g_pDSound->LoadWAV("down.wav"); ///2007때 수정.
		g_ClickSound = g_SheetSound[SHEET_SND_CLICK] = g_pDSound->LoadWAV("click.wav");
		g_SheetSound[SHEET_SND_APP] = g_pDSound->LoadWAV("app.wav");
	}
}

void CSheet::DeleteRes()
{
	m_pLoginPg->DeleteRes();
	m_pSelCharPg2->DeleteRes();
	m_pCrtCharPg->DeleteRes();
	m_pOptionPg->DeleteRes();

	m_pSMRender->DeleteRes();

	if (m_pIndexedTexture)
		m_pIndexedTexture->InvalidateAllTextures();

	SAFE_DELETE(m_pIndexedTexture);

	for (int i=0; i < LOLO_INDEX; i++)
	{
		SAFE_DELETE(m_pLolos[i]);
	}

	if (g_pLdDecoTexture)
		g_pLdDecoTexture->InvalidateAllTextures();


	SAFE_DELETE(m_pLandDemo);
	SAFE_DELETE(m_pLandSelChar);
	DeleteLdDecoObjData(0);
	DeleteLdDecoObjData(1);

	m_GoHomePage.DeleteRes();
}

char *basic_bpd[ NUM_RACE*NUM_SEX ] = { "bm_st_peace.bpd", "w_st_peace.bpd"
										, "m_st_peace.bpd", "w_st_peace.bpd"
										, "a_st_peace.bpd", "a_st_peace.bpd"
										, "m_st_peace.bpd", "w_st_peace.bpd"
										, "fm_st_peace.bpd", "fw_st_peace.bpd"
										, "pm_st_peace.BPD", "pw_st_peace.BPD"
									  };

void CSheet::LoadBipedList()
{
	CBiped *pBiped=NULL;

	char bpd_file[100];
	for( int i = 0 ; i < NUM_RACE*NUM_SEX ; i ++ )
	{
		if (m_dpBipedList[i] == NULL)
		{
			pBiped = new CBiped;

			strcpy(bpd_file, "data/Character/bpd/");
			strcat(bpd_file, basic_bpd[i]);
			pBiped->Read(bpd_file);
			m_dpBipedList[i] = pBiped;
		}
	}
}


char *CSheet::SetSvrAddr(char *str, int port)
{
	m_nSvrPort = port;

	g_bSafari = FALSE;

	if (str != NULL)
	{
		strcpy(m_strSvrAddr, str);
		return str;
	}
	else
	{
		m_strSvrAddr[0] = NULL;
		return NULL;
	}

}

void CSheet::SetChaSet(CCapsyong **dpChaCapsyong, IndexedTexture *pChaTexture)
{
	m_dpChaCapsyong = dpChaCapsyong;
}

// ------------------------------------------------------------
// AutoLogin()
// Desc : 유저가 타이핑 치지 않고 자동 로긴할 때 호출되는 프로시저.
// ------------------------------------------------------------
BOOL CSheet::AutoLogin(int nSvrIdx, char *strUserId, char *strUserPw)
{
	if( strUserId == NULL
			|| strUserPw == NULL )
		return FALSE;

	BOOL bRes;

	if (m_pLoginPg)
	{
		if (m_pLoginPg->m_pMatrixSvr[nSvrIdx] > 0 )
		{
			m_pLoginPg->m_bShowMatrixList = TRUE;
		}
		else
		{
			bRes = m_pLoginPg->LoginProcess(nSvrIdx, strUserId, strUserPw, TRUE);
		}
	}

	return FALSE;
}

BOOL CSheet::Connect(char *id_name, char *passwd,bool flags)
{
	if( !id_name || !passwd || !g_pTcpIp )
		return FALSE;

	BOOL connected = FALSE;

	if( g_pTcpIp->ConnectToServer(m_strSvrAddr, m_nSvrPort) == FALSE )
	{
		g_pTcpIp->CloseSocket();
		return FALSE;
	}

	char strVersion[30];
	sprintf(strVersion, "%d\n", PACKET_VER);

	m_pLoginPg->m_bKickLogin = FALSE;

	DWORD lastTime = timeGetTime();
	DWORD curTime = lastTime;
	do
	{
		if (g_pTcpIp->SendNetMessage(strVersion, TRUE))
		{
			connected = TRUE;
			break;
		}
		curTime = timeGetTime();
	}
	while (connected == FALSE && curTime < lastTime + 5000);


	if (connected)
	{
		strcpy(m_sUserId, id_name);
		strcpy(m_sUserPW, passwd);

		int loginType = 0;

		bool confirm90day = flags;
		if( CommonConfig::Instance()->GetLoginProcess() == false )
		{
			confirm90day = false;
		}

		SendLogin(0, false, confirm90day, g_dev_client, id_name, passwd);
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}

int CSheet::UpdateChars()
{
	for (int i = 0; i < SLOT_COUNT; i++)
	{
		m_EmptySlotCheck[i] = 0;

		if (m_Slot[i])
		{
			delete m_Slot[i];
			m_Slot[i] = NULL;
		}
	}

	m_ChaCount = 0;

	// 이곳에서 로긴시 무한으로 빠지는 경우가 있음...
	memset(m_Buf, 0, sizeof(m_Buf));
	if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
	{
		return -1;
	}

	char *arg = AnyOneArg(m_Buf, m_Buf2);

	if( !strcmp(m_Buf2, "locate") )
	{
		memset(m_Buf, 0, sizeof(m_Buf));
		if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
		{
			return -1;
		}

		arg = AnyOneArg(m_Buf, m_Buf2);
	}

	if( !strcmp(m_Buf2, "lag_rush") )
	{
		memset(m_Buf, 0, sizeof(m_Buf));
		if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
		{
			return -1;
		}

		arg = AnyOneArg(m_Buf, m_Buf2);
	}

	if( !strcmp(m_Buf2, "chars_start") )
	{
		memset(m_Buf, 0, sizeof(m_Buf));
		if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
		{
			return -1;
		}

		arg = AnyOneArg(m_Buf, m_Buf2);
	}

	if( !strcmp(m_Buf2, "fail") )
	{
		// 음...한번더 필터링을?
		return 0;
	}

	m_bIsChangeInfo = false;

	if (!strcmp(m_Buf2, "cfirm"))
	{
		arg = AnyOneArg(arg, m_Buf2);

		if ( !strcmp(m_Buf2, "passwd_90day"))
		{
			m_bIsChangeInfo = TRUE;
			return 0;
		}
	}	

	while (!strcmp(m_Buf2, "chars_exist"))
	{
		m_ChaCount ++;
		arg = AnyOneArg(arg, m_Buf2);
		int slot_num = atoi(m_Buf2);
		if (m_Slot[slot_num])
			delete m_Slot[slot_num];
		m_Slot[slot_num] = new CSlotChar;

		m_EmptySlotCheck[slot_num] = 1;

		{
			//////////////////////////////////////////////////////////////////////////
			// 길드 가입 승인을 기다리고 있는 캐릭터를 삭제시에 쓰일 캐릭터 인덱스 서버에서 사용한다.
			arg = AnyOneArg(arg, m_Buf2);
			m_CharsIndex[slot_num] = atoi(m_Buf2);
			//////////////////////////////////////////////////////////////////////////
		}

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Name = _StrDup(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Race = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Sex = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Hair = atoi(m_Buf2);
		if (m_Slot[slot_num]->m_Hair >= 10)
		{
			m_Slot[slot_num]->m_Hair = (m_Slot[slot_num]->m_Hair % 10) + 5;
		}
		if (m_Slot[slot_num]->m_Hair < 0 || m_Slot[slot_num]->m_Hair >= 10)
		{
			m_Slot[slot_num]->m_Hair = 0;
		}

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Level = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Vital = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_MaxVital = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Mana = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_MaxMana = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Stamina = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_MaxStamina = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Epower = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_MaxEpower = (atoi(m_Buf2) == 0?1:atoi(m_Buf2)); ///국가대항전 시엔 캐릭터 셋팅관련해서 이 값들이 서버에서 0으로 와버린다. 그럼 출력할때 게이지 출력시 튕기므로(릴리즈모드에서만) 이게 0이면 1로 바꿔주기로함.

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Str = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Int = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Dex = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Con = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Cha = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_BlockedTime = atoi(m_Buf2);

		arg = AnyOneArg(arg, m_Buf2);				// 길드 인덱스...없으면 0, 있으면 1
		m_Slot[slot_num]->m_GuildIndex = atoi(m_Buf2);

		for (int where = 0; where < WEARING_SHOW_NUM; where++)
		{
			arg = AnyOneArg(arg, m_Buf2);
			m_Slot[slot_num]->m_Wearing[where] = atoi(m_Buf2);
		}

		// 길마인가 아닌가 확인하는 코드.
		arg = AnyOneArg(arg, m_Buf2);
		// 길마에 상관없이 에이디아를 만든다.

		// 신섭에서 길드 가입 상태인지 가입한 상태인지를 체크한다.
		// 0: 미가입 혹은 가입 승인을 기다리는 상태
		// 1~: 이상은 가입된 상태
		m_Slot[slot_num]->m_GuilMa = atoi(m_Buf2);
		//m_Slot[slot_num]->m_GuilMa = 0;

		arg = AnyOneArg(arg, m_Buf2);
		m_Slot[slot_num]->m_Support = atoi(m_Buf2);

		memset(m_Buf, 0, sizeof(m_Buf));
		if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
			return -1;

		arg = AnyOneArg(m_Buf, m_Buf2);
	}
//---------------------------------------------
	//넷마블 유저 계정 미등록 메시지

	if(!strcmp(m_Buf2, "chars_end"))
	{
		AnyOneArg(arg, m_Buf2);
		if( g_dwClientCountry == CTRY_BRA ) // 브라질이면... ///브라질결제
		{
			m_bPaidUser = atoi(m_Buf2); // 결제 여부.
		}
		else if( g_dwClientCountry == CTRY_KOR ) // 한국 넷마블 계정전환
		{
// 			if(!strcmp(m_Buf2, "1"))
// 			{
// 				m_GoHomePage.PutString("계정전환이 안된 상태입니다. 전환 하시겠습니까?");
// 			}
		}

		AnyOneArg(arg, m_Buf2); // 유저 인덱스 앞에 인자가 하나 더 있다.

		// 계정 만료일 표시기능.
		if( g_dwClientCountry == CTRY_KOR ) // 계정만료일은 일단 한국만 지원

		{
			AnyOneArg(arg, m_Buf2); // 유저 인덱스.

			m_pSelCharPg2->GetAccountEndString( atoi(m_Buf2) );
		}

		pCMyApp->m_dwPlayingTime = timeGetTime(); // 접속 시간을 알려주기 위한 틱 기억.
	}
//---------------------------------------------
// 에디디아는 다 만들수 있다.
//==================================
	g_bAidia = TRUE;
	BYTE byCanCharCreate = 0;
	for (int i = 0; i < SLOT_COUNT; i++)
	{
		if (m_Slot[i])
		{
			SetSlotCha(i);



			if( m_Slot[i]->m_Level >= CREATE_PEROM_LEVEL )
			{
				byCanCharCreate = 2;
			}
			else if( byCanCharCreate != 2 && m_Slot[i]->m_Level >= CREATE_HYBRIDER_LEVEL )
			{
				byCanCharCreate = 1;
			}



		}
	}
	if( m_pCrtCharPg )
		m_pCrtCharPg->SetRaceBox( byCanCharCreate );
	if( m_pSelCharPg2 )
	{
		m_pSelCharPg2->SetCharNames();
		m_pSelCharPg2->m_servername.SetString( g_pNk2DFrame->GetConnectServerName() );
	}
	return 1;
}

char *CSheet::AnyOneArg(char *argument, char *first_arg)
{
	SkipSpaces(&argument);

	while (*argument && !is_space(*argument))
	{
		*(first_arg++) = *argument;
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}

void CSheet::SkipSpaces(char **string)
{
	if( *string == NULL )
		return;

	for (; **string && is_space(**string); (*string)++);
}

char *CSheet::_StrDup(char *str)
{
	if( str == NULL )
		return NULL;

	char *str_new = new char[strlen(str) + 1];
	strcpy(str_new, str);

	return str_new;
}

bool CSheet::GetOneLine(char* msg, int maxSize)
{
	if( msg == NULL )
		return false;

	char* pp = NULL;
	RecvPacketMgr* packMgr = RecvPacketMgr::GetInstance();

	int read_len = 0;
	while( 1 )
	{
		read_len = g_pTcpIp->ReadMessage();
		if( read_len == -1 )
		{
			return false;
		}

		pp = packMgr->PopPacket();
		if( pp != NULL )
			break;
	}

	stPacketHeader* pHeader = reinterpret_cast<stPacketHeader*>(pp);

	if( pHeader->size_ >= maxSize )
		return false;

	g_pTcpIp->decrypt(&pp[PACKET_HEADER_SIZE], pHeader->size_);

	stPacketSubHeader shd;
	memcpy(&shd, &pp[PACKET_HEADER_SIZE], PACKET_SUB_HEADER_SIZE);

	int bodySize = pHeader->size_ - PACKET_SUB_HEADER_SIZE;
	memcpy(msg, &pp[ALL_PACKET_HEADER_SIZE], bodySize);
	msg[bodySize] = '\0';

	if( g_bWndNetMessage )
	{
		G_DevAddLogNetwork(msg,"--");
	}

	return true;
}

char* CSheet::SendNewChar(int town_num)
{
	int nHair;
	nHair = m_TempSlot.m_Hair;

	if (nHair >= 5)
		nHair += 5;

	sprintf(m_Buf, "char_new %d %s "
			"%d %d %d "
			"%d %d %d "
			"%d %d %d\n",
			m_SlotNum, m_TempSlot.m_Name,
			m_TempSlot.m_Race, m_TempSlot.m_Sex, m_TempSlot.m_Hair,
			m_TempSlot.m_Str, m_TempSlot.m_Int, m_TempSlot.m_Dex,
			m_TempSlot.m_Con, m_TempSlot.m_Cha, town_num);

	g_pTcpIp->SendNetMessage(m_Buf, TRUE);

	memset(m_Buf, 0, sizeof(m_Buf));
	if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
		return "fail";

	char *arg = AnyOneArg(m_Buf, m_Buf2);

	if (!strcmp(m_Buf2, "success"))
	{
		m_pSelCharPg2->UpdateButton(m_SlotNum);
		return NULL;
	}

	SkipSpaces(&arg);
	return arg;
}

char* CSheet::SendDelChar(bool& outFail, char * str)
{
	outFail = false;

	// 일본용으로 다시 작성 필요
	if( g_dwLangType == LANG_KOR || g_dwLangType == LANG_ENG )
	{
		sprintf(m_Buf, "char_del %d %d d %s\n",	m_SlotNum, m_CharsIndex[m_SlotNum], str);
	}
	else
	{
		sprintf(m_Buf, "char_del %d %d\n", m_SlotNum, m_CharsIndex[m_SlotNum] );
	}

	g_pTcpIp->SendNetMessage(m_Buf, TRUE);

	memset(m_Buf, 0, sizeof(m_Buf));
	if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
	{
		outFail = true;
		return (char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION);
	}

	char *arg = AnyOneArg(m_Buf, m_Buf2);

	if (!strcmp(m_Buf2, "success"))
	{
		m_pSelCharPg2->UpdateButton(-1);
		return NULL;
	}

	if(!strcmp(m_Buf2, "not_success"))
		return "not_success";

	if( strcmp(m_Buf2, "fail") == 0 )
	{
		outFail = true;
	}

	SkipSpaces(&arg);
	return arg;
}

int CSheet::ConfirmExist(char *name)
{
	if( name == NULL )
		return 0;

	sprintf(m_Buf, "char_exist %s\n", name);
	g_pTcpIp->SendNetMessage(m_Buf, TRUE);

	memset(m_Buf, 0, sizeof(m_Buf));
	if( GetOneLine(m_Buf, sizeof(m_Buf)) == false )
		return -1;

	char *arg = AnyOneArg(m_Buf, m_Buf2);

	if (!strcmp(m_Buf2, "success"))
	{	
		return 1;
	}

	return 0;
}

void CSheet::StartGame(START_OPTION option)
{
	if (option == START_GAME)
	{
		sprintf(m_Buf, "start %d 0 %d %d %d %d %d %d\n", m_SlotNum, m_ChaCount,
				m_EmptySlotCheck[0], m_EmptySlotCheck[1], m_EmptySlotCheck[2], m_EmptySlotCheck[3], m_EmptySlotCheck[4] );

		g_pTcpIp->SendNetMessage(m_Buf, TRUE);

		m_eStartOption = START_GAME;
	}
	CPacketControl::EnableSend(FALSE);

}

void CSheet::GoCreateChaPage()
{
	m_pCrtCharPg->m_RaceBox.m_CurSel = 0;
	m_pCrtCharPg->m_SexBox.m_CurSel = 0;
	m_pCrtCharPg->m_HairBox.m_CurSel = 0;
	m_pCrtCharPg->m_NameBox.SetEnable(TRUE);
	m_pCrtCharPg->m_NameBox.ClearText();
	SetBasicCha(0, 0, 0);
}

void CSheet::SetBasicCha(int nRace, int nSex, int nHair)
{
	if (m_pCha)
	{
		m_pCha->SetBiped(m_dpBipedList[nRace*2+nSex]);

		CSYMAP_it it = pCMyApp->m_race_csy_map.begin();
		for (int i=0; i < NKCHA_HAIR; i++)
		{
			it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(i&0xFF)<<8);
			if( it != pCMyApp->m_race_csy_map.end() )
				m_pCha->SetCapsyong((*it).second, i);
			else
			{
				_asm nop;
			}
		}

		it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(5&0xFF)<<8|(nHair&0xFF));
		if( it != pCMyApp->m_race_csy_map.end() )
			m_pCha->SetCapsyong((*it).second,NKCHA_HAIR);
		else
		{
			_asm nop;
		}

		m_pCha->m_Race = nRace;
		m_pCha->m_BpdRace = nRace * 2 + nSex;
		m_pCha->m_Sex = nSex;

		m_pCha->m_wy = 100.0f;			// 캐릭터가 수영을 하지 않게 하려고.
	}
}

void CSheet::SetSlotCha(int idx)
{
	int body_offset = 0;
	int hair_offset = 0;
	int i;

	int nRace = m_Slot[idx]->m_Race;
	int nSex = m_Slot[idx]->m_Sex;
	int nHair = m_Slot[idx]->m_Hair;

	if (m_pSlotCha[idx])
	{
		delete m_pSlotCha[idx];
		m_pSlotCha[idx] = NULL;
		m_pSlotCha[idx] = new CNkCharacter;
		m_pSlotCha[idx]->m_wx = m_pSlotCha[idx]->m_wy = m_pSlotCha[idx]->m_wz = 0.0f;
		m_pSlotCha[idx]->m_dir = g_PI+g_PI_DIV_2;
	}

	if (m_pSlotCha[idx])
	{
		m_pSlotCha[idx]->SetBiped(m_dpBipedList[nRace*2+nSex]);

		CSYMAP_it it = pCMyApp->m_race_csy_map.begin();
		for (i=0; i < NKCHA_HAIR; i++)
		{
			it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(i&0xFF)<<8);
			if( it != pCMyApp->m_race_csy_map.end() )
				m_pSlotCha[idx]->SetCapsyong((*it).second, i);
			else
			{
				_asm nop;
			}
		}

		it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(5&0xFF)<<8|(nHair&0xFF));
		if( it != pCMyApp->m_race_csy_map.end() )
			m_pSlotCha[idx]->SetCapsyong((*it).second,NKCHA_HAIR);
		else
		{
			_asm nop;
		}

		for (i = 0; i < WEARING_SHOW_NUM; i++)
		{
			if (m_Slot[idx]->m_Wearing[i] >= 0)
			{
				m_pSlotCha[idx]->Wear(i, -1, m_Slot[idx]->m_Wearing[i]);
			}
		}

		m_pSlotCha[idx]->m_Sex = nSex;
		m_pSlotCha[idx]->m_Race = nRace;
		m_pSlotCha[idx]->m_BpdRace = nRace*2+nSex;
	}

	m_pSlotCha[idx]->m_wy = 100.0f;				// 캐릭터가 수영을 하지 않게 하려고.
}

void CSheet::RenderBasicCha(int x, int y, int width, int height, BOOL bSelected)
{
	if( m_pSMRender )
		m_pSMRender->RenderCha(x, y, width, height, m_pCha, CRTCHARPAGE, bSelected);
}

void CSheet::RenderSlotCha(int idx, int x, int y, int width, int height, BOOL bSelected, BOOL bBlocked)
{
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	char fCharPosDir[] = {-1.23f, -1.4, -1.57f, -1.74f, -1.91f};

	static int lastTickCount = 0;
	static int	 frame = 0;

	//D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	mtrl.diffuse.a = 0.5f;
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 월드 트랜스폼

	vEyePt    = D3DVECTOR(0, 140, -140);
	vLookatPt = D3DVECTOR(0, 110, 0);
	vUpVec    = D3DVECTOR(0, 1, 0);


	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );

	D3DLIGHT7 m_d3dLight;

	ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
	m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	m_d3dLight.dcvDiffuse.a   = 0.5f;
	m_d3dLight.dcvDiffuse.r   = 1.0f;
	m_d3dLight.dcvDiffuse.g   = 1.0f;
	m_d3dLight.dcvDiffuse.b   = 1.0f;
	m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
	m_d3dLight.dvDirection = Normalize(D3DVECTOR(1000, -1000, -1000));

	m_d3dLight.dvAttenuation0 = 1.0f;
	m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
	GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
	GET_D3DDEVICE()->LightEnable(0, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	// testscenerender2에서 가져온 projection matrix
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProjSel);

	D3DUtil_SetIdentityMatrix( matWorld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	DWORD dwAmbientOld, dwAmbient;

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbientOld );
	if (bSelected)
	{
		m_pSlotCha[idx]->m_wx = m_vSelCharPos.x;
		m_pSlotCha[idx]->m_wy = m_vSelCharPos.y;
		m_pSlotCha[idx]->m_wz = m_vSelCharPos.z;

		m_pSlotCha[idx]->m_dir = -1.57f;

		dwAmbient = 0xFFDDDDDD;
	}
	else
	{
		m_pSlotCha[idx]->m_wx = m_vCharPos[idx].x;
		m_pSlotCha[idx]->m_wy = m_vCharPos[idx].y;
		m_pSlotCha[idx]->m_wz = m_vCharPos[idx].z;
		m_pSlotCha[idx]->m_dir = -1.57f; //fCharPosDir[idx];

		dwAmbient = 0xFF555555;
	}

	if( bBlocked )
	{
		dwAmbient = 0xFFFF0000;
	}

	if (g_bCsyMipmap)
	{
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
	}

	m_pSlotCha[idx]->SetAmbient(TRUE, dwAmbient);
	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		m_pSlotCha[idx]->Render(NULL, FALSE);

		GET_D3DDEVICE()->EndScene();
	}
	m_pSlotCha[idx]->SetAmbient(FALSE, 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbientOld );

	m_GoHomePage.Draw();
}

BOOL g_bHolo = TRUE;
#define SEL_CHA_GAP		(-20.0f)
#define CHA_ROTATE_GAP		(0.02f)
void CSheet::RenderSlotCha2(int idx, int x, int y, int width, int height, BOOL bSelected)
{
	static float cur_dir = 0.0f;
	int to_front_idx = 0;

	to_front_idx = m_SlotNum;
	if (to_front_idx < 0 || to_front_idx >= 5)
	{
		to_front_idx = 0;
	}
	float to_dir = g_2_PI / 5 * to_front_idx;

	if (to_dir - cur_dir > g_PI)
	{
		cur_dir += g_2_PI;
	}
	if (cur_dir - to_dir > g_PI)
	{
		cur_dir -= g_2_PI;
	}
	if (to_dir > cur_dir)
	{
		cur_dir += CHA_ROTATE_GAP;
		if (to_dir < cur_dir)
			cur_dir = to_dir;
	}
	else if (to_dir < cur_dir)
	{
		cur_dir -= CHA_ROTATE_GAP;
		if (to_dir > cur_dir)
			cur_dir = to_dir;
	}

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;

	static int lastTickCount = 0;
	static int	 frame = 0;

	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	mtrl.diffuse.a = 0.5f;
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 월드 트랜스폼

	vEyePt    = D3DVECTOR(0, 140, -140);
	vLookatPt = D3DVECTOR(0, 110, 0);
	vUpVec    = D3DVECTOR(0, 1, 0);




	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );

	// 라이트.
	D3DLIGHT7 m_d3dLight;

	ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
	m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	m_d3dLight.dcvDiffuse.a   = 0.5f;
	m_d3dLight.dcvDiffuse.r   = 1.0f;
	m_d3dLight.dcvDiffuse.g   = 1.0f;
	m_d3dLight.dcvDiffuse.b   = 1.0f;
	m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
	m_d3dLight.dvDirection = Normalize(D3DVECTOR(1000, -1000, -1000));

	m_d3dLight.dvAttenuation0 = 1.0f;
	m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
	GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
	GET_D3DDEVICE()->LightEnable(0, TRUE);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	D3DMATRIX matTemp;
	D3DUtil_SetTranslateMatrix(matWorld, 0.0f, 0.0f, SEL_CHA_GAP);
	D3DUtil_SetRotateYMatrix(matTemp, (g_2_PI / 5 * (5-idx)) + cur_dir);
	D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);
	D3DVECTOR vectTemp;
	vectTemp.x = 0.0f;
	vectTemp.y = 0.0f;
	vectTemp.z = 0.0f;
	D3DMath_VectorMatrixMultiply(vectTemp, vectTemp, matWorld);

	m_pSlotCha[idx]->m_wx = m_vCharPos[idx].x;
	m_pSlotCha[idx]->m_wy = m_vCharPos[idx].y;
	m_pSlotCha[idx]->m_wz = m_vCharPos[idx].z;
	if (g_bCsyMipmap)
	{
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
	}

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		m_pSlotCha[idx]->Render(NULL, FALSE);

		GET_D3DDEVICE()->EndScene();
	}
#define HOLO_HEIGHT		(25.0f)
#define HOLO_WIDTH		(5.0f)
#define HOLO_COLOR		(0x00ffffff)
#define HOLO_TEXTR_HEIGHT	(0.5f)
	if (FALSE)
	{
		float map_start = (rand() % 100) / 100.0f;
		D3DLVERTEX vertHolo[4];
		vertHolo[0] = D3DLVERTEX(D3DVECTOR(HOLO_WIDTH,
										   HOLO_HEIGHT,
										   0.0f),
								 HOLO_COLOR, 0L, map_start, 1);
		vertHolo[1] = D3DLVERTEX(D3DVECTOR(HOLO_WIDTH,
										   0.0f,
										   0.0f),
								 HOLO_COLOR, 0L, map_start, 0);
		vertHolo[2] = D3DLVERTEX(D3DVECTOR(-HOLO_WIDTH,
										   HOLO_HEIGHT,
										   0.0f),
								 HOLO_COLOR, 0L, map_start+HOLO_TEXTR_HEIGHT, 1);
		vertHolo[3] = D3DLVERTEX(D3DVECTOR(-HOLO_WIDTH,
										   0.0f,
										   0.0f),
								 HOLO_COLOR, 0L, map_start+HOLO_TEXTR_HEIGHT, 0);
		unsigned short indexHolo[6];
		indexHolo[0] = 0;
		indexHolo[1] = 1;
		indexHolo[2] = 2;
		indexHolo[3] = 1;
		indexHolo[4] = 3;
		indexHolo[5] = 2;

		D3DUtil_SetTranslateMatrix(matWorld, m_pSlotCha[idx]->m_wx, 100.0f, m_pSlotCha[idx]->m_wz-10.0f);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	}
}

// 화면 모드가 바뀔때 지우고 다시 만든다.
void CSheet::LoadTexture()
{
	// 텍스처 매니저

	if (m_pIndexedTexture)
	{
		m_pIndexedTexture->InvalidateAllTextures();
		delete m_pIndexedTexture;
	}
	m_pIndexedTexture = NULL;
	m_pIndexedTexture = new IndexedTexture(100);
	m_pIndexedTexture->SetTexturePath("data/back/textr/");

	m_wSky = m_pIndexedTexture->CreateTextureFromFile("sky.bmp");
	m_wSky_Cloud = m_pIndexedTexture->CreateTextureFromFile("sky_cloud.bmp");

	///2007로딩변경

	m_LogoTextrIndex = m_pIndexedTexture->CreateTextureFromFile( "companylogo.tga" );

	m_pIndexedTexture->SetTexturePath("data/effect/textr/");
	m_wBlok = m_pIndexedTexture->CreateTextureFromFile("serverlist.tga");
	m_pIndexedTexture->SetTexturePath("data/back/textr/");

	// 지형 관련 로드.
	if (!m_pLandDemo) ///2007로딩변경

	{
		m_pLandDemo = new CLandDemo;
		m_pLandDemo->LoadNewMap( "data/back/roomlogin/", "roomlogin.map" );
	}

	if (!m_pLandSelChar)
	{
		m_pLandSelChar = new CLandDemo;
		m_pLandSelChar->LandDemoInit("data/back/roomselect/");
	}

	m_pWeatherMgr->LoadRes();

	// 건물 로딩.
	LdDecoObjInitialLoading("data/back/roomlogin/", 0);
	LdDecoObjInitialLoading("data/back/roomselect/", 1);
	if (g_pLdDecoTexture)
		g_pLdDecoTexture->RestoreAllTextures(GET_D3DDEVICE());

	m_pIndexedTexture->RestoreAllTextures(GET_D3DDEVICE());
}

void CSheet::LoadTexture2()
{
}

unsigned short starIndex[6] = {0, 1, 2, 1, 3, 2};

void CSheet::TestSceneRender(int IndexNum)
{
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DMATRIX matTemp;

	static int lastTickCount = 0;
	static int	 frame = 0;
	int	       ThisTickCount;

	//IndexNum = 0;
	//---------만일 에니메이션이 존재한다면 프레임카운트를 조정한다----
	ThisTickCount = GetTickCount();

	if (IndexNum == 0)
	{
		// 로그인 화면


		if( m_pLoginPg->m_bIsLoadingEnd && !m_pLoginPg->m_bIsAniEnd )
		{
			DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;

			GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
			GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
			GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

			DWORD  dwTextureState[3];

			GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
			GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
			GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );



			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);

			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

			DWORD dwElapsedTick = m_pLoginPg->m_dwPhaseTick;
			int LogoFrame = 0;

			switch( m_pLoginPg->m_nOpeningPhase ) // 페이드 인 및 천둥,로고 등의 처리.
			{
			case 0:
				if( dwElapsedTick >= g_PhaseEndTick[0] && dwElapsedTick <= g_PhaseEndTick[0]+1500 ) // 로고 서서희 보임. else 가 어님에 유의.
				{
					D3DTLVERTEX TLVec[4];
					int Alpha = 255; // 0이면 불투명, 255면 완전투명이다.
					int backalpha = 255;

					// 로고용 판때기
					if( dwElapsedTick < 1000 )
						Alpha = 255;
					else
						Alpha = 255-( (float)( dwElapsedTick-1000 ) * 0.5f );

					if( Alpha < 0 )
						Alpha = 0;
					if( Alpha > 255 )
						Alpha = 255;


					TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,1024,0),	1, RGBA_MAKE(0,0,0,backalpha), 0, 0, 1.0f );
					TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0),		1, RGBA_MAKE(0,0,0,backalpha), 0, 0, 0 );
					TLVec[2] = D3DTLVERTEX( D3DVECTOR(1024,0,0),	1, RGBA_MAKE(0,0,0,backalpha), 0, 1.0f, 0 );
					TLVec[3] = D3DTLVERTEX( D3DVECTOR(1024,1024,0), 1, RGBA_MAKE(0,0,0,backalpha), 0, 1.0f, 1.0f );
					GET_D3DDEVICE()->SetTexture(0, NULL );
					GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

					TLVec[0] = D3DTLVERTEX( D3DVECTOR(272,428,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
					TLVec[1] = D3DTLVERTEX( D3DVECTOR(272,172,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
					TLVec[2] = D3DTLVERTEX( D3DVECTOR(528,172,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
					TLVec[3] = D3DTLVERTEX( D3DVECTOR(528,428,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

					LogoFrame = dwElapsedTick / ( 1000/20 );//MAX_LOGO_TEXTR );

					if( LogoFrame >= 20 )
						LogoFrame = 0;

					GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_LogoTextrIndex) );
					GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DFVF_TLVERTEX, TLVec, 4, 0);
				}

				break;
			}

			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
		}

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
		GET_D3DDEVICE()->EndScene();

	}
}

void CSheet::RenderDeco(int nStage)
{
	BOOL bAlpha = FALSE, bOpenRoof = FALSE;
	CLdDecoObj *pDecoObj;
	list<CLdDecoObj *>::iterator prDeco;

	float render_dist;

	int w, h, w_pos, h_pos;
	D3DVECTOR vPos, vEye;


	if( nStage == 0 )
	{
		for (prDeco = g_listLdDecoObj[0][0].begin(); prDeco != g_listLdDecoObj[0][0].end(); prDeco++)
		{
			pDecoObj = *prDeco;

			if( pDecoObj->m_nLoloId == 21 ) // 문
			{
				if( m_pLoginPg->m_nOpeningPhase < 1 && m_pLoginPg->m_dwPhaseTick < 300 ) // 문열리기 전

				{
					pDecoObj->m_nCurFrame = 0;
					pDecoObj->Render(FALSE, DECO_ALPHA_NONE,FALSE);
				}
				else if( m_pLoginPg->m_dwPhaseTick > g_PhaseEndTick[2]+2000 ) // 다 열렸을 시간
				{
					pDecoObj->m_nCurFrame = 39;
					pDecoObj->Render(FALSE, DECO_ALPHA_NONE,FALSE);
				}
				else
				{
					pDecoObj->Render(FALSE, DECO_ALPHA_NONE,TRUE); // 애니메이션을 시킴.(애니메이션이 반복되는 문제는 지나가는 타이밍을 조절해서 해결시켜놨다.)
				}
			}
			else
				pDecoObj->Render(FALSE);
		}

		return;
	}

	w_pos = gLdMapWidth*gLdMapExtention;
	h_pos = gLdMapHeight*gLdMapExtention;

	w_pos /= g_nLdMapCellWidth;
	h_pos /= g_nLdMapCellHeight;
	w_pos /= 2;
	h_pos /= 2;

	if (pCMyApp->GetCurWorld() == WORLD_GUILD)
	{
		// 성 근처에 있는가 확인한다.
		render_dist = 1000.0f;
	}
	else
		render_dist = 1500.0f;


	for (h = h_pos - 15; h <= h_pos+15; h++)
	{
		if (h < 0 || h >= g_nLdMapCellHeightCount)	// 맵 영역을 벗어났는가 확인.
			continue;
		for (w = w_pos-15; w <= w_pos+15; w++)
		{
			if (w < 0 || w >= g_nLdMapCellWidthCount)
				continue;

			if (nStage == 0)
			{
				for (prDeco = g_listLdDecoObj2[h][w].begin(); prDeco != g_listLdDecoObj2[h][w].end(); prDeco++)
				{
					pDecoObj = *prDeco;
					if( pDecoObj->m_LineEffectsLoad )
					{
						pDecoObj->DeleteEffectNum();
					}
				}
				for (prDeco = g_listLdDecoObj[h][w].begin(); prDeco != g_listLdDecoObj[h][w].end(); prDeco++)
				{
					pDecoObj = *prDeco;
					//if (pDecoObj->m_nLoloId >= 1 && pDecoObj->m_nLoloId <= 3)
					pDecoObj->Render(NULL, FALSE);
				}
			}
			else
			{
				for (prDeco = g_listLdDecoObj[h][w].begin(); prDeco != g_listLdDecoObj[h][w].end(); prDeco++)
				{
					pDecoObj = *prDeco;
					if( pDecoObj->m_LineEffectsLoad )
					{
						pDecoObj->DeleteEffectNum();
					}
				}
				for (prDeco = g_listLdDecoObj2[h][w].begin(); prDeco != g_listLdDecoObj2[h][w].end(); prDeco++)
				{
					pDecoObj = *prDeco;
					//if (pDecoObj->m_nLoloId < 33 || pDecoObj->m_nLoloId > 37)
					pDecoObj->Render(NULL, FALSE);
				}
			}
		}
	}
}

void CSheet::TestSceneRender2()
{
	D3DMATRIX matWorld, matView;
	D3DMATRIX matTemp;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;


	static int lastTickCount = 0;
	static int	 frame = 0;
	int	       ThisTickCount;

	ThisTickCount = GetTickCount();

	D3DVIEWPORT7 vpNew;

	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);

	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	RECT rect;

	GET_D3DDEVICE()->GetViewport(&vpNew);
	GetClientRect(pCMyApp->Get_hWnd(), &rect);
	vpNew.dwWidth  = rect.right - rect.left ;
	//vpNew.dwHeight = rect.bottom - rect.top ;
	// 황금율 적용.
	int nHeight;
	nHeight = (vpNew.dwWidth / m_fGoldRate);
	if ( (nHeight - (rect.bottom-rect.top)) > 0)
		nHeight = rect.bottom - rect.top;
	vpNew.dwY = ( (rect.bottom - rect.top) - nHeight ) / 2;
	vpNew.dwHeight = nHeight;

	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	D3DUtil_SetProjectionMatrix(m_matProjSel, m_FovSel, fAspect, m_zNear, m_zFar );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProjSel);

	D3DUtil_SetViewMatrix(m_matView,  m_vEye,     m_vLookAt, m_vUpVec );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &m_matView );

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	m_dwAmbientSel = CombineRGB(m_dwRS, m_dwGS, m_dwBS);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwAmbientSel);

	// 라이트.
	D3DLIGHT7 m_d3dLight;

	ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
	m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	m_d3dLight.dcvDiffuse.r   = 1.0f;
	m_d3dLight.dcvDiffuse.g   = 1.0f;
	m_d3dLight.dcvDiffuse.b   = 1.0f;
	m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
	m_d3dLight.dvDirection = Normalize(D3DVECTOR(0, -1000, 1000));

	m_d3dLight.dvAttenuation0 = 1.0f;
	m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
	GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
	GET_D3DDEVICE()->LightEnable(0, TRUE);

	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	SetFog(1, TRUE, TRUE, m_zSkyNear, m_zSkyFar);
	SkyRender(0, 0, 0);

	// 지면

	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	if (m_pLandSelChar)
	{
		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			m_pLandSelChar->Render();
			GET_D3DDEVICE()->EndScene();
		}
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		RenderDeco(1);
		GET_D3DDEVICE()->EndScene();
	}

	SetFog(1, FALSE);

	pCMyApp->DrawDrvTestPickVex();
	pCMyApp->SetDrvTypeClrAlloc();

	// 원상복귀.
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
}

void CSheet::TestSceneRender3()
{
	D3DMATRIX matWorld, matView;
	D3DMATRIX matTemp;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;


	static int lastTickCount = 0;
	static int	 frame = 0;
	int	       ThisTickCount;

	ThisTickCount = GetTickCount();

	D3DVIEWPORT7 vpNew;

	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);

	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	RECT rect;

	GET_D3DDEVICE()->GetViewport(&vpNew);
	GetClientRect(pCMyApp->Get_hWnd(), &rect);
	vpNew.dwWidth  = rect.right - rect.left ;
	int nHeight;
	nHeight = (vpNew.dwWidth / m_fGoldRate);
	if ( (nHeight - (rect.bottom-rect.top)) > 0)
		nHeight = rect.bottom - rect.top;
	vpNew.dwY = ( (rect.bottom - rect.top) - nHeight ) / 2;
	vpNew.dwHeight = nHeight;

	GET_D3DDEVICE()->SetViewport(&vpNew);

	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	D3DUtil_SetProjectionMatrix(m_matProjSel, m_FovSel, fAspect, m_zNear, m_zFar );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProjSel);

	D3DUtil_SetViewMatrix(m_matView,  m_vEye,     m_vLookAt, m_vUpVec );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &m_matView );

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	m_dwAmbientSel = CombineRGB(m_dwRS, m_dwGS, m_dwBS);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwAmbientSel);

	D3DLIGHT7 m_d3dLight;

	ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
	m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	m_d3dLight.dcvDiffuse.r   = 1.0f;
	m_d3dLight.dcvDiffuse.g   = 1.0f;
	m_d3dLight.dcvDiffuse.b   = 1.0f;
	m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
	m_d3dLight.dvDirection = Normalize(D3DVECTOR(0, -1000, 1000));

	m_d3dLight.dvAttenuation0 = 1.0f;
	m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
	GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
	GET_D3DDEVICE()->LightEnable(0, TRUE);

	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	SetFog(1, TRUE, TRUE, m_zSkyNear, m_zSkyFar);
	SkyRender(0, 0, 0);


	D3DUtil_SetIdentityMatrix(matWorld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	if (m_pLandSelChar)
	{
		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			m_pLandSelChar->Render();
			GET_D3DDEVICE()->EndScene();
		}
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);

	SetFog(1, FALSE);

	pCMyApp->DrawDrvTestPickVex();
	pCMyApp->SetDrvTypeClrAlloc();

	// 원상복귀.
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
}


void CSheet::TestSceneRender_back(int IndexNum)
{
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DMATRIX matTemp;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;


	static int lastTickCount = 0;
	static int	 frame = 0;
	int	       ThisTickCount;

	ThisTickCount = GetTickCount();

	if (IndexNum == 0)
	{
		if(ThisTickCount -lastTickCount > 10)
		{
			int frame_tick = 1;
			if (ThisTickCount -lastTickCount >= 1000)   // 1초가 지났으면 좀 이상하지?
			{
				frame++;
			}
			else
			{
				frame_tick = (ThisTickCount -lastTickCount) / 10;
				frame += frame_tick;
			}
			lastTickCount = ThisTickCount;
		}

		D3DVIEWPORT7 vpOld, vpNew;

		DWORD	dwColor;
		D3DMATERIAL7 mtrl;

		D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
		GET_D3DDEVICE()->SetMaterial(&mtrl);

		GET_D3DDEVICE()->GetViewport(&vpOld);
		vpNew = vpOld;
		vpNew.dwWidth  = 640;
		vpNew.dwHeight = 480;
		GET_D3DDEVICE()->SetViewport(&vpNew);

		float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
		GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
		D3DUtil_SetProjectionMatrix(matProjNew, 0.5f, fAspect, 1.0f, 2000.0f );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);



		vEyePt    = D3DVECTOR(0, -700, -10);
		vLookatPt = D3DVECTOR(0, 0, 0);
		vUpVec    = D3DVECTOR(0, 1, 0);


		D3DUtil_SetViewMatrix(matView,  vEyePt,     vLookatPt, vUpVec );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView );

		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00AAAAAA);

		D3DLIGHT7 m_d3dLight;

		ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
		m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
		m_d3dLight.dcvDiffuse.r   = 1.0f;
		m_d3dLight.dcvDiffuse.g   = 1.0f;
		m_d3dLight.dcvDiffuse.b   = 1.0f;
		m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
		m_d3dLight.dvDirection = Normalize(D3DVECTOR(1000, -1000, -1000));

		m_d3dLight.dvAttenuation0 = 1.0f;
		m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
		GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
		GET_D3DDEVICE()->LightEnable(0, TRUE);

		GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );


		D3DUtil_SetTranslateMatrix(matWorld, -150.0f, -30.0f, 0.0f);
		D3DUtil_SetTranslateMatrix(matTemp, 0.0f, 0.0f, -400.0f + (frame * 1.0f));
		D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		//----------------그리기 시작 / 끝 --------------
		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			m_pLolos[0]->Render(GET_D3DDEVICE(), frame);

			GET_D3DDEVICE()->EndScene();
		}

		D3DUtil_SetTranslateMatrix(matWorld, 20.0f, 20.0f, 0.0f);
		D3DUtil_SetTranslateMatrix(matTemp, 0.0f, 0.0f, -200.0f + (frame * 4.0f));
		D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			m_pLolos[1]->Render(GET_D3DDEVICE(), frame);

			GET_D3DDEVICE()->EndScene();
		}

		GET_D3DDEVICE()->SetViewport(&vpOld);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	}
	else
	{
		// 우주선 내부 1

		if(ThisTickCount -lastTickCount > 10) // 1초에 100프레임 그리자!
		{
			if (ThisTickCount -lastTickCount >= 1000) // 1초가 지났으면 좀 이상하지?
				frame++;
			else
				frame += (ThisTickCount -lastTickCount) / 10;
		}

		D3DVIEWPORT7 vpOld, vpNew;

		DWORD	dwColor;
		D3DMATERIAL7 mtrl;

		// 머티리얼 설정
		D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
		GET_D3DDEVICE()->SetMaterial(&mtrl);

		// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
		GET_D3DDEVICE()->GetViewport(&vpOld);
		vpNew = vpOld;
		vpNew.dwWidth  = 640;
		vpNew.dwHeight = 480;
		GET_D3DDEVICE()->SetViewport(&vpNew);

		//// 프로젝션 매트릭스 수정.
		float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
		GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
		D3DUtil_SetProjectionMatrix(matProjNew, 0.5f, fAspect, 1.0f, 2000.0f );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

		vEyePt    = D3DVECTOR(0, 1, -200);
		vLookatPt = D3DVECTOR(0, 0, 0);
		vUpVec    = D3DVECTOR(0, 1, 0);

		D3DUtil_SetViewMatrix(matView,  vEyePt,     vLookatPt, vUpVec );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView );

		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
		//GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00333377);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);

		D3DLIGHT7 m_d3dLight;

		ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
		m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
		m_d3dLight.dcvDiffuse.r   = 0.7f;
		m_d3dLight.dcvDiffuse.g   = 0.7f;
		m_d3dLight.dcvDiffuse.b   = 1.0f;
		m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
		m_d3dLight.dvDirection = Normalize(D3DVECTOR(0, -5, -1));

		m_d3dLight.dvAttenuation0 = 1.0f;
		m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
		GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
		GET_D3DDEVICE()->LightEnable(0, TRUE);

		GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

		D3DUtil_SetRotateYMatrix(matWorld, g_PI/2);
		D3DUtil_SetTranslateMatrix(matTemp, 0.0f, -150.0f, 750.0f);
		D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
		{
			m_pLolos[3]->Render(GET_D3DDEVICE(), frame);

			GET_D3DDEVICE()->EndScene();
		}

		GET_D3DDEVICE()->SetViewport(&vpOld);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	}
}


void CSheet::RenderOj(int xpos, int ypos, int cx, int cy)
{
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	//D3DVECTOR vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	GET_D3DDEVICE()->GetViewport(&vpOld);

	RECT rect;

	GET_D3DDEVICE()->GetViewport(&vpNew);
	GetClientRect(pCMyApp->Get_hWnd(), &rect);
	vpNew.dwWidth  = rect.right - rect.left ;
	int nHeight;
	nHeight = (vpNew.dwWidth / m_fGoldRate);
	if ( (nHeight - (rect.bottom-rect.top)) > 0)
		nHeight = rect.bottom - rect.top;
	vpNew.dwY = ( (rect.bottom - rect.top) - nHeight ) / 2;
	vpNew.dwHeight = nHeight;


	GET_D3DDEVICE()->SetViewport(&vpNew);

	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos,      0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos,     0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos+cy,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos+cy,  0), 1, 0x00ffffff, 0, 1, 1);


	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);


		LPDIRECTDRAWSURFACE7 lpTexture = m_pIndexedTexture->GetSurfaceByIdx(m_wBlok);
		GET_D3DDEVICE()->SetTexture(0, lpTexture);

		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}

void CSheet::Draw()
{
	if( !g_pDisplay )
		return;

	switch ( m_iPageState )
	{
	case LOGOPAGE:
//			m_pLogoPg->Draw();
		return;

	case STARTPAGE:
//			m_pStartPg->Draw();
		return ;

	case LOGINPAGE:
		m_pLoginPg->Draw();
		return ;

	case SELCHARPAGE:
		m_pSelCharPg2->Draw();
		break;

	case CRTCHARPAGE:
		m_pCrtCharPg->Draw();
		return ;

	case SETVALPAGE:
		//m_pSetValPg->Draw();
		return ;

	case SELVILPAGE:
		//m_pSelVilPg->Draw();
		return ;

	case OPTIONPAGE:
		m_pOptionPg->Draw();
		return;

	case MAKEACCPAGE:
		return;
	}
}

void CSheet::SendDummyMsg()
{
	if (g_pTcpIp)
	{
		g_pTcpIp->SendNetMessage("\n");
		m_dwDummyBaseTime = timeGetTime();
	}
}

BOOL g_bWaitFxCharWave = FALSE;
BOOL g_bFinishFxCharWave = FALSE;
void CSheet::GoPage(int x)
{
	static EffectSort ef_sort;

	if (m_iPageState == LOGINPAGE && x == SELCHARPAGE)
	{
		AnimateSelChar();
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_CHARWAVE;
		ef_sort.vPos = m_vSelCharPos;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		g_bWaitFxCharWave = FALSE;
		g_bFinishFxCharWave = FALSE;
	}

	m_iPageState = x;

	switch(m_iPageState)
	{
	case LOGINPAGE:
		// 카메라의 값을 원래대로 바꾸어 논다.
		m_vLookAt = m_vLoginLookAt;
		m_vEye = m_vEndEye;
		// FxCharWave 안 보이거나 지우는 곳.

		// 안개의 값을 원래대로 바꾼다.
		m_zSkyNear = m_zSkyNearLogin;
		m_zSkyFar =  m_zSkyFarLogin;
		m_zLandNear = m_zLandNearLogin;
		m_zLandFar = m_zLandFarLogin ;

		m_dwFogSkyR = m_dwFogSkyLoginR;
		m_dwFogSkyG = m_dwFogSkyLoginG;
		m_dwFogSkyB = m_dwFogSkyLoginB ;

		m_dwFogLandR = m_dwFogLandLoginR;
		m_dwFogLandG = m_dwFogLandLoginG;
		m_dwFogLandB = m_dwFogLandLoginB;

		g_bFinishFxCharWave = TRUE;
		break;

	case SELCHARPAGE:
		// 안개값을 저장.
		m_zSkyNearLogin = m_zSkyNear;
		m_zSkyFarLogin = m_zSkyFar;
		m_zLandNearLogin = m_zLandNear;
		m_zLandFarLogin = m_zLandFar;

		m_dwFogSkyLoginR= m_dwFogSkyR;
		m_dwFogSkyLoginG= m_dwFogSkyG;
		m_dwFogSkyLoginB = m_dwFogSkyB;

		m_dwFogLandLoginR = m_dwFogLandR;
		m_dwFogLandLoginG = m_dwFogLandG;
		m_dwFogLandLoginB = m_dwFogLandB;

		// 안개의 값을 초기화 바꾼다.
		m_zSkyNear = m_zSkyNearSel;
		m_zSkyFar =  m_zSkyFarSel;
		m_zLandNear = m_zLandNearSel;
		m_zLandFar = m_zLandFarSel ;

		m_dwFogSkyR = m_dwFogSkySelR;
		m_dwFogSkyG = m_dwFogSkySelG;
		m_dwFogSkyB = m_dwFogSkySelB ;

		m_dwFogLandR = m_dwFogLandSelR;
		m_dwFogLandG = m_dwFogLandSelG;
		m_dwFogLandB = m_dwFogLandSelB;

		g_bWaitFxCharWave = FALSE;

		break;
	case CRTCHARPAGE:
		break;

	default:
		break;
	}
}

LRESULT CSheet::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if(m_GoHomePage.CheckMsgExist() == TRUE)
	{
		m_GoHomePage.MsgProc(hWnd, msg, wParam, lParam);
		return 0;
	}

	switch ( m_iPageState )
	{
	case LOGOPAGE:
		break;

	case STARTPAGE:
		m_pLoginPg->m_IdBox.SetEnable(TRUE);
		m_pLoginPg->m_PasswdBox.ClearText();
		break;

	case LOGINPAGE:
		m_pLoginPg->MsgProc( hWnd, msg, wParam, lParam );
		break;

	case SELCHARPAGE:
		m_pSelCharPg2->MsgProc( hWnd, msg, wParam, lParam );
		break;

	case CRTCHARPAGE:
		m_pCrtCharPg->MsgProc( hWnd, msg, wParam, lParam );
		break;

	case SETVALPAGE:		
		break;

	case SELVILPAGE:		
		break;

	case OPTIONPAGE:
		m_pOptionPg->MsgProc( hWnd, msg, wParam, lParam);
		break;

	case MAKEACCPAGE:		
		break;

	}

	if( (m_iPageState > LOGINPAGE) &&
		msg == WM_TIMER && wParam == 2 )
	{
		if (g_pTcpIp)
		{
			char buf[MAX_PATH] = {0,};
			strcpy(buf, "\n");
			g_pTcpIp->SendNetMessage(buf);
		}
	}

	return 0;
}

// 3인칭 자유형 카메라로 바꾸엇다. ///2007로딩변경
void CSheet::GoForward()
{
	// 바라보는 방향 벡터. 오른쪽 수직 벡터를 구한다.
	D3DVECTOR CamViewDir = m_vLookAt - m_vEye;
	CamViewDir = Normalize( CamViewDir );
	D3DVECTOR CamRightDir = CrossProduct( m_vUpVec, CamViewDir );
	CamRightDir = Normalize( CamRightDir );

	// 앞으로 이동.
	m_vEye += CamViewDir*5.0f;
	m_vLookAt += CamViewDir*5.0f;
}

void CSheet::GoBackward()
{
	// 바라보는 방향 벡터. 오른쪽 수직 벡터를 구한다.
	D3DVECTOR CamViewDir = m_vLookAt - m_vEye;
	CamViewDir = Normalize( CamViewDir );
	D3DVECTOR CamRightDir = CrossProduct( m_vUpVec, CamViewDir );
	CamRightDir = Normalize( CamRightDir );

	// 앞으로 이동.
	m_vEye -= CamViewDir*5.0f;
	m_vLookAt -= CamViewDir*5.0f;
}

void CSheet::GoLeft()
{
	// 바라보는 방향 벡터. 오른쪽 수직 벡터를 구한다.
	D3DVECTOR CamViewDir = m_vLookAt - m_vEye;
	CamViewDir = Normalize( CamViewDir );
	D3DVECTOR CamRightDir = CrossProduct( m_vUpVec, CamViewDir );
	CamRightDir = Normalize( CamRightDir );

	// 앞으로 이동.
	m_vEye -= CamRightDir*5.0f;
	m_vLookAt -= CamRightDir*5.0f;
}

void CSheet::GoRight()
{
	// 바라보는 방향 벡터. 오른쪽 수직 벡터를 구한다.
	D3DVECTOR CamViewDir = m_vLookAt - m_vEye;
	CamViewDir = Normalize( CamViewDir );
	D3DVECTOR CamRightDir = CrossProduct( m_vUpVec, CamViewDir );
	CamRightDir = Normalize( CamRightDir );

	// 앞으로 이동.
	m_vEye += CamRightDir*5.0f;
	m_vLookAt += CamRightDir*5.0f;
}

void CSheet::UpEye()
{
	m_vEye.y += 5.0f;
	m_vLookAt.y += 5.0f;
}

void CSheet::DownEye()
{
	m_vEye.y -= 5.0f;
	m_vLookAt.y -= 5.0f;
}

void CSheet::TurnLeft()
{
	D3DMATRIX matRot;
	D3DUtil_SetRotationMatrix( matRot, m_vUpVec, 0.2f );
	m_vLookAt -= m_vEye; // Eye를 원점으로 하게 이동시키고.
	D3DMath_VectorMatrixMultiply( m_vLookAt, m_vLookAt, matRot ); // 돌리고

	m_vLookAt += m_vEye; // 다시 원래 좌표로 돌려준다.
}

void CSheet::TurnRight()
{
	D3DMATRIX matRot;
	D3DUtil_SetRotationMatrix( matRot, m_vUpVec, -0.2f );
	m_vLookAt -= m_vEye; // Eye를 원점으로 하게 이동시키고.
	D3DMath_VectorMatrixMultiply( m_vLookAt, m_vLookAt, matRot ); // 돌리고

	m_vLookAt += m_vEye; // 다시 원래 좌표로 돌려준다.
}

void CSheet::TurnUp()
{
	// 바라보는 방향 벡터. 오른쪽 수직 벡터를 구한다.
	D3DVECTOR CamViewDir = m_vLookAt - m_vEye;
	CamViewDir = Normalize( CamViewDir );
	D3DVECTOR CamRightDir = CrossProduct( m_vUpVec, CamViewDir );
	CamRightDir = Normalize( CamRightDir );

	D3DMATRIX matRot;
	D3DUtil_SetRotationMatrix( matRot, CamRightDir, 0.2f );
	m_vLookAt -= m_vEye; // Eye를 원점으로 하게 이동시키고.
	D3DMath_VectorMatrixMultiply( m_vLookAt, m_vLookAt, matRot ); // 돌리고

	m_vLookAt += m_vEye; // 다시 원래 좌표로 돌려준다.
}

void CSheet::TurnDown()
{
	// 바라보는 방향 벡터. 오른쪽 수직 벡터를 구한다.
	D3DVECTOR CamViewDir = m_vLookAt - m_vEye;
	CamViewDir = Normalize( CamViewDir );
	D3DVECTOR CamRightDir = CrossProduct( m_vUpVec, CamViewDir );
	CamRightDir = Normalize( CamRightDir );

	D3DMATRIX matRot;
	D3DUtil_SetRotationMatrix( matRot, CamRightDir, -0.2f );
	m_vLookAt -= m_vEye; // Eye를 원점으로 하게 이동시키고.
	D3DMath_VectorMatrixMultiply( m_vLookAt, m_vLookAt, matRot ); // 돌리고

	m_vLookAt += m_vEye; // 다시 원래 좌표로 돌려준다.
}

bool strinc(char *str, const char *inc)
{
	if( str == NULL
			|| inc == NULL )
		return FALSE;

	int str_len = strlen(str);
	int inc_len = strlen(inc);

	if (str_len < inc_len)
		return false;

	for (int i = 0; i <= str_len-inc_len; i++)
	{
		if (!strncmp(str+i, inc, inc_len))
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------
//-- OutputText
//----------------------------------------------------------------------------
//-- 2005.06.14
//-- Lyul / Comment

//-- Whoami 모드 에서
//-- Show Option 에서만 사용된다.

void CSheet::OutputText( DWORD x, DWORD y, TCHAR* str )
{
	if( str == NULL )
		return;

	HDC hDC;

	// Get a DC for the surface. Then, write out the buffer
	if( m_pddsBackBuffer )
	{
		if( SUCCEEDED( m_pddsBackBuffer->GetDC(&hDC) ) )
		{
			SetBkMode( hDC, TRANSPARENT );
			SetTextColor( hDC, RGB(255,255,0) );
			ExtTextOut( hDC, x, y, 0, NULL, str, lstrlen(str), NULL );
			m_pddsBackBuffer->ReleaseDC(hDC);
		}
	}
}

void CSheet::SkyRender(float x, float y, float z)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR(0.0f, 0.0f, 0.0f);

	float sphere_height = 800.0f;
	float sphere_scale = 4000.0f;

	vDrawPos.x = x;
	vDrawPos.y = y;
	vDrawPos.z = z;

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE,   TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE,   FALSE );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

		DWORD  dwTextureState[3];

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_wSky));

		DWORD m_dwFogColor;
		m_dwFogColor = RGBA_MAKE ( 255, 255, 255, 255 );

		sky->SetColor( m_dwFogColor );
		sky->SetRotationY(g_PI*2);
		sky->RotateZ(g_PI*0.5f);
		sky->Scale(sphere_scale * 0.3f, sphere_scale, sphere_scale);
		sky->SetTranslationXYZ(vDrawPos.x, vDrawPos.y - sphere_height, vDrawPos.z);
		sky->trans_texture(0.0005f);
		sky->SkyRender();

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTexture(0, m_pIndexedTexture->GetSurfaceByIdx(m_wSky_Cloud));

		sky_cloud->SetColor( m_dwFogColor );
		sky_cloud->SetRotationY(g_PI*2);
		sky_cloud->RotateZ(g_PI*0.5f);
		sky_cloud->Scale(sphere_scale * 0.3f, sphere_scale, sphere_scale);
		sky_cloud->SetTranslationXYZ(vDrawPos.x, vDrawPos.y - sphere_height, vDrawPos.z);
		sky_cloud->trans_texture(0.001f);
		sky_cloud->SkyRender();

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

		GET_D3DDEVICE()->EndScene();
	}
}

DWORD CSheet::CombineRGB(DWORD R, DWORD G, DWORD B)
{
	DWORD dwResColor;

	dwResColor = 0;
	dwResColor |= R;
	dwResColor = dwResColor << 8;
	dwResColor |= G;
	dwResColor = dwResColor << 8;
	dwResColor |= B;

	return dwResColor;
}

void CSheet::SetFog(int nStage, BOOL bFog, BOOL bSky, float zNear, float zFar)
{
	if (bFog)
	{
		BOOL bUsingTableFog = FALSE;		// pixel fog가 table 포그이다.
		BOOL bRangeBasedFog = FALSE;		// 지원하는 하드웨어가 없다. 오직 vertexfog에서 cpu를 잡는다.

		float fFogStart, fFogEnd, fFogDensity;
		DWORD dwFogColor;

		if (nStage == 0)
		{
			fFogStart = (zNear * (m_zFar-m_zNear)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
			fFogEnd = (zFar * (m_zFar-m_zNear)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
			fFogDensity = 0.0f;
			if (bSky)
			{
				dwFogColor = m_dwFogSkyColor = CombineRGB (m_dwFogSkyR, m_dwFogSkyG, m_dwFogSkyB);
			}
			else
			{
				dwFogColor = m_dwFogLandColor = CombineRGB (m_dwFogLandR, m_dwFogLandG, m_dwFogLandB);
			}
		}
		else
		{
			fFogStart = (zNear * (m_zFar-m_zNear)) + 20.0f; // ( fStart*(m_zFar-m_zNear) ) + m_zNear
			fFogEnd = (zFar * (m_zFar-m_zNear)) + 20.0f; // ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
			fFogDensity = 0.0f;
			if (bSky)
			{
				dwFogColor = m_dwFogSkyColor = CombineRGB (m_dwFogSkyR, m_dwFogSkyG, m_dwFogSkyB);
			}
			else
			{
				dwFogColor = m_dwFogLandColor = CombineRGB (m_dwFogLandR, m_dwFogLandG, m_dwFogLandB);
			}
		}

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&fFogStart)));
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&fFogEnd)));
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&fFogDensity)));

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  D3DFOG_LINEAR );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, bRangeBasedFog );

	}
	else
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
	}
}

void CSheet::AnimateSelChar()
{
	m_vEye = m_vEyeSelStart;
	m_vLookAt = m_vSelLookAt;

	m_vEye = D3DVECTOR(300, 112, 54);
	m_vLookAt = D3DVECTOR(300, 206, 1156);
}

D3DVECTOR CSheet::GetEyeDirection()
{
	D3DVECTOR vec;

	vec = m_vLookAt - m_vEye;
	return vec;
}

void CSheet::AnimateGate(int nGateNum, int nState)
{
	int nGateIndex[5] = {33, 34, 35, 36, 37};

	CLdDecoObj *pDecoObj;
	list<CLdDecoObj *>::iterator prDeco;

	int w, h, w_pos, h_pos;
	D3DVECTOR vPos, vEye;

	if (nGateNum > 4)
		return;

	w_pos = gLdMapWidth*gLdMapExtention;
	h_pos = gLdMapHeight*gLdMapExtention;

	w_pos /= g_nLdMapCellWidth;
	h_pos /= g_nLdMapCellHeight;
	w_pos /= 2;
	h_pos /= 2;

	for (h = h_pos - 15; h <= h_pos+15; h++)
	{
		if (h < 0 || h >= g_nLdMapCellHeightCount)	// 맵 영역을 벗어났는가 확인.
			continue;
		for (w = w_pos-15; w <= w_pos+15; w++)
		{
			if (w < 0 || w >= g_nLdMapCellWidthCount)
				continue;

			for (prDeco = g_listLdDecoObj2[h][w].begin(); prDeco != g_listLdDecoObj2[h][w].end(); prDeco++)
			{
				pDecoObj = *prDeco;
				if (pDecoObj->m_nLoloId == nGateIndex[nGateNum])
				{
					if (nState == SEL_GATE_UP)
					{
						pDecoObj->AdjustYPos(0.0f, TRUE, FALSE);
					}
					else if (nState == SEL_GATE_DOWN_ING)
					{
						pDecoObj->AdjustYPos(-30.0f, FALSE, TRUE);
						if (g_pDSound)
						{
							g_pDSound->Play(g_SheetSound[SHEET_SND_STONE]);
						}
					}
					else if (nState == SEL_GATE_UP_ING)
					{
						pDecoObj->AdjustYPos(0.0f, TRUE, TRUE);
					}
					else if (nState == SEL_GATE_DOWN)
					{
						pDecoObj->AdjustYPos(-30.0f, FALSE, FALSE);
					}
				}
			}
		}
	}
}

void CSheet::OpenNewGate(int nGateNum)
{
	int nGateIndex[5] = {33, 34, 35, 36, 37};

	CLdDecoObj *pDecoObj;
	list<CLdDecoObj *>::iterator prDeco;

	int w, h, w_pos, h_pos;
	D3DVECTOR vPos, vEye;

	if (nGateNum > 5) // 5는 일단 모두 닫기할때 씀.(캐릭터가 있으면 안닫힘.) ///2007로딩변경

		return;

	w_pos = gLdMapWidth*gLdMapExtention;
	h_pos = gLdMapHeight*gLdMapExtention;

	w_pos /= g_nLdMapCellWidth;
	h_pos /= g_nLdMapCellHeight;
	w_pos /= 2;
	h_pos /= 2;

	for (h = h_pos - 15; h <= h_pos+15; h++)
	{
		if (h < 0 || h >= g_nLdMapCellHeightCount)	// 맵 영역을 벗어났는가 확인.
			continue;
		for (w = w_pos-15; w <= w_pos+15; w++)
		{
			if (w < 0 || w >= g_nLdMapCellWidthCount)
				continue;

			for (prDeco = g_listLdDecoObj2[h][w].begin(); prDeco != g_listLdDecoObj2[h][w].end(); prDeco++)
			{
				pDecoObj = *prDeco;
				for (int i=0; i < 5; i++)
				{
					if (pDecoObj->m_nLoloId == nGateIndex[i])
					{
						if ( i == nGateNum )
						{
							// gate를 다운 시킨다.
							pDecoObj->AdjustYPos(-30.0f, FALSE, TRUE);
							if (g_pDSound)
							{
								g_pDSound->Play(g_SheetSound[SHEET_SND_STONE]); ///2007로딩변경때 수정
							}
						}
						else
						{
							if (m_Slot[i] == NULL && pDecoObj->m_fAdjustY < 0.0f)
							{
								pDecoObj->AdjustYPos(0.0f, TRUE, TRUE);
							}
						}
					}
				}
			}
		}
	}
}


void CSheet::ResetAllGate()
{
	int nGateIndex[5] = {33, 34, 35, 36, 37};

	CLdDecoObj *pDecoObj;
	list<CLdDecoObj *>::iterator prDeco;

	int w, h, w_pos, h_pos;
	D3DVECTOR vPos, vEye;

	w_pos = gLdMapWidth*gLdMapExtention;
	h_pos = gLdMapHeight*gLdMapExtention;

	w_pos /= g_nLdMapCellWidth;
	h_pos /= g_nLdMapCellHeight;
	w_pos /= 2;
	h_pos /= 2;

	for (h = h_pos - 15; h <= h_pos+15; h++)
	{
		if (h < 0 || h >= g_nLdMapCellHeightCount)	// 맵 영역을 벗어났는가 확인.
			continue;
		for (w = w_pos-15; w <= w_pos+15; w++)
		{
			if (w < 0 || w >= g_nLdMapCellWidthCount)
				continue;

			for (prDeco = g_listLdDecoObj2[h][w].begin(); prDeco != g_listLdDecoObj2[h][w].end(); prDeco++)
			{
				pDecoObj = *prDeco;
				for (int i=0; i < 5; i++)
				{
					if (m_Slot[i] == NULL && pDecoObj->m_fAdjustY < 0.0f)
					{
						pDecoObj->AdjustYPos(0.0f, TRUE, FALSE);
					}
				}
			}
		}
	}
}
void CSheet::BltClip(CSurface *pSurface, int dest_x, int dest_y)
{
	if( !g_pDisplay || !pSurface)
		return;

	RECT cliRect;
	GetClientRect(pCMyApp->Get_hWnd(), &cliRect);

	RECT srcRect, destRect;
	//int nSrcWidth, nSrcHeight;

	srcRect.left = 0;
	srcRect.top = 0;
	srcRect.right = pSurface->GetWidth();
	srcRect.bottom = pSurface->GetHeight();

	// 양수인 경우. (그림이 화면에 다 들어오는 경우.
	destRect.left = dest_x;
	destRect.top = dest_y;
	destRect.right = dest_x + pSurface->GetWidth();
	destRect.bottom = dest_y + pSurface->GetHeight();

	// left clipping
	if (dest_x < 0 && dest_x+srcRect.right < 0)
	{
		return;
	}
	else if (dest_x < 0 && dest_x+srcRect.right < cliRect.right)
	{
		srcRect.left = -dest_x;
		destRect.left = 0;
		destRect.right = srcRect.right + dest_x;
	}
	else if (dest_x >= 0 && dest_x+srcRect.right < cliRect.right)
	{
		destRect.left = dest_x;
		destRect.right = dest_x+srcRect.right;
	}
	else if (dest_x <= cliRect.right && dest_x+srcRect.right >= cliRect.right)
	{
		destRect.right = cliRect.right;
		//srcRect.right = -(cliRect.right - dest_x);
		srcRect.right = cliRect.right - dest_x;
	}
	else if (dest_x > cliRect.right)
	{
		return;
	}
	else
		return;

	g_pDisplay->Blt(destRect.left, destRect.top, destRect.right, destRect.bottom,
					pSurface->GetDDrawSurface(), &srcRect, DDBLT_KEYSRC);
}

//===========================================================================
// nate 2004 - 12
// opening
void CSheet::SetOpeningChar( int nIndex, int nRace, int nSex )
{
	int body_offset = 0;
	int hair_offset = 0;
	int nHair = 0;
	int i = 0;

	if( m_pOpeningCha[  nIndex  ] )
	{
		delete m_pOpeningCha[ nIndex ];
		m_pOpeningCha[ nIndex ] = NULL;
	}
	m_pOpeningCha[ nIndex ] = new CNkCharacter;


// by evilkiki 신규캐릭터체크
	if (m_pOpeningCha[ nIndex ])
	{
		body_offset = (nRace * 10) + (nSex*5);		// 10는 종족내, 5은 성별로 구분..
		hair_offset = 50 + (nRace * 20) + (nSex*10);	// 50은 기본 오프셋 50으로 변경

		if( nRace == 4 && nSex == 1 )
			hair_offset = 131;

		CBiped *pBiped= new CBiped;

		char filename[128] = {0, };
		switch( nIndex )
		{
		case 0:
			strcpy(filename , "data/Character/bpd/bm_run_2axe.bpd" );
			break;
		case 1:
			strcpy(filename , "data/Character/bpd/w_run_1hand.bpd" );
			break;
		case 2:
			strcpy(filename , "data/Character/bpd/a_run_1hand.bpd" );
			break;
		case 3:
			strcpy(filename , "data/Character/bpd/w_run_2gun.bpd" );
			break;
		case 4:
			strcpy(filename , "data/Character/bpd/fm_run_2axe.bpd" );
			break;
		case 5:
			strcpy(filename , "data/Character/bpd/fw_run_Dsword.bpd" );
			break;
		}

		pBiped->Read(filename);


		m_pOpeningCha[ nIndex ]->SetBiped( pBiped );

		CSYMAP_it it = pCMyApp->m_race_csy_map.begin();
		for (int i=0; i < NKCHA_HAIR; i++)
		{
			it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(i&0xFF)<<8);
			if( it != pCMyApp->m_race_csy_map.end() )
				m_pOpeningCha[ nIndex ]->SetCapsyong((*it).second, i);
			else
			{
				_asm nop;
			}
		}

		it = pCMyApp->m_race_csy_map.find((nRace&0xFF)<<24|(nSex&0xFF)<<16|(5&0xFF)<<8|(nHair&0xFF));
		if( it != pCMyApp->m_race_csy_map.end() )
			m_pOpeningCha[ nIndex ]->SetCapsyong((*it).second,NKCHA_HAIR);
		else
		{
			_asm nop;
		}

		m_pOpeningCha[ nIndex ]->m_Sex = nSex;
		m_pOpeningCha[ nIndex ]->m_Race = nRace;
		m_pOpeningCha[ nIndex ]->m_BpdRace = nRace * 2 + nSex;

		switch( nIndex )
		{
		case RACE_BULKAN:
			{
				// 불칸 남자
				m_pOpeningCha[ nIndex ]->m_wx = 1455.27f;
				m_pOpeningCha[ nIndex ]->m_wy = 368.000f;
				m_pOpeningCha[ nIndex ]->m_wz = 1205.11f;
				m_pOpeningCha[ nIndex ]->m_dir = 2.4f;
			}
			break; // 바로 바라보는 방향

		case RACE_KAILIPTON:
			{
				// 카이 여자
				m_pOpeningCha[ nIndex ]->m_wx = 1525.27f;
				m_pOpeningCha[ nIndex ]->m_wy = 368.000f;
				m_pOpeningCha[ nIndex ]->m_wz = 1100.11f;
				m_pOpeningCha[ nIndex ]->m_dir = 2.3f;
			}
			break;
		case RACE_AIDIA:
			{
				// 에이디아 남자
				m_pOpeningCha[ nIndex ]->m_wx = 1530.27f;
				m_pOpeningCha[ nIndex ]->m_wy = 368.000f;
				m_pOpeningCha[ nIndex ]->m_wz = 1135.11f;
				m_pOpeningCha[ nIndex ]->m_dir = 2.45f;
			}
			break;
		case RACE_HUMAN:
			{
				// 휴먼 여자
				m_pOpeningCha[ nIndex ]->m_wx = 1486.27f;
				m_pOpeningCha[ nIndex ]->m_wy = 368.000f;
				m_pOpeningCha[ nIndex ]->m_wz = 1116.11f;
				m_pOpeningCha[ nIndex ]->m_dir = 2.25f;
			}
			break;
		case RACE_FREAK:
			{
				// 프리크 남자
				m_pOpeningCha[ nIndex ]->m_wx = 1490.27f;
				m_pOpeningCha[ nIndex ]->m_wy = 368.000f;
				m_pOpeningCha[ nIndex ]->m_wz = 1150.11f;
				m_pOpeningCha[ nIndex ]->m_dir = 2.35f;
			}
			break;
// 		case 5:
// 			{	// 프리크 여자
// 				m_pOpeningCha[ nIndex ]->m_wx = 1465.27f;
// 				m_pOpeningCha[ nIndex ]->m_wy = 368.000f;
// 				m_pOpeningCha[ nIndex ]->m_wz = 1145.11f;
// 				m_pOpeningCha[ nIndex ]->m_dir = 2.2f;
// 			}
// 			break;
		}
	}
}

// 옷 입히고...
void CSheet::SetOpeningCharWear()
{
	// nate 2004 - 12 : opening - 0:불칸, 1:카이, 2:에댜, 3:휴먼, 4:하이브리더

	// nate 2005 - 01 : opening - 아이템 장착

	if( rand()%1000 < 30 ) // 만분의 1확률로 캐릭터들이 루돌프 복장을 입는다. 폭탄.ㅋㅋ
	{
		// 불칸
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_HELMET, -1, 1501 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_ARMOR, -1, 1502 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_PANTS, -1, 1503 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_CLOAK, -1, 730 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_SHOES, -1, 1201 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_SHIELD, -1, 1027 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_WEAPON, -1, 1319 );
		m_pOpeningCha[ RACE_BULKAN ]->ToggleCombat();
		// 카이
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_HELMET, -1, 1501 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_ARMOR, -1, 1502 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_PANTS, -1, 1503 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_CLOAK, -1, 1029 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_SHOES, -1, 1213 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_SHIELD, -1, 1021 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_WEAPON, -1, 1320 );
		m_pOpeningCha[ RACE_KAILIPTON ]->ToggleCombat();
		// 에이디아
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_HELMET, -1, 1501 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_ARMOR, -1, 1502 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_PANTS, -1, 1503 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_CLOAK, -1, 575 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_SHOES, -1, 899 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_WEAPON, -1, 1317 );
		m_pOpeningCha[ RACE_AIDIA ]->ToggleCombat();
		// 휴먼
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_HELMET, -1, 1501 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_ARMOR, -1, 1502 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_PANTS, -1, 1503 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_CLOAK, -1, 756 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_SHOES, -1, 1221 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_SHIELD, -1, 1028 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_WEAPON, -1, 1316 );
		m_pOpeningCha[ RACE_HUMAN ]->ToggleCombat();
		// 프리크 남
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_HELMET, -1, 1501 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_ARMOR, -1, 1502 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_PANTS, -1, 1503 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_SHOES, -1, 1197 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_WEAPON, -1, 1325 );
		m_pOpeningCha[ RACE_FREAK ]->ToggleCombat();

// 		m_pOpeningCha[ 5 ]->Wear( WEARING_HELMET, -1, 1501 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_ARMOR, -1, 1502 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_PANTS, -1, 1503 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_SHOES, -1, 1173 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_WEAPON, -1, 1323 );
// 		m_pOpeningCha[ 5 ]->ToggleCombat();
	}
	else
	{
		// 불칸
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_HELMET, -1, 1198 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_ARMOR, -1, 1199 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_PANTS, -1, 1200 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_CLOAK, -1, 730 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_SHOES, -1, 1201 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_SHIELD, -1, 1027 );
		m_pOpeningCha[ RACE_BULKAN ]->Wear( WEARING_WEAPON, -1, 1319 );
		m_pOpeningCha[ RACE_BULKAN ]->ToggleCombat();
		// 카이
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_HELMET, -1, 1210 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_ARMOR, -1, 1211 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_PANTS, -1, 1212 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_CLOAK, -1, 1029 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_SHOES, -1, 1213 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_SHIELD, -1, 1021 );
		m_pOpeningCha[ RACE_KAILIPTON ]->Wear( WEARING_WEAPON, -1, 1320 );
		m_pOpeningCha[ RACE_KAILIPTON ]->ToggleCombat();
		// 에이디아
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_HELMET, -1, 896 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_ARMOR, -1, 897 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_PANTS, -1, 898 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_CLOAK, -1, 575 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_SHOES, -1, 899 );
		m_pOpeningCha[ RACE_AIDIA ]->Wear( WEARING_WEAPON, -1, 1317 );
		m_pOpeningCha[ RACE_AIDIA ]->ToggleCombat();
		// 휴먼
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_HELMET, -1, 1218 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_ARMOR, -1, 1219 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_PANTS, -1, 1220 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_CLOAK, -1, 756 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_SHOES, -1, 1221 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_SHIELD, -1, 1028 );
		m_pOpeningCha[ RACE_HUMAN ]->Wear( WEARING_WEAPON, -1, 1316 );
		m_pOpeningCha[ RACE_HUMAN ]->ToggleCombat();
		// 프리크 남
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_HELMET, -1, 1194 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_ARMOR, -1, 1195 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_PANTS, -1, 1196 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_SHOES, -1, 1197 );
		m_pOpeningCha[ RACE_FREAK ]->Wear( WEARING_WEAPON, -1, 1325 );
		m_pOpeningCha[ RACE_FREAK ]->ToggleCombat();
		// 프리크 여
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_HELMET, -1, 1170 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_ARMOR, -1, 1171 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_PANTS, -1, 1172 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_SHOES, -1, 1173 );
// 		m_pOpeningCha[ 5 ]->Wear( WEARING_WEAPON, -1, 1323 );
// 		m_pOpeningCha[ 5 ]->ToggleCombat();
	}
}
//===========================================================================

HRESULT CSheet::Restore()
{
	HRESULT hr = S_OK;

	if( !g_pDisplay )
		return hr;

	m_pLoginPg->Restore();
	m_pSelCharPg2->Restore();
	m_pOptionPg->Restore();
	m_pCrtCharPg->Restore();

	return hr;
}