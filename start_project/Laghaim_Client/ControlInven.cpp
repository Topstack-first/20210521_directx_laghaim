#include "stdafx.h"
#include <boost/format.hpp>

#include "headers.h"
#include "math.h"
#include "main.h"
#include "ControlInven.h"
#include "ControlShop.h"
#include "ddutil.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "Nk2DFrame.h"
#include "ControlWareHouse.h"

#include "ControlGuildSHouse.h"

// nate 2004 - 4 : exchange
#include "ControlExchange.h"
#include "tcpipcon.h"
#include "AlphaPopUp.h"
#include "Mouse.h"
#include "msgpopup.h"
#include "SpecialItem.h"
#include "LoadEffectSound.h"
#include "uimgr.h"
#include "Country.h"
#include "ControlBottom.h"
#include "g_stringmanager.h"
// nate 2004 - 7 : Image Manager
#include "ImageDefine.h"
#include "Scroll.h"
#include "Sprite.h" ///�Ź���ȭ�ý���
#include "ControlUpgrade.h" ///�Ź���ȭ�ý���
#include "ControlUserShop.h" ///���λ���
#include "ControlCashShop.h" ///�������
#include "gamble.h"
#include "skill.h"

#include "LHItemSystem_Manager.h"
#include "LHParam_EnchantItem.h"
#include "DefenseValMgr.h"
#include "ItemFakeOptionMgr.h"

#include "CommonConfig.h"
#include "SetItemEffMgr.h"
#include "PacketSend.h"

extern BOOL CheckCanNpcUpgradeItem( CItem *pItem ); // ��ȭ�� ������ ������������ üũ��. ///�Ź���ȭ�ý���
extern BOOL CheckIsConversionItem( CItem *pItem, int type = 0 ,int sub_type=0); // �������Ǿ� ������ ������������ üũ.(�������� Ǯ�� ���ȴ�.)

extern int InsertPriceComma( char *DestString, char *SrcString, char FindChar ); // ���� ��Ʈ������ ���� �ο� �޸��� �־��ִ� �Լ�. ///����ǥ�ð���
extern char *ConvertMoneyToString( char *DestString, __int64 Money, int SpaceSize, int Type = 0 ); // �� �ݾ��� ������ ���� �ٲ��ִ� �Լ�.

extern BOOL CheckIsGodItem(CItem *pItem);

extern int				g_EffectTextr[];

extern BOOL g_bGlobalMatrix; ///���������� �� Ŭ���̾�Ʈ ���� ����.

extern DWORD			g_dwLangType;
extern DWORD			g_dwClientCountry;
extern HINSTANCE		g_hDllInst;
extern BOOL				g_bFreeUser;	// ���� �̺�Ʈ
extern BOOL g_bUseEventWear; ///�̺�Ʈ��������

// ������� ������ ��ȭ �ܰ� ġ�� ���ϴµ� �ʿ��� �迭��... //�������۰��
extern int g_BraceletSpecialMask[SPECIAL_BRACELET_NUM];
extern int g_BraceletSpecialShift[SPECIAL_BRACELET_NUM]; //-- Bit Shift ��


#define  DIV_TYPE_WHERE_ATT    0 // ���ݺκ�
#define  DIV_TYPE_WHERE_DEF    1 // ���κ�

#define  DIV_TYPE_MASUK_NOME   0 // �� ����
#define  DIV_TYPE_MASUK_NIMPH  1 // ���� ����
#define  DIV_TYPE_MASUK_SIL	   2 // �Ƿ��� ����

#define  LEVEL_MAX_JASU			0 // �ڼ���
#define  LEVEL_MAX_TOPHA		1 // ������
#define  LEVEL_MAX_OPAL			2 // ����
#define  LEVEL_MAX_SAPHA		3 // �����̾�
#define  LEVEL_MAX_RUBI			4 // ���
#define  LEVEL_MAX_EME			5 // ���Ӷ���

int plus_percent[10] = {0, 12, 26, 44, 68, 100, 100, 100, 100, 100};
int plus_percent_dia[22] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210};//1118

// ������ ���� �ɼ� ������ % ��ġ ���� ���� ���� �߰� (05-09-23 ����)
extern int g_WeaponSpecialMask[SPECIAL_WEAPON_NUM];
extern int g_WeaponSpecialShift[SPECIAL_WEAPON_NUM];
extern int g_WeaponSpecialValue[SPECIAL_WEAPON_NUM][SPECIAL_LEVEL_NUM];

extern int g_NecklaceSpecialMask[SPECIAL_NECKLACE_NUM];
extern int g_NecklaceSpecialShift[SPECIAL_NECKLACE_NUM];

#define IS_2011_ITEM_ARMOR(vnum) ((2295<=vnum && vnum<=2316) || (2942<=vnum && vnum<=2953) || (3152<=vnum && vnum<=3155) || (3227<=vnum && vnum<=3230) ? TRUE : FALSE )
#define IS_2011_ITEM_WEAPON(vnum) (/*(2743<=vnum && vnum<=2749) || */(2939<=vnum && vnum<=2941) || (3151==vnum) /*|| (3231==vnum)*/ ? TRUE : FALSE )

#define IS_G_LEVEL_ITEM(vnum) ( (2173<=vnum && vnum<=2179) || (2743<=vnum && vnum<=2749)|| (3231==vnum))

#define EVENT_ENCHANT_MAX_PLUS	50

extern BOOL g_bAdmin2;

CControlInven::CControlInven()
	: m_hWnd(NULL)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_slotstart_x(0)
	, m_slotstart_y(0)
	, m_timeInvenLock(0)
	, m_cx(0)
	, m_cy(0)
	, m_bDam_min(FALSE)
	, m_bDam_max(FALSE)
	, m_bDam_all(FALSE)
	, m_bDam_per(FALSE)
	, m_bHit_1(FALSE)
	, m_bHit_2(FALSE)
	, m_bDef_melee(FALSE)
	, m_bResist(FALSE)
	, m_bDam_all2(FALSE)
	, m_bDeadHit(FALSE)
	, m_pImgRecycle(NULL)
{
	memset(m_EquipSlot, 0, sizeof(m_EquipSlot));
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_MultiSaleString, 0, sizeof(m_MultiSaleString));

	m_pSurfaceBack = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot = NULL;
	m_pImgGem = NULL;
	m_pNkCha = NULL;
	m_pItemInfo = new CAlphaPopUp;
	m_bInvenLock = FALSE;
	// ���� ���׷��̵� �߰� �κ�
	m_pBack_Masuk = NULL;
	m_bAct_Masuk  = FALSE;
	m_x = m_y = 0;
	m_iCurItemLevel = -1;

	for(int i=0; i<NUM_BTN_MASUK; i++)
		m_bShowBtnAry[i] = false;

	Init();
	m_iCurWhere = -1;
	m_bDropEngBall = FALSE;
	m_iMobControl_vnum = -1;
	m_iMobControl_idx = -1;
	m_bDrawInfoUpdate = FALSE; // ������ ���� �׸����� ���� ������Ʈ �÷��� ///�ð���������
	m_byPopupType = 0;

	m_pSurfaceBackEvWear = NULL; ///�̺�Ʈ��������
	m_pWarringEvWear = NULL; ///�̺�Ʈ��������
	m_pEvWearTimeBack = NULL; ///�̺�Ʈ��������
	m_EventWearTimeText = NULL; ///�̺�Ʈ��������
	m_bEventWearView = FALSE; ///�̺�Ʈ�������� ���� �����ִ����ΰ� ����. �̰� ��� Ȱ��ȭ�ʹ� �������.
	m_EventWearTime = -1; ///�̺�Ʈ�������� �� ����� �ð����� �ð���. ������ ��츦 ������ �ʱ�ȭ�� ���Ѿ����� -1�� ������.
	m_EventWearTimeUpdateTick = 0;


	m_dwMouseClickTickTime = 0;

	m_pSurEventWearFlagON = NULL;
	m_pSurEventWearFlagOFF = NULL;
}

CControlInven::~CControlInven()
{
	DeleteRes();

	SAFE_DELETE( m_pItemInfo );
	SAFE_DELETE(m_EventWearTimeText);
	SAFE_DELETE(m_pEvWearTimeBack);
	SAFE_DELETE(m_pWarringEvWear);
	SAFE_DELETE(m_pSurfaceBackEvWear);
	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pImgGem);
	SAFE_DELETE(m_pBack_Masuk);
	SAFE_DELETE(m_pImgRecycle);

	m_pNkCha = NULL;
}

void CControlInven::Init()
{
	m_CloseBtn.SetFileName("common/btn_close_01");
	m_InvenBtn1.SetFileName("inventory/btn_vtab_01");
	m_InvenBtn2.SetFileName("inventory/btn_vtab_02");
	m_InvenBtn3.SetFileName("inventory/btn_vtab_03");
	m_InvenBtn4.SetFileName("inventory/btn_vtab_04");
//	m_SortBtn.SetFileName("inventory/btn_sort");


	if( g_bUseEventWear ) ///�̺�Ʈ�������� �ý����� ����Ǿ��ִٸ�...
	{
		m_btnNormalEquip.SetFileName("inventory/btn_tab_default");
		m_btnEventEquip.SetFileName("inventory/btn_tab_event");
	}


	m_MoneyText.Init(12, _FCOLOR_YELLOW, 0, 0);
	m_iOldMoney = -1;
	m_timePickExtraTime = timeGetTime();
}

void CControlInven::SetEquipArea(int add)
{
	m_EquipSlot[0].m_X = m_pSurfaceBack->Xpos+121; // ���
	m_EquipSlot[0].m_Y = m_pSurfaceBack->Ypos+94;
	m_EquipSlot[0].m_W = HELMET_SLOT_W	;
	m_EquipSlot[0].m_H = HELMET_SLOT_H	;

	m_EquipSlot[1].m_X = m_pSurfaceBack->Xpos+121; // ����
	m_EquipSlot[1].m_Y = m_pSurfaceBack->Ypos+151;
	m_EquipSlot[1].m_W = ARMOR_SLOT_W	;
	m_EquipSlot[1].m_H = ARMOR_SLOT_H	;

	m_EquipSlot[2].m_X = m_pSurfaceBack->Xpos+121; // ����
	m_EquipSlot[2].m_Y = m_pSurfaceBack->Ypos+216;
	m_EquipSlot[2].m_W = PANTS_SLOT_W	;
	m_EquipSlot[2].m_H = PANTS_SLOT_H	;

	m_EquipSlot[3].m_X = m_pSurfaceBack->Xpos+223; // ����
	m_EquipSlot[3].m_Y = m_pSurfaceBack->Ypos+228;
	m_EquipSlot[3].m_W = CLOAK_SLOT_W	;
	m_EquipSlot[3].m_H = CLOAK_SLOT_H	;

	m_EquipSlot[4].m_X = m_pSurfaceBack->Xpos+121;  // �Ź�
	m_EquipSlot[4].m_Y = m_pSurfaceBack->Ypos+291;
	m_EquipSlot[4].m_W = SHOES_SLOT_W	;
	m_EquipSlot[4].m_H = SHOES_SLOT_H	;

	m_EquipSlot[5].m_X = m_pSurfaceBack->Xpos+17; // ����
	m_EquipSlot[5].m_Y = m_pSurfaceBack->Ypos+197;
	m_EquipSlot[5].m_W = SHIELD_SLOT_W	;
	m_EquipSlot[5].m_H = SHIELD_SLOT_H	;

	m_EquipSlot[6].m_X = m_pSurfaceBack->Xpos+77;  // �����
	m_EquipSlot[6].m_Y = m_pSurfaceBack->Ypos+116;
	m_EquipSlot[6].m_W = NECKLACE_SLOT_W	;
	m_EquipSlot[6].m_H = NECKLACE_SLOT_H	;

	m_EquipSlot[7].m_X = m_pSurfaceBack->Xpos+193;  // ����
	m_EquipSlot[7].m_Y = m_pSurfaceBack->Ypos+201;
	m_EquipSlot[7].m_W = RING_SLOT_W		;
	m_EquipSlot[7].m_H = RING_SLOT_H		;

	m_EquipSlot[8].m_X = m_pSurfaceBack->Xpos+64;  // ����
	m_EquipSlot[8].m_Y = m_pSurfaceBack->Ypos+187;
	m_EquipSlot[8].m_W = GLOVES_SLOT_W	;
	m_EquipSlot[8].m_H = GLOVES_SLOT_H	;

	m_EquipSlot[9].m_X = m_pSurfaceBack->Xpos+224; // ����
	m_EquipSlot[9].m_Y = m_pSurfaceBack->Ypos+161;
	m_EquipSlot[9].m_W = WEAPON_SLOT_W	;
	m_EquipSlot[9].m_H = WEAPON_SLOT_H	;


	m_EquipSlot[10].m_X = m_pSurfaceBack->Xpos+20;
	m_EquipSlot[10].m_Y = m_pSurfaceBack->Ypos+294;
	m_EquipSlot[10].m_W = PET_SLOT_W;
	m_EquipSlot[10].m_H = PET_SLOT_H;

	m_EquipSlot[11].m_X = m_pSurfaceBack->Xpos+20+PET_SLOT_W+22 ;
	m_EquipSlot[11].m_Y = m_pSurfaceBack->Ypos+288;
	m_EquipSlot[11].m_W = PET_SLOT_W;
	m_EquipSlot[11].m_H = PET_SLOT_H;

	m_EquipSlot[12].m_X = m_pSurfaceBack->Xpos+20+PET_SLOT_W+152 ;
	m_EquipSlot[12].m_Y = m_pSurfaceBack->Ypos+294;
	m_EquipSlot[12].m_W = PET_SLOT_W;
	m_EquipSlot[12].m_H = PET_SLOT_H;

	m_EquipSlot[13].m_X = m_pSurfaceBack->Xpos+20+PET_SLOT_W+198 ;
	m_EquipSlot[13].m_Y = m_pSurfaceBack->Ypos+294;
	m_EquipSlot[13].m_W = PET_SLOT_W;
	m_EquipSlot[13].m_H = PET_SLOT_H;
}

void CControlInven::LoadRes()
{
	if( !g_pDisplay )
		return;

	// �������׷��̵� ���� �߰� �κ�
	Load_Masuk();

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
	m_ScreenHeight = g_pNk2DFrame->GetClientHeight();

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/inventory/inven.bmp") ) )
		return;

	g_pDisplay->CreateSurfaceFromBitmap (&m_pEnableSlot, "interface/slot/slot_enable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pDisableSlot, "interface/slot/slot_disable.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pExistSlot, "interface/slot/slot_exist.bmp");
	g_pDisplay->CreateSurfaceFromBitmap (&m_pImgGem, "interface/slot/img_gem.bmp");

	m_pSurfaceBack->SetColorKey( TRANS_COLOR );
	m_pImgGem->SetColorKey(RGB(0, 0, 0));

	m_CloseBtn.LoadRes();
	m_InvenBtn1.LoadRes();
	m_InvenBtn2.LoadRes();
	m_InvenBtn3.LoadRes();
	m_InvenBtn4.LoadRes();
	//m_SortBtn.LoadRes();
	if( g_bUseEventWear ) ///�̺�Ʈ�������� �ý����� ����Ǿ��ִٸ�...
	{
		if( FAILED( g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBackEvWear, "interface/inventory/inven2.bmp") ) )
			return;

		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurEventWearFlagON, "interface/common/check_none.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurEventWearFlagOFF, "interface/common/check.bmp");
		m_pSurfaceBackEvWear->SetColorKey( TRANS_COLOR );

		m_btnNormalEquip.LoadRes();
		m_btnEventEquip.LoadRes();
	}



	if( m_pItemInfo )
		m_pItemInfo->LoadRes(m_ScreenWidth, m_ScreenHeight);

	m_pSurfaceBack->Xpos = m_ScreenWidth - m_pSurfaceBack->GetWidth();
	m_pSurfaceBack->Ypos = 0;

	m_slotstart_x = m_pSurfaceBack->Xpos+25;
	m_slotstart_y = m_pSurfaceBack->Ypos+370;

	m_CloseBtn.SetPosition(m_pSurfaceBack->Xpos+272, m_pSurfaceBack->Ypos+5);
	m_InvenBtn1.SetPosition(m_pSurfaceBack->Xpos+241, m_pSurfaceBack->Ypos+367);
	m_InvenBtn2.SetPosition(m_pSurfaceBack->Xpos+241, m_pSurfaceBack->Ypos+408);
	m_InvenBtn3.SetPosition(m_pSurfaceBack->Xpos+241, m_pSurfaceBack->Ypos+449);
	m_InvenBtn4.SetPosition(m_pSurfaceBack->Xpos+241, m_pSurfaceBack->Ypos+490);
	//m_SortBtn.SetPosition(m_pSurfaceBack->Xpos+199, m_pSurfaceBack->Ypos+331);
	SetEquipArea(52);
	m_pItemInfo->Init(14);
	m_MoneyText.SetPos(m_pSurfaceBack->Xpos+73, m_pSurfaceBack->Ypos+548+6);



	if( g_bUseEventWear ) ///�̺�Ʈ�������� �ý����� ����Ǿ��ִٸ�...
	{
		m_btnNormalEquip.SetPosition(m_pSurfaceBack->Xpos+41, m_pSurfaceBack->Ypos+62);	// �� ��ư�� m_InvenBtn1 ���� ����� ��ġ�� �����ϴϱ� �ػ� ���� �� �ʿ䰡 ����.
		m_btnEventEquip.SetPosition(m_pSurfaceBack->Xpos+118, m_pSurfaceBack->Ypos+62);

		m_pSurEventWearFlagON->Xpos	 = m_pSurfaceBack->Xpos + 81;
		m_pSurEventWearFlagON->Ypos  = m_pSurfaceBack->Ypos + 33;
		m_pSurEventWearFlagOFF->Xpos = m_pSurfaceBack->Xpos + 81;
		m_pSurEventWearFlagOFF->Ypos = m_pSurfaceBack->Ypos + 33;

		if( m_EventWearTimeText == NULL )
		{
			m_EventWearTimeText = new CTextOutBox;
			m_EventWearTimeText->SetString("0");
			m_EventWearTimeText->Init(12, RGB(255, 255, 0), 0, 0);
			m_EventWearTimeText->SetPos( m_InvenBtn1.PosX-180,  m_InvenBtn1.PosY-14 );
		}
	}


	if(m_pNkCha)
	{
		m_MoneyText.SetString(m_pNkCha->m_Money, true);
		m_iOldMoney = m_pNkCha->m_Money;
	}

	g_pDisplay->CreateSurfaceFromBitmap(&m_pImgRecycle, "interface/inventory/RECYCLE.BMP");
	m_pImgRecycle->SetColorKey( TRANS_COLOR );
	m_pImgRecycle->SetPos(m_pSurfaceBack->Xpos + 23, m_pSurfaceBack->Ypos + 543);
}

void CControlInven::DeleteRes()
{
	SAFE_DELETE_RES(m_EventWearTimeText); ///�̺�Ʈ��������
	SAFE_DELETE(m_pEvWearTimeBack); ///�̺�Ʈ��������
	SAFE_DELETE(m_pWarringEvWear); ///�̺�Ʈ��������
	SAFE_DELETE(m_pSurfaceBackEvWear); ///�̺�Ʈ��������
	SAFE_DELETE(m_pSurfaceBack);
	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);
	SAFE_DELETE(m_pImgGem);

	// [12/22/2008 D.K ] : memory leak.
	SAFE_DELETE( m_pSurEventWearFlagON );
	SAFE_DELETE( m_pSurEventWearFlagOFF );

	if( g_bUseEventWear ) ///�̺�Ʈ�������� �ý����� ����Ǿ��ִٸ�...
	{
		m_btnEventEquip.DeleteRes();
		m_btnNormalEquip.DeleteRes();
	}

	m_CloseBtn.DeleteRes();
	m_InvenBtn1.DeleteRes();
	m_InvenBtn2.DeleteRes();
	m_InvenBtn3.DeleteRes();
	m_InvenBtn4.DeleteRes();
	//m_SortBtn.DeleteRes();
	m_pItemInfo->DeleteRes();
	m_MoneyText.DeleteRes();



	Del_Masuk();

	SAFE_DELETE_RES(m_pItemInfo);
}

#define GET_SLOT_X(point_x)		((point_x - m_slotstart_x) / SLOT_WIDTH)
#define GET_SLOT_Y(point_y)		((point_y - m_slotstart_y) / SLOT_HEIGHT)

