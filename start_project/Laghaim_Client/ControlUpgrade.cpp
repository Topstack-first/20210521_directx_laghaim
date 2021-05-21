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



extern BOOL CheckCanNpcUpgradeItem( CItem *pItem ); // 강화가 가능한 아이템인지의 체크용.
extern BOOL CheckCanConversionItem( CItem *pItem ); // 컨버젼이 가능한 아이템인지의 체크용.
extern BOOL CheckIsConversionItem( CItem *pItem, int type = 0 ,int sub_type=0); // 컨버젼되어 나오는 아이템인지의 체크.(마석제한 풀기등에 사용된다.)
extern BOOL ChechIs261LvWeponItem(CItem *pItem); // 261 진 무기루 인지 확인
extern BOOL CheckIsGodItem(CItem *pItem);


// 임시 한글 스트링용 주석 => // han_msg

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




	Init(); // 변수들 초기화.


}

CControlUpgrade::~CControlUpgrade()
{
	DeleteRes();

	m_pNkCha = NULL;


}

void CControlUpgrade::Init()
{
	int i = 0;

	m_iType = 0; // 강화인지 가공인지 플래그. 현재는 0이면 강화.1이면 가공.
	m_my_slotstart_x = 0;
	m_my_slotstart_y = 0;

	m_NeedGemText.SetString( _T(" ")/*atoi(m_strMoney1)*/, true);
	m_NeedGemNumText.SetString( _T(" ")/*atoi(m_strMoney1)*/, true);
	m_NeedMoneyText1.SetString( _T(" ")/*atoi(m_strMoney1)*/, true);
	m_NeedMoneyText2.SetString( _T("0")/*atoi(m_strMoney1)*/, true);
	m_HaveMoneyText.SetString( _T("0")/*atoi(m_strMoney1)*/, true);

	m_bSlotLock = FALSE; // 교체시 슬롯에서 빼는 것을 방지시키기 위한 플래그.
	m_NowState = UPGRADE_ANI_NON;

	m_dwLastTickTime = 0;
	m_dwStartTickTime = 0;
	m_dwMinAniTime = 0;
	m_dwMaxAniTime = 0;
	m_iAniChangeFrame = 0;

	for( i = 0 ; i < MAX_NEED_ITEM_IN_NPC_UPGRADE ; ++i )
	{
		NeedItem[i] = 0; // 강화 또는 가공시 아이템 종류별로 필요한 갯수.
	}
	NeedLaim = 0; // 필요 라임.

	m_iOldMoney = 0; // 이전 보유금액

	m_Result = 0;

	m_iTextInfoIndex = 200; // 컨버젼시엔 텍스트 인포를 상황에 따라서 이미지를 다르게 로드해서 쓴다. 어떤 이미지가 로드되어있는지와 이전 이미지의 번호 인덱스를 기억하는 변수.
	m_iOldTextInfoIndex = 0; // 컨버젼시엔 텍스트 인포를 상황에 따라서 이미지를 다르게 로드해서 쓴다. 어떤 이미지가 로드되어있는지와 이전 이미지의 번호 인덱스를 기억하는 변수.

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

	Init(); // 변수들을 한번 초기화.

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


	m_BtnCancel.SetFileName( "common/btn_close_01" );	  // 취소버튼

	m_NeedGemText.Init( 12, RGB(255,255,0), 0, 0 );
	m_NeedGemNumText.Init( 12, RGB(255,255,0), 0, 0 );
	m_NeedMoneyText1.Init( 12, RGB(255,255,0), 0, 0 );
	m_NeedMoneyText2.Init( 12, RGB(255,255,0), 0, 0 );
	m_HaveMoneyText.Init( 12, RGB(255,255,0), 0, 0 );


	// 강화냐 가공이냐에 따라 다르게 로딩하는 녀석들...
	switch( m_iType )
	{
	case INTERFACE_TYPE_UPARMOR: // 방어구 강화
	case INTERFACE_TYPE_UPWEAPON: // 무기 강화
	case INTERFACE_TYPE_UPARMOR_400:
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/NpcUpgrade/NpcUpgrade_info.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_enhancement" );	  // 취소버튼

			if( g_dwClientCountry == CTRY_KOR ) // 한국은 레미 본야스키란 어떤 XXX 땜에 애니메이션 빼기로 했다.
			{
				// 아예 애니메이션 로드를 안한다.

				// 기타 수치는 0으로 셋팅.
				m_dwMinAniTime = 0; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 0; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 0; // 'ing'의 8프레임에서 전환
			}
			else // 다른 나라는 넣어주자.
			{
				// 스프라이트 애니매이션 로드
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type0_ing", 13, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type0_success", 25, 110 );
				m_SpriteAni[UPGRADE_ANI_FAIL].LoadRes( "interface/NpcUpgrade/effect/type0_fail", 16, 110 );
				m_SpriteAni[UPGRADE_ANI_BROKEN].LoadRes( "interface/NpcUpgrade/effect/type0_broken", 18, 120 );

				// 기타 애니메이션에 필요한 수치들 입력.
				m_dwMinAniTime = 2000; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 5000; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 8; // 'ing'의 8프레임에서 전환


				// 효과음 로드.
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

	case INTERFACE_TYPE_CONVERSION: // 변환
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/Conversion_info200.bmp" );	// 파일명 번호의 의미는 2(변화)의 00번(기본멘트) 이미지 라는 뜻.
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_conversion" );

			if( g_dwClientCountry == CTRY_KOR ) // 한국은 레미 본야스키란 어떤 XXX 땜에 애니메이션 빼기로 했다.
			{
				// 아예 애니메이션 로드를 안한다.

				// 기타 수치는 0으로 셋팅.
				m_dwMinAniTime = 0; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 0; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 0; // 'ing'의 8프레임에서 전환
			}
			else // 다른 나라는 넣어주자.
			{
				// 스프라이트 애니매이션 로드
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type2_ing", 14, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type2_success", 25, 110 );
				m_SpriteAni[UPGRADE_ANI_FAIL].LoadRes( "interface/NpcUpgrade/effect/type2_fail", 24, 100 );

				// 기타 애니메이션에 필요한 수치들 입력.
				m_dwMinAniTime = 3000; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 5000; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 13; // 'ing'의 8프레임에서 전환

				// 효과음 로드.
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

	case INTERFACE_TYPE_CONFUSION: // 랜덤 다시 돌리기.
		{
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/Confusion_info.bmp" );
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_confusion" );

			if( g_dwClientCountry == CTRY_KOR ) // 한국은 레미 본야스키란 어떤 XXX 땜에 애니메이션 빼기로 했다.
			{
				// 아예 애니메이션 로드를 안한다.

				// 기타 수치는 0으로 셋팅.
				m_dwMinAniTime = 0; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 0; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 0; // 'ing'의 8프레임에서 전환
			}
			else // 다른 나라는 넣어주자.
			{
				// 스프라이트 애니매이션 로드 // 이녀석은 무조건 성공이다.
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type2_ing", 14, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type2_success", 25, 110 );

				// 기타 애니메이션에 필요한 수치들 입력.
				m_dwMinAniTime = 3000; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 5000; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 13; // 'ing'의 8프레임에서 전환

				// 효과음 로드.
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
			g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, "interface/description/npcupgrade/Process_info200.bmp" );	// 파일명 번호의 의미는 2(변화)의 00번(기본멘트) 이미지 라는 뜻.
			m_UpgradeBtn.SetFileName( "NpcUpgrade/btn_refining" );

			if( g_dwClientCountry == CTRY_KOR ) // 한국은 레미 본야스키란 어떤 XXX 땜에 애니메이션 빼기로 했다.
			{
				// 아예 애니메이션 로드를 안한다.

				// 기타 수치는 0으로 셋팅.
				m_dwMinAniTime = 0; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 0; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 0; // 'ing'의 8프레임에서 전환
			}
			else // 다른 나라는 넣어주자.
			{
				m_SpriteAni[UPGRADE_ANI_ING].LoadRes( "interface/NpcUpgrade/effect/type2_ing", 14, 110 );
				m_SpriteAni[UPGRADE_ANI_SUCCESS].LoadRes( "interface/NpcUpgrade/effect/type2_success", 25, 110 );
				m_SpriteAni[UPGRADE_ANI_FAIL].LoadRes(  "interface/NpcUpgrade/effect/type2_fail", 24, 100 );

				// 기타 애니메이션에 필요한 수치들 입력.
				m_dwMinAniTime = 3000; // 패킷이 왔어도 요시간 후엔 전환.
				m_dwMaxAniTime = 5000; // 패킷이 안 왔어도 요시간 후엔 전환.
				m_iAniChangeFrame = 13; // 'ing'의 8프레임에서 전환

				// 효과음 로드.
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

	m_HaveMoneyText.SetPos( m_pBackSur->Xpos+35, m_pBackSur->Ypos+423 ); // 두번재 칸


	if( m_iType == INTERFACE_TYPE_CONVERSION  // 컨버젼, 정련은 "필요보석"글자의 위치를 살짝 바꿔준다.
			|| m_iType == INTERFACE_TYPE_PROCESS )
	{
		m_NeedGemText.m_PosY -= 64;
	}

	m_HaveMoneyText.SetString( g_pRoh->m_Money, TRUE ); // 자신의 보유금액을 인터페이스 열때 셋팅. 강화로 인한 사용시 갱신해주는 부분을 빼먹지 말자.
	m_iOldMoney = g_pRoh->m_Money;

	CheckUpgradeNeed(); // 로드시에 교환창에 미리 넣어둔경우도 있으니 이때도 필요 라임등을 계산해야한다.

	if( m_iType == INTERFACE_TYPE_CONVERSION  // 컨버젼, 정련은 "필요보석"글자의 위치를 살짝 바꿔준다.
			|| m_iType == INTERFACE_TYPE_PROCESS
			|| m_iType == INTERFACE_TYPE_320LV_WEAPON_CONVERSION
	  )
	{
		UpdateTextInfo( m_iType, m_iTextInfoIndex ); // 바뀌었다면 갱신.
	}
}

CItem* CControlUpgrade::Draw()
{
	if( !g_pDisplay )
		return NULL;

	m_pOverItem = NULL; // 여기서 초기화 하고 밑에 DrawMySlot()에서 셋팅된다.

	g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur );

	if ( NULL != m_pBack_Title )
	{
		g_pDisplay->Blt( m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBack_Title );
	}

	DrawNeedInfo(); // 아이템을 강화에 필요한 보석과 금액을 출력.

	m_UpgradeBtn.Draw();
	m_BtnCancel.Draw();

	// 돈이 변화되었나 체크해서 갱신해준다.
	if( g_pRoh && m_iOldMoney != g_pRoh->m_Money )
	{
		m_HaveMoneyText.SetString( g_pRoh->m_Money, TRUE );
		m_iOldMoney = g_pRoh->m_Money;
	}

	m_HaveMoneyText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+423 , 163);


	// 상태 전환 처리( 상태의 애니메이션 처리가 아니라 상태 바꾸기 처리임에 유의 )
	ProcessState();

	if( m_NowState == UPGRADE_ANI_NON ) // 애니메이션 중이아닐경우.
	{
		DrawMySlot(); // 슬롯을 그린다.
	}
	else if( m_NowState >= 0 && m_NowState < MAX_UPGRADE_ANI ) // 이상 범위 체크.
	{
		m_SpriteAni[m_NowState].Update( timeGetTime()-m_dwLastTickTime );
		m_SpriteAni[m_NowState].Draw( m_my_slotstart_x, m_my_slotstart_y );
	}



	m_dwLastTickTime = timeGetTime(); // 틱은 계속 갱신 시키자.

	return m_pOverItem; // 이 리턴을 하는 이유는 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신해서 표시해주기 위함인듯...
}

void CControlUpgrade::DrawNeedInfo() // 아이템을 강화에 필요한 보석과 금액을 출력.
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
				if( m_iNeedGemVnum[i] == 3331 ) // 신석
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

			// 젬 그림 그리기
			for( i = 0 ; i < MAX_NEED_ITEM_IN_NPC_UPGRADE ; ++i )
			{
				if( NeedItem[i] ) // 필요 갯수가 있는 녀석이 있다면...
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

					// 현재는 무조건 한종류 1개이므로 필요갯수가 있는 녀석이 있으면 찍고 루프 탈출한다.
					//			i = MAX_NEED_ITEM_IN_NPC_UPGRADE; // 루프 탈출용 꽁수. 현재는 탈출할 필요가 없어서 뺌.
				}
			}

			if( mx !=0 )
			{
				m_NeedGemNumText.SetPos(mx, m_NeedGemText.m_PosY );
			}
			m_NeedGemNumText.Draw();
		}
		break;

	case INTERFACE_TYPE_CONVERSION : // 컨버젼 인터페이스의 경우
		{
			if( m_iTextInfoIndex >= 203 ) // 203 이상은 필요조건 그림상태이다.
				m_NeedGemText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+206,163);

			m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);
		}
		break;

	case INTERFACE_TYPE_CONFUSION : // 랜덤 다시 돌리기
		{
			//m_NeedGemText.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+206,163);
			////m_NeedGemNumText.Draw();
			//m_NeedMoneyText1.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+170,163);
			//m_NeedMoneyText2.Draw(m_pBackSur->Xpos+35, m_pBackSur->Ypos+383,163);

			//int mx = 0;

			//// 젬 그림 그리기 // 이 녀석은 무조건 다이아다.
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

	case INTERFACE_TYPE_PROCESS: // 컨버젼 인터페이스의 경우
		{
			if( m_iTextInfoIndex >= 203 ) // 203 이상은 필요조건 그림상태이다.
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

	// 아이템이 위치한 슬롯 표시
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

	// Extra Slot에 있는 아이템이 놓일 곳 표시
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

	// 내 교환품 그리기..
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
		m_pItemRender->BeginRenderLolo();		// 중요..  m_pItemRender->RenderLolo의 파라미터에 영향을 준다.
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

	if( m_bSlotLock ) // 교환하는순간 extra 하는 것을 막기 위함이다.lock
	{
		// 일정 시간 후에 락을 강제로 해제해주는건 어떨런지?
		return 1;
	}

	switch (msg)
	{
	case WM_RBUTTONDBLCLK:

		// 대형 장비 이동 불가
		if( pCMyApp->m_bEquipLock == 1 )
			return 1;
		// 반지 이동 불가
		if( pCMyApp->m_bRingLock == 1 )
			return 1;
		if(pCMyApp->m_bBrLock ==1)
			return 1;
		if(pCMyApp->m_bNecklaceLock==1)		///페어리
			return 1;

		// ExtraSlot에 아이템이 있을경우
		if( g_pRoh->m_ExtraSlot )
			return 1;

		if( IsInside( x, y ) )
		{
			// 마석 업그레이드 창이 떠있을 경우
			if( g_pNk2DFrame
					&& g_pNk2DFrame->GetControlInven()
					&& g_pNk2DFrame->GetControlInven()->m_bAct_Masuk )
			{
				g_pNk2DFrame->GetControlInven()->Close_Masuk();
				return 1;
			}

			// 영역 검사
			if( g_pRoh
					&& ( x >= m_my_slotstart_x )
					&& ( x < m_my_slotstart_x + ( SLOT_WIDTH * EXG_SLOT_X_NUM ) )
					&& ( y >= m_my_slotstart_y )
					&& ( y < m_my_slotstart_y + ( SLOT_HEIGHT * EXG_SLOT_Y_NUM ) ) )
			{
				int nSlot_X_Num = 0;
				int nSlot_Y_Num = 0;

				// 슬롯 계산
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

				// 인텍스 값을 이용한 아이테의 wnum 추출
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

				// const 변수 선언
				char szTmp[ 10 ] = {0,};
				char szStr[ 100 ] = {0,};

				// 작업동안 딜레이 타임 1.5초
				// 최소한의 안전장치
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlExchange() )
				{
					if( g_pNk2DFrame->GetControlExchange()->m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > g_pNk2DFrame->GetControlExchange()->m_timeExchLock + 1500) // 1.5초가 지났으면
							g_pNk2DFrame->GetControlExchange()->m_byExchLock = 0;
						else
							return 1;
					}

					// 딜레이가 지났으면 플레그 해제
					g_pNk2DFrame->GetControlExchange()->SetExchLock( 1 );
				}

				// 선택한 아이템과 동일 아이템 추출
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

				CheckUpgradeNeed(); // 검사뿐만 아니라 텍스트 셋팅도 해준다.

				sprintf( commOutBuf, "exch_get %d %s\n", g_pRoh->m_CurPackNum, szStr );

				if( g_pTcpIp )
					g_pTcpIp->SendNetMessage( commOutBuf );

			}

		}

		break;

	case WM_LBUTTONDOWN:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_UpgradeBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_BtnCancel.MsgProc( hWnd, msg, wParam, lParam );

			if (x >= m_my_slotstart_x && x < m_my_slotstart_x + (SLOT_WIDTH * EXG_SLOT_X_NUM)
					&& y >= m_my_slotstart_y && y < m_my_slotstart_y + (SLOT_HEIGHT * EXG_SLOT_Y_NUM) ) // 교환창 슬롯 부분 클릭시...
			{

				if( timeGetTime() - m_dwMouseClickTickTime < 300 )  // 0.3초
					return 1;

				m_dwMouseClickTickTime = timeGetTime();
				int slot_x_num, slot_y_num;
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlExchange() )
				{
					if( g_pNk2DFrame->GetControlExchange()->m_byExchLock )
					{
						DWORD curTime = timeGetTime();

						if (curTime > g_pNk2DFrame->GetControlExchange()->m_timeExchLock + 1500) // 1.5초가 지났으면
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
							&& slot_y_num >= 0 && slot_y_num + GET_ITEM_HEIGHT(g_pRoh->m_ExtraSlot) - 1 < EXG_SLOT_Y_NUM ) // 클릭한 위치가 유효한 위치면..(범위만 체크)
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
					// 액션중이라면 픽안되게...

					if(    g_pRoh && g_pRoh->AddToExgInven( (x-m_my_slotstart_x)/SLOT_WIDTH, (y-m_my_slotstart_y)/SLOT_HEIGHT )     )
					{
						sprintf( commOutBuf, "exch inven %d %d\n", (x-m_my_slotstart_x) / SLOT_WIDTH, (y-m_my_slotstart_y) / SLOT_HEIGHT );
						if( g_pTcpIp )
							g_pTcpIp->SendNetMessage(commOutBuf);
					}
				}

				// 아이템을 움직이면 아이템을 검사해서 필요조건을 출력시켜준다.
				if( m_iType == INTERFACE_TYPE_CONVERSION || m_iType == INTERFACE_TYPE_PROCESS )
				{
					if( m_iTextInfoIndex == 201 || m_iTextInfoIndex == 202 ) // 결과 멘트 상태에서는 인벤창 클릭시까지 TextInfo를 바꾸지 않는다. (새로 클릭되면 다시 조건 표시검사를 하도록..)
					{
						m_iTextInfoIndex = 200; // 클릭했으면 기본 멘트로...(변환 아이템이 있으면 바로 조건으로 바뀔꺼다.)
					}
				}

				// 컨버젼 인터페이스의 경우는 TextInfo를 서피스 자체를 바꿔준다.
				// 클릭시에만 반응해서 TextInfo를 수정한다.
				if( m_iTextInfoIndex != 201 && m_iTextInfoIndex != 202 ) // 결과 멘트 상태에서는 아이템이 있어도 TextInfo를 바꾸지 않는다.(인벤창 클릭시 갱신해준다.)
				{
					m_iTextInfoIndex = 200;	// 컨버젼시 TextInfo 출력해주는 부분을 0으로 초기화.(0은 기본 멘트다.)
				}

				if( m_iType == INTERFACE_TYPE_320LV_WEAPON_CONVERSION )
				{
					if( m_iTextInfoIndex != 50 )
						m_iTextInfoIndex = 50;
				}
				CheckUpgradeNeed(); // 검사뿐만 아니라 텍스트 셋팅도 해준다.

				if( m_iType == INTERFACE_TYPE_CONVERSION || m_iType == INTERFACE_TYPE_PROCESS
						|| m_iType == INTERFACE_TYPE_320LV_WEAPON_CONVERSION
				  )
				{
					UpdateTextInfo( m_iType, m_iTextInfoIndex ); // 바뀌었다면 갱신.
				}



			}
		}
		else if( IsPlayArea(x,y) ) // 인터페이스 밖의 땅을 클릭시.
		{
			g_pNk2DFrame->ToggleUpgradeWindow( m_iType, TRUE ); // Toggle 함수를 사용해서 강제 종료시킨다.
		}
		else
			return 0;

		return 1;

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		if( IsInside(x,y) ) // 인터페이스의 안쪽을 클릭했을 경우.
		{
			m_UpgradeBtn.MsgProc(hWnd, msg, wParam, lParam);
			m_BtnCancel.MsgProc( hWnd, msg, wParam, lParam );


			if( g_pNk2DFrame && m_BtnCancel.GetState()==BTN_ACTION )
			{
				g_pNk2DFrame->ToggleUpgradeWindow( m_iType, TRUE ); // Toggle 함수를 사용해서 강제 종료시킨다.
				m_BtnCancel.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
			}


			if( m_UpgradeBtn.GetState() == BTN_ACTION ) // 업그레이드 버튼을 클릭시.
			{

				if( g_pRoh->m_ExtraSlot )
					return 1;


				if( CheckSlotPure() != -1 ) // 교환창에 부적절한 아이템이 하나도 없었으면...
				{
					if( !CheckBrokenItem() ) // 깨어질 염려가 있을땐 경고창을 한번 더 띄워준다.
					{
						m_Result = 0; // 초기화.

						switch( m_iType ) // 타입에 따라 패킷이 다르다.
						{
						case INTERFACE_TYPE_UPARMOR_400:
							sprintf( commOutBuf, "npc_up armor_400\n" );
							break;
						case INTERFACE_TYPE_UPARMOR: // 강화 인터페이스(방어구)의 경우.
							sprintf( commOutBuf, "npc_up armor\n" );
							break;
						case INTERFACE_TYPE_UPWEAPON: // 강화 인터페이스(무기)의 경우
							sprintf( commOutBuf, "npc_up weapon\n" );
							break;
						case INTERFACE_TYPE_CONVERSION: // 컨버젼 인터페이스의 경우
							sprintf( commOutBuf, "npc_up conv\n" );
							break;
						case INTERFACE_TYPE_CONFUSION: // 랜덤 다시 돌리기
							sprintf( commOutBuf, "npc_up reload\n" );
							break;
						case INTERFACE_TYPE_PROCESS: // 가공
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
							// 패킷을 보낸다. (애니메이션의 끝을 기다리지 말고 바로 보낸다.)
							m_bSlotLock = TRUE; // 교체가 시작 된 후에 빼지 못하도록....
						}
					}


				}

				m_UpgradeBtn.SetState(BTN_NORMAL);	// 버튼의 스테이터스를 직접 풀어줘야하나보다.
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
		// 패킷을 보낸다. (애니메이션의 끝을 기다리지 말고 바로 보낸다.)
		m_bSlotLock = TRUE; // 교체가 시작 된 후에 빼지 못하도록....
		m_Result = 0; // 초기화.


		// 타입에 따라 패킷이 다르다.
		switch(m_iType )
		{
		case INTERFACE_TYPE_UPARMOR_400:
			sprintf( commOutBuf, "npc_up armor_400\n" );
			break;
		case INTERFACE_TYPE_UPARMOR: // 강화 인터페이스(방어구)의 경우.
			sprintf( commOutBuf, "npc_up armor\n" );
			break;
		case INTERFACE_TYPE_UPWEAPON: // 강화 인터페이스(무기)의 경우
			sprintf( commOutBuf, "npc_up weapon\n" );
			break;
		case INTERFACE_TYPE_CONVERSION : // 컨버젼 인터페이스의 경우
			sprintf( commOutBuf, "npc_up conv\n" );
			break;
		case INTERFACE_TYPE_CONFUSION : // 랜덤 다시 돌리기
			sprintf( commOutBuf, "npc_up reload\n" );
			break;
		case INTERFACE_TYPE_PROCESS : // 가공
			sprintf( commOutBuf, "npc_up refine\n" );
			break;
		case INTERFACE_TYPE_320LV_WEAPON_CONVERSION : // 가공
			sprintf( commOutBuf, "npc_up conv\n" );
			break;
		case INTERFACE_TYPE_320LV_WEAPON_UPGRADE : // 가공
			sprintf( commOutBuf, "npc_up weapon\n" );
			break;
		case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
			sprintf( commOutBuf, "npc_up newup\n" );
			break;
		case INTERFACE_TYPE_320LV_WEAPON_GODWPOWER : // 가공
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

BOOL CControlUpgrade::IsPlayArea( int x, int y ) // 인터페이스를 벗어나 땅을 클릭했는지의 체크.
{
	if( !IsInside(x,y) && !g_pNk2DFrame->IsInPopup(x,y) )
		return true;

	return false;
}

int CControlUpgrade::CheckUpgradeNeed() // 강화에 필요한 젬(종류,갯수)과 라임을 구한다. 측정한 아이템의 포인터를 리턴.(아이템이 동시에 여러개 올라가있으면 NULL을 리턴한다. 오로지 방어구 하나만 올라가있을때 제대로 리턴. NULL일 경우 유저가 잘못 올린 경우다.)
{
	CItem* pItem = NULL;
	CItem* pCheckItem = NULL;
	CItem* pCheckItem2 = NULL;
	//	CItem* pGem = NULL; // 들어가있는 보석의 포인터.(여러개 들어가있을 경우 현재는 마지막 껏만 체크된다.)(보석이 여러개 혹은 여러종류가 필요해질 경우 소스에 수정이 필요하다.)
	int ItemCount = 0, ItemCount2 = 0; // 들어가있는 아이템 갯수 체크용. 방어구만 체크한다.
	int i = 0;
	int special_level = -1, special_level1 = -1, special_level2 = -1, special_level3 = -1, special_level4 = -1;
	char strTemp[256];
	char strTemp2[30];

	ZeroMemory( m_need_gem , sizeof(m_need_gem));

	// 필요조건 초기화.( 이 함수 가장 앞에 해줘야함에 유의. )
	for( i = 0 ; i < MAX_NEED_ITEM_IN_NPC_UPGRADE ; ++i )
	{
		NeedItem[i] = 0; // 강화 또는 가공시 아이템 종류별로 필요한 갯수.
	}
	NeedLaim = 0; // 필요 라임.


	if( g_pRoh )
		pItem = g_pRoh->m_ExgInven;

	if(!pItem)
	{
		m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
		m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
		m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
		m_NeedMoneyText2.SetString( "", TRUE );

		return -1;
	}

	// 텍스트도 초기화
	//-- IDS_NEED_GEM : 필요 보석:
	sprintf( strTemp, (char*)G_STRING(IDS_NEED_GEM) );
	m_NeedGemText.SetString( strTemp, FALSE ); // 첫번째 인자가 문자열이면 두번째 인자는 폰트 색이다.FALSE로 주면 알아서 init()때 지정한 색이 된다.
	sprintf( strTemp, "(%d)", NeedItem[UPGRADE_NEED_REGENT_DIA] );
	m_NeedGemNumText.SetString( strTemp, FALSE ); // 첫번째 인자가 문자열이면 두번째 인자는 폰트 색이다.FALSE로 주면 알아서 init()때 지정한 색이 된다.
	m_NeedMoneyText1.SetString( "", TRUE ); // 첫번째 인자가 숫자면 두번째 인자는 콤마 여부다.
	m_NeedMoneyText2.SetString( "", TRUE ); // 안보이도록 초기화.(검은색으로 출력이라고 보면된다.)

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
				if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // 무기도 추가되었음. 무기와 방어구만 체크한다.
				{

					++ItemCount;
					pCheckItem = pItem; // 포인터를 기억. 아이템이 정상적으로 하나 뿐이었다면 이 포인터를 이용해서 필요한걸 구한다.

					// 아이템 강화때 파괴딜 수 있는 조건에서 경고 문구를 띄워준다.
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

			if( ItemCount != 1 ) // 무기,방어구가 1개가 아니었으면 에러.
			{
				m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else if( GET_TYPE(pCheckItem)==ITYPE_WEAPON && ChechIs261LvWeponItem(pCheckItem) && pCheckItem->m_PlusNum >= 10 )
			{
				m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				//-- IDS_NPCUPGRADE_MAXLEVEL : 더이상 강화가 불가능한 아이템입니다.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), FALSE );

				return -1;
			}
			else if( (GET_TYPE(pCheckItem)==ITYPE_ARMOR && pCheckItem->m_PlusNum >= 15) ||  // 방어구는 15단계 까지만 업그레이드가 가능하다. (05-11-10 20단계까지에서 15단계 까지로 변경)
					 (GET_TYPE(pCheckItem)==ITYPE_ARMOR && pCheckItem->m_MinLevel_Org >= 360 && pCheckItem->m_PlusNum >= 10) )
			{
				m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				//-- IDS_NPCUPGRADE_MAXLEVEL : 더이상 강화가 불가능한 아이템입니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), TYPE_NOR_OK, 1 );
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), FALSE );

				return -1;
			}
			else // 정상적으로 무기,방어구가 하나뿐이면 필요한 라임등을 구한다.
			{
				if( pCheckItem ) // 이거 검사해야한다.
				{
					if( !CheckCanNpcUpgradeItem(pCheckItem) ) // 특정 아이템만 강화 가능.
					{
						m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
						m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}

					// [2008/7/7 Theodoric]  레어 방어구 강화 추가
					if( (pCheckItem->m_Vnum >= 1560 && pCheckItem->m_Vnum <= 1579) || (pCheckItem->m_Vnum >= 1797 && pCheckItem->m_Vnum <= 1816)

							|| ( TRUE == IS_2011_ITEM_ARMOR( pCheckItem->m_Vnum ) )

					  )
					{
						++NeedItem[UPGRADE_NEED_REGENT_DIA]; // 300레벨은 무조건 리젠트 하나씩.
					}
					else if( (pCheckItem->m_Vnum >= 1190 && pCheckItem->m_Vnum <= 1229) || (pCheckItem->m_Vnum >= 1313 && pCheckItem->m_Vnum <= 1326) ) // 기존 무기,방어구
					{
						if( (pCheckItem->m_PlusNum+1) > NPC_UPGRADE_NEED_REGENT_LEVEL ) // +9이상 강화는 리젠트로...
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
						if( (pCheckItem->m_PlusNum+1) > NPC_UPGRADE_NEED_REGENT_LEVEL ) // +9이상 강화는 리젠트로...
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
						if( (pCheckItem->m_PlusNum+1) > NPC_UPGRADE_NEED_REGENT_LEVEL ) // +9이상 강화는 리젠트로...
						{
							++NeedItem[UPGRADE_NEED_REGENT_DIA];
						}
						else
						{
							++NeedItem[UPGRADE_NEED_DIA];
						}
					}

					//-- IDS_NPCUPGRADE_NEEDGEM : 필요 보석:
					m_NeedGemText.SetString( (char*)G_STRING(IDS_NPCUPGRADE_NEEDGEM), FALSE ); // 여기다 하는 이유는 보석 종류가 틀릴경우 다른 메세지로 고치기 위해서...
					sprintf( strTemp, "(%d)", 1 ); // 일단은 무조건 필요갯수가 하나니까 강제로 1로 셋팅. 추후 갯수가 바뀌면 고치자.
					m_NeedGemNumText.SetString( strTemp, FALSE ); // 여기다 하는 이유는 보석 종류가 틀릴경우 다른 메세지로 고치기 위해서...

					if ( pCheckItem->m_Vnum >= 1797 && pCheckItem->m_Vnum <= 1816 ) // [2008/7/7 Theodoric]  레어 방어구 강화 추가
						NeedLaim = 450000 * (pCheckItem->m_PlusNum+1);
					else if( pCheckItem->m_Vnum >= 1560 && pCheckItem->m_Vnum <= 1579
							 || ( TRUE == IS_2011_ITEM_ARMOR( pCheckItem->m_Vnum ) )
						   ) // 300레벨 신규 방어구 추가.
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
					else if( (pCheckItem->m_Vnum >= 1190 && pCheckItem->m_Vnum <= 1229 ) || (pCheckItem->m_Vnum >= 1313 && pCheckItem->m_Vnum <= 1326) ) // 기존 무기,방어구
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

					// 무기 강화냐 방어구강화냐를 클라이언트 내부적으로 바꿔준다.(오픈시엔 그냥 방어구강화로 열지만 올릴때 바꾸고 패킷만 다르게 보낸다. 나머진 다 같다.)
					if( GET_TYPE(pCheckItem)==ITYPE_ARMOR )
					{
						m_iType = INTERFACE_TYPE_UPARMOR;
					}
					else // 혹시라도 방어구도,무기도 아닐경우도 그냥 일단 무기로 취급.(최소한 꼬이진 않아야하지 않겠는가?)
					{
						m_iType = INTERFACE_TYPE_UPWEAPON;
					}
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONVERSION:
		{
			// 컨버젼 인터페이스는 필요조건을 조건별로 이미지를 바꿔서 표시...
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON || GET_TYPE(pItem)==ITYPE_ARMOR ) // 무기또는 방어구 체크.
				{
					++ItemCount;
					pCheckItem = pItem; // 포인터를 기억. 아이템이 정상적으로 하나 뿐이었다면 이 포인터를 이용해서 필요한걸 구한다.
				}

				// 젬 체크는 하지 않는다.

				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // 무기,방어구가 1개가 아니었으면 에러.
			{
				m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else if( pCheckItem->m_PlusNum != 15 ) // 현재는 +15 아이템만 컨버젼이 가능하다.
			{
				m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else // 정상적으로 무기,방어구가 하나뿐이면 필요한 라임등을 구한다.
			{
				if( pCheckItem ) // 이거 검사해야한다.
				{
					if( !CheckCanConversionItem(pCheckItem) ) // 특정 아이템만 강화 가능.
					{
						m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
						m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}

					// 그려야할 메세지 파일이 바뀌었다면 다시 로드.
					if( m_iTextInfoIndex != 201 && m_iTextInfoIndex != 202 ) // 결과 멘트 상태에서는 아이템이 있어도 TextInfo를 바꾸지 않는다.(인벤창 클릭시 갱신해준다.)
					{
						if( GET_ITEM_LEVEL(pCheckItem) == 195 || GET_ITEM_LEVEL(pCheckItem) == 200 ) // 필요조건 3타입. (하이브리더용 동급 무기는 200레벨 제한이라서 추가로 검사)
						{
							m_iTextInfoIndex = 203; // 0,1,2는 멘트들...
						}
						else if( GET_ITEM_LEVEL(pCheckItem) == 210 ) // 필요조건 4타입.
						{
							m_iTextInfoIndex = 204; // 0,1,2는 멘트들...
						}
						else if( GET_ITEM_LEVEL(pCheckItem) == 261 ) // 필요조건 5타입.
						{
							m_iTextInfoIndex = 205; // 0,1,2는 멘트들...
						}
					}

					NeedLaim = 10000000; // 현재는 고정 1000만.
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONFUSION:
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON || GET_TYPE(pItem)==ITYPE_ARMOR ) // 무기또는 방어구 체크.
				{
					++ItemCount;
					pCheckItem = pItem; // 포인터를 기억. 아이템이 정상적으로 하나 뿐이었다면 이 포인터를 이용해서 필요한걸 구한다.
				}

				// 젬 체크는 하지 않는다.

				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // 무기,방어구가 1개가 아니었으면 에러.
			{
				m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
				//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
				m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

				return -1;
			}
			else // 정상적으로 무기,방어구가 하나뿐이면 필요한 라임등을 구한다.
			{
				if( pCheckItem ) // 이거 검사해야한다.
				{
					if( !CheckIsConversionItem(pCheckItem,0,2) ) // 특정 아이템(컨버젼된 녀석 만)만 강화 가능.
					{
						m_NeedGemText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
						m_NeedGemNumText.SetString( "", TRUE ); // 이렇게 하면 안보인다.
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}

					NeedItem[UPGRADE_NEED_DIA] = (pCheckItem->m_PlusNum-1) / 4;

					if( NeedItem[UPGRADE_NEED_DIA] < 0 )
						NeedItem[UPGRADE_NEED_DIA] = 0;

					//-- IDS_NPCUPGRADE_NEEDGEM : 필요 보석:
					m_NeedGemText.SetString( (char*)G_STRING(IDS_NPCUPGRADE_NEEDGEM), FALSE ); // 여기다 하는 이유는 보석 종류가 틀릴경우 다른 메세지로 고치기 위해서...
					sprintf( strTemp, "(%d)", NeedItem[UPGRADE_NEED_DIA] ); // 현재는
					m_NeedGemNumText.SetString( strTemp, FALSE ); // 여기다 하는 이유는 보석 종류가 틀릴경우 다른 메세지로 고치기 위해서...

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
				if( GET_TYPE(pItem) == ITYPE_ARMOR ) // 방어구 체크.
				{
					++ItemCount;
					pCheckItem = pItem;
				}

				// 젬 체크는 하지 않는다.
				pItem = pItem->m_Next;
			}

			if( ItemCount != 1 ) // 방어구아니었으면 에러.
			{
				m_NeedGemText.SetString( "", TRUE );
				m_NeedGemNumText.SetString( "", TRUE );
				//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
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
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );

						return -1;
					}
				}
			}

			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_ETC) // 정련서 체크.
				{
					pCheckItem2 = pItem; // 포인터를 기억. 아이템이 정상적으로 하나 뿐이었다면 이 포인터를 이용해서 필요한걸 구한다.
					++ItemCount2;
				}
				pItem = pItem->m_Next;
			}

			if( ItemCount2 == 0 ) // 정련서아니었으면 에러.
			{
				m_NeedGemText.SetString( "", TRUE );
				m_NeedGemNumText.SetString( "", TRUE );
				//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
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

			if( m_iTextInfoIndex != 201 && m_iTextInfoIndex != 202 ) // 결과 멘트 상태에서는 아이템이 있어도 TextInfo를 바꾸지 않는다.(인벤창 클릭시 갱신해준다.)
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
					//-- IDS_MAX_OPTION : 현재 옵션이 최대치입니다.
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
					//-- IDS_MAX_OPTION : 현재 옵션이 최대치입니다.
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
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
						return -1;
					}
					if( targetItem == NULL )
						targetItem = pItem;
				}
				else if (GET_TYPE(pItem)==ITYPE_ARMOR )
				{
					//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
					m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
					return -1;
				}
				pItem = pItem->m_Next;
			}

			NeedItem[UPGRADE_NEED_DIA] = 3;
			NeedItem[UPGRADE_NEED_REGENT_DIA] = 8;

			// 일본은 라임을 틀리게 요청
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
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
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
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), FALSE );
						return -1;
					}
				}

				pItem = pItem->m_Next;
			}

			NeedItem[UPGRADE_NEED_REGENT_DIA]++;
			//		m_NeedGemText.SetString("용의 인장 1set");
			// 일본은 라임을 틀리게 요청
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
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) ); // 콤마를 넣기위해 임시로 m_NeedMoneyText1의 버퍼를 사용.
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
				sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) ); // 콤마를 넣기위해 임시로 m_NeedMoneyText1의 버퍼를 사용.
				m_NeedMoneyText1.SetString( strTemp, FALSE );
				m_NeedMoneyText2.SetString( NeedLaim, TRUE );
				return -1;
			}
		}
		break;


	}

	sprintf( strTemp2, "%d", NeedLaim );
	sprintf( strTemp, (char*)G_STRING(IDS_NPCUPGRADE_NEEDLAIM), m_NeedMoneyText1.TransComma(strTemp2) ); // 콤마를 넣기위해 임시로 m_NeedMoneyText1의 버퍼를 사용.
	m_NeedMoneyText1.SetString( strTemp, FALSE );
	m_NeedMoneyText2.SetString( NeedLaim, TRUE );

	return 1;
}

int CControlUpgrade::CheckSlotPure() // 업그레이드를 시도하는데 부적절한 아이템이 포함되어있는 지의 체크.
{
	CItem* pItem = NULL;
	CItem* pCheckItem = NULL;
	int GemCount = 0; // 젬 갯수 세기
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
	case INTERFACE_TYPE_UPWEAPON: // 강화 인터페이스의 경우. 일일이 체크 해주면서 필요 문구들을 만들어준다.
		{
			while (pItem)
			{
				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON
						&& ( pItem->m_Vnum != 228 && pItem->m_Vnum != 1966 )
						&& ( pItem->m_Vnum != 1068 && pItem->m_Vnum != 1967 )
						&& ( pItem->m_Vnum != 1739 && pItem->m_Vnum != 1740 )
						&& ( pItem->m_Vnum != 1832 )
				  ) // 현재는 방어구,무기,다이아,리젠트 다이아 이외에는 지원안함.(후에 가공이 들어가면 iType 플래그로 구분해서 체크하자.)
				{
					// 엉뚱한 아이템이 들어가 있으면...
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // 방어구일때...무기도...
				{
					// by evilkiki 2010.03.16 무기/방어구 에대해서만 시간제 적용
					if( pItem->m_bTimeLimit ) // 시간제 아이템 불가처리. 추가. (06-02-24 원석)
					{
						//-- IDS_LIMITED_CANT : 제한된 아이템은 불가능합니다.
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
						return -1;
					}

					if( ChechIs261LvWeponItem(pItem) && pItem->m_PlusNum >= 10)
					{
						//-- IDS_NPCUPGRADE_MAXLEVEL : 더이상 강화가 불가능한 아이템입니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_MAXLEVEL), TYPE_NOR_OK, 1 );
						return -1;
					}
					else if( !CheckCanNpcUpgradeItem(pItem) ) // 특정 아이템만 강화 가능.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( pItem->m_Vnum == 228 || pItem->m_Vnum == 1068
						|| pItem->m_Vnum == 1966 || pItem->m_Vnum == 1967
				  ) // 보석 갯수 세기.
				{
					++GemCount;
				}

				pItem = pItem->m_Next;
			}

			if( GemCount != 1 ) // 현재의 보석요구량은 무조건 1이다.
			{
				//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}

			if( CheckUpgradeNeed() == -1 ) // 이 함수에는 방어구 종류가 1개뿐인지 체크해주는 기능도 있다.
			{
				// 방어구가 2개이상이면...
				return -1;
			}

			// 요구 젬을 구했으므로 젬이 맞게 들어가있는지 검사.
			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON ) // 방어구는 빼고 검사. 무기도...
				{
					if( NeedItem[UPGRADE_NEED_DIA]
							&& ( pItem->m_Vnum != 228 && pItem->m_Vnum != 1966 )
					  ) // 다이아를 요구하는데 다이아 이외의 것이 있으면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
					else if( NeedItem[UPGRADE_NEED_REGENT_DIA]
							 && ( pItem->m_Vnum != 1068 && pItem->m_Vnum != 1967 )
						   ) // 리젠트 다이아를 요구하는데 그 이외의 것이 있으면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else // 방어구는 내구도를 검사해준다.
				{
					if( pItem->m_MaxUpgradeEndurance && pItem->m_UpgradeEndurance <= 0 ) // 강화 내구도가 다 떨어졌으면... // 07-04-17 맥스 치가 있을때만 내구도 검사하도록 수정.
					{
						//-- IDS_NPCUPGRADE_EDRLOW : 강화 내구도가 부족합니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_EDRLOW), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				pItem = pItem->m_Next;
			}
		}
		break;

	case INTERFACE_TYPE_CONVERSION: // 컨버젼 인터페이스의 경우
		{
			// 필요 보석 플래그.
			int Amethyst = 0, Topaz = 0, Opal = 0, Sapphire = 0, Ruby = 0, Emerald = 0, Diamond = 0, RegentDia = 0, Platinum = 0;

			// 필요 젬 검사.
			while (pItem)
			{
				if( pItem->m_bTimeLimit ) // 시간제 아이템 불가처리. 추가. (06-02-24 원석)
				{
					//-- IDS_LIMITED_CANT : 제한된 아이템은 불가능합니다..
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON && GET_TYPE(pItem)!=IETC_GEM && GET_TYPE(pItem)!=IETC_GEMCHIP ) // 없어야 할 종류의 아이템이 있다면...(칩은 매직플래티늄때문. 매직 플래티늄의 구분은 칩이다.)
				{
					// 엉뚱한 아이템이 들어가 있으면...
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // 방어구일때...또는 무기일때.
				{
					if( !CheckCanConversionItem(pItem) ) // 특정 아이템만 변화 가능.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( g_dwLangType == LANG_KOR)
				{
					switch(pItem->m_Vnum)
					{
					case 222:
					case 1960:	// 매직 자수정, 그레이트
						Amethyst++;
						break;
					case 223:
					case 1961:	// 매직 토파즈, 그레이트
						Topaz++;
						break;
					case 224:
					case 1962:	// 매직 오팔, 그레이트
						Opal++;
						break;
					case 225:
					case 1963:	// 매직 사파이어, 그레이트
						Sapphire++;
						break;
					case 226:
					case 1964:	// 매직 루비, 그레이트
						Ruby++;
						break;
					case 227:
					case 1965:	// 매직 에메랄드, 그레이트
						Emerald++;
						break;
					case 228:
					case 1966:	// 매직 다이아몬드, 그레이트
						Diamond++;
						break;
					case 1068:
					case 1967:	// 매직 리젠트 다이아, 그레이트
						RegentDia++;
						break;
					case 978:				// 매직 플레티늄
						Platinum++;
						break;
					}
				}
				else
				{
					if( pItem->m_Vnum == 222 ) // 매직자수정
						++Amethyst;
					if( pItem->m_Vnum == 223 ) // 매직토파즈
						++Topaz;
					if( pItem->m_Vnum == 224 ) // 매직오팔
						++Opal;
					if( pItem->m_Vnum == 225 ) // 매직사파이어
						++Sapphire;
					if( pItem->m_Vnum == 226 ) // 매직루비
						++Ruby;
					if( pItem->m_Vnum == 227 ) // 매직에메랄드
						++Emerald;
					if( pItem->m_Vnum == 228 ) // 매직다이아몬드
						++Diamond;
					if( pItem->m_Vnum == 1068 ) // 매직리젠트다이아몬드
						++RegentDia;
					if( pItem->m_Vnum == 978 ) // 매직플래티늄
						++Platinum;
				}

				pItem = pItem->m_Next;
			}

			if( CheckUpgradeNeed() == -1 ) // 이 함수에는 방어구나 무기 종류가 1개뿐인지 체크해주는 기능도 있다.
			{
				// 방어구나 무기가 2개이상이면...
				return -1;
			}

			// 보석갯수가 맞는지의 체크. (위의 CheckUpgradeNeed를 먼저 거치는 이유는 무기나 방어구 아이템이 하나뿐이란 증거가 되기때문.)
			pItem = g_pRoh->m_ExgInven; // 다시 처음부터 검사준비.

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_WEAPON || GET_TYPE(pItem)==ITYPE_ARMOR ) // 무기또는 방어구 체크.
				{
					break; // 어차피 하나뿐이니 바로 탈출.
				}

				pItem = pItem->m_Next;
			}

			if( pItem ) // 확인 사살.
			{
				// 아이템 레벨 등급별 필요조건을 확인한다.
				if( GET_ITEM_LEVEL(pItem) == 195 ) // 필요조건 3타입.
				{
					if( Amethyst != 1 || Topaz != 1 || Opal != 1 || Sapphire != 1
							|| Ruby != 1 || Emerald != 1 || Diamond != 10 || RegentDia != 0 || Platinum != 2 )
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else if( GET_ITEM_LEVEL(pItem) == 210 ) // 필요조건 4타입.
				{
					if( Amethyst != 1 || Topaz != 1 || Opal != 1 || Sapphire != 1
							|| Ruby != 1 || Emerald != 1 || Diamond != 5 || RegentDia != 0  || Platinum != 7 )
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else if( GET_ITEM_LEVEL(pItem) == 261 ) // 필요조건 5타입.
				{
					if( Amethyst != 1 || Topaz != 1 || Opal != 1 || Sapphire != 1
							|| Ruby != 1 || Emerald != 1 || Diamond != 1 || RegentDia != 1  || Platinum != 10 )
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
			}
		}
		break;

	case INTERFACE_TYPE_CONFUSION: // 랜덤 다시 돌리기
		{
			while (pItem)
			{
				if( pItem->m_bTimeLimit ) // 시간제 아이템 불가처리. 추가. (06-02-24 원석)
				{
					//-- IDS_LIMITED_CANT : 제한된 아이템은 불가능합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON&& pItem->m_Vnum != 228) // 현재는 방어구,무기,다이아 이외에는 지원안함.(후에 가공이 들어가면 iType 플래그로 구분해서 체크하자.)
				{
					// 엉뚱한 아이템이 들어가 있으면...
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem)==ITYPE_ARMOR || GET_TYPE(pItem)==ITYPE_WEAPON ) // 방어구일때...무기도...
				{
					if( !CheckIsConversionItem(pItem,0,2) ) // 특정 아이템(컨버젼 된 아이템만)만 강화 가능.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( pItem->m_Vnum == 228 ) // 보석 갯수 세기.
					++GemCount;

				pItem = pItem->m_Next;
			}

			if( GemCount != NeedItem[UPGRADE_NEED_DIA] )
			{
				//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}

			if( CheckUpgradeNeed() == -1 ) // 이 함수에는 방어구 종류가 1개뿐인지 체크해주는 기능도 있다.
			{
				// 방어구가 2개이상이면...
				return -1;
			}

			// 요구 젬을 구했으므로 젬이 맞게 들어가있는지 검사.
			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=ITYPE_WEAPON ) // 방어구는 빼고 검사. 무기도...
				{
					if( NeedItem[UPGRADE_NEED_DIA] && pItem->m_Vnum != 228) // 다이아를 요구하는데 다이아 이외의 것이 있으면...
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				pItem = pItem->m_Next;
			}
		}
		break;

	case INTERFACE_TYPE_PROCESS: // 가공
		{
			// 필요 보석 플래그.
			int Amethyst = 0, Topaz = 0, Opal = 0, Sapphire = 0, Ruby = 0,
				Emerald = 0, Diamond = 0, RegentDia = 0, Platinum = 0,
				ProcessFire = 0, ProcessWater = 0, ProcessLight = 0, ProcessEarth = 0, ProcessWind = 0;

			// 필요 젬 검사.
			while (pItem)
			{
				if( pItem->m_bTimeLimit ) // 시간제 아이템 불가처리. 추가. (06-02-24 원석)
				{
					//-- IDS_LIMITED_CANT : 제한된 아이템은 불가능합니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LIMITED_CANT), TYPE_NOR_OK, 1 );
					return -1;
				}

				if( GET_TYPE(pItem)!=ITYPE_ARMOR && GET_TYPE(pItem)!=IETC_GEM && GET_TYPE(pItem)!=IETC_GEMCHIP ) // 없어야 할 종류의 아이템이 있다면...(칩은 매직플래티늄때문. 매직 플래티늄의 구분은 칩이다.)
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				else if( GET_TYPE(pItem) == ITYPE_ARMOR) // 방어구일때
				{
					if( !(GET_ATT_FLAG(pItem) & IATT_REFINE) ) // 특정 아이템만 변화 가능.
					{
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}

				if( g_dwLangType == LANG_KOR )
				{
					switch(pItem->m_Vnum)
					{
					case 222:
					case 1960:	// 매직 자수정, 그레이트
						Amethyst++;
						break;
					case 223:
					case 1961:	// 매직 토파즈, 그레이트
						Topaz++;
						break;
					case 224:
					case 1962:	// 매직 오팔, 그레이트
						Opal++;
						break;
					case 225:
					case 1963:	// 매직 사파이어, 그레이트
						Sapphire++;
						break;
					case 226:
					case 1964:	// 매직 루비, 그레이트
						Ruby++;
						break;
					case 227:
					case 1965:	// 매직 에메랄드, 그레이트
						Emerald++;
						break;
					case 228:
					case 1966:	// 매직 다이아몬드, 그레이트
						Diamond++;
						break;
					case 1068:
					case 1967:	// 매직 리젠트 다이아, 그레이트
						RegentDia++;
						break;
					case 978:				// 매직 플레티늄
						Platinum++;
						break;
					}
				}
				else
				{
					if( pItem->m_Vnum == 222 ) // 매직자수정
						++Amethyst;
					if( pItem->m_Vnum == 223 ) // 매직토파즈
						++Topaz;
					if( pItem->m_Vnum == 224 ) // 매직오팔
						++Opal;
					if( pItem->m_Vnum == 225 ) // 매직사파이어
						++Sapphire;
					if( pItem->m_Vnum == 226 ) // 매직루비
						++Ruby;
					if( pItem->m_Vnum == 227 ) // 매직에메랄드
						++Emerald;
					if( pItem->m_Vnum == 228 ) // 매직다이아몬드
						++Diamond;
					if( pItem->m_Vnum == 1068 ) // 매직리젠트다이아몬드
						++RegentDia;
					if( pItem->m_Vnum == 978 ) // 매직플래티늄
						++Platinum;
				}
				pItem = pItem->m_Next;
			}

			if( CheckUpgradeNeed() == -1 ) // 이 함수에는 방어구나 무기 종류가 1개뿐인지 체크해주는 기능도 있다.
			{
				// 방어구나 무기가 2개이상이면...
				return -1;
			}

			// 보석갯수가 맞는지의 체크. (위의 CheckUpgradeNeed를 먼저 거치는 이유는 무기나 방어구 아이템이 하나뿐이란 증거가 되기때문.)
			if( g_pRoh )
				pItem = g_pRoh->m_ExgInven; // 다시 처음부터 검사준비.

			while (pItem)
			{
				if( GET_TYPE(pItem)==ITYPE_ARMOR ) // 무기또는 방어구 체크.
				{
					break; // 어차피 하나뿐이니 바로 탈출.
				}

				pItem = pItem->m_Next;
			}

			pItem = g_pRoh->m_ExgInven;

			while (pItem)
			{
				if( GET_TYPE(pItem) == ITYPE_ETC && GET_SHAPE(pItem) == IETC_ETC) // 정련서 체크.
				{
					pCheckItem = pItem; // 포인터를 기억. 아이템이 정상적으로 하나 뿐이었다면 이 포인터를 이용해서 필요한걸 구한다.
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
				// 보석 종류벌로 체크
				offset = pItem->m_Vnum - 222;
				if( offset >= 0 && offset < 7 ) // 222(매직 자수정) ~ 228(매직 다이아)
				{
					Gem[offset] ++;
				}
				else if( ChechIs261LvWeponItem(pItem) )
				{
					if( pItem->m_PlusNum < 10 || pItem->m_LimitTime != 0 )
					{
						//-- IDS_NPCUPGRADE_PLZUPITEM : 해당 아이템을 올려주세요.
						g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_NPCUPGRADE_PLZUPITEM), TYPE_NOR_OK, 1 );
						return -1;
					}
					count++;
					pCheckItem = pItem;
				}
				else if( pItem->m_Vnum == 1068 )// 매직 리젠트 다이아
				{
					MRDia++;
				}
				else if( pItem->m_Vnum == 1966 )// 그래이트 매직 다이아.
				{
					GMDia++;
				}
				else if( pItem->m_Vnum == 1967 )// 그레이트 매직 리젠트 다이아.
				{
					GMRDia++;
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
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
						//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
				else if( pCheckItem->m_PlusNum == 10 )
				{
					totalGems = Gem[0] + Gem[1] + Gem[2] + Gem[3] + Gem[4] + Gem [5] + Gem[6];
					if( !(totalGems == 7 && GMDia == 1 && GMRDia == 1 && MRDia == 3 && count == 1) )
					{
						//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
						return -1;
					}
				}
			}
			else if( count > 1)
			{
				//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CHECK_ITEM_COUNT), TYPE_NOR_OK, 1 );
				return -1;
			}
			else
			{
				//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
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
				else if( pItem->m_Vnum == 1068 || pItem->m_Vnum == 1967 ) // 둘중에 하나면 무조건
				{
					Dia++;
				}
				else if( pItem->m_Vnum == 1739 || pItem->m_Vnum == 1740 )
				{
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
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
				// 보석 종류벌로 체크
				if( ( pItem->m_Vnum >= 2049 && pItem->m_Vnum <= 2055 )
						//|| ( pItem->m_Vnum >= 2939 && pItem->m_Vnum <= 2941 )
						//|| ( pItem->m_Vnum >= 2942 && pItem->m_Vnum <= 2953 )
						|| ( TRUE == IS_2011_ITEM_WEAPON( pItem->m_Vnum ) )

				  )
				{
					if( pItem->m_PlusNum != 15 || pItem->m_LimitTime != 0 )
					{
						//-- IDS_NPCUPGRADE_MAXLEVEL : 더이상 강화가 불가능한 아이템입니다.
						//m_NeedMoneyText1.SetString( "교환 할 수 없는 아이템입니다.", FALSE );

						//-- IDS_UPGRADE_GODMSG : 교환 할 수 없는 아이템입니다.
						m_NeedMoneyText1.SetString( (char*)G_STRING(IDS_UPGRADE_GODMSG), FALSE );
						return -1;
					}
					count++;
				}
				else
				{
					//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
					return -1;
				}
				pItem = pItem->m_Next;
			}

			if( count != 1)
			{
				//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
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
				//-- IDS_QUEST_ITEM_NOT_MATCH : 다른 종류의 아이템이 섞여있습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
				//g_pNk2DFrame->InsertPopup( "아이템 제작에 필요한 재료를 확인하세요.", TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;	

	case INTERFACE_TYPE_ITEMSEPARATE:
		{
			if ( false == LHIS_SEPERATE()->CheckNeedItem() )
			{
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_ITEM_NOT_MATCH), TYPE_NOR_OK, 1 );
				//g_pNk2DFrame->InsertPopup( "아이템을 분해할 수 없습니다.", TYPE_NOR_OK, 1 );
				return -1;
			}
		}
		break;

	case INTERFACE_TYPE_GMAGICSTONE:
		{
			__int64 nNeedLaim = 0;
			if ( false == LHIS_GMAGICSTONE()->CheckNeedItem( nNeedLaim ) )
			{
				//-- IDS_CHECK_ITEM_COUNT : 아이템 개수를 확인해주세요.
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

		if( m_SpriteAni[m_NowState].m_NowFrame == 0 ) // 첫프레임일때...
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

				m_NowState = UPGRADE_ANI_NON; // 상태 복원.
				m_bSlotLock = FALSE; // 락을 해제.
				m_Result = 0; // 결과값 초기화...

				return;
			}
		}
		else if( timeGetTime() - m_dwStartTickTime > m_dwMinAniTime ) // 적당한 시간이 지났다면...특정 프레임을 기다린다.
		{
			if( m_SpriteAni[m_NowState].m_NowFrame == m_iAniChangeFrame ) // 특정 프레임일때 다음 애니메이션으로 넘어가도록..(프레임 번호는 하드코딩.)
			{
				if( m_Result > 0 && m_Result < MAX_UPGRADE_ANI ) // 범위검사
				{
					m_NowState = m_Result;

					m_SpriteAni[m_NowState].m_AniType = SPRITE_ANITYPE_ONEPLAY; // 이 애니메이션은 한번 반복으로 셋팅.
					m_SpriteAni[m_NowState].SetStart(); // 애니메이션 시작.

					if( g_pDSound && m_pAniSound[m_NowState] )
					{
						g_pDSound->PlayToOutside( m_pAniSound[m_NowState] ); // 상태 전환시에 효과음 플래이를 시킨다.
					}
				}
				else // 이 경우는 뭔가 이상이 있는거다.
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCUPGRADE_ERROR), TYPE_NOR_OK, 1 );

					m_NowState = UPGRADE_ANI_NON; // 상태 복원.
					m_bSlotLock = FALSE; // 락을 해제.
					m_Result = 0; // 결과값 초기화...

					return;
				}
			}
		}
		break;

	case UPGRADE_ANI_SUCCESS: // 성공으로 패킷이 전달 됐을 때...
	case UPGRADE_ANI_FAIL:
	case UPGRADE_ANI_BROKEN:

		if( m_SpriteAni[m_NowState].IsAniEnd() ) // 애니메이션의 한번 플레이가 끝났으면...
		{
			if( g_pNk2DFrame )
			{
				switch( m_iType )
				{
				case INTERFACE_TYPE_UPARMOR:
				case INTERFACE_TYPE_UPWEAPON: // 타입따라 메세지가 바뀌어야 한다.
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_UIMGR_SUCCEED_UPGRADE : ◈ 장비 업그레이드에 성공했습니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_UIMGR_FAIL_UPGRADE : ◈ 장비 업그레이드에 실패했습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_BROKEN:
							//-- IDS_UIMGR_REMOVE_UPGRADE : ◈ 장비가 소멸되었습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), -1); // 채팅창에도 남겨준다.

							break;
						}
					}
					break;

				case INTERFACE_TYPE_CONVERSION : // 타입따라 메세지가 바뀌어야 한다.
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_CONVERSION_SUCC : 컨버젼에 성공하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_SUCC), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_SUCC), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_CONVERSION_FAIL : 컨버젼에 실패하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_FAIL), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_FAIL), -1); // 채팅창에도 남겨준다.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_CONFUSION: // 랜덤 다시 돌리기
					{
						// 이녀석은 무조건 성공이다.
						//-- IDS_EQUIP_STATUS_RESET : 장비의 수치가 새로이 적용되었습니다.
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_EQUIP_STATUS_RESET), TYPE_NOR_OK, 32 );
						g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_EQUIP_STATUS_RESET), -1); // 채팅창에도 남겨준다.
					}
					break;

				case INTERFACE_TYPE_PROCESS: // 타입따라 메세지가 바뀌어야 한다.
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_NPCPROCESS_SUCC : 정련에 성공하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCPROCESS_SUCC), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_NPCPROCESS_SUCC), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_NPCPROCESS_FAIL : 정련에 실패하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NPCPROCESS_FAIL), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_NPCPROCESS_FAIL), -1); // 채팅창에도 남겨준다.
							break;
						}
					}
					break;
				case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_CONVERSION_SUCC : 컨버젼에 성공하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_SUCC), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_SUCC), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_CONVERSION_FAIL : 컨버젼에 실패하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONVERSION_FAIL), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_CONVERSION_FAIL), -1); // 채팅창에도 남겨준다.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_320LV_WEAPON_UPGRADE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_UIMGR_SUCCEED_UPGRADE : ◈ 장비 업그레이드에 성공했습니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_UIMGR_FAIL_UPGRADE : ◈ 장비 업그레이드에 실패했습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_BROKEN:
							//-- IDS_UIMGR_REMOVE_UPGRADE : ◈ 장비가 소멸되었습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_GOD_WEAPON_UPGRADE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//-- IDS_UIMGR_SUCCEED_UPGRADE : ◈ 장비 업그레이드에 성공했습니다
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_SUCCEED_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//-- IDS_UIMGR_FAIL_UPGRADE : ◈ 장비 업그레이드에 실패했습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_FAIL_UPGRADE), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_BROKEN:
							//-- IDS_UIMGR_REMOVE_UPGRADE : ◈ 장비가 소멸되었습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UIMGR_REMOVE_UPGRADE), -1); // 채팅창에도 남겨준다.
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
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UPGRADE_MSG3), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_UPGRADE_MSG4), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_UPGRADE_MSG4), -1); // 채팅창에도 남겨준다.
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
							g_pNk2DFrame->AddChatStr( "Item success", -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Item creation failed", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Item creation failed", -1); // 채팅창에도 남겨준다.
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
							g_pNk2DFrame->AddChatStr( "Buff item creation success", -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Buff item creation failed", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Buff item creation failed", -1); // 채팅창에도 남겨준다.
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
							g_pNk2DFrame->AddChatStr( "Buff Item Gatche Success", -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Buff Item Gatcha Fail", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Buff Item Gatcha Fail", -1); // 채팅창에도 남겨준다.
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
							g_pNk2DFrame->AddChatStr( "Successful exchange of medals", -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Failed medal exchange", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Failed medal exchange", -1); // 채팅창에도 남겨준다.
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
							g_pNk2DFrame->AddChatStr( "Item breakup success", -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							g_pNk2DFrame->InsertPopup( "Item breakdown failed", TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr( "Item breakdown failed", -1); // 채팅창에도 남겨준다.
							break;
						}
					}
					break;

				case INTERFACE_TYPE_GMAGICSTONE:
					{
						switch( m_Result )
						{
						case UPGRADE_ANI_SUCCESS:
							//정제에 성공하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_GMASUK_SUCCESS), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_ITEM_GMASUK_SUCCESS), -1); // 채팅창에도 남겨준다.
							break;

						case UPGRADE_ANI_FAIL:
							//정제에 실패하였습니다.
							g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_GMASUK_FAILED), TYPE_NOR_OK, 32 );
							g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_ITEM_GMASUK_FAILED), -1); // 채팅창에도 남겨준다.
							break;
						}
					}
					break;


				}
			}

			pCMyApp->m_bInverseReturn = TRUE; // 리턴을 막기위해 셋팅

			m_NowState = UPGRADE_ANI_NON; // 상태 복원.
			m_bSlotLock = FALSE; // 락을 해제.
			m_Result = 0; // 결과값 초기화...

			CheckUpgradeNeed(); // 요구 조건을 갱신.
		}

		break;
	}
}

void CControlUpgrade::UpdateTextInfo( int nType, int TextInfoIndex ) // TextInfo 서피스를 바꿔로드해주는 녀석.
{
	char strTemp[256];

	if( m_iTextInfoIndex != m_iOldTextInfoIndex ) // 서피스를 바꿔줘야 하는 경우.
	{
		SAFE_DELETE(m_pTextInfo); // 우선 지우고...

		switch( nType)
		{
		case INTERFACE_TYPE_CONVERSION:
			sprintf( strTemp, "interface/description/NpcUpgrade/Conversion_info%03d.bmp", TextInfoIndex ); // 번호별로 info를 읽어온다.
			break;

		case INTERFACE_TYPE_PROCESS:
			sprintf( strTemp, "interface/description/NpcUpgrade/Process_info%03d.bmp", TextInfoIndex ); // 번호별로 info를 읽어온다.
			break;
		case INTERFACE_TYPE_320LV_WEAPON_CONVERSION:
			sprintf( strTemp, "interface/description/npcupgrade/info_Conversion2_%2d.bmp", TextInfoIndex ); // 번호별로 info를 읽어온다.
			break;
		}

		g_pDisplay->CreateSurfaceFromBitmap( &m_pTextInfo, strTemp );  // 다시 로드.

		m_hWnd =  g_pDisplay->GetHWnd();
		GetClientRect( m_hWnd, &m_ClientRc );

		if( m_pTextInfo )
		{
			m_pTextInfo->Xpos = m_pBackSur->Xpos + 34;
			m_pTextInfo->Ypos = m_pBackSur->Ypos + 80;
		}
	}

	m_iOldTextInfoIndex = m_iTextInfoIndex; // 갱신.
}

BOOL CheckCanNpcUpgradeItem( CItem *pItem ) // 강화가 가능한 아이템인지의 체크용.
{
	if( !pItem )
		return FALSE;

	if( (GET_ATT_FLAG(pItem) & IATT_NO_GEM) )
		return FALSE;

	// m_Vnum : 아이템 DB번호
	if( (pItem->m_Vnum >= 1190 && pItem->m_Vnum <= 1229) || (pItem->m_Vnum >= 1313 && pItem->m_Vnum <= 1326) ) // 현재는 이 범위의 아이템만 가능
		return TRUE;

	if( pItem->m_Vnum >= 1560 && pItem->m_Vnum <= 1579 ) // 300레벨 신규 방어구 추가.
		return TRUE;

	if( pItem->m_Vnum >= 1797 && pItem->m_Vnum <= 1816 ) // [2008/7/7 Theodoric]  레어 방어구 강화 추가
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

BOOL CheckCanConversionItem( CItem *pItem ) // 컨버젼이 가능한 아이템인지의 체크용.
{
	if( !pItem )
		return FALSE;

	// 좀더 빠르게 처리를 위해 점핑 시키자.
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

BOOL CheckIsConversionItem( CItem *pItem, int type ,int sub_type) // 컨버젼되어 나오는 아이템인지의 체크.(마석제한 풀기등에 사용된다.)
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
	case 1: // 320 무기
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