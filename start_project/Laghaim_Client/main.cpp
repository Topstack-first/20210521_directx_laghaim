#include "stdafx.h"
#define D3D_OVERLOADS
#include <tchar.h>
#include <math.h>
#include <time.h>
#include <process.h>
#include <stdlib.h>
#include <assert.h>
#include <direct.h>
#include <DbgHelp.h>
#include "D3DApp.h"
#include "D3DFrame.h"
#include "D3DMath.h"
#include "LightCtrl.h"
#include "main.h"
#include "Land.h"
#include "resource.h"
#include "Frustum.h"
#include "HanIme.h"
#include "ObjectList.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "UIMgr.h"
#include "GUtil.h"
#include "DecoObj.h"
#include "DirtSnd.h"
#include "Tcpipcon.h"
#include "TcpUtil.h"
#include "ClrAlloc.h"
#include "DMusicMgr.h"
#include "Sheet.h"
#include "Compas.h"
#include "WeatherMgr.h"
#include "GoodShadow.h"
#include "LightCtrl.h"
#include "Mouse.h"
#include "ddutil.h"
#include "ChaAct.h"
#include "ControlBottom.h"
#include "ControlScore.h"
#include "ControlPetShop2.h"
#include "SMRender.h"
#include "MsgPopUp.h"
#include "FxSet.h"
#include "pages.h"
#include "ItemProto.h"
#include "FireFly.h"
#include "skill.h"
#include "Help.h"
#include "ControlExchange.h"
#include "ControlParty.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "g_ImageManager.h"
#include "scrollbox.h"
#include "2dresmgr.h"
#include "Path.h"
#include "WarInfo.h"
#include "refine.h"
#include "rshiftmenu.h"
#include "LShiftMenu.h"
#include "changehair.h"
#include "wartax.h"
#include "guardtower.h"
#include "warrequest.h"
#include "ControlPetShop.h"
#include "ControlPeiceShop.h"
#include "ControlPetKeep.h"
#include "guildwar.h"
#include "matlist.h"
#include "matmakeroom.h"
#include "NewListBox.h"
#include "matbattleui.h"
#include "matsurvivalui.h"
#include "CloudSea.h"
#include "SkyPlane.h"
#include "Country.h"
#include "gamble.h"
#include "Event_Ancient.h"
#include "QuestData.h"
#include "Mp3Module.h"
#include "ControlTeleportItem.h"
#include "NKObjQuadNode.h"
#include "ControlLowGuildMaster.h"
#include "GuildHunt.h"
#include "DmitronBattle.h"
#include "ReservationWindow.h"
#include "Pet_Level.h"
#include "g_stringmanager.h"
#include "g_devlogbox.h"
#include "Process.h"
#include "WindowMgr.h"
#include "ItemRender.h"

#include "CsyManager.h"
#include "LoloManager.h"

#include "WindowMgr.h"
#include "PVPWidget.h"
#include "ControlGuildT.h"

#include "LH_SelectCharacterPage.h"

#include "PremiumExchange.h"

#include "LHUI_OpEvent.h"

#include "AutoPlay_test.h"


#include "NPC_Define.h"

#include "EventOnOffMgr.h"

#include "LHExcelReader.h"
#include "LHParam_Manager.h"
#include "LHPath_Define.h"

#include "BugTrap.h"
#pragma comment(lib, "BugTrap.lib")

#include "DefenseValMgr.h"
#include "ItemFakeOptionMgr.h"
#include "ItemOutlineMgr.h"
#include "ChatUtilMgr.h"

#include "UI_GmMacro.h"
#include "DataFriend.h"

#include "md5.h"

#include "CommonConfig.h"

#include "SpeedCheck.h"

#include "FishingManager.h"
#include "FishingRewardManager.h"

#include "DecardiBattleManager.h"
#include "rnpacket.h"

#include "BuffItemMgr.h"

#include "PacketSend.h"

#include "LottoManager.h"

#include "SetItemEffMgr.h"

#include "BingoManager.h"
#include "LetterManager.h"
#include "BaseballManager.h"
#include "LotteryManager.h"
#include "DiceGameManager.h"
#include "DiceGameRewardManager.h"

#include "SkillExtendManager.h"

//#include <iostream>
//#include <string>
//#include<tchar.h>
//#include <process.h>
//#include <windows.h>
//#include <tlhelp32.h>


//#include <boost/thread/thread.hpp> 


#pragma comment(lib, "Psapi")
#include <Psapi.h>

#include "..\Laghaim_Update\UpdateThread.h"

#include <atlbase.h>

#include <GdiPlus.h>
using namespace Gdiplus;
#include <gdiplusimaging.h>
#include "SplashWnd.h"



#pragma warning( disable : 4995 )

//////////////////////////////////////////////////////////////////////////
// define
#define CAMERA_HEIGHT_DIFF	100.0f
#define TIMER_HOUR			1
#define TIMER_INTERVAL		3000
#define MAX_NUM_WAV			555

const int MAX_CAPSYONG_TEXTURE = 4000;


//////////////////////////////////////////////////////////////////////////
// extern func
extern LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
extern void	PlayerInitialLoading();
extern void DecoObjInitialLoading( char* strFilePath, char* strFileName );
extern void DeletePlayerData();
extern void DeleteDecoObjData();
extern void MobInitialLoading(int world_num);
extern void PetInitialLoading();
extern void DeletingMobLoading();
extern void DeletingPetLoading();
extern int GetMagicDistance(int skill_level);
extern void Dummy( int nCode, FILE *fp );
extern void PDummy( int nCode, PFILE *fp );
extern void Decode( char* strTemp, int nCode, bool bTap );
extern BOOL LoadItems();
extern void LoadEffectSound();
extern int GetSnifingRange(int skill_level);

extern void SendLogin(const int& type, const bool& web, const bool& flag, const bool& dev, const char* id, const char* pw);

//////////////////////////////////////////////////////////////////////////
// extern variable
extern enum 
{
	SKILL_DTA = 0x70,
	RACE_SKILL = 0x57,
	BPDINDEX = 0x86,
	BPDLIST = 0x23,
	RACECSY = 0x94
};

extern GUID					g_AppGUID;
extern TCHAR				g_strAppName[256];
extern HINSTANCE			g_hInst;
extern IndexedTexture*		g_pMobTexture;
extern CChaAct				g_ChaActList[2][MAX_CHA_ACT_NUM];
extern list<CDecoObj*>		g_listDecoObj[MAP_CELL_HEIGHT_COUNT][MAP_CELL_WIDTH_COUNT];
extern list<WORD>			g_listRenderedDecoColor;
extern int					gMapHeight;
extern int					gMapWidth;
extern int					gMapHeight;
extern int					gMapExtention;
extern float				gfMapExtention;
extern int					g_EffectSound[];
extern POINT				g_po_ime;
extern POINT				g_po_edit;
extern float				g_SummonScale;
extern DWORD				g_MySummon;
extern BOOL					g_bNewLetter;
extern int					g_EffectTextr[];
extern DWORD				g_skyFogColor[];
extern BOOL					g_bSendSalesOff;
extern BOOL					g_bSnow;
extern int					g_nServerIndex;
extern CDecoObj*			g_pGuildGateClosed;
extern CDecoObj*			g_pGuildGate2Opened;
extern CDecoObj*			g_pOutsideDoorClosed;
extern CDecoObj*			g_pCastleGateClosed;
extern CDecoObj*			g_pBossGateClosed;
extern BOOL					g_bSakura;

//////////////////////////////////////////////////////////////////////////
// global func
void DisplayError( CHAR* strMessage );
void DetermineLang();
bool CheckUpdate();
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );
void SetupBugTrap();
void SetupSingletons();

//////////////////////////////////////////////////////////////////////////
// global variable
bool m_helldown_ing = true;
HDC hdc;
HWND g_hWnd;
float g_fTimeKey;
DWORD g_dwBaseTime;
float g_SummonScale = 0.0f;
float g_fDSDistance = 20.0f;
BOOL g_bNewLetter = FALSE;
int g_ClickSound = -1;
float g_fAccTimeKey;
DWORD g_dwSpeedHackBaseTime;
int g_nBaseClock;
float g_fAccClock;
HCURSOR g_hCursor;
CMyD3DApplication * pCMyApp = NULL;
CClrAlloc* g_ClrAlloc;

// ???? ????.
BOOL	g_bCsyMipmap = TRUE;
BOOL	g_bMipmap = TRUE;
BOOL	g_bDetailMipmap = TRUE;
BOOL	g_bDrawDetailGround = TRUE;
int		g_nCurWorldIdx;

// ???? ????.
BOOL g_bDungeon = TRUE;
BOOL g_bDviewUp = TRUE;
BOOL g_bDviewBottom = TRUE;
BOOL g_bNc = FALSE;
BOOL g_bGuildWar = TRUE;
BOOL g_bNewTerrainDungeon = FALSE;
BOOL g_bHuman = TRUE;
BOOL g_bRestart = FALSE;
BOOL g_bWorldLoading = FALSE;
BOOL g_bTimerHelp = FALSE;
BOOL g_bDummyMsg = FALSE;

DWORD nCurrentPID;
HANDLE pProcessHandle;
DWORD nCurrentPIDold;
HANDLE pProcessHandleold;
int nNumberOfModulesold;

// ?????? ???? ????????
int		g_nMobRenNum = 0;
int		g_nMobInNum = 0;
int		g_nMobListNum = 0;
int		g_nPlRenNum = 0;
int		g_nPlInNum = 0;
int		g_nPlListNum = 0;
int		g_nItemListNum = 0;
int		g_nPetRenNum = 0;
int		g_nPetInNum = 0;
int		g_nPetListNum = 0;

// ?????? ???? ????????.
CLolos	*g_pWhangLolo1;
CLolos	*g_pWhangLolo2;
CLolos	*g_pWhangLolo3;
CLolos	*g_pWhangLolo4;

BOOL	g_bWXY_FORMAT =	TRUE;
BOOL	g_bUse3DSound = FALSE;
char	g_strPartner[20];
BOOL	g_bTestServer = FALSE;
BOOL	g_bNewDungeon = TRUE;
BOOL	g_bAidia = TRUE;
BOOL	g_bFFullMode = TRUE;
BOOL	g_bAdmin = FALSE;
BOOL	g_bAdmin2 = FALSE;
BOOL	g_bPath = FALSE;
BOOL	g_bGhost = FALSE;
BOOL	g_bUseVoiceChat = FALSE;
DWORD	g_dwLangType = LANG_ENG;
BOOL	g_ThaiClient = FALSE;
DWORD	g_dwClientCountry = CTRY_KOR;  

BOOL    g_bFixedWidthFont = FALSE;
BOOL	g_bMapAttrEncrypt = FALSE;
int		g_MatrixSvr = MATRIXSVR_NO;
BOOL    g_bMatrixMatch = FALSE;
char	g_strFontName[100];

float g_FogStart = 0.06f;
float g_FogEnd = 0.33f;

D3DMATRIX g_matView;
D3DMATRIX g_invView;

DWORD	g_dwBeforeGoWorldTime;
BOOL	g_bBeforeGoSky = FALSE;
BOOL	g_bNewTerrain = FALSE;
BOOL	g_bFreeUser = FALSE;

HINSTANCE g_hDllInst = NULL;

BOOL g_bUseMatrixCountry = FALSE;
BOOL g_bUseCashShop;
BOOL g_bUseEventWear = FALSE;
BOOL g_bGlobalMatrix = FALSE;
//BOOL g_bGlobalMatrix = TRUE; //ENABLE FOR MATRIX WAR ONLY  PABLO 02/10/2019

SVR_TYPE g_SvrType = ST_12_OVER;

eServerMode g_SvrMode = eSVR_NORMAL;

bool g_dev_client = false;
bool g_EventMad_Aleady = false;
//
//////////////////////////////////////////////////////////////////////////

std::string GetMd5CheckSum()
{
	MD5 md5;
	std::string strMd5 = md5.digestFile("Game.exe");
	return strMd5;
}

void DetermineLang()
{
	g_dwClientCountry = CTRY_KOR;

	if( CommonConfig::Instance()->GetThaFlag() == true )
	{
		g_dwClientCountry = CTRY_TH;
	}

	LANGID langId;

	g_bUseMatrixCountry = TRUE;
	g_bUseEventWear = TRUE;
	g_bMapAttrEncrypt = FALSE;

	if( g_dwClientCountry == CTRY_TH )
	{
		g_bUseCashShop = TRUE;
	}
	else
	{
		g_bUseCashShop = FALSE;	
	}	

	if (g_dwClientCountry == CTRY_TH)
	{
		g_dwLangType = LANG_TH;
		strcpy(g_strFontName, "MS Sans Serif");
		g_bMapAttrEncrypt = TRUE;
		g_bFixedWidthFont = FALSE;

		return;
	}

	langId = GetSystemDefaultLangID();
	
	if(GetSystemDefaultLangID() == 0x0412)//Korean
	{
		g_dwLangType = 0;
		strcpy(g_strFontName, "??????");
	}
	else if(GetSystemDefaultLangID() == 0x0411)// Japanese
	{
		g_dwLangType = 1;
		strcpy(g_strFontName, "?l?r ????");
	}
	else if (GetSystemDefaultLangID() == 0x0404)
	{
		// Chinese Taiwan
		g_dwLangType = 3;
		strcpy(g_strFontName, "??????"); // se myung che
	}
	else if (GetSystemDefaultLangID() == 0x0804)
	{
		// Simplication Chinese
		g_dwLangType = 4;
		strcpy(g_strFontName, "????");					// song che
	}
	else if(GetSystemDefaultLangID() == 0x041E)// Thai
	{
		g_dwLangType = 0;
		strcpy(g_strFontName, "MS Sans Serif");
		g_ThaiClient = true;
	}
	else
	{
		g_dwLangType = 0;
		//strcpy(g_strFontName, "MS Gothic");
		strcpy(g_strFontName, "??????");
	}
}

void Check3DSound()
{
	FILE *fp = NULL;

	if ( (fp=fopen("3dsound.ini", "rt")) != NULL)
	{
		fscanf(fp, "%d", &g_bUse3DSound);
		fclose(fp);
	}
	else
	{
		g_bUse3DSound = FALSE;
	}
}

bool CheckUpdate()
{
	FILE* fp = fopen("LaghaimNew.exe", "rb");
	if( fp )
	{
		fclose(fp);
		Sleep(1000);

		int LoopCount = 0;

		while( remove("Laghaim.exe") ) // ?????? ???????? ???????? remove ?? ???? MFC ???? ?????? ???? ?????? ???? ?? ????.(100%?????? ????). ???? ???????? ???????????? ?????????? ?????? ???????? ??????. ?????? 5???? ???? 5?? ???? ?? ???????????? ???? (07-01-04 ????)
		{
			Sleep(1000);
			LoopCount++;

			if( LoopCount > 5 ) // 5?? ?? ?????????? ??????..
			{
				if( g_dwLangType == 0)
					MessageBox(NULL, "?????????? ???? ?????? UpdateNew.exe?? ???? ???????? ??????.", "Launcher Update Failed!", MB_OK);
				else
					MessageBox(NULL, "Launcher Update Failed. Please run [UpdateNew.exe] for Manual Update", "Launcher Update Failed!", MB_OK);

				return false; // ???? ??????????????.
			}
		}

		Sleep(1000);

		int ret = rename("LaghaimNew.exe", "Laghaim.exe");
		if (!ret)
		{			
			MessageBox(NULL, "Update is finished.\nRestart the game, please.", "Update Finished!", MB_OK);

			return false;
		}
		else // rename ???????? ?????? ???????? ????. (07-01-04 ????)
		{
			if( g_dwLangType == 0)
				MessageBox(NULL, "?????????? ???? ?????? UpdateNew.exe?? ???? ???????? ??????.", "Launcher Update Failed!", MB_OK);
			else
				MessageBox(NULL, "Launcher Update Failed. Please run [UpdateNew.exe] for Manual Update", "Launcher Update Failed!", MB_OK);

			return false; // ???? ??????????????.
		}
	}

	return true;
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	if (strlen(strCmdLine) <= 0)
	{
		MessageBox(NULL, "This Program Cannot Be Excuted Solely", "Error!!!", MB_OK);
		return 0;
	}

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CSplashWnd splash;
	Image* pImage = new Image(L".\\Interface\\Loading\\Update.bmp");
	splash.SetImage(pImage);
	splash.Show();

	UpdateThread();
	pPackInitialize();

	if( CommonConfig::Instance()->Init() == false )
	{
		OutputDebugString("CommonConfig Load Fail");
	}

	if( SkillExtendManager::instnace()->Init() == false )
	{
		OutputDebugString("SkillExtendManager Load Fail");
	}

	DetermineLang();
	Load_Global_Font();

	if( CheckUpdate() == false )
	{
		return 0;
	}	

	rnPacket::init();
	CTcpIpConnection::InitSeedEncrypt();
	
	SetupBugTrap();
	SetupSingletons();				
	
	Check3DSound();		

	CMyD3DApplication d3dApp;
	d3dApp.hInstApp = hInst;

	if(g_bFFullMode == FALSE && d3dApp.IsWinMode() == TRUE)
	{
		HDC hdc;
		hdc = GetDC(NULL);
		int nBit = GetDeviceCaps(hdc, BITSPIXEL);
		ReleaseDC(NULL, hdc);
	}
	
	CoInitialize(NULL);
	splash.Hide();
	if( FAILED( d3dApp.Create( hInst, strCmdLine ) ) )
	{
		return 0;
	}

	HWND hwnd = d3dApp.Get_hWnd();
	g_hWnd = d3dApp.Get_hWnd();
	HIMC hIMC=NULL;

	SetWindowText(g_hWnd, "LaghaimOnline New");	

	hIMC = ImmGetContext(hwnd);
	if (hIMC)
	{
		ImmGetConversionStatus(hIMC,&d3dApp.m_dwImmConv,&d3dApp.m_dwImmSent);
		d3dApp.m_bImmOpen = ImmGetOpenStatus(hIMC);
		ImmReleaseContext(hwnd,hIMC);
	}

	d3dApp.m_hkl = GetKeyboardLayout(0);

	// ?????? ?????? ????.
	if (SetTimer (hwnd, 1, 60000, NULL))
		g_bTimerHelp = TRUE;
	else
		g_bTimerHelp = FALSE;

	if (SetTimer (hwnd, 2, 30000, NULL))
		g_bDummyMsg = TRUE;
	else
		g_bDummyMsg = FALSE;

	SetTimer (hwnd, 3, 600000, NULL);
	SetTimer (hwnd, 4, 10000, NULL);
	SetTimer (hwnd, 5, 4500, NULL);
	SetTimer (hwnd, 6, 1000, NULL);
	SetTimer (hwnd, 7, 10000, NULL);

	nCurrentPID = 0;
	pProcessHandle = NULL;
	SetTimer (hwnd, 8, 1000, NULL);

	// ???????? ???? ????.
	char *strPath = NULL;
	strPath = new char[_MAX_PATH+_MAX_FNAME+_MAX_EXT];
	_getcwd(strPath, _MAX_PATH+_MAX_FNAME+_MAX_EXT);
	strcpy(d3dApp.GetExePath(), strPath);
	delete [] strPath;	

	EventOnOffMgr::GetInstance();

	
	d3dApp.InitVariables();	
	d3dApp.LoadResources();			
	d3dApp.Run();
	SetCursor(NULL);
	FreeLibrary(g_hDllInst);
	DestroyCursor(g_hCursor);
	CoUninitialize();

	rnPacket::deinit();
		

	pPackUnInitialize();
	GdiplusShutdown(gdiplusToken);
	return TRUE;
}

//---------------------------------------------------------------------------
// Name: AboutProc()
// Desc: Minimal message proc function for the about box
//---------------------------------------------------------------------------
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog (hWnd, TRUE);

	return ( WM_INITDIALOG == uMsg ) ? TRUE : FALSE;
}

//---------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//---------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
	m_strWindowTitle  = TEXT( "LaghaimOnlineNew" );
	pCMyApp = this;
//	g_MyApp = this;

	// TCPIP /////////////
	WORD      wVersionRequested;
	WSADATA   wsaData;

	wVersionRequested = MAKEWORD(1,1);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		WSACleanup();
		return ;
	}
	//////////////////
	m_bWindowed = TRUE;
	Read_ChooseMode("win_mode.txt");
	Init();


	//[2009/4/16 Theodoric] mutex
	m_hMutex = NULL;

	m_iCurWorldID = 4;	// [2008/5/6 Theodoric] ?? ???????? ?????? ?????? ?????? ???? ?????????? ???????? ???? ?????? 4?? ???? ????.

	m_param_manager = new LHParam_Manager;
	m_param_manager->Load();
}

//--------------------------------------------------------------------------
// App Create
//--------------------------------------------------------------------------
// GAME ?? Create ????.
HRESULT CMyD3DApplication::Create( HINSTANCE hInst, TCHAR* strCmdLine)
{
	HRESULT hr;
	// ???????? ????

	g_hInst = hInst;

	if (FAILED(hr = EnumerateAndSelect()))
		return hr;

	// Initialize the app's custom scene stuff
	if( FAILED( hr = OneTimeSceneInit() ) )
		return hr;

	CD3DFramework7* pFramework;
	if( NULL == ( pFramework = new CD3DFramework7() ) )
		return E_OUTOFMEMORY;

	SetFramework(pFramework);

	// ?????? ?????? ????
	WNDCLASS wndClass = { CS_DBLCLKS, WndProc, 0, 0, hInst,
						  LoadIcon( hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
						  NULL,
						  (HBRUSH)GetStockObject(HOLLOW_BRUSH),
						  NULL, _T("D3D Window")
						};

	RegisterClass( &wndClass );

	// ?????? ????
	DWORD dwStyle;
	RECT rect = {0, 0, 800, 600};


	if (m_bWindowed)
	{
		if (!g_bFFullMode)
		{
			if (!g_bAdmin)
			{
				dwStyle = 0;
				dwStyle = WS_SYSMENU|WS_CAPTION|WS_BORDER|WS_POPUP|WS_VISIBLE;
				dwStyle ^= WS_MAXIMIZE;
				dwStyle ^= WS_MINIMIZE;

				AdjustWindowRect(&rect, dwStyle, FALSE);
				m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
									   dwStyle,
									   CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top, 0L,
									   NULL,
									   hInst, 0L );
			}
			else
			{
				dwStyle = WS_SYSMENU|WS_CAPTION|WS_BORDER|WS_POPUP|WS_VISIBLE;
				AdjustWindowRect(&rect, dwStyle, TRUE);

				m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
									   dwStyle,
									   CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top, 0L,
									   LoadMenu( hInst, MAKEINTRESOURCE(IDR_MENU) ),
									   hInst, 0L );
			}
		}
		else
		{
			m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
								   WS_POPUP|WS_VISIBLE,
								   0, 0, rect.right-rect.left, rect.bottom-rect.top, 0L,
								   NULL,
								   hInst, 0L );
		}
	}
	else
	{
		m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle,
							   WS_POPUP|WS_VISIBLE,
							   0, 0, 800, 600, 0L,
							   NULL,
							   hInst, 0L );
	}

	if (!m_hWnd)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}

	UpdateWindow( m_hWnd );

	// EnumerateAndSelect() ?????? ???????? ???????? ?????????? ???? ?????? ??????.
	if (m_pDeviceInfo && !m_bWindowed)
	{
		m_pDeviceInfo->ddsdFullscreenMode.dwWidth = 800;
		m_pDeviceInfo->ddsdFullscreenMode.dwHeight = 600;
	}

	// Initialize the 3D environment for the app
	if( FAILED( hr = Initialize3DEnvironment() ) )
	{
		MessageBox(0, "Init 3d fail", 0, 0);
		Cleanup3DEnvironment();
		return E_FAIL;
	}

	// Setup the app so it can support single-stepping
	SetBaseTime(timeGetTime());
	// ?????????????? ?????? ?????? ??????.
	SetbReady(TRUE);

	return S_OK;
}

//---------------------------------------------------------------------------
// Name : ConvertImmStatus()
// Desc : ???? ?????? ????.
//---------------------------------------------------------------------------
void CMyD3DApplication::ConvertImmStatus()
{
	HIMC hImc = NULL;
	HKL hkl;

	if (SaveImmStatus())
	{
		if (g_dwLangType == 3 || g_dwLangType == 4)
		{
			while (ImmIsIME(hkl = GetKeyboardLayout(0)))
				ActivateKeyboardLayout( (HKL) HKL_PREV, 0);
		}
		else
		{
			hImc = ImmGetContext(m_hWnd);
			if (hImc)
			{
				ImmSetOpenStatus(hImc, FALSE);
				ImmSetConversionStatus(hImc, IME_CMODE_ALPHANUMERIC, m_dwImmSent) ;		// ???? ?????? ??????.
				ImmReleaseContext(m_hWnd, hImc);
			}
		}
	}
}

//---------------------------------------------------------------------------
// Name : SaveImmStatus()
// Desc : ???? ?????? ????.
//---------------------------------------------------------------------------
BOOL CMyD3DApplication::SaveImmStatus()
{
	HIMC hImc = NULL;

	if (g_dwLangType == 3 || g_dwLangType == 4)
	{
		m_hkl = GetKeyboardLayout(0);
		return TRUE;
	}
	else
	{
		hImc = ImmGetContext(m_hWnd);
		if (hImc)
		{
			ImmGetConversionStatus(hImc, &m_dwImmConv, &m_dwImmSent);
			m_bImmOpen = ImmGetOpenStatus(hImc);
			char temp[256];
			sprintf(temp, "%x %x", m_dwImmConv, m_dwImmSent);
			ImmReleaseContext(m_hWnd, hImc);

			return TRUE;
		}
	}

	return FALSE;
}

//---------------------------------------------------------------------------
// Name : RestoreImmStatus()
// Desc : ???? ?????? ????.
//---------------------------------------------------------------------------
void CMyD3DApplication::RestoreImmStatus()
{
	HIMC hImc = NULL;

	if (g_dwLangType == 3 || g_dwLangType == 4)
	{
		if (ImmIsIME(m_hkl))
		{
			ActivateKeyboardLayout( m_hkl, 0);
		}
	}
	else
	{
		hImc = ImmGetContext(m_hWnd);

		if (hImc)
		{
			ImmSetOpenStatus(hImc, m_bImmOpen);
			ImmSetConversionStatus(hImc, m_dwImmConv, m_dwImmSent);
			ImmReleaseContext(m_hWnd, hImc);
		}
	}
}

//---------------------------------------------------------------------------
// Name : Init()
// Desc : ???????? ???????? ???????? ????.
//---------------------------------------------------------------------------
void CMyD3DApplication::Init()
{
	if( g_dwLangType == LANG_IDN/*||g_dwLangType == LANG_DEU*/|| g_dwLangType == LANG_TH || g_dwLangType == LANG_ENG)
		// 06-11-14 ???? ???????? ????(?? ???????? ????????. ?????? ?????? ???? ?????? ?????????? ????)
		Font_AddPosY = -2;
	else
		Font_AddPosY = 0;

	m_bAppUseZBuffer  = TRUE;
	m_fnConfirmDevice = ConfirmDevice;
	m_bShowStats = FALSE;
	m_bShowStats_Nalrim = FALSE;
	m_bWireframe = FALSE;
	m_bPickView = FALSE;
	m_bPickBegin = FALSE;
	m_bShadowView = FALSE;
	m_bGameMode = FALSE;	//..
	m_pLand = NULL;
	m_pPath = NULL;
	m_pFrustum  = NULL;
	m_pCloudSea = NULL;
	m_pSkyPlane = NULL;
	m_bSatelliteMode = FALSE;
	m_bSelTile		= FALSE;
	m_pHanIme = new CHanIme;
	g_pNk2DFrame = new CNk2DFrame;
	g_pDisplay = new CDisplay;
	m_GraphicMode = NKGRAPHIC2D;	// ???? ?????? ????.
	m_pLightCtrl = new CLightCtrl;
	m_pszMapName = new char[255];
	m_MsgPopUp.Init(TYPE_ESC);
	m_pMyPet = NULL;
	m_pSlotChar = NULL;
	m_bDrawShadow = FALSE;
	m_bHeroMoving = FALSE;
	m_bInsideBuilding = FALSE;
	m_HeroAniRate = 0.05f;
	m_pDecoTexture = NULL;
	m_dpChaCapsyong = NULL;
	m_pUIMgr		= NULL;
	m_pGoodShadow = NULL;
	m_pCompas = NULL;
	m_pFxSet = NULL;
//==============================
	// nate 2004 - 4 : CastleWar
	g_pWhangLolo1 = NULL;
	g_pWhangLolo2 = NULL;
	g_pWhangLolo3 = NULL;
	g_pWhangLolo4 = NULL;
//==============================
	m_pWeatherMgr = NULL;
	m_pFireFly = NULL;
	m_bShowCutton = FALSE;
	m_bTmp = FALSE;
	m_pMouse = NULL;
	m_pMouse = new CMouse;
	if( m_pMouse )
		m_pMouse->SetDelay(50);
	m_fSpeedMul = 10.0f;
	m_fAtHeight = 14.0f;
	m_fCamHeight = 0.0f;
	m_bRotateCamera = FALSE;
	m_pDMusicMgr = NULL;
	m_pMusicManager = NULL;
	m_p2DResMgr = NULL;
	m_zNear = 5.0f;
	m_zFar = 2500.0f;
	m_Fov = 0.9250f;	// 53??.
	m_fHeroPx = 0.0f;
	m_fHeroPz = 0.0f;
	m_pNearMob = NULL;
	m_pLoadDisp = NULL;
	srand( (unsigned) time(NULL) );
	m_bPlaying = FALSE;
	m_bFastMove = FALSE;
	m_pHeroAttGoCha = NULL;
	m_pHeroAttGoMob = NULL;
	// Pick Buffer
	m_pddsPickRender = NULL;
	m_pddsPickZBuffer = NULL;

	D3DVECTOR d3dVector1;
	d3dVector1.x = 0;
	d3dVector1.y = 10.0;
	d3dVector1.z = 0.098;

	D3DVECTOR d3dVector2;
	d3dVector2.x = 0;
	d3dVector2.y = 0;
	d3dVector2.z = 0.098;

	D3DVECTOR d3dVector3;
	d3dVector3.x = 10;
	d3dVector3.y = 10.0;
	d3dVector3.z = 0.098;

	D3DVECTOR d3dVector4;
	d3dVector4.x = 10;
	d3dVector4.y = 0;
	d3dVector4.z = 0.098;

	m_pickMesh[0] = D3DTLVERTEX(d3dVector1, 1, 0, 0, 0.0f, 1.0f);
	m_pickMesh[1] = D3DTLVERTEX(d3dVector2, 1, 0, 0, 0.0f, 0.0f);
	m_pickMesh[2] = D3DTLVERTEX(d3dVector3, 1, 0, 0, 1.0f, 1.0f);
	m_pickMesh[3] = D3DTLVERTEX(d3dVector4, 1, 0, 0, 1.0f, 0.0f);

	m_nShowRohEnerge = 0;
	m_bRohRunning = TRUE;
	m_startRunningTime = timeGetTime();
	SAFE_DELETE(g_pTcpIp);
	g_pTcpIp = new CTcpIpConnection;
	m_pTcpIpAlter = NULL;
	m_bBeforeGoWorld = FALSE;
	m_bWaitingAt = FALSE;
	m_nCurWorldIdx = 4;
	g_nCurWorldIdx = m_nCurWorldIdx;
	m_iCurVillageId = -1;
	m_bHeroSwimming = FALSE;
	m_bShowRenderSky2 = TRUE;
	m_fog_start =0.1f;
	m_fog_end = 0.45f;
	m_fRotate_Cloud_Angle = 0.00f;
	m_nCurSkill = 0;							// ???????? ???? ????.
	m_IsMsgPopupInputEnable = FALSE;
	// rendersky3 ?? ?????? ????.
	sky = NULL;
	sky_cloud = NULL;
	sky_fog = NULL;
	ground = NULL;
	m_lEquipTime = 0;
	m_lClickTime = 0;//1118
	m_bEquipLock = FALSE;//???????? ??.
	m_bRingLock =FALSE;
	m_bBrLock = FALSE;
	m_bNecklaceLock = FALSE; ///??????
	m_bSelectCha = FALSE; //1125
	m_bInitEnergyBar=FALSE;
	// Happy New Year Event
	DrawHappyNewYear = FALSE;
	m_bInverseReturn = FALSE;
	m_DrvType = 0;
	m_PlaySndType = -1;
	m_IsPcBang_User = 0;
	DrawBattleStart = FALSE;
	m_nBSRendNum = 0;
	DrawPlaceInfo = FALSE;
	m_nCurPlaceNum = 0;
	m_pRegenStart = NULL;
	m_pRegenStart = new CFxRegenStart();
	m_bRegenStart = FALSE;
	DrawCasting = FALSE;		//mungmae-2005/07/14 : ?????? ???????? ????? ?????
	m_dPressTime = 0;			//mungmae-2005/07/14 : ?????? ???????? ?????? ?? ????
	m_dwCastingTime = 0;		//mungmae-2005/07/14 : ?????? ???????? ?????? ???? ????
	m_bHellDownReady = FALSE;	//mungmae-2005/07/14 : ?????? 1?????? ?????? ???? ????
	m_bHellDownSec = FALSE;
	m_bHellDown_Cooltime = FALSE;
	m_nGrade = 0;
	m_nHellDown_Grade = 0;
	m_nCurHellDown_Grade = 0;
	m_nCurHour = 0; // ???? ?????? ?????? ?????? ??????. ???? ?????? 0???? ??????. (by ????)
	m_dwPlayingTime	= 0; // ???????? ?????? ???? ??????.
	m_dwOldPlayingTime = 0; // ???? ???????? ?????? ????...

	m_RotateFrame = 0;
	m_ZoomInFrame = 0;

	m_SpiritRotateCamera = FALSE;
	m_bSpiritZoomIn = FALSE;

	m_pCasting = NULL; // ?????? ?????? ?? ?????? ?????? ???? ????. 07-03-14 ???? ????

	m_NvsnMoveDelay = 0; // ?????????? ??????  ///????????????????

	// [12/24/2007 ??????] ???? ?????? ?????? ???? ???? ?????? ??????.
	m_NewYear = NULL;

	m_bPostLoading = FALSE;
	m_PcBangStartTime = 0;

	m_fMoveValue = 0.0f;

	for(int i = 0; i < 4; i++)
	{
		m_pMp3Module[i] = NULL;
	}

	CPacketControl::EnableSend(TRUE);
	
	m_pLoadingBack = NULL;
	m_pLoadingFrame = NULL;
	m_nLoadingFrameReserve = -1;
	m_nLoadingFrameCurrent = -2;

	m_pLoadingGoWorldBack = NULL;
	m_pLoadingGoWorldBox = NULL;
	m_pLoadingGoWorldFrame = NULL;
	m_pLoadingGoWorldText = NULL;

	m_nLoadingGoWorldFrameReserve = 0;
	m_nLoadingGoWorldFrameCurrent = -1;

	g_MyPetStatus.Init();
}

//---------------------------------------------------------------------------
// Name: OutputText()
// Desc: Draws text on the window.
//---------------------------------------------------------------------------
VOID CMyD3DApplication::OutputText( DWORD x, DWORD y, TCHAR* str )
{
	HDC hDC;
	// Get a DC for the surface. Then, write out the buffer
	if( m_pddsRenderTarget )
	{
		if( SUCCEEDED( m_pddsRenderTarget->GetDC(&hDC) ) )
		{
			m_hOldFont = (HFONT) SelectObject(hDC, m_hFont);
			SetBkMode( hDC, TRANSPARENT );
			SetTextColor( hDC, RGB(0,0,0) );
			ExtTextOut( hDC, x, y+1, 0, NULL, str, lstrlen(str), NULL );
			ExtTextOut( hDC, x+1, y, 0, NULL, str, lstrlen(str), NULL );
			ExtTextOut( hDC, x, y-1, 0, NULL, str, lstrlen(str), NULL );
			ExtTextOut( hDC, x-1, y, 0, NULL, str, lstrlen(str), NULL );
			SetTextColor( hDC, RGB(255,255,0) );
			ExtTextOut( hDC, x, y, 0, NULL, str, lstrlen(str), NULL );
			m_hFont = (HFONT) SelectObject(hDC, m_hOldFont);
			if( m_pddsRenderTarget )m_pddsRenderTarget->ReleaseDC(hDC);
		}
	}

	// Do the same for the left surface (in case of stereoscopic viewing).
	if( m_pddsRenderTargetLeft )
	{
		if( SUCCEEDED( m_pddsRenderTargetLeft->GetDC( &hDC ) ) )
		{
			// Use a different color to help distinguish left eye view
			SetTextColor( hDC, RGB(255,0,255) );
			SetBkMode( hDC, TRANSPARENT );
			ExtTextOut( hDC, x, y, 0, NULL, str, lstrlen(str), NULL );
			m_pddsRenderTargetLeft->ReleaseDC(hDC);
		}
	}
}

float g_cS = 0.5f, g_cE = 1.3f, g_cD = 0.0f;
int g_cR = 22, g_cG = 30, g_cB = 68;
//---------------------------------------------------------------------------
// Name: ShowStats2()
// Desc: ???????? ???????? ?????? ???????? ??????
//---------------------------------------------------------------------------
void CMyD3DApplication::ShowStats2()
{
	static FLOAT fFPS      = 0.0f;
	static FLOAT fLastTime = 0.0f;
	static DWORD dwFrames  = 0L;
	static int nCallCount = 0;
	static DWORD dwDecoTime, dwLandTime,  dwRohTime, dwPlayerTime, dwMobTime, dwItemTime, dwWeatherTime;
	static DWORD dwRenderRohTime;
	static DWORD dwSaveDecoTime, dwSaveLandTime,  dwSaveRohTime, dwSavePlayerTime, dwSaveMobTime, dwSaveItemTime, dwSaveWeatherTime;
	static DWORD dwSaveRenderRohTime;

	if (m_bShowStats == FALSE)
		return;

	if (m_GraphicMode == NKGRAPHIC2D)
		ShowStats();				// CD3DApplication?? ???? ?????? ?? ???????? ?????? ????????.
	else
	{
		//ShowStats();				// CD3DApplication?? ???? ?????? ?? ???????? ?????? ????????.
		// Keep track of the time lapse and frame count
		FLOAT fTime = timeGetTime() * 0.001f; // Get current time in seconds
		++dwFrames;

		dwSaveDecoTime += m_dwDecoTime;
		dwSaveLandTime += m_dwLandTime;
		dwSaveRohTime += m_dwRohTime;
		dwSaveRenderRohTime += m_dwRenderRohTime;
		dwSavePlayerTime += m_dwPlayerTime;
		dwSaveMobTime += m_dwMobTime;
		dwSaveItemTime += m_dwItemTime;
		dwSaveWeatherTime += m_dwWeatherTime;

		// Update the frame rate once per second
		if( fTime - fLastTime > 0.01f )
		{
			if (dwFrames > 0)
			{
				dwDecoTime = dwSaveDecoTime / dwFrames;
				dwLandTime = dwSaveLandTime / dwFrames;
				dwRohTime = dwSaveRohTime / dwFrames;
				dwPlayerTime = dwSavePlayerTime / dwFrames;
				dwMobTime = dwSaveMobTime / dwFrames;
				dwItemTime = dwSaveItemTime / dwFrames;
				dwWeatherTime = dwSaveWeatherTime / dwFrames;
				dwRenderRohTime = dwSaveRenderRohTime / dwFrames;
			}
			dwSaveDecoTime = dwSaveLandTime = dwSaveRohTime = dwSaveRenderRohTime = 0;
			dwSavePlayerTime = dwSaveMobTime = dwSaveItemTime = dwSaveWeatherTime = 0;

			fFPS      = dwFrames / (fTime - fLastTime);
			fLastTime = fTime;
			dwFrames  = 0;
		}

		// Setup the text buffer to write out dimensions
		TCHAR buffer[150];
		int numOfRenderedDeco = g_listRenderedDecoColor.size();
		sprintf(buffer, "Time:%d, ??(%d,%d), Fov:%4.2f, At(%5.2f,%5.2f,%5.2f), Eye(%5.2f,%5.2f,%5.2f)",
				m_nCurHour, (int)(m_vAt.x/gMapExtention), (int) (gMapHeight - m_vAt.z/gMapExtention), m_Fov,
				m_vAt.x, m_vAt.y, m_vAt.z, m_vEye.x, m_vEye.y, m_vEye.z);
		OutputText( 0, 0, buffer );
		sprintf(buffer, "FogStart: %f, FogEnd: %f", g_FogStart, g_FogEnd);
		sprintf(buffer, "FogStart: %f, FogEnd: %f, FogD: %f", g_cS, g_cE, g_cD);
		sprintf(buffer, "FogStart: %f, FogEnd: %f", m_fog_start, m_fog_end);
		OutputText(0, 20, buffer);

		DWORD	dwColor;
		if( m_pd3dDevice )
			m_pd3dDevice->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
		sprintf(buffer, "dwColor: %x, dwColor: %d", dwColor, (int)dwColor);
		OutputText(0, 40, buffer);
		sprintf(buffer, "FPS: %3.2f", fFPS);
		OutputText(0, 60, buffer);
		sprintf(buffer, "char m_dir : %f", g_pRoh->m_dir);
		OutputText(0, 80, buffer);
	}
}

void CMyD3DApplication::ShowStats_Nalrim()
{
	if( !m_bShowStats_Nalrim )
		return;

	if( !m_pUIMgr )
		return;

	TCHAR buffer[150];
	sprintf(buffer, "Entity Size = %d", m_pUIMgr->m_colorkey_map.size() );
	OutputText(0 ,100 , buffer);
	sprintf(buffer, "Character Size = %d", m_pUIMgr->m_character_map.size() );
	OutputText(0 , 120 , buffer);
	sprintf(buffer, "Monster Size = %d", m_pUIMgr->m_mob_map.size() );
	OutputText(0 , 140 , buffer);
	sprintf(buffer, "UserNpc Size = %d", m_pUIMgr->m_usershopnpc_map.size() );
	OutputText(0 , 160 , buffer);
	sprintf(buffer, "Pet Size = %d", m_pUIMgr->m_pet_map.size() );
	OutputText(0 , 180 , buffer);
	sprintf(buffer, "Item Size = %d", m_pUIMgr->m_iteminfo_map.size() );
	OutputText(0 , 200 , buffer);
}

//---------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//---------------------------------------------------------------------------
VOID CMyD3DApplication::Cleanup3DEnvironment()
{
	SetbActive(FALSE);
	SetbReady(FALSE);

	//[2009/4/16 Theodoric] mutex
	ReleaseGameMutex();

	if( GetFramework() )
	{
		DeleteUserObjects();
		DeleteDeviceObjects();
		DeleteFramework();
		FinalCleanup();
	}

	D3DEnum_FreeResources();

	safe_delete(m_param_manager);
}


//[2009/4/16 Theodoric] mutex
void CMyD3DApplication::ReleaseGameMutex()
{
	if( m_hMutex )
	{
		::ReleaseMutex( m_hMutex );
		::CloseHandle( m_hMutex );
		m_hMutex = NULL;
	}
}

LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
									LPARAM lParam )
{
	HMENU hMenu;
	BOOL	bUIProcess;
	static int nUIProcess;
	static BOOL	bMustRestoreVBs = FALSE;
	BOOL	bDblClk = FALSE;

	switch( uMsg )
	{
	case WM_CLOSE: // global font ?? ????????.
		{
			Destroy_Global_Font(); // Lyul
			return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
		}
		break;
	case WM_DESTROY:
		{
			KillTimer(m_hWnd, 1);
			Cleanup3DEnvironment();
			PostQuitMessage(0);

			return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
		}
		break;
/*
	case 0x555:
			if(g_pTcpIp)
						g_pTcpIp->CloseSocket();
			break;*/
	case WM_TIMER:
		{
			if (wParam == 2 && g_bDummyMsg == TRUE && pCMyApp->m_bPlaying)
			{
				if (g_pTcpIp)
				{
					g_pTcpIp->SendNetMessage("\n");
				}
			}
		}
		break;
	}

	if (uMsg == WM_LBUTTONDBLCLK)
	{
		bDblClk = TRUE;
		uMsg = WM_LBUTTONDOWN;
	}

	if( GetbActive() == FALSE )
		return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );

	bUIProcess = 0;

	if( m_MsgPopUp.m_bActive == TRUE && uMsg != WM_RBUTTONUP ) // ?????? ?????? ???? ?????? ?????????? WM_RBUTTONUP ???????? ?????????? ????. ?????? ?????? ???? ???? ???????? ???? ?? ??. 07-03-13 ????
	{
		if(uMsg == WM_MOVE)
			return 0;

		nUIProcess = m_MsgPopUp.MsgProc(hWnd, uMsg, wParam, lParam);
		if (nUIProcess == 1)
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
		else if (nUIProcess == -1)
			m_MsgPopUp.m_bActive = FALSE;
		else if ( nUIProcess == 2)
		{
			// ?????? ????????.
			RestartGame();
		}
		
		if( uMsg != WM_IME_COMPOSITION )
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}		
	}

	// Control bottom ??????
	if (m_GraphicMode == NKGRAPHIC3D  && !m_bBeforeGoWorld)
	{
		if (g_pNk2DFrame)
			bUIProcess = g_pNk2DFrame->MsgProc(hWnd, uMsg, wParam, lParam, bDblClk);
		if (bUIProcess)
		{
			if( uMsg != WM_IME_COMPOSITION )
			{
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}
	}

	{
		if( g_pNk2DFrame &&
				g_pNk2DFrame->GetUIWindow() &&
				uMsg == WM_RBUTTONDOWN &&
				uMsg == WM_RBUTTONUP )
		{
			if(g_pNk2DFrame->GetUIWindow()->IsOpenWindow())
			{
				if(uMsg == WM_MOVE) return 0;

				if( uMsg != WM_IME_COMPOSITION )
				{
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}

	if( g_SvrType == ST_ADULT_ONLY )
	{
		if( g_pNk2DFrame &&
				g_pNk2DFrame->GetTgaUIWindow() &&
				uMsg == WM_RBUTTONDOWN &&
				uMsg == WM_RBUTTONUP )
		{
			if(g_pNk2DFrame->GetTgaUIWindow()->IsOpenWindow())
			{
				if(uMsg == WM_MOVE) return 0;

				if( uMsg != WM_IME_COMPOSITION )
				{
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}

	// ???? ??????.
	if (m_pUIMgr && m_bGameMode == TRUE)
	{
		if ( (!g_pRoh || (g_pRoh && !(g_pRoh->IsDead()))) && !m_bBeforeGoWorld)
		{
			bUIProcess = m_pUIMgr->Message(uMsg, wParam, lParam);

			if (bUIProcess)
			{
				if( uMsg != WM_IME_COMPOSITION )
				{
					return DefWindowProc( hWnd, uMsg, wParam, lParam );
				}
			}
		}
	}

	// ?????? ???? ??????????.
	if (g_pNk2DFrame && m_GraphicMode == NKGRAPHIC2D)
	{
		if ( (!g_pRoh || (g_pRoh && !(g_pRoh->IsDead()))) && !m_bBeforeGoWorld)
		{
			bUIProcess = g_pNk2DFrame->MsgProc(hWnd, uMsg, wParam, lParam, bDblClk);
			if (bUIProcess)
			{
				if( uMsg != WM_IME_COMPOSITION )
				{
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
				}
			}
		}
	}

	hMenu = GetMenu( hWnd );

	switch( uMsg )
	{
	case WM_INITMENUPOPUP:
		if (m_bWireframe == TRUE)
			CheckMenuItem (hMenu, IDM_WIREFRAME, MF_CHECKED);
		else
			CheckMenuItem (hMenu, IDM_WIREFRAME, MF_UNCHECKED);

		if (m_bPickView == TRUE)
			CheckMenuItem (hMenu, IDM_PICKVIEW, MF_CHECKED);
		else
			CheckMenuItem (hMenu, IDM_PICKVIEW, MF_UNCHECKED);

		if (m_bShowStats == TRUE)
			CheckMenuItem (hMenu, IDM_SHOWSTAT, MF_CHECKED);
		else
			CheckMenuItem (hMenu, IDM_SHOWSTAT, MF_UNCHECKED);

		if (m_bFastMove == TRUE)
			CheckMenuItem (hMenu, IDM_FASTMOVE, MF_CHECKED);
		else
			CheckMenuItem (hMenu, IDM_FASTMOVE, MF_UNCHECKED);

		break;

	case WM_ACTIVATEAPP:
		m_bActiveApp = (BOOL) wParam;
		if( m_bGameMode && !m_ProOption.bWindowed )		// Alt+tab in fullscreen
		{
			if( m_bActiveApp )
			{
				if( bMustRestoreVBs )
				{
					RestoreVBs();
					bMustRestoreVBs = FALSE;
				}
			}
			else
				bMustRestoreVBs = TRUE;
		}
		break;	

	case WM_SYSCOMMAND:
		{
			switch( wParam )
			{
			case SC_KEYMENU:
				return 1;
			}
		}
		break;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDM_VIEW_SHADOW:
			if (m_bShadowView)
			{
				m_bShadowView = FALSE;
				CheckMenuItem (hMenu, IDM_VIEW_SHADOW, MF_UNCHECKED);
			}
			else
			{
				m_bShadowView = TRUE;
				CheckMenuItem (hMenu, IDM_VIEW_SHADOW, MF_CHECKED);
			}
			break;

		case IDM_PICKVIEW:
			if (m_bPickView)
			{
				m_bPickView = FALSE;
				CheckMenuItem (hMenu, IDM_PICKVIEW, MF_UNCHECKED);
				if( GetFramework() )
					GetFramework()->SetPickView(m_bPickView);
			}
			else
			{
				m_bPickView = TRUE;
				CheckMenuItem (hMenu, IDM_PICKVIEW, MF_CHECKED);
				if( GetFramework() )
					GetFramework()->SetPickView(m_bPickView);
			}
			break;

		case MENU_LIGHT_SOURCES:
			break;

		case IDM_WIREFRAME:
			ToggleWireframe();
			if (g_pNk2DFrame && g_pNk2DFrame->GetSheet() && g_pNk2DFrame->GetSheet()->m_pSMRender)
				g_pNk2DFrame->GetSheet()->m_pSMRender->SetRenderWireframe(m_bWireframe);

			break;

		case ID_FILE_SAVE_HEIGHT:
			//m_pLand->OnSaveHeightMap();
			break;

		case ID_FILE_SAVE_MAP:
			break;

		case IDM_COMM_CONNECT:
			// NetLog ?? Admin ?? ???????? ???? ?? ???? ???????? ?? ?? ????.
			// ?????? ?????? ???? ???????? ???????? ????
			// ???? UIMgr ?? ???? ?????? ????.
			//
//			if (g_bAdmin2) // 07-03-09 ?????? ???? ?????? ???????????? ???? ????????. ???????????? ???????? ???????? ??????.
			G_DevCreateDlg();
			break;

		case ID_COMM_WINDEBUG:			
			break;

		case ID_TEST_ADDPLAYER:
			if (m_pUIMgr)
				m_pUIMgr->AddPlayerTest();
			break;

		case ID_TEST_DELPLAYER:
			if (m_pUIMgr)
				m_pUIMgr->DeletePlayerTest();
			break;

		case ID_TEST_ATTACK:
			if (m_pUIMgr)
				m_pUIMgr->AttackTest();
			break;

		case IDM_MAP_OPEN:
			//OpenNewMap();
			break;

		case IDM_SHOWSTAT:
			m_bShowStats = m_bShowStats? FALSE:TRUE;
			CheckMenuItem (hMenu, IDM_SHOWSTAT, m_bShowStats? MF_CHECKED : MF_UNCHECKED);
			break;

		case IDM_FASTMOVE:			
			break;

		case IDM_EXIT:
			if(m_MsgPopUp.m_bActive == TRUE)
				m_MsgPopUp.m_bActive = FALSE;
			else
				SmartExit(hWnd, uMsg, wParam, lParam);

			return 1;

		default:
			return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
		}
		break;

	case WM_GETMINMAXINFO:

		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;

	case WM_KEYDOWN:

		switch((int)wParam)
		{
		case VK_ESCAPE:
			if(m_MsgPopUp.m_bActive == TRUE)
				m_MsgPopUp.m_bActive = FALSE;
			else
				SmartExit(hWnd, uMsg, wParam, lParam);

			break;
		}
		break;

	case WM_CHAR:

		switch((int)wParam)
		{
		case ']':
			if (g_bAdmin && g_bWorldLoading)
			{
				m_nCurHour++;

				if(m_nCurHour >29) m_nCurHour = 0;
				pCMyApp->ChangeHour(m_nCurHour);
			}
			g_cE += 0.01f;
			g_cR++;
			break;

		case '[':
			if (g_bAdmin && g_bWorldLoading)
			{
				m_nCurHour--;

				if(m_nCurHour < 0) m_nCurHour = 29;
				pCMyApp->ChangeHour(m_nCurHour);
			}
			g_cE -= 0.01f;
			g_cR--;
			break;

		case '-':
			g_FogEnd += 0.01f;
			m_fog_start +=0.01f;
			g_cS += 0.01f;
			g_cG++;
			break;

		case '=':
			g_FogEnd -= 0.01f;
			m_fog_start -=0.01f;
			g_cS -= 0.01f;
			g_cG--;
			break;

		case '9':
			g_FogStart += 0.01f;
			m_fog_end +=0.01f;
			g_cD += 0.01f;
			g_cB++;
			break;

		case '0':
			g_FogStart -= 0.01f;
			m_fog_end -=0.01f;
			g_cD -= 0.01f;
			g_cB--;
			break;
		}
		break;

	case WM_IME_COMPOSITION:
		return 0;		

	default:
		return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

// ?????? ???????? ???? ???????? ?????? g_fTimeKey?? Init??????.
void CMyD3DApplication::InitTimeCtrl()
{
	// ?????? ?????? ??????
	SetBaseTime(timeGetTime());
	g_dwBaseTime = GetBaseTime();
	g_fTimeKey = ( timeGetTime() - g_dwBaseTime ) * 0.001f;

	// ???????? ???? ??????.
	g_fAccTimeKey = 0.0f;
	g_dwSpeedHackBaseTime = timeGetTime();
	g_nBaseClock = clock();
	g_fAccClock = 0.0f;
}

#define ABS(a)	((a) >= 0 ? (a) : (-(a)))

//---------------------------------------------------------------------------
// Name: ProcessNextFrame()
// Desc:
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::ProcessNextFrame(HWND hwnd)
{
	static int speedCount = 0;
	static clock_t baseSpeedCountTime = 0;
	HRESULT hr;
	static DWORD dwSoundLastTime = 0;

	// Check the cooperative level before rendering
	if( m_bActive && (!m_pDD || FAILED( hr = m_pDD->TestCooperativeLevel() )) )
	{
		switch( hr )
		{
		//case DDERR_NOEXCLUSIVEMODE:
		case DDERR_EXCLUSIVEMODEALREADYSET:
			// Do nothing because some other app has exclusive mode
			return S_OK;

		case DDERR_WRONGMODE:
			// The display mode changed on us. Resize accordingly
			if( m_pDeviceInfo && m_pDeviceInfo->bWindowed )
				return Change3DEnvironment();
			break;

		case DDERR_SURFACELOST:
		case DDERR_NOEXCLUSIVEMODE:
			Sleep(20);
			RestoreSurfaces();
			break;

		default:
			return hr;
		}
	}

	// Show the frame on the primary surface.
	bool bCheckRestore = CheckRestore();
	if( m_bActive && (false == bCheckRestore ||
			FAILED( hr = GetFramework()->ShowFrame() )) )
	{
		if( true == bCheckRestore && DDERR_SURFACELOST != hr )
			return hr;

		Sleep(20);
		RestoreSurfaces();

		return S_OK;
	}

	char chkValid[MAX_PATH] = {0,};
	DWORD dwPass = 0;
	hr = GET_D3DDEVICE()->ValidateDevice(&dwPass);
	if( m_bActive && hr != S_OK )
	{
		sprintf(chkValid, "DEVICE VALID ERROR : %X\n", hr);
		OutputDebugString(chkValid);
		Sleep(20);

		return S_OK;
	}

	// ???? ???? ????.
	g_fTimeKey = ( timeGetTime() - GetBaseTime() ) * 0.001f;
	SetBaseTime(timeGetTime());				// ???????? ???????? ????.. ???? ????.
	g_dwBaseTime = GetBaseTime();

	// ?????? ?? ???? ????.
	g_fAccTimeKey += (timeGetTime() - g_dwSpeedHackBaseTime) * 0.001f;
	g_dwSpeedHackBaseTime = timeGetTime();
	int nClock = clock();
	g_fAccClock = g_fAccClock + (float) (nClock - g_nBaseClock) / ((float) CLOCKS_PER_SEC);
	g_nBaseClock = nClock;

	if ( !m_bGameMode )
	{
		g_fAccTimeKey = 0.0f;
		g_fAccClock = 0.0f;
	}

	if (((nClock - baseSpeedCountTime) / CLOCKS_PER_SEC) >= 60.0f)
	{
		speedCount = 0;
		baseSpeedCountTime = nClock;
	}


	if (g_fAccClock >= 10.0f)
	{
		g_fAccTimeKey = 0.0f;
		g_fAccClock = 0.0f;

		if( FindWindow((LPCSTR)(DWORD)WC_DIALOG, "Speed Hack - PCGameHacks.com")
				|| FindWindow((LPCSTR)(DWORD)WC_DIALOG, "Brothers Speeder")
				|| FindWindow((LPCSTR)(DWORD)WC_DIALOG, "Speeder - Registered")
				|| FindWindow((LPCSTR)(DWORD)WC_DIALOG, "Speeder - Unregistered")
				|| FindWindow((LPCSTR)(DWORD)WC_DIALOG, "???????? Ver 1.0 ")
				|| FindWindow((LPCSTR)(DWORD)WC_DIALOG, "?????????? ?????????? 2.0 ??????")
				|| FindWindow((LPCSTR)(DWORD)WC_DIALOG, "                            ")
		  )
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_MAIN_SPEEDHACK), TYPE_NOR_OK, 4 );
	}

	// 3D Rendering First and then 2D display on it.
	if (m_GraphicMode == NKGRAPHIC3D)
	{
		DWORD dwWait;
		if (m_bWaitingAt)
		{
			dwWait = timeGetTime();

			if ( (dwWait - m_dwGoWorldBaseTime) > 20000)
			{
				MessageBox( NULL, "Server don`t response", "Caution", MB_OK ); // ???????? ?????? ???? 20?????? at ?????? ?????? ?????????? ???? ????????????. ?????? ???? ???? ???????? ???????? ????.
				SendMessage(m_hWnd, WM_CLOSE, 0, 0);
				return S_OK;
			}
		}
		if (m_bBeforeTrans)
		{
			dwWait = timeGetTime();

			if ( (dwWait - m_dwTransBaseTime) > 5000)
				m_bBeforeTrans = FALSE;
		}

		if (g_bBeforeGoSky)
		{
			dwWait = timeGetTime();

			if ( (dwWait - g_dwBeforeGoWorldTime) > 3000)
			{
				m_bBeforeGoWorld = FALSE;
				g_bBeforeGoSky = FALSE;
			}
		}

		if (m_bPlaying)
		{
			Render3DEnvironment();
			Render2DEnvironment(hwnd);
		}
		else
		{
			Render3DEnvironment();
			// ?????? ??????, ???????? ?????????? ???? ???????? ???????? ??????
			// ?????? ??????.
			DDBLTFX ddbltfx;
			ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
			ddbltfx.dwSize      = sizeof(ddbltfx);
			ddbltfx.dwFillColor = 0;
			m_pddsRenderTarget->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

			// ???? ?? ???? ???? ???? ???? 07-03-02 ????
			if ( g_pNk2DFrame && g_pNk2DFrame->m_Popup )
				g_pNk2DFrame->m_Popup->pMsg_Popup.Draw();
		}
	}
	else if( m_GraphicMode == NKGRAPHICGOWORLD )
	{
		Sleep(1);
		CheckLoadingGoWorldImg();
		DrawLoadingGoWorld();
	}
	else if( m_GraphicMode == NKGRAPHICLOADING )
	{
		Sleep(1);
		CheckLoadngImg();
		DrawLoading();
	}	
	else
	{
		Render2DEnvironment(hwnd);
	}

	// Show the frame rate, etc.
	if( m_bShowStats && g_bAdmin2)
		ShowStats2();


	ShowStats_Nalrim();

	//////////////////
	m_MsgPopUp.Draw();  // ???????????? ????????
	//////////////////

	if (m_bShadowView)
		m_pGoodShadow->Render2D();	

	if (m_pMouse)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
		m_pMouse->Draw(pt);
	}

	return S_OK;
}

HRESULT CMyD3DApplication::Render2DEnvironment(HWND hwnd)
{
	HRESULT hr;
	// Display the sprites on the screen
	if( FAILED( hr = DisplayFrame() ) )
	{
		if( hr != DDERR_SURFACELOST )
			return hr;
	}

	return S_OK;
}



//---------------------------------------------------------------------------
// Name: RestoreSurfaces()
// Desc : ????+?? ???? ?????? 2D Surface???? lost???? ?????? ???????? ????

//    ???????? ?? ?????????? ?????? ???????? ??????.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreSurfaces()
{
	GetFramework()->RestoreSurfaces();

	char buf[MAX_PATH] = {0,};

	HRESULT hr = S_OK;

	hr = GetRenderTarget()->Restore();
	sprintf(buf, "[Restore] RenderTarget Restore : %X\n", hr);
	OutputDebugString(buf);
	hr = GetFramework()->GetZBuffer()->Restore();
	sprintf(buf, "[Restore] Z-Buffer Restore : %X\n", hr);
	OutputDebugString(buf);
	hr = GetFramework()->GetZBufferPick()->Restore();
	sprintf(buf, "[Restore] Z-Buffer_Pick Restore : %X\n", hr);
	OutputDebugString(buf);
	hr = GetFramework()->GetBackBuffer()->Restore();
	sprintf(buf, "[Restore] BackBuffer Restore : %X\n", hr);
	OutputDebugString(buf);
	hr = GetFramework()->GetBackBufferPick()->Restore();
	sprintf(buf, "[Restore] BackBuffer_Pick Restore : %X\n", hr);
	OutputDebugString(buf);
	hr = GetFramework()->GetFrontBuffer()->Restore();
	sprintf(buf, "[Restore] FrontBuffer Restore : %X\n", hr);
	OutputDebugString(buf);

	RestoreVBs();

	if( m_pSkyPlane )
		m_pSkyPlane->SetDevice();
	if( g_pMobTexture )
		g_pMobTexture->RestoreAllTextures(m_pd3dDevice);
	if( m_pDecoTexture )
		m_pDecoTexture->RestoreAllTextures(m_pd3dDevice);
	if( g_pCapsyongTexture )
		g_pCapsyongTexture->RestoreAllTextures(m_pd3dDevice);

	if( g_pNk2DFrame )
		g_pNk2DFrame->RestoreSurfaces();

	if( m_pGoodShadow )
		m_pGoodShadow->Restore();

	return S_OK;
}

bool CMyD3DApplication::CheckRestore()
{
	if( GetRenderTarget() && GetRenderTarget()->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	if( GetFramework()->GetZBuffer() && GetFramework()->GetZBuffer()->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	if( GetFramework()->GetZBufferPick() && GetFramework()->GetZBufferPick()->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	if( GetFramework()->GetBackBufferPick() && GetFramework()->GetBackBufferPick()->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	if( GetFramework()->GetBackBuffer() && GetFramework()->GetBackBuffer()->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	if( g_pNk2DFrame && g_pNk2DFrame->GetItemRender() &&
			g_pNk2DFrame->GetItemRender()->CheckRestore() == false )
	{
		return false;
	}

	if( m_pGoodShadow && m_pGoodShadow->CheckRestore() == false )
	{
		return false;
	}

	return true;
}

HRESULT CMyD3DApplication::Render3DEnvironment()
{
	HRESULT hr;

	if (m_pUIMgr)
		m_pUIMgr->IsVk1Pressed();

	if( GetbFrameMoving() || GetSingleStep() )
	{
		if( FAILED( hr = FrameMove( 0 ) ) )
			return hr;

		SetSingleStep(FALSE);
	}

	// ?????? ????????, InitDeviceObject???? ???????? ?????? ??????.
	if (!m_bSatelliteMode)
	{
		if( m_pd3dDevice )
			m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matView );
		if( m_pd3dDevicePick )
			m_pd3dDevicePick->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matView );
	}

	if (m_bPlaying)
	{
		if( FAILED( hr = Render() ) )
			return hr;
	}

	// prevent automouse
	if (m_pUIMgr)
		m_pUIMgr->IsVk1Pressed();

	return S_OK;
}

//---------------------------------------------------------------------------
// Name: DisplayFrame()
// Desc:
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::DisplayFrame()
{
	if( g_pNk2DFrame )
		g_pNk2DFrame->UpdateFrame();
	return S_OK;
}

//---------------------------------------------------------------------------
// Name: InitVariables()
// Desc: ?????? ???????? ?????? ???????? ??????
//---------------------------------------------------------------------------
void CMyD3DApplication::InitVariables()
{
	// Set the transform matrices
	D3DMATRIX matWorld;
	D3DUtil_SetIdentityMatrix( matWorld );
	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD,      &matWorld );

	m_Fov = 0.41f;
	m_Aspect = 0.75;
	m_zNear = 5.0f;
	m_zFar = 10000.0f;
	UpdateProjectMatrix();
	m_pFrustum = new CFrustum;
	if( m_pFrustum )
	{
		if( g_bNewTerrain )
			m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 2000.0f );
		else
			m_pFrustum->InitFrustum(m_Fov, m_Aspect, m_zNear, m_zFar);
		m_pFrustum->InitObjFrustum( CNKObjQuadNode::GetRenderDist() );
	}

	// ?????? ???? ???? ????
	m_vEye = D3DVECTOR_New(0, 100.0f, -100.0f);
	m_vAt = D3DVECTOR_New(0, 0, 0);
	m_vUp = D3DVECTOR_New(0, 1, 0);
	SetViewParams( &m_vEye, &m_vAt, &m_vUp, 0.2f );
	// ?????? ?????? ???????????? ??????
	m_vEyeZ.x = m_matView._13;
	m_vEyeZ.y = m_matView._23;
	m_vEyeZ.z = m_matView._33;
	m_vEyeY.x = m_matView._12;
	m_vEyeY.y = m_matView._22;
	m_vEyeY.z = m_matView._32;
}

//---------------------------------------------------------------------------
// Name: StartGame()
// Desc: ???? ????
//---------------------------------------------------------------------------
char g_commOut[512];
void CMyD3DApplication::StartGame()
{
	if( !g_pNk2DFrame )
		return;

	m_GraphicMode = NKGRAPHIC3D;
	g_pNk2DFrame->SetGraphicMode(m_GraphicMode);
	
	m_pSlotChar = new CSlotChar;
	if( g_pNk2DFrame->GetSheet() )
	{
		g_pNk2DFrame->GetSheet()->m_Slot[g_pNk2DFrame->GetSheet()->m_SlotNum]->Clone(m_pSlotChar);
		// ???? ?????? ?????? ?????? ????.
		m_SlotNum = g_pNk2DFrame->GetSheet()->m_SlotNum;
		strcpy(m_strUserId, g_pNk2DFrame->GetSheet()->m_sUserId);
		strcpy(m_strUserPw, g_pNk2DFrame->GetSheet()->m_sUserPW);
	}

	g_pNk2DFrame->DeleteStartSheet();

	LoadResources( WORLD_START, false );
	//LoadResources( WORLD_MATRIX, false );  //ENABLE FOR MATRIX WAR ONLY  PABLO 02/10/2019

	SAFE_DELETE(m_pSlotChar);	
}

//---------------------------------------------------------------------------
BOOL CMyD3DApplication::LoadResources(int which, bool force3d)
{
	if( !g_pNk2DFrame )
		return false;

	char strMapPath[255] = {0,};
	char strDecoPath[255] = {0,};

	if( g_pCapsyongTexture == NULL )
	{
		g_pCapsyongTexture = new IndexedTexture(MAX_CAPSYONG_TEXTURE); // ???????? ???? 07-06-30 ????

		G_LoadEffectImage();
		G_LoadFXEffectImage();
		G_LoadEffectTestTexImage();
	}
		
	if( m_GraphicMode == NKGRAPHIC3D || force3d == true )
	{
		LoadGameResources();
		if (!g_bWorldLoading)
			MidLoading(2);

		if( m_param_manager )
		{
			if( !LoadWorldMap(which) )
				return false;

			m_nCurWorldIdx = which;
		}
		else
		{
			return false;
		}

		if (!g_bWorldLoading)
			MidLoading(4);
		if (g_pTcpIp == NULL)
		{
			// ?????? ???????? ???? ???? ????????.
			SetHeroInitPos(35, 0, 40, 1.5707f);

			if( g_bAdmin ) // ?????? ???? ?????? ????????...
				MessageBox(NULL, "Wrong Position Error!", "Debug Info", MB_OK); // ???????? ???? ???????? ?????? ???????? ?????? ???????? ?????????? ?? ???????? ?????? ???? ?????? ?????? ?????? ????. (by ????)

			SetHeroInitPos(7066, 0, 5257, 1.5707f); // ???? ?????????? ?????? ?????? ??????????.
		}

		if( m_pUIMgr )
		{
			m_pUIMgr->Init(Get_hWnd(), m_pLand);
			m_pUIMgr->SetFrustum(m_pFrustum);
			m_pUIMgr->InitChatPopups();
			m_pUIMgr->InitEventScene(); // ?????????? ???? ??????. ///????1 ?? ????
		}

		if (!g_bWorldLoading)
			MidLoading(5);

		if( m_pDecoTexture )
			m_pDecoTexture->RestoreAllTextures(m_pd3dDevice);

		if (!g_bWorldLoading)
			MidLoading(6);

		if (g_pCapsyongTexture)
			g_pCapsyongTexture->RestoreAllTextures(m_pd3dDevice);

		if (!g_bWorldLoading)
			MidLoading(7);

		Sleep(100);

		m_bGameMode = TRUE;

		if( m_ProOption.bMusicOn && m_pMp3Module[0] )
		{
			if( m_pMp3Module[0] && m_pMp3Module[0]->IsPlaying() )
				m_pMp3Module[0]->Stop();
		}

		if(
			m_pMusicManager
			&& m_ProOption.bUseMusicForEachZone )
		{
			if(m_ProOption.bMusicOn)
				PlayMusic( 4 );
			else
				StopMusic();
		}

		if (g_pRoh->m_Level <= 40 && g_MatrixSvr == 0)
			g_pNk2DFrame->ToggleHelpWindow();

		if (g_pRoh->m_GuildIndex)
			if( m_pUIMgr )
				m_pUIMgr->isFirstViewGuildMsg = TRUE;
			else
			{
				if (g_dwLangType == 0 && g_dwClientCountry == 0)
				{
					if (g_pRoh->m_Level <= 5 &&  g_MatrixSvr == 0) //1204
						if( m_pUIMgr )
							m_pUIMgr->HelpMessage(0);
				}
			}
		g_pRoh->ToggleMovingState();

		if (g_MatrixSvr == 1 && g_dwClientCountry == 0 && !g_bMatrixMatch)
			if( m_pUIMgr )
				m_pUIMgr->MatrixMessage(0);

		AdjustViewport();
	}
	else if (m_GraphicMode == NKGRAPHIC2D)
	{
		InitNkChaSet();
		g_pNk2DFrame->SetChaSet(m_dpChaCapsyong, g_pCapsyongTexture);

		if (g_bRestart == FALSE)
			InitProgress(5);	// main.loadresources

		LoadItems();

		if (g_bRestart == FALSE)
			InitProgress(6);	// main.loadresources

		if (g_pNk2DFrame->GetSheet())
		{
			g_pNk2DFrame->GetSheet()->m_pOptionPg->ReadConfigFile("config.ini");

			g_pNk2DFrame->GetSheet()->SetOpeningChar( 0, 0, 0);	// ????
			g_pNk2DFrame->GetSheet()->SetOpeningChar( 1, 1, 1);	// ????
			g_pNk2DFrame->GetSheet()->SetOpeningChar( 2, 2, 0);	// ????
			g_pNk2DFrame->GetSheet()->SetOpeningChar( 3, 3, 1);	// ????

			g_pNk2DFrame->GetSheet()->SetOpeningChar( 4, 4, 0);	// ?????? ??
			g_pNk2DFrame->GetSheet()->SetOpeningChar( 5, 4, 1);	// ?????? ??
			g_pNk2DFrame->GetSheet()->SetOpeningCharWear();		// ?????????? ??????

			//================================
		}

		g_pCapsyongTexture->SetTexturePath("data/effect/textr/");
		g_pShadowRect = new CShadowRect;
		m_pFxSet = new CFxSet;

		if( m_pFxSet )
			m_pFxSet->LoadRes(g_pCapsyongTexture, g_pShadowRect);

		if (g_bRestart == FALSE)
			InitProgress(7);

		if (g_pCapsyongTexture)
			g_pCapsyongTexture->RestoreAllTextures(m_pd3dDevice);

		if (g_bRestart == FALSE)
			InitProgress(8);	// main.loadresources
		g_bRestart = TRUE;
		g_pDSound = NULL;

		{
			g_pDSound = new CDirectSound(Get_hWnd());
			if(g_pDSound && g_pDSound->InitDirectSound() == FALSE)
			{
				delete g_pDSound;
				g_pDSound = NULL;
			}
		}

		if (g_pDSound)
		{
			g_pDSound->SetWavSize(MAX_NUM_WAV);

			g_pDSound->GetVolFromConfigFile(m_ProOption.bMusicEffOn?true:false, m_ProOption.nMusicEffLevel );
			g_pDSound->SetDistanceFactor(1);
			g_pDSound->_SetListenerPos(0.0f, 0.0f, 0.0f);
		}

		if( g_pDSound && g_pDSound->GetDSound() && (m_ProOption.bUseMusicForEachZone||g_dwClientCountry==CTRY_JPN) )  ///????BGM ???????? ?????? ??????. ?????? bUseMusicForEachZone ?????? ?? ?? ????!
		{
			{
				HRESULT musicHr = 0;

				m_pMusicManager = new CMusicManager();
				if( m_pMusicManager && SUCCEEDED( m_pMusicManager->Initialize( Get_hWnd() ) ) )
				{
					musicHr = m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[0], "data//music//back.mid", TRUE, TRUE );
					musicHr |= m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[1], "data//music//d1_back.mid", TRUE, TRUE );
					musicHr |= m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[2], "data//music//d2_back.mid", TRUE, TRUE );
					musicHr |= m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[3], "data//music//3_back.mid", TRUE, TRUE );
					musicHr |= m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[4], "data//music//4_back.mid", TRUE, TRUE );
					musicHr |= m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[5], "data//music//5_back.mid", TRUE, TRUE );
					musicHr |= m_pMusicManager->LoadSegmentFromFile( m_nBackMidi[6], "data//music//6_back.mid", TRUE, TRUE );

					m_pMusicManager->SetVolumeLevelAll(m_ProOption.nMusicLevel);

					if( musicHr != S_OK )
					{
						system("pause");
					}
				}
				if( m_pMusicManager && !m_ProOption.bMusicOn )
				{
					StopMusic();
				}
			}
		}
		for( int index = 0; index < 4; index++ )
			m_pMp3Module[index] = NULL;

		if( m_ProOption.bMusicOn )
		{
			if( g_dwClientCountry == CTRY_JPN && m_pMusicManager )
			{
				int LoginMidi = 0;
				m_pMusicManager->LoadSegmentFromFile( LoginMidi, "data//music//start.mid", TRUE, TRUE );
				m_pMusicManager->Play( LoginMidi, true );

				Sleep(2500);
			}
			m_pMp3Module[0] = new CMp3Module();
			if( m_pMp3Module[0] )
				m_pMp3Module[0]->LoadSound("login.mp3");

			int volume = -2000 + ( 2000 * (0.007 * m_ProOption.nMusicLevel) );
			if( volume <= -2000 )
				volume = -10000;
			else if( volume >= 0 )
				volume = 0;
			if( m_pMp3Module[0] )
			{
				m_pMp3Module[0]->m_pBA->put_Volume(volume);
				if( g_dwClientCountry!=CTRY_JPN ) ///????BGM ???????? ?????? ??????.
					m_pMp3Module[0]->Play();
			}
		}

		if (g_pNk2DFrame->GetSheet())
			g_pNk2DFrame->GetSheet()->LoadSheetWav();

	}



	return TRUE;
}


//---------------------------------------------------------------------------
// Name : RestartGame()
// Desc : ?????? ?????? ???? ????.
//---------------------------------------------------------------------------
void CMyD3DApplication::RestartGame()
{
	HRESULT hr;
	static BOOL  bOldWindowedState = TRUE;
	static DWORD dwSavedStyle;
	static RECT  rcSaved;
	// ???????? ???? ???????? ?? ??????.
	Cleanup3DEnvironment();

	m_param_manager = new LHParam_Manager;
	m_param_manager->Load();

	// ???????? ?? ????????.
	m_bPlaying = FALSE;
	m_GraphicMode = NKGRAPHIC2D;
	g_bNewTerrain = FALSE;	

	DataFriend::GetInstance()->Clear();
	ChatUtilMgr::GetInstance()->Clear();
	FishingManager::GetInstance()->Clear();
	FishingRewardManager::GetInstance()->Clear();
	DecardiBattleManager::GetInstance()->Clear();
	BuffItemMgr::GetInstance()->Clear();
	LottoManager::GetInstance()->Clear();
	SetItemEffMgr::GetInstance()->Clear();
	BingoManager::GetInstance()->Clear();
	LetterManager::GetInstance()->Clear();
	BaseballManager::GetInstance()->Clear();
	BaseballRewardManager::GetInstance()->Clear();
	LotteryManager::GetInstance()->Clear();
	DiceGameManager::GetInstance()->Clear();
	DiceGameRewardManager::GetInstance()->Clear();

	Init();
	EnumerateAndSelect();

	if( m_pDeviceInfo )
	{
		for (int i=0; i < m_pDeviceInfo->dwNumModes; i++)
		{
			if( ( m_pDeviceInfo->pddsdModes[i].dwWidth == 800 ) &&
					( m_pDeviceInfo->pddsdModes[i].dwHeight == 600 ) &&
					( m_pDeviceInfo->pddsdModes[i].ddpfPixelFormat.dwRGBBitCount == m_ProOption.nBackBufferBit ) )
			{
				m_pDeviceInfo->ddsdFullscreenMode = m_pDeviceInfo->pddsdModes[i];
				m_pDeviceInfo->dwCurrentMode      = i;
			}
		}
	}

	RECT rect;
	DWORD dwStyle;

	if (m_bWindowed && g_bFFullMode)
		m_pDeviceInfo->bWindowed = TRUE;
	else if (m_bWindowed && g_bFFullMode == FALSE)
	{
		if( m_pDeviceInfo )
			m_pDeviceInfo->bWindowed = TRUE;
		dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		rect.left = 0;
		rect.top = 0;
		rect.right = 800;
		rect.bottom = 600;
		if (g_bAdmin)
			AdjustWindowRect(&rect, dwStyle, TRUE);
		else
			AdjustWindowRect(&rect, dwStyle, FALSE);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW | SWP_NOSENDCHANGING);
	}
	else
	{
		if( m_pDeviceInfo )
			m_pDeviceInfo->bWindowed = FALSE;

		if( m_pDeviceInfo && m_pDeviceInfo->bWindowed )
		{
			// Coming from fullscreen mode, so restore window properties
			SetWindowLong( m_hWnd, GWL_STYLE, dwSavedStyle );
			SetWindowPos( m_hWnd, HWND_NOTOPMOST, rcSaved.left, rcSaved.top,
						  ( rcSaved.right - rcSaved.left ),
						  ( rcSaved.bottom - rcSaved.top ), SWP_SHOWWINDOW );
		}
		else
		{
			// ?????? ??????.
			SetMenu(m_hWnd, NULL);
			// Going to fullscreen mode, save/set window properties as needed
			dwSavedStyle = GetWindowLong( m_hWnd, GWL_STYLE );
			GetWindowRect( m_hWnd, &rcSaved );
			//SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
			SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_VISIBLE );
		}
	}

	CD3DFramework7* pFramework = NULL;

	if( NULL == ( pFramework = new CD3DFramework7() ) )
		return;

	SetFramework(pFramework);

	if (m_bWindowed && g_bFFullMode == FALSE)
	{}
	else if (g_bFFullMode)
	{
		SetWindowPos( m_hWnd, HWND_TOP, 0, 0,
					  800,
					  600, SWP_SHOWWINDOW );
	}

	CsyManager::GetInstance()->Clear();
	LoloManager::GetInstance()->Clear();

	// Inform the framework class of the driver change. It will internally
	// re-create valid surfaces, a d3ddevice, etc.
	if( FAILED( hr = Initialize3DEnvironment() ) )
	{
		MessageBox(0, "Init 3d fail", 0, 0);
		SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
		return ;
	}

	if( g_pDisplay )
	{
		g_pDisplay->SetDirectDraw( GetFramework()->GetDirectDraw() );
		g_pDisplay->SetBackBuffer( GetFramework()->GetRenderSurface() );
	}

	InitVariables();
	LoadResources();
	
	SetbActive(TRUE);
	SetbReady(TRUE);

	::SetWindowText(pCMyApp->Get_hWnd(), "LaghaimOnline New");
}


//---------------------------------------------------------------------------
// Name: InitBasicCharacter()
// Desc: ?????? ???? ???? ????.
//
// Race.dta
// ??????, ??????, ????, ????, ????
// tribe : ????,????,????,????,??????,????
// sex : ??,??
// type : ????,????,??,????,??,????
//---------------------------------------------------------------------------
BOOL CMyD3DApplication::InitNkChaSet()
{
	if( g_pCapsyongTexture )
		g_pCapsyongTexture->SetTexturePath(LHPATH_CHARACTER_TEXTR);

	LHExcelReader er;
	if( false == er.Open(LHPATH_RACE_TXT) )
		return false;

	char path[MAX_PATH] = {0, };
	for( int row=0 ; row<er.GetRowSize() ; ++row )
	{
		int col = 0;

		er.GetInt(row, col++);
		char * filename = (char*)er.GetData(row, col++);
		int tribe		= er.GetInt(row, col++);
		int sex			= er.GetInt(row, col++);
		int type		= er.GetInt(row, col++);
		int extra		= er.GetInt(row, col++);

		int index = (tribe&0xFF)<<24 | (sex&0xFF)<<16 | (type&0xFF)<<8 | (extra);

		CCapsyong * capsyong = new CCapsyong(g_pCapsyongTexture);
		sprintf( path , "%s%s" , LHPATH_CHARACTER_CSY , filename );
		if( VF_ERR_NONE > capsyong->Read(path) )
		{
			SAFE_DELETE(capsyong);

		}
		m_race_csy_map.insert(CSYMAP_vt(index,capsyong));
	}

	m_nCsyNum = m_race_csy_map.size();

	return TRUE;
}



//---------------------------------------------------------------------------
// Name: StartGameScr()
// Desc: ???? ???? ???? ?? ???? ????????.
//---------------------------------------------------------------------------
void CMyD3DApplication::StartGameScr(int nBmpNum)
{}

//????
#define PROGRESS_WIDTH		213
#define PROGRESS_HEIGHT		55
#define PROGRESS_X			214
#define PROGRESS_Y			425
#define HELPTEXT_X			382
#define HELPTEXT_Y			32
#define BASE_WIDTH			640
#define BASE_HEIGHT			480

void CMyD3DApplication::MidLoading(int n)
{	
	CSurface *pBack=NULL;
	CSurface *pFrame = NULL;
	CSurface *pFrame2 = NULL;// ???????? ????????..

	HRESULT  hr;
	char fname[80];
	int screen_mode = m_ProOption.resolution;
	int screen_cx, screen_cy;
	int left, top;
	float perc_x, perc_y;
	perc_x = perc_y = 0;

	if(screen_mode == R640)
	{
		screen_cx = 640;
		screen_cy = 480;
		left = 0;
		top = 0;
	}
	else if(screen_mode == R800)
	{
		screen_cx = 800;
		screen_cy = 600;
		left = 80;
		top = 60;
	}
	else if(screen_mode == R1024)
	{
		screen_cx = 1024;
		screen_cy = 768;
		left = 192;
		top = 144;
	}
	else if( screen_mode == R1280 )
	{
		screen_cx = 1280;
		screen_cy = 960;
		left = 320;
		top = 240;
	}
	else if( screen_mode == R1280W )
	{
		screen_cx = 1280;
		screen_cy = 720;
		left = 320;
		top = 120;
	}
	else if( screen_mode == R1600W )
	{
		screen_cx = 1600;
		screen_cy = 900;
		left = 480;
		top = 210;
	}
	else if( screen_mode == R1920W )
	{
		screen_cx = 1920;
		screen_cy = 1080;
		left = 640;
		top = 300;
	}


	char random_str[64] = {0, };

	sprintf(random_str, "interface/loading/loading.bmp" );

	if( g_pDisplay )
		g_pDisplay->CreateSurfaceFromBitmap(&pBack, random_str);

	if(n < -1 || n > 7)
		return;

	if (g_pDisplay && n >= 0)
	{
		{
			sprintf(fname, "interface/loading/loading_bar%d.bmp", n);
			g_pDisplay->CreateSurfaceFromBitmap(&pFrame, fname);
			if( pFrame )
			{
				pFrame->SetColorKey(RGB(0, 0, 0));
				pFrame->Xpos = left;
				// 				if( g_dwClientCountry==CTRY_JPN ) ///070917
				// 					pFrame->Ypos = top + 287; // 2007
				// 				else if( g_dwClientCountry==CTRY_TH ) // [5/8/2008 ??????] ???? ?????? ?????? ???? ????
				// 					pFrame->Ypos = top + 399;
				// 				else
				pFrame->Ypos = top + 419; // 2007
			}
		}
	}

	if( g_pDisplay )
	{
		g_pDisplay->Clear();

		if (n == -1)
			hr = g_pDisplay->Blt(left, top, pBack);
		else
		{
			hr = g_pDisplay->Blt(left, top, pBack);
			hr = g_pDisplay->Blt(pFrame->Xpos, pFrame->Ypos, pFrame);
		}
	}

	if( FAILED( hr = GetFramework()->ShowFrame() ) )
	{
		if( DDERR_SURFACELOST == hr )
		{
			(GetFramework())->RestoreSurfaces();
			RestoreSurfaces();
		}
	}

	if(pBack)
		SAFE_DELETE(pBack);
	if(pFrame)
		SAFE_DELETE(pFrame);
	// nate 2005 - 01 : loading2
	SAFE_DELETE(pFrame2);
	
	CPacketControl::EnableSend(TRUE);
	SendDummyMsg();
	CPacketControl::EnableSend(FALSE);
}

void CMyD3DApplication::MidLoadingReserve(int n)
{
	m_nLoadingFrameReserve = n;
}

void CMyD3DApplication::LoadingProgress(int percent, int nBmpNum)
{
	if( !g_pDisplay )
		return;

	static int progressX, progressY, progressWidth, progressHeight, help_x, help_y;
	static int nWidth, nHeight;
	char loading[] = "interface/Loading/loading.bmp";   // ???? ??????

	char help[] = "interface/Loading/loading_help.bmp";
	CSurface *pSurfaceTest=NULL;
	CSurface *pProgressSurf = NULL;
	CSurface *pSurfaceHelp = NULL;
	HRESULT hr;
	char *strPath = GetExePath();
	_chdir(strPath);

	if (percent <= 0)
	{
		int sizeX = 640;
		int sizeY = 480;
		if (m_ProOption.resolution == R800)
		{
			sizeX = 800;
			sizeY = 600;			
		}
		else if (m_ProOption.resolution == R1024)
		{
			sizeX = 1024;
			sizeY = 768;			
		}
		else if( m_ProOption.resolution == R1280 )
		{
			sizeX = 1280;
			sizeY = 960;			
		}
		else if( m_ProOption.resolution == R1280W )
		{
			sizeX = 1280;
			sizeY = 720;
		}
		else if( m_ProOption.resolution == R1600W )
		{
			sizeX = 1600;
			sizeY = 900;
		}
		else if( m_ProOption.resolution == R1920W )
		{
			sizeX = 1920;
			sizeY = 1080;
		}

		progressX = PROGRESS_X * sizeX / BASE_WIDTH;
		progressY = PROGRESS_Y * sizeY / BASE_HEIGHT;
		progressWidth = PROGRESS_WIDTH * sizeX / BASE_WIDTH;
		progressHeight = PROGRESS_HEIGHT * sizeY / BASE_HEIGHT;
		help_x = HELPTEXT_X * sizeX / BASE_WIDTH;
		help_y = HELPTEXT_Y * sizeY / BASE_HEIGHT;
		nWidth = sizeX;
		nHeight = sizeY;

		// ?????? ???? ?????? ????.
		g_pDisplay->CreateSurfaceFromBitmap(&pSurfaceTest, loading, nWidth, nHeight);
		g_pDisplay->CreateSurfaceFromBitmap(&pSurfaceHelp, help);
		g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar.bmp", progressWidth, progressHeight);

		g_pDisplay->Blt(0, 0, pSurfaceTest);
		g_pDisplay->Blt(progressX, progressY, pProgressSurf);
		g_pDisplay->Blt(help_x, help_y, pSurfaceHelp);
	}
	else if (g_pDisplay)
	{
		// Progressing......
		int nSel = percent / 10;

		switch (nSel)
		{
		case 1:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar0.bmp", progressWidth, progressHeight);
			break;
		case 2:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar1.bmp", progressWidth, progressHeight);
			break;
		case 3:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar2.bmp", progressWidth, progressHeight);
			break;
		case 4:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar3.bmp", progressWidth, progressHeight);
			break;
		case 5:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar4.bmp", progressWidth, progressHeight);
			break;
		case 6:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar5.bmp", progressWidth, progressHeight);
			break;
		case 7:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar6.bmp", progressWidth, progressHeight);
			break;
		case 8:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar7.bmp", progressWidth, progressHeight);
			break;
		case 9:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar8.bmp", progressWidth, progressHeight);
			break;
		case 10:
			g_pDisplay->CreateSurfaceFromBitmap(&pProgressSurf, "interface/loading/loading_bar9.bmp", progressWidth, progressHeight);
			break;
		default:
			pProgressSurf = NULL;
			break;
		}

		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.bottom = progressHeight;
		rect.right = progressWidth * percent / 100;

		// ?????? ???? ?????? ????.
		g_pDisplay->CreateSurfaceFromBitmap(&pSurfaceTest, loading, nWidth, nHeight);
		g_pDisplay->CreateSurfaceFromBitmap(&pSurfaceHelp, help);
		g_pDisplay->Blt(0, 0, pSurfaceTest);
		if (pProgressSurf)
			g_pDisplay->Blt(progressX, progressY, pProgressSurf);
		g_pDisplay->Blt(help_x, help_y, pSurfaceHelp);
	}

	// Show the frame on the primary surface.
	if( FAILED( hr = GetFramework()->ShowFrame() ) )
	{
		if( DDERR_SURFACELOST == hr )
		{
			(GetFramework())->RestoreSurfaces();
			RestoreSurfaces();
		}
	}
	// ????..
	SAFE_DELETE(pSurfaceTest);
	SAFE_DELETE(pSurfaceHelp);
	SAFE_DELETE(pProgressSurf);

	SendDummyMsg();
}



//---------------------------------------------------------
// name : LoadingWorldImg(int percent)
// desc : ???????? ?????????? Progress Bar ????
//---------------------------------------------------------
#define LOADING_WORLD_TEXT_CNT  8

void CMyD3DApplication::LoadingWorldImg(int percent)
{
	if( !g_pDisplay )
		return;

	RECT rc;
	HRESULT hr;
	int nWidth, nHeight;
	char back[] = "interface/loading/world_loading_back.bmp";
	char prog[] = "interface/loading/world_loading_bar.bmp";
	char text_name[100];
	int idx;
	int cur_world = GetCurWorldID();
	int cur_village = GetCurVillageID();

	if(cur_world == 0 && cur_village == -1) // ?? ?????? ????
		idx = 0;
	else if(cur_world == 0 && cur_village != -1) // ?????? ????
		idx = 0;
	else if(cur_world == 4)
	{
		// ?? ?????? ????
		idx = 4;
		m_iCurVillageId = -1;
	}
	else if(cur_world == 1)
	{
		// ?? ???????? ????
		idx = 1;
		m_iCurVillageId = -1;
	}
	else if(cur_world == 2)
	{
		// ?? ???????? ????
		idx = 2;
		m_iCurVillageId = -1;
	}
	else if(cur_world == 3)
		idx = 9;
	else if(cur_world == 5)
		idx = 10;
	else if(cur_world == 6)
		idx = 11;
	else if( cur_world == 7 )
		idx = 12;
	else if( cur_world == 8 )
		idx = 13;
	else if( cur_world == 9 )	// nate 2005-08-30 : ???????? ???? ??????

		idx = 14;
	else if( cur_world == 11)	// ???? ???? ??
		idx = 15;
	else if( cur_world == 13)	// ???? ??????
		idx = 16;
	else if( cur_world == 14)	// ???????? ??????
		idx = 17;
	else if( cur_world == 15)	// ???????? ??????
		idx = 18;
	else if( cur_world == 16)	// ???? ??????
		idx = 19;
	else if( cur_world == 17)	// ?????????? ??????
		idx = 20;
	else if( cur_world == 18)	// ???? ??????
		idx = 21;
	else if( cur_world == 21)	// KRUMA
		idx = 22;
	else if( cur_world == 20)	// ??????????
		idx = 23;
	else if( cur_world == 22)	// ???????? S2
		idx = 24;
	else if( cur_world == 23)	// ???????? S3
		idx = 25;
	else if( cur_world == 24)	// ???????? S4
		idx = 26;
	else if( cur_world == 19)	// Moebius
		idx = 6;
	else if( cur_world == 25)	// Hightland
		idx = 7;
	else if( cur_world == 10)	// MATRIX
		idx = 27;
	else if( cur_world == 26)	// Tritian
		idx = 8;
	else if( cur_world == 27)	// Space
		idx = 28;
	else if( cur_world == 28)	// royale
		idx = 29;
	else if( cur_world == 29)	// dun910
		idx = 30;
	else if( cur_world == 30)	// zone930
		idx = 31;
	else
	{
		idx = 99;	// ???? ????????????.
		m_iCurVillageId = -1;
	}

	CSurface *pBackSur=NULL;
	CSurface *pProgSur = NULL;
	CSurface *pTextSur = NULL;

	nWidth = g_pNk2DFrame->GetClientWidth();
	nHeight = g_pNk2DFrame->GetClientHeight();

	g_pDisplay->CreateSurfaceFromBitmap(&pBackSur, back); // BACK
	if( pBackSur )
	{
		pBackSur->SetColorKey(RGB(0, 0, 0));
		pBackSur->Xpos = ( g_pNk2DFrame->GetClientWidth() - pBackSur->GetWidth() )>>1;
		pBackSur->Ypos = ( g_pNk2DFrame->GetClientHeight() - pBackSur->GetHeight() )>>1;

		g_pDisplay->CreateSurfaceFromBitmap(&pProgSur, prog); // BAR
		if( pProgSur )
		{
			pProgSur->Xpos = pBackSur->Xpos+35;
			pProgSur->Ypos = pBackSur->Ypos+66;
		}

		sprintf(text_name, "interface/loading/text%d.bmp", idx); // TEXT
		g_pDisplay->CreateSurfaceFromBitmap(&pTextSur, text_name);
		if( !pTextSur )
			g_pDisplay->CreateSurfaceFromBitmap(&pTextSur, "interface/loading/text99.bmp");

		if( pTextSur )
		{
			pTextSur->SetColorKey(RGB(0, 0, 0));
			pTextSur->Xpos = pBackSur->Xpos+(pBackSur->GetWidth()/2)-(pTextSur->GetWidth()/2);
			pTextSur->Ypos = pBackSur->Ypos+20;
		}
	}

	int cx = 0, cy = 0;
	if( pProgSur )
	{
		cx = pProgSur->GetWidth();
		cy = pProgSur->GetHeight();
	}
	int width = percent * cx / 100;
	SetRect(&rc, 0, 0, width, cy);

	if( pBackSur )
		g_pDisplay->Blt(pBackSur->Xpos, pBackSur->Ypos, pBackSur);

	if( width > 0 && pProgSur )
		g_pDisplay->Blt(pProgSur->Xpos, pProgSur->Ypos, pProgSur, &rc);

	if( pTextSur )
		g_pDisplay->Blt(pTextSur->Xpos, pTextSur->Ypos, pTextSur);


	// Show the frame on the primary surface.
	if( FAILED( hr = (GetFramework())->ShowFrame() ) )
	{
		if( DDERR_SURFACELOST == hr )
		{
			(GetFramework())->RestoreSurfaces();
			RestoreSurfaces();
		}
	}

	SAFE_DELETE(pBackSur);
	SAFE_DELETE(pProgSur);
	SAFE_DELETE(pTextSur);

	CPacketControl::EnableSend(TRUE);
	SendDummyMsg();
	CPacketControl::EnableSend(FALSE);
}

void CMyD3DApplication::LoadingWorldImgReserve(int percent)
{
	m_nLoadingGoWorldFrameReserve = percent;
}

//---------------------------------------------------------------------------
// Name: LoadGameResources()
// Desc: ???? ?????? ????

//     : ???? ?????????? ?????? ????.
//---------------------------------------------------------------------------
BOOL CMyD3DApplication::LoadGameResources()
{
	if( g_dwLangType == LANG_IDN/*|| g_dwLangType == LANG_DEU*/|| g_dwLangType == LANG_TH || g_dwLangType == LANG_ENG) // 06-11-14 ???? ???????? ????(?? ???????? ????????. ?????? ?????? ???? ?????? ?????????? ????)
	{
		if (m_ProOption.resolution == R640)
			Font_AddPosY = -2;
		else
			Font_AddPosY = -3;
	}
	else
		Font_AddPosY = 0;

	m_bGameMode = FALSE;
	// ???????? ???? ?????? ?????? ????.

	// Config ???? ????.
	FILE *fpCfg = NULL;

	if (g_bFFullMode)
		ExcuteFFullOption();
	else if (g_bFFullMode != TRUE && m_bWindowed)
		ExcuteWindowedOption();
	else
		ExcuteOption();

	// 4/4 ?????? 2D ?????? ???? ???? ????.
	if (m_ProOption.resolution == R800)
		if( g_pNk2DFrame )
			g_pNk2DFrame->Load2DResourcesInGame();

	if (!g_bWorldLoading)
		MidLoading(-1); // ????....

	m_pDecoTexture = new IndexedTexture(300);

	if( m_pDecoTexture )
		m_pDecoTexture->SetTexturePath("data/building/textr/");

	// ???? ????. ?????? ?????? ????.
	m_pLand = new CLand;
	m_pPath = new CPath;
	if( m_pPath )
		m_pPath->Init(m_pLand);
	if( g_pShadowRect )
		g_pShadowRect->Init( m_pLand );

	// ???? ??????.
	g_ClrAlloc = new CClrAlloc(m_bRGB565);
	LoadEffectSound();
	//-------------------------------------------------
	// nate 2005-05-06 : ???????? ?????? ?????? ????
	g_pCapsyongTexture->SetTexturePath("data/item/textr/");
	if( g_MatrixSvr != 0 )
	{
		g_pWhangLolo1 = new CLolos;
		g_pWhangLolo1->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo1->Read( "data/effect/lolos/m_symbol_g.LOL" );

		g_pWhangLolo2 = new CLolos;
		g_pWhangLolo2->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo2->Read( "data/effect/lolos/m_symbol_b.LOL" );

		g_pWhangLolo3 = new CLolos;
		g_pWhangLolo3->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo3->Read( "data/effect/lolos/m_symbol_y.LOL" );

		g_pWhangLolo4 = new CLolos;
		g_pWhangLolo4->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo4->Read( "data/effect/lolos/Map4_Crystal.lol" );
	}
	// nate 2004 - 4 CastleWar
	else
	{
		g_pWhangLolo1 = new CLolos;
		g_pWhangLolo1->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo1->Read( "data/effect/lolos/c_top_2.lol" );

		g_pWhangLolo2 = new CLolos;
		g_pWhangLolo2->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo2->Read( "data/effect/lolos/c_top.lol" );

		g_pWhangLolo3 = new CLolos;
		g_pWhangLolo3->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo3->Read( "data/effect/lolos/c_top.lol" );

		g_pWhangLolo4 = new CLolos;
		g_pWhangLolo4->SetIndexedTexture( g_pCapsyongTexture );
		g_pWhangLolo4->Read( "data/effect/lolos/Map4_Crystal.lol" );
	}
	//-------------------------------------------------
	MidLoading(0);
	PlayerInitialLoading();
	m_dwPrevFrameTime = timeGetTime();

	if (m_ProOption.shadow == SHADOW_NONE)
	{
		g_pRoh = new CNkCharacter( g_pShadowRect, NULL, g_ClrAlloc );
	}
	else
	{
		g_pRoh = new CNkCharacter( g_pShadowRect, m_pGoodShadow, g_ClrAlloc );
	}

	m_pCompas = new CCompas;
	if( m_pCompas )
		m_pCompas->LoadRes();
	m_pWeatherMgr = new CWeatherMgr;
	if( m_pWeatherMgr )
		m_pWeatherMgr->LoadRes();
	m_pFireFly = new CFireFly;
	if( m_pFireFly )
		m_pFireFly->SetDevice();

	if (m_pSlotChar)
	{
		g_pRoh->SetInfo(-1, m_pSlotChar->m_Name);		// SetInfo???? id?? ?????? ??????.
		g_pRoh->MakeBasicBody(m_pSlotChar->m_Race, m_pSlotChar->m_Sex, m_pSlotChar->m_Hair);
		g_pRoh->SetLevel(m_pSlotChar->m_Level);
		g_pRoh->SetVital(m_pSlotChar->m_Vital, m_pSlotChar->m_MaxVital);
		g_pRoh->SetMana(m_pSlotChar->m_Mana, m_pSlotChar->m_MaxMana);
		g_pRoh->SetStamina(m_pSlotChar->m_Stamina, m_pSlotChar->m_MaxStamina);
		g_pRoh->SetEpower(m_pSlotChar->m_Epower, m_pSlotChar->m_MaxEpower);
		g_pRoh->SetAbil(m_pSlotChar->m_Str, m_pSlotChar->m_Int, m_pSlotChar->m_Dex,
						m_pSlotChar->m_Con, m_pSlotChar->m_Cha);

		if( g_MatrixSvr != 0 ) // ???????? ???? ???? ???? ????.(05-11-15 ????)
			g_pRoh->m_GuildIndex = 0; // ???????? ?????? ??????????.(???? ???? ???????? ?????????? ?????? ???????? ???????? ??????????????.)
		else
			g_pRoh->m_GuildIndex = m_pSlotChar->m_GuildIndex;

		g_pRoh->SetMotionState(CHA_STAND);// ?????? ?? ???? ?????? ??????????.
	}
	else
	{
		g_pRoh->SetInfo(-1, "woohyun");
		g_pRoh->MakeBasicBody(3, 1, 0);
		g_pRoh->m_dir = 1.5707f;
		g_pRoh->SetMotionState(CHA_STAND);
		// tutorial game play?? ??????.
		m_bPlaying = TRUE;
	}
	if( g_pNk2DFrame )
		g_pNk2DFrame->SetNkCha(g_pRoh);
	//MidLoading(4);
	m_pUIMgr = new CUIMgr;
	m_pFxSet->LoadResAfter();

	switch( g_MatrixSvr )
	{
	case 1:
		MobInitialLoading( WORLD_MATRIX );
		break;
	case 2:
		MobInitialLoading( WORLD_MATRIX_2009 );
		break;
	default:
		MobInitialLoading( WORLD_START );
		break;
	}

	MidLoading(1);
	PetInitialLoading();

	AdjustViewport();

	// ???? ????
	// Initialize the tree and background meshes

	D3DVECTOR d3dVector;
	d3dVector.x = 0;
	d3dVector.y = 0;
	d3dVector.z = 0.99999f;

	D3DVECTOR d3dVector4;
	d3dVector4.x = 0;
	d3dVector4.y = 0;
	d3dVector4.z = 0.999f;

	m_CloudMesh[0] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 0, 1 );
	m_CloudMesh[1] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 0, 0 );
	m_CloudMesh[2] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 1, 1 );
	m_CloudMesh[3] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 1, 0 );

	m_CloudLayerMesh[0] = D3DTLVERTEX( d3dVector4, 1.0f, 0x00300668, 0, 0, 1 );
	m_CloudLayerMesh[1] = D3DTLVERTEX( d3dVector4, 1.0f, 0x00300668, 0, 0, 0 );
	m_CloudLayerMesh[2] = D3DTLVERTEX( d3dVector4, 1.0f, 0x00300668, 0, 1, 1 );
	m_CloudLayerMesh[3] = D3DTLVERTEX( d3dVector4, 1.0f, 0x00300668, 0, 1, 0 );

	TLVec[0] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 0, 1 );
	TLVec[1] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 1, 1 );
	TLVec[3] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 1, 0 );

	TLVec2[0] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 0, 1 );
	TLVec2[1] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 0, 0 );
	TLVec2[2] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 1, 1 );
	TLVec2[3] = D3DTLVERTEX( d3dVector, 1.0f, 0xFF999999, 0, 1, 0 );

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = FX_WEAPONEFFECTS;
	ef_sort.pNkChaTo = g_pRoh;
	if( pCMyApp->m_pFxSet )
		g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

// 	//===============================================================
// 	fprintf( logF, "current time : %d \n", timeGetTime() );
// 	fprintf( logF, "end : %d \n", timeGetTime() - tempCurTime );
// 	fprintf( logF, "//==========================================================//\n" );
// 	fclose( logF );
// 	//===============================================================
	return TRUE;
}

//---------------------------------------------------------------------------
// Name: SetLoginInfo()
// Desc: ???? ???? ????
//---------------------------------------------------------------------------
void CMyD3DApplication::SetLoginInfo(int nSvrIdx, char *strUserId, char *strUserPw, char *strUserKey)
{
	m_nAutoLoginSvrIdx = nSvrIdx;
	strcpy(m_strUserId, strUserId);
	strcat(m_strUserId, " ");
	strcat(m_strUserId, strUserKey);
	strcpy(m_strUserPw, strUserPw);
}

//---------------------------------------------------------------------------
// Name: AutoLogin()
// Desc: ???? ????
//---------------------------------------------------------------------------
void CMyD3DApplication::AutoLogin()
{
	if (g_pNk2DFrame && g_pNk2DFrame->GetSheet())
		g_pNk2DFrame->GetSheet()->AutoLogin(m_nAutoLoginSvrIdx, m_strUserId, m_strUserPw);
}

//---------------------------------------------------------------------------
// Name: PrepareChangeWorld()
// Desc: ?????? ???????? ???????? ?????????? ??????.
//---------------------------------------------------------------------------
void CMyD3DApplication::PrepareChangeWorld()
{
	// ???????? ?????? - ???? ???? ??????
	CDmitronInfo::SetBuff(0, 0);
	CDmitronInfo::SetBuff(1, 0);
	CDmitronInfo::SetBuff(2, 0);
	CDmitronInfo::SetBuff(3, 0);

	if (GetCurWorld() == WORLD_DUNGEON_S)
	{
		g_dwBeforeGoWorldTime = timeGetTime();
		g_bBeforeGoSky = TRUE;
	}
	else
		g_bBeforeGoSky = FALSE;

	m_bBeforeGoWorld = TRUE;

	CPacketControl::EnableSend(FALSE);
}

void CMyD3DApplication::SetTransTime( BOOL bBeforeTrans)
{
	m_dwTransBaseTime = timeGetTime();
	m_bBeforeTrans = bBeforeTrans; // TRUE
}

//---------------------------------------------------------------------------
// Name: ConnectWorld()
// Desc:
//---------------------------------------------------------------------------
BOOL CMyD3DApplication::ConnectWorld(char *strIpAddr, int nPort)
{
	char strBuf[1024] = "";
	char strVersion[256] = "";

	if (g_pTcpIp)
	{
		g_pTcpIp->ResetConnection();
		//1217 ?????????? ???? ??????(pulse ??????)?? ???? ?????? ??????...
		delete g_pTcpIp;
		g_pTcpIp = NULL;
		g_pTcpIp = new CTcpIpConnection;
		if( !g_pTcpIp )
			return false;

		g_pTcpIp->ResetSendingBuffer();
		
		if( g_pTcpIp->ConnectToServer(strIpAddr, nPort) == FALSE )
		{
			g_pTcpIp->CloseSocket();
			return FALSE;
		}
		
		sprintf(strVersion, "%d\n", PACKET_VER);
		g_pTcpIp->SendNetMessage(strVersion, TRUE);
		
		int loginType = 1;
		SendLogin(loginType, false, true, g_dev_client, m_strUserId, m_strUserPw);		
	}
	else
	{
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_SERVER_INIT), TYPE_NOR_OK, 1 );
//		MessageBox( NULL, "???? ?????? ?????? ??????????.", "Caution", MB_OK ); // ???????? ?????? ???? 20?????? at ?????? ?????? ?????????? ???? ????????????. ?????? ???? ???? ???????? ???????? ????.
	}

	return TRUE;
}

// ?? ?????? ???? ???? ???? ????

// ?????? ???? ?????? ?????? ??????.
// ?? ?????? ??????(???? ??????) ?? ?????? ????.
BOOL CMyD3DApplication::GoWorld(char *strIpAddr, int nPort, int nWorldNum, int nAreaNum)
{	
	char strMapPath[255] = "";
	char strDecoPath[255] = "";
	char strBuf[256] = "";

	if( !g_pNk2DFrame )
		return FALSE;

	g_pNk2DFrame->CloseAllWindow(); // ?? ???????? ???? ???????? ???? ???????? ????.(???????? ???? ?????? ?????? ????-???????? ????.) (05-10-27 ????) (???? ???? ?????? ?????? ???? ?????? ????)

	{
		if( g_pNk2DFrame->GetControlGuild() != NULL )
			g_pNk2DFrame->GetControlGuild()->SetRecomInfo(FALSE);
		g_pNk2DFrame->GetUIWindow()->AllCloseWindow();
	}

	if( g_SvrType == ST_ADULT_ONLY )
	{
		g_pNk2DFrame->GetTgaUIWindow()->AllCloseWindow();
	}

	// ???? Shadow?? Reset ????.
	G_SK_ResetShadow();
	Goworld_CharInit();	// nate 2005-07-28 : ???? ???? ??????
	g_bWorldLoading = TRUE;
	g_bNewTerrainDungeon = FALSE;	// wan:2004-11
	// ?????? ????.
	CPacketControl::EnableSend(TRUE);
	SpeedChecker::GetInstance()->Reset();
	if (!ConnectWorld(strIpAddr, nPort))
	{
		CPacketControl::EnableSend(FALSE);
		return FALSE;
	}
	CPacketControl::EnableSend(FALSE);

	m_nCurWorldIdx = nWorldNum;
	m_nAreaNum = nAreaNum;
	g_nCurWorldIdx = m_nCurWorldIdx;
	SetCurWorldID(g_nCurWorldIdx);
	LoadingWorldImg(0); // ????....

	if(g_pNk2DFrame->GetInterface(QUEST))
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetNoQuest();

	if (g_pNk2DFrame->GetControlParty())
		g_pNk2DFrame->GetControlParty()->RemoveMemberAll();

	if (g_pDSound)
	{
		if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_MUTE3]))
			g_pDSound->Stop(g_EffectSound[EFF_SND_MUTE3]);
	}

	LoadingWorldImg(5); // ????....

	// ?????? ????.
	if( m_pMusicManager )
		m_pMusicManager->StopAll();

	if (m_pUIMgr)
		m_pUIMgr->EraseAllList();

	DeleteEffect_Fairy(SKILL_FAIRY);
	DeleteEffect_Fairy(SKILL_LUCKYPIG1);
	DeleteEffect_Fairy(SKILL_LUCKYPIG2);
	DeleteEffect_Fairy(SKILL_LUCKYPIG3);
	DeleteEffect_Fairy(SKILL_LUCKYPIG4);
	DeleteEffect_Fairy(SKILL_BABYSOUL_1);
	DeleteEffect_Fairy(SKILL_BABYSOUL_2);
	DeleteEffect_Fairy(SKILL_BABYSOUL_3);
	DeleteEffect_Fairy(SKILL_VAMPIREBAT);
	DeleteEffect_Fairy(SKILL_PETRUDOLPH);

	DeleteEffect_Fairy(SKILL_RING_LIGHT_WHITE);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_DARK);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_RED);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_BLUE);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_GREEN);

	DeleteEffect_Fairy(SKILL_ROYALE_1);
	DeleteEffect_Fairy(SKILL_ROYALE_2);
	DeleteEffect_Fairy(SKILL_ROYALE_3);
	DeleteEffect_Fairy(SKILL_ROYALE_4);
	DeleteEffect_Fairy(SKILL_ROYALE_5);
	DeleteEffect_Fairy(SKILL_ROYALE_6);
	DeleteEffect_Fairy(SKILL_ROYALE_7);
	DeleteEffect_Fairy(SKILL_ROYALE_8);
	DeleteEffect_Fairy(SKILL_ROYALE_9);
	DeleteEffect_Fairy(SKILL_ROYALE_10);
	DeleteEffect_Fairy(SKILL_ROYALE_12);
	DeleteEffect_Fairy(SKILL_ROYALE_12);
	DeleteEffect_Fairy(SKILL_ROYALE_13);
	DeleteEffect_Fairy(SKILL_ROYALE_14);
	DeleteEffect_Fairy(SKILL_ROYALE_15);

	DeleteEffect_Fairy(SKILL_ARUPAKA_PET);
	DeleteEffect_Fairy(SKILL_ARUPAKA_PET_BE);
	DeleteEffect_Fairy(SKILL_ARUPAKA_PET_BR);

	DeleteEffect_Fairy(SKILL_PET_CATS_BLACK);
	DeleteEffect_Fairy(SKILL_PET_CATS_WHITE);
	DeleteEffect_Fairy(SKILL_PET_CATS_MIX);

	DeleteEffect_Fairy(SKILL_PET_ZORO_BLACK);
	DeleteEffect_Fairy(SKILL_PET_ZORO_WHITE);
	DeleteEffect_Fairy(SKILL_PET_ZORO_GREEN);

	DeleteEffect_Fairy(SKILL_PET_VALO_BLACK);
	DeleteEffect_Fairy(SKILL_PET_VALO_WHITE);
	DeleteEffect_Fairy(SKILL_PET_VALO_GREEN);

	DeleteEffect_Fairy(SKILL_FAIRY_HUMAN_SILVER);
	DeleteEffect_Fairy(SKILL_FAIRY_HUMAN_BLACK);
	DeleteEffect_Fairy(SKILL_FAIRY_KAI_BLUE);
	DeleteEffect_Fairy(SKILL_FAIRY_KAI_RED);
	DeleteEffect_Fairy(SKILL_FAIRY_HYB_SILVER);
	DeleteEffect_Fairy(SKILL_FAIRY_HYB_GOLD);
	DeleteEffect_Fairy(SKILL_FAIRY_AID_BLUE);
	DeleteEffect_Fairy(SKILL_FAIRY_AID_GREEN);
	DeleteEffect_Fairy(SKILL_FAIRY_BUL_SILVER);
	DeleteEffect_Fairy(SKILL_FAIRY_BUL_VIOLET);

	DeleteEffect_Fairy(SKILL_FAIRY_NECK1);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK2);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK3);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK4);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK5);

	DeleteEffect_Fairy(SKILL_RING_LIGHT_GOLD);


	DeleteEffect_Fairy(SKILL_PET_BEAR_BLACK);
	DeleteEffect_Fairy(SKILL_PET_BEAR_WHITE);

	DeleteEffect_Fairy(SKILL_PET_HORSE_BLUE);
	DeleteEffect_Fairy(SKILL_PET_HORSE_RED);

	DeleteEffect_Fairy(SKILL_PET_JEGUAR_1);
	DeleteEffect_Fairy(SKILL_PET_JEGUAR_2);
	DeleteEffect_Fairy(SKILL_PET_JEGUAR_3);

	DeleteEffect_Fairy(SKILL_PET_BONI_1);
	DeleteEffect_Fairy(SKILL_PET_BONI_2);
	DeleteEffect_Fairy(SKILL_PET_BONI_3);

	DeleteEffect_Fairy(SKILL_PET_AZUL);		//NEW PET PABLO
	DeleteEffect_Fairy(SKILL_PET_VERDE);
	DeleteEffect_Fairy(SKILL_PET_ROXO);
	DeleteEffect_Fairy(SKILL_PET_AMARELO);
	DeleteEffect_Fairy(SKILL_PET_VERMELHO);
	DeleteEffect_Fairy(SKILL_PET_CINZA);
	DeleteEffect_Fairy(SKILL_PET_FESHIK);
	DeleteEffect_Fairy(SKILL_PET_ALTAIR_P);
	DeleteEffect_Fairy(SKILL_PET_FESHIK_P);
	DeleteEffect_Fairy(SKILL_PET_EZULAN);
	DeleteEffect_Fairy(SKILL_PET_EZULAN_P);
	DeleteEffect_Fairy(SKILL_PET_THUNDERBIRD);
	DeleteEffect_Fairy(SKILL_PET_THUNDERBIRD_P);
	DeleteEffect_Fairy(SKILL_PET_PETFAME1);
	DeleteEffect_Fairy(SKILL_PET_PETFAME2);
	DeleteEffect_Fairy(SKILL_PET_PETFAME3);
	DeleteEffect_Fairy(SKILL_PET_PETFAME4);
	DeleteEffect_Fairy(SKILL_PET_HELPER);
	DeleteEffect_Fairy(SKILL_PET_HALL1);
	DeleteEffect_Fairy(SKILL_PET_HALL2);
	DeleteEffect_Fairy(SKILL_PET_HALL3);
	DeleteEffect_Fairy(SKILL_PET_NATAL1);
	DeleteEffect_Fairy(SKILL_PET_NATAL2);
	DeleteEffect_Fairy(SKILL_PET_NATAL3);

	LoadingWorldImg(10); // ????....

	m_music_id = 0;
	m_bGameMode = FALSE;
	m_bPlaying = FALSE;
	g_bBeforeGoSky = FALSE;
	g_bSendSalesOff = FALSE;
	m_music_id = 0;

	char *strPath = GetExePath();
	_chdir(strPath);

	LoadingWorldImg(15); // ????....

	g_bPath = FALSE;
	g_bDrawDetailGround = TRUE;

	if (m_pDecoTexture)
	{
		m_pDecoTexture->InvalidateAllTextures();
		m_pDecoTexture->DestroyTextureAll();
		SAFE_DELETE(m_pDecoTexture);
	}

	// nate 2005-08-24 : ???????? ???? ?????? ???? ????
	if( m_nCurWorldIdx == WORLD_DMITRON ||
		m_nCurWorldIdx == WORLD_DMITRON_BATTLE)
		m_pDecoTexture = new IndexedTexture(400);
	else
		m_pDecoTexture = new IndexedTexture(300); // 07-01-15 ?????????? ?????? ???????? ???? ///??????????????


	LoadingWorldImg(30); // ????....
	
	//// ?????? ????????????????. ?????? ????????????. 2
	LoadingWorldImg(40); // ????....
	// nate 2004 - 7 m_pTestTexture????

	LoadWorldMap(m_nCurWorldIdx);

	if (g_bDungeon)
	{
		ChangeHour(24);
		if( m_pWeatherMgr )
			m_pWeatherMgr->DungeonWeatherSystem();
	}
	else if (GetCurWorld() == WORLD_SKY || GetCurWorld() ==WORLD_DUNGEON4 || GetCurWorld() ==WORLD_DUN910)
	{
		ChangeHour(0);
		if( m_pWeatherMgr )
			m_pWeatherMgr->DungeonWeatherSystem();		// ???? ?? ???? ???? ????????.
	}
	else
	{
		if( m_pWeatherMgr )
			m_pWeatherMgr->StopWeatherSystem();

		// ?? ???????? ?? ???????? ???? ????????. (05-12-12 ????)
		if( g_pDSound )
		{
			if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_L_RAIN]))
			{
				g_pDSound->Stop(g_EffectSound[EFF_SND_L_RAIN]);
			}

			if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_H_RAIN]))
			{
				g_pDSound->Stop(g_EffectSound[EFF_SND_H_RAIN]);
			}
		}

		ChangeHour(m_nCurHour);
		if( m_pWeatherMgr )
			m_pWeatherMgr->ResumeWeatherSystem();
	}

	if( m_pDecoTexture )
		m_pDecoTexture->RestoreAllTextures(m_pd3dDevice);

	LoadingWorldImg(60); // ????....
	MobInitialLoading(m_nCurWorldIdx);
	LoadingWorldImg(70);

	LoadingWorldImg(80); // ????....
	//// ?????? ????????????????. ?????? ????????????. 4.
	LoadingWorldImg(90); // ????....
	// ???? ??????.
	//----------------------------------------------------------------------
	// Music - Midi
	//----------------------------------------------------------------------
	if( g_dwClientCountry != CTRY_JPN )  ///????BGM ???????? ?????? ?????? ?? ?????? ????????
	{
		// ?????? ?? ?????? Midi ?? ????.
		if(m_nCurWorldIdx == WORLD_DUNGEON4
			|| m_nCurWorldIdx == WORLD_DMITRON
			|| m_nCurWorldIdx == WORLD_DISPOSAL
			|| m_nCurWorldIdx == WORLD_FORLORN_S2
			|| m_nCurWorldIdx == WORLD_FORLORN_S3
			|| m_nCurWorldIdx == WORLD_FORLORN_S4
			)
		{
			// ?????? ????.
			if( m_pMusicManager )
				m_pMusicManager->StopAll();
			// 			else if( m_pMusicManager && m_ProOption.bUseMusicForEachZone )
			// 				m_pMusicManager->Play( m_nBackMidi[m_music_id], true );
		}
		else
		{
			if( m_pMusicManager && m_ProOption.bUseMusicForEachZone )
				m_pMusicManager->Play( m_nBackMidi[m_music_id], true );
		}
	}

	if( m_nCurWorldIdx == WORLD_GUILD ) // ?????? ???????? ?? ???? ?????? ???? ?????????????? ?????? ????.(???????? ?? ?????? ?????? ???? ????????.?????? ???????? ?????? ???????? ?????? ???? ?????? ??????)
	{
		pCMyApp->GetLandPtr()->SetFieldAttr( 3724, 2398, 3763, 2431, TRUE );
	}
	else if( m_nCurWorldIdx == WORLD_WHITEHORN ) // ???? ?????? ???? ????????. ///07-04-26 ????
	{
		pCMyApp->GetLandPtr()->SetFieldAttr( 10320, 7820, 10353, 7843, FALSE ); // ???? ???? ???????????? ??????????...
	}
	else if( m_nCurWorldIdx == WORLD_DMITRON ) // ???? ?????? ???? ????????. ///07-04-26 ????
	{
		//pCMyApp->GetLandPtr()->SetFieldAttr( 6564, 6260, 6705, 6400, TRUE ); //??????????.

		// ITS #1790 (387, 119) ???? ?? ???? ???? ???? ???? by. Sol9 [2012.06.14]
		pCMyApp->GetLandPtr()->SetFieldAttr(3884, 11608, 3892, 11641, FALSE);	// ?????? ????????
		pCMyApp->GetLandPtr()->SetFieldAttr(3740, 11616, 3920, 11650, FALSE);	// ???? ????
		pCMyApp->GetLandPtr()->SetFieldAttr(3893, 11642, 3916, 11650, FALSE);	// ?????? ????
	}

	m_bGameMode = TRUE;
	m_bWaitingAt = TRUE;
	if( m_pUIMgr )
	{
		m_pUIMgr->m_bOnBattle = FALSE;					// ??????????? ?????? FALSE
		// nate 2004 - 5 : ???????? ???? ???? - ???????? ????????
		m_pUIMgr->m_byOnBattleDecardi = 0;
	}

	m_dwGoWorldBaseTime = timeGetTime();
	LoadingWorldImg(100); // ????....
	g_bWorldLoading = FALSE;

	if( m_pUIMgr )
	{
		if( m_pUIMgr->isFirstNewLetter == FALSE)
			m_pUIMgr->isFirstNewLetter = TRUE;
	}

	CPacketControl::EnableSend(FALSE);

	return TRUE;
}

void CMyD3DApplication::GoWorldLoading()
{
	LoadingWorldImg(0); // ????....

	if(g_pNk2DFrame->GetInterface(QUEST))
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetNoQuest();

	if (g_pNk2DFrame->GetControlParty())
		g_pNk2DFrame->GetControlParty()->RemoveMemberAll();

	if (g_pDSound)
	{
		if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_MUTE3]))
			g_pDSound->Stop(g_EffectSound[EFF_SND_MUTE3]);
	}

	LoadingWorldImg(5); // ????....

	// ?????? ????.
	if( m_pMusicManager )
		m_pMusicManager->StopAll();

	if (m_pUIMgr)
		m_pUIMgr->EraseAllList();

	DeleteEffect_Fairy(SKILL_FAIRY);
	DeleteEffect_Fairy(SKILL_LUCKYPIG1);
	DeleteEffect_Fairy(SKILL_LUCKYPIG2);
	DeleteEffect_Fairy(SKILL_LUCKYPIG3);
	DeleteEffect_Fairy(SKILL_LUCKYPIG4);
	DeleteEffect_Fairy(SKILL_BABYSOUL_1);
	DeleteEffect_Fairy(SKILL_BABYSOUL_2);
	DeleteEffect_Fairy(SKILL_BABYSOUL_3);
	DeleteEffect_Fairy(SKILL_VAMPIREBAT);
	DeleteEffect_Fairy(SKILL_PETRUDOLPH);

	DeleteEffect_Fairy(SKILL_RING_LIGHT_WHITE);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_DARK);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_RED);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_BLUE);
	DeleteEffect_Fairy(SKILL_RING_LIGHT_GREEN);

	DeleteEffect_Fairy(SKILL_ROYALE_1);
	DeleteEffect_Fairy(SKILL_ROYALE_2);
	DeleteEffect_Fairy(SKILL_ROYALE_3);
	DeleteEffect_Fairy(SKILL_ROYALE_4);
	DeleteEffect_Fairy(SKILL_ROYALE_5);
	DeleteEffect_Fairy(SKILL_ROYALE_6);
	DeleteEffect_Fairy(SKILL_ROYALE_7);
	DeleteEffect_Fairy(SKILL_ROYALE_8);
	DeleteEffect_Fairy(SKILL_ROYALE_9);
	DeleteEffect_Fairy(SKILL_ROYALE_10);
	DeleteEffect_Fairy(SKILL_ROYALE_12);
	DeleteEffect_Fairy(SKILL_ROYALE_12);
	DeleteEffect_Fairy(SKILL_ROYALE_13);
	DeleteEffect_Fairy(SKILL_ROYALE_14);
	DeleteEffect_Fairy(SKILL_ROYALE_15);

	DeleteEffect_Fairy(SKILL_ARUPAKA_PET);
	DeleteEffect_Fairy(SKILL_ARUPAKA_PET_BE);
	DeleteEffect_Fairy(SKILL_ARUPAKA_PET_BR);

	DeleteEffect_Fairy(SKILL_PET_CATS_BLACK);
	DeleteEffect_Fairy(SKILL_PET_CATS_WHITE);
	DeleteEffect_Fairy(SKILL_PET_CATS_MIX);

	DeleteEffect_Fairy(SKILL_PET_ZORO_BLACK);
	DeleteEffect_Fairy(SKILL_PET_ZORO_WHITE);
	DeleteEffect_Fairy(SKILL_PET_ZORO_GREEN);

	DeleteEffect_Fairy(SKILL_PET_VALO_BLACK);
	DeleteEffect_Fairy(SKILL_PET_VALO_WHITE);
	DeleteEffect_Fairy(SKILL_PET_VALO_GREEN);

	DeleteEffect_Fairy(SKILL_FAIRY_HUMAN_SILVER);
	DeleteEffect_Fairy(SKILL_FAIRY_HUMAN_BLACK);
	DeleteEffect_Fairy(SKILL_FAIRY_KAI_BLUE);
	DeleteEffect_Fairy(SKILL_FAIRY_KAI_RED);
	DeleteEffect_Fairy(SKILL_FAIRY_HYB_SILVER);
	DeleteEffect_Fairy(SKILL_FAIRY_HYB_GOLD);
	DeleteEffect_Fairy(SKILL_FAIRY_AID_BLUE);
	DeleteEffect_Fairy(SKILL_FAIRY_AID_GREEN);
	DeleteEffect_Fairy(SKILL_FAIRY_BUL_SILVER);
	DeleteEffect_Fairy(SKILL_FAIRY_BUL_VIOLET);

	DeleteEffect_Fairy(SKILL_FAIRY_NECK1);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK2);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK3);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK4);
	DeleteEffect_Fairy(SKILL_FAIRY_NECK5);

	DeleteEffect_Fairy(SKILL_RING_LIGHT_GOLD);


	DeleteEffect_Fairy(SKILL_PET_BEAR_BLACK);
	DeleteEffect_Fairy(SKILL_PET_BEAR_WHITE);

	DeleteEffect_Fairy(SKILL_PET_HORSE_BLUE);
	DeleteEffect_Fairy(SKILL_PET_HORSE_RED);

	DeleteEffect_Fairy(SKILL_PET_JEGUAR_1);
	DeleteEffect_Fairy(SKILL_PET_JEGUAR_2);
	DeleteEffect_Fairy(SKILL_PET_JEGUAR_3);

	DeleteEffect_Fairy(SKILL_PET_BONI_1);
	DeleteEffect_Fairy(SKILL_PET_BONI_2);
	DeleteEffect_Fairy(SKILL_PET_BONI_3);

	DeleteEffect_Fairy(SKILL_PET_AZUL);		//NEW PET PABLO
	DeleteEffect_Fairy(SKILL_PET_VERDE);
	DeleteEffect_Fairy(SKILL_PET_ROXO);
	DeleteEffect_Fairy(SKILL_PET_AMARELO);
	DeleteEffect_Fairy(SKILL_PET_VERMELHO);
	DeleteEffect_Fairy(SKILL_PET_CINZA);
	DeleteEffect_Fairy(SKILL_PET_FESHIK);
	DeleteEffect_Fairy(SKILL_PET_ALTAIR_P);
	DeleteEffect_Fairy(SKILL_PET_FESHIK_P);

	LoadingWorldImg(10); // ????....	

	char *strPath = GetExePath();
	_chdir(strPath);

	LoadingWorldImg(15); // ????....

	g_bPath = FALSE;
	g_bDrawDetailGround = TRUE;

	if (m_pDecoTexture)
	{
		m_pDecoTexture->InvalidateAllTextures();
		m_pDecoTexture->DestroyTextureAll();
		SAFE_DELETE(m_pDecoTexture);
	}

	// nate 2005-08-24 : ???????? ???? ?????? ???? ????
	if( m_nCurWorldIdx == WORLD_DMITRON ||
		m_nCurWorldIdx == WORLD_DMITRON_BATTLE)
		m_pDecoTexture = new IndexedTexture(400);
	else
		m_pDecoTexture = new IndexedTexture(300); // 07-01-15 ?????????? ?????? ???????? ???? ///??????????????


	LoadingWorldImg(30); // ????....	

	//// ?????? ????????????????. ?????? ????????????. 2
	LoadingWorldImg(40); // ????....
	// nate 2004 - 7 m_pTestTexture????

	LoadWorldMap(m_nCurWorldIdx);	
	
	LoadingWorldImg(60); // ????....	

	if (g_bDungeon)
	{
		ChangeHour(24);
		if( m_pWeatherMgr )
			m_pWeatherMgr->DungeonWeatherSystem();
	}
	else if (GetCurWorld() == WORLD_SKY || GetCurWorld() ==WORLD_DUNGEON4 || GetCurWorld() ==WORLD_DUN910)
	{
		ChangeHour(0);
		if( m_pWeatherMgr )
			m_pWeatherMgr->DungeonWeatherSystem();		// ???? ?? ???? ???? ????????.
	}
	else
	{
		if( m_pWeatherMgr )
			m_pWeatherMgr->StopWeatherSystem();

		// ?? ???????? ?? ???????? ???? ????????. (05-12-12 ????)
		if( g_pDSound )
		{
			if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_L_RAIN]))
			{
				g_pDSound->Stop(g_EffectSound[EFF_SND_L_RAIN]);
			}

			if (g_pDSound->IsPlaying(g_EffectSound[EFF_SND_H_RAIN]))
			{
				g_pDSound->Stop(g_EffectSound[EFF_SND_H_RAIN]);
			}
		}

		ChangeHour(m_nCurHour);
		if( m_pWeatherMgr )
			m_pWeatherMgr->ResumeWeatherSystem();
	}

	if( m_pDecoTexture )
		m_pDecoTexture->RestoreAllTextures(m_pd3dDevice);
		
	LoadingWorldImg(100);
	MobInitialLoading(m_nCurWorldIdx);
	
	// ???? ??????.
	//----------------------------------------------------------------------
	// Music - Midi
	//----------------------------------------------------------------------
	if( g_dwClientCountry != CTRY_JPN )  ///????BGM ???????? ?????? ?????? ?? ?????? ????????
	{
		// ?????? ?? ?????? Midi ?? ????.
		if(m_nCurWorldIdx == WORLD_DUNGEON4
			|| m_nCurWorldIdx == WORLD_DMITRON
			|| m_nCurWorldIdx == WORLD_DISPOSAL
			|| m_nCurWorldIdx == WORLD_FORLORN_S2
			|| m_nCurWorldIdx == WORLD_FORLORN_S3
			|| m_nCurWorldIdx == WORLD_FORLORN_S4
			)
		{
			// ?????? ????.
			if( m_pMusicManager )
				m_pMusicManager->StopAll();
			// 			else if( m_pMusicManager && m_ProOption.bUseMusicForEachZone )
			// 				m_pMusicManager->Play( m_nBackMidi[m_music_id], true );
		}
		else
		{
			if( m_pMusicManager && m_ProOption.bUseMusicForEachZone )
				m_pMusicManager->Play( m_nBackMidi[m_music_id], true );
		}
	}

	if( m_nCurWorldIdx == WORLD_GUILD ) // ?????? ???????? ?? ???? ?????? ???? ?????????????? ?????? ????.(???????? ?? ?????? ?????? ???? ????????.?????? ???????? ?????? ???????? ?????? ???? ?????? ??????)
	{
		pCMyApp->GetLandPtr()->SetFieldAttr( 3724, 2398, 3763, 2431, TRUE );
	}
	else if( m_nCurWorldIdx == WORLD_WHITEHORN ) // ???? ?????? ???? ????????. ///07-04-26 ????
	{
		pCMyApp->GetLandPtr()->SetFieldAttr( 10320, 7820, 10353, 7843, FALSE ); // ???? ???? ???????????? ??????????...
	}
	else if( m_nCurWorldIdx == WORLD_DMITRON ) // ???? ?????? ???? ????????. ///07-04-26 ????
	{
		//pCMyApp->GetLandPtr()->SetFieldAttr( 6564, 6260, 6705, 6400, TRUE ); //??????????.

		// ITS #1790 (387, 119) ???? ?? ???? ???? ???? ???? by. Sol9 [2012.06.14]
		pCMyApp->GetLandPtr()->SetFieldAttr(3884, 11608, 3892, 11641, FALSE);	// ?????? ????????
		pCMyApp->GetLandPtr()->SetFieldAttr(3740, 11616, 3920, 11650, FALSE);	// ???? ????
		pCMyApp->GetLandPtr()->SetFieldAttr(3893, 11642, 3916, 11650, FALSE);	// ?????? ????
	}

	m_bGameMode = TRUE;
	m_bWaitingAt = TRUE;
	if( m_pUIMgr )
	{
		m_pUIMgr->m_bOnBattle = FALSE;					// ??????????? ?????? FALSE
		// nate 2004 - 5 : ???????? ???? ???? - ???????? ????????
		m_pUIMgr->m_byOnBattleDecardi = 0;
	}

	m_dwGoWorldBaseTime = timeGetTime();		

	g_bWorldLoading = FALSE;

	if( m_pUIMgr )
	{
		if( m_pUIMgr->isFirstNewLetter == FALSE)
			m_pUIMgr->isFirstNewLetter = TRUE;
	}

	m_GraphicMode = NKGRAPHIC3D;
	g_pNk2DFrame->SetGraphicMode(m_GraphicMode);
	g_pNk2DFrame->SetLoadingGoworldNow(false);
}

//---------------------------------------------------------------------------
// Name: LoadWorldMap()
// Desc: ?? ????.
//---------------------------------------------------------------------------
BOOL CMyD3DApplication::LoadWorldMap(int zone_index)
{
	if( !m_pLand )
		return false;

	if( m_ProOption.resolution == R1280W ||
		m_ProOption.resolution == R1600W ||
		m_ProOption.resolution == R1920W )
	{
		m_Aspect = 0.5625f;
	}
	else
	{
		m_Aspect = 0.75f;
	}

	m_pLand->SetDevice( GetFramework()->GetD3DDevice() );
	m_pLand->SetD3D( GetFramework()->GetDirect3D() );
	m_pLand->DoModulate( m_bCanDoModulate );
	m_pLand->DoModulate2X( m_bCanDoModulate2X );
	m_pLand->DoHighQuality( m_bCanHighQuality );


	const LHParam_MapInfo * info = m_param_manager->GetMapInfo(zone_index);

	if( !info )
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		return FALSE;
	}

	// ???? ?????? ???? ????
	g_bDungeon = info->m_dungeon;
	g_bDrawDetailGround = info->m_detail_ground;
	g_bNewTerrainDungeon = info->m_new_dungeon;
	g_bNewTerrain = info->m_new_terrain;
	g_bSnow = info->m_snow;
	m_music_id = info->m_music_id;

	if( CommonConfig::Instance()->GetEventSakura() == true )
	{
		if( zone_index == 4 )
			g_bSakura = TRUE;
		else
			g_bSakura = FALSE;
	}
	assert(info->m_new_terrain == g_bNewTerrain);
	if( info->m_new_terrain )
	{
		if( !m_pLand->LoadMap( info->m_path_map.c_str() , info->m_filename.c_str() ) )
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}

		if( m_pFrustum )
		{
			if( IsCurWorldID_BossraidZone() ) // ?????????? ???????? ???? ?????? ????????.
			{
				float fDist = 1600.0f;//CNKObjQuadNode::GetRenderDist();
				m_pFrustum->InitObjFrustum( fDist * 2.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, fDist * 2.0f );
			}
			else if( pCMyApp->GetCurWorld() == 29 ) // ?????????? ???????? ???? ?????? ????????.
			{
				m_pFrustum->InitObjFrustum( 800.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 800.0f );
			}
			else
			{
				m_pFrustum->InitObjFrustum( 1600.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 1600.0f );
			}
		}
	}
	else
	{
		m_pDecoTexture->SetTexturePath("data/effect/testtex/");
		if (m_pLand->Init((char*)info->m_path_map.c_str(), m_pDecoTexture, zone_index) == FALSE)
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}
		if( m_pDecoTexture )
			m_pDecoTexture->SetTexturePath("data/building/textr/");
		m_pLand->CalcWaterNoise();

		if( m_pFrustum )
			m_pFrustum->InitFrustum(m_Fov, m_Aspect, m_zNear, m_zFar);
	}

	if( info->m_floor != -1 )
		m_pUIMgr->SetFloor(0);

	//////////////////////////////////////////////////////////////

	if (!g_bWorldLoading)
		MidLoading(3);
	if (!m_pCloudSea)
	{
		m_pCloudSea = new CCloudSea;
		if( m_pCloudSea )
		{
			m_pCloudSea->MakeInitStruct(0, LAND_VISIBLE_DEPTH);
			m_pCloudSea->MakeNoise();
			m_pCloudSea->DoModulate2X(m_bCanDoModulate2X);
		}
	}
	if (!m_pSkyPlane)
	{
		m_pSkyPlane = new CSkyPlane;
		if( m_pSkyPlane )
		{
			m_pSkyPlane->Init(280.0f, 295.0f, 5.0f, 5.0f, 142.0f);
		}
	}

	DecoObjInitialLoading( (char*)info->m_path_deco.c_str(), (char*)info->m_filename.c_str() );

	return TRUE;
}

BOOL CMyD3DApplication::LoadWorldMap(char *strMapFilePath, char *strDecoFilePath, int nMapNum)
{
	if( !m_pLand )
		return false;

	TCHAR	szMapName[255];
	char *strPath = GetExePath();
	_chdir(strPath);

	m_pLand->SetDevice( GetFramework()->GetD3DDevice() );
	m_pLand->SetD3D( GetFramework()->GetDirect3D() );
	m_pLand->DoModulate( m_bCanDoModulate );
	m_pLand->DoModulate2X( m_bCanDoModulate2X );
	m_pLand->DoHighQuality( m_bCanHighQuality );

	if( g_bNewTerrain )
	{
		if( nMapNum == WORLD_START )
		{
			_tcscpy( szMapName, _T("start") );
		}
		else if( nMapNum == WORLD_GUILD_WAR )
		{
			_tcscpy( szMapName, _T("gbattle"));
		}
		else if( nMapNum == WORLD_GUILD )
			_tcscpy( szMapName, _T( "base" ) );		
		else if( nMapNum == WORLD_WHITEHORN )
			_tcscpy( szMapName, _T("whitehorn") );
		else if( nMapNum == WORLD_FIELD )
			_tcscpy( szMapName, _T("neogeo") );		
		else if( nMapNum == WORLD_DUNGEON4 )
			_tcscpy( szMapName, _T("dungeon4") );		
		else if( nMapNum == WORLD_DMITRON )
			_tcscpy( szMapName, _T("dmitron"));
		else if( nMapNum == WORLD_MATRIX )
			_tcscpy( szMapName, _T( "matrix" ) );
		else if( nMapNum == WORLD_DUNGEON1 )
			_tcscpy( szMapName, _T("dungeon1") );
		else if( nMapNum == WORLD_DMITRON_BATTLE )
			_tcscpy( szMapName, _T("dmitron_battle") );
		else if( nMapNum == WORLD_BOSSRAID_BULKAN )
			_tcscpy( szMapName, _T("bulkan_boss") );
		else if( nMapNum == WORLD_BOSSRAID_KAILIPTON )
			_tcscpy( szMapName, _T("kailipton_boss") );
		else if( nMapNum == WORLD_BOSSRAID_AIDIA )
			_tcscpy( szMapName, _T("aidia_boss") );
		else if( nMapNum == WORLD_BOSSRAID_HUMAN )
			_tcscpy( szMapName, _T("human_boss") );
		else if( nMapNum == WORLD_BOSSRAID_HYBRIDER )
			_tcscpy( szMapName, _T("hybrid_boss") );
		else if( nMapNum == WORLD_BOSSRAID_FINALBOSS )
			_tcscpy( szMapName, _T("final_boss") );
		else if( nMapNum == WORLD_MATRIX_2009 )
			_tcscpy( szMapName, _T("moebius") );
		else if( nMapNum == 20 )
		{
			_tcscpy( szMapName, _T("forlom_zon01") );
		}
		else if( nMapNum == 21 )
		{
			_tcscpy( szMapName, _T("aidia") );
		}
		else
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}

		if( m_pLand->LoadMap( strMapFilePath, szMapName ) == FALSE )
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}

		if( m_pFrustum )
		{
			if( IsCurWorldID_BossraidZone() ) // ?????????? ???????? ???? ?????? ????????.
			{
				float fDist = 1600.0f;//CNKObjQuadNode::GetRenderDist();
				m_pFrustum->InitObjFrustum( fDist * 2.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, fDist * 2.0f );
			}
			else if( pCMyApp->GetCurWorld() == 29 ) // ?????????? ???????? ???? ?????? ????????.
			{
				m_pFrustum->InitObjFrustum( 800.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 800.0f );
			}
			else
			{
				m_pFrustum->InitObjFrustum( 1600.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 1600.0f );
			}
		}
	}
	else
	{
		// nate 2004 - 7 m_pTestTexture????
		m_pDecoTexture->SetTexturePath("data/effect/testtex/");
		if (m_pLand->Init(strMapFilePath, m_pDecoTexture, nMapNum) == FALSE)  	// pIndexedTexture?? ???? ???????????? ????.
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}
		if( m_pDecoTexture )
			m_pDecoTexture->SetTexturePath("data/building/textr/");
		m_pLand->CalcWaterNoise();
		if( m_pFrustum )
			m_pFrustum->InitFrustum(m_Fov, m_Aspect, m_zNear, m_zFar);
		szMapName[0] = NULL;
	}

	if (!g_bWorldLoading)
		MidLoading(3); // ????....
	if (!m_pCloudSea)
	{
		m_pCloudSea = new CCloudSea;
		if( m_pCloudSea )
		{
			m_pCloudSea->MakeInitStruct(0, LAND_VISIBLE_DEPTH);
			m_pCloudSea->MakeNoise();
//			m_pCloudSea->SetDevice(m_pd3dDevice);
			m_pCloudSea->DoModulate2X(m_bCanDoModulate2X);
		}
	}
	if (!m_pSkyPlane)
	{
		m_pSkyPlane = new CSkyPlane;
		if( m_pSkyPlane )
		{
			m_pSkyPlane->Init(280.0f, 295.0f, 5.0f, 5.0f, 142.0f);
//			m_pSkyPlane->SetDevice(m_pd3dDevice);
		}
	}

	// Decoration Object ????

	DecoObjInitialLoading( strDecoFilePath, szMapName );

	return TRUE;
}

BOOL CMyD3DApplication::GetLoadFilePath(const char *strFilePathName, char *strMapPath, char *strDecoPath)
{
	PFILE *fp = NULL;
	char strTemp[MAX_PATH];
	char *res = NULL;

	// ?????????? ???? ??????.
	char *strPath = GetExePath();
	_chdir(strPath);

	fp = pfopen(strFilePathName, "rt");
	if (fp == NULL)
		return FALSE;
	res = pfgets(strTemp, sizeof(strTemp), fp);
	assert(res != NULL);
	strcpy(strMapPath, strTemp);
	res = pfgets(strTemp, sizeof(strTemp), fp);
	assert(res != NULL);
	strcpy(strDecoPath, strTemp);
	pfclose(fp);

	return TRUE;
}


//---------------------------------------------------------------------------
// Name: OpenNewMap()
// Desc: ?? ?????? ???? ?????????? ????.
//---------------------------------------------------------------------------
void CMyD3DApplication::OpenNewMap()
{
	static char szFilter[] = "Load File (*.lod)\0*.lod\0\0";
	FILE *fp = NULL;
	OPENFILENAME ofn;
	char strFilePathName[MAX_PATH] = {0,};
	char strFileName[MAX_PATH] = {0,};
	char strMapPath[MAX_PATH] = {0,};
	char strDecoPath[MAX_PATH] = {0,};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = pCMyApp->Get_hWnd();
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = strFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = strFileName;;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_READONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "lod";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	// ???? ?????????? ?????? ???????? ?????? ???? ?????? ????????.
	BOOL bSuccess = GetOpenFileName(&ofn);

	if (bSuccess)
	{
		bSuccess = GetLoadFilePath(strFilePathName, strMapPath, strDecoPath);
		if (bSuccess)
		{
			char *strPath = GetExePath();
			_chdir(strPath);
			LoadWorldMap(strMapPath, strDecoPath );
		}
	}
}

//------------------------------------------------------------------------------------------
// ???? ???????? "choose_mode.txt "?? ???? "0"???? ????????????  "1"???? ???????????? ????????
//------------------------------------------------------------------------------------------
void CMyD3DApplication::Read_ChooseMode(char *filename)
{
	FILE *fp = NULL;
	char *sentence = NULL;
	char buf[256] = {0, }, first_arg[256] = {0, }, second_arg[256] = {0, };
	char line[256] = {0, };
	int res;
	char config_file[100] = {0, };
	strcpy(config_file, "config.ini");

	// ?????????? ???????? ????????.
	fp = fopen(config_file, "rt");
	m_bWindowed = FALSE;
	g_bFFullMode = FALSE;

	if (fp != NULL)
	{
		sentence = GetSentence(buf, sizeof(buf), fp);

		while (sentence != NULL)
		{
			AnyTwoArg(buf, first_arg, second_arg);
			res = atoi(second_arg);

			if (!stricmp("Windowed", first_arg))
			{
				if (res == 1)
					m_bWindowed = TRUE;
			}
			sentence = GetSentence(buf, sizeof(buf), fp);
		}
		fclose(fp);
	}
	else
	{
		m_bWindowed = FALSE;
		g_bFFullMode = TRUE;
	}

	strcat(config_file, "3");
	fp = fopen(config_file, "rt");

	if (fp == NULL)
		g_bAdmin = FALSE;
	else
	{
		int val = 0;
		res = 0;
		while (fgets(line, 10, fp) != NULL)
		{
			val = atoi(line);
			res += val;
		}

		if (res == 8888)
		{
			g_dev_client = true;
			g_bAdmin = TRUE;
		}
		else
			g_bAdmin = FALSE;

		fclose(fp);
	}

	strcpy(config_file, "broad");
	strcat(config_file, "cast");
	fp = fopen(config_file, "rt");
	if (fp)
	{
		int val;
		res = 0;
		while (fgets(line, 10, fp) != NULL)
		{
			val = atoi(line);
			res += val;
		}

		if (res == 10)
		{
			g_bAdmin = TRUE;
			//g_bBroad = TRUE;
		}

		fclose(fp);
	}

	if (g_dwClientCountry == CTRY_IDN)
		return;
	if (g_dwClientCountry == CTRY_ENG)	// ?????????? ????

		return;
	if (g_dwClientCountry == CTRY_JPN)
		return;
	if (g_dwClientCountry == CTRY_DEU)
		return;
	if (g_dwClientCountry == CTRY_TH)	// ?????????? ????

		return;
	if (g_dwClientCountry == CTRY_DENG)
		return;
	if (g_dwClientCountry == CTRY_MENG)
		return;
	if ( g_dwClientCountry == CTRY_BRA )
		return;

	// ???? ???? ?????? ?????? ?????? ???? ??????.
	if (g_dwClientCountry != 0 && g_dwLangType != 0 && g_bAdmin != TRUE)
		m_bWindowed = FALSE;

	return;
}

//---------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
	// ???? ???????? ???? ??.
	///m_Origin[0] = D3DLVERTEX(D3DVECTOR_New( 5.0f,0.0f,-5.0f),0x00ffffff,0,0,0);
	///m_Origin[1] = D3DLVERTEX(D3DVECTOR_New(-5.0f,0.0f,-5.0f),0x00ffffff,0,0,0);
	///m_Origin[2] = D3DLVERTEX(D3DVECTOR_New(-5.0f,10.0f,-5.0f),0x00ffffff,0,0,0);
	///m_Origin[3] = D3DLVERTEX(D3DVECTOR_New( 5.0f,10.0f,-5.0f),0x00ffffff,0,0,0);
	///m_Origin[4] = D3DLVERTEX(D3DVECTOR_New( 5.0f,0.0f, 5.0f),0x00ffffff,0,0,0);
	///m_Origin[5] = D3DLVERTEX(D3DVECTOR_New(-5.0f,0.0f, 5.0f),0x00ffffff,0,0,0);
	///m_Origin[6] = D3DLVERTEX(D3DVECTOR_New(-5.0f,10.0f, 5.0f),0x00ffffff,0,0,0);
	///m_Origin[7] = D3DLVERTEX(D3DVECTOR_New( 5.0f,10.0f, 5.0f),0x00ffff00,0,0,0);

	// ??????
	m_idxOrigin[0] = 0;
	m_idxOrigin[1] = 1;
	m_idxOrigin[2] = 2;
	m_idxOrigin[3] = 0;
	m_idxOrigin[4] = 2;
	m_idxOrigin[5] = 3;
	m_idxOrigin[6] = 0;
	m_idxOrigin[7] = 3;
	m_idxOrigin[8] = 7;
	m_idxOrigin[9] = 0;
	m_idxOrigin[10]= 7;
	m_idxOrigin[11]= 4;
	m_idxOrigin[12]= 1;
	m_idxOrigin[13]= 5;
	m_idxOrigin[14]= 6;
	m_idxOrigin[15]= 1;
	m_idxOrigin[16]= 6;
	m_idxOrigin[17]= 2;
	m_idxOrigin[18]= 2;
	m_idxOrigin[19]= 6;
	m_idxOrigin[20]= 7;
	m_idxOrigin[21]= 2;
	m_idxOrigin[22]= 7;
	m_idxOrigin[23]= 3;
	m_idxOrigin[24]= 0;
	m_idxOrigin[25]= 4;
	m_idxOrigin[26]= 5;
	m_idxOrigin[27]= 0;
	m_idxOrigin[28]= 5;
	m_idxOrigin[29]= 1;
	m_idxOrigin[30]= 4;
	m_idxOrigin[31]= 7;
	m_idxOrigin[32]= 6;
	m_idxOrigin[33]= 4;
	m_idxOrigin[34]= 6;
	m_idxOrigin[35]= 5;

	return S_OK;
}

//---------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove( FLOAT fTimeKey )
{
	if (m_bSatelliteMode)
	{
		D3DVECTOR	from, at, up;
		from = D3DVECTOR_New(g_pRoh->m_wx, 250.0f, g_pRoh->m_wz);
		at = D3DVECTOR_New(g_pRoh->m_wx, 0.0f, g_pRoh->m_wz);
		up = D3DVECTOR_New(0.0f, 1.0f, 0.0f);
		D3DUtil_SetViewMatrix(m_matVirView, from, at, up);
		m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matVirView );
		m_pd3dDevicePick->SetTransform( D3DTRANSFORMSTATE_VIEW, &m_matVirView );
	}

	if (m_pUIMgr)
		m_pUIMgr->ProcessFrame();

	if (m_bHeroMoving)
	{
		if (g_bPath)
			HeroMovingT();
		else
			HeroMoving();
	}

	if (m_bRotateCamera)
		RotateCamera();

	if( m_SpiritRotateCamera )
		SpiritRotateCamera();

	if( m_bSpiritZoomIn )
		SpiritZoomIn();

	assert(m_pLand != NULL);

	if( m_pFrustum )
		m_pFrustum->SetFrustum(m_matView, m_vEye);

	if( !g_bNewTerrain )
	{
		if (g_bDungeon)
			m_pLand->DungeonInit(m_vAt.x, m_vAt.z);
		else
			m_pLand->QuadInit(m_pFrustum, m_vAt.x, m_vAt.z);
	}

	if (m_pCloudSea && GetCurWorld() == WORLD_SKY)
		m_pCloudSea->QuadInit(m_pFrustum, m_vAt.x, m_vAt.z);

	if (g_pDSound)
	{
		D3DVECTOR vPlayer = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		g_pDSound->AdjustSoundSourceDirMatrix(m_vEye, vPlayer);

		D3DVECTOR vDir;
		vDir.x = m_vEye.x - vPlayer.x;
		vDir.y = m_vEye.y - vPlayer.y;
		vDir.z = m_vEye.z - vPlayer.z;
		vDir = Normalize(vDir);
		m_vListener = vPlayer;

		m_vListener.x += (vDir.x * 50);
		m_vListener.y += (vDir.y * 50);
		m_vListener.z += (vDir.z * 50);
	}

	return S_OK;
}

//---------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
	if( !m_pd3dDevice )
		return S_FALSE;

	LPDIRECTDRAWSURFACE7 lpDDsurface;
	D3DMATRIX matWorld, matTrans, matRotate;
	static DWORD dwAmbiColor;
	static DWORD dwPrevTime = 0;

	lpDDsurface = NULL;

	if (NeedPostLoading())
	{
		m_pUIMgr->PostLoadGuildMark();
		m_bPostLoading = FALSE;
	}

	if( g_bNewTerrain )
		m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_dwFogColor, 1.0f, 0L );
	else if (GetCurWorld() == WORLD_DUNGEON_S)
		m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DRGB(0.06666, 0.07843, 0.09803), 1.0f, 0L );
	else
		m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DRGB(0, 0, 0), 1.0f, 0L );

	if( m_pd3dDevicePick )
		m_pd3dDevicePick->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DRGBA(0, 0, 0, 0), 1.0f, 0L );

	if (m_pUIMgr)
		m_pUIMgr->IsVk1Pressed();

	m_bPickBegin = FALSE;

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		if (m_pd3dDevicePick && SUCCEEDED(m_pd3dDevicePick->BeginScene()))
		{
			m_bPickBegin = TRUE;
			m_pd3dDevicePick->SetTexture(0, NULL);
			m_pd3dDevicePick->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
			///m_pd3dDevicePick->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_pickMesh, 4, 0 );
			m_pd3dDevicePick->DrawPrimitive(D3DPT_TRIANGLESTRIP,  4, 0);
			m_pd3dDevicePick->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
		}
		else
		{
			OutputDebugString("PickBegin Fail\n");
		}

		D3DLIGHT7 d3dLight;
		float dx, dz;
		if( m_pLightCtrl )
			m_pLightCtrl->LightingEnalbe(TRUE);

		if ( m_pLightCtrl && m_pLightCtrl->IsEnabled(NKLIGHT_POINT) )
		{
			GetDirectionElement(g_pRoh->m_dir, dx, dz);
			if (GetCurWorld() != WORLD_SKY)
			{
				m_pLightCtrl->InitLight(NKLIGHT_POINT, D3DLIGHT_POINT,
										m_vAt.x+dx*5.0f, m_pLand->GetHFHeight(m_vAt.x, m_vAt.z)+30.0f, m_vAt.z+dz*5.0f,
										m_fPointRColor, m_fPointGColor, m_fPointBColor);
				m_pLightCtrl->SetLight(NKLIGHT_POINT);
				m_pLightCtrl->LightEnable(NKLIGHT_POINT, TRUE);
			}
			else
			{
				m_pLightCtrl->LightEnable(NKLIGHT_POINT, FALSE);
			}
		}

		if (!(g_bDungeon||g_bNewTerrainDungeon))
		{
			if( pCMyApp->GetCurWorld() == WORLD_GUILD_WAR )
				RenderSky();
			else if(m_bShowRenderSky2 && m_bCanDoModulate && m_bCanDoModulate2X && m_bCanDoAdd)
			{
				if (m_pSkyPlane)
					m_pSkyPlane->Render(g_pRoh->m_wx, 1800.0f, g_pRoh->m_wz, m_bCanDoVertexFog);
				else
					RenderSky2();
			}
			else
				RenderSky();
		}

		if (m_pUIMgr)
			m_pUIMgr->IsVk1Pressed();

		if( m_pLightCtrl )
			m_pLightCtrl->LightingEnalbe(TRUE);

		dwPrevTime = timeGetTime();
		if( m_pLightCtrl )
			m_pLightCtrl->LightingEnalbe(TRUE);
		if (g_bCsyMipmap)
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );

		SetLandFogParameter();

		if (m_pUIMgr)
			m_pUIMgr->RenderDecoObjs();
		if (g_bCsyMipmap)
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );

		m_dwDecoTime = timeGetTime() - dwPrevTime;

		if (m_pUIMgr)
			m_pUIMgr->IsVk1Pressed();

		dwPrevTime = timeGetTime();
		if( m_pLightCtrl )
			m_pLightCtrl->LightingEnalbe(FALSE);
		D3DUtil_SetIdentityMatrix(m_matWorld);
		m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );


		if( m_pLand )
		{
			if( g_bNewTerrain )
				m_pLand->RenderTerrain( m_pFrustum );
			else
				m_pLand->Render(g_pRoh->m_wx, g_pRoh->m_wz);
		}

		if( m_pLightCtrl )
			m_pLightCtrl->LightingEnalbe(TRUE);
		m_dwLandTime = timeGetTime() - dwPrevTime;

		if (m_pLand && m_bSelTile)
			m_pLand->DrawSelectedPoints();


		if (m_pCloudSea && GetCurWorld() == WORLD_SKY)
			m_pCloudSea->Render(g_pRoh->m_wx, g_pRoh->m_wz, m_bCanDoVertexFog);

		if (m_bSatelliteMode)
		{
			D3DVECTOR *pviewVolumeVector = m_pFrustum->GetFrustumPoints();
			D3DVERTEX Line[4];

			Line[0] = D3DVERTEX(m_vEye, D3DVECTOR_New(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
			Line[1] = D3DVERTEX(pviewVolumeVector[0], D3DVECTOR_New(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
			Line[2] = D3DVERTEX(m_vEye, D3DVECTOR_New(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
			Line[3] = D3DVERTEX(pviewVolumeVector[1], D3DVECTOR_New(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_CLIPPING , FALSE);
			m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_LVERTEX, Line, 4, 0);
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_CLIPPING , TRUE);
		}

		if (g_bCsyMipmap)
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );

		m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, &g_matView);
		D3DMath_MatrixInvert(g_invView, g_matView);


		if (m_pUIMgr && !m_pUIMgr->IsEventScenePlaying() )
		{
			m_pUIMgr->RenderPlayers();
			m_pUIMgr->RenderMobs();
			m_pUIMgr->RenderUserShopNpc(); ///????????.
			m_pUIMgr->RenderPets();
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			m_pUIMgr->RenderItems();
		}

		if (g_pRoh && m_pUIMgr && !m_pUIMgr->IsEventScenePlaying() )
			DrawPlayer();


		if (m_pUIMgr && m_pUIMgr->IsEventScenePlaying() )
		{
			m_pUIMgr->PlayEventScene(); // ?????? ???? ????.
		}

		if (g_bCsyMipmap)
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );

		if (m_pUIMgr)
			m_pUIMgr->RenderAlphaObjs();

		// prevent automouse
		if (m_pUIMgr)
			m_pUIMgr->IsVk1Pressed();


		if( m_pLand )
		{
			if( g_bNewTerrain )
			{
				if( IsWaterRender() )
				{
					if( g_bCsyMipmap )
						m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );

					CNKObjQuadNode::RenderWater();

					if( g_bCsyMipmap )
						m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_NONE );
				}
			}
			else
			{
				if (GetCurWorld() == WORLD_MATRIX)
				{
					m_pLand->RenderWaterArea(g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_MATWATER]),
											 g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_MATWAVE]));
				}
				else
				{
					m_pLand->RenderWaterArea(g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_WATER]),
											 g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_WAVE]));
				}
			}
		}

		if (m_pUIMgr)
			m_pUIMgr->Render();

		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);

		if (m_bCanDoVertexFog && g_bDungeon != TRUE )
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);

		if (m_pUIMgr)
			m_pUIMgr->IsVk1Pressed();

		dwPrevTime = timeGetTime();

		if (m_pFxSet)
			m_pFxSet->Render();


		if (m_pCompas && !g_pNk2DFrame->CheckFullScreenInterface() )
		{
			D3DVECTOR vPlayer = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
			m_pCompas->CalDirection(m_vEye, vPlayer);

			if( g_SvrType == ST_ADULT_ONLY )
			{
				if( !g_pNk2DFrame->IsPVPVisible() )
				{
					m_pCompas->Render(m_pCompas->GetHalfWidth(), m_pCompas->GetHalfHeight());
				}
			}
			else
			{
				m_pCompas->Render(m_pCompas->GetHalfWidth(), m_pCompas->GetHalfHeight());
			}
		}


		dwPrevTime = timeGetTime();

		if (m_pWeatherMgr && !(g_bDungeon || g_bNewTerrainDungeon ))
			m_pWeatherMgr->Render();


		m_dwWeatherTime = timeGetTime() - dwPrevTime;
		if (m_pFireFly && !g_bDungeon)
			m_pFireFly->Render();

		if (m_bShowCutton && g_bDungeon)
			RenderCutton();

#define PLAYER_SPEAR_SIZE 20.0f
#define MONSTER_SPEAR_SIZE 30.0f

		CNkCharacter* pCha;
		LHCharacterMap_it pr;
		for( pr = m_pUIMgr->m_character_map.begin() ; pr != m_pUIMgr->m_character_map.end() ; pr ++ )
		{
			pCha = (*pr).second;
			if( pCha && pCha->m_byShowCrash_Miss )
			{
				if( m_pFrustum->FrustumCullPoint( pCha->m_wx, pCha->m_wy, pCha->m_wz,
												  PLAYER_SPEAR_SIZE ) == VFCULL_OUTSIDE )
					continue;
				pCha->RenderCrash_Miss();
			}
		}



		CNkMob* pMob;
		LHMobMap_it prMob;
		for( prMob = m_pUIMgr->m_mob_map.begin() ; prMob != m_pUIMgr->m_mob_map.end() ; prMob ++ )
		{
			pMob = (*prMob).second;

			if( m_pFrustum->FrustumCullPoint( pMob->m_wx, pMob->m_wy, pMob->m_wz, MONSTER_SPEAR_SIZE ) == VFCULL_OUTSIDE )
				continue;

			if(pMob->m_nMobNum == 255 || pMob->m_nMobNum == 268  )	// Lyul ???? ?? ????????
				pMob->RenderNPC_Event();
			else if ( pMob->m_nMobNum == 336)
				pMob->RenderNPC_Event(2);

			// NPC_INDEX_GOODS , NPC_INDEX_PRESENT ???? ????? ???? ???????????? ????
			else if( pMob->m_nMobNum == NPC_INDEX_MCEE )
			{
				if( g_pRoh->m_byShowGiftMessage )
					pMob->RenderNPC_Event( 1 );
			}
			else if( pMob->m_nMobNum == 380 || pMob->m_nMobNum == 6060 )
			{
				if( CommonConfig::Instance()->GetUsingLP() == true )
				{
					pMob->RenderNPC_Event( 6 );
				}
				else
				{
					pMob->RenderNPC_Event( 4 );
				}
			}
			else if ( pMob->m_nMobNum == 748 || pMob->m_nMobNum == 606 )
				pMob->RenderNPC_Event( 3 );
			else if( m_nCurWorldIdx == WORLD_MATRIX && pMob->m_nMobNum == 277 )
				pMob->RenderNPC_Event( 3 );
			else if( m_nCurWorldIdx == WORLD_DUNGEON1 && pMob->m_nMobNum == 64 ) // ?????? ???? ???????? ???????? ???????? ?????? ??????. ///????1
				pMob->RenderNPC_Event( 3 );
			else if( g_bGlobalMatrix && pMob->m_Flag&NPC_SHOPKEEPER ) ///?????????? ?? ????(BP?????? ?????? ???? ?????? SHOP ???? ????.
			{
				pMob->RenderNPC_Event( pMob->m_nMobNum==489?4:3 ); // BP ?????? BP-Shop ????...
			}
			else if( g_pRoh->m_bYanQuest && pMob->m_nMobNum == 257 && GetCurWorldID() == WORLD_START )
				pMob->RenderNPC_Event( 5 );
			else if( (g_dwClientCountry == CTRY_KOR || g_dwClientCountry == CTRY_JPN) && pMob->m_nMobNum == 321 && pMob->m_wx == 6832 && pMob->m_wz == 5110 ) // ???? 2007 ???????????? ???? ??????(???? ?????? ???????? ????...)
				pMob->RenderNPC_Event();

			if( pMob->m_byShowCrash_Miss )
				pMob->RenderCrash_Miss();
		}


		if( g_pRoh->m_byShowCrash_Miss )
			g_pRoh->RenderCrash_Miss();

		if (m_pUIMgr && g_pNk2DFrame->GetControlBottom() )
		{
			if( g_pNk2DFrame->GetControlBottom()->m_ChatOutOpt_BalloonOnOff.GetToggleState() ) // ???????? ??/?????? ?????????? ?????? ???????? ???? ????.
			{
				if (g_pRoh)
					g_pRoh->DrawChatPopup();
			}
			m_pUIMgr->DrawChatPopups();

			if (m_pUIMgr->IsAdjustingCam())
				m_pUIMgr->FixCameraPos();

			// ?????? ?????? ????????!!
			m_pUIMgr->DrawWarCastleInfo();
		}

		// prevent automouse
		if (m_pUIMgr)
			m_pUIMgr->IsVk1Pressed();


		if (g_pRoh && !g_bGhost && !m_pUIMgr->IsEventScenePlaying() ) // ?????? ?????? ?????? ?????? ?????? ??????. ///????1 ??????.
		{
			if (g_pRoh && !g_bGhost)
			{
				if(g_pRoh->m_bIsParty && g_pNk2DFrame->GetControlParty()->m_MemberCount >1)
				{
					// ???????? ?????? ?????????? "???? ????????"  "??????????????" ?? ????????????

					if((g_pRoh->m_bShowPartysVital) || (g_pNk2DFrame->IsInterfaceVisible(MAP)))
					{
						static float startTime = timeGetTime();
						float curTime = timeGetTime();
						if((curTime - startTime) > 1000)
						{
							g_pTcpIp->SendNetMessage("pv\n");
							startTime = curTime;
						}
						if(g_pRoh->m_bShowPartysVital && g_pRoh->m_bRecivedPartysInfo)
							ShowMyPartysVital();
					}
				}
				else
				{
					g_pRoh->m_bRecivedPartysInfo = FALSE;
				}
			}
			ShowRohEnergeBar();
		}

		if( DrawHappyNewYear && m_NewYear && m_NewYear->GetRenderState() )
		{
			m_NewYear->Render();
		}
		else if( DrawHappyNewYear && m_NewYear && !m_NewYear->GetRenderState() )
		{
			DeleteHappyNewYear();
		}

		//==================================================
		if( DrawBattleStart && m_BattleStart )
		{
			m_BattleStart->Render(m_nBSRendNum);
			if(!DrawBattleStart)
				DeleteBattleStart();
		}

		// ?????? ???????? ??????. 10???????????????
		{
			if( g_pNk2DFrame->GetControlGuildBattle() && g_pNk2DFrame->IsControlGuildBattle() )
				g_pNk2DFrame->GetControlGuildBattle()->Render();

			if( g_pNk2DFrame->GetGuildRankWarBattleCount() && g_pNk2DFrame->GetGuildRankWarBattleCount()->IsActive() )
				g_pNk2DFrame->GetGuildRankWarBattleCount()->Render();
		}

		if( DrawPlaceInfo && m_PlaceInfo )
		{
			m_PlaceInfo->Render();
			if(!DrawPlaceInfo)
				DeletePlaceInfo();
		}

		if (m_pd3dDevicePick && m_bPickBegin)
			m_pd3dDevicePick->EndScene();



		if( m_bRegenStart )
			m_pRegenStart->Render();


		if( m_pUIMgr->m_bRClickDown == 1
				&& g_ChaSkill.m_SkillLevel[26] > 0
				&& !m_pUIMgr->m_b_MovingForAttack
				&& DrawCasting )
		{
			m_dwCastingTime = timeGetTime() - m_dPressTime;

			int nLevelGrade = g_ChaSkill.m_SkillLevel[26];	//?????? ????
			int nHellMas_Level = g_ChaSkill.m_SkillLevel[25];	//?? ???????? ????

			if( m_dwCastingTime > 0
					&& m_dwCastingTime < 3000 - ( 30 * nHellMas_Level * 2.5)
					&& nLevelGrade > 0 )
			{
				m_nGrade = 0;
				m_nHellDown_Grade = 0;
			}

			else if( m_dwCastingTime >= 3000 - ( 30 * nHellMas_Level * 2.5)
					 && m_dwCastingTime < 6000 - ( 60 * nHellMas_Level * 2.5)
					 && nLevelGrade > 6 )
			{
				m_nGrade = 1;
				m_nHellDown_Grade = 1;
			}

			else if( m_dwCastingTime >= 6000 - ( 60 * nHellMas_Level * 2.5)
					 && m_dwCastingTime < 8000 - ( 80 * nHellMas_Level * 2.5)
					 && nLevelGrade > 12 )
			{
				m_nGrade = 2;
				m_nHellDown_Grade = 2;
			}

			else if( m_dwCastingTime >= 8000 - ( 80 * nHellMas_Level * 2.5)
					 && m_dwCastingTime < 10000 - ( 100 * nHellMas_Level * 2.5)
					 && nLevelGrade > 18 )
			{
				m_nGrade = 3;
				m_nHellDown_Grade = 3;
			}

			else if( m_dwCastingTime >= 10000 - ( 100 * nHellMas_Level * 2.5)
					 && m_dwCastingTime < 12000 - ( 120 * nHellMas_Level * 2.5)  ///07-04-10 ?????? ??????????????.?????? ?? ???????? ???????? 4?????? ????
					 && nLevelGrade > 24 )
			{
				m_nGrade = 4;
				m_nHellDown_Grade = 4;
			}


// 			else if( m_dwCastingTime >= 12000 - ( 120 * nHellMas_Level * 2.5)
// 				&& nLevelGrade > 24 )
// 			{
// 				m_nHellDown_Grade = 5;
// 			}

			//???? ???????? ?????? ???? ???????? ???? ???????? ????
			if( nLevelGrade > 0 && nLevelGrade <= 6
					&& m_dwCastingTime > 3000 - ( 30 * nHellMas_Level * 2.5) )
			{
//				m_nCurHellDown_Grade = 0;
				m_helldown_ing = false;
				m_dwCastingTime = 3000 - ( 30 * nHellMas_Level * 2.5);
			}
			else if( nLevelGrade > 6 && nLevelGrade <= 12
					 && m_dwCastingTime > 6000 - ( 60 * nHellMas_Level * 2.5 ) )
			{
//				m_nCurHellDown_Grade = 1;
				m_helldown_ing = false;
				m_dwCastingTime = 6000 - ( 60 * nHellMas_Level * 2.5);
			}
			else if( nLevelGrade > 12 && nLevelGrade <= 18
					 && m_dwCastingTime > 8000 - ( 80 * nHellMas_Level * 2.5) )
			{
//				m_nCurHellDown_Grade = 2;
				m_helldown_ing = false;
				m_dwCastingTime = 8000 - ( 80 * nHellMas_Level * 2.5);
			}
			else if( nLevelGrade > 18 && nLevelGrade <= 24
					 && m_dwCastingTime > 10000 - (100 * nHellMas_Level * 2.5) )
			{
//				m_nCurHellDown_Grade = 3;
				m_helldown_ing = false;
				m_dwCastingTime = 10000 - ( 100 * nHellMas_Level * 2.5);
			}
			else if( nLevelGrade > 24 && nLevelGrade <= 30
					 && m_dwCastingTime > 12000 - ( 120 * nHellMas_Level * 2.5) )
			{
//				m_nCurHellDown_Grade = 4;
				m_helldown_ing = false;
				m_dwCastingTime = 12000 - ( 120 * nHellMas_Level * 2.5);
			}

			//1???? ???? ???????? ?????? ???? ????
			if( m_dwCastingTime >= ( 3000 - ( 30 * nHellMas_Level * 2.5) ) )
				m_bHellDownReady = TRUE;

			if( m_bHellDownReady && m_nCurHellDown_Grade != m_nHellDown_Grade )
			{
				if(m_pHeroAttGoMob)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, m_nHellDown_Grade, 1);
				}
				else if(m_pHeroAttGoCha)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, m_nHellDown_Grade, 1);
				}
				
				m_nCurHellDown_Grade = m_nHellDown_Grade;
				m_helldown_ing = true;
			}

			if( !m_pCasting ) // ??????...?? ,???? ?????????? ???? ????.
				m_pCasting = new CFxCasting(); //  ???? ?? ???? ?????? ???? ??????? ?????? ????????... 07-03-14 ???? ????

			if( m_pCasting )
			{
				if( m_pCasting->GetTexNum() != m_nGrade )
					m_pCasting->SetTexNum( m_nGrade );

				m_pCasting->Render( m_dwCastingTime );
			}

		}


		///????????????????
		if( m_NvsnMoveDelay )
		{
			int dwTempTick =  (int)( m_NvsnMoveDelay - timeGetTime() ); // ???? ????.
			if( dwTempTick > 0 )
			{
				if(  (timeGetTime()%1000) > 450  )
				{
					char strTemp[256];

					// IDS_ACCTIME : ????.
					sprintf( strTemp, G_STRING(IDS_NVN_MOVETIMEMSG), dwTempTick/1000 );	// han_msg

					int font_size = 16;
					int len;
					len = strlen(strTemp);
					if( len )
					{
						// ???? ???????? ????.
						CTextOutBox		TempTextOut;		// ???????? ?? ?????? ???????? ??
						TempTextOut.Init(font_size, RGB(0, 0, 0), (g_pNk2DFrame->GetClientWidth()-len*(font_size/2)) / 2, g_pNk2DFrame->GetClientHeight()/2 - 100 );
						TempTextOut.SetString_withOutLine( strTemp, RGB(1, 1, 1), RGB(255, 255, 255) ); // ???? : ??, ??????, ?????? ??

						int DrawWidth = TempTextOut.m_PosX;
						int DrawHeight = TempTextOut.m_PosY;

						D3DTLVERTEX				TLVec[4];

						DWORD dwAlphaBlend, dwRenState[2];
						m_pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
						m_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
						m_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

						m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
						m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
						m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

						TLVec[0] = D3DTLVERTEX(D3DVECTOR_New( DrawWidth-20, DrawHeight+TempTextOut.GetBoxHeight()+18, 0), 1, 0xffffffff, 0, 0, 1.0f );
						TLVec[1] = D3DTLVERTEX(D3DVECTOR_New( DrawWidth-20, DrawHeight-20, 0), 1, 0xffffffff, 0, 0, 0 );
						TLVec[2] = D3DTLVERTEX(D3DVECTOR_New( DrawWidth+TempTextOut.GetBoxWidth()+20, DrawHeight-20, 0), 1, 0xffffffff, 0, 1.0f, 0 );
						TLVec[3] = D3DTLVERTEX(D3DVECTOR_New( DrawWidth+TempTextOut.GetBoxWidth()+20,  DrawHeight+TempTextOut.GetBoxHeight()+18, 0), 1, 0xffffffff, 0, 1.0f, 1.0f );

						m_pd3dDevice->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_FX_HEAL_LIGHT]) );
						m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

						m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
						m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );
						m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);


						TempTextOut.Draw();
					}
				}
			}
			else
			{
				m_NvsnMoveDelay = 0; // ?????????? ??????

				if (g_pTcpIp)
				{
					pCMyApp->m_pUIMgr->m_bNextGoto = FALSE; // ???? ???? ???? ???? ???? ????.
					g_pTcpIp->SendNetMessage( "mat_jump 1\n" );
				}
			}
		}

		m_pd3dDevice->EndScene();
	}

	if(m_pUIMgr->m_bIsScrollBox)
		m_pUIMgr->m_pScrollBox->Draw();
	if(m_pUIMgr->m_bIsDelivBox)
		m_pUIMgr->m_pDelivBox->Draw();
	if(m_pUIMgr->m_bShowPresentBox)
		m_pUIMgr->DrawAllPresent();
	if(m_pUIMgr->m_bIsChSexBox)
		m_pUIMgr->m_pChSexBox->Draw();
	if(m_pUIMgr->m_bIsChangeHair)
		m_pUIMgr->m_pChangeHair->Draw();
	if(m_pUIMgr->m_bIsWarTax)
		m_pUIMgr->m_pWarTax->Draw();
	if(m_pUIMgr->m_bIsTower)
		m_pUIMgr->m_pTower->Draw();
	if(m_pUIMgr->m_bIsWarReq)
		m_pUIMgr->m_pWarReq->Draw();
	if(m_pUIMgr->m_bIsPetShop)
		m_pUIMgr->m_pPetShop->Draw();
	if(m_pUIMgr->m_bIsPetKeep)
		m_pUIMgr->m_pPetKeep->Draw();
	if(m_pUIMgr->m_bIsGuildWar)
		m_pUIMgr->m_pGuildWar->Draw();
	if(m_pUIMgr->m_bIsMatMakeRoom)
		m_pUIMgr->m_pMatMakeRoom->Draw();
	if(m_pUIMgr->m_bIsMatList)
		m_pUIMgr->m_pMatList->Draw();
	if(m_pUIMgr->m_bShowScore)
		m_pUIMgr->ShowScore();
	if(m_pUIMgr->m_bIsMatBattle || m_pUIMgr->m_bIsMatDeath)
		m_pUIMgr->m_pBattleUI->Draw();
	if(m_pUIMgr->m_bIsMatSurvival || m_pUIMgr->m_bIsMatSurvivalMon || m_pUIMgr->m_bIsMatChat)
		m_pUIMgr->m_pSurvivalUI->Draw();
	if(m_pUIMgr->m_bIsTeleportItem)
		m_pUIMgr->m_pTeleportItem->Draw();
	if( m_pUIMgr->m_pGmMacro )
		m_pUIMgr->m_pGmMacro->Draw();
	if(m_pUIMgr->m_bIsLowGuildMaster)
		m_pUIMgr->m_pLowGuildMaster->Draw();
//=====================================================================================
	// nate 2004 - 9
	// GuildHunt
	if( m_pUIMgr->m_bIsGuildHunt )
	{
		if( m_pUIMgr->m_pGuildHunt )
		{
			m_pUIMgr->m_pGuildHunt->Draw();
		}
	}
	if( m_pUIMgr->m_bIsReservationWindow )
	{
		if( m_pUIMgr->m_pReservationWindow )
		{
			m_pUIMgr->m_pReservationWindow->Draw();
		}
	}
//=====================================================================================

	DWORD checktime = 30; // ??

	DWORD curTime = timeGetTime() - m_PcBangStartTime;
	float gage = 0;

	if( curTime >= (1000*60*checktime) )
		gage = 100.0f;
	else
		gage = 100.0f * ( (float)curTime / (float)(1000*60*checktime));

	g_pNk2DFrame->GetNewEvent()->SetPcLpTime(gage);

//==============================================================================
	if (g_pTcpIp)
		g_pTcpIp->FlushSendingBuffer();


	// Music - mp3
	// ?????? ?? ???? ( Option ?? ???????? ?????? ???? )
	if( m_pMp3Module[ 0 ] )
	{
		if( g_dwClientCountry == CTRY_JPN ) ///????BGM ???????? ?????? ??????.
		{
			if( m_ProOption.bMusicOn ) // ???? ?????? ???? ??????...
			{
				// ???? ?????? ?????? ?????????? ????. mp3?? mid?? ???????? ????.
				if( m_PlaySndType == SIEGEWAR_SOUND || m_PlaySndType == MAP1_SOUND || m_PlaySndType == MAP2_SOUND
						|| m_PlaySndType == PEACEZONE_SOUND || m_PlaySndType == WARZONE_SOUND || m_PlaySndType == MAP3_SOUND
						|| m_PlaySndType == MAP4_SOUND || m_PlaySndType == MAP0_SOUND || m_PlaySndType == MAP5_SOUND 
						|| m_PlaySndType == MAP6_SOUND || m_PlaySndType == MAP7_SOUND || m_PlaySndType == MAP8_SOUND 
						|| m_PlaySndType == MAP9_SOUND || m_PlaySndType == MAP10_SOUND || m_PlaySndType == MAP11_SOUND
						|| m_PlaySndType == MAP12_SOUND || m_PlaySndType == MAP13_SOUND || m_PlaySndType == MAP14_SOUND
						|| m_PlaySndType == MAP15_SOUND || m_PlaySndType == MAP16_SOUND || m_PlaySndType == MAP17_SOUND 
						|| m_PlaySndType == MAP18_SOUND || m_PlaySndType == MAP19_SOUND || m_PlaySndType == MAP20_SOUND
						|| m_PlaySndType == MAP21_SOUND || m_PlaySndType == MAP22_SOUND || m_PlaySndType == MAP23_SOUND 
						|| m_PlaySndType == MAP24_SOUND || m_PlaySndType == MAP25_SOUND || m_PlaySndType == MAP26_SOUND || m_PlaySndType == MAP27_SOUND
				  ) // mp3?????? ????.
				{
					if( !m_pMp3Module[ 0 ]->IsPlaying() )
						m_pMp3Module[ 0 ]->Play();
				}
				else // ???? ?????? ????.
				{
				}

				if( m_pUIMgr->m_bOnBattle )
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();


					if( m_PlaySndType != SIEGEWAR_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("kong.mp3");
						m_PlaySndType = SIEGEWAR_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FIELD)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP0_SOUND )
					{

						m_pMp3Module[ 0 ]->SwapSourceFilter("dekard.mp3");
						m_PlaySndType = MAP0_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON1 || m_nCurWorldIdx == WORLD_DUN910)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP1_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("dekaren.mp3");
						m_PlaySndType = MAP1_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON2)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP2_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("dekadun.mp3");
						m_PlaySndType = MAP2_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_GUILD)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP3_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("shilon.mp3");
						m_PlaySndType = MAP3_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_START)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP4_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("laglamia.mp3");
						m_PlaySndType = MAP4_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON_S)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP5_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("searost.mp3");
						m_PlaySndType = MAP5_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_SKY)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP6_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("paros.mp3");
						m_PlaySndType = MAP6_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_WHITEHORN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP7_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("GWH.mp3");
						m_PlaySndType = MAP7_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON4)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP8_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("genus.mp3");
						m_PlaySndType = MAP8_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DMITRON)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP9_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("dmit.mp3");
						m_PlaySndType = MAP9_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_BULKAN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP13_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("turan.mp3");
						m_PlaySndType = MAP13_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_KAILIPTON)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP14_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("balkarya.mp3");
						m_PlaySndType = MAP14_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_AIDIA)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP15_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("barsha.mp3");
						m_PlaySndType = MAP15_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_HUMAN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP16_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("queiz.mp3");
						m_PlaySndType = MAP16_SOUND;
					}
				}
			    else if( m_nCurWorldIdx == WORLD_BOSSRAID_HYBRIDER)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP17_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("beargrid.mp3");
						m_PlaySndType = MAP17_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_FINALBOSS)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP18_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("marboden.mp3");
						m_PlaySndType = MAP18_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DISPOSAL)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP20_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s1.mp3");
						m_PlaySndType = MAP20_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_VIDBLINE)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP21_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("kruma.mp3");
						m_PlaySndType = MAP21_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FORLORN_S2)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP22_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s2.mp3");
						m_PlaySndType = MAP22_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FORLORN_S3)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP23_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s3.mp3");
						m_PlaySndType = MAP23_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FORLORN_S4)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP24_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s4.mp3");
						m_PlaySndType = MAP24_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_HIGHTLAND)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP25_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("hightland.mp3");
						m_PlaySndType = MAP25_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_SPACE)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP27_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("venatus.mp3");
						m_PlaySndType = MAP27_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_TRITIAN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP26_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("laglamia.mp3");
						m_PlaySndType = MAP26_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_ROYALE)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP26_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("laglamia.mp3");
						m_PlaySndType = MAP26_SOUND;
					}
				}
			/*else if( m_nCurWorldIdx == WORLD_GUILD ) // ??????
				{
					if( m_PlaySndType != OLD_SHAILON_SOUND )
					{
						m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

						if (m_pDMusicMgr)
						{
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
							m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[3], true );
						}

						if( m_pMusicManager )
						{
							m_pMusicManager->StopAll();
							m_pMusicManager->Play( m_nBackMidi[3], true );
						}

						m_PlaySndType = OLD_SHAILON_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON_S ) // ????????
				{
					if( m_PlaySndType != OLD_SIROOST_SOUND )
					{
						m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

						if (m_pDMusicMgr)
						{
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
							m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[5], true );
						}

						if( m_pMusicManager )
						{
							m_pMusicManager->StopAll();
							m_pMusicManager->Play( m_nBackMidi[5], true );
						}

						m_PlaySndType = OLD_SIROOST_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_SKY ) // ??????
				{
					if( m_PlaySndType != OLD_PAROS_SOUND )
					{
						m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

						if (m_pDMusicMgr)
						{
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
							m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[6], true );
						}

						if( m_pMusicManager )
						{
							m_pMusicManager->StopAll();
							m_pMusicManager->Play( m_nBackMidi[6], true );
						}

						m_PlaySndType = OLD_PAROS_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FIELD ) // ????????

				{
					if( m_PlaySndType != OLD_DECARDI_SOUND )
					{
						m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

						if (m_pDMusicMgr)
						{
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
							m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[0], true );
						}

						if( m_pMusicManager )
						{
							m_pMusicManager->StopAll();
							m_pMusicManager->Play( m_nBackMidi[0], true );
						}

						m_PlaySndType = OLD_DECARDI_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON1 ) // ??????
				{
					if( m_PlaySndType != OLD_DEKAREN_SOUND )
					{
						m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

						if (m_pDMusicMgr)
						{
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
							m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[1], true );
						}

						if( m_pMusicManager )
						{
							m_pMusicManager->StopAll();
							m_pMusicManager->Play( m_nBackMidi[1], true );
						}

						m_PlaySndType = OLD_DEKAREN_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON2 ) // ??????
				{
					if( m_PlaySndType != OLD_DEKADUN_SOUND )
					{
						m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

						if (m_pDMusicMgr)
						{
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
							m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[2], true );
						}

						if( m_pMusicManager )
						{
							m_pMusicManager->StopAll();
							m_pMusicManager->Play( m_nBackMidi[2], true );
						}

						m_PlaySndType = OLD_DEKADUN_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_FINALBOSS ) //???? ???? ????
				{
					//if( !m_pMp3Module[ 0 ]->IsPlaying() )
					//	m_pMp3Module[ 0 ]->Play();
					//if( m_PlaySndType != SIEGEWAR_SOUND )
					//{
					//	m_pMp3Module[ 0 ]->SwapSourceFilter("kong.mp3");
					//	m_PlaySndType = SIEGEWAR_SOUND;
					//}
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != SIEGEWAR_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("kong.mp3");
						m_PlaySndType = SIEGEWAR_SOUND;
					}
				}*/
				else // ?????? bUseMusicForEachZone ?????? ?? ?? ????!
				{
					// ???? ?????? ???????????? ???????? ?????? ????????. ?????????? ?????????? ????...
					if( IsPeaceArea() && m_PlaySndType != PEACEZONE_SOUND )
					{
						if (m_pDMusicMgr)
							m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

						if( m_pMusicManager )
							m_pMusicManager->StopAll();

						m_pMp3Module[ 0 ]->SwapSourceFilter("town.mp3");
						m_PlaySndType = PEACEZONE_SOUND;
					}
					/*else if( !IsPeaceArea() )
					{
						if( m_nCurWorldIdx == WORLD_START && m_PlaySndType != OLD_LAGLAMIA_SOUND ) // ???????????? ???????????? ???? ??????..
						{
							m_pMp3Module[ 0 ]->Stop(); // ?????? ???????? mp3?? ??????.

							if (m_pDMusicMgr)
							{
								m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.
								m_pDMusicMgr->DirectMusicPlay( m_nBackMidi[4], true );
							}

							if( m_pMusicManager )
							{
								m_pMusicManager->StopAll();
								m_pMusicManager->Play( m_nBackMidi[4], true );
							}

							m_PlaySndType = OLD_LAGLAMIA_SOUND;
						}
						else if( m_nCurWorldIdx == WORLD_WHITEHORN && m_PlaySndType != WARZONE_SOUND )
						{
							if (m_pDMusicMgr)
								m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

							if( m_pMusicManager )
								m_pMusicManager->StopAll();

							m_pMp3Module[ 0 ]->SwapSourceFilter("battle.mp3");
							m_PlaySndType = WARZONE_SOUND;
						}
					}*/
				}
			}
		}
		else
		{
		if( m_pUIMgr->m_bOnBattle )
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();


					if( m_PlaySndType != SIEGEWAR_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("kong.mp3");
						m_PlaySndType = SIEGEWAR_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FIELD)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP0_SOUND )
					{

						m_pMp3Module[ 0 ]->SwapSourceFilter("dekard.mp3");
						m_PlaySndType = MAP0_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON1 || m_nCurWorldIdx == WORLD_DUN910)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP1_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("dekaren.mp3");
						m_PlaySndType = MAP1_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON2)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP2_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("dekadun.mp3");
						m_PlaySndType = MAP2_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_GUILD)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP3_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("shilon.mp3");
						m_PlaySndType = MAP3_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_START)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP4_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("laglamia.mp3");
						m_PlaySndType = MAP4_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON_S)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP5_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("searost.mp3");
						m_PlaySndType = MAP5_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_SKY)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP6_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("paros.mp3");
						m_PlaySndType = MAP6_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_WHITEHORN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP7_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("GWH.mp3");
						m_PlaySndType = MAP7_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DUNGEON4)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP8_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("genus.mp3");
						m_PlaySndType = MAP8_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DMITRON)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP9_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("dmit.mp3");
						m_PlaySndType = MAP9_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_BULKAN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP13_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("turan.mp3");
						m_PlaySndType = MAP13_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_KAILIPTON)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP14_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("balkarya.mp3");
						m_PlaySndType = MAP14_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_AIDIA)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP15_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("barsha.mp3");
						m_PlaySndType = MAP15_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_HUMAN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP16_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("queiz.mp3");
						m_PlaySndType = MAP16_SOUND;
					}
				}
			    else if( m_nCurWorldIdx == WORLD_BOSSRAID_HYBRIDER)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP17_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("beargrid.mp3");
						m_PlaySndType = MAP17_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_BOSSRAID_FINALBOSS)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP18_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("marboden.mp3");
						m_PlaySndType = MAP18_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_DISPOSAL)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP20_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s1.mp3");
						m_PlaySndType = MAP20_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_VIDBLINE)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP21_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("kruma.mp3");
						m_PlaySndType = MAP21_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FORLORN_S2)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP22_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s2.mp3");
						m_PlaySndType = MAP22_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FORLORN_S3)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP23_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s3.mp3");
						m_PlaySndType = MAP23_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_FORLORN_S4)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP24_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("s4.mp3");
						m_PlaySndType = MAP24_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_HIGHTLAND)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP25_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("hightland.mp3");
						m_PlaySndType = MAP25_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_SPACE)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP27_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("venatus.mp3");
						m_PlaySndType = MAP27_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_TRITIAN)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP26_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("laglamia.mp3");
						m_PlaySndType = MAP26_SOUND;
					}
				}
				else if( m_nCurWorldIdx == WORLD_ROYALE)
				{
					if (m_pDMusicMgr)
						m_pDMusicMgr->DirectMusicStop(); // ?????????? ??????.

					if( m_pMusicManager )
						m_pMusicManager->StopAll();

					if( m_PlaySndType != MAP26_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("laglamia.mp3");
						m_PlaySndType = MAP26_SOUND;
					}
				}
			else if( m_ProOption.bMusicOn && !m_ProOption.bUseMusicForEachZone )
			{
				if( !m_pMp3Module[ 0 ]->IsPlaying() )
					m_pMp3Module[ 0 ]->Play();

				if( m_pUIMgr->m_bOnBattle )
				{
					if( m_PlaySndType != SIEGEWAR_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("kong.mp3");
						m_PlaySndType = SIEGEWAR_SOUND;
					}
				}
				else
				{
					if( IsPeaceArea() && m_PlaySndType != PEACEZONE_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("town.mp3");
						m_PlaySndType = PEACEZONE_SOUND;
					}
					else if( !IsPeaceArea() && m_PlaySndType != WARZONE_SOUND )
					{
						m_pMp3Module[ 0 ]->SwapSourceFilter("battle.mp3");
						m_PlaySndType = WARZONE_SOUND;
					}
				}
			}
			else if( m_ProOption.bMusicOn )// nate 2005-08-31 : ?? ?????? ???? ???? ???? ?????? ?????? ???? ???? ????
			{
				if( m_PlaySndType != 0 )
				{
					m_pMp3Module[ 0 ]->Stop();
					m_PlaySndType = 0;
				}
			}
		}
	}



	//???? ????????	????
	if(m_pUIMgr && m_pUIMgr->m_bShowLetterMsgBox)
		m_pUIMgr->DrawLetterMsgBox();


	// ???? ???? ???? ????.(?????? ???????? ????????.)
	if( timeGetTime()-m_dwOldPlayingTime > 3600000 ) // ???????? ???????? ????????

	{
		if( m_dwPlayingTime && timeGetTime()-m_dwPlayingTime > 3600000 ) // ???? ???????? ???? ???????? ??????...
		{
			char strTemp[256];

			// IDS_AFTER_LOGIN : ???? ?? %d?????? ??????????.
			sprintf( strTemp, (char*)G_STRING(IDS_AFTER_LOGIN), (timeGetTime()-m_dwPlayingTime)/3600000 );
			g_pNk2DFrame->AddChatStr( strTemp, -1 ); // ?????????? ????????.


			if( g_dwClientCountry == CTRY_KOR ) // ?????? ???????? ???? ???? ?????? ??????????????. 07-01-24
			{
				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_TLVERTEX;
				ef_sort.Index = EFFECT_KOREA_SERVICE_LEVEL_MARK;
				ef_sort.dwColor = timeGetTime(); // ???? ???????? ????.
				pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
			}

		}
		m_dwOldPlayingTime = timeGetTime(); // ?????? ?????? ?????? ????.
	}

	return S_OK;
}

// nate
//--------------------------------------------------------------------------
// Name: AdjustViewport()
// Desc: ???????? ?????? ?? ????????.
//--------------------------------------------------------------------------
HRESULT CMyD3DApplication::AdjustViewport()
// ?????? ??????
{
	if( !m_pd3dDevice )
		return S_FALSE;

	// ?????? ????.
	D3DVIEWPORT7 vp;
	HRESULT hr;
	RECT	rcClient;

	if( FAILED( hr = m_pd3dDevice->GetViewport( &vp ) ) )
		return hr;

	GetClientRect(m_hWnd, &rcClient);

	if (m_GraphicMode == NKGRAPHIC2D)
	{
	}
	else if (m_GraphicMode == NKGRAPHIC3D)
	{
		if (g_pNk2DFrame)
		{
			vp.dwX = 0;
			vp.dwY = 0;
			vp.dwHeight = rcClient.bottom;
		}
	}

	if (FAILED(hr = m_pd3dDevice->SetViewport( &vp ))) return hr;

	if (g_pNk2DFrame->IsPVPVisible())
		g_pNk2DFrame->GetPVPWidget()->AdjustVariables(vp.dwWidth, vp.dwHeight);

	if (m_pd3dDevicePick)
	{
		if (FAILED(hr = m_pd3dDevice->GetViewport( &vp )))
			return hr;

		vp.dwHeight = vp.dwHeight / 2;
		vp.dwWidth = vp.dwWidth / 2;

		if (FAILED(hr = m_pd3dDevicePick->SetViewport( &vp )))
			return hr;
	}

	UpdateProjectMatrix();

	return S_OK;
}

//--------------------------------------------------------------------------
// Name: UpdateProjectMatrix()
// Desc: ???????? ???????? ?????? ?????????? ?????????? ???????? ?????? ????.
//       ???? ?????????? ???????? ?????????? ??????.
//--------------------------------------------------------------------------
HRESULT CMyD3DApplication::UpdateProjectMatrix()
{
	if( !m_pd3dDevice || !m_pFrustum )
		return S_FALSE;

	D3DVIEWPORT7 vp;
	HRESULT hr;

	if (FAILED(hr = m_pd3dDevice->GetViewport( &vp )))
		return hr;
	if ( vp.dwWidth <= 0 || vp.dwHeight <= 0)
		return E_FAIL;

	m_Aspect = (float) ((double) vp.dwHeight / (double) vp.dwWidth);

	D3DUtil_SetProjectionMatrix(m_matProj, m_Fov, m_Aspect, m_zNear, m_zFar );

	m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProj);
	if( m_pd3dDevicePick )
		m_pd3dDevicePick->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProj);

	if (m_GraphicMode == NKGRAPHIC3D)
	{
		if( g_bNewTerrain )
		{
			if( IsCurWorldID_BossraidZone() ) // ?????????? ???????? ???? ?????? ????????.
			{
				float fDist = 1600.0f;//;CNKObjQuadNode::GetRenderDist();
				m_pFrustum->InitObjFrustum( fDist * 2.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, fDist * 2.0f );
			}
			else if( pCMyApp->GetCurWorld() == 29 ) // ?????????? ???????? ???? ?????? ????????.
			{
				m_pFrustum->InitObjFrustum( 800.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 800.0f );
			}
			else
			{
				m_pFrustum->InitObjFrustum( 1600.0f );
				m_pFrustum->InitFrustum( m_Fov, m_Aspect, m_zNear, 1600.0f );
			}
		}
		else
			m_pFrustum->InitFrustum(m_Fov, m_Aspect, m_zNear, m_zFar);
	}

	return S_OK;
}


//---------------------------------------------------------------------------
// Name: Init2DFrame()
// Desc: Initialize scene objects.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::Init2DFrameObjects()
{
	RECT rcClient;
	g_pNk2DFrame->SetHwnd(Get_hWnd());
	GetClientRect(Get_hWnd(), &rcClient);

	if( g_pNk2DFrame && GetFramework() )
		g_pNk2DFrame->Init(rcClient.right, rcClient.bottom);

	m_p2DResMgr = new C2DResMgr();

	if( g_pNk2DFrame )
		g_pNk2DFrame->Load2DResources();

	if (m_GraphicMode == NKGRAPHIC2D && g_bRestart == FALSE)
		InitProgress(3);							// main.Init2DFrameObjects

	if (m_pUIMgr)
	{
		m_pUIMgr->InitChatPopups();
		if (m_pUIMgr->m_pWarInfo)
			m_pUIMgr->m_pWarInfo->LoadRes(rcClient.right, rcClient.bottom);
	}

	if( m_pMouse )
	{
//		m_pMouse->LoadRes(m_pddsRenderTarget, rcClient.right, rcClient.bottom);
		m_pMouse->LoadRes(rcClient.right, rcClient.bottom);
	}

	m_MsgPopUp.LoadRes();

	HDC	hDC;
	if( m_pddsRenderTarget )
	{
		if( SUCCEEDED( m_pddsRenderTarget->GetDC(&hDC) ) )
		{
			m_hFont = CreateFont(-MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72), 0,
								 0, 0,
								 FW_NORMAL,
								 FALSE,
								 FALSE,
								 FALSE,
								 DEFAULT_CHARSET,
								 OUT_DEFAULT_PRECIS,
								 CLIP_DEFAULT_PRECIS,
								 DEFAULT_QUALITY,
								 DEFAULT_PITCH,
								 "??????");

			m_pddsRenderTarget->ReleaseDC(hDC);
		}
	}

	return S_OK;
}

char *strLoadingPath[] =
{
	"interface/loading/init_load_scr1.bmp",
	"interface/loading/init_load_scr2.bmp",
	"interface/loading/init_load_scr3.bmp",
	"interface/loading/init_load_scr4.bmp",
	"interface/loading/init_load_scr5.bmp",
	"interface/loading/init_load_scr6.bmp",
	"interface/loading/init_load_scr7.bmp",
	"interface/loading/init_load_scr8.bmp",
	"interface/loading/init_load_scr9.bmp"
};

void CMyD3DApplication::InitProgress(int nFrame)
{
// 	if (!m_pLoadDisp || !m_pddsRenderTarget)
// 		return;
	if( !g_pDisplay || !m_pddsRenderTarget )
		return;

	CSurface *pSurfaceBack = NULL, *pSurface = NULL;
	HRESULT hr;

	g_pDisplay->Clear();
	g_pDisplay->CreateSurfaceFromBitmap(&pSurfaceBack, "interface/loading/init_load_scr.bmp");

	if (nFrame >=0 && nFrame < 9)
	{
		if( nFrame>0 )
			g_pDisplay->CreateSurfaceFromBitmap(&pSurface, strLoadingPath[nFrame], 0, 0);
	}
	else
	{
		SAFE_DELETE(pSurfaceBack);
		return;
	}

	m_pddsRenderTarget->BltFast(0, 0, pSurfaceBack->GetDDrawSurface(), NULL, DDBLTFAST_WAIT);

	if( nFrame > 0 && pSurface )
	{
		if( g_dwClientCountry == CTRY_JPN )
			m_pddsRenderTarget->BltFast(0, 540, pSurface->GetDDrawSurface(), NULL, DDBLTFAST_WAIT);
	}


	while (1)
	{
		if( FAILED( hr = GetFramework()->ShowFrame() ) )
		{
			if( DDERR_SURFACELOST == hr )
				(GetFramework())->RestoreSurfaces();
		}
		if( hr != DDERR_WASSTILLDRAWING )
			break;
	}

	SAFE_DELETE(pSurface);
	SAFE_DELETE(pSurfaceBack);

// 	m_pLoadDisp->Clear();
// 	m_pLoadDisp->CreateSurfaceFromBitmap(&pSurfaceBack, "interface/loading/init_load_scr.bmp");
//
// 	if (nFrame >=0 && nFrame < 9)
// 	{
// 		if( nFrame>0 )
// 			m_pLoadDisp->CreateSurfaceFromBitmap(&pSurface, strLoadingPath[nFrame], 0, 0);
// 	}
// 	else
// 	{
// 		SAFE_DELETE(pSurfaceBack);
// 		return;
// 	}
//
// 	m_pddsRenderTarget->BltFast(0, 0, pSurfaceBack->GetDDrawSurface(), NULL, DDBLTFAST_WAIT);
//
// 	if( nFrame > 0 && pSurface )
// 	{
// 		if( g_dwClientCountry == CTRY_JPN )
// 			m_pddsRenderTarget->BltFast(0, 341, pSurface->GetDDrawSurface(), NULL, DDBLTFAST_WAIT);
// 	}
//
//
// 	while (1)
// 	{
// 		if( FAILED( hr = GetFramework()->ShowFrame() ) )
// 		{
// 			if( DDERR_SURFACELOST == hr )
// 				(GetFramework())->RestoreSurfaces();
// 		}
// 		if( hr != DDERR_WASSTILLDRAWING )
// 			break;
// 	}
//
// 	SAFE_DELETE(pSurface);
// 	SAFE_DELETE(pSurfaceBack);
}

void CMyD3DApplication::InitLoadingScr()
{
	if( !m_pddsRenderTarget )
		return;

	static CSurface	*pSurfaceTest = NULL;
	char img[100] = "";

	strcpy(img, "interface/Loading/init_load_scr.bmp");

	if( g_pDisplay )
	{
		g_pDisplay->SetDirectDraw(m_pDD);
		g_pDisplay->SetBackBuffer(m_pddsRenderTarget);
		g_pDisplay->Clear();
	}

	HRESULT hr;
	while (1)
	{
		if( FAILED( hr = (GetFramework())->ShowFrame() ) )
		{
			if( DDERR_SURFACELOST == hr )
				(GetFramework())->RestoreSurfaces();
		}

		if( hr != DDERR_WASSTILLDRAWING )
			break;
	}


	if( FAILED( hr = (GetFramework())->ShowFrame() ) )
	{
		if( DDERR_SURFACELOST == hr )
			(GetFramework())->RestoreSurfaces();
	}

	SAFE_DELETE(pSurfaceTest);
}

//---------------------------------------------------------------------------
// Name: InitScreen()
// Desc: ?????? ?????? ?????? ?????? ???????? ???????? ???? ??????
//---------------------------------------------------------------------------
void CMyD3DApplication::InitScreen()
{
	if( !GetFramework() )
		return;

	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0x00FF0000;

	if( m_pddsRenderTarget )
		m_pddsRenderTarget->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
	if( GetFramework()->GetFrontBuffer() )
		GetFramework()->GetFrontBuffer()->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

	HRESULT hr;
	if( FAILED( hr = (GetFramework())->ShowFrame() ) )
	{
		if( DDERR_SURFACELOST == hr )
		{
			(GetFramework())->RestoreSurfaces();
			RestoreSurfaces();
		}
	}

	if( m_pddsRenderTarget )
		m_pddsRenderTarget->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

	if( FAILED( hr = (GetFramework())->ShowFrame() ) )
	{
		if( DDERR_SURFACELOST == hr )
		{
			(GetFramework())->RestoreSurfaces();
			RestoreSurfaces();
		}
	}
}

//---------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
	if( !m_pd3dDevice )
		return S_FALSE;

	CheckDeviceCaps();

	if( m_pDD )
	{
		DDDEVICEIDENTIFIER2 dddi[2];
		m_pDD->GetDeviceIdentifier(dddi, 0);
		LogBugTrap(dddi[0].szDescription);
	}

	if (m_bInitLoadingScr == TRUE)
	{
		if (g_pDisplay)
		{
			g_pDisplay->SetDirectDraw(m_pDD);
			g_pDisplay->SetBackBuffer(m_pddsRenderTarget);
		}
	}
	else
		m_bInitLoadingScr = TRUE;

	int i = 0;
	// Create and set up the object material
	D3DMATERIAL7 mtrl;
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	m_pd3dDevice->SetMaterial( &mtrl );

	AdjustViewport();

	// ?????? ????.
	D3DVIEWPORT7 vp;
	HRESULT hr;

	if( m_pLightCtrl )
	{
		m_pLightCtrl->LightEnable(NKLIGHT_DIRECTIONAL, TRUE);
		m_pLightCtrl->LightEnable(NKLIGHT_POINT, TRUE);
		m_pLightCtrl->LightingEnalbe(TRUE);
		m_pLightCtrl->SetAmbientLight(0x008f96b3);
	}

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);
	m_ShadeMode = D3DSHADE_GOURAUD;
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SHADEMODE, m_ShadeMode);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTFN_LINEAR );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	if (m_pLand)
	{
		m_pLand->DoModulate( m_bCanDoModulate );
		m_pLand->DoModulate2X( m_bCanDoModulate2X );
		m_pLand->RestoreDeviceObjects( m_pD3D, m_pd3dDevice );
	}

	if( g_bNewTerrain )
		CNKObjQuadNode::RestoreDeviceObjects();

	if (m_pCloudSea)
	{
//		m_pCloudSea->SetDevice(m_pd3dDevice);
		m_pCloudSea->DoModulate2X(m_bCanDoModulate2X);
	}

	if (m_pSkyPlane)
		m_pSkyPlane->SetDevice();
	if (g_pMobTexture)
		g_pMobTexture->RestoreAllTextures(m_pd3dDevice);
	if (m_pDecoTexture)
		m_pDecoTexture->RestoreAllTextures(m_pd3dDevice);
	if (g_pCapsyongTexture)
		g_pCapsyongTexture->RestoreAllTextures(m_pd3dDevice);
	if (!m_pGoodShadow)
		m_pGoodShadow = new CGoodShadow;
	if( m_pGoodShadow )
		m_pGoodShadow->Init();
	if (m_pCompas)
		m_pCompas->LoadRes();
	if (m_pUIMgr)
		m_pUIMgr->ChangeEnvironment();

	// ??????
	for (i=0; i < 8; i++)
		m_pd3dDevice->LightEnable(i, FALSE);

	if( m_pLightCtrl )
	{
		m_pLightCtrl->LightEnable(NKLIGHT_DIRECTIONAL, TRUE);
		m_pLightCtrl->InitLight(NKLIGHT_DIRECTIONAL, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		m_pLightCtrl->SetLight(NKLIGHT_DIRECTIONAL);
	}

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x00000030);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);

	if (m_pd3dDevicePick)
	{
		// ????????

		m_pd3dDevice->GetMaterial(&mtrl);
		D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
		m_pd3dDevicePick->SetMaterial(&mtrl);
		// ??????

		m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT);
		m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_DITHERENABLE,       FALSE );
		m_pd3dDevicePick->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);
		m_pd3dDevicePick->SetRenderState(D3DRENDERSTATE_COLORKEYBLENDENABLE, FALSE);
		m_pd3dDevicePick->SetRenderState(D3DRENDERSTATE_ANTIALIAS, FALSE);

		if (FAILED(hr = m_pd3dDevice->GetViewport( &vp )))
			return hr;

		vp.dwHeight = vp.dwHeight / 2;
		vp.dwWidth = vp.dwWidth / 2;

		if (FAILED(hr = m_pd3dDevicePick->SetViewport( &vp )))
			return hr;

		// ???????? ???????? ????.
		// light
		for (i = 0; i < 8; i++)
			m_pd3dDevicePick->LightEnable(i, FALSE);

		m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		// ??????????

		m_pd3dDevicePick->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
		m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);
	}

	if (m_GraphicMode == NKGRAPHIC2D && g_bRestart)
		pCMyApp->InitProgress(4);

	return S_OK;
}



//---------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
	return S_OK;
}


//---------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: ?????????????? ??????, ???? ?????? ??????
//       ?? ?????? ?????????? ???????? ???????? ??????.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
	// ???? ???????????? ?????????
	if (g_pNk2DFrame)
		g_pNk2DFrame->DeleteVoiceChat();

	if (m_pLand)
		m_pLand->InvalidateDeviceObjects();

	CNKObjQuadNode::InvalidateDeviceObjects();

	if (g_pCapsyongTexture != NULL )
		g_pCapsyongTexture->InvalidateAllTextures();

	if (g_pMobTexture != NULL)
		g_pMobTexture->InvalidateAllTextures();

	if (m_pDecoTexture != NULL)
		m_pDecoTexture->InvalidateAllTextures();

	if (g_pNk2DFrame)
		g_pNk2DFrame->DeleteObjects();

	if (m_pMouse)
		m_pMouse->DeleteRes();

	if (m_pGoodShadow)
	{
		m_pGoodShadow->DeleteRes();
		m_pGoodShadow->Invalidate();
	}

	if (m_pUIMgr)
		m_pUIMgr->DeleteRes();

	if (m_pCompas)
		m_pCompas->DeleteRes();

	// ?????? ???? ????
	DeleteObject(m_hFont);
	m_MsgPopUp.DeleteRes();
	SAFE_DELETE(m_p2DResMgr);

	if( GetFramework() )
	{
		GetFramework()->DestroyObjects();
	}

	return S_OK;
}

//---------------------------------------------------------------------------
// Name: DeleteUserObjects()
// Desc: ?????????? ?????? ???? ????????.
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteUserObjects()
{
	SAFE_DELETE(m_pLoadingBack);
	SAFE_DELETE(m_pLoadingFrame);
	SAFE_DELETE(m_pLoadingGoWorldBack);
	SAFE_DELETE(m_pLoadingGoWorldBox);
	SAFE_DELETE(m_pLoadingGoWorldFrame);
	SAFE_DELETE(m_pLoadingGoWorldText);

	SAFE_DELETE(m_pMyPet);
	SAFE_DELETE(m_pCasting);
	SAFE_DELETE(m_pLand);
	SAFE_DELETE(m_pPath);
	SAFE_DELETE(m_pFrustum);
	SAFE_DELETE(m_pHanIme);
	SAFE_DELETE(m_pCloudSea);
	SAFE_DELETE(m_pSkyPlane);
	SAFE_DELETE(g_pNk2DFrame);
	SAFE_DELETE(m_pMouse);
	SAFE_DELETE(m_pLightCtrl);
	SAFE_DELETE(g_pRoh);
	SAFE_DELETE(m_pDecoTexture);
	SAFE_DELETE(g_pCapsyongTexture);
	SAFE_DELETE(g_pMobTexture);
	SAFE_DELETE(g_pShadowRect);
	SAFE_DELETE(m_pUIMgr);
	SAFE_DELETE(m_pGoodShadow);
	SAFE_DELETE(m_pCompas);	
	SAFE_DELETE( g_pWhangLolo1 );
	SAFE_DELETE( g_pWhangLolo2 );
	SAFE_DELETE( g_pWhangLolo4 );
	if( g_MatrixSvr != 0 )
		SAFE_DELETE( g_pWhangLolo3 );

	SAFE_DELETE(m_pWeatherMgr);
	SAFE_DELETE(m_pFireFly);

	SAFE_DELETE(sky);
	SAFE_DELETE(sky_cloud);
	SAFE_DELETE(sky_fog);
	SAFE_DELETE(ground);
	SAFE_DELETE(m_pRegenStart);
	SAFE_DELETE(g_pTcpIp);
	SAFE_DELETE(m_pTcpIpAlter);

	DeletePlayerData();
	DeleteDecoObjData();
	DeletingMobLoading();
	DeletingPetLoading();

	// ???? ???????? ?????? ???????????? ?????? ?????? ?????? ???? ??????????. (by ????)
	// ???? g_ClrAlloc?? ???? ?????????? g_ClrAlloc?? ???????????? ?????? ?????? ?????? ???? ???? ?????? ????.
	SAFE_DELETE( g_pGuildGateClosed );
	SAFE_DELETE( g_pGuildGate2Opened );
	SAFE_DELETE( g_pOutsideDoorClosed );
	SAFE_DELETE( g_pCastleGateClosed );
	SAFE_DELETE( g_pBossGateClosed );
	SAFE_DELETE(g_ClrAlloc);
	SAFE_DELETE_ARRAY(m_pszMapName);

	safe_delete_associate(m_race_csy_map);

	SAFE_DELETE(m_pFxSet);

	SAFE_DELETE( m_pMusicManager );
	SAFE_DELETE(m_pDMusicMgr);
	SAFE_DELETE(g_pDSound);

	for( int i = 0; i < 4; i++ )
		SAFE_DELETE( m_pMp3Module[i] );

	SAFE_DELETE( g_pDisplay );

	g_bAdmin2 = FALSE;
	g_EventMad_Aleady = false;

	SpeedChecker::GetInstance()->Reset();

	return S_OK;
}

void CMyD3DApplication::ToggleWireframe()
{
	if( m_pd3dDevice )
	{
		m_bWireframe = !m_bWireframe;
		if (m_bWireframe)
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
		else
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
	}
}

void CMyD3DApplication::SetCameraInitPos(D3DVECTOR &vFrom, D3DVECTOR &vAt)
{
	m_vEye = vFrom;

	// ???? ???? ????.
	float ty = 0.0;
	if( m_pLand )
		ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z);

	// ???? ????.
	if (m_vEye.y < (ty + 20.0f))
		m_vEye.y = ty+20.0f;

	m_vEyeZ.x = vAt.x - m_vEye.x;
	m_vEyeZ.y = vAt.y - m_vEye.y;
	m_vEyeZ.z = vAt.z - m_vEye.z;
	m_vAt = vAt;

	m_fCamHeight = m_vEye.y - vAt.y;

	SituationEye(m_matView, m_vEye, m_vEyeZ, m_vEyeY);
}
void CMyD3DApplication::SituationEye(D3DMATRIX &m, D3DVECTOR &eye, D3DVECTOR &eye_z, D3DVECTOR &eye_y)
{
	D3DVECTOR vUp = D3DVECTOR_New(0, 1, 0);
	D3DVECTOR eye_x = CrossProduct(vUp, eye_z);
	eye_y = CrossProduct(eye_z, eye_x);

	eye_x = Normalize(eye_x);
	eye_y = Normalize(eye_y);
	eye_z = Normalize(eye_z);

	D3DUtil_SetIdentityMatrix(m);

	m._11 = eye_x.x;
	m._21 = eye_x.y;
	m._31 = eye_x.z;
	m._12 = eye_y.x;
	m._22 = eye_y.y;
	m._32 = eye_y.z;
	m._13 = eye_z.x;
	m._23 = eye_z.y;
	m._33 = eye_z.z;

	m._41 = -DotProduct(eye, eye_x);
	m._42 = -DotProduct(eye, eye_y);
	m._43 = -DotProduct(eye, eye_z);
}

void CMyD3DApplication::SetViewMatrix(D3DMATRIX &mat)
{
	SituationEye(mat, m_vEye, m_vEyeZ, m_vEyeY);
}

void CMyD3DApplication::SetSpotLight(D3DVECTOR vPos, D3DVECTOR vDir, BOOL bOn)
{
	for (int i=0; i < 3; i++)
		vPos -= vDir;
	if (bOn)
	{
		D3DLIGHT7 light;
		ZeroMemory( &light, sizeof(light) );
		light.dltType       = D3DLIGHT_SPOT;
		light.dcvDiffuse.r  = 0.5f + 0.5f;
		light.dcvDiffuse.g  = 0.5f + 0.5f;
		light.dcvDiffuse.b  = 0.5f + 0.5f;
		light.dvRange       = D3DLIGHT_RANGE_MAX;
		light.dvPosition	= vPos;
		light.dvDirection    = vDir;
		light.dvFalloff      = 100.0f;
		light.dvTheta        =   0.8f;
		light.dvPhi          =   1.0f;
		light.dvAttenuation0 =   1.0f;

		// Set the light
		m_pd3dDevice->SetLight( 15, &light );
	}
	else
		m_pd3dDevice->SetLight( 1, NULL);
}

//-------------------------------------------------------------------------
// ???????? ?????? ???? ??????

//-------------------------------------------------------------------------
void CMyD3DApplication::GoForward()
{
	float  dx, dz;
	dx = m_vEyeZ.x*m_fSpeedMul*2;
	dz = m_vEyeZ.z*m_fSpeedMul*2;

	m_vEye.x += dx;
	m_vEye.z += dz;

	m_vAt.x += dx;
	m_vAt.z += dz;
	// nate 2004 - 6
	// ?????? ???? ?????? ??????
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ?????? ???? ?????? ?????? ???????? ????.
	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;

	SetViewMatrix(m_matView);
}

void CMyD3DApplication::GoBackward()
{
	float  dx, dz;
	float  temp_y;

	dx = m_vEyeZ.x*m_fSpeedMul;
	dz = m_vEyeZ.z*m_fSpeedMul;

	temp_y = m_vEye.y;
	m_vEye.x -= dx;
	m_vEye.z -= dz;

	m_vAt.x -= dx;
	m_vAt.z -= dz;
//===================================================
	// nate 2004 - 6
	// ?????? ???? ?????? ??????
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;
//===================================================
	m_vEye.y = temp_y;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ?????? ???? ?????? ?????? ???????? ????.
	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;
	SetViewMatrix(m_matView);
}

// [11/27/2007 ??????] ?????? ???????? ???? ???? ???? ????

void CMyD3DApplication::GoRight(float right)
{
	m_vEye.x += right;

	// ?????? ???? ?????? ??????
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ?????? ???? ?????? ?????? ???????? ????.

	SetViewMatrix(m_matView);
}

void CMyD3DApplication::GoLeft(float left)
{
	m_vEye.x -= left;

	// ?????? ???? ?????? ??????
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ?????? ???? ?????? ?????? ???????? ????.

	SetViewMatrix(m_matView);
}


//-------------------------------------------------------------------------
// ???????? ?????? ????.
// ???? ???????? ???????? ?????? ?????? FrameMove ????????.
//-------------------------------------------------------------------------
void CMyD3DApplication::StartRotateCamera(int mx, int my)
{
	m_ptCurPos.x = mx;
	m_ptCurPos.y = my;
	m_bRotateCamera = TRUE;
}

void CMyD3DApplication::EndRotateCamera()
{
	m_bRotateCamera = FALSE;
}

void CMyD3DApplication::RotateCamera()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	RotateCamera(m_ptCurPos, pt.x, pt.y);

	// Save Old Position
	m_ptCurPos.x = pt.x;
	m_ptCurPos.y = pt.y;
}

void CMyD3DApplication::SpiritRotateCamera()
{
	// by evilkiki 2009.11.27 ?????????? ?????? ?????? ???????? ????!
	return;

	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	SpiritRotateCamera(m_ptCurPos, pt.x, pt.y);

	// Save Old Position
	m_ptCurPos.x = pt.x;
	m_ptCurPos.y = pt.y;
}

void CMyD3DApplication::RotateCameraPI()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	RotateCameraPI(m_ptCurPos, pt.x, pt.y);

	// Save Old Position
	m_ptCurPos.x = pt.x;
	m_ptCurPos.y = pt.y;
}

#define ROTATE_Y_START_ANG 1.0f

//--------------------------------------------------------------------------
// RotateCamera
//--------------------------------------------------------------------------
// Camera

void CMyD3DApplication::RotateCamera(POINT ptOld, int mx, int my)
{
	D3DMATRIX matRot;
	D3DVECTOR	eye_x;
	D3DVECTOR   vec;
	static BOOL bRotateY = FALSE;
	static DWORD bRotateY_InitTime;
	static float	fRotateYAng = 0.0f;		// ???? ?????????? ?????? ??.
	static float	fRotateYSpeed = 2.0f;	// ?????? ????????.
	static float    fAccKeyTime = 0.0f;		// ???? ????.
	static BOOL bRotateYRight = FALSE;
	int dx = ptOld.x - mx;
	int dy = ptOld.y - my;

	if (abs(dy) >= abs(dx)) // y???? ????

	{
		if(dy == 0)
			return;

		float fDegToRadY = dy * g_DEGTORAD;	// ?????? ????
		float fDownAngle = 0.0f;			// ???? ???? ????

		// end
		vec = D3DVECTOR_New(0.0f, 1.0f, 0.0f);
		// ?????????? ?????? ??????.
		m_vEye -= m_vAt;
		eye_x = CrossProduct(vec, m_vEyeZ);

		// wan 2004-11 : dy???? -1???? else?? ???????? ?????????? ?????????????? ?????? ???????? ?????? ????
		if (dy < 0)
		{
			if (dy < -8)
				dy = -8;

			fDownAngle = 1.48f;
			// wan 2004-11
			// atan(m_vEye.y /  sqrt(m_vEye.x*m_vEye.x+m_vEye.z*m_vEye.z)=>???? ???? ????
			// (-dy * g_DEGTORAD) => ?????? ????
			if ( atan(m_vEye.y / sqrt(m_vEye.x*m_vEye.x+m_vEye.z*m_vEye.z)) - fDegToRadY > fDownAngle)
			{
				m_vEye += m_vAt;	// ?????? ?????? ???? ????
				return;
			}
		}
		else
		{
			if (dy > 8)
				dy = 8;

			// ?????? ???? ????
			if (g_bDungeon)
			{
				if(!g_bAdmin2) //Pablo zoom test !g_bAdmin2
					fDownAngle = 0.75f;
				else
					fDownAngle = -1.5f;

				if ( atan(m_vEye.y / sqrt(m_vEye.x*m_vEye.x+m_vEye.z*m_vEye.z)) - fDegToRadY < fDownAngle)
				{
					m_vEye += m_vAt;	// ?????? ?????? ???? ????
					return;
				}
			}
			// wan 2004-11 : Whitehorn Dungeon
			// ???????? ?????? ???? ???? ?????? ?????? ??????
			else if (g_bNewTerrainDungeon)
			{
				///????1
				if( m_nCurWorldIdx == 8 ) // ?????? ??
				{
					if(!g_bAdmin2) //Pablo zoom test !g_bAdmin2
						fDownAngle = 0.35f;
					else
						fDownAngle = -1.5f;
				}
				else if( m_nCurWorldIdx == 1 ) // ?????? ????

				{
					if(!g_bAdmin2) //Pablo zoom test !g_bAdmin2
						fDownAngle = 0.25f; // ???????? ???? ???????? ??. ?????????? ?? ??????????????..
					else
						fDownAngle = -1.5f; ///????????
				}

				else if( m_nCurWorldIdx == 29 ) // ?????? ????

				{
					if(!g_bAdmin2) //Pablo zoom test !g_bAdmin2
						fDownAngle = 0.5f; // ???????? ???? ???????? ??. ?????????? ?? ??????????????..
					else
						fDownAngle = -1.5f; ///????????
				}

				if ( atan(m_vEye.y / sqrt(m_vEye.x*m_vEye.x+m_vEye.z*m_vEye.z)) - fDegToRadY < fDownAngle)
				{
					m_vEye += m_vAt;	// ?????? ?????? ???? ????
					return;
				}
			}
			else
			{
				if(!g_bAdmin2)  //Pablo zoom test  !g_bAdmin2
					fDownAngle = -0.5f;
				else
					fDownAngle = -1.5f;

				if ( atan(m_vEye.y / sqrt(m_vEye.x*m_vEye.x+m_vEye.z*m_vEye.z)) - fDegToRadY < fDownAngle)
				{
					m_vEye += m_vAt;	// ?????? ?????? ???? ????
					return;
				}
			}
		}
		D3DUtil_SetRotationMatrix(matRot, eye_x, fDegToRadY);
		D3DMath_VectorMatrixMultiply(m_vEye, m_vEye, matRot);
		m_vEye += m_vAt;	// ?????? ?????? ???? ????
		// ?????? ?????????? ????
		bRotateY = FALSE;
	}
	else
	{
		float fDegToRadX = - ( dx * g_DEGTORAD );	// ?????? ( (-)?? X???? ???????? )
		if (dx < 0)
		{
			if (dx <= -25)
				dx = -25;

			Nk3dMath_VectorRotateYByVertex(m_vEye, m_vAt, fDegToRadX);
		}
		else if ( dx > 0)
		{
			if (dx > 25)
				dx = 25;

			Nk3dMath_VectorRotateYByVertex(m_vEye, m_vAt, fDegToRadX);
		}
		// ?????? ??????????..
		bRotateY = TRUE;
	}

	// wan:2004-11 : Whitehorn Dungeon
	// ?????? ???????????? ?????? ???????? ???????? ?????? ??????????????...
	float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z) + 5.0f;

	if (m_vEye.y < ty)
		m_vEye.y = ty;

	m_vEyeZ = m_vAt - m_vEye;	// ?????????? ????

	m_fCamHeight = m_vEye.y - m_vAt.y;

	SetViewMatrix(m_matView);

	// ?????? ??????????..
	if (bRotateY)
		RotateCloud();
}

void CMyD3DApplication::RotateCameraPI(POINT ptOld, int mx, int my)
{
	D3DMATRIX matRot;
	D3DVECTOR	eye_x;
	D3DVECTOR   vec;
	int dx = ptOld.x - mx;
	int dy = ptOld.y - my;

	float fDegToRadX = -g_PI;	// ?????? ( (-)?? X???? ???????? )
	Nk3dMath_VectorRotateYByVertex(m_vEye, m_vAt, fDegToRadX);

	// wan:2004-11 : Whitehorn Dungeon
	// ?????? ???????????? ?????? ???????? ???????? ?????? ??????????????...
	float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z) + 5.0f;

	if (m_vEye.y < ty)
		m_vEye.y = ty;

	m_vEyeZ = m_vAt - m_vEye;	// ?????????? ????

	m_fCamHeight = m_vEye.y - m_vAt.y;

	SetViewMatrix(m_matView);
	RotateCloud();
}

void CMyD3DApplication::RotateRight()
{
	Nk3dMath_VectorRotateYByVertex(m_vEye, m_vAt, 2.5f*g_DEGTORAD);
	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;
	SetViewMatrix(m_matView);
}

void CMyD3DApplication::RotateLeft()
{
	Nk3dMath_VectorRotateYByVertex(m_vEye, m_vAt, -2.5f*g_DEGTORAD);
	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;
	SetViewMatrix(m_matView);
}

void CMyD3DApplication::ToggleSatelliteMode()
{
	static float oldZFar;
	m_bSatelliteMode = !m_bSatelliteMode;

	if (m_bSatelliteMode)
	{
		oldZFar = m_zFar;
		m_zFar = 6000;
		UpdateProjectMatrix();
	}
	else
	{
		m_zFar = oldZFar;
		UpdateProjectMatrix();
	}
}

void CMyD3DApplication::ForwardCamDirection()
{
	if(DistVecToVec(m_vEye, m_vAt) < 50.0f) 
	{
		if(!g_bAdmin2)  //Arnold zoom test  !g_bAdmin2
			return;
		if (g_dwClientCountry == CTRY_TH
				|| g_dwClientCountry == CTRY_TEN
				|| g_dwClientCountry == CTRY_ENG
				|| g_dwClientCountry == CTRY_DENG
				|| g_dwClientCountry == CTRY_DEU
				||g_dwClientCountry == CTRY_MENG)
		{
			if (m_Fov < 0.50000)
				return;
		}
		else
		{
			if(!g_bAdmin2) //ZOOM ARNOLD   (!g_bAdmin2)
			{
				if (m_Fov < 0.34906)
					return;
			}
		}
		m_Fov -= 0.0349f;
		UpdateProjectMatrix();
	}
	else
	{
		m_vEye.x += (m_vEyeZ.x*m_fSpeedMul*2);
		m_vEye.y += (m_vEyeZ.y*m_fSpeedMul*2);
		m_vEye.z += (m_vEyeZ.z*m_fSpeedMul*2);
		m_fCamHeight = m_vEye.y - m_vAt.y;
		SetViewMatrix(m_matView);
	}
}

void CMyD3DApplication::BackwardCamDirection()
{
	float fBackwardDist;

	if (g_bDungeon && m_pLand)
	{
		float fAtHeight = m_pLand->GetHFHeight(m_vAt.x, m_vAt.z);

		if( !g_bAdmin2 )//if(0) //( !g_bAdmin2 ) Arnold
		{
			if (m_Fov < 0.75f)  			// 0.7853?? ?? 45??.
			{
				m_Fov += 0.0349f;
				UpdateProjectMatrix();
				return;
			}
// /*
//===============================================================
			// nate 2004 - 6
			// ?????? ???????? ?????? ?? ???? ???? ????
			else if (Dist3PTo3P(m_vEye.x, m_vEye.y, m_vEye.z, m_vAt.x, fAtHeight, m_vAt.z) >= 220)  //pablo +100		//?????? 150??????.
			{
				return;
			}
//===============================================================
// */
			else
			{
				m_vEye.x -= (m_vEyeZ.x*m_fSpeedMul*2);
				m_vEye.y -= (m_vEyeZ.y*m_fSpeedMul*2);
				m_vEye.z -= (m_vEyeZ.z*m_fSpeedMul*2);
				m_fCamHeight = m_vEye.y - m_vAt.y;

			}
		}
		else
		{
			if (m_Fov < 0.75f)  			// 0.7853?? ?? 45??.
			{
				m_Fov += 0.0349f;
				UpdateProjectMatrix();
				return;
			}
			else
			{
				m_vEye.x -= (m_vEyeZ.x*m_fSpeedMul*2);
				m_vEye.y -= (m_vEyeZ.y*m_fSpeedMul*2);
				m_vEye.z -= (m_vEyeZ.z*m_fSpeedMul*2);
				m_fCamHeight = m_vEye.y - m_vAt.y;

			}
		}
	}
	else
	{
		if (GetCurWorld() == WORLD_DUN910)
		{
			if (g_dwClientCountry == 1 || g_dwLangType == 1)
				fBackwardDist = 230.0f; // pablo zoom -100
			else if (g_dwClientCountry == 3 || g_dwClientCountry == 4 || g_dwLangType == 3 || g_dwLangType == 4)
				fBackwardDist = 200.0f; // pablo zoom -100
			else if (g_dwClientCountry == 0 || g_dwLangType == 0)
				fBackwardDist = 230.0f; // pablo zoom -100
			else if (g_dwClientCountry == CTRY_TH || g_dwClientCountry == CTRY_TEN)
				fBackwardDist = 160.0f; // pablo zoom -100
			else
				fBackwardDist = 230.0f; // pablo zoom -100
		}
		else
		{
			if (g_dwClientCountry == 1 || g_dwLangType == 1)
				fBackwardDist = 380.0f; // pablo zoom +200
			else if (g_dwClientCountry == 3 || g_dwClientCountry == 4 || g_dwLangType == 3 || g_dwLangType == 4)
				fBackwardDist = 330.0f; // pablo zoom +200
			else if (g_dwClientCountry == 0 || g_dwLangType == 0)
				fBackwardDist = 380.0f; // pablo zoom +200
			else if (g_dwClientCountry == CTRY_TH || g_dwClientCountry == CTRY_ENG || g_dwClientCountry == CTRY_TEN)
				fBackwardDist = 320.0f; // pablo zoom +200
			else if (g_dwClientCountry == CTRY_DEU|| g_dwClientCountry == CTRY_DENG || g_dwClientCountry == CTRY_MENG)
				fBackwardDist = 360.0f; // pablo zoom +200
			else
				fBackwardDist = 380.0f;
		}

	if( !g_bAdmin2 )//	if(0)
		{
			if (m_Fov < 0.75f)  			// 0.7853?? ?? 45??.
			{
				m_Fov += 0.0349f;
				UpdateProjectMatrix();
			}
// /*
//===============================================================
			// nate 2004 - 6
			// ?????? ???????? ?????? ?? ???? ???? ????
			else if (Dist3PTo3P(m_vEye.x, m_vEye.y, m_vEye.z, m_vAt.x, m_vAt.y, m_vAt.z) >= fBackwardDist)	// ?????? 300??????.
				return;
//===============================================================
// */
			else
			{
				m_vEye.x -= (m_vEyeZ.x*m_fSpeedMul*2);
				m_vEye.y -= (m_vEyeZ.y*m_fSpeedMul*2);
				m_vEye.z -= (m_vEyeZ.z*m_fSpeedMul*2);
				m_fCamHeight = m_vEye.y - m_vAt.y;
				UpdateProjectMatrix();
			}
		}
		else
		{
			if (m_Fov < 0.75f)  			// 0.7853?? ?? 45??.
			{
				m_Fov += 0.0349f;
				UpdateProjectMatrix();
			}
			///???????? // ???? ?????? ???? ???????? ???? ?????? ????????.
//			else if (Dist3PTo3P(m_vEye.x, m_vEye.y, m_vEye.z, m_vAt.x, m_vAt.y, m_vAt.z) >= fBackwardDist)	// ?????? 300??????.
//				return;
			else
			{
				m_vEye.x -= (m_vEyeZ.x*m_fSpeedMul*2);
				m_vEye.y -= (m_vEyeZ.y*m_fSpeedMul*2);
				m_vEye.z -= (m_vEyeZ.z*m_fSpeedMul*2);
				m_fCamHeight = m_vEye.y - m_vAt.y;

			}
		}
		float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z) + 5.0f;

		if (m_vEye.y < ty)
			m_vEye.y = ty;
	}
	m_vEyeZ = m_vAt - m_vEye;	// ?????????? ????

	SetViewMatrix(m_matView);
}

// ---------------------------------------------------------------------
// SetHeroInitPos()
// Desc : ?????????? ?????? ???? ?????????? ?????? ?????? ?????? ?????? ???????? ??????.
// ---------------------------------------------------------------------
void CMyD3DApplication::SetHeroInitPos(float x, float y, float z, float dir, BOOL bInit)
{
	static BOOL bFirst = TRUE;
	D3DVECTOR from, at, vDiff;
	m_bBeforeTrans = FALSE;

	// bInit means initialize angle and view
	if (g_pRoh && bInit)
	{
		g_pRoh->m_wx = x;
		g_pRoh->m_wy = GetLandPtr()->GetHFHeight(x, z);
		g_pRoh->m_wz = z;
		g_pRoh->SetTargetPos(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		g_pRoh->m_dir = dir;
		g_pRoh->m_toDir = dir;
		g_pRoh->m_bCrystalOwner = FALSE;

		if (g_bDungeon)
		{
			from = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy + 90.0f, g_pRoh->m_wz-90.0f);
			at = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy+m_fAtHeight, g_pRoh->m_wz);
		}
		else
		{
			// ???? ?????? ???????????? ????.
			if (bFirst)
			{
				from = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy + 70.0f, g_pRoh->m_wz-100.0f);
				at = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy+m_fAtHeight, g_pRoh->m_wz);
				m_Fov = 0.45f;
				g_pRoh->m_dir = g_pRoh->m_toDir = g_PI+g_PI_DIV_2;
				bFirst = FALSE;
			}
			else if (g_dwClientCountry == 0 || g_dwLangType == 0)
			{
				from = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy + 50.0f, g_pRoh->m_wz-100.0f);
				at = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy+m_fAtHeight, g_pRoh->m_wz);
			}
			else if (g_dwClientCountry == 1)
			{
				from = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy + 50.0f, g_pRoh->m_wz-100.0f);
				at = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy+m_fAtHeight, g_pRoh->m_wz);
			}
			else
			{
				from = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy + 50.0f, g_pRoh->m_wz-100.0f);
				at = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy+m_fAtHeight, g_pRoh->m_wz);
			}
		}
	}
	else if (g_pRoh && bInit == FALSE)
	{
		// binit(false) means that change only position.
		float diff_x, diff_y, diff_z;
		diff_x = x - g_pRoh->m_wx;
		diff_z = z - g_pRoh->m_wz;
//--------------------------------------------------------------------------------------------
		//wan:2005-01	???????? ?????????????? ???? ????
		if( g_bNewTerrain )
		{
			float f_LandHeight = GetLandPtr()->GetHFHeight( x, z );
			float f_ObjHeight = CNKObjQuadNode::GetHeightOnObject( z, x, y );
			if( f_ObjHeight < f_LandHeight )
			{
				diff_y = f_LandHeight - g_pRoh->m_wy;
				g_pRoh->m_wy = f_LandHeight;
			}
			else
			{
				diff_y = f_ObjHeight - g_pRoh->m_wy;
				g_pRoh->m_wy = f_ObjHeight;
			}
		}
		else
		{
			diff_y = GetLandPtr()->GetHFHeight(x, z) - GetLandPtr()->GetHFHeight(g_pRoh->m_wx, g_pRoh->m_wz);
			g_pRoh->m_wy = GetLandPtr()->GetHFHeight(x, z);
		}
//--------------------------------------------------------------------------------------------
		g_pRoh->m_wx = x;
		g_pRoh->m_wz = z;
		g_pRoh->SetTargetPos(g_pRoh->m_wx, g_pRoh->m_wy, g_pRoh->m_wz);
		g_pRoh->m_dir = dir;
		g_pRoh->m_toDir = dir;
		g_pRoh->m_bCrystalOwner = FALSE;

		from = D3DVECTOR_New(m_vEye.x + diff_x, m_vEye.y + diff_y , m_vEye.z + diff_z);
		float	fWaterHeight;
		if( g_bNewTerrain )
			fWaterHeight = CNKObjQuadNode::GetHeightOnWater( g_pRoh->m_wz, g_pRoh->m_wx );
		else
			fWaterHeight = WATER_HEIGHT;
		if( g_pRoh->m_wy < fWaterHeight )
		{
			at = D3DVECTOR_New( g_pRoh->m_wx, fWaterHeight+m_fAtHeight, g_pRoh->m_wz );
			from.y = at.y + m_fCamHeight;
		}
		else
			at = D3DVECTOR_New( g_pRoh->m_wx, g_pRoh->m_wy+m_fAtHeight, g_pRoh->m_wz );
	}
	// ?????? ???? ????
	SetCameraInitPos(from, at);
}

#define SIGN(a)		((a) >= 0 ? 1 : -1)

void CMyD3DApplication::FindHeroPath(float &tx, float &tz, BOOL bWaypointMove)
{
	static int nWaypoint = 0;
	float wx = g_pRoh->m_wx;
	float wz = g_pRoh->m_wz;
	float x_diff = ABS(tx-wx);
	float z_diff = ABS(tz-wz);
	int sign_x = SIGN(tx-wx);
	int sign_z = SIGN(tz-wz);

	float dx, dz;
	float ix, iz;
	int add;
	int diff_count = 0;
	BOOL bEnd = FALSE;
	if (x_diff > z_diff)
	{
		dx = 10.0f;
		dz = 10.0f * z_diff / x_diff * sign_z;
		if (sign_x > 0)
		{
			ix = wx;
			iz = wz;
			while (ix <= tx-dx && diff_count < 5)
			{
				ix += dx;
				iz += dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix, iz+add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz+add))
							{
								//ix -= dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz+add)) > 10 &&
									 m_pLand->IsMovableArea(ix-dx, iz+add) &&
									 !IsBlockedPath(wx, wz, ix-dx, iz+add))
							{
								ix -= dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pLand->IsMovableArea(ix, iz-add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz-add))
							{
								//ix -= dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz-add)) > 10 &&
									 m_pLand->IsMovableArea(ix-dx, iz-add) &&
									 !IsBlockedPath(wx, wz, ix-dx, iz-add))
							{
								ix -= dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
		else
		{
			ix = wx;
			iz = wz;
			while (ix >= tx+dx && diff_count < 5)
			{
				ix -= dx;
				iz += dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix, iz+add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz+add))
							{
								//ix += dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz+add)) > 10 &&
									 m_pLand->IsMovableArea(ix+dx, iz+add) &&
									 !IsBlockedPath(wx, wz, ix+dx, iz+add))
							{
								ix += dx;
								iz += add;
								bEnd = TRUE;
								break;
							}

						}
						if (m_pLand->IsMovableArea(ix, iz-add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz-add))
							{
								//ix += dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz-add)) > 10 &&
									 m_pLand->IsMovableArea(ix+dx, iz-add) &&
									 !IsBlockedPath(wx, wz, ix+dx, iz-add))
							{
								ix += dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
	}
	else
	{
		dx = 10.0f * x_diff / z_diff * sign_x;
		dz = 10.0f;
		if (sign_z > 0)
		{
			ix = wx;
			iz = wz;
			while (iz <= tz-dz && diff_count < 5)
			{
				ix += dx;
				iz += dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix+add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix+add, iz))
							{
								ix += add;
								//iz -= dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix+add)) > 10 &&
									 m_pLand->IsMovableArea(ix+add, iz-dz) &&
									 !IsBlockedPath(wx, wz, ix+add, iz-dz))
							{
								ix += add;
								iz -= dz;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pLand->IsMovableArea(ix-add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix-add, iz))
							{
								ix -= add;
								//iz -= dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix-add)) > 10 &&
									 m_pLand->IsMovableArea(ix-add, iz-dz) &&
									 !IsBlockedPath(wx, wz, ix-add, iz-dz))
							{
								ix -= add;
								iz -= dz;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
		else
		{
			ix = wx;
			iz = wz;
			while (iz >= tz+dz && diff_count < 5)
			{
				ix += dx;
				iz -= dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix+add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix+add, iz))
							{
								ix += add;
								//iz += dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix+add)) > 10 &&
									 m_pLand->IsMovableArea(ix+add, iz+dz) &&
									 !IsBlockedPath(wx, wz, ix+add, iz+dz))
							{
								ix += add;
								iz += dz;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pLand->IsMovableArea(ix-add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix-add, iz))
							{
								ix -= add;
								//iz += dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix-add)) > 10 &&
									 m_pLand->IsMovableArea(ix-add, iz+dz) &&
									 !IsBlockedPath(wx, wz, ix-add, iz+dz))
							{
								ix -= add;
								iz += dz;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
	}

	if (bWaypointMove)
		nWaypoint++;
	else
		nWaypoint = 0;

	if (bEnd && nWaypoint < 7)
	{
		m_fHeroPx = tx;
		m_fHeroPz = tz;
		tx = ix;
		tz = iz;
	}
	else
	{
		m_fHeroPx = 0.0f;
		m_fHeroPz = 0.0f;
		nWaypoint = 0;
	}
}
//--------------------------------------------------------------------------
// DashHero
//--------------------------------------------------------------------------
void CMyD3DApplication::DashHero(float dx, float dz)
{
	if( !m_pLand )
		return;

	m_vAt.x += dx;
	m_vAt.y= m_pLand->GetHFHeight(m_vAt.x, m_vAt.z);
	m_vAt.z += dz;

	m_vEye.x += dx;
	m_vEye.z += dz;

	// ???????? ?????????? ???????? ????????.
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;

	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ???? ?????? ???? ??????.

	// ???????? ????.
	float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z);
	// ???? ????.
	if (m_vEye.y < (ty + 20.0f))
	{
		m_vEye.y = ty+20.0f;
		m_fCamHeight = m_vEye.y - m_vAt.y;
	}

	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;

	SetViewMatrix(m_matView);
}

// ---------------------------------------------------------------------
// MoveHero()
// Desc : ?????????????? ?????? ???????? ???????? ???? ???? ?????? ?????? ????

//  ???? ?????????? ?????????? ??????.
//  tx, tz : ???? ???? ????.
// ---------------------------------------------------------------------
BOOL CMyD3DApplication::MoveHero(float tx, float tz, BOOL bWaypointMove)
{
	if( !m_pLand || !g_pRoh )
		return false;

	float mag;
	int dY, dir;

	// ????
	if(g_pRoh->m_CancelState == 1)
	{
		if( g_pRoh->m_ChaCondition==CONDITION_STONY || g_pRoh->m_ChaCondition==CONDITION_ENTANGLE )
		{
			StopHero();
			return TRUE;
		}
	}

	// Path?? WayPoint?? ????????.
	FindHeroPath(tx, tz, bWaypointMove);

	// ?????? ?????? ????????.
	// ???????? ????
	m_fHeroTx = tx;
	m_fHeroTz = tz;
	m_fHeroDx = m_fHeroTx - g_pRoh->m_wx;
	m_fHeroDz = m_fHeroTz - g_pRoh->m_wz;

	// ???? Motion ?? ????????.
	if (g_pRoh->m_bRun && (g_pRoh->m_Stamina >= 2 || (GetCurWorld() == WORLD_SKY && !m_pUIMgr->m_pLand->IsDrawTile(g_pRoh->m_wx, g_pRoh->m_wz))))
	{
		if (!m_bRohRunning)
		{
			m_bRohRunning = TRUE;
			m_startRunningTime = timeGetTime();
		}
		g_pRoh->SetMotionState(CHA_RUN);
	}
	else
	{
		g_pRoh->SetMotionState(CHA_WALK);
	}

	mag = m_fHeroDx*m_fHeroDx + m_fHeroDz*m_fHeroDz;
	mag = (float) sqrt( (double) mag );

	if (mag < g_EPSILON)
	{
		g_pRoh->SetMotionState(CHA_STAND);
		m_bHeroMoving = FALSE;
		return FALSE;
	}

	g_pRoh->m_toDir = (float) atan2(m_fHeroDz, m_fHeroDx);
	m_fHeroDx = m_fHeroDx / mag;
	m_fHeroDz = m_fHeroDz / mag;

	// 1?????????? ???????? ????.
	if (m_bRohRunning)
	{
		if (GetCurWorld() == WORLD_SKY && m_pUIMgr->m_pLand->IsDrawTile(g_pRoh->m_wx, g_pRoh->m_wz) )
		{
			// 20???????? 40 ????
			m_fHeroDx *= 2.0f + ( 2.0f * m_fMoveValue );
			m_fHeroDz *= 2.0f + ( 2.0f * m_fMoveValue );
		}
		else
		{
			// 20?????? ???? 50?? ????????.
			m_fHeroDx *= 2.5f + ( 2.5f * m_fMoveValue );
			m_fHeroDz *= 2.5f + ( 2.5f * m_fMoveValue );
			
			if( m_pUIMgr->m_bWorldCupDash )
			{
				m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
				m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
			}
		}
	}
	else
	{
		if (GetCurWorld() == WORLD_SKY)
		{
			// 20 ???????? 30 ????
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
		else
		{
			// 20?????? ???? 30?? ????????.
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
	}

	// 1?????? ?????? ????????.
	m_fHeroIx = g_pRoh->m_wx + m_fHeroDx * 20.0f; // 20?????????? ???????? ????.
	m_fHeroIz = g_pRoh->m_wz + m_fHeroDz * 20.0f;

	// 1?????? ???????? ???????? ???????? ????????.
	// ???????? ???????? ??????, ???? ????????..
	// ???????? ??????.. ?????? ?????????? ?????? ????.
	// ?????? ?????? ?????? ?????? ??????.
	float fx, fz, fmx, fmz;
	// ?????? ????.
	fx = m_vAt.x;
	fz = m_vAt.z;
	// 1???? ???? ????.
	if (m_bRohRunning)
	{
		fmx = m_fHeroDx * 4;	// 4 * 2.5 = 10 => 1????.
		fmz = m_fHeroDz * 4;
	}
	else
	{
		fmx = m_fHeroDx * 6.6666f;	// 6.6 * 1.5 = 9.9 ?? 1????.
		fmz = m_fHeroDz * 6.6666f;
	}

	int i = 4;
	if (m_bInsideBuilding)
	{
		if (!m_pLand->IsMovableArea(fx+fmx/2, fz+fmz/2))
			i = 0;
		else if (!m_pLand->IsMovableArea(fx+fmx, fz+fmz))
			i = 0;
		else if (!m_pLand->IsMovableArea(fx+fmx*2, fz+fmz*2))
			i = 0;
		else if (!m_pLand->IsMovableArea(fx+fmx*3, fz+fmz*3))
			i = 1;
		else if (!m_pLand->IsMovableArea(fx+fmx*4, fz+fmz*4))
			i = 2;
		else if (!m_pLand->IsMovableArea(fx+fmx*5, fz+fmz*5))
			i = 3;
	}
	else
	{
		if (!m_pLand->IsMovableArea(fx+fmx/2, fz+fmz/2))
			i = 0;
		else if (!m_pLand->IsMovableArea(fx+fmx, fz+fmz))
			i = 0;
		else if (!m_pLand->IsMovableArea(fx+fmx*2, fz+fmz*2))
			i = 1;
		else if (!m_pLand->IsMovableArea(fx+fmx*3, fz+fmz*3))
			i = 2;
		else if (!m_pLand->IsMovableArea(fx+fmx*4, fz+fmz*4))
			i = 3;
		else if (!m_pLand->IsMovableArea(fx+fmx*5, fz+fmz*5))
			i = 4;
	}

	if ( i < 4 )
	{
		m_fHeroTx = fx + fmx*i;
		m_fHeroTz = fz + fmz*i;
		m_bMoreToGo = FALSE;

		// 2004 check later
		if( g_bNewTerrain )
		{
			int	nY = (int)m_pLand->GetHFHeight( m_fHeroTx, m_fHeroTz );
			dY = (int)CNKObjQuadNode::GetHeightOnObject( m_fHeroTz, m_fHeroTx, g_pRoh->m_wy );
			if( dY < nY )
			{
				dY = nY;
			}
		}
		else
		{
			dY = (int) GetLandPtr()->GetHFHeight(m_fHeroTx, m_fHeroTz);
		}

		dir = (int) (g_pRoh->m_toDir*10000);

		if( m_fHeroTx <=0 || m_fHeroTz <= 0 )
		{
			OutputDebugString( "main::MoveHero\n" );
			return FALSE;
		}

		sprintf(g_commOut, "%s %d %d %d %d %d %d", "place", (int) m_fHeroTx, (int) m_fHeroTz, dY, dir, i*5, (m_bRohRunning?1:0));
		strcat(g_commOut, "\n");
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(g_commOut);

		m_bHeroMoving = TRUE;
		m_dwPrevFrameTime = timeGetTime();

		return TRUE;
	}


	// 1?????? ?????? ???? ???? ?? ???? ?????? ?? ???????? ????.
	if (fabs(m_fHeroDx) > fabs(m_fHeroDz))
	{
		if ( ( (m_fHeroIx-g_pRoh->m_wx)/m_fHeroDx) < ( (m_fHeroTx-g_pRoh->m_wx)/m_fHeroDx) )
			m_bMoreToGo = TRUE;
		else
			m_bMoreToGo = FALSE;
	}
	else
	{
		if ( ( (m_fHeroIz-g_pRoh->m_wz)/m_fHeroDz) < ( (m_fHeroTz-g_pRoh->m_wz)/m_fHeroDz) )
			m_bMoreToGo = TRUE;
		else
			m_bMoreToGo = FALSE;
	}

	if (g_pTcpIp)
	{
		if (m_bMoreToGo)
		{
			if (m_bBeforeTrans != TRUE)
			{
				if( g_bNewTerrain )
				{
					int	nY = (int)m_pLand->GetHFHeight( m_fHeroIx, m_fHeroIz );
					dY = (int)CNKObjQuadNode::GetHeightOnObject( m_fHeroIz, m_fHeroIx, g_pRoh->m_wy );
					if( dY < nY )
					{
						dY = nY;
					}
				}
				else
				{
					dY = (int) GetLandPtr()->GetHFHeight(m_fHeroIx, m_fHeroIz);
				}

				sprintf(g_commOut, "%s %d %d %d %d", "walk", (int) m_fHeroIx, (int) m_fHeroIz, dY, (m_bRohRunning?1:0));
				strcat(g_commOut, "\n");
				g_pTcpIp->SendNetMessage(g_commOut);

				if(g_MatrixSvr != 0 && !DrawPlaceInfo)
				{
					int newPlaceNum = PlaceInfo(m_fHeroIx, m_fHeroIz);

					if(newPlaceNum != m_nCurPlaceNum)
					{
						m_nCurPlaceNum = newPlaceNum;

						if(m_nCurPlaceNum)
						{
							CreatePlaceInfo(m_nCurPlaceNum);
						}
					}
				}
			}
		}
		else
		{
			int nRemainFrameX, nRemainFrameZ;
			// ???? ???????? ???? ?? ???? ???????? ?? ?? ?? ?? ???? ??????.
			nRemainFrameX = (int) ((m_fHeroTx-g_pRoh->m_wx) / m_fHeroDx);
			nRemainFrameZ = (int) ((m_fHeroTz-g_pRoh->m_wz) / m_fHeroDz);
			// 2004 check later
			if( g_bNewTerrain )
			{
				int	nY = (int)m_pLand->GetHFHeight( m_fHeroTx, m_fHeroTz );
				dY = (int)CNKObjQuadNode::GetHeightOnObject( m_fHeroTz, m_fHeroTx, g_pRoh->m_wy );
				if( dY < nY )
					dY = nY;
			}
			else
			{
				dY = (int) GetLandPtr()->GetHFHeight(m_fHeroTx, m_fHeroTz);
			}

			dir = (int) (g_pRoh->m_toDir*10000);
			if (m_bBeforeTrans != TRUE)
			{
				if( m_fHeroTx <=0 || m_fHeroTz <= 0 )
				{
					OutputDebugString( "main::MoveHero2\n" );
					return FALSE;
				}

				if (nRemainFrameX > nRemainFrameZ )
					sprintf(g_commOut, "%s %d %d %d %d %d %d", "place", (int) m_fHeroTx, (int) m_fHeroTz, dY, dir, nRemainFrameX, (m_bRohRunning?1:0));
				else
					sprintf(g_commOut, "%s %d %d %d %d %d %d", "place", (int) m_fHeroTx, (int) m_fHeroTz, dY, dir, nRemainFrameZ, (m_bRohRunning?1:0));

				strcat(g_commOut, "\n");
				if( g_pTcpIp )
				{
					g_pTcpIp->SendNetMessage(g_commOut);
				}
			}
		}
	}
	m_bHeroMoving = TRUE;
	m_dwPrevFrameTime = timeGetTime();

	return TRUE;
}



BOOL CMyD3DApplication::IsBlockedPath(float wx, float wz, float tx, float tz)
{
	float mag = 0.0f;
	m_fHeroDx = tx - wx;
	m_fHeroDz = tz - wz;

	mag = m_fHeroDx*m_fHeroDx + m_fHeroDz*m_fHeroDz;
	mag = (float) sqrt( (double) mag );

	if (mag < g_EPSILON)
		return FALSE;

	m_fHeroDx = m_fHeroDx / mag;
	m_fHeroDz = m_fHeroDz / mag;

	// 1?????????? ???????? ????.
	if (m_bRohRunning)
	{
		if (GetCurWorld() == WORLD_SKY &&
				m_pUIMgr->m_pLand->IsDrawTile(g_pRoh->m_wx, g_pRoh->m_wz))  	// 20???????? 40 ????
		{
			m_fHeroDx *= 2.0f + ( 2.0f * m_fMoveValue );
			m_fHeroDz *= 2.0f + ( 2.0f * m_fMoveValue );
		}
		else  					// 20?????? ???? 50?? ????????.
		{
			m_fHeroDx *= 2.5f + ( 2.5f * m_fMoveValue );
			m_fHeroDz *= 2.5f + ( 2.5f * m_fMoveValue );
		}
	}
	else
	{
		if (GetCurWorld() == WORLD_SKY)  	// 20 ???????? 30 ????
		{
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
		else  				// 20?????? ???? 30?? ????????.
		{
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
	}

	// 1?????? ?????? ????????.
	m_fHeroIx = g_pRoh->m_wx + m_fHeroDx * 20.0f; // 20?????????? ???????? ????.
	m_fHeroIz = g_pRoh->m_wz + m_fHeroDz * 20.0f;

	// 1?????? ???????? ???????? ???????? ????????.
	// ???????? ???????? ??????, ???? ????????..
	// ???????? ??????.. ?????? ?????????? ?????? ????.
	// ?????? ?????? ?????? ?????? ??????.
	float fx, fz, fmx, fmz;
	// ?????? ????.
	fx = m_vAt.x;
	fz = m_vAt.z;
	// 1???? ???? ????.
	fmx = m_fHeroDx * 5;
	fmz = m_fHeroDz * 5;

	// ?????????? 1???? ???????? ?? ?? ???? ???????? ????????


	// ?????????? 2???? ?????? ?????? ????.
	int sign_x = SIGN(fmx);

	for (float step = 0.5f; step <= 4.0f && (fx+fmx*step)*sign_x <= tx*sign_x; step += 0.5)
	{
		if (!m_pLand->IsMovableArea(fx+fmx/2, fz+fmz/2))
			return TRUE;
	}

	return FALSE;
}

// ---------------------------------------------------------------------
// MoveHero()
// Desc : ?????????????? ?????? ???????? ???????? ???? ???? ?????? ?????? ????


//  ???? ?????????? ?????????? ??????.
//  tx, tz : ???? ???? ????.
// ---------------------------------------------------------------------
BOOL CMyD3DApplication::MoveHeroT(float tx, float tz, BOOL bWaypointMove)
{
	if( !g_pRoh || !m_pLand || !m_pPath)
		return false;

	float mag;
	int dY, dir;

	// Path Finding ???? ?????? ????.
	FindHeroPathTN(tx, tz, bWaypointMove);

	// Path?? WayPoint?? ???????? ???? ????.

	// ?????? ?????? ????????.

	// ???????? ????
	m_fHeroTx = tx;
	m_fHeroTz = tz;

	m_fHeroDx = m_fHeroTx - g_pRoh->m_wx;
	m_fHeroDz = m_fHeroTz - g_pRoh->m_wz;

	if (g_pRoh->m_bRun && (g_pRoh->m_Stamina >= 2 || (GetCurWorld() == WORLD_SKY && !m_pUIMgr->m_pLand->IsDrawTile(g_pRoh->m_wx, g_pRoh->m_wz))))
	{
		if (!m_bRohRunning)
		{
			m_bRohRunning = TRUE;
			m_startRunningTime = timeGetTime();
		}
		g_pRoh->SetMotionState(CHA_RUN);
	}
	else
		g_pRoh->SetMotionState(CHA_WALK);

	mag = m_fHeroDx*m_fHeroDx + m_fHeroDz*m_fHeroDz;
	mag = (float) sqrt( (double) mag );

	if (mag < g_EPSILON)
	{
		// ?????? ?????? ????, ?????? ????????.
		g_pRoh->SetMotionState(CHA_STAND);
		m_bHeroMoving = FALSE;
		return FALSE;
	}

	g_pRoh->m_toDir = (float) atan2(m_fHeroDz, m_fHeroDx);

	m_fHeroDx = m_fHeroDx / mag;
	m_fHeroDz = m_fHeroDz / mag;

	// 1?????????? ???????? ????.
	if (m_bRohRunning)
	{
		if (GetCurWorld() == WORLD_SKY &&
				m_pUIMgr->m_pLand->IsDrawTile(g_pRoh->m_wx, g_pRoh->m_wz))  	// 20???????? 40 ????
		{
			m_fHeroDx *= 2.0f + ( 2.0f * m_fMoveValue );
			m_fHeroDz *= 2.0f + ( 2.0f * m_fMoveValue );
		}
		else  					// 20?????? ???? 50?? ????????.
		{
			m_fHeroDx *= 2.5f + ( 2.5f * m_fMoveValue );
			m_fHeroDz *= 2.5f + ( 2.5f * m_fMoveValue );
		}
	}
	else
	{
		if (GetCurWorld() == WORLD_SKY)  	// 20 ???????? 30 ????
		{
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
		else  				// 20?????? ???? 30?? ????????.
		{
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
	}

	// 1?????? ?????? ????????.
	m_fHeroIx = g_pRoh->m_wx + m_fHeroDx * 20.0f; // 20?????????? ???????? ????.
	m_fHeroIz = g_pRoh->m_wz + m_fHeroDz * 20.0f;

	// 1?????? ???????? ???????? ???????? ????????.
	// ???????? ???????? ??????, ???? ????????..
	// ???????? ??????.. ?????? ?????????? ?????? ????.
	// ?????? ?????? ?????? ?????? ??????.
	float fx, fz, fmx, fmz;
	// ?????? ????.
	fx = m_vAt.x;
	fz = m_vAt.z;
	// 1???? ???? ????.
	if (m_bRohRunning)
	{
		fmx = m_fHeroDx * 4;	// 4 * 2.5 = 10 => 1????.
		fmz = m_fHeroDz * 4;
	}
	else
	{
		fmx = m_fHeroDx * 6.6666f;	// 6.6 * 1.5 = 9.9 ?? 1????.
		fmz = m_fHeroDz * 6.6666f;
	}

	// ?????????? 1???? ???????? ?? ?? ???? ???????? ????????


	// ?????????? 2???? ?????? ?????? ????.
	int i = 4;
	if (m_bInsideBuilding)
	{
		if (!m_pPath->IsMoveArea(fx+fmx/2, fz+fmz/2))
			i = 0;
		else if (!m_pPath->IsMoveArea(fx+fmx, fz+fmz))
			i = 0;
		else if (!m_pPath->IsMoveArea(fx+fmx*2, fz+fmz*2))
			i = 0;
		else if (!m_pPath->IsMoveArea(fx+fmx*3, fz+fmz*3))
			i = 1;
		else if (!m_pPath->IsMoveArea(fx+fmx*4, fz+fmz*4))
			i = 2;
		else if (!m_pPath->IsMoveArea(fx+fmx*5, fz+fmz*5))
			i = 3;
	}
	else
	{
		if (!m_pPath->IsMoveArea(fx+fmx/2, fz+fmz/2))
			i = 0;
		else if (!m_pPath->IsMoveArea(fx+fmx, fz+fmz))
			i = 0;
		else if (!m_pPath->IsMoveArea(fx+fmx*2, fz+fmz*2))
			i = 1;
		else if (!m_pPath->IsMoveArea(fx+fmx*3, fz+fmz*3))
			i = 2;
		else if (!m_pPath->IsMoveArea(fx+fmx*4, fz+fmz*4))
			i = 3;
		else if (!m_pPath->IsMoveArea(fx+fmx*5, fz+fmz*5))
			i = 4;
	}

	if ( i < 4 )
	{
		m_fHeroTx = fx + fmx*i;
		m_fHeroTz = fz + fmz*i;
		m_bMoreToGo = FALSE;

		if (i == 0 && m_vAt.x == fx && m_vAt.z == fz)
		{
			g_pRoh->SetMotionState(CHA_STAND);
			m_bHeroMoving = FALSE;
			return FALSE;
		}
		// 2004 check later
		if( g_bNewTerrain )
		{
			int	nY = (int)m_pLand->GetHFHeight( m_fHeroTx, m_fHeroTz );
			dY = (int)CNKObjQuadNode::GetHeightOnObject( m_fHeroTz, m_fHeroTx, g_pRoh->m_wy );
			if( dY < nY )
				dY = nY;
		}
		else
		{
			dY = (int) GetLandPtr()->GetHFHeight(m_fHeroTx, m_fHeroTz);
		}

		dir = (int) (g_pRoh->m_toDir*10000);

		if( m_fHeroTx <=0 || m_fHeroTz <= 0 )
		{
			OutputDebugString( "main::MoveHeroT\n" );
			return FALSE;
		}

		sprintf(g_commOut, "%s %d %d %d %d %d %d", "place", (int) m_fHeroTx, (int) m_fHeroTz, dY, dir, i*5, (m_bRohRunning?1:0));
		strcat(g_commOut, "\n");
		if (g_pTcpIp)
		{
			g_pTcpIp->SendNetMessage(g_commOut);
		}

		m_bHeroMoving = TRUE;
		m_dwPrevFrameTime = timeGetTime();

		return TRUE;
	}


	// 1?????? ?????? ???? ???? ?? ???? ?????? ?? ???????? ????.
	if (fabs(m_fHeroDx) > fabs(m_fHeroDz))
	{
		if ( ( (m_fHeroIx-g_pRoh->m_wx)/m_fHeroDx) < ( (m_fHeroTx-g_pRoh->m_wx)/m_fHeroDx) )
			m_bMoreToGo = TRUE;
		else
		{
			m_bMoreToGo = FALSE;
		}
	}
	else
	{
		if ( ( (m_fHeroIz-g_pRoh->m_wz)/m_fHeroDz) < ( (m_fHeroTz-g_pRoh->m_wz)/m_fHeroDz) )
			m_bMoreToGo = TRUE;
		else
		{
			m_bMoreToGo = FALSE;
		}
	}

	if (g_pTcpIp)
	{
		if (m_bMoreToGo)
		{
			if (m_bBeforeTrans != TRUE)
			{
				// 2004 check later
				if( g_bNewTerrain )
				{
					int	nY = (int)m_pLand->GetHFHeight( m_fHeroIx, m_fHeroIz );
					dY = (int)CNKObjQuadNode::GetHeightOnObject( m_fHeroIz, m_fHeroIx, g_pRoh->m_wy );
					if( dY < nY )
						dY = nY;
				}
				else
				{
					dY = (int) GetLandPtr()->GetHFHeight(m_fHeroIx, m_fHeroIz);
				}

				sprintf(g_commOut, "%s %d %d %d %d", "walk", (int) m_fHeroIx, (int) m_fHeroIz, dY, (m_bRohRunning?1:0));
				strcat(g_commOut, "\n");
				g_pTcpIp->SendNetMessage(g_commOut);
				//wan_mat 2005-04	???????? ???? ????
				if(g_MatrixSvr != 0 && !DrawPlaceInfo)
				{
					int newPlaceNum = PlaceInfo(m_fHeroIx, m_fHeroIz);
					if(newPlaceNum != m_nCurPlaceNum)
					{
						m_nCurPlaceNum = newPlaceNum;
						if(m_nCurPlaceNum)
							CreatePlaceInfo(m_nCurPlaceNum);
					}
				}
			}
		}
		else
		{
			int nRemainFrameX, nRemainFrameZ;
			// ???? ???????? ???? ?? ???? ???????? ?? ?? ?? ?? ???? ??????.
			nRemainFrameX = (int) ((m_fHeroTx-g_pRoh->m_wx) / m_fHeroDx);
			nRemainFrameZ = (int) ((m_fHeroTz-g_pRoh->m_wz) / m_fHeroDz);

			// 2004 check later
			if( g_bNewTerrain )
			{
				int	nY = (int)m_pLand->GetHFHeight( m_fHeroTx, m_fHeroTz );
				dY = (int)CNKObjQuadNode::GetHeightOnObject( m_fHeroTz, m_fHeroTx, g_pRoh->m_wy );
				if( dY < nY )
					dY = nY;
			}
			else
				dY = (int) GetLandPtr()->GetHFHeight(m_fHeroTx, m_fHeroTz);

			dir = (int) (g_pRoh->m_toDir*10000);
			if (m_bBeforeTrans != TRUE)
			{
				if( m_fHeroTx <=0 || m_fHeroTz <= 0 )
				{
					OutputDebugString( "main::MoveHeroT2\n" );
					return FALSE;
				}

				if (nRemainFrameX > nRemainFrameZ )
					// ?????? ???????????? 10000?? ???? ?? ?????? ????.
					sprintf(g_commOut, "%s %d %d %d %d %d %d", "place", (int) m_fHeroTx, (int) m_fHeroTz, dY, dir, nRemainFrameX, (m_bRohRunning?1:0));
				else
					sprintf(g_commOut, "%s %d %d %d %d %d %d", "place", (int) m_fHeroTx, (int) m_fHeroTz, dY, dir, nRemainFrameZ, (m_bRohRunning?1:0));

				strcat(g_commOut, "\n");
				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage(g_commOut);
			}
		}
	}

	m_bHeroMoving = TRUE;
	m_dwPrevFrameTime = timeGetTime();

	return TRUE;
}

// m_fHeroDx : 1/20 ?????? ???????? ?? ????.\
// m_fHeroIx : 1?? ???? ?????? ????. m_fHeroDx???? ?????? ???? ???? ?? ????. ???? ?????? m_bMoreToGo?? TRUE
// m_fHeroTx : ???? ???? ????.
// temp_x : ???? ?? ?????????? ???????? ?? ????.. ???? ???? ?????? ?????? ????.
void CMyD3DApplication::HeroMovingT()
{
	float temp_x, temp_z, temp_x2, temp_z2;
	BOOL bOverWalk = FALSE;


	if (CompareHeroAttGo())
		return;

	// temp_x, temp_z?? ?????????? ???????? ???? ????.
	temp_x = m_fHeroDx * g_fTimeKey / 0.05f ;		// 0.05f ?? ?????????? ?? ?????? ????
	temp_z = m_fHeroDz * g_fTimeKey / 0.05f ;		// HeroDx, HeroDz ?? ?????????? ?? ???????? ????.

	// ???????????? ???? ?????????? ???????? ?????? ?? ?? ???? ?????? ?????? ????????.
	int offset = 1;

	float fTimeKey = g_fTimeKey;
	while(fTimeKey > 0.05f)
	{
		if (m_pPath->IsMoveArea(m_vAt.x+m_fHeroDx*offset, m_vAt.z+m_fHeroDz*offset))
		{
			fTimeKey -= 0.05f;
			offset++;
		}
		else
		{
			// ???? ?????? ???????? ??????.
			// ?????????? 1?????? ?? ?????? ????????.
			temp_x = m_fHeroDx * (offset-1);
			temp_z = m_fHeroDz * (offset-1);

			// Tx, Tx?? ?????? ?????? ????.
			// ?? ???????? ???? ????????.
			m_fHeroTx = m_vAt.x + temp_x;
			m_fHeroTz = m_vAt.z + temp_z;


			m_bMoreToGo = FALSE;
			StopHero();
			break;
		}
	}

	// ???? ???? ????.. ??.. ???? ????...
	m_vEye.x += temp_x;
	m_vEye.z += temp_z;
	m_vAt.x += temp_x;
	m_vAt.z += temp_z;

	// ???? ?????? ???????? ?????? ????.
	if (!IsInsideMap(m_vAt.x, m_vAt.z))
	{
		m_vEye.x -= temp_x;
		m_vEye.z -= temp_z;
		m_vAt.x -= temp_x;
		m_vAt.z -= temp_z;

		// ?????????? ???????? ??????.
		return;
	}

	// ?????? ?????? ?????? ???? ?????? ???????? ?????? ????.
	if (m_bMoreToGo)
	{
		// 1/4
		if ( m_fHeroDx >= 0 && m_fHeroDz >= 0)
		{
			if (m_vAt.x >= m_fHeroIx && m_vAt.z >= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHeroT(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		} // 2 / 4
		else if ( m_fHeroDx <= 0 && m_fHeroDz >= 0)
		{
			if (m_vAt.x <= m_fHeroIx && m_vAt.z >= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHeroT(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		} // 3/4
		else if ( m_fHeroDx <= 0 && m_fHeroDz <= 0)
		{
			if (m_vAt.x <= m_fHeroIx && m_vAt.z <= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHeroT(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		} // 4/4
		else if ( m_fHeroDx >= 0 && m_fHeroDz <= 0)
		{
			if (m_vAt.x >= m_fHeroIx && m_vAt.z <= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHeroT(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		}
	}
	else
	{
		// 1/4
		/*if ( m_fHeroDx >= 0 && m_fHeroDz >= 0) {
			if (m_vAt.x >= m_fHeroTx && m_vAt.z >= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			// ?????? ???? ???????? ?????? ???? ???? ??????.
			if ( (m_fHeroTx - m_vAt.x) < 0 || (m_fHeroTz - m_vAt.z) < 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} // 2 / 4
		else if ( m_fHeroDx <= 0 && m_fHeroDz >= 0) {
			if (m_vAt.x <= m_fHeroTx && m_vAt.z >= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			if ( (m_fHeroTx - m_vAt.x) > 0 || (m_fHeroTz - m_vAt.z) < 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} // 3/4
		else if ( m_fHeroDx <= 0 && m_fHeroDz <= 0) {
			if (m_vAt.x <= m_fHeroTx && m_vAt.z <= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			if ( (m_fHeroTx - m_vAt.x) > 0 || (m_fHeroTz - m_vAt.z) > 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} // 4/4
		else if ( m_fHeroDx >= 0 && m_fHeroDz <= 0) {
			if (m_vAt.x >= m_fHeroTx && m_vAt.z <= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			if ( (m_fHeroTx - m_vAt.x) < 0 || (m_fHeroTz - m_vAt.z) > 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} else {
			bOverWalk = TRUE;
			StopHero();
		}*/
		if ( m_fHeroDx > 0.0f && m_vAt.x > m_fHeroTx)
		{
			bOverWalk = TRUE;
		}
		else if ( m_fHeroDx < 0.0f && m_vAt.x < m_fHeroTx)
		{
			bOverWalk = TRUE;
		}
		else if ( m_fHeroDz > 0.0f && m_vAt.z > m_fHeroTz)
		{
			bOverWalk = TRUE;
		}
		else if ( m_fHeroDz < 0.0f && m_vAt.z < m_fHeroTz)
		{
			bOverWalk = TRUE;
		}
	}

	if (bOverWalk)
	{
		// ?????? ???? ????..
		m_vEye.x -= temp_x;
		m_vEye.z -= temp_z;
		m_vAt.x -= temp_x;
		m_vAt.z -= temp_z;

		// ???? ??????
		temp_x = m_fHeroTx - m_vAt.x;
		temp_z = m_fHeroTz - m_vAt.z;

		m_vEye.x += temp_x;
		m_vEye.z += temp_z;
		m_vAt.x += temp_x;
		m_vAt.z += temp_z;

		if (m_fHeroPx != 0.0f)
		{
			MoveHeroT(m_fHeroPx, m_fHeroPz, TRUE);
		}
		else
		{
			//m_bHeroMoving = FALSE;
			StopHero();
		}
	}


	// ???????? ?????????? ???????? ????????.
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;

	//if (m_bHeroMoving == FALSE)
	//	g_pRoh->SetMotionState(CHA_STAND);

	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ???? ?????? ???? ??????.

	// ???????? ????.
	float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z);
	// ???? ????.
	if (m_vEye.y < (ty + 20.0f))
	{
		m_vEye.y = ty+20.0f;
		m_fCamHeight = m_vEye.y - m_vAt.y;
	}

	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;

	SetViewMatrix(m_matView);
}

void CMyD3DApplication::FindHeroPathT(float &tx, float &tz, BOOL bWaypointMove)
{
	static int nWaypoint = 0;
	float wx = g_pRoh->m_wx;
	float wz = g_pRoh->m_wz;
	float x_diff = ABS(tx-wx);
	float z_diff = ABS(tz-wz);
	int sign_x = SIGN(tx-wx);
	int sign_z = SIGN(tz-wz);

	float dx, dz;
	float ix, iz;
	int add;
	int diff_count = 0;
	BOOL bEnd = FALSE;
	if (x_diff > z_diff)
	{
		dx = 10.0f;
		dz = 10.0f * z_diff / x_diff * sign_z;
		if (sign_x > 0)
		{
			ix = wx;
			iz = wz;
			while (ix <= tx-dx && diff_count < 5)
			{
				ix += dx;
				iz += dz;
				diff_count++;
				if (!m_pPath->IsMoveArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pPath->IsMoveArea(ix, iz+add))
						{
							if (!IsBlockedPathT(wx, wz, ix, iz+add))
							{
								//ix -= dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz+add)) > 10 &&
									 m_pPath->IsMoveArea(ix-dx, iz+add) &&
									 !IsBlockedPathT(wx, wz, ix-dx, iz+add))
							{
								ix -= dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pPath->IsMoveArea(ix, iz-add))
						{
							if (!IsBlockedPathT(wx, wz, ix, iz-add))
							{
								//ix -= dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz-add)) > 10 &&
									 m_pPath->IsMoveArea(ix-dx, iz-add) &&
									 !IsBlockedPathT(wx, wz, ix-dx, iz-add))
							{
								ix -= dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
		else
		{
			ix = wx;
			iz = wz;
			while (ix >= tx+dx && diff_count < 5)
			{
				ix -= dx;
				iz += dz;
				diff_count++;
				if (!m_pPath->IsMoveArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pPath->IsMoveArea(ix, iz+add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz+add))
							{
								//ix += dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz+add)) > 10 &&
									 m_pPath->IsMoveArea(ix+dx, iz+add) &&
									 !IsBlockedPathT(wx, wz, ix+dx, iz+add))
							{
								ix += dx;
								iz += add;
								bEnd = TRUE;
								break;
							}

						}
						if (m_pPath->IsMoveArea(ix, iz-add))
						{
							if (!IsBlockedPathT(wx, wz, ix, iz-add))
							{
								//ix += dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz-add)) > 10 &&
									 m_pPath->IsMoveArea(ix+dx, iz-add) &&
									 !IsBlockedPathT(wx, wz, ix+dx, iz-add))
							{
								ix += dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
	}
	else
	{
		dx = 10.0f * x_diff / z_diff * sign_x;
		dz = 10.0f;
		if (sign_z > 0)
		{
			ix = wx;
			iz = wz;
			while (iz <= tz-dz && diff_count < 5)
			{
				ix += dx;
				iz += dz;
				diff_count++;
				if (!m_pPath->IsMoveArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pPath->IsMoveArea(ix+add, iz))
						{
							if (!IsBlockedPathT(wx, wz, ix+add, iz))
							{
								ix += add;
								//iz -= dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix+add)) > 10 &&
									 m_pPath->IsMoveArea(ix+add, iz-dz) &&
									 !IsBlockedPathT(wx, wz, ix+add, iz-dz))
							{
								ix += add;
								iz -= dz;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pPath->IsMoveArea(ix-add, iz))
						{
							if (!IsBlockedPathT(wx, wz, ix-add, iz))
							{
								ix -= add;
								//iz -= dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix-add)) > 10 &&
									 m_pPath->IsMoveArea(ix-add, iz-dz) &&
									 !IsBlockedPathT(wx, wz, ix-add, iz-dz))
							{
								ix -= add;
								iz -= dz;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
		else
		{
			ix = wx;
			iz = wz;
			while (iz >= tz+dz && diff_count < 5)
			{
				ix += dx;
				iz -= dz;
				diff_count++;
				if (!m_pPath->IsMoveArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pPath->IsMoveArea(ix+add, iz))
						{
							if (!IsBlockedPathT(wx, wz, ix+add, iz))
							{
								ix += add;
								//iz += dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix+add)) > 10 &&
									 m_pPath->IsMoveArea(ix+add, iz+dz) &&
									 !IsBlockedPathT(wx, wz, ix+add, iz+dz))
							{
								ix += add;
								iz += dz;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pPath->IsMoveArea(ix-add, iz))
						{
							if (!IsBlockedPathT(wx, wz, ix-add, iz))
							{
								ix -= add;
								//iz += dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix-add)) > 10 &&
									 m_pPath->IsMoveArea(ix-add, iz+dz) &&
									 !IsBlockedPathT(wx, wz, ix-add, iz+dz))
							{
								ix -= add;
								iz += dz;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
	}

	if (bWaypointMove)
		nWaypoint++;
	else
		nWaypoint = 0;

	if (bEnd && nWaypoint < 7)
	{
		m_fHeroPx = tx;
		m_fHeroPz = tz;
		tx = ix;
		tz = iz;
	}
	else
	{
		m_fHeroPx = 0.0f;
		m_fHeroPz = 0.0f;
		nWaypoint = 0;
	}
}


// ---------------------------------------------------------------
// Name : FindHeroPathTN()
// FindHeroPathT???? ?????? ?????? ?? ???? ???? ????


// 2002?? 4?? 3?? ????
// ---------------------------------------------------------------
void CMyD3DApplication::FindHeroPathTN(float &tx, float &tz, BOOL bWaypointMove)
{
	static int nWaypoint = 0;
	float wx = g_pRoh->m_wx;
	float wz = g_pRoh->m_wz;
	float x_diff = ABS(tx-wx);
	float z_diff = ABS(tz-wz);
	int sign_x = SIGN(tx-wx);
	int sign_z = SIGN(tz-wz);

	float dx, dz;
	float ix, iz;
	int add;
	int diff_count = 0;

	//return;

	BOOL bEnd = FALSE;
	if (x_diff > z_diff)
	{
		dx = 10.0f;
		dz = 10.0f * z_diff / x_diff * sign_z;
		if (sign_x > 0)
		{
			ix = wx;
			iz = wz;
			while (ix <= tx-dx && diff_count < 5)
			{
				ix += dx;
				iz += dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix, iz+add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz+add))
							{
								//ix -= dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz+add)) > 10 &&
									 m_pLand->IsMovableArea(ix-dx, iz+add) &&
									 !IsBlockedPath(wx, wz, ix-dx, iz+add))
							{
								ix -= dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pLand->IsMovableArea(ix, iz-add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz-add))
							{
								//ix -= dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz-add)) > 10 &&
									 m_pLand->IsMovableArea(ix-dx, iz-add) &&
									 !IsBlockedPath(wx, wz, ix-dx, iz-add))
							{
								ix -= dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
		else
		{
			ix = wx;
			iz = wz;
			while (ix >= tx+dx && diff_count < 5)
			{
				ix -= dx;
				iz += dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix, iz+add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz+add))
							{
								//ix += dx;
								iz += add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz+add)) > 10 &&
									 m_pLand->IsMovableArea(ix+dx, iz+add) &&
									 !IsBlockedPath(wx, wz, ix+dx, iz+add))
							{
								ix += dx;
								iz += add;
								bEnd = TRUE;
								break;
							}

						}
						if (m_pLand->IsMovableArea(ix, iz-add))
						{
							if (!IsBlockedPath(wx, wz, ix, iz-add))
							{
								//ix += dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wz-(iz-add)) > 10 &&
									 m_pLand->IsMovableArea(ix+dx, iz-add) &&
									 !IsBlockedPath(wx, wz, ix+dx, iz-add))
							{
								ix += dx;
								iz -= add;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
	}
	else
	{
		dx = 10.0f * x_diff / z_diff * sign_x;
		dz = 10.0f;
		if (sign_z > 0)
		{
			ix = wx;
			iz = wz;
			while (iz <= tz-dz && diff_count < 5)
			{
				ix += dx;
				iz += dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix+add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix+add, iz))
							{
								ix += add;
								//iz -= dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix+add)) > 10 &&
									 m_pLand->IsMovableArea(ix+add, iz-dz) &&
									 !IsBlockedPath(wx, wz, ix+add, iz-dz))
							{
								ix += add;
								iz -= dz;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pLand->IsMovableArea(ix-add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix-add, iz))
							{
								ix -= add;
								//iz -= dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix-add)) > 10 &&
									 m_pLand->IsMovableArea(ix-add, iz-dz) &&
									 !IsBlockedPath(wx, wz, ix-add, iz-dz))
							{
								ix -= add;
								iz -= dz;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
		else
		{
			ix = wx;
			iz = wz;
			while (iz >= tz+dz && diff_count < 5)
			{
				ix += dx;
				iz -= dz;
				diff_count++;
				if (!m_pLand->IsMovableArea(ix, iz))
				{
					for (add = 10; add <= 50; add += 10)
					{
						if (m_pLand->IsMovableArea(ix+add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix+add, iz))
							{
								ix += add;
								//iz += dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix+add)) > 10 &&
									 m_pLand->IsMovableArea(ix+add, iz+dz) &&
									 !IsBlockedPath(wx, wz, ix+add, iz+dz))
							{
								ix += add;
								iz += dz;
								bEnd = TRUE;
								break;
							}
						}
						if (m_pLand->IsMovableArea(ix-add, iz))
						{
							if (!IsBlockedPath(wx, wz, ix-add, iz))
							{
								ix -= add;
								//iz += dz;
								bEnd = TRUE;
								break;
							}
							else if (ABS(wx-(ix-add)) > 10 &&
									 m_pLand->IsMovableArea(ix-add, iz+dz) &&
									 !IsBlockedPath(wx, wz, ix-add, iz+dz))
							{
								ix -= add;
								iz += dz;
								bEnd = TRUE;
								break;
							}
						}
					}
					break;
				}
				if (bEnd)
					break;
			}
		}
	}

	if (bWaypointMove)
		nWaypoint++;
	else
		nWaypoint = 0;

	if (bEnd && nWaypoint < 7)
	{
		m_fHeroPx = tx;
		m_fHeroPz = tz;
		tx = ix;
		tz = iz;
	}
	else
	{
		m_fHeroPx = 0.0f;
		m_fHeroPz = 0.0f;
		nWaypoint = 0;
	}
}

BOOL CMyD3DApplication::IsBlockedPathT(float wx, float wz, float tx, float tz)
{
	float mag;

	//m_fHeroTx = tx; m_fHeroTz = tz;

	//m_fHeroDx = m_fHeroTx - m_vAt.x;
	//m_fHeroDz = m_fHeroTz - m_vAt.z;
	m_fHeroDx = tx - wx;
	m_fHeroDz = tz - wz;

	mag = m_fHeroDx*m_fHeroDx + m_fHeroDz*m_fHeroDz;
	mag = (float) sqrt( (double) mag );

	if (mag < g_EPSILON)
	{
		//m_bHeroMoving = FALSE;
		return FALSE;
	}

	m_fHeroDx = m_fHeroDx / mag;
	m_fHeroDz = m_fHeroDz / mag;

	// 1?????????? ???????? ????.
	if (m_bRohRunning)
	{
		if (GetCurWorld() == WORLD_SKY &&
				m_pUIMgr->m_pLand->IsDrawTile(g_pRoh->m_wx, g_pRoh->m_wz))  	// 20???????? 40 ????
		{
			m_fHeroDx *= 2.0f + ( 2.0f * m_fMoveValue );
			m_fHeroDz *= 2.0f + ( 2.0f * m_fMoveValue );
		}
		else  					// 20?????? ???? 50?? ????????.
		{
			m_fHeroDx *= 2.5f + ( 2.5f * m_fMoveValue );
			m_fHeroDz *= 2.5f + ( 2.5f * m_fMoveValue );
		}
	}
	else
	{
		if (GetCurWorld() == WORLD_SKY)  	// 20 ???????? 30 ????
		{
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
		else  				// 20?????? ???? 30?? ????????.
		{
			m_fHeroDx *= 1.5f + ( 1.5f * m_fMoveValue );
			m_fHeroDz *= 1.5f + ( 1.5f * m_fMoveValue );
		}
	}

	// 1?????? ?????? ????????.
	m_fHeroIx = g_pRoh->m_wx + m_fHeroDx * 20.0f; // 20?????????? ???????? ????.
	m_fHeroIz = g_pRoh->m_wz + m_fHeroDz * 20.0f;

	// 1?????? ???????? ???????? ???????? ????????.
	// ???????? ???????? ??????, ???? ????????..
	// ???????? ??????.. ?????? ?????????? ?????? ????.
	// ?????? ?????? ?????? ?????? ??????.
	float fx, fz, fmx, fmz;
	// ?????? ????.
	fx = m_vAt.x;
	fz = m_vAt.z;
	// 1???? ???? ????.
	fmx = m_fHeroDx * 5;
	fmz = m_fHeroDz * 5;

	// ?????????? 1???? ???????? ?? ?? ???? ???????? ????????


	// ?????????? 2???? ?????? ?????? ????.
	int sign_x = SIGN(fmx);

	for (float step = 0.5f; step <= 4.0f && (fx+fmx*step)*sign_x <= tx*sign_x; step += 0.5)
	{
		if (!m_pPath->IsMoveArea(fx+fmx/2, fz+fmz/2))
		{
			return TRUE;
		}
	}

	return FALSE;

}



void CMyD3DApplication::SetHeroAttGoCha(CNkCharacter *pCha)
{
	if (m_pHeroAttGoMob)
	{
		m_pHeroAttGoMob->m_bHeroAttCome = FALSE;
		m_pHeroAttGoMob = NULL;
	}
	if (m_pHeroAttGoCha)
	{
		m_pHeroAttGoCha->m_bHeroAttCome = FALSE;
		m_pHeroAttGoCha = NULL;
	}
	m_pHeroAttGoCha = pCha;
	if (pCha)
		pCha->m_bHeroAttCome = TRUE;
}

void CMyD3DApplication::SetHeroAttGoMob(CNkMob *pMob)
{
	if (m_pHeroAttGoMob)
	{
		m_pHeroAttGoMob->m_bHeroAttCome = FALSE;
		m_pHeroAttGoMob = NULL;
	}
	if (m_pHeroAttGoCha)
	{
		m_pHeroAttGoCha->m_bHeroAttCome = FALSE;
		m_pHeroAttGoCha = NULL;
	}
	m_pHeroAttGoMob = pMob;
	if (pMob)
		pMob->m_bHeroAttCome = TRUE;
}

void CMyD3DApplication::SetHeroCurSkill(int nSkill)
{
	m_nCurSkill = nSkill;
}

extern CNkMob* g_pTargetMob[];
extern CNkCharacter* g_pTargetCha[];


//--------------------------------------------------------------------------
// SkillConditionCheck
//--------------------------------------------------------------------------
// Lyul Skill!!!!
// ???? Skill ?? ???? ?????????? Check ????.
// Return Vlue flase : Skill ???? ????.
//             true  : Skill ???? ????.
// !! Default Return : ???? ???? ???? Skill ???? ?????? ???? ????
//                  Default ?? true ?? Return ????.
//                  False ?? ???? Skill ???? ?????? ?????????? ?????? ????.
bool CMyD3DApplication::SkillConditionCheck(int cur_skill)
{
	int li_selskill = 0 ;
	int req_STM = 0; // ???? Stamina
	int req_MNA = 0; // ???? Mana
	int req_ELE = 0; // ???? Electric
	int req_VAT = 0; // ???? Vaital
	int temp_point = 0;

	// ASSERTION
	assert(cur_skill >=0);
	if( !g_ChaSkill.m_SkillIndex )
		return false;

	li_selskill = g_ChaSkill.m_SkillIndex[cur_skill];


	if( g_pRoh->m_Wearing[WEARING_WEAPON]
			&& cur_skill!=0
			&& ( g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 1238
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 1259
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 1260
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 1261
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 2170
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 2171
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 2172
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 3504
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 2102
				 || g_pRoh->m_Wearing[WEARING_WEAPON]->m_Vnum == 2697
			   ))
		return false;

	req_STM = g_Skills[li_selskill].GetEnerge0(g_ChaSkill.m_SkillLevel[cur_skill]) ;
	req_ELE = g_Skills[li_selskill].GetEnerge0(g_ChaSkill.m_SkillLevel[cur_skill]) ;
	req_MNA = g_Skills[li_selskill].GetEnerge0(g_ChaSkill.m_SkillLevel[cur_skill]) ;

	// ???? ????
	if(G_SK_Getflag(li_selskill))
		return false;

	// Chakram
	if(m_pUIMgr->m_bChakram)
	{
		if(!g_pRoh->m_QuickSlot[m_pUIMgr->m_ChakramSlotNum])
			return false;
		return true;
	}
	// BookSpell : ??????

	if(m_pUIMgr->m_bBookSpell)
	{
		if(!g_pRoh->m_QuickSlot[m_pUIMgr->m_BookSlotNum])
			return false;
		//?????? ???? ?????? ????????

		if(  m_pUIMgr->m_bRClickDown == 1 && pCMyApp->DrawCasting )
			return false;
		return true;
	}

	switch(li_selskill)
	{
	case SKILL_RECALL:
		if(g_MatrixSvr != 0)
			return false;
		return TRUE;
	// BULKAN SKILL Check
	// SLAYER : ???????? ????, ???????? ??????, ????????.
	case SKILL_THROW:
		// ????
		if( !(g_pRoh->m_Wearing[WEARING_CLOAK]) )
			return false;
		// ????
		if( !(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_CLOAK]) & IATT_SUB_WEAPON) )
			return false;
		// ??????
		if(g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special ==0
				|| g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special ==1 )
			return false;
		// ????????
		if(g_pRoh->m_Stamina < g_pRoh->m_Wearing[WEARING_CLOAK]->m_Num4)
			return false;

		return true;
	// Drop Attack / Rotate Attack : ??/???? , ????????.
	case SKILL_DROP_ATTACK:   // Drop Attack
	case SKILL_ROTATE_ATTACK: // Rotate Attack
		// ????

		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		// ?? ????
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SWORD
				&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_AXE)
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM )
			return false;

		return true;
	// Force Attack : ??/???? , ????????
	case SKILL_FORCE_ATTACK : // Force Attack
		// ????

		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		// ?? ????
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SWORD
				&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_AXE)
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM )
			return false;
		return true;
	// Hell Cry : ??/????, ????????
	case SKILL_HELL_CRY     : // Hell Cry
		//	if(g_bMatrixSvr)return false;	//wan:???????? ???? ???? ????????
		// ????

		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		// ?? ????
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SWORD
				&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_AXE)
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM )
			return false;

		return true;
	// KAI SKILL Check
	// SKILL_TORNADO_RUIN : ????, ????
	case SKILL_TORNADO_RUIN     : // SKILL_TORNADO_RUIN
		//	if(g_bMatrixSvr)return false;	//wan:???????? ???? ???? ????????
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;

		return true;
	//  SKILL_HELL_TOUCH / SKILL_VANPIRE_TOUCH : ????, ????, ?????? ????. ????
	case SKILL_HELL_TOUCH :    // SKILL_TORNADO_RUIN
	case SKILL_VANPIRE_TOUCH : // SKILL_VANPIRE_TOUCH
		// ???? ????
		if(g_pRoh->m_Mana < 2)
			return false;
		// ????

		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		// ?? ???? ????.
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_Vnum != 1021)
			return false;
		// ????

		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)
			return false;

		return true;
	// SKILL_MAGIC_MOVE (????) : ???? ????, ????, ???? ????
	case SKILL_MAGIC_MOVE :
		// ????

		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		// ????

		if(!(GET_SHAPE(g_pRoh->m_Wearing[WEARING_SHIELD]) & ISUB_WAND))
			return false;
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_Vnum == 1021)
			return false; // ?????? ???? ????
		// ????
		if(g_pRoh->m_Mana < g_pRoh->m_Wearing[WEARING_SHIELD]->m_Num4)
			return false;
		// ????

		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)
			return false;
		return true;
	//????????
	case SKILLF_E_REFLECTION :
		// ????
		if( !g_pRoh->m_Wearing[WEARING_WEAPON]
				|| !( GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER))
			return false;
		if( g_pRoh->m_ShowReflection || g_pRoh->m_bShowAdrenaline )
			return false;
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;
	//??????

	case SKILL_C_HELLDOWN :
		// ????

		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_STAFF)
			return false;
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;
	case SKILL_FIRE_BALL	:	      // ????????
	case SKILL_LIGHTNING_BALL :	  // ???????? ??
	case SKILL_PEBBLE_SHOT :	    // ??????
	case SKILL_FIRE_PILLAR :	    // ??????
	case SKILL_LIGHTNING :		    // ????????
	case SKILL_FALL_STONE :	    	// ????
	case SKILL_SHOCK_SPEAR :	    // ???? ??????
	case SKILL_STONE_LANCE :	    // ???? ????
	case SKILL_STAR_FLARE :	      // ???? ??????
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;

		return true;
	// HUMAN SKILL Check
	// SKILL_HANDLE : ???? ????, ????, ??????
	case SKILL_HANDLE :
		// ????

		if(!g_pRoh->m_Wearing[WEARING_CLOAK])
			return false;
		// ???? ????

		if(!(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_CLOAK]) & IATT_SUB_WEAPON))
			return false;
		// ???? ????
		if(g_pRoh->m_Wearing[WEARING_CLOAK]->m_PlusNum==0)
			return false;
		// ???? ??????
		if(g_pRoh->m_Epower < g_pRoh->m_Wearing[WEARING_CLOAK]->m_Num4)
			return false;

		return true;
	// SKILL_AIR_RAID : ???? , ??????
	case SKILL_AIR_RAID:
		//	if(g_bMatrixSvr)return false;	//wan:???????? ???? ???? ????????
		// ???? ????	???????????? ???????? ????????. ????????,???????? ????
		if(g_bDungeon||g_bNewTerrainDungeon)
		{
			switch( g_dwLangType ) // [2008/12/10 Theodoric] ?????????? ???? ?????? ???????? ???? ????
			{
			case LANG_KOR:
				return true;
			default:
				return false;
			}
		}

		// ??????
		if(g_pRoh->m_Epower < req_ELE )
			return false;

		return true;
	// SKILL_SNIFING : ??????, ?? ????

	case SKILL_SNIFING :
		// ????

		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		// ??
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_GUN)
			return false;

		// ??????
		req_ELE = g_pRoh->m_Wearing[WEARING_WEAPON]->m_Num4;
		temp_point = (g_Skills[li_selskill].GetApp0(g_ChaSkill.m_SkillLevel[cur_skill]) - 10) * 10;
		req_ELE += req_ELE * temp_point / 100;
		if(g_pRoh->m_Epower < req_ELE)
			return false;

		return true;
	// ?? / ???? ???? : ?????? ????
	case SKILL_DEEP_ATTACK  : // ?? ????
	case SKILL_CHAIN_ATTACK : // ???? ????
		// ????

		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		// ??????
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_Vnum != 1023)
			return false;
		// ???? ????
		if(g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum==0)
			return false;

		return true;
	// ?????? : ????????, ????????????, ??????.
	case SKILL_HIDE:

		if( g_pRoh->GetCombat()		// ????????

				|| g_pRoh->m_bCrystalOwner	// ???????? ??????
				|| m_pUIMgr->m_b_Attack)	// ??????
			return false;

		return true;
	// AIDIA SKILL Check
	// SKILL_SUMMON_PARTITION: ???????? ????/ ??????/ ???? / ????
	case SKILL_SUMMON_PARTITION:
		//	if(g_bMatrixSvr)return false;	//wan:???????? ???? ???? ????????
		// ?????? / ????????

		// [2008/4/25 Theodoric] ???? ?????? ???????????? ???????? ?????? ????

		if( g_pRoh->GetMyGuildData() != NULL &&
				m_pHeroAttGoCha != NULL &&
				g_pRoh->GetMyGuildData()->CheckInvasionGuild( m_pHeroAttGoCha->m_GuildIndex )  &&
				!(m_pUIMgr->m_pSummonMob[0] == NULL &&m_pUIMgr->m_pSummonMob[1] == NULL ) &&
				req_MNA <= g_pRoh->m_Mana && 
				!(GetCurWorld() == 4 &&  IsInsideRect(m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wz, 6800, 5450, 7250, 5000) )
		  )
			return true;

		if(!m_pUIMgr->m_pSummonMob ||
				(pCMyApp->IsPeaceArea() && !g_pNk2DFrame->IsPVPVisible()))
			return false;
		if(req_MNA > g_pRoh->m_Mana)
			return false;

		return true;

	case SKILL_TELEPORT:
		// ????
		if(IsAcropolis() == 1 || g_pRoh->m_Mana < req_MNA)	// ?????????? ????????
			return false;

		return true;
	case SKILL_MELODY_OF_DEF:
	case SKILL_MELODY_OF_GUD:
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if(!(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON))
			return false;
		if(IsAcropolis() == 1 || g_pRoh->m_Mana < req_MNA)
			return false;
		if( g_pRoh->m_Wearing[WEARING_SHIELD]->m_PlusNum == 0 )
			return false;

		return true;

	case SKILLF_A_DSTONE: // ?????? ????

		// ????
		if(!(g_pRoh->m_Wearing[WEARING_CLOAK]))
			return false;
		// ???? GET_SHAPE(m_ExtraSlot) == ISUB_DSTONE)
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_CLOAK]) == ISUB_DSTONE))
			return false;
		// ??????
		if(g_pRoh->m_Wearing[WEARING_CLOAK]->m_Special < 20)
			return false;
		// ????
		if(g_pRoh->m_Mana < g_pRoh->m_Wearing[WEARING_CLOAK]->m_Num4)
			return false;

		if(g_pRoh->m_bstone) // g_pRoh->m_resmobidx
			return false;

		return true;

	case SKILLF_A_DSLASH: // ???? ??????
		if( g_pRoh->m_bCloaking	|| g_pRoh->m_bCloaking_Waring)
			return false;
		// ????
		if(!(g_pRoh->m_Wearing[WEARING_WEAPON]))
			return	 false;
		// ?? ????
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD))
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM)
			return false;

		return true;

	// SKILLF_A_BASH
	case SKILLF_A_BASH: // ????
		// ????
		if(!(g_pRoh->m_Wearing[WEARING_WEAPON]))
			return false;
		// ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		// ???? ????

		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER))
			return false;

		return true;

	case SKILLF_A_ASSAULT: // ??????
		if( g_pRoh->m_bCloaking	|| g_pRoh->m_bCloaking_Waring)
			return false;
		// ????
		if(!(g_pRoh->m_Wearing[WEARING_WEAPON]))
			return false;
		// ?? ????
		if(!( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD))
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM)
			return false;

		return true;

	case SKILLF_E_BLEED:
		if( g_pRoh->m_bCloaking	|| g_pRoh->m_bCloaking_Waring)
			return false;

		return true;

	// SKILLF_A_DASH : ????????
	case SKILLF_A_DASH: // ????
		if(IsAcropolis() == 1 || g_pRoh->m_Stamina < req_STM)	// ??????, ????????
			return false;

		return true;

	case SKILLF_E_CLOAKING: // ?????? ???????? ?????? ?????? ????.

		// ???????? ?????? ????
		if( g_pRoh->m_Stamina < req_STM )
			return false;

		return true;


	// SKILLF_A_TIMECHARGE : ????????

	case SKILLF_A_TIMECHARGE:
		// ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		// ?????? ???? ????????
		if(!g_pRoh->m_bstone)
			return false;

		return true;

	// nate 2005-06-08 : Burning Crash - SkillConditionCheck()
	case SKILL_B_BURNINGCRASH:
		// ???? ????
		if( !g_pRoh->m_Wearing[ WEARING_CLOAK ] )
			return false;
		// ?????????? ???????? 60???????? ????
		if( g_pRoh->m_Wearing[ WEARING_CLOAK ]->m_Special < 60 )
			return false;
		// ?????????? ???????? ?????? ????
		if( GET_SHAPE( g_pRoh->m_Wearing[ WEARING_CLOAK ] ) != ISUB_SLAYER )
			return false;
		// ???????? ?????? ????
		if( g_pRoh->m_Stamina < req_STM )
			return false;
		return true;
	// nate 2005-07-19 : Life Expansion - SkillConditionCheck()
	case SKILL_A_LIFE_EXPANSION:
		// ???? ????
		if( !g_pRoh->m_Wearing[ WEARING_SHIELD ] )
			return false;
		// ???????? ???? ???? ????
		if( GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 1022 		// 1022 : ????????
		 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 3067 
		 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 10206 // Update Elite
		 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 10417
		 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 11065
		 && GET_ITEM_NUM( g_pRoh->m_Wearing[ WEARING_SHIELD ] ) != 11086
		 
		 )	// 3067 : ????????
			return false;
		return true;
	case SKILL_B_FLYINGCRASH://mungmae: 2005/11/16 ?????? ??????
		if( IsAcropolis() == 1 || g_pRoh->m_Stamina < req_STM ) // ??????, ????????
			return false;
		if( g_pRoh->m_CancelState && (
					g_pRoh->m_ChaCondition == CONDITION_STONY ||
					g_pRoh->m_ChaCondition == CONDITION_ENTANGLE ) ) //mungmae: 2006/03/29 ???? ???????? ???? ????
			return false;
		return true;
	case SKILL_F_SHADOWSLASH://mungmae: 2005/11/18 ?????? ??????
		if( !g_pRoh->m_Wearing[WEARING_WEAPON]
				|| !( GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_DUAL_SWORD))
			return false;
		if(IsAcropolis() == 1 || g_pRoh->m_Stamina < req_STM) //??????, ???????? ????

			return false;
		if( g_pRoh->m_CancelState && (
					g_pRoh->m_ChaCondition == CONDITION_STONY ||
					g_pRoh->m_ChaCondition == CONDITION_ENTANGLE) ) //mungmae: 2006/03/29 ???? ???????? ???? ????
			return false;
		return true;
	case SKILL_F_ADRENALINE ://mungmae-2005/11/21 - ??????????
		if( !g_pRoh->m_Wearing[WEARING_WEAPON] || !( GET_SHAPE( g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_HAMMER))
			return false;
		if( g_pRoh->m_bShowAdrenaline || g_pRoh->m_ShowReflection )
			return false;
		if( g_pRoh->m_Mana < req_MNA )
			return false;
		return true;

	case SKILL_H_AUTOMINE:

		// ??????
		if(g_pRoh->m_Epower < req_ELE )
			return false;
		return true;
	case SKILL_A_SUMMONS_SQUAD: // ????????
		if( g_pRoh->m_Mana < req_MNA ) // ???? ???? ???? 06-07-12 ????
			return false;
		return true;
	case SKILL_F_SPACESLASH:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_DUAL_SWORD )
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM )
			return false;
		return true;
	case SKILL_F_ELECTRICHAMMER:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_HAMMER )
			return false;
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;
	case SKILL_B_PROTECTION_SHIELDS :
		// ????
		if(!g_pRoh->m_Wearing[WEARING_SHIELD])
			return false;
		if( GET_SHAPE( g_pRoh->m_Wearing[WEARING_SHIELD]) != IARMOR_SHIELD )
			return false;
		if( g_pRoh->m_bProtectionShield )
			return false;
		//  ???? ????
		if(g_pRoh->m_Stamina < req_STM )
			return false;

		// ?????? ???? ?????? ???????? ???? ?????? ????( 07-02-26 ???? )
		if( g_pRoh->GetMotionState() == CHA_B_BURNINGCRASH ) //???? ??????...
			return false;

		return true;
	case SKILL_H_RANDOMFIRE:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_GUN)
			return false;
		if(g_pRoh->m_Epower < req_ELE)
			return false;
		return true;
	case SKILL_K_ICEBOLT:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_STAFF)
			return false;
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;
	case SKILL_F_MERCERNERY_SLASH:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_DUAL_SWORD )
			return false;
		// ????????
		if(g_pRoh->m_Stamina < req_STM )
			return false;
		return true;
	case SKILL_H_FROZENSHOT:
		// ????

		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		// ??
		if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_GUN)
			return false;
		// ??????
		if(g_pRoh->m_Epower < req_ELE )
			return false;
		return true;
	case SKILL_K_FLAMESPEAR:
		/*
				if(!g_pRoh->m_Wearing[WEARING_WEAPON])
					return false;
				if(GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_STAFF)
					return false;
		*/
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;
	case SKILL_A_RESURRECTION:
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;
	case SKILL_F_TORNADO_HAMMER:
		if(!g_pRoh->m_Wearing[WEARING_WEAPON])
			return false;
		if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_HAMMER )
			return false;
		//  ???? ????
		if(g_pRoh->m_Mana < req_MNA)
			return false;
		return true;

	case SKILL_H_ELECTRICSHOT:
		if( g_pRoh->m_Epower < req_ELE)
			return false;
		return true;

	// ?????? ?? ?????????? ???? ????
	case PEROM_SKILL_S127:
	case PEROM_SKILL_S128:
	case PEROM_SKILL_S129:
	case PEROM_SKILL_S130:
	case PEROM_SKILL_S131:
	case PEROM_SKILL_S132:
	case PEROM_SKILL_S133:
	case PEROM_SKILL_S135:
	case PEROM_SKILL_S136:
	case PEROM_SKILL_S137:
	case PEROM_SKILL_S138:
		{
			if( !g_pRoh->m_Wearing[WEARING_WEAPON] )
				return false;
			if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SPEAR )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		return true;

	case PEROM_SKILL_S134:
		{
			if( g_pRoh->IsActiveField() )
				return false;
			if(!g_pRoh->m_Wearing[WEARING_WEAPON])
				return false;
			if( GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) != IWEAPON_SPEAR )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		break;
	case PEROM_SKILL_S139:
		{
			if(!g_pRoh->m_Wearing[WEARING_CLOAK])
				return false;
			if( GET_SHAPE( g_pRoh->m_Wearing[ WEARING_CLOAK ] ) != ISUB_PANEL )
				return false;
			if( g_pRoh->IsActiveDSystem() )
			{
// 				if(g_pRoh->m_Epower < req_ELE )
// 					return false;
			}
			else
			{
				if( g_pRoh->m_Wearing[ WEARING_CLOAK ]->m_Special <= 5 )
					return false;
				if( g_pRoh->m_Epower < req_ELE )
					return false;
			}
		}
		return true;

	case PEROM_SKILL_S140:	// 140
		{
			if( !g_pRoh->IsActiveDSystem() )
				return false;

			if(!g_pRoh->m_Wearing[WEARING_CLOAK])
				return false;
			if( GET_SHAPE( g_pRoh->m_Wearing[ WEARING_CLOAK ] ) != ISUB_PANEL )
				return false;
			if( g_pRoh->m_Wearing[ WEARING_CLOAK ]->m_Special < 2 )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		return true;

	case PEROM_SKILL_S141:	// 141
		{
			if( !g_pRoh->IsActiveDSystem() )
				return false;

			if(!g_pRoh->m_Wearing[WEARING_CLOAK])
				return false;
			if( GET_SHAPE( g_pRoh->m_Wearing[ WEARING_CLOAK ] ) != ISUB_PANEL )
				return false;
			if( !g_pRoh->m_CancelState )
				return 1;
			if( g_pRoh->m_Wearing[ WEARING_CLOAK ]->m_Special < 2 )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		return true;
	case PEROM_SKILL_S150:	// 150
		{
			if( !g_pRoh->IsActiveField() )
				return false;
			if( !g_pRoh->IsActiveDSystem() )
				return false;
			if( g_pRoh->IsActiveField_Ex() )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		return true;

	case PEROM_SKILL_S148:
		{
			if( g_pRoh->IsActiveCritical() )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		return true;
	case PEROM_SKILL_S149:
		{
			if( g_pRoh->IsActiveEngage() )
				return false;
			if( g_pRoh->m_Epower < req_ELE )
				return false;
		}
		return true;
	}
	// Default Return
	return true;
}


bool CMyD3DApplication::CheckSkillCondition(int cur_skilll)
{
	// ???? ????????...
	//if( g_pRoh->m_Race == RACE_KAILIPTON)

	return true;
}


// nate 2005-12-15 : ?????? ???? ????

int CMyD3DApplication::GetTargetCountOfSkill( int nCurSkill )
{
	int nCount = 0;
	switch(g_ChaSkill.m_SkillIndex[nCurSkill])
	{
	case SKILL_FIRE_BALL:			// ???????? - 2
		nCount = 2;
		break;
	case SKILL_FIRE_PILLAR:			// ?????? - 4
		nCount = 4;
		break;
	case SKILL_STAR_FLARE:			// ???? ?????? - 5
		nCount = 5;
		break;
	case SKILLF_A_BASH:				// ???? - 5
		nCount = (int(ceil( (float)(g_ChaSkill.m_SkillLevel[nCurSkill]) *5.0f /20.0f )));
	case SKILL_TORNADO_RUIN:		// ???????? ????.  - 10
	case SKILL_K_POISON:			// nate 2005-11-08 : ?????? - AttackMsgSend - 10
	case SKILL_K_FLAMESPEAR:
	case SKILL_AIR_RAID:			// ?????????? - 10
	case SKILL_HELL_CRY:			// Hell Cry - 10
	case SKILL_SUMMON_PARTITION:	// ????????
		nCount = 10;
		break;
	case PEROM_SKILL_S130:
	case PEROM_SKILL_S132:
	case PEROM_SKILL_S133:
	case PEROM_SKILL_S136:
	case PEROM_SKILL_S140:
		nCount = 5;
		break;
	default:
		nCount = 1;
		break;
	}

	return nCount;
}

// nate 2005-11-23 : ???????? - ???? ?????? ????
// ???? : ?????? ?????? ?????? ???? case?? ???? ????????
// ???? : ?????? ?????? ??????
// ?????? :	true - ???????? ???????? ??????
//			false - ???? ????
BOOL CMyD3DApplication::AttackMsgSend(int cur_skill)
{
	//====================================================================
	// ???? ???? start
	if(m_pUIMgr->m_bChakram)	// Chakram
	{
		if(m_pHeroAttGoMob)
			sprintf(g_commOut, "use_sub %d npc %d\n", m_pUIMgr->m_ChakramSlotNum,m_pHeroAttGoMob->m_nVirId);
		else if ( m_pHeroAttGoCha)
			sprintf(g_commOut, "use_sub %d char %d\n", m_pUIMgr->m_ChakramSlotNum,m_pHeroAttGoCha->m_nCharIndex);

		m_pUIMgr->SendAttackMessage(g_commOut);
		return true;
	}

	if(m_pUIMgr->m_bBookSpell)	// BookSpell
	{
		if(m_pHeroAttGoMob)
			sprintf(g_commOut, "use_sub %d npc %d\n", m_pUIMgr->m_BookSlotNum, m_pHeroAttGoMob->m_nVirId);
		else if ( m_pHeroAttGoCha)
			sprintf(g_commOut, "use_sub %d char %d\n", m_pUIMgr->m_BookSlotNum, m_pHeroAttGoCha->m_nCharIndex);

		m_pUIMgr->SendAttackMessage(g_commOut);
		return true ;
	}
	// ???? ???? end
	//====================================================================

	assert(cur_skill >=0);
	int li_selskill = 0;
	///????????
	if( cur_skill == SKILL_F_DASHSLASH || cur_skill == SKILL_F_SLASHSHADOW
			|| cur_skill == SKILL_F_STONEBASH1 || cur_skill == SKILL_F_STONEBASH2
			|| cur_skill == SKILL_F_BASHREFLECT1 || cur_skill == SKILL_F_BASHREFLECT2
			|| cur_skill == SKILL_K_FLAMESPEAR_TARGET ) // ???? ???? ???????? ???????? ???????? ???? ????. ???????? ?????? ????????. (??>?????????? ????)
	{
		li_selskill = cur_skill;
	}
	else
	{
		li_selskill = g_ChaSkill.m_SkillIndex[cur_skill];
	}

	int target_count = 0;	// ???? Target ????

	int att_count = 0;		// ???? ?????? Target ???? ????

	int area = 0;
	float fDist = 0.0f;		// ???? ????
	float att_range = 0.0f;	// ?????????????? ???? ?????????? ?????? ????.
	CNkMob *pMob = NULL;	// ?????????????? ????	??????

	DWORD dwTime = timeGetTime();

	if( m_pHeroAttGoCha && ( m_pHeroAttGoCha->IsDead()
							 && ( li_selskill != SKILL_A_RESURRECTION || li_selskill == SKILL_ATTACK ) ) )
	{
		return FALSE;
	}

	// ?????????? ???? ?????????? ?????? ???????? ?????? ????????.(?????? ???????? ?????????? ???????? ???????? ???????? ???? ??????... ///????????
	if( li_selskill == SKILLF_A_BASH ) // ?????? ??????????
	{
		if( m_pHeroAttGoMob && m_pHeroAttGoMob->m_nVirId == g_pRoh->m_resmobidx ) // ???? ???? ?????? ?????? ?????? ????????...
		{
			if( timeGetTime()-g_pRoh->m_dwComboTick > 500 && timeGetTime()-g_pRoh->m_dwComboTick < 1000 ) // ???????? ????(... ///????????
			{
				li_selskill = cur_skill = SKILL_F_STONEBASH1; // ?????? ???? ???? ?????????? ????.
			}
		}
	}

	switch(li_selskill)
	{
	//============================================================
	// "subat" ??????

	// ?????? : true
	case SKILL_THROW:	// ???? ???? - Slayer
	case SKILL_MAGIC_MOVE:
		if(m_pHeroAttGoMob)
			sprintf(g_commOut, "subat npc %d %d %d\n",
					m_pHeroAttGoMob->m_nVirId,	// ?? ??????

					li_selskill,				// ????????
					FALSE );					// ????
		else if ( m_pHeroAttGoCha)
			sprintf(g_commOut, "subat char %d %d %d\n",
					m_pHeroAttGoCha->m_nCharIndex,	// ???? ??????

					li_selskill,				// ????????
					FALSE );					// ????

		m_pUIMgr->SendAttackMessage(g_commOut);
		return true;

	//============================================================
	// "use_skill" + ??????
	// ?????? : true
	case SKILL_DROP_ATTACK:		// ????????

	case SKILL_ROTATE_ATTACK:	// ????????

	case SKILL_FORCE_ATTACK:	// ????????
	case SKILL_REMOVE:			// ???? ????
	case SKILL_LIFE_LIGHT:		// ?????? ??

	case SKILLF_A_ASSAULT:		// ??????
	case SKILLF_A_TIMECHARGE:	// ????????

	case SKILLF_A_DSLASH:		// ??????????
	case SKILL_H_ELECTRICSHOT:	// ???????? ??
	case SKILL_HELL_TOUCH:		// ??????
	case SKILL_VANPIRE_TOUCH:	// ????????

	case SKILL_F_SLASHSHADOW:   // ???????????? ///????????
	case SKILL_K_ICEBOLT:
	case SKILL_B_WILDSMASH:
	case SKILL_F_SPACESLASH:
		{
			if(m_pHeroAttGoMob)
			{	
				SendCharUseSkill_Target(li_selskill, eCharType_Npc, m_pHeroAttGoMob->m_nVirId);
			}
			else if ( m_pHeroAttGoCha)
			{	
				SendCharUseSkill_Target(li_selskill, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex);
			}

			if( li_selskill == SKILL_F_SLASHSHADOW )
				G_SK_SetCooltime( SKILL_F_SHADOWSLASH );
			else
				G_SK_SetCooltime(li_selskill);	// ??????			
		}
		return true;

	//============================================================
	// "use_skill"
	// ?????? : true
	case SKILL_SNIFING:	// ????????
		{
			if(m_pHeroAttGoMob)
			{	
				SendCharUseSkill_Target(li_selskill, eCharType_Npc, m_pHeroAttGoMob->m_nVirId);
			}
			else if(m_pHeroAttGoCha)
			{	
				SendCharUseSkill_Target(li_selskill, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex);
			}			
		}
		return true;

	case SKILL_BLESSING:		// ?????? ????
	case SKILL_JOINT_LIFE:
	case SKILL_MELODY_OF_DEF:	// ?????? ????
		{
			if( li_selskill == SKILL_MELODY_OF_DEF )
			{
				if(!g_pRoh->m_Wearing[WEARING_SHIELD])	// ????

					return false;

				if(!(GET_ATT_FLAG(g_pRoh->m_Wearing[WEARING_SHIELD]) & IATT_SUB_WEAPON))	// ????
					return false;
			}

			if(m_pHeroAttGoCha)
			{
				if( li_selskill == SKILL_JOINT_LIFE &&
						g_pNk2DFrame->GetControlParty()->IsMyParty(m_pHeroAttGoCha->m_nCharIndex) == FALSE )
					return false;
				
				SendCharUseSkill_Target(li_selskill, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex);
				// ?????????? ?????? ???????? ?????? ???? ???? ???????? ???????? ?????? ???? ?????? ???????? ?????? ???????? ?????????????? ?????? ??????. ?????? ???????? ???????? ???????? ???? ?????? ?????????? ????. ///?????????????????? ???????? ?????????? ??????.
				if( li_selskill != SKILL_JOINT_LIFE && li_selskill != SKILL_BLESSING && li_selskill != SKILL_MELODY_OF_DEF )
					G_SK_SetCooltime(li_selskill);
				return true;
			}
		}
		return FALSE;

	//============================================================
	case SKILL_HANDLE:       // ??????
		{
			if(m_pUIMgr->m_b_Attack )
			{
				if(m_pHeroAttGoMob)
				{
					SendCharAttack(eCharType_Npc, m_pHeroAttGoMob->m_nVirId, false);					
				}
				else if(m_pHeroAttGoCha)
				{
					SendCharAttack(eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, false);					
				}				
			}

			if(dwTime - m_pUIMgr->dwDelayTime < 1200)
			{
				g_pRoh->m_RenderMultiShot = g_pRoh->m_MultiShot = g_pRoh->m_NextMultiShot;
				g_pRoh->m_NextMultiShot = 0;
				return false;
			}

			m_pUIMgr->dwDelayTime = dwTime;
		}

	case SKILL_DEEP_ATTACK:		// ?? ????
	case SKILL_CHAIN_ATTACK:	// ???? ????
		{
			if(m_pHeroAttGoMob )
				sprintf(g_commOut, "subat npc %d %d %d\n",
						m_pHeroAttGoMob->m_nVirId,			//?? ??????

						g_ChaSkill.m_SkillIndex[cur_skill],	//???? ????
						FALSE );							//????
			else if(m_pHeroAttGoCha)
				sprintf(g_commOut, "subat char %d %d %d\n",
						m_pHeroAttGoCha->m_nCharIndex,			//???? ??????

						g_ChaSkill.m_SkillIndex[cur_skill],	//???? ????
						FALSE );							//????

			m_pUIMgr->SendAttackMessage(g_commOut);
		}
		return true;

	//============================================================
	// "use_skill"
	// ???? : ???? ????
	case SKILL_B_BURNINGCRASH:	// nate 2005-06-09 : Burning Crash - ?????? ????
		{
			if( g_pRoh->m_byBurningStep )
			{
				// Send Net Messege
				if( m_pHeroAttGoMob )
				{	
					SendCharUseSkill_TargetValue2(SKILL_B_BURNINGCRASH, eCharType_Npc, pCMyApp->m_pHeroAttGoMob->m_nVirId, g_pRoh->m_byBurningStep, 0);
				}
				else if( m_pHeroAttGoCha )
				{	
					SendCharUseSkill_TargetValue2(SKILL_B_BURNINGCRASH, eCharType_Char, pCMyApp->m_pHeroAttGoCha->m_nCharIndex, g_pRoh->m_byBurningStep, 0);
				}				
			}
		}
		return true;

	//============================================================
	// "use_skill"
	// ???? : ???? ????
	case SKILL_H_AUTOMINE:	// nate 2005-06-28 : Auto Mine - ???????? ???????? ?????? ????
	case SKILL_A_LIFE_EXPANSION:	// nate 2005-07-18 : Life Expansion - AttackMsgSend
		{
			if(m_pHeroAttGoMob && li_selskill != SKILL_A_LIFE_EXPANSION)
			{	
				SendCharUseSkill_Target(li_selskill, eCharType_Npc, m_pHeroAttGoMob->m_nVirId);
			}			
			else if(m_pHeroAttGoCha)
			{	
				SendCharUseSkill_Target(li_selskill, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex);
			}			

			if( li_selskill == SKILL_H_AUTOMINE ) // ?????????? ???? ?? ?????? ?????????? ?????????? ?????? ?????????????? ????????. 07-03-22 ????
				G_SK_SetCooltime( SKILL_H_AUTOMINE );
		}
		return false;

	//============================================================
	// "use_skill" + ?????? ????
	// ???? : ?????? ????
	case SKILL_B_FLYINGCRASH: // muungmae 2005/11/16 : ?????? ??????
	case SKILL_F_SHADOWSLASH: // muungmae 2005/11/16 : ?????? ??????
		{
			if (m_pHeroAttGoMob )
			{	
				SendCharUseSkill_TargetValue3(li_selskill, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, 0, (int)m_pHeroAttGoMob->m_wx, (int)m_pHeroAttGoMob->m_wz);
			}
			if(m_pHeroAttGoCha)
			{	
				SendCharUseSkill_TargetValue3(li_selskill, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, 0, (float)m_pHeroAttGoCha->m_wx, (float)m_pHeroAttGoCha->m_wz);
			}			

			if( li_selskill == SKILL_B_FLYINGCRASH ) // ?????????????? ???? ?? ?????? ?????????? ?????????? ?????? ?????????????? ????????. 07-03-22 ????
				G_SK_SetCooltime( SKILL_B_FLYINGCRASH );
		}
		return FALSE;
	//============================================================
	case SKILLF_A_DASH:	// nate 2005-11-15 : ?????? ?????????? ???? ????
		return FALSE;

	//============================================================
	// ?????? ???? ???? ???? - ?????? ?????????? ???????? ???? ????
	case SKILL_C_HELLDOWN:
		{
			if( m_pUIMgr->m_bRClickDown == 1
					&& m_pUIMgr->m_bSendMessage == FALSE )
			{
				if(m_pHeroAttGoMob)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, m_nCurHellDown_Grade, 1);
				}
				else if(m_pHeroAttGoCha)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, m_nCurHellDown_Grade, 1);
				}				
				m_pUIMgr->m_bSendMessage = TRUE;
				m_pUIMgr->m_byCheckHellDown = 1;
				DrawCasting = TRUE;
				m_bHellDownReady = FALSE;
				m_helldown_ing = true;
				m_dPressTime = timeGetTime();
				return TRUE;
			}
			else if(m_pUIMgr->m_bRClickDown == 2
					&& m_pUIMgr->m_bSendMessage == FALSE
					&& m_bHellDownReady == TRUE	)//1?????? ?????? ??
			{				
				if(m_pHeroAttGoMob)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, m_nCurHellDown_Grade, 10);
				}
				else if(m_pHeroAttGoCha)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, m_nCurHellDown_Grade, 10);
				}			
				
				m_pUIMgr->m_bSendMessage = TRUE;
				m_pUIMgr->m_byCheckHellDown = 1;
				DrawCasting = FALSE;
				m_bHellDownReady = FALSE;
				m_nCurHellDown_Grade = 0;
				m_helldown_ing = false;
				G_SK_SetCooltime( SKILL_C_HELLDOWN );
				return TRUE;
			}
			else if(( m_pUIMgr->m_bRClickDown == 2
					  && pCMyApp->m_bHellDownReady == FALSE ))  //1?????? ??????????
			{
				if(m_pHeroAttGoMob)
				{	
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, -1, 0);
				}
				else if(m_pHeroAttGoCha)
				{
					SendCharUseSkill_TargetValue2(SKILL_C_HELLDOWN, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, -1, 0);
				}				
				m_pUIMgr->m_bRClickDown = 0;
				DrawCasting = FALSE;
				m_bHellDownReady = FALSE;
				m_pUIMgr->m_byCheckHellDown = 1;
				m_dPressTime = 0L;
				g_pRoh->m_bAttacked = FALSE;
				if (g_pDSound)
				{
					if( g_pRoh->m_Sex == 0)
						g_pDSound->Stop(g_EffectSound[EFF_SND_HELLDOWN_CAST_M]);
					else
						g_pDSound->Stop(g_EffectSound[EFF_SND_HELLDOWN_CAST_W]);
				}
				G_SK_SetCooltime( SKILL_C_HELLDOWN );
				return FALSE;
			}
		}
		return FALSE;
	//============================================================
	// "use_skill" + TargetCount + CoolTime
	// ???? ???? ????

	case SKILL_FIRE_BALL:			// ???????? - 2
	case SKILL_FIRE_PILLAR:			// ?????? - 4
	case SKILL_STAR_FLARE:			// ???? ?????? - 5
	case SKILL_TORNADO_RUIN:		// ???????? ????.  - 10
	case SKILL_K_POISON:			// nate 2005-11-08 : ?????? - AttackMsgSend - 10
	case SKILL_AIR_RAID:			// ?????????? - 10
	case SKILLF_A_BASH:				// ???? - 5
	case SKILL_HELL_CRY:			// Hell Cry - 10
	case SKILL_LIGHTNING_BALL:		// ???????? ??
	case SKILL_PEBBLE_SHOT:			// ??????
	case SKILL_LIGHTNING:			// ????????
	case SKILL_FALL_STONE:			// ????
	case SKILL_SHOCK_SPEAR:			// ???? ??????
	case SKILL_STONE_LANCE:			// ???? ????
		{
			if( li_selskill == SKILL_K_POISON && !g_pRoh->m_Wearing[WEARING_WEAPON] )
				return false;

			target_count = GetTargetCountOfSkill( cur_skill ) - 1;	// ???? ????

			if( li_selskill == SKILL_FIRE_BALL || li_selskill == SKILLF_A_BASH)
				area = 30;
			else
				area = 60;

			if( li_selskill == SKILL_HELL_CRY )
				fDist = 60;
			else
				fDist = GetMagicDistance(g_ChaSkill.m_SkillLevel[cur_skill]) + area;

			if(m_pHeroAttGoMob
					&& fDist >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoMob->m_wx, m_pHeroAttGoMob->m_wz))	// Mob ????
			{	
				std::vector<tINT> vecMobIndex;
				vecMobIndex.push_back(m_pHeroAttGoMob->m_nVirId);

				if(target_count > 0)
				{
					int nCount = m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoMob,  fDist, area, true, target_count);

					std::string temp;
					for (int i = 0; i < nCount; i++)
					{
						if ( li_selskill != SKILL_TORNADO_RUIN && li_selskill != SKILL_K_POISON )	// ???????? ?????? ???????? Effect?? ??????
							g_pRoh->AddMagicMob(g_pTargetMob[i]);
						
						vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
					}
				}

				SendCharUseSkill_MultiTarget(li_selskill, eCharType_Npc, vecMobIndex);
			}
			else if (m_pHeroAttGoCha
					 && fDist >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wz))	// Char ????
			{
				std::vector<tINT> vecMobIndex;
				vecMobIndex.push_back(m_pHeroAttGoCha->m_nCharIndex);				
				
				if (target_count > 0)
				{
					int nCount = m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoCha,  fDist, area, false, target_count);

					std::string temp;
					for (int i = 0; i < nCount; i++)
					{
						if ( li_selskill != SKILL_TORNADO_RUIN && li_selskill != SKILL_K_POISON )	// ???????? ?????? ???????? Effect?? ??????
							g_pRoh->AddMagicCha(g_pTargetCha[i]);
						
						vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
					}
				}
				
				SendCharUseSkill_MultiTarget(li_selskill, eCharType_Char, vecMobIndex);
			}
			else
			{
				return false;
			}

			G_SK_SetCooltime(li_selskill);			
		}
		return true;

	case SKILL_SUMMON_PARTITION:	// ???? ????
		{
			pMob = m_pUIMgr->m_pSummonMob[0];
			if(!pMob)
				return FALSE;

			att_count = g_Skills[g_ChaSkill.m_SkillIndex[cur_skill]].m_AttCount;

			if( m_pUIMgr->m_bOnBattle && g_MatrixSvr != 0 ) // ???????????? ???????? ?????? ????????????.(?????? ?????????? ?? ?????? ??????.)
			{
				if(m_pHeroAttGoMob) // ?????? ???? ??????.
				{
					SendCharUseSkill_Target(g_ChaSkill.m_SkillIndex[cur_skill], eCharType_Npc, m_pHeroAttGoMob->m_nVirId);
					G_SK_SetCooltime(li_selskill);
					return TRUE;
				}
				else if (m_pHeroAttGoCha)
				{	
					SendCharUseSkill_Target(g_ChaSkill.m_SkillIndex[cur_skill], eCharType_Char, m_pHeroAttGoCha->m_nCharIndex);
					G_SK_SetCooltime(li_selskill);
					return TRUE;
				}

				// ???? ???? ?????? ???? ?????? ???????? ??????.
			}
			if( g_pRoh->GetMyGuildData() != NULL &&
					m_pHeroAttGoCha != NULL &&
					g_pRoh->GetMyGuildData()->CheckInvasionGuild( m_pHeroAttGoCha->m_GuildIndex )  )
			{	
				SendCharUseSkill_Target(g_ChaSkill.m_SkillIndex[cur_skill], eCharType_Char, m_pHeroAttGoCha->m_nCharIndex);
				G_SK_SetCooltime(li_selskill);
				return TRUE;
			}
			// ???? ?????? ???? ?????? ???? ?????? ?????? ???? ?????? ?? ?????? ??????.
			if (att_count > 1)
			{
				area =60;
				att_range = 120;// ???? ????
				if( att_range >= DistPlToPlAbs(pMob->m_wx,pMob->m_wz, g_pRoh->m_wx, g_pRoh->m_wz) )
				{
					target_count = GetTargetCountOfSkill( cur_skill );
					// ?????????? ?????????? ?????? ???????? ????????
					if ( m_pUIMgr->bControlKey )
					{	
						target_count = m_pUIMgr->FindNearTarget(pMob->m_wx, pMob->m_wz, g_pRoh, att_range, area, false, target_count);

						if( target_count <= 0 )
							return FALSE;

						std::vector<tINT> vecMobIndex;
						
						for (int i = 0; i < target_count ; i++)
						{	
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						}

						SendCharUseSkill_MultiTarget(g_ChaSkill.m_SkillIndex[cur_skill], eCharType_Char, vecMobIndex);

						return TRUE;
					}
					else
					{	
						target_count = m_pUIMgr->FindNearTarget(pMob->m_wx, pMob->m_wz, pMob, att_range, area, true, target_count);

						if( target_count <= 0 )
							return FALSE;

						std::vector<tINT> vecMobIndex;

						for (int i = 0; i < target_count ; i++)
						{	
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
						}

						SendCharUseSkill_MultiTarget(g_ChaSkill.m_SkillIndex[cur_skill], eCharType_Npc, vecMobIndex);

						return TRUE;
					}
				}
				else
				{
					return FALSE;
				}
			}			
		}
		return FALSE;

	case SKILL_F_DASHSLASH: ///????????
		{
			if( (g_MatrixSvr != 0 && m_pUIMgr->m_bOnBattle) || g_MatrixSvr == 2  ) // ???????? ?????? ?????? ?????? ??????.
				target_count = 1;
			else
				target_count = 7; // ???? 7 ????

			if( g_pRoh->m_bNowFreePVP )
			{
				CNkCharacter* pCha = m_pUIMgr->FindChaById(g_pRoh->m_dwPvpDestIndex, FALSE);				
				if( pCha )
					pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, pCha, NULL, EFFECT_SKILL_DSLASH );

				SendCharUseSkill_Target(SKILL_F_DASHSLASH, eCharType_Char, (tINT)g_pRoh->m_dwPvpDestIndex);	
			}
			else if( IsPeaceArea() && !IsInsideRect(g_pRoh->m_wx, g_pRoh->m_wz, 6800, 5450, 7250, 5000) )
			{
				if(	g_pRoh->GetMyGuildData() != NULL &&
						( g_pRoh->GetMyGuildData()->IsInvasion_Attack() || g_pRoh->GetMyGuildData()->IsInvasion_Defense() ) )
				{	
					target_count = m_pUIMgr->FindLineTargetGuildWar( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, target_count, 1/*??????*/ ); // ???? ?????? ????.

					if( target_count == 0 ) // ?????? ?????? ?????? ???? ?????? ????.
						return FALSE;

					std::vector<tINT> vecMobIndex;
					
					for (int i = 0; i < target_count ; i++)
					{
						vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
						
						if( g_pTargetCha[i] )
							pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, g_pTargetCha[i], NULL, EFFECT_SKILL_DSLASH );
					}

					SendCharUseSkill_MultiTarget(SKILL_F_DASHSLASH, eCharType_Char, vecMobIndex);
				}
			}
			else if( GetCurWorldID() == WORLD_GUILD_WAR )
			{	
				target_count = m_pUIMgr->FindLineTargetGuildWar( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, target_count, 0/*??????*/); // ???? ?????? ????.

				if( target_count == 0 ) // ?????? ?????? ?????? ???? ?????? ????.
					return FALSE;

				std::vector<tINT> vecMobIndex;

				for (int i = 0; i < target_count ; i++)
				{	
					vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
					
					if( g_pTargetCha[i] )
						pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, g_pTargetCha[i], NULL, EFFECT_SKILL_DSLASH );
				}

				SendCharUseSkill_MultiTarget(SKILL_F_DASHSLASH, eCharType_Char, vecMobIndex);
			}
			else if ( m_pUIMgr->bControlKey ) // ?????????? ?????????? ?????? ???????? ????????
			{	
				target_count = m_pUIMgr->FindLineTarget( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, false, target_count ); // ???? ?????? ????.

				if( target_count == 0 ) // ?????? ?????? ?????? ???? ?????? ????.
					return FALSE;

				std::vector<tINT> vecMobIndex;

				for (int i = 0; i < target_count ; i++)
				{
					vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);					
					
					if( g_pTargetCha[i] )
						pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, g_pTargetCha[i], NULL, EFFECT_SKILL_DSLASH );
				}

				SendCharUseSkill_MultiTarget(SKILL_F_DASHSLASH, eCharType_Char, vecMobIndex);
			}
			else
			{
				if(	g_pRoh->GetMyGuildData() != NULL &&
						( g_pRoh->GetMyGuildData()->IsInvasion_Attack() || g_pRoh->GetMyGuildData()->IsInvasion_Defense() ) )
				{
					int cha_target_count = m_pUIMgr->FindLineTargetGuildWar( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, target_count, 1/*??????*/); // ???? ?????? ????.

					if(cha_target_count >= 7 )
					{
						if( target_count <= 0 )
							return FALSE;

						std::vector<tINT> vecMobIndex;

						for (int i = 0; i < target_count ; i++)
						{	
							vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);
							
							if( g_pTargetCha[i] )
								pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, g_pTargetCha[i], NULL, EFFECT_SKILL_DSLASH );
						}

						SendCharUseSkill_MultiTarget(SKILL_F_DASHSLASH, eCharType_Char, vecMobIndex);
					}
					else if( cha_target_count <= 0 )
					{	
						target_count = m_pUIMgr->FindLineTarget( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, true, target_count ); // ???? ?????? ????.

						if( target_count <= 0 ) // ?????? ?????? ?????? ???? ?????? ????.
							return FALSE;

						std::vector<tINT> vecMobIndex;
						
						for (int i = 0; i < target_count ; i++)
						{
							vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
							
							if( g_pTargetMob[i] )
								pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, g_pTargetMob[i], EFFECT_SKILL_DSLASH );
						}

						SendCharUseSkill_MultiTarget(SKILL_F_DASHSLASH, eCharType_Npc, vecMobIndex);
					}
					else
					{	
						int tempcount = 7 - cha_target_count;
						int mob_target_count = m_pUIMgr->FindLineTarget( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, true, tempcount ); // ???? ?????? ????.
												
						std::vector<tINT> vecCharIndex;
						std::vector<tINT> vecMobIndex;

						for(int i = 0; i < cha_target_count; i++)
						{	
							vecCharIndex.push_back(g_pTargetCha[i]->m_nCharIndex);

							if( g_pTargetCha[i] )
								pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, g_pTargetCha[i], NULL, EFFECT_SKILL_DSLASH );
						}

						if( mob_target_count > 0)
						{	
							for( int i=0; i<mob_target_count; i++)
							{
								vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
								
								if( g_pTargetMob[i] )
									pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, g_pTargetMob[i], EFFECT_SKILL_DSLASH );
							}
						}
						
						SendCharUseSkill_MultiTargetAll(SKILL_F_DASHSLASH, vecCharIndex, vecMobIndex);
					}
				}
				else
				{	
					target_count = m_pUIMgr->FindLineTarget( g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->GetToX(), g_pRoh->GetToZ(), 15.0f, true, target_count ); // ???? ?????? ????.

					if( target_count <= 0 ) // ?????? ?????? ?????? ???? ?????? ????.
						return FALSE;

					std::vector<tINT> vecMobIndex;
					
					for (int i = 0; i < target_count ; i++)
					{	
						vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);

						if( g_pTargetMob[i] )
							pCMyApp->m_pUIMgr->InsertMagic( NULL, NULL, NULL, g_pTargetMob[i], EFFECT_SKILL_DSLASH );
					}

					SendCharUseSkill_MultiTarget(SKILL_F_DASHSLASH, eCharType_Npc, vecMobIndex);
				}
			}			
		}
		return TRUE;

	case SKILL_F_STONEBASH1: // ???? ???? ???? /// ????????
		{	
			SendCharUseSkill_StoneBash1(SKILL_F_STONEBASH1, g_pRoh->m_resmobidx);
			G_SK_SetCooltime( SKILLF_A_BASH );  // ?????? ???????? ??????.
		}
		return FALSE; // ???? FALSE ???????? ???? ???????? ????????.

	case SKILL_F_STONEBASH2: // ???? ???? ???? ///????????
		{
			fDist = 0; // ???? ????.(???? FindNearTarget ?????? ???? ??????????..)
			area = 60;  // ???? ????.(???? ???? ????) 60 ?? ???? ?????? ???? ????

			if( (g_MatrixSvr != 0 && m_pUIMgr->m_bOnBattle) || g_MatrixSvr == 2 ) // ???????? ?????? ?????? ?????? ??????.
				target_count = 1;
			else
				target_count = 5; // ???? 5 ????

			if ( m_pUIMgr->bControlKey ) // ?????????? ?????????? ?????? ???????? ????????
			{	
				target_count = m_pUIMgr->FindNearTarget( g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoCha,  fDist, area, false, target_count, true ); // ???? ???????? ???? ???????? ?????????? ???? ?????? ???? ???????? ????????????. (06-11-25 ???? ???????? ?????? ?????????????? ???????? ??????????)

				g_pRoh->m_MultiTargetKind = MULTITARGET_KIND_CHAR; // ?????? ????.

				std::vector<tINT> vecMobIndex;

				for (int i = 0; i < target_count ; i++)
				{	
					vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);

					// ???? ?????? ???? ???? ???????? ??????????.
					g_pRoh->m_MultiTargetArray[i] = g_pTargetCha[i]->m_nCharIndex;
					g_pRoh->m_MultiTargetPos[i].x = g_pTargetCha[i]->m_wx;
					g_pRoh->m_MultiTargetPos[i].y = g_pTargetCha[i]->m_wz;
				}

				SendCharUseSkill_MultiTarget(SKILL_F_STONEBASH2, eCharType_Char, vecMobIndex);
			}
			else
			{	
				target_count = m_pUIMgr->FindNearTarget( g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoMob,  fDist, area, true, target_count, true ); // m_pHeroAttGoMob ?? ?????? ??????????????.

				g_pRoh->m_MultiTargetKind = MULTITARGET_KIND_MOB; // ?? ????.

				std::vector<tINT> vecMobIndex;

				for (int i = 0; i < target_count ; i++)
				{	
					vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);

					// ???? ?????? ???? ???? ?????? ?????? ??????????.
					g_pRoh->m_MultiTargetArray[i] = g_pTargetMob[i]->m_nVirId;
					g_pRoh->m_MultiTargetPos[i].x = g_pTargetMob[i]->m_wx;
					g_pRoh->m_MultiTargetPos[i].y = g_pTargetMob[i]->m_wz;
				}

				SendCharUseSkill_MultiTarget(SKILL_F_STONEBASH2, eCharType_Npc, vecMobIndex);
			}			

			g_pRoh->m_MultiTargetNum = target_count; // m_nComboFrame ?? ???? ???????? ??????????.(???? SKILL_F_BASHREFLECT ?? ???????? ??????.)
		}

		return FALSE; // ???? FALSE ???????? ???? ???????? ????????.

	case SKILL_F_BASHREFLECT1: // ???? ???? ///????????

		{
			g_pRoh->m_bComboSucceed = FALSE;
			m_pUIMgr->m_b_MotionForAttack = TRUE; // ???? ?????? ?? ???????????? ?????? ????????.

			SendCharUseSkill(SKILL_F_BASHREFLECT1);

			G_SK_SetCooltime( SKILLF_E_REFLECTION );  // ?????? ???????? ???????? ??????.
		}

		break;

	case SKILL_F_BASHREFLECT2: // ???? ???? ///????????
		{

			// ?? ?????? ?????? ???? ?????? ?????? ???? ?????? ?????????? ???? ???????? ???????? ????????.
			if( g_pRoh->m_MultiTargetKind == MULTITARGET_KIND_CHAR && g_pRoh->m_MultiTargetNum ) // ?????? ?????? ??????...
			{
				std::vector<tINT> vecMobIndex;
				
				for (int i = 0; i < g_pRoh->m_MultiTargetNum ; i++)
				{
					vecMobIndex.push_back(g_pRoh->m_MultiTargetArray[i]);					
				}

				SendCharUseSkill_MultiTarget(SKILL_F_BASHREFLECT2, eCharType_Char, vecMobIndex);
			}
			else if( g_pRoh->m_MultiTargetKind == MULTITARGET_KIND_MOB && g_pRoh->m_MultiTargetNum ) // ?????? ?????? ??????...(???? ?????? ???? ????)
			{
				std::vector<tINT> vecMobIndex;
				
				for ( int i = 0; i < g_pRoh->m_MultiTargetNum ; i++)
				{
					vecMobIndex.push_back(g_pRoh->m_MultiTargetArray[i]);					
				}

				SendCharUseSkill_MultiTarget(SKILL_F_BASHREFLECT2, eCharType_Npc, vecMobIndex);
			}			

			g_pRoh->m_bComboSucceed = TRUE;
		}

		break;
	case SKILL_F_ELECTRICHAMMER:
	case SKILL_F_TORNADO_HAMMER:
	case SKILL_A_SUMMONFIRE:
	case SKILL_H_FROZENSHOT:
	case SKILL_F_MERCERNERY_SLASH: //???????? ??????
	case SKILL_H_RANDOMFIRE:
	case SKILL_A_RESURRECTION:
		{
			// Send Net Messege
			if( m_pHeroAttGoMob )
			{	
				SendCharUseSkill_TargetValue2(li_selskill, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, 1, 1);
			}
			else if( m_pHeroAttGoCha )
			{	
				SendCharUseSkill_TargetValue2(li_selskill, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, 1, 1);
			}
			
			if( li_selskill != SKILL_A_SUMMONFIRE ) // [1/15/2008 ??????] ?????????? ???????? ???? ?????? ????
			{
				G_SK_SetCooltime( li_selskill );
			}
		}
		return TRUE;
	case SKILL_K_FLAMESPEAR: // ?????? ??????.
		{

			if( !m_pHeroAttGoCha && !m_pHeroAttGoMob ) // ?????? ??????.
				return FALSE; // ???? ?????? ??????.

			// use_skill ?? ???????? ??????????.
			if( m_pHeroAttGoCha )
			{	
				SendCharUseSkill_TargetValue2(SKILL_K_FLAMESPEAR, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, 1, 1);
			}
			else if( m_pHeroAttGoMob )
			{	
				SendCharUseSkill_TargetValue2(SKILL_K_FLAMESPEAR, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, 1, 1);
			}
			G_SK_SetCooltime( li_selskill );
		}
		break;; // ?????????????? ???????? ???? ?????? ???????????? ?????? ???? ?????? ??????.(?? TRUE????????.) use_skill 1???? ?? ?????? click ?? ????????, ???? ?????? ???? ?????? ????????.

	case SKILL_K_FLAMESPEAR_TARGET: // ?????? ?????? ???? ???? ??????. ???????????? ?????? ?????????? ??????.
		{

			if( !m_pHeroAttGoCha && !m_pHeroAttGoMob ) // ?????? ??????.
				return FALSE; // ???? ?????? ??????.

			fDist = 0; // ???? ????.(???? FindNearTarget ?????? ???? ??????????..)
			area = 60;  // ???? ????.(???? ???? ????) 60 ?? ???? ?????? ???? ????

			if( (g_MatrixSvr != 0 && m_pUIMgr->m_bOnBattle) || g_MatrixSvr == 2  ) // ???????? ?????? ?????? ?????? ??????.
				target_count = 1;
			else
				target_count = 9;	// ?? ?????? ???????? ???????? ?????? ???? 10 ??????.

			if ( m_pHeroAttGoCha ) // ?? ?????? ?????? ?????? ???????? ???????? ?????? ???? ???????? ?????? ??????. ???? ???????? ?????? ???? ?????? ???????? ???????? ?????? ???????? ???????????? ????????.
			{
				target_count = m_pUIMgr->FindNearTarget( m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wz, m_pHeroAttGoCha,  fDist, area, false, target_count ); // ???? ???????? ???? ???????? ?????????? ???? ?????? ???? ???????? ????????????.
				
				std::vector<tINT> vecMobIndex;

				vecMobIndex.push_back(m_pHeroAttGoCha->m_nCharIndex);

				for (int i = 0; i < target_count ; i++)
				{
					vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);					
				}

				SendCharUseSkill_MultiTargetStep(SKILL_K_FLAMESPEAR, eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, 1, vecMobIndex);
			}
			else if( m_pHeroAttGoMob )
			{
				target_count = m_pUIMgr->FindNearTarget( m_pHeroAttGoMob->m_wx, m_pHeroAttGoMob->m_wz, m_pHeroAttGoMob,  fDist, area, true, target_count ); // m_pHeroAttGoMob ?? ?????? ??????????????.
				
				std::vector<tINT> vecMobIndex;

				vecMobIndex.push_back(m_pHeroAttGoMob->m_nVirId);

				for (int i = 0; i < target_count ; i++)
				{
					vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
				}

				SendCharUseSkill_MultiTargetStep(SKILL_K_FLAMESPEAR, eCharType_Npc, m_pHeroAttGoMob->m_nVirId, 1, vecMobIndex);
			}			
		}

		break;

	case PEROM_SKILL_S127:
	case PEROM_SKILL_S128:
	case PEROM_SKILL_S129:
	case PEROM_SKILL_S130:
	case PEROM_SKILL_S131:
	case PEROM_SKILL_S132:
	case PEROM_SKILL_S133:
	case PEROM_SKILL_S135:
	case PEROM_SKILL_S136:
	case PEROM_SKILL_S137:
	case PEROM_SKILL_S138:
	case PEROM_SKILL_S140:
		{
			target_count = GetTargetCountOfSkill( cur_skill ) - 1;
			area = 30;
			fDist = GetMagicDistance(g_ChaSkill.m_SkillLevel[cur_skill]) + area;

			if(m_pHeroAttGoMob
					&& fDist >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoMob->m_wx, m_pHeroAttGoMob->m_wz))	// Mob ????
			{
				std::vector<int> vecMobIndex;
				vecMobIndex.push_back(m_pHeroAttGoMob->m_nVirId);

				if(target_count > 0)
				{
					int nCount = m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoMob,  fDist, area, true, target_count);

					for (int i = 0; i < nCount; i++)
					{
						vecMobIndex.push_back(g_pTargetMob[i]->m_nVirId);
					}
				}

				SendCharUseSkill_MultiTarget(li_selskill, eCharType_Npc, vecMobIndex);
			}
			else if (m_pHeroAttGoCha
					 && fDist >= DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wz))
			{
				std::vector<int> vecMobIndex;
				vecMobIndex.push_back(m_pHeroAttGoCha->m_nCharIndex);

				if (target_count > 0)
				{
					int nCount = m_pUIMgr->FindNearTarget(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoCha,  fDist, area, false, target_count);
										
					for (int i = 0; i < nCount; i++)
					{
						vecMobIndex.push_back(g_pTargetCha[i]->m_nCharIndex);						
					}
				}

				SendCharUseSkill_MultiTarget(li_selskill, eCharType_Char, vecMobIndex);
			}
			else
				return false;

			G_SK_SetCooltime(li_selskill);			
		}
		return true;

	default :
		{
			if(g_pRoh->m_Race == RACE_FREAK)
			{
				if(g_ChaSkill.m_SkillLevel[2] == 30)
					g_pRoh->m_bhaste = 1;
				else
					g_pRoh->m_bhaste = 0;
			}
			
			if(m_pHeroAttGoMob)
			{
				SendCharAttack(eCharType_Npc, m_pHeroAttGoMob->m_nVirId, g_pRoh->m_bhaste);				
			}
			if(m_pHeroAttGoCha)
			{
				SendCharAttack(eCharType_Char, m_pHeroAttGoCha->m_nCharIndex, g_pRoh->m_bhaste);				
			}			
		}
		break;
	}
	return true;
}

// ?? ?????? ???? ???????? ??????.
// ???????? Scroll ?? ?????????? ?????? ???????? ?????? ?? ?????? ????.
// ?????? ?????? ?? ???? ?????? ???? ????????.
int CMyD3DApplication::GetSkillDistance(int cur_skill)
{
	int att_range = ATTACK_RANGE;
	int li_selskill = 0;
	// ?????? ????
	if(cur_skill<0)
		return 0;
	// Get Selected Skill
	li_selskill = g_ChaSkill.m_SkillIndex[cur_skill];

	// Chakram
	if(m_pUIMgr->m_bChakram)
	{
		att_range = CHAKRAM_ATTACK_RANGE;
		return att_range;
	}
	// BookSpell : ??????

	if(m_pUIMgr->m_bBookSpell)
	{
		att_range = 600;
		return att_range;
	}

	//???????? ?????? ???? ???? ????(mungmae-3/22)
	if( m_nCurWorldIdx == WORLD_MATRIX )
	{
		if( m_pHeroAttGoMob )
		{
			if( m_pHeroAttGoMob->m_nMobNum == 371 || m_pHeroAttGoMob->m_nMobNum == 372 || m_pHeroAttGoMob->m_nMobNum == 373 )
				att_range = GROUND_RANGE;
			if( m_pHeroAttGoMob->m_nMobNum == 374 || m_pHeroAttGoMob->m_nMobNum == 375 || m_pHeroAttGoMob->m_nMobNum == 376 )
				att_range = 60;//GP_RANGE;
			if( m_pHeroAttGoMob->m_nMobNum == 377 || m_pHeroAttGoMob->m_nMobNum == 378 || m_pHeroAttGoMob->m_nMobNum == 379 )
				att_range = GATE_RANGE;
		}
	}
	if( m_nCurWorldIdx == WORLD_MATRIX_2009 )
	{
		if( m_pHeroAttGoMob )
		{
			switch(m_pHeroAttGoMob->m_nMobNum )
			{
			case 623:
			case 624:	//	????????
				att_range = GROUND_RANGE;
				break;
			case 625:
			case 626:
			case 627:	// GP ????

				att_range = 60;
				break;
			case 628:
			case 629:
			case 630:
			case 631:
			case 632:	// G ????

				att_range = 60;
				break;
			}
		}
	}
	switch(li_selskill)
	{
	//----------------------------------------------------------------------
	// BULKAN SKILL Distance
	//----------------------------------------------------------------------
	case SKILL_B_BURNINGCRASH:	// nate 2005-06-18 : Burning Crash - ???? ???? ????
		att_range = g_pRoh->m_Wearing[WEARING_CLOAK]->m_Num5 - 20;
		break;
	case SKILL_THROW : // Slayer
		att_range = g_pRoh->m_Wearing[WEARING_CLOAK]->m_Num5;
		break;
	case SKILL_FORCE_ATTACK : // Force Attack
		att_range += g_Skills[li_selskill].GetApp1(g_ChaSkill.m_SkillLevel[cur_skill]);
		break;
	case SKILL_HELL_CRY : // Hell Cry
	case SKILL_F_SPACESLASH :
		att_range = HELL_ATTACK_RANGE ;
		break;
	//----------------------------------------------------------------------
	// HUMAN SKILL Distance
	//----------------------------------------------------------------------
	case SKILL_HANDLE     : // ??????
		att_range += g_pRoh->m_Wearing[WEARING_CLOAK]->m_Num5;
		break;
	case SKILL_DEEP_ATTACK  : // ?? ????
	case SKILL_CHAIN_ATTACK : // ???? ????
		att_range += g_Skills[li_selskill].GetApp1(g_ChaSkill.m_SkillLevel[cur_skill]);
		break;
	case SKILL_AIR_RAID :     // ??????????
		att_range += 150;
		break;
	case SKILL_SNIFING :      // ????
		att_range += GET_ATT_RANGE(g_pRoh->m_Wearing[WEARING_WEAPON]);
		att_range += GetSnifingRange(g_ChaSkill.GetLevelByIndex(SKILL_SNIFING));
		break;
	case SKILL_H_AUTOMINE:	// nate 2005-06-23 : Auto Mine - skill ???????? ????
	case SKILL_A_SUMMONFIRE:
		att_range += 135;
		break;
	case SKILL_C_HELLDOWN:
		att_range += 200;
		break;
	//----------------------------------------------------------------------
	// KAI SKILL Distance
	//----------------------------------------------------------------------
	case SKILL_MAGIC_MOVE: // ????????
		att_range += g_pRoh->m_Wearing[WEARING_SHIELD]->m_Num5;
		break;
	//----------------------------------------------------------------------
	// AIDIA SKILL Distance
	//----------------------------------------------------------------------
	case SKILL_A_RESURRECTION: ///070723 ?????????????? ????
		att_range += g_Skills[li_selskill].GetApp1(g_ChaSkill.m_SkillLevel[cur_skill]);
		break;
	//----------------------------------------------------------------------
	// ?????? SKILL Distance
	//----------------------------------------------------------------------
	case SKILLF_A_DASH: // ????
		att_range = 70 +(int(ceil( (float)(g_ChaSkill.m_SkillLevel[cur_skill]) *5.0f )));
		break;
	case SKILLF_A_TIMECHARGE:   // ????????
	case SKILLF_A_DSLASH:       // ???? ??????
		att_range = 62;
		break;
	case SKILL_B_FLYINGCRASH: //mungmae: 2005/11/16 ?????? ?????? ???? ????
	case SKILL_F_SHADOWSLASH: //mungmae: 2005/11/18 ?????? ?????? ???? ????
	case SKILL_H_ELECTRICSHOT:	// nate 2005-11-24 : ???????? ??
		att_range = 70 +(int(ceil( (float)(g_ChaSkill.m_SkillLevel[cur_skill]) *5.0f )));
		break;

	default :
		if( g_pRoh->m_Race == RACE_BULKAN || g_pRoh->m_Race == RACE_FREAK)
		{
			if(g_pRoh->m_Wearing[WEARING_WEAPON])
				att_range += GET_ATT_RANGE(g_pRoh->m_Wearing[WEARING_WEAPON]);
		}
		else if( g_pRoh->m_Race == RACE_KAILIPTON )
		{
			// ???? ???? ???? ?????? ????

			if(cur_skill == 0
					&& g_pRoh->m_Wearing[WEARING_WEAPON]
					&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
					&&g_pRoh->m_Epower >= g_pRoh->m_Wearing[WEARING_WEAPON]->m_Num4 )
				att_range += GET_ATT_RANGE(g_pRoh->m_Wearing[WEARING_WEAPON]);

			if(cur_skill > 0)
				att_range = GetMagicDistance(g_ChaSkill.m_SkillLevel[cur_skill]);
		}
		else if( g_pRoh->m_Race == RACE_HUMAN )
		{
			// ?????? ???? ???? ????????.
			if(g_pRoh->m_Wearing[WEARING_WEAPON])
				att_range += GET_ATT_RANGE(g_pRoh->m_Wearing[WEARING_WEAPON]);
		}
		else if( g_pRoh->m_Race == RACE_AIDIA )
		{
			// ?????? ???? ???? ????????.
			if(g_pRoh->m_Wearing[WEARING_WEAPON])
				att_range += GET_ATT_RANGE(g_pRoh->m_Wearing[WEARING_WEAPON]);
		}
		else if( g_pRoh->m_Race == RACE_PEROM )
		{
			if(g_pRoh->m_Wearing[WEARING_WEAPON])
				att_range += GET_ATT_RANGE(g_pRoh->m_Wearing[WEARING_WEAPON]);
		}
		break;
	}
	// ?????? ???? ???? ????
	// flag ???? ???????? ??????.
	if(m_pHeroAttGoMob)
	{
		/////////////////////////////////////////////////////////////////
		// ??????   : ??????
		// ??????   : 05-03-31
		// ???????? : ???????? ?????? ?????? ???????????? //DEC_TOWER
		if( pCMyApp->GetCurWorld() == 0 && pCMyApp->m_pUIMgr->m_bOnBattle )
		{
			if (m_pHeroAttGoMob->m_nMobNum == 326 ) // ???????? ????

			{
				// att_range += TOWER_RANGE; // ???????? ?????? ?????? ?????????? ??????????.
			}
			if (m_pHeroAttGoMob->m_nMobNum == 327 ) // ??????.(???????? ?????? ?? ???? ???????? ???? ????)
				att_range += TOWER_RANGE+40;

			if( m_pHeroAttGoMob->m_nMobNum == 328 || m_pHeroAttGoMob->m_nMobNum == 329 )
				att_range += TOWER_RANGE+20;
		}
		if (m_pHeroAttGoMob->m_nMobNum == 170
				|| m_pHeroAttGoMob->m_nMobNum == 307
				|| m_pHeroAttGoMob->m_nMobNum == 311
				|| ( m_pHeroAttGoMob->m_nMobNum >= 179
					 && m_pHeroAttGoMob->m_nMobNum <= 187))
			att_range += TOWER_RANGE;
	}

	// ???? ???????? Skill ?? Setting ????.
	G_DevAddSkillProp(0, li_selskill);
	G_DevAddSkillProp(1, att_range);

	return att_range;
}
//--------------------------------------------------------------------------
// Compare Hero Attack / Go
//--------------------------------------------------------------------------
// Lyul Skill!!!!
BOOL CMyD3DApplication::CompareHeroAttGo()
{
	int cur_skill;            // ???? ?????? ?????? ???? ????.
	int Selected_Skill;           // ?????? Skill
	int block_1_exit = false; // block 1?? ???????? ???? Flag
	bool b_can_use_skill = false; // ???? ?????? Skill ?? ?????? ?? ??????.
	float fDist = 0.0f;       // ???????? ????
	float att_Dist =0.0f;    // ???? ???? ????.= ATTACK_RANGE;

	// ???????? ?????? return false
	if(!m_pUIMgr->m_b_Attack
			&& !m_pUIMgr->m_b_SecAttack
			&& !m_pUIMgr->m_b_MovingForAttack
			&& !m_pUIMgr->m_bChakramAtt
			&& !m_pUIMgr->m_bBookSpellAtt)
		return false;

	//--------------------------------------------------------------------------
	// ???????? ?????? ???? ????.
	//--------------------------------------------------------------------------
	if (m_pHeroAttGoMob || m_pHeroAttGoCha)
	{
		// ???? ???? ????.
		if(m_pHeroAttGoMob && (m_pHeroAttGoMob->IsDead() || g_pRoh->IsDead() || !m_pLand->IsMovableArea(g_pRoh->m_wx, g_pRoh->m_wz)
							   ||(g_pRoh->m_CancelState==1 && (
									  g_pRoh->m_ChaCondition==CONDITION_PARALYSIS ||
									  g_pRoh->m_ChaCondition==CONDITION_ENTANGLE ) )
							   //wan:2005-02	???????? ???? ?????? ?????? ?? ???? ????

							   || (m_pHeroAttGoMob->m_MobClassType==4 &&!(m_nCurSkill && g_ChaSkill.m_SkillIndex[g_pNk2DFrame->GetRSkill()] == SKILL_LIFE_LIGHT ) ) ) )
		{
			m_pUIMgr->m_b_SecAttack  = false;
			m_pUIMgr->m_b_Attack  = false;
			m_pUIMgr->m_b_MovingForAttack = false;
			m_pUIMgr->m_bChakramAtt = false;
			m_pUIMgr->m_bBookSpellAtt = false;
			SetHeroAttGoMob(NULL);
			return FALSE;
		}

		if(m_pHeroAttGoCha && ( m_pHeroAttGoCha->IsDead() || g_pRoh->IsDead() || !m_pLand->IsMovableArea(g_pRoh->m_wx, g_pRoh->m_wz) // ???? ???????? ???????? ?????? ?????????? ???? ????. ///07-05-31 ????
								||(g_pRoh->m_CancelState==1 && (
									   g_pRoh->m_ChaCondition==CONDITION_PARALYSIS ||
									   g_pRoh->m_ChaCondition==CONDITION_ENTANGLE ))))
		{
			m_pUIMgr->m_b_SecAttack  = false;
			m_pUIMgr->m_b_Attack  = false;
			m_pUIMgr->m_b_MovingForAttack = false;
			m_pUIMgr->m_bChakramAtt = false;
			m_pUIMgr->m_bBookSpellAtt = false;
			SetHeroAttGoCha(NULL);
			return FALSE;
		}


		//----------------------------------------------------------------------
		// Logic Block 1 ???? ????
		//----------------------------------------------------------------------
		do
		{
			//--------------------------------------------------------------------
			// Cmpare Block SecAttack ?????? ?????? R????.
			if(m_pUIMgr->m_b_SecAttack)
			{
				cur_skill = g_pNk2DFrame->GetRSkill();
			}
			// ???? ?????? ????.
			else
			{
				if ( m_nCurSkill == 0 || m_pUIMgr->m_bBookSpell )
					cur_skill = g_pNk2DFrame->GetLSkill();
				else
					cur_skill = g_pNk2DFrame->GetRSkill();
			}

			// Error ????.
			if(cur_skill < 0) cur_skill = 0;

			//--------------------------------------------------------------------
			// Skill ???? ?????? ???? ????.
			b_can_use_skill = SkillConditionCheck(cur_skill);

			//--------------------------------------------------------------------
			// Cmpare Block 2 ???? ????.
			if(b_can_use_skill)	// ???? ????????
				block_1_exit = true;
			else // ???? ???? ??????.
			{
				//------------------------------------------------------------------
				// Cmpare Block 3 ?????????? ???? . ????
				if(m_pUIMgr->m_b_SecAttack)
				{
					m_pUIMgr->m_b_SecAttack = false;
					m_pUIMgr->m_b_Attack = true;
				}
				// ??????.
				else if (m_pUIMgr->m_bChakramAtt || m_pUIMgr->m_bChakram)
				{
					m_pUIMgr->m_bChakramAtt = false;
					m_pUIMgr->m_bChakram = false;
					m_pUIMgr->m_b_Attack = true;
				}
				else if (m_pUIMgr->m_bBookSpellAtt || m_pUIMgr->m_bBookSpell)
				{
					m_pUIMgr->m_bBookSpellAtt = false;
					m_pUIMgr->m_bBookSpell = false;
					m_pUIMgr->m_b_Attack = true;
				}
				else if (m_nCurSkill==1)
				{
					if( g_pRoh->m_Race == RACE_BULKAN && cur_skill == 11 )	// nate 2005-08-18 : ?????? ?????????? ???? ????????
					{
						if( g_pRoh->m_Wearing[WEARING_WEAPON]
								&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE )
							cur_skill = 3;
						else if( g_pRoh->m_Wearing[WEARING_WEAPON]
								 && GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD )
							cur_skill = 5;
						else
							cur_skill = 0;

						block_1_exit = true; // ???? ???? ????

					}
					else
					{
						m_pUIMgr->m_b_SecAttack = false;
						m_pUIMgr->m_b_Attack = false;
						return false;
					}
				}
				else
				{
					if( g_pRoh->m_Race == RACE_BULKAN && cur_skill == 11 )	// nate 2005-08-18 : ?????? ?????????? ???? ????????
					{
						if( g_pRoh->m_Wearing[WEARING_WEAPON]
								&& GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_AXE )
							cur_skill = 3;
						else if( g_pRoh->m_Wearing[WEARING_WEAPON]
								 && GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_SWORD )
							cur_skill = 5;
						else
							cur_skill = 0;
					}
					else
						cur_skill = 0;

					block_1_exit = true; // ???? ???? ????

				}
			}
		}
		while(!block_1_exit);

		//-----------------------------------------------------------------------
		// ???????????? ???????? ???????? ????

		if (cur_skill == 0)
		{
			// Gun
			if(g_pRoh->m_Wearing[WEARING_WEAPON] && GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_GUN
					&& g_pRoh->m_Epower < g_pRoh->m_Wearing[WEARING_WEAPON]->m_Num4 )
			{
				m_pUIMgr->m_b_Attack = false;
				m_pUIMgr->m_b_MovingForAttack = false;
				m_pUIMgr->m_b_SecAttack = false;
				m_pUIMgr->m_bChakramAtt = false;
				m_pUIMgr->m_bChakram = false;
				m_pUIMgr->m_bBookSpellAtt = false;
				m_pUIMgr->m_bBookSpell = false;
				return false;
			}

			// Ring
			if(g_pRoh->m_Wearing[WEARING_WEAPON] && GET_SHAPE(g_pRoh->m_Wearing[WEARING_WEAPON]) == IWEAPON_RING
					&& g_pRoh->m_Wearing[WEARING_WEAPON]->m_Num4 > g_pRoh->m_Mana )
			{
				m_pUIMgr->m_b_Attack = false;
				m_pUIMgr->m_b_MovingForAttack = false;
				m_pUIMgr->m_b_SecAttack = false;
				m_pUIMgr->m_bChakramAtt = false;
				m_pUIMgr->m_bChakram = false;
				m_pUIMgr->m_bBookSpellAtt = false;
				m_pUIMgr->m_bBookSpell = false;
				return false;
			}
		}
		//-----------------------------------------------------------------------
		// ?????? ?????? ????.
		if( !g_ChaSkill.m_SkillIndex )
			return false;
		Selected_Skill = g_ChaSkill.m_SkillIndex[cur_skill];

		// ??????.(chakram)
		if(m_pUIMgr->m_bChakram)
			Selected_Skill = SKILL_THROW;
		// ?? ???? (bookspell)
		if(m_pUIMgr->m_bBookSpell)
			Selected_Skill = SKILL_BOOK_READ;

		//----------------------------------------------------------------------
		// Logic Block 2 ???? ????.
		//----------------------------------------------------------------------
		float fDist;
		if(m_pHeroAttGoCha)
			fDist = DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wz);
		if(m_pHeroAttGoMob)
			fDist = DistPlToPlAbs(g_pRoh->m_wx, g_pRoh->m_wz, m_pHeroAttGoMob->m_wx, m_pHeroAttGoMob->m_wz);

		// nate 2005-05-20 : ????????

		if( g_pRoh->m_Wearing[ WEARING_WEAPON ]
				&& ( g_pRoh->m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1259
					 || g_pRoh->m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1260
					 || g_pRoh->m_Wearing[ WEARING_WEAPON ]->m_Vnum == 1261
					 || g_pRoh->m_Wearing[ WEARING_WEAPON ]->m_Vnum == 2170	// [2008/11/24 Theodoric] WOR<D_MATRIX_2009
					 || g_pRoh->m_Wearing[ WEARING_WEAPON ]->m_Vnum == 2171))
			att_Dist = 145;
		else
			att_Dist = GetSkillDistance(cur_skill);

		//-----------------------------------------------------------------------
		// Cmpare Block 1  ?????? ?????? ????
		if(att_Dist < fDist )
		{
			// Shift ???? ???? ???????? ???????? return false
			if(GetKeyState(VK_SHIFT) & 0x80)
			{
				m_pUIMgr->m_b_Attack = false;
				m_pUIMgr->m_b_MovingForAttack = false;
				m_pUIMgr->m_b_SecAttack = false;
				m_pUIMgr->m_bChakramAtt = false;
				m_pUIMgr->m_bChakram = false;
				m_pUIMgr->m_bBookSpellAtt = false;
				m_pUIMgr->m_bBookSpell = false;
				return false;
			}
			// Compare Block 2 ???????? ???? ?????? ????.
			if(!m_pUIMgr->m_b_MovingForAttack)
			{
				// ???? ?? ???? ?????????? ?????? ???? : ???? ???? (MotionForattack==1 ??????)
				// SecAttack ????
				if(!m_pUIMgr->m_b_MotionForAttack || m_pUIMgr->m_b_SecAttack )
				{
					m_pUIMgr->m_b_MovingForAttack = true;
					if(m_pHeroAttGoMob)MoveHero( m_pHeroAttGoMob->m_wx, m_pHeroAttGoMob->m_wz);
					if(m_pHeroAttGoCha)MoveHero( m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wz);
				}
				else
					m_pUIMgr->m_b_Attack = false;
			}

			return false;
		}
		else
		{
			//wan:2005-03
			//???????? ?????? ?????? ???????? ???????? ???? ??????
			//?????? ???????? ???????? ???? ????
			if( ( m_nCurWorldIdx == WORLD_MATRIX
					|| m_nCurWorldIdx == WORLD_MATRIX_2009
				) && !m_pUIMgr->m_b_Attack && GetKeyState( VK_SHIFT ) & 0x80 )
			{
				m_pUIMgr->m_b_Attack = false;
				m_pUIMgr->m_b_MovingForAttack = false;
				m_pUIMgr->m_b_SecAttack = false;
				m_pUIMgr->m_bChakramAtt = false;
				m_pUIMgr->m_bChakram = false;
				m_pUIMgr->m_bBookSpellAtt = false;
				m_pUIMgr->m_bBookSpell = false;
				return false;
			}

			m_pUIMgr->m_b_MovingForAttack = false;
		}
		//-----------------------------------------------------------------------
		// Cmpare Block 3 SecAttack ????.
		if(m_pUIMgr->m_b_SecAttack)
		{
			m_pUIMgr->m_b_SecAttack = false;
			// SecAttack ?? ?????? ?????? ??????

			if(Selected_Skill == SKILL_MAGIC_MOVE || Selected_Skill == SKILL_HANDLE)
			{
				m_pUIMgr->m_bRMouseDown=TRUE;
			}
		}
		// Compare Block 1 Right SecAttack ???? ???? ????

		else if(G_SK_IsSecAttack(cur_skill))
		{
			if( Selected_Skill == SKILL_FORCE_ATTACK )
				m_pUIMgr->m_b_Attack = true;
			else
				m_pUIMgr->m_b_Attack = false;
		}
		// ???? ???? ???? ???? ????

		if(G_SK_IsOnceAttack(cur_skill))
		{
			m_pUIMgr->m_b_Attack = false;
		}


		//----------------------------------------------------------------------
		// Logic Block 3 ????

		//----------------------------------------------------------------------
		//mungmae-2005/04/11
		//????: ???? ?????? ???????? ?????? ?????? ?????? ?? ???? ??????.
		//		m_b_MotionForAttack==>?????? false?? ?????? ?????? ?????? ????????..??
		// ???????? ???????? ???? ???????? ???????? ???? ????
		if(g_pRoh->m_bComboReady
				|| Selected_Skill == SKILL_B_FLYINGCRASH
				|| Selected_Skill == SKILL_F_SHADOWSLASH )
			m_pUIMgr->m_b_MotionForAttack = false;
		else
			m_pUIMgr->m_b_MotionForAttack = true;

		pCMyApp->StopHero();
		if( Selected_Skill != SKILL_B_FLYINGCRASH && Selected_Skill != SKILL_F_SHADOWSLASH)
		{
			if(m_pHeroAttGoMob)
				g_pRoh->SetAttackMobId(m_pHeroAttGoMob->m_nVirId);
			if(m_pHeroAttGoCha)
			{
				// ???? ???????? ???????????? ???? ?????? ???????? ?????? ???????? ??????.
				if( g_ChaSkill.m_SkillIndex[cur_skill] == SKILL_BLESSING     //-- ?????? ????
						|| g_ChaSkill.m_SkillIndex[cur_skill] == SKILL_JOINT_LIFE     //-- ???? ????
						|| g_ChaSkill.m_SkillIndex[cur_skill] == SKILL_LIFE_LIGHT     //-- ?????? ??
						|| g_ChaSkill.m_SkillIndex[cur_skill] == SKILL_MELODY_OF_DEF //-- ?????? ????

				  ) // ???? ???????? ???? ?????????? ????.(?????? ?????? ???? ????????.)
					g_pRoh->SetAttackChaId( m_pHeroAttGoCha->m_nCharIndex, FALSE ); // ???? ?????????? ???????? ????. (???? ?????? ?????? ???????? ??????????..) 06-01-31 ????
				else
					g_pRoh->SetAttackChaId(m_pHeroAttGoCha->m_nCharIndex);
			}
		}


		// Attack ???? ???? ???? ????.(??????/?????? ????)
		if(AttackMsgSend(cur_skill))
		{
			// ???? / ?????? ???? ????

			if(G_SK_IsEffectSkill(Selected_Skill))
			{
				if(m_pHeroAttGoMob)
					g_pRoh->SetMagicMob(m_pHeroAttGoMob, Selected_Skill);
				if(m_pHeroAttGoCha)
					g_pRoh->SetMagicCha(m_pHeroAttGoCha, Selected_Skill);
			}
			else
			{
				if(m_pHeroAttGoMob)
					g_pRoh->Attack(m_pHeroAttGoMob->m_wx, m_pHeroAttGoMob->m_wy, m_pHeroAttGoMob->m_wz, Selected_Skill);
				if(m_pHeroAttGoCha)
					g_pRoh->Attack(m_pHeroAttGoCha->m_wx, m_pHeroAttGoCha->m_wy, m_pHeroAttGoCha->m_wz, Selected_Skill);
			}
		}

		return true;
	}// end of if(m_pHeroAttGoMob || m_pHeroAttGoCha)

	return false;
}

// m_fHeroDx : 1/20 ?????? ???????? ?? ????.\
// m_fHeroIx : 1?? ???? ?????? ????. m_fHeroDx???? ?????? ???? ???? ?? ????. ???? ?????? m_bMoreToGo?? TRUE
// m_fHeroTx : ???? ???? ????.
// temp_x : ???? ?? ?????????? ???????? ?? ????.. ???? ???? ?????? ?????? ????.
void CMyD3DApplication::HeroMoving()
{
	float temp_x, temp_z, temp_x2, temp_z2;
	BOOL bOverWalk = FALSE;

	if (CompareHeroAttGo())
		return;

	/*
	dwCurTime = timeGetTime();
	dwTimeCap = dwCurTime - m_dwPrevFrameTime;
	m_dwPrevFrameTime = dwCurTime;


	// temp_x, temp_z?? ?????????? ???????? ???? ????.
	temp_x = m_fHeroDx * ((float) dwTimeCap / 50.0f) ;		// 50.0f ?? ?????????? ?? ?????? ????
	temp_z = m_fHeroDz * ((float) dwTimeCap / 50.0f);		// HeroDx, HeroDz ?? ?????????? ?? ???????? ????.
	*/

	// temp_x, temp_z?? ?????????? ???????? ???? ????.
	temp_x = m_fHeroDx * g_fTimeKey / 0.05f ;		// 0.05f ?? ?????????? ?? ?????? ????
	temp_z = m_fHeroDz * g_fTimeKey / 0.05f ;		// HeroDx, HeroDz ?? ?????????? ?? ???????? ????.

	// ???????????? ???? ?????????? ???????? ?????? ?? ?? ???? ?????? ?????? ????????.
	int offset = 1;

	float fTimeKey = g_fTimeKey;
	while(fTimeKey > 0.05f)
	{
		if (m_pLand->IsMovableArea(m_vAt.x+m_fHeroDx*offset, m_vAt.z+m_fHeroDz*offset))
		{
			fTimeKey -= 0.05f;
			offset++;
		}
		else
		{
			// ???? ?????? ???????? ??????.
			// ?????????? 1?????? ?? ?????? ????????.
			temp_x = m_fHeroDx * (offset-1);
			temp_z = m_fHeroDz * (offset-1);

			// Tx, Tx?? ?????? ?????? ????.
			// ?? ???????? ???? ????????.
			m_fHeroTx = m_vAt.x + temp_x;
			m_fHeroTz = m_vAt.z + temp_z;

			m_bMoreToGo = FALSE;

			// ???? ?? ???? ????... ?? ?????????? ???????? ????. ??.??
			while (!m_bFastMove && !m_pLand->IsMovableArea(m_vAt.x, m_vAt.z))
			{
				m_vAt.x -= m_fHeroDx;
				m_vAt.z -= m_fHeroDz;
				m_vEye.x -= m_fHeroDx;
				m_vEye.z -= m_fHeroDz;
				g_pRoh->m_wx -= m_fHeroDx;
				g_pRoh->m_wz -= m_fHeroDz;
			}
			StopHero();
			break;
		}
	}

	// ???? ???? ????.. ??.. ???? ????...
	m_vEye.x += temp_x;
	m_vEye.z += temp_z;
	m_vAt.x += temp_x;
	m_vAt.z += temp_z;

	// ???? ?????? ???????? ?????? ????.
	if (!IsInsideMap(m_vAt.x, m_vAt.z))
	{
		m_vEye.x -= temp_x;
		m_vEye.z -= temp_z;
		m_vAt.x -= temp_x;
		m_vAt.z -= temp_z;

		// ?????????? ???????? ??????.
		return;
	}

	// ?????? ?????? ?????? ???? ?????? ???????? ?????? ????.
	if (m_bMoreToGo)
	{
		// 1/4
		if ( m_fHeroDx >= 0 && m_fHeroDz >= 0)
		{
			if (m_vAt.x >= m_fHeroIx && m_vAt.z >= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHero(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		} // 2 / 4
		else if ( m_fHeroDx <= 0 && m_fHeroDz >= 0)
		{
			if (m_vAt.x <= m_fHeroIx && m_vAt.z >= m_fHeroIz)

			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHero(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		} // 3/4
		else if ( m_fHeroDx <= 0 && m_fHeroDz <= 0)
		{
			if (m_vAt.x <= m_fHeroIx && m_vAt.z <= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHero(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		} // 4/4
		else if ( m_fHeroDx >= 0 && m_fHeroDz <= 0)
		{
			if (m_vAt.x >= m_fHeroIx && m_vAt.z <= m_fHeroIz)
			{
				// ?????? ???? ????..
				m_vEye.x -= temp_x;
				m_vEye.z -= temp_z;
				m_vAt.x -= temp_x;
				m_vAt.z -= temp_z;

				// ???? ??????
				temp_x2 = m_fHeroIx - m_vAt.x;
				temp_z2 = m_fHeroIz - m_vAt.z;

				m_vEye.x += temp_x2;
				m_vEye.z += temp_z2;
				m_vAt.x += temp_x2;
				m_vAt.z += temp_z2;
				//if (m_fHeroPx == 0.0f)
				MoveHero(m_fHeroTx, m_fHeroTz);
				//else
				//	MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
			}
		}
	}
	else
	{
		// 1/4
		/*if ( m_fHeroDx >= 0 && m_fHeroDz >= 0) {
			if (m_vAt.x >= m_fHeroTx && m_vAt.z >= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			// ?????? ???? ???????? ?????? ???? ???? ??????.
			if ( (m_fHeroTx - m_vAt.x) < 0 || (m_fHeroTz - m_vAt.z) < 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} // 2 / 4
		else if ( m_fHeroDx <= 0 && m_fHeroDz >= 0) {
			if (m_vAt.x <= m_fHeroTx && m_vAt.z >= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			if ( (m_fHeroTx - m_vAt.x) > 0 || (m_fHeroTz - m_vAt.z) < 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} // 3/4
		else if ( m_fHeroDx <= 0 && m_fHeroDz <= 0) {
			if (m_vAt.x <= m_fHeroTx && m_vAt.z <= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			if ( (m_fHeroTx - m_vAt.x) > 0 || (m_fHeroTz - m_vAt.z) > 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		} // 4/4
		else if ( m_fHeroDx >= 0 && m_fHeroDz <= 0) {
			if (m_vAt.x >= m_fHeroTx && m_vAt.z <= m_fHeroTz) {
				bOverWalk = TRUE;
			}
			if ( (m_fHeroTx - m_vAt.x) < 0 || (m_fHeroTz - m_vAt.z) > 0) {
				bOverWalk = TRUE;
				StopHero();
			}
		}*/
		if ( m_fHeroDx >= 0.0f && m_vAt.x >= m_fHeroTx)
		{
			bOverWalk = TRUE;
		}
		else if ( m_fHeroDx <= 0.0f && m_vAt.x <= m_fHeroTx)
		{
			bOverWalk = TRUE;
		}
		else if ( m_fHeroDz >= 0.0f && m_vAt.z >= m_fHeroTz)
		{
			bOverWalk = TRUE;
		}
		else if ( m_fHeroDz <= 0.0f && m_vAt.z <= m_fHeroTz)
		{
			bOverWalk = TRUE;
		}
	}

	if (bOverWalk)
	{
		// ?????? ???? ????..
		m_vEye.x -= temp_x;
		m_vEye.z -= temp_z;
		m_vAt.x -= temp_x;
		m_vAt.z -= temp_z;

		// ???? ??????
		temp_x = m_fHeroTx - m_vAt.x;
		temp_z = m_fHeroTz - m_vAt.z;

		m_vEye.x += temp_x;
		m_vEye.z += temp_z;
		m_vAt.x += temp_x;
		m_vAt.z += temp_z;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ?????? : ??????
		// ?????? : 05-03-03
		// ???????? : ?????? ???? ?????? ???????? ?????? ???? ????.(???? ???? ??????)(place?????? ???? ???? ?????? ???????? ?????????? ???? ???? ???? ?????? ??????.)
		// ???? : ???? ?????? ???????? ?????? ???????? ?????? ???? ???? ???????????? ???????? ?? ??. ?????? MoveHero(m_fHeroPx, m_fHeroPz, TRUE); ?? ??????
		//        ???? ???????? ?????? ???? ???????? ???? ???? ???? ???? ?????? ?????? ???? ?????? ???? ???? ?????? ???? ?????? ????????. ???????? ???????? ????????.
		//        ?? ?????? ?????? ???????? ?????? ?????? ???????? ???????? ?????? ???????? ???????? ?????? Stop?????? ???????? ?????? ??????.
		//        ???? ???????? ???????? ?????? ?????? ?????? ?????? ?????? ???? ?????? ?????? ???? ???? ???????? ?????????? ???? ?? ?????? ??????????.
		/*
				if (m_fHeroPx != 0.0f) {
					MoveHero(m_fHeroPx, m_fHeroPz, TRUE);
				} else {
		*/			// ???? ?? ???? ????... ?? ?????????? ???????? ????. ??.??
		while (!m_bFastMove && !m_pLand->IsMovableArea(m_vAt.x, m_vAt.z))
		{
			m_vAt.x -= m_fHeroDx;
			m_vAt.z -= m_fHeroDz;
			m_vEye.x -= m_fHeroDx;
			m_vEye.z -= m_fHeroDz;
			g_pRoh->m_wx -= m_fHeroDx;
			g_pRoh->m_wz -= m_fHeroDz;
		}
		StopHero();
		//m_bHeroMoving = FALSE; // ???? StopHero()?????? ????. ???????????? ?????? ???? ?????? ?????? ????!
//		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// ???????? ?????????? ???????? ????????.
	m_vAt.y = g_pRoh->m_wy + m_fAtHeight;

	//if (m_bHeroMoving == FALSE)
	//	g_pRoh->SetMotionState(CHA_STAND);

	m_vEyeZ.x = m_vAt.x - m_vEye.x;
	m_vEye.y = m_vAt.y + m_fCamHeight;		// ???????? ?????? ???? ???? ?????? ???? ??????.

	// ???????? ????.
	float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z) + 5.0f;
	// ???? ????.
	if (m_vEye.y < ty)
	{
		m_vEye.y = ty;
		m_fCamHeight = m_vEye.y - m_vAt.y;
	}

	m_vEyeZ.y = m_vAt.y - m_vEye.y;
	m_vEyeZ.z = m_vAt.z - m_vEye.z;

	SetViewMatrix(m_matView);
}

void CMyD3DApplication::StopHero(BOOL bMsg)
{
	if (!IsHeroMoving())
		return;

	// ???? ???????? ???????? ?????? ???? ?????? ????.

	// ???? ???????? ???????? ????.
	/*if (m_pNearMob) {
		if (DistPlToPl(g_pRoh->m_wx, g_pRoh->m_wz, m_pNearMob->m_wx, m_pNearMob->m_wz) <= 20.0f) {
			AvoidObj(m_pNearMob->m_wx, m_pNearMob->m_wz);
			return;
		}
	}*/

	m_bHeroMoving = FALSE;
	m_bMoreToGo = FALSE;
	g_pRoh->SetMotionState(CHA_STAND);

	if (bMsg)
	{
		sprintf(g_commOut, "stop %.0f %.0f %.0f %.0f\n", g_pRoh->m_wx, g_pRoh->m_wz, g_pRoh->m_wy, (g_pRoh->m_toDir) * 10000);
		if (g_pTcpIp)
			g_pTcpIp->SendNetMessage(g_commOut);
	}
}

void CMyD3DApplication::AvoidObj(float x, float z)
{
	// ???? ???????? ?? ?? ???? ?????? ????.
	// ???? ????.. ^^;
	float to_x, to_z;

	to_x = cosf(g_pRoh->m_toDir) * 20;
	to_z = sinf(g_pRoh->m_toDir) * 20;

	MoveHero(g_pRoh->m_wx+to_x, g_pRoh->m_wz+to_z);
}

//  [8/3/2009 ppmmjj83] ???????????????? ???????? ???????? ???????? ???????? ???? ????
BOOL CMyD3DApplication::IsBossradePeaceArea(CNkCharacter *pNkCha, float wx, float wz)
{
	if (!pNkCha)
		pNkCha = g_pRoh;
	if (wx == 0.0f)
		wx = pNkCha->m_wx;
	if (wz == 0.0f)
		wz = pNkCha->m_wz;

	if( m_nCurWorldIdx == WORLD_BOSSRAID_BULKAN )
	{
		if (IsInsideRect(wx, wz, 8971, 11455, 11440, 9459))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_KAILIPTON )
	{
		if (IsInsideRect(wx, wz, 9115, 4342, 12434, 1274))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_AIDIA )
	{
		if (IsInsideRect(wx, wz, 1105, 7531, 4592, 4262))
			return TRUE;
		if (IsInsideRect(wx, wz, 8746, 12289, 12265, 9045))
			return TRUE;
		if (IsInsideRect(wx, wz, 8828, 4896, 11597, 1221))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_HUMAN )
	{
		if (IsInsideRect(wx, wz, 1671, 8717, 5985, 4248))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_HYBRIDER )
	{
		if (IsInsideRect(wx, wz, 8057, 12139, 12275, 7320))
			return TRUE;
	}

	return FALSE;
}

// ------------------------------------------------------
// Name: IsPeaceArea()
// Desc: ???? ?????? ?????? ?? ?? ???? ???????? ???? ????.
//       ?????????? ???? ???? ???????????? ?????? ???? ????????.
// Warning: ?? ???? ???????? ?????? ???? ??????.
// -------------------------------------------------------
BOOL CMyD3DApplication::IsPeaceArea(CNkCharacter *pNkCha, float wx, float wz)
{
	if (!pNkCha)
		pNkCha = g_pRoh;
	if (wx == 0.0f)
		wx = pNkCha->m_wx;
	if (wz == 0.0f)
		wz = pNkCha->m_wz;

	//  [11/20/2008 parkmj] ?????? ???????????????? ?????? ????????????
	if( m_nCurWorldIdx == 13 )
	{
		if (IsInsideRect(wx, wz, 4954, 6752, 5353, 6289))
		{
			//-- IDS_CANT_USE_THIS_AREA : ?? ?????????? ?????? ?? ????????.
			//g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_CANT_USE_THIS_AREA) , -1); // ?????????? ????????.
			return TRUE;
		}
	}

	if( m_nCurWorldIdx == WORLD_FIELD )
	{
		if( pCMyApp->m_pUIMgr->m_bOnBattle ||
			DecardiBattleManager::GetInstance()->IsStart() == true )
		{
			// 5552,7102
			// 7657,5475
			if (IsInsideRect(wx, wz, 8800, 6900, 9580, 6400))
				return TRUE;
		}
		else	// ??????????
		{
//			if (IsInsideRect(wx, wz, 5100, 7600, 8050, 5000)) // ?? ?????? ????.
			if (IsInsideRect(wx, wz, 5200, 7450, 7950, 5180)) // ?????? ???????? ?????????? ????????.
				return TRUE;
		}
	}
	else if (m_nCurWorldIdx == WORLD_START)	// ??????????
	{
		if (IsInsideRect(wx, wz, 6100, 5500, 6447, 4995) ||  //A
				IsInsideRect(wx, wz, 6452, 5299, 6950, 5000) ||  //B
				IsInsideRect(wx, wz, 6802, 5444, 7245, 5000) ||  //C
				IsInsideRect(wx, wz, 6901, 5746, 7149, 5448) ||  //D
				IsInsideRect(wx, wz, 6950, 5003, 7100, 4853) ||  //E
				IsInsideRect(wx, wz, 7250, 5299, 7802, 5150) ||  //F
				IsInsideRect(wx, wz, 7799, 5547, 8498, 4851) ||  //G
				IsInsideRect(wx, wz, 8503, 5047, 9097, 4951) ||  //H
				IsInsideRect(wx, wz, 9099, 5246, 9546, 4799)  //I
		   )
			return TRUE;

		if (IsInsideRect(wx, wz, 6800, 5450, 7250, 5000) || IsInsideRect(wx, wz, 7255, 5270 , 7794, 5156) )
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_GUILD)
	{
		if (IsInsideRect(wx, wz, 6900, 11950, 7900, 11250))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_SKY)
	{
		if (IsInsideRect(wx, wz, 950, 1850, 1750, 1050) || IsInsideRect(wx, wz, 11368, 12110, 12100, 11320) )
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_WHITEHORN)
	{
		if (IsInsideRect(wx, wz, 8400, 8230, 9300, 7500))
			return TRUE;
		if (IsInsideRect(wx, wz, 8060, 6960, 8540, 6340))
			return TRUE;
	}
	else if( m_nCurWorldIdx == WORLD_DMITRON )	// nate 2005-07-15 : Dmitron - ????????
	{
		if( IsInsideRect( wx, wz, 10005, 7196, 11608, 5371 ))
			return TRUE;

	}
	else if( m_nCurWorldIdx == WORLD_DUNGEON4 ) // ?????? ???????? ????(by ????)
	{
		if(  IsInsideRect(wx, wz, 540, 612, 1122, 255)  ) // ???? ????.
			return TRUE;
		else if(  IsInsideRect(wx, wz, 3382, 2853, 4542, 1740)  ) // ???? ????.
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_DUNGEON1)
	{
		if (IsInsideRect(wx, wz, 7300, 2200, 8500, 1300) ) ///????1
			return TRUE;
		else if (IsInsideRect(wx, wz, 407, 5785, 589, 5451) ) ///????1
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_DUNGEON2)
	{
		if (IsInsideRect(wx, wz, 195, 740, 367, 448))
			return TRUE;
		else if (IsInsideRect(wx, wz, 115, 768, 254, 615))
			return TRUE;
		else if (IsInsideRect(wx, wz, 1699, 5025, 1835, 4896))
			return TRUE;
		else if (IsInsideRect(wx, wz, 2940, 1858, 3049, 1697))
			return TRUE;
		else if (IsInsideRect(wx, wz, 4792, 3884, 4979, 3813))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_DUNGEON_S)
	{
		if (IsInsideRect(wx, wz, 1, 470 , 350, 1) )
			return TRUE;
		else if (IsInsideRect(wx, wz, 1100, 5000, 1360, 5000))
			return TRUE;
		else if (IsInsideRect(wx, wz, 2428, 4163, 2700, 3900))
			return TRUE;
		else if (IsInsideRect(wx, wz, 2202, 2078, 2466, 1885))
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_BULKAN ) // ???? ???? ????
	{
		if( IsInsideRect(wx, wz, 580, 1600, 1430, 300 ) )	// ?????? ??
			return TRUE;

		if( IsInsideRect(wx, wz, 1430, 1300, 1600, 300 ) )	// ?????? ??
			return TRUE;

		if( IsInsideRhombus(wx, wz, 1390,1360,1500,1260,1540,1300,1440,1410) ) // ?????? ??
			return TRUE;


		if( IsInsideRect(wx, wz, 8950, 10580, 11440, 10310 ) ) // ???? ?????
			return TRUE;

		if( IsInsideRect(wx, wz, 10080, 11470, 10330, 8940) ) // ???? ????? ?????? ?????? ...
			return TRUE;

		if( IsInsideRect(wx, wz, 10040, 10610, 10300, 10350) ) // ?????? ???? ????
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_KAILIPTON ) // ???????? ???? ????
	{
		if( IsInsideRect(wx, wz, 630, 11580, 1880, 10150 ) )	// ?????? ??
			return TRUE;


		if( IsInsideRect(wx, wz, 9170, 3130, 9770, 2490 ) )	// ?????? ????

			return TRUE;

		if( IsInsideRect(wx, wz, 9680, 3170, 10160, 2340 ) )	// ?????? ????
			return TRUE;

		if( IsInsideRect(wx, wz, 10160, 3110, 10470, 2460 ) )	// ?????? ????

			return TRUE;

		if( IsInsideRect(wx, wz, 10330, 3090, 10720, 2530 ) )	// ?????? ?? ????

			return TRUE;

	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_AIDIA) // ???????? ???? ????
	{
		if( IsInsideRect(wx, wz, 810, 2400, 1900, 880 ) )		// ?????? ??
			return TRUE;

		if( IsInsideRect(wx, wz, 9350, 8470, 11340, 5250 ) )	// ?????? ???? ????
			return TRUE;

	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_HUMAN ) // ???? ???? ????
	{
		if( IsInsideRect(wx, wz, 890, 1900, 2020, 590 ) )	// ?????? ?? ???? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 1680,1480,1880,790,2550,1110,2170,1690) ) // ?????? ?? ???? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 770,1580,1790,1060,2180,1720,1290,2390) ) // ?????? ?? ???? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 1910,1700,2110,1450,2620,1920,2420,2120) ) // ?????? ?? ???? ?? (???? ?? ????)
			return TRUE;


		if( IsInsideRect(wx, wz, 920,12110,2510,10220 ) )	// ?????? ?? ???? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 790,10400,1580,9870,2360,10370,1680,10940) ) // ?????? ?? ???? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 1490,11300,2380,10420,3110,11700,2060,12030) ) // ?????? ?? ???? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 2030,10240,2360,10230,2550,10430,2190,10770) ) // ?????? ?? ???? ?? (???? ?? ????)
			return TRUE;


		if( IsInsideRhombus(wx, wz, 2950,7170,3520,6580,3660,6720,3080,7300) ) // 10??????

			return TRUE;

		if( IsInsideRhombus(wx, wz, 3990,6740,4030,6590,4610,7170,4470,7310) ) // 1??????

			return TRUE;

		if( IsInsideRhombus(wx, wz, 3890,6220,4460,5660,4600,5790,4020,6360) ) // 5??????

			return TRUE;

		if( IsInsideRhombus(wx, wz, 2960,5790,3090,5650,3670,6220,3520,6370) ) // 7??????

			return TRUE;

		if( IsInsideRhombus(wx, wz, 3410,6480,3770,6120,4100,6480,3770,6840) ) // ??????
			return TRUE;

		if( IsInsideRect(wx, wz, 4020, 6570, 4240, 6370) ) // ????
			return TRUE;
	}
	else if (m_nCurWorldIdx == WORLD_BOSSRAID_HYBRIDER ) // ???????? ???? ????
	{
		if( IsInsideRect(wx, wz, 720,2170,1550,1750) )	// ?????? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRect(wx, wz, 600,1760,1790,510) )	// ?????? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRect(wx, wz, 600,1760,1790,510) )	// ?????? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRect(wx, wz, 1310,1180,3110,510) )	// ?????? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRect(wx, wz, 1780,1530,1900,1360) )	// ?????? ?? (???? ?? ????)
			return TRUE;

		if( IsInsideRhombus(wx, wz, 860,2150,1120,1980,1260,2210,1130,2330) ) // 10??????
			return TRUE;

		if( IsInsideRect(wx, wz, 9800,104700,10590,9690) ) // ????
			return TRUE;

		if( IsInsideRect(wx, wz, 10100,9790,10270,9540) ) // ????
			return TRUE;
	}
	else if( m_nCurWorldIdx == WORLD_BOSSRAID_FINALBOSS )
	{
		if( IsInsideRect(wx, wz, 5200, 4730, 6700, 3240) )
			return TRUE;
	}
	else if( m_nCurWorldIdx == WORLD_DISPOSAL )
	{
		if( IsInsideRect(wx, wz, 189, 6212, 564, 5577) )
			return TRUE;
		
		if( IsInsideRect(wx, wz, 3885, 1266, 4323, 189) )
			return TRUE;
	}
/*	else if( m_nCurWorldIdx == WORLD_VIDBLINE )
	{
		if( IsInsideRect(wx, wz, 810, 2400, 1900, 880 ) )		// ?????? ??
			return TRUE;
	}*/

	else if( m_nCurWorldIdx == WORLD_FORLORN_S2 )
	{
		if( IsInsideRect(wx, wz, 3449,7302,3800,6845)
				|| IsInsideRect(wx, wz, 718,7218,1045,6853) )
			return TRUE;		
	}

	/*else if( m_nCurWorldIdx == WORLD_FORLORN_S3 )
	{
		if( IsInsideRect(wx, wz, 6679, 7348, 7233, 6631) )
			return TRUE;

		if( IsInsideRect(wx, wz, 12006, 12484, 12529, 11723) )
			return TRUE;
	}

	else if( m_nCurWorldIdx == WORLD_FORLORN_S4 ) // Add For S4
	{
		if( IsInsideRect(wx, wz, 3449,7302,3800,6845)
				|| IsInsideRect(wx, wz, 718,7218,1045,6853) )
			return TRUE;	
	}*/

	else if( m_nCurWorldIdx == WORLD_TRITIAN ) 
	{
		if( IsInsideRect(wx, wz, 2700,2700,3700,3700))
			return TRUE;	
	}
	return FALSE;
}

BOOL CMyD3DApplication::IsColo(float x, float z)
{
	switch( m_nCurWorldIdx )
	{
	case 4:
		if (x > 7700 && x < 8600 && z > 3900 && z < 4700)
			return TRUE;
		break;

	case 10: // ???????? ???? ???????? ???????? ????
		if (x > 2600 && x < 3530 && z > 1425 && z < 2380)
			return TRUE;
		break;
	case 19: // [2008/11/20 Theodoric] WORLD_MATRIX_2009
		if (x > 9200 && x < 10050 && z > 10650 && z < 11650)
			return TRUE;
		break;
	}
	
	return FALSE;
}

BOOL CMyD3DApplication::IsInsideMap(float x, float z)
{
	// (500, 12300)
	float left = 500.0f, top = 12300.0f, right = 12300.0f, bottom = 500.0f;

	if (g_bDungeon)
	{
		if (x >= 0.0f && x <= 5100.0f && z <= 5100.0f && z >= 0.0f)
			return TRUE;
	}
	else
	{
		// ???? ?????? ???? ???? ????.
		if (m_nCurWorldIdx == WORLD_GUILD)
		{
			// ???????? ???????? ?? ???? ????.
			if ( x >= 510.0f && x <= 12290.0f && z <= 12290.0f && z >= 4710.0f)
				return TRUE;
			else if (x >= 3425.0f && x <= 4958.0f && z >= 1896.0f && z <= 3219.0f)
				return TRUE;
			else if (x >= 7129.0f && x <= 9228.0f && z >= 1828.0f && z <= 3481.0f)
				return TRUE;
		}
		else if (m_nCurWorldIdx == WORLD_SKY)
		{
			if (x >= 1000.0f && x <= 7440 && z <= 7349.0f && z >= 1000.0f)
				return TRUE;
			else if ( x >= left && x <= right && z <= top && z >= bottom)
				return TRUE;
		}
		// nate 2004 - 10 : GuildHunt
		else if( m_nCurWorldIdx == WORLD_WHITEHORN )
		{
			// ???????? ?????????? ???? ???? ????
			if ( x >= left && x <= right && z <= top && z >= 3400 || IsGuildHuntArea( x, z ) )
				return TRUE;
		}
		//wan:2004-11 : Whitehorn Dungeon - ???????????? ???? ????, ????????????
		else if( m_nCurWorldIdx == WORLD_DUNGEON4 )
		{
			if ( x >= 50 && x <= 7300 && z <= 5600 && z >= 50 )
				return TRUE;
		}
		else if( m_nCurWorldIdx == WORLD_DMITRON )
		{
			return IsInsideRect( x, z, 200, 12600, 12600, 200 );
		}
		// nate 2005-03-22 : Matrix - ?? ?????? ????
		else if( m_nCurWorldIdx == WORLD_MATRIX )
		{
			if ( x >= 1000.0f && x <= 11800.0f && z <= 11840.0f && z >= 700.0f )
				return TRUE;
		}
		else if( m_nCurWorldIdx == WORLD_DUNGEON1 ) ///????1
		{
			if ( x >= 100.0f && x <= 12600.0f && z <= 12600.0f && z >= 100.0f )
				return TRUE;
		}
		else if( m_nCurWorldIdx == WORLD_GUILD_WAR ) // ????????
		{
			if ( x >= 100.0f && x <= 12600.0f && z <= 12600.0f && z >= 100.0f )
				return TRUE;
		}
		else if( m_nCurWorldIdx >= WORLD_BOSSRAID_BULKAN && m_nCurWorldIdx <= WORLD_BOSSRAID_FINALBOSS )
		{
			if ( x >= 50.0f && x <= 12800.0f && z <= 12800.0f && z >= 50.0f )
				return TRUE;
		}
		else if( m_nCurWorldIdx == WORLD_DISPOSAL )
		{
			return TRUE;
		}
		else if( m_nCurWorldIdx == WORLD_VIDBLINE  || m_nCurWorldIdx == WORLD_SPACE || m_nCurWorldIdx == WORLD_ROYALE || m_nCurWorldIdx == WORLD_DUN910 || m_nCurWorldIdx == WORLD_MAP930)
		{
			return TRUE;
		}
		else if( m_nCurWorldIdx == WORLD_FORLORN_S2 || m_nCurWorldIdx == WORLD_FORLORN_S3 || m_nCurWorldIdx == WORLD_FORLORN_S4 || m_nCurWorldIdx == WORLD_HIGHTLAND || m_nCurWorldIdx == WORLD_TRITIAN )
		{
			return TRUE;
		}
		else
		{
			if ( x >= left && x <= right && z <= top && z >= bottom)
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CMyD3DApplication::IsInsideRect(float x, float y, float left, float top, float right, float bottom)
{
	if ( x >= left && x <= right && y <= top && y >= bottom)
		return TRUE;
	else
		return FALSE;
}

//
// ???? ???? ???????? ???? ?????? ???????? (???? ????.)
//
// ???? ???? ???? : 9?? 6?? 3?? 12??
//
//               * 4??
//             ?? ??
//			  /     ??
//      1??  *        * 3??

//            ??    ??
//              ?? ??
//                *  2??
//
BOOL CMyD3DApplication::IsInsideRhombus(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	float Gradient_1 = (y2 - y1) / (x2 - x1) ;
	float Gradient_2 = (y2 - y3) / (x2 - x3) ;
	float Gradient_3 = (y3 - y4) / (x3 - x4);
	float Gradient_4 = (y4 - y1) / (x4 - x1) ;
	float yy1 = (Gradient_1 * x) + (y1 - (x1*Gradient_1));
	float yy2 = (Gradient_2 * x) + (y2 - (x2*Gradient_2));
	float yy3 = (Gradient_3 * x) + (y3 - (x3*Gradient_3));
	float yy4 = (Gradient_4 * x) + (y4 - (x4*Gradient_4));

	if( yy1 <= y && yy2 <= y && yy3 >= y && yy4 >= y )
		return TRUE;

	return FALSE;
}


void CMyD3DApplication::DrawPlayer()
{
	D3DMATRIX	matTrans, matRotate;
	DWORD dwPrevTime;

	g_pRoh->m_wx = m_vAt.x;
	if( g_bNewTerrain )
	{
		float	fY = m_pLand->GetHFHeight( m_vAt.x, m_vAt.z );
		g_pRoh->m_wy = CNKObjQuadNode::GetHeightOnObject( m_vAt.z, m_vAt.x, g_pRoh->m_wy );
		if( g_pRoh->m_wy < fY )
			g_pRoh->m_wy = fY;
	}
	else
		g_pRoh->m_wy = m_pLand->GetHFHeight(m_vAt.x, m_vAt.z);
	g_pRoh->m_wz = m_vAt.z;

	dwPrevTime = timeGetTime();
	// ???????? ?????????? ?????? ?????? ????.
	g_pRoh->Render(NULL, FALSE, !g_bGhost);

	// ?????? ???? ?????? ????
	if (m_pMyPet && !m_bBeforeGoWorld)
	{
		if (m_pMyPet->GetDeadFrame() <= 100)
		{
			m_pMyPet->Render(FALSE, !g_bGhost);
			DWORD curTime = timeGetTime();

			if( g_MyPetStatus.m_bUseMacroMode && !m_pMyPet->IsDead() ) // ?????? ?????? ???? ????????...
				g_pNk2DFrame->GetLShiftMenu()->m_pControlMacro->CheckMacroTime(); // ?????? ?????? ????.


			if (m_pMyPet->Social(curTime))
			{}
			else if (m_pMyPet->PickItem())
			{
				m_pMyPet->InitSocialTime(curTime);
			}
			else if (m_pMyPet->AttackCombatMob())
			{
				m_pMyPet->InitSocialTime(curTime);
			}
			else if (m_pMyPet->AttackNearestMob())
			{
				m_pMyPet->InitSocialTime(curTime);
			}
			else if (m_pMyPet->FollowMaster())
			{
				m_pMyPet->InitSocialTime(curTime);
			}
		}
		else
		{
			delete m_pMyPet;
			m_pMyPet = NULL;

			g_MyPetStatus.mode = MYPET_MODE_KEEP; // ???????? ?????? ???????? ?????? ??????..(?????? ?????? ???? ????????.) ?????? ????.
			g_MyPetStatus.m_bUseMacroMode = FALSE; // ????????.. ?????? ?????? ?????? ????.
		}
	}
//	DisplayPartyVital(g_pRoh, (g_pRoh->m_Vital/g_pRoh->m_MaxVital)*100);
	m_dwRenderRohTime = timeGetTime() - dwPrevTime;
}

void CMyD3DApplication::ShowRohEnergeBar()
{
	// ?????? ???? ???? ????????.
	if (g_pRoh->GetCombat())
	{
		if (m_nShowRohEnerge >= 0)
		{
			int energe, max_energe;
			CSurface *pFrontSurface;
			D3DMATRIX matPipe;
			D3DVECTOR vScr, vWorld;
			int vital_x;
			RECT rect;
			// ?? ?????? ???? ?????? ???? ???? ????????~
			if(m_nShowRohEnerge==0 && !m_bInitEnergyBar)
			{
				switch(g_pRoh->m_Race)
				{
				case RACE_BULKAN:
				case RACE_FREAK:
					m_nShowRohEnerge= 5;
					break;
				case RACE_KAILIPTON:
				case RACE_AIDIA:
					m_nShowRohEnerge= 4;
					break;
				case RACE_HUMAN:
					m_nShowRohEnerge= 6;
					break;
				case RACE_PEROM:
					m_nShowRohEnerge= 5;
					break;
				default:
					m_nShowRohEnerge= 0;
					break;
				}
				m_bInitEnergyBar=TRUE;
			}

			// ???????? ?????? ???? ???? ????????.
			if (m_pMyPet)
			{
				energe = m_pMyPet->m_Vital;
				max_energe = m_pMyPet->m_MaxVital;
				pFrontSurface = m_pUIMgr->m_pPetVitalFront;

				if (energe < 0)
					energe = 0;

				vWorld = D3DVECTOR_New(m_pMyPet->m_wx, m_pMyPet->m_wy-0.0f, m_pMyPet->m_wz);

				// ?????? CLand?? ?????? ????.
				if( m_pUIMgr->m_pLand )
					m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);

				// ???????? ???? ???????? ?????? ??????.
				D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

				vScr.y -= 15;	// ?????? ???????? ????.
				if (vScr.y < 0)
					vScr.y = 0;
				if (vScr.y >= g_pNk2DFrame->GetClientHeight() - VITAL_BACK_HEIGHT - 105)
				{
					vScr.y = g_pNk2DFrame->GetClientHeight() - VITAL_BACK_HEIGHT - 106;
				}
				vital_x = vScr.x - (VITAL_BACK_WIDTH / 2);
				if (vital_x < 0)
					vital_x = 0;
				if (vital_x >= g_pNk2DFrame->GetClientWidth() - VITAL_BACK_WIDTH)
				{
					vital_x = g_pNk2DFrame->GetClientWidth() - VITAL_BACK_WIDTH - 1;
				}

				g_pDisplay->Blt(vital_x, vScr.y, m_pUIMgr->m_pVitalBack);

				vital_x = vScr.x - (VITAL_FRONT_WIDTH / 2);
				if (vital_x < (VITAL_BACK_WIDTH - VITAL_FRONT_WIDTH) / 2)
					vital_x = (VITAL_BACK_WIDTH - VITAL_FRONT_WIDTH) / 2;
				if (vital_x >= g_pNk2DFrame->GetClientWidth() - VITAL_BACK_WIDTH + (VITAL_BACK_WIDTH - VITAL_FRONT_WIDTH) / 2)
				{
					vital_x = g_pNk2DFrame->GetClientWidth() - VITAL_BACK_WIDTH + (VITAL_BACK_WIDTH - VITAL_FRONT_WIDTH) / 2 - 1;
				}
				rect.top = 0;
				rect.bottom = VITAL_FRONT_HEIGHT;
				rect.left = 0;
				rect.right = VITAL_FRONT_WIDTH * energe / max_energe;
				g_pDisplay->Blt(vital_x+((VITAL_BACK_WIDTH-VITAL_FRONT_WIDTH)/2)-1,
								vScr.y+(VITAL_FRONT_Y-VITAL_BACK_Y), pFrontSurface, &rect);
			}
			// ?????? ??...
			switch (m_nShowRohEnerge)
			{
			case 1: // mana
				energe = (int)g_pRoh->m_Mana;
				max_energe = g_pRoh->m_MaxMana;
				pFrontSurface = m_pUIMgr->m_pManaFront;
				break;
			case 2: // stamina
				energe = (int)g_pRoh->m_Stamina;
				max_energe = g_pRoh->m_MaxStamina;
				pFrontSurface = m_pUIMgr->m_pStamFront;
				break;
			case 3: // epower
				energe = (int)g_pRoh->m_Epower;
				max_energe = g_pRoh->m_MaxEpower;
				pFrontSurface = m_pUIMgr->m_pEpowerFront;
				break;
			case 0: // vital
			case 4: // vital + mana
			case 5: // vital + stamina
			case 6: // vital + epower
			default:
				energe = (int)g_pRoh->m_Vital;
				max_energe = (int)g_pRoh->m_MaxVital;
				pFrontSurface = m_pUIMgr->m_pVitalFront;
				break;
			}

			if (energe < 0)
				energe = 0;

			vWorld = D3DVECTOR_New(g_pRoh->m_wx, g_pRoh->m_wy-0.0f, g_pRoh->m_wz);
			// ?????? CLand?? ?????? ????.
			if( m_pUIMgr->m_pLand )
				m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
			// ???????? ???? ???????? ?????? ??????.
			D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

			vScr.y += 15;	// ?????? ???????? ????.

			if (vScr.y < 0)
				vScr.y = 0;

			if (vScr.y >= g_pNk2DFrame->GetClientHeight())
				vScr.y = g_pNk2DFrame->GetClientHeight() - 10;

			vital_x = vScr.x - (VITAL_BACK_WIDTH / 2);

			g_pDisplay->Blt(vital_x, vScr.y, m_pUIMgr->m_pVitalBack);

			vital_x = vScr.x - (VITAL_FRONT_WIDTH / 2);
			rect.top = 0;
			rect.bottom = VITAL_FRONT_HEIGHT;
			rect.left = 0;
			rect.right = VITAL_FRONT_WIDTH * energe / max_energe;


			if( g_pRoh && g_pRoh->m_Accumulate )   // ?????????? ??????
			{
				g_pDisplay->Blt(vital_x+((VITAL_BACK_WIDTH-VITAL_FRONT_WIDTH)/2)-1,
								vScr.y+(VITAL_FRONT_Y-VITAL_BACK_Y), m_pUIMgr->m_pVitalBack_E, &rect);
			}
			else
			{
				g_pDisplay->Blt(vital_x+((VITAL_BACK_WIDTH-VITAL_FRONT_WIDTH)/2)-1,
								vScr.y+(VITAL_FRONT_Y-VITAL_BACK_Y), pFrontSurface, &rect);
			}
			if (m_nShowRohEnerge > 3)
			{
				switch (m_nShowRohEnerge)
				{
				case 4: // vital + mana
					energe = (int)g_pRoh->m_Mana;
					max_energe = g_pRoh->m_MaxMana;
					pFrontSurface = m_pUIMgr->m_pManaFront;
					break;
				case 5: // vital + stamina
					energe = (int)g_pRoh->m_Stamina;
					max_energe = g_pRoh->m_MaxStamina;
					pFrontSurface = m_pUIMgr->m_pStamFront;
					break;
				case 6: // vital + epower
					energe = (int)g_pRoh->m_Epower;
					max_energe = g_pRoh->m_MaxEpower;
					pFrontSurface = m_pUIMgr->m_pEpowerFront;
					break;
				}

				vital_x = vScr.x - (VITAL_BACK_WIDTH / 2);
				g_pDisplay->Blt(vital_x, vScr.y+VITAL_BACK_HEIGHT, m_pUIMgr->m_pVitalBack);

				vital_x = vScr.x - (VITAL_FRONT_WIDTH / 2);
				rect.top = 0;
				rect.bottom = VITAL_FRONT_HEIGHT;
				rect.left = 0;
				rect.right = VITAL_FRONT_WIDTH * energe / max_energe;
				g_pDisplay->Blt(vital_x+((VITAL_BACK_WIDTH-VITAL_FRONT_WIDTH)/2)-1,
								vScr.y+(VITAL_FRONT_Y-VITAL_BACK_Y)+VITAL_BACK_HEIGHT,
								pFrontSurface, &rect);
			}
		}
	}
}

//-----------------------------------------------------------------
// ?????? id?? vital ?? ?????? ?????? ?? ?? ????????.
//-----------------------------------------------------------------
int CMyD3DApplication::ShowMyPartysVital()
{
	CNkCharacter * p = NULL;

	for(int i=0; i<g_pRoh->m_iMaxPartysNum; i++)
	{
		if(g_pRoh->m_iPartysListId[i] <0)
			return 1;

		p = m_pUIMgr->FindChaById(g_pRoh->m_iPartysListId[i], TRUE);

		if(p)
			DisplayPartyVital(p, g_pRoh->m_fPartysVitalList[i]);
	}
	return 1;
}

//--------------------------------------------------------------------------
// DisplayPartyVital
//--------------------------------------------------------------------------
// ???????? ?????? ????????.
int CMyD3DApplication::DisplayPartyVital(CNkCharacter * p, float perVital)
{
	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;
	int vital_x;
	RECT rect;

	if(!p)
		return 0;

	if(perVital <=0)
		perVital = 1;
	
	//vWorld = D3DVECTOR_New(p->m_wx, p->m_wy+23.0f, p->m_wz);
	vWorld = D3DVECTOR_New(p->m_wx, p->m_wy-0.0f, p->m_wz);
	// ?????? CLand?? ?????? ????.
	if( m_pUIMgr->m_pLand )
		m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
	// ???????? ???? ???????? ?????? ??????.
	D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

	int front_cx = 48;
	int front_cy = 3;
	int back_cx = 54;
	int back_cy = 6;
	vScr.y += 15;	// ?????? ???????? ????.

	if (vScr.y < 0)
		vScr.y = 0;

	if (vScr.y >= g_pNk2DFrame->GetClientHeight())
		vScr.y = g_pNk2DFrame->GetClientHeight() - 10;

	vital_x = vScr.x - (back_cx / 2);
	vital_x = vScr.x - (front_cx / 2);
	rect.top = 0;
	rect.bottom = front_cy;
	rect.left = 0;
	rect.right = front_cx * perVital / 100;
	if(rect.right <=0 )
		rect.right = 1;


	RenderOthersVital(vital_x+((back_cx-front_cx)/2)+1, vScr.y+((front_cy-back_cy)/2)+2, rect.right, rect.bottom, vital_x, vScr.y, back_cx, back_cy ,0);
	return 1;
}
//--------------------------------------------------------------------------
// DisplayPayerVital
//--------------------------------------------------------------------------
/*int CMyD3DApplication::DisplayPlayerVital(CNkCharacter * p, float perVital)//desativado
{ 
	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;
	int vital_x;
	RECT rect;

	if(!p)
		return 0;

	if(perVital <=0)
		perVital = 1;

	vWorld = D3DVECTOR_New(p->m_wx, p->m_wy+30.0f, p->m_wz); //30.0f
	if(!m_pUIMgr)return 0;
	if( m_pUIMgr->m_pLand )
		m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
	D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

	int front_cx = 62;
	int front_cy = 3;
	int back_cx = 70;
	int back_cy = 6;
	vScr.y += 15;

	if (vScr.y < 0)
		vScr.y = 0;

	if (vScr.y >= g_pNk2DFrame->GetClientHeight())
		vScr.y = g_pNk2DFrame->GetClientHeight() - 10;

	vital_x = vScr.x - (back_cx / 2);
	vital_x = vScr.x - (front_cx / 2);
	rect.top = 0;
	rect.bottom = front_cy;
	rect.left = 0;
	rect.right = front_cx * perVital / 100;
	if(rect.right <=0 )
		rect.right = 1;

	RenderOthersVital(vital_x+((back_cx-front_cx)/2)+1, vScr.y+((front_cy-back_cy)/2)+2, rect.right, rect.bottom, vital_x, vScr.y, back_cx, back_cy ,0);
	return 1;
}*/
//--------------------------------------------------------------------------
// DisplaySummonVital
//--------------------------------------------------------------------------
// ???????? ?????? ????????.
int CMyD3DApplication::DisplaySummonVital(CNkMob *pMob)
{
	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;
	int vital_x;
	RECT rect;

	if(!pMob)
		return 0;

	if( pMob->m_Vital<=0)
		pMob->m_Vital= 1;

	vWorld = D3DVECTOR_New(pMob->m_wx, pMob->m_wy-0.0f, pMob->m_wz);
	// ?????? CLand?? ?????? ????.
	if( m_pUIMgr->m_pLand )
		m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
	// ???????? ???? ???????? ?????? ??????.
	D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

	int front_cx = 48;
	int front_cy = 3;
	int back_cx = 54;
	int back_cy = 6;
	vScr.y += 15;	// ?????? ???????? ????.

	if (vScr.y < 0)
		vScr.y = 0;

	if (vScr.y >= g_pNk2DFrame->GetClientHeight())
		vScr.y = g_pNk2DFrame->GetClientHeight() - 10;

	//vital_x = vScr.x - (back_cx / 2);
	vital_x = vScr.x - (front_cx / 2);
	rect.top = 0;
	rect.bottom = front_cy;
	rect.left = 0;
	rect.right = front_cx * pMob->m_Vital / 100;

	if(rect.right <=0 )
		rect.right = 1;

	RenderOthersVital(vital_x+((back_cx-front_cx)/2)+1, vScr.y+((front_cy-back_cy)/2)+2, rect.right, rect.bottom, vital_x, vScr.y, back_cx, back_cy, 1);
	return 1;
}

//--------------------------------------------------------------------------
// DisplayDivineVital
//--------------------------------------------------------------------------
// ?????? ?????? ?????? ????????.
int CMyD3DApplication::DisplayDivineVital(CNkMob *pMob)
{
	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;
	int vital_x;
	RECT rect;

	if(!pMob)
		return 0;

	if( pMob->m_Vital<=0)
		pMob->m_Vital= 1;

	vWorld = D3DVECTOR_New(pMob->m_wx, pMob->m_wy-0.0f, pMob->m_wz);
	// ?????? CLand?? ?????? ????.
	if( m_pUIMgr->m_pLand )
		m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
	// ???????? ???? ???????? ?????? ??????.
	D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

	int front_cx = 48;
	int front_cy = 3;
	int back_cx = 54;
	int back_cy = 6;
	vScr.y += 15;	// ?????? ???????? ????.

	if (vScr.y < 0)
		vScr.y = 0;

	if (vScr.y >= g_pNk2DFrame->GetClientHeight())
		vScr.y = g_pNk2DFrame->GetClientHeight() - 10;

	vital_x = vScr.x - (back_cx / 2);
	rect.top = 0;
	rect.bottom = front_cy;
	rect.left = 0;
	rect.right = front_cx * pMob->m_Vital / 100;
	if(rect.right <=0 )
		rect.right = 48;

	RenderOthersVital(vital_x+((back_cx-front_cx)/2)+1, vScr.y+((front_cy-back_cy)/2)+2, rect.right, rect.bottom, vital_x, vScr.y, back_cx, back_cy,2);

	return 1;
}


extern int g_EffectTextr[];
//--------------------------------------------------------------------------
// RenderOthersVital
//--------------------------------------------------------------------------
// ????, ??????, ?????? ?????? ???????? ?????? ????.
// kind == 0 ????, kind == 1 ??????, kind == 2 ?????? ????

void CMyD3DApplication::RenderOthersVital(int x, int y, int cx, int cy, int x2, int y2, int cx2, int cy2, int kind)
{
	if( !m_pd3dDevice )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;
	DWORD	barcolor;

	int screenCx, screenCy;
	if(m_ProOption.resolution == R640)
	{
		screenCx = 640;
		screenCy = 480;
	}
	else if(m_ProOption.resolution == R800)
	{
		screenCx = 800;
		screenCy = 600;
	}
	else if(m_ProOption.resolution == R1024)
	{
		screenCx = 1024;
		screenCy = 768;
	}
	else if(m_ProOption.resolution == R1280)
	{
		screenCx = 1280;
		screenCy = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		screenCx = 1280;
		screenCy = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		screenCx = 1600;
		screenCy = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		screenCx = 1920;
		screenCy = 1080;
	}

	// Color ????
	if( kind == 0)
		barcolor = 0xffee9999;
	else if( kind == 1 )
		barcolor = 0xff99ee99;
	else
		barcolor = 0xffff6300;

	// ?????? ???????? ???????? ?????? ???????? ????????.
	m_pd3dDevice->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = screenCx;
	vpNew.dwHeight = screenCy;
	m_pd3dDevice->SetViewport(&vpNew);

	TLVec3[0] = D3DTLVERTEX(D3DVECTOR_New(x,    y,      0), 1, barcolor, 0, 0, 0);
	TLVec3[1] = D3DTLVERTEX(D3DVECTOR_New(x+cx, y,     0), 1, barcolor, 0, 1, 0);
	TLVec3[2] = D3DTLVERTEX(D3DVECTOR_New(x,    y+cy,  0), 1, barcolor, 0, 0, 1);
	TLVec3[3] = D3DTLVERTEX(D3DVECTOR_New(x+cx, y+cy,  0), 1, barcolor, 0, 1, 1);

	TLVec4[0] = D3DTLVERTEX(D3DVECTOR_New(x2,	  y2,      0), 1, 0xfffffff, 0, 0, 0);
	TLVec4[1] = D3DTLVERTEX(D3DVECTOR_New(x2+cx2, y2,     0), 1, 0xfffffff, 0, 1, 0);
	TLVec4[2] = D3DTLVERTEX(D3DVECTOR_New(x2,     y2+cy2,  0), 1, 0xfffffff, 0, 0, 1);
	TLVec4[3] = D3DTLVERTEX(D3DVECTOR_New(x2+cx2, y2+cy2,  0), 1, 0xfffffff, 0, 1, 1);

	DWORD dwState[5];
	// ??????
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , &dwState[0]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[1]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[2]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[3]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwState[4]);

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);

	lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_VITAL_BAR_BACK]);
	m_pd3dDevice->SetTexture(0, lpTexture);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec4, 4, 0);

	lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_VITAL_BAR]);
	m_pd3dDevice->SetTexture(0, lpTexture);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec3, 4, 0);

	// ????????.
	m_pd3dDevice->SetViewport(&vpOld);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , dwState[0]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[1]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[2]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[3]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, dwState[4]);
}

//--------------------------------------------------------------------------
// DisplayChaHitpoints
//--------------------------------------------------------------------------
void CMyD3DApplication::DisplayChaHitpoints(CNkCharacter *pCha)
{
	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;

	if(!pCha)
		return;
	
	vWorld = D3DVECTOR_New(pCha->m_wx, pCha->m_wy-0.0f, pCha->m_wz);
	if( m_pUIMgr->m_pLand )
		m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
	D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

	vScr.y -= (100);
	if (vScr.y < 0)
		vScr.y = 0;

	if (vScr.y >= g_pNk2DFrame->GetClientHeight())
		vScr.y = g_pNk2DFrame->GetClientHeight() - 10;
	DWORD timenow = GetTickCount();
	for(int i = 0, j = 0; i< 10; i++)
	{
		if(pCha->hitpoints[i][0] > 0)
		{
			RenderHitPoints(vScr.x, vScr.y-(j*22), pCha->hitpoints[i][0], pCha->hitpoints[i][2], (timenow - pCha->hitpoints[i][1]));
		//	j++;
		}
	}
}
//--------------------------------------------------------------------------
// DisplayMobHitpoints
//--------------------------------------------------------------------------
void CMyD3DApplication::DisplayMobHitpoints(CNkMob *pMob)
{
	D3DMATRIX matPipe;
	D3DVECTOR vScr, vWorld;
	
	if(!pMob)
		return;

	vWorld = D3DVECTOR_New(pMob->m_wx, pMob->m_wy-0.0f, pMob->m_wz);
	if( m_pUIMgr->m_pLand )
		m_pUIMgr->m_pLand->GetPipelineMatrix(matPipe);
	D3DMath_VectorMatrixMultiply(vScr, vWorld, matPipe);

	
	//vScr.y -= (pMob->m_Height*100);
	vScr.y -= (100);

	if (vScr.y < 0)
		vScr.y = 0;

	if (vScr.y >= g_pNk2DFrame->GetClientHeight())
		vScr.y = g_pNk2DFrame->GetClientHeight() - 10;
	DWORD timenow = GetTickCount();
	for(int i = 0, j = 0; i< 10; i++)
	{
		if(pMob->hitpoints[i][0] > 0)
		{
			RenderHitPoints(vScr.x, vScr.y-(j*22), pMob->hitpoints[i][0], pMob->hitpoints[i][2], (timenow - pMob->hitpoints[i][1]));
		//	j++;
		}
	}
}
//HFONT HitpointFont;
//void CreateHitpointFont()
//{
//	HitpointFont = CreateFont (20, 0,			// Height, Width
//						0, 0,					// Escapement, Orientation
//						FW_NORMAL,				// Weight
//						FALSE,					// Italic
//						FALSE,					// Underline
//						FALSE,					// Strikeout ??????
//						DEFAULT_CHARSET,		// Char SET
//						OUT_DEFAULT_PRECIS,
//						CLIP_DEFAULT_PRECIS,
//						DEFAULT_QUALITY,
//						DEFAULT_PITCH,
//						"Arial");
//}
//--------------------------------------------------------------------------
// RenderHitPoints
//--------------------------------------------------------------------------
void CMyD3DApplication::RenderHitPoints(int x, int y, DWORD kind, DWORD attacker, DWORD time)
{
	if( !m_pd3dDevice )
		return;
	/*if ( HitpointFont == NULL)
		CreateHitpointFont();
	CSurface *m_pSurf;*/
	char amount[20];
	char format[20];
	strcpy(amount, "" );
	strcat(amount, itoa(kind, format, 10));
//	g_pDisplay->CreateSurfaceFromText(&m_pSurf, HitpointFont, amount, RGB(0, 0, 0), RGB(255, 255, 0));

//	g_pDisplay->Blt(x, y , m_pSurf);

	
	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;

	int screenCx, screenCy;
	if(m_ProOption.resolution == R640)
	{
		screenCx = 640;
		screenCy = 480;
	}
	else if(m_ProOption.resolution == R800)
	{
		screenCx = 800;
		screenCy = 600;
	}
	else if(m_ProOption.resolution == R1024)
	{
		screenCx = 1024;
		screenCy = 768;
	}
	else if(m_ProOption.resolution == R1280)
	{
		screenCx = 1280;
		screenCy = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		screenCx = 1280;
		screenCy = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		screenCx = 1600;
		screenCy = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		screenCx = 1920;
		screenCy = 1080;
	}

	m_pd3dDevice->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = screenCx;
	vpNew.dwHeight = screenCy;
	m_pd3dDevice->SetViewport(&vpNew);


	DWORD dwState[5];
	// save render state
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , &dwState[0]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwState[1]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwState[2]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwState[3]);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwState[4]);

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);

	float height = 45.0f, width = 33.0f;

	float sizefraction = 0.5f * ((2000.0f-(float)time) / 2000.0f);
	height *= sizefraction;
	width *= sizefraction;
	float x2 = (float)x - ((float)strlen(amount)/2 * width);
	float y2 = (float)y - ((float)time/60);

	

	float j = 0;
	for(int i = 0; i < strlen(amount); i++)
	{
		if(amount[i])
		{
			TLVec4[0] = D3DTLVERTEX(D3DVECTOR_New(x2+(width*j),	  y2,      0), 1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f ), 0, 0, 0);
			TLVec4[1] = D3DTLVERTEX(D3DVECTOR_New(x2+width+(width*j), y2,     0), 1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f ), 0, 1, 0);
			TLVec4[2] = D3DTLVERTEX(D3DVECTOR_New(x2+(width*j),     y2+height,  0), 1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f ), 0, 0, 1);
			TLVec4[3] = D3DTLVERTEX(D3DVECTOR_New(x2+width+(width*j), y2+height,  0), 1, D3DRGBA( 1.0f, 1.0f, 1.0f, 1.0f ), 0, 1, 1);
			int idx = 0;
			if(attacker == g_pRoh->m_nCharIndex)
				idx = EFF_NUMBER_10 + (amount[i]- 0x30);
			else
				idx = EFF_NUMBER_0 + (amount[i]- 0x30);
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[idx]);
			m_pd3dDevice->SetTexture(0, lpTexture);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec4, 4, 0);

			j++;
		}
	}
	
	// restore render state
	m_pd3dDevice->SetViewport(&vpOld);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , dwState[0]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwState[1]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwState[2]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, dwState[3]);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, dwState[4]);
}





void CMyD3DApplication::SetFogParameters(DWORD dwMode, DWORD dwColor,
		BOOL bRangeBased, BOOL bTableFog,
		FLOAT fStart, FLOAT fEnd, FLOAT fDensity )
{
	m_dwFogMode      = dwMode;
	m_bRangeBasedFog = bRangeBased;
	m_bUsingTableFog = bTableFog;
	m_dwFogColor     = dwColor;
	m_fFogStart      = ( fStart*(m_zFar-m_zNear) ) + m_zNear;
	m_fFogEnd        = ( fEnd*(m_zFar-m_zNear) ) + m_zNear;
	m_fFogDensity    = fDensity;
}

void CMyD3DApplication::SetLandFogParameter()
{
	static float	start_fog[30] =
	{
		0.02f, 0.03f, 0.04f, 0.05f, 0.06f,		// 211.4,  302.1,  392.8,  483.5,  574.2
		0.07f, 0.08f, 0.09f, 0.09f, 0.09f,		// 664.9,  755.6,  846.3,  846.3,  846.3
		0.09f, 0.09f, 0.09f, 0.09f, 0.10f,		// 846.3,  846.3,  846.3,  846.3,  937
		0.10f, 0.09f, 0.09f, 0.09f, 0.09f,		// 937,    846.3,  846.3,  846.3,  846.3
		0.09f, 0.09f, 0.09f, 0.08f, 0.07f,		// 846.3,  846.3,  846.3,  755.6,  664.9
		0.06f, 0.05f, 0.04f, 0.03f, 0.02f		// 574.2,  483.5,  392.8,  302.1,  211.4
	};
	static float	fNewStartFog[30] =
	{
		0.057f,	0.069f,	0.080f,	0.091f,	0.101f,		// 610,		700,	790,	880,	970
		0.111f,	0.121f,	0.133f,	0.133f,	0.133f,		// 1060,	1150,	1250,	1250,	1250
		0.133f,	0.133f,	0.133f,	0.133f,	0.143f,		// 1250,	1250,	1250,	1250,	1340
		0.143f,	0.133f,	0.133f,	0.133f,	0.133f,		// 1340,	1250,	1250,	1250,	1250
		0.133f,	0.133f,	0.133f,	0.121f,	0.111f,		// 1250,	1250,	1250,	1150,	1060
		0.101f,	0.091f,	0.080f,	0.069f,	0.057f		// 970,		880,	790,	700,	610
	};
	static float	end_fog[30] =
	{
		0.18f, 0.185f, 0.19f, 0.195f, 0.20f,	// 1662.6,   1707.95,  1753.3,   1798.65,  1844
		0.21f, 0.18f, 0.18f, 0.18f, 0.18f,		// 1934.7,   1662.6,   1662.6,   1662.6,   1662.6
		0.18f, 0.18f, 0.18f, 0.18f, 0.18f,		// 1662.6,   1662.6,   1662.6,   1662.6,   1662.6
		0.18f, 0.18f, 0.18f, 0.18f, 0.18f,		// 1662.6,   1662.6,   1662.6,   1662.6,   1662.6
		0.18f, 0.18f, 0.18f, 0.18f, 0.18f,		// 1662.6,   1662.6,   1662.6,   1662.6,   1662.6
		0.20f, 0.195f, 0.19f, 0.185f, 0.18f		// 1844,     1798.65,  1753.3,   1707.95,  1662.6
	};
	static float	fNewEndFog[30] =
	{
		0.223f,	0.227f,	0.233f,	0.238f,	0.244f,		// 2060,	2100,	2150,	2200,	2250
		0.255f,	0.223f,	0.223f,	0.223f,	0.223f,		// 2350,	2060,	2060,	2060,	2060
		0.223f,	0.223f,	0.223f,	0.223f,	0.223f,		// 2060,	2060,	2060,	2060,	2060
		0.223f,	0.223f,	0.223f,	0.223f,	0.223f,		// 2060,	2060,	2060,	2060,	2060
		0.223f,	0.223f,	0.223f,	0.223f,	0.223f,		// 2060,	2060,	2060,	2060,	2060
		0.244f,	0.238f,	0.233f,	0.227f,	0.223f		// 2250,	2200,	2150,	2100,	2060
	};

	if( GetCurWorld() != WORLD_SKY && !g_bDungeon )
	{
		float FogEnd, FogStart;				// for linear mode
		float FogDensity = 0.0f;			// for exponential mod
		if( g_bNewTerrain )
		{
			if( GetCurWorldID() == WORLD_BOSSRAID_BULKAN && ( m_nCurHour == 2 || m_nCurHour == 3))
			{
				FogEnd = fNewEndFog[1];
				FogStart = start_fog[1];
			}
			else if ( GetCurWorldID() == WORLD_BOSSRAID_FINALBOSS )
			{
				FogEnd	 = fNewEndFog[15];
				FogStart = start_fog[15] * 0.5f;
			}
			else
			{
				FogEnd = fNewEndFog[m_nCurHour];
				FogStart = start_fog[m_nCurHour];
			}

		}
		else
		{
			FogEnd = end_fog[m_nCurHour];
			FogStart = fNewStartFog[m_nCurHour];
		}

		if (!m_bCanDoVertexFog || !m_pd3dDevice)
			return;

		//wan:2004-11
		//Whitehorn Dungeon
		//???????? ?????? ?????? ?????? ??????
		if( g_bNewTerrainDungeon )
		{
			///????1
			if( m_nCurWorldIdx == 8 ) // ?????? ??
			{
				FogEnd = fNewEndFog[m_nCurHour]/2;
				FogStart = start_fog[m_nCurHour]/2;
				FogDensity = 0.7f;
				//?????? ????????
				SetFogParameters(D3DFOG_LINEAR, 0xffffff, FALSE, FALSE, FogStart, FogEnd, FogDensity);
				// ???? ???????? ???????? ?? ????.
				m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
				m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, 0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
			}
			else if( m_nCurWorldIdx == 1 ) // ?????? ????

			{
				FogEnd = 1.022f;
				FogStart = 0.06f;
				FogDensity = 0.7f;
				//?????? ????????
				SetFogParameters(D3DFOG_LINEAR, 0x00222222, FALSE, FALSE, FogStart, FogEnd, FogDensity);
				// ???? ???????? ???????? ?? ????.
				m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
				m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, 0x00222222);
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
			}
			else if(m_nCurWorldIdx == 29 ) // ?????? ????

			{
				FogEnd = 1.022f;
				FogStart = 0.06f;
				FogDensity = 0.7f;
				//?????? ????????
				SetFogParameters(D3DFOG_LINEAR, 0x00000000, FALSE, FALSE, FogStart, FogEnd, FogDensity);
				// ???? ???????? ???????? ?? ????.
				m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
				m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, 0x00000000);
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
				m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
				m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
			}
		}
		else if (m_bCanDoVertexFog && g_bDungeon != TRUE)
		{
			DWORD dwskyFogColor = g_skyFogColor[m_nCurHour];

			if( GetCurWorldID() == WORLD_BOSSRAID_HUMAN || GetCurWorldID() == WORLD_BOSSRAID_FINALBOSS )
				dwskyFogColor = 0x00000033;
			if( GetCurWorldID() == WORLD_BOSSRAID_BULKAN && ( m_nCurHour == 2 || m_nCurHour == 3) )
				dwskyFogColor = g_skyFogColor[1];

			SetFogParameters(D3DFOG_LINEAR, dwskyFogColor, FALSE, FALSE, FogStart, FogEnd, FogDensity);

			// ???? ???????? ???????? ?? ????.
			m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
			m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, m_dwFogColor);
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
		}
		else
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
	}
}



void CMyD3DApplication::RenderCutton()
{
	if( !m_pd3dDevice )
		return;

	static D3DTLVERTEX tlVtx[4];
	D3DVIEWPORT7 vp;
	LPDIRECTDRAWSURFACE7 lpSurface;

	m_pd3dDevice->GetViewport(&vp);

	tlVtx[0] = D3DTLVERTEX(D3DVECTOR_New(0,    vp.dwHeight,     0), 1, 0x00ffffff, 0, 0.0f, 1.0f);
	tlVtx[1] = D3DTLVERTEX(D3DVECTOR_New(0,    0,     0), 1, 0x00ffffff, 0, 0.0f, 0.0f);
	tlVtx[2] = D3DTLVERTEX(D3DVECTOR_New(vp.dwWidth,   vp.dwHeight,     0), 1, 0x00ffffff, 0, 1.0f, 1.0f);
	tlVtx[3] = D3DTLVERTEX(D3DVECTOR_New(vp.dwWidth,   0,     0), 1, 0x00ffffff, 0, 1.0f, 0.0f);

	static DWORD dwPrevTime = 0;
	DWORD dwCurTime = timeGetTime();
	static BOOL bToggle = TRUE;
	if (dwCurTime - dwPrevTime > 1000)
	{
		bToggle = !bToggle;
		dwPrevTime = dwCurTime;
	}

//========================================
	// nate 2004 - 7
	// Imaage Manager
	if (bToggle)
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_CUTTON_IDX1 ] );
	else
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_CUTTON_IDX2 ] );
//========================================

	m_pd3dDevice->SetTexture(0, lpSurface);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR);
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, tlVtx, 4, 0 );
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
}

void CMyD3DApplication::PlayMusic(int id)
{
	if (m_pDMusicMgr)
		m_pDMusicMgr->DirectMusicPlay(id, true);

	if( m_pMusicManager )
		m_pMusicManager->Play( id, true );
}

void CMyD3DApplication::StopMusic(int id)
{
	if (m_pDMusicMgr)
	{
		m_pDMusicMgr->DirectMusicStop();
		SAFE_DELETE(m_pDMusicMgr);
	}

	if( m_pMusicManager )
	{
		m_pMusicManager->StopAll();
		SAFE_DELETE( m_pMusicManager );
	}
}

// ----------------------------------------------------------
// Name : ChangeHour()
// Desc : ???? ?????? ???? ???? ?????? ?????? ???? ?????? ???? ?????? ????

// 4-8 : ????.
// 8-21 : ????

// 22-26 : ????.
// 27-03 : ????.
// ----------------------------------------------------------
void CMyD3DApplication::ChangeHour(int hour)
{
	// nate 2004 - 6 : ?????? ???? ???????? ???? ???????? ???????? ???? ???????? ????

	if( !m_pLand )
		return;

	static int current_hour = 6;
	// 4-8 : ????.
	// 8-21 : ????


	// 22-26 : ????.
	// 27-03 : ????.
	float fLightFactor[30] =
	{
		0.5f, 0.5f, 0.5f, 0.5f, 0.6f,	// 4
		0.7f, 0.8f, 0.9f, 1.0f, 1.0f,	// 9
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// 14
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// 19
		1.0f, 1.0f, 0.9f, 0.8f, 0.7f,	// 24
		0.6f, 0.5f, 0.5f, 0.5f, 0.5f	// 29
	};

	if(GetCurWorldID() == WORLD_BOSSRAID_HUMAN ) // ???????? ??????
		fLightFactor[hour] = 0.1f;
	else if( GetCurWorldID() == WORLD_BOSSRAID_BULKAN )
		fLightFactor[2] = fLightFactor[3] = fLightFactor[1];
	else if( GetCurWorldID() == WORLD_BOSSRAID_FINALBOSS )
		fLightFactor[hour] = 0.8f; // 23???? ????

	BOOL bDirLight[30] =
	{
		FALSE, FALSE, FALSE, FALSE, TRUE,
		TRUE, TRUE, TRUE, TRUE, TRUE,
		TRUE, TRUE, TRUE, TRUE, TRUE,
		TRUE, TRUE, TRUE, TRUE, TRUE,
		TRUE, TRUE, TRUE, TRUE, TRUE,
		TRUE, TRUE, TRUE, FALSE, FALSE
	};

	BOOL bPointLight[30] =
	{
		TRUE, TRUE, TRUE, TRUE, TRUE,
		TRUE, TRUE, FALSE, FALSE, FALSE,
		FALSE, FALSE, FALSE, FALSE, FALSE,
		FALSE, FALSE, FALSE, FALSE, FALSE,
		FALSE, FALSE, TRUE, TRUE, TRUE,
		TRUE, TRUE, TRUE, TRUE, TRUE
	};

	DWORD dwAmbient[30] =
	{
		0x00666666, 0x00666666, 0x00707070, 0x00707070, 0x00777777,
		0x00777777, 0x00808080, 0x00888888, 0x00888888, 0x00888888,
		0x00888888, 0x00888888, 0x00888888, 0x00888888, 0x00888888,
		0x00888888, 0x00888888, 0x00888888, 0x00888888, 0x00888888,
		0x00888888, 0x00888888, 0x00808080, 0x00777777, 0x00777777,
		0x00777777, 0x00707070, 0x00707070, 0x00666666, 0x00666666
	};

	if( GetCurWorldID() == WORLD_BOSSRAID_HUMAN ) // ???????? ??????
	{
		fLightFactor[hour]	= 0.5f;
		dwAmbient[hour]		= 0x00333333;
		bDirLight[hour]		= FALSE;
		bPointLight[hour]	= TRUE;
	}
	else if( GetCurWorldID() == WORLD_BOSSRAID_BULKAN )
	{
		dwAmbient[2] = 0x006a6a6a;
		dwAmbient[3] = 0x006c6c6c;
	}
	else if( GetCurWorldID() == WORLD_BOSSRAID_FINALBOSS ) // ???????? ??????
	{
		fLightFactor[hour]	= 0.8f;
		dwAmbient[hour]		= 0x00808080;
		bDirLight[hour]		= TRUE;
		bPointLight[hour]	= TRUE;
	}

	struct NkFColorDir
	{
		float r, g, b;
	};

	NkFColorDir DirColor[30] =
	{
		0.7f, 0.7f, 0.7f,		// 0
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.8f, 0.8f, 0.8f,		// 4
		0.8f, 0.8f, 0.8f,		// 5
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 10
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 15
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 20
		1.0f, 1.0f, 1.0f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,		// 25
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
	};

	NkFColorDir PointColor[30] =
	{
		0.8f, 0.8f, 0.8f,		// 0
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.8f, 0.8f, 0.8f,
		0.7f, 0.7f, 0.7f,		// 4
		0.7f, 0.7f, 0.7f,		// 5
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 10
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 15
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		// 20
		1.0f, 1.0f, 1.0f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,
		0.7f, 0.7f, 0.7f,		// 25
		0.7f, 0.7f, 0.7f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f,
	};

	// ?????? ???????? ???? ???????? RANGE ?????? ???????? ???? ???? ????..
	// ????
	int point_light_range[30] =
	{
		100,	100,	90,		80,		70,
		70,		60,		60,		60,		60,
		0,		0,		0,		0,		0,
		0,		0,		0,		0,		0,
		60,		60,		60,		60,		70,
		70,		80,		90,		100,	100,
	};

	// ???? ???????? ???? ?????? ????..
	float point_light_fac[30] =
	{
		0.6f,	0.6f,	0.6f,	0.6f,	0.6f,
		0.6f,	0.5f,	0.4f,	0.4f,	0.4f,
		0,		0,		0,		0,		0,
		0,		0,		0,		0,		0,
		0.4f,	0.4f,	0.4f,	0.5f,	0.6f,
		0.6f,	0.6f,	0.6f,	0.6f,	0.6f,
	};

	if( GetCurWorldID() == WORLD_BOSSRAID_HUMAN )
	{
		DirColor[hour].b	= 0.7f;
		DirColor[hour].r	= 0.7f;
		DirColor[hour].g	= 0.7f;

		point_light_range[hour] = 100;
		point_light_fac[hour]   = 0.6f;
	}
	else  if( GetCurWorldID() == WORLD_BOSSRAID_FINALBOSS )
	{
		DirColor[hour].b	= 0.7f;
		DirColor[hour].r	= 0.7f;
		DirColor[hour].g	= 0.7f;

		point_light_range[hour] = 60;
		point_light_fac[hour]   = 0.4f;
	}

	if (hour < 0 || current_hour >= 30)
		current_hour = 0;
	else
		current_hour = hour;

	// ?????? ????
	m_nCurHour = current_hour;

	// ?????? ?????? ??????.
	// ?????? ?????????? ???? ????.
	if (g_bDungeon||g_bNewTerrainDungeon)
		g_pRoh->SetCurrentHour(12);	// ??????.
	else if (GetCurWorld() == WORLD_SKY)
		g_pRoh->SetCurrentHour(12);	// ??????.
	else
		g_pRoh->SetCurrentHour(current_hour);

	// ?????? ???? ????.
	if( g_bNewTerrain )
	{
		//wan:2004-11
		//Whitehorn Dungeon
		//???????? ???????? ????
		if(g_bNewTerrainDungeon)
		{
			///????1
			if( m_nCurWorldIdx == 8 ) // ?????? ??
				m_pLand->SetWorldTime( 22 );
			else if( m_nCurWorldIdx == 1 ) // ?????? ????
				m_pLand->SetWorldTime( 27 );
			else if( m_nCurWorldIdx == 29 ) // ?????? ????
				m_pLand->SetWorldTime( 27 );
		}
		else
			// Update the light map of the terrain
			m_pLand->SetWorldTime( current_hour );
		// ?????? ???????????? ???? ????
	}
	else
	{
		// ???? ?????? ???? ?????? ??????.
		if (GetCurWorld() == WORLD_SKY)
			m_pLand->SetHourLight(FALSE);
		else
			m_pLand->SetHourLight(TRUE);

		if (g_bDungeon)
			m_pLand->SetHourLightDungeon(80.0f, 0.8f);
		else if (GetCurWorld() == WORLD_SKY)
			m_pLand->SetHourLight(FALSE);
		else
		{
			// SetLightFactor?? ???? ?????? ???? ????????. (?????????? ????)
			m_pLand->SetLightFactor(fLightFactor[current_hour] /*0.28f*/);
			// ?????????? ???? ???????? ????
			m_pLand->SetHourLight(bPointLight[current_hour]);
			// ?????? ???? ???? ???????? ???? ?? ???? ????
			m_pLand->SetHourLightLand(point_light_range[current_hour], point_light_fac[current_hour]);
		}
	}

	if( !m_pLightCtrl )
		return;

	// ambient ????.
	if (g_bDungeon ||g_bNewTerrainDungeon)
		m_pLightCtrl->SetAmbientLight(0x00888888);
	else if( m_nCurWorldIdx == WORLD_SKY) // ?????? ??  (???????? ?????? ????)
		m_pLightCtrl->SetAmbientLight(0x003D6EA0);
	else
		m_pLightCtrl->SetAmbientLight(dwAmbient[current_hour]);

	// directional ????.
	if (g_bDungeon||g_bNewTerrainDungeon)
	{
		// Dungeon???? ?????? ???????? ????.
		m_pLightCtrl->LightEnable(NKLIGHT_DIRECTIONAL, TRUE);
		m_pLightCtrl->InitLight(NKLIGHT_DIRECTIONAL, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	}
	else if ( m_nCurWorldIdx == WORLD_SKY)      // ?????? ??
	{
		m_pLightCtrl->LightEnable(NKLIGHT_DIRECTIONAL, TRUE);
		m_pLightCtrl->InitLight(NKLIGHT_DIRECTIONAL, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_pLightCtrl->LightEnable(NKLIGHT_DIRECTIONAL, !bPointLight[current_hour]);
		m_pLightCtrl->InitLight(NKLIGHT_DIRECTIONAL, D3DLIGHT_DIRECTIONAL, -1.0f, -1.0f, 0.0f, 0x00000000, 0x00000000, 0x00000000);
	}

	// point ????.
	// point?? ?????? ?????? ??????.
	// ???? ?????????
	if (g_bDungeon||g_bNewTerrainDungeon)
	{
		// ?????????? ?????? ?????? ???????? ????????.
		m_pLightCtrl->LightEnable(NKLIGHT_POINT, FALSE);

		if(true)
		{
			float dx, dz;
			GetDirectionElement(g_pRoh->m_dir, dx, dz);
			m_pLightCtrl->InitLight(NKLIGHT_POINT, D3DLIGHT_POINT,
									m_vAt.x+dx*10.0f, m_pLand->GetHFHeight(m_vAt.x, m_vAt.z)+25.0f, m_vAt.z+dz*10.0f,
									PointColor[current_hour].r, PointColor[current_hour].g, PointColor[current_hour].b);
			m_fPointRColor = 1.0f;
			m_fPointGColor = 1.0f;
			m_fPointBColor = 1.0f;

			// ?????? ???? ?????? ???????? ???????? ?????? ???? ????..
			m_pLightCtrl->SetPointLightRange(point_light_range[24]); // ?????? ????.
		}
	}
	else if (GetCurWorld() == WORLD_SKY)
		m_pLightCtrl->LightEnable(NKLIGHT_POINT, FALSE);
	else
	{
		m_pLightCtrl->LightEnable(NKLIGHT_POINT, bPointLight[current_hour]);
		// ?????? ?????? ???????? ????.
		//m_pLightCtrl->LightEnable(NKLIGHT_POINT, FALSE);

		if (bPointLight[current_hour])
		{
			float dx, dz;
			GetDirectionElement(g_pRoh->m_dir, dx, dz);
			m_pLightCtrl->InitLight(NKLIGHT_POINT, D3DLIGHT_POINT,
									m_vAt.x+dx*10.0f, m_pLand->GetHFHeight(m_vAt.x, m_vAt.z)+25.0f, m_vAt.z+dz*10.0f,
									PointColor[current_hour].r, PointColor[current_hour].g, PointColor[current_hour].b);
			m_fPointRColor = PointColor[current_hour].r;
			m_fPointGColor = PointColor[current_hour].g;
			m_fPointBColor = PointColor[current_hour].b;

			// ?????? ???? ?????? ???????? ???????? ?????? ???? ????..
			m_pLightCtrl->SetPointLightRange(point_light_range[current_hour]);
		}
	}
}

//---------------------------------------------------------------------------
// Name: CheckDeviceCaps()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//---------------------------------------------------------------------------
HRESULT CMyD3DApplication::CheckDeviceCaps()
{
	if( !m_pd3dDevice )
		return E_FAIL;

	D3DDEVICEDESC7 ddDesc;
	m_pd3dDevice->GetCaps( &ddDesc );

	m_bCanRender32Bit = ddDesc.dwDeviceRenderBitDepth & DDBD_32;

	m_bCanDoMultitexture = FALSE;
	if (ddDesc.wMaxTextureBlendStages > 1)
		m_bCanDoMultitexture = TRUE;

	m_bCanDoAdd = FALSE;
	if (ddDesc.dwTextureOpCaps & D3DTEXOPCAPS_ADD && ddDesc.wMaxSimultaneousTextures > 1)
		m_bCanDoAdd = TRUE;

	m_bCanDoModulate = FALSE;
	if (ddDesc.dwTextureOpCaps & D3DTEXOPCAPS_MODULATE && ddDesc.wMaxSimultaneousTextures > 1)
		m_bCanDoModulate = TRUE;

	m_bCanDoModulate2X= FALSE;
	if (ddDesc.dwTextureOpCaps & D3DTEXOPCAPS_MODULATE2X && ddDesc.wMaxSimultaneousTextures > 1)
		m_bCanDoModulate2X = TRUE;

	m_bCanDoAddSigned = FALSE;
	if ( ddDesc.dwTextureOpCaps & D3DTEXOPCAPS_ADDSIGNED)
		m_bCanDoAddSigned = TRUE;

	BOOL	bExecuteBufferInVMem = ddDesc.dwDevCaps & D3DDEVCAPS_EXECUTEVIDEOMEMORY;
	m_bCanHighQuality = FALSE;
	if( bExecuteBufferInVMem && m_bCanDoMultitexture && m_bCanDoModulate2X )
		m_bCanHighQuality = TRUE;

	BOOL bSrcColor = ddDesc.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_SRCCOLOR;
	BOOL bDestColor = ddDesc.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_DESTCOLOR;

	if (!m_bCanDoMultitexture && !(bSrcColor && bDestColor) )
	{
		// device couldn't do the alpha blending
		MessageBox(NULL, "Device can not do alpha blending", "Warning", MB_OK);
		return E_FAIL;
	}

	// Fog?? ???????? ???? Check
	DWORD dwCaps = ddDesc.dpcTriCaps.dwRasterCaps;

	m_bCanDoTableFog	= (dwCaps&D3DPRASTERCAPS_FOGTABLE)  ? TRUE : FALSE;
	m_bCanDoTableFog	= FALSE;
	m_bCanDoVertexFog	= (dwCaps&D3DPRASTERCAPS_FOGVERTEX) ? TRUE : FALSE;
	m_bCanDoWFog		= (dwCaps&D3DPRASTERCAPS_WFOG)      ? TRUE : FALSE;

	// ???? ?????? ???????? ?????? ?????? ???????? ???? ???? ???? ???? ????.
	FILE *fp = NULL;
	if ( (fp=fopen("fog_disable.txt", "rt")) != NULL)
	{
		m_bCanDoVertexFog = FALSE;
		fclose(fp);
	}

	return S_OK;
}
//---------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//---------------------------------------------------------------------------

HRESULT CMyD3DApplication::ConfirmDevice( DDCAPS* pddDriverCaps )
{
	return S_OK;
}


void CMyD3DApplication::ExcuteOption()
{
	if( !m_pDeviceInfo )
		return;

	if( 32 == m_ProOption.nBackBufferBit && !m_bCanRender32Bit )
		m_ProOption.nBackBufferBit = 16;

	int i;

	{
		if(m_ProOption.resolution == R640)
			m_ProOption.resolution = R800;
	}

	int sizeX = 640;
	int sizeY = 480;

	if (m_ProOption.resolution == R800)
	{
		sizeX = 800;
		sizeY = 600;		
	}
	else if (m_ProOption.resolution == R1024)
	{	
		sizeX = 1024;
		sizeY = 768;
	}
	else if( m_ProOption.resolution == R1280 )
	{
		sizeX = 1280;
		sizeY = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		sizeX = 1280;
		sizeY = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		sizeX = 1600;
		sizeY = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		sizeX = 1920;
		sizeY = 1080;
	}

	for (i=0; i < m_pDeviceInfo->dwNumModes; i++)
	{
		if( ( m_pDeviceInfo->pddsdModes[i].dwWidth == sizeX ) &&
			( m_pDeviceInfo->pddsdModes[i].dwHeight == sizeY ) &&
			( m_pDeviceInfo->pddsdModes[i].ddpfPixelFormat.dwRGBBitCount == m_ProOption.nBackBufferBit ) )
		{
			m_pDeviceInfo->ddsdFullscreenMode = m_pDeviceInfo->pddsdModes[i];
			m_pDeviceInfo->dwCurrentMode      = i;
		}
	}

	if( m_ProOption.resolution != R800 ||
			( m_ProOption.resolution == R800 && m_ProOption.nBackBufferBit == 32 ) )
	{
		m_bWindowed = FALSE;
		m_pDeviceInfo->bWindowed = 0;
		m_pDeviceInfo->bStereo = 0;
		Change3DEnvironment();

		if( g_pDisplay )
		{
			g_pDisplay->SetDirectDraw( m_pDD );
			g_pDisplay->SetBackBuffer( m_pddsRenderTarget );
		}
	}
}

void CMyD3DApplication::ExcuteFFullOption()
{
	if( m_ProOption.resolution == R800 )
	{
		return;
	}

	int sizeX = 640;
	int sizeY = 480;

	if (m_ProOption.resolution == R1024)
	{
		sizeX = 1024;
		sizeY = 768;
	}
	else if( m_ProOption.resolution == R1280 )
	{
		sizeX = 1280;
		sizeY = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		sizeX = 1280;
		sizeY = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		sizeX = 1600;
		sizeY = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		sizeX = 1920;
		sizeY = 1080;
	}

	DeleteDeviceObjects();
	GetFramework()->DestroyObjects();
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, sizeX, sizeY, SWP_SHOWWINDOW | SWP_NOSENDCHANGING);

	m_bWindowed = TRUE;
	m_pDeviceInfo->bWindowed = TRUE;
	m_pDeviceInfo->bStereo = 0;

	if (g_pDisplay)
	{
		g_pDisplay->SetDirectDraw(m_pDD);
		g_pDisplay->SetBackBuffer(m_pddsRenderTarget);
	}
	
}


void CMyD3DApplication::ExcuteWindowedOption()
{
	RECT rect;
	DWORD dwStyle;

	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		
	if( m_ProOption.resolution == R640 )
	{
		m_ProOption.resolution = R800;
	}
	
	if( m_ProOption.resolution == R800 )
	{
		return;
	}

	// ?????? ????.
	int sizeX = 640;
	int sizeY = 480;
	if (m_ProOption.resolution == R1024)
	{
		sizeX = 1024;
		sizeY = 768;
	}
	else if( m_ProOption.resolution == R1280 )
	{
		sizeX = 1280;
		sizeY = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		sizeX = 1280;
		sizeY = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		sizeX = 1600;
		sizeY = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		sizeX = 1920;
		sizeY = 1080;
	}

	DeleteDeviceObjects();
	GetFramework()->DestroyObjects();

	rect.left = 0;
	rect.top = 0;
	rect.right = sizeX;
	rect.bottom = sizeY;
	if (g_bAdmin)
		AdjustWindowRect(&rect, dwStyle, TRUE);
	else
		AdjustWindowRect(&rect, dwStyle, FALSE);
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW | SWP_NOSENDCHANGING);
	
	m_bWindowed = TRUE;
	m_pDeviceInfo->bWindowed = TRUE;
	m_pDeviceInfo->bStereo = 0;


	if (g_pDisplay)
	{
		g_pDisplay->SetDirectDraw(m_pDD);
		g_pDisplay->SetBackBuffer(m_pddsRenderTarget);
	}	
}

void CMyD3DApplication::SmartExit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !g_pNk2DFrame )
		return;

	switch(m_GraphicMode)
	{
	case NKGRAPHIC2D:

		if(g_pNk2DFrame->GetSheet()->m_pLoginPg->m_MsgPopUp.m_bActive == TRUE)
		{
			g_pNk2DFrame->GetSheet()->m_pLoginPg->m_MsgPopUp.m_bActive = FALSE;
			return;
		}

		if(g_pNk2DFrame->GetSheet()->m_pSelCharPg2->m_MsgPopUp.m_bActive == TRUE)
		{
			g_pNk2DFrame->GetSheet()->m_pSelCharPg2->m_MsgPopUp.m_bActive = FALSE;
			return;
		}

		if(g_pNk2DFrame->GetSheet()->m_pCrtCharPg->m_MsgPopUp.m_bActive == TRUE)
		{
			g_pNk2DFrame->GetSheet()->m_pCrtCharPg->m_MsgPopUp.m_bActive = FALSE;
			return;
		}

		if(g_pNk2DFrame->GetSheet()->m_pLoginPg->m_GoHomePage.m_bActive == TRUE)
		{
			g_pNk2DFrame->GetSheet()->m_pLoginPg->m_GoHomePage.m_bActive = FALSE;
			return;
		}

		// IDS_MAIN_END_GAME : ?????? ??????????
		m_MsgPopUp.PutString((char*)G_STRING(IDS_MAIN_END_GAME));
		break;

	case NKGRAPHIC3D:

		if( g_pNk2DFrame->m_Popup )		// POPUP
		{
			if( g_pNk2DFrame->m_Popup->nProcess != POPUP_GUILD_RANKWAR_INVITE )
				g_pNk2DFrame->DeletePopup();

			return;
		}


		if(g_pNk2DFrame->IsBossraidInfoVisible() == TRUE)
		{
			g_pNk2DFrame->ShowBossraidInfoWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsInvenVisible() == TRUE)
		{
			if(!g_pNk2DFrame->GetGamble()->m_bGambleStart)
			{
				g_pNk2DFrame->ShowInvenWindow(FALSE);
				g_pNk2DFrame->ShowWareHouseWindow(FALSE);
				return;
			}
		}

		if( g_pNk2DFrame->IsHeroPrivilegeVisable())
		{
			g_pNk2DFrame->ShowHeroPrivilegeWindow( FALSE );
			return;
		}

		if( g_pNk2DFrame->IsHeroRankListeVisable())
		{
			g_pNk2DFrame->ShowHeroRankListWindow( FALSE );
			return;
		}

		if(g_pNk2DFrame->IsWareHouseVisible())
		{
			g_pNk2DFrame->ShowInvenWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsHelperVisible() )
		{
			g_pNk2DFrame->ShowHelperWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(MAP) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, MAP);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(SCORE) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, SCORE);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(GATE) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, GATE);
			m_pUIMgr->m_bMoveKit  = FALSE;
			m_pUIMgr->m_bParosKit = FALSE;
			m_pUIMgr->m_bDmitron  = FALSE;
			m_pUIMgr->m_bMarboden = FALSE;	// [2008/6/23 Theodoric] ???????? ????????
			return;
		}

		if(g_pNk2DFrame->IsSkillVisible() == TRUE)
		{
			g_pNk2DFrame->ShowSkillWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsHelpVisible() == TRUE)
		{
			g_pNk2DFrame->ShowHelpWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(QUEST))
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(SOCIAL) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE,SOCIAL);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(DOCUMENT) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, DOCUMENT);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(YUTBOARD) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, YUTBOARD);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(FISHING) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, FISHING);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(FISHING_REWARD) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, FISHING_REWARD);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(BASEBALL) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, BASEBALL);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(BASEBALL_REWARD) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, BASEBALL_REWARD);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(LOTTO) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, LOTTO);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(BINGO) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, BINGO);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(LOTTERY) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, LOTTERY);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(DICEGAME) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, DICEGAME);
			return;
		}

		if(g_pNk2DFrame->IsInterfaceVisible(DICEGAME_REWARD) == TRUE)
		{
			g_pNk2DFrame->ShowInterfaceWindow(FALSE, DICEGAME_REWARD);
			return;
		}

// 		if(g_pNk2DFrame->IsInterfaceVisible(LETTER_WINDOW) == TRUE)
// 		{
// 			g_pNk2DFrame->ShowInterfaceWindow(FALSE, LETTER_WINDOW);
// 			return;
// 		}
// 
// 		if(g_pNk2DFrame->IsInterfaceVisible(LETTER_SEND) == TRUE)
// 		{
// 			g_pNk2DFrame->ShowInterfaceWindow(FALSE, LETTER_SEND);
// 			return;
// 		}
// 
// 		if(g_pNk2DFrame->IsInterfaceVisible(LETTER_RECV) == TRUE)
// 		{
// 			g_pNk2DFrame->ShowInterfaceWindow(FALSE, LETTER_RECV);
// 			return;
// 		}

		if(g_pNk2DFrame->IsPartyVisible() )
		{
			g_pNk2DFrame->ShowPartyWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsPetStatusVisible() == TRUE)
		{
			g_pNk2DFrame->ShowPetStatusWindow(FALSE);
			return;
		}

		if(g_pNk2DFrame->IsLevelMenu() ==TRUE)
		{
			g_pNk2DFrame->GetPetLevel()->CloseMenu();
			return;
		}

		if(g_pNk2DFrame->IsHelperVisible() )	//??????
		{
			g_pNk2DFrame->ShowHelperWindow(FALSE);
			return;
		}

		if( m_pUIMgr->m_pScrollBox )
		{
			SAFE_DELETE(m_pUIMgr->m_pScrollBox);
			m_pUIMgr->m_bIsScrollBox = FALSE;
			return;
		}

		if( g_pNk2DFrame->IsRefineVisible() )
		{
			g_pNk2DFrame->ShowRefineWindow(FALSE);
			return;
		}

		if(m_pUIMgr->m_bIsDelivBox)
		{
			if(m_pUIMgr->m_pDelivBox)
			{
				SAFE_DELETE(m_pUIMgr->m_pDelivBox);
				m_pUIMgr->m_bIsDelivBox = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsScrollBox)
		{
			if(m_pUIMgr->m_pScrollBox)
			{
				SAFE_DELETE(m_pUIMgr->m_pScrollBox)
				m_pUIMgr->m_bIsScrollBox = FALSE;
				return;
			}
		}

		if(g_pNk2DFrame->IsRShiftVisible())
		{
			g_pNk2DFrame->GetRShiftMenu()->CloseMenu();
			return;
		}

		if(m_pUIMgr->m_bIsChSexBox)
		{
			if(m_pUIMgr->m_pChSexBox)
			{
				SAFE_DELETE(m_pUIMgr->m_pChSexBox);
				m_pUIMgr->m_bIsChSexBox = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsChangeHair)
		{
			if(m_pUIMgr->m_pChangeHair)
			{
				SAFE_DELETE(m_pUIMgr->m_pChangeHair);
				m_pUIMgr->m_bIsChangeHair = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsWarTax)
		{
			if(m_pUIMgr->m_pWarTax)
			{
				SAFE_DELETE(m_pUIMgr->m_pWarTax);
				m_pUIMgr->m_bIsWarTax = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsTower)
		{
			if(m_pUIMgr->m_pTower)
			{
				SAFE_DELETE(m_pUIMgr->m_pTower);
				m_pUIMgr->m_bIsTower = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsPetShop)
		{
			if(m_pUIMgr->m_pPetShop)
			{
				SAFE_DELETE(m_pUIMgr->m_pPetShop);
				m_pUIMgr->m_bIsPetShop = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsPetKeep)
		{
			if(m_pUIMgr->m_pPetKeep)
			{
				SAFE_DELETE(m_pUIMgr->m_pPetKeep);
				m_pUIMgr->m_bIsPetKeep = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsWarReq)
		{
			if(m_pUIMgr->m_pWarReq)
			{
				SAFE_DELETE(m_pUIMgr->m_pWarReq);
				m_pUIMgr->m_bIsWarReq = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsGuildWar)
		{
			if(m_pUIMgr->m_pGuildWar)
			{
				SAFE_DELETE(m_pUIMgr->m_pGuildWar);
				m_pUIMgr->m_bIsGuildWar = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsMatMakeRoom)
		{
			if(m_pUIMgr->m_pMatMakeRoom)
			{
				SAFE_DELETE(m_pUIMgr->m_pMatMakeRoom);
				m_pUIMgr->m_bIsMatMakeRoom = FALSE;
				return;
			}
		}

		if(m_pUIMgr->m_bIsMatList)
		{
			if (m_pUIMgr->m_pMatList)
			{
				SAFE_DELETE(m_pUIMgr->m_pMatList);
				m_pUIMgr->m_bIsMatList = FALSE;
				return;
			}
		}

		if( m_pUIMgr->m_bIsGuildHunt ) // ???? ????
		{
			if( m_pUIMgr->m_pGuildHunt )
			{
				m_pUIMgr->m_bIsGuildHunt = FALSE;
				SAFE_DELETE( m_pUIMgr->m_pGuildHunt );
				return;
			}
		}

		if( m_pUIMgr->m_bIsReservationWindow )
		{
			if( m_pUIMgr->m_pReservationWindow )
			{
				m_pUIMgr->m_pReservationWindow->SendQuestApplyMsg( 2 );	// GUILDHUNT_CLOSE == 2
				m_pUIMgr->m_bIsReservationWindow = FALSE;
				SAFE_DELETE( m_pUIMgr->m_pReservationWindow );
				return;
			}
		}

		{
			if(g_pNk2DFrame->IsControlGuild() == TRUE)
			{
				g_pNk2DFrame->ShowControlGuild(FALSE);
				return;
			}

			if(g_pNk2DFrame->IsControlGuildManage() == TRUE)
			{
				g_pNk2DFrame->ShowControlGuildManage(FALSE);
				return;
			}

			if( g_pNk2DFrame->GetUIWindow() && g_pNk2DFrame->GetUIWindow()->IsOpenWindow())
			{
				g_pNk2DFrame->GetUIWindow()->CloseTopWindow();
				return;
			}
		}
		if( g_SvrType == ST_ADULT_ONLY )
		{
			if( g_pNk2DFrame->GetTgaUIWindow() && g_pNk2DFrame->GetTgaUIWindow()->IsOpenWindow())
			{
				g_pNk2DFrame->GetTgaUIWindow()->CloseTopWindow();
				return;
			}
		}
		// IDS_MAIN_END_GAME : ?????? ??????????
		m_MsgPopUp.PutString((char*)G_STRING(IDS_MAIN_END_GAME));//"Exit Game");
	}
}

//---------------------------------------------------------------------------
// Name: EnumPickZBufferFormatsCallback()
// Desc: Simply returns the first matching enumerated z-buffer format
//      d3dframe.cpp?? ???? ?? ?????? ???? ???? ?????? ??????.
//---------------------------------------------------------------------------
static HRESULT WINAPI EnumPickZBufferFormatsCallback( DDPIXELFORMAT* pddpf,
		VOID* pContext )
{
	DDPIXELFORMAT* pddpfOut = (DDPIXELFORMAT*)pContext;

	if( pddpfOut->dwRGBBitCount == pddpf->dwRGBBitCount )
	{
		(*pddpfOut) = (*pddpf);
		return D3DENUMRET_CANCEL;
	}

	return D3DENUMRET_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////
// Picking Buffer
HRESULT	CMyD3DApplication::CreatePickBuffer()
{
	m_nPickWidth = GetFramework()->GetRenderWidth();
	m_nPickHeight = GetFramework()->GetRenderHeight();

	HRESULT hr;
	DDSURFACEDESC2 ddsd;

	// For Safety
	DeletePickBuffer();

	// ?????? ???? ??????.
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS;

	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = m_nPickWidth;
	ddsd.dwHeight       = m_nPickHeight;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;

	if( !m_pDD || FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsPickRender, NULL ) ) )
	{
		DEBUG_ERR( hr, "Error: Couldn't create Picking Rendering Target" );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( "Error: Out of video memory" );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	// z-Buffer ??????.
	// Check if the device supports z-bufferless hidden surface removal. If so,
	// we don't really need a z-buffer
	D3DDEVICEDESC7 ddDesc;
	m_pd3dDevice->GetCaps( &ddDesc );
	if( ddDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
	{
		OutputDebugString("Z-Buffer ???? ????");
		return S_OK;
	}

	// Get z-buffer dimensions from the render target
	//DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	if( m_pddsPickRender )
		m_pddsPickRender->GetSurfaceDesc( &ddsd );

	// Setup the surface desc for the z-buffer.
	DWORD dwDeviceMemType;
	dwDeviceMemType = GetFramework()->GetDeviceMemType();
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | dwDeviceMemType;
	ddsd.ddpfPixelFormat.dwSize = 0;  // Tag the pixel format as unitialized.

	// Get an appropiate pixel format from enumeration of the formats. On the
	// first pass, we look for a zbuffer dpeth which is equal to the frame
	// buffer depth (as some cards unfornately require this).

	// ???? ???? ????


	D3DEnum_DeviceInfo* pDeviceInfo;
	pDeviceInfo = GetDeviceInfo();
	m_pD3D->EnumZBufferFormats( *(pDeviceInfo->pDeviceGUID) , EnumPickZBufferFormatsCallback,
								(VOID*)&ddsd.ddpfPixelFormat );
	if( 0 == ddsd.ddpfPixelFormat.dwSize )
	{
		// Try again, just accepting any 16-bit zbuffer
		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
		m_pD3D->EnumZBufferFormats( *(pDeviceInfo->pDeviceGUID), EnumPickZBufferFormatsCallback,
									(VOID*)&ddsd.ddpfPixelFormat );

		if( 0 == ddsd.ddpfPixelFormat.dwSize )
		{
			DEBUG_MSG( _T("Device doesn't support requested zbuffer format") );
			return D3DFWERR_NOZBUFFER;
		}
	}

	// Create and attach a z-buffer
	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsPickZBuffer, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't create a ZBuffer surface for Picking") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOZBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	if( FAILED( m_pddsPickRender->AddAttachedSurface( m_pddsPickZBuffer ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
		return D3DFWERR_NOZBUFFER;
	}

	return S_OK;
}

void CMyD3DApplication::DeletePickBuffer()
{
	SAFE_RELEASE( m_pddsPickZBuffer);
	SAFE_RELEASE( m_pddsPickRender );
}

void CMyD3DApplication::ClearPickBuffer()
{
	if( !m_pd3dDevice )
		return;

	LPDIRECTDRAWSURFACE7 lpRenderTarget = NULL;
	static D3DVIEWPORT7 vpOld, vpNew;

	m_pd3dDevice->GetRenderTarget(&lpRenderTarget);				// ???????????? 1????????.
	m_pd3dDevice->SetRenderTarget(m_pddsPickRender, 0);
	m_pd3dDevice->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth = vpOld.dwWidth / 2;
	vpNew.dwHeight = vpOld.dwHeight / 2;
	m_pd3dDevice->SetViewport(&vpNew);
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DRGB(0, 0, 0), 1.0f, 0L );
	m_pd3dDevice->SetRenderTarget(lpRenderTarget, 0);
	if( lpRenderTarget )
		lpRenderTarget->Release();			// ??????.. ????????..
	m_pd3dDevice->SetViewport(&vpOld);
}




void CMyD3DApplication::ReadAndAppSkill()
{
	// ???????? ???? ????.
	char strPath[MAX_PATH+_MAX_FNAME+_MAX_EXT] = "";
	char strTemp[256] = "";
	int n = 0;
	strcpy (m_sCheckStr,"<\\\":>?|*/");
	m_sCheckStr[9]='\0';
	int  size  = strlen(m_SaveChaName);
	char tmp[30] = "";
	strcpy(tmp, m_SaveChaName);

	// ???????? ???????? ?????? ?????? ???????????? ???? ???? ???????????????? "-"?? ?????? ????
	for(int i=0; i<9; i++)
	{
		for(int ii=0; ii<size; ii++)
		{
			if (m_sCheckStr[i] == tmp[ii])
			{
				tmp[ii] = '_';
			}
		}
	}

	_getcwd(strPath, _MAX_PATH+_MAX_FNAME+_MAX_EXT);
	strcat(strPath, "//");
	strcat(strPath, tmp);
	strcat(strPath, ".cfg");

	FILE* fp;
	if ( (fp = fopen(strPath, "rt")) == NULL)    // ?????? ???????? ???????? ?????? ???? ??????
	{
		SetDefaultVal();
		return;
	}

	fgets(strTemp, 256, fp);
	n = sscanf(strTemp, "%s %d %d", &m_SaveChaName, &m_SaveCha_Rskill, &m_SaveCha_Lskill);
	if (n != 3)
	{
		// ????.
		SetDefaultVal();
		fclose(fp);
		return;
	}

	// ?????? ?????? ???????? ?????? ????????
	// ???? ?????? ??????..?????? ???? ???????? ???????? ???? ???????? ?????? ????
	// ???????? ???? ?????? ?????? ?????? ????????
	if (m_SaveCha_Rskill >=0 && g_ChaSkill.m_bRight[m_SaveCha_Rskill]== TRUE)
	{
	}
	else
	{
		if(m_SaveCha_Rskill == -1)
			m_SaveCha_Rskill = -1;
		else
			m_SaveCha_Rskill = -2;
	}

	if (m_SaveCha_Lskill >=0 && g_ChaSkill.m_bLeft[m_SaveCha_Lskill]== TRUE)
	{
	}
	else
	{
		m_SaveCha_Lskill = -1; // ???? ???? ????
	}

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
	{
		// ???????? ?????? ???????????? ???????? ?????????? ??????????
		g_pNk2DFrame->SetRSkill(m_SaveCha_Rskill);
		g_pNk2DFrame->SetLSkill(m_SaveCha_Lskill);

		char ttmp[20] = "";
		for(int j=0; j<FUNCTION_NUM; j++)
		{
			fgets(strTemp, 256, fp);
			n = sscanf(strTemp, "%s %d %d", &ttmp ,&g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].type
					   ,&g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].skill);
			if (n != 3)
			{
				// ????.
				SetDefaultVal();
				fclose(fp);
				return;
			}
			g_pNk2DFrame->GetControlBottom()->GetKeyText()[j].m_Type = g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].type;
		}
	}
	fclose(fp);
}

void CMyD3DApplication::SetDefaultVal()
{
	m_SaveCha_Rskill = -2; // ???? ????
	m_SaveCha_Lskill = -1; // ???? ???? ????
	if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
	{
		for(int j=0; j<FUNCTION_NUM; j++)
		{
			g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].type  = _TYPE_NONE;
			g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].skill = -99;
		}
	}
}



void CMyD3DApplication::WriteLastSkill()
{
	// ???????? ???? ????.
	char strPath[_MAX_PATH+_MAX_FNAME+_MAX_EXT];
	int  size  = strlen(m_SaveChaName);

	for(int i=0; i<9; i++)
	{
		for(int ii=0; ii<size; ii++)
		{
			if (m_sCheckStr[i] == m_SaveChaName[ii])
			{
				m_SaveChaName[ii] = '_';
			}
		}
	}

	strcpy(strPath, GetExePath());
	strcat(strPath, "//");
	strcat(strPath, m_SaveChaName);
	strcat(strPath, ".cfg");
	FILE *fp = NULL;
	if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
	{
		if ( (fp = fopen(strPath, "w")) == NULL)
			return;

		fprintf(fp, "%s	%d	%d\n", m_SaveChaName,
				g_pNk2DFrame->GetRSkill(),
				g_pNk2DFrame->GetLSkill());
		// f2 ???? f8????
		for(int j=0; j<FUNCTION_NUM; j++)
		{
			fprintf(fp, "f%d   %d     %d\n", j+2, g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].type
					, g_pNk2DFrame->GetControlBottom()->GetQuickSkill()[j].skill);
		}

		fclose(fp);
	}
}


void CMyD3DApplication::RenderSky()
{
	if( !m_pd3dDevice )
		return;

	if(m_nCurHour < 0 || m_nCurHour > 29)
		return;

	m_dwFogColor = 0xFF888888;

	D3DVIEWPORT7 vp;
	m_pd3dDevice->GetViewport( &vp );
	m_CloudMesh[0].sy = (FLOAT)vp.dwHeight/4 * 3;
	m_CloudMesh[2].sy = (FLOAT)vp.dwHeight/4 * 3;
	m_CloudMesh[2].sx = (FLOAT)vp.dwWidth;
	m_CloudMesh[3].sx = (FLOAT)vp.dwWidth;

	m_CloudLayerMesh[0].sy = (FLOAT)vp.dwHeight/4 * 3;
	m_CloudLayerMesh[2].sy = (FLOAT)vp.dwHeight/4 * 3;
	m_CloudLayerMesh[2].sx = (FLOAT)vp.dwWidth;
	m_CloudLayerMesh[3].sx = (FLOAT)vp.dwWidth;

	static float fAccTimeKey;
	fAccTimeKey += g_fTimeKey;
	FLOAT tu = (fAccTimeKey/100)-(FLOAT)floor(fAccTimeKey/100);
	tu = (tu+m_fRotate_Cloud_Angle);

	if (tu > 1.0f)
		tu -= 1.0f;

	m_CloudMesh[0].tu = m_CloudMesh[1].tu = tu;
	m_CloudMesh[2].tu = m_CloudMesh[3].tu = tu - 1.0f;

	static float fAccTimeKey2;
	fAccTimeKey2 += g_fTimeKey;
	FLOAT tu2 = (fAccTimeKey2/40)-(FLOAT)floor(fAccTimeKey2/40);
	tu2 = (tu2+m_fRotate_Cloud_Angle);

	if (tu2 > 1.0f)
		tu2 -= 1.0f;

	m_CloudLayerMesh[0].tu = m_CloudLayerMesh[1].tu = tu2;
	m_CloudLayerMesh[2].tu = m_CloudLayerMesh[3].tu = tu2 - 1.0f;
	m_CloudLayerMesh[0].color = m_CloudLayerMesh[1].color = m_CloudLayerMesh[2].color = m_CloudLayerMesh[3].color = m_dwFogColor;


	if( pCMyApp->GetCurWorld() == WORLD_GUILD_WAR )
	{
		m_pd3dDevice->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY05 ] ) );
		m_dwFogColor =RGBA_MAKE ( 15, 5, 0, 255 );
	}
	else if(pCMyApp->GetCurWorld() == WORLD_BOSSRAID_BULKAN && ( m_nCurHour == 2 || m_nCurHour == 3 ) )
	{
		m_pd3dDevice->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY05 ] ) );
		m_dwFogColor =RGBA_MAKE ( 15, 5, 0, 255 );
	}
	else if(m_nCurHour >= 7 && m_nCurHour <= 21)
	{
		m_pd3dDevice->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY01 ] ) );
		m_dwFogColor =RGBA_MAKE ( 45, 85, 116, 255 );
	}
	else if(m_nCurHour >=22 && m_nCurHour <= 27)
	{
		m_pd3dDevice->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY04 ] ) );
		m_dwFogColor =RGBA_MAKE ( 20, 11, 6, 255 );
	}
	else if(m_nCurHour >= 2 && m_nCurHour <= 6)
	{
		m_pd3dDevice->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY06 ] ) );
		m_dwFogColor =RGBA_MAKE ( 50, 40, 40, 255 );

	}
	else
	{
		m_pd3dDevice->SetTexture( 0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY05 ] ) );
		m_dwFogColor =RGBA_MAKE ( 15, 5, 0, 255 );
	}

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_CloudMesh, 4, 0 );
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	m_pd3dDevice->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY03 ] ));
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_CloudLayerMesh, 4, 0);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

	static float start_fog[30] =
	{
		0.02f, 0.03f, 0.04f, 0.05f, 0.06f,
		0.07f, 0.08f, 0.09f, 0.10f, 0.11f,
		0.12f, 0.13f, 0.14f, 0.09f, 0.10f,
		0.10f, 0.09f, 0.14f, 0.13f, 0.12f,
		0.11f, 0.10f, 0.09f, 0.08f, 0.07f,
		0.06f, 0.05f, 0.04f, 0.03f, 0.02f
	};

	static float end_fog[30] =
	{
		0.18f, 0.185f, 0.19f, 0.195f, 0.20f,
		0.21f, 0.25f, 0.27f, 0.29f, 0.31f,
		0.33f, 0.35f, 0.42f, 0.45f, 0.45f,
		0.45f, 0.45f, 0.42f, 0.35f, 0.33f,
		0.31f, 0.29f, 0.27f, 0.25f, 0.21f,
		0.20f, 0.195f, 0.19f, 0.185f, 0.18f
	};

	float FogEnd = end_fog[m_nCurHour], FogStart = start_fog[m_nCurHour];// for linear mode
	float FogDensity = 0.0f;				// for exponential mod

	if( m_pLightCtrl )
		m_pLightCtrl->LightingEnalbe(FALSE);

	// ????????(??????????)?? ???????? ?????? ??????.
	// ???? ?????????? ???? ??????.
	//m_bCanDoTableFog
	//m_bCanDoWFog
	if (!m_bCanDoVertexFog)
		return;

	if (m_bCanDoVertexFog && g_bDungeon != TRUE)
	{
		SetFogParameters(D3DFOG_LINEAR, m_dwFogColor, FALSE, FALSE, FogStart, FogEnd, FogDensity);

		// ???? ???????? ???????? ?? ????.
		m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
		m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, m_dwFogColor);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
	}
	else
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
}



// ???? ?????? ????????
void CMyD3DApplication::RenderSky2()
{
	if( !GetFramework()->GetD3DDevice() )
		return;

	if(m_nCurHour < 0 || m_nCurHour > 29)
		return;

	LPDIRECTDRAWSURFACE7 m_lpSkyTexture1 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture2 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture3 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture4 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture5 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture6 = NULL;

	DWORD  dwTextureState[10];
	DWORD  dwRenState[5];
//============================================================
	// nate 2004 - 7 : Image Manager
	m_lpSkyTexture1 = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY01 ] );
	m_lpSkyTexture2 = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY02 ] );
	m_lpSkyTexture3 = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY03 ] );
	m_lpSkyTexture4 = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY04 ] );
	m_lpSkyTexture5 = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY05 ] );
	m_lpSkyTexture6 = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SKY06 ] );
//============================================================
	D3DVIEWPORT7 vp;
	m_pd3dDevice->GetViewport( &vp );
	TLVec[0].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec[2].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec[2].sx = (FLOAT)vp.dwWidth;
	TLVec[3].sx = (FLOAT)vp.dwWidth;

	TLVec2[0].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec2[2].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec2[2].sx = (FLOAT)vp.dwWidth;
	TLVec2[3].sx = (FLOAT)vp.dwWidth;

	static float fAccTimeKey;
	fAccTimeKey += g_fTimeKey;
	FLOAT tu = (fAccTimeKey/100)-(FLOAT)floor(fAccTimeKey/100);
	tu = (tu+m_fRotate_Cloud_Angle);
	if (tu > 1.0f)
		tu -= 1.0f;
	TLVec[0].tu = TLVec[1].tu = tu;
	TLVec[2].tu = TLVec[3].tu = tu - 1.0f;

	static float fAccTimeKey2;
	fAccTimeKey2 += g_fTimeKey;
	FLOAT tu2 = (fAccTimeKey2/40)-(FLOAT)floor(fAccTimeKey2/40);
	tu2 = (tu2+m_fRotate_Cloud_Angle);
	if (tu2 > 1.0f)
		tu2 -= 1.0f;
	TLVec2[0].tu = TLVec2[1].tu = tu2;
	TLVec2[2].tu = TLVec2[3].tu = tu2 - 1.0f;

	if (m_bCanDoMultitexture && m_bCanDoAdd)
	{
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwTextureState[0] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_COLOROP,  &dwTextureState[2] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[3] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAOP,  &dwTextureState[4] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_COLORARG1, &dwTextureState[5] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_COLOROP,  &dwTextureState[6] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_COLORARG2, &dwTextureState[7] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_ALPHAOP,  &dwTextureState[8] );
		// ????????
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,  D3DTOP_SELECTARG1 );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1 );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,  D3DTOP_ADD );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );

		if(m_nCurHour >= 7 && m_nCurHour <= 21)
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture1);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture2);
			m_dwFogColor =RGBA_MAKE ( 45, 85, 116, 255 );
		}
		else if(m_nCurHour >= 22 && m_nCurHour <= 27)
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture4);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture3);
			m_dwFogColor =RGBA_MAKE ( 35, 5, 5, 255 );
		}
		else if(m_nCurHour >= 2 && m_nCurHour <= 6)
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture5);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture6);			;
			m_dwFogColor =RGBA_MAKE ( 82, 69, 66, 255 );
		}
		else
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture5);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture3);
			m_dwFogColor =RGBA_MAKE ( 15, 5, 0, 255 );
		}
		m_CloudLayerMesh[0].color = m_CloudLayerMesh[1].color = m_CloudLayerMesh[2].color = m_CloudLayerMesh[3].color = m_dwFogColor;

		GetFramework()->GetD3DDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		// ???? ??????
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, dwTextureState[0] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,  dwTextureState[2] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[3] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,  dwTextureState[4] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG1, dwTextureState[5] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,  dwTextureState[6] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG2, dwTextureState[7] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,  dwTextureState[8] );
	}

	GetFramework()->GetD3DDevice()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , &dwRenState[0]);
	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );
	GetFramework()->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture3);
	GetFramework()->GetD3DDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec2, 4, 0);
	GetFramework()->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , dwRenState[0]);
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	static float start_fog[30] =
	{
		0.02f, 0.03f, 0.04f, 0.05f, 0.06f,
		0.07f, 0.08f, 0.09f, 0.10f, 0.11f,
		0.12f, 0.13f, 0.14f, 0.09f, 0.10f,
		0.10f, 0.09f, 0.14f, 0.13f, 0.12f,
		0.11f, 0.10f, 0.09f, 0.08f, 0.07f,
		0.06f, 0.05f, 0.04f, 0.03f, 0.02f
	};

	static float end_fog[30] =
	{
		0.13f, 0.28f, 0.28f, 0.28f, 0.28f,
		0.28f, 0.28f, 0.33f, 0.33f, 0.33f,
		0.33f, 0.33f, 0.33f, 0.33f, 0.33f,
		0.33f, 0.33f, 0.33f, 0.33f, 0.33f,
		0.33f, 0.33f, 0.22f, 0.18f, 0.16f,
		0.14f, 0.13f, 0.13f, 0.13f, 0.13f
	};

	float FogEnd = end_fog[m_nCurHour], FogStart = 0.10f;//start_fog[m_nCurHour];// for linear mode
	float FogDensity = 0.0f;				// for exponential mod

	if( m_pLightCtrl )
		m_pLightCtrl->LightingEnalbe(FALSE);

	// ????????(??????????)?? ???????? ?????? ??????.
	// ???? ?????????? ???? ??????.
	//m_bCanDoTableFog
	//m_bCanDoWFog
	if (!m_bCanDoVertexFog)
		return;

	if( m_pd3dDevice )
		return;

	if (m_bCanDoVertexFog && g_bDungeon != TRUE)
	{
		SetFogParameters(D3DFOG_LINEAR, m_dwFogColor, FALSE, FALSE, FogStart, FogEnd, FogDensity);

		// ???? ???????? ???????? ?? ????.
		m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
		m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, m_dwFogColor);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
	}
	else
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
}



// ???? ?????? ????????
void CMyD3DApplication::RenderSky3()
{
	if( !GetFramework()->GetD3DDevice() )
		return;

	if(m_nCurHour < 0 || m_nCurHour > 29)
		return;

	LPDIRECTDRAWSURFACE7 m_lpSkyTexture1 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture2 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture3 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture4 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture5 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyTexture6 = NULL;
	LPDIRECTDRAWSURFACE7 m_lpSkyFog = NULL;

	DWORD  dwTextureState[10];
	DWORD  dwRenState[5];
//=====================================================================
	// nate 2004 - 7
	// Image Manager
	m_lpSkyTexture1 = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_SKY01 ]);
	m_lpSkyTexture2 = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_SKY02 ]);
	m_lpSkyTexture3 = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_SKY03 ]);
	m_lpSkyTexture4 = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_SKY04 ]);
	m_lpSkyTexture5 = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_SKY05 ]);
	m_lpSkyTexture6 = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_SKY06 ]);
//=====================================================================
	m_lpSkyFog = g_pCapsyongTexture->GetSurfaceByIdx(m_wSky_Fog);

	D3DVIEWPORT7 vp;
	if( m_pd3dDevice )
		m_pd3dDevice->GetViewport( &vp );
	TLVec[0].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec[2].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec[2].sx = (FLOAT)vp.dwWidth;
	TLVec[3].sx = (FLOAT)vp.dwWidth;

	TLVec2[0].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec2[2].sy = (FLOAT)vp.dwHeight/4 * 3;
	TLVec2[2].sx = (FLOAT)vp.dwWidth;
	TLVec2[3].sx = (FLOAT)vp.dwWidth;

	static float fAccTimeKey;
	fAccTimeKey += g_fTimeKey;
	FLOAT tu = (fAccTimeKey/100)-(FLOAT)floor(fAccTimeKey/100);
	tu = (tu+m_fRotate_Cloud_Angle);
	if (tu > 1.0f)
		tu -= 1.0f;
	TLVec[0].tu = TLVec[1].tu = tu;
	TLVec[2].tu = TLVec[3].tu = tu - 1.0f;

	static float fAccTimeKey2;
	fAccTimeKey2 += g_fTimeKey;
	FLOAT tu2 = (fAccTimeKey2/40)-(FLOAT)floor(fAccTimeKey2/40);
	tu2 = (tu2+m_fRotate_Cloud_Angle);
	if (tu2 > 1.0f)
		tu2 -= 1.0f;
	TLVec2[0].tu = TLVec2[1].tu = tu2;
	TLVec2[2].tu = TLVec2[3].tu = tu2 - 1.0f;

	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &dwRenState[0] );
	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR_New(g_pRoh->m_wx, pCMyApp->GetLandPtr()->GetHFHeight(g_pRoh->m_wx, g_pRoh->m_wz), g_pRoh->m_wz);
	float sphere_height = 500.0f;
	float sphere_scale = 4000.0f;

	if (m_bCanDoMultitexture && m_bCanDoAdd && sky)
	{
		// ????????

		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwTextureState[0] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_COLOROP,  &dwTextureState[2] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[3] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAOP,  &dwTextureState[4] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_COLORARG1, &dwTextureState[5] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_COLOROP,  &dwTextureState[6] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_COLORARG2, &dwTextureState[7] );
		GetFramework()->GetD3DDevice()->GetTextureStageState( 1, D3DTSS_ALPHAOP,  &dwTextureState[8] );
		// ????????
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,  D3DTOP_SELECTARG1 );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1 );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,  D3DTOP_ADD );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );

		if(m_nCurHour >= 7 && m_nCurHour <= 21)
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture1);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture2);
			m_dwFogColor =RGBA_MAKE ( 45, 85, 116, 255 );
		}
		else if(m_nCurHour >= 22 && m_nCurHour <= 27)
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture4);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture3);
			m_dwFogColor =RGBA_MAKE ( 35, 5, 5, 255 );
		}
		else if(m_nCurHour >= 2 && m_nCurHour <= 6)
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture5);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture6);			;
			m_dwFogColor =RGBA_MAKE ( 82, 69, 66, 255 );
		}
		else
		{
			GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture5);
			GetFramework()->GetD3DDevice()->SetTexture(1, m_lpSkyTexture3);
			m_dwFogColor =RGBA_MAKE ( 15, 5, 0, 255 );
		}
		m_CloudLayerMesh[0].color = m_CloudLayerMesh[1].color = m_CloudLayerMesh[2].color = m_CloudLayerMesh[3].color = m_dwFogColor;

		sky->SetColor( m_dwFogColor );
		sky->SetRotationY(g_PI);
		sky->RotateZ(g_PI*0.5f);
		sky->Scale(sphere_scale * 0.3f, sphere_scale, sphere_scale);
		sky->SetTranslationXYZ(vDrawPos.x, vDrawPos.y - sphere_height, vDrawPos.z);
		sky->trans_texture(g_fTimeKey * 0.015f);
		sky->SkyRender();

		// ???? ??????
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, dwTextureState[0] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,  dwTextureState[2] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[3] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,  dwTextureState[4] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG1, dwTextureState[5] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLOROP,  dwTextureState[6] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_COLORARG2, dwTextureState[7] );
		GetFramework()->GetD3DDevice()->SetTextureStageState( 1, D3DTSS_ALPHAOP,  dwTextureState[8] );
	}
// ???????? ???? seo

	GetFramework()->GetD3DDevice()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , &dwRenState[0]);
	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	GetFramework()->GetD3DDevice()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );
	GetFramework()->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture3);

	if( sky_cloud )
	{
		sky_cloud->SetColor( m_dwFogColor );
		sky_cloud->SetRotationY(g_PI);
		sky_cloud->RotateZ(g_PI*0.5f);
		sky_cloud->Scale(sphere_scale * 0.3f, sphere_scale, sphere_scale);
		sky_cloud->SetTranslationXYZ(vDrawPos.x, vDrawPos.y - sphere_height, vDrawPos.z);
		sky_cloud->trans_texture(g_fTimeKey * 0.04f);
		sky_cloud->SkyRender();
	}

	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTALPHA );

	DWORD  dwTextrState[3];
	GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextrState[0] );
	GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextrState[1] );
	GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextrState[2] );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyFog);

	if( sky_fog )
	{
		sky_fog->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		sky_fog->SetRotationY(g_PI);
		sky_fog->SetTranslationXYZ(vDrawPos.x, vDrawPos.y - sphere_height*1.5f, vDrawPos.z);
		sky_fog->Render();
	}

	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextrState[0] );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextrState[1] );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextrState[2] );
	GetFramework()->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , dwRenState[0]);
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	static float start_fog[30] =
	{
		0.02f, 0.03f, 0.04f, 0.05f, 0.06f,
		0.07f, 0.08f, 0.09f, 0.10f, 0.11f,
		0.12f, 0.13f, 0.14f, 0.09f, 0.10f,
		0.10f, 0.09f, 0.14f, 0.13f, 0.12f,
		0.11f, 0.10f, 0.09f, 0.08f, 0.07f,
		0.06f, 0.05f, 0.04f, 0.03f, 0.02f
	};

	static float end_fog[30] =
	{
		0.13f, 0.28f, 0.28f, 0.28f, 0.28f,
		0.28f, 0.28f, 0.33f, 0.33f, 0.33f,
		0.33f, 0.33f, 0.33f, 0.33f, 0.33f,
		0.33f, 0.33f, 0.33f, 0.33f, 0.33f,
		0.33f, 0.33f, 0.22f, 0.18f, 0.16f,
		0.14f, 0.13f, 0.13f, 0.13f, 0.13f
	};

	float FogEnd = end_fog[m_nCurHour], FogStart = 0.10f;//start_fog[m_nCurHour];// for linear mode
	float FogDensity = 0.0f;				// for exponential mod

	if( m_pLightCtrl )
		m_pLightCtrl->LightingEnalbe(FALSE);


	if (!m_bCanDoVertexFog)
		return;

	if( m_pd3dDevice )
	{
		if (m_bCanDoVertexFog && g_bDungeon != TRUE)
		{
			SetFogParameters(D3DFOG_LINEAR, m_dwFogColor, FALSE, FALSE, FogStart, FogEnd, FogDensity);

			// ???? ???????? ???????? ?? ????.
			m_bUsingTableFog = FALSE;		// pixel fog?? table ????????.
			m_bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, m_dwFogColor);

			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&m_fFogStart)));
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&m_fFogEnd)));
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&FogDensity)));

			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  m_dwFogMode );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, m_bRangeBasedFog );
		}
		else
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
	}

	GetFramework()->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTexState[3];
	GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTexState[0] );
	GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTexState[1] );
	GetFramework()->GetD3DDevice()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTexState[2] );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	GetFramework()->GetD3DDevice()->SetTexture(0, m_lpSkyTexture1);

	if( ground )
	{
		ground->SetColor( m_dwFogColor );
		ground->SetRotationX(g_PI * 0.5f);
		ground->Scale(sphere_scale, sphere_scale, 0.0f);
		ground->SetTranslationXYZ(vDrawPos.x, vDrawPos.y - sphere_height + 50.0f, vDrawPos.z);
	}

	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTexState[0] );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTexState[1] );
	GetFramework()->GetD3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTexState[2] );
	GetFramework()->GetD3DDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , dwRenState[0]);
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	GetFramework()->GetD3DDevice()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );
}


//--------------------------------------------------
// ?????? ?????? ???? ?????????? ??????
//--------------------------------------------------
void CMyD3DApplication::RotateCloud()
{
	float dir;
	float angle_180; //	???????? ?????? ????..???????? ???? (0 ~ 180 / -179 ~ -1)
	float angle_360; // angle_180?? ???????? 360?? ???? ?????? ??????.
	float angle_90;  // angle_360?? ???????? ???? ?????? ????(?????? ???? ???????????? ?????? ???? = 90)?? ????????

	// ?????? ???????? ??????..
	dir= (float) atan2(m_vEyeZ.z, m_vEyeZ.x);
	angle_180= (180/((3.141592)))*(dir);

	// ?????? ???? 360?? ???? ????????
	if(angle_180 <0)
		angle_360 = 180+(180+angle_180);
	else
		angle_360 = angle_180;

	// ???? ?????? ?????????? ?????? ???? ?????? 0 ~ 90 ??????..
	// 0 ~ 90?? ?? ???? ?????? ????..
	angle_90 = fmod(angle_360, 90);
	// ???????????? ???? ?????? ???????? ?????? ???? ?? ????..
	// m_vt_CurCloud[0].tu = m_vt_CurCloud[1].tu = 0.0 ~ 1.0
	// m_vt_CurCloud[2].tu = m_vt_CurCloud[3].tu = 0.0 ~ 1.0 - (1.0)
	// ???????? m_tmpX ?? ???????? ???????? 0.0 ~ 1.0 ?? ?????? ???? ???????? ????..
	m_fRotate_Cloud_Angle = (angle_90/90);
	// ?????????? ???????? ??????..????????m_tmpX?? ???? tu?? ??????
}


void CMyD3DApplication::SendDummyMsg()
{
	if (g_pTcpIp)
	{
		g_pTcpIp->SendNetMessage("\n");
	}
}

BOOL CMyD3DApplication::CanUseLEquip()
{
	if (g_dwBaseTime >= m_lEquipTime + 1000)
	{
		m_lEquipTime = g_dwBaseTime;
		return TRUE;
	}

	return FALSE;
}

BOOL CMyD3DApplication::CanClick()//?????? ???? ????.(?????????? ????????)
{
	if (g_dwBaseTime >= m_lClickTime + 200)  //0.3???? ????.->0.2
	{
		m_lClickTime = g_dwBaseTime;
		return TRUE;
	}

	return FALSE;
}

void CMyD3DApplication::SetFog(BOOL bFog)
{
	return;

	if (m_pd3dDevice && (GetCurWorld() == WORLD_SKY))
	{
		if (bFog)
		{
			BOOL bUsingTableFog = FALSE;		// pixel fog?? table ????????.
			BOOL bRangeBasedFog = FALSE;		// ???????? ?????????? ????. ???? vertexfog???? cpu?? ??????.

			float fFogStart = (m_fog_start * (m_zFar-m_zNear) ) + m_zNear;
			float fFogEnd = (m_fog_end * (m_zFar-m_zNear) ) + m_zNear;
			float fFogDensity = 0.0f;
			DWORD dwFogColor = RGBA_MAKE ( 48, 64, 110, 255 );

			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);

			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD *)(&fFogStart)));
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD *)(&fFogEnd)));
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD *)(&fFogDensity)));

			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGTABLEMODE,   D3DFOG_NONE );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE,  D3DFOG_LINEAR );
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_RANGEFOGENABLE, bRangeBasedFog );

		}
		else
		{
			m_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
		}
	}
}




void CMyD3DApplication::DrawDrvTestPickVex()
{
	D3DVERTEX			PickVex[6];
	D3DVIEWPORT7		vp;

	DWORD				dwAmbient;
	WORD				g_wIdx;

	HICOLOR hicolor;
	unsigned char r24, g24, b24;
	int Width, Height;
	Width = 100;
	Height = 100;

	PickVex[0] = D3DVERTEX( D3DVECTOR_New( -Width, -Height, 0.0f ), D3DVECTOR_New(0, 0, -1), 0.0f, 1.0f );
	PickVex[1] = D3DVERTEX( D3DVECTOR_New( -Width, Height, 0.0f ), D3DVECTOR_New(0, 0, -1), 0.0f, 0.0f );
	PickVex[2] = D3DVERTEX( D3DVECTOR_New( Width, -Height, 0.0f ), D3DVECTOR_New(0, 0, -1), 1.0f, 1.0f );
	PickVex[3] = D3DVERTEX( D3DVECTOR_New( Width, Height, 0.0f ), D3DVECTOR_New(0, 0, -1), 1.0f, 0.0f );
	PickVex[4] = D3DVERTEX( D3DVECTOR_New( -Width, -Height, 0.0f ), D3DVECTOR_New(0, 0, 1), 0.0f, 1.0f );
	PickVex[5] = D3DVERTEX( D3DVECTOR_New( -Width, Height, 0.0f ), D3DVECTOR_New(0, 0, 1), 0.0f, 0.0f );

	hicolor.Color = g_wIdx = 40307;

	r24 = hicolor.Bit565.R << 3;
	g24 = hicolor.Bit565.G << 2;
	b24 = hicolor.Bit565.B << 3;

	// ???????? ?????? ???????? ???? ( ?????? ?????? ?????????? ????  )
	dwAmbient = ( (r24 << 16) | (g24 << 8) | (b24 << 0) );

	D3DMATRIX matWorld, matView, matProj;
	D3DUtil_SetIdentityMatrix(matWorld);

	matWorld._41 = 315;
	matWorld._42 = 120;
	matWorld._43 = 50;

	if( !m_pd3dDevicePick || !m_pd3dDevice )
		return;

	m_pd3dDevicePick->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
	m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);

	m_pd3dDevice->GetViewport( &vp );
	vp.dwWidth = vp.dwWidth / 2;
	vp.dwHeight = vp.dwHeight / 2;

	m_pd3dDevicePick->SetViewport( &vp );

	D3DMATERIAL7 mtrl;
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	m_pd3dDevicePick->SetMaterial(&mtrl);
	m_pd3dDevice->GetTransform( D3DTRANSFORMSTATE_VIEW, &matView );
	m_pd3dDevicePick->SetTransform( D3DTRANSFORMSTATE_VIEW, &matView );
	m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProj);
	m_pd3dDevicePick->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj);

	D3DLIGHT7 m_d3dLight;
	m_pd3dDevice->GetLight(0, &m_d3dLight);
	m_pd3dDevicePick->SetLight(0, &m_d3dLight);
	m_pd3dDevicePick->LightEnable(0, FALSE);
	m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT);
	m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE);
	m_pd3dDevicePick->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DRGB(0, 0, 0), 1.0f, 0L );

	if( SUCCEEDED( m_pd3dDevicePick->BeginScene() ) )
	{
		m_pd3dDevicePick->SetTexture(0, NULL);
		m_pd3dDevicePick->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX,
										 PickVex, 6, NULL );
		m_pd3dDevicePick->EndScene();
	}

	m_pd3dDevicePick->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	return;
}


void CMyD3DApplication::SetDrvTypeClrAlloc()
{
	LPDIRECTDRAWSURFACE7 lpddsBufferPick = pCMyApp->GetFramework()->GetBackBufferPick();
	DDSURFACEDESC2	ddsd;
	POINT			pt;
	WORD			inx;		// 16bit ???? ??.
	DWORD			dwIdx;

	int  nResolution = pCMyApp->GetCurResolution();

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	lpddsBufferPick->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

	if( ddsd.lpSurface == NULL )
	{
		lpddsBufferPick->Unlock(NULL);
		return;
	}

	pt.x = (ddsd.dwWidth / 2);
	pt.y = (ddsd.dwHeight / 2);

	if (nResolution == 16)
	{
		inx = *((WORD *)((BYTE *)ddsd.lpSurface + (pt.y * ddsd.lPitch + (pt.x<<1))));
		lpddsBufferPick->Unlock(NULL);

		if( inx == 38226 )
			m_DrvType = INTEL_DRV;
		else
			m_DrvType = NORMAL_DRV;

	}
	else if (nResolution == 32)
	{
		dwIdx = *((DWORD *)((BYTE *)ddsd.lpSurface + (pt.y * ddsd.lPitch + (pt.x<<2))));
		lpddsBufferPick->Unlock(NULL);

		HICOLOR hicolor;
		TRUECOLOR truecolor;
		BYTE R, G, B;

		truecolor.dwColor = dwIdx;
		hicolor.Color = 0;
		B = truecolor.BYTE8888.B;
		G = truecolor.BYTE8888.G;
		R = truecolor.BYTE8888.R;

		hicolor.Bit565.B = B >> 3;
		hicolor.Bit565.G = G >> 2;
		hicolor.Bit565.R = R >> 3;

		inx = hicolor.Color;

		if( inx == 38226 )
			m_DrvType = XABRE_DRV;
		else
			m_DrvType = NORMAL_DRV;

	}

	return;
}

void CMyD3DApplication::RestoreVBs()
{
	if( !g_bNewTerrain || !m_pLand )
		return ;

	// Invalidate vbs
	m_pLand->InvalidateVB();
	CNKObjQuadNode::InvalidateDeviceObjects();

	// Restore vb of the terrain
	m_pLand->SetDevice( GetFramework()->GetD3DDevice() );
	m_pLand->SetD3D( GetFramework()->GetDirect3D() );
	m_pLand->InitVertexBuffer();

	// Restore vb of water objects
	CNKObjQuadNode::RestoreDeviceObjects();
}

void CMyD3DApplication::PcBangUserDraw()
{
	if( !m_pd3dDevice )
		return;

	D3DTLVERTEX				m_rect[4];

	DWORD dwlingting, dwAlphaTest, dwAlphaBlend, dwRenState[2];
	DWORD dwTextureState[3];

	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwlingting );
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	m_pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &dwAlphaTest);

	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwRenState[0] );
	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwRenState[1] );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	m_rect[0] = D3DTLVERTEX( D3DVECTOR_New( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 1.0f );
	m_rect[1] = D3DTLVERTEX( D3DVECTOR_New( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 0.0f, 0.0f );
	m_rect[2] = D3DTLVERTEX( D3DVECTOR_New( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 1.0f );
	m_rect[3] = D3DTLVERTEX( D3DVECTOR_New( 0.0f, 0.0f, 0.0f ), 1.0f, 0xffffffff, 0, 1.0f, 0.0f );

	int sizeX = 640;
	int sizeY = 480;
	if( m_ProOption.resolution == R800 )
	{
		sizeX = 800;
		sizeY = 600;		
	}
	else if( m_ProOption.resolution == R1024 )
	{
		sizeX = 1024;
		sizeY = 768;		
	}
	else if( m_ProOption.resolution == R1280 )
	{
		sizeX = 1280;
		sizeY = 768;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		sizeX = 1280;
		sizeY = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		sizeX = 1600;
		sizeY = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		sizeX = 1920;
		sizeY = 1080;
	}

	m_rect[0].sx = sizeX - 110;
	m_rect[1].sx = sizeX - 110;
	m_rect[2].sx = sizeX - 110 + 32;
	m_rect[3].sx = sizeX - 110 + 32;

	m_rect[0].sy = sizeY - 100 + 32;
	m_rect[1].sy = sizeY - 100;
	m_rect[2].sy = sizeY - 100 + 32;
	m_rect[3].sy = sizeY - 100;


	switch( m_IsPcBang_User )
	{
	case 1:
		m_pd3dDevice->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_PCBANG_USER]));
		break;
	case 2:
		m_pd3dDevice->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_PREMIUM_USER]));
		break;
	}

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rect, 4, 0 );


	if( m_PcBangStartTime == 0 || g_dwClientCountry != CTRY_KOR )
	{
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
		m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

		return;
	}

	switch( m_ProOption.resolution )
	{	
	case R800:
		{
			sizeX = 800;
			sizeY = 600;
		}
		break;
	case R1024:
		{
			sizeX = 1024;
			sizeY = 768;
		}
		break;
	case R1280:
		{
			sizeX = 1280;
			sizeY = 960;
		}
		break;
	case R1280W:
		{
			sizeX = 1280;
			sizeY = 720;
		}
		break;
	case R1600W:
		{
			sizeX = 1600;
			sizeY = 900;
		}
		break;
	case R1920W:
		{
			sizeX = 1920;
			sizeY = 1080;
		}
		break;
	default:
		{
			sizeX = 640;
			sizeY = 480;
		}
		break;
	}

	m_rect[0].sx = sizeX - 109;
	m_rect[1].sx = sizeX - 109;
	m_rect[2].sx = sizeX - 109 + 30;
	m_rect[3].sx = sizeX - 109 + 30;

	m_rect[0].sy = sizeY - 99;
	m_rect[2].sy = sizeY - 99 + 30;

	// ???????? ???????? ???? ???? ??????

	DWORD checktime = 30; // ??

	DWORD curTime = timeGetTime() - m_PcBangStartTime;
	float gage = 0;

	if( curTime >= (1000*60*checktime) )
		gage = 30.0f;
	else
		gage = 30.0f * ( (float)curTime / (float)(1000*60*checktime));

	m_rect[1].sy = m_rect[3].sy = m_rect[0].sy - gage;

	m_pd3dDevice->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_PCBANG_USER_GAGE]));

	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, m_rect, 4, 0 );

	// [12/23/2008 D.K ] : auto system.
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwRenState[0] );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwRenState[1] );

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, dwlingting );
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, dwAlphaTest);

	return;
}

BOOL CMyD3DApplication::IsPrisonArea( float fX, float fY )
{
	if( IsInsideRect( fX, fY, 5990, 5150, 6360, 4790 ) )
		return TRUE;

	return FALSE;
}

//===============================
// nate 2004 - 9
// GuildHunt
BOOL CMyD3DApplication::IsGuildHuntArea( float fX, float fZ )
{
	if( fX == 0.0f && fZ == 0.0f )
	{
		fX = g_pRoh->m_wx;
		fZ = g_pRoh->m_wz;
	}

	if( m_nCurWorldIdx == WORLD_GUILD
			&& IsInsideRect( fX, fZ, 3470, 3190, 4950, 1910 ) )
		return TRUE;
	else if( m_nCurWorldIdx == WORLD_WHITEHORN
			 && IsInsideRect( fX, fZ, 7570, 1830, 11420, 520 ) )
		return TRUE;

	return FALSE;
}

BOOL CMyD3DApplication::IsFianlBossRoomArea( float fX, float fZ )
{
	if( fX == 0.0f && fZ == 0.0f )
	{
		fX = g_pRoh->m_wx;
		fZ = g_pRoh->m_wz;
	}

	if( m_nCurWorldIdx == WORLD_BOSSRAID_FINALBOSS 	&& IsInsideRect( fX, fZ, 9600, 6540, 11400, 4760 ) )
		return TRUE;

	return FALSE;
}

BOOL	_IsInsideRect( float fX, float fZ, float left, float top, float right, float bottom )
{
	if( fX >= left && fX <= right && fZ >= top && fZ <= bottom )
		return TRUE;
	return FALSE;
}

BOOL	_IsInsideRect( float fX, float fZ, RECT rc )
{
	return _IsInsideRect( fX, fZ, rc.left, rc.top, rc.right, rc.bottom );
}

BOOL CMyD3DApplication::IsBossRoomArea( float fX, float fZ )
{
	if( fX == 0.0f && fZ == 0.0f )
	{
		fX = g_pRoh->m_wx;
		fZ = g_pRoh->m_wz;
	}
	// ??????
	if( m_nCurWorldIdx == WORLD_BOSSRAID_BULKAN && _IsInsideRect( fX, fZ, 8135, 8865, 12225, 12065 ) )
		return TRUE;

	// ????????.
	if( m_nCurWorldIdx == WORLD_BOSSRAID_KAILIPTON && _IsInsideRect( fX, fZ, 8765, 4535, 12255, 1105 ) )
		return TRUE;
	// ??????.
	if( m_nCurWorldIdx == WORLD_BOSSRAID_AIDIA &&
			( _IsInsideRect( fX, fZ, 1195, 4515, 4055, 7575 ) || // ????????

			  _IsInsideRect( fX, fZ, 9025, 8955, 12515, 12385 ) || // ????
			  _IsInsideRect( fX, fZ, 8455, 1155, 12335, 4335 ) ) )	// ????????.
		return TRUE;
	// ??????

	if( m_nCurWorldIdx == WORLD_BOSSRAID_HUMAN && _IsInsideRect( fX, fZ, 2005, 4875, 5705, 8275 ) )
		return TRUE;
	// ??????????.
	if( m_nCurWorldIdx == WORLD_BOSSRAID_HYBRIDER && _IsInsideRect( fX, fZ, 8005, 7235, 12385, 12195 ) )
		return TRUE;

	return FALSE;
}
//===============================
//===============================
// nate 2004 - 11
// Happy New Year Event
BOOL CMyD3DApplication::CreateHappyNewYear()
{
	m_NewYear = new CHappyNewYear();
	if( m_NewYear )
	{
		DrawHappyNewYear = TRUE;
		m_NewYear->LoadRes();
	}
	else
		return DrawHappyNewYear = FALSE;

	return DrawHappyNewYear;
}

void CMyD3DApplication::DeleteHappyNewYear()
{
	DrawHappyNewYear = FALSE;
	delete m_NewYear;
}
//===============================

BOOL CMyD3DApplication::CreateBattleStart(int rendnum)
{
	m_BattleStart = new CBattleStart();

	if( m_BattleStart )
	{
		DrawBattleStart = TRUE;
		m_BattleStart->LoadRes();
		m_nBSRendNum = rendnum;
		if (g_pDSound)
		{
			if(m_nBSRendNum==1)
			{
				//win
				g_pDSound->Play(g_EffectSound[EFF_SND_CONGR]);
			}
		}
	}
	else
		return DrawBattleStart = FALSE;

	return DrawBattleStart;
}
//========================================
void CMyD3DApplication::DeleteBattleStart()
{
	if(m_BattleStart)
	{
		m_BattleStart->DeleteRes();
		delete m_BattleStart;
		m_BattleStart = NULL;
		DrawBattleStart = FALSE;
	}
}
//========================================
//wan_mat 2005-03
//???????? ???? ???? ????

int CMyD3DApplication::PlaceInfo(float x, float z)
{
	switch( m_nCurWorldIdx )
	{
	case WORLD_MATRIX:
		if (x > 5500 && x < 7000 && z > 1700 && z < 3600)//???? ????

			return 1;
		else if (x > 8600 && x < 10100 && z > 3800 && z < 5200)//??????1
			return 2;
		else if (x > 8500 && x < 10100 && z > 7300 && z < 9100)//?????? ????

			return 3;
		else if (x > 5800 && x < 6800 && z > 9200 && z < 10700)//??????3
			return 4;
		else if (x > 2500 && x < 4200 && z > 7300 && z < 9100)//????????

			return 5;
		else if (x > 2500 && x < 4000 && z > 3800 && z < 5200)//??????2
			return 6;
		else if (x > 9600 && z < 3600)//???????? ??????

			return 7;
		else if (x < 3700 && z < 2500)//??????
			return 8;
		else if (x > 5750 && x < 6800 && z > 5700 && z < 6900)//??????????
			return 9;
		break;
	case WORLD_MATRIX_2009:
		if (x >   481 && x <  2217 && z >   248 && z <  2783)	// ???????? ??????

			return 1;
		else if (x >  9091 && x < 11547 && z > 10502 && z < 11746)	// ??????
			return 2;
		else if (x >   481 && x <  5049 && z >  9253 && z < 11994)	// ????????
			return 3;
		else if (x >  5050 && x <  8354 && z >  8077 && z < 10176)	// ???????? ????
			return 4;
		else if (x >  8355 && x < 11074 && z >  7228 && z < 10176)	// ???????? ????
			return 5;
		else if (x >  2218 && x <  5049 && z >  5477 && z <  9252)	// ???????? ????
			return 6;
		else if (x >  5050 && x <  8354 && z >  5477 && z <  8077)	// ??????
			return 7;
		else if (x >  8355 && x < 11074 && z >  4262 && z <  7227)	// ???????? ????
			return 8;
		else if (x >  2218 && x <  5049 && z >  2433 && z <  5476)	// ???????? ????
			return 9;
		else if (x >  5050 && x <  8354 && z >  2443 && z <  4261)	// ???????? ????
			return 10;
		else if (x >  8355 && x < 11735 && z >   692 && z <  4261)	// ????????
			return 11;
		break;
	}

	return 0;

}
//========================================
//???????? ??????

void CMyD3DApplication::CreatePlaceInfo(int textNum)
{
	m_PlaceInfo = new CPlaceInfo();
	if( m_PlaceInfo )
	{
		m_PlaceInfo->LoadRes(textNum );
		DrawPlaceInfo = TRUE;
	}
	else
		DrawBattleStart = FALSE;
}
//========================================
//???????? ????
void CMyD3DApplication::DeletePlaceInfo()
{
	if(m_PlaceInfo)
	{
		m_PlaceInfo->DeleteRes();
		delete m_PlaceInfo;
		m_PlaceInfo = NULL;
	}
}

// nate 2005-07-28 : ???? ???? ??????
void CMyD3DApplication::Goworld_CharInit()
{
	// ?????? ??????
	g_pRoh->m_bBleed = false;
	// ?????? ???? ??????
	g_pRoh->m_bstone = false;
	g_pRoh->m_bresstone = false;
	g_pRoh->SetMotionState(CHA_STAND);
	g_pRoh->RemoveAll();
	g_pRoh->SetManaShield(FALSE);
	g_pRoh->SetElectricShield(FALSE);
	g_pRoh->SetHideMode(HIDE_SHOWN);
	g_pRoh->m_ChaCondition = CONDITION_NORMAL;//???? ??????.????,?????? ????.
	g_pRoh->m_ChaCondition2 = CONDITION_NORMAL;//???? ??????.????,?????? ????.
	g_pRoh->m_Accumulate = FALSE; //???? ????.
	g_pRoh->m_CancelState = 0;//???? ????.?????? 0????.
	g_pRoh->m_Chaotic_Grade = 0; //?????? ????.
	g_pRoh->m_Free_Pk = 0; //???? ?????? ????.
	g_pRoh->m_ShowConditionEffect = FALSE; //????????

	g_pRoh->m_ShowConditionEffect2 = FALSE; //????????2
	g_pRoh->m_ShowSkillEffect = FALSE; //????????????

	g_pRoh->m_ShowAidiaSkillEffect = FALSE; //???????? ????????????

	g_pRoh->m_InChaSkill = 0; //???? ??????.
	g_pRoh->m_InChaAidiaSkill = 0;	// ???? ???? ?????? ??????
	g_pRoh->m_ShowReflection = FALSE; //???????? ???? ????.(mungmae-05/04/20)
	g_pRoh->m_bProtectionShield = FALSE;
	g_pRoh->SetFlyLevel(0);
	g_pRoh->m_bNonPK = FALSE; //0310
	g_pRoh->m_byShowLIfeExpantion = 0;	// ???? ????????

	m_pUIMgr->m_byCheckHellDown = TRUE;	// ??????

	m_pUIMgr->m_bRClickDown = 0;		// ??????

	m_bHellDownReady = FALSE;
	DrawCasting = FALSE;
	g_pRoh->m_bCloaking = FALSE;
	g_pRoh->m_bCloaking_Waring = FALSE;
	g_pRoh->m_bShowAdrenaline = FALSE; //?????????? ???? ????(mungmae-/05/11/21)
}

// nate 2006-01-16 : ?????? ???? ????
int CMyD3DApplication::IsAcropolis(float fX, float fZ)
{
	float fPosX = fX, fPosZ = fZ;
	if( !fPosX ) fPosX = g_pRoh->m_wx;
	if( !fPosZ ) fPosZ = g_pRoh->m_wz;

	if(m_nCurWorldIdx == WORLD_DMITRON)
	{
		if(IsInsideRect(fPosX, fPosZ, 8800, 12100, 10450, 11000))	// ???? ??
			return 1;
		if(IsInsideRect(fPosX, fPosZ, 10450, 12000, 11650, 11000))	// ???? ??
			return 2;
	}
	else if( m_nCurWorldIdx == WORLD_START ) ///???????? ?? ????
	{
		if(IsInsideRect(fPosX, fPosZ, 500, 12200, 3200, 9300))	// ???????? ??.
			return 1;
	}

	return 0;
}

// nate 2006-05-15 : ???????? ?????? ??????

int CMyD3DApplication::IsSoccerZone(float fX, float fZ)
{
	float fPosX = fX, fPosZ = fZ;
	if( !fPosX ) fPosX = g_pRoh->m_wx;
	if( !fPosZ ) fPosZ = g_pRoh->m_wz;

	if(m_nCurWorldIdx == WORLD_MATRIX)
	{
//		if(IsInsideRect(fPosX, fPosZ, 7800, 4830, 8570, 3600))
		if(IsInsideRect(fPosX, fPosZ, 9550, 11840, 11900, 9850))
		{
			g_pNk2DFrame->m_bWorldCup = TRUE;
			return 1;
		}
		else
			g_pNk2DFrame->m_bWorldCup = FALSE;
	}
	return 0;
}


void CMyD3DApplication::SpiritRotateCamera(POINT ptOld, int mx, int my)
{
	static float fSaveTime = 0.0f;
	int nAddFrame = 0;

	fSaveTime += g_fTimeKey;

	while (fSaveTime > 0.05f)
	{
		nAddFrame++;
		fSaveTime -= 0.05f;
	}

	m_RotateFrame += nAddFrame;

	float fDegToRadX = (float)nAddFrame * 0.314f;	// ?????? ( (-)?? X???? ???????? )

	m_vEye.x -= (m_vEyeZ.x * m_RotateFrame);
	m_vEye.y -= (m_vEyeZ.y * m_RotateFrame);
	m_vEye.z -= (m_vEyeZ.z * m_RotateFrame);
	m_fCamHeight = m_vEye.y - m_vAt.y;

	if( m_RotateFrame >= 20  )
	{
		m_SpiritRotateCamera = FALSE;
		m_RotateFrame = 0;
		m_bSpiritZoomIn = TRUE;
	}

	Nk3dMath_VectorRotateYByVertex(m_vEye, m_vAt, fDegToRadX);

	m_vEyeZ = m_vAt - m_vEye;	// ?????????? ????

	m_fCamHeight = m_vEye.y - m_vAt.y;

	SetViewMatrix(m_matView);
	RotateCloud();
}

void CMyD3DApplication::SpiritZoomIn()
{
	static float fSaveTime2 = 0.0f;
	int nAddFrame = 0;

	fSaveTime2 += g_fTimeKey;

	while (fSaveTime2 > 0.05f)
	{
		nAddFrame++;
		fSaveTime2 -= 0.05f;
	}

	m_ZoomInFrame += nAddFrame;

	if( m_ZoomInFrame >= 10  )
	{
		m_bSpiritZoomIn = FALSE;
		m_ZoomInFrame = 0;
	}

	m_vEye.x += (m_vEyeZ.x * m_ZoomInFrame * 4);
	m_vEye.y += (m_vEyeZ.y * m_ZoomInFrame * 4);
	m_vEye.z += (m_vEyeZ.z * m_ZoomInFrame * 4);
	m_fCamHeight = m_vEye.y - m_vAt.y;

	float ty = m_pLand->GetHFHeight(m_vEye.x, m_vEye.z) + 40.0f;

	if (m_vEye.y < ty)
		m_vEye.y = m_vAt.y + m_fCamHeight;

	m_vEyeZ = m_vAt - m_vEye;
	SetViewMatrix(m_matView);
}

BOOL CMyD3DApplication::CheckGameMutex()
{
	m_hMutex = ::CreateMutex( NULL, TRUE, "Laghaim_Matrix");
	DWORD err = GetLastError();

	if( err == ERROR_ALREADY_EXISTS )
	{
		::ReleaseMutex( m_hMutex );
		::CloseHandle( m_hMutex );
		return FALSE;
	}
	return TRUE;
}

BOOL CMyD3DApplication::IsCurWorldID_BossraidZone()
{
	switch( GetCurWorldID() )
	{
	case WORLD_BOSSRAID_BULKAN:
	case WORLD_BOSSRAID_KAILIPTON:
	case WORLD_BOSSRAID_AIDIA:
	case WORLD_BOSSRAID_HUMAN:
	case WORLD_BOSSRAID_HYBRIDER:
	case WORLD_BOSSRAID_FINALBOSS:
		return TRUE;
	}

	return FALSE;
}

void CMyD3DApplication::SignalPostLoad()
{
	m_bPostLoading = TRUE;
}

BOOL CMyD3DApplication::NeedPostLoading()
{
	return m_bPostLoading;
}

BOOL CMyD3DApplication::IsAdultServer()
{
	if( g_SvrType == ST_ADULT_ONLY )
		return TRUE;

	return FALSE;
}

void CMyD3DApplication::SetTSS(int stage)
{
}

void CMyD3DApplication::GetTSS_Alpha(int stage, DWORD & arg1, DWORD & arg2, DWORD & op)
{
	GetFramework()->GetD3DDevice()->GetTextureStageState( stage, D3DTSS_ALPHAARG1, &arg1 );
	GetFramework()->GetD3DDevice()->GetTextureStageState( stage, D3DTSS_ALPHAARG2, &arg2 );
	GetFramework()->GetD3DDevice()->GetTextureStageState( stage, D3DTSS_ALPHAOP,   &op );
}

void CMyD3DApplication::SetTSS_Alpha(int stage, DWORD arg1, DWORD arg2, DWORD op)
{
	GetFramework()->GetD3DDevice()->SetTextureStageState( stage, D3DTSS_ALPHAARG1, arg1 );
	GetFramework()->GetD3DDevice()->SetTextureStageState( stage, D3DTSS_ALPHAARG2, arg2 );
	GetFramework()->GetD3DDevice()->SetTextureStageState( stage, D3DTSS_ALPHAOP,   op );
}

void CMyD3DApplication::SetTextureStageState()
{
}




void CMyD3DApplication::DeleteEffect_Fairy(int effect_skill_index)
{
	EffectSort *temp_effect = NULL;
	do
	{
		temp_effect = g_pRoh->FindMyFairy( effect_skill_index );
		if( temp_effect )
		{
			g_pRoh->DeleteEffectNum( temp_effect->nNum );
			pCMyApp->m_pFxSet->DeleteEffect( temp_effect->nNum );
		}
	}
	while( temp_effect );
}

void CMyD3DApplication::ChangeResolution(RESOLUTION iRes)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	if( iRes == R800 )
	{
		rect.right = 800;
		rect.bottom = 600;
	}
	else if( iRes == R1024 )
	{
		rect.right = 1024;
		rect.bottom = 768;
	}
	else if( iRes == R1280 )
	{
		rect.right = 1280;
		rect.bottom = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		rect.right = 1280;
		rect.bottom = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		rect.right = 1600;
		rect.bottom = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		rect.right = 1920;
		rect.bottom = 1080;
	}
	else
	{
		return;
	}

	m_ProOption.resolution = iRes;

	DWORD dwStyle;
	dwStyle = GetWindowLong(Get_hWnd(), GWL_STYLE);

	AdjustWindowRect(&rect, dwStyle, FALSE);
	SetWindowPos(Get_hWnd(), HWND_TOP, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW | SWP_NOSENDCHANGING);
}

void CMyD3DApplication::ChangeFullMode(BOOL bFullMode)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	if( m_ProOption.resolution == R800 )
	{
		rect.right = 800;
		rect.bottom = 600;
	}
	else if( m_ProOption.resolution == R1024 )
	{
		rect.right = 1024;
		rect.bottom = 768;
	}
	else if( m_ProOption.resolution == R1280 )
	{
		rect.right = 1280;
		rect.bottom = 960;
	}
	else if( m_ProOption.resolution == R1280W )
	{
		rect.right = 1280;
		rect.bottom = 720;
	}
	else if( m_ProOption.resolution == R1600W )
	{
		rect.right = 1600;
		rect.bottom = 900;
	}
	else if( m_ProOption.resolution == R1920W )
	{
		rect.right = 1920;
		rect.bottom = 1080;
	}

	DWORD dwStyle;
	BOOL bChanged = FALSE;
	if( bFullMode == TRUE && IsWinMode() == TRUE )
	{
		bChanged = TRUE;
		SetWinMode(FALSE);
		g_bFFullMode = TRUE;

		dwStyle = WS_POPUP|WS_VISIBLE;
	}
	else if( bFullMode == FALSE && IsWinMode() == FALSE )
	{
		bChanged = TRUE;
		SetWinMode(TRUE);
		g_bFFullMode = FALSE;

		dwStyle = WS_SYSMENU|WS_CAPTION|WS_BORDER|WS_POPUP|WS_VISIBLE;
		dwStyle ^= WS_MAXIMIZE;
		dwStyle ^= WS_MINIMIZE;
	}

	if( bChanged == TRUE )
	{
		SetWindowLong(Get_hWnd(), GWL_STYLE, dwStyle);
		AdjustWindowRect(&rect, dwStyle, FALSE);
		Change3DEnvironment();
		SetWindowPos(Get_hWnd(), HWND_TOP, 0, 0, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW | SWP_NOSENDCHANGING);
	}
}

void CMyD3DApplication::DrawLoading()
{
	if( g_pDisplay )
	{
		g_pDisplay->Clear();		

		if( m_pLoadingBack )
		{
			g_pDisplay->Blt(m_pLoadingBack->Xpos, m_pLoadingBack->Ypos, m_pLoadingBack);
		}
		if( m_pLoadingFrame )
		{
			g_pDisplay->Blt(m_pLoadingFrame->Xpos, m_pLoadingFrame->Ypos, m_pLoadingFrame);
		}		
	}	
}

void CMyD3DApplication::CheckLoadngImg()
{
	if( m_nLoadingFrameReserve != m_nLoadingFrameCurrent )
	{
		m_nLoadingFrameCurrent = m_nLoadingFrameReserve;
		MidLoading(m_nLoadingFrameCurrent);
	}	
}

void CMyD3DApplication::DrawLoadingGoWorld()
{
	if( g_pDisplay )
	{
		g_pDisplay->Clear();		

		if( m_pLoadingGoWorldBack )
		{
			g_pDisplay->Blt(0, 0, m_pLoadingGoWorldBack);
		}

		RECT rc;
		int cx = 0, cy = 0;
		if( m_pLoadingGoWorldFrame )
		{
			cx = m_pLoadingGoWorldFrame->GetWidth();
			cy = m_pLoadingGoWorldFrame->GetHeight();
		}
		int width = m_nLoadingGoWorldFrameCurrent * cx / 100;
		SetRect(&rc, 0, 0, width, cy);

		if( m_pLoadingGoWorldBox )
			g_pDisplay->Blt(m_pLoadingGoWorldBox->Xpos, m_pLoadingGoWorldBox->Ypos, m_pLoadingGoWorldBox);

		if( width > 0 && m_pLoadingGoWorldFrame )
			g_pDisplay->Blt(m_pLoadingGoWorldFrame->Xpos, m_pLoadingGoWorldFrame->Ypos, m_pLoadingGoWorldFrame, &rc);

		if( m_pLoadingGoWorldText )
			g_pDisplay->Blt(m_pLoadingGoWorldText->Xpos, m_pLoadingGoWorldText->Ypos, m_pLoadingGoWorldText);		
	}
}

void CMyD3DApplication::CheckLoadingGoWorldImg()
{
	if( m_nLoadingGoWorldFrameReserve != m_nLoadingGoWorldFrameCurrent )
	{
		m_nLoadingGoWorldFrameCurrent = m_nLoadingGoWorldFrameReserve;
		LoadingWorldImg(m_nLoadingGoWorldFrameCurrent);
	}
}
	
BOOL					IS_ADULT_SERVER()
{
	return pCMyApp->IsAdultServer();
}

LPDIRECTDRAW7		 	GET_DD7()
{
	return pCMyApp->GetFramework()->GetDirectDraw();
}
LPDIRECT3D7          	GET_D3D()
{
	return pCMyApp->GetFramework()->GetDirect3D();
}
LPDIRECT3DDEVICE7    	GET_D3DDEVICE()
{
	return pCMyApp->GetFramework()->GetD3DDevice();
}
LPDIRECTDRAWSURFACE7 	GET_DDSFB()
{
	return pCMyApp->GetFramework()->GetFrontBuffer();
}
LPDIRECTDRAWSURFACE7 	GET_DDSBB()
{
	return pCMyApp->GetFramework()->GetBackBuffer();
}
LPDIRECTDRAWSURFACE7 	GET_DDSL()
{
	return pCMyApp->GetFramework()->GetRenderSurfaceLeft();
}
LPDIRECT3DDEVICE7    	GET_D3DPICK()
{
	return pCMyApp->GetFramework()->GetD3DDevicePick();
}
LPDIRECTDRAWSURFACE7 	GET_DDSBPICK()
{
	return pCMyApp->GetFramework()->GetBackBufferPick();
}
LPDIRECTDRAWSURFACE7 	GET_DDSRT()
{
	return pCMyApp->GetRenderTarget();
}

UINT GET_CLIENT_WIDTH()
{
	return g_pNk2DFrame->GetClientWidth();
}
UINT GET_CLIENT_HEIGHT()
{
	return g_pNk2DFrame->GetClientHeight();
}
bool LH_SEND_NET_MSG(char * buf)
{
	if(g_pTcpIp && buf)
	{
		g_pTcpIp->SendNetMessage(buf);
		return true;
	}

	return false;
}

void LogMachineInfo();
void LogBugTrap(std::string strLog);

INT_PTR iLogHandle;
void SetupBugTrap()
{
	if( CommonConfig::Instance()->GetBugTrap() == false )
		return;

	FILE* fp = fopen("symindex", "rt");
	if( fp == NULL )
		return;

	char buf[256] = {0,};

	fscanf(fp, "%s", buf);

	fclose(fp);

	BT_InstallSehFilter();

	// ?????????????? ?????? ??????.
	// TCP?????? ?????? ???? ?????? ???? ???????????? ?????????? ?????? ?????? ????????.
	BT_SetAppVersion(buf);

	// ???????????? ?????? ????????.
	BT_SetAppName("LaghaimOnline New");

	DWORD dwFlag = BTF_DETAILEDMODE | BTF_SCREENCAPTURE;

	// BugTrap?? ?????? ????????.
	BT_SetFlags( dwFlag );

	// ???????? ???????? ?????? ????.
	// BTF_DETAILEDMODE : ?????????? ???????????? ???????? ???? ??????????. ???????? ???? ???? ???????? ?????? ??????????.
	// BTF_SCREENCAPTURE : ?????????? ????????.

	// ?????????? ???????????? ??????.
	BT_SetDumpType( MiniDumpWithDataSegs | MiniDumpWithIndirectlyReferencedMemory );

	// ?????? TCP?????? ????????.
	BT_SetSupportServer(_T("14.63.127.18"), 9999);

	iLogHandle = BT_OpenLogFile(NULL, BTLF_TEXT);
	BT_AddLogFile(BT_GetLogFileName(iLogHandle));

	LogMachineInfo();
}

void LogBugTrap(std::string strLog)
{
	if( CommonConfig::Instance()->GetBugTrap() == false )
		return;

	BT_InsLogEntry(iLogHandle, BTLL_INFO, strLog.c_str());
}

void SetupSingletons()
{
	DefenseValMgr::GetInstance()->Load();
	ItemFakeOptionMgr::GetInstance()->Load();
	ItemOutlineMgr::GetInstance()->Load();
}

//////////////////////////////////////////////////////////////////////////
// Get Machine Info
//////////////////////////////////////////////////////////////////////////
#include <strsafe.h>
#include <intrin.h>
#pragma comment(lib, "User32.lib")

#define BUFSIZE 512

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

BOOL GetOSDisplayString(LPTSTR pszOS)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	if( ! bOsVersionInfoEx ) return 1;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")),
				"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId &&
			osvi.dwMajorVersion > 4 )
	{
		StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

		// Test for the specific product.

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
			}

			if ( osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2 )
			{
				if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
				else if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8 "));
				else if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 0 && osvi.dwMajorVersion == 10) // marcel edit fix windows 10
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 10 ")); 
				else
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
			}

			pGPI = (PGPI) GetProcAddress(
					   GetModuleHandle(TEXT("kernel32.dll")),
					   "GetProductInfo");

			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			switch( dwType )
			{
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
				break;
			case PRODUCT_PROFESSIONAL:
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional" ));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
				break;
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			if( GetSystemMetrics(SM_SERVERR2) )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
			else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
					 si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
			}
			else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));

			// Test for the server type.
			if ( osvi.wProductType != VER_NT_WORKSTATION )
			{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
				}

				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
				}

				else
				{
					if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
					else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
				}
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
			else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
			}
			else
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
				else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
			}
		}

		// Include service pack (if any) and build number.

		if( _tcslen(osvi.szCSDVersion) > 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
		}

		TCHAR buf[80];

		StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);

		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
		}

		return TRUE;
	}

	else
	{
		printf( "This sample does not support this version of Windows.\n");
		return FALSE;
	}
}

BOOL GetMemInfo(LPSTR meminfo)
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG virtualMemAvail = memInfo.ullAvailPageFile;
	DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
	DWORDLONG physMemAvail = memInfo.ullAvailPhys;
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

	sprintf(meminfo, "Total VMem : %lld\nAvail VMem : %lld\nUsed  VMem : %lld\nTotal PMem : %lld\nAvail PMem : %lld\nUsed  PMem : %lld",
			totalVirtualMem, virtualMemAvail, virtualMemUsed, totalPhysMem, physMemAvail, physMemUsed);

	return TRUE;
}

BOOL GetCpuName(LPSTR cpuName)
{
	unsigned nHighestFeature;
	unsigned nHighestFeatureEx;

	int nBuff[4];
	char szMan[13];

	__cpuid(nBuff, 0);
	nHighestFeature = (unsigned)nBuff[0];
	*(int*)&szMan[0] = nBuff[1];
	*(int*)&szMan[4] = nBuff[3];
	*(int*)&szMan[8] = nBuff[2];
	szMan[12] = 0;

	__cpuid(nBuff, 0x80000000);
	nHighestFeatureEx = (unsigned)nBuff[0];

	if(nHighestFeatureEx >= 0x80000004)
	{

		char szCPUName[49];
		szCPUName[0] = 0;
		__cpuid((int*)&szCPUName[0], 0x80000002);
		__cpuid((int*)&szCPUName[16], 0x80000003);
		__cpuid((int*)&szCPUName[32], 0x80000004);
		szCPUName[48] = 0;

		for(int i=(int)strlen(szCPUName)-1; i>=0; --i)
		{
			if( szCPUName[i] == ' ' )
				szCPUName[i] = '\0';
			else
				break;
		}

		sprintf(cpuName, "CPU : %s(%s)", szCPUName, szMan);
	}

	else
	{
		sprintf(cpuName, "CPU : %s", szMan);
	}

	return TRUE;
}

void LogMachineInfo()
{
	char info[512];

	if( GetOSDisplayString(info) )
	{
		LogBugTrap(info);
	}
	if( GetCpuName(info) )
	{
		LogBugTrap(info);
	}
	if( GetMemInfo(info) )
	{
		LogBugTrap(info);
	}
}