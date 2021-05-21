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
#include "Fx_BossRaid_bulkan1_02.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Land.h"




extern int					g_EffectSound[];
extern float				g_fDSDistance;

CFxBossRaidBulkan1_02::CFxBossRaidBulkan1_02()
	: m_fScale(0)
{
	memset(m_LoloTextrIndex, 0, sizeof(m_LoloTextrIndex));

	m_pLolo = NULL;

	m_Smoke = new CRectPlane;
	if( m_Smoke )
		m_Smoke->Create(30.0f, 30.0f, FALSE);
}


CFxBossRaidBulkan1_02::~CFxBossRaidBulkan1_02()
{
	DeleteRes();
}

void CFxBossRaidBulkan1_02::LoadRes()
{
	PFILE *fpLolo = NULL;

	if (!m_pLolo) // ���� ������ ���ư��� lolo ������Ʈ
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/eff_moving_energe.LOL");
	}




	g_pCapsyongTexture->SetTexturePath("data/effect/textr/");

	m_LoloTextrIndex[0] = g_pCapsyongTexture->CreateTextureFromFile("moving_energe02.sek");
	m_LoloTextrIndex[1] = g_pCapsyongTexture->CreateTextureFromFile("moving_energe03.sek");
}


void CFxBossRaidBulkan1_02::DeleteRes()
{
	SAFE_DELETE( m_pLolo );
	SAFE_DELETE( m_Smoke );
}


void CFxBossRaidBulkan1_02::FrameMove( EffectSort &ef_sort, int nAddFrame)
{
	LoloFrameMove( ef_sort, nAddFrame );
	//ParticleFRameMove( ef_sort, nAddFrame );
}