CItem* CControlInven::Draw()
{
	if (!g_pDisplay || !m_pSurfaceBack)
		return NULL;

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);
	CItem *pOverItem = NULL;

	if( g_bUseEventWear && m_bEventWearView && m_pSurfaceBackEvWear ) ///�̺�Ʈ�������� �� �������̶��..
		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBackEvWear);
	else
		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

	m_CloseBtn.DrawToBlt();
	//m_SortBtn.DrawToBlt();
	if( m_pNkCha )
	{
		switch(m_pNkCha->m_CurPackNum)
		{
		case 0:
			m_InvenBtn1.SetState( BTN_DOWN );
			m_InvenBtn2.SetState( BTN_NORMAL );
			m_InvenBtn3.SetState( BTN_NORMAL );
			m_InvenBtn4.SetState( BTN_NORMAL );
			break;
		case 1:
			m_InvenBtn1.SetState( BTN_NORMAL );
			m_InvenBtn2.SetState( BTN_DOWN );
			m_InvenBtn3.SetState( BTN_NORMAL );
			m_InvenBtn4.SetState( BTN_NORMAL );
			break;
		case 2:
			m_InvenBtn1.SetState( BTN_NORMAL );
			m_InvenBtn2.SetState( BTN_NORMAL );
			m_InvenBtn3.SetState( BTN_DOWN );
			m_InvenBtn4.SetState( BTN_NORMAL);
			break;
		case 4:
			m_InvenBtn1.SetState( BTN_NORMAL );
			m_InvenBtn2.SetState( BTN_NORMAL );
			m_InvenBtn3.SetState( BTN_NORMAL );
			m_InvenBtn4.SetState( BTN_DOWN );
			break;
		}

		m_InvenBtn1.Draw();
		m_InvenBtn2.Draw();
		m_InvenBtn3.Draw();
		m_InvenBtn4.Draw();
	}


	if( g_bUseEventWear ) ///�̺�Ʈ�������� �ý����� ����Ǿ��ִٸ�...
	{
		//if( g_dwClientCountry != CTRY_JPN )
		{
			if(g_pRoh && g_pRoh->m_bEventWearFlag )
				g_pDisplay->Blt(m_pSurEventWearFlagON->Xpos, m_pSurEventWearFlagON->Ypos, m_pSurEventWearFlagON);
			else
				g_pDisplay->Blt(m_pSurEventWearFlagOFF->Xpos, m_pSurEventWearFlagOFF->Ypos, m_pSurEventWearFlagOFF);
		}

		if( m_bEventWearView )
		{
			m_btnNormalEquip.SetState( BTN_NORMAL );
			m_btnEventEquip.SetState( BTN_DOWN );
		}
		else
		{
			m_btnNormalEquip.SetState( BTN_DOWN );
			m_btnEventEquip.SetState( BTN_NORMAL );
		}

		m_btnNormalEquip.Draw();
		m_btnEventEquip.Draw();

		// �ð������� ������� �ð��� ǥ�����ش�.
		if( m_bEventWearView && m_EventWearTime != -1 )
		{
			DWORD NowTick = timeGetTime(); // ���־��ϱ� �̸� ���ΰ�.
			DWORD Elapsed = NowTick - m_EventWearTimeUpdateTick; // ������ �ð�.

			if( Elapsed >= 1000 ) // 1���̻� �����ٸ�...
			{
				m_EventWearTime -= Elapsed/1000; // �ʸ� �����ش�.
				m_EventWearTimeUpdateTick = timeGetTime(); // �ֽ� �ð����� ����.

				if( m_EventWearTime < 0 )
					m_EventWearTime = 0; // 0������ ��������....
			}


			int	minut = m_EventWearTime / 60 ;
			int vsec = m_EventWearTime % 60;
			int vhour = ( minut / 60 );
			int vminu =   minut % 60 ;

			char strTemp[64];

			if( m_EventWearTime == 0 )
			{
				sprintf( strTemp, "-" );
			}
			else
			{
				if( (timeGetTime()%1500) > 450 )
					sprintf( strTemp, "%02d : %02d : %02d", vhour, vminu, vsec);
				else
					sprintf( strTemp, "%02d   %02d   %02d", vhour, vminu, vsec);
			}

			m_EventWearTimeText->SetString( strTemp );

			if( m_pEvWearTimeBack )
				g_pDisplay->Blt( m_InvenBtn1.PosX-216, m_InvenBtn1.PosY-18, m_pEvWearTimeBack );

			m_EventWearTimeText->Draw();
		}
	}


	if(m_pNkCha
			&& m_iOldMoney != m_pNkCha->m_Money)
	{
		m_iOldMoney = m_pNkCha->m_Money;
		m_MoneyText.SetString(m_pNkCha->m_Money, true);
	}
	m_MoneyText.Draw(m_pSurfaceBack->Xpos+73, m_pSurfaceBack->Ypos+548+6,170);

	CItem *pItem = NULL;
	if( m_pNkCha )
		pItem = m_pNkCha->m_Inven[m_pNkCha->m_CurPackNum];

	while (pItem)
	{
		rcRect.left = 0;
		rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top = 0;
		rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
		g_pDisplay->Blt( m_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX),
						 m_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY),
						 m_pExistSlot, &rcRect);
		pItem = pItem->m_Next;
	}

	// Extra Slot�� �ִ� �������� ���� �� ǥ��
	int slot_x_num, slot_y_num;
	if (m_pNkCha
			&& m_pNkCha->m_ExtraSlot)
	{
		if (point.x >= m_slotstart_x
				&& point.x < m_slotstart_x + (SLOT_WIDTH * SLOT_X_NUM)
				&& point.y >= m_slotstart_y
				&& point.y < m_slotstart_y + (SLOT_HEIGHT * SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
				slot_x_num = (point.x - m_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
			else
				slot_x_num = (point.x - m_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);

			if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
				slot_y_num = (point.y - m_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
			else
				slot_y_num = (point.y - m_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

			if (slot_x_num >= 0
					&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < SLOT_X_NUM
					&& slot_y_num >= 0
					&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot);
				if (m_pNkCha->GetItemIndexInSlot(m_pNkCha->m_CurPackNum
												 , slot_x_num, slot_y_num
												 , GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)
												 , GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)) >= -1)
				{
					g_pDisplay->Blt( m_slotstart_x + (SLOT_WIDTH*slot_x_num),
									 m_slotstart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pEnableSlot, &rcRect);
				}
				else
				{
					g_pDisplay->Blt( m_slotstart_x + (SLOT_WIDTH*slot_x_num),
									 m_slotstart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// �����ս� ����� ���� ��...
	if( g_pNk2DFrame->GetItemRender() && m_pNkCha )
	{
		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();
		// ��� �׸���...
		for (int i = 0; i < WEARING_NUM; i++)
		{
			if( g_bUseEventWear && m_bEventWearView ) ///�̺�Ʈ��������
			{
				if (m_pNkCha->m_EventWearing[i]) // �̺�Ʈ ���� ������ �������̸� �̺�Ʈ ���� ���Ը� �����ش�.
				{
					CLolos *pLolo = GET_LOLO(m_pNkCha->m_EventWearing[i]);
					if (pLolo)
					{
						if (point.x >= m_EquipSlot[i].m_X
								&& point.x < m_EquipSlot[i].m_X + m_EquipSlot[i].m_W
								&& point.y >= m_EquipSlot[i].m_Y
								&& point.y < m_EquipSlot[i].m_Y + m_EquipSlot[i].m_H)
						{
							g_pNk2DFrame->RenderItemInUI( m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2 ,
														  m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2 ,
														  m_pNkCha->m_EventWearing[i] , TRUE ,FALSE, TRUE);

							pOverItem = m_pNkCha->m_EventWearing[i];
						}
						else
						{
							g_pNk2DFrame->RenderItemInUI( m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2 ,
														  m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2 ,
														  m_pNkCha->m_EventWearing[i] , FALSE ,FALSE, TRUE);
						}
					}
				}
			}
			else
			{
				if (m_pNkCha->m_Wearing[i])
				{
					CLolos *pLolo = GET_LOLO(m_pNkCha->m_Wearing[i]);
					if (pLolo)
					{
						if (point.x >= m_EquipSlot[i].m_X
								&& point.x < m_EquipSlot[i].m_X + m_EquipSlot[i].m_W
								&& point.y >= m_EquipSlot[i].m_Y
								&& point.y < m_EquipSlot[i].m_Y + m_EquipSlot[i].m_H)
						{
							g_pNk2DFrame->RenderItemInUI(m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2 ,
														 m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2 ,
														 m_pNkCha->m_Wearing[i] , TRUE , FALSE , TRUE );
							pOverItem = m_pNkCha->m_Wearing[i];
						}
						else
						{
							g_pNk2DFrame->RenderItemInUI( m_EquipSlot[i].m_X + (m_EquipSlot[i].m_W - ITEM_RENDER_WIDTH) / 2 ,
														  m_EquipSlot[i].m_Y + (m_EquipSlot[i].m_H - ITEM_RENDER_HEIGHT) / 2 ,
														  m_pNkCha->m_Wearing[i] , FALSE , FALSE , TRUE );
						}
					}
				}
			}
		}

		// ����ǰ �׸���...
		if (point.x >= m_slotstart_x && point.y >= m_slotstart_y)
		{
			slot_x_num = GET_SLOT_X(point.x);
			slot_y_num = GET_SLOT_Y(point.y);
		}
		else
		{
			slot_x_num = -1;
			slot_y_num = -1;
		}
		int index = m_pNkCha->GetSlotIndex(slot_x_num, slot_y_num);
		pItem = m_pNkCha->m_Inven[m_pNkCha->m_CurPackNum];

		while (pItem)
		{
			if (pItem->m_Index == index )
			{
				g_pNk2DFrame->RenderItemInUI( m_slotstart_x + pItem->m_SlotX * SLOT_WIDTH ,
											  m_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT ,
											  pItem , TRUE , FALSE , FALSE );
				pOverItem = pItem;
			}
			else
			{
				g_pNk2DFrame->RenderItemInUI( m_slotstart_x + pItem->m_SlotX * SLOT_WIDTH ,
											  m_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT ,
											  pItem , FALSE , FALSE , FALSE );
			}
			pItem = pItem->m_Next;
		}
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}

	if( g_bUseEventWear ) ///�̺�Ʈ�������� �ý����� ����Ǿ��ִٸ�...
	{
		if( m_bEventWearView )
		{
			// ���� �̺�Ʈ ���� ��������� ����Ǿ��ִٸ� ���⼭ ������ �ڽ� �� �޼��� ����� ó�����ش�.
			if( m_EventWearTime != -1 && m_EventWearTime <= 0 ) // -1�� ������...
			{
				RECT rect;
				rect.left = m_EquipSlot[9].m_X-7; // ���� ���� ��ġ�� ��������.
				rect.top = m_EquipSlot[9].m_Y-8;
				rect.right = 248;
				rect.bottom = 160;

				DWORD color = RGBA_MAKE ( 0, 0, 0, 150 );

				RenderOj( rect, color );

				// �޼��� ���
				if( m_pWarringEvWear && (timeGetTime()%1500) > 450 )
					g_pDisplay->Blt( rect.left+55, rect.top+75, m_pWarringEvWear );
			}
		}
	}

	g_pDisplay->Blt(m_pImgRecycle->Xpos, m_pImgRecycle->Ypos, m_pImgRecycle);

	CMultiText multiText;
	if (m_pNkCha && m_pNkCha->m_ExtraSlot)
	{
		int can_wear = m_pNkCha->CanWear();

		if (m_pItemInfo && can_wear < 0 && can_wear > -20)
		{
			if (can_wear != m_pItemInfo->GetIndex())
			{
				switch (can_wear)
				{
				case -2: //-- IDS_INVEN_REQUILE_MORE_LEVEL : ������ �ȵ�
					multiText.AddString((char*)G_STRING(IDS_INVEN_REQUILE_MORE_LEVEL), RGB(252, 132, 3));
					break;
				case -3: //-- IDS_INVEN_SCARCE_STR : ���� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_SCARCE_STR), RGB(252, 132, 3));
					break;
				case -4: //-- IDS_INVEN_SCARCE_ETC : �������� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_SCARCE_ETC), RGB(252, 132, 3));
					break;
				case -5: //-- IDS_INVEN_CANNOT_HOLD_SHIELD : �μչ���� ���� ����.
					multiText.AddString((char*)G_STRING(IDS_INVEN_CANNOT_HOLD_SHIELD), RGB(252, 132, 3));
					break;
				case -6: //-- IDS_INVEN_CANNOT_HOLD_WEAPON : ���� �� �μ� ���� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_CANNOT_HOLD_WEAPON), RGB(252, 132, 3));
					break;
				case -7: //-- IDS_INVEN_SCARCE_INT : ������ ������.
					multiText.AddString((char*)G_STRING(IDS_INVEN_SCARCE_INT), RGB(252, 132, 3));
					break;
				case -8: //-- IDS_INVEN_SCAREC_QUICKNESS : ��μ��� ������.
					multiText.AddString((char*)G_STRING(IDS_INVEN_SCAREC_QUICKNESS), RGB(252, 132, 3));
					break;
				case -9: //-- IDS_INVEN_CANNOT_HOLD_SHIELD2 : ���� �� ���� ����.
					multiText.AddString((char*)G_STRING(IDS_INVEN_CANNOT_HOLD_SHIELD2), RGB(252, 132, 3));
					break;
				case -10: //-- IDS_INVEN_CANNOT_HOLD_WEAPON2 : ���� �� �μ� ���� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_CANNOT_HOLD_WEAPON2), RGB(252, 132, 3));
					break;
				case -11: //-- IDS_INVEN_WRONG_RACE : �����̳� ������ ���� ����.
					multiText.AddString((char*)G_STRING(IDS_INVEN_WRONG_RACE), RGB(252, 132, 3));
					break;
				case -12:	// IDS_CANNOT_STATE_WEAR : ���� ������ �����۰� ���� ������ �Ұ����մϴ�.
					multiText.AddString((char*)G_STRING( IDS_CANNOT_STATE_WEAR ), RGB(252, 132, 3));
					break;
				case -13:	// IDS_CANNOT_STATE_WEAR : ���� ������ �����۰� ���� ������ �Ұ����մϴ�.
					multiText.AddString((char*)G_STRING( IDS_DIFFER_UNION ), RGB(252, 132, 3));
					break;
				case -14:  // IDS_DONT_USE_CASH_STASH : �ش� ������ �̿��Ͻ� �� �����ϴ�.   ///�̺�Ʈ�������� ���� ��밡�� üũ�� ���� �������� ���.
					multiText.AddString((char*)G_STRING( IDS_DONT_USE_CASH_STASH ), RGB(252, 132, 3));
					break;
				}
				multiText.AddString(" ");
				multiText.AddString(" ");
				m_pItemInfo->PutString(&multiText);
				m_pItemInfo->SetIndex(can_wear);
			}
			m_pItemInfo->Draw(point.x, point.y);
		}
	}
	else if (pCMyApp->m_pMouse && pOverItem)
	{
		pCMyApp->m_pMouse->SetMouseType(M_HOLD);
		return pOverItem;
	}
	else if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	if(m_bAct_Masuk)
		Draw_Masuk();

	return NULL;
}


//----------------------------------------------------------------------------
//-- DrawItemInfo
//----------------------------------------------------------------------------
//-- Inven â�� ��ġ�ϰ� �ִ� Item �� ������ �����ش�.
void CControlInven::DrawItemInfo(CItem *pOverItem, int where_item)
{
	if (!pOverItem || !m_pNkCha || m_pNkCha->m_ExtraSlot || !m_pItemInfo)
		return;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	char strTemp[MAX_PATH] = {0,};
	char strTemp2[MAX_PATH] = {0,};
	char strTemp3[MAX_PATH] = {0,};
	CMultiText multiText;
	float dam1;
	float dam2;
	float defense;
	float magic_dam_bonus;
	int gem_num = 0;
	char speed_str[200];
	__int64 price;
	int i_defense = 0;

	int CommaPos = 0; ///����ǥ�ð���


	if (pOverItem->m_Index != m_pItemInfo->GetIndex() || m_bDrawInfoUpdate) // �ð��� �������� �������� ���� ������Ʈ �÷��׸� ���� ��. m_bUpdate�� ���õǸ� �ٽ� �׸���.(by ����) ///�ð���������
	{
		m_bDrawInfoUpdate = FALSE; // �ٷ� ���� ///�ð���������

		long race_flag = GET_ITEM_RACE(pOverItem);



		///�ð���������
		if( pOverItem->m_bTimeLimit )
			sprintf( strTemp2, "%s %s", G_STRING(IDS_TIMELIMIT_ITEM_TITLE), GET_ITEM_NAME_VNUM(pOverItem->m_Vnum)  ); // �̸� �տ� ������ �߰�.
		else if( pOverItem->m_Vnum == 1307 ) //mungmae-2005/10/5 �°�&���� �Ӵ� ����ǥ��
			sprintf(strTemp2, "%s(%d Just)", GET_ITEM_NAME_VNUM(pOverItem->m_Vnum) , pOverItem->m_Special / 10000); // �̰� �ѱ��� �����״� �׳� �Ѿ��
		else if( pOverItem->m_Vnum == 1239 ) // BP �ڽ��� �ɼǿ� ���� BP�� ǥ��.
			sprintf(strTemp2, "%d %s", pOverItem->m_Special, GET_ITEM_NAME_VNUM(pOverItem->m_Vnum)  );
		else if( pOverItem->m_Vnum == 1359 && pOverItem->m_Special2 > 0 ) //mungmae-2005/04/14 ����Ʈ ���� �������� ǥ��
		{
			if( g_dwClientCountry == CTRY_KOR && pOverItem->m_Special2 == 100 ) // �ѱ��� 100���� ����Ʈ ������ "���� ����Ʈ ����"�� �ٲ㼭 ǥ��. ���� ���Ƕ����̶���.
				sprintf(strTemp2, G_STRING(IDS_LHSTRING1742) );
			else
				sprintf(strTemp2, G_STRING(IDS_LHSTRING1743), pOverItem->m_Special2, GET_ITEM_NAME_VNUM(pOverItem->m_Vnum)  ); ///07-04-23 �ѱ۷� �ڵ��Ȱ� ����
		}
		else if( pOverItem->m_Vnum == 1398 && pOverItem->m_Special > 0 ) //mungmae-2005/04/14 ����Ʈ ���� �������� ǥ��
			sprintf(strTemp2, G_STRING(IDS_LHSTRING1743), pOverItem->m_Special, GET_ITEM_NAME_VNUM(pOverItem->m_Vnum)  ); ///07-04-23 �ѱ۷� �ڵ��Ȱ� ����
		else // �ð��� �ƿ����� �ƴ϶��..
			sprintf( strTemp2, "%s", GET_ITEM_NAME_VNUM(pOverItem->m_Vnum) ); // �׳� �̸��� ���

		if( pOverItem->m_PriceType == 2 ) // ĳ�� �������̸�... ///�������.
			//-- IDS_ITEMNAME_CASH : (CASH)
			strcat( strTemp2, (char*)G_STRING(IDS_ITEMNAME_CASH) ); // �̸� �ڿ� ĳ������������ ǥ�����ش�.

		if( g_bAdmin2 )
		{
			sprintf(strTemp3, "[%d] %s", pOverItem->m_Vnum, strTemp2);
			strcpy(strTemp2, strTemp3);
		}

		//==================================
		////2003 ����������� �÷��� ���������� �ν��ϴ°� ����.
		if (GET_TYPE(pOverItem) == ITYPE_SUB)
		{
			strcpy(strTemp, strTemp2); ///�ð���������
			multiText.AddString(strTemp, _FCOLOR_YELLOW);
		}
		else if( pOverItem->m_Vnum ==1087
				 || pOverItem->m_Vnum ==1399 // 345���� ������ ������ ����� �߰� ///��
				 || pOverItem->m_Vnum ==922
				 || pOverItem->m_Vnum ==933
				 || pOverItem->m_Vnum ==934
				 || pOverItem->m_Vnum ==935
				 || pOverItem->m_Vnum ==1032)
		{
			//0407 ���������..
			strcpy(strTemp, strTemp2); ///�ð���������
			multiText.AddString(strTemp, SPECIAL_ITEM_COLOR_REF);
		}
		else
		{
			if (pOverItem->m_PlusNum > 0
					&& (GET_TYPE(pOverItem) == ITYPE_ARMOR
						|| GET_TYPE(pOverItem) == ITYPE_WEAPON))
			{
				sprintf(strTemp, "%s +%d", strTemp2, pOverItem->m_PlusNum); ///�ð���������
				multiText.AddString(strTemp, UPGRADE_ITEM_COLOR_REF);
			}
			else if (pOverItem->m_Special > 0)
			{
				strcpy(strTemp, strTemp2); ///�ð���������
				multiText.AddString(strTemp, SPECIAL_ITEM_COLOR_REF);
			}
			else
			{
				strcpy(strTemp, strTemp2); ///�ð���������
				multiText.AddString(strTemp, _FCOLOR_YELLOW);
			}

			if (pOverItem->m_GemNum > 0
					&& ( GET_TYPE(pOverItem) == ITYPE_ARMOR
						 || GET_TYPE(pOverItem) == ITYPE_WEAPON )
					&& !(pOverItem->m_Vnum == 1087
						 || pOverItem->m_Vnum ==922
						 || pOverItem->m_Vnum ==933
						 || pOverItem->m_Vnum ==934
						 || pOverItem->m_Vnum ==935
						 || pOverItem->m_Vnum ==1032 ))
			{
				//�������� ������ ������ �ȵ�
				multiText.AddString(" ", _FCOLOR_WHITE);
				gem_num = pOverItem->m_GemNum;
			}
		}

		if (pOverItem->m_MinLevel > 0)
		{
			if(GET_ATT_FLAG(pOverItem) & IATT_MAX_LEVEL)
				//int proto_num;
				//proto_num = g_ItemList.FindItem(pOverItem->m_ProtoNum);
				//-- IDS_INVEN_REQ_LEVEL : �䱸���� : %d - %d
				sprintf(strTemp, G_STRING(IDS_INVEN_REQ_LEVEL), pOverItem->m_MinLevel, g_ItemList.m_ItemArray[pOverItem->m_ProtoNum].m_MaxUse);
			else
				//IDS_INVEN_REGUILE_LEVEL
				sprintf(strTemp, G_STRING(IDS_INVEN_REGUILE_LEVEL), pOverItem->m_MinLevel);

			multiText.AddString(strTemp, _FCOLOR_RED);
		}

		
		if( GET_ATT_FLAG(pOverItem) & IATT_EVENT_WEAR )
		{
			if( GET_ATT_FLAG(pOverItem) & IATT_DISABLE_NORMAL )
				multiText.AddString("(Costume exclusive items)", _FCOLOR_SKY);
			else
				multiText.AddString("(Costume items)", _FCOLOR_SKY);
		}

		if( pOverItem->m_Vnum != 2956 && pOverItem->m_Vnum != 2963 )
			SetSpecial_SkillDoc(pOverItem,multiText);

		switch (GET_TYPE(pOverItem))
		{
		case ITYPE_SUB:
			{
				if (pOverItem->m_MinLevel > 0)
				{
					sprintf(strTemp, G_STRING(IDS_INVEN_MIN_LEVEL), pOverItem->m_MinLevel);
					multiText.AddString(strTemp, _FCOLOR_RED);
				}

				switch (race_flag)
				{
				case 0x0003: //-- IDS_INVEN_ONLY_BULKAN : ��ĭ�� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_BULKAN), RGB(255, 150, 150));
					break;
				case 0x000c: //-- IDS_INVEN_ONLY_KAI : ī�̸� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_KAI), RGB(255, 150, 150));
					break;
				case 0x0030: //-- IDS_INVEN_ONLY_AID : ���̵�Ƹ� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_AID), RGB(255, 150, 150));
					break;
				case 0x00c0: //-- IDS_INVEN_ONLY_HUMAN : �޸� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HUMAN), RGB(255, 150, 150));
					break;
				case 0x0300: //-- IDS_INVEN_ONLY_HIBRIDER : ���̺긮�� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HIBRIDER), RGB(255, 150, 150));
					break;
				case 0x0C00:
					multiText.AddString((char*)G_STRING(IDS_LHSTRING1942), RGB(255, 150, 150));
					break;
				}
				//----------------------------------------------------------------------
				//-- ����� ���� ���� ǥ��
				if(GET_SHAPE(pOverItem)==ISUB_DSTONE)
				{
					//-- ����
					if(pOverItem->m_Num8 != 0)
						//-- IDS_TYPE_ATTACK : ������
						sprintf(strTemp, G_STRING(IDS_TYPE_ATTACK));
					else
						//-- IDS_TYPE_SUB : ������
						sprintf(strTemp, G_STRING(IDS_TYPE_SUB));

					multiText.AddString(strTemp, _FCOLOR_SKY);

					if( pOverItem->m_Vnum != 1143 && pOverItem->m_Vnum != 3066 && pOverItem->m_Vnum != 10373 && pOverItem->m_Vnum != 10416 && pOverItem->m_Vnum != 11064 && pOverItem->m_Vnum != 11085) // ����ũ ������ �� ��ġ�� ���� ���ݷ¿� ����� ���ϹǷ� ǥ�þ���.(������ ������ ����.) ///070917 // Update Elite
					{
						//-- IDS_INVEN_ATT : ���� ��ġ: %d-%d
						sprintf(strTemp, G_STRING(IDS_APPLY), pOverItem->m_Num0,pOverItem->m_Num1);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					//-- IDS_REDDRAGON_TARGET : ���� Ÿ��
					sprintf(strTemp, G_STRING(IDS_REDDRAGON_TARGET), pOverItem->m_Num2);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					if(pOverItem->m_Special <=1)
						pOverItem->m_Special = 0;
					//mungmae-2006/4/26 �Ϲ� ���������� �������� �������� ���̰� �Ѵ�.
					if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
							&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
					{
						//-- IDS_INVEN_LASTING : ������
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Num3,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					else
					{
						//-- IDS_INVEN_LASTING : ������
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Special,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					//-- IDS_INVEN_USE_MANA : �����Ҹ�
					sprintf(strTemp, G_STRING(IDS_INVEN_USE_MANA), pOverItem->m_Num4);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					//-- IDS_SKILL_KEEP_TIME : ���� �ð�
					sprintf(strTemp,G_STRING(IDS_SKILL_KEEP_TIME), pOverItem->m_Num7);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					//-- ����
					//-- ����ũ ����
					if(pOverItem->m_Vnum == 1143 || pOverItem->m_Vnum == 3066 || pOverItem->m_Vnum == 10373 || pOverItem->m_Vnum == 10416 || pOverItem->m_Vnum == 11064 || pOverItem->m_Vnum == 11085)
						//-- IDS_DESC_DIV1 : �������� ������ �������� �ش�
						sprintf(strTemp,G_STRING(IDS_DESC_DIV1));
					//-- ������ ����
					else if (pOverItem->m_Vnum == 1155)
						//-- IDS_DESC_DIV2 : ������� �ʴ� 100�� ȸ����Ų��
						sprintf(strTemp,G_STRING(IDS_DESC_DIV2));
					//-- ������ ����
					else
						//-- IDS_DESC_DIV3 : �������� ���ÿ� 10%Ȯ���� 3Lv ȥ���� �Ǵ�
						sprintf(strTemp,G_STRING(IDS_DESC_DIV3));

					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
				//----------------------------------------------------------------------
				//-- �����̾� ���� ǥ��
				else if(GET_SHAPE(pOverItem)==ISUB_SLAYER )
				{
					//-- IDS_INVEN_ATT : ���ݷ�: %d-%d
					sprintf(strTemp, G_STRING(IDS_INVEN_ATT), pOverItem->m_Num0,pOverItem->m_Num1);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					if(pOverItem->m_Num2 != 0)
					{
						//-- IDS_INVEN_DEMOL : ��������Ȯ��: %d%%
						sprintf(strTemp,  G_STRING(IDS_INVEN_DEMOL), pOverItem->m_Num2);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					if(pOverItem->m_Special <=1)
						pOverItem->m_Special = 0;
					//mungmae-2006/4/26 �Ϲ� ���������� �������� �������� ���̰� �Ѵ�.
					if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
							&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
					{
						//-- IDS_INVEN_LASTING : ������
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Num3,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					else
					{
						//-- IDS_INVEN_LASTING : ������
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Special,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					//-- IDS_INVEN_USE_STAMINA : �Ҹ� ���׹̳�
					sprintf(strTemp, G_STRING(IDS_INVEN_USE_STAMINA), pOverItem->m_Num4);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
				//----------------------------------------------------------------------
				//-- ��ũ�� ���� ǥ��
				else if(GET_SHAPE(pOverItem)==ISUB_CHAKRAM)
				{
					if(pOverItem->m_Num2 != 0)
					{
						//-- IDS_INVEN_CUTTING_RATE : ����Ȯ��
						sprintf(strTemp, G_STRING(IDS_INVEN_CUTTING_RATE), pOverItem->m_Num2);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					if(pOverItem->m_Special <=1)
						pOverItem->m_Special = 0;
					//mungmae-2006/4/26 �Ϲ� ���������� �������� �������� ���̰� �Ѵ�.
					if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
							&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
					{
						//-- IDS_INVEN_LASTING : ������
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Num3,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					else
					{
						//-- IDS_INVEN_LASTING : ������
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Special,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					//-- IDS_INVEN_STONE_RATE : ��ȭȮ��: %d%%
					sprintf(strTemp, G_STRING(IDS_INVEN_STONE_RATE), pOverItem->m_Num7);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					//-- IDS_INVEN_RATE_COLD : �ݵ�Ȯ��: %d%%
					sprintf(strTemp, G_STRING(IDS_INVEN_RATE_COLD), pOverItem->m_Num8);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					//-- IDS_INVEN_USE_STAMINA : �Ҹ� ���׹̳�
					sprintf(strTemp,  G_STRING(IDS_INVEN_USE_STAMINA), pOverItem->m_Num4);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
				else if(GET_SHAPE(pOverItem)==ISUB_L_EQUIP)
				{
					int flag=0;
					for( int k = 0 ; k < 16 ; k ++ )
					{
						flag = 1 << k;
						if(pOverItem->m_Special & flag)
						{
							switch (k)
							{
							case 0://-- IDS_INVEN_ASSEMB_RADAR : ���̴�
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_RADAR));
								break;
							case 1://-- IDS_INVEN_ASSEMB_HARD : �ϵ�踮�ȯ
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_HARD));
								break;
							case 2://-- IDS_INVEN_ASSEMB_PURGE : ������ȯ
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_PURGE));
								break;
							case 3://-- IDS_INVEN_ASSEMB_VOLT : ��Ʈ����
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_VOLT));
								break;
							case 4://-- IDS_INVEN_ASSEMB_VOLCANIC : ��ĳ������
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_VOLCANIC));
								break;
							case 5://-- IDS_INVEN_ASSEMB_STONE : ��ȭ����
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_STONE));
								break;
							case 6://-- IDS_INVEN_ASSEMB_COLD : �ݵ�����
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_COLD));
								break;
							case 7://-- IDS_INVEN_ASSEMB_ELEC_HIT : �������
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_ELEC_HIT));
								break;
							case 8://-- IDS_INVEN_ASSEMB_ELEC_SUCTION : �������
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_ELEC_SUCTION));
								break;
							case 9://-- IDS_INVEN_ASSEMB_ELEC_DOUBLE : ��������
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_ELEC_DOUBLE));
								break;
							case 10://-- IDS_INVEN_ASSEMB_ATOM : �����̵�
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_ATOM));
								break;
							case 11://-- IDS_INVEN_ASSEMB_GAB : �����ְ�
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_GAB));
								break;
							case 12://-- IDS_INVEN_ASSEMB_GAB2 : ���������
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_GAB2));
								break;
							case 13://-- IDS_INVEN_ASSEMB_INTEG : ��������
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_INTEG));
								break;
							case 15://-- IDS_INVEN_ASSEMB_INTEG2 : ���հ迭
								sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_INTEG2));
								break;
							}
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}//-- end of if(pOverItem->m_Special & flag)
					}//-- end of for

					if( pOverItem->m_Vnum == 743
							|| pOverItem->m_Vnum == 748
							|| pOverItem->m_Vnum == 753 ) // 05-06-10 ���� �ż���� ���ݷ� ǥ�� �����ְ� ó��.(by ����)
					{
						// Ȥ�� ���߿� ������ ���𰡸� ǥ�����ְ� ������ �������.
						// ���ǹ��� ������ �����ؼ� �̷������� if���� ó��.
					}
					else // ���� �ż��Ⱑ �ƴϸ� ���������� ���ݷ� ǥ��.
					{
						//-- IDS_INVEN_ATT : ���ݼӵ�: %s // �̰� ���ݼӵ��� �ƴ϶� ���ݷ� �ε�. Ȯ���غ���.(by ����)
						sprintf(strTemp,  G_STRING(IDS_INVEN_ATT), pOverItem->m_Num0,pOverItem->m_Num1);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					switch(pOverItem->m_Num2)
					{
					case 1://-- IDS_INVEN_DAM_DURAITON : ����������: %d
						sprintf(strTemp, G_STRING(IDS_INVEN_DAM_DURAITON), pOverItem->m_Num8);
						multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					case 8://-- IDS_INVEN_RATE_BLAST : ����Ȯ��: %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RATE_BLAST), pOverItem->m_Num8);
						multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					case 4://-- IDS_INVEN_DAM_MINE : ���ڵ�����: %d // 05-06-10 ���� �������� ǥ�����ֱ� ���� �κ��� �־� �ƿ� ǥ�þ��ϵ��� ����.(by ����)
						//            sprintf(strTemp, G_STRING(IDS_INVEN_DAM_MINE), pOverItem->m_Num8);
						//            multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					case 100://-- IDS_INVEN_GATHER_PARTS : ���꿡 �ʿ��� ��ǰ���� ä���Ѵ�
						sprintf(strTemp, G_STRING(IDS_INVEN_GATHER_PARTS));
						multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					default:
						break;
					}

					if(pOverItem->m_Vnum != 695) //-- ä���⸦ ������ �������
					{
						//-- IDS_INVEN_NUM_BULLET : źȯ��: %d/%d
						sprintf(strTemp, G_STRING(IDS_INVEN_NUM_BULLET), pOverItem->m_PlusNum,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					//-- IDS_INVEN_USE_ELEC : ����¼Ҹ�: %d
					sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), pOverItem->m_Num4);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					//-- IDS_INVEN_NUM_SLOT : ���԰���: %d
					sprintf(strTemp, G_STRING(IDS_INVEN_NUM_SLOT), pOverItem->m_Num7);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
				//----------------------------------------------------------------------
				//-- �ϵ�
				else if(GET_SHAPE(pOverItem)==ISUB_WAND)
				{
					if( pOverItem->m_Num0 && pOverItem->m_Num1 )
					{
						//-- IDS_INVEN_DAM_MAGIC : ����������: %d-%d
						sprintf(strTemp, G_STRING(IDS_INVEN_DAM_MAGIC), pOverItem->m_Num0,pOverItem->m_Num1);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					switch(pOverItem->m_Num2) //-- �Ӽ�
					{
					case 1://-- IDS_INVEN_DAM_DURAITON : ����������: %d
						sprintf(strTemp, G_STRING(IDS_INVEN_DAM_DURAITON), pOverItem->m_Num8); // ����������
						multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					case 2://-- IDS_INVEN_RATE_COLD : �ݵ�Ȯ��: %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RATE_COLD), pOverItem->m_Num8);
						multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					case 3://-- IDS_INVEN_RATE_CHAOS : ȥ��Ȯ��: %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RATE_CHAOS), pOverItem->m_Num8);
						multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					case 4://-- IDS_INVEN_DEATH : �ֺ����� ���
						//sprintf(strTemp, G_STRING(IDS_INVEN_DEATH));
						//multiText.AddString(strTemp, _FCOLOR_SKY);
						break;
					default:
						break;
					}
					//mungmae-2006/4/26 �Ϲ� ���������� ���� ������ �������� ���̰� �Ѵ�.
					if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
							&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
					{
						//-- IDS_INVEN_REMAIN_MAGIC : ��������: %d/%d
						sprintf(strTemp, G_STRING(IDS_INVEN_REMAIN_MAGIC), pOverItem->m_Num3, pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					else
					{
						//-- IDS_INVEN_REMAIN_MAGIC : ��������: %d/%d
						sprintf(strTemp, G_STRING(IDS_INVEN_REMAIN_MAGIC), pOverItem->m_PlusNum, pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					if( pOverItem->m_Num4 )
					{
						//-- IDS_INVEN_USE_MANA : �����Ҹ�: %d
						sprintf(strTemp, G_STRING(IDS_INVEN_USE_MANA), pOverItem->m_Num4);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					if( pOverItem->m_Num5 )
					{
						//-- IDS_INVEN_DIST_ATT : ���ݰŸ�: %d
						sprintf(strTemp, G_STRING(IDS_INVEN_DIST_ATT), pOverItem->m_Num5);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
				}
				//----------------------------------------------------------------------
				//-- �����
				else if(GET_SHAPE(pOverItem)==ISUB_BOOK)
				{
					if(pOverItem->m_Vnum ==801 ||pOverItem->m_Vnum ==802)
						//-- IDS_INVEN_REC_SCROLL_4 : 4���������� ��ũ���� ����Ѵ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REC_SCROLL_4));
					else if(pOverItem->m_Vnum ==803 ||pOverItem->m_Vnum ==804)
						//-- IDS_INVEN_REC_SCROLL_5 : 5���������� ��ũ���� ����Ѵ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REC_SCROLL_5));
					else if(pOverItem->m_Vnum ==805 ||pOverItem->m_Vnum ==806)
						//-- IDS_INVEN_REC_SCROLL_6 : 6���������� ��ũ���� ����Ѵ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REC_SCROLL_6));

					multiText.AddString(strTemp, _FCOLOR_SKY);

					switch (pOverItem->m_Special)
					{
					case 780://-- IDS_INVEN_SCROLL_0 : 1Lv.����������
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_0));
						break;
					case 781://-- IDS_INVEN_SCROLL_1 : 3Lv.����������
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_1));
						break;
					case 782://-- IDS_INVEN_SCROLL_2 : 5Lv.����������
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_2));
						break;
					case 783://-- IDS_INVEN_SCROLL_3 : 1Lv.����
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_3));
						break;
					case 784://-- IDS_INVEN_SCROLL_4 : 3Lv.����
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_4));
						break;
					case 785://-- IDS_INVEN_SCROLL_5 : 5Lv.����
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_5));
						break;
					case 786://-- IDS_INVEN_SCROLL_6 : 1Lv.ȥ��
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_6));
						break;
					case 787://-- IDS_INVEN_SCROLL_7 : 3Lv.ȥ��
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_7));
						break;
					case 788://-- IDS_INVEN_SCROLL_8 : 5Lv.ȥ��
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_8));
						break;
					case 789://-- IDS_INVEN_SCROLL_9 : 1Lv.��Ȧ
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_9));
						break;
					case 790://-- IDS_INVEN_SCROLL_10 : 3Lv.��Ȧ
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_10));
						break;
					case 791://-- IDS_INVEN_SCROLL_11 : 5Lv.��Ȧ
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_11));
						break;
					case 792://-- IDS_INVEN_SCROLL_12 : 2Lv.�ݵ�
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_12));
						break;
					case 793://-- IDS_INVEN_SCROLL_13 : 4Lv.�ݵ�
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_13));
						break;
					case 794://-- IDS_INVEN_SCROLL_14 : 6Lv.�ݵ�
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_14));
						break;
					case 795://-- IDS_INVEN_SCROLL_15 : 2Lv.��ȭ
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_15));
						break;
					case 796://-- IDS_INVEN_SCROLL_16 : 4Lv.��ȭ
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_16));
						break;
					case 797://-- IDS_INVEN_SCROLL_17 : 6Lv.��ȭ
						strcpy(strTemp2, G_STRING(IDS_INVEN_SCROLL_17));
						break;
					default://-- IDS_INVEN_EMPTY : �������
						strcpy(strTemp2, G_STRING(IDS_INVEN_EMPTY));
						break;
					}
					//-- IDS_INVEN_ORDER : �ֹ�����: %s
					sprintf(strTemp, G_STRING(IDS_INVEN_ORDER), strTemp2);
					multiText.AddString(strTemp, _FCOLOR_SKY);
					//-- IDS_INVEN_SCROLL : ��ũ��: %d/%d
					sprintf(strTemp, G_STRING(IDS_INVEN_SCROLL), pOverItem->m_PlusNum,pOverItem->m_Num3);
					multiText.AddString(strTemp, _FCOLOR_SKY);
					//-- IDS_INVEN_ANCIENT_LEV : ���� ����: %d
					sprintf(strTemp, G_STRING(IDS_INVEN_ANCIENT_LEV), pOverItem->m_Num7);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}

				//----------------------------------------------------------------------
				//-- �������
				else if(GET_SHAPE(pOverItem)==ISUB_S_EQUIP)
				{
					switch (pOverItem->m_Num2)
					{
					case 1://-- IDS_INVEN_ABLE_SHOW_MON : ������迭 ���͸� ���� �ִ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_ABLE_SHOW_MON));
						break;
					case 2://-- IDS_INVEN_REMOVE_HARDB : �ϵ�踮��迭�� �����Ҽ� �ִ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REMOVE_HARDB));
						break;
					case 3://-- IDS_INVEN_REMOVE_PURGE : �����迭�� �����Ҽ� �ִ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REMOVE_PURGE));
						break;
					case 4://-- IDS_INVEN_INC_VOLT : ��Ʈ ��Ʈ����ũ ���׷��� �÷��ش�.
						sprintf(strTemp, G_STRING(IDS_INVEN_INC_VOLT));
						break;
					case 5://-- IDS_INVEN_INC_VOLCANIC : ��ĳ�м� ���׷��� �÷��ش�.
						sprintf(strTemp, G_STRING(IDS_INVEN_INC_VOLCANIC));
						break;
					case 6://-- IDS_INVEN_INC_STONE : ��ȭ ���׷��� �÷��ش�.
						sprintf(strTemp, G_STRING(IDS_INVEN_INC_STONE));
						break;
					case 7://-- IDS_INVEN_INC_COLD : �ݵ� ���׷��� �÷��ش�.
						sprintf(strTemp, G_STRING(IDS_INVEN_INC_COLD));
						break;
					case 8://-- IDS_INVEN_REFLECTION_DAM : ���濡�� �ݻ� �������� �ش�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REFLECTION_DAM));
						break;
					case 9://-- IDS_INVEN_DEC_ELEC : �Ϸ�Ʈ�� �ǵ��� ����Ҹ��� ���ҽ�Ų��.
						sprintf(strTemp, G_STRING(IDS_INVEN_DEC_ELEC));
						break;
					case 10://-- IDS_INVEN_INC_ATT : ���⸦ ���� �Ҹ��Ͽ� ���ݷ��� ��½�Ų��.
						sprintf(strTemp, G_STRING(IDS_INVEN_INC_ATT));
						break;
					case 11://-- IDS_INVEN_TELEPORT : �������� �ڷ���Ʈ�� �Ѵ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_TELEPORT));
						break;
					case 12://-- IDS_INVEN_INVISIBLE : ���濡�� ������ �ʰ� �ȴ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_INVISIBLE));
						break;
					case 13://-- IDS_INVEN_VISIBLE : �ֺ��� ������ ���͸� ������ ���̰� �Ѵ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_VISIBLE));
						break;
					case 14://-- IDS_INVEN_INC_RESISTANT : ��� ���׷��� �ö󰣴�.
						sprintf(strTemp, G_STRING(IDS_INVEN_INC_RESISTANT));
						break;
					case 16://-- IDS_INVEN_REMOVE_CURSE : �ı�,���ְ迭�� �����Ҽ� �ִ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_REMOVE_CURSE));
						break;
					}
					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}
				//----------------------------------------------------------------------
				//-- �޸��� ���� ��ǰ
				else if(GET_SHAPE(pOverItem)==ISUB_COMPONENT)
				{
					//-- IDS_INVEN_ASSEMB_PARTS : �޸��� �����ǰ
					sprintf(strTemp, G_STRING(IDS_INVEN_ASSEMB_PARTS));
					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}

				if( GET_SHAPE(pOverItem)==ISUB_WAND
						|| GET_SHAPE(pOverItem)==ISUB_SLAYER
						|| GET_SHAPE(pOverItem)==ISUB_CHAKRAM
						|| GET_SHAPE(pOverItem)==ISUB_L_EQUIP )
				{
					switch (pOverItem->m_Speed)
					{
					case ISPEED_VERY_FAST: //-- IDS_INVEN_VERY_FAST
						strcpy(speed_str,G_STRING(IDS_INVEN_VERY_FAST));
						break;
					case ISPEED_FAST: //-- IDS_INVEN_FAST
						strcpy(speed_str,G_STRING(IDS_INVEN_FAST));
						break;
					case ISPEED_SLOW: //-- IDS_INVEN_SLOW
						strcpy(speed_str,G_STRING(IDS_INVEN_SLOW));
						break;
					case ISPEED_VERY_SLOW: //-- IDS_INVEN_VERY_SLOW
						strcpy(speed_str,G_STRING(IDS_INVEN_VERY_SLOW));
						break;
					default://-- IDS_INVEN_MID
						strcpy(speed_str,G_STRING(IDS_INVEN_MID));
						break;
					}
					//-- IDS_INVEN_ATT_SPEED
					sprintf(strTemp, G_STRING(IDS_INVEN_ATT_SPEED), speed_str); // ���� �ӵ�
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}

				if( GET_SHAPE(pOverItem)==ISUB_PANEL )
				{
					//-- IDS_INVEN_ATT : ���ݷ�: %d-%d
					sprintf(strTemp, G_STRING(IDS_INVEN_ATT), pOverItem->m_Num0,pOverItem->m_Num1);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					if(pOverItem->m_Special <=1)
						pOverItem->m_Special = 0;

					if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
							&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
					{
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Num3,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					else
					{
						sprintf(strTemp, G_STRING(IDS_INVEN_LASTING), pOverItem->m_Special,pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					sprintf(strTemp, G_STRING(IDS_INVEN_USE_ELEC), pOverItem->m_Num4);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
			}
			break;

		case ITYPE_ARMOR:
			{

				switch (race_flag)
				{
				case 0x0003: //-- IDS_INVEN_ONLY_BULKAN : ��ĭ�� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_BULKAN), RGB(255, 150, 150));
					break;
				case 0x000c: //-- IDS_INVEN_ONLY_KAI : ī�̸� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_KAI), RGB(255, 150, 150));
					break;
				case 0x0030: //-- IDS_INVEN_ONLY_AID : ���̵�Ƹ� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_AID), RGB(255, 150, 150));
					break;
				case 0x00c0: //-- IDS_INVEN_ONLY_HUMAN : �޸� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HUMAN), RGB(255, 150, 150));
					break;
				case 0x0300: //-- IDS_INVEN_ONLY_HIBRIDER : ���̺긮�� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HIBRIDER), RGB(255, 150, 150));
					break;
				case 0x0C00:
					multiText.AddString((char*)G_STRING(IDS_LHSTRING1942), RGB(255, 150, 150));
					break;
				}
				if( pOverItem->m_Vnum != 1330 && pOverItem->m_Vnum != 1358 && pOverItem->m_Vnum != 1504 )	//���� ����� ����Ȱ��� �������� ����� �� // 2000�ϱ�����̴� ��ġ�� ���� ������ �ƴϴ�. 2000���̶�� �� 200���ܴ�.
					GetSpecial(multiText, ITYPE_ARMOR, GET_SHAPE(pOverItem), pOverItem->m_Special, pOverItem->m_Special2);

				if( pOverItem->m_Vnum == 1504 ) // 2000�ϱ�����̴� ��ġ�� ���� ������ �ƴϴ�. 2000���̶�� �� 200���ܴ�.
				{
					for( int sp_idx = 0; sp_idx < SPECIAL_NECKLACE_NUM; sp_idx++ )
					{
						int special_level = (pOverItem->m_Special & g_NecklaceSpecialMask[sp_idx]) >> g_NecklaceSpecialShift[sp_idx];
						if (special_level > 0)
						{
							switch (sp_idx)
							{
							case SPECIAL_NECK_INC_VITAL:
								//-- IDS_SPECIAL_VITAL_INC : ����� %d ���
								sprintf(strTemp, G_STRING(IDS_SPECIAL_VITAL_INC), 200 );
								break;
							case SPECIAL_NECK_INC_MANA:
								//-- IDS_SPECIAL_MANA_INC : ������ %d ���
								sprintf(strTemp, G_STRING(IDS_SPECIAL_MANA_INC), 200 );
								break;
							case SPECIAL_NECK_INC_STAM:
								//-- IDS_SPECIAL_STA_INC : ���׹̳� %d ���
								sprintf(strTemp, G_STRING(IDS_SPECIAL_STA_INC), 200 );
								break;
							case SPECIAL_NECK_INC_EPOWER:
								//-- IDS_SPECIAL_ELECTRIC_INC : ����� %d ���
								sprintf(strTemp, G_STRING(IDS_SPECIAL_ELECTRIC_INC), 200 );
								break;
							}

							multiText.AddString( strTemp, RGB(100, 255, 100) );
						}
					}
				}

				if( pOverItem->m_Vnum == 901 )
					multiText.AddString( (char*)G_STRING(IDS_INVEN_ITEM_DECARDI_BRACELET), RGB(100, 255, 100) );

				// IDS_USINGNUM ���Ƚ��: %d/%d
				if( pOverItem->m_Vnum == 1330 )
				{
					sprintf(strTemp, G_STRING(IDS_USINGNUM), pOverItem->m_Special, 30 );
					multiText.AddString( strTemp, _FCOLOR_YELLOW );
				}

				if( pOverItem->m_Vnum == 1358 )
				{
					sprintf(strTemp, G_STRING(IDS_USINGNUM), pOverItem->m_Special, 5 );
					multiText.AddString( strTemp, _FCOLOR_YELLOW );
				}

				// �� ������ �����Ǹ�  ControlScore::GetItemDefense(CItem * pItem) �Լ��� ����������
				defense = pOverItem->m_Defense;
				i_defense = defense;

				//-- 2004. 03. 22  / Lyul
				//-- ������ ���̾� ��� �ν� ���� Big Fix
				if ( IS_2011_ITEM_ARMOR( pOverItem->m_Vnum ) )
				{
					switch(pOverItem->m_PlusNum)
					{
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 10) / 100);
						break;
					case 10:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 15) / 100);
						break;
					case 11:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 20) / 100);
						break;
					case 12:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 25) / 100);
						break;
					case 13:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 30) / 100);
						break;
					case 14:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 35) / 100);
						break;
					case 15:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 40) / 100);
						break;
					default:
						i_defense += ((i_defense * pOverItem->m_PlusNum * 10) / 100);
						break;
					}
				}
				else if(GET_ITEM_LEVEL(pOverItem)>=181)
				{
					//���̾� ���ϰ��
					float add_rate;
					add_rate = i_defense * (pOverItem->m_PlusNum *10)/100;
					i_defense += add_rate;
				}
				else
				{
					//���̾� ���� �ƴҰ��.
					if(pOverItem->m_Vnum==1087
							|| pOverItem->m_Vnum==922
							|| pOverItem->m_Vnum==933
							|| pOverItem->m_Vnum==934
							|| pOverItem->m_Vnum==935
							|| pOverItem->m_Vnum==1032  )
					{
						//0407 �������� ���׷��̵� �ȵ�.
					}
					else if (pOverItem->m_PlusNum >= 5)
					{
						i_defense *= 2;
					}
					else if (pOverItem->m_PlusNum > 0)
						i_defense += defense * float(plus_percent[pOverItem->m_PlusNum]) / 100.0f + 0.5f;
				}

				int defenseLV400 = DefenseValMgr::GetInstance()->GetDefense(pOverItem);
				if( defenseLV400 != 0 )
				{
					i_defense = defenseLV400;
				}

				///�Ź���ȭ�ý��� // �� ���� ������ ����. ���� ���¿� �⺻������ % ����ġ�� ���ϴ� ������� �ٲ����. (05-11-11 ����)
				//if( (pOverItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION) ) // �� ����ġ�� �ִ����� üũ. (�� ����ũ�� ��ȣ��Ʈ�� �� 4��Ʈ�� ���´�.)
				//{
				//	if( (pOverItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION_SIGN) ) // ��ȣ ��Ʈ�� ������...
				//		i_defense -= GET_DEFENSE(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // -�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
				//	else
				//		i_defense += GET_DEFENSE(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // +�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
				//}

				//-- IDS_INVEN_DEFANCE
				sprintf(strTemp, G_STRING(IDS_INVEN_DEFANCE), i_defense);
				multiText.AddString(strTemp, _FCOLOR_SKY);

				// nate 2005-04-14
				// ���̺긮�� ������ �ɼ� ����
				//if( pOverItem->m_Vnum== 875			// ��ĭ ��Ÿ
				//	|| pOverItem->m_Vnum== 1027		// ��ĭ ���̸�Ÿ
				//	|| pOverItem->m_Vnum== 1234		// ���̺긮�� ��Ÿ
				//	|| pOverItem->m_Vnum== 1235 )	// ���̺긮�� ���̸�Ÿ
				if( ItemFakeOptionMgr::GetInstance()->HasOption(eFakeOptionType_DEF70, pOverItem->m_Vnum) == true )
				{
					//-- IDS_INVEN_DEF_70 : �Ѽչ��� ���� ���� +70
					sprintf(strTemp, G_STRING(IDS_INVEN_DEF_70));
					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}
				//-- IDS_INVEN_DEFANCE_MANA_RATE : ��������� %d%% ���
				//if( pOverItem->m_Vnum== 876
				//	|| pOverItem->m_Vnum== 1028 )
				if( ItemFakeOptionMgr::GetInstance()->HasOption(eFakeOptionType_DEFMANARATE, pOverItem->m_Vnum) == true )
				{
					sprintf(strTemp, G_STRING( IDS_INVEN_DEFANCE_MANA_RATE), pOverItem->m_MagicDefense =10);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}

				//1113 ��ĭ�� ���̸�Ÿ����
				//if( pOverItem->m_Vnum== 1027		// ��ĭ ���̸�Ÿ
				//	|| pOverItem->m_Vnum== 876	// �޸� ��Ÿ
				//	|| pOverItem->m_Vnum== 875	// ��ĭ ��Ÿ
				//	|| pOverItem->m_Vnum== 1028	// �޸� ���̸�Ÿ
				//	|| pOverItem->m_Vnum== 1234	// ���̺긮�� ��Ÿ
				//	|| pOverItem->m_Vnum== 1235 )	// ���̺긮�� ���̸�Ÿ
				if( ItemFakeOptionMgr::GetInstance()->HasOption(eFakeOptionType_RESIST_NUM2, pOverItem->m_Vnum) == true )
				{
					//-- IDS_SKILL_INC_RESIS : ��ü ���׷�
					sprintf(strTemp, G_STRING(IDS_SKILL_INC_RESIS), pOverItem->m_Num2);
					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}

				//-- ���ڵ� ���� / ���� ���� / ī�� ��Ÿ ������ / ���̵�� ���� ����
				//-- ���̸�Ÿ ������,�Ͼ���� ����
				//if( pOverItem->m_Vnum== 912
				//	|| pOverItem->m_Vnum== 913
				//	|| pOverItem->m_Vnum== 573
				//	|| pOverItem->m_Vnum== 575
				//	|| pOverItem->m_Vnum== 1029
				//	|| pOverItem->m_Vnum== 1030
				//	|| pOverItem->m_Vnum== 1236	// ���̺긮�� ��Ÿ ������
				//	|| pOverItem->m_Vnum== 1237 )	// ���̺긮�� ���̸�Ÿ ������
				if( ItemFakeOptionMgr::GetInstance()->HasOption(eFakeOptionType_RESIST_NUM3, pOverItem->m_Vnum) == true )
				{
					//-- IDS_SKILL_INC_RESIS : ��ü ���׷�
					sprintf(strTemp, G_STRING(IDS_SKILL_INC_RESIS), pOverItem->m_Num3);
					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}

				if(GET_SHAPE(pOverItem) == IARMOR_RING
						&& pOverItem->m_Num2!=0)
				{
					//���̾� ���ϰ��
					switch (pOverItem->m_Num1)
					{
					case 0://-- IDS_INVEN_RESIS_VOLT : ��Ʈ ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLT),pOverItem->m_Num2);
						break;
					case 1://-- IDS_INVEN_RESIS_VOLCANIC : ��ĳ�� ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLCANIC),pOverItem->m_Num2);
						break;
					case 2://-- IDS_INVEN_RESIS_STONE : ��ȭ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_STONE),pOverItem->m_Num2);
						break;
					case 3://-- IDS_INVEN_RESIS_COLD : �ݵ����׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_COLD),pOverItem->m_Num2);
						break;
					}

					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}
				//-- 2004. 03. 22  / Lyul
				//-- ������ ���̾� ��� �ν� ���� Big Fix
				else if(GET_ITEM_LEVEL(pOverItem)>=181
						&& GET_SHAPE(pOverItem) != IARMOR_SHIELD
						&& GET_SHAPE(pOverItem) != IARMOR_CLOAK
						&& GET_SHAPE(pOverItem) != IARMOR_RING
						&& GET_SHAPE(pOverItem) != IARMOR_NECKLACE // ����̵� ���׷µ� �Ⱥٰ� �߰� ( 06-04-01 ���� )
						&& GET_SHAPE(pOverItem) != IARMOR_BRACELET)
				{
					//���̾� ���ϰ��
					switch (pOverItem->m_Num1)
					{
					case 0://-- IDS_INVEN_RESIS_VOLT : ��Ʈ ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLT),pOverItem->m_Num2);
						break;
					case 1://-- IDS_INVEN_RESIS_VOLCANIC : ��ĳ�� ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLCANIC),pOverItem->m_Num2);
						break;
					case 2://-- IDS_INVEN_RESIS_STONE : ��ȭ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_STONE),pOverItem->m_Num2);
						break;
					case 3://-- IDS_INVEN_RESIS_COLD : �ݵ����׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_COLD),pOverItem->m_Num2);
						break;
					}

					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}

				//  [6/22/2009 ppmmjj83] �Ϻ� ������ ��Ʈ�� �ɼ� �߰�
				else if( g_dwClientCountry == CTRY_JPN
						 && ( pOverItem->m_Vnum >= 2320 && pOverItem->m_Vnum <= 2331 )
					   )
				{
					switch (pOverItem->m_Num1)
					{
					case 0://-- IDS_INVEN_RESIS_VOLT : ��Ʈ ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLT),pOverItem->m_Num2);
						break;
					case 1://-- IDS_INVEN_RESIS_VOLCANIC : ��ĳ�� ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLCANIC),pOverItem->m_Num2);
						break;
					case 2://-- IDS_INVEN_RESIS_STONE : ��ȭ���׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_STONE),pOverItem->m_Num2);
						break;
					case 3://-- IDS_INVEN_RESIS_COLD : �ݵ����׷� %d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_COLD),pOverItem->m_Num2);
						break;
					}

					multiText.AddString(strTemp, _FCOLOR_GREEN);
				}

				////////
				if (GET_SHAPE(pOverItem) == IARMOR_CLOAK)
				{
					if (pOverItem->m_MagicDefense > 0)
					{
						//-- IDS_INVEN_DEFANCE_MANA_RATE : ��������� %d%% ���
						sprintf(strTemp, G_STRING( IDS_INVEN_DEFANCE_MANA_RATE), pOverItem->m_MagicDefense);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					if (GET_REDUCE_MANA_USAGE(pOverItem) > 0)
					{
						if(race_flag == 0x0030)//1217 ���̵�� �������...
							//-- IDS_INVEN_PHYSCAL_DAM : �����������ǵ� +%d%%
							sprintf(strTemp, G_STRING(IDS_INVEN_PHYSCAL_DAM) , GET_REDUCE_MANA_USAGE(pOverItem));
						else
							//-- IDS_INVEN_MANA_USE : �����¼Ҹ� -%d%%
							sprintf(strTemp, G_STRING(IDS_INVEN_MANA_USE), GET_REDUCE_MANA_USAGE(pOverItem));

						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					if (GET_REFLECT_DAM(pOverItem) > 0)
					{
						//-- IDS_INVEN_DAM_REFLECTION2 : ���� �ݻ� ������
						sprintf(strTemp, G_STRING(IDS_INVEN_DAM_REFLECTION2), GET_REFLECT_DAM(pOverItem));
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
				}
			}
			break;

		case ITYPE_WEAPON:
			{


				switch (race_flag)
				{
				case 0x0003: //-- IDS_INVEN_ONLY_BULKAN : ��ĭ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_BULKAN), _FCOLOR_GREEN);
					break;
				case 0x000c: //-- IDS_INVEN_ONLY_KAI : ī������
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_KAI), _FCOLOR_GREEN);
					break;
				case 0x0030: //-- IDS_INVEN_ONLY_AID : ���̵������
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_AID), _FCOLOR_GREEN);
					break;
				case 0x00c0: //-- IDS_INVEN_ONLY_HUMAN : �޸� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HUMAN), _FCOLOR_GREEN);
					break;
				case 0x0300: //-- IDS_INVEN_ONLY_HIBRIDER : ���̺긮�� ����
					multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HIBRIDER), _FCOLOR_GREEN);
					break;
				case 0x0C00:
					multiText.AddString((char*)G_STRING(IDS_LHSTRING1942), _FCOLOR_GREEN);
					break;
				}


				GetSpecial(multiText, ITYPE_WEAPON, GET_SHAPE(pOverItem), pOverItem->m_Special, pOverItem->m_Special2);
				dam1 = pOverItem->m_Dam1;
				dam2 = pOverItem->m_Dam2;

				int upgrade_type = LHIS_ENCHANT()->IsUpgadeType(pOverItem->m_Vnum);

				if( -1 != upgrade_type )
				{
					const LHParam_EnchantItem_Ex::sData * param_ex = LHIS_ENCHANT_EX()->GetParam(upgrade_type, pOverItem->m_PlusNum-1);

					if( param_ex )
					{
						dam1 = (dam1*param_ex->m_upgrade_addvalue)*0.001f;
						dam2 = (dam2*param_ex->m_upgrade_addvalue)*0.001f;
					}
					else
					{
						dam1 = pOverItem->m_Dam1;
						dam2 = pOverItem->m_Dam2;
					}
				}
				else
				{
					if ( IS_2011_ITEM_WEAPON( pOverItem->m_Vnum ) )
					{
						dam1 += (dam1 * pOverItem->m_PlusNum *1.37+50)/100;
						dam2 += (dam2 * pOverItem->m_PlusNum *1.37+50)/100;
					}
					else if( CheckIsConversionItem(pOverItem) ) // ������ ����� ����� �ٸ���...
					{
						dam1 += dam1 * (pOverItem->m_PlusNum *4)/100;
						dam2 += dam2 * (pOverItem->m_PlusNum *4)/100;
					}
					else if( CheckIsConversionItem(pOverItem, 1) )
					{
						dam1 += dam1 * (pOverItem->m_PlusNum *1.37)/100;
						dam2 += dam2 * (pOverItem->m_PlusNum *1.37)/100;
					}
					else if( CheckIsConversionItem(pOverItem, 2) )
					{
						dam1 += dam1 * (pOverItem->m_PlusNum *1.37)/100;
						dam2 += dam2 * (pOverItem->m_PlusNum *1.37)/100;
					}
					//-- ����Ʈ ���̾� ���� ��� ������ 11% �����
					else if(GET_ITEM_LEVEL(pOverItem) >=261 )
					{
						dam1 += floor(dam1 * (pOverItem->m_PlusNum *11)/100);
						dam2 += floor(dam2 * (pOverItem->m_PlusNum *11)/100);
					}
					//-- ���̾� ���� ��� ������ 10% �����
					else if(GET_ITEM_LEVEL(pOverItem) >=181 )
					{
						dam1 += dam1 * (pOverItem->m_PlusNum *10)/100;
						dam2 += dam2 * (pOverItem->m_PlusNum *10)/100;
						// �ݿ˸�
						dam1 += 0.5f;
						dam2 += 0.5f;
					}
					else
					{
						//-- �Ϲ� ������ ���.
						if (pOverItem->m_PlusNum >= 5)
						{
							dam1 *= 2;
							dam2 *= 2;
						}
						else if (pOverItem->m_PlusNum > 0)
						{
							dam1 += dam1 * plus_percent[pOverItem->m_PlusNum] / 100 + 0.5f;
							dam2 += dam2 * plus_percent[pOverItem->m_PlusNum] / 100 + 0.5f;
						}
					}
				}

				// ���⵵ ���� ����ġ �ɼ��� ��.(���� ���ݷ¿� ���� �Էµ� �⺻���� �� % �� �����ش�.)
				//if( (pOverItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION) ) // �� ����ġ�� �ִ����� üũ. (�� ����ũ�� ��ȣ��Ʈ�� �� 4��Ʈ�� ���´�.)
				//{
				//	if( (pOverItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION_SIGN) ) // ��ȣ ��Ʈ�� ������...
				//	{
				//		dam1 -= GET_DAM1(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // -�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
				//		dam2 -= GET_DAM2(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // -�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
				//	}
				//	else
				//	{
				//		dam1 += GET_DAM1(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // +�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
				//		dam2 += GET_DAM2(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // +�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
				//	}
				//}

				// nate 2005-05-20 : Ȧ���δ�
				if( pOverItem->m_Vnum == 1259
						|| pOverItem->m_Vnum == 1260
						|| pOverItem->m_Vnum == 1261
						|| pOverItem->m_Vnum == 2170	// [2008/11/11 Theodoric] WORLD_MATRIX_2009 Ȧ�� ������ (����)
						|| pOverItem->m_Vnum == 2171	// [2008/11/11 Theodoric] WORLD_MATRIX_2009 Ȧ�� ������ (�ȱ�)
				  )
				{
					//-- IDS_INVEN_HEAL : ġ�� �ɷ�ġ
					sprintf(strTemp, G_STRING(IDS_INVEN_HEAL), (int)dam1, (int)dam2);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
				else if (GET_RENDER_FLAG(pOverItem) > 0 && GET_SHAPE(pOverItem) != IWEAPON_GUN)
				{
					//-- IDS_INVEN_TOWHAND_DAMAGE : �μ� ����
					sprintf(strTemp, G_STRING(IDS_INVEN_TOWHAND_DAMAGE), (int)dam1, (int)dam2);
					multiText.AddString(strTemp, _FCOLOR_SKY);

					//-- IDS_INVEN_ONEHNAD_DAMAGE : �Ѽ� ����
					sprintf(strTemp, G_STRING(IDS_INVEN_ONEHNAD_DAMAGE), ((int)dam1)*2/3, ((int)dam2)*2/3);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}
				else
				{
					//-- IDS_INVEN_DAMAGE : ������
					sprintf(strTemp, G_STRING(IDS_INVEN_DAMAGE), (int)dam1, (int)dam2);
					multiText.AddString(strTemp, _FCOLOR_SKY);
				}

				if (GET_WATT(pOverItem) > 0)
				{
					//-- GUN
					if (GET_SHAPE(pOverItem) == IWEAPON_GUN)
						//-- IDS_INVEN_ELECT_USE
						sprintf(strTemp, G_STRING(IDS_INVEN_ELECT_USE), GET_WATT(pOverItem));
					// ��
					else
						//-- IDS_INVEN_KAI_MANA_USE
						sprintf(strTemp, G_STRING(IDS_INVEN_KAI_MANA_USE), GET_WATT(pOverItem));

					multiText.AddString(strTemp, _FCOLOR_SKY);
				}

				switch (pOverItem->m_Speed)
				{
				case ISPEED_VERY_FAST://-- IDS_INVEN_VERY_FAST
					strcpy(speed_str,G_STRING(IDS_INVEN_VERY_FAST));
					break;
				case ISPEED_FAST: //-- IDS_INVEN_FAST
					strcpy(speed_str,G_STRING(IDS_INVEN_FAST));
					break;
				case ISPEED_SLOW: //-- IDS_INVEN_SLOW
					strcpy(speed_str,G_STRING(IDS_INVEN_SLOW));
					break;
				case ISPEED_VERY_SLOW: //-- IDS_INVEN_VERY_SLOW
					strcpy(speed_str,G_STRING(IDS_INVEN_VERY_SLOW));
					break;
				default://-- IDS_INVEN_MID
					strcpy(speed_str,G_STRING(IDS_INVEN_MID));
					break;
				}
				//-- IDS_INVEN_ATT_SPEED
				sprintf(strTemp, G_STRING(IDS_INVEN_ATT_SPEED), speed_str);
				multiText.AddString(strTemp, _FCOLOR_SKY);

				//----------------------------------------------------------------------
				//-- ������.
				if (GET_SHAPE(pOverItem) == IWEAPON_STAFF)
				{
					if (pOverItem->m_MagicAttackSpeed > 0)
					{
						//-- IDS_INVEN_MANA_SPEED
						sprintf(strTemp,G_STRING(IDS_INVEN_MANA_SPEED), pOverItem->m_MagicAttackSpeed);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}

					if (pOverItem->m_MagicDamBonus > 0)
					{
						// �������� ���Ͽ� �ö󰡴� ������ ������ % �÷��� ��ġ�� ���Ѵ�.(05-09-23 ����)
						int masuk_dam_per_plus = 0; // ���� �ɼ����� ���Ͽ� �ö󰡴� ���������� ������ % �÷�����.

						for (int sp_idx = SPECIAL_STAFF_NUM-1; sp_idx >= 0; sp_idx--) // �ϴ� �� ���������� ���鼭 ����.
						{
							int special_level = (pOverItem->m_Special & g_WeaponSpecialMask[sp_idx]) >> g_WeaponSpecialShift[sp_idx]; // pOverItem->m_Special�� SetSpecialValue() �� special ���ϰ� Ʋ�� ��찡 �ִٸ� ��� ����� ������ ����������� ����.
							if (special_level > 0)
							{
								switch (sp_idx)
								{
								case SPECIAL_DAM_PER:
									masuk_dam_per_plus += g_WeaponSpecialValue[sp_idx][special_level-1]; // �������� �� ����� SetSpecialValue() �Լ����� m_MagicDamBonus�� �������ѹ��ȴ�. �׷��� �߸��� ���� ��µǾ�����. ( 05-09-23 ���� )
									break;
								}
							}
						}

						/////////////////////////////////////////////////////////////////////////////////////////////////////
						// �����̴� ���·� ����ġ�� ���������� �ʿ��� ��������ش�.
						int Temp_MagicDam = pOverItem->m_MagicDamBonus;

						//if( (pOverItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION) ) // �� ����ġ�� �ִ����� üũ. (�� ����ũ�� ��ȣ��Ʈ�� �� 4��Ʈ�� ���´�.)(�����̴� �� �������� �ƴ϶� �ٸ��ſ� ����.)
						//{
						//	if( (pOverItem->m_Special2 & SPECIAL2_MASK_DEF_REVISION_SIGN) ) // ��ȣ ��Ʈ�� ������...
						//	{
						//		Temp_MagicDam -= GET_MAGICDAM_BONUS(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // -�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
						//	}
						//	else
						//	{
						//		Temp_MagicDam += GET_MAGICDAM_BONUS(pOverItem)*(pOverItem->m_Special2&SPECIAL2_MASK_DEF_REVISION)*0.01f; // +�ۼ�Ʈ. �⺻ ������ �������� ����ؼ� �����ش�.
						//	}
						//}

						magic_dam_bonus = Temp_MagicDam;

						int upgrade_type = LHIS_ENCHANT()->IsUpgadeType(pOverItem->m_Vnum);
						if( -1 != upgrade_type )
						{
							const LHParam_EnchantItem_Ex::sData * param_ex = LHIS_ENCHANT_EX()->GetParam(upgrade_type, pOverItem->m_PlusNum-1);

							if( param_ex )
							{
								magic_dam_bonus = GET_MAGICDAM_BONUS(pOverItem) * param_ex->m_upgrade_addvalue  * 0.001f;
							}
						}
						else							
						{
							//-- ������ ���� ��� ������ 4% �����

							if( CheckIsConversionItem(pOverItem) ) // ������ ����� ����� �ٸ���...
								magic_dam_bonus += GET_MAGICDAM_BONUS(pOverItem) * (pOverItem->m_PlusNum *35)/1000; // 3.5% �� ����.(������ �ȹ��� �༮�� ��������..)
							else if( CheckIsConversionItem(pOverItem,1) ) // ������ ����� ����� �ٸ���...
								magic_dam_bonus += GET_MAGICDAM_BONUS(pOverItem) * (pOverItem->m_PlusNum *125)/10000; // 1.25% �� ����.(������ �ȹ��� �༮�� ��������..)
							else if( CheckIsConversionItem(pOverItem,2) ) // ������ ����� ����� �ٸ���...
								magic_dam_bonus += GET_MAGICDAM_BONUS(pOverItem) * (pOverItem->m_PlusNum *125)/10000; // 1.25% �� ����.(������ �ȹ��� �༮�� ��������..)
							//-- 2004. 03. 22  / Lyul
							//-- ����Ʈ �� ������
							else if(GET_ITEM_LEVEL(pOverItem) >=261 )
								magic_dam_bonus = magic_dam_bonus + (pOverItem->m_PlusNum * 26);
							//-- ���̾Ʊ� ������.
							else if(GET_ITEM_LEVEL(pOverItem) >=181 )
								magic_dam_bonus = magic_dam_bonus +
												  (magic_dam_bonus * plus_percent_dia[pOverItem->m_PlusNum] / 57);
							// 5������ �ص� ������...
							else if (pOverItem->m_PlusNum > 0 && pOverItem->m_PlusNum < 10)
								magic_dam_bonus = magic_dam_bonus +
												  (magic_dam_bonus * plus_percent[pOverItem->m_PlusNum] / 57);
						}

						magic_dam_bonus += masuk_dam_per_plus; // ���� �ɼ����� ���Ͽ� �ö� ���� ���������ش�. (05-09-23 ����)

						//-- IDS_INVEN_MANA_DAMAGE : ���������� +%d%%
						sprintf(strTemp, G_STRING(IDS_INVEN_MANA_DAMAGE), (int)magic_dam_bonus);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
				}
			}
			break;

		case ITYPE_ETC:
			{
				switch (GET_SHAPE(pOverItem))
				{
				case IETC_RECOVERY:
					{
						switch (GET_APPTO(pOverItem))
						{
						case 0:
							sprintf(strTemp,G_STRING(IDS_INVEN_RECOVERY_VATAL), GET_RECVAL(pOverItem));
							break;
						case 1:
							sprintf(strTemp, G_STRING(IDS_INVER_RECOVERY_MANA), GET_RECVAL(pOverItem));
							break;
						case 2:
							sprintf(strTemp, G_STRING(IDS_INVEN_RECOVERY_STA), GET_RECVAL(pOverItem));
							break;
						case 3:
							sprintf(strTemp, G_STRING(IDS_INVEN_RECOVERY_ELECT), GET_RECVAL(pOverItem));
							break;
						case 4:
							break;
						case 5:
							sprintf(strTemp, G_STRING(IDS_INVEN_ALL_RECOVERY), GET_RECVAL(pOverItem));
							break;
						case 6:
							sprintf(strTemp, G_STRING(IDS_LHSTRING1872) );
							break;
						case 7:
							sprintf(strTemp, G_STRING(IDS_LHSTRING1873) );
							break;
						case 8:
							sprintf(strTemp,G_STRING(IDS_INVEN_RECOVERY_VATAL), GET_RECVAL(pOverItem));
							strcat( strTemp , "%%" );
							break;
						case 9:
							sprintf(strTemp, G_STRING(IDS_INVER_RECOVERY_MANA), GET_RECVAL(pOverItem));
							strcat( strTemp , "%%" );
							break;
						case 10:
							sprintf(strTemp, G_STRING(IDS_INVEN_RECOVERY_STA), GET_RECVAL(pOverItem));
							strcat( strTemp , "%%" );
							break;
						case 11:
							sprintf(strTemp, G_STRING(IDS_INVEN_RECOVERY_ELECT), GET_RECVAL(pOverItem));
							strcat( strTemp , "%%" );
							break;
						case 12:
							sprintf(strTemp, G_STRING(IDS_LHSTRING1874), GET_RECVAL(pOverItem));
							break;
						case 13:
							sprintf(strTemp, G_STRING(IDS_LHSTRING1875), GET_RECVAL(pOverItem)); // % ȸ��
							break;
						}
						multiText.AddString(strTemp, _FCOLOR_WHITE);

						if( pOverItem->m_Num9 )
						{
							if( g_pNk2DFrame->IsShopVisible() && !g_pRoh->FindItemByIndexFromAll(pOverItem->m_Index) )
							{
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Num9);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
							else
							{
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Special);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
						}
					}
					break;

				case IETC_GEMSTONE:
					{
						//-- IDS_INVEN_ENABLE_SEGONG_LEVEL
						sprintf(strTemp, G_STRING(IDS_INVEN_ENABLE_SEGONG_LEVEL), GET_ITEM_LEVEL(pOverItem));
						multiText.AddString(strTemp, _FCOLOR_RED);

						//-- IDS_INVEN_NEED_KIT_NUM
						sprintf(strTemp, G_STRING(IDS_INVEN_NEED_KIT_NUM), GET_NEEDKIT_NUM(pOverItem));
						multiText.AddString(strTemp, _FCOLOR_WHITE);
					}
					break;

				case IETC_GEM:
					{
						//-- 2004. 03. 22  / Lyul
						//-- ������ ���̾� ��� �ν� ���� Big Fix
						if(GET_ITEM_LEVEL(pOverItem) >= 181)
							//1217 ���̾ƴ� max�� ����.
							//-- IDS_INVEN_UPGRADE_LEVEL : ���׷��̵� ���� ������ ����: %d�̻�
							sprintf(strTemp, G_STRING(IDS_INVEN_UPGRADE_LEVEL), GET_GEMMIN_LEVEL(pOverItem));
						else
							//-- IDS_INVEN_UPGRADE_ITEM_LEVEL : ���׷��̵� ���� ������ ����: %d-%d
							sprintf(strTemp, G_STRING(IDS_INVEN_UPGRADE_ITEM_LEVEL), GET_GEMMIN_LEVEL(pOverItem), GET_GEMMAX_LEVEL(pOverItem));

						multiText.AddString(strTemp, _FCOLOR_WHITE);
					}
					break;

				case IETC_GIFT:
					{
						//-- IDS_INVEN_GIFT01 : ���ϵ帳�ϴ�!!!
						sprintf(strTemp, G_STRING(IDS_INVEN_GIFT01));
						multiText.AddString(strTemp, _FCOLOR_RED);
						//-- IDS_INVEN_GIFT02 : �̺�Ʈ ��ǰ�Դϴ�.
						sprintf(strTemp, G_STRING(IDS_INVEN_GIFT02));
						multiText.AddString(strTemp, _FCOLOR_WHITE);
						//-- IDS_INVEN_GIFT03 : Ȩ�������� ���ż� ���ɹ�����
						sprintf(strTemp, G_STRING(IDS_INVEN_GIFT03));
						multiText.AddString(strTemp, _FCOLOR_WHITE);
						//-- IDS_INVEN_GIFT04 : �ּҸ� �Է����ּ���
						sprintf(strTemp, G_STRING(IDS_INVEN_GIFT04));
						multiText.AddString(strTemp, _FCOLOR_WHITE);
					}
					break;

				//--------------------------------------------------------------------
				//-- QUICK SLOT �� Item
				case IETC_WARP:
					{
					}
					break;

				case IETC_BUFF:
					{
						switch( pOverItem->m_Num0 )
						{
						case 0:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_00) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 1:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_01) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 2:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_02) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 3:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_03) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 4:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_04) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 5:
								sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_05) , pOverItem->m_Num2 , pOverItem->m_Num3 );

							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 6:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_06) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 7:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_07) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 8:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_08) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 9:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_09) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 10:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_10) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 11:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_11) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 12:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_12) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 13:
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_13) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							break;
						case 14:
							sprintf(strTemp2, G_STRING(IDS_BUFF_INFO_14) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 15:
							sprintf(strTemp2, G_STRING(IDS_LHSTRING1828) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 16: // �̼�����
							sprintf(strTemp2, G_STRING(IDS_LHSTRING1864) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 17: // ���̿����Ǽ�
							sprintf(strTemp2, G_STRING(IDS_LHSTRING1876) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 18: // ��Ƽ��!?
							sprintf(strTemp2, G_STRING(IDS_LHSTRING1877) , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						case 19:
							sprintf(strTemp2, G_STRING(IDS_LHSTRING1943) );// , pOverItem->m_Num2 , pOverItem->m_Num3 );
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						default:
							sprintf(strTemp2, "not defined!!!");
							multiText.AddString(strTemp2, _FCOLOR_WHITE);
							break;
						}

						if( pOverItem->m_Num9 )
						{
							if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
									&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
							{
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Num9);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
							else
							{
								if( g_pNk2DFrame->IsShopVisible() && !g_pRoh->FindItemByIndexFromAll(pOverItem->m_Index) )
								{
									sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Num9);
									multiText.AddString(strTemp, _FCOLOR_SKY);
								}
								else
								{
									sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Special);
									multiText.AddString(strTemp, _FCOLOR_SKY);
								}
							}
						}

					}
					break;
				case IETC_SUMMONNPC:
					{
						if( pOverItem->m_Num9 )
						{
							if( g_pNk2DFrame->IsShopVisible() && !g_pRoh->FindItemByIndexFromAll(pOverItem->m_Index) )
							{
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Num9);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
							else
							{
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Special);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
						}
					}
					break;
				case IETC_RESOURCE:
					// ��������
					if (pOverItem->m_Vnum == 495 && g_dwClientCountry != CTRY_JPN ) // �Ϻ��� �� �������� ���� ����̴�.
					{
						//-- IDS_INVEN_QUICK02
						sprintf(strTemp, G_STRING(IDS_INVEN_QUICK02));
						multiText.AddString(strTemp, _FCOLOR_WHITE);
					}
					break;

				case IETC_QUEST:
					{
						if (pOverItem->m_Vnum == 926 || pOverItem->m_Vnum == 936 )
						{
							//mungmae-2006/4/26 �Ϲ� ���������� ���� ������ �������� ���̰� �Ѵ�.
							if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
									&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
							{
								//-- IDS_INVEN_LEFT_CNT : ���� ���� : %d
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Num3);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
							else
							{
								//-- IDS_INVEN_LEFT_CNT : ���� ���� : %d
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Special);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
						}
					}
					break;
				case IETC_PET:
					{	
						if( pOverItem->m_Num0 != 0 )
						{
							switch(pOverItem->m_Num1)
							{
							case 0:
								sprintf(strTemp, G_STRING(IDS_LHSTRING1879), pOverItem->m_Num3);
								break;
							case 1:
								sprintf(strTemp, G_STRING(IDS_LHSTRING1880), pOverItem->m_Num2, pOverItem->m_Num3);
								break;
							case 2:
								sprintf(strTemp, G_STRING(IDS_LHSTRING1881), pOverItem->m_Num2, pOverItem->m_Num3);
								break;
							case 3:
								sprintf(strTemp, G_STRING(IDS_LHSTRING1882), pOverItem->m_Num2, pOverItem->m_Num3);
								break;
							}
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else
						{

						}
					}
					break;
				case IETC_QUICK:
					{
					}
					break;

				case IETC_ETC:
					{
						//-- ���� õ���� �����
						if( pOverItem->m_Vnum == 966 )
						{
							//-- IDS_MELEE_AVOID10 : ���� ���� ȸ�Ƿ�
							sprintf(strTemp, G_STRING(IDS_MELEE_AVOID10));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
							//-- IDS_SP_MAGIC_AVOID30 : Ư�� ���� ȸ�Ƿ�
							sprintf(strTemp, G_STRING(IDS_SP_MAGIC_AVOID30));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						//-- ���� ������ �����
						else if( pOverItem->m_Vnum == 967 )
						{
							//-- IDS_MELEE_AVOID20
							sprintf(strTemp,  G_STRING(IDS_MELEE_AVOID20));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
							//-- IDS_SP_MAGIC_AVOID10
							sprintf(strTemp, G_STRING(IDS_SP_MAGIC_AVOID10));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						//-- ������� ���� �߰� �ֹ��� - 2004.10.18 / Lyul �߰�.
						else if(pOverItem->m_Vnum == 1088 )
						{
							//-- IDS_EVOLBR_VITAL : ���������� ����� �ɼ��� �ΰ��ϴ� ������
							sprintf(strTemp, G_STRING(IDS_EVOLBR_VITAL));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1089 )
						{
							//-- IDS_EVOLBR_MANA
							sprintf(strTemp, G_STRING(IDS_EVOLBR_MANA));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1090 )
						{
							//-- IDS_EVOLBR_ELEC
							sprintf(strTemp, G_STRING(IDS_EVOLBR_ELEC));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1091 )
						{
							//-- IDS_EVOLBR_STAM
							sprintf(strTemp, G_STRING(IDS_EVOLBR_STAM));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1092 )
						{
							//-- IDS_EVOLBR_REF
							sprintf(strTemp, G_STRING(IDS_EVOLBR_REF));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1093 )
						{
							//-- IDS_EVOLBR_ABS
							sprintf(strTemp, G_STRING(IDS_EVOLBR_ABS));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1094 )
						{
							//-- IDS_EVOLBR_RESI
							sprintf(strTemp, G_STRING(IDS_EVOLBR_RESI));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1095 )
						{
							//-- IDS_EVOLBR_CHAOS
							sprintf(strTemp, G_STRING(IDS_EVOLBR_CHAOS));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1096 )
						{
							//-- IDS_EVOLBR_DEF_M
							sprintf(strTemp, G_STRING(IDS_EVOLBR_DEF_M));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
						else if(pOverItem->m_Vnum == 1097 )
						{
							//-- IDS_EVOLBR_RECOV
							sprintf(strTemp, G_STRING(IDS_EVOLBR_RECOV));
							multiText.AddString(strTemp, _FCOLOR_GREEN);
						}
					}
					break;
				case IETC_CHARGE:
					{
						SetItemInfo_Tribe(pOverItem , multiText);

						if( pOverItem->m_Vnum == 739 || pOverItem->m_Vnum == 807 )
						{
							//-- IDS_INVEN_INC_DURAION
							sprintf( strTemp, G_STRING(IDS_INVEN_INC_DURAION) );
							multiText.AddString( strTemp, _FCOLOR_SKY );
						}
						else if( pOverItem->m_Vnum == 1005 || pOverItem->m_Vnum == 1006 )
						{
							//-- IDS_INVEN_INC_DURAION_CHAKRAM
							sprintf( strTemp,  G_STRING(IDS_INVEN_INC_DURAION_CHAKRAM) );
							multiText.AddString( strTemp, _FCOLOR_SKY );
						}
						//mungmae-2005/05/03
						else if( pOverItem->m_Vnum == 1160)
						{
							//-- IDS_INVEN_INC_DSTONE-����� ������ �������� �÷��ش�.
							sprintf( strTemp,  G_STRING(IDS_INVEN_INC_DSTONE) );
							multiText.AddString( strTemp, _FCOLOR_SKY );
						}
						else if( pOverItem->m_Vnum == 3149 || pOverItem->m_Vnum == 3150 )
						{
							sprintf( strTemp,  G_STRING(IDS_LHSTRING1898) );
							multiText.AddString( strTemp, _FCOLOR_SKY );
						}
					}
					break;
				//----------------------------------------------------------------------
				//-- źȯ ���� ǥ��
				case IETC_SHELL:
					{
						SetItemInfo_Tribe(pOverItem , multiText);
						switch (pOverItem->m_Num2)
						{
						case 1: //-- IDS_INVEN_CANNON_BULLET : �������� �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_CANNON_BULLET));
							break;
						case 2: //-- IDS_INVEN_FIREGUN_BULLET : ȭ�����⿡ �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_FIREGUN_BULLET));
							break;
						case 3: //-- IDS_INVEN_LASER_BULLET : �������� �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_LASER_BULLET));
							break;
						case 4: //-- IDS_INVEN_MINE_THRAW_BULLET : ������ô�⿡ �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_MINE_THRAW_BULLET));
							break;
						case 5: //-- IDS_INVEN_S_THRAW_BULLET : ���� �������� �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_S_THRAW_BULLET));
							break;
						case 6: //-- IDS_INVEN_M_THRAW_BULLET : ���� �������� �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_M_THRAW_BULLET));
							break;
						case 7: //-- IDS_INVEN_NUCLEAR_BULLET : �ٷ�ó�� �ִ� źȯ
							strcpy(strTemp, G_STRING(IDS_INVEN_NUCLEAR_BULLET));
							break;
						}
						multiText.AddString(strTemp, _FCOLOR_SKY);

						//-- IDS_INVEN_CHARGE_RATE : ������: %d
						sprintf(strTemp, G_STRING(IDS_INVEN_CHARGE_RATE), pOverItem->m_Num3);
						multiText.AddString(strTemp, _FCOLOR_SKY);
					}
					break;
				//----------------------------------------------------------------------
				//-- ��ȯ�� ���� ǥ��
				case IETC_SUMMON:
					{
						if (pOverItem->m_MinLevel > 0)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_MIN_LEVEL), pOverItem->m_MinLevel);
							multiText.AddString(strTemp, _FCOLOR_RED);
						}

						SetItemInfo_Tribe(pOverItem , multiText);
						if(pOverItem->m_Vnum ==808 ||pOverItem->m_Vnum ==809) //-- ũ�ŷ� ����ũ�ŷ�
						{
							//-- IDS_INVEN_SUMMONS_C : ũ�޷θ� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_C));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==810 ||pOverItem->m_Vnum ==811 ||pOverItem->m_Vnum ==1553)
						{
							//-- IDS_INVEN_SUMMONS_AJ : ��ũ��Į�� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_AJ));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==812 ||pOverItem->m_Vnum ==813 ||pOverItem->m_Vnum ==1554)
						{
							//-- IDS_INVEN_SUMMONS_DG : �������̾�Ʈ�� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_DG));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==814 ||pOverItem->m_Vnum ==815 ||pOverItem->m_Vnum ==1555)
						{
							//-- IDS_INVEN_SUMMONS_GS : ���±׸��ڸ� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_GS));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==816 ||pOverItem->m_Vnum ==817 ||pOverItem->m_Vnum ==1556)
						{
							//-- IDS_INVEN_SUMMONS_DK : ���Ű���� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_DK));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==861 ||pOverItem->m_Vnum ==862)
						{
							//-- IDS_INVEN_SUMMONS_TIGER : ��ȣ�� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_TIGER));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==927 ||pOverItem->m_Vnum ==928)
						{
							//-- IDS_INVEN_SUMMONS_ALEN : ���̷��� ��ȯ�Ѵ�.
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_ALEN));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==1069 ||pOverItem->m_Vnum ==1070 ||pOverItem->m_Vnum ==1557)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_WINTER));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==3268)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_TANTA));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==10207)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_WARRIOR));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==10702)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_OGRE));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==11100)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_KELIAL));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else if(pOverItem->m_Vnum ==11101)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_SUMMONS_DENEB));
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}

						// 2006��ų ���� �������� ������� ������ ����
						if( pOverItem->m_Vnum >= 1331 && pOverItem->m_Vnum <= 1335 )
						{
							//mungmae-2006/4/26 �Ϲ� ���������� ���� ������ �������� ���̰� �Ѵ�.
							if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
									&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
							{
								//-- IDS_INVEN_LEFT_CNT : ���� ���� : %d
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Num3);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
							else
							{
								//-- IDS_INVEN_LEFT_CNT : ���� ���� : %d
								sprintf(strTemp, G_STRING(IDS_INVEN_LEFT_CNT), pOverItem->m_Special);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}

							//-- IDS_INVEN_USE_MANA : �����Ҹ�: %d
							sprintf(strTemp, G_STRING(IDS_INVEN_USE_MANA), pOverItem->m_Num4);
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
						else
						{
							//mungmae-2006/4/26 �Ϲ� ���������� ���� �ð��� �������� ���̰� �Ѵ�.
							if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()
									&& where_item == ITEM_INFO_SHOP && !(g_pNk2DFrame->GetControlShop()->m_shop_type & MASK_SP_SHOP) )
							{
								//-- IDS_INVEN_REMAIN_TIME : ���� �ð�: %d/%d ��
								sprintf(strTemp, G_STRING(IDS_INVEN_REMAIN_TIME), pOverItem->m_Num3,pOverItem->m_Num3);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}
							else
							{
								//-- IDS_INVEN_REMAIN_TIME : ���� �ð�: %d/%d ��
								sprintf(strTemp, G_STRING(IDS_INVEN_REMAIN_TIME), pOverItem->m_Special,pOverItem->m_Num3);
								multiText.AddString(strTemp, _FCOLOR_SKY);
							}

							//-- IDS_INVEN_USE_MANA : �����Ҹ�: %d
							sprintf(strTemp, G_STRING(IDS_INVEN_USE_MANA), pOverItem->m_Num4);
							multiText.AddString(strTemp, _FCOLOR_SKY);
						}
					}
					break;

				//----------------------------------------------------------------------
				//-- ī�̸��� ��ũ��
				case IETC_SCROLL:
					{
						if (pOverItem->m_MinLevel > 0)
						{
							sprintf(strTemp, G_STRING(IDS_INVEN_MIN_LEVEL), pOverItem->m_MinLevel);
							multiText.AddString(strTemp, _FCOLOR_RED);
						}
						SetItemInfo_Tribe(pOverItem , multiText);
						switch (pOverItem->m_Num3)
						{
						case 1://-- IDS_INVEN_RECHARGE_WAND : %d ���� �ϵ�����
							sprintf(strTemp, G_STRING(IDS_INVEN_RECHARGE_WAND),pOverItem->m_Num7);
							break;
						case 2://-- IDS_INVEN_DAM_DURAITON : ����������: %d
							sprintf(strTemp, G_STRING(IDS_INVEN_DAM_DURAITON),pOverItem->m_Num8);
							break;
						case 3://-- IDS_INVEN_RATE_STUN : ����Ȯ�� : %d%%
							sprintf(strTemp, G_STRING(IDS_INVEN_RATE_STUN),pOverItem->m_Num8);
							break;
						case 4://-- IDS_INVEN_RATE_CHAOS2 : ȥ��Ȯ�� : %d%%
							sprintf(strTemp, G_STRING(IDS_INVEN_RATE_CHAOS2),pOverItem->m_Num8);
							break;
						case 5://-- IDS_INVEN_DEC_VIATL : ����� %d%% ����
							sprintf(strTemp, G_STRING(IDS_INVEN_DEC_VIATL),pOverItem->m_Num8);
							break;
						case 6://-- IDS_INVEN_RATE_FREEZE : �ð�Ȯ�� : %d%%
							sprintf(strTemp, G_STRING(IDS_INVEN_RATE_FREEZE),pOverItem->m_Num8);
							break;
						case 7://-- IDS_INVEN_STONE_RATE : ��ȭȮ��: %d%%
							sprintf(strTemp, G_STRING(IDS_INVEN_STONE_RATE),pOverItem->m_Num8);
							break;
						case 8://-- IDS_INVEN_PROTECTION : %d�ʰ� ��ȣ
							sprintf(strTemp, G_STRING(IDS_INVEN_PROTECTION),pOverItem->m_Num8);
							break;
						default:
							break;
						}
						multiText.AddString(strTemp, _FCOLOR_GREEN);
					}
					break;


				case IETC_RECIPE:
					{
						sprintf( strTemp,  G_STRING(IDS_LHSTRING1899)  , GET_ITEM_NAME_VNUM(pOverItem->m_Num1) );
						multiText.AddString( strTemp, _FCOLOR_SKY );
						if( 0 == pOverItem->m_Num2 )
						{
							sprintf( strTemp,  G_STRING(IDS_LHSTRING1900) );
							multiText.AddString( strTemp, _FCOLOR_RED );
						}
						/*
						�����۸� ��(��) ����� �ִ� ������
						num1-> ������ �ε���
						num2-> ���а��ü�ġ( 1:�޼������� , 0:���ۿ� �����Ҽ� �ֽ��ϴ�.)
						*/
					}
					break;
				case IETC_MATERIAL:
					{
						sprintf( strTemp,  G_STRING(IDS_LHSTRING1901) );
						multiText.AddString( strTemp, _FCOLOR_SKY );
						switch(pOverItem->m_Num0 )
						{
						case 0: // ����
							sprintf( strTemp,  G_STRING(IDS_LHSTRING1902) );
							break;
						case 1: // ȥ
							sprintf( strTemp,  G_STRING(IDS_LHSTRING1903) );
							break;
						case 2: // lp
							sprintf( strTemp,  G_STRING(IDS_LHSTRING1904) );
							break;
						}
						multiText.AddString( strTemp, _FCOLOR_SKY );
						// 1896 , 1897 1898 1899
						/* num0 -> 0 : ���� , 1 : ȥ , 2 : LP
						���ۿ� ���Ǵ� �����
						���������� ȹ����
						( ����->�����ۺ��ظ� ���� ȹ���Ҽ� �ִ�.
						( ȥ->���� ����� ���� ȹ���Ҽ� �ִ�.
						( LP->LP ������ ���� �����Ҽ� �ִ�.
						*/
					}
					break;
				}
			}
			break;

		case ITYPE_PET:
			if (pOverItem->m_Num0 > 0)
			{
				//-- IDS_INVEN_VITAL : �����
				sprintf(strTemp, G_STRING(IDS_INVEN_VITAL) , pOverItem->m_Num0);
				multiText.AddString(strTemp, _FCOLOR_RED);
			}
			if (pOverItem->m_Num1 > 0)
			{
				//-- IDS_INVEN_ATT_RANGE : ���Ÿ� ���ݷ�
				sprintf(strTemp, G_STRING(IDS_INVEN_ATT_RANGE), pOverItem->m_Num1);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			if (pOverItem->m_Num2 > 0)
			{
				//-- IDS_INVEN_ATT_MELEE : �ٰŸ� ���ݷ�
				sprintf(strTemp, G_STRING(IDS_INVEN_ATT_MELEE), pOverItem->m_Num2);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			if (pOverItem->m_Num3 > 0)
			{
				//-- IDS_INVEN_DEF_RANGE : ���Ÿ� ����
				sprintf(strTemp, G_STRING(IDS_INVEN_DEF_RANGE), pOverItem->m_Num3);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			if (pOverItem->m_Num4 > 0)
			{
				//-- IDS_INVEN_DEF_MELEE : �ٰŸ� ����
				sprintf(strTemp, G_STRING(IDS_INVEN_DEF_MELEE), pOverItem->m_Num4);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			if (pOverItem->m_Num5 > 0)
			{
				//-- IDS_INVEN_SPEED : �̵� �ӵ�
				sprintf(strTemp, G_STRING(IDS_INVEN_SPEED), pOverItem->m_Num5);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			if (pOverItem->m_Num6 > 0)
			{
				//-- IDS_INVEN_COMPOS_LEVEL : ���շ���: %d
				sprintf(strTemp, G_STRING(IDS_INVEN_COMPOS_LEVEL), pOverItem->m_Num6);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			if (pOverItem->m_Num7 > 0)
			{
				//-- IDS_INVEN_COLLECTION : ��������: %d
				sprintf(strTemp, G_STRING(IDS_INVEN_COLLECTION), pOverItem->m_Num7);
				multiText.AddString(strTemp, _FCOLOR_WHITE);
			}
			break;
		}

		if( pOverItem->m_Vnum == 1588 || pOverItem->m_Vnum == 1589 || pOverItem->m_Vnum == 1037 
			|| pOverItem->m_Vnum == 1038 || pOverItem->m_Vnum == 1288 || pOverItem->m_Vnum == 1127 
			|| pOverItem->m_Vnum == 10712 || pOverItem->m_Vnum == 2219 || pOverItem->m_Vnum == 2220 
			|| pOverItem->m_Vnum == 2221 || pOverItem->m_Vnum == 10852 || pOverItem->m_Vnum == 11235
			|| pOverItem->m_Vnum == 11274 || pOverItem->m_Vnum == 4634) ///�Ϻ����������׽�Ʈ ������ �ɼ� ����ó��.
		{
			if (GET_REFLECT_DAM(pOverItem) > 0)
			{
				//-- IDS_INVEN_DAM_REFLECTION2 : ���� �ݻ� ������
				sprintf(strTemp, G_STRING(IDS_INVEN_DAM_REFLECTION2), GET_REFLECT_DAM(pOverItem));
				multiText.AddString(strTemp, _FCOLOR_SKY);
			}
		}



		if( pOverItem->m_Vnum == 1516 ) // 070213 �Ϻ� ������ �������� ��Ʈ���� 5 ��� �ϵ��ڵ� ����
		{
			//-- IDS_INVEN_RESIS_VOLT : ��Ʈ ���׷� %d%%
			sprintf(strTemp, G_STRING(IDS_INVEN_RESIS_VOLT), 5 );
			multiText.AddString(strTemp, _FCOLOR_GREEN);
		}

		///�������� // �ɼ� ���� ������ ���ɷ� ����.
		if( pOverItem->m_Vnum == 1521 || pOverItem->m_Vnum == 1522
				|| pOverItem->m_Vnum == 1523 || pOverItem->m_Vnum == 1524 ///��������
		  )
		{
			if( pOverItem->m_Special )
			{
				//-- IDS_DONT_HEAL : ȸ���ɷ� ����
				sprintf(strTemp, G_STRING(IDS_DONT_HEAL) );
				multiText.AddString(strTemp, _FCOLOR_GREEN );
			}
		}

		if( pOverItem->m_Vnum >= 1709 && pOverItem->m_Vnum <= 1711 ) // [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ���̺� �ҿ�
		{
			if( pOverItem->m_Special )
			{
				//-- IDS_NOTHAVE_ABIL : �ɷ� ����
				sprintf(strTemp, G_STRING(IDS_NOTHAVE_ABIL) );
				multiText.AddString(strTemp, _FCOLOR_GREEN );
			}
		}

		// 275���� �̻� ���� ��ȭ ������ ǥ��. ///�Ź���ȭ�ý���
		if( pOverItem->m_MaxUpgradeEndurance && !(GET_ATT_FLAG(pOverItem) & IATT_NO_UPGRADE_ENDUR) ) // �̰� 0�̸� ������ �������� �ν�����.
		{
			//-- IDS_INVEN_UPGRADE_EDR : ��ȭ������ : %d / %d
			sprintf(strTemp, G_STRING(IDS_INVEN_UPGRADE_EDR), pOverItem->m_UpgradeEndurance, pOverItem->m_MaxUpgradeEndurance );

			if( pOverItem->m_UpgradeEndurance == 0 ) // 0 �̸� ������ ///�׷����� �� �۾�.
				multiText.AddString( strTemp, RGB(230, 0, 0) );
			else if( pOverItem->m_UpgradeEndurance <= 10 ) // ����ġ ���� ������ ��Ȳ������ ǥ��
				multiText.AddString( strTemp, RGB(230, 120, 120) );
			else
				multiText.AddString( strTemp, _FCOLOR_YELLOW );
		}

		if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] ������ ���� :: ������ ���� �ݾ��� �ѷ�����.
		{
			if( pOverItem->m_MaxEndurance != 0 )
			{
				sprintf(strTemp, (char*)G_STRING(IDS_REPAIR_CURRENT), pOverItem->m_Endurance, pOverItem->m_MaxEndurance );

				if( pOverItem->m_Endurance == 0 ) // 0 �̸� ������ ///�׷����� �� �۾�.
				{
					multiText.AddString( strTemp, RGB(230, 0, 0) );
				}
				else if( pOverItem->m_Endurance <= pOverItem->m_MaxEndurance * 0.5f ) // ����ġ ���� ������ ��Ȳ������ ǥ��
				{
					multiText.AddString( strTemp, RGB(230, 120, 120) );
				}
				else
				{
					multiText.AddString( strTemp, _FCOLOR_YELLOW );
				}

				// ������ �ʿ��� �ݾ��� �ٷ� ���⿡�� �ѷ�����.
				if( pOverItem->m_Endurance != pOverItem->m_MaxEndurance )
				{
					// [2008/3/19 Theodoric] ������ ������ ��� �Ź���
					sprintf(strTemp, (char*)G_STRING(IDS_REPAIR_LAIM), GetEnduranceCharge(pOverItem) );

					CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
					multiText.AddString(strTemp2, _FCOLOR_RED); ///����ǥ�ð���

				}
			}
		}

		if (GET_TYPE(pOverItem) != ITYPE_ETC
				|| GET_SHAPE(pOverItem) != IETC_GIFT)
		{
			if (pOverItem->m_Weight > 0)
			{
				if (GET_TYPE(pOverItem) == ITYPE_PET)
				{
					int cmp_exp = 0;
					if (pCMyApp->m_pMyPet)
					{
						if (pCMyApp->m_pMyPet->m_BodyItem[GET_SHAPE(pOverItem)] >= 0)
							cmp_exp = (int) g_ItemList.m_ItemArray[pCMyApp->m_pMyPet->m_BodyItem[GET_SHAPE(pOverItem)]].m_Weight * 0.3;
					}
					//-- IDS_INVEN_EXP_BUY : ���� ����ġ: %d
					sprintf(strTemp, G_STRING(IDS_INVEN_EXP_BUY), pOverItem->m_Weight);
					multiText.AddString(strTemp, _FCOLOR_WHITE);
					//-- IDS_INVEN_EXP_RESTORE : ���� ����ġ: %d
					sprintf(strTemp, G_STRING(IDS_INVEN_EXP_RESTORE), cmp_exp);
					multiText.AddString(strTemp, _FCOLOR_WHITE);
					//-- IDS_INVEN_EXP_RESUME : �Һ� ����ġ: %d
					sprintf(strTemp, G_STRING(IDS_INVEN_EXP_RESUME), pOverItem->m_Weight - cmp_exp);
					multiText.AddString(strTemp, _FCOLOR_RED);
				}
				else if( pOverItem->m_Vnum >= 1430 && pOverItem->m_Vnum <= 1441 ) // �±� ī������ ���� ���� �߰�.
				{
					// �±� ī�������� �䱸��(����)�� ��� ������ ����ϰ� �ִ�. �䱸���� ǥ������ ����.
				}
				else
				{
					//0121 ��� ���� ��÷Ȯ�� ��Ÿ���� ���Ѱ�.
					if(pOverItem->m_Vnum == 917)
					{
						//-- IDS_INVEN_PRIZ_RATE : ��÷Ȯ�� 68%%
						sprintf(strTemp, G_STRING(IDS_INVEN_PRIZ_RATE));
						multiText.AddString(strTemp, _FCOLOR_GREEN);
					}
					else if(pOverItem->m_Vnum == 918)
					{
						//-- IDS_INVEN_PRIZ_RATE2 : ��÷Ȯ�� 17%%
						sprintf(strTemp, G_STRING(IDS_INVEN_PRIZ_RATE2));
						multiText.AddString(strTemp, _FCOLOR_GREEN);
					}
					//============================================
					// nate 2004 - 4 : quest
					// ������ ó��
					else if( pOverItem->m_Vnum == 925
							 || pOverItem->m_Vnum == 1054 )
						//============================================
					{
						//-- IDS_INVEN_EXPECT_PRICE : ���� ������ : %d
						sprintf(strTemp, G_STRING(IDS_INVEN_EXPECT_PRICE), pOverItem->m_Special);
						multiText.AddString(strTemp, RGB(255, 150, 150));
					}
					//-- IDS_INVEN_WEIGHT : �䱸��
					//sprintf(strTemp, G_STRING(IDS_INVEN_WEIGHT), pOverItem->m_Weight);
					sprintf(strTemp, G_STRING(IDS_INVEN_WEIGHT), pOverItem->m_Weight);

					multiText.AddString(strTemp, _FCOLOR_WHITE);
				}
			}
			if (pOverItem->m_Int > 0 && GET_TYPE(pOverItem) != ITYPE_PET && !(GET_TYPE(pOverItem) == ITYPE_ETC && GET_SHAPE(pOverItem) == IETC_SUMMON) ) // ��ȯ�� �ʿ����� ǥ�þ��ϰ� ����( 07-01-09 ���� )
			{
				if( pOverItem->m_Vnum >= 1430 && pOverItem->m_Vnum <= 1441 ) // �±� ī������ ���� ���� �߰�.
				{
					// �±� ī�������� �ʿ� ������ ���ġ�� ����ϰ� �ִ�. ǥ������ ����.
				}
				else
				{
					//-- IDS_INVEN_REQUILE_INT : �ʿ� ����
					sprintf(strTemp, G_STRING(IDS_INVEN_REQUILE_INT), pOverItem->m_Int); // �ʿ�����
					multiText.AddString(strTemp, _FCOLOR_WHITE);
				}
			}
			if (pOverItem->m_Dex > 0 && GET_TYPE(pOverItem) != ITYPE_PET)
			{
				if( (pOverItem->m_Vnum >= 1430 && pOverItem->m_Vnum <= 1441) // �±� ī������ ���� ���� �߰�.
						|| (pOverItem->m_Vnum >= 1474 && pOverItem->m_Vnum <= 1484) // �±� ���� ���� ������ �߰�.
				  )
				{
					// �±� ī�������� ��μ��� ���ġ�� ����ϰ� �ִ�. ǥ������ ����.
				}
				else
				{
					//-- IDS_INVEN_REQUILE_QUICKNESS : �ʿ� ���
					sprintf(strTemp, G_STRING(IDS_INVEN_REQUILE_QUICKNESS), pOverItem->m_Dex);
					multiText.AddString(strTemp, _FCOLOR_WHITE);
				}
			}

			// Ƚ�� ���� ������ ���Ƚ�� ǥ��. ///Ƚ������

			if( where_item == ITEM_INFO_CASHSHOP ) ///�������
			{
				// ĳ���� ������ ������ ������ ǥ������ �ʵ��� �ѱ��.
			}
			else if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop())
			{
				if (where_item == ITEM_INFO_SHOP)
				{
					if( g_pNk2DFrame->GetControlShop()->m_shop_type&MASK_SP_SHOP ) // ������ Ư�� ������ ���..
						price = pOverItem->m_ExPrice * g_pNk2DFrame->GetControlShop()->m_SellRate / 100; // ������ ���� ���� Ư�� �������� ǥ��.
					else
						price = (__int64)GET_PRICE(pOverItem) * (__int64)g_pNk2DFrame->GetControlShop()->m_SellRate / 100;

					if( (g_pNk2DFrame->GetControlShop()->m_shop_type&MASK_SHOP_PAY_TYPE) == SHOP_PAY_TYPE_BP ) // BP�������̸� BP�����̶�� ǥ��
					{
						// �ǸźҰ� �������� ������ �Ⱥ����ش�.
						if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
						{
						}
						else
						{
							//-- IDS_BP_PRICE : BP����: %d
							sprintf(strTemp, G_STRING(IDS_BP_PRICE), price);

							///����ǥ�ð���
							CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
							multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

							if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
							{
								ConvertMoneyToString( strTemp2, price, CommaPos, 1 ); ///����ǥ�ð��� // �ڿ� 1�� BP���� ǥ��.
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
							}
						}
					}
					else if((g_pNk2DFrame->GetControlShop()->m_shop_type&MASK_SHOP_PAY_TYPE) == SHOP_PAY_TYPE_LP )
					{
						{
							//-- IDS_BP_PRICE : BP����: %d
							sprintf(strTemp, G_STRING(IDS_LHSTRING1744),  price);

							///����ǥ�ð���
							CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
							multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

							if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
							{
								ConvertMoneyToString( strTemp2, price, CommaPos, 3); ///����ǥ�ð��� // �ڿ� 1�� BP���� ǥ��.
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
							}
						}
					}
					else
					{
						// �ǸźҰ� �������� ������ �Ⱥ����ش�.
						if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
						{
						}
						else
						{
							//-- IDS_INVEN_PRICE
							DWORD text_color = _FCOLOR_WHITE;
							int chaotic = g_pRoh->m_Chaotic_Num * 0.1;
							if( chaotic < 0 && chaotic > -500 )
							{
								price *= 3;
								text_color = _FCOLOR_YELLOW;
							}
							else if( chaotic <= -500 )
							{
								price *= 10;
								text_color = _FCOLOR_RED;
							}
							sprintf(strTemp, G_STRING(IDS_INVEN_PRICE), price);

							///����ǥ�ð���
							CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
							multiText.AddString(strTemp2, text_color); ///����ǥ�ð���

							if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
							{
								ConvertMoneyToString( strTemp2, price, CommaPos, 0 ); ///����ǥ�ð��� // �ڿ� 0�� �������� ǥ��.
								multiText.AddString(strTemp2, text_color); ///����ǥ�ð���
							}
						}
					}
				}
				else
				{
					if( pOverItem->m_bExPrice ) // ������ ������ �����Ǵ� �������̸�... ///BP����
						price = pOverItem->m_ExPrice; // ������ ���� ���� Ư�� �������� ǥ��.
					else
						price = GET_PRICE(pOverItem);

					if( GET_ATT_FLAG(pOverItem) & IATT_SAME_PRICE )
					{
						// �� ���� ������ �״�δϱ� price�� �Ȱ��ĵ� �ȴ�.
						if( g_bGlobalMatrix ) ///���������� ���� '100% ���� �ɼ� ���ý�Ų��.(���Ӱ��ݵ��� �Ϻη� 0���� ����ϱ�)
						{
							price = (price*g_pNk2DFrame->GetControlShop()->m_BuyRate) / 100; // ���� ����Ʈ ����.
						}
					}
					else if (pOverItem->m_Vnum == 919)
					{
						// �ݻ���.
						price = 10000;
						//-- IDS_INVEN_SELLING_PRICE
						sprintf(strTemp, G_STRING(IDS_INVEN_SELLING_PRICE), price);
					}
					else if (pOverItem->m_Vnum == 931)
					{
						// 0401 �ĸ��� ����.
						price = m_pNkCha->m_Level;
						price *= 100;
						if (price < 5000) //�ּ� 5000
							price = 5000;
						else if(price > 25000)//�ִ� 25000
							price = 25000;

					}
					else
					{
						if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] ������ ���� :: ������ �� ���� �ǸŰ��� �����ݾ� ���
						{
							float endurPrice = 1.0f;
							if( pOverItem->m_Endurance < pOverItem->m_MaxEndurance && pOverItem->m_MaxEndurance > 0 )
							{
								endurPrice = (int)(( (float)pOverItem->m_Endurance / (float)pOverItem->m_MaxEndurance) );
							}

							price = ((price*g_pNk2DFrame->GetControlShop()->m_BuyRate) / 100);	// ���� ����Ʈ ����.
							price = (int)((float)price*endurPrice);								// �������� ���� ���� �ݾ� ����.

							if( price < 1 ) // �ּ� �Ǹ� �ݾ��� 1������ ���� ����.
							{
								price = 1;
							}
						}
						else
						{
							price = (price*g_pNk2DFrame->GetControlShop()->m_BuyRate) / 100; // ���� ����Ʈ ����.
						}
					}

					if( pOverItem->m_PriceType == 1 ) // BP�������̸� BP�����̶�� ǥ��
					{
						// �ǸźҰ� �������� ������ �Ⱥ����ش�.
						if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
						{
						}
						else
						{
							//-- IDS_SELLING_BP_PRICE : �Ĵ� BP����: %d
							sprintf(strTemp, G_STRING(IDS_SELLING_BP_PRICE), price);

							///����ǥ�ð���
							CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
							multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

							if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
							{
								ConvertMoneyToString( strTemp2, price, CommaPos, 1 ); ///����ǥ�ð���
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
							}
						}
					}
					else if( pOverItem->m_PriceType == 3 )
					{
						// �ǸźҰ� �������� ������ �Ⱥ����ش�.
						if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
						{
						}
						else
						{
							//-- IDS_SELLING_BP_PRICE : �Ĵ� BP����: %d
							sprintf(strTemp, G_STRING(IDS_LHSTRING1745) , price);

							///����ǥ�ð���
							CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
							multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

							if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
							{
								ConvertMoneyToString( strTemp2, price, CommaPos, 3 ); ///����ǥ�ð���
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
							}
						}
					}
					else
					{
						// �ǸźҰ� �������� ������ �Ⱥ����ش�.
						if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
						{
						}
						else
						{
							//-- IDS_INVEN_SELLING_PRICE
							sprintf(strTemp, G_STRING(IDS_INVEN_SELLING_PRICE), price);

							///����ǥ�ð���
							CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
							multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

							if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
							{
								ConvertMoneyToString( strTemp2, price, CommaPos, 0 ); ///����ǥ�ð���
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
							}
						}
					}
				}
			}
			else if( g_pNk2DFrame->IsUserShopVisible() && g_pNk2DFrame->GetControlUserShop() && where_item == ITEM_INFO_USERSHOP ) //���� ������ ���� ���°� �Ǹŵǰ��ִ� ������ �̶��... ///���λ���
			{
				// �ʿ� �ݱ�,����,������ ������ ǥ���Ѵ�. (�ִ� �༮��...)
				if( pOverItem->m_NeedRedLime )
				{
					sprintf( strTemp, "Required red bar: %d", pOverItem->m_NeedRedLime );
					multiText.AddString(strTemp, RGB(255, 0, 0));
				}

				if( pOverItem->m_NeedPlatinumLime )
				{
					sprintf( strTemp, "Required platinum bar: %d", pOverItem->m_NeedPlatinumLime );
					multiText.AddString(strTemp, RGB(255, 165, 0));
				}
				
				if( pOverItem->m_NeedGoldLime )
				{
					//-- IDS_NEED_GOLDLIME : �ʿ� �ݱ�: %d
					sprintf( strTemp, (char*)G_STRING(IDS_NEED_GOLDLIME), pOverItem->m_NeedGoldLime );
					multiText.AddString(strTemp, RGB(234, 236, 22));
				}
				if( pOverItem->m_NeedSilverLime )
				{
					//-- IDS_NEED_SILVERLIME : �ʿ� ����: %d
					sprintf( strTemp, (char*)G_STRING(IDS_NEED_SILVERLIME), pOverItem->m_NeedSilverLime );
					multiText.AddString(strTemp, RGB(122, 220, 122));
				}
				if( pOverItem->m_NeedBronzeLime )
				{
					//-- IDS_NEED_BRONZELIME : �ʿ� ����: %d
					sprintf( strTemp, (char*)G_STRING(IDS_NEED_BRONZELIME), pOverItem->m_NeedBronzeLime );
					multiText.AddString(strTemp, RGB(255, 152, 57));
				}

				price = pOverItem->m_ExPrice; // ExPrice�� ������ ������ ������ �����صξ���.

				//-- IDS_NEED_LIME : �ʿ� ����: %d
				sprintf(strTemp, (char*)G_STRING(IDS_NEED_LIME), price);

				///����ǥ�ð���
				CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
				multiText.AddString(strTemp2, RGB(233, 233, 233)); ///����ǥ�ð���

				if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
				{
					ConvertMoneyToString( strTemp2, price, CommaPos ); ///����ǥ�ð���
					multiText.AddString(strTemp2, RGB(233, 233, 233)); ///����ǥ�ð���
				}
			}
			else
			{
				if( pOverItem->m_bExPrice ) // ������ ������ �����Ǵ� �������̸�... ///BP����
					price = pOverItem->m_ExPrice; // ������ ���� ���� Ư�� �������� ǥ��.
				else if( pOverItem->m_Vnum == 1307 ) //mungmae-2005/10/5 �°�&���� �Ӵ� ����ǥ��
					price = pOverItem->m_Special;
				else
					price = GET_PRICE(pOverItem);

				if (GET_TYPE(pOverItem) != ITYPE_PET)
				{
					if (g_dwLangType == 0 && g_dwClientCountry == 0)
					{
						if(!(GET_ATT_FLAG(pOverItem) & IATT_NO_SELL))
						{
							if( pOverItem->m_PriceType == 1 ) // BP�������̸� BP�����̶�� ǥ��
							{
								// �ǸźҰ� �������� ������ �Ⱥ����ش�.
								if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
								{
								}
								else
								{
									//-- IDS_BP_PRICE : BP����: %d
									sprintf(strTemp, G_STRING(IDS_BP_PRICE), price);

									///����ǥ�ð���
									CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
									multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

									if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
									{
										ConvertMoneyToString( strTemp2, price, CommaPos, 1 ); ///����ǥ�ð���
										multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
									}
								}
							}
							else
							{
								// �ǸźҰ� �������� ������ �Ⱥ����ش�.
								if( GET_ATT_FLAG(pOverItem) & IATT_NO_SELL || pOverItem->m_bTimeLimit )
								{
								}
								else
								{
									//-- IDS_INVEN_PRICE
									sprintf(strTemp, G_STRING(IDS_INVEN_PRICE), price); // ����

									///����ǥ�ð���
									CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
									multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

									if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
									{
										ConvertMoneyToString( strTemp2, price, CommaPos, 0 ); ///����ǥ�ð���
										multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
									}
								}
							}
						}
					}
					else
					{
						if(pOverItem->m_Vnum == 614
								|| pOverItem->m_Vnum == 615
								|| pOverItem->m_Vnum == 616
								|| pOverItem->m_Vnum == 617
								|| pOverItem->m_Vnum == 1161
								|| pOverItem->m_Vnum == 2319
								|| pOverItem->m_Vnum == 3158
						  ) // egg
							price /= 2; // �ݰ�.
						else	// �׿ܿ� �� ���� �״��...
						{
						}

						if( pOverItem->m_PriceType == 1 ) // BP�������̸� BP�����̶�� ǥ��
						{
							if( price != 0 )
							{
								//-- IDS_BP_PRICE : BP����: %d
								sprintf(strTemp, G_STRING(IDS_BP_PRICE), price);

								///����ǥ�ð���
								CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

								if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
								{
									ConvertMoneyToString( strTemp2, price, CommaPos, 1 ); ///����ǥ�ð���
									multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
								}
							}
						}
						else
						{
							if( price != 0 && !( pOverItem->m_bTimeLimit || (GET_ATT_FLAG(pOverItem) & IATT_NO_SELL)) )

							{
								//-- IDS_INVEN_PRICE
								sprintf(strTemp, G_STRING(IDS_INVEN_PRICE), price); // ����

								///����ǥ�ð���
								CommaPos = InsertPriceComma( strTemp2, strTemp, ':' ); // �޸��� ���� ��Ʈ������ �ٲ۴�.
								multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���

								if( g_dwClientCountry == CTRY_KOR && price != 0 ) // ������ 0�̸� ǥ�� ���ϵ��� ���� 07-02-27 ����
								{
									ConvertMoneyToString( strTemp2, price, CommaPos, 0 ); ///����ǥ�ð���
									multiText.AddString(strTemp2, _FCOLOR_WHITE); ///����ǥ�ð���
								}
							}
						}
					}
				}
				else
				{
					sprintf(strTemp, " ");
					multiText.AddString(strTemp, _FCOLOR_WHITE);
				}
			}
		}

		// BP ���� ���� ���� ǥ��( 06-01-19 ����)
		if (g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop())
		{
			if (where_item == ITEM_INFO_SHOP)
			{
				if( g_pNk2DFrame->GetControlShop()->m_shop_type&MASK_SP_SHOP ) // ������ Ư�� ������ ���..
				{
					if( pOverItem->m_LimitCount != -1 ) // ���� ������ �ִ� �������̸�...
					{
						//-- IDS_LIMIT_COUNT : ���� ����: %d
						sprintf(strTemp, G_STRING(IDS_LIMIT_COUNT), pOverItem->m_LimitCount );
						multiText.AddString( strTemp, RGB(230,230,100) );
					}
				}
			}
		}

		///�ð���������
		if( pOverItem->m_bTimeLimit ) // �ð��� �������̶��...
		{
			int Limit_Day = ( (pOverItem->m_LimitTime/60)/60 ) / 24; // ���� �ϼ�.
			int Limit_Hour = ( (pOverItem->m_LimitTime/60)/60 ) % 24; // ���� �ð�.
			int Limit_Min  = (pOverItem->m_LimitTime/60) % 60; // ���� ��

			//--IDS_REMAIN_LIMIT_TIME : ���� �ð���
			sprintf( strTemp, "%s", G_STRING(IDS_REMAIN_LIMIT_TIME) );

			if( Limit_Day )
			{
				//--IDS_DAY : ��
				sprintf( strTemp2, " %d%s", Limit_Day, G_STRING(IDS_DAY) ); // �ϼ� ǥ��
				strcat( strTemp, strTemp2 );
			}

			if( Limit_Hour )
			{
				//--IDS_HOUR : �ð�
				sprintf( strTemp2, " %d%s", Limit_Hour, G_STRING(IDS_HOUR) ); // �ð� ǥ��
				strcat( strTemp, strTemp2 );
			}

			if( Limit_Min || (!Limit_Day&&!Limit_Hour&&!Limit_Min) ) // 0�� 0�ð� 0���� ��� 0���� ǥ�����ش�.
			{
				//--IDS_MIN : ��
				sprintf( strTemp2, " %d%s", Limit_Min, G_STRING(IDS_MIN) ); // �� ǥ��
				strcat( strTemp, strTemp2 );
			}

			multiText.AddString(strTemp, RGB(250,10,10), RGB(52,52,52) );
			// �ð��� �������� �Ұ� �Ӽ� ���� ǥ��
			*strTemp = '\0';

			if( g_dwClientCountry != CTRY_KOR ) // �ѱ��ܿ��� ������ �Ұ�.
			{
				strcat(strTemp, G_STRING(IDS_INVEN_THROW_UNABLE)); //--IDS_INVEN_THROW_UNABLE : ������ �Ұ�
			}
			else
			{
				if (GET_ATT_FLAG(pOverItem) & IATT_NO_DROP)
					strcat(strTemp, G_STRING(IDS_INVEN_THROW_UNABLE)); //--IDS_INVEN_THROW_UNABLE : ������ �Ұ�
				else
					strcat(strTemp, G_STRING(IDS_INVEN_DROP_DELETE)); //-- IDS_INVEN_DROP_DELETE : ������ ����
			}

			{
				strcat(strTemp, "  ");
				strcat(strTemp, G_STRING(IDS_INVEN_SEL_UNABLE)); //-- IDS_INVEN_SEL_UNABLE : �ȱ�Ұ�
			}

			strcat(strTemp, "  ");
			strcat(strTemp, G_STRING(IDS_INVEN_EXCHA_UNABLE)); //-- IDS_INVEN_EXCHA_UNABLE : ��ȯ �Ұ�.
			strcat(strTemp, "  ");
			strcat(strTemp, G_STRING(IDS_IMPOSSIBLE_WARESHOUSE)); //-- IDS_IMPOSSIBLE_WARESHOUSE : â��Ұ�

			if( g_dwLangType == LANG_DEU || g_dwLangType == LANG_FR )
				multiText.AddStringMulti( strTemp, 80, TRUE, RGB(127, 255, 127) );
			else
				multiText.AddString(strTemp, RGB(127, 255, 127));
		}
		else if( pOverItem->m_PriceType == 2 ) // ĳ�� �����۵� �Ұ�ó��. ///�������.
		{
			*strTemp = '\0';
			strcat(strTemp, G_STRING(IDS_INVEN_THROW_UNABLE)); //--IDS_INVEN_THROW_UNABLE : ������ �Ұ�
			strcat(strTemp, "  ");

			if( g_dwLangType == LANG_DEU || g_dwLangType == LANG_FR )
				multiText.AddStringMulti( strTemp, 80, TRUE, RGB(127, 255, 127) );
			else
				multiText.AddString(strTemp, RGB(127, 255, 127));
		}
		else // �ð��� �������� �ƴϸ�...
		{
			*strTemp = '\0';
			//--IDS_INVEN_THROW_UNABLE : ������ �Ұ�
			if (GET_ATT_FLAG(pOverItem) & IATT_NO_DROP)
				strcat(strTemp, G_STRING(IDS_INVEN_THROW_UNABLE));
			//-- IDS_INVEN_SEL_UNABLE : �ȱ� �Ұ�
			if (GET_ATT_FLAG(pOverItem) & IATT_NO_SELL)
			{
				if (*strTemp)strcat(strTemp, "  ");
				strcat(strTemp, G_STRING(IDS_INVEN_SEL_UNABLE));
			}
			//-- IDS_INVEN_EXCHA_UNABLE : ��ȯ �Ұ�.
			if (GET_ATT_FLAG(pOverItem) & IATT_NO_EXCHANGE)
			{
				if (*strTemp)strcat(strTemp, "  ");
				strcat(strTemp, G_STRING(IDS_INVEN_EXCHA_UNABLE));
			}
			//-- IDS_IMPOSSIBLE_WARESHOUSE : â��Ұ�
			if (GET_ATT_FLAG(pOverItem) & IATT_NO_STASH)
			{
				if (*strTemp)strcat(strTemp, "  ");
				strcat(strTemp, G_STRING(IDS_IMPOSSIBLE_WARESHOUSE));
			}
			if( (GET_ATT_FLAG(pOverItem) & IATT_GUILD_NO_STASH) )
			{
				if (*strTemp)strcat(strTemp, "  ");
				strcat(strTemp, G_STRING(IDS_IMPOSSIBLE_GUILDHOUSE));
			}


			if( g_dwClientCountry == CTRY_KOR && pOverItem->m_Vnum == 1519 ) // 1519 ����ġ �������� ��� �ѱ��� �ؿ��� �Ӽ� ������ ������ �ѱ��� ��ȯ�Ұ��� ���ܷ� �����Ų��.
			{
				if (*strTemp)strcat(strTemp, "  ");
				strcat(strTemp, G_STRING(IDS_INVEN_EXCHA_UNABLE));
			}

			if (*strTemp)
			{
				if( g_dwLangType == LANG_DEU || g_dwLangType == LANG_FR )
					multiText.AddStringMulti( strTemp, 80, TRUE, RGB(127, 255, 127) );
				else
					multiText.AddString(strTemp, RGB(127, 255, 127));
			}
		}
		//Item Label for Set Items
		if( g_pRoh->IsSetItem(pOverItem->m_Vnum) != SETITEM_NONE && g_pRoh->GetSetItem() != SETITEM_NONE)
		{
			if( g_pRoh->IsSetItem(pOverItem->m_Vnum) == g_pRoh->GetSetItem())
			{
				if( g_pRoh->IsWeared(pOverItem) == true )
				{
					char str[64]= {0,};
					//Get the String output
					g_pRoh->GetSetItemEffectString(str, false);
					//Slap the text on the surface
					multiText.AddString(str, RGB(255, 0, 255));
				}				
			}
		}
		//As above
		if( g_pRoh->IsSetItemEventWear(pOverItem->m_Vnum) != SETITEM_NONE && g_pRoh->GetSetItemEventWear() != SETITEM_NONE)
		{
			if( g_pRoh->IsSetItemEventWear(pOverItem->m_Vnum) == g_pRoh->GetSetItemEventWear())
			{
				if( g_pRoh->IsEventWeared(pOverItem) == true )
				{
					char str[64]= {0,};
					g_pRoh->GetSetItemEffectString(str, true);
					multiText.AddString(str, RGB(255, 0, 255));
				}				
			}
		}

		//Addflag2spbuff
		if((GET_ATT_FLAG(pOverItem) >> 25) & 1)
		{
			char ttt[100];
			
			int spBuffEx[6][7] = {	{1000,2000,3000,4000,5000,6000,7500},
						{1000,2000,3000,4000,5000,6000,7500},
						{1000,2000,3000,4000,5000,6000,7500},
						{1000,2000,3000,4000,5000,6000,7500},
						{2,-1,-1,-1,-1,-1,-1},
						{7,-1,-1,-1,-1,-1,-1}};


			if((((pOverItem->m_Special2 >> 0) & 7) > 0) /*&& pOverItem->m_spbuff[0] > 0*/) //Buff0 Maximum HP +
			{

				sprintf(ttt,"Maximum HP +%d",spBuffEx[0][((pOverItem->m_Special2 >> 0) & 7)-1]);
				multiText.AddString(ttt, RGB(255, 0, 255));
			}
			if((((pOverItem->m_Special2 >> 3) & 7) > 0) /*&& pOverItem->m_spbuff[1] > 0*/) //Buff0 Maximum MP +
			{
				sprintf(ttt,"Maximum MP +%d",spBuffEx[1][((pOverItem->m_Special2 >> 3) & 7)-1]);
				multiText.AddString(ttt, RGB(255, 0, 255));
			}
			if((((pOverItem->m_Special2 >> 6) & 7) > 0) /*&& pOverItem->m_spbuff[2] > 0*/) //Buff0 Maximum SP +
			{
				sprintf(ttt,"Maximum SP +%d",spBuffEx[2][((pOverItem->m_Special2 >> 6) & 7)-1]);
				multiText.AddString(ttt, RGB(255, 0, 255));
			}
			if((((pOverItem->m_Special2 >> 25) & 7) > 0) /*&& pOverItem->m_spbuff[3] > 0*/) //Buff0 Maximum EP +
			{
				sprintf(ttt,"Maximum EP +%d",spBuffEx[3][((pOverItem->m_Special2 >> 25) & 7)-1]);
				multiText.AddString(ttt, RGB(255, 0, 255));
			}
			if((((pOverItem->m_Special2 >> 9) & 1) > 0) /*&& pOverItem->m_spbuff[4] > 0*/) //PVP DMG ABS +
			{
				sprintf(ttt,"PVP Damage Absorb %d%%",spBuffEx[4][((pOverItem->m_Special2 >> 9) & 1)-1]);
				multiText.AddString(ttt, RGB(255, 0, 255));
			}
			if((((pOverItem->m_Special2 >> 28) & 1) > 0) /*&& pOverItem->m_spbuff[5] > 0*/) //PVE DMG ABS +
			{
				sprintf(ttt,"PVE Damage Absorb %d%%",spBuffEx[5][((pOverItem->m_Special2 >> 28) & 1)-1]);
				multiText.AddString(ttt, RGB(255, 0, 255));
			}
		}


		SetItemEffMgr* setItemEffMgr = SetItemEffMgr::GetInstance();
		std::vector< std::pair<int, int> > vecStatus = setItemEffMgr->GetEffect(pOverItem->m_Index);
		if( vecStatus.size() > 0 )
		{
			AddSetItemString(vecStatus, multiText);
		}		

		if( GET_ITEM_DESC(pOverItem).length() > 0 )
		{
			multiText.AddStringMulti(const_cast<char*>(g_ItemList.m_ItemArray[pOverItem->m_ProtoNum].m_strDesc.c_str()), 50, TRUE, RGB(255, 246, 18));
		}

		m_pItemInfo->PutString(&multiText);
		m_pItemInfo->SetIndex(pOverItem->m_Index);
		m_pItemInfo->SetGemNum(gem_num);

		if( where_item == ITEM_INFO_CASHSHOP ) ///������� // ITEM_INFO_CASHSHOP �� ȭ�鿡 ������� �ʵ���...
		{
			// �׳� ���糪 �����ϸ� �ȵȴ�.
			for( int k = 0 ; k < multiText.m_LineCount ; ++k )
				g_pNk2DFrame->GetControlCashShop()->m_MultiText.AddStringMulti( multiText.m_Text[k], 30, TRUE, multiText.m_TextColor[k], multiText.m_TextBackColor[k] );

			return; // �׸��� �ʵ��� ���������. (m_pItemInfo->PutString(&multiText); ������ �ؾ��Կ� ����.)
		}
	}
	m_pItemInfo->Draw(point.x, point.y, 4);
	if (m_pItemInfo->GetGemNum() > 0)
		m_pItemInfo->DrawImgGem(m_pImgGem, (GET_ITEM_LEVEL(pOverItem)-1)/30);

	if( pCMyApp->m_pMouse )
		pCMyApp->m_pMouse->SetMouseType(M_HOLD);

	if( g_pNk2DFrame->GetItemRender() )
	{
		int offsetX = ITEM_RENDER_WIDTH ;
		int offsetY = m_pItemInfo->GetSurHeight() / 2 - (ITEM_RENDER_HEIGHT / 2);

		float fScale = 1.0f;
		int width = GET_ITEM_WIDTH_S(pOverItem);
		int height = GET_ITEM_HEIGHT_S(pOverItem);

		if( width == 1 && height == 1 )
		{
			fScale = 2.0f;
		}

		g_pNk2DFrame->GetItemRender()->BeginRenderLolo();
		g_pNk2DFrame->RenderItemInUI(m_pItemInfo->GetXpos() - offsetX, m_pItemInfo->GetYpos() + offsetY,
									 pOverItem , FALSE , FALSE , TRUE, TRUE, fScale);
		g_pNk2DFrame->GetItemRender()->EndRenderLolo();
	}
}

