#ifndef _CONTROL_INVEN_H_
#define _CONTROL_INVEN_H_

#include "Config.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "BaseInterface.h"
#include "AlphaPopup.h"
#include "TextOutBox.h"
#include "msgpopup.h"


class CSurface;
class CTcpIpConnection;
class CItemRender;
class CNkCharacter;
//class CTextOutBox;
class CMsgPopUp;
class CTextOutBox;

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

//#define INVEN_WIDTH	(273 * m_ScreenWidth / BASE_X_SIZE)
//#define INVEN_HEIGHT	(388 * m_ScreenHeight / BASE_Y_SIZE)

#define INVEN_WIDTH		(273)// * m_ScreenWidth / BASE_X_SIZE)
#define INVEN_HEIGHT	(388)// * m_ScreenHeight / BASE_Y_SIZE)
#define INVEN_START_X	(366)// * m_ScreenWidth / BASE_X_SIZE)
#define INVEN_START_Y	(29)// * m_ScreenHeight / BASE_Y_SIZE)

#define ABLE_WIDTH		(81)// * m_ScreenWidth / BASE_X_SIZE)
#define ABLE_HEIGHT		(108)// * m_ScreenHeight / BASE_Y_SIZE)

#define SLOT_WIDTH		(27)// * m_ScreenWidth / BASE_X_SIZE)
#define SLOT_HEIGHT		(27)// * m_ScreenHeight / BASE_Y_SIZE)

#define SLOT_START_X	(382)// * m_ScreenWidth / BASE_X_SIZE)//((BASE_X_SIZE - INVEN_WIDTH) + 13)
#define SLOT_START_Y	(222)// * m_ScreenHeight / BASE_Y_SIZE)

#define PLAY_AREA_X		(367)// * m_ScreenWidth / BASE_X_SIZE)
#define PLAY_AREA_Y		(394)// * m_ScreenHeight / BASE_Y_SIZE)

/*#define _SLOT_X
#define _SLOT_Y
#define _SLOT_W
#define _SLOT_H*/

#define INVEN_EQUIP_X	INVEN_START_X
#define INVEN_EQUIP_Y	INVEN_START_Y
#define INVEN_EQUIP_WIDTH	INVEN_WIDTH
#define INVEN_EQUIP_HEIGHT	(193 * m_ScreenHeight / BASE_Y_SIZE)

// ����
#define HELMET_SLOT_X	(536)// m_ScreenWidth / BASE_X_SIZE)
#define HELMET_SLOT_Y	(82)// * m_ScreenHeight / BASE_Y_SIZE)
#define HELMET_SLOT_W	(54)// * m_ScreenWidth / BASE_X_SIZE)
#define HELMET_SLOT_H	(54)// * m_ScreenHeight / BASE_Y_SIZE)

// ����
#define ARMOR_SLOT_X	(479)// * m_ScreenWidth / BASE_X_SIZE)
#define ARMOR_SLOT_Y	(55)// * m_ScreenHeight / BASE_Y_SIZE)
#define ARMOR_SLOT_W	(54 )//* m_ScreenWidth / BASE_X_SIZE)
#define ARMOR_SLOT_H	(54 )//* m_ScreenHeight / BASE_Y_SIZE)
 
// ����
#define PANTS_SLOT_X	ARMOR_SLOT_X
#define PANTS_SLOT_Y	(121)// * m_ScreenHeight / BASE_Y_SIZE)
#define PANTS_SLOT_W	54
#define PANTS_SLOT_H	54 // (80 )//* m_ScreenHeight / BASE_Y_SIZE)
 
// ����
#define CLOAK_SLOT_X	(567)// * m_ScreenWidth / BASE_X_SIZE)
#define CLOAK_SLOT_Y	(54)// * m_ScreenHeight / BASE_Y_SIZE)
#define CLOAK_SLOT_W	54
#define CLOAK_SLOT_H	54 // 81
 
// �Ź�
#define SHOES_SLOT_X	(388)//* m_ScreenWidth / BASE_X_SIZE)
#define SHOES_SLOT_Y	(147)// * m_ScreenHeight / BASE_Y_SIZE)
#define SHOES_SLOT_W	54
#define SHOES_SLOT_H	54
 
// ����
#define SHIELD_SLOT_X	567
#define SHIELD_SLOT_Y	147
#define SHIELD_SLOT_W	54
#define SHIELD_SLOT_H	54
 
// �����
#define NECKLACE_SLOT_X	(446)// * m_ScreenWidth / BASE_X_SIZE)
#define NECKLACE_SLOT_Y	82
#define NECKLACE_SLOT_W	26
#define NECKLACE_SLOT_H	26
 
// ����
#define RING_SLOT_X		(542)// * m_ScreenWidth / BASE_X_SIZE)
#define RING_SLOT_Y		SHOES_SLOT_Y
#define RING_SLOT_W		26
#define RING_SLOT_H		26
 
