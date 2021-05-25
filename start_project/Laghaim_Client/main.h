#ifndef _MAIN_H_
#define _MAIN_H_

#include "Config.h"

#include "D3DApp.h"
#include "D3DFrame.h"
#include "D3DEnum.h"
#include "D3DUtil.h"
#include "MsgPopUp.h"
#include "sfx.h"
// Happy New Year Event
#include "HappyNewYear.h"
#include "BattleStart.h"
#include "PlaceInfo.h"
#include "Sprite.h"
#include "SpiritMode.h"
#include "FxCasting.h"
#include "FxRegenStart.h"
#include "LHSingleton.h"

#include "Conversion.h"

#define WIN32_LEAN_AND_MEAN

typedef enum { NKGRAPHIC2D, NKGRAPHIC3D, NKGRAPHICLOADING, NKGRAPHICGOWORLD } NKGRAPHIC_MODE;

#define TYPE_SAFE_ZONE		11
#define TYPE_HUNTING_ZONE   12
#define TYPE_PK_ZONE		13

#define NORMAL_DRV 1
#define INTEL_DRV 2
#define XABRE_DRV 3

#define PEACEZONE_SOUND 1
#define WARZONE_SOUND 2
#define SIEGEWAR_SOUND 3
/*#define DUNGEON8_SOUND 4
#define DMITRON_SOUND 5
#define OLD_SHAILON_SOUND 6 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define OLD_LAGLAMIA_SOUND 7 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define OLD_SIROOST_SOUND 8 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define OLD_PAROS_SOUND 9 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define OLD_DECARDI_SOUND 10 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define OLD_DEKAREN_SOUND 11 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define OLD_DEKADUN_SOUND 12 ///일본BGM 옛날껄로 돌리는 이벤트 때문에 추가.
#define FORLORN_SOUND		13 // 20번존 사운드*/
#define MAP0_SOUND		4
#define MAP1_SOUND		5
#define MAP2_SOUND		6
#define MAP3_SOUND		7
#define MAP4_SOUND		8
#define MAP5_SOUND		9
#define MAP6_SOUND		10
#define MAP7_SOUND		11
#define MAP8_SOUND		12
#define MAP9_SOUND		13
#define MAP10_SOUND		14
#define MAP11_SOUND		15
#define MAP12_SOUND		16
#define MAP13_SOUND		17
#define MAP14_SOUND		18
#define MAP15_SOUND		19
#define MAP16_SOUND		20
#define MAP17_SOUND		21
#define MAP18_SOUND		22
#define MAP19_SOUND		23
#define MAP20_SOUND		24
#define MAP21_SOUND		25
#define MAP22_SOUND		26
#define MAP23_SOUND		27
#define MAP24_SOUND		28
#define MAP25_SOUND		29
#define MAP26_SOUND		30
#define MAP27_SOUND		31





//////////////////////////////////////////////////////////////////////////
// [6/5/2007 Theodoric] 서버 타입 결정
typedef enum Server_Type
{
	ST_12_OVER = 0,			// 12세 이상
	ST_MATRIX,				// 매트릭스 서버
	ST_MATRIX_TOURNAMENT,	// 매트릭스 대회
	ST_ADULT_ONLY,			// 성인용
// by evilkiki 2009.11.05 길드랭킹전
	ST_GUILD_RANK			// 길드랭킹
} SVRTYPE, SERVERTYPE, SVR_TYPE ;
//////////////////////////////////////////////////////////////////////////

extern SVR_TYPE	g_SvrType;


enum eServerMode { eSVR_NORMAL = 0, eSVR_MASTER };
extern eServerMode g_SvrMode;

#include <map>
typedef std::map<int, CCapsyong*>		CSYMAP;
typedef CSYMAP::iterator				CSYMAP_it;
typedef CSYMAP::value_type				CSYMAP_vt;

//---------------------------------------------------------------------------
// Defines, constants, and global variables
//---------------------------------------------------------------------------
class CLand;
class CFrustum;
class CHanIme;
class CObjectList;
class CNkCharacter;
class CNkMyPet;
class IndexedTexture;
class CShadowRect;
class CUIMgr;
class CDecoObjSet;
class CTcpIpConnection;
class CDMusicMgr;
class CSlotChar;
class CCompas;
// nate 2004 - 7
// 사용 안하고 있음
//class CTestEffect;
class CWeatherMgr;
class CCapsyong;
class CLightCtrl;
class CGoodShadow;
class CMouse;
class CDecoObj;
class CMsgPopUp;
class CFxSet;
class CFireFly;
class CNkMob;
class C2DResMgr;
class CPath;
class CCloudSea;
class CSkyPlane;
class CMp3Module;
class CMusicManager;
class CMusicSegment;
class LHParam_Manager;

