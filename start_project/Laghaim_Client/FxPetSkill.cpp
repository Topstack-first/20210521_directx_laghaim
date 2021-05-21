#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d.h>
#include <ddraw.h>
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

#include "FxPetSkill.h"






CFxPetSkill::CFxPetSkill()
{
	m_pBillBoard = NULL;
	m_pPoison = NULL;
	m_pParticleInfoArray = NULL;
}

CFxPetSkill::~CFxPetSkill()
{
	DeleteRes();
}

void CFxPetSkill::DeleteRes()
{
	SAFE_DELETE( m_pBillBoard );
	SAFE_DELETE( m_pPoison );
}

void CFxPetSkill::LoadRes()
{
	DeleteRes();

	m_pBillBoard = new CRectPlane;
	if( m_pBillBoard )
		m_pBillBoard->Create( 5.0f, 5.0f, FALSE ); // ũ�⸦ ���Ƿ� �����س��´�.(�� ����Ʈ���� �������� ũ�⸦ �������ش�.)

	// �� ����Ʈ �ѷ� ///��

	g_pCapsyongTexture->SetTexturePath( "data/effect/textr/" );

	CLolos *pLolo;
	pLolo = new CLolos(g_pCapsyongTexture);
	pLolo->Read("data/effect/lolos/pet_poison.LOL");
	m_pPoison = pLolo;




}