void CControlInven::AddSetItemString(const std::vector< std::pair<int, int> >& vecStatus, CMultiText& multiText)
{
	unsigned int size = vecStatus.size();
	if( size == 0 )
		return;

	int type = -1;
	int val = -1;
	for(unsigned int i = 0; i < size; i++)
	{
		type = vecStatus[i].first;
		val = vecStatus[i].second;

		if( type < 0 || type >= eStatusType_Max )
		{
			continue;
		}
		if( val == 0 )
		{
			continue;
		}

		std::string strType;
		std::string strVal;
		std::string strPlusMinus;
		std::string strPercent;
		if( val >= 0 )
		{
			strPlusMinus = "+";
		}
		else
		{
			strPlusMinus = "-";
		}		

		strType = G_STRING(IDS_MAX_VITAL + type);
		strVal = boost::str(boost::format("%1%") % val);

		if( type == eStatusType_PVE_DecraseDam ||
			type == eStatusType_PVP_DecraseDam )
		{
			strPlusMinus = "";
			strPercent = "%";
		}

		std::string strStatus = boost::str(boost::format("%1% %2%%3%%4%") % strType % strPlusMinus % strVal % strPercent);
		multiText.AddString((char*)strStatus.c_str(), RGB(255, 0, 255));
	}	
}

