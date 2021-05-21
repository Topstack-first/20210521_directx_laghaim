#include "stdafx.h"



#include <string.h>
#include <TCHAR.h>
#include <direct.h>
#include "main.h"
#include "basicbutton.h"
#include "headers.h"
#include "ItemRender.h"
#include "ItemProto.h"
#include "Nk2DFrame.h"
#include "UIMgr.h"
#include "tcpipcon.h"
#include "textoutbox.h"
#include "controlbottom.h"
#include "controlinven.h"
#include "controlexchange.h"
#include "msgpopup.h"
#include "g_stringmanager.h"
#include "NkCharacter.h"
#include "DirtSnd.h"
#include "Country.h"

#include "Sprite.h"
#include "ControlUpgrade.h"
#include "Mouse.h"


#include "LHItemSystem_Manager.h"
#include "LHParam_MakeItem.h"
#include "LHParam_SeperateItem.h"
#include "LHParam_GMagicStone.h"

#include "LHParam_ConfusionItem.h"
#include "LHParam_EnchantItem.h"



#define IS_2011_ITEM_ARMOR(vnum) ((2295<=vnum && vnum<=2316) || (2942<=vnum && vnum<=2953) || (3152<=vnum && vnum<=3155) || (3227<=vnum && vnum<=3230) ? TRUE : FALSE )
#define IS_2011_ITEM_WEAPON(vnum) (/*(2743<=vnum && vnum<=2749) ||*/ (2939<=vnum && vnum<=2941) || (3151==vnum) /*|| (3231==vnum) */? TRUE : FALSE )

#define IS_G_LEVEL_ITEM(vnum) ( (2173<=vnum && vnum<=2179) || (2743<=vnum && vnum<=2749) )

#ifndef GET_MY_SLOT_X
#define GET_MY_SLOT_X(point_x)		((point_x - m_my_slotstart_x) / SLOT_WIDTH)
#endif

#ifndef GET_MY_SLOT_Y
#define GET_MY_SLOT_Y(point_y)		((point_y - m_my_slotstart_y) / SLOT_HEIGHT)
#endif

extern DWORD				g_dwLangType;

extern int					g_EffectSound[];
extern DWORD				g_dwClientCountry;



extern BOOL CheckCanNpcUpgradeItem( CItem *pItem ); // ��ȭ�� ������ ������������ üũ��.
extern BOOL CheckCanConversionItem( CItem *pItem ); // �������� ������ ������������ üũ��.
extern BOOL CheckIsConversionItem( CItem *pItem, int type = 0 ,int sub_type=0); // �������Ǿ� ������ ������������ üũ.(�������� Ǯ�� ���ȴ�.)
extern BOOL ChechIs261LvWeponItem(CItem *pItem); // 261 �� ����� ���� Ȯ��
extern BOOL CheckIsGodItem(CItem *pItem);


// �ӽ� �ѱ� ��Ʈ���� �ּ� => // han_msg

const __int64 NeedLaim_GodPower = 200000000;


CControlUpgrade::CControlUpgrade()
	: m_hWnd(NULL)
	, m_pNkCha(NULL)
	, m_pItemSystemMgr(NULL)
{
	memset(m_iNeedGemVnum, 0, sizeof(m_iNeedGemVnum));
	memset(m_iNeedGemCount, 0, sizeof(m_iNeedGemCount));
	memset(m_need_gem, 0, sizeof(m_need_gem));

	m_pBackSur = NULL;
	m_pBack_Title = NULL;
	m_pTextInfo = NULL;
	m_pEnableSlot = NULL;
	m_pDisableSlot = NULL;
	m_pExistSlot  = NULL;
	m_pDiaGem = NULL;
	m_pRegentDiaGem = NULL;
	m_pRegOrGrtDiaGem = NULL;
	m_pCapellaGem = NULL;  //Upgrade 850+ weapon

	for( int i = 0 ; i < MAX_UPGRADE_ANI ; ++i )
		m_pAniSound[i] = NULL;




	Init(); // ������ �ʱ�ȭ.


}

CControlUpgrade::~CControlUpgrade()
{
	DeleteRes();

	m_pNkCha = NULL;


}

void CControlUpgrade::Init()
{
	int i = 0;

	m_iType = 0; // ��ȭ���� �������� �÷���. ����� 0�̸� ��ȭ.1�̸� ����.
	m_my_slotstart_x = 0;
	m_my_slotstart_y = 0;

	m_NeedGemText.SetString( _T(" ")/*atoi(m_strMoney1)*/, true);
	m_NeedGemNumText.SetString( _T(" ")/*atoi(m_strMoney1)*/, true);
	m_NeedMoneyText1.SetString( _T(" ")/*atoi(m_strMoney1)*/, true);
	m_NeedMoneyText2.SetString( _T("0")/*atoi(m_strMoney1)*/, true);
	m_HaveMoneyText.SetString( _T("0")/*atoi(m_strMoney1)*/, true);

	m_bSlotLock = FALSE; // ��ü�� ���Կ��� ���� ���� ������Ű�� ���� �÷���.
	m_NowState = UPGRADE_ANI_NON;

	m_dwLastTickTime = 0;
	m_dwStartTickTime = 0;
	m_dwMinAniTime = 0;
	m_dwMaxAniTime = 0;
	m_iAniChangeFrame = 0;

	for( i = 0 ; i < MAX_NEED_ITEM_IN_NPC_UPGRADE ; ++i )
	{
		NeedItem[i] = 0; // ��ȭ �Ǵ� ������ ������ �������� �ʿ��� ����.
	}
	NeedLaim = 0; // �ʿ� ����.

	m_iOldMoney = 0; // ���� �����ݾ�

	m_Result = 0;

	m_iTextInfoIndex = 200; // �������ÿ� �ؽ�Ʈ ������ ��Ȳ�� ���� �̹����� �ٸ��� �ε��ؼ� ����. � �̹����� �ε�Ǿ��ִ����� ���� �̹����� ��ȣ �ε����� ����ϴ� ����.
	m_iOldTextInfoIndex = 0; // �������ÿ� �ؽ�Ʈ ������ ��Ȳ�� ���� �̹����� �ٸ��� �ε��ؼ� ����. � �̹����� �ε�Ǿ��ִ����� ���� �̹����� ��ȣ �ε����� ����ϴ� ����.

	m_pItemRender = NULL;
	m_pOverItem = NULL;
	m_dwMouseClickTickTime = timeGetTime();
}

void CControlUpgrade::DeleteRes()
{
	m_UpgradeBtn.DeleteRes();
	m_BtnCancel.DeleteRes();
	m_NeedGemText.DeleteRes();
	m_NeedGemNumText.DeleteRes();
	m_NeedMoneyText1.DeleteRes();
	m_NeedMoneyText2.DeleteRes();
	m_HaveMoneyText.DeleteRes();

	for( int i = 0 ; i < MAX_UPGRADE_ANI ; ++i )
	{
		m_SpriteAni[i].DeleteRes();
		SAFE_RELEASE( m_pAniSound[i] );
	}


	SAFE_DELETE(m_pBackSur);
	SAFE_DELETE(m_pBack_Title);
	SAFE_DELETE(m_pTextInfo);

	SAFE_DELETE(m_pEnableSlot);
	SAFE_DELETE(m_pDisableSlot);
	SAFE_DELETE(m_pExistSlot);

	SAFE_DELETE(m_pDiaGem);
	SAFE_DELETE(m_pRegentDiaGem);
	SAFE_DELETE(m_pRegOrGrtDiaGem);
	SAFE_DELETE(m_pCapellaGem);
}

void CControlUpgrade::LoadRes( int Type, CItemRender *pItemRender )
{
	if( !g_pDisplay || !pItemRender )
		return;

	int i = 0;

	Init(); // �������� �ѹ� �ʱ�ȭ.

	m_pItemRender = pItemRender;

	m_iType = Type;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect( m_hWnd, &m_ClientRc );

	if( FAILED( g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/npcupgrade/NPC_upgrade.BMP" ) ) )
		return;

	g_pDisplay->CreateSurfaceFromBitmap( &m_pEnableSlot, "interface/slot/slot_enable.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pDisableSlot, "interface/slot/slot_disable.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pExistSlot, "interface/slot/slot_exist.bmp" );

	g_pDisplay->CreateSurfaceFromBitmap( &m_pDiaGem, "interface/NpcUpgrade/NormalDia.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pRegentDiaGem, "interface/NpcUpgrade/RegentDia.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_pRegOrGrtDiaGem, "interface/NpcUpgrade/RegOrGrtDia.bmp" );


	m_BtnCancel.SetFileName( "common/btn_close_01" );	  // ��ҹ�ư

	m_NeedGemText.Init( 12, RGB(255,255,0), 0, 0 );
	m_NeedGemNumText.Init( 12, RGB(255,255,0), 0, 0 );
	m_NeedMoneyText1.Init( 12, RGB(255,255,0), 0, 0 );
	m_NeedMoneyText2.Init( 12, RGB(255,255,0), 0, 0 );
	m_HaveMoneyText.Init( 12, RGB(255,255,0), 0, 0 );


	// ��ȭ�� �����̳Ŀ� ���� �ٸ��� �ε��ϴ� �༮��...
	switch( m_iType )
	{
	case INTERFACE_TYPE_UPARMOR: // �� ��ȭ
	case INTERFACE_TYPE_UPWEAPON: // ���� ��ȭ
	case INTERFACE_TYPE_UPARMOR_400:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/NpcUpgrade/NpcUpgrade_info.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_enhancement" );	  // ��ҹ�ư

			if( g_dwClientCountry == CTRY_KOR ) // �ѱ��� ���� ���߽�Ű�� � XXX ���� �ִϸ��̼� ����� �ߴ�.
			{
				// �ƿ� �ִϸ��̼� �ε带 ���Ѵ�.

				// ��Ÿ ��ġ�� 0���� ����.
				m_dwMinAniTime = 0; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 0; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 0; // 'ing'�� 8�����ӿ��� ��ȯ
			}
			else // �ٸ� ����� �־�����.
			{
				// ��������Ʈ �ִϸ��̼� �ε�
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type0_ing", 13, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type0_success", 25, 110 );
				m_SpriteAni[UPGRADE_ANI_FAIL].LoadRes( "interface/NpcUpgrade/effect/type0_fail", 16, 110 );
				m_SpriteAni[UPGRADE_ANI_BROKEN].LoadRes( "interface/NpcUpgrade/effect/type0_broken", 18, 120 );

				// ��Ÿ �ִϸ��̼ǿ� �ʿ��� ��ġ�� �Է�.
				m_dwMinAniTime = 2000; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 5000; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 8; // 'ing'�� 8�����ӿ��� ��ȯ


				// ȿ���� �ε�.
				for( i = 0 ; i < MAX_UPGRADE_ANI ; ++i )
				{
					SAFE_RELEASE( m_pAniSound[i] );
				}

				if( g_pDSound )
				{
					m_pAniSound[UPGRADE_ANI_ING] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_ing.wav" );
					m_pAniSound[UPGRADE_ANI_SUCCESS] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_success.wav" );
					m_pAniSound[UPGRADE_ANI_FAIL] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_fail.wav" );
					m_pAniSound[UPGRADE_ANI_BROKEN] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_broken.wav" );
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONVERSION: // ��ȯ
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/Conversion_info200.bmp" );	// ���ϸ� ��ȣ�� �ǹ̴� 2(��ȭ)�� 00��(�⺻��Ʈ) �̹��� ��� ��.
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_conversion" );

			if( g_dwClientCountry == CTRY_KOR ) // �ѱ��� ���� ���߽�Ű�� � XXX ���� �ִϸ��̼� ����� �ߴ�.
			{
				// �ƿ� �ִϸ��̼� �ε带 ���Ѵ�.

				// ��Ÿ ��ġ�� 0���� ����.
				m_dwMinAniTime = 0; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 0; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 0; // 'ing'�� 8�����ӿ��� ��ȯ
			}
			else // �ٸ� ����� �־�����.
			{
				// ��������Ʈ �ִϸ��̼� �ε�
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type2_ing", 14, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type2_success", 25, 110 );
				m_SpriteAni[UPGRADE_ANI_FAIL].LoadRes( "interface/NpcUpgrade/effect/type2_fail", 24, 100 );

				// ��Ÿ �ִϸ��̼ǿ� �ʿ��� ��ġ�� �Է�.
				m_dwMinAniTime = 3000; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 5000; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 13; // 'ing'�� 8�����ӿ��� ��ȯ

				// ȿ���� �ε�.
				for( i = 0 ; i < MAX_UPGRADE_ANI ; ++i )
				{
					SAFE_RELEASE( m_pAniSound[i] );
				}

				if( g_pDSound )
				{
					m_pAniSound[UPGRADE_ANI_ING] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_ing.wav" );
					m_pAniSound[UPGRADE_ANI_SUCCESS] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_success.wav" );
					m_pAniSound[UPGRADE_ANI_FAIL] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_broken.wav" );
					//			m_pAniSound[UPGRADE_ANI_BROKEN] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_broken.wav" );
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONFUSION: // ���� �ٽ� ������.
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/Confusion_info.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_confusion" );

			if( g_dwClientCountry == CTRY_KOR ) // �ѱ��� ���� ���߽�Ű�� � XXX ���� �ִϸ��̼� ����� �ߴ�.
			{
				// �ƿ� �ִϸ��̼� �ε带 ���Ѵ�.

				// ��Ÿ ��ġ�� 0���� ����.
				m_dwMinAniTime = 0; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 0; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 0; // 'ing'�� 8�����ӿ��� ��ȯ
			}
			else // �ٸ� ����� �־�����.
			{
				// ��������Ʈ �ִϸ��̼� �ε� // �̳༮�� ������ �����̴�.
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type2_ing", 14, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type2_success", 25, 110 );

				// ��Ÿ �ִϸ��̼ǿ� �ʿ��� ��ġ�� �Է�.
				m_dwMinAniTime = 3000; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 5000; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 13; // 'ing'�� 8�����ӿ��� ��ȯ

				// ȿ���� �ε�.
				for( i = 0 ; i < MAX_UPGRADE_ANI ; ++i )
				{
					SAFE_RELEASE( m_pAniSound[i] );
				}

				if( g_pDSound )
				{
					m_pAniSound[UPGRADE_ANI_ING] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_ing.wav" );
					m_pAniSound[UPGRADE_ANI_SUCCESS] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_success.wav" );
				}
			}
		}
		break;

	case INTERFACE_TYPE_PROCESS:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/Process_info200.bmp" );	// ���ϸ� ��ȣ�� �ǹ̴� 2(��ȭ)�� 00��(�⺻��Ʈ) �̹��� ��� ��.
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_refining" );

			if( g_dwClientCountry == CTRY_KOR ) // �ѱ��� ���� ���߽�Ű�� � XXX ���� �ִϸ��̼� ����� �ߴ�.
			{
				// �ƿ� �ִϸ��̼� �ε带 ���Ѵ�.

				// ��Ÿ ��ġ�� 0���� ����.
				m_dwMinAniTime = 0; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 0; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 0; // 'ing'�� 8�����ӿ��� ��ȯ
			}
			else // �ٸ� ����� �־�����.
			{
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type2_ing", 14, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type2_success", 25, 110 );
				m_SpriteAni[UPGRADE_ANI_FAIL].LoadRes(  "interface/NpcUpgrade/effect/type2_fail", 24, 100 );

				// ��Ÿ �ִϸ��̼ǿ� �ʿ��� ��ġ�� �Է�.
				m_dwMinAniTime = 3000; // ��Ŷ�� �Ծ ��ð� �Ŀ� ��ȯ.
				m_dwMaxAniTime = 5000; // ��Ŷ�� �� �Ծ ��ð� �Ŀ� ��ȯ.
				m_iAniChangeFrame = 13; // 'ing'�� 8�����ӿ��� ��ȯ

				// ȿ���� �ε�.
				for( i = 0 ; i < MAX_UPGRADE_ANI ; ++i )
				{
					SAFE_RELEASE( m_pAniSound[i] );
				}

				if( g_pDSound )
				{
					m_pAniSound[UPGRADE_ANI_ING] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_ing.wav" );
					m_pAniSound[UPGRADE_ANI_SUCCESS] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_success.wav" );
					m_pAniSound[UPGRADE_ANI_FAIL] = g_pDSound->LoadWavToOutside( "data/sound/NpcUp_broken.wav" );
				}
			}
		}
		break;
	case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/info_Conversion2_50.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_conversion" );
			m_iTextInfoIndex = 50;
			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/info_upgrade.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_enhancement" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;

	case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/info_upgrade.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_enhancement" );

			m_gem_image.Init("npcupgrade/jewellist",5,7);

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/info_godpow.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_Godpower" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;


	case INTERFACE_TYPE_ITEMMAKE:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pBack_Title, "interface/npcupgrade/TITLE_makeitem.bmp" );
			if( m_pBack_Title )
				m_pBack_Title->SetColorKey(TRANS_COLOR);
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/itemmake.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_production" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
			break;
		}

	case INTERFACE_TYPE_BUFFMAKE_MAKE:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pBack_Title, "interface/npcupgrade/TITLE_makeitem.bmp" );
			if( m_pBack_Title )
				m_pBack_Title->SetColorKey(TRANS_COLOR);
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/buffmake_make.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_production" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;

	case INTERFACE_TYPE_BUFFMAKE_GATCHA:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pBack_Title, "interface/npcupgrade/TITLE_gatcha.bmp" );
			if( m_pBack_Title )
				m_pBack_Title->SetColorKey(TRANS_COLOR);
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/buffmake_gatcha.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_gatcha" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;

	case INTERFACE_TYPE_BUFFMAKE_MEDAL:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pBack_Title, "interface/npcupgrade/TITLE_medal.bmp" );
			if( m_pBack_Title )
				m_pBack_Title->SetColorKey(TRANS_COLOR);
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/buffmake_medal.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_medal" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
		}
		break;

	case INTERFACE_TYPE_ITEMSEPARATE:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pBack_Title, "interface/npcupgrade/TITLE_disjointing.bmp" );
			if( m_pBack_Title )
				m_pBack_Title->SetColorKey(TRANS_COLOR);
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/itemseparate.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_disassamble" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
			break;
		}

	case INTERFACE_TYPE_GMAGICSTONE:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pBack_Title, "interface/REFINE/Refine_title.bmp" );
			if( m_pBack_Title )
				m_pBack_Title->SetColorKey(TRANS_COLOR);

			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/info_G_Magicstone.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/BTN_REFINING" );

			m_dwMinAniTime = 0;
			m_dwMaxAniTime = 0;
			m_iAniChangeFrame = 0;
			break;
		}




	default:
		return;
	}

	m_UpgradeBtn.LoadRes();
	m_BtnCancel.LoadRes();

	m_pBackSur->SetColorKey( TRANS_COLOR );

	m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth() - 291;
	m_pBackSur->Ypos = 0;

	m_pDiaGem->Xpos = m_pBackSur->Xpos + 105;
	m_pDiaGem->Ypos = m_pBackSur->Ypos + 198;
	m_pRegentDiaGem->Xpos = m_pBackSur->Xpos + 105;
	m_pRegentDiaGem->Ypos = m_pBackSur->Ypos + 198;

	m_pRegOrGrtDiaGem->Xpos = m_pBackSur->Xpos + 105;
	m_pRegOrGrtDiaGem->Ypos = m_pBackSur->Ypos + 198;

	m_my_slotstart_x = m_pBackSur->Xpos + 34;
	m_my_slotstart_y = m_pBackSur->Ypos + 251;


	m_UpgradeBtn.SetPosition( m_pBackSur->Xpos+47, m_pBackSur->Ypos+199 );

	m_BtnCancel.SetPosition( m_pBackSur->Xpos+214, m_pBackSur->Ypos+7 );

	m_NeedGemText.SetPos( m_pBackSur->Xpos+35, m_pBackSur->Ypos+201+5 );
	m_NeedGemNumText.SetPos( m_pBackSur->Xpos+35, m_pBackSur->Ypos+201+5 );
	m_NeedMoneyText1.SetPos( m_pBackSur->Xpos+39, m_pBackSur->Ypos+170 ); // Place the item on it.
	m_NeedMoneyText2.SetPos( m_pBackSur->Xpos+35, m_pBackSur->Ypos+383 );

	m_HaveMoneyText.SetPos( m_pBackSur->Xpos+35, m_pBackSur->Ypos+423 ); // �ι��� ĭ


	if( m_iType == INTERFACE_TYPE_CONVERSION  // ������, ������ "�ʿ亸��"������ ��ġ�� ��¦ �ٲ��ش�.
			|| m_iType == INTERFACE_TYPE_PROCESS )
	{
		m_NeedGemText.m_PosY -= 64;
	}

	m_HaveMoneyText.SetString( g_pRoh->m_Money, TRUE ); // �ڽ��� �����ݾ��� �������̽� ���� ����. ��ȭ�� ���� ���� �������ִ� �κ��� ������ ����.
	m_iOldMoney = g_pRoh->m_Money;

	CheckUpgradeNeed(); // �ε�ÿ� ��ȯâ�� �̸� �־�а�쵵 ������ �̶��� �ʿ� ���ӵ��� ����ؾ��Ѵ�.

	if( m_iType == INTERFACE_TYPE_CONVERSION  // ������, ������ "�ʿ亸��"������ ��ġ�� ��¦ �ٲ��ش�.
			|| m_iType == INTERFACE_TYPE_PROCESS
			|| m_iType == INTERFACE_TYPE_320LV_WEAPON_CONVERSION
	  )
	{
		UpdateTextInfo( m_iType, m_iTextInfoIndex ); // �ٲ���ٸ� ����.
	}
}