const WORD	g_wControl_Panel_Height = 90;

enum RESOLUTION {R640, R800, R1024, R1280, R1280W, R1600W, R1920W} ;
enum SHADOW_OPTION {SHADOW_NONE, SHADOW_ALONE, SHADOW_PLAYER, SHADOW_MONSTER} ;

typedef struct
{
	RESOLUTION		resolution;
	SHADOW_OPTION	shadow;
	BOOL			bWeather;
	BOOL			bMusicOn;
	BOOL			bMusicEffOn;
	int				nMusicLevel;
	int				nMusicEffLevel;
	BOOL			bWindowed;
	int				nBackBufferBit;
	BOOL			bUseMusicForEachZone;
} PROGRAM_OPTION;

class CMyD3DApplication : public CD3DApplication // , public LHSingleton<CMyD3DApplication>
{
	virtual VOID Cleanup3DEnvironment();

protected:

	HRESULT OneTimeSceneInit();
	HRESULT	DeleteUserObjects();
	HRESULT InitDeviceObjects();
	HRESULT Init2DFrameObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT Render();
	HRESULT DisplayFrame();
	HRESULT FrameMove( FLOAT fTimeKey );
	HRESULT FinalCleanup();
	HRESULT RestoreSurfaces();

	// 디바이스 능력 확인 함수.
	///static HRESULT ConfirmDevice( DDCAPS* pddDriverCaps, D3DDEVICEDESC7* pd3dDeviceDesc );
	static HRESULT ConfirmDevice(DDCAPS* pddDriverCaps);
	HRESULT			CheckDeviceCaps();
	BOOL			m_bCanDoMultitexture, m_bCanDoAddSigned, m_bCanDoModulate, m_bCanDoModulate2X;
	BOOL			m_bCanDoAdd;
	BOOL			m_bCanRender32Bit, m_bCanHighQuality;
	D3DSHADEMODE	m_ShadeMode;

	// 카메라
	D3DVECTOR		m_vEye, m_vEyeZ, m_vEyeY, m_vAt, m_vUp;
	D3DMATRIX		m_matVirView;
	D3DVALUE		m_Fov, m_Aspect, m_zNear, m_zFar;
	BOOL			m_bWireframe;
	float			m_fAtHeight, m_fCamHeight;

	// 지형
	CLand			*m_pLand;
	int				m_idxLandTexture, m_idxLandTexture2;
	BOOL			m_bSelTile;
	CCloudSea		*m_pCloudSea;	// 운해
	CSkyPlane		*m_pSkyPlane;	// 네모하늘
	CPath			*m_pPath;
	CFrustum		*m_pFrustum;	// 뷰 프러스텀
	// 카메라가 보고 있는 곳 오브젝트.
	///D3D9Vertex		m_Origin[8];
	WORD			m_idxOrigin[36];
	// 한글 입력
	CHanIme			*m_pHanIme;
	BOOL			m_bGameMode;
	HFONT			m_hFont, m_hOldFont;
	// 통신 부분
	CTcpIpConnection *m_pTcpIpAlter;
	// 2D, 3D Mode 변수
	NKGRAPHIC_MODE	 m_GraphicMode;
	// 2D 로고와 인터페이스...
	CDMusicMgr		*m_pDMusicMgr;
	CMusicManager	*m_pMusicManager;
	C2DResMgr       *m_p2DResMgr;

public:

	CNkMyPet		*m_pMyPet;		// 주인공꺼 애완동물이다.
	CSlotChar		*m_pSlotChar;	// 캐릭터의 특성값을 가지고 있다.

	D3DVECTOR		m_vListener;
	void			PlayMusic(int id = 0);
	void			StopMusic(int id = 0);
	int             GetCurGameMode()
	{
		return m_GraphicMode;
	}

	CNkMob			*m_pNearMob;

	CFxSet			*m_pFxSet;	// 특수 효과.
	CWeatherMgr		*m_pWeatherMgr;
	CFireFly		*m_pFireFly;
	CMsgPopUp		m_MsgPopUp;	// 종료시에 띄울 메세지 팝업
	// 레이다
	CCompas			*m_pCompas;
	int				m_nCompasX, m_nCompasY;

