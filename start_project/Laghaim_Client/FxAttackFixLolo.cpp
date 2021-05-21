#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "FxAttackFixLolo.h"





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAttackFixLolo::CFxAttackFixLolo()
{
	for( int i = 0 ; i < EFF_NUM ; i ++ )
		m_pLolo[ i ] = NULL;

	light = new CRectPlane;
	if( light )
		light->Create( 10.0f, 10.0f, TRUE );
	m_numTLight = 0;
	m_fScale = 0.0f;
	m_nOldFrame = 0;
}

CFxAttackFixLolo::~CFxAttackFixLolo()
{
	DeleteRes();

	SAFE_DELETE(light);
}

void CFxAttackFixLolo::LoadRes()
{

	if( !m_pLolo[SPORE] )
	{
		m_pLolo[SPORE]			= new CLolos;
		m_pLolo[SPORE]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[SPORE]->Read("data/effect/lolos/spore0001.LOL");
	}
	if( !m_pLolo[ELECTRICSHOT] )
	{
		m_pLolo[ELECTRICSHOT]	= new CLolos;
		m_pLolo[ELECTRICSHOT]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[ELECTRICSHOT]->Read("data/effect/lolos/electricshot001.LOL");
	}
	if( !m_pLolo[SNAKEBOSS] )
	{
		m_pLolo[SNAKEBOSS]		= new CLolos;
		m_pLolo[SNAKEBOSS]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[SNAKEBOSS]->Read("data/effect/lolos/SerpentQueen_effect.LOL");
	}
	if( !m_pLolo[SNAKEBOSS_MUL] )
	{
		m_pLolo[SNAKEBOSS_MUL]	= new CLolos;
		m_pLolo[SNAKEBOSS_MUL]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[SNAKEBOSS_MUL]->Read("data/effect/lolos/snakemonster_01.LOL");
	}
	if( !m_pLolo[FROZEN_SHOT] )
	{
		m_pLolo[FROZEN_SHOT]	= new CLolos;
		m_pLolo[FROZEN_SHOT]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[FROZEN_SHOT]->Read("data/effect/lolos/FrozenShot.LOL");
	}
	if( !m_pLolo[LEGKNOCK] )
	{
		m_pLolo[LEGKNOCK]		= new CLolos;
		m_pLolo[LEGKNOCK]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[LEGKNOCK]->Read("data/effect/lolos/legknock_att00-eff.LOL");
	}
	if( !m_pLolo[LEGKNOCK_MUL] )
	{
		m_pLolo[LEGKNOCK_MUL]	= new CLolos;
		m_pLolo[LEGKNOCK_MUL]->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo[LEGKNOCK_MUL]->Read("data/effect/lolos/legknock_att01-eff.LOL");
	}

}

void CFxAttackFixLolo::DeleteRes()
{
	for( int i = 0 ; i < EFF_NUM ; i ++ )
	{
		SAFE_DELETE(m_pLolo[ i ]);
	}
}