CItem* CControlUpgrade::Draw()
{
	if( !g_pDisplay )
		return NULL;

	m_pOverItem = NULL; // ���⼭ �ʱ�ȭ �ϰ� �ؿ� DrawMySlot()���� ���õȴ�.

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	if ( NULL != m_pBack_Title )
	{
		g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBack_Title );
	}

	DrawNeedInfo(); // �������� ��ȭ�� �ʿ��� ������ �ݾ��� ���.

	m_UpgradeBtn.Draw();
	m_BtnCancel.Draw();

	// ���� ��ȭ�Ǿ��� üũ�ؼ� �������ش�.
	if( g_pRoh && m_iOldMoney != g_pRoh->m_Money )
	{
		m_HaveMoneyText.SetString( g_pRoh->m_Money, TRUE );
		m_iOldMoney = g_pRoh->m_Money;
	}

	m_HaveMoneyText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+423 , 163);


	// ���� ��ȯ ó��( ������ �ִϸ��̼� ó���� �ƴ϶� ���� �ٲٱ� ó���ӿ� ���� )
	ProcessState();

	if( m_NowState == UPGRADE_ANI_NON ) // �ִϸ��̼� ���̾ƴҰ��.
	{
		DrawMySlot(); // ������ �׸���.
	}
	else if( m_NowState >= 0 && m_NowState < MAX_UPGRADE_ANI ) // �̻� ���� üũ.
	{
		m_SpriteAni[m_NowState].Update( timeGetTime()-m_dwLastTickTime );
		m_SpriteAni[m_NowState].Draw( m_my_slotstart_x, m_my_slotstart_y );
	}



	m_dwLastTickTime = timeGetTime(); // ƽ�� ��� ���� ��Ű��.

	return m_pOverItem; // �� ������ �ϴ� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ؼ� ǥ�����ֱ� �����ε�...
}

void CControlUpgrade::DrawNeedInfo() // �������� ��ȭ�� �ʿ��� ������ �ݾ��� ���.
{
	int i = 0;

	if( m_pTextInfo )
		g_pDisplay->Blt( m_pBackSur->Xpos + 34, m_pBackSur->Ypos + 80, m_pTextInfo );

	switch(m_iType)
	{
	case INTERFACE_TYPE_UPARMOR_400:
		{
			if( CheckUpgradeNeed() == -1 )
			{
				break;
			}

			m_NeedGemText.Draw(m_pBackSur->Xpos+59, m_pBackSur->Ypos+145);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);

			const int gemGap = 16;

			int gemPosX = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2 + 30;
			int gemCntPosX = gemPosX + gemGap + m_NeedGemText.m_FontWidth / 2;
			int offsetY = m_pDiaGem->Ypos-74;

			char buf[MAX_PATH] = {0,};
			g_pNk2DFrame->GetItemRender()->BeginRenderLolo();
			for(int i = 0; i < NEEDGEM_MAX; i++)
			{
				if( m_iNeedGemCount[i] == 0 )
					continue;

				sprintf(buf, "(%d)", m_iNeedGemCount[i]);
				g_pNk2DFrame->RenderItemInUIWithSize(m_iNeedGemVnum[i], gemPosX, offsetY, 27, 54);
				if( m_iNeedGemVnum[i] == 3331 ) // �ż�
				{						
					g_pNk2DFrame->RenderItemInUIWithSize(3332, gemPosX + 29, offsetY, 27, 54);

					sprintf(buf, "OR   (%d)", m_iNeedGemCount[i]);
				}
				m_NeedGemText.SetString(buf, FALSE);
				m_NeedGemText.Draw(gemPosX + 20, offsetY + 21);

				offsetY += gemGap;
			}
			g_pNk2DFrame->GetItemRender()->EndRenderLolo();
		}
		break;
	case INTERFACE_TYPE_UPARMOR:
	case INTERFACE_TYPE_UPWEAPON:
		{
			m_NeedGemText.Draw(m_pBackSur->Xpos+59, m_pBackSur->Ypos+160);
			m_NeedGemNumText.Draw(m_pBackSur->Xpos+150, m_pBackSur->Ypos+160);
			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+175,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);

			int mx = 0;

			// �� �׸� �׸���
			for( i = 0 ; i < MAX_NEED_ITEM_IN_NPC_UPGRADE ; ++i )
			{
				if( NeedItem[i] ) // �ʿ� ������ �ִ� �༮�� �ִٸ�...
				{
					switch( i )
					{
					case UPGRADE_NEED_DIA:
						mx = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2 + 30;
						g_pDisplay->Blt( mx, m_pDiaGem->Ypos-42, m_pDiaGem );
						mx = mx + m_pDiaGem->GetWidth() + m_NeedGemText.m_FontWidth / 2;
						break;
					case UPGRADE_NEED_REGENT_DIA:
						mx = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2 + 30;
						g_pDisplay->Blt( mx, m_pRegentDiaGem->Ypos-42, m_pRegentDiaGem );
						mx = mx + m_pRegentDiaGem->GetWidth() + m_NeedGemText.m_FontWidth / 2;
						break;
					}

					// ����� ������ ������ 1���̹Ƿ� �ʿ䰹���� �ִ� �༮�� ������ ��� ���� Ż���Ѵ�.
					//			i = MAX_NEED_ITEM_IN_NPC_UPGRADE; // ���� Ż��� �Ǽ�. ����� Ż���� �ʿ䰡 ��� ��.
				}
			}

			if( mx !=0 )
			{
				m_NeedGemNumText.SetPos(mx, m_NeedGemText.m_PosY );
			}
			m_NeedGemNumText.Draw();
		}
		break;

	case INTERFACE_TYPE_CONVERSION : // ������ �������̽��� ���
		{
			if( m_iTextInfoIndex >= 203 ) // 203 �̻��� �ʿ����� �׸������̴�.
				m_NeedGemText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+206,163);

			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;

	case INTERFACE_TYPE_CONFUSION : // ���� �ٽ� ������
		{
			//m_NeedGemText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+206,163);
			////m_NeedGemNumText.Draw();
			//m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			//m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);

			//int mx = 0;

			//// �� �׸� �׸��� // �� �༮�� ������ ���̾ƴ�.
			//if( NeedLaim )
			//{
			//	mx = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2;
			//	g_pDisplay->Blt( mx, m_pDiaGem->Ypos, m_pDiaGem );
			//	mx = mx + m_pDiaGem->GetWidth() + m_NeedGemText.m_FontWidth / 2;
			//}

			m_NeedGemText.Draw(m_pBackSur->Xpos+59, m_pBackSur->Ypos+160);
			m_NeedGemNumText.Draw(m_pBackSur->Xpos+150, m_pBackSur->Ypos+160);
			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+175,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);

			int mx = 0;


			if( NeedLaim )
			{
				mx = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2 + 30;
				g_pDisplay->Blt( mx, m_pDiaGem->Ypos-42, m_pDiaGem );
				mx = mx + m_pDiaGem->GetWidth() + m_NeedGemText.m_FontWidth / 2;
			}


			if( mx !=0 )
			{
				m_NeedGemNumText.SetPos(mx, m_NeedGemText.m_PosY );
			}
			m_NeedGemNumText.Draw();

		}
		break;

	case INTERFACE_TYPE_PROCESS: // ������ �������̽��� ���
		{
			if( m_iTextInfoIndex >= 203 ) // 203 �̻��� �ʿ����� �׸������̴�.
				m_NeedGemText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+206,163);

			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;
	case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
		{
			if(m_iTextInfoIndex == 53)
			{
				m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			}
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
		{
			int mx = 0;

			if( NeedItem[UPGRADE_NEED_REGENT_DIA] )
			{
				mx = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2 + 30;
				g_pDisplay->Blt( mx, m_pRegOrGrtDiaGem->Ypos-46, m_pRegOrGrtDiaGem );
				mx = mx + m_pRegOrGrtDiaGem->GetWidth() + m_NeedGemText.m_FontWidth / 2;
			}

			m_NeedGemText.Draw(m_pBackSur->Xpos+59, m_pBackSur->Ypos+154);
			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
			if( mx != 0 )
				m_NeedGemNumText.SetPos(mx, m_NeedGemText.m_PosY );
			m_NeedGemNumText.Draw();
		}
		break;

	case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
		{
			int mx = m_NeedGemText.m_PosX + m_NeedGemText.m_Width + m_NeedGemText.m_FontWidth / 2 + 30;

			int gem1 = GetGemToIndex(m_need_gem[0]);
			int gem2 = GetGemToIndex(m_need_gem[1]);
			int gem3 = GetGemToIndex(m_need_gem[2]);

			if( gem1 != -1 )
				m_gem_image.Draw(mx , m_pBackSur->Ypos+150 ,gem1);

			if( gem2 != -1 )
			{
				// or
				m_gem_image.Draw(mx + 16 , m_pBackSur->Ypos+150 ,34);
				m_gem_image.Draw(mx + 32 , m_pBackSur->Ypos+150 ,gem2);
			}
			if( gem3 != -1 )
			{
				m_NeedMoneyText1.SetString((char*)G_STRING(IDS_LHSTRING1954));
				m_NeedMoneyText1.Draw(m_pBackSur->Xpos+59, m_pBackSur->Ypos+170);
				m_gem_image.Draw(mx  , m_pBackSur->Ypos+168 ,gem3);
			}
			else
			{
				m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			}

			if( gem1 != -1 || gem2 != -1 )
				m_NeedGemText.Draw(m_pBackSur->Xpos+59, m_pBackSur->Ypos+154);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER:
		{
			if( NeedItem[UPGRADE_NEED_REGENT_DIA] )
			{
			}
			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;

	case INTERFACE_TYPE_ITEMMAKE:
	case INTERFACE_TYPE_BUFFMAKE_MAKE:
	case INTERFACE_TYPE_BUFFMAKE_GATCHA:	
		{
			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;

	case INTERFACE_TYPE_ITEMSEPARATE:
		{

		}
		break;

	case INTERFACE_TYPE_GMAGICSTONE:
		{
			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;


	}
}

void CControlUpgrade::DrawMySlot()
{
	if( !g_pDisplay )
		return;

	RECT rcRect;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

	// �������� ��ġ�� ���� ǥ��
	CItem *pItem = NULL;
	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	while (pItem)
	{
		rcRect.left = 0;
		rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(pItem);
		rcRect.top = 0;
		rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(pItem);
		g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*pItem->m_SlotX),
						 m_my_slotstart_y + (SLOT_HEIGHT*pItem->m_SlotY),
						 m_pExistSlot, &rcRect);
		pItem = pItem->m_Next;
	}

	// Extra Slot�� �ִ� �������� ���� �� ǥ��
	int slot_x_num = 0, slot_y_num = 0;
	if (g_pRoh && g_pRoh->m_ExtraSlot)
	{
		if (point.x >= m_my_slotstart_x
				&& point.x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM )
				&& point.y >= m_my_slotstart_y
				&& point.y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) )
		{
			if (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2)
				slot_x_num = (point.x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
			else
				slot_x_num = (point.x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

			if (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2)
				slot_y_num = (point.y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
			else
				slot_y_num = (point.y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);

			if (slot_x_num >= 0
					&& slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
					&& slot_y_num >= 0
					&& slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM)
			{
				rcRect.left = 0;
				rcRect.right = SLOT_WIDTH * GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot);
				rcRect.top = 0;
				rcRect.bottom = SLOT_HEIGHT * GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot);
				if (g_pRoh->GetItemIndexInExgSlot(slot_x_num, slot_y_num,
												  GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot), GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot))
						>= -1)
				{
					g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
									 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pEnableSlot, &rcRect);
				}
				else
				{
					g_pDisplay->Blt( m_my_slotstart_x + (SLOT_WIDTH*slot_x_num),
									 m_my_slotstart_y + (SLOT_HEIGHT*slot_y_num),
									 m_pDisableSlot, &rcRect);
				}
			}
		}
	}

	// �� ��ȯǰ �׸���..
	if (point.x >= m_my_slotstart_x && point.y >= m_my_slotstart_y)
	{
		slot_x_num = GET_MY_SLOT_X(point.x);
		slot_y_num = GET_MY_SLOT_Y(point.y);
	}
	else
	{
		slot_x_num = -1;
		slot_y_num = -1;
	}
	int index = 0;
	if( g_pRoh )
		index = g_pRoh->GetExgSlotIndex(slot_x_num, slot_y_num);

	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if( m_pItemRender )
	{
		m_pItemRender->BeginRenderLolo();		// �߿�..  m_pItemRender->RenderLolo�� �Ķ���Ϳ� ������ �ش�.
		while (pItem)
		{
			if (pItem->m_Index == index)
			{
				g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem , TRUE , FALSE , FALSE, TRUE );
				m_pOverItem = pItem;
			}
			else
			{
				DWORD ambient = 0x00555555;
				if (g_pNk2DFrame->IsScroll(pItem) )
					ambient = 0x00cccccc;
				else if (pItem->m_PlusNum > 0)
					ambient = UPGRADE_ITEM_COLOR;

				g_pNk2DFrame->RenderItemInUI(m_my_slotstart_x + pItem->m_SlotX * SLOT_WIDTH,
											 m_my_slotstart_y + pItem->m_SlotY * SLOT_HEIGHT,
											 pItem , FALSE , FALSE , FALSE, TRUE );
			}
			pItem = pItem->m_Next;
		}
		m_pItemRender->EndRenderLolo();
	}
}