void CControlInven::DrawExtra()
{
	if (m_pNkCha && m_pNkCha->m_ExtraSlot)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		DWORD ambient = 0x00555555;
		//��ũ�Ѱ� ��й�� ambient�� ����
		if( g_pNk2DFrame->IsScroll(m_pNkCha->m_ExtraSlot)
				|| m_pNkCha->m_ExtraSlot->m_Vnum == 1251
				|| m_pNkCha->m_ExtraSlot->m_Vnum == 1308 // ���ǻ��� ���� ��� ���� 05-10-05 ����
				|| m_pNkCha->m_ExtraSlot->m_Vnum == 1286
				|| ( m_pNkCha->m_ExtraSlot->m_Vnum >= 1598 && m_pNkCha->m_ExtraSlot->m_Vnum <= 1695 ) // ����� ��û ���̱������ ������ �����⿡�� �����̹Ƿ� ����ó��.
				|| m_pNkCha->m_ExtraSlot->m_Vnum == 1287 ) // �̱� ������ ����, ������ ������ ��Ӱ� ������ Ŭ���̾�Ʈ���� ���� ��� ó��.(by ����)
			ambient = 0x00cccccc;
		else if (m_pNkCha->m_ExtraSlot->m_PlusNum > 0)
			ambient = UPGRADE_ITEM_COLOR;

		if( g_pNk2DFrame->GetItemRender() )
			g_pNk2DFrame->RenderItemInUI(point.x - (ITEM_RENDER_WIDTH/2), point.y - (ITEM_RENDER_HEIGHT/2),
										 m_pNkCha->m_ExtraSlot , FALSE , TRUE , TRUE , FALSE );

		if( pCMyApp->m_pMouse )
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
	}
}