void CFxPetSkill::FrameMove( EffectSort &effect_sort, int nAddFrame ) // ���� �������� ��������.
{
	int TotalFrame = 0; // �� Ÿ��,�ִϸ��̼� �� �� ������.
	float speed = 0.0f; // �����̴� �ӵ�.
	D3DVECTOR vTempVec;
	D3DVECTOR vTempVec2;
	D3DVECTOR vTempVec3;
	float fTempFloat = 0.0f;
	float MasterDist = 0.0f; // �����Ϳ��� �Ÿ� ��.
	D3DMATRIX TempMatrix;

	D3DVECTOR	vViewVec, vRightVec;
	vViewVec = pCMyApp->GetEyeDirection();
	vRightVec = CrossProduct( D3DVECTOR(0.0f,1.0f,0.0f),vViewVec ); // ��� ���� ����Ʈ�� ���س��´�.
	vRightVec = Normalize( vRightVec );

	// ������ ����
	effect_sort.nCurFrame += nAddFrame;

	// Ÿ�Ժ��� �ٸ� AI�� �����Ѵ�.
	switch( effect_sort.Index ) // effect_sort �� Index�� Ÿ������ ����Ѵ�.
	{
	case PET_SKILL_EFFECT_TYPE1: // ��󿡰� ���׸� ���ڰ� ���󰡼� �ε�ġ�� ����Ʈ.

		if( effect_sort.nCurFrame < 0 ) // -�������϶� ����
			return;

		if( effect_sort.subNum == 0 ) // ���ư��� ����
		{
			TotalFrame = PET_SKILL_EFFECT_TYPE1_FRAME;

			// �̵������� ���Ѵ�. Ÿ���� ������ Ÿ�ٿ� ���缭...
			if( effect_sort.pNkChaTo )
				vTempVec2 = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+13.0f, effect_sort.pNkChaTo->m_wz );
			else if( effect_sort.pMobTo )
				vTempVec2 = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+13.0f, effect_sort.pMobTo->m_wz );
			else
				vTempVec2 = effect_sort.vTargetPos;

			vTempVec = vTempVec2 - effect_sort.vCurPos;
			fTempFloat = Magnitude( vTempVec ); // ���� �Ÿ��� ���ؼ�.
			effect_sort.vSortPos.y = fTempFloat / (PET_SKILL_EFFECT_TYPE1_FRAME-effect_sort.nCurFrame); // ���� �����Ӿȿ� �̵��ϵ��� �ӵ��� �ٲ��ش�.
			vTempVec = Normalize(vTempVec); // �ϴ� �������ͷ� �̾Ƽ�...

			// �̵�.
//			effect_sort.vCurPos = effect_sort.vPos + (vTempVec*(float)effect_sort.nCurFrame*effect_sort.vSortPos.y); // ���� �����Ӿȿ� �̵��ϵ���...
			effect_sort.vCurPos = effect_sort.vCurPos + (vTempVec*(float)nAddFrame*effect_sort.vSortPos.y); // ���� �����Ӿȿ� �̵��ϵ���...
			/*
						BOOL bFinish = FALSE;

						vTempVec3 = effect_sort.vTargetPos - effect_sort.vPos;
						vTempVec3 = Normalize(vTempVec); // �ϴ� �������ͷ� �̾Ƽ�...
			*/
			if( effect_sort.nCurFrame >= TotalFrame )
//			if( bFinish )
			{
				effect_sort.subNum = 1; // ���¸� ���߷� �ٲ��ش�.
				effect_sort.nCurFrame = 0;

				if( m_pParticleInfoArray )
				{
					for( int i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
					{
						m_pParticleInfoArray[i].life_time = 5; // curTailPosNum�� �������� ���.
					}
				}
			}
		}
		else if( effect_sort.subNum == 1 ) // �����ϴ�(Ŀ����) ����
		{
			TotalFrame = 20;

			// Ŀ���� ���¶� ��ġ�� ���󰡵���.
			if( effect_sort.pNkChaTo )
				effect_sort.vCurPos = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy+13.0f, effect_sort.pNkChaTo->m_wz );
			else if( effect_sort.pMobTo )
				effect_sort.vCurPos = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy+13.0f, effect_sort.pMobTo->m_wz );
			else
				effect_sort.vCurPos = effect_sort.vTargetPos;


			if( effect_sort.nCurFrame >= TotalFrame )
			{
				effect_sort.subNum = 2; // ���ֱ�...
			}
		}

		// ��ƼŬ ����.
		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // ������ effect_sort ������ �Ǿ������� �����͸� �޾ƿͼ� ó��.

		if( m_pParticleInfoArray )
		{
			for( int i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
			{
				m_pParticleInfoArray[i].life_time += nAddFrame%3; // curTailPosNum�� �������� ���.

				if( m_pParticleInfoArray[i].life_time > 2+(int)RandomNum(-2.0f,4.0f) )
				{
					// ������ ���� �༮�� ������ ���� ����
					m_pParticleInfoArray[i].life_time = 0;
					m_pParticleInfoArray[i].cur_Pos.x = effect_sort.vCurPos.x+RandomNum(-1.7f,1.7f);
					m_pParticleInfoArray[i].cur_Pos.y = effect_sort.vCurPos.y+RandomNum(-1.7f,1.7f);
					m_pParticleInfoArray[i].cur_Pos.z = effect_sort.vCurPos.z+RandomNum(-1.7f,1.7f);

					m_pParticleInfoArray[i].fade = 1.0f; // ����. ���� �پ���.
					m_pParticleInfoArray[i].scale = RandomNum( 0.5f, 1.0f ); // ����ũ�⸦ ����.
				}

				m_pParticleInfoArray[i].fade -= 0.04f*(float)nAddFrame; // ����. ���� �پ���.
				if( m_pParticleInfoArray[i].fade < 0.0f )
					m_pParticleInfoArray[i].fade = 0.0f;

				if( effect_sort.subNum == 1 )
				{
					m_pParticleInfoArray[i].cur_Pos.x += RandomNum( -2.1f, 2.1f ); // �ణ ��鸮��...
					m_pParticleInfoArray[i].cur_Pos.y -= (float)nAddFrame*5.2f; // ���� �ӵ��� ��������.
					m_pParticleInfoArray[i].cur_Pos.z += RandomNum( -2.1f, 2.1f ); // �ణ ��鸮��...
				}
			}
		}

		break;

	case PET_SKILL_EFFECT_TYPE2: // ������ ����Ʈ. ������ ���� ��ü�� �߻��ؼ� �������� ������.

		if( !effect_sort.pMobFrom )
			return;

		if( effect_sort.nCurFrame >= 22 && effect_sort.subNum == 0 ) // �Ĺݺο� �ѹ��� ���� ����Ʈ�� �־��ش�.
		{
			// ���⿡ ���� ����Ʈ �־�����Ѵ�.
			EffectSort ef_sort;

			ef_sort.nType = FX_POISON;
			ef_sort.Index = 10082;	// ���� ���� ������ ��ų ����
			ef_sort.nCurFrame = 0; // �̷��� �ϸ� ���Ⱑ �ٷ� ���´�.

			float fX = cos(effect_sort.pMobFrom->m_dir) * 20.0f; // ���� ���� ������...
			float fZ = sin(effect_sort.pMobFrom->m_dir) * 20.0f;
			ef_sort.vPos = D3DVECTOR( effect_sort.vPos.x + fX, effect_sort.vPos.y, effect_sort.vPos.z + fZ);

			ef_sort.vTargetPos = ef_sort.vPos;

			effect_sort.pMobFrom->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

			effect_sort.subNum = 1; // �ѹ��� �ְ�..
		}

		break;
	}
}

BOOL CFxPetSkill::Render( EffectSort &effect_sort ) // �ְ�� ����� ��ο�.(����� 2D�� ����ӿ� ����. �����ú��� 3D�� �ƴ϶� 2D��ǥ��.)
{
	int i;
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	DWORD dwLighting, dwSrc, dwDest;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	float rectwidth = 0.0f;
	float tu = 0.0f;
	float tv = 0.0f;

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;

	switch( effect_sort.Index ) // effect_sort �� Index�� Ÿ������ ����Ѵ�.
	{
	case PET_SKILL_EFFECT_TYPE1: // ��󿡰� ���׸� ���ڰ� ���󰡼� �ε�ġ�� ����Ʈ.

		if( effect_sort.nCurFrame < 0 ) // -�������϶� ����
			return FALSE;

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE2X );

		if( effect_sort.subNum == 0 ) // ���ư��� ����
		{
			// �����̴� ����Ʈ
			rectwidth += RandomNum(4.25f,4.7f);

			if( m_pBillBoard )
			{
				m_pBillBoard->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pBillBoard->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );

				m_pBillBoard->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 0.0f )  );
				m_pBillBoard->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 0.0f )  );

				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) );
				m_pBillBoard->Render();
			}
		}
		else if( effect_sort.subNum == 1 ) // Ȯ��Ǿ� ������ ����.
		{
			// �����̴� ����Ʈ
			rectwidth = 12.5f + (effect_sort.nCurFrame-10)*1.0f;

			if( m_pBillBoard )
			{
				m_pBillBoard->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pBillBoard->SetTranslationXYZ( effect_sort.vCurPos.x, effect_sort.vCurPos.y, effect_sort.vCurPos.z );

				m_pBillBoard->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 0.0f, 0.0f )  );
				m_pBillBoard->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f), 0xbbffffff, 0, 1.0f, 0.0f )  );

				GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FAIRY_LIGHT ] ) );
				m_pBillBoard->Render();
			}

		}
		else // ���� ���¸� �����ش�.
		{
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
			GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
			GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

			return TRUE;
		}

		// ��ƼŬ ���.
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // ������ effect_sort ������ �Ǿ������� �����͸� �޾ƿͼ� ó��.

		for( i = 0 ; i < MAX_FXPETSKILL_PARTICLE ; ++i )
		{
			if( fViewDist > 50.0f && fViewDist < 500.0f ) // ī�޶�� �����Ÿ� �̻� �������ִٸ�... �ʹ� �־ ���� ���Ѵ�.
				rectwidth = m_pParticleInfoArray[i].scale * (fViewDist*0.02f) * 0.5f;
			else // ī�޶�� ����ﶩ �׳� ���
				rectwidth = m_pParticleInfoArray[i].scale * 0.5f;

			if( m_pBillBoard )
			{
				m_pBillBoard->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 0.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 0.0f, 0.0f )  );
				m_pBillBoard->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 1.0f, 1.0f )  );
				m_pBillBoard->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f)
												, D3DRGBA(1.0f,1.0f,1.0f,m_pParticleInfoArray[i].fade), 0, 1.0f, 0.0f )  );

				m_pBillBoard->SetBillBoard( vViewVec, FALSE, 0.0f );
				m_pBillBoard->SetTranslationXYZ( m_pParticleInfoArray[i].cur_Pos.x
												 , m_pParticleInfoArray[i].cur_Pos.y, m_pParticleInfoArray[i].cur_Pos.z );

				m_pBillBoard->Render();
			}
		}

		break;

	case PET_SKILL_EFFECT_TYPE2: // ������ ����Ʈ. ������ ���� ��ü�� �߻��ؼ� �������� ������.

		if( !effect_sort.pMobFrom )
			return FALSE;

		if( effect_sort.nCurFrame >= 52 ) // �������� �����ٸ�...
		{
			return TRUE;  // ����Ʈ�� �����Ѵ�.
		}

		// ����.
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.pMobFrom->m_dir );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

		D3DMath_MatrixMultiply( matTrans, matRotate, matTrans );

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matTrans );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		m_pPoison->Render( GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE );

		break;

	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return FALSE;
}
