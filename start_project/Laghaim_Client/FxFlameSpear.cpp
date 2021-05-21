#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
#include <assert.h>
#include <time.h>
#include "FxSet.h"
#include "ddutil.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "GUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "land.h"
#include "Skill.h"

#include "FxFireParticle.h"
#include "FxFlameSpear.h"

// ���� ������
#define PHASE_SUMMONING 0 // ��ȯ�Ǵ� �ܰ�
#define PHASE_THROWING	1 // ���󰡴� �ܰ�
#define PHASE_EXPLOSION 2 // �����ϴ� �ܰ�







extern CLolos	*g_pFairyProtect; // �� ����Ʈ �ѷ� ///��

CFxFlameSpear::CFxFlameSpear()
	: m_pMagicCircle2(NULL)
{
	m_pMagicCircle1 = NULL;
	m_pSpear = NULL;
	m_pSpearTail = NULL;
//	m_pSpearAura = NULL;
	m_pRollingEff = NULL;
}

CFxFlameSpear::~CFxFlameSpear()
{
	DeleteRes();
}

void CFxFlameSpear::DeleteRes()
{
	SAFE_DELETE( m_pRollingEff );
	SAFE_DELETE( m_pSpearTail );
	SAFE_DELETE( m_pSpear );
	SAFE_DELETE( m_pMagicCircle1 );
}

void CFxFlameSpear::LoadRes()
{
	DeleteRes();

	m_pMagicCircle1 = new CRectPlane;
	if( m_pMagicCircle1 )
		m_pMagicCircle1->Create( 25.0f, 25.0f, FALSE );

	m_pRollingEff = new CCylinderPlane;
	if( m_pRollingEff )
		m_pRollingEff->Create( 2.0f, 3.0f, 50.0f, 30.0f, 0.0f, TRUE, 2.0f);


	if (!m_pSpear)
	{
		m_pSpear = new CLolos;
		m_pSpear->SetIndexedTexture( g_pCapsyongTexture );
		m_pSpear->Read("data/effect/lolos/FlameSpear.LOL");
	}

	if (!m_pSpearTail)
	{
		m_pSpearTail = new CLolos;
		m_pSpearTail->SetIndexedTexture( g_pCapsyongTexture );
		m_pSpearTail->Read("data/effect/lolos/FlameSpearTail.LOL");
	}


}