LRESULT CControlUpgrade::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char commOutBuf[512];
	int hr = -1;

	int x, y;
	x = LOWORD( lParam );
	y = HIWORD( lParam );

	if( m_bSlotLock ) // ��ȯ�ϴ¼��� extra �ϴ� ���� ���� �����̴�.lock
	{
		// ���� �ð� �Ŀ� ���� ������ �������ִ°� �����?
		return 1;
	}

	switch (msg)
	{
	case WM_RBUTTONDBLCLK:

		// ���� ��� �̵� �Ұ�
		if( pCMyApp->m_bEquipLock == 1 )
			return 1;
		// ���� �̵� �Ұ�
		if( pCMyApp->m_bRingLock == 1 )
			return 1;
		if(pCMyApp->m_bBrLock ==1)
			return 1;
		if(pCMyApp->m_bNecklaceLock==1)		///��
			return 1;

		// ExtraSlot�� �������� �������
		if( g_pRoh->m_ExtraSlot )
			return 1;

		if( IsInside( x, y ) )
		{
			// ���� ���׷��̵� â�� ������ ���
			if( g_pNk2DFrame
					&& g_pNk2DFrame->GetControlInven()
					&& g_pNk2DFrame->GetControlInven()->m_bAct_Masuk )
			{
				g_pNk2DFrame->GetControlInven()->Close_Masuk();
				return 1;
			}

			// ���� �˻�
			if( g_pRoh
					&& ( x >= m_my_slotstart_x )
					&& ( x < m_my_slotstart_x + ( SLOT_WIDTH * EXG_SLOT_X_NUM ) )
					&& ( y >= m_my_slotstart_y )
					&& ( y < m_my_slotstart_y + ( SLOT_HEIGHT * EXG_SLOT_Y_NUM ) ) )
			{
				int nSlot_X_Num = 0;
				int nSlot_Y_Num = 0;

				// ���� ���
				nSlot_X_Num = ( x - m_my_slotstart_x ) / SLOT_WIDTH;
				nSlot_Y_Num = ( y - m_my_slotstart_y ) / SLOT_HEIGHT;

				int nItemIndex = g_pRoh->GetExgSlotIndex( nSlot_X_Num, nSlot_Y_Num );
				if( nItemIndex < 0 )
					return 1;

				int nVNum = 0;
				int nPlusNum = 0;
				int nSpecialNum = 0;

				CItem* pItem = g_pRoh->m_ExgInven;
				CItem* pTempItem = pItem;

				// ���ؽ� ���� �̿��� �������� wnum ����
				while( pItem )
				{
					if( pItem->m_Index == nItemIndex )
					{
						int nWidth = GET_ITEM_WIDTH( pItem );
						int nHeight = GET_ITEM_HEIGHT( pItem );

						if( nWidth != 1 || nHeight != 1 )
							return 0;

						nVNum = pItem->m_Vnum;
						nPlusNum = pItem->m_PlusNum;
						nSpecialNum = pItem->m_Special;

						break;
					}
					pItem = pItem->m_Next;
				}

				// const ���� ����
				char szTmp[ 10 ] = {0,};
				char szStr[ 100 ] = {0,};

				// �۾����� ������ Ÿ�� 1.5��
				// �ּ����� ������ġ
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlExchange() )
				{
					if( g_pNk2DFrame->GetControlExchange()->m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > g_pNk2DFrame->GetControlExchange()->m_timeExchLock + 1500) // 1.5�ʰ� ��������
							g_pNk2DFrame->GetControlExchange()->m_byExchLock = 0;
						else
							return 1;
					}

					// �����̰� �������� �÷��� ����
					g_pNk2DFrame->GetControlExchange()->SetExchLock( 1 );
				}

				// ������ �����۰� ���� ������ ����
				while( pTempItem )
				{
					if( ( pTempItem->m_Vnum == nVNum )
							&& ( pTempItem->m_PlusNum == nPlusNum )
							&& ( pTempItem->m_Special == nSpecialNum ) )
					{
						sprintf( szTmp, "%d %d ", pTempItem->m_SlotX, pTempItem->m_SlotY );
						strcat( szStr, szTmp );
					}
					pTempItem = pTempItem->m_Next;
				}

				CheckUpgradeNeed(); // �˻�Ӹ� �ƴ϶� �ؽ�Ʈ ���õ� ���ش�.

				sprintf( commOutBuf, "exch_get %d %s\n", g_pRoh->m_CurPackNum, szStr );

				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

			}

		}

		break;

	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_UpgradeBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_BtnCancel.MsgProc( hWnd, msg, wParam, lParam );

			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					&& y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) ) // ��ȯâ ���� �κ� Ŭ����...
			{

				if( timeGetTime() - m_dwMouseClickTickTime < 300 )  // 0.3��
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlExchange() )
				{
					if( g_pNk2DFrame->GetControlExchange()->m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > g_pNk2DFrame->GetControlExchange()->m_timeExchLock + 1500) // 1.5�ʰ� ��������
							g_pNk2DFrame->GetControlExchange()->m_byExchLock = 0;
						else
							return 1;
					}
				}

				if( g_pRoh && g_pRoh->m_ExtraSlot )
				{
					if( GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) % 2 )
						slot_x_num = (x - m_my_slotstart_x) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);
					else
						slot_x_num = (x - m_my_slotstart_x + (SLOT_WIDTH/2)) / SLOT_WIDTH - (GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot)/2);

					if( GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) % 2 )
						slot_y_num = (y - m_my_slotstart_y) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);
					else
						slot_y_num = (y - m_my_slotstart_y + (SLOT_HEIGHT/2)) / SLOT_HEIGHT - (GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot)/2);



					if( slot_x_num >= 0 && slot_x_num + GET_ITEM_WIDTH(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_X_NUM
							&& slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM ) // Ŭ���� ��ġ�� ��ȿ�� ��ġ��..(������ üũ)
					{
						if( g_pRoh->AddToExgInven(slot_x_num,slot_y_num) )
						{
							sprintf( commOutBuf, "exch inven %d %d\n", slot_x_num, slot_y_num );
							if( g_pTcpIp )
								g_pTcpIp->SendNetMessage( commOutBuf );
						}
					}
				}
				else
				{
					// �׼����̶�� �Ⱦȵǰ�...

					if(    g_pRoh && g_pRoh->AddToExgInven( (x-m_my_slotstart_x)/SLOT_WIDTH, (y-m_my_slotstart_y)/SLOT_HEIGHT )     )
					{
						sprintf( commOutBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT );
						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage(commOutBuf);
					}
				}

				// �������� �����̸� �������� �˻��ؼ� �ʿ������� ��½����ش�.
				if( m_iType == INTERFACE_TYPE_CONVERSION || m_iType == INTERFACE_TYPE_PROCESS )
				{
					if( m_iTextInfoIndex == 201 || m_iTextInfoIndex == 202 ) // ��� ��Ʈ ���¿����� �κ�â Ŭ���ñ��� TextInfo�� �ٲ��� �ʴ´�. (���� Ŭ���Ǹ� �ٽ� ���� ǥ�ð˻縦 �ϵ���..)
					{
						m_iTextInfoIndex = 200; // Ŭ�������� �⺻ ��Ʈ��...(��ȯ �������� ������ �ٷ� �������� �ٲ𲨴�.)
					}
				}

				// ������ �������̽��� ���� TextInfo�� ���ǽ� ��ü�� �ٲ��ش�.
				// Ŭ���ÿ��� �����ؼ� TextInfo�� �����Ѵ�.
				if( m_iTextInfoIndex != 201 && m_iTextInfoIndex != 202 ) // ��� ��Ʈ ���¿����� �������� �־ TextInfo�� �ٲ��� �ʴ´�.(�κ�â Ŭ���� �������ش�.)
				{
					m_iTextInfoIndex = 200;	// �������� TextInfo ������ִ� �κ��� 0���� �ʱ�ȭ.(0�� �⺻ ��Ʈ��.)
				}

				if( m_iType == INTERFACE_TYPE_320LV_WEAPON_CONVERSION )
				{
					if( m_iTextInfoIndex != 50 )
						m_iTextInfoIndex = 50;
				}
				CheckUpgradeNeed(); // �˻�Ӹ� �ƴ϶� �ؽ�Ʈ ���õ� ���ش�.

				if( m_iType == INTERFACE_TYPE_CONVERSION || m_iType == INTERFACE_TYPE_PROCESS
						|| m_iType == INTERFACE_TYPE_320LV_WEAPON_CONVERSION
				  )
				{
					UpdateTextInfo( m_iType, m_iTextInfoIndex ); // �ٲ���ٸ� ����.
				}



			}
		}
		else if( IsPlayArea(x,y) ) // �������̽� ���� ���� Ŭ����.
		{
			g_pNk2DFrame->ToggleUpgradeWindow( m_iType, TRUE ); // Toggle �Լ��� ����ؼ� ���� �����Ų��.
		}
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		if( IsInside(x,y) ) // �������̽��� ������ Ŭ������ ���.
		{
			m_UpgradeBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_BtnCancel.MsgProc( hWnd, msg, wParam, lParam );


			if( g_pNk2DFrame && m_BtnCancel.GetState()==BTN_ACTION )
			{
				g_pNk2DFrame->ToggleUpgradeWindow( m_iType, TRUE ); // Toggle �Լ��� ����ؼ� ���� �����Ų��.
				m_BtnCancel.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
			}


			if( m_UpgradeBtn.GetState() == BTN_ACTION ) // ���׷��̵� ��ư�� Ŭ����.
			{

				if( g_pRoh->m_ExtraSlot )
					return 1;


				if( CheckSlotPure() != -1 ) // ��ȯâ�� �������� �������� �ϳ��� ��������...
				{
					if( !CheckBrokenItem() ) // ������ ������ ������ ���â�� �ѹ� �� ����ش�.
					{
						m_Result = 0; // �ʱ�ȭ.

						switch( m_iType ) // Ÿ�Կ� ���� ��Ŷ�� �ٸ���.
						{
						case INTERFACE_TYPE_UPARMOR_400:
							sprintf( commOutBuf, "npc_up armor_400\n" );
							break;
						case INTERFACE_TYPE_UPARMOR: // ��ȭ �������̽�(��)�� ���.
							sprintf( commOutBuf, "npc_up armor\n" );
							break;
						case INTERFACE_TYPE_UPWEAPON: // ��ȭ �������̽�(����)�� ���
							sprintf( commOutBuf, "npc_up weapon\n" );
							break;
						case INTERFACE_TYPE_CONVERSION: // ������ �������̽��� ���
							sprintf( commOutBuf, "npc_up conv\n" );
							break;
						case INTERFACE_TYPE_CONFUSION: // ���� �ٽ� ������
							sprintf( commOutBuf, "npc_up reload\n" );
							break;
						case INTERFACE_TYPE_PROCESS: // ����
							sprintf( commOutBuf, "npc_up refine\n" );
							break;
						case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
							sprintf( commOutBuf, "npc_up conv\n" );
							break;
						case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
							sprintf( commOutBuf, "npc_up weapon\n" );
							break;
						case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
							sprintf( commOutBuf, "npc_up newup\n" );
							break;
						case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER:
							sprintf( commOutBuf, "npc_up godpower\n" );
							break;

						case INTERFACE_TYPE_ITEMMAKE:
							{
								sprintf( commOutBuf, "makes\n" );
							}
							break;

						case INTERFACE_TYPE_BUFFMAKE_MAKE:
							{
								sprintf( commOutBuf, "buffmake_make\n");
							}
							break;

						case INTERFACE_TYPE_BUFFMAKE_GATCHA:
							{
								sprintf( commOutBuf, "buffmake_gatcha\n");
							}
							break;

						case INTERFACE_TYPE_BUFFMAKE_MEDAL:
							{
								sprintf( commOutBuf, "buffmake_medal\n");
							}
							break;

						case INTERFACE_TYPE_ITEMSEPARATE:
							{
								sprintf( commOutBuf, "seperate\n" );
							}
							break;

						case INTERFACE_TYPE_GMAGICSTONE:
							{
								sprintf( commOutBuf, "ex_item ma_stone\n" );
							}
							break;


						}

						if( g_pTcpIp )
						{
							g_pTcpIp->SendNetMessage( commOutBuf );
							// ��Ŷ�� ������. (�ִϸ��̼��� ���� ��ٸ��� ���� �ٷ� ������.)
							m_bSlotLock = TRUE; // ��ü�� ���� �� �Ŀ� ���� ���ϵ���....
						}
					}


				}

				m_UpgradeBtn.SetState(BTN_NORMAL);	// ��ư�� �������ͽ��� ���� Ǯ������ϳ�����.
			}

			return 1;
		}

	case WM_RBUTTONDOWN:

		x = LOWORD( lParam );
		y = HIWORD( lParam );
		if( IsInside(x,y) )
			return 1;
		break;
	}
	return 0;
}