	BOOL			m_bShowCutton;
	void			RenderCutton();
	void			ShowCutton()
	{
		m_bShowCutton = !m_bShowCutton;
	};
	BOOL			m_bTmp;
	// 백그라운드 하늘 그리기.
	void			RenderSky();
	// 주인공 수영상태 지정
	void			SetHeroSwimming(BOOL bSwimming)
	{
		m_bHeroSwimming = bSwimming;
	}
	BOOL			GetHeroSwimming()
	{
		return m_bHeroSwimming;
	}

protected:

	BOOL		m_bHeroMoving;
	BOOL		m_bHeroSwimming;
	BOOL		m_bMoreToGo;
	BOOL		m_bInsideBuilding;
	BOOL		m_bWaypointMove;

	float		m_fHeroDx, m_fHeroDz;
	float		m_fHeroTx, m_fHeroTz;
	float		m_fHeroIx, m_fHeroIz;
	float		m_fHeroPx, m_fHeroPz;
	float		m_HeroAniRate;
	DWORD		m_dwPrevFrameTime;
	float		m_fSpeedMul;

	CDecoObjSet	*m_pDecoObj;	// Decoration Object Sample
	// picking buffer에 무언가 제대로 써 있는가를 보기 위한 테스트.
	BOOL		m_bPickView;
	BOOL		m_bPickBegin;
	BOOL		m_bShadowView;	// 그림자 버퍼에 뭔가가 제대로 써졌나 보려구...
	D3DVECTOR	m_vMouse;		// 마우스가 찍은 곳의 맵 좌표
	HRESULT		Render2DEnvironment(HWND hwnd);
	HRESULT		Render3DEnvironment();
	void		RestoreVBs();

public:

	CUIMgr				*m_pUIMgr;	// 유저 인터페이스 매니저.
	CLightCtrl			*m_pLightCtrl;	// 허접한 라이트 컨트롤.

	// 잡다한 테스트용 정보를 넣을 텍스처 클래스
	// nate 2004 - 7 m_pTestTexture제거
	//	IndexedTexture		*m_pTestTexture;
	// 캐릭터와 장비 텍스처.
	//IndexedTexture		*m_pChaTexture;
	// Shadow Test
	// nate 2004 - 7 글로벌 ShadowRect사용
	//	CShadowRect			*m_pShadowRect;
	// nate 2004 - 7 글로벌 ShadowRect사용
	//	CShadowRect *		GetShadowRectMgr() { return m_pShadowRect; };

	IndexedTexture	*m_pDecoTexture;

	BOOL			m_bDrawShadow;

	CGoodShadow		*m_pGoodShadow;	// 머찐 그림자...
	CMouse			*m_pMouse;	// 애니메이션 마우스 포인터.
	CMp3Module		*m_pMp3Module[4];


public:
	//===============================
	// nate 2004 - 9 : GuildHunt
	BOOL IsGuildHuntArea( float fX = 0.0f, float fZ = 0.0f );
	BOOL IsPrisonArea( float fX, float fY );
	BOOL IsFianlBossRoomArea( float fX, float fZ );

	//===============================
	// [5/19/2009 D.K ] : boss room area.
	BOOL IsBossRoomArea( float fX, float fZ );
	//===============================
	BOOL m_bPlaying, m_bFastMove;

	void SetSelTile(BOOL bSelTile)
	{
		m_bSelTile = bSelTile;
	};
	BOOL IsTileSelected()
	{
		return m_bSelTile;
	};
	void SetDrawShadow(BOOL bDrawShadow)
	{
		m_bDrawShadow = bDrawShadow;
	};
	void SetMouseWorldCoord(D3DVECTOR &vMouse)
	{
		m_vMouse = vMouse;
	};

	BOOL IsRgb565()
	{
		return m_bRGB565;
	};
	BOOL IsPickBegin()
	{
		return m_bPickBegin;
	};

	// 카메라를 통한 회전과 이동.
	BOOL m_bRotateCamera;
	POINT m_ptCurPos;
	void GoForward();
	void GoBackward();
	void GoRight(float right);	// [11/28/2007 반재승] 카메라 eye값만 이동
	void GoLeft(float left);	// [11/28/2007 반재승] 카메라 eye값만 이동
	void StartRotateCamera(int mx, int my);
	void EndRotateCamera();
	void RotateCamera(POINT ptOld, int mx, int my);
	void RotateCamera();
	void RotateCameraPI(POINT ptOld, int mx, int my); //mungmae-2006/07/04 카메라 180회전을 위한 함수.
	void RotateCameraPI();
	void RotateRight();
	void RotateLeft();
	void ForwardCamDirection();
	void BackwardCamDirection();
	void SetFov(float fov)
	{
		m_Fov = fov;
	};
	// 관찰자 모드
	BOOL m_bSatelliteMode;
	void ToggleSatelliteMode();