void CFxBossRaidBulkan1_02::LoloFrameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	switch(ef_sort.num)
	{
	case 0: // ����Ʈ ���
		{
			CNkMob* pMob = ef_sort.pMobFrom;

			if( pMob->m_nMobNum == 511 ) // �������̵� ��ĭ ���� 1
			{
				if( pMob->GetMotionState() == MOB_EMPTY1 && pMob->GetCurFrame() > 25 ) // MOB_EMPTY1 == attack2 ����
				{

					D3DMATRIX matRot;
					D3DVECTOR vTmp;

					vTmp = D3DVECTOR(0.0f, 0.0f, 0.0f);
					D3DUtil_SetIdentityMatrix(matRot);
					Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
					D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[0], matRot);

					ef_sort.vCurPos = vTmp;
					ef_sort.vCurPos.x += pMob->m_wx;
					ef_sort.vCurPos.y = 0.0f; // += pMob->m_wy;
					ef_sort.vCurPos.z += pMob->m_wz;

//					vTmp = D3DVECTOR(0.0f, 0.0f, 0.0f);
//					D3DUtil_SetIdentityMatrix(matRot);
//					Nk3DUtil_SetRotateCCWYMatrix(matRot,pMob->m_dir);
//					D3DMath_VectorMatrixMultiply(vTmp, pMob->m_pCapSyong->m_LineVector[1], matRot);

//					ef_sort.vTargetPos = vTmp;
//					ef_sort.vTargetPos.x += pMob->m_wx;
//					ef_sort.vTargetPos.y = 0.0f; // += pMob->m_wy;
//					ef_sort.vTargetPos.z += pMob->m_wz;


					//ef_sort.vCurPos	= D3DVECTOR( pMob->m_wx, 0.0f, pMob->m_wz);	// ���� ����
					//////////////////////////////////////////////////////////////////////////
					// ������ ��ǥ�� ���ؼ� �߻�
					//ef_sort.vPos = ef_sort.vTargetPos - ef_sort.vCurPos;							// ��ǥ�� ���ϰ� �ϴ� ���� ���� �����.
					//////////////////////////////////////////////////////////////////////////
					// ������ ��Ʈ�� �� �������� �߻�
					ef_sort.vPos = ef_sort.vCurPos - D3DVECTOR(pMob->m_wx, 0.0f, pMob->m_wz);		// ��ǥ�� ���ϰ� �ϴ� ���� ���� �����.
					//////////////////////////////////////////////////////////////////////////
					ef_sort.vPos = Normalize( ef_sort.vPos );						// ��� ���� ���ͷ� �����.

					ef_sort.vSortPos.x = (float)atan2(ef_sort.vPos.z, ef_sort.vPos.x); // ��ǥ ���� ��

					m_vCurPos = D3DVECTOR(0.0f, 0.0f, 0.0f);
					m_vCurPos.x = ef_sort.vCurPos.x	+= ef_sort.vPos.x;
					m_vCurPos.z = ef_sort.vCurPos.z	+= ef_sort.vPos.z;
					ef_sort.vTargetPos = ef_sort.vCurPos;
					//ef_sort.vPos		*= 10.0f; // �⺻ �ӵ� �ӵ�
					m_fScale = ef_sort.vSortPos.y = 4.0f;  // ���� ������
					ef_sort.dwColor		= timeGetTime();

					ef_sort.num = 1;

//					if (g_pDSound)
//					{
//						// ���� ��� ����
//						g_pDSound->Play(g_EffectSound[EFF_SND_EXPLO],
//										(m_vCurPos.x - g_pRoh->m_wx)/(g_fDSDistance*2.0f),
//										0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
//										(m_vCurPos.z - g_pRoh->m_wz)/(g_fDSDistance*2.0f));
//					// ���� ����� �Ⱑ ����Ǿ� ������ ����Ʈ
//						//g_pDSound->Play(g_EffectSound[EFF_SND_IRON_SKIN],
//						//				0.0f, //(m_vCurPos.x - g_pRoh->m_wx)/g_fDSDistance,
//						//				0.0f, //(vCurPos.y - g_pRoh->m_wy)/g_fDSDistance,
//						//				0.0f); //(m_vCurPos.z - g_pRoh->m_wz)/g_fDSDistance);
//
//					}
				}
			}
		}
		break;

	case 1: // ����Ʈ ����
		{
			/*
						// ���������� ��� ���� ��
						float dist = DistVecToVec( pEfsort[0].vTargetPos, pEfsort[0].vCurPos );

						if( dist >= 400.0f )
						{
							ef_sort.num = 2;
							break;
						}

						ef_sort.vCurPos.x  += ef_sort.vPos.x;
						ef_sort.vCurPos.z  += ef_sort.vPos.z;
						ef_sort.vSortPos.y += 0.05f; // ������
			*/

			// �ð����� ����
			DWORD dwTime = timeGetTime() - ef_sort.dwColor ;
			float fTagetRange = 400.0f;
			float fReachTime  = 500.0f;

			if( dwTime >= (DWORD)fReachTime )
			{
				ef_sort.num = 2;
				break;
			}

			// ������ �Ÿ� ���
			m_vCurPos		= D3DVECTOR(0.0f, 0.0f, 0.0f);
			D3DVECTOR vCur	= ef_sort.vPos * (fTagetRange * ( (float)dwTime / fReachTime));
			m_vCurPos.x		= ef_sort.vCurPos.x + vCur.x;
			m_vCurPos.z		= ef_sort.vCurPos.z + vCur.z;

			//������ ������ ���
			m_fScale = ef_sort.vSortPos.y + ( 6.0f * ( (float)dwTime / fReachTime ));
		}
		break;

	case 2: // v��ƼŬ�� ������ ��ٸ���
		ef_sort.num = 3;
		break;

	case 3:
		ef_sort.nCurFrame = FX_BOSSRAID_BULKAN1_02_FRAMECOUNT;
		// �� ���ǿ� ������ ����Ʈ�� �����Ѵ�.
		break;
	}
}

void CFxBossRaidBulkan1_02::ParticleFRameMove( EffectSort &ef_sort, int nAddFrame )
{
	int i=0;

	switch(ef_sort.num)
	{
	case 0: // ����Ʈ ���
		break;

	case 1: // ����Ʈ ����
	case 2:
		for(i=0; i< FX_BOSSRAID_BULKAN1_02_MAX_PARTICLE; i++ )
		{
			if(ef_sort.num != 2 && ef_sort.pParticle[i].texIndex == 0 ) // ������ ���ư��� ����Ʈ�� ������ �ʾҴٸ� ��ŰŬ�� ��� �������ش�.
			{



				ef_sort.pParticle[i].texIndex = 1; // ������ ��ƼŬ �̴�.
			}
			else // ������ ��ƼŬ���� �������ش�.
			{

//				if() // �������� ��ƼŬ�� �׾����� ȭ���� ������.
//					ef_sort.pParticle[i].texIndex == 0 // ������ ���� ��ƼŬ�̴�.
			}
		}
		break;

	case 3: // ����Ʈ ��
		// �����ǿ� ������ ����Ʈ�� �����Ѵ�.
		break;
	}
}