HRESULT CControlUpgrade::BrokenPopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	char commOutBuf[512];

	if(nMsg == 1)
	{
		// ��Ŷ�� ������. (�ִϸ��̼��� ���� ��ٸ��� ���� �ٷ� ������.)
		m_bSlotLock = TRUE; // ��ü�� ���� �� �Ŀ� ���� ���ϵ���....
		m_Result = 0; // �ʱ�ȭ.


		// Ÿ�Կ� ���� ��Ŷ�� �ٸ���.
		switch(m_iType )
		{
		case INTERFACE_TYPE_UPARMOR_400:
			sprintf( commOutBuf, "npc_up armor_400\n" );
			break;
		case INTERFACE_TYPE_UPARMOR: // ��ȭ �������̽�(��)�� ���.
			sprintf( commOutBuf, "npc_up armor\n" );
			break;
		case INTERFACE_TYPE_UPWEAPON: // ��ȭ �������̽�(����)�� ���
			sprintf( commOutBuf, "npc_up weapon\n" );
			break;
		case INTERFACE_TYPE_CONVERSION : // ������ �������̽��� ���
			sprintf( commOutBuf, "npc_up conv\n" );
			break;
		case INTERFACE_TYPE_CONFUSION : // ���� �ٽ� ������
			sprintf( commOutBuf, "npc_up reload\n" );
			break;
		case INTERFACE_TYPE_PROCESS : // ����
			sprintf( commOutBuf, "npc_up refine\n" );
			break;
		case INTERFACE_TYPE_320LV_WEAPON_CONVERSION : // ����
			sprintf( commOutBuf, "npc_up conv\n" );
			break;
		case INTERFACE_TYPE_320LV_WEAPON_UPGRADE : // ����
			sprintf( commOutBuf, "npc_up weapon\n" );
			break;
		case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
			sprintf( commOutBuf, "npc_up newup\n" );
			break;
		case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER : // ����
			sprintf( commOutBuf, "npc_up godpower\n" );
			break;
		case INTERFACE_TYPE_ITEMMAKE:
			sprintf( commOutBuf, "makes\n" );
			break;
		case INTERFACE_TYPE_BUFFMAKE_MAKE:
			sprintf( commOutBuf, "buffmake_make\n");			
			break;
		case INTERFACE_TYPE_BUFFMAKE_GATCHA:
			sprintf( commOutBuf, "buffmake_gatcha\n");			
			break;
		case INTERFACE_TYPE_BUFFMAKE_MEDAL:
			sprintf( commOutBuf, "buffmake_medal\n");
			break;
		case INTERFACE_TYPE_ITEMSEPARATE:
			sprintf( commOutBuf, "seperate\n" );
			break;
		case INTERFACE_TYPE_GMAGICSTONE:
			sprintf( commOutBuf, "ex_item ma_stone\n" );
			break;
		}
		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage( commOutBuf );
	}

	return 0L;
}

BOOL CControlUpgrade::IsInside( int x, int y )
{
	if( !m_pBackSur )
		return FALSE;

	if (x >= m_pBackSur->Xpos
			&& x < m_pBackSur->Xpos + m_pBackSur->GetWidth()
			&& y >= m_pBackSur->Ypos
			&& y < m_pBackSur->Ypos + m_pBackSur->GetHeight() )
		return TRUE;

	return FALSE;
}

BOOL CControlUpgrade::IsPlayArea( int x, int y ) // �������̽��� ��� ���� Ŭ���ߴ����� üũ.
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

