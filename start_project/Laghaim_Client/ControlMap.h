#ifndef _CONTROL_MAP_H_
#define _CONTROL_MAP_H_

#include "ControlMap.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "BaseInterface.h"
#include "alphapopup.h" ///미니맵길드원찾기

class CSurface;
class CNkCharacter;

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

#define MAP_WIDTH		(274)// * m_ScreenWidth / BASE_X_SIZE)
#define MAP_HEIGHT		(391)// * m_ScreenHeight / BASE_Y_SIZE)
#define MAP_START_X		(366)// * m_ScreenWidth / BASE_X_SIZE)
#define MAP_START_Y		(29)// * m_ScreenHeight / BASE_Y_SIZE)

#define MINI_MAP_WIDTH	(242)//(256) //* m_ScreenWidth / BASE_X_SIZE)
#define MINI_MAP_HEIGHT	(256) //* m_ScreenHeight / BASE_Y_SIZE)
#define MINI_MAP_POS_X	(MAP_START_X+16)// * m_ScreenWidth / BASE_X_SIZE)
#define MINI_MAP_POS_Y	(MAP_START_Y+58)// * m_ScreenHeight / BASE_Y_SIZE)
 
#define POS_POINT_WIDTH	(16 )//* m_ScreenWidth / BASE_X_SIZE)
#define POS_POINT_HEIGHT (16)// * m_ScreenHeight / BASE_Y_SIZE)
 
#define MAP_CLOSE_BTN_X		(MAP_START_X+166)// * m_ScreenWidth / BASE_X_SIZE)
#define MAP_CLOSE_BTN_Y		(MAP_START_Y+357)// * m_ScreenHeight / BASE_Y_SIZE)
//#define CLOSE_BTN_W		(24 * m_ScreenWidth / BASE_X_SIZE)
//#define CLOSE_BTN_H		(40 * m_ScreenHeight / BASE_Y_SIZE)
 
#define NEW_DG_MAP_WIDTH    200
#define NEW_DG_MAP_HEIGHT   200
 
// 플레이어 좌표.
#define PLAYERPOS_FONTSIZE 12
#define PLAYERPOS_FONTCOLOR RGB(255, 255, 0)
 
//================================
// nate 2004 - 9
// GuildHunt
#define GUILDHUNT_SHILON		1
#define GUILDHUNT_WHITEHORN		2
//================================
 
#define VILLAGE_NUM		 3
#define VILLAGE_BTN_NUM  VILLAGE_NUM
 
class C3d_PosPoint;
 
/////////////////////////////////////////////////////////////////////////////
// 미니맵 길드원 찾기 기능 (05-10-18 원석)  ///미니맵길드원찾기
#define MAX_DRAW_GUILD_POS_NAME 10 // 동시에 길드원 이름을 표시해주는 한계수.(실제론 9명까지만 표시. 10번째는 ...을 표시해주기 위해 사용)
 
struct sGuildPosInfo
{
int m_Grade; // 길드 등급. 이걸로 길마인지등을 구분할수 있다.
char m_strName[30]; // 캐릭명.
int m_PosX; // 위치.
int m_PosY; // 위치.
};
/////////////////////////////////////////////////////////////////////////////
 