BOOL CFxFlameSpear::Render( EffectSort &effect_sort )
{
	if( !effect_sort.pNkChaFrom || (!effect_sort.pNkChaTo&&!effect_sort.pMobTo) ) // Ȥ�� Ÿ���� �ϳ��� �������ٸ�...
		return TRUE;

	DWORD dwLighting, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );



	D3DVECTOR TempVec, TempVec2, TempVec3;
	D3DMATRIX matWorld;
	D3DMATRIX matTrans;
	D3DMATRIX matRotate, matRotate2;

	D3DXQUATERNION DestQuat; // ����� ���ϴ� ȸ��.
	D3DXQUATERNION TempQuat;

	float fTargetDist = 0.0f;

	// ���ϴ� ������ Ÿ���� �׻� ���󰡰� ���ش�.
	if( effect_sort.num != PHASE_EXPLOSION && effect_sort.nCurFrame <= 35 ) // �� ������ ���Ŀ� ������ ������� �ʴ´�. ���� ������ �����̴�.
	{
		if( effect_sort.pNkChaTo )
		{
			// Ÿ�ٰ� �̵� ������ ����.
			effect_sort.vTargetPos = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+0.0f, effect_sort.pNkChaTo->m_wz ); // ���� ��ǥ�� �Ǵ� ��ġ.(���⸦ �������� Ÿ���� �����Ѵ�.) // �ణ ���� Ÿ�����Ѵ�.
			effect_sort.vSortPos = effect_sort.vTargetPos - effect_sort.vCurPos; // ���� Ÿ�� ��ġ�� �ƴ϶� â�� ���ϴ� ���� ���ͷ� ���.
			fTargetDist = Magnitude( effect_sort.vSortPos ); // Ÿ�ٱ����� ���� �Ÿ��� ���.
			effect_sort.vSortPos = Normalize( effect_sort.vSortPos ); // ���� Ÿ�� ��ġ�� �ƴ϶� â�� ���ϴ� ���� ���ͷ� ���.
		}
		else if( effect_sort.pMobTo )
		{
			// Ÿ�ٰ� �̵� ������ ����.
			effect_sort.vTargetPos = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+0.0f, effect_sort.pMobTo->m_wz ); // ���� ��ǥ�� �Ǵ� ��ġ.(���⸦ �������� Ÿ���� �����Ѵ�.) // �ణ ���� Ÿ�����Ѵ�.
			effect_sort.vSortPos = effect_sort.vTargetPos - effect_sort.vCurPos; // ���� Ÿ�� ��ġ�� �ƴ϶� â�� ���ϴ� ���� ���ͷ� ���.
			fTargetDist = Magnitude( effect_sort.vSortPos ); // Ÿ�ٱ����� ���� �Ÿ��� ���.
			effect_sort.vSortPos = Normalize( effect_sort.vSortPos ); // ���� Ÿ�� ��ġ�� �ƴ϶� â�� ���ϴ� ���� ���ͷ� ���.
		}
	}


	float fLoloRotateY = atan2f(effect_sort.vSortPos.z, effect_sort.vSortPos.x); // y ������Ʈ ���� ��.
	float fLoloRotateX = acos( DotProduct( D3DVECTOR(effect_sort.vSortPos.x,effect_sort.vSortPos.y,effect_sort.vSortPos.z), D3DVECTOR(effect_sort.vSortPos.x,0.0f,effect_sort.vSortPos.z) ) ); // x ������Ʈ ���� ��.
	float fLoloRotateZ = (float)(effect_sort.nCurFrame)*0.5f; // z ������Ʈ ���� ��. ���ư����� ���ư���.

	float rectwidth;

	switch( effect_sort.num ) // Phase �� ���δ�.
	{
	case PHASE_SUMMONING: // ��ȯ�Ǵ� �ܰ�

		if( effect_sort.nCurFrame <= 25 ) // �������� �̵��ϴ� �ܰ�
		{
			// �ӽ÷� ũ������.
			rectwidth = (float)effect_sort.nCurFrame*0.3f;
			m_pMagicCircle1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 0.0f, 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, 0.0f, 0.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 1.0f , 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, 1.0f , 0.0f )  );

			// ������ ȸ���� ���⼭ ����.
			m_pMagicCircle1->SetRotationY( 0.0f ); // �ʱ�ȭ�� ���� ����.
			m_pMagicCircle1->RotateAxis( D3DVECTOR(0.0f,1.0f,0.0f), fLoloRotateY+1.57f ); // 90�� ������..
			m_pMagicCircle1->RotateAxis( D3DVECTOR(0.0f,0.0f,1.0f), (float)effect_sort.nCurFrame*0.7f ); // �� ������� �������� ȸ����Ų��.

			// �����ϰ� x,z����� ���⺤�͸� ���� ������ �� �Ҳ��� ������ �����ش�.( vSortPos�� ���� ������ �Ÿ������� ���̰� �޶��� ������.
			TempVec3 = Normalize( D3DVECTOR(effect_sort.vSortPos.x,0.0f,effect_sort.vSortPos.z) );

			TempVec = effect_sort.vCurPos + ( TempVec3 * ((float)effect_sort.nCurFrame*0.85f) );
			TempVec2 = effect_sort.vCurPos + ( TempVec3 * ((float)effect_sort.nCurFrame*(-0.85f)) ); // �� ���� �ݴ��������.

			if( effect_sort.nCurFrame <= 22 ) // �Ҳ��� �ణ ���� �׸�������..
			{
				if( effect_sort.subNum != effect_sort.nCurFrame ) // 1�����ӿ� �ߺ����� �ʵ���..
				{
					// ù��° ������ �Ҳ� ����Ʈ �߰�.
					InsertFire( effect_sort, TempVec ); // ��ġ�� ������ ��ġ��...
					// �ι�° ������ �Ҳ� ����Ʈ �߰�.
					InsertFire( effect_sort, TempVec2 ); // ��ġ�� ������ ��ġ��...
				}
			}
		}
		else // �������� �̵����� �ʴ´ܰ�..
		{
			rectwidth = 9.0f;
			m_pMagicCircle1->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 0.0f, 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xffffffff, 0, 0.0f, 0.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xffffffff, 0, 1.0f , 1.0f )  );
			m_pMagicCircle1->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xffffffff, 0, 1.0f , 0.0f )  );

			// ������ ȸ���� ���⼭ ����.
			m_pMagicCircle1->SetRotationY( 0.0f ); // �ʱ�ȭ�� ���� ����.
			m_pMagicCircle1->RotateAxis( D3DVECTOR(0.0f,1.0f,0.0f), fLoloRotateY+1.57f ); // 90�� ������..

			// �����ϰ� x,z����� ���⺤�͸� ���� ������ �� �Ҳ��� ������ �����ش�.( vSortPos�� ���� ������ �Ÿ������� ���̰� �޶��� ������.
			TempVec3 = Normalize( D3DVECTOR(effect_sort.vSortPos.x,0.0f,effect_sort.vSortPos.z) );

			TempVec = effect_sort.vCurPos + ( TempVec3 * (21.25f) );
			TempVec2 = effect_sort.vCurPos + ( TempVec3 * (-21.25f) ); // �� ���� �ݴ��������.
		}

		// ������ ���. ��ȯ�Ǵ� �ܰ迡�� ���� �������� �׷��ش�.
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		// �������� �ΰ� �׸���.
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAMESPEAR_MC ] ) ); // �ؽ��Ĵ� ������.

		// ù��° ������...
		m_pMagicCircle1->SetTranslationXYZ( TempVec.x, TempVec.y, TempVec.z );
		m_pMagicCircle1->Render();

		// �ι�° ������...
		m_pMagicCircle1->SetTranslationXYZ( TempVec2.x, TempVec2.y, TempVec2.z );
		m_pMagicCircle1->Render();

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);


		if( effect_sort.nCurFrame > 20 ) // ���� ������ ���ĺ��͸� â�� ���
		{
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

			D3DMath_QuaternionFromAngles( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
										  , 0.0f, fLoloRotateY, 0.0f );

			if( effect_sort.nCurFrame > 25 ) // �� ���ĺ��ʹ� â�� ��ǥ�� ���� ������ ȸ���ϰ� ���ش�.
			{
				D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
											  , fLoloRotateX, fLoloRotateY, 0.0f );

				D3DMath_QuaternionSlerp( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
										 , TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
										 , DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
										 , (float)(effect_sort.nCurFrame-25)*0.07f ); // ���ʹϿ��� �����ӿ� ���� �����ذ���.
				/*
								// �� �ܰ���� Roll ȸ���� ���δ�.
								D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
									, 0.0f, 0.0f, fLoloRotateZ ); // Roll�� ������ ���̸� ���ϴ� ����� �ȳ�������. ���� Roll ȸ���� ���� ���ʹϿ³��� �ռ���Ų��.

								D3DMath_QuaternionMultiply( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
									, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
									, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );
				*/
				D3DMath_MatrixFromQuaternion( matRotate, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );
			}
			else
			{
				D3DMath_MatrixFromQuaternion( matRotate, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w );
			}

			D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

			m_pSpear->Render(GET_D3DDEVICE(), 0, FALSE, FALSE); // �ִϸ���Ʈ �� ��Ŵ�� ����
		}

		if( effect_sort.nCurFrame > 39 )
			effect_sort.num = PHASE_THROWING; // ���� ������� ����

		break;

	case PHASE_THROWING: // ���󰡴� �ܰ�

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// �� �ܰ������ Roll ȸ���� ���δ�.
		D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
									  , fLoloRotateX, fLoloRotateY, 0.0f );
		/*
				D3DMath_QuaternionFromAngles( TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
					, 0.0f, 0.0f, fLoloRotateZ ); // Roll�� ������ ���̸� ���ϴ� ����� �ȳ�������. ���� Roll ȸ���� ���� ���ʹϿ³��� �ռ���Ų��.

				D3DMath_QuaternionMultiply( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
					, TempQuat.x, TempQuat.y, TempQuat.z, TempQuat.w
					, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w );
		*/
		D3DMath_MatrixFromQuaternion( matRotate, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w );

		fTargetDist = (float)(effect_sort.nCurFrame-35) * 2.5f; // �׳� ���ӵ�.

		effect_sort.vCurPos += effect_sort.vSortPos * fTargetDist; // ��ǥ�� �̵�.

		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );
		D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

		m_pSpear->Render( GET_D3DDEVICE(), 0 );


		// ���󰥶� �ڿ� ȿ����(����) ����Ʈ�� ���.
		m_pSpearTail->Render( GET_D3DDEVICE(), /*(effect_sort.nCurFrame-35)*/5 ); // �̳༮�� �ִϸ���Ʈ ��Ų��.


		/*
				// ���󰥶��� ����Ʈ�� �߰�.
				TempVec = m_pSpear->m_LineVector0[0];
				D3DMath_VectorMatrixMultiply( TempVec, TempVec, matWorld );
				InsertFire2( effect_sort, TempVec ); // ��ġ�� ������ ��ġ��...
		*/
		/*
				if( g_pCapsyongTexture )
					GET_D3DDEVICE()->SetTexture(0, /*g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FLAME_TAIL ] )/ NULL );
		//			GET_D3DDEVICE()->SetTexture(0, NULL );

				if( m_pRollingEff )
				{
					TempVec = effect_sort.vCurPos - ( effect_sort.vSortPos*10.0f );

					m_pRollingEff->SetColor(1.0f, 0.5f, 0.5f, 1.0f);
		//			m_pRollingEff->SetRotationY( g_PI );
		//			m_pRollingEff->RotateY( m_fRotation * 0.1f );
		//			m_pRollingEff->Scale( 1.0f, m_fScale * 300.0f, 1.0f );

		//			m_pRollingEff->ReCreate( 2.0f, 3.0f, 150.0f, 20.0f, 0.0f, 2.0f);
					m_pRollingEff->SetRotationX( fLoloRotateX );
					m_pRollingEff->SetRotationY( fLoloRotateY );
		//			m_pRollingEff->SetRotation( matRotate );
		//			m_pRollingEff->RotateY( 1.57f );
					m_pRollingEff->SetTranslationXYZ( effect_sort.vCurPos.x,effect_sort.vCurPos.y, effect_sort.vCurPos.z );
					m_pRollingEff->Render( GET_D3DDEVICE() );
				}
		*/



		// ������ ���� �˻�.
		if( effect_sort.vCurPos.y < pCMyApp->GetLandPtr()->GetHFHeight(effect_sort.vCurPos.x,effect_sort.vCurPos.z) ) // â�� ���� �浹�ϸ� ���߽�Ų��.
		{
			effect_sort.nCurFrame = 0; // ���� �������� �����ϱ� ���� �ʱ�ȭ.
			effect_sort.num = PHASE_EXPLOSION; // ���� ������� ����

			// ���� �ڱ� �ڽ��� �÷��� ���Ǿ��� Ÿ�� ��Ŷ�� ���⼭ ������.
			if( effect_sort.pNkChaFrom == g_pRoh )
			{
				if( effect_sort.pNkChaTo )
					pCMyApp->SetHeroAttGoCha( effect_sort.pNkChaTo );
				else if( effect_sort.pMobTo )
					pCMyApp->SetHeroAttGoMob( effect_sort.pMobTo );

				pCMyApp->AttackMsgSend( SKILL_K_FLAMESPEAR_TARGET );
			}
		}

		break;

	case PHASE_EXPLOSION: // �����ϴ� �ܰ�
		/*
				if( effect_sort.nCurFrame < 10 ) // �����ð��� â�� ���̵���
				{
					GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
					GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

					D3DMath_QuaternionFromAngles( DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w
						, fLoloRotateX, fLoloRotateY, 0.0f );

					D3DMath_MatrixFromQuaternion( matRotate, DestQuat.x, DestQuat.y, DestQuat.z, DestQuat.w );

					D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );
					D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

					m_pSpear->Render( GET_D3DDEVICE(), 0 ); // �ִϸ���Ʈ ��Ų��.
				}

				// ���� ȿ���� ȣ���Ѵ�.
				if( effect_sort.pNkChaTo )
				{
					// Ÿ�ٰ� �̵� ������ ����.
					TempVec = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+3.0f, effect_sort.pNkChaTo->m_wz ); // ���� ��ǥ�� �Ǵ� ��ġ.(���⸦ �������� Ÿ���� �����Ѵ�.) // �ణ ���� Ÿ�����Ѵ�.
				}
				else if( effect_sort.pMobTo )
				{
					// Ÿ�ٰ� �̵� ������ ����.
					TempVec = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+3.0f, effect_sort.pMobTo->m_wz ); // ���� ��ǥ�� �Ǵ� ��ġ.(���⸦ �������� Ÿ���� �����Ѵ�.) // �ణ ���� Ÿ�����Ѵ�.
				}
		*/

		if( effect_sort.nCurFrame > 0 && effect_sort.subNum != effect_sort.nCurFrame ) // 1�����ӿ� �ߺ����� �ʵ���..
		{
			EffectSort ef_sort;
			ef_sort.nType = FX_METEORITEBOOM;
			ef_sort.Index = 1; // �ٴ� �������°� ��ǥ���ϵ���...

			ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-2.0f, 2.0f)*(float)effect_sort.nCurFrame, effect_sort.vCurPos.y+RandomNum(0.0f, 3.5f)*(float)effect_sort.nCurFrame, effect_sort.vCurPos.z+RandomNum(-2.0f, 2.0f)*(float)effect_sort.nCurFrame );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