int CControlUpgrade::CheckUpgradeNeed() // ��ȭ�� �ʿ��� ��(����,����)�� ������ ���Ѵ�. ������ �������� �����͸� ����.(�������� ���ÿ� ������ �ö������� NULL�� �����Ѵ�. ������ �� �ϳ��� �ö������� ����� ����. NULL�� ��� ������ �߸� �ø� ����.)
{
	CItem* pItem = NULL;
	CItem* pCheckItem = NULL;
	CItem* pCheckItem2 = NULL;
	//	CItem* pGem = NULL; // ���ִ� ������ ������.(������ ������ ��� ����� ������ ���� üũ�ȴ�.)(������ ������ Ȥ�� ���������� �ʿ����� ��� �ҽ��� ������ �ʿ��ϴ�.)
	int ItemCount = 0, ItemCount2 = 0; // ���ִ� ������ ���� üũ��. ���� üũ�Ѵ�.
	int i = 0;
	int special_level = -1, special_level1 = -1, special_level2 = -1, special_level3 = -1, special_level4 = -1;
	char strTemp[256];
	char strTemp2[30];

	ZeroMemory( m_need_gem , sizeof(m_need_gem));

	// �ʿ����� �ʱ�ȭ.( �� �Լ� ���� �տ� ������Կ� ����. )
	for( i = 0 ; i < MAX_NEED_ITEM_IN_NPC_UPGRADE ; ++i )
	{
		NeedItem[i] = 0; // ��ȭ �Ǵ� ������ ������ �������� �ʿ��� ����.
	}
	NeedLaim = 0; // �ʿ� ����.


	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if(!pItem)
	{
		m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
		m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
		m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
		m_NeedMoneyText2.SetString( "", TRUE );

		return -1;
	}

	// �ؽ�Ʈ�� �ʱ�ȭ
	//-- IDS_NEED_GEM : �ʿ� ����:
	sprintf( strTemp, (char*)G_STRING(IDS_NEED_GEM) );
	m_NeedGemText.SetString( strTemp, FALSE ); // ù��° ���ڰ� ���ڿ��̸� �ι�° ���ڴ� ��Ʈ ���̴�.FALSE�� �ָ� �˾Ƽ� init()�� ������ ���� �ȴ�.
	sprintf( strTemp, "(%d)", NeedItem[UPGRADE_NEED_REGENT_DIA] );
	m_NeedGemNumText.SetString( strTemp, FALSE ); // ù��° ���ڰ� ���ڿ��̸� �ι�° ���ڴ� ��Ʈ ���̴�.FALSE�� �ָ� �˾Ƽ� init()�� ������ ���� �ȴ�.
	m_NeedMoneyText1.SetString( "", TRUE ); // ù��° ���ڰ� ���ڸ� �ι�° ���ڴ� �޸� ���δ�.
	m_NeedMoneyText2.SetString( "", TRUE ); // �Ⱥ��̵��� �ʱ�ȭ.(���������� ����̶�� ����ȴ�.)

	memset(&m_iNeedGemVnum, 0, sizeof(m_iNeedGemVnum));
	memset(&m_iNeedGemCount, 0, sizeof(m_iNeedGemCount));

	switch(m_iType )
	{
	case INTERFACE_TYPE_UPARMOR_400:
		{
			DefenseValMgr* defValMgr = DefenseValMgr::GetInstance();
			if( defValMgr == NULL )
				break;

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON )
				{
					if( defValMgr->IsExist(pItem->m_Vnum) )
					{
						++ItemCount;
						pCheckItem = pItem;
					}
				}

				pItem = pItem->m_Next;
			}

			m_NeedGemText.SetString( "", TRUE );
			m_NeedGemNumText.SetString( "", TRUE );

			if( ItemCount != 1 )
			{
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}

			if( pCheckItem )
			{
				int vnum = pCheckItem->m_Vnum;
				int itemPlus = pCheckItem->m_PlusNum + 1;
				NeedLaim = defValMgr->GetLaim(vnum, itemPlus);
				sprintf( strTemp2, "%d", NeedLaim );
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) );
				m_NeedMoneyText1.SetString("", TRUE);
				m_NeedMoneyText2.SetString(NeedLaim, TRUE);

				m_NeedGemText.SetString( (char*)G_STRING(IDS_NPCUPGRADE_NEEDGEM), FALSE );

				for(int i = 0; i < NEEDGEM_MAX; i++)
				{
					m_iNeedGemVnum[i] = defValMgr->GetGemVnum(vnum, itemPlus, i);
					m_iNeedGemCount[i] = defValMgr->GetGemCount(vnum, itemPlus, i);
				}

				return 1;
			}
			else
			{
				return -1;
			}
		}
		break;
	case INTERFACE_TYPE_UPARMOR:
	case INTERFACE_TYPE_UPWEAPON:
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // ���⵵ �߰��Ǿ���. ����� ���� üũ�Ѵ�.
				{

					++ItemCount;
					pCheckItem = pItem; // �����͸� ���. �������� ���������� �ϳ� ���̾��ٸ� �� �����͸� �̿��ؼ� �ʿ��Ѱ� ���Ѵ�.

					// ������ ��ȭ�� �ı��� �� �ִ� ���ǿ��� ��� ������ ����ش�.
					if( g_SvrType == ST_ADULT_ONLY )
					{
						if( GET_TYPE(pItem) == ITYPE_WEAPON ) // && ChechIs261LvWeponItem(pItem) )
						{
							if( ChechIs261LvWeponItem(pItem)
									|| ( pItem->m_Vnum >= 1313 && pItem->m_Vnum <= 1317 || pItem->m_Vnum == 1324)
									&& !( pItem->m_MinLevel_Org >= 300 || IS_2011_ITEM_WEAPON(pItem->m_Vnum) )
							  )
							{
								switch( pItem->m_PlusNum)
								{
								case 3:
								case 7:
								case 11:
									g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG_01), TYPE_NOR_OK);
									break;
								}
							}
						}
						else
						{
							if( GET_TYPE(pItem) == ITYPE_ARMOR )
							{
								switch( pItem->m_PlusNum)
								{
								case 3:
								case 8:
									{
										g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG_01), TYPE_NOR_OK);
										break;
									}
								case 11:
									{
										if( pItem->m_MinLevel_Org > 300 || IS_2011_ITEM_ARMOR(pItem->m_Vnum) )
										{
											g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG_01), TYPE_NOR_OK);
											break;
										}
									}
								}
							}
						}
					}
				}
				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // ����,���� 1���� �ƴϾ����� ����.
			{
				m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else if( GET_TYPE(pCheckItem)==ITYPE_WEAPON && ChechIs261LvWeponItem(pCheckItem) && pCheckItem->m_PlusNum >= 10 )
			{
				m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				//-- IDS_NPCUPGRADE_MAXLEVEL : ���̻� ��ȭ�� �Ұ����� �������Դϴ�.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), FALSE );

				return -1;
			}
			else if( (GET_TYPE(pCheckItem)==ITYPE_ARMOR && pCheckItem->m_PlusNum >= 15) ||  // ���� 15�ܰ� ������ ���׷��̵尡 �����ϴ�. (05-11-10 20�ܰ�������� 15�ܰ� ������ ����)
					 (GET_TYPE(pCheckItem)==ITYPE_ARMOR && pCheckItem->m_MinLevel_Org >= 360 && pCheckItem->m_PlusNum >= 10) )
			{
				m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				//-- IDS_NPCUPGRADE_MAXLEVEL : ���̻� ��ȭ�� �Ұ����� �������Դϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), TYPE_NOR_OK, 1 );
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), FALSE );

				return -1;
			}
			else // ���������� ����,���� �ϳ����̸� �ʿ��� ���ӵ��� ���Ѵ�.
			{
				if( pCheckItem ) // �̰� �˻��ؾ��Ѵ�.
				{
					if( !CheckCanNpcUpgradeItem(pCheckItem) ) // Ư�� �����۸� ��ȭ ����.
					{
						m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
						m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}

					// [2008/7/7 Theodoric]  ���� �� ��ȭ �߰�
					if( (pCheckItem->m_Vnum >= 1560 && pCheckItem->m_Vnum <= 1579) || (pCheckItem->m_Vnum >= 1797 && pCheckItem->m_Vnum <= 1816)

							|| ( TRUE == IS_2011_ITEM_ARMOR( pCheckItem->m_Vnum ) )

					  )
					{
						++NeedItem[UPGRADE_NEED_REGENT_DIA]; // 300������ ������ ����Ʈ �ϳ���.
					}
					else if( (pCheckItem->m_Vnum >= 1190 && pCheckItem->m_Vnum <= 1229) || (pCheckItem->m_Vnum >= 1313 && pCheckItem->m_Vnum <= 1326) ) // ���� ����,��
					{
						if( (pCheckItem->m_PlusNum+1) > NPC_UPGRADE_NEED_REGENT_LEVEL ) // +9�̻� ��ȭ�� ����Ʈ��...
						{
							++NeedItem[UPGRADE_NEED_REGENT_DIA];
						}
						else
						{
							++NeedItem[UPGRADE_NEED_DIA];
						}
					}
					else if( ( pCheckItem->m_Vnum >= 2580 && pCheckItem->m_Vnum <= 2583 )
							 || ( pCheckItem->m_Vnum >= 2604 && pCheckItem->m_Vnum <= 2607 )
							 || ( pCheckItem->m_Vnum >= 2612 && pCheckItem->m_Vnum <= 2615 )
							 || ( pCheckItem->m_Vnum >= 2620 && pCheckItem->m_Vnum <= 2623 )
							 || ( pCheckItem->m_Vnum >= 2628 && pCheckItem->m_Vnum <= 2631 )
						   )
					{
						if( (pCheckItem->m_PlusNum+1) > NPC_UPGRADE_NEED_REGENT_LEVEL ) // +9�̻� ��ȭ�� ����Ʈ��...
						{
							++NeedItem[UPGRADE_NEED_REGENT_DIA];
						}
						else
						{
							++NeedItem[UPGRADE_NEED_DIA];
						}
					}
					else if( ( pCheckItem->m_Vnum >= 2636 && pCheckItem->m_Vnum <= 2639 )
							 || ( pCheckItem->m_Vnum >= 2644 && pCheckItem->m_Vnum <= 2647 )
							 || ( pCheckItem->m_Vnum >= 2652 && pCheckItem->m_Vnum <= 2655 )
							 || ( pCheckItem->m_Vnum >= 2660 && pCheckItem->m_Vnum <= 2663 )
							 || ( pCheckItem->m_Vnum >= 2668 && pCheckItem->m_Vnum <= 2671 )
						   )
					{
						if( (pCheckItem->m_PlusNum+1) > NPC_UPGRADE_NEED_REGENT_LEVEL ) // +9�̻� ��ȭ�� ����Ʈ��...
						{
							++NeedItem[UPGRADE_NEED_REGENT_DIA];
						}
						else
						{
							++NeedItem[UPGRADE_NEED_DIA];
						}
					}

					//-- IDS_NPCUPGRADE_NEEDGEM : �ʿ� ����:
					m_NeedGemText.SetString( (char*)G_STRING(IDS_NPCUPGRADE_NEEDGEM), FALSE ); // ����� �ϴ� ������ ���� ������ Ʋ����� �ٸ� �޼����� ��ġ�� ���ؼ�...
					sprintf( strTemp, "(%d)", 1 ); // �ϴ��� ������ �ʿ䰹���� �ϳ��ϱ� ������ 1�� ����. ���� ������ �ٲ�� ��ġ��.
					m_NeedGemNumText.SetString( strTemp, FALSE ); // ����� �ϴ� ������ ���� ������ Ʋ����� �ٸ� �޼����� ��ġ�� ���ؼ�...

					if ( pCheckItem->m_Vnum >= 1797 && pCheckItem->m_Vnum <= 1816 ) // [2008/7/7 Theodoric]  ���� �� ��ȭ �߰�
						NeedLaim = 450000 * (pCheckItem->m_PlusNum+1);
					else if( pCheckItem->m_Vnum >= 1560 && pCheckItem->m_Vnum <= 1579
							 || ( TRUE == IS_2011_ITEM_ARMOR( pCheckItem->m_Vnum ) )
						   ) // 300���� �ű� �� �߰�.
					{
						if ( GET_TYPE(pCheckItem)==ITYPE_ARMOR )
						{
							NeedLaim = 450000 * (pCheckItem->m_PlusNum+1);
						}
						else
						{
							int	newMoney[15] = { 4500000,5000000,5500000,6000000,6500000,7000000,7500000,8000000,8500000,9000000,9500000,10000000,10500000,11000000,12000000 };
							NeedLaim = newMoney[pCheckItem->m_PlusNum];
						}
					}
					else if( (pCheckItem->m_Vnum >= 1190 && pCheckItem->m_Vnum <= 1229 ) || (pCheckItem->m_Vnum >= 1313 && pCheckItem->m_Vnum <= 1326) ) // ���� ����,��
						NeedLaim = 300000 * (pCheckItem->m_PlusNum+1);
					else if( ( pCheckItem->m_Vnum >= 2580 && pCheckItem->m_Vnum <= 2583 )
							 || ( pCheckItem->m_Vnum >= 2604 && pCheckItem->m_Vnum <= 2607 )
							 || ( pCheckItem->m_Vnum >= 2612 && pCheckItem->m_Vnum <= 2615 )
							 || ( pCheckItem->m_Vnum >= 2620 && pCheckItem->m_Vnum <= 2623 )
							 || ( pCheckItem->m_Vnum >= 2628 && pCheckItem->m_Vnum <= 2631 )
						   )
						NeedLaim = 300000 * (pCheckItem->m_PlusNum+1);
					else if( ( pCheckItem->m_Vnum >= 2636 && pCheckItem->m_Vnum <= 2639 )
							 || ( pCheckItem->m_Vnum >= 2644 && pCheckItem->m_Vnum <= 2647 )
							 || ( pCheckItem->m_Vnum >= 2652 && pCheckItem->m_Vnum <= 2655 )
							 || ( pCheckItem->m_Vnum >= 2660 && pCheckItem->m_Vnum <= 2663 )
							 || ( pCheckItem->m_Vnum >= 2668 && pCheckItem->m_Vnum <= 2671 )
						   )
						NeedLaim = 300000 * (pCheckItem->m_PlusNum+1);

					// ���� ��ȭ�� ����ȭ�ĸ� Ŭ���̾�Ʈ ���������� �ٲ��ش�.(���½ÿ� �׳� ����ȭ�� ������ �ø��� �ٲٰ� ��Ŷ�� �ٸ��� ������. ������ �� ����.)
					if( GET_TYPE(pCheckItem)==ITYPE_ARMOR )
					{
						m_iType = INTERFACE_TYPE_UPARMOR;
					}
					else // Ȥ�ö� ����,���⵵ �ƴҰ�쵵 �׳� �ϴ� ����� ���.(�ּ��� ������ �ʾƾ����� �ʰڴ°�?)
					{
						m_iType = INTERFACE_TYPE_UPWEAPON;
					}
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONVERSION:
		{
			// ������ �������̽��� �ʿ������� ���Ǻ��� �̹����� �ٲ㼭 ǥ��...
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON || GET_TYPE(pItem)==ITYPE_ARMOR ) // ����Ǵ� �� üũ.
				{
					++ItemCount;
					pCheckItem = pItem; // �����͸� ���. �������� ���������� �ϳ� ���̾��ٸ� �� �����͸� �̿��ؼ� �ʿ��Ѱ� ���Ѵ�.
				}

				// �� üũ�� ���� �ʴ´�.

				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // ����,���� 1���� �ƴϾ����� ����.
			{
				m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else if( pCheckItem->m_PlusNum != 15 ) // ����� +15 �����۸� �������� �����ϴ�.
			{
				m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else // ���������� ����,���� �ϳ����̸� �ʿ��� ���ӵ��� ���Ѵ�.
			{
				if( pCheckItem ) // �̰� �˻��ؾ��Ѵ�.
				{
					if( !CheckCanConversionItem(pCheckItem) ) // Ư�� �����۸� ��ȭ ����.
					{
						m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
						m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}

					// �׷����� �޼��� ������ �ٲ���ٸ� �ٽ� �ε�.
					if( m_iTextInfoIndex != 201 && m_iTextInfoIndex != 202 ) // ��� ��Ʈ ���¿����� �������� �־ TextInfo�� �ٲ��� �ʴ´�.(�κ�â Ŭ���� �������ش�.)
					{
						if( GET_ITEM_LEVEL(pCheckItem) == 195 || GET_ITEM_LEVEL(pCheckItem) == 200 ) // �ʿ����� 3Ÿ��. (���̺긮���� ���� ����� 200���� �����̶� �߰��� �˻�)
						{
							m_iTextInfoIndex = 203; // 0,1,2�� ��Ʈ��...
						}
						else if( GET_ITEM_LEVEL(pCheckItem) == 210 ) // �ʿ����� 4Ÿ��.
						{
							m_iTextInfoIndex = 204; // 0,1,2�� ��Ʈ��...
						}
						else if( GET_ITEM_LEVEL(pCheckItem) == 261 ) // �ʿ����� 5Ÿ��.
						{
							m_iTextInfoIndex = 205; // 0,1,2�� ��Ʈ��...
						}
					}

					NeedLaim = 10000000; // ����� ���� 1000��.
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONFUSION:
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON || GET_TYPE(pItem)==ITYPE_ARMOR ) // ����Ǵ� �� üũ.
				{
					++ItemCount;
					pCheckItem = pItem; // �����͸� ���. �������� ���������� �ϳ� ���̾��ٸ� �� �����͸� �̿��ؼ� �ʿ��Ѱ� ���Ѵ�.
				}

				// �� üũ�� ���� �ʴ´�.

				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // ����,���� 1���� �ƴϾ����� ����.
			{
				m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
				//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else // ���������� ����,���� �ϳ����̸� �ʿ��� ���ӵ��� ���Ѵ�.
			{
				if( pCheckItem ) // �̰� �˻��ؾ��Ѵ�.
				{
					if( !CheckIsConversionItem(pCheckItem,0,2) ) // Ư�� ������(�������� �༮ ��)�� ��ȭ ����.
					{
						m_NeedGemText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
						m_NeedGemNumText.SetString( "", TRUE ); // �̷��� �ϸ� �Ⱥ��δ�.
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}

					NeedItem[UPGRADE_NEED_DIA] = (pCheckItem->m_PlusNum-1) / 4;

					if( NeedItem[UPGRADE_NEED_DIA] < 0 )
						NeedItem[UPGRADE_NEED_DIA] = 0;

					//-- IDS_NPCUPGRADE_NEEDGEM : �ʿ� ����:
					m_NeedGemText.SetString( (char*)G_STRING(IDS_NPCUPGRADE_NEEDGEM), FALSE ); // ����� �ϴ� ������ ���� ������ Ʋ����� �ٸ� �޼����� ��ġ�� ���ؼ�...
					sprintf( strTemp, "(%d)", NeedItem[UPGRADE_NEED_DIA] ); // �����
					m_NeedGemNumText.SetString( strTemp, FALSE ); // ����� �ϴ� ������ ���� ������ Ʋ����� �ٸ� �޼����� ��ġ�� ���ؼ�...

					int TempInt = pCheckItem->m_PlusNum-5;
					if( TempInt < 0 )
						TempInt = 0;

					NeedLaim = 2000000 + ( TempInt*400000 );
				}
			}
		}
		break;


	case INTERFACE_TYPE_PROCESS:
		{
			while (pItem)
			{
				if( GET_TYPE(pItem) == ITYPE_ARMOR ) // �� üũ.
				{
					++ItemCount;
					pCheckItem = pItem;
				}

				// �� üũ�� ���� �ʴ´�.
				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // ���ƴϾ����� ����.
			{
				m_NeedGemText.SetString( "", TRUE );
				m_NeedGemNumText.SetString( "", TRUE );
				//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else
			{
				if( pCheckItem )
				{
					if( !(GET_ATT_FLAG(pCheckItem) & IATT_REFINE) )
					{
						m_NeedGemText.SetString( "", TRUE );
						m_NeedGemNumText.SetString( "", TRUE );
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}
				}
			}

			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_ETC) // ���ü� üũ.
				{
					pCheckItem2 = pItem; // �����͸� ���. �������� ���������� �ϳ� ���̾��ٸ� �� �����͸� �̿��ؼ� �ʿ��Ѱ� ���Ѵ�.
					++ItemCount2;
				}
				pItem = pItem->m_Next;
			}

			if( ItemCount2 == 0 ) // ���ü��ƴϾ����� ����.
			{
				m_NeedGemText.SetString( "", TRUE );
				m_NeedGemNumText.SetString( "", TRUE );
				//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}

			if( pCheckItem2 )
			{
				if( pCheckItem2->m_Vnum == 1362 )
					special_level = (pCheckItem->m_Special2 & 0x1C00) >> 10;
				else if( pCheckItem2->m_Vnum == 1363 )
					special_level1 = (pCheckItem->m_Special2 & 0xE000) >> 13;
				else if( pCheckItem2->m_Vnum == 1364 )
					special_level2 = (pCheckItem->m_Special2 & 0x70000) >> 16;
				else if( pCheckItem2->m_Vnum == 1365 )
					special_level3 = (pCheckItem->m_Special2 & 0x380000) >> 19;
				else if( pCheckItem2->m_Vnum == 1366 )
					special_level4 = (pCheckItem->m_Special2 & 0x1C00000) >> 22;
			}

			if( m_iTextInfoIndex != 201 && m_iTextInfoIndex != 202 ) // ��� ��Ʈ ���¿����� �������� �־ TextInfo�� �ٲ��� �ʴ´�.(�κ�â Ŭ���� �������ش�.)
			{
				if( special_level == 0 || special_level1 == 0 || special_level2 == 0 || special_level3 == 0 || special_level4 == 0)
				{
					m_iTextInfoIndex = 203;
					NeedLaim = 261000;
				}
				else if( special_level == 1 || special_level1 == 1 || special_level2 == 1 || special_level3 == 1 || special_level4 == 1 )
				{
					m_iTextInfoIndex = 204;
					NeedLaim = 522000;
				}
				else if( special_level == 2 || special_level1 == 2 || special_level2 == 2 || special_level3 == 2 || special_level4 == 2 )
				{
					m_iTextInfoIndex = 205;
					NeedLaim = 783000;
				}
				else if( special_level == 3 || special_level1 == 3 || special_level2 == 3 || special_level3 == 3 || special_level4 == 3 )
				{
					m_iTextInfoIndex = 206;
					NeedLaim = 1044000;
				}
				else if( special_level == 4 || special_level1 == 4 || special_level2 == 4 || special_level3 == 4 || special_level4 == 4 )
				{
					m_iTextInfoIndex = 207;
					NeedLaim = 1305000;
				}
				else if( special_level >= 5 || special_level1 >= 5 || special_level2 >= 5 || special_level3 >= 5 || special_level4 >= 5 )
				{
					//-- IDS_MAX_OPTION : ���� �ɼ��� �ִ�ġ�Դϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_MAX_OPTION), TYPE_NOR_OK, 1 );
					return -1;
				}
				/*else if( special_level == 5 || special_level1 == 5 || special_level2 == 5 || special_level3 == 5 || special_level4 == 5 )
				{
					m_iTextInfoIndex = 207;
					NeedLaim = 1500000;
				}
				else if( special_level == 6 || special_level1 == 6 || special_level2 == 6 || special_level3 == 6 || special_level4 == 6 )
				{
					//-- IDS_MAX_OPTION : ���� �ɼ��� �ִ�ġ�Դϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_MAX_OPTION), TYPE_NOR_OK, 1 );
					return -1;
				}*/
			}
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
		{
			CItem * targetItem = NULL;
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON )
				{
					if( !ChechIs261LvWeponItem(pItem) || pItem->m_PlusNum < 10 || pItem->m_LimitTime )
					{
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
						return -1;
					}
					if( targetItem == NULL )
						targetItem = pItem;
				}
				else if (GET_TYPE(pItem)==ITYPE_ARMOR )
				{
					//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
					m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
					return -1;
				}
				pItem = pItem->m_Next;
			}

			NeedItem[UPGRADE_NEED_DIA] = 3;
			NeedItem[UPGRADE_NEED_REGENT_DIA] = 8;

			// �Ϻ��� ������ Ʋ���� ��û
			if( g_dwClientCountry == CTRY_JPN ) NeedLaim = 50000000;
			else								NeedLaim = 10000000;

			if( targetItem != NULL)
			{
				if( targetItem->m_PlusNum > 10)
					m_iTextInfoIndex = 54;
				else
					m_iTextInfoIndex = 53;
			}

			targetItem = NULL;
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
		{
			int GMRDia = 0;
			int Warranty = 0;

			pCheckItem = NULL;
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON )
				{
					pCheckItem = pItem;

					if( !( (pItem->m_Vnum >= 2049 && pItem->m_Vnum <= 2055)/* || (pItem->m_Vnum >= 2743 && pItem->m_Vnum <= 2749 )*/
							//|| (pItem->m_Vnum >= 2939 && pItem->m_Vnum <= 2941)
							|| ( TRUE == IS_2011_ITEM_WEAPON( pItem->m_Vnum ) )
						 )
							|| pItem->m_LimitTime )
					{
						ResetText();
						return -1;
					}

					if( pItem->m_PlusNum >= 15)
					{
						ResetText();
						return -1;
					}
				}
				else if( pItem->m_Vnum == 1967) // Great Magic Regent Diamond
				{
					GMRDia ++;
				}
				else if( pItem->m_Vnum == 1068 ) // Regent Diamond
				{		}
				else if( pItem->m_Vnum == 1739 || pItem->m_Vnum == 1740 ) // warranty item
				{
					if(++Warranty > 1)
					{
						g_pNk2DFrame->InsertPopup("Only one warranty item allowed", TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else
				{
					ResetText();
					return -1;
				}

				pItem = pItem->m_Next;
			}

			if( pCheckItem )
			{
				if( GMRDia == 0)
				{
					// Print a warning message.
					if( g_SvrType == ST_ADULT_ONLY)
					{
						if( GET_TYPE(pCheckItem) == ITYPE_WEAPON )
						{
							switch( pCheckItem->m_PlusNum )
							{
							case 3:
							case 7:
							case 11:
								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG_01), TYPE_NOR_OK);
								break;
								break;
							}
						}
					}
					else
					{
						switch( pCheckItem->m_PlusNum )
						{
						case 3:
						case 7:
							// IDS_UPGRADE_MSG1 Can be initialized in case of upgrade failure.
							g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG1), TYPE_NOR_OK, 1 );
							break;
						case 11:
							// IDS_UPGRADE_MSG2 Failure to upgrade can lead to a drop.
							g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG2), TYPE_NOR_OK, 1 );
							break;
						}
					}
				}

				// Japan asks for lime wrong
				if( g_dwClientCountry == CTRY_JPN )
				{
					switch( pCheckItem->m_PlusNum )
					{
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
						NeedLaim = 3000000 + ( pCheckItem->m_PlusNum * 500000 );
						break;
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
						NeedLaim = 6000000 + ( (pCheckItem->m_PlusNum-5) * 1000000 );
						break;
					default:
						NeedLaim = 12000000 + ( (pCheckItem->m_PlusNum-10) * 2000000 );
						break;
					}
				}
				else
				{
					// There is no problem, so let's calculate the lime..
					switch( pCheckItem->m_PlusNum )
					{
					case 13:
					case 14:
						if ( TRUE == IS_2011_ITEM_WEAPON( pCheckItem->m_Vnum ) )
						{
							int	newMoney[15] = { 4500000,5000000,5500000,6000000,6500000,7000000,7500000,8000000,8500000,9000000,9500000,10000000,10500000,11000000,12000000 };
							NeedLaim = newMoney[pCheckItem->m_PlusNum];
						}
						else
						{
							NeedLaim = 11000000 + ((pCheckItem->m_PlusNum-12)*1000000);
						}
						break;
					default:
						NeedLaim = 4500000 + (pCheckItem->m_PlusNum * 500000);
						break;
					}
				}


				NeedItem[UPGRADE_NEED_REGENT_DIA]++;
				m_NeedGemText.SetString( (char*)G_STRING(IDS_NPCUPGRADE_NEEDGEM), FALSE );
				sprintf( strTemp, "(%d)", 1 );
				m_NeedGemNumText.SetString( strTemp, FALSE );
			}
		}
		break;

	case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
		{
			if( GET_TYPE(pItem) == ITYPE_WEAPON )
			{
				if( CheckIsGodItem(pItem) && pItem->m_PlusNum >= 10 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), TYPE_NOR_OK, 1 );
					m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), FALSE );
					return -1;
				}
			}

			if( NULL == LHIS_ENCHANT() )
			{
				NeedLaim = 0;
				return -1;
			}

			CItem * current_item = NULL;
			int upgrade_type = LHIS_ENCHANT()->CheckUpgrade_Weapon(&current_item);
			if( upgrade_type == -1 || !current_item )
			{
				NeedLaim = 0;
				return -1;
			}

			if( NULL == LHIS_ENCHANT_EX() )
			{
				NeedLaim = 0;
				return -1;
			}

			const LHParam_EnchantItem_Ex::sData * param_ex = LHIS_ENCHANT_EX()->GetParam(upgrade_type, current_item->m_PlusNum);

			if( !param_ex )
				return -1;

			CopyMemory( m_need_gem, param_ex->m_gem, sizeof(m_need_gem) );
			NeedLaim = LHIS_ENCHANT_EX()->GetNeedLaim(upgrade_type,current_item->m_PlusNum);
			m_NeedMoneyText2.SetString( NeedLaim, TRUE );
			return -1;
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER:
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON )
				{
					if( !(pItem->m_Vnum >= 2049 && pItem->m_Vnum <= 2055)
							//|| ( pItem->m_Vnum >= 2939 && pItem->m_Vnum <= 2941 )
							|| ( TRUE == IS_2011_ITEM_WEAPON( pItem->m_Vnum ) )
							|| pItem->m_LimitTime
							|| pItem->m_PlusNum != 15 )
					{
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
						return -1;
					}
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR )
				{
					if( !(pItem->m_Vnum >= 2942 && pItem->m_Vnum <= 2953)
							|| pItem->m_LimitTime
							|| pItem->m_PlusNum != 15 )
					{
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
						return -1;
					}
				}

				pItem = pItem->m_Next;
			}

			NeedItem[UPGRADE_NEED_REGENT_DIA]++;
			//		m_NeedGemText.SetString("���� ���� 1set");
			// �Ϻ��� ������ Ʋ���� ��û
			NeedLaim = NeedLaim_GodPower;
		}
		break;


	case INTERFACE_TYPE_ITEMMAKE:		
		{
			if ( false == LHIS_MAKE()->CheckNeedItem( NeedLaim ) )
			{
				sprintf( strTemp2, "%d", NeedLaim );
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) );
				m_NeedMoneyText1.SetString( strTemp, FALSE );
				m_NeedMoneyText2.SetString( NeedLaim, TRUE );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_BUFFMAKE_MAKE:
		{
			if ( false == LHIS_MAKE()->CheckNeedItemBuff( NeedLaim ) )
			{
				sprintf( strTemp2, "%d", NeedLaim );
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) );
				m_NeedMoneyText1.SetString( strTemp, FALSE );
				m_NeedMoneyText2.SetString( NeedLaim, TRUE );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_BUFFMAKE_GATCHA:
		{
			NeedLaim = 1000000;
			sprintf( strTemp2, "%d", NeedLaim );
			sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) );
			m_NeedMoneyText1.SetString( strTemp, FALSE );
			m_NeedMoneyText2.SetString( NeedLaim, TRUE );
		}
		break;	

	case INTERFACE_TYPE_ITEMSEPARATE:
		{
			if ( false == LHIS_SEPERATE()->CheckNeedItem() )
			{
				sprintf( strTemp2, "%d", NeedLaim );
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) ); // �޸��� �ֱ����� �ӽ÷� m_NeedMoneyText1�� ���۸� ���.
				m_NeedMoneyText1.SetString( strTemp, FALSE );
				m_NeedMoneyText2.SetString( NeedLaim, TRUE );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_GMAGICSTONE:
		{
			if ( false == LHIS_GMAGICSTONE()->CheckNeedItem( NeedLaim ) )
			{
				sprintf( strTemp2, "%d", NeedLaim );
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) ); // �޸��� �ֱ����� �ӽ÷� m_NeedMoneyText1�� ���۸� ���.
				m_NeedMoneyText1.SetString( strTemp, FALSE );
				m_NeedMoneyText2.SetString( NeedLaim, TRUE );
				return -1;
			}
		}
		break;


	}

	sprintf( strTemp2, "%d", NeedLaim );
	sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) ); // �޸��� �ֱ����� �ӽ÷� m_NeedMoneyText1�� ���۸� ���.
	m_NeedMoneyText1.SetString( strTemp, FALSE );
	m_NeedMoneyText2.SetString( NeedLaim, TRUE );

	return 1;
}