	// 카메라 와 플레이어.
	HRESULT UpdateProjectMatrix();
	void SetSpotLight(D3DVECTOR vPos, D3DVECTOR vDir, BOOL bOn);
	void SetViewMatrix(D3DMATRIX &mat);
	void SituationEye(D3DMATRIX &m, D3DVECTOR &eye, D3DVECTOR &eye_z, D3DVECTOR &eye_y);
	void SetCameraInitPos(D3DVECTOR &vFrom, D3DVECTOR &vAt);
	D3DVECTOR GetEyeDirection()
	{
		return m_vEyeZ;
	};
	D3DVECTOR GetEyeAtPos()
	{
		return m_vAt;
	};
	void	  GetEyeAtPos(D3DVECTOR &vPos)
	{
		vPos = m_vAt;
	};
	void	  GetEyePos(D3DVECTOR &vPos)
	{
		vPos = m_vEye;
	};
	BOOL IsHeroMoving()
	{
		return m_bHeroMoving;
	};

	int  m_iOldAreaType;
	int  m_iAreaType;
	BOOL IsPeaceArea(CNkCharacter *pNkCha = NULL, float wx = 0.0f, float wz = 0.0f);
	BOOL IsColo(float x, float z);
	BOOL IsInsideMap(float x, float z);
	BOOL IsInsideRect(float x, float y, float left, float top, float right, float bottom);
	BOOL IsInsideRhombus(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void SmartExit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // 종료시에 호출되는 함수

	// 시간의 변화에 따른 빛과 그림자 컨트롤.
	int		m_nCurHour;				// 디버깅용.. 현재 시간을 알려고.
	void	ChangeHour(int hour = -1);			// 간단한 테스트 함수.

	void	ToggleWireframe();

	CLand* GetLandPtr()
	{
		return m_pLand;
	};
	CPath* GetPath()
	{
		return m_pPath;
	};
	void SetHeroInitPos(float x, float y, float z, float dir, BOOL bInit = TRUE);
	// 이동관련 함수.
	void FindHeroPath(float &tx, float &tz, BOOL bWaypointMove = FALSE);
	BOOL IsBlockedPath(float wx, float wz, float tx, float tz);
	BOOL MoveHero(float x, float z, BOOL bWaypointMove = FALSE);
	void HeroMoving();
	void StopHero(BOOL bMsg = TRUE);
	void AvoidObj(float x, float z);
	void DrawPlayer();
	float m_fPointRColor, m_fPointGColor, m_fPointBColor;

	// 타일단위 이돈관련함수.
	void FindHeroPathT(float &tx, float &tz, BOOL bWaypointMove = FALSE);
	void FindHeroPathTN(float &tx, float &tz, BOOL bWaypointMove = FALSE);
	BOOL IsBlockedPathT(float wx, float wz, float tx, float tz);
	BOOL MoveHeroT(float x, float z, BOOL bWaypointMove = FALSE);
	void HeroMovingT();

	// 때리려고 다갑니다!!!
	void SetHeroAttGoCha(CNkCharacter *pCha);
	void SetHeroAttGoMob(CNkMob *pMob);
	void SetHeroCurSkill(int nSkill);

	CNkCharacter *m_pHeroAttGoCha;
	CNkMob *m_pHeroAttGoMob;

	BOOL	CompareHeroAttGo();
	int		GetSkillDistance(int li_selskill);
	BOOL	AttackMsgSend(int cur_skill);
	bool	SkillConditionCheck(int cur_skill);
	int		GetTargetCountOfSkill( int nCurSkill );	// nate 2005-12-15 : 스킬별 타겟 갯수
	int		m_nCurSkill;					// 카이립톤 자동 공격을 위한 스킬.


	bool CheckSkillCondition(int cur_skilll);


	// 구름 효과.
	D3DTLVERTEX m_CloudMesh[4];
	D3DTLVERTEX m_CloudLayerMesh[4];
	WORD		m_wCloudIdx, m_wCloudLayerIdx;

	CMyD3DApplication();
	void	Init();

	CFrustum *GetFrustum()
	{
		return m_pFrustum;
	};

	BOOL	m_bBufferPaused;
	BOOL    m_bToggleFullscreen;
	BOOL	IsRenderingOk;

	VOID    OutputText( DWORD x, DWORD y, TCHAR* str );
	void	ShowStats2();

	void	ShowStats_Nalrim();
	void	ToggleStats_Nalrim()
	{
		m_bShowStats_Nalrim = !m_bShowStats_Nalrim;
	}

	HRESULT ProcessNextFrame(HWND hwnd);							// 2D, 3D Rendering 호출 루틴.
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
					 LPARAM lParam );