BOOL CFxAttackFixLolo::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matWorld, matRotate, matTrans;
	D3DVECTOR vDrawPos;


	// Ÿ�� ��ǥ�� ���Ѵ�.
	vDrawPos = effect_sort.vPos;
	if( effect_sort.Index == 83 )
		vDrawPos.y += 9.0f;

	int nFrame = effect_sort.nCurFrame;

	// ������ ������ ���� ����..
	if( effect_sort.Index == 135 && nFrame >= 11 && timeGetTime()-effect_sort.dwColor <= 3000 ) // 3�ʰ��� ������ �ʵ���..
	{
		effect_sort.nCurFrame = nFrame = 22; // 20 �����ӿ��� �����.
	}

	if( effect_sort.pMobFrom && effect_sort.Index == 4010 ) ///�δ�1
	{
		if( effect_sort.pMobFrom->GetMotionState() != MOB_ATTACK ) // �ִϸ��̼��� �ٲ���ٸ�
			return TRUE; // �����ش�.

		effect_sort.nCurFrame = effect_sort.pMobFrom->GetCurFrame(); // ���� �����ӿ� �����ش�.(Ÿ�ֻ̹� ��߳���...)
		vDrawPos = D3DVECTOR( effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy, effect_sort.pMobFrom->m_wz ); // �ֽ���ǥ�� ����
	}


	if( effect_sort.pMobFrom
			&& ( effect_sort.Index == 4007 || effect_sort.Index == 4010 || effect_sort.Index == 4011 ) ///�δ�1
	  )	// ���Ⱚ ���
	{
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, effect_sort.pMobFrom->m_dir);
		D3DUtil_SetTranslateMatrix(matTrans, vDrawPos);
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );

		if( effect_sort.Index == 4010 ) // ���׳� ũ�⸦ ���Ƿ� Ű���� ���� ���� ����Ʈ�� Ű���ش�.
		{
			float ExceptionScale = GetMobExceptioncale( effect_sort.pMobFrom ); // �������� Ŭ���̾�Ʈ �������� ����Ǵ� ��� ������ ���� �������ִ� �Լ�.

			D3DMATRIX		matS;
			D3DUtil_SetScaleMatrix(matS, ExceptionScale, ExceptionScale, ExceptionScale);
			D3DMath_MatrixMultiply(matWorld, matS, matWorld);
		}
	}
	else if( effect_sort.Index == 135 ) // ������ ������ ���� ��ġ ���.
	{
		if( effect_sort.pNkChaTo )
		{
			D3DMATRIX		matTemp;
			D3DMATRIX		matTemp2;
			D3DUtil_SetIdentityMatrix( matTemp );
			D3DUtil_SetIdentityMatrix( matTemp2 );

			CBiped *pBiped = effect_sort.pNkChaTo->GetBiped();
			int CurFrame = effect_sort.pNkChaTo->Get_m_CurFrame();

			if( pBiped->m_bHalfFrame )
			{
				if( CurFrame % 2 )
				{
					g_pRoh->GetHalfMat( matTemp, pBiped, effect_sort.subNum, CurFrame / 2 ); // �Լ��� ��������.
				}
				else
				{
					matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame / 2];
				}
			}
			else
			{
				matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame];
			}

			vDrawPos = D3DVECTOR( 0.0f,0.0f,0.0f ); // �ʱ�ȭ�ϰ�.
			D3DUtil_SetTranslateMatrix(matWorld, vDrawPos);
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);

			Nk3DUtil_SetRotateCCWYMatrix( matTemp2, effect_sort.pNkChaTo->m_dir );
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);

			D3DUtil_SetTranslateMatrix( matTemp2
										, D3DVECTOR(effect_sort.pNkChaTo->m_wx,effect_sort.pNkChaTo->m_wy,effect_sort.pNkChaTo->m_wz) ); // ����󿡼��� ��ġ�� �̵���Ű��...
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);
		}
		else if( effect_sort.pMobTo )
		{
			D3DMATRIX		matTemp;
			D3DMATRIX		matTemp2;
			D3DUtil_SetIdentityMatrix( matTemp );
			D3DUtil_SetIdentityMatrix( matTemp2 );

			CBiped *pBiped = effect_sort.pMobTo->GetBiped( effect_sort.pMobTo->GetMotionState() );
			int CurFrame = effect_sort.pMobTo->GetCurFrame();

			if( pBiped->m_bHalfFrame )
			{
				if( CurFrame % 2 )
				{
					g_pRoh->GetHalfMat( matTemp, pBiped, effect_sort.subNum, CurFrame / 2 ); // �Լ��� ��������.
				}
				else
				{
					matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame / 2];
				}
			}
			else
			{
				matTemp = pBiped->m_AniMat[effect_sort.subNum].m_Mats[CurFrame];
			}

			vDrawPos = D3DVECTOR( 0.0f,0.0f,0.0f ); // �ʱ�ȭ�ϰ�.
			D3DUtil_SetTranslateMatrix(matWorld, vDrawPos);

			// Ŭ���̾�Ʈ���� ������ �Ǵ� ������ ���� ���⼭�� ������ ó���� ������Ѵ�.
			float ExceptionScale = GetMobExceptioncale( effect_sort.pMobTo ); // �������� Ŭ���̾�Ʈ �������� ����Ǵ� ��� ������ ���� �������ִ� �Լ�.
			if( ExceptionScale != 1.0f ) // 1.0f�̸� ���� ���� �ʿ������ ��ŵ.
			{
				// ������ ��Ʈ������ ������ ���� �̵����� �������� �̵������ش�.
				D3DVECTOR vDrawPos2 = D3DVECTOR( matTemp._41,matTemp._42,matTemp._43 );
				vDrawPos2 *= ExceptionScale;
				matTemp._41 = vDrawPos2.x;
				matTemp._42 = vDrawPos2.y;
				matTemp._43 = vDrawPos2.z;
			}

			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp);


			Nk3DUtil_SetRotateCCWYMatrix( matTemp2, effect_sort.pMobTo->m_dir );
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);

			D3DUtil_SetTranslateMatrix( matTemp2
										, D3DVECTOR(effect_sort.pMobTo->m_wx,effect_sort.pMobTo->m_wy,effect_sort.pMobTo->m_wz) ); // ����󿡼��� ��ġ�� �̵���Ű��...
			D3DMath_MatrixMultiply(matWorld, matWorld, matTemp2);
		}
		else
			return TRUE; // ���� �پ����� ����� ���������� �ٷ� �����.

		//������ ����
		D3DMATRIX		matS;
		if( nFrame >= 35 ) // ������ ������ �� ũ�� ����.(�׷��ȿ��� �����༭...)
			D3DUtil_SetScaleMatrix(matS, 6.0f, 6.0f, 6.0f);
		else
			D3DUtil_SetScaleMatrix(matS, 3.0f, 3.0f, 3.0f);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}
	else
		D3DUtil_SetTranslateMatrix(matWorld, vDrawPos);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	DWORD dwLighting, dwAmbient, dwZWrite;
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

	if( effect_sort.Index == 4006 && m_pLolo[SPORE] )
		m_pLolo[SPORE]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 83 && m_pLolo[ELECTRICSHOT] )
		m_pLolo[ELECTRICSHOT]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4007 && m_pLolo[SNAKEBOSS] )
		m_pLolo[SNAKEBOSS]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4009 && m_pLolo[SNAKEBOSS_MUL] )
		m_pLolo[SNAKEBOSS_MUL]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 135 && m_pLolo[FROZEN_SHOT] ) // �޸� ��ų ������ ��
		m_pLolo[FROZEN_SHOT]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4010 && m_pLolo[LEGKNOCK] && effect_sort.pMobFrom ) ///�δ�1
		m_pLolo[LEGKNOCK]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	else if( effect_sort.Index == 4011 && m_pLolo[LEGKNOCK_MUL] ) ///�δ�1
	{
		// �� ���� �ߵ��߿� ī�޶� ���Ʒ��� ��� ���� ������ �ش�.
		if( effect_sort.nCurFrame > 30 ) // ���� ģ �ĺ��� ��鸮��...
		{

			D3DVECTOR Eye;
			pCMyApp->GetEyePos( Eye );
			D3DVECTOR At;
			pCMyApp->GetEyeAtPos( At );
			float Adjust = RandomNum( -3.0f, 3.0f );

			Eye.y += Adjust;
			At.y += Adjust;

			pCMyApp->SetCameraInitPos( Eye, At ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.

			g_pRoh->m_wy += Adjust; // �̵��߿� ��鸮���� �̰͵� ���� ������Ѵ�.(HeroMoving �Լ����� �� ī�޶� ������ ���ΰ� wy�� ������� ��ǥ�� �ٲپ������.)(�̷��� �ٲ㵵 wy �� �ڵ� �������� �ǵ�����)
		}


		m_pLolo[LEGKNOCK_MUL]->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
	}


	if( effect_sort.Index == 83 )
		RenderLight(effect_sort);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );

	if(  effect_sort.Index == 83 )	// �Ϸ�Ʈ�� ��
	{
		if( nFrame > 12 )
		{
			EffectSort ef_sort;
			ZeroMemory( &ef_sort, sizeof( EffectSort ) );
			ef_sort.nType = FX_MAGICLASER;
			ef_sort.Index = effect_sort.Index;
			ef_sort.vPos = vDrawPos;
			ef_sort.vTargetPos = effect_sort.vTargetPos;
			effect_sort.pNkChaFrom->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			bFinished = true;
		}
	}
	else if( effect_sort.Index == 4006 )	// ������
	{
		if( nFrame > 20 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4007 )
	{
		if( nFrame > 8 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4009 )
	{
		if( nFrame > 32 )
			bFinished = true;
	}
	else if( effect_sort.Index == 135 ) // �޸� ��ų ������ ��
	{
		if( nFrame > 40 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4010 ) // ��ī�� �δ� ���� ���׳� �Ϲݰ��� ����Ʈ. ///�δ�1
	{
		if( nFrame > 56 )
			bFinished = true;
	}
	else if( effect_sort.Index == 4011 ) // ��ī�� �δ� ���� ���׳� ���߰��� ����Ʈ. ///�δ�1
	{
		if( nFrame > 54 )
		{
			// ī�޶� ȭ�� y ���� ���� ���� ������ �ٲ��ش�.
			D3DVECTOR Eye;
			pCMyApp->GetEyePos( Eye );
			D3DVECTOR At;
			pCMyApp->GetEyeAtPos( At );
			Eye.y = effect_sort.vSortPos.x;
			At.y = effect_sort.vSortPos.y;
			pCMyApp->SetCameraInitPos( Eye, At ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.

			bFinished = true;
		}
	}

	return bFinished;
}


void CFxAttackFixLolo::RenderLight(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return;

	BOOL bFinished = FALSE;
	D3DMATRIX matWorld;
	D3DVECTOR vDrawPos;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,	FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	vDrawPos = effect_sort.vTargetPos;

	if( ++m_numTLight >= 4 )
		m_numTLight = 0;

	m_nOldFrame = effect_sort.nCurFrame % 8;
	float fScale = (3.0f * m_nOldFrame) / 7;
	m_fScale = 3.0f - fScale;

	int nFrame = effect_sort.nCurFrame;
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_SHOCKSPEARLIGHT + m_numTLight ] ));

	if( light )
	{
		light->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
		light->SetRotationY( g_PI * 0.5f );
		light->RotateZ( -nFrame * 0.3f );
		light->Scale(m_fScale, 0.0f, m_fScale);
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 9.0f, vDrawPos.z);
		light->Render();

		light->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
		light->SetRotationX( g_PI * 0.5f );
		light->RotateZ( -nFrame * 0.3f );
		light->Scale(m_fScale, 0.0f, m_fScale);
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 9.0f, vDrawPos.z);
		light->Render();

		light->SetColor(0.8f, 0.8f, 1.0f, 1.0f);
		light->SetRotationY( g_PI * 0.5f );
		light->RotateX( -nFrame * 0.3f );
		light->Scale(m_fScale, 0.0f, m_fScale);
		light->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + 9.0f, vDrawPos.z);
		light->Render();
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}