int CControlUpgrade::CheckSlotPure() // ���׷��̵带 �õ��ϴµ� �������� �������� ���ԵǾ��ִ� ���� üũ.
{
	CItem* pItem = NULL;
	CItem* pCheckItem = NULL;
	int GemCount = 0; // �� ���� ����
	int special_level = -1, special_level1 = -1, special_level2 = -1, special_level3 = -1, special_level4 = -1;

	if( !g_pRoh )
		return -1;

	pItem = g_pRoh->m_ExgInven;

	if(!pItem)
		return -1;

	if( NeedLaim > g_pRoh->m_Money )
	{
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
		return -1;
	}

	switch( m_iType )
	{
	case INTERFACE_TYPE_UPARMOR_400:
		break;
	case INTERFACE_TYPE_UPARMOR:
	case INTERFACE_TYPE_UPWEAPON: // ��ȭ �������̽��� ���. ������ üũ ���ָ鼭 �ʿ� �������� ������ش�.
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON
						&& ( pItem->m_Vnum != 228 && pItem->m_Vnum != 1966 )
						&& ( pItem->m_Vnum != 1068 && pItem->m_Vnum != 1967 )
						&& ( pItem->m_Vnum != 1739 && pItem->m_Vnum != 1740 )
						&& ( pItem->m_Vnum != 1832 )
				  ) // ����� ��,����,���̾�,����Ʈ ���̾� �̿ܿ��� ��������.(�Ŀ� ������ ���� iType �÷��׷� �����ؼ� üũ����.)
				{
					// ������ �������� �� ������...
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // ���϶�...���⵵...
				{
					// by evilkiki 2010.03.16 ����/�� �����ؼ��� �ð��� ����
					if( pItem->m_bTimeLimit ) // �ð��� ������ �Ұ�ó��. �߰�. (06-02-24 ����)
					{
						//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
						return -1;
					}

					if( ChechIs261LvWeponItem(pItem) && pItem->m_PlusNum >= 10)
					{
						//-- IDS_NPCUPGRADE_MAXLEVEL : ���̻� ��ȭ�� �Ұ����� �������Դϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), TYPE_NOR_OK, 1 );
						return -1;
					}
					else if( !CheckCanNpcUpgradeItem(pItem) ) // Ư�� �����۸� ��ȭ ����.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( pItem->m_Vnum == 228 || pItem->m_Vnum == 1068
						|| pItem->m_Vnum == 1966 || pItem->m_Vnum == 1967
				  ) // ���� ���� ����.
				{
					++GemCount;
				}

				pItem = pItem->m_Next;
			}

			if( GemCount != 1 ) // ������ �����䱸���� ������ 1�̴�.
			{
				//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}

			if( CheckUpgradeNeed() == -1 ) // �� �Լ����� �� ������ 1�������� üũ���ִ� ��ɵ� �ִ�.
			{
				// ���� 2���̻��̸�...
				return -1;
			}

			// �䱸 ���� �������Ƿ� ���� �°� ���ִ��� �˻�.
			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON ) // ���� ���� �˻�. ���⵵...
				{
					if( NeedItem[UPGRADE_NEED_DIA]
							&& ( pItem->m_Vnum != 228 && pItem->m_Vnum != 1966 )
					  ) // ���̾Ƹ� �䱸�ϴµ� ���̾� �̿��� ���� ������...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					else if( NeedItem[UPGRADE_NEED_REGENT_DIA]
							 && ( pItem->m_Vnum != 1068 && pItem->m_Vnum != 1967 )
						   ) // ����Ʈ ���̾Ƹ� �䱸�ϴµ� �� �̿��� ���� ������...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else // ���� �������� �˻����ش�.
				{
					if( pItem->m_MaxUpgradeEndurance && pItem->m_UpgradeEndurance <= 0 ) // ��ȭ �������� �� ����������... // 07-04-17 �ƽ� ġ�� �������� ������ �˻��ϵ��� ����.
					{
						//-- IDS_NPCUPGRADE_EDRLOW : ��ȭ �������� �����մϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_EDRLOW), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				pItem = pItem->m_Next;
			}
		}
		break;

	case INTERFACE_TYPE_CONVERSION: // ������ �������̽��� ���
		{
			// �ʿ� ���� �÷���.
			int Amethyst = 0, Topaz = 0, Opal = 0, Sapphire = 0, Ruby = 0, Emerald = 0, Diamond = 0, RegentDia = 0, Platinum = 0;

			// �ʿ� �� �˻�.
			while (pItem)
			{
				if( pItem->m_bTimeLimit ) // �ð��� ������ �Ұ�ó��. �߰�. (06-02-24 ����)
				{
					//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�..
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON && GET_TYPE(pItem)!=IETC_GEM && GET_TYPE(pItem)!=IETC_GEMCHIP ) // ����� �� ������ �������� �ִٸ�...(Ĩ�� �����÷�Ƽ������. ���� �÷�Ƽ���� ������ Ĩ�̴�.)
				{
					// ������ �������� �� ������...
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // ���϶�...�Ǵ� �����϶�.
				{
					if( !CheckCanConversionItem(pItem) ) // Ư�� �����۸� ��ȭ ����.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( g_dwLangType == LANG_KOR)
				{
					switch(pItem->m_Vnum)
					{
					case 222:
					case 1960:	// ���� �ڼ���, �׷���Ʈ
						Amethyst++;
						break;
					case 223:
					case 1961:	// ���� ������, �׷���Ʈ
						Topaz++;
						break;
					case 224:
					case 1962:	// ���� ����, �׷���Ʈ
						Opal++;
						break;
					case 225:
					case 1963:	// ���� �����̾�, �׷���Ʈ
						Sapphire++;
						break;
					case 226:
					case 1964:	// ���� ���, �׷���Ʈ
						Ruby++;
						break;
					case 227:
					case 1965:	// ���� ���޶���, �׷���Ʈ
						Emerald++;
						break;
					case 228:
					case 1966:	// ���� ���̾Ƹ��, �׷���Ʈ
						Diamond++;
						break;
					case 1068:
					case 1967:	// ���� ����Ʈ ���̾�, �׷���Ʈ
						RegentDia++;
						break;
					case 978:				// ���� �÷�Ƽ��
						Platinum++;
						break;
					}
				}
				else
				{
					if( pItem->m_Vnum == 222 ) // �����ڼ���
						++Amethyst;
					if( pItem->m_Vnum == 223 ) // ����������
						++Topaz;
					if( pItem->m_Vnum == 224 ) // ��������
						++Opal;
					if( pItem->m_Vnum == 225 ) // ���������̾�
						++Sapphire;
					if( pItem->m_Vnum == 226 ) // �������
						++Ruby;
					if( pItem->m_Vnum == 227 ) // �������޶���
						++Emerald;
					if( pItem->m_Vnum == 228 ) // �������̾Ƹ��
						++Diamond;
					if( pItem->m_Vnum == 1068 ) // ��������Ʈ���̾Ƹ��
						++RegentDia;
					if( pItem->m_Vnum == 978 ) // �����÷�Ƽ��
						++Platinum;
				}

				pItem = pItem->m_Next;
			}

			if( CheckUpgradeNeed() == -1 ) // �� �Լ����� ���� ���� ������ 1�������� üũ���ִ� ��ɵ� �ִ�.
			{
				// ���� ���Ⱑ 2���̻��̸�...
				return -1;
			}

			// ���������� �´����� üũ. (���� CheckUpgradeNeed�� ���� ��ġ�� ������ ���⳪ �� �������� �ϳ����̶� ���Ű� �Ǳ⶧��.)
			pItem = g_pRoh->m_ExgInven; // �ٽ� ó������ �˻��غ�.

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON || GET_TYPE(pItem)==ITYPE_ARMOR ) // ����Ǵ� �� üũ.
				{
					break; // ������ �ϳ����̴� �ٷ� Ż��.
				}

				pItem = pItem->m_Next;
			}

			if( pItem ) // Ȯ�� ���.
			{
				// ������ ���� ��޺� �ʿ������� Ȯ���Ѵ�.
				if( GET_ITEM_LEVEL(pItem) == 195 ) // �ʿ����� 3Ÿ��.
				{
					if( Amethyst != 1 || Topaz != 1 || Opal != 1 || Sapphire != 1
							|| Ruby != 1 || Emerald != 1 || Diamond != 10 || RegentDia != 0 || Platinum != 2 )
					{
						//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else if( GET_ITEM_LEVEL(pItem) == 210 ) // �ʿ����� 4Ÿ��.
				{
					if( Amethyst != 1 || Topaz != 1 || Opal != 1 || Sapphire != 1
							|| Ruby != 1 || Emerald != 1 || Diamond != 5 || RegentDia != 0  || Platinum != 7 )
					{
						//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else if( GET_ITEM_LEVEL(pItem) == 261 ) // �ʿ����� 5Ÿ��.
				{
					if( Amethyst != 1 || Topaz != 1 || Opal != 1 || Sapphire != 1
							|| Ruby != 1 || Emerald != 1 || Diamond != 1 || RegentDia != 1  || Platinum != 10 )
					{
						//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONFUSION: // ���� �ٽ� ������
		{
			while (pItem)
			{
				if( pItem->m_bTimeLimit ) // �ð��� ������ �Ұ�ó��. �߰�. (06-02-24 ����)
				{
					//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON&& pItem->m_Vnum != 228) // ����� ��,����,���̾� �̿ܿ��� ��������.(�Ŀ� ������ ���� iType �÷��׷� �����ؼ� üũ����.)
				{
					// ������ �������� �� ������...
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // ���϶�...���⵵...
				{
					if( !CheckIsConversionItem(pItem,0,2) ) // Ư�� ������(������ �� �����۸�)�� ��ȭ ����.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( pItem->m_Vnum == 228 ) // ���� ���� ����.
					++GemCount;

				pItem = pItem->m_Next;
			}

			if( GemCount != NeedItem[UPGRADE_NEED_DIA] )
			{
				//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}

			if( CheckUpgradeNeed() == -1 ) // �� �Լ����� �� ������ 1�������� üũ���ִ� ��ɵ� �ִ�.
			{
				// ���� 2���̻��̸�...
				return -1;
			}

			// �䱸 ���� �������Ƿ� ���� �°� ���ִ��� �˻�.
			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON ) // ���� ���� �˻�. ���⵵...
				{
					if( NeedItem[UPGRADE_NEED_DIA] && pItem->m_Vnum != 228) // ���̾Ƹ� �䱸�ϴµ� ���̾� �̿��� ���� ������...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				pItem = pItem->m_Next;
			}
		}
		break;

	case INTERFACE_TYPE_PROCESS: // ����
		{
			// �ʿ� ���� �÷���.
			int Amethyst = 0, Topaz = 0, Opal = 0, Sapphire = 0, Ruby = 0,
				Emerald = 0, Diamond = 0, RegentDia = 0, Platinum = 0,
				ProcessFire = 0, ProcessWater = 0, ProcessLight = 0, ProcessEarth = 0, ProcessWind = 0;

			// �ʿ� �� �˻�.
			while (pItem)
			{
				if( pItem->m_bTimeLimit ) // �ð��� ������ �Ұ�ó��. �߰�. (06-02-24 ����)
				{
					//-- IDS_LIMITED_CANT : ���ѵ� �������� �Ұ����մϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=IETC_GEM && GET_TYPE(pItem)!=IETC_GEMCHIP ) // ����� �� ������ �������� �ִٸ�...(Ĩ�� �����÷�Ƽ������. ���� �÷�Ƽ���� ������ Ĩ�̴�.)
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem) == ITYPE_ARMOR) // ���϶�
				{
					if( !(GET_ATT_FLAG(pItem) & IATT_REFINE) ) // Ư�� �����۸� ��ȭ ����.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( g_dwLangType == LANG_KOR )
				{
					switch(pItem->m_Vnum)
					{
					case 222:
					case 1960:	// ���� �ڼ���, �׷���Ʈ
						Amethyst++;
						break;
					case 223:
					case 1961:	// ���� ������, �׷���Ʈ
						Topaz++;
						break;
					case 224:
					case 1962:	// ���� ����, �׷���Ʈ
						Opal++;
						break;
					case 225:
					case 1963:	// ���� �����̾�, �׷���Ʈ
						Sapphire++;
						break;
					case 226:
					case 1964:	// ���� ���, �׷���Ʈ
						Ruby++;
						break;
					case 227:
					case 1965:	// ���� ���޶���, �׷���Ʈ
						Emerald++;
						break;
					case 228:
					case 1966:	// ���� ���̾Ƹ��, �׷���Ʈ
						Diamond++;
						break;
					case 1068:
					case 1967:	// ���� ����Ʈ ���̾�, �׷���Ʈ
						RegentDia++;
						break;
					case 978:				// ���� �÷�Ƽ��
						Platinum++;
						break;
					}
				}
				else
				{
					if( pItem->m_Vnum == 222 ) // �����ڼ���
						++Amethyst;
					if( pItem->m_Vnum == 223 ) // ����������
						++Topaz;
					if( pItem->m_Vnum == 224 ) // ��������
						++Opal;
					if( pItem->m_Vnum == 225 ) // ���������̾�
						++Sapphire;
					if( pItem->m_Vnum == 226 ) // �������
						++Ruby;
					if( pItem->m_Vnum == 227 ) // �������޶���
						++Emerald;
					if( pItem->m_Vnum == 228 ) // �������̾Ƹ��
						++Diamond;
					if( pItem->m_Vnum == 1068 ) // ��������Ʈ���̾Ƹ��
						++RegentDia;
					if( pItem->m_Vnum == 978 ) // �����÷�Ƽ��
						++Platinum;
				}
				pItem = pItem->m_Next;
			}

			if( CheckUpgradeNeed() == -1 ) // �� �Լ����� ���� ���� ������ 1�������� üũ���ִ� ��ɵ� �ִ�.
			{
				// ���� ���Ⱑ 2���̻��̸�...
				return -1;
			}

			// ���������� �´����� üũ. (���� CheckUpgradeNeed�� ���� ��ġ�� ������ ���⳪ �� �������� �ϳ����̶� ���Ű� �Ǳ⶧��.)
			if( g_pRoh )
				pItem = g_pRoh->m_ExgInven; // �ٽ� ó������ �˻��غ�.

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_ARMOR ) // ����Ǵ� �� üũ.
				{
					break; // ������ �ϳ����̴� �ٷ� Ż��.
				}

				pItem = pItem->m_Next;
			}

			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_ETC) // ���ü� üũ.
				{
					pCheckItem = pItem; // �����͸� ���. �������� ���������� �ϳ� ���̾��ٸ� �� �����͸� �̿��ؼ� �ʿ��Ѱ� ���Ѵ�.
				}
				pItem = pItem->m_Next;
			}

			if( pCheckItem )
			{
				if( pCheckItem->m_Vnum == 1362 )
					special_level = (pCheckItem->m_Special2 & 0x1C00) >> 10;
				else if( pCheckItem->m_Vnum == 1363 )
					special_level1 = (pCheckItem->m_Special2 & 0xE000) >> 13;
				else if( pCheckItem->m_Vnum == 1364 )
					special_level2 = (pCheckItem->m_Special2 & 0x70000) >> 16;
				else if( pCheckItem->m_Vnum == 1365 )
					special_level3 = (pCheckItem->m_Special2 & 0x380000) >> 19;
				else if( pCheckItem->m_Vnum == 1366 )
					special_level4 = (pCheckItem->m_Special2 & 0x1C00000) >> 22;
			}
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
		{
			int Gem[7] = {0,};
			int MRDia = 0;
			int GMRDia = 0;
			int	GMDia = 0;
			int Item = 0;
			int offset = 0;
			int count = 0;
			int	trash = 0;
			CItem* pCheckItem = NULL;

			while (pItem)
			{
				// ���� �������� üũ
				offset = pItem->m_Vnum - 222;
				if( offset >= 0 && offset < 7 ) // 222(���� �ڼ���) ~ 228(���� ���̾�)
				{
					Gem[offset] ++;
				}
				else if( ChechIs261LvWeponItem(pItem) )
				{
					if( pItem->m_PlusNum < 10 || pItem->m_LimitTime != 0 )
					{
						//-- IDS_NPCUPGRADE_PLZUPITEM : �ش� �������� �÷��ּ���.
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), TYPE_NOR_OK, 1 );
						return -1;
					}
					count++;
					pCheckItem = pItem;
				}
				else if( pItem->m_Vnum == 1068 )// ���� ����Ʈ ���̾�
				{
					MRDia++;
				}
				else if( pItem->m_Vnum == 1966 )// �׷���Ʈ ���� ���̾�.
				{
					GMDia++;
				}
				else if( pItem->m_Vnum == 1967 )// �׷���Ʈ ���� ����Ʈ ���̾�.
				{
					GMRDia++;
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				pItem = pItem->m_Next;
			}

			int totalGems = 0;

			if( count == 1 )
			{
				if( pCheckItem->m_PlusNum > 10 )
				{
					totalGems = Gem[0] + Gem[1] + Gem[2] + Gem[3] + Gem[4] + Gem [5] + Gem[6] + GMDia + GMRDia + MRDia ;
					if( totalGems != 0 )
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else if( pCheckItem->m_PlusNum == 10 )
				{
					totalGems = Gem[0] + Gem[1] + Gem[2] + Gem[3] + Gem[4] + Gem [5] + Gem[6];
					if( !(totalGems == 7 && GMDia == 1 && GMRDia == 1 && MRDia == 3 && count == 1) )
					{
						//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
			}
			else if( count > 1)
			{
				//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
			else
			{
				//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
		{
			//			int MRDia = 0;
			//			int GMRDia = 0;
			int Dia = 0;
			int count = 0;
			int	trash = 0;

			CItem *pItemTemp = NULL;

			while (pItem)
			{
				if( ( pItem->m_Vnum >= 2049 && pItem->m_Vnum <= 2055 )
						|| ( TRUE == IS_2011_ITEM_WEAPON( pItem->m_Vnum ) )
						/*|| ( pItem->m_Vnum >= 2743 && pItem->m_Vnum <= 2749 )*/ )
				{
					if( pItem->m_PlusNum == 15 )
					{
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), FALSE );
						return -1;
					}

					if( GET_TYPE(pItem) == ITYPE_WEAPON  )
					{
						if( pItem->m_LimitTime != 0  )
						{
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
							return -1;
						}
					}

					pItemTemp = pItem;
					count++;
				}
				else if( pItem->m_Vnum == 1068 || pItem->m_Vnum == 1967 ) // ���߿� �ϳ��� ������
				{
					Dia++;
				}
				else if( pItem->m_Vnum == 1739 || pItem->m_Vnum == 1740 )
				{
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				pItem = pItem->m_Next;
			}

			if( !(Dia == 1 && count == 1) )
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
		{
			if( NULL == LHIS_ENCHANT() )
				return -1;

			CItem * current_item = NULL;
			int upgrade_type = LHIS_ENCHANT()->CheckUpgrade(&current_item);
			if( upgrade_type == -1 || !current_item )
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}

			LHParam_EnchantItem_Ex * enchant_ex_system = LHIS_ENCHANT_EX();
			if( NULL == LHIS_ENCHANT_EX() )
			{
				return -1;
			}
			if( false == LHIS_ENCHANT_EX()->CheckGem(upgrade_type, current_item->m_PlusNum) )
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
			if( false == LHIS_ENCHANT_EX()->CheckEtc(upgrade_type, current_item->m_PlusNum) )
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER:
		{
			int count = 0;
			int	trash = 0;

			while (pItem)
			{
				// ���� �������� üũ
				if( ( pItem->m_Vnum >= 2049 && pItem->m_Vnum <= 2055 )
						//|| ( pItem->m_Vnum >= 2939 && pItem->m_Vnum <= 2941 )
						//|| ( pItem->m_Vnum >= 2942 && pItem->m_Vnum <= 2953 )
						|| ( TRUE == IS_2011_ITEM_WEAPON( pItem->m_Vnum ) )

				  )
				{
					if( pItem->m_PlusNum != 15 || pItem->m_LimitTime != 0 )
					{
						//-- IDS_NPCUPGRADE_MAXLEVEL : ���̻� ��ȭ�� �Ұ����� �������Դϴ�.
						//m_NeedMoneyText1.SetString( "��ȯ �� �� ���� �������Դϴ�.", FALSE );

						//-- IDS_UPGRADE_GODMSG : ��ȯ �� �� ���� �������Դϴ�.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_UPGRADE_GODMSG), FALSE );
						return -1;
					}
					count++;
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				pItem = pItem->m_Next;
			}

			if( count != 1)
			{
				//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;


	case INTERFACE_TYPE_ITEMMAKE:	
		{
			__int64 nNeedLaim = 0;
			if ( false == LHIS_MAKE()->CheckNeedItem( nNeedLaim ) )
			{
				//-- IDS_QUEST_ITEM_NOT_MATCH : �ٸ� ������ �������� �����ֽ��ϴ�.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
				//g_pNk2DFrame->InsertPopup( "������ ���ۿ� �ʿ��� ��Ḧ Ȯ���ϼ���.", TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;	

	case INTERFACE_TYPE_ITEMSEPARATE:
		{
			if ( false == LHIS_SEPERATE()->CheckNeedItem() )
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
				//g_pNk2DFrame->InsertPopup( "�������� ������ �� �����ϴ�.", TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_GMAGICSTONE:
		{
			__int64 nNeedLaim = 0;
			if ( false == LHIS_GMAGICSTONE()->CheckNeedItem( nNeedLaim ) )
			{
				//-- IDS_CHECK_ITEM_COUNT : ������ ������ Ȯ�����ּ���.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;


	}
	return 1;
}

BOOL CControlUpgrade::CheckBrokenItem()
{
	if( m_iType == INTERFACE_TYPE_GOD_WEAPON_UPGRADE )
	{
		if( NULL == LHIS_ENCHANT() )
			return FALSE;

		CItem * current_item = NULL;
		int upgrade_type = LHIS_ENCHANT()->CheckUpgrade_Weapon(&current_item);
		if( upgrade_type == -1 || !current_item )
			return FALSE;

		if( NULL == LHIS_ENCHANT_EX() )
			return FALSE;

		const LHParam_EnchantItem_Ex::sData * param_ex = LHIS_ENCHANT_EX()->GetParam(upgrade_type, current_item->m_PlusNum);


		if( !param_ex )
			return FALSE;

		int msg_index = -1;

		switch( param_ex->m_penalty_type )
		{
//		case 0:		msg_index = IDS_LHSTRING1946;		break;
		case 1:
			msg_index = IDS_LHSTRING1947;
			break;
		case 2:
			msg_index = IDS_LHSTRING1948;
			break;
		case 3:
			msg_index = IDS_LHSTRING1949;
			break;
		case 4:
			msg_index = IDS_LHSTRING1950;
			break;
		case 5:
			msg_index = IDS_LHSTRING1951;
			break;
		case 6:
			msg_index = IDS_LHSTRING1952;
			break;
		case 7:
			msg_index = IDS_LHSTRING1953;
			break;
		}

		if( msg_index != -1 )
		{
			g_pNk2DFrame->InsertPopup((char*)G_STRING(msg_index),TYPE_NOR_OKCANCLE, 30);
			return TRUE;
		}
	}
	else if( m_iType == INTERFACE_TYPE_UPARMOR || m_iType == INTERFACE_TYPE_UPWEAPON  || m_iType == INTERFACE_TYPE_320LV_WEAPON_UPGRADE )
	{
		CItem* pItem = NULL;

		pItem = g_pRoh->m_ExgInven;

		if(!pItem)
			return FALSE;

		if( g_SvrType != ST_ADULT_ONLY )
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_ARMOR )
				{
					if( pItem->m_PlusNum == 3 || pItem->m_PlusNum == 8 )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_11_WARNING),TYPE_NOR_OKCANCLE, 30);
						return TRUE;
					}
					else if( pItem->m_PlusNum == 11 && (pItem->m_MinLevel_Org > 300 || IS_2011_ITEM_ARMOR(pItem->m_Vnum )) )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1955),TYPE_NOR_OKCANCLE, 30);
						return TRUE;
					}
				}
				pItem = pItem->m_Next;
			}

			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON )
				{
					if( pItem->m_PlusNum == 3 || pItem->m_PlusNum == 7 )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG1), TYPE_NOR_OKCANCLE, 30 );
						return TRUE;
					}
					else if(pItem->m_PlusNum == 11 )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_MSG2), TYPE_NOR_OKCANCLE, 30 );
						return TRUE;
					}
				}
				pItem = pItem->m_Next;
			}
		}
		else
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_ARMOR )
				{
					if( pItem->m_PlusNum == 3 || pItem->m_PlusNum == 8 )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_11_WARNING),TYPE_NOR_OKCANCLE, 30);
						return TRUE;
					}
					else if(pItem->m_PlusNum == 11 && pItem->m_MinLevel_Org > 300 )
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_11_WARNING),TYPE_NOR_OKCANCLE, 30);
						return TRUE;
					}
				}
				pItem = pItem->m_Next;
			}

			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON )
				{
					if( pItem->m_PlusNum == 3 || pItem->m_PlusNum == 7 || pItem->m_PlusNum == 11)
					{
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_11_WARNING), TYPE_NOR_OKCANCLE, 30 );
						return TRUE;
					}
				}
				pItem = pItem->m_Next;
			}
		}
	}


	return FALSE;
}

