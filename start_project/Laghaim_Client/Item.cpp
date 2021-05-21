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

unsigned short boundIndex[30] = {4, 5, 6, 4, 6, 7, //��
								 0, 1, 5, 0, 5, 4, // ����
								 2, 3, 7, 2, 7, 6, // ������
								 0, 4, 7, 0, 7, 3, // ��
								 1, 2, 6, 1, 6, 5
								}; // ��
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
	// �ð��� �����ۿ� �׸� �߰�.(050711 ����) ///�ð���������
	m_bTimeLimit = FALSE; // �ð����� ���������� �ƴ��� �÷���.
	m_LimitTime = 0; // ���� ���� ���� �ð�. �ʴ���
	m_bExPrice = 0; // Ư�� ���� ���� ������������ �÷���. // m_Special�� ���� �ι�° ��Ʈ�� Ư���������� ������������ ���õǾ��ֵ�. ������ �������� ������ �� ���� 0���� �ʱ�ȭ�Ѵ�.(������������ �״�� �θ� �ٸ��� m_Special ������ ����ϴ� ������ ���۵��� �������Ƿ� �÷��׷� �Űܵΰ� ���� �����ش�.)
	m_PriceType = 0; // Ư�� ���� ���� �������� ���� Ÿ��. ���� �������� BP�������� ���� �˷��ش�. // m_Special�� ���� 3��° ��Ʈ�� 4��° ��Ʈ 2��Ʈ�� ǥ��.(��, ���� Ÿ���� 4���� �Ѿ�� �ȵȴ�.) ������ �������� ������ �� ���� 0���� �ʱ�ȭ�Ѵ�.(������������ �״�� �θ� �ٸ��� m_Special ������ ����ϴ� ������ ���۵��� �������Ƿ� �÷��׷� �Űܵΰ� ���� �����ش�.)
	m_ExPrice = 0; // ��������
	m_LimitCount = -1; // ���� ����

	///�Ź���ȭ�ý���
	m_Special2 = 0; // �ι�° �ɼ� �÷���, ���� �ɼ� �÷��׸����� �����ؼ� Ȯ��. ���� �ɼ� �������� �� �����̴�.( 275�����̻� ������ ���� ���������� �� �⺻ ����  -10% ~ +10%�� �ٰ� �ȴ�. �� ����ġ�� �� ������ ����. )
	m_UpgradeEndurance = 0; // ��ȭ ������. 275���� �̻� ���� ��ȭ �������� ������ ���� ��ȭ�� �ణ�� ������ �ΰ� �Ǿ���.
	m_MaxUpgradeEndurance = 0; // ��ȭ �������� �ʱ�ġ. �����۸��� 70~130�� ���̰� �ִ�. �������� �̿��� ��ȭ �������� ä����ִ� �Ѱ�ġ��� ���� �ȴ�.

	// [6/1/2007 Theodoric] ������ ����
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
	// �ð��� �����ۿ� �׸� �߰�.(050711 ����) ///�ð���������
	m_bTimeLimit = FALSE; // �ð����� ���������� �ƴ��� �÷���.
	m_LimitTime = 0; // ���� ���� ���� �ð�. �ʴ���
	m_bExPrice = 0; // Ư�� ���� ���� ������������ �÷���. // m_Special�� ���� �ι�° ��Ʈ�� Ư���������� ������������ ���õǾ��ֵ�. ������ �������� ������ �� ���� 0���� �ʱ�ȭ�Ѵ�.(������������ �״�� �θ� �ٸ��� m_Special ������ ����ϴ� ������ ���۵��� �������Ƿ� �÷��׷� �Űܵΰ� ���� �����ش�.)
	m_PriceType = 0; // Ư�� ���� ���� �������� ���� Ÿ��. ���� �������� BP�������� ���� �˷��ش�. // m_Special�� ���� 3��° ��Ʈ�� 4��° ��Ʈ 2��Ʈ�� ǥ��.(��, ���� Ÿ���� 4���� �Ѿ�� �ȵȴ�.) ������ �������� ������ �� ���� 0���� �ʱ�ȭ�Ѵ�.(������������ �״�� �θ� �ٸ��� m_Special ������ ����ϴ� ������ ���۵��� �������Ƿ� �÷��׷� �Űܵΰ� ���� �����ش�.)
	m_ExPrice = 0; // ��������
	m_LimitCount = -1; // ���� ���� // -1�̸� ������ �̴�.

	///�Ź���ȭ�ý���
	m_Special2 = 0; // �ι�° �ɼ� �÷���, ���� �ɼ� �÷��׸����� �����ؼ� Ȯ��. ���� �ɼ� �������� �� �����̴�.( 275�����̻� ������ ���� ���������� �� �⺻ ����  -10% ~ +10%�� �ٰ� �ȴ�. �� ����ġ�� �� ������ ����. )
	m_UpgradeEndurance = 0; // ��ȭ ������. 275���� �̻� ���� ��ȭ �������� ������ ���� ��ȭ�� �ణ�� ������ �ΰ� �Ǿ���.
	m_MaxUpgradeEndurance = 0; // ��ȭ �������� �ʱ�ġ. �����۸��� 70~130�� ���̰� �ִ�. �������� �̿��� ��ȭ �������� ä����ִ� �Ѱ�ġ��� ���� �ȴ�.

	// [6/1/2007 Theodoric] ������ ����
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
				//���� �������� �Ҹ��� �� Ư���ϰ� �Ѵ�.
				// �Ÿ��� ������ ���� �ޱ� �ϱ� ���ؼ�
				// �Ҹ��� ���⸦ ���� �ް� �ϱ� ���ؼ� �븻������ ��Ų��.
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
		if (rand() % 20 == 0) // ���� ��ġ�� ������ �ְ� �;... �ٵ� �� ȿ�� ������...
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

		// ���� Ʈ������. - ĳ���͸� �׸��� ����.
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
		pLolo->Render2(GET_D3DDEVICE(), GET_LOLOTEXTR_NUM(this), m_AniFrame, FALSE, FALSE, FALSE, &matWorld, pMatView, FALSE); ///�ѷ��ؽ��ĺ���. ������ ������ ������ �Է��� �ؽ��ĸ� �ѷο� �ݿ����ش�. 07-07-04 ����
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
		// pick buffer ����.
		if (GET_D3DPICK() && m_BoundVert && bDontPick == FALSE)
		{
			GET_D3DPICK()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
			GET_D3DPICK()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
			GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColorPick );
//			GET_D3DPICK()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_wColorIdx); ///��ŷ���׼���. DWORD ������ ��ȯ�Ѱɾ������� �׳� WORD���� �ٷ� �������� �ִ´�. �̷��� �ص� 32767�� ���� ���� ��ŷ�� ������ ��. ��ȯ�� ���� ���̴� ��찡 �ִµ��ؼ� �̷��� ������. (07-01-02 ����)
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

void CItem::SetDiaGemNum(int gem_num)//���̾� ���� ���׷��̵�.
{
	m_PlusNum = gem_num; //���������� �ٷ� �÷�����
	m_GemNum = 0;	  //�����ִ� ������ ����.
}

void CItem::SetTimeLimitItem( int LimitTime ) // �ð��� ���������� ����. ///�ð���������
{
	m_bTimeLimit = TRUE;
	m_LimitTime = LimitTime;
}

void CItem::SetExPrice( int PriceType, int Price ) // Ư�� �������� ���������� ����.
{
	m_bExPrice = TRUE; // Ư�������������������� �÷���.
	m_PriceType = PriceType; // �������� BP������..0�̸� ����. 1�̸� BP
	m_ExPrice = Price; // Ư������.
}

BOOL CItem::IsSameItem( CItem *DestItem ) // ���� ���������� �����ִ� �Լ�. ///���λ���
{
	if( this->m_Vnum == DestItem->m_Vnum
			&& this->m_PlusNum == DestItem->m_PlusNum
			&& this->m_Special == DestItem->m_Special
			&& this->m_Special2 == DestItem->m_Special2
			&& this->m_UpgradeEndurance == DestItem->m_UpgradeEndurance
			&& this->m_MaxUpgradeEndurance == DestItem->m_MaxUpgradeEndurance )
	{
		if( g_SvrType == ST_ADULT_ONLY )	// [6/7/2007 Theodoric] ������ ����
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