// �尩
#define GLOVES_SLOT_X	NECKLACE_SLOT_X
#define GLOVES_SLOT_Y	SHOES_SLOT_Y
#define GLOVES_SLOT_W	54
#define GLOVES_SLOT_H	54
 
// ����
#define WEAPON_SLOT_X	388
#define WEAPON_SLOT_Y	54
#define WEAPON_SLOT_W	54
#define WEAPON_SLOT_H	54 // 81
 
#define INBEN_CLOSE_BTN_X		(INVEN_START_X+166)// * m_ScreenWidth / BASE_X_SIZE)
#define INBEN_CLOSE_BTN_Y		(INVEN_START_Y+357)// * m_ScreenHeight / BASE_Y_SIZE)
 
#define PET_SLOT_W		26
#define PET_SLOT_H		26
 
 
 
 
 
#define INVEN_WEIGHT_X	(563)// * m_ScreenWidth / BASE_X_SIZE)
#define INVEN_WEIGHT_Y	(75)// * m_ScreenHeight / BASE_X_SIZE)
 
#define ITEM_INFO_INVEN		0
#define ITEM_INFO_SHOP		1
#define ITEM_INFO_BOTTOM	2
#define ITEM_INFO_EXCHANGE	3
#define ITEM_INFO_INTERCHANGE 4
#define ITEM_INFO_WAREHOUSE 5
#define ITEM_INFO_PIECESHOP 6
#define ITEM_INFO_PRODUCT   7
#define	ITEM_INFO_PETPRODUCT 8
#define ITEM_INFO_USERSHOP 9 ///���λ���
#define ITEM_INFO_CASHSHOP 10 ///������� Info��
 
 
 
#define IDS_CTR_INVEN_CNT   29
#define NUM_BTN_MASUK		14
 
#define SPECIAL2_MASK_DEF_REVISION		0x0000000F // ���� 4��Ʈ�� ������ ���� ����ũ. ���� ����ġ�� ��ġ �κ�. ///�Ź���ȭ�ý���
#define SPECIAL2_MASK_DEF_REVISION_SIGN 0x00000010 // ���� 5��Ʈ °�� ������ ���� ����ũ. ���� ����ġ�� ��ȣ �κ�. ///�Ź���ȭ�ý���
 
#define SPECIAL2_MASK_LEV_REVISION		0x000001E0 // �������� ����ġ�� ��ġ �κ�.
#define SPECIAL2_MASK_LEV_REVISION_SIGN 0x00000200 // �������� ����ġ�� ��ȣ �κ�.
 
class CAlphaPopUp;
 
class CEquipSlot
{
public:
int m_X;
int m_Y;
int m_W;
int m_H;
};
 
class CControlInven
{
 
public:
HWND	m_hWnd;
RECT	m_ClientRc;
 
CControlInven();
virtual ~CControlInven();
 
void Init();
void LoadRes();
void DeleteRes();
CItem* Draw();
void DrawItemInfo(CItem *pOverItem, int where_item);
void AddSetItemString(const std::vector< std::pair<int, int> >& vecStatus, CMultiText& multiText);
void DrawExtra();
void ResetItemInfo() 
{ 
m_pItemInfo->SetIndex(-1); 
}
void SetNkCha(CNkCharacter *pNkCha);
BOOL IsInside(int x, int y);
BOOL IsInsideEquip(int x, int y);
int	 WhichWearing(int x, int y);
BOOL IsPlayArea(int x, int y, BOOL bEnabled);
void SendMsg();
void SetEquipArea(int);
LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL bEnabled, BOOL bWindowOpened );
 
 
HRESULT RestoreSurfaces();
 
CSurface *m_pSurfaceBack;
CSurface *m_pEnableSlot;
CSurface *m_pDisableSlot;
CSurface *m_pExistSlot;
CSurface *m_pImgGem;
CSurface *m_pImgRecycle;
CNkCharacter *m_pNkCha;
CEquipSlot m_EquipSlot[WEARING_NUM];
 
//CBasicButton m_SortBtn;
CBasicButton m_CloseBtn;
CBasicButton m_InvenBtn1;
CBasicButton m_InvenBtn2;
CBasicButton m_InvenBtn3;
CBasicButton m_InvenBtn4;
 
CTextOutBox  m_MoneyText;
__int64		 m_iOldMoney; ///�ִ�ݾ� �ø���
 
char m_MsgBuf[256];
 
int m_ScreenWidth;
int m_ScreenHeight;
 
CAlphaPopUp *m_pItemInfo;
 
//----���ο� ������
int m_slotstart_x, m_slotstart_y;
 
BOOL m_bInvenLock;	// nate : �κ� ��� ���ൿ�� ���� �ɾ���� �÷���
BOOL GetInvenLockFlag()	
{ 
return m_bInvenLock; 
}
 
DWORD m_timeInvenLock;
void SetInvenLock(BOOL bLock);
 
DWORD m_timePickExtraTime;			// 1�� �κ��� �� á���� extra�� ������ �������� �ʵ��� �ϴ� �ð�.
void SetPickExtraTime();				//
 