void CControlUpgrade::ProcessState()
{
	switch( m_NowState )
	{
	case UPGRADE_ANI_ING:

		if( m_SpriteAni[m_NowState].m_NowFrame == 0 ) // ù�������϶�...
		{
			if( g_pDSound && m_pAniSound[m_NowState] )
			{
				g_pDSound->PlayToOutside( m_pAniSound[m_NowState] );
			}
		}

		if( timeGetTime() - m_dwStartTickTime > m_dwMaxAniTime )
		{
			if( m_Result > 0 && m_Result < MAX_UPGRADE_ANI )
			{
				m_NowState = m_Result;

				m_SpriteAni[m_NowState].m_AniType = SPRITE_ANITYPE_ONEPLAY;
				m_SpriteAni[m_NowState].SetStart();

				if( g_pDSound && m_pAniSound[m_NowState] )
				{
					g_pDSound->PlayToOutside( m_pAniSound[m_NowState] );
				}
			}
			else
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_ERROR), TYPE_NOR_OK, 1 );

				m_NowState = UPGRADE_ANI_NON; // ���� ����.
				m_bSlotLock = FALSE; // ���� ����.
				m_Result = 0; // ����� �ʱ�ȭ...

				return;
			}
		}
		else if( timeGetTime() - m_dwStartTickTime > m_dwMinAniTime ) // ������ �ð��� �����ٸ�...Ư�� �������� ��ٸ���.
		{
			if( m_SpriteAni[m_NowState].m_NowFrame == m_iAniChangeFrame ) // Ư�� �������϶� ���� �ִϸ��̼����� �Ѿ����..(������ ��ȣ�� �ϵ��ڵ�.)
			{
				if( m_Result > 0 && m_Result < MAX_UPGRADE_ANI ) // �����˻�
				{
					m_NowState = m_Result;

					m_SpriteAni[m_NowState].m_AniType = SPRITE_ANITYPE_ONEPLAY; // �� �ִϸ��̼��� �ѹ� �ݺ����� ����.
					m_SpriteAni[m_NowState].SetStart(); // �ִϸ��̼� ����.

					if( g_pDSound && m_pAniSound[m_NowState] )
					{
						g_pDSound->PlayToOutside( m_pAniSound[m_NowState] ); // ���� ��ȯ�ÿ� ȿ���� �÷��̸� ��Ų��.
					}
				}
				else // �� ���� ���� �̻��� �ִ°Ŵ�.
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_ERROR), TYPE_NOR_OK, 1 );

					m_NowState = UPGRADE_ANI_NON; // ���� ����.
					m_bSlotLock = FALSE; // ���� ����.
					m_Result = 0; // ����� �ʱ�ȭ...

					return;
				}
			}
		}
		break;

	case UPGRADE_ANI_SUCCESS: // �������� ��Ŷ�� ���� ���� ��...
	case UPGRADE_ANI_FAIL:
	case UPGRADE_ANI_BROKEN:

		if( m_SpriteAni[m_NowState].IsAniEnd() ) // �ִϸ��̼��� �ѹ� �÷��̰� ��������...
		{
			if( g_pNk2DFrame )
			{
				switch( m_iType )
				{
				case INTERFACE_TYPE_UPARMOR:
				case INTERFACE_TYPE_UPWEAPON: // Ÿ�Ե��� �޼����� �ٲ��� �Ѵ�.
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_UIMGR_SUCCEED_UPGRADE : �� ��� ���׷��̵忡 �����߽��ϴ�
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_UIMGR_FAIL_UPGRADE : �� ��� ���׷��̵忡 �����߽��ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_BROKEN:
							//-- IDS_UIMGR_REMOVE_UPGRADE : �� ��� �Ҹ�Ǿ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), -1); // ä��â���� �����ش�.

							break;
						}
					}
					break;

				case INTERFACE_TYPE_CONVERSION : // Ÿ�Ե��� �޼����� �ٲ��� �Ѵ�.
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_CONVERSION_SUCC : �������� �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_SUCC), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_SUCC), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_CONVERSION_FAIL : �������� �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_FAIL), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_FAIL), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_CONFUSION: // ���� �ٽ� ������
					{
						// �̳༮�� ������ �����̴�.
						//-- IDS_EQUIP_STATUS_RESET : ����� ��ġ�� ������ ����Ǿ����ϴ�.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EQUIP_STATUS_RESET), TYPE_NOR_OK, 32 );
						g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_EQUIP_STATUS_RESET), -1); // ä��â���� �����ش�.
					}
					break;

				case INTERFACE_TYPE_PROCESS: // Ÿ�Ե��� �޼����� �ٲ��� �Ѵ�.
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_NPCPROCESS_SUCC : ���ÿ� �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCPROCESS_SUCC), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_NPCPROCESS_SUCC), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_NPCPROCESS_FAIL : ���ÿ� �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCPROCESS_FAIL), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_NPCPROCESS_FAIL), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;
				case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_CONVERSION_SUCC : �������� �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_SUCC), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_SUCC), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_CONVERSION_FAIL : �������� �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_FAIL), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_FAIL), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_UIMGR_SUCCEED_UPGRADE : �� ��� ���׷��̵忡 �����߽��ϴ�
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_UIMGR_FAIL_UPGRADE : �� ��� ���׷��̵忡 �����߽��ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_BROKEN:
							//-- IDS_UIMGR_REMOVE_UPGRADE : �� ��� �Ҹ�Ǿ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_UIMGR_SUCCEED_UPGRADE : �� ��� ���׷��̵忡 �����߽��ϴ�
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_UIMGR_FAIL_UPGRADE : �� ��� ���׷��̵忡 �����߽��ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_BROKEN:
							//-- IDS_UIMGR_REMOVE_UPGRADE : �� ��� �Ҹ�Ǿ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UPGRADE_MSG3), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UPGRADE_MSG3), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UPGRADE_MSG4), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UPGRADE_MSG4), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_ITEMMAKE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							g_pNk2DFrame->InsertPopup( "Item success", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Item success", -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Item creation failed", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Item creation failed", -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_BUFFMAKE_MAKE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							g_pNk2DFrame->InsertPopup( "Buff item creation success", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Buff item creation success", -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Buff item creation failed", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Buff item creation failed", -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_BUFFMAKE_GATCHA:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							g_pNk2DFrame->InsertPopup( "Buff Item Gatche Success", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Buff Item Gatche Success", -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Buff Item Gatcha Fail", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Buff Item Gatcha Fail", -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_BUFFMAKE_MEDAL:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							g_pNk2DFrame->InsertPopup( "Successful exchange of medals", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Successful exchange of medals", -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Failed medal exchange", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Failed medal exchange", -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_ITEMSEPARATE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							g_pNk2DFrame->InsertPopup( "Item breakup success", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Item breakup success", -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Item breakdown failed", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Item breakdown failed", -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_GMAGICSTONE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//������ �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_GMASUK_SUCCESS), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_ITEM_GMASUK_SUCCESS), -1); // ä��â���� �����ش�.
							break;

						case UPGRADE_ANI_FAIL:
							//������ �����Ͽ����ϴ�.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_GMASUK_FAILED), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_ITEM_GMASUK_FAILED), -1); // ä��â���� �����ش�.
							break;
						}
					}
					break;


				}
			}

			pCMyApp->m_bInverseReturn = TRUE; // ������ �������� ����

			m_NowState = UPGRADE_ANI_NON; // ���� ����.
			m_bSlotLock = FALSE; // ���� ����.
			m_Result = 0; // ����� �ʱ�ȭ...

			CheckUpgradeNeed(); // �䱸 ������ ����.
		}

		break;
	}
}