//			ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-5.0f, 5.0f), effect_sort.vCurPos.y+RandomNum(0.0f, 5.0f), effect_sort.vCurPos.z+RandomNum(-5.0f, 5.0f) );
//			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);


			// �� ������ �ΰ��� �߰���Ų��.(Ÿ�� ��ǥ ��ó�� �ϳ��� ���� �������� �ϳ�.
//			ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-5.0f, 5.0f), effect_sort.vCurPos.y+RandomNum(0.0f, 5.0f), effect_sort.vCurPos.z+RandomNum(-5.0f, 5.0f) );
//			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			if( effect_sort.nCurFrame > 10 )
			{
				ef_sort.vPos = D3DVECTOR( effect_sort.vCurPos.x+RandomNum(-35.0f, 35.0f), effect_sort.vCurPos.y+RandomNum(0.0f, 20.0f), effect_sort.vCurPos.z+RandomNum(-35.0f, 35.0f) );
				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			}

		}

		if( effect_sort.nCurFrame > 15 )
			return TRUE;

		break;
	}



	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	effect_sort.subNum = effect_sort.nCurFrame; // subNum �� PrevFrame���� ���.

	return FALSE;
}


void CFxFlameSpear::InsertFire( EffectSort &effect_sort, D3DVECTOR vDrawVec ) // ����Ʈ�ִ°� �� �� �Լ��� �и�.
{
	Deco_Particle *pFire_Particle = NULL;

	pFire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE]; // �� ���ڸ� �ڴ�� �ø��� �ȵȴ�. // �� �༮�� �޸� ���� Ÿ�̹��� CFxFireParticle �� Render���� ���Žô�.

	if( !pFire_Particle )
		return;

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ ) // �ʱ�ȭ. Ư���� ���� �����ϰ� ������ �ʿ�� ����.
	{
		pFire_Particle[i].start_time = 0;
		pFire_Particle[i].life_time = 0;
		pFire_Particle[i].cur_time = 0;
		pFire_Particle[i].scale = 0.0f; // �� ���ڴ� ����Ʈ ���� �ٸ� ���� CFxFireParticle �� �ٸ���. Ÿ���� ����->�Ҹ� Ÿ���̱� ������.
		pFire_Particle[i].fade = 1.0f;

		pFire_Particle[i].red = 1.0f;
		pFire_Particle[i].green = 1.0f;
		pFire_Particle[i].blue = 1.0f;

		pFire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		pFire_Particle[i].texIndex = 0;
		pFire_Particle[i].texFrame = 0;
	}

	// ���� ����Ʈ �߰�
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = FX_FIREPARTICLE;
	ef_sort.effects = pFire_Particle;
	ef_sort.num = REDFIRE_ONETIME; // Ÿ��
	ef_sort.vPos = vDrawVec; // ��ġ
	ef_sort.Index = 5; // �Ҳ� ũ��.
	ef_sort.dwColor = 1; // ���� ������ �ʱ�ȭ. (1���� �����ϵ���)
	ef_sort.subNum = 25; // ������(�����Ӵ���)
	ef_sort.vSortPos.x = 0.3f; // �ʹ� Ŀ���� �ӵ�
	ef_sort.vSortPos.y = 0.1f; // �Ĺ� ������ �ӵ�

	pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
}

