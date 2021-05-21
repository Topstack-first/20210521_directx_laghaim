#ifndef _CONTROL_ROYALE_H_
#define _CONTROL_ROYALE_H_

#include "ControlRoyale.h"
#include "BasicStatic.h"
#include "NkCharacter.h"
#include "BasicButton.h"
#include "BaseInterface.h"
#include "alphapopup.h" ///นฬดฯธสฑๆตๅฟ๘รฃฑโ

class CSurface;
class CNkCharacter;

#define MAP_WIDTH		(274)// * m_ScreenWidth / BASE_X_SIZE)
#define MAP_HEIGHT		(391)// * m_ScreenHeight / BASE_Y_SIZE)
#define MAP_START_X		(366)// * m_ScreenWidth / BASE_X_SIZE)
#define MAP_START_Y		(29)// * m_ScreenHeight / BASE_Y_SIZE)

 
#define POS_POINT_WIDTH	(16 )//* m_ScreenWidth / BASE_X_SIZE)
#define POS_POINT_HEIGHT (16)// * m_ScreenHeight / BASE_Y_SIZE)


class CControlRoyale /*: public CBaseInterface*/
{
public:
CBasicStatic	m_hp;
CControlRoyale();
virtual ~CControlRoyale();
 
void Init();
void LoadRes();
void DeleteRes();
void Draw();
 
void Draw_Bar(CBasicStatic* ui , int base_value, int max_value);
void Draw_Percent( CTextOutBox* textout , __int64 base_value, __int64 max_value);

int  IsInside(int, int, int);
CTextOutBox		m_textout[12];
CTextOutBox		m_bodytext[20];
char rewardbody[100][100];
int curReward;


CBasicButton	m_btnLeft;
CBasicButton	m_btnRight;	


CSurface*	m_Icon[200];
CBasicStatic	m_faceICon;
HWND	  m_hWnd;
RECT	  m_ClientRc;
 
//==========================
 
CSurface *m_pBack;

 
int m_nAniframe;
DWORD	m_dwAniTime;

 
int m_ScreenWidth;
int m_ScreenHeight;
int m_curpostext_x, m_curpostext_y;
int m_minimap_x, m_minimap_y;
int m_iChaRoomPosX, m_iChaRoomPosZ;
	
int m_MyPosInVillage_x;
int m_MyPosInVillage_y;
 
 
enum	STATE	{ MOUSE_MOVE, MOUSE_CLICK, MOUSE_NONE};
STATE   m_eState;
 
BOOL IsInside(int x, int y);

void DrawRewardBodyMessage();

LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 
};
 
#endif // _CONTROL_ROYALE_H_
 