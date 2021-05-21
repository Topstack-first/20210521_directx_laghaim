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
#include "Fx_BossRaid_Hybri2_02.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Land.h"





extern int					g_EffectSound[];
extern float				g_fDSDistance;


CFxBossRaidHybri2_02::CFxBossRaidHybri2_02()
{
	m_pLolo = NULL;
}


CFxBossRaidHybri2_02::~CFxBossRaidHybri2_02()
{
	DeleteRes();
}

void CFxBossRaidHybri2_02::LoadRes()
{
	if (!m_pLolo) // ���� ������ ���ư��� lolo ������Ʈ
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/thorn.LOL");
	}




}


void CFxBossRaidHybri2_02::DeleteRes()
{
	SAFE_DELETE( m_pLolo );
}


void CFxBossRaidHybri2_02::FrameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	EffectSort* pEfSortR = (EffectSort*)ef_sort.effects;

	switch(ef_sort.num)
	{
	case 0: // ����Ʈ ���
		{
			CNkMob* pMob = ef_sort.pMobFrom;

			if( pMob->m_nMobNum == 521 && pMob->GetMotionState() == MOB_EMPTY1 && pMob->GetCurFrame() >= 42 ) // MOB_EMPTY1 == attack2 ����
			{
				D3DMATRIX matRot;
				D3DVECTOR vTmp;

				D3DUtil_SetIdentityMatrix(matRot);
				Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
				D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[1], matRot);

				ef_sort.vCurPos = vTmp;			// �̳��� ������ ���� ��ǥ�� ������ �ִ´�.
				ef_sort.vCurPos.x += pMob->m_wx;
				ef_sort.vCurPos.y += pMob->m_wy;
				ef_sort.vCurPos.z += pMob->m_wz;

				Deco_Particle* pDptc = ef_sort.pParticle;

				// ����Ʈ ����
				for( int i=0 ; i<FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE; i++)
				{
					pDptc[i].direction.x = (float)(((rand()%2) == 1 ? -1 : 1 ) * (rand() % 100));
					pDptc[i].direction.y = -1.0f * (rand() % 20);
					//pDptc[i].direction.y = (float)(((rand()%2) == 1 ? -1 : 1 ) * (rand() % 10));
					pDptc[i].direction.z = (float)(((rand()%2) == 1 ? -1 : 1 ) * (rand() % 100));

					pDptc[i].first_Pos   = ef_sort.vCurPos;
					pDptc[i].first_Pos.y += pDptc[i].direction.y;

					pDptc[i].direction = Normalize(pDptc[i].direction);	// ������ �����Ѵ�. �����ϰ� ����.

					pDptc[i].start_time = (int)(rand() % 100); // ���� Ÿ�̹�. �ణ�� �ð����� �ΰ� ���������� �غ���.
					pDptc[i].life_time  = (int)(rand() % 100) + 100; // ������ Ÿ���� �ƴ϶�... �����ϰ� �̵� Ÿ�̹�

					pDptc[i].scale = 0.001f; // �������� 1���� Ŀ����.
					pDptc[i].fade = 0.0f; // ���İ�.

					if (g_pDSound)
					{
						float x, y, z;
						x = (pDptc[i].direction.x - g_pRoh->m_wx) / g_fDSDistance;
						y = (pDptc[i].direction.y - g_pRoh->m_wy) / g_fDSDistance;
						z = (pDptc[i].direction.z - g_pRoh->m_wz) / g_fDSDistance;

						g_pDSound->Play(g_EffectSound[EFF_SND_SWORD], x, y, z);
					}
				}
				ef_sort.num = 1;
				ef_sort.dwColor = timeGetTime();
			}
		}
		break;

	case 1: // ����Ʈ ����
		{
			Deco_Particle* pDptc = ef_sort.pParticle;
			int beforTime = (int)(timeGetTime() - ef_sort.dwColor);

			int bFinished = 0;

			for( int i=0 ; i<FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE; i++)
			{
				if( beforTime >= pDptc[i].start_time )
				{
					float tempTime = (float)( beforTime - pDptc[i].start_time ) / (float)pDptc[i].life_time; // ������ �ð��� ���� �Ÿ� ���

					if( tempTime > 1.0f )
					{
						bFinished ++;
						continue;
					}
										
					pDptc[i].cur_Pos = ( pDptc[i].first_Pos + (pDptc[i].direction * ( tempTime  * 40.0f) ) ) ;// ��������.
					pDptc[i].scale = tempTime * 3.0f;
					pDptc[i].fade = tempTime * 1.0f;
				}
			}

			if( bFinished == FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE )
			{
				ef_sort.num = 2;
				ef_sort.dwColor = timeGetTime();
			}
		}
		break;

	case 2: // ��� ��ƼŬ �̵��� �������� ������ ����� ��������.
		{
			DWORD fadeTime = 300;

			Deco_Particle* pDptc = ef_sort.pParticle;
			DWORD beforTime = timeGetTime() - ef_sort.dwColor;

			if(beforTime >= fadeTime )
			{
				ef_sort.num = 3;
				break;
			}

			for( int i=0 ; i<FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE; i++)
			{
				//pDptc[i].cur_Pos = ( pDptc[i].cur_Pos - (pDptc[i].direction * ( ((float)beforTime / (float)fadeTime )  * 1.0f) ) ) ;// ��������.
				pDptc[i].fade  -= ((float)beforTime / (float)fadeTime ) * 1.0f;
				pDptc[i].scale = 1.5f - ((float)beforTime / (float)fadeTime );
			}

		}
		break;

	case 3:
		// �� ���ǿ� ������ ����Ʈ�� �����Ѵ�.
		break;
	}
}