// ���� ���׷��̵�� +5 ����� ������ ��� �޼���
CMsgPopUp  m_plus5Message;
int		   m_iCurWhere;
 
//-----------------------------------------------------------------
// �������׷��̵� ���� 
//-----------------------------------------------------------------
 
D3DTLVERTEX		  TLVec[4];
CSurface*			m_pBack_Masuk;
CBasicButton		m_Btn_Masuk[NUM_BTN_MASUK];
/*
0 �ּҵ����� ���
1 �ִ뵥���� ���
2 ����Ÿ20%  ���
3 ���ⵥ����% ���
4 ���ⵥ���� ���
5 ���������� ���
6 ġ��Ÿ50% ���
7 ����������%���
8 �������� ���
12 ���鸮��Ʈ% ���
13 ���ⵥ���� ���
*/

int	 m_x, m_y;
int  m_cx, m_cy;
BOOL m_bAct_Masuk;
BOOL m_bShowBtnAry[NUM_BTN_MASUK];
BOOL m_bDam_min, m_bDam_max, m_bDam_all, m_bDam_per, m_bHit_1, m_bHit_2, m_bDef_melee, m_bResist, m_bDef_all, m_bDef_per,mb_ignore_avo;
BOOL m_bDam_all2, m_bDeadHit;
int  m_iCurItemLevel;
int  m_bDropEngBall;
int  m_iMobControl_vnum, m_iMobControl_idx;

void	Load_Masuk();
void	Draw_Masuk();
void	CalcPos_Masuk(int x, int y);
void    Del_Masuk();
void	RenderOj();
void	RenderOj( RECT Rect, DWORD color ); ///�̺�Ʈ�������� �� �߰�.
void	Divergence(int where, int vnum, int level, int x, int y);
void	Close_Masuk();
void	Action_Masuk(int type);
int     GetCurItemLevel()
{
	return m_iCurItemLevel;
}
BOOL    IsInside_Masuk_Back(int x, int y);
BOOL	IsInside_Recycle(int x, int y);
BOOL	Drop_EnergyBall(bool, int, int); // ������ ��  ����..

BYTE	m_byPopupType;
BOOL    m_bDrawInfoUpdate; // �ð��� �������� �������� ������ ���� ����� ���� ������Ʈ �÷��׸� ���� ��. m_bDrawInfoUpdate ���õǸ� ������ ������ �ٽ� �׸���.(by ����) ///�ð���������
HRESULT WarningPopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
std::string m_strStashPacketTemp;

///�����Ǹ� �� ���Ǵ� ���� �� �Լ�
char m_MultiSaleString[256]; // �߰��� ��������ϱ⶧���� ��Ŷ ��Ʈ�� �����

//////////////////////////////////////////////////
///�̺�Ʈ��������
CSurface *m_pSurfaceBackEvWear; ///�̺�Ʈ�������� ���� ���ÿ� �����ִ� ���.
CSurface *m_pWarringEvWear; ///�̺�Ʈ�������� ���� ���ÿ� ��� ������ �ɷ������� ��µǴ� ����.
CSurface *m_pEvWearTimeBack; ///�̺�Ʈ�������� �� ��밡�� �ð����� ���.
BOOL m_bEventWearView; ///�̺�Ʈ�������� ���� �����ִ����ΰ� ����. �̰� ��� Ȱ��ȭ�ʹ� �������.
CTextOutBox		*m_EventWearTimeText; // tlrks cnffurdyd.
int m_EventWearTime; ///�̺�Ʈ�������� �� ����� �ð����� �ð���.(ƽ������ ���� DWORD�� �ؾ߰����� -1�̸� ���������� ������ int�� ��. ������ ��û ���� �ð��밡 �ȿ��״ϱ�...)
DWORD m_EventWearTimeUpdateTick; // �ð� �ǽð� ���� �� ƽ.
CBasicButton m_btnNormalEquip; ///�̺�Ʈ�������� ���� ��ư��
CBasicButton m_btnEventEquip; ///�̺�Ʈ�������� ���� ��ư��
//////////////////////////////////////////////////

// [2008/3/19 Theodoric] ������ ������ ��� �Լ�
int  GetEnduranceCharge(CItem* pItem);

CSurface *m_pSurEventWearFlagON;
CSurface *m_pSurEventWearFlagOFF;

BOOL IsInsideEventWearFlag(int x, int y);
void ToggleEventWearFlag();

DWORD	m_dwMouseClickTickTime;


void SetItemInfo_Tribe(CItem * pItem , CMultiText &multiText);
void SetSpecial_SkillDoc(CItem * pItem , CMultiText &multiText);

private:
BOOL procEventEnchant(int where);
BOOL procEventEnchant_old(int where);
BOOL checkEventEnchantWear(int where);
BOOL procEventEnchant_2015(int where);

bool check_disable_normal();
};

#endif // _CONTROL_INVEN_H_