	void			InitVariables();				// 게임에 사용할 변수의 초기화
	BOOL			LoadResources(int which = 4, bool force3d = false);				// 맵과 오브젝트와 같은 리소스를 로딩


	HINSTANCE hInstApp;
	BOOL			m_bActiveApp;
	char			strExePath[_MAX_PATH+_MAX_FNAME+_MAX_EXT];
	char			*GetExePath()
	{
		return strExePath;
	};
	BOOL			IsActiveApp()
	{
		return m_bActiveApp;
	};

	// 게임 리소스 로딩
	void			StartGame();	
	BOOL			LoadGameResources();			// 게임 실행중에 필요한 리소스 로딩
	HRESULT			AdjustViewport();
	void			InitTimeCtrl();

	// 프로그램 옵션.
	PROGRAM_OPTION	m_ProOption;
	void			ReadConfigFile(char *file_name);
	void			WriteConfigFile(char *file_name);


	void			ExcuteOption();
	void			ExcuteFFullOption();			// Fake Full-Mode Option
	void			ExcuteWindowedOption();			// 창모드 지원.

	// 디바이스 체크.
	inline BOOL	CanDoMultitexture()
	{
		return m_bCanDoMultitexture;
	};
	inline BOOL CanDoAddSigned()
	{
		return m_bCanDoAddSigned;
	};
	inline BOOL CanDoModulate()
	{
		return m_bCanDoModulate;
	};
	inline BOOL	CanHighQuality()
	{
		return m_bCanHighQuality;
	}


	// 통계 자료..
	DWORD			m_dwDecoTime, m_dwLandTime, m_dwRohTime, m_dwPlayerTime, m_dwMobTime, m_dwItemTime, m_dwWeatherTime;
	DWORD			m_dwRenderRohTime;
	DWORD			m_dwPlayingTime; // 접속시간 출력을 위한 기억틱.
	DWORD			m_dwOldPlayingTime; // 접속시간 출력을 위한 기억틱.

	void	Read_ChooseMode(char *);  // choose_mode.txt 파일을 읽고 내용이"0"이면 창화면, "1"이면 전화면모드로 전화시킨다

	// 데이타 로딩시 화면 디스플레이.
	CDisplay *m_pLoadDisp;
	void	InitScreen();
	void	InitLoadingScr();
	void	InitProgress(int nFrame);
	void	StartGameScr(int nBmpNum);
	// 로딩시 Progress 바...
	void	LoadingProgress(int percent, int nBmpNum=3);
	void	MidLoading(int n);
	void	MidLoadingReserve(int n);
	void	LoadingWorldImg(int percent);
	void	LoadingWorldImgReserve(int percent);

	void	DrawLoading();
	void	CheckLoadngImg();

	void	DrawLoadingGoWorld();
	void	CheckLoadingGoWorldImg();	

private:
	CSurface* m_pLoadingBack;
	CSurface* m_pLoadingFrame;
	int m_nLoadingFrameReserve;
	int m_nLoadingFrameCurrent;

	CSurface* m_pLoadingGoWorldBack;
	CSurface* m_pLoadingGoWorldBox;
	CSurface* m_pLoadingGoWorldFrame;
	CSurface* m_pLoadingGoWorldText;
	int m_nLoadingGoWorldFrameReserve;
	int m_nLoadingGoWorldFrameCurrent;	

public:
	int     m_iCurVillageId;
	int     m_iCurWorldID;
	void    SetCurWorldID(int id)
	{
		m_iCurWorldID = id;
	}
	int     GetCurWorldID()
	{
		return m_iCurWorldID;
	}
	void    SetCurVillageID(int id = -1)
	{
		m_iCurVillageId = id;
	}
	int     GetCurVillageID()
	{
		return m_iCurVillageId;
	}