BOOL CControlInven::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return FALSE;

	return m_pSurfaceBack->IsIn(x,y);
}

// �������׷��̵�â
BOOL CControlInven::IsInside_Masuk_Back(int x, int y)
{
	if (x >= m_x
			&& x < m_x +m_cx
			&& y >= m_y
			&& y < m_y + m_cy )
		return TRUE;

	return FALSE;
}

BOOL CControlInven::IsInside_Recycle(int x, int y)
{
	if( m_pImgRecycle == NULL )
		return FALSE;

	if( x >= m_pImgRecycle->Xpos &&
			x <= m_pImgRecycle->Xpos + 30 &&
			y >= m_pImgRecycle->Ypos &&
			y <= m_pImgRecycle->Ypos + 30 )
		return TRUE;

	return FALSE;
}

BOOL CControlInven::IsInsideEquip(int x, int y)
{
	if (x >= m_EquipSlot[WEARING_SHIELD].m_X
			&& x < m_EquipSlot[WEARING_WEAPON].m_X + m_EquipSlot[WEARING_WEAPON].m_W
			&& y >= m_EquipSlot[WEARING_HELMET].m_Y
			&& y < m_EquipSlot[WEARING_SHOES].m_Y + m_EquipSlot[WEARING_SHOES].m_H )
		return TRUE;
	return FALSE;
}