void CControlUpgrade::UpdateTextInfo( int nType, int TextInfoIndex ) // TextInfo ���ǽ��� �ٲ�ε����ִ� �༮.
{
	char strTemp[256];

	if( m_iTextInfoIndex != m_iOldTextInfoIndex ) // ���ǽ��� �ٲ���� �ϴ� ���.
	{
		SAFE_DELETE(m_pTextInfo); // �켱 �����...

		switch( nType)
		{
		case INTERFACE_TYPE_CONVERSION:
			sprintf( strTemp, "interface/description/NpcUpgrade/Conversion_info%03d.bmp", TextInfoIndex ); // ��ȣ���� info�� �о�´�.
			break;

		case INTERFACE_TYPE_PROCESS:
			sprintf( strTemp, "interface/description/NpcUpgrade/Process_info%03d.bmp", TextInfoIndex ); // ��ȣ���� info�� �о�´�.
			break;
		case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
			sprintf( strTemp, "interface/description/npcupgrade/info_Conversion2_%2d.bmp", TextInfoIndex ); // ��ȣ���� info�� �о�´�.
			break;
		}

		g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, strTemp );  // �ٽ� �ε�.

		m_hWnd =  g_pDisplay->GetHWnd();
		GetClientRect( m_hWnd, &m_ClientRc );

		if( m_pTextInfo )
		{
			m_pTextInfo->Xpos = m_pBackSur->Xpos + 34;
			m_pTextInfo->Ypos = m_pBackSur->Ypos + 80;
		}
	}

	m_iOldTextInfoIndex = m_iTextInfoIndex; // ����.
}

BOOL CheckCanNpcUpgradeItem( CItem *pItem ) // ��ȭ�� ������ ������������ üũ��.
{
	if( !pItem )
		return FALSE;

	if( (GET_ATT_FLAG(pItem) & IATT_NO_GEM) )
		return FALSE;

	// m_Vnum : ������ DB��ȣ
	if( (pItem->m_Vnum >= 1190 && pItem->m_Vnum <= 1229) || (pItem->m_Vnum >= 1313 && pItem->m_Vnum <= 1326) ) // ����� �� ������ �����۸� ����
		return TRUE;

	if( pItem->m_Vnum >= 1560 && pItem->m_Vnum <= 1579 ) // 300���� �ű� �� �߰�.
		return TRUE;

	if( pItem->m_Vnum >= 1797 && pItem->m_Vnum <= 1816 ) // [2008/7/7 Theodoric]  ���� �� ��ȭ �߰�
		return TRUE;

	if( ( pItem->m_Vnum >= 2580 && pItem->m_Vnum <= 2583 )
			|| ( pItem->m_Vnum >= 2604 && pItem->m_Vnum <= 2607 )
			|| ( pItem->m_Vnum >= 2612 && pItem->m_Vnum <= 2615 )
			|| ( pItem->m_Vnum >= 2620 && pItem->m_Vnum <= 2623 )
			|| ( pItem->m_Vnum >= 2628 && pItem->m_Vnum <= 2631 )
	  )
		return TRUE;
	if(    ( pItem->m_Vnum >= 2636 && pItem->m_Vnum <= 2639 )
			|| ( pItem->m_Vnum >= 2644 && pItem->m_Vnum <= 2647 )
			|| ( pItem->m_Vnum >= 2652 && pItem->m_Vnum <= 2655 )
			|| ( pItem->m_Vnum >= 2660 && pItem->m_Vnum <= 2663 )
			|| ( pItem->m_Vnum >= 2668 && pItem->m_Vnum <= 2671 )
	  )
		return TRUE;



	if ( TRUE == IS_2011_ITEM_ARMOR( pItem->m_Vnum ) )
	{
		return TRUE;
	}
	if ( TRUE == IS_2011_ITEM_WEAPON( pItem->m_Vnum ) && pCMyApp->GetCurWorld() == 7 )
	{
		return TRUE;
	}

	if( pItem->m_Vnum >= 3227 && pItem->m_Vnum <= 3230 )
		return TRUE;

	return FALSE;

}

BOOL CheckCanConversionItem( CItem *pItem ) // �������� ������ ������������ üũ��.
{
	if( !pItem )
		return FALSE;

	// ���� ������ ó���� ���� ���� ��Ű��.
	switch( pItem->m_Vnum )
	{
	case 641:
	case 642:
	case 643:
	case 644:
	case 930:
	case 979:
	case 980:
	case 981:
	case 982:
	case 983:
	case 1062:
	case 1063:
	case 1064:
	case 1065:
	case 1066:
	case 1142:
	case 1146:
	case 1147:
	case 1150:
	case 1151:
	case 1152:
		return TRUE;
	}
	return FALSE;
}

BOOL CheckIsConversionItem( CItem *pItem, int type ,int sub_type) // �������Ǿ� ������ ������������ üũ.(�������� Ǯ�� ���ȴ�.)
{
	if( !pItem )
		return FALSE;

	switch( type )
	{
	case 0:
		{
			if( sub_type == 1 )
			{
				if( pItem->m_Vnum >= 2694 && pItem->m_Vnum <= 2700)
					return TRUE;
				if( pItem->m_Vnum >= 1318 && pItem->m_Vnum <= 1321 )
					return TRUE;
				if( pItem->m_Vnum == 1323
						|| pItem->m_Vnum == 1325
						|| pItem->m_Vnum == 1326 )
					return TRUE;
			}
			else if( sub_type == 2 )
			{
				if( (pItem->m_Vnum >= 1313 && pItem->m_Vnum <= 1326)
						|| (pItem->m_Vnum >= 1703 && pItem->m_Vnum <= 1707)
				  )
					return TRUE;
			}
			else if( sub_type == 0 )
			{
				if( (pItem->m_Vnum >= 1313 && pItem->m_Vnum <= 1326)
						|| (pItem->m_Vnum >= 1703 && pItem->m_Vnum <= 1707)
						|| (pItem->m_Vnum >= 2694 && pItem->m_Vnum <= 2700)
				  )
					return TRUE;
			}
		}
		break;
	case 1: // 320 ����
		if( pItem->m_Vnum >= 2049 && pItem->m_Vnum <= 2055 )
			return TRUE;


	/*case 2:
		{
			if( pItem->m_Vnum >= 2743 && pItem->m_Vnum <= 2749 )
				return TRUE;
			if( pItem->m_Vnum == 3231 )
				return TRUE;
		}*/
		break;
	}

	return FALSE;
}
BOOL ChechIs261LvWeponItem(CItem *pItem)
{
	if( pItem == NULL )
		return  FALSE;

	if( pItem->m_Vnum >= 1318 && pItem->m_Vnum <= 1321)
		return TRUE;

	switch( pItem->m_Vnum )
	{
	case 1323:
	case 1325:
	case 1326:
		return TRUE;
	}

	return FALSE;
}





BOOL CheckIsGodItem(CItem * pItem)
{
	if( !pItem )
		return FALSE;

	switch(pItem->m_Vnum)
	{
	case 2173:
	case 2174:
	case 2175:
	case 2176:
	case 2177:
	case 2178:
	case 2179:
	case 3323:
	case 2743: 	// START WEAPON 400, 500, 600, 700
	case 2744: 
	case 2745: 
	case 2746:
	case 2747: 
	case 2748: 
	case 2749: 
	case 3231: 
	case 10454:
	case 10455:
	case 10456:
	case 10457:
	case 10458:
	case 10459:
	case 10460:
	case 10461:
	case 10462:
	case 10463:
	case 10464:
	case 10465:
	case 10466:
	case 10467:
	case 10468:
	case 10469:
	case 10470:
	case 10471:
	case 10472:
	case 10473:
	case 10474:
	case 10475:
	case 10476:
	case 10477:
	case 10478:
	case 10479:
	case 10480:
	case 10481:
	case 10482:
	case 10483:
	case 10484:
	case 10517:
	case 10522:
	case 10523:
	case 10524:
	case 10525:
	case 10526:
	case 10527:
	case 10528:
	case 10529:
	case 10530:
	case 10531:
	case 10532:
	case 10533:
	case 10534:
	case 10535:
	case 10536:
	case 10537:
	case 10538:
	case 10539:
	case 10540:
	case 10541:
	case 10542:
	case 10543:
	case 10544:
	case 10545:
	case 10546:
	case 10547:
	case 10548:
	case 10549:
	case 10550:
	case 10551:
	case 10552:
	case 10553:
	case 10554:
	case 10555:
	case 10556:
	case 10557:
	case 10558:
	case 10559:
	case 10560:
	case 10561:
	case 10562:
	case 10563:
	case 10564:
	case 10565:
	case 10566:
	case 10567:
	case 10568:
	case 10569:
	case 10570:
	case 10571:
	case 10572:
	case 10573:
	case 10574:
	case 10575:
	case 10576:
	case 10577:
	case 10578:
	case 10579:
	case 10580:
	case 10581:
	case 10582:
	case 10583:
	case 10584:
	case 10585:// END WEAPON 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000.
		return TRUE;
	}

	return FALSE;
}

void CControlUpgrade::ResetText()
{
	m_NeedGemText.SetString( "", TRUE );
	m_NeedGemNumText.SetString( "", TRUE );
	m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
}

int CControlUpgrade::GetGemToIndex(int item_index)
{
	switch( item_index )
	{
	case 222:
		return 0;
	case 223:
		return 1;
	case 224:
		return 2;
	case 225:
		return 3;
	case 226:
		return 4;
	case 227:
		return 5;
	case 228:
		return 6;
	case 1068:
		return 7;
	case 1960:
		return 8;
	case 1961:
		return 9;
	case 1962:
		return 10;
	case 1963:
		return 11;
	case 1964:
		return 12;
	case 1965:
		return 13;
	case 1966:
		return 14;
	case 1967:
		return 15;
	case 3331:
		return 16;
	case 3332:
		return 17;
	case 10008:
		return 18;
	case 10032:
		return 19;
	case 10282:
		return 20;
	case 10283:
		return 21;
	case 10284:
		return 22;
	case 10686:
		return 23;
	case 10687:
		return 24;
	case 1832:
		return 25;
	case 10021:
		return 26;
	case 11240:
		return 27;
	default:
		return -1;
	}
	return -1;
}