	// 로드 맵 함수.
	void			OpenNewMap();
	// nate 2004 - 7 m_pTestTexture제거
	// BOOL			LoadWorldMap(char *strMapFilePath, char *strDecoFilePath, IndexedTexture *pIndexedTexture, int nMapNum=4);
	BOOL			LoadWorldMap(int zone_index);
	BOOL			LoadWorldMap(char *strMapFilePath, char *strDecoFilePath, int nMapNum=4);
	//BOOL			LoadWorldMap(char *strMapFilePath, char *strDecoFilePath, int nMapNum=10);  //ENABLE FOR MATRIX WAR ONLY  PABLO 02/10/2019
	BOOL			GetLoadFilePath(const char *strOpen, char *strMapPath, char *strDecoPath);
	char			*m_pszMapName;
	DWORD			m_dwGoWorldBaseTime;
	DWORD			m_dwTransBaseTime;
	BOOL			m_bBeforeGoWorld;
	BOOL			m_bBeforeTrans;
	BOOL			m_bWaitingAt;
	BOOL			IsBeforeGoWorld()
	{
		return m_bBeforeGoWorld;
	};
	void			PrepareChangeWorld();
	void			SetTransTime(BOOL bBeforeTrans = TRUE);
	BOOL			ConnectWorld(char *strIpAddr, int nPort);
	BOOL			GoWorld(char *strIpAddr, int nPort, int nWorldNum, int nAreaNum);
	void			GoWorldLoading();
	
	int				m_nCurWorldIdx;							// 현재 내가 있는 월드.
	int				m_nAreaNum;
	int				GetCurWorld()
	{
		return m_nCurWorldIdx;
	}
	int				m_SlotNum;
	char			m_strUserId[32];
	char			m_strLetterUserId[30];
	char			m_strUserPw[2048];
	int				m_nAutoLoginSvrIdx;
	// 자동 로긴을 위해서.
	void			SetLoginInfo(int nSvrIdx, char *strUserId, char *strUserPw, char *strUserKey);
	void			AutoLogin();

	BOOL			m_bTest;
	D3DTLVERTEX		m_pickMesh[4];

	// 새로운 픽킹 버퍼.
	LPDIRECTDRAWSURFACE7	m_pddsPickRender;
	LPDIRECTDRAWSURFACE7	m_pddsPickZBuffer;

	// Fog 관련.
	BOOL			m_bCanDoTableFog;
	BOOL			m_bCanDoVertexFog;
	BOOL			m_bCanDoWFog;

	DWORD m_dwFogMode;
	FLOAT m_fFogStart;
	FLOAT m_fFogEnd;
	FLOAT m_fFogDensity;
	DWORD m_dwFogColor;

	BOOL  m_bRangeBasedFog;
	BOOL  m_bUsingTableFog;
	BOOL  m_bDeviceUsesWFog;

	void	SetFogParameters(DWORD dwMode, DWORD dwColor, BOOL bRangeBased, BOOL bTableFog, float fStart, float fEnd, float fDensity );
	void	SetLandFogParameter();
	LPDIRECTDRAWSURFACE7	GetPickBuffer()
	{
		return m_pddsPickRender;
	};
	void	ClearPickBuffer();
	HRESULT	CreatePickBuffer();
	void	DeletePickBuffer();
	int		m_nPickWidth, m_nPickHeight;
	void	RestartGame();

	//------------------------------------------------------------------------
	// 캐릭 밑에 상태바 ...
	int m_nShowRohEnerge;
	void ShowRohEnergeBar();
	// 파티원 상태바 Draw
	int	 DisplayPartyVital(CNkCharacter*, float);
	//int  DisplayPlayerVital(CNkCharacter*, float); //desativado
	// Summon 상태바
	int  DisplaySummonVital(CNkMob *pMob);
	//디바인 스톤의 상태바
	int  DisplayDivineVital(CNkMob *pMob);
	int	 ShowMyPartysVital();

	// vital Bar 를 표현한다.
	void RenderOthersVital(int, int, int, int, int, int, int, int,int);
	// Hitpoints
	void DisplayChaHitpoints(CNkCharacter *pCha);
	void DisplayMobHitpoints(CNkMob *pMob);
	void RenderHitPoints(int, int, DWORD, DWORD, DWORD = 2000);
	// 주인공이 뛰고 있는지 아닌지...
	BOOL m_bRohRunning;
	DWORD m_startRunningTime;
	// 스킬저장에 관한 변수 / 함수....
	char    m_sCheckStr[10];
	char	m_SaveChaName[30];
	int		m_SaveCha_Rskill;
	int		m_SaveCha_Lskill;
	void	WriteLastSkill();  // 마지막 선택된 스킬을 저장한다
	void    ReadAndAppSkill();
	void    SetDefaultVal();
	// 하늘 + 구름 렌더링을위한..
	D3DTLVERTEX  TLVec[4];  // 하늘 배경 사각형
	D3DTLVERTEX  TLVec2[4]; // 흐르는 구름 사각형
	D3DTLVERTEX  TLVec3[4];
	D3DTLVERTEX  TLVec4[4];

