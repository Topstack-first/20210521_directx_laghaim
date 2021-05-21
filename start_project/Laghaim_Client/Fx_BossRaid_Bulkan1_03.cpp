#include "stdafx.h"
#define D3D_OVERLOADS


#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Fx_BossRaid_bulkan1_03.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Land.h"


// nate 2004 - 7 �۷ι� ShadowRect���


extern int					g_EffectSound[];
extern float				g_fDSDistance;

CFxBossRaidBulkan1_03::CFxBossRaidBulkan1_03()
	: m_BeforTime(0)
{
	m_pLolo = NULL;
}


CFxBossRaidBulkan1_03::~CFxBossRaidBulkan1_03()
{
	DeleteRes();
}

void CFxBossRaidBulkan1_03::LoadRes()
{
	if (!m_pLolo) // ���� ������ ���ư��� lolo ������Ʈ
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_shield.LOL");
	}




	m_BeforTime = 0;
}


void CFxBossRaidBulkan1_03::DeleteRes()
{
	SAFE_DELETE( m_pLolo );
}


void CFxBossRaidBulkan1_03::FrameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	switch(ef_sort.num)
	{
	case 0: // ����Ʈ ���
		{
			CNkMob* pMob = ef_sort.pMobFrom;

			if( pMob->m_nMobNum == 511 ) // �������̵� ��ĭ ���� 1
			{
				if( pMob->GetMotionState() == MOB_EMPTY2 && pMob->GetCurFrame() > 20 ) // MOB_EMPTY1 == attack2 ����
				{
					ef_sort.vCurPos = D3DVECTOR( pMob->m_wx, 0.0f, pMob->m_wz);	// ���� ����
					ef_sort.vPos = ef_sort.vTargetPos - ef_sort.vCurPos;		// ��ǥ�� ���ϰ� �ϴ� ���� ���� �����.
					ef_sort.vPos = Normalize( ef_sort.vPos );				// ��� ���� ����� �����.
					ef_sort.vSortPos.x = (float)atan2(ef_sort.vPos.z, ef_sort.vPos.x); // ��ǥ ���� ��
					ef_sort.vTargetPos = ef_sort.vCurPos;

					D3DVECTOR vecTemp = ef_sort.vPos * 15.0f;
					ef_sort.vCurPos.x += vecTemp.x;
					ef_sort.vCurPos.z += vecTemp.z;

					vecTemp = ef_sort.vPos * (15.0f - 5.0f); // �ι�° ����Ʈ ��ǥ�� ����Ѵ�.
					ef_sort.vTargetPos.x += vecTemp.x;
					ef_sort.vTargetPos.z += vecTemp.z;

					ef_sort.vPos		*= 5.0f; // �ӵ�
					ef_sort.vSortPos.y	= 0.0f; // ������

					ef_sort.vSortPos.z  = (float)timeGetTime();

					ef_sort.num = 1;

//					if (g_pDSound)
//					{
//						// ���� ��� ����
//						g_pDSound->Play(g_EffectSound[EFF_SND_BOOM4],
//										(ef_sort.vCurPos.x - g_pRoh->m_wx)/(g_fDSDistance*2.0f),
//										0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
//										(ef_sort.vCurPos.z - g_pRoh->m_wz)/g_fDSDistance*2.0f);
//					}
				}
			}
		}
		break;

	case 1: // ����Ʈ ����
		{
			DWORD timeleft = timeGetTime() - (DWORD)ef_sort.vSortPos.z;

			if( timeleft > 100 )
			{
				ef_sort.num = 2;
				break;
			}

			ef_sort.vCurPos.x		+= ef_sort.vPos.x;
			ef_sort.vCurPos.z		+= ef_sort.vPos.z;
			ef_sort.vTargetPos.x	+= ef_sort.vPos.x;
			ef_sort.vTargetPos.z	+= ef_sort.vPos.z;

			ef_sort.vSortPos.y = 0.03f * (float)timeleft;
		}
		break;

	case 2: // v��ƼŬ�� ������ ��ٸ���
		ef_sort.num = 3;
		break;

	case 3:
		ef_sort.nCurFrame = FX_BOSSRAID_BULKAN1_03_FRAMECOUNT;
		// �� ���ǿ� ������ ����Ʈ�� �����Ѵ�.
		break;
	}
}



void CFxBossRaidBulkan1_03::Render(EffectSort &ef_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( ef_sort.num == 3)
	{
		//pCMyApp->m_pFxSet->DeleteEffect( ef_sort.nNum );
		return;
	}

	if( ef_sort.num != 1)
		return;

	DWORD  dwTextureState[3];

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	for(int i=0; i<2; i++)
	{

		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		//////////////////////////////////////////////////////////////////////////


		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);
		D3DUtil_SetIdentityMatrix(matScale);

		D3DXVECTOR3 vTemp;
		switch( i )
		{
		case 0:
			vTemp = ef_sort.vCurPos;
			break;
		case 1:
			vTemp = ef_sort.vTargetPos;
			break;
		}

		D3DUtil_SetTranslateMatrix( matTrans, vTemp.x,
									pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(ef_sort.vCurPos.x, ef_sort.vCurPos.z) + 28.0f + ef_sort.vSortPos.y*8.0f, // ������ Ÿ�� �޷��� �ϱ⶧���� �������� y���� ������ �´�.
									vTemp.z );
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, ef_sort.vSortPos.x ); // vSortPos.x�� ���Ⱚ���� ���.
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

		D3DUtil_SetScaleMatrix(matScale, 1.4f-(1.4f*(float)i)+ef_sort.vSortPos.y,
							   1.4f-(1.4f*(float)i)+ef_sort.vSortPos.y,
							   1.4f-(1.4f*(float)i)+ef_sort.vSortPos.y );
		D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		if( m_pLolo)
			m_pLolo->Render2(GET_D3DDEVICE(), -1, 0, FALSE, FALSE, FALSE, &matWorld);


		///////////////////////////////////////////////////////////////////

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	}
}