BOOL CControlInven::IsPlayArea(int x, int y, BOOL bEnabled)
{
	if( !IsInside( x, y ) && !g_pNk2DFrame->IsInPopup(x, y) )
		return true;

	return false;
}

int CControlInven::WhichWearing(int x, int y)
{
	for (int i = 0; i < WEARING_NUM; i++)
	{
		if (x >= m_EquipSlot[i].m_X
				&& x < m_EquipSlot[i].m_X + m_EquipSlot[i].m_W
				&& y >= m_EquipSlot[i].m_Y
				&& y < m_EquipSlot[i].m_Y + m_EquipSlot[i].m_H)
			return i;
	}

	return -1;
}

LRESULT CControlInven::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL bEnabled, BOOL bWindowOpened )
{
	if( !m_pNkCha || !g_pNk2DFrame )
		return FALSE;

	static char commOutBuf[512] = "";
	int x = 0, y = 0, i = 0;
	int nExtraShape = 0;
	int count = 0;

	if( pCMyApp && pCMyApp->m_pUIMgr && g_pNk2DFrame && g_pNk2DFrame->IsUserShopVisible() )
	{
		if( msg == WM_LBUTTONDOWN
				&& m_pNkCha->m_ExtraSlot
				&& IsPlayArea( LOWORD(lParam),HIWORD(lParam),bWindowOpened ) )
			return 1;
	}

	if( g_pNk2DFrame->FindPopup(POPUP_MULTI_SALE) || g_pNk2DFrame->FindPopup(POPUP_MULTI_SALE_WARRING) )
		return 1;


	x = LOWORD (lParam);
	y = HIWORD (lParam);



	switch ( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if(m_bAct_Masuk)
			{
				for(i=0; i<NUM_BTN_MASUK; i++)
				{
					if(m_Btn_Masuk[i].GetState() != BTN_DISABLE)
					{
						m_Btn_Masuk[i].MsgProc(hWnd, msg, wParam, lParam);
						if(m_Btn_Masuk[i].GetState() == BTN_ACTION && m_bShowBtnAry[i])
						{
							m_Btn_Masuk[i].m_iBtnFlg = BTN_NORMAL;
							Action_Masuk(i);
							Close_Masuk();
							return 1;
						}
					}
				}
			}

			if (!bEnabled || !IsInside(x ,y))
				return 0;

			m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_InvenBtn1.MsgProc(hWnd, msg, wParam, lParam);
			m_InvenBtn2.MsgProc(hWnd, msg, wParam, lParam);
			m_InvenBtn3.MsgProc(hWnd, msg, wParam, lParam);
			m_InvenBtn4.MsgProc(hWnd, msg, wParam, lParam);
			//m_SortBtn.MsgProc(hWnd, msg, wParam, lParam);

			if( g_pNk2DFrame )
			{
				if (m_CloseBtn.GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->ShowInvenWindow(FALSE);
					m_CloseBtn.SetState(BTN_NORMAL);

					if( g_pNk2DFrame->GetControlExchange()
							&& g_pNk2DFrame->GetControlExchange()->IsExch() )
						g_pNk2DFrame->GetControlExchange()->ExchCancel(TRUE);
				}

				/*if( m_SortBtn.GetState() == BTN_ACTION )
				{
					m_SortBtn.SetState(BTN_NORMAL);
					g_pTcpIp->SendNetMessage("spirit start\n");
				}*/
			}

			if( g_bUseEventWear )
			{
				if( msg == WM_LBUTTONUP )
				{
					if (m_btnNormalEquip.IsInside(x, y))
						m_bEventWearView = FALSE;
					else if (m_btnEventEquip.IsInside(x, y))
						m_bEventWearView = TRUE;
				}
				else if( msg == WM_MOUSEMOVE )
				{
				}
			}
		}
		return 1;

	case WM_LBUTTONDOWN:
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( !m_pNkCha )
				return 1;
			if(pCMyApp->m_bEquipLock==1)
				return 1;
			if(pCMyApp->m_bRingLock==1)
				return 1;
			if(pCMyApp->m_bBrLock==1)
				return 1;
			if(pCMyApp->m_bNecklaceLock==1)
				return 1;


			if(m_bAct_Masuk)
			{
				if(IsInside_Masuk_Back(x, y))
				{
					for(i=0; i<NUM_BTN_MASUK; i++)
					{
						if(m_Btn_Masuk[i].GetState() != BTN_DISABLE
								&& m_Btn_Masuk[i].IsInside(x, y) )
							m_Btn_Masuk[i].MsgProc(hWnd, msg, wParam, lParam);
					}
				}
				else
					Close_Masuk();

				return 1;
			}

			if( IsInside_Recycle(x, y) && m_pNkCha->m_ExtraSlot )
			{
				char buf[MAX_PATH] = {0,};
				sprintf(buf, (char*)G_STRING(IDS_RECYCLE_CONFIRM), GET_ITEM_NAME_VNUM(m_pNkCha->m_ExtraSlot->m_Vnum));
				int itemIndex = m_pNkCha->m_ExtraSlot->m_Index;
				g_pNk2DFrame->InsertPopup(buf, TYPE_NOR_OKCANCLE, POPUP_RECYCLE_ITEM, 30, 0, (void*)itemIndex);

				return 1;
			}

			if (bEnabled && IsInside(x, y))
			{
				if (m_bInvenLock)
				{
					DWORD curTime = timeGetTime();
					if (curTime > m_timeInvenLock + 1500)
						m_bInvenLock = FALSE;
					else
						return 1;
				}

				if( timeGetTime() - m_dwMouseClickTickTime < 300 )
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				if( g_bUseEventWear )
				{
					if( IsInsideEventWearFlag(x,y))
					{
						ToggleEventWearFlag();
						return 1;
					}

					if (m_btnNormalEquip.IsInside(x, y))
						m_btnNormalEquip.MsgProc(hWnd, msg, wParam, lParam);
					else if (m_btnEventEquip.IsInside(x, y))
						m_btnEventEquip.MsgProc(hWnd, msg, wParam, lParam);
				}

				if (m_CloseBtn.IsInside(x, y))
				{
					m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
				}
				//else if( m_SortBtn.IsInside(x, y) )
				//{
				//	m_SortBtn.MsgProc(hWnd, msg, wParam, lParam);
				//}
				else if (m_InvenBtn1.IsInside(x, y))
				{
					m_InvenBtn1.SetState(BTN_ACTION);
					m_InvenBtn2.SetState(BTN_NORMAL);
					m_InvenBtn3.SetState(BTN_NORMAL);
					m_InvenBtn4.SetState(BTN_NORMAL);
					m_pNkCha->ChangePackNum(0);
				}
				else if (m_InvenBtn2.IsInside(x, y))
				{
					m_InvenBtn1.SetState(BTN_NORMAL);
					m_InvenBtn2.SetState(BTN_ACTION);
					m_InvenBtn3.SetState(BTN_NORMAL);
					m_InvenBtn4.SetState(BTN_NORMAL);
					m_pNkCha->ChangePackNum(1);
				}
				else if (m_InvenBtn3.IsInside(x, y))
				{
					m_InvenBtn1.SetState(BTN_NORMAL);
					m_InvenBtn2.SetState(BTN_NORMAL);
					m_InvenBtn3.SetState(BTN_ACTION);
					m_InvenBtn4.SetState(BTN_NORMAL);
					m_pNkCha->ChangePackNum(2);
				}
				else if (m_InvenBtn4.IsInside(x, y))
				{
					m_InvenBtn1.SetState(BTN_NORMAL);
					m_InvenBtn2.SetState(BTN_NORMAL);
					m_InvenBtn3.SetState(BTN_NORMAL);
					m_InvenBtn4.SetState(BTN_ACTION);
					m_pNkCha->ChangePackNum(4);
				}
				else if (x >= m_slotstart_x
						 && x < m_slotstart_x + (SLOT_WIDTH * SLOT_X_NUM)
						 && y >= m_slotstart_y
						 && y < m_slotstart_y + (SLOT_HEIGHT * SLOT_Y_NUM) )
				{

					int slot_x_num, slot_y_num;
					if (m_pNkCha->m_ExtraSlot)
					{
						// Extra -> Inven
						if (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) % 2)
							slot_x_num = (x - m_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
						else
							slot_x_num = (x - m_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot)/2);
						if (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) % 2)
							slot_y_num = (y - m_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);
						else
							slot_y_num = (y - m_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot)/2);

						if (slot_x_num >= 0
								&& slot_x_num + GET_ITEM_WIDTH(m_pNkCha->m_ExtraSlot) - 1 < SLOT_X_NUM
								&& slot_y_num >= 0
								&& slot_y_num + GET_ITEM_HEIGHT(m_pNkCha->m_ExtraSlot) - 1 < SLOT_Y_NUM)
						{
							m_pNkCha->AddToInven(slot_x_num, slot_y_num);
							sprintf(m_MsgBuf, "inven %d %d %d\n", m_pNkCha->m_CurPackNum, slot_x_num, slot_y_num);
							SendMsg();
						}
					}
					else
					{
						if( pCMyApp->m_pMouse->GetMouseType() == M_REPAIR && g_SvrType == ST_ADULT_ONLY )
						{
							slot_x_num = ( x - m_slotstart_x ) / SLOT_WIDTH ;
							slot_y_num = ( y - m_slotstart_y ) / SLOT_HEIGHT ;

							sprintf(m_MsgBuf, "repair %d %d %d\n", m_pNkCha->m_CurPackNum, slot_x_num, slot_y_num);
							SendMsg();
							pCMyApp->m_pMouse->SetMouseType(M_REPAIR);
							return 1;
						}
						else
						{
							m_pNkCha->AddToInven((x-m_slotstart_x) / SLOT_WIDTH, (y-m_slotstart_y) / SLOT_HEIGHT);
							sprintf(m_MsgBuf, "inven %d %d %d\n", m_pNkCha->m_CurPackNum, (x-m_slotstart_x) / SLOT_WIDTH, (y-m_slotstart_y) / SLOT_HEIGHT);
							SendMsg();
						}
					}
				}
				else
				{
					if (m_pNkCha->m_ExtraSlot)
					{

						int where;
						if (IsInsideEquip(x, y))
						{
							if( g_bUseEventWear && m_bEventWearView )
							{
								if( GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_EVENT_WEAR )
								{
									if ((where = m_pNkCha->EventWear(-1)) >= 0)
									{
										if( where == WEARING_HELMET || where == WEARING_ARMOR || where == WEARING_PANTS
												|| where == WEARING_CLOAK || where == WEARING_SHOES || where == WEARING_SHIELD || where == WEARING_RING
												|| where == WEARING_WEAPON)
										{
											sprintf(m_MsgBuf, "ev_wear %d\n", where);
											SendMsg();

											m_pNkCha->SetDefense();
											m_pNkCha->OnBossRaidHeroEffect();
										}
									}
								}

								return 1;
							}

							if ((where = WhichWearing(x, y)) >= 0)
							{
								if( pCMyApp->m_pUIMgr->m_nStoneState[0] == 1 && g_pRoh->GetMotionState() == CHA_ATTACK )
									return 1;
								if( TRUE == procEventEnchant(where) )
								{
									return 1;
								}

								if( m_pNkCha->m_ExtraSlot->m_Vnum == 1327 || m_pNkCha->m_ExtraSlot->m_Vnum == 1328 || m_pNkCha->m_ExtraSlot->m_Vnum == 1329 ) // �׷������� �ϳ��̸�..
								{
									if( !(m_pNkCha->m_Wearing[where]) ) // ���� ��� ������..
										return 1;

									int EdrMinus = m_pNkCha->m_Wearing[where]->m_MaxUpgradeEndurance - m_pNkCha->m_Wearing[where]->m_UpgradeEndurance; // �پ����ִ� ������ ��.

									if( (m_pNkCha->m_ExtraSlot->m_Vnum==1327 && EdrMinus>=20) // �׷�����(��) 20 ����.
											|| (m_pNkCha->m_ExtraSlot->m_Vnum==1328 && EdrMinus>=40)  // �׷�����(��) 40 ����.
											|| (m_pNkCha->m_ExtraSlot->m_Vnum==1329 && EdrMinus>=60)  // �׷�����(��) 60 ����.
									  )
									{
										sprintf( m_MsgBuf, "fill_item %d\n", where );
										SendMsg();

										SAFE_DELETE(m_pNkCha->m_ExtraSlot);

										return 1;
									}
									else
									{
									}
								}

								if( where==3 &&
										m_pNkCha->m_Wearing[where] &&
										GET_SHAPE(m_pNkCha->m_Wearing[where])==ISUB_L_EQUIP &&
										(m_pNkCha->m_ExtraSlot) &&
										GET_SHAPE(m_pNkCha->m_ExtraSlot)==ISUB_S_EQUIP &&
										m_pNkCha->m_ExtraSlot->m_Vnum>=829 &&
										m_pNkCha->m_ExtraSlot->m_Vnum<=844 )
								{
									if(m_pNkCha->m_ExtraSlot->m_Vnum==841)
										return 1;

									if( m_pNkCha->m_Wearing[where]->m_bTimeLimit || m_pNkCha->m_ExtraSlot->m_bTimeLimit )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

										return 1;
									}

									sprintf(m_MsgBuf, "combine\n");
									SendMsg();
									SAFE_DELETE(m_pNkCha->m_ExtraSlot);
									pCMyApp->m_bEquipLock = TRUE;//1009

									for(int i = 0 ; i <= 5 ; i++)
										m_pNkCha->m_bQuickSlotLock[i] = TRUE;

									return 1;
								}
								else if( where == IARMOR_BRACELET &&
										 m_pNkCha->m_Wearing[where] &&
										 m_pNkCha->m_Wearing[where]->m_Vnum == 1087 &&
										 m_pNkCha->m_ExtraSlot )
								{
									if( m_pNkCha->m_ExtraSlot->m_Vnum >= 1088 &&
											m_pNkCha->m_ExtraSlot->m_Vnum <= 1097 )
									{
										int up_level = 0;

										for (int sp_idx = 4; sp_idx < 14; sp_idx++)
										{
											int att_level = (m_pNkCha->m_Wearing[where]->m_Special & g_BraceletSpecialMask[sp_idx]) >> g_BraceletSpecialShift[sp_idx]; // �� �Ӽ� �� ������ ���´�.

											if( att_level > 0 ) // 0���� Ŭ���� ����. (������ ������ �𸣴� üũ�ؼ� ������Ų��.)
												up_level += att_level;
										}

										if( up_level > 4 ) // 5���ʹ� ��� �޼��� ���.
										{
											m_byPopupType = 3;
											if( g_pNk2DFrame->IsShopVisible() )
											{
												g_pNk2DFrame->ShowShopWindow( FALSE );
												g_pNk2DFrame->ShowInvenWindow( TRUE );
											}
											//-- IDS_UPGRADE_11_WARNING : ���н� ��� ����� �� �ֽ��ϴ�. �õ��Ͻðڽ��ϱ�?
											g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_11_WARNING),TYPE_NOR_OKCANCLE, 26);
											pCMyApp->m_bInverseReturn = TRUE;
										}
										else
										{
											sprintf(m_MsgBuf, "upbr 0\n");
											SendMsg();
											SAFE_DELETE(m_pNkCha->m_ExtraSlot);
											pCMyApp->m_bBrLock = TRUE;//1009
											for(int i = 0 ; i <= 5 ; i++)
												m_pNkCha->m_bQuickSlotLock[i] = TRUE;
										}

										return 1;
										///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									}
									//-- ��� ��� �ִ´ٸ�
									else if(m_pNkCha->m_ExtraSlot->m_Vnum != 1087
											&& GET_SHAPE(m_pNkCha->m_ExtraSlot) == IARMOR_BRACELET)
									{
										if( m_pNkCha->m_Wearing[where]->m_bTimeLimit || m_pNkCha->m_ExtraSlot->m_bTimeLimit ) // �ð��� �������� �и� ����. 06-03-23 ����
										{
											//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
											g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

											return 1;
										}

										//-- Plus Item Plus ���� ��� â.
										if(m_pNkCha->m_ExtraSlot->m_PlusNum!=0)
										{
											m_byPopupType = 2;
											if( g_pNk2DFrame->IsShopVisible() )
											{
												g_pNk2DFrame->ShowShopWindow( FALSE );
												g_pNk2DFrame->ShowInvenWindow( TRUE );
											}
											//-- IDS_INVEN_WARNING_RING : �÷��� ������ �ռ��� �÷����� ������ϴ�.
											g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_WARNING_RING),TYPE_NOR_OKCANCLE, 26);
											pCMyApp->m_bInverseReturn = TRUE;
										}
										else
										{
											if(!pCMyApp->m_bBrLock)
											{
												sprintf(m_MsgBuf, "upbr 1\n");
												SendMsg();
												SAFE_DELETE(m_pNkCha->m_ExtraSlot);
												pCMyApp->m_bBrLock = TRUE;//1009
												for(int i = 0 ; i <= 5 ; i++)
													m_pNkCha->m_bQuickSlotLock[i] = TRUE;
											}
										}
										return 1;
									}
								}
								else if( where==IARMOR_NECKLACE &&
										 m_pNkCha->m_Wearing[where] &&
										 m_pNkCha->m_ExtraSlot &&
										 GET_SHAPE(m_pNkCha->m_ExtraSlot)==IARMOR_NECKLACE &&
										 m_pNkCha->m_ExtraSlot->m_Vnum!=1399 &&
										 m_pNkCha->m_Wearing[IARMOR_NECKLACE]->m_Vnum == 1399 )
								{
									if( m_pNkCha->m_ExtraSlot->m_PlusNum != 0 )
									{
										m_byPopupType = 5;

										if( g_pNk2DFrame->IsShopVisible() )
										{
											g_pNk2DFrame->ShowShopWindow( FALSE );
											g_pNk2DFrame->ShowInvenWindow( TRUE );
										}
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_FUSION_PLUSREMOVE), TYPE_NOR_OKCANCLE, 26 );
									}
									else if( !pCMyApp->m_bRingLock )
									{
										if( m_pNkCha->m_ExtraSlot->m_bTimeLimit && g_dwClientCountry == CTRY_JPN )
										{
											g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

											return 1;
										}

										sprintf(m_MsgBuf, "upnk 1\n");
										SendMsg();
										SAFE_DELETE(m_pNkCha->m_ExtraSlot);
										pCMyApp->m_bNecklaceLock = TRUE;
										for(int i = 0 ; i <= 5 ; i++)
											m_pNkCha->m_bQuickSlotLock[i] = TRUE;
									}

									return 1;
								}
								else if( where==IARMOR_RING &&
										 m_pNkCha->m_Wearing[where] &&
										 m_pNkCha->m_ExtraSlot &&
										 GET_SHAPE(m_pNkCha->m_ExtraSlot)==IARMOR_RING  &&
										 m_pNkCha->m_ExtraSlot->m_Vnum!=922 &&
										 m_pNkCha->m_ExtraSlot->m_Vnum!=933 &&
										 m_pNkCha->m_ExtraSlot->m_Vnum!=934 &&
										 m_pNkCha->m_ExtraSlot->m_Vnum!=935 &&
										 m_pNkCha->m_ExtraSlot->m_Vnum!=1032 &&
										 m_pNkCha->m_ExtraSlot->m_Vnum !=1777 &&
										 (m_pNkCha->m_Wearing[IARMOR_RING]->m_Vnum == 922 ||
										  m_pNkCha->m_Wearing[IARMOR_RING]->m_Vnum == 933  ||
										  m_pNkCha->m_Wearing[IARMOR_RING]->m_Vnum == 934 ||
										  m_pNkCha->m_Wearing[IARMOR_RING]->m_Vnum == 935 ||
										  m_pNkCha->m_Wearing[IARMOR_RING]->m_Vnum == 1032 ) )
								{
									if( m_pNkCha->m_Wearing[where]->m_bTimeLimit || m_pNkCha->m_ExtraSlot->m_bTimeLimit ) // �ð��� �������� �и� ����. 06-03-23 ����
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

										return 1;
									}

									if(m_pNkCha->m_ExtraSlot->m_PlusNum!=0)
									{
										m_byPopupType = 1;
										if( g_pNk2DFrame->IsShopVisible() )
										{
											g_pNk2DFrame->ShowShopWindow( FALSE );
											g_pNk2DFrame->ShowInvenWindow( TRUE );
										}
										g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_WARNING_RING),TYPE_NOR_OKCANCLE, 26);
										pCMyApp->m_bInverseReturn = TRUE;
									}
									else if(!pCMyApp->m_bRingLock)
									{
										sprintf(m_MsgBuf, "rg 1\n");
										SendMsg();
										SAFE_DELETE(m_pNkCha->m_ExtraSlot);
										pCMyApp->m_bRingLock = TRUE;//1009
										for(int i = 0 ; i <= 5 ; i++)
											m_pNkCha->m_bQuickSlotLock[i] = TRUE;
									}

									return 1;
								}
								else if( m_pNkCha->m_Wearing[where] &&
										 m_pNkCha->m_Wearing[where]->m_PlusNum == 4 &&
										 m_pNkCha->m_Wearing[where]->m_GemNum == 4)
								{
									nExtraShape = GET_SHAPE(m_pNkCha->m_ExtraSlot);
									if( GET_TYPE(m_pNkCha->m_ExtraSlot) == ITYPE_ETC &&
											( nExtraShape == IETC_GEMSTONE || nExtraShape == IETC_GEM ) )
									{
										int minLevel = GET_ITEM_LEVEL(m_pNkCha->m_Wearing[where]);
										int curVnum = m_pNkCha->m_ExtraSlot->m_Vnum;
										if( g_dwLangType == LANG_KOR )
										{
											if( minLevel >=0  && minLevel <= 30  && (curVnum == 222 || curVnum == 1960) ||
													minLevel >30  && minLevel <= 60  && (curVnum == 223 || curVnum == 1961) ||
													minLevel >60  && minLevel <= 90  && (curVnum == 224 || curVnum == 1962) ||
													minLevel >90  && minLevel <= 120 && (curVnum == 225 || curVnum == 1963) ||
													minLevel >120 && minLevel <= 150 && (curVnum == 226 || curVnum == 1964) ||
													minLevel >150 && minLevel <= 180 && (curVnum == 227 || curVnum == 1965) )
											{
												//-- IDS_INVEN_PLUSE5_WORRING : �÷��� 5 ���
												if( g_SvrType == ST_ADULT_ONLY ) // IDS_INVEN_PLUSE5_WORRING1 "+5 ���� �� �������� �ı��� �� �ֽ��ϴ�. �õ� �Ͻðڽ��ϱ�?"
													g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_PLUSE5_WORRING1), TYPE_NOR_OKCANCLE, 26);
												else
													g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_PLUSE5_WORRING),TYPE_NOR_OKCANCLE, 26);

												if(g_pNk2DFrame->IsShopVisible())
												{
													g_pNk2DFrame->ShowShopWindow(false);

													if(!g_pNk2DFrame->IsInvenVisible())
														g_pNk2DFrame->ShowInvenWindow(true);
												}
												m_byPopupType = 4;
												m_iCurWhere = where;
												return 1;
											}
										}
										else
										{
											if( minLevel >=0  && minLevel <= 30  && curVnum == 222 ||
													minLevel >30  && minLevel <= 60  && curVnum == 223 ||
													minLevel >60  && minLevel <= 90  && curVnum == 224 ||
													minLevel >90  && minLevel <= 120 && curVnum == 225 ||
													minLevel >120 && minLevel <= 150 && curVnum == 226 ||
													minLevel >150 && minLevel <= 180 && curVnum == 227 )
											{
												//-- IDS_INVEN_PLUSE5_WORRING : �÷��� 5 ���
												if( g_SvrType == ST_ADULT_ONLY ) // IDS_INVEN_PLUSE5_WORRING1 "+5 ���� �� �������� �ı��� �� �ֽ��ϴ�. �õ� �Ͻðڽ��ϱ�?"
													g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_PLUSE5_WORRING1), TYPE_NOR_OKCANCLE, 26);
												else
													g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_PLUSE5_WORRING),TYPE_NOR_OKCANCLE, 26);

												if(g_pNk2DFrame->IsShopVisible())
												{
													g_pNk2DFrame->ShowShopWindow(false);

													if(!g_pNk2DFrame->IsInvenVisible())
														g_pNk2DFrame->ShowInvenWindow(true);
												}
												m_byPopupType = 4;
												m_iCurWhere = where;
												return 1;
											}
										}
									}
								}

								if ((where = m_pNkCha->Upgrade(where)) >= 0)
								{
									if(m_pNkCha->m_ExtraSlot->m_Vnum >=5400 && m_pNkCha->m_ExtraSlot->m_Vnum <= 5405)
									{
										sprintf(m_MsgBuf, "sp_upgrade %d\n", where);
										SendMsg();
										SAFE_DELETE(m_pNkCha->m_ExtraSlot);
									}
									else
									{
									sprintf(m_MsgBuf, "upgrade %d\n", where);
									SendMsg();
									SAFE_DELETE(m_pNkCha->m_ExtraSlot);
									}
								}
							}

							if( !(CHA_STAND <= m_pNkCha->GetMotionState() && m_pNkCha->GetMotionState() <= CHA_DEAD) )
								return 1;

							if(m_pNkCha->CanWear() != 3 ||
									( !m_pNkCha->m_bresstone && !m_pNkCha->m_bstone ) )
							{
								if( check_disable_normal() == false )
								{
									if ((where = m_pNkCha->Wear(-1)) >= 0)
									{
										m_pNkCha->SetDefense();									
										m_pNkCha->OnBossRaidHeroEffect();									

										sprintf(m_MsgBuf, "wear %d\n", where);
										SendMsg();									
									}
								}
							}


							///--------------------------------------------------------------
							//-- ���� ���׷��̵�.
							if(m_pNkCha->m_ExtraSlot !=NULL)
							{

								int vnum = m_pNkCha->m_ExtraSlot->m_Vnum;
								int level;


								if( g_SvrType == ST_ADULT_ONLY )
								{
									if(vnum == 554
									  )
									{
										where = WhichWearing(x, y);
										if (where == 9 )// || where == 0 || where == 1 || where == 2 || where == 3 || where == 4 || where == 5 )
										{
											if(m_pNkCha->m_Wearing[where] && !(GET_ATT_FLAG(m_pNkCha->m_Wearing[where]) & IATT_NO_GEM))
											{
												if(m_pNkCha->m_Wearing[where]->m_Vnum >=0)
												{
													// �������⿡ ���������� �ʵȴ�..�Ѥ�;
													if(where == 3)
													{
														if(m_pNkCha->m_Wearing[3]->m_Vnum == 614)
															return 1;
														if(GET_ATT_FLAG(m_pNkCha->m_Wearing[3]) & IATT_SUB_WEAPON)
															return 1;
													}
													else if(where == 9)
													{
														if(GET_ATT_FLAG(m_pNkCha->m_Wearing[9]) & IATT_SUB_WEAPON)
															return 1;
													}



													m_iCurWhere = where;		// ������ ��ġ.
													m_bAct_Masuk = TRUE;
													level = GET_ITEM_LEVEL(m_pNkCha->m_Wearing[where]);
													m_x = x, m_y = y;
													CalcPos_Masuk(x, y);

													Divergence(where, vnum, level, m_x, m_y);
													return 1;
												}
											}
										}
									}
								}
								else
								{
									if( vnum == 551 || vnum == 552 || vnum == 553 || vnum == 554
									  )
									{
										where = WhichWearing(x, y);
										if (where == 0 || where == 1 || where == 2 || where == 3 || where == 4 || where == 5 || where == 9)
										{
											if(m_pNkCha->m_Wearing[where] && !(GET_ATT_FLAG(m_pNkCha->m_Wearing[where]) & IATT_NO_GEM))
											{
												if(m_pNkCha->m_Wearing[where]->m_Vnum >=0)
												{
													// �������⿡ ���������� �ʵȴ�..�Ѥ�;
													if(where == 3)
													{
														if(m_pNkCha->m_Wearing[3]->m_Vnum == 614)
															return 1;
														if(GET_ATT_FLAG(m_pNkCha->m_Wearing[3]) & IATT_SUB_WEAPON)
															return 1;
													}
													else if(where == 9)
													{
														if(GET_ATT_FLAG(m_pNkCha->m_Wearing[9]) & IATT_SUB_WEAPON)
															return 1;
													}


													m_iCurWhere = where;		// ������ ��ġ.
													m_bAct_Masuk = TRUE;
													level = GET_ITEM_LEVEL(m_pNkCha->m_Wearing[where]);
													m_x = x, m_y = y;
													CalcPos_Masuk(x, y);

													Divergence(where, vnum, level, m_x, m_y);
													return 1;
												}
											}
										}
									}
								}

								if (vnum >= 865 && vnum <= 871) //-- ����� ������ġ ���༮7(���ȵ�)
								{
									where = WhichWearing(x, y);
									if (where == WEARING_SHOES && m_pNkCha->m_Wearing[where] &&
											!IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_FLY, GET_ITEM_NUM0(m_pNkCha->m_ExtraSlot)-20) )
									{
										strcpy(m_MsgBuf, "up_sky\n");
										SendMsg();
										SAFE_DELETE( m_pNkCha->m_ExtraSlot );
									}
								}
							}
						}


						//
					}
					//-- ������ �������� �� ���
					//-- 2005.02.14 / Lyul
					//-- ����� ������ ����ϰ� ���� ��� ����� ������ �� �� ���� ����
					//-- ��Ÿ ���� �������� ��쵵 �� ��� �Ϸ��� ���⼭ üũ
					else
					{

						if( pCMyApp->m_pUIMgr && !pCMyApp->m_pUIMgr->m_bPickLock ) // �� ���� �ƴҶ��� �����ְ� ó�� ( by ���� ) ///pick_bug
						{
							// �±��� ����: ���� ��ų ���� �ֺ� ĳ���� �ñ�� ���׷� ���� ��ų ��� �� ���� ������ Ŭ�� �Ұ�
							if( !(CHA_STAND <= m_pNkCha->GetMotionState() && m_pNkCha->GetMotionState() <= CHA_DEAD) )
								return 1;

							if( pCMyApp->m_pUIMgr->m_nStoneState[0] == 1 && g_pRoh->GetMotionState() == CHA_ATTACK )
								return 1;

							for (i = 0; i < WEARING_NUM; i++)
							{
								if (x >= m_EquipSlot[i].m_X
										&& x < m_EquipSlot[i].m_X + m_EquipSlot[i].m_W
										&& y >= m_EquipSlot[i].m_Y
										&& y < m_EquipSlot[i].m_Y + m_EquipSlot[i].m_H)
								{

									///�̺�Ʈ�������� �ý��� ���� �˻��Ѵ�.
									if( g_bUseEventWear && m_bEventWearView ) ///�̺�Ʈ�������� �������� ���׷��̵尡 �ȵǵ��� ó��.
									{
										if( m_EventWearTime != 0 ) // �ð� ���� üũ
										{
											// �������ִ� ��Ȳ�̶��...
											if( i < WEARING_SHOW_NUM && m_pNkCha->EventWear(i) >= 0 ) // ���� ���� üũ.
											{
												sprintf(m_MsgBuf, "ev_wear %d\n", i);
												SendMsg();

												m_pNkCha->SetDefense();
												m_pNkCha->OnBossRaidHeroEffect();
											}
											else if( m_pNkCha->EventWear(i) >= 0 )
											{
												sprintf(m_MsgBuf, "ev_wear %d\n", i);
												SendMsg();

												m_pNkCha->SetDefense();
												m_pNkCha->OnBossRaidHeroEffect();
											}
										}

										return 1; // �̺�Ʈ ���� ���� �϶� Ŭ���ߴٸ� ���� ĵ��.
									}

									// [5/30/2007 Theodoric] ������ ���� :: ������ ������ ����. ���콺 Ÿ������ ������ �Ұ��� �����Ѵ�.
									if( pCMyApp->m_pMouse->GetMouseType() == M_REPAIR  && g_SvrType == ST_ADULT_ONLY )
									{
										sprintf(m_MsgBuf, "repair 99 %d 0\n", i);
										SendMsg();
										pCMyApp->m_pMouse->SetMouseType(M_REPAIR); // ��� �������� ó�� �Ǿ� �ִ�.������ ������ �ٲ�� �ִ� ���� ������ ������ �ٲ��־� Ǯ����.
										return 1;
									}

									//-- ����� ���� üũ
									if( m_pNkCha->m_Wearing[i] &&
											GET_SHAPE(m_pNkCha->m_Wearing[i]) != ISUB_DSTONE ||
											(!m_pNkCha->m_bresstone && !m_pNkCha->m_bstone) )
									{
										if( check_disable_normal() == false )
										{
											if (m_pNkCha->Wear(i) >= 0)
											{
												m_pNkCha->SetDefense();
												m_pNkCha->OnBossRaidHeroEffect();
												sprintf(m_MsgBuf, "wear %d\n", i);
												if( pCMyApp->m_pUIMgr->m_bRClickDown == 1 &&
														pCMyApp->DrawCasting )
												{
													m_pNkCha->HellDownFail( g_pRoh, NULL );
												}
												SendMsg();
											}
										}
										break;
									}
								}
							}
						}
					}
				}
			}
			else if(m_pNkCha->m_ExtraSlot && IsPlayArea(x, y, bWindowOpened) &&
					CommonConfig::Instance()->GetDontDrop() == false )
			{
				// pick_extra �޽����� �����ϴ� ��ƾ.
				if ( (timeGetTime()-m_timePickExtraTime) < 2000)
					return 0;

				nExtraShape = GET_SHAPE(m_pNkCha->m_ExtraSlot);

				if((GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_DROP)
						&& m_pNkCha->m_ExtraSlot->m_Vnum != 613)
				{
					if( pCMyApp->GetCurWorld() == 19 ) // WORLD_MATRIX_2009
					{
						if( m_pNkCha->m_ExtraSlot->m_LimitTime == 0 )
							return 1;
					}
					else
						return 1;
				}

				if( m_pNkCha->m_ExtraSlot->m_PriceType == 2 ) // ĳ�� ������ �Ұ�ó�� �߰�. ///�������
					return 1;

				if( m_pNkCha->m_ExtraSlot->m_bTimeLimit ) // �ð��� �������̸� ��� �Ұ�ó�� (by ����) ///�ð���������
				{
					if( g_dwClientCountry == CTRY_KOR ) // �ѱ��ܿ��� ������ �Ұ�.
					{
						//-- IDS_TIMELIMIT_ITEM_DROP : �ð��� �������� ������ �����˴ϴ�. �����ðڽ��ϱ�?
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_TIMELIMIT_ITEM_DROP) ,TYPE_NOR_OKCANCLE, 28);
						pCMyApp->m_bInverseReturn = TRUE;
					}

					return 1;
				}
				if( CheckIsConversionItem(m_pNkCha->m_ExtraSlot) )
				{
					if( CheckIsConversionItem(m_pNkCha->m_ExtraSlot , 0 , 1 ) )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DROP_REALY),TYPE_NOR_OKCANCLE, 26);
						pCMyApp->m_bInverseReturn = TRUE;
						return 1;
					}
					else
					{
						//-- IDS_CONVERSIONED_ITEM_DROP : ������ �� �������Դϴ�. �����ðڽ��ϱ�?
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_CONVERSIONED_ITEM_DROP),TYPE_NOR_OKCANCLE, 26);
						pCMyApp->m_bInverseReturn = TRUE;
						return 1;
					}
				}

				if( CheckIsGodItem(m_pNkCha->m_ExtraSlot) )
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DROP_REALY),TYPE_NOR_OKCANCLE, 26);
					pCMyApp->m_bInverseReturn = TRUE;
					return 1;
				}
				
				if( CommonConfig::Instance()->GetWeapon360() == true )
				{				
					if( CheckIsConversionItem(m_pNkCha->m_ExtraSlot,2)
							|| CheckIsConversionItem(m_pNkCha->m_ExtraSlot,1) )
					{
						g_pNk2DFrame->InsertPopup("It is an expensive item. Would you like to?",TYPE_NOR_OKCANCLE, 26);	// 5:POPUP_SHOP
						return 1;
					}
				}

				if(m_pNkCha->m_ExtraSlot->m_PlusNum >0)
				{
					//2003 ����������� �÷��� ���������� �ν��ϴ� �� ����
					if(GET_TYPE(m_pNkCha->m_ExtraSlot)!=ITYPE_SUB)
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_PLUS_ITEM),TYPE_NOR_OKCANCLE, 26);
						pCMyApp->m_bInverseReturn = TRUE;
						return 1;
					}
				}
				//==========================================================================
				// nate 2004 - 4
				// ���� ������ Ȯ�� �ý���
				// �����̾�, ��ũ��, ����å, �ϵ�, �������, �������
				// äũ�ϰ� ����ڿ��� ��¥�� ������ �����.
				if( GET_TYPE( m_pNkCha->m_ExtraSlot ) == ITYPE_SUB )
				{
					if( GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_SLAYER
							|| GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_CHAKRAM
							|| GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_BOOK
							|| GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_WAND
							|| GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_S_EQUIP
							|| GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_L_EQUIP
							|| GET_SHAPE( m_pNkCha->m_ExtraSlot ) == ISUB_PANEL
					  )
					{
						// IDS_DROP_REALY:���� �����ðڽ��ϱ�?
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_DROP_REALY),TYPE_NOR_OKCANCLE, 26);
						pCMyApp->m_bInverseReturn = TRUE;
						return 1;
					}
				}
				//==========================================================================
				if(GET_TYPE(m_pNkCha->m_ExtraSlot) == ITYPE_ETC && (nExtraShape == IETC_GEMSTONE || nExtraShape == IETC_GEM))
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_JEWEL_ITEM),TYPE_NOR_OKCANCLE, 26);
					pCMyApp->m_bInverseReturn = TRUE;
					return 1;
				}
				if(GET_TYPE(m_pNkCha->m_ExtraSlot) == ITYPE_ETC && (m_pNkCha->m_ExtraSlot->m_Vnum >=550 && m_pNkCha->m_ExtraSlot->m_Vnum <=554))
				{
					//-- IDS_INVEN_MASUCK_DROP : �����Դϴ�. ���� �����ðڽ��ϱ�?
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_MASUCK_DROP),TYPE_NOR_OKCANCLE, 26);
					pCMyApp->m_bInverseReturn = TRUE;
					return 1;
				}
				if(m_pNkCha->m_ExtraSlot->m_Special > 0
						&& !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_DROP))
				{
					if(GET_TYPE(m_pNkCha->m_ExtraSlot)!=ITYPE_SUB)
					{
						if(GET_TYPE(m_pNkCha->m_ExtraSlot)==ITYPE_ETC )
						{
							if( GET_SHAPE(m_pNkCha->m_ExtraSlot)!=IETC_SUMMON
							  )
							{
								//-- IDS_INVEN_SPECIALITEM_DROP : ����� ������ �Դϴ�. �����ðڽ��ϱ�?
								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_SPECIALITEM_DROP),TYPE_NOR_OKCANCLE, 26);
								pCMyApp->m_bInverseReturn = TRUE;
								return 1;

							}
							else
							{
							}
						}
					}
				}

				if( m_pNkCha->m_ExtraSlot ) // ������ ������ Ȯ��â ����ִ� ����ó�� ǰ���..
				{
					switch( m_pNkCha->m_ExtraSlot->m_Vnum )
					{
					case 1521:
					case 1522:
					case 1523:
					case 1524:
					case 1709:
					case 1710:
					case 1711: // [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ���̺� �ҿ�
					case 1791:
					case 1827:
					case 1828:
					case 1829:
					case 1830:
					case 2163: // [2008/11/3 Theodoric] ���� �����̾� ��ǰ
						{
							//-- IDS_INVEN_SPECIALITEM_DROP : ����� ������ �Դϴ�. �����ðڽ��ϱ�?
							g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_INVEN_SPECIALITEM_DROP),TYPE_NOR_OKCANCLE, 26);
							pCMyApp->m_bInverseReturn = TRUE;
							return 1;
						}
					}
				}				

				if( !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_DROP) )
				{
					sprintf(m_MsgBuf, "drop 1\n");
					SendMsg();
					SAFE_DELETE( m_pNkCha->m_ExtraSlot );
				}

				if(m_pNkCha->m_ExtraSlot)
				{
					if(m_pNkCha->m_ExtraSlot->m_Vnum == 613)
						return 0;
				}


			}
			else
			{
				return 0;
			}

			Close_Masuk();
		}
		return 1;

	case WM_RBUTTONDOWN:

		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);
			if( !m_pNkCha )
				return 0;
			
			if (bEnabled && IsInside(x, y))
			{	
				/////////2003 ���콺 ��Ŭ������ ������񿡼� ������� ����.
				if (!m_pNkCha->m_ExtraSlot)
				{
					int where;
					if (IsInsideEquip(x, y) && !(g_bUseEventWear&&m_bEventWearView) ) ///�̺�Ʈ�������� �������� ���׷��̵尡 �ȵǵ��� ó��.
					{
						if ((where = WhichWearing(x, y)) >= 0 && m_pNkCha->m_Wearing[where] )
						{
							if( m_pNkCha->m_Wearing[where]->m_bTimeLimit ) // �ð��� �������� �и� ����. 06-03-23 ����
							{
								//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
								g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

								return 1;
							}

							if(where==3 && m_pNkCha->m_Wearing[where]
									&& GET_TYPE(m_pNkCha->m_Wearing[where])==ITYPE_SUB
									&& GET_SHAPE(m_pNkCha->m_Wearing[where])==ISUB_L_EQUIP
									&& m_pNkCha->m_Wearing[where]->m_Special !=0)
							{
								sprintf(m_MsgBuf, "split\n");
								SendMsg();

								pCMyApp->m_bEquipLock=TRUE;//������� ��.1009
								for(int i = 0 ; i <= 5 ; i++)
								{
									m_pNkCha->m_bQuickSlotLock[i] = TRUE;
								}
							}
							//-- Evolution ����� �Ϲ� ���� �и�. 2004.10.18/Lyul
							else if(where==IARMOR_BRACELET && m_pNkCha->m_Wearing[where]
									&& GET_SHAPE(m_pNkCha->m_Wearing[where])==IARMOR_BRACELET
									&& m_pNkCha->m_Wearing[where]->m_Vnum==1087 )
							{
								if(!pCMyApp->m_bBrLock)
								{
									sprintf(m_MsgBuf, "upbr -1\n");
									SendMsg();

									pCMyApp->m_bBrLock=TRUE;//���� ��
									for(int i = 0 ; i <= 5 ; i++)
										m_pNkCha->m_bQuickSlotLock[i] = TRUE;
								}
							}
							// 345���� ������ ������ ����� �ռ�. ///��
							else if( where==IARMOR_NECKLACE
									 && m_pNkCha->m_Wearing[where]
									 && GET_SHAPE(m_pNkCha->m_Wearing[where])==IARMOR_NECKLACE
									 && m_pNkCha->m_Wearing[where]->m_Vnum==1399
								   )
							{
								if( !pCMyApp->m_bNecklaceLock )
								{
									sprintf(m_MsgBuf, "upnk -1\n");
									SendMsg();

									pCMyApp->m_bNecklaceLock = TRUE; //����� ��
									for(int i = 0 ; i<= 5 ; i++)
										m_pNkCha->m_bQuickSlotLock[i] = TRUE;
								}
							}
							//-- ������ ���� �Ϲ� ���� �и�. 2004.10.18/Lyul
							else if(where==IARMOR_RING
									&& m_pNkCha->m_Wearing[where]
									&& GET_SHAPE(m_pNkCha->m_Wearing[where])==IARMOR_RING
									&& (m_pNkCha->m_Wearing[where]->m_Vnum==922
										|| m_pNkCha->m_Wearing[where]->m_Vnum==933
										|| m_pNkCha->m_Wearing[where]->m_Vnum==934
										|| m_pNkCha->m_Wearing[where]->m_Vnum==935
										|| m_pNkCha->m_Wearing[where]->m_Vnum==1032 ))
							{
								if(pCMyApp->GetCurWorld() != 10 && !pCMyApp->m_bRingLock)
								{
									sprintf(m_MsgBuf, "rg 0\n");
									SendMsg();

									pCMyApp->m_bRingLock=TRUE;//���� ��
									for(int i = 0 ; i<= 5 ; i++)
										m_pNkCha->m_bQuickSlotLock[i] = TRUE;
								}
							}//-- end of if ������
						}
					}
					else if (x >= m_slotstart_x
						&& x < m_slotstart_x + (SLOT_WIDTH * SLOT_X_NUM)
						&& y >= m_slotstart_y
						&& y < m_slotstart_y + (SLOT_HEIGHT * SLOT_Y_NUM) )
					{	
						int slot_x_num = (x-m_slotstart_x) / SLOT_WIDTH;
						int slot_y_num = (y-m_slotstart_y) / SLOT_HEIGHT;

						int index = m_pNkCha->GetItemIndexInSlot(m_pNkCha->m_CurPackNum, slot_x_num, slot_y_num, 1, 1);
						if (index < 0)
						{
							return 1;
						}

						CItem* pItem = m_pNkCha->FindItemByIndexFromAll(index);
						if (pItem == NULL)
						{
							return 1;
						}

						if (GET_TYPE(pItem) != ITYPE_PET)
						{
							return 1;
						}
						
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_NEWPET_MSG2), TYPE_NOR_OKCANCLE, POPUP_PET_EQUIP, 30, 0, (void*)pItem->m_Index);
					}
				}
				return 1;
			}
		}
		break;
	//======================================================================================
	// nate
	// â�� ������ �ټ� �̵� ���
	case WM_RBUTTONDBLCLK:
		{
			// ExtraSlot �� �������� ���� ��쿡 �۵� ����
			if( !m_pNkCha || m_pNkCha->m_ExtraSlot )
				return 0;

			x = LOWORD (lParam);
			y = HIWORD (lParam);

			// ���� ������ �˻�
			if (!IsInside(x, y))
				return 0;


			// ���λ��� ���� �̵� ó�� ///���λ���
			if( g_pNk2DFrame->IsUserShopVisible() ) // ���λ����� �����ִٸ�...
			{
				return g_pNk2DFrame->GetControlUserShop()->MsgProc( hWnd, msg, wParam, lParam ); // ���� Ŭ������ �޼����� �Űܼ� ó�����ش�.(Ŭ���� �κ����� ������ ó���� ���� Ŭ��������..)
			}

			//===========================================================
			// nate 2004 - 4
			// exchange
			// â�� â�� ���ȴ��� Ȯ���ϰ� ���������� ����
			if( !g_pNk2DFrame->GetControlGStorehouse()->IsActivate() )
				if( !g_pNk2DFrame->GetControlWareHouse()->IsActivate()
						&& !g_pNk2DFrame->GetControlGStorehouse()->IsActivate()
						&& !g_pNk2DFrame->IsExchangeVisible() && !g_pNk2DFrame->IsUpgradeVisible()	// ��ȭ�������̽� �󿡼��� �����̵� �����ϰ� ó�� 05-11-24 ����
						&& !( g_pNk2DFrame->IsGambleVisible() && g_pNk2DFrame->GetGamble()->m_bType == 0 ) 	// Ĩ���� �������̽� �󿡵� �����̵� ����
						&& !g_pNk2DFrame->IsShopVisible()	// �������� ///�����Ǹ�
						&& !g_pNk2DFrame->IsEventAncientVisible() )
					return 0;

			//===========================================================
			// Ŭ���� ��ġ�� �ι� �ε��� �˻�
			int slot_x_num, slot_y_num;
			slot_x_num = (x - m_slotstart_x ) / SLOT_WIDTH;		// ���� Ŭ�� ��ġ
			slot_y_num = (y - m_slotstart_y ) / SLOT_HEIGHT;		// ���� Ŭ�� ��ġ

			// ���õ� �������� �ε��� ����
			int index = m_pNkCha->GetSlotIndex(slot_x_num, slot_y_num);
			if( index < 0 )
				return 1;

			int nVnum = 0;
			int nPlus = 0;
			int nSpecial = 0;
			int nSpecial2 = 0; // �ɼ�2
			int nNowEdr = 0; // ���� ������
			int nNowEdr2 = 0; // �ִ� ������

			char tmp[ 10 ] = {0,};
			char str[ 200 ] = {0,};

			// ���� �κ����� ���� ������ ����
			// ���� ���㸮��Ʈ�� �������ִ�
			// Item Ŭ������ m_Next�� ���� ������ ����
			CItem *pItem = m_pNkCha->m_Inven[m_pNkCha->m_CurPackNum];
			CItem *pTmpItem = pItem;

			// ������ ������ ���ؽ������� ���� ������ �˻�
			while( pItem )
			{
				if( pItem->m_Index == index )
				{
					int nWidth = GET_ITEM_WIDTH_S( pItem );
					int nHeight = GET_ITEM_HEIGHT_S( pItem );

					// 1ĭ¥�� �����۸� �ٷ� �̵� ����
					if( nWidth != 1 || nHeight != 1 )
						return 0;

					nVnum = pItem->m_Vnum;				// ������ ��ȣ
					nPlus = pItem->m_PlusNum;			// ������ �÷�����
					nSpecial = pItem->m_Special;		// ������ ����Ȱ�
					nSpecial2 = pItem->m_Special2;		// ������ ����Ȱ�
					nNowEdr = pItem->m_UpgradeEndurance;		// ������ ����Ȱ�
					nNowEdr2 = pItem->m_MaxUpgradeEndurance;		// ������ ����Ȱ�

					break;		// ã������ �������;�¡.....!
				}
				pItem = pItem->m_Next;		// ���� �������� üũ
			}

			if( g_pNk2DFrame->IsGambleVisible() && g_pNk2DFrame->GetGamble()->m_bType == 0 )
			{
				while( pTmpItem && count < 5 )
				{
					if( !pTmpItem->m_bTimeLimit ) //�ð��� �������� ���� �ȵ���.. ///�ð���������
					{
						// Wnum, PlusNum, Special ��� ��ġ�ؾ���
						if( pTmpItem->m_Vnum == nVnum
								&& pTmpItem->m_PlusNum == nPlus
								&& pTmpItem->m_Special == nSpecial
								&& pTmpItem->m_Special2 == nSpecial2
								&& pTmpItem->m_UpgradeEndurance == nNowEdr
								&& pTmpItem->m_MaxUpgradeEndurance == nNowEdr2
						  )
						{
							sprintf( tmp, "%d %d ", pTmpItem->m_SlotX, pTmpItem->m_SlotY );
							strcat( str, tmp );		// ���� �������� ���̾� �ٿ�����
							count++;
						}
					}
					pTmpItem = pTmpItem->m_Next;
				}
			}
			else
			{


				// ���õ� �����۰� ���� ������ ����
				while( pTmpItem && count < 10 ) // ���� �̵��� 10��������... ///�����ǸŽ� �۾�
				{
					if( !pTmpItem->m_bTimeLimit ) //�ð��� �������� ���� �ȵ���.. ///�ð���������
					{
						// Wnum, PlusNum, Special ��� ��ġ�ؾ���
						if( pTmpItem->m_Vnum == nVnum
								&& pTmpItem->m_PlusNum == nPlus
								&& pTmpItem->m_Special == nSpecial
								&& pTmpItem->m_Special2 == nSpecial2
								&& pTmpItem->m_UpgradeEndurance == nNowEdr
								&& pTmpItem->m_MaxUpgradeEndurance == nNowEdr2
						  )
						{
							sprintf( tmp, "%d %d ", pTmpItem->m_SlotX, pTmpItem->m_SlotY );
							strcat( str, tmp );		// ���� �������� ���̾� �ٿ�����
							count++;	// [theodoric] ������ ���Ƽ� �߰�
						}
					}

					pTmpItem = pTmpItem->m_Next;
				}
			}

			// Ŭ���� ����
			if ( x >= m_slotstart_x
					&& x < m_slotstart_x + ( SLOT_WIDTH * SLOT_X_NUM )
					&& y >= m_slotstart_y
					&& y < m_slotstart_y + ( SLOT_HEIGHT * SLOT_Y_NUM ) )
			{
				// �۾����� ������ Ÿ�� 1.5��
				// �ּ����� ������ġ
				if( m_bInvenLock )
				{
					DWORD curTime = timeGetTime();

					if (curTime > m_timeInvenLock + 1500) // 1.5�ʰ� ��������
						m_bInvenLock = FALSE;
					else
						return 1;
				}

				// �����̰� �������� �÷��� ����
				SetInvenLock( TRUE );

				//===========================================================
				// ante 2004 - 4
				// exchange
				if( g_pNk2DFrame->GetControlWareHouse()
						&& g_pNk2DFrame->GetControlWareHouse()->IsActivate() )
				{
					if( pItem->m_bTimeLimit ) // �ð��� �������̸� â�� �Ұ�ó�� (by ����) ///�ð���������
					{
						//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

						return 1;
					}

					if( GET_ATT_FLAG( pItem ) & IATT_NO_STASH )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_STASH_WITH_NOSTASH), TYPE_NOR_OK, 1 );
						return 1;
					}

					//  [11/26/2008 parkmj] ����â��� ����Ŭ������ ������ �ִ� �κ� �߰�
					// �޼��� ����
					CControlWareHouse* pControlWareHouse = g_pNk2DFrame->GetControlWareHouse();
					BOOL bBySummonNpc = pControlWareHouse->IsFromSummonNpc();
					if( g_pNk2DFrame->GetControlWareHouse()->IsCashStoreUsing() ) // Ȯ�� ���� ó��. ///071003 �߰�
					{
						sprintf( m_MsgBuf, "stash_put %d %d %d %d %s\n", bBySummonNpc, nVnum, m_pNkCha->m_CurPackNum,
								 g_pNk2DFrame->GetControlWareHouse()->GetCurPackNum()+3, str );
					}
					else
					{
						sprintf( m_MsgBuf, "stash_put %d %d %d %d %s\n", bBySummonNpc, nVnum, m_pNkCha->m_CurPackNum,
								 g_pNk2DFrame->GetControlWareHouse()->GetCurPackNum(), str );
					}

					if( CommonConfig::Instance()->GetStashPopup() == true )
					{					
						m_strStashPacketTemp = m_MsgBuf;
						g_pNk2DFrame->InsertPopup(" Click OK if you confirm to keep out item in warehouse ", TYPE_NOR_OKCANCLE, POPUP_STASH_FROM_INVEN_PROC);
						return 1;
					}
					else
					{
						g_pNk2DFrame->GetControlWareHouse()->m_bMsgWaiting = TRUE;
					}
				}
				else if( g_pNk2DFrame->GetControlGStorehouse()
						 && g_pNk2DFrame->GetControlGStorehouse()->IsActivate())
				{
					if( pItem->m_bTimeLimit ) // �ð��� �������̸� â�� �Ұ�ó�� (by ����) ///�ð���������
					{
						//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );

						return 1;
					}

					if( GET_ATT_FLAG( pItem ) & IATT_GUILD_NO_STASH )
					{
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_GSTASH_WITH_NOGSTASH), TYPE_NOR_OK, 1 );
						return 1;
					}

					// �޼��� ����
					sprintf( m_MsgBuf, "mg stash p %d %d %d %s\n", nVnum, m_pNkCha->m_CurPackNum,
							 g_pNk2DFrame->GetControlGStorehouse()->GetCurPackNum(), str );

					g_pNk2DFrame->GetControlGStorehouse()->m_bMsgWaiting = TRUE;
				}
				else if( g_pNk2DFrame->IsExchangeVisible() || g_pNk2DFrame->IsUpgradeVisible()
						 || ( g_pNk2DFrame->IsGambleVisible() && g_pNk2DFrame->GetGamble()->m_bType == 0 ) // ��ȭ�������̽� �󿡼��� �����̵� �����ϰ� ó�� 05-11-24 ����
						 || g_pNk2DFrame->IsEventAncientVisible() )
				{
					// �޼��� ����
					sprintf( m_MsgBuf, "exch_put %d %s\n", m_pNkCha->m_CurPackNum, str );
				}
				else if( g_pNk2DFrame->IsShopVisible() ) // ���� â�� �������� ���� 10������ �ȱ�. ///�����Ǹ�
				{
					if( !pItem )
						return 1;

					if( CommonConfig::Instance()->GetUsingLP() == true )
					{
						if(pItem->m_bTimeLimit )
						{
							pCMyApp->m_pUIMgr->CmdErrorMsg( "Part-time items cannot be sold." );
							return 1;
						}

						if( GET_ATT_FLAG(pItem) & IATT_NO_SELL )
						{
							pCMyApp->m_pUIMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_CANT_SELL) );
							return 1;
						}
					}
					
					// ��Ŷ ��Ʈ���� �����д�.
					ZeroMemory( m_MultiSaleString, sizeof(m_MultiSaleString) ); // Ȥ�� �𸣴� �̰͵� �ʱ�ȭ.
					sprintf( m_MultiSaleString, "m_sell %d %d %d %s\n", g_pNk2DFrame->GetControlShop()->m_KeeperIndex, m_pNkCha->m_CurPackNum, count, str );

					if( g_pNk2DFrame->GetControlShop()->CheckSellWarning(pItem,POPUP_MULTI_SALE_WARRING) ) // ��� ������ϴ� ���������� üũ
						return 1; // ��� ����� ���...

					// �޼��� ����
					char strTemp[256] = "";
					std::string str = boost::str(boost::format(G_STRING(IDS_MULTI_SALE)) % GET_ITEM_NAME_VNUM(pItem->m_Vnum) % count);
					sprintf(strTemp, "%s", str.c_str());

					g_pNk2DFrame->InsertPopup( strTemp,TYPE_NOR_OKCANCLE, POPUP_MULTI_SALE );


					return 1; // �ؿ� ���� �޼��� ���ϵ���...
				}


				//===========================================================
				SendMsg();		// ������ �޼��� ����
			}
		}
		break;
	}

	return 0;
}

