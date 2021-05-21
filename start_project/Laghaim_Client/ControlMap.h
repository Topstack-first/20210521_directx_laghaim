#ifndef _CONTROL_MAP_H_
#define _CONTROL_MAP_H_

#include "ControlMap.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "BaseInterface.h"
#include "alphapopup.h" ///�̴ϸʱ���ã��

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
 
// �÷��̾� ��ǥ.
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
// �̴ϸ� ���� ã�� ��� (05-10-18 ����)  ///�̴ϸʱ���ã��
#define MAX_DRAW_GUILD_POS_NAME 10 // ���ÿ� ���� �̸��� ǥ�����ִ� �Ѱ��.(������ 9������� ǥ��. 10��°�� ...�� ǥ�����ֱ� ���� ���)
 
struct sGuildPosInfo
{
int m_Grade; // ��� ���. �̰ɷ� �渶�������� �����Ҽ� �ִ�.
char m_strName[30]; // ĳ����.
int m_PosX; // ��ġ.
int m_PosY; // ��ġ.
};
/////////////////////////////////////////////////////////////////////////////
 
class CControlMap : public CBaseInterface
{
public:
int m_iDmitronPopup; // ���Ʈ�� ������ - ���� ��Ȳ
RECT m_rcDmitronPopup; // ���Ʈ�� ������ - ���� ��Ȳ
CTextOutMultiBox m_DmitronPopupText; // ���Ʈ�� ������ - ���� ��Ȳ
 
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
 
CSurface *m_pDeckaren_0; ///�δ�1
CSurface *m_pDeckaren_1;
CSurface *m_pDeckaren_2;
 
CSurface *m_pInDoor;
CSurface *m_pOutDoor;
CSurface *m_pGWar;
CSurface *m_pLagramia_detail;
CSurface* m_pTutorialIsland;	///Ʃ�丮�� ����. ��׶�̾��� ������ ��.
CSurface *m_pShailon_detail;
CSurface *m_pLost_detail;
CSurface *m_pSeaRoost_first; //1227 
CSurface *m_pSeaRoost_second;
CSurface *m_pSeaRoost_third;
CSurface *m_pSeaRoost_forth;
CSurface *m_pParos;
CSurface *m_pShilon;
// nate 2004 - 9 : GuildHunt
CSurface* m_pShilonGuildHuntMap;	// ���Ϸ� �����
CSurface* m_pWhiteHornGuildhuntMap;	// ȭ��Ʈȥ �����
// wan:2004-11 : Whitehorn Dungeon
CSurface* m_pWhiteHornDungeon;
// nate 2005-03-21 : Matrix
CSurface* m_pMatrix;	// ��Ʋ���� �̴ϸ�
CSurface* m_pDmitron;	// nate 2005-07-12 : Dmitron
	
CSurface* m_pBossRaidBulkan;	// ��ĭ
CSurface* m_pBossRaidKailipton;	// ī�̸���
CSurface* m_pBossRaidAidia;		// ���̵��
CSurface* m_pBossRaidHuman;		// �޸�
CSurface* m_pBossRaidHybrider;	// ��ĭ
CSurface* m_pBossRaidFinalboss;	// ���� ����
CSurface* m_pBossRaidFinalboss_Room;	// ���� ���� ��
 
//========================================================================
// nate 2005-03-31
// ��Ʈ���� �ֵ����� ��� ��ũ ���
CSurface* m_pHawklaw;	// hawklaw
CSurface* m_pCoolrain;	// coolrain
CSurface* m_pShiver;	// shiver
// nate 2005-10-24 : ������ ��ġ ǥ��
CSurface* m_pArrow;
int m_nAniframe;
DWORD	m_dwAniTime;
// nate 2005-04-18
// ����Ʈ�� ��������
CSurface* m_pBrokenGate;
int m_nArmyPostX[ 3 ];
int m_nArmyPostY[ 3 ];
int m_nBrokenGatePosX[ 6 ];
int m_nBrokenGatePosY[ 6 ];
	
CSurface* m_pMoebius;
CSurface* m_pTundmar;	// ���帶��
CSurface* m_pAngmar;	// �ȱ׸���
CSurface* m_pOpenGate;	// �������Ʈ ���� ǥ��	
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
// �̴ϸ� ���� ã�� ��� (05-10-18 ����)  ///�̴ϸʱ���ã��
void ShowGuildMember(); // ���� ����Ʈ ��±��
POINT GetWorldToMiniMap( int x, int y ); // ���� ��ǥ�� ���� �̴ϸʿ� ǥ�õ� ��ġ�� ���ؿ´�. ///�̴ϸʱ���ã��
 
CBasicButton m_PlayerFindBtn;
CBasicButton m_GuildFindBtn; // ���� ã�� ��ư.
DWORD GuildFindTick; // ���� ã�� ��ư�� ���� ƽ�� ���.(10�ʰ� ǥ�ÿ� ��ư ��Ÿ�� ���µ� ���.)
vector<sGuildPosInfo> m_GuildPosVector; // ������ ���� ����.
 
C3d_PosPoint*	m_pMyGuilds; // �׷��ֱ����� ��ü
int m_DrawGuildNameNum; // ����� �̸��� �׷����ϴ°����� ����. �̰� 0�̸� �ȱ׷��൵ �Ǵ°Ŵ�.
CMultiText m_GuildPosMultiText;
CAlphaPopUp m_GuildPosNameBox;
POINT m_DrawGuildNamePos; // �׸� ��ġ ���.
/////////////////////////////////////////////////////////////////////////////
	
 
CBasicButton  m_GWarBtn;
CBasicButton m_CloseBtn;
CTextOutBox	 m_PosBox;				 // �÷��̾� ��ǥ
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
// ũ����Ż ������ ��ġ ǥ��
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
// ��Ʈ���� �ֵ����� ��� ��ũ ���
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
void Show_Dmitron_Battle_Status(); // ���Ʈ�� ������ - ���� ��Ȳ
void MakeDmitronString(int Zone);  // ���Ʈ�� ������ - ���� ��Ȳ
void CalcPopupIndex(int x, int y);  // ���Ʈ�� ������ - ���� ��Ȳ
void CalPos(RECT rc, int, int, int, int);
void CalPos_Partys(RECT rc, int start_x, int start_y, int width, int height);
void CalPos_For_Flag(RECT rc, int start_x, int start_y, int width, int height);
void Get_Door_Pos(RECT rc, int , int, int, int); // �Ա�/ �ⱸ ������ ��� ��ġ	
void Get_Door_Pos_SeaRoost(RECT rc, int door_x1, int door_y1, int door_x2, int door_y2);//1227
BOOL GetMapMousePos(int &x, int &y);
 
int  IsInside(int, int, int);
LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 
void RenderLimitSight( int HeroX, int HeroY ); // �ڱ� �ֺ� �̴ϸʸ� ���̰� ���ִ� ȿ�� ///�δ�1 �� �߰�
};
 
#endif // _CONTROL_MAP_H_
 