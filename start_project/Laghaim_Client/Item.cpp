#include "stdafx.h"


#define D3D_OVERLOADS
#include <d3d.h>
#include "main.h"
#include "UIMgr.h"
#include "Item.h"
#include "ItemProto.h"
#include "ClrAlloc.h"
#include "Capsyong.h"
#include "d3dmath.h"
#include "Land.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "NkCharacter.h"

extern CLolos	*g_FlyItems[];
extern float g_ChaFlyHeight[];

unsigned short boundIndex[30] = {4, 5, 6, 4, 6, 7, //위
								 0, 1, 5, 0, 5, 4, // 왼쪽
								 2, 3, 7, 2, 7, 6, // 오른쪽
								 0, 4, 7, 0, 7, 3, // 앞
								 1, 2, 6, 1, 6, 5
								}; // 뒤
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem()
	: m_MinLevel_Org(0)
{

	m_spbuff[0]= 0;
	m_spbuff[1]= 0;
	m_spbuff[2]= 0;
	m_spbuff[3]= 0;
	m_spbuff[4]= 0;
	m_spbuff[5]= 0;


	m_Index = -1;
	m_Vnum = -1;
	m_ProtoNum = -1;
	m_Next = NULL;
	m_pClrAlloc = NULL;
	m_Dropping = -1;
	m_AniFrame = 0;
	m_dwBaseTime = timeGetTime();
	m_BoundVert = NULL;
	m_PlusNum = 0;
	m_GemNum = 0;
	m_Special = 0;
	// 시간제 아이템용 항목 추가.(050711 원석) ///시간제아이템
	m_bTimeLimit = FALSE; // 시간제한 아이템인지 아닌지 플래그.
	m_LimitTime = 0; // 실제 남은 제한 시간. 초단위
	m_bExPrice = 0; // 특별 가격 지정 아이템인지의 플래그. // m_Special의 상위 두번째 비트에 특별가격지정 아이템인지가 셋팅되어있따. 하지만 서버에서 받은후 이 값은 0으로 초기화한다.(가격지정값을 그대로 두면 다른쪽 m_Special 변수를 사용하는 곳에서 오작동할 수있으므로 플래그로 옮겨두고 값을 지워준다.)
	m_PriceType = 0; // 특별 가격 지정 아이템의 가격 타입. 라임 가격인지 BP가격인지 등을 알려준다. // m_Special의 상위 3번째 비트와 4번째 비트 2비트로 표시.(즉, 지불 타입이 4개를 넘어가선 안된다.) 하지만 서버에서 받은후 이 값은 0으로 초기화한다.(가격지정값을 그대로 두면 다른쪽 m_Special 변수를 사용하는 곳에서 오작동할 수있으므로 플래그로 옮겨두고 값을 지워준다.)
	m_ExPrice = 0; // 지정가격
	m_LimitCount = -1; // 제한 개수

	///신방어구강화시스템
	m_Special2 = 0; // 두번째 옵션 플래그, 기존 옵션 플래그만으론 부족해서 확장. 가공 옵션 정보등이 들어갈 예정이다.( 275레벨이상 방어구에는 방어력 보정개념이 들어가 기본 방어도에  -10% ~ +10%가 붙게 된다. 이 보정치도 이 변수에 들어간다. )
	m_UpgradeEndurance = 0; // 강화 내구도. 275레벨 이상 방어구는 강화 내구도의 개념이 들어가서 강화에 약간의 제약을 두게 되었다.
	m_MaxUpgradeEndurance = 0; // 강화 내구도의 초기치. 아이템마다 70~130의 차이가 있다. 아이템을 이용해 강화 내구도를 채울수있는 한계치라고 보면 된다.

	// [6/1/2007 Theodoric] 내구도 수리
	m_Endurance = 0;
	m_MaxEndurance = 0;

	m_pClrAlloc = NULL;
	m_pTextOutBox = NULL;
	m_Num0 = 0;
	m_Num1 = 0;
	m_Num2 = 0;
	m_Num3 = 0;
	m_Num4 = 0;
	m_Num5 = 0;
	m_Num6 = 0;
	m_Num7 = 0;
	m_Num8 = 0;
	m_Num9 = 0;
	m_Weight = 0;
	m_Int = 0;
	m_Dex = 0;
	m_MinLevel = 0;
	m_nFlyFrame = rand() % CHA_FLY_FRAME;

	m_X = 0;
	m_Y = 0;
	m_Z = 0;

	m_wColorIdx = 0;
	m_dwColorPick = 0x00ffffff;


	m_nDamageUp2 = 0;
	m_nDeadHit = 0;
}