BOOL CFxBossRaidHybri2_02::Render(EffectSort &ef_sort)
{
	if( ef_sort.num == 3 )
		return TRUE;

	if( !GET_D3DDEVICE() && ef_sort.num == 0 )
		return FALSE;

	Deco_Particle* pDptc = ef_sort.pParticle;


	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matRotX;
	D3DMATRIX matRotY;
	D3DMATRIX matRotZ;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

	//////////////////////////////////////////////////////////////////////////
	//int i=0;

	for( int i=0 ; i<FX_BOSSRAID_HYBRIDER2_02_MAX_PARTICLE; i++)
	{
		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);
		D3DUtil_SetIdentityMatrix(matRotX);
		D3DUtil_SetIdentityMatrix(matRotY);
		D3DUtil_SetIdentityMatrix(matRotZ);
		D3DUtil_SetIdentityMatrix(matScale);
		D3DUtil_SetIdentityMatrix(matWorld);

		// �ð����� ����

		if( ef_sort.num == 1)
			D3DUtil_SetScaleMatrix( matScale, pDptc[i].scale, 1.0f, 1.0f);
		else
			D3DUtil_SetScaleMatrix( matScale, 4.0f, pDptc[i].scale, pDptc[i].scale);

		D3DMath_MatrixMultiply( matWorld, matWorld, matScale );

		//D3DVECTOR vTmp = pDptc[i].direction;
		//D3DVECTOR vTmp = pDptc[i].cur_Pos - ef_sort.vCurPos;
		//D3DVECTOR vTmp = pDptc[i].direction - D3DVECTOR(0.0f, 0.0f, 0.0f);

		//////////////////////////////////////////////////////////////////////////

		//float Yaw = atan2f(vTmp.z, vTmp.x); // y ������Ʈ ���� ��.
		//float Pitch = acos( DotProduct( D3DVECTOR(vTmp.x, vTmp.y, vTmp.z), D3DVECTOR(vTmp.x, 0.0f, vTmp.z) ) ); // x ������Ʈ ���� ��.
		//float Roll = 0.0f;//(float)(effect_sort.nCurFrame)*0.5f; // z ������Ʈ ���� ��. ���ư����� ���ư���.

//		float rY	= atan2f(vTmp.x, vTmp.z);
//		float rX	= atan2f(vTmp.x*cosf(rY) + vTmp.z * sinf(rY), vTmp.y); - (3.14f/2.0f);
//		float rZ	= 0.0f;

//		D3DUtil_SetRotateXMatrix(matRotX,	rX);
//		D3DUtil_SetRotateYMatrix(matRotY,	rY);
		//D3DUtil_SetRotateZMatrix(matRotZ,	rZ);
//		D3DMath_MatrixMultiply(matRotate, matRotX, matRotY);
		//D3DMath_MatrixMultiply(matRotate, matRotate, matRotZ);
//		D3DMath_MatrixMultiply(matWorld, matWorld, matRotate);

		/////////////////////////////////////////////////////////////////////////

//		D3DUtil_SetRotationMatrix(matRotate, pDptc[i].direction, 3.14f );
//		Nk3DUtil_SetRotateCCWYMatrix(matRotate,);
//		D3DMath_MatrixMultiply(matWorld, matWorld, matRotate);

//		matWorld._41 = pDptc[i].cur_Pos.x;
//		matWorld._42 = pDptc[i].cur_Pos.y;
//		matWorld._43 = pDptc[i].cur_Pos.z;

		float fTmp = (float)atan2(pDptc[i].direction.z, pDptc[i].direction.x);

		Nk3DUtil_SetRotateCCWYMatrix(matRotate,fTmp );
		D3DMath_MatrixMultiply(matWorld, matWorld, matRotate);

		D3DUtil_SetTranslateMatrix( matTrans, pDptc[i].cur_Pos );
		D3DMath_MatrixMultiply( matWorld, matWorld, matTrans );

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_pLolo->Render(GET_D3DDEVICE(),0, FALSE, FALSE, FALSE, &matWorld, NULL, FALSE, TRUE, (BYTE)(pDptc[i].fade * 255) );
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return FALSE;
}