void CFxFlameSpear::InsertFire2( EffectSort &effect_sort, D3DVECTOR vDrawVec ) // ����Ʈ�ִ°� �� �� �Լ��� �и�.
{
	Deco_Particle *pFire_Particle = NULL;

	pFire_Particle = new Deco_Particle[MAX_FIRE_PARTICLE]; // �� ���ڸ� �ڴ�� �ø��� �ȵȴ�. // �� �༮�� �޸� ���� Ÿ�̹��� CFxFireParticle �� Render���� ���Žô�.

	if( !pFire_Particle )
		return;

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ ) // �ʱ�ȭ. Ư���� ���� �����ϰ� ������ �ʿ�� ����.
	{
		pFire_Particle[i].start_time = 0;
		pFire_Particle[i].life_time = 0;
		pFire_Particle[i].cur_time = 0;
		pFire_Particle[i].scale = 0.0f; // �� ���ڴ� ����Ʈ ���� �ٸ� ���� CFxFireParticle �� �ٸ���. Ÿ���� ����->�Ҹ� Ÿ���̱� ������.
		pFire_Particle[i].fade = 1.0f;

		pFire_Particle[i].red = 1.0f;
		pFire_Particle[i].green = 1.0f;
		pFire_Particle[i].blue = 1.0f;

		pFire_Particle[i].first_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].cur_Pos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].direction = D3DVECTOR( 0.0f, 0.0f, 0.0f );
		pFire_Particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );

		pFire_Particle[i].texIndex = 0;
		pFire_Particle[i].texFrame = 0;
	}

	// ���� ����Ʈ �߰�
	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));

	ef_sort.nType = FX_FIREPARTICLE;
	ef_sort.effects = pFire_Particle;
	ef_sort.num = REDFIRE_ONETIME; // Ÿ��
	ef_sort.vPos = vDrawVec; // ��ġ
	ef_sort.Index = 1; // �Ҳ� ũ��.
	ef_sort.dwColor = 1; // ���� ������ �ʱ�ȭ. (1���� �����ϵ���)
	ef_sort.subNum = 5; // ������(�����Ӵ���)
	ef_sort.vSortPos.x = 1.3f; // �ʹ� Ŀ���� �ӵ�
	ef_sort.vSortPos.y = 1.1f; // �Ĺ� ������ �ӵ�

	pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
}