void CFxBossRaidBulkan1_02::Render(EffectSort &ef_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	if( ef_sort.num == 3)
		return;

	if( ef_sort.num != 1)
		return;

	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	float scale = 0.2f;


	EarthCrackRender( ef_sort );
	SmokeRender( ef_sort );

	for( int i=0 ; i<FX_BOSSRAID_BULKAN1_02_MAX_COUNT; i++)
	{
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
		GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );

		//////////////////////////////////////////////////////////////////////////

		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);
		D3DUtil_SetIdentityMatrix(matScale);
		/*
				// ���������� ����
				D3DUtil_SetTranslateMatrix( matTrans, pEfsort[i].vCurPos.x,
											pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(pEfsort[i].vCurPos.x, pEfsort[i].vCurPos.z), // ������ Ÿ�� �޷��� �ϱ⶧���� �������� y���� ������ �´�.
											pEfsort[i].vCurPos.z );
				Nk3DUtil_SetRotateCCWYMatrix( matRotate, pEfsort[i].vSortPos.x ); // vSortPos.x�� ���Ⱚ���� ���.
				D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

				D3DUtil_SetScaleMatrix( matScale, 2.0f+pEfsort[i].vSortPos.y, 2.0f+pEfsort[i].vSortPos.y, 2.0f+pEfsort[i].vSortPos.y/2.0f );
				D3DMath_MatrixMultiply( matWorld, matScale, matWorld );
		*/

		// �ð����� ����
		D3DUtil_SetTranslateMatrix( matTrans, m_vCurPos.x,
									pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(m_vCurPos.x, m_vCurPos.z), // ������ Ÿ�� �޷��� �ϱ⶧���� �������� y���� ������ �´�.
									m_vCurPos.z );
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, ef_sort.vSortPos.x ); // vSortPos.x�� ���Ⱚ���� ���.
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

		scale = scale + ((float)(timeGetTime()%2)+1) * 0.2f;
		D3DUtil_SetScaleMatrix( matScale, m_fScale + ( scale * i), m_fScale + ( scale * i), m_fScale + ( scale * i));
		D3DMath_MatrixMultiply( matWorld, matScale, matWorld );

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

		if( m_pLolo)
		{
			int index = timeGetTime() % 3;
			index = (index + i) % 3;
			switch( index )
			{
			case 0:
			case 1:
				//case 2:
				m_pLolo->Render2(GET_D3DDEVICE(), m_LoloTextrIndex[index], 0, FALSE, FALSE, FALSE, &matWorld );
				break;
			case 2:
				m_pLolo->Render2(GET_D3DDEVICE(), -1, 0, FALSE, FALSE, FALSE, &matWorld );
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////

//		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
//		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
//		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );


		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	}


}


void CFxBossRaidBulkan1_02::SmokeRender(EffectSort &ef_sort)
{
	DWORD LiveTime = timeGetTime() - ef_sort.dwColor;

	if( LiveTime >= 1000 )
		return;

	float up_height  = (LiveTime / 1000.0f ) * 10.0f + 4.0f;
	float smoke_fade = 0.9f - ((LiveTime / 1000.0f ) * 0.9f );

	DWORD dwTextureState[3];
	DWORD dwLighting, dwZWrite, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HELLCRY_SMOKE ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	ef_sort.vTargetPos.y = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(ef_sort.vTargetPos.x, ef_sort.vTargetPos.z) + 5.0f;

	if( m_Smoke )
	{
		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(3.0f, 3.0f, 3.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x , ef_sort.vTargetPos.y + up_height* 1.5f, ef_sort.vTargetPos.z);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 2.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x + 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(4.0f, 4.0f, 4.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z + 15.0f);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 3.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x - 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 2.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z - 15.0f);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(4.0f, 5.0f, 4.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x + 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z + 15.0f);
		m_Smoke->Render();

		m_Smoke->SetColor(1.0f, 1.0f, 1.0f, smoke_fade);
		m_Smoke->SetBillBoard(vViewVec, FALSE, 0.0f);
		m_Smoke->Scale(2.0f, 2.0f, 2.0f);
		m_Smoke->SetTranslationXYZ(ef_sort.vTargetPos.x - 15.0f, ef_sort.vTargetPos.y + up_height, ef_sort.vTargetPos.z - 15.0f);
		m_Smoke->Render();
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	// RenderState �ǵ�����
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	//GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
}


void CFxBossRaidBulkan1_02::EarthCrackRender(EffectSort &ef_sort)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR	vDrawPos;
	vDrawPos = ef_sort.vTargetPos;

	// �۷ι� ShadowRect����Ͽ� ������ �׸���.
	if( g_pShadowRect )
	{
		// nate 2004 - 7 - image
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANCE2 ] ) ;
		// nate 2004 - 7 �۷ι� ShadowRect���
		g_pShadowRect->SetTileCoord(vDrawPos, 120, FALSE);		// �οﰪ�� �ð��ΰ� �ƴѰ�
		g_pShadowRect->Render(lpSurface, FALSE);	// �οﰪ�� ����׸����ΰ� �ƴѰ�?
	}
}