class CControlMap : public CBaseInterface
{
public:
int m_iDmitronPopup; // 드미트론 점령전 - 점령 현황
RECT m_rcDmitronPopup; // 드미트론 점령전 - 점령 현황
CTextOutMultiBox m_DmitronPopupText; // 드미트론 점령전 - 점령 현황
 
HWND	  m_hWnd;
RECT	  m_ClientRc;
 
C3d_PosPoint*	m_pPosPoint; // 
C3d_PosPoint*	m_pFlag_A; // 
C3d_PosPoint*	m_pFlag_B; // 
C3d_PosPoint*	m_pMyPartys; // 
//==========================
// nate 2005-02-28
// Matrix : Indication
C3d_PosPoint*	m_pIndication_Point;
BYTE			m_bIndication_Point;
DWORD			m_dwTimeIndication;
int				m_nCurClick_X;
int				m_nCurClick_Y;
//==========================
 
CSurface *m_pBack;
CSurface *m_pDeckardi;
CSurface *m_pLagramia;
//CSurface *m_pDeckadum;
CSurface *m_pWhiteHorn;
 
CSurface *m_pDeckadum_1;
CSurface *m_pDeckadum_2;
CSurface *m_pDeckadum_3;
CSurface *m_pDeckadum_4;
 
CSurface *m_pDeckaren_0; ///인던1
CSurface *m_pDeckaren_1;
CSurface *m_pDeckaren_2;
 
CSurface *m_pInDoor;
CSurface *m_pOutDoor;
CSurface *m_pGWar;
CSurface *m_pLagramia_detail;
CSurface* m_pTutorialIsland;	///튜토리얼 지역. 라그라미아의 숨겨진 섬.
CSurface *m_pShailon_detail;
CSurface *m_pLost_detail;
CSurface *m_pSeaRoost_first; //1227 
CSurface *m_pSeaRoost_second;
CSurface *m_pSeaRoost_third;
CSurface *m_pSeaRoost_forth;
CSurface *m_pParos;
CSurface *m_pShilon;
// nate 2004 - 9 : GuildHunt
CSurface* m_pShilonGuildHuntMap;	// 샤일론 길드사냥
CSurface* m_pWhiteHornGuildhuntMap;	// 화이트혼 길드사냥
// wan:2004-11 : Whitehorn Dungeon
CSurface* m_pWhiteHornDungeon;
// nate 2005-03-21 : Matrix
CSurface* m_pMatrix;	// 메틀릭스 미니맵
CSurface* m_pDmitron;	// nate 2005-07-12 : Dmitron
	
CSurface* m_pBossRaidBulkan;	// 불칸
CSurface* m_pBossRaidKailipton;	// 카이립톤
CSurface* m_pBossRaidAidia;		// 에이디아
CSurface* m_pBossRaidHuman;		// 휴먼
CSurface* m_pBossRaidHybrider;	// 불칸
CSurface* m_pBossRaidFinalboss;	// 최종 보스
CSurface* m_pBossRaidFinalboss_Room;	// 최종 보스 방
 
//========================================================================
// nate 2005-03-31
// 메트릭스 주둔지에 길드 마크 출력
CSurface* m_pHawklaw;	// hawklaw
CSurface* m_pCoolrain;	// coolrain
CSurface* m_pShiver;	// shiver
// nate 2005-10-24 : 지휘자 위치 표시
CSurface* m_pArrow;
int m_nAniframe;
DWORD	m_dwAniTime;
// nate 2005-04-18
// 게이트가 깨졌을때
CSurface* m_pBrokenGate;
int m_nArmyPostX[ 3 ];
int m_nArmyPostY[ 3 ];
int m_nBrokenGatePosX[ 6 ];
int m_nBrokenGatePosY[ 6 ];
	
CSurface* m_pMoebius;
CSurface* m_pTundmar;	// 툰드마르
CSurface* m_pAngmar;	// 안그마르
CSurface* m_pOpenGate;	// 가드게이트 열림 표시	
CMultiText	m_MatrixStoneInfoMulti;
CAlphaPopUp m_MatrixStoneInfoPopup;
 
int	GetInsideStone(int x, int y);
void DrawMatrixStoneInfo();
 
CSurface* m_pDisposal;

CSurface* m_pVidBline; 
 
CSurface* m_zone_22;
CSurface* m_zone_23;
CSurface* m_zone_24;
CSurface* m_Hightland;
CSurface* m_Tritian;
CSurface* m_Space;
CSurface* m_Royale;
CSurface* m_dun910;
CSurface* m_zone930;
 
//========================================================================
 
/////////////////////////////////////////////////////////////////////////////
// 미니맵 길드원 찾기 기능 (05-10-18 원석)  ///미니맵길드원찾기
void ShowGuildMember(); // 길드원 포인트 출력기능
POINT GetWorldToMiniMap( int x, int y ); // 월드 좌표로 부터 미니맵에 표시될 위치를 구해온다. ///미니맵길드원찾기
 
CBasicButton m_PlayerFindBtn;
CBasicButton m_GuildFindBtn; // 길드원 찾기 버튼.
DWORD GuildFindTick; // 길드원 찾기 버튼을 누른 틱을 기억.(10초간 표시와 버튼 연타를 막는데 사용.)
vector<sGuildPosInfo> m_GuildPosVector; // 길드원들 정보 벡터.
 
C3d_PosPoint*	m_pMyGuilds; // 그려주기위한 객체
int m_DrawGuildNameNum; // 길원들 이름을 그려야하는갯수용 변수. 이게 0이면 안그려줘도 되는거다.
CMultiText m_GuildPosMultiText;
CAlphaPopUp m_GuildPosNameBox;
POINT m_DrawGuildNamePos; // 그릴 위치 기억.
/////////////////////////////////////////////////////////////////////////////
	
 
CBasicButton  m_GWarBtn;
CBasicButton m_CloseBtn;
CTextOutBox	 m_PosBox;				 // 플레이어 좌표
CTextOutBox	 m_LocBox;
 
int m_ScreenWidth;
int m_ScreenHeight;
int m_curpostext_x, m_curpostext_y;
int m_minimap_x, m_minimap_y;
int m_iChaRoomPosX, m_iChaRoomPosZ;
	
int m_MyPosInVillage_x;
int m_MyPosInVillage_y;
 
int m_iCurWorld;
int m_iCurVillage;
int m_iCurDeckadum;
int m_iSeaRoost;
int m_iCurDeckaLen;
 
BOOL  m_bShowGWarMap;
 
enum	STATE	{ MOUSE_MOVE, MOUSE_CLICK, MOUSE_NONE};
STATE   m_eState;
 
//===================================================================
// nate 2004 - 5
// 크리스탈 소유자 위치 표시
int m_nCristalOwnerX;
int m_nCristalOwnerZ;
//===================================================================
 
public:
//================================
// nate 2004 - 9
// GuildHunt
void Show_PosPoint_InGuildHuntArea( BYTE byZone );
//================================
//================================
// nate 2005-03-02
// Matrix
void Show_PosPoint_Indication();
// nate 2005-03-31
// 메트릭스 주둔지에 길드 마크 출력
void Show_PosPOint_StoneState();
void Show_PosPOint_StoneState_2009();
//================================
CControlMap();
virtual ~CControlMap();
 
void Init();
void LoadRes();
void DeleteRes();
void Draw();
 
void Show_Map(int index);
void Show_PosPoint_InVillage();
void Show_PosPoint_InDeckadum();
void Show_PosPoint_InGWar();
void Show_Pos_Text(int x, int z);
void Show_PosPoint_InSeaRoost();
void Show_PosPoint_Partys();
void Show_Dmitron_Battle_Status(); // 드미트론 점령전 - 점령 현황
void MakeDmitronString(int Zone);  // 드미트론 점령전 - 점령 현황
void CalcPopupIndex(int x, int y);  // 드미트론 점령전 - 점령 현황
void CalPos(RECT rc, int, int, int, int);
void CalPos_Partys(RECT rc, int start_x, int start_y, int width, int height);
void CalPos_For_Flag(RECT rc, int start_x, int start_y, int width, int height);
void Get_Door_Pos(RECT rc, int , int, int, int); // 입구/ 출구 아이콘 출력 위치	
void Get_Door_Pos_SeaRoost(RECT rc, int door_x1, int door_y1, int door_x2, int door_y2);//1227
BOOL GetMapMousePos(int &x, int &y);
 
int  IsInside(int, int, int);
LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 
void RenderLimitSight( int HeroX, int HeroY ); // 자기 주변 미니맵만 보이게 해주는 효과 ///인던1 시 추가
};
 
#endif // _CONTROL_MAP_H_
 