	CSphere*		sky;
	CSphere*		sky_cloud;
	CCylinderPlane*	sky_fog;
	CRectPlane*		ground;

	WORD			m_wSky_Fog;

	void RenderSky2();
	void RenderSky3();
	WORD     m_wSky01, m_wSky02, m_wSky03, m_wSky04, m_wSky05, m_wSky06;
	BOOL      m_bShowRenderSky2;

	float m_fog_end, m_fog_start;
	void SetFog(BOOL bFog);


	// 카메라 회전각에 따른 하늘 텍스쳐 회전 관련 ..
	float m_fRotate_Cloud_Angle;
	void RotateCloud();

	C2DResMgr* Get2DResMgr()
	{
		return m_p2DResMgr;
	}

	// ime improments
	DWORD m_dwImmConv, m_dwImmSent;
	BOOL  m_bImmOpen;
	HKL		m_hkl;
	void	ConvertImmStatus();
	BOOL	SaveImmStatus();
	void	RestoreImmStatus();

	int		m_nBackMidi[7];

	void SendDummyMsg();

	BOOL	m_IsMsgPopupInputEnable;

	// 대형장비 딜레이
	DWORD	m_lEquipTime;
	BOOL	CanUseLEquip();

	//------------------------------------------------------------------------
	// Lock : 서버의 응답이 각 장비에 적용 되어야 하는 경우 응답 전 아이템 이동 방지.
	BOOL	m_bEquipLock; // 대형장비 lock
	BOOL	m_bRingLock;  // 반지 lock
	BOOL  m_bBrLock;    // 팔찌 Lock (2004.10.18/Lyul팔찌)
	BOOL  m_bNecklaceLock;    // 목걸이 Lock (06-03-29 원석) ///페어리


	///1118  오토마우스 막기위한...
	DWORD	m_lClickTime;
	BOOL	CanClick();

	BOOL	m_bSelectCha;
	BOOL	m_bInitEnergyBar; //첨에 그려주는 에너지바인지...

	BOOL	m_bInverseReturn;//0124 +아템을 버리거나 팔때 뜨는 팝업창에서 리턴키는 Ok가 아니라 취소로 하기위해...

	virtual HRESULT Create( HINSTANCE, TCHAR* );

	//==============================================================================
	// nate 2004 - 11 : Happy New Year Event
	CHappyNewYear*	m_NewYear;
	BOOL CreateHappyNewYear();
	void DeleteHappyNewYear();
	BOOL DrawHappyNewYear;
	//==============================================================================
	CBattleStart*	m_BattleStart;
	BOOL CreateBattleStart(int rendnum);
	void DeleteBattleStart();
	BOOL DrawBattleStart;
	int m_nBSRendNum;
	//==============================================================================
	int		Font_AddPosY;

	int		m_DrvType;

	void DrawDrvTestPickVex();
	void SetDrvTypeClrAlloc();

	int m_PlaySndType;

	int m_IsPcBang_User; // [2008/11/10 Theodoric] 유저들의 결재 타입을 구분 한다. ( 0:일반 결재 / 1:PC방 / 2:PREMIUM )
	DWORD m_PcBangStartTime; // [2008/5/29 Theodoric] Pc방 30분 채크

	void PcBangUserDraw();

	void DashHero(float dx, float dz); // Lyul_2005
	//wan_mat 매트릭스존 위치 정보
	int	PlaceInfo(float x, float z);
	CPlaceInfo*	m_PlaceInfo;
	void CreatePlaceInfo(int texNum);
	void DeletePlaceInfo();
	BOOL DrawPlaceInfo;
	int m_nCurPlaceNum;

	//mungmae-2005/06/23 뫼비우스 리젠후 스타트
	CFxRegenStart*	m_pRegenStart;
	BOOL		    m_bRegenStart;

	//mungmae-2005/07/13 카이립톤 헬다운 관련 추가
	CFxCasting*	m_pCasting;
	BOOL    DrawCasting;
	DWORD   m_dPressTime;
	DWORD   m_dwCastingTime;
	BOOL    m_bHellDownReady;
	BOOL    m_bHellDownSec;			//헬다운 두번째 이펙트
	BOOL	m_bHellDown_Cooltime;
	int		m_nGrade;				//텍스쳐 번호
	int		m_nHellDown_Grade;
	int		m_nCurHellDown_Grade;
	void	Goworld_CharInit();		// nate 2005-07-28 : 존 이동시 주캐릭 변수 초기화
	int		IsAcropolis(float fX = 0.0f, float fZ = 0.0f);	// nate 2006-01-16 : 유적지 영역 체크
	int		IsSoccerZone(float fX = 0.0f, float fZ = 0.0f);	// nate 2006-05-15 : 메트릭스 월드컵 경기장

	int     m_RotateFrame;
	int     m_ZoomInFrame;
	BOOL    m_SpiritRotateCamera;
	BOOL    m_bSpiritZoomIn;

	void    SpiritRotateCamera();
	void    SpiritRotateCamera(POINT ptOld, int mx, int my);
	void    SpiritZoomIn();

	DWORD m_NvsnMoveDelay; // 전장이동시 딜레이  ///국가전이동딜레이

protected:
	BOOL	m_bWaterRender;

public:
	BOOL	IsCurWorldID_BossraidZone();
	BOOL	IsWaterRender()
	{
		return m_bWaterRender;
	}
	void	SetWaterRender(BOOL bWaterRender)
	{
		m_bWaterRender = bWaterRender;
	}

protected:
	BOOL	m_bPostLoading;

public:
	void	SignalPostLoad();
	BOOL	NeedPostLoading();
	
	HANDLE m_hMutex;
	BOOL CheckGameMutex();
	void ReleaseGameMutex();

	// 보스레이드존에서 보스있는 곳에서는 귀환하지 못하도록 좌표 설정
	BOOL IsBossradePeaceArea(CNkCharacter *pNkCha = NULL, float wx = 0.0f, float wz = 0.0f);

	float	m_fMoveValue;		// 일동 이동속도 향상 아이템 적용을 위해

	BOOL IsAdultServer();


public:
	void SetTextureStageState();

	void SetTSS(int stage);

	void GetTSS_Alpha(int stage, DWORD & arg1, DWORD & arg2, DWORD & op);
	void SetTSS_Alpha(int stage, DWORD arg1, DWORD arg2, DWORD op);



public:
	CSYMAP			m_race_csy_map;
	BOOL			InitNkChaSet();
	CCapsyong		**m_dpChaCapsyong;
	int				m_nCsyNum;

public:
	LHParam_Manager * m_param_manager;
	int m_music_id;

public:
	void DeleteEffect_Fairy(int effect_skill_index);

public:
	void SetWinMode(BOOL bWindowed)
	{
		m_bWindowed = bWindowed;
	}

	void ChangeResolution(RESOLUTION iRes);
	void ChangeFullMode(BOOL bFullMode);

	bool CheckRestore();

};

extern bool				   g_dev_client;
extern CMyD3DApplication * pCMyApp;

LPDIRECTDRAW7        	GET_DD7();
LPDIRECT3D7          	GET_D3D();
LPDIRECT3DDEVICE7    	GET_D3DDEVICE();
LPDIRECTDRAWSURFACE7 	GET_DDSFB();
LPDIRECTDRAWSURFACE7 	GET_DDSBB();
LPDIRECTDRAWSURFACE7 	GET_DDSL();
LPDIRECT3DDEVICE7    	GET_D3DPICK();
LPDIRECTDRAWSURFACE7 	GET_DDSBPICK();
LPDIRECTDRAWSURFACE7 	GET_DDSRT();

UINT 					GET_CLIENT_WIDTH();
UINT 					GET_CLIENT_HEIGHT();

BOOL					IS_ADULT_SERVER();

// #define GET_DD7()		pCMyApp->GetFramework()->GetDirectDraw()
// #define GET_D3D()		pCMyApp->GetFramework()->GetDirect3D()
// #define GET_D3DDEVICE()	pCMyApp->GetFramework()->GetD3DDevice()
// #define GET_DDSFB()		pCMyApp->GetFramework()->GetFrontBuffer()
// #define GET_DDSBB()		pCMyApp->GetFramework()->GetBackBuffer()
// #define GET_DDSL()		pCMyApp->GetFramework()->GetRenderSurfaceLeft()
// #define GET_D3DPICK()	pCMyApp->GetFramework()->GetD3DDevicePick()
// #define GET_DDSBPICK()	pCMyApp->GetFramework()->GetBackBufferPick()

bool LH_SEND_NET_MSG(char * buf);

#endif // _MAIN_H_