void CControlInven::SetNkCha(CNkCharacter *pNkCha)
{
	m_pNkCha = pNkCha;
}

void CControlInven::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}

void CControlInven::SetInvenLock(BOOL bLock)
{
	if (m_bInvenLock)
		m_timeInvenLock += 1500;
	else
	{
		m_bInvenLock = TRUE;
		m_timeInvenLock = timeGetTime();
	}
}

void CControlInven::Load_Masuk()
{
	if( !g_pDisplay )
		return;
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack_Masuk, "interface/masuk/back.bmp");
	if( m_pBack_Masuk )
	{
		m_pBack_Masuk->SetColorKey(RGB(0, 0, 0));
		m_cx = m_pBack_Masuk->GetWidth();
		m_cy = m_pBack_Masuk->GetHeight();
	}
	/*0 �ּҵ����� ���
	1 �ִ뵥���� ���
	2 ����Ÿ20%  ���
	3 ���ⵥ����% ���
	4 ���ⵥ���� ���
	5 ���������� ���
	6 ġ��Ÿ50% ���
	7 ����������%���
	8 �������� ���
	9 ���
	10 ȸ���� ����.
	11 ??
	12 ���鸮��Ʈ(%) ���
	13 ���ⵥ���� ���
	*/
	char tmp[100];
	for(int i=0; i<NUM_BTN_MASUK; i++)
	{
		int nIndex = i;
		if (13 == nIndex) nIndex = 4;

		sprintf(tmp, "masuk/btn%d", nIndex);
		m_Btn_Masuk[i].SetFileName(tmp);
		m_Btn_Masuk[i].LoadRes();
	}
}

void CControlInven::Draw_Masuk()
{
	if( !g_pDisplay )
		return;
	RenderOj();
	g_pDisplay->Blt(m_x, m_y, m_pBack_Masuk);

	for(int i=0; i<NUM_BTN_MASUK; i++)
	{
		if(m_bShowBtnAry[i])
		{
			m_Btn_Masuk[i].Draw();
		}
	}
}


//----------------------------------------------------------------------------
//-- Divergence
//----------------------------------------------------------------------------
//-- ���� ���׷��̵��� ����.
void CControlInven::Divergence(int where, int vnum, int level, int x, int y)
{
	int x_gap = 5;
	int y_gap = 7;

	x = x+x_gap;
	y = y+y_gap;

	int div_num;

	for(int i=0; i<NUM_BTN_MASUK; i++)
		m_bShowBtnAry[i] = false;

	// ���� ������ ���� ������ �˾ƺ���
	for( int i=0; i<12; i++)
	{
		// 3600 �������� �Ǵ� ����.
		if(level > i*30 && level <= (i+1)*30)
			m_iCurItemLevel = i;
	}

	if( level >= 360 )
		m_iCurItemLevel = 11;


	if( m_pNkCha && m_pNkCha->m_Wearing[where] && CheckIsConversionItem(m_pNkCha->m_Wearing[where]) ) // �������Ǿ� ���� �������̸�...
	{
		int temp_vnum = m_pNkCha->m_Wearing[where]->m_Vnum;
		if( temp_vnum == 1313 || temp_vnum == 1314
				|| temp_vnum == 1315 || temp_vnum == 1316
				|| temp_vnum == 1317 || temp_vnum == 1322
				|| temp_vnum == 1324
		  ) // �Ϻ� ������ (��-���̾� �޾�����)�� ��󸸴� ������ 1�ܰ������ �ٸ����ֵ���.
		{
			// 1�ܰ� ������ �ٸ����ְ�. �� ���Ĵ� ���ٸ���...
			if( GET_TYPE(m_pNkCha->m_Wearing[where]) == ITYPE_WEAPON   // ���� ��󸸴� ������ ���⿡�� �ٸ����ִ�.
					&& (m_pNkCha->m_Wearing[where]->m_Special & g_WeaponSpecialMask[SPECIAL_IGNORE_AVOID]) == 0 ) // �ȹ߶�����������...
				level = 300; // �� ������ �����ϸ� ��󸸴� �ٸ����ִ�.
			else
				level = 260; // �� ������ �����ϸ� ��󸸴� ���ٸ���.
		}
		else // �� ���� ������ �����
		{
			level = 300; // �� ������ �����ϸ� ��󸸴� �ٸ����ִ�.
		}
	}
	else if ( m_pNkCha && m_pNkCha->m_Wearing[where] && CheckIsConversionItem(m_pNkCha->m_Wearing[where], 1) )
	{
	}

	m_bDam_min = m_bDam_max = m_bDam_all = m_bDam_per = m_bHit_1 = m_bHit_2 = m_bDef_melee = m_bResist = m_bDef_all = m_bDef_per = mb_ignore_avo = false;
	m_bDam_all2 = m_bDeadHit = false;

	// ���ݺκ� �����ϰ��
	if(where == 9)
	{
		m_bDam_min	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DAM_MIN);		// �ּҵ�����
		m_bDam_max	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DAM_MAX);		// �ִ뵥����
		m_bDam_all	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DAM_ALL);		// ���ⵥ���� ���
		m_bDam_per	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DAM_PER);		// ���ⵥ���� % ���
		m_bHit_1	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_HIT_1);			// ����Ÿ 20% ���
		m_bHit_2	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_HIT_2);			// ġ��Ÿ 50% ���
		mb_ignore_avo = IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_IGNORE_AVOID); // ��� ȸ���� ����
		m_bDam_all2	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DAMAGE_UP2);		// ���ⵥ���� ���
		m_bDeadHit	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DEAD_HIT);		// ���鸮��Ʈ ���

		if(vnum == 551)
		{
			// �� ���� 0, 1, 2
			div_num = (m_cy / 4);
			if(m_iCurItemLevel == LEVEL_MAX_JASU)
			{
				m_Btn_Masuk[0].SetPosition(x, y-3);
				m_bShowBtnAry[0] = true;
			}
			else if(m_iCurItemLevel == LEVEL_MAX_TOPHA)
			{
				m_Btn_Masuk[0].SetPosition(x, y-3);
				m_Btn_Masuk[1].SetPosition(x, y+(div_num*1)-3);
				m_bShowBtnAry[0] = true;
				m_bShowBtnAry[1] = true;
			}
			else if(m_iCurItemLevel >= LEVEL_MAX_OPAL)
			{
				m_Btn_Masuk[0].SetPosition(x, y-3);
				m_Btn_Masuk[1].SetPosition(x, y+(div_num*1)-3);
				m_Btn_Masuk[2].SetPosition(x, y+(div_num*2)-3);
				m_bShowBtnAry[0] = true;
				m_bShowBtnAry[1] = true;
				m_bShowBtnAry[2] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*3)-3);
			m_bShowBtnAry[10] = true;
		}
		else if(vnum == 552)
		{
			// ���� ���� 4, 6
			div_num = (m_cy / 3)-2;
			if(m_iCurItemLevel == LEVEL_MAX_SAPHA)
			{
				m_Btn_Masuk[4].SetPosition(x, y);
				m_bShowBtnAry[4] = true;
			}
			else if(m_iCurItemLevel >= LEVEL_MAX_RUBI)
			{
				m_Btn_Masuk[4].SetPosition(x, y);
				m_Btn_Masuk[6].SetPosition(x, y+(div_num*1));
				m_bShowBtnAry[4] = true;
				m_bShowBtnAry[6] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
		else if(vnum == 553)
		{
			// �Ƿ��� ���� 3
			div_num = (m_cy / 3)-2;
			if(m_iCurItemLevel >= LEVEL_MAX_EME)
			{
				m_Btn_Masuk[3].SetPosition(x, y);
				m_bShowBtnAry[3] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
		//-- ��󸸴� ����
		else if (vnum == 554)
		{
			div_num = (m_cy / 3)-2;
			if(level >= 261)
			{
				m_Btn_Masuk[11].SetPosition(x, y);
				m_bShowBtnAry[11] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
		// ���κ� �����ϰ��
	}
	else if(where == 0 || where == 1 || where == 2 || where == 3 || where == 4 || where == 5)
	{
		m_bDef_melee= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DEFENSE_MELEE);	// ���������� ���
		m_bResist	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_RESIST_MAGIC);	// �������� ���
		m_bDef_all	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DEFENSE_ALL);    // ��ü���� ���
		m_bDef_per	= IsOptionFull(m_pNkCha->m_Wearing[where], SPECIAL_DEFENSE_PER);	// ��ü���� % ���

		if(vnum == 551)
		{
			// �� ���� 5, 8
			div_num = (m_cy/3)-2;
			if(m_iCurItemLevel == LEVEL_MAX_TOPHA)
			{
				m_Btn_Masuk[5].SetPosition(x, y);
				m_bShowBtnAry[5] = true;
			}
			else if(m_iCurItemLevel >= LEVEL_MAX_OPAL)
			{
				m_Btn_Masuk[5].SetPosition(x, y);
				m_Btn_Masuk[8].SetPosition(x, y+(div_num*1));
				m_bShowBtnAry[5] = true;
				m_bShowBtnAry[8] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
		else if(vnum == 552)
		{
			// ���� ����
			div_num = (m_cy/3)-2;
			if(m_iCurItemLevel >= LEVEL_MAX_SAPHA)
			{
				m_Btn_Masuk[7].SetPosition(x, y);
				m_bShowBtnAry[7] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
		else if(vnum == 553)
		{
			// �Ƿ��� ����
			div_num = (m_cy/3)-2;
			if(m_iCurItemLevel >= LEVEL_MAX_EME)
			{
				m_Btn_Masuk[9].SetPosition(x, y);
				m_bShowBtnAry[9] = true;
			}
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
		else if(vnum == 554)
		{
			// ��󸸴� ����
			div_num = (m_cy/3)-2;
			m_Btn_Masuk[10].SetPosition(x, y+(div_num*2));
			m_bShowBtnAry[10] = true;
		}
	}

	if(m_bDam_min)
		m_Btn_Masuk[0].m_iBtnFlg = BTN_DISABLE;
	if(m_bDam_max)
		m_Btn_Masuk[1].m_iBtnFlg = BTN_DISABLE;
	if(m_bDam_all)
		m_Btn_Masuk[4].m_iBtnFlg = BTN_DISABLE;
	if(m_bDam_per)
		m_Btn_Masuk[3].m_iBtnFlg = BTN_DISABLE;
	if(m_bHit_1)
		m_Btn_Masuk[2].m_iBtnFlg = BTN_DISABLE;
	if(m_bHit_2)
		m_Btn_Masuk[6].m_iBtnFlg = BTN_DISABLE;
	if(m_bDef_melee)
		m_Btn_Masuk[5].m_iBtnFlg = BTN_DISABLE;
	if(m_bResist)
		m_Btn_Masuk[8].m_iBtnFlg = BTN_DISABLE;
	if(m_bDef_all)
		m_Btn_Masuk[7].m_iBtnFlg = BTN_DISABLE;
	if(m_bDef_per)
		m_Btn_Masuk[9].m_iBtnFlg = BTN_DISABLE;
	if(mb_ignore_avo)
		m_Btn_Masuk[11].m_iBtnFlg = BTN_DISABLE;
	if(m_bDeadHit)
		m_Btn_Masuk[12].m_iBtnFlg = BTN_DISABLE;
	if(m_bDam_all2)
		m_Btn_Masuk[13].m_iBtnFlg = BTN_DISABLE;
}

void CControlInven::Close_Masuk()
{
	m_bAct_Masuk = FALSE;

	m_iCurItemLevel = -1;
	for(int i=0; i<NUM_BTN_MASUK; i++)
	{
		m_bShowBtnAry[i] = false;
		m_Btn_Masuk[i].m_iBtnFlg = BTN_NORMAL;
	}
	m_bDam_min = m_bDam_max = m_bDam_all = m_bDam_per = m_bHit_1 = m_bHit_2 = m_bDef_melee = m_bResist = m_bDef_all = m_bDef_per = mb_ignore_avo = false;;
}

void CControlInven::Del_Masuk()
{
	SAFE_SURFACE_DESTROY(m_pBack_Masuk);
	for(int i=0; i<NUM_BTN_MASUK; i++)
		m_Btn_Masuk[i].DeleteRes();
}

void CControlInven::Action_Masuk(int type)
{
	if( !m_pNkCha )
		return;

	switch(type)
	{
	case 0:// �ּҵ����� ��� (��)
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 1:// �ִ뵥���� ��� (��)
		sprintf(m_MsgBuf, "ms_upgrade %d 1\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 2:// ����Ÿ20%��� (��)
		sprintf(m_MsgBuf, "ms_upgrade %d 2\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 3:// ���ⵥ���� %��� (�Ƿ���)
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 4:// ���ⵥ���� ��� (����)
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 5:// ���������»�� (��)
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 6:// ġ��Ÿ50%���
		sprintf(m_MsgBuf, "ms_upgrade %d 1\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 7:// ��ü ���� ��� (����)
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 8:// �������� ��� (��)
		sprintf(m_MsgBuf, "ms_upgrade %d 1\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 9:// ��ü����%��� (�Ƿ���)
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;

	case 10:// ��ҹ�ư
		break;

	case 11:// ��� ȸ���� ����.
		sprintf(m_MsgBuf, "ms_upgrade %d 0\n", m_iCurWhere);
		SendMsg();
		SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		break;


	}
}



void CControlInven::CalcPos_Masuk(int x, int y)
{
	if (x + (m_cx / 2)  >= m_ClientRc.right)
		m_x = m_ClientRc.right - m_cx ;
	else
		m_x = x - m_cx / 2;

	if (y + m_cy  >= m_ClientRc.bottom)
		m_y = m_ClientRc.bottom - m_cy ;
	else if (y - m_cy  < 0)
		m_y = y;
	else
		m_y = y - m_cy;

	if (m_x < 0)
		m_x = 0;
	if (m_y < 0)
		m_y = 0;
	if (m_y < 29)
		m_y = 29;
}


BOOL CControlInven::Drop_EnergyBall(bool bAct, int vnum, int idx)
{
	m_iMobControl_vnum = vnum;
	m_iMobControl_idx = idx;
	m_bDropEngBall = bAct;
	return TRUE;
}


void CControlInven::RenderOj()
{
	if( !pCMyApp->GetDevice() )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;

	pCMyApp->GetDevice()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	pCMyApp->GetDevice()->SetViewport(&vpNew);

	//// �������� ��Ʈ���� ����.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	TLVec[0] = D3DTLVERTEX(D3DVECTOR((float)m_x,				(float)m_y,		  0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR((float)m_x+(float)m_cx,	(float)m_y,		  0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR((float)m_x,				(float)m_y+m_cy,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR((float)m_x+(float)m_cx,	(float)m_y+m_cy,  0), 1, 0x00ffffff, 0, 1, 1);

	// ������
	pCMyApp->GetDevice()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	if(SUCCEEDED( pCMyApp->GetDevice()->BeginScene() ) )
	{
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		if( g_pCapsyongTexture )
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_BLACK]);
		pCMyApp->GetDevice()->SetTexture(0, lpTexture);

		pCMyApp->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
		pCMyApp->GetDevice()->EndScene();
	}

	// ���󺹱�.
	pCMyApp->GetDevice()->SetViewport(&vpOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}

void CControlInven::RenderOj( RECT Rect, DWORD color ) ///�̺�Ʈ�������� �� �߰�.
{
	if( !pCMyApp->GetDevice() )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;

	pCMyApp->GetDevice()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ClientRc.right;
	vpNew.dwHeight = m_ClientRc.bottom;
	pCMyApp->GetDevice()->SetViewport(&vpNew);

	//// �������� ��Ʈ���� ����.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	pCMyApp->GetDevice()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);



	TLVec[0] = D3DTLVERTEX(D3DVECTOR((float)Rect.left,				(float)Rect.top,		  0), 1, color, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR((float)Rect.left+(float)Rect.right,	(float)Rect.top,		  0), 1, color, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR((float)Rect.left,				(float)Rect.top+Rect.bottom,  0), 1, color, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR((float)Rect.left+(float)Rect.right,	(float)Rect.top+Rect.bottom,  0), 1, color, 0, 1, 1);


	// ������
	pCMyApp->GetDevice()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	if(SUCCEEDED( pCMyApp->GetDevice()->BeginScene() ) )
	{
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		if( g_pCapsyongTexture )
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_TEXTR_BLACK]);
		pCMyApp->GetDevice()->SetTexture(0, lpTexture);

		pCMyApp->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
		pCMyApp->GetDevice()->EndScene();
	}

	// ���󺹱�.
	pCMyApp->GetDevice()->SetViewport(&vpOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	pCMyApp->GetDevice()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	pCMyApp->GetDevice()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}

void CControlInven::SetPickExtraTime()
{
	m_timePickExtraTime = timeGetTime();
}

HRESULT CControlInven::WarningPopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	// �����̳� ���׷��̵� �������� �ٴڿ� ����߸��� �Ҷ�..�޼���
	if(nMsg == 1)
	{
		if( m_byPopupType == 5 ) // ����� �ռ� ���. ///��
		{
			if(!pCMyApp->m_bNecklaceLock)
			{
				sprintf(m_MsgBuf, "upnk 1\n");
				SendMsg();
				if( m_pNkCha )
					SAFE_DELETE(m_pNkCha->m_ExtraSlot);
				pCMyApp->m_bNecklaceLock = TRUE;
				for(int i = 0 ; i <= 5 ; i ++)
					if( m_pNkCha )
						m_pNkCha->m_bQuickSlotLock[i] = TRUE;
			}
		}
		else if(m_byPopupType == 4)
		{
			if(m_iCurWhere<0)
				return 1;

			if (m_pNkCha
					&& (m_iCurWhere = m_pNkCha->Upgrade(m_iCurWhere)) >= 0)
			{
				// ���⿡ ��� ��ġ���ٰ� ������ ������������ ������.
				// �������� ��쿡�� m_EquipSlot[where] �� �ȴ�.
				//m_pNkCha->m_Wearing[m_iCurWhere]->m_PlusNum;
				//m_pNkCha->m_Wearing[m_iCurWhere]->m_GemNum;
				sprintf(m_MsgBuf, "upgrade %d\n", m_iCurWhere);
				SendMsg();
				// ����� ��ٸ��� �ʰ� �����.. ����.
				SAFE_DELETE(m_pNkCha->m_ExtraSlot);
				m_iCurWhere = -1;
			}
		}
		else if(m_byPopupType == 3) // ������� ���� ���׷��̵� ��.. //�������۰��
		{
			if(!pCMyApp->m_bBrLock)
			{
				sprintf(m_MsgBuf, "upbr 0\n");
				SendMsg();
				if( m_pNkCha )
					SAFE_DELETE(m_pNkCha->m_ExtraSlot);
				pCMyApp->m_bBrLock = TRUE;

				for( int i = 0 ; i <= 5 ; i++ )
					if( m_pNkCha )
						m_pNkCha->m_bQuickSlotLock[i] = TRUE;
			}
		}
		else if(m_byPopupType == 2)
		{
			if(!pCMyApp->m_bBrLock)
			{
				sprintf(m_MsgBuf, "upbr 1\n");
				SendMsg();
				if( m_pNkCha )
					SAFE_DELETE(m_pNkCha->m_ExtraSlot);
				pCMyApp->m_bBrLock = TRUE;

				for( int i = 0 ; i <= 5 ; i++ )
					if( m_pNkCha )
						m_pNkCha->m_bQuickSlotLock[i] = TRUE;
			}
		}
		else if(m_byPopupType == 1)
		{
			if(!pCMyApp->m_bRingLock)
			{
				sprintf(m_MsgBuf, "rg 1\n");
				SendMsg();
				if( m_pNkCha )
					SAFE_DELETE(m_pNkCha->m_ExtraSlot);
				pCMyApp->m_bRingLock = TRUE;
				for(int i = 0 ; i <= 5 ; i ++)
					if( m_pNkCha )
						m_pNkCha->m_bQuickSlotLock[i] = TRUE;
			}
		}
		else if (m_pNkCha
				 && m_pNkCha->m_ExtraSlot
				 && !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_NO_DROP)
				 && !(m_pNkCha->m_ExtraSlot->m_PriceType == 2) // ĳ�� ������ �Ұ�ó�� �߰�. ///�������
				)
		{
			sprintf(m_MsgBuf, "drop 1\n");
			SendMsg();
			SAFE_DELETE(m_pNkCha->m_ExtraSlot);
		}

	}

	m_iCurWhere = -1;
	m_byPopupType = 0;
	pCMyApp->m_bInverseReturn = FALSE;

	return 0L;
}

LRESULT CControlInven::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1)	// Ȯ�ι�ư�� �������
	{
		strcpy(m_MsgBuf, m_strStashPacketTemp.c_str());
		SendMsg();
		g_pNk2DFrame->GetControlWareHouse()->m_bMsgWaiting = TRUE;
	}
	else if(nMsg == -1) // ��ҹ�ư�� �������
	{
		m_strStashPacketTemp.clear();
	}

	return 0L;
}

int CControlInven::GetEnduranceCharge(CItem* pItem)
{
	// [2008/3/19 Theodoric] ������ ������ ��� �Ź���
	int		nItemRepairLevelGroup;
	int		nCharge;				// by evilkiki 2008.07.23 warning remove
	double	aItemLevelGroupPlusRatio[6] = {0.0, 0.25, 0.45, 0.65, 0.85, 1.0}; //������ ���� �׷캰 ������� �÷��� ����

	double	nDurPercent = (double)pItem->m_Endurance * 100.0 / (double)pItem->m_MaxEndurance;
	int		nPercent = ((double)nDurPercent > (double)99.0) ? 0 : 100 - (int)nDurPercent;

	//������ �������� �׷� ����
	if (	 pItem->m_MinLevel >= 0 && pItem->m_MinLevel <= 100 )
		nItemRepairLevelGroup = 0;
	else if (pItem->m_MinLevel >= 101 && pItem->m_MinLevel <= 150 )
		nItemRepairLevelGroup = 1;
	else if (pItem->m_MinLevel >= 151 && pItem->m_MinLevel <= 200 )
		nItemRepairLevelGroup = 2;
	else if (pItem->m_MinLevel >= 201 && pItem->m_MinLevel <= 250 )
		nItemRepairLevelGroup = 3;
	else if (pItem->m_MinLevel >= 251 && pItem->m_MinLevel <= 300 )
		nItemRepairLevelGroup = 4;
	else //pItem->m_MinLevel >= 301
		nItemRepairLevelGroup = 5;

	//������ ���
	if (nPercent == 0)
	{
		if( pItem->m_Endurance < pItem->m_MaxEndurance )
			nCharge = 1;
		else
			nCharge = 0;
	}
	else if (nPercent >= 1 && nPercent <= 4 )
	{
		nCharge = (int)((double)(GET_PRICE(pItem) * ( (double)(1.0 + aItemLevelGroupPlusRatio[nItemRepairLevelGroup]) / 100.0 )));
	}
	else if (nPercent >= 5 && nPercent <= 9 )
	{
		nCharge = (int)((double)(GET_PRICE(pItem) * ( (double)(1.25 + aItemLevelGroupPlusRatio[nItemRepairLevelGroup]) / 100.0 )));
	}
	else if (nPercent >= 10 && nPercent <= 14 )
	{
		nCharge = (int)((double)(GET_PRICE(pItem) * ( (double)(1.5 + aItemLevelGroupPlusRatio[nItemRepairLevelGroup]) / 100.0 )));
	}
	else if (nPercent >= 15 && nPercent <= 19 )
	{
		nCharge = (int)((double)(GET_PRICE(pItem) * ( (double)(1.75 + aItemLevelGroupPlusRatio[nItemRepairLevelGroup]) / 100.0 )));
	}
	else // 20% �̻�
	{
		nCharge = (int)((double)(GET_PRICE(pItem) * ( (double)(2.0 + (double)(nPercent-20)*0.25 + aItemLevelGroupPlusRatio[nItemRepairLevelGroup]) / 100.0 )));
	}

	return nCharge;
}


BOOL CControlInven::IsInsideEventWearFlag(int x, int y)
{
	if( !g_bUseEventWear )
		return FALSE;

	if( !m_pSurEventWearFlagON )
		return FALSE;

	int x1 = m_pSurEventWearFlagON->Xpos;
	int x2 = x1 + m_pSurEventWearFlagON->GetWidth();
	int y1 = m_pSurEventWearFlagON->Ypos;
	int y2 = y1 + m_pSurEventWearFlagON->GetHeight();

	if( x1< x && x2 > x && y1 < y && y2 > y )
		return TRUE;

	return FALSE;
}

void CControlInven::ToggleEventWearFlag()
{
	g_pRoh->m_bEventWearFlag = !g_pRoh->m_bEventWearFlag;



	switch(g_pRoh->m_bEventWearFlag)
	{
	case TRUE:
		sprintf( m_MsgBuf, "ev_wearflag %d 1\n", g_pRoh->m_nUserIndex);
		break;
	case FALSE:
		sprintf( m_MsgBuf, "ev_wearflag %d 0\n", g_pRoh->m_nUserIndex);
		break;
	}

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}



void CControlInven::SetItemInfo_Tribe(CItem * pItem , CMultiText &multiText)
{
	long race_flag = GET_ITEM_RACE(pItem);


	switch (GET_ITEM_RACE(pItem))
	{
	case 0x0003:
		multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_BULKAN), RGB(255, 150, 150));
		break;
	case 0x000c:
		multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_KAI), RGB(255, 150, 150));
		break;
	case 0x0030:
		multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_AID), RGB(255, 150, 150));
		break;
	case 0x00c0:
		multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HUMAN), RGB(255, 150, 150));
		break;
	case 0x0300:
		multiText.AddString((char*)G_STRING(IDS_INVEN_ONLY_HIBRIDER), RGB(255, 150, 150));
		break;
	case 0x0C00:
		multiText.AddString((char*)G_STRING(IDS_LHSTRING1942), RGB(255, 150, 150));
		break;
	}
}

void CControlInven::SetSpecial_SkillDoc(CItem * pItem , CMultiText &multiText)
{
	if( !pItem )
		return;

	switch( pItem->m_Vnum )
	{
	case 1403:
		multiText.AddString((char*)G_STRING(IDS_LHSTRING1858) , _FCOLOR_WHITE);
		break;
	case 1404:
		multiText.AddString((char*)G_STRING(IDS_LHSTRING1859) , _FCOLOR_WHITE);
		break;
	case 1405:
		multiText.AddString((char*)G_STRING(IDS_LHSTRING1860) , _FCOLOR_WHITE);
		break;
	case 1406:
		multiText.AddString((char*)G_STRING(IDS_LHSTRING1861) , _FCOLOR_WHITE);
		break;
	case 1407:
		multiText.AddString((char*)G_STRING(IDS_LHSTRING1862) , _FCOLOR_WHITE);
		break;
	case 3837:
		multiText.AddString((char*)G_STRING(IDS_SKILLBOOK_26) , _FCOLOR_WHITE);		
		break;
	case 3838:
		multiText.AddString((char*)G_STRING(IDS_SKILLBOOK_27) , _FCOLOR_WHITE);		
		break;
	case 3839:
		multiText.AddString((char*)G_STRING(IDS_SKILLBOOK_28) , _FCOLOR_WHITE);		
		break;
	case 3840:
		multiText.AddString((char*)G_STRING(IDS_SKILLBOOK_29) , _FCOLOR_WHITE);		
		break;
	case 3841:
		multiText.AddString((char*)G_STRING(IDS_SKILLBOOK_30) , _FCOLOR_WHITE);
		break;
	}
}

BOOL CControlInven::procEventEnchant(int where)
{
	if( m_pNkCha->m_Wearing[where] == NULL )
		return FALSE;	

	if( m_pNkCha->m_ExtraSlot == NULL )
		return FALSE;

	if( procEventEnchant_2015(where) == TRUE )
		return TRUE;

	if( procEventEnchant_old(where) == TRUE )
		return TRUE;

	return FALSE;
}

BOOL CControlInven::procEventEnchant_old(int where)
{
	if( FALSE == checkEventEnchantWear(where) )
		return FALSE;

	if( m_pNkCha->m_ExtraSlot->m_PlusNum >= EVENT_ENCHANT_MAX_PLUS )
		return TRUE;

	int type = GET_TYPE(m_pNkCha->m_ExtraSlot);
	int shape = GET_SHAPE(m_pNkCha->m_ExtraSlot);

	if( false == (type == ITYPE_ETC && shape == IETC_GEM) )
		return FALSE;

	int jewelVnum = m_pNkCha->m_ExtraSlot->m_Vnum;

	if( 3664 != jewelVnum )
		return TRUE;

	sprintf( m_MsgBuf, "gear_up_event %d\n", where );
	SendMsg();	

	return TRUE;
}

BOOL CControlInven::procEventEnchant_2015(int where)
{
	CommonConfig* commonCfg = CommonConfig::Instance();
		
	int gem_vnum = m_pNkCha->m_ExtraSlot->m_Vnum;
	int wear_vnum = m_pNkCha->m_Wearing[where]->m_Vnum;
	
	int check_gem_vnum = commonCfg->GetGearGemIndex();
	int check_wear_vnum = commonCfg->GetGearItemVnum(where);

	if( check_gem_vnum != gem_vnum )
		return FALSE;

	if( check_wear_vnum != wear_vnum )
		return FALSE;

	SendGearUpgrade(where);

	return TRUE;
}

BOOL CControlInven::checkEventEnchantWear(int where)
{
	return m_pNkCha->m_Wearing[where]->IsEventEnchantItem();
}

HRESULT CControlInven::RestoreSurfaces()
{
	m_MoneyText.SetString(m_pNkCha->m_Money, true);

	m_plus5Message.RestoreAll();

	return S_OK;
}

bool CControlInven::check_disable_normal()
{
	if( m_pNkCha->m_ExtraSlot == NULL )
		return false;

	if( !(GET_ATT_FLAG(m_pNkCha->m_ExtraSlot) & IATT_DISABLE_NORMAL) )
		return false;

	return true;
}