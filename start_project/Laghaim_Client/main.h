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
#define OLD_SHAILON_SOUND 6 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define OLD_LAGLAMIA_SOUND 7 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define OLD_SIROOST_SOUND 8 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define OLD_PAROS_SOUND 9 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define OLD_DECARDI_SOUND 10 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define OLD_DEKAREN_SOUND 11 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define OLD_DEKADUN_SOUND 12 ///�Ϻ�BGM �������� ������ �̺�Ʈ ������ �߰�.
#define FORLORN_SOUND		13 // 20���� ����*/
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
// [6/5/2007 Theodoric] ���� Ÿ�� ����
typedef enum Server_Type
{
	ST_12_OVER = 0,			// 12�� �̻�
	ST_MATRIX,				// ��Ʈ���� ����
	ST_MATRIX_TOURNAMENT,	// ��Ʈ���� ��ȸ
	ST_ADULT_ONLY,			// ���ο�
// by evilkiki 2009.11.05 ��巩ŷ��
	ST_GUILD_RANK			// ��巩ŷ
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
// ��� ���ϰ� ����
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

	// ����̽� �ɷ� Ȯ�� �Լ�.
	static HRESULT ConfirmDevice( DDCAPS* pddDriverCaps, D3DDEVICEDESC7* pd3dDeviceDesc );
	HRESULT			CheckDeviceCaps();
	BOOL			m_bCanDoMultitexture, m_bCanDoAddSigned, m_bCanDoModulate, m_bCanDoModulate2X;
	BOOL			m_bCanDoAdd;
	BOOL			m_bCanRender32Bit, m_bCanHighQuality;
	D3DSHADEMODE	m_ShadeMode;

	// ī�޶�
	D3DVECTOR		m_vEye, m_vEyeZ, m_vEyeY, m_vAt, m_vUp;
	D3DMATRIX		m_matVirView;
	D3DVALUE		m_Fov, m_Aspect, m_zNear, m_zFar;
	BOOL			m_bWireframe;
	float			m_fAtHeight, m_fCamHeight;

	// ����
	CLand			*m_pLand;
	int				m_idxLandTexture, m_idxLandTexture2;
	BOOL			m_bSelTile;
	CCloudSea		*m_pCloudSea;	// ����
	CSkyPlane		*m_pSkyPlane;	// �׸��ϴ�
	CPath			*m_pPath;
	CFrustum		*m_pFrustum;	// �� ��������
	// ī�޶� ���� �ִ� �� ������Ʈ.
	D3DLVERTEX		m_Origin[8];
	WORD			m_idxOrigin[36];
	// �ѱ� �Է�
	CHanIme			*m_pHanIme;
	BOOL			m_bGameMode;
	HFONT			m_hFont, m_hOldFont;
	// ��� �κ�
	CTcpIpConnection *m_pTcpIpAlter;
	// 2D, 3D Mode ����
	NKGRAPHIC_MODE	 m_GraphicMode;
	// 2D �ΰ�� �������̽�...
	CDMusicMgr		*m_pDMusicMgr;
	CMusicManager	*m_pMusicManager;
	C2DResMgr       *m_p2DResMgr;

public:

	CNkMyPet		*m_pMyPet;		// ���ΰ��� �ֿϵ����̴�.
	CSlotChar		*m_pSlotChar;	// ĳ������ Ư������ ������ �ִ�.

	D3DVECTOR		m_vListener;
	void			PlayMusic(int id = 0);
	void			StopMusic(int id = 0);
	int             GetCurGameMode()
	{
		return m_GraphicMode;
	}

	CNkMob			*m_pNearMob;

	CFxSet			*m_pFxSet;	// Ư�� ȿ��.
	CWeatherMgr		*m_pWeatherMgr;
	CFireFly		*m_pFireFly;
	CMsgPopUp		m_MsgPopUp;	// ����ÿ� ��� �޼��� �˾�
	// ���̴�
	CCompas			*m_pCompas;
	int				m_nCompasX, m_nCompasY;

	BOOL			m_bShowCutton;
	void			RenderCutton();
	void			ShowCutton()
	{
		m_bShowCutton = !m_bShowCutton;
	};
	BOOL			m_bTmp;
	// ��׶��� �ϴ� �׸���.
	void			RenderSky();
	// ���ΰ� �������� ����
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
	// picking buffer�� ���� ����� �� �ִ°��� ���� ���� �׽�Ʈ.
	BOOL		m_bPickView;
	BOOL		m_bPickBegin;
	BOOL		m_bShadowView;	// �׸��� ���ۿ� ������ ����� ������ ������...
	D3DVECTOR	m_vMouse;		// ���콺�� ���� ���� �� ��ǥ
	HRESULT		Render2DEnvironment(HWND hwnd);
	HRESULT		Render3DEnvironment();
	void		RestoreVBs();

public:

	CUIMgr				*m_pUIMgr;	// ���� �������̽� �Ŵ���.
	CLightCtrl			*m_pLightCtrl;	// ������ ����Ʈ ��Ʈ��.

	// ����� �׽�Ʈ�� ������ ���� �ؽ�ó Ŭ����
	// nate 2004 - 7 m_pTestTexture����
	//	IndexedTexture		*m_pTestTexture;
	// ĳ���Ϳ� ��� �ؽ�ó.
	//IndexedTexture		*m_pChaTexture;
	// Shadow Test
	// nate 2004 - 7 �۷ι� ShadowRect���
	//	CShadowRect			*m_pShadowRect;
	// nate 2004 - 7 �۷ι� ShadowRect���
	//	CShadowRect *		GetShadowRectMgr() { return m_pShadowRect; };

	IndexedTexture	*m_pDecoTexture;

	BOOL			m_bDrawShadow;

	CGoodShadow		*m_pGoodShadow;	// ���� �׸���...
	CMouse			*m_pMouse;	// �ִϸ��̼� ���콺 ������.
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

	// ī�޶� ���� ȸ���� �̵�.
	BOOL m_bRotateCamera;
	POINT m_ptCurPos;
	void GoForward();
	void GoBackward();
	void GoRight(float right);	// [11/28/2007 �����] ī�޶� eye���� �̵�
	void GoLeft(float left);	// [11/28/2007 �����] ī�޶� eye���� �̵�
	void StartRotateCamera(int mx, int my);
	void EndRotateCamera();
	void RotateCamera(POINT ptOld, int mx, int my);
	void RotateCamera();
	void RotateCameraPI(POINT ptOld, int mx, int my); //mungmae-2006/07/04 ī�޶� 180ȸ���� ���� �Լ�.
	void RotateCameraPI();
	void RotateRight();
	void RotateLeft();
	void ForwardCamDirection();
	void BackwardCamDirection();
	void SetFov(float fov)
	{
		m_Fov = fov;
	};
	// ������ ���
	BOOL m_bSatelliteMode;
	void ToggleSatelliteMode();

	// ī�޶� �� �÷��̾�.
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
	void SmartExit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // ����ÿ� ȣ��Ǵ� �Լ�

	// �ð��� ��ȭ�� ���� ���� �׸��� ��Ʈ��.
	int		m_nCurHour;				// ������.. ���� �ð��� �˷���.
	void	ChangeHour(int hour = -1);			// ������ �׽�Ʈ �Լ�.

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
	// �̵����� �Լ�.
	void FindHeroPath(float &tx, float &tz, BOOL bWaypointMove = FALSE);
	BOOL IsBlockedPath(float wx, float wz, float tx, float tz);
	BOOL MoveHero(float x, float z, BOOL bWaypointMove = FALSE);
	void HeroMoving();
	void StopHero(BOOL bMsg = TRUE);
	void AvoidObj(float x, float z);
	void DrawPlayer();
	float m_fPointRColor, m_fPointGColor, m_fPointBColor;

	// Ÿ�ϴ��� �̵������Լ�.
	void FindHeroPathT(float &tx, float &tz, BOOL bWaypointMove = FALSE);
	void FindHeroPathTN(float &tx, float &tz, BOOL bWaypointMove = FALSE);
	BOOL IsBlockedPathT(float wx, float wz, float tx, float tz);
	BOOL MoveHeroT(float x, float z, BOOL bWaypointMove = FALSE);
	void HeroMovingT();

	// �������� �ٰ��ϴ�!!!
	void SetHeroAttGoCha(CNkCharacter *pCha);
	void SetHeroAttGoMob(CNkMob *pMob);
	void SetHeroCurSkill(int nSkill);

	CNkCharacter *m_pHeroAttGoCha;
	CNkMob *m_pHeroAttGoMob;

	BOOL	CompareHeroAttGo();
	int		GetSkillDistance(int li_selskill);
	BOOL	AttackMsgSend(int cur_skill);
	bool	SkillConditionCheck(int cur_skill);
	int		GetTargetCountOfSkill( int nCurSkill );	// nate 2005-12-15 : ��ų�� Ÿ�� ����
	int		m_nCurSkill;					// ī�̸��� �ڵ� ������ ���� ��ų.


	bool CheckSkillCondition(int cur_skilll);


	// ���� ȿ��.
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

	HRESULT ProcessNextFrame(HWND hwnd);							// 2D, 3D Rendering ȣ�� ��ƾ.
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
					 LPARAM lParam );

	void			InitVariables();				// ���ӿ� ����� ������ �ʱ�ȭ
	BOOL			LoadResources(int which = 4, bool force3d = false);				// �ʰ� ������Ʈ�� ���� ���ҽ��� �ε�


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

	// ���� ���ҽ� �ε�
	void			StartGame();	
	BOOL			LoadGameResources();			// ���� �����߿� �ʿ��� ���ҽ� �ε�
	HRESULT			AdjustViewport();
	void			InitTimeCtrl();

	// ���α׷� �ɼ�.
	PROGRAM_OPTION	m_ProOption;
	void			ReadConfigFile(char *file_name);
	void			WriteConfigFile(char *file_name);


	void			ExcuteOption();
	void			ExcuteFFullOption();			// Fake Full-Mode Option
	void			ExcuteWindowedOption();			// â��� ����.

	// ����̽� üũ.
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


	// ��� �ڷ�..
	DWORD			m_dwDecoTime, m_dwLandTime, m_dwRohTime, m_dwPlayerTime, m_dwMobTime, m_dwItemTime, m_dwWeatherTime;
	DWORD			m_dwRenderRohTime;
	DWORD			m_dwPlayingTime; // ���ӽð� ����� ���� ���ƽ.
	DWORD			m_dwOldPlayingTime; // ���ӽð� ����� ���� ���ƽ.

	void	Read_ChooseMode(char *);  // choose_mode.txt ������ �а� ������"0"�̸� âȭ��, "1"�̸� ��ȭ����� ��ȭ��Ų��

	// ����Ÿ �ε��� ȭ�� ���÷���.
	CDisplay *m_pLoadDisp;
	void	InitScreen();
	void	InitLoadingScr();
	void	InitProgress(int nFrame);
	void	StartGameScr(int nBmpNum);
	// �ε��� Progress ��...
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

	// �ε� �� �Լ�.
	void			OpenNewMap();
	// nate 2004 - 7 m_pTestTexture����
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
	
	int				m_nCurWorldIdx;							// ���� ���� �ִ� ����.
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
	// �ڵ� �α��� ���ؼ�.
	void			SetLoginInfo(int nSvrIdx, char *strUserId, char *strUserPw, char *strUserKey);
	void			AutoLogin();

	BOOL			m_bTest;
	D3DTLVERTEX		m_pickMesh[4];

	// ���ο� ��ŷ ����.
	LPDIRECTDRAWSURFACE7	m_pddsPickRender;
	LPDIRECTDRAWSURFACE7	m_pddsPickZBuffer;

	// Fog ����.
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
	// ĳ�� �ؿ� ���¹� ...
	int m_nShowRohEnerge;
	void ShowRohEnergeBar();
	// ��Ƽ�� ���¹� Draw
	int	 DisplayPartyVital(CNkCharacter*, float);
	//int  DisplayPlayerVital(CNkCharacter*, float); //desativado
	// Summon ���¹�
	int  DisplaySummonVital(CNkMob *pMob);
	//����� ������ ���¹�
	int  DisplayDivineVital(CNkMob *pMob);
	int	 ShowMyPartysVital();

	// vital Bar �� ǥ���Ѵ�.
	void RenderOthersVital(int, int, int, int, int, int, int, int,int);
	// Hitpoints
	void DisplayChaHitpoints(CNkCharacter *pCha);
	void DisplayMobHitpoints(CNkMob *pMob);
	void RenderHitPoints(int, int, DWORD, DWORD, DWORD = 2000);
	// ���ΰ��� �ٰ� �ִ��� �ƴ���...
	BOOL m_bRohRunning;
	DWORD m_startRunningTime;
	// ��ų���忡 ���� ���� / �Լ�....
	char    m_sCheckStr[10];
	char	m_SaveChaName[30];
	int		m_SaveCha_Rskill;
	int		m_SaveCha_Lskill;
	void	WriteLastSkill();  // ������ ���õ� ��ų�� �����Ѵ�
	void    ReadAndAppSkill();
	void    SetDefaultVal();
	// �ϴ� + ���� ������������..
	D3DTLVERTEX  TLVec[4];  // �ϴ� ��� �簢��
	D3DTLVERTEX  TLVec2[4]; // �帣�� ���� �簢��
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


	// ī�޶� ȸ������ ���� �ϴ� �ؽ��� ȸ�� ���� ..
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

	// ������� ������
	DWORD	m_lEquipTime;
	BOOL	CanUseLEquip();

	//------------------------------------------------------------------------
	// Lock : ������ ������ �� ��� ���� �Ǿ�� �ϴ� ��� ���� �� ������ �̵� ����.
	BOOL	m_bEquipLock; // ������� lock
	BOOL	m_bRingLock;  // ���� lock
	BOOL  m_bBrLock;    // ���� Lock (2004.10.18/Lyul����)
	BOOL  m_bNecklaceLock;    // ����� Lock (06-03-29 ����) ///��


	///1118  ���丶�콺 ��������...
	DWORD	m_lClickTime;
	BOOL	CanClick();

	BOOL	m_bSelectCha;
	BOOL	m_bInitEnergyBar; //÷�� �׷��ִ� ������������...

	BOOL	m_bInverseReturn;//0124 +������ �����ų� �ȶ� �ߴ� �˾�â���� ����Ű�� Ok�� �ƴ϶� ��ҷ� �ϱ�����...

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

	int m_IsPcBang_User; // [2008/11/10 Theodoric] �������� ���� Ÿ���� ���� �Ѵ�. ( 0:�Ϲ� ���� / 1:PC�� / 2:PREMIUM )
	DWORD m_PcBangStartTime; // [2008/5/29 Theodoric] Pc�� 30�� äũ

	void PcBangUserDraw();

	void DashHero(float dx, float dz); // Lyul_2005
	//wan_mat ��Ʈ������ ��ġ ����
	int	PlaceInfo(float x, float z);
	CPlaceInfo*	m_PlaceInfo;
	void CreatePlaceInfo(int texNum);
	void DeletePlaceInfo();
	BOOL DrawPlaceInfo;
	int m_nCurPlaceNum;

	//mungmae-2005/06/23 ����콺 ������ ��ŸƮ
	CFxRegenStart*	m_pRegenStart;
	BOOL		    m_bRegenStart;

	//mungmae-2005/07/13 ī�̸��� ��ٿ� ���� �߰�
	CFxCasting*	m_pCasting;
	BOOL    DrawCasting;
	DWORD   m_dPressTime;
	DWORD   m_dwCastingTime;
	BOOL    m_bHellDownReady;
	BOOL    m_bHellDownSec;			//��ٿ� �ι�° ����Ʈ
	BOOL	m_bHellDown_Cooltime;
	int		m_nGrade;				//�ؽ��� ��ȣ
	int		m_nHellDown_Grade;
	int		m_nCurHellDown_Grade;
	void	Goworld_CharInit();		// nate 2005-07-28 : �� �̵��� ��ĳ�� ���� �ʱ�ȭ
	int		IsAcropolis(float fX = 0.0f, float fZ = 0.0f);	// nate 2006-01-16 : ������ ���� üũ
	int		IsSoccerZone(float fX = 0.0f, float fZ = 0.0f);	// nate 2006-05-15 : ��Ʈ���� ������ �����

	int     m_RotateFrame;
	int     m_ZoomInFrame;
	BOOL    m_SpiritRotateCamera;
	BOOL    m_bSpiritZoomIn;

	void    SpiritRotateCamera();
	void    SpiritRotateCamera(POINT ptOld, int mx, int my);
	void    SpiritZoomIn();

	DWORD m_NvsnMoveDelay; // �����̵��� ������  ///�������̵�������

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

	// �������̵������� �����ִ� �������� ��ȯ���� ���ϵ��� ��ǥ ����
	BOOL IsBossradePeaceArea(CNkCharacter *pNkCha = NULL, float wx = 0.0f, float wz = 0.0f);

	float	m_fMoveValue;		// �ϵ� �̵��ӵ� ��� ������ ������ ����

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