CItem::CItem(CClrAlloc *pClrAlloc)
	: m_MinLevel_Org(0)
{
	m_Index = -1;
	m_Vnum = -1;
	m_ProtoNum = -1;
	m_Next = NULL;
	m_Dropping = -1;
	m_AniFrame = 0;
	m_dwBaseTime = timeGetTime();
	m_BoundVert = NULL;
	m_pClrAlloc = pClrAlloc;

	if (m_pClrAlloc)
		m_pClrAlloc->ColorAlloc(&m_dwColorPick, &m_wColorIdx);
	else
	{
		m_wColorIdx = 0;
		m_dwColorPick = 0x00ffffff;
	}

	m_PlusNum = 0;
	m_GemNum = 0;
	m_Special = 0;
	// 시간제 아이템용 항목 추가.(050711 원석) ///시간제아이템
	m_bTimeLimit = FALSE; // 시간제한 아이템인지 아닌지 플래그.
	m_LimitTime = 0; // 실제 남은 제한 시간. 초단위
	m_bExPrice = 0; // 특별 가격 지정 아이템인지의 플래그. // m_Special의 상위 두번째 비트에 특별가격지정 아이템인지가 셋팅되어있따. 하지만 서버에서 받은후 이 값은 0으로 초기화한다.(가격지정값을 그대로 두면 다른쪽 m_Special 변수를 사용하는 곳에서 오작동할 수있으므로 플래그로 옮겨두고 값을 지워준다.)
	m_PriceType = 0; // 특별 가격 지정 아이템의 가격 타입. 라임 가격인지 BP가격인지 등을 알려준다. // m_Special의 상위 3번째 비트와 4번째 비트 2비트로 표시.(즉, 지불 타입이 4개를 넘어가선 안된다.) 하지만 서버에서 받은후 이 값은 0으로 초기화한다.(가격지정값을 그대로 두면 다른쪽 m_Special 변수를 사용하는 곳에서 오작동할 수있으므로 플래그로 옮겨두고 값을 지워준다.)
	m_ExPrice = 0; // 지정가격
	m_LimitCount = -1; // 제한 개수 // -1이면 무제한 이다.

	///신방어구강화시스템
	m_Special2 = 0; // 두번째 옵션 플래그, 기존 옵션 플래그만으론 부족해서 확장. 가공 옵션 정보등이 들어갈 예정이다.( 275레벨이상 방어구에는 방어력 보정개념이 들어가 기본 방어도에  -10% ~ +10%가 붙게 된다. 이 보정치도 이 변수에 들어간다. )
	m_UpgradeEndurance = 0; // 강화 내구도. 275레벨 이상 방어구는 강화 내구도의 개념이 들어가서 강화에 약간의 제약을 두게 되었다.
	m_MaxUpgradeEndurance = 0; // 강화 내구도의 초기치. 아이템마다 70~130의 차이가 있다. 아이템을 이용해 강화 내구도를 채울수있는 한계치라고 보면 된다.

	// [6/1/2007 Theodoric] 내구도 수리
	m_Endurance = 0;
	m_MaxEndurance = 0;

	m_pTextOutBox = NULL;
	m_nFlyFrame = rand() % CHA_FLY_FRAME;

	m_X = 0;
	m_Y = 0;
	m_Z = 0;

}

CItem::~CItem()
{
	if( m_pClrAlloc )
		m_pClrAlloc->ColorFree(m_wColorIdx);

	if (m_BoundVert)
		delete[] m_BoundVert;

	if (m_pTextOutBox)
		delete m_pTextOutBox;
	m_pTextOutBox = NULL;
}

void CItem::SetColorAlloc(CClrAlloc *pClrAlloc)
{
	m_pClrAlloc = pClrAlloc;

	if (m_pClrAlloc)
	{
		m_pClrAlloc->ColorAlloc(&m_dwColorPick, &m_wColorIdx);
	}
	else
	{
		m_wColorIdx = 0;
		m_dwColorPick = 0x00ffffff;
	}
}

void CItem::SetInitPos(float x, float z, float dir)
{
	m_X = x;
	m_Z = z;
	m_Y = pCMyApp->GetLandPtr()->GetHFHeight(m_X, m_Z);
	m_Dir = dir;
}

void CItem::SetInfo(int item_index, int vnum, int proto_num)
{
	m_Index = item_index;
	m_Vnum = vnum;
	m_ProtoNum = proto_num;
	if (proto_num >= 0)
	{
		CLolos *pLolo = GET_LOLO(this);
		if (pLolo)
		{
			if (m_BoundVert)
				delete[] m_BoundVert;
			m_BoundVert = new D3DVERTEX[8];

			m_BoundVert[0] = D3DVERTEX(pLolo->m_BoundVect[0], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[1] = D3DVERTEX(pLolo->m_BoundVect[1], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[2] = D3DVERTEX(pLolo->m_BoundVect[2], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[3] = D3DVERTEX(pLolo->m_BoundVect[3], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[4] = D3DVERTEX(pLolo->m_BoundVect[4], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[5] = D3DVERTEX(pLolo->m_BoundVect[5], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[6] = D3DVERTEX(pLolo->m_BoundVect[6], D3DVECTOR(0, 0, 0), 0, 0);
			m_BoundVert[7] = D3DVERTEX(pLolo->m_BoundVect[7], D3DVECTOR(0, 0, 0), 0, 0);
		}
	}
}

float drop_height[16] = {0.3f, 6.3f, 11.7f, 16.3f, 20.3f, 23.6f,
						 26.3f, 28.2f, 29.5f, 30.1f, 30.1f,
						 29.3f, 27.9f, 25.9f, 23.1f, 19.7f
						};
void CItem::Render(D3DMATRIX *pMatView, BOOL bVisible, BOOL bDontPick)
{
	if (m_ProtoNum < 0)
		return;

	DWORD dwCurtime = timeGetTime();
	int nInc = 0;
	while ((dwCurtime - m_dwBaseTime) >= 50)
	{
		nInc++;
		m_dwBaseTime += 50;
	}
	if (m_Dropping >= 0)
	{
		m_Dropping -= nInc;
		if (m_Dropping < 0)
			if (g_pDSound)
			{
				//물건 떨어지는 소리는 좀 특이하게 한다.
				// 거리의 영향을 적게 받기 하기 위해서
				// 소리의 감쇄를 적게 받게 하기 위해서 노말라이즈 시킨다.
				D3DVECTOR vPos;
				vPos.x = (m_X - g_pRoh->m_wx) ;
				vPos.y = (m_Y - g_pRoh->m_wy) ;
				vPos.z = (m_Z - g_pRoh->m_wz) ;
				vPos = Normalize(vPos);
				g_pDSound->Play(g_EffectSound[EFF_SND_DROP], vPos.x * 0.05, 0.0f, vPos.z * 0.05f);
			}
	}
	m_AniFrame += nInc;

	if (pCMyApp->m_nCurWorldIdx == WORLD_SKY
			&& !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_X, m_Z))
	{
		m_nFlyFrame += nInc;
		m_Dir += (g_2_PI / 80) * nInc;
		if (rand() % 20 == 0) // 가끔 위치에 변동을 주고 싶어서... 근데 별 효과 없더라...
			m_nFlyFrame++;
		while (m_nFlyFrame >= CHA_FLY_FRAME)
		{
			m_nFlyFrame -= CHA_FLY_FRAME;
		}
	}

	if (bVisible == FALSE)
		return;

	CLolos *pLolo = GET_LOLO(this);
	if (pLolo)
	{
		D3DMATRIX matTrans, matRotate, matWorld, matDrop;

		// 월드 트랜스폼. - 캐릭터를 그리기 위한.
		D3DUtil_SetTranslateMatrix( matTrans, m_X, m_Y, m_Z);
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_Dir);
		if (m_Dropping >= 0)
		{
			D3DVECTOR vDir = D3DVECTOR ( 1, 1, 1);
			D3DUtil_SetRotationMatrix(matDrop, vDir, m_Dropping / 1.2f);
			matTrans._42 += drop_height[m_Dropping];
			D3DMath_MatrixMultiply(matRotate, matRotate, matDrop);
		}
		else if (pCMyApp->m_nCurWorldIdx == WORLD_SKY &&
				 !pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_X, m_Z))
		{
			matTrans._42 += g_ChaFlyHeight[m_nFlyFrame];
		}
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		//GET_D3DDEVICE()->LightEnable(1, TRUE);
		pLolo->Render2(GET_D3DDEVICE(), GET_LOLOTEXTR_NUM(this), m_AniFrame, FALSE, FALSE, FALSE, &matWorld, pMatView, FALSE); ///롤로텍스쳐변경. 아이템 툴에서 별도로 입력한 텍스쳐를 롤로에 반영해준다. 07-07-04 원석
		if (m_Dropping < 0 &&
				pCMyApp->m_nCurWorldIdx == WORLD_SKY &&
				!pCMyApp->m_pUIMgr->m_pLand->IsDrawTile(m_X, m_Z))
		{
			int fly_item = GET_ITEM_WIDTH(this);
			if (fly_item < GET_ITEM_HEIGHT(this))
				fly_item = GET_ITEM_HEIGHT(this);
			if (fly_item >= FLY_ITEM_NUM)
				fly_item = FLY_ITEM_NUM - 1;
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			if( g_FlyItems[fly_item] )
				g_FlyItems[fly_item]->Render(GET_D3DDEVICE());
		}

		///////////////////////////////////////
		// pick buffer 쓰기.
		if (GET_D3DPICK() && m_BoundVert && bDontPick == FALSE)
		{
			GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
//			GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///피킹버그수정. DWORD 형으로 변환한걸쓰지말고 그냥 WORD형을 바로 색상으로 넣는다. 이렇게 해도 32767개 까지 동시 피킹이 가능할 터. 변환중 뭔가 꼬이는 경우가 있는듯해서 이렇게 변경함. (07-01-02 원석)
			GET_D3DPICK()->SetTexture(0, NULL);
			GET_D3DPICK()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX,
												 m_BoundVert, 8,	boundIndex, 30, NULL );
			GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		}
	}
}

void CItem::SetGemNum(int gem_num)
{
	if( TRUE == IsEventEnchantItem() )
	{
		SetDiaGemNum(gem_num);
		return;
	}

	if (gem_num >= 15)
	{
		m_PlusNum = 5;
		m_GemNum = 0;
	}
	else if (gem_num >= 10)
	{
		m_PlusNum = 4;
		m_GemNum = gem_num - 10;
	}
	else if (gem_num >= 6)
	{
		m_PlusNum = 3;
		m_GemNum = gem_num - 6;
	}
	else if (gem_num >= 3)
	{
		m_PlusNum = 2;
		m_GemNum = gem_num - 3;
	}
	else if (gem_num >= 1)
	{
		m_PlusNum = 1;
		m_GemNum = gem_num - 1;
	}
	else
	{
		m_PlusNum = 0;
		m_GemNum = 0;
	}
}

void CItem::SetDiaGemNum(int gem_num)//다이아 보석 업그레이드.
{
	m_PlusNum = gem_num; //보석갯수가 바로 플러스값
	m_GemNum = 0;	  //박혀있는 보석은 없다.
}

void CItem::SetTimeLimitItem( int LimitTime ) // 시간제 아이템으로 셋팅. ///시간제아이템
{
	m_bTimeLimit = TRUE;
	m_LimitTime = LimitTime;
}

void CItem::SetExPrice( int PriceType, int Price ) // 특별 가격지정 아이템으로 셋팅.
{
	m_bExPrice = TRUE; // 특별가격지정아이템인지 플래그.
	m_PriceType = PriceType; // 라임인지 BP인지등..0이면 라임. 1이면 BP
	m_ExPrice = Price; // 특별가격.
}

BOOL CItem::IsSameItem( CItem *DestItem ) // 같은 아이템인지 비교해주는 함수. ///개인상점
{
	if( this->m_Vnum == DestItem->m_Vnum
			&& this->m_PlusNum == DestItem->m_PlusNum
			&& this->m_Special == DestItem->m_Special
			&& this->m_Special2 == DestItem->m_Special2
			&& this->m_UpgradeEndurance == DestItem->m_UpgradeEndurance
			&& this->m_MaxUpgradeEndurance == DestItem->m_MaxUpgradeEndurance )
	{
		if( g_SvrType == ST_ADULT_ONLY )	// [6/7/2007 Theodoric] 내구도 수리
		{
			if( this->m_Endurance    == DestItem->m_Endurance &&
					this->m_MaxEndurance == DestItem->m_MaxEndurance )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CItem::IsEventEnchantItem()
{
	switch(m_Vnum)
	{
	case 3660:
	case 3661:
	case 3662:
	case 3663:
		break;
	default:
		return FALSE;
	}

	return TRUE;
}