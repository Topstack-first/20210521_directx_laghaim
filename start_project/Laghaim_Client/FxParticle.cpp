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

#include "FxParticle.h"





CFxTypeParticle::CFxTypeParticle()
{
	m_pRectPlane = NULL;
	m_pParticleInfoArray = NULL;
}

CFxTypeParticle::~CFxTypeParticle()
{
	DeleteRes();
}

void CFxTypeParticle::DeleteRes()
{
	SAFE_DELETE( m_pRectPlane );
}

void CFxTypeParticle::LoadRes()
{
	DeleteRes();

	m_pRectPlane = new CRectPlane;
	if( m_pRectPlane )
		m_pRectPlane->Create( 5.0f, 5.0f, FALSE ); // ũ�⸦ ���Ƿ� �����س��´�.(�� ����Ʈ���� �������� ũ�⸦ �������ش�.)
}


void CFxTypeParticle::FrameMove( EffectSort &effect_sort, int nAddFrame )
{
	int i = 0;
	DWORD dwElapsedTick = 0;
	DWORD ParticleDieCount = 0;

	// Ÿ�Ժ��� �ٸ� AI�� �����Ѵ�.
	switch( effect_sort.Index ) // effect_sort �� Index�� Ÿ������ ����Ѵ�.
	{
	case FX_PARTICLE_TYPE_LIP : // 2007.03.14 ȭ��Ʈ
	case FX_PARTICLE_TYPE_L	  :
	case FX_PARTICLE_TYPE_O	  :
	case FX_PARTICLE_TYPE_V	  :
	case FX_PARTICLE_TYPE_E	  :
	case FX_PARTICLE_TYPE_HEARTLORD: // �Ϻ� �߷�Ÿ�ε��� ������ ��Ʈ�ε�,���귯��ε� �� ����Ʈ. ��Ʈ���� �ȶ� �ȶ�~ ///��Ʈ�ε�

		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // ������ effect_sort ������ �Ǿ������� �����͸� �޾ƿͼ� ó��.

		for( i = 0 ; i < MAX_PARTICLE_FOR_HEARTLOAD ; ++i )
		{
			dwElapsedTick = timeGetTime() - m_pParticleInfoArray[i].cur_time; // �������� �����ð�.
			m_pParticleInfoArray[i].cur_time = timeGetTime() - m_pParticleInfoArray[i].start_time; // ������ ��� �ð�.

			if( m_pParticleInfoArray[i].cur_time < 0 ) // �� ���� ���� �����ȵ� �༮.
				continue;

			if( m_pParticleInfoArray[i].cur_time > m_pParticleInfoArray[i].life_time ) // ������ ��������...
			{
				++ParticleDieCount; // ��ƼŬ ������ ���� ���ڸ� ���д�.
				continue;
			}


			// gravity �� �������� direction �� �ӵ� ��ŭ �̵�.
//			float v = m_pParticleInfoArray[i].direction.x + (m_pParticleInfoArray[i].direction.y*(float)m_pParticleInfoArray[i].cur_time); // �ӵ��� ����.
			float v = m_pParticleInfoArray[i].direction.x; // �ӵ��� ����.
			/*
						if( v > m_pParticleInfoArray[i].direction.z ) // �ӵ� ����
							v = m_pParticleInfoArray[i].direction.z;
			*/
			/*
						m_pParticleInfoArray[i].direction.y +=

						float v = m_pParticleInfoArray[i].direction.x + m_pParticleInfoArray[i].direction.y; // �ӵ��� ����.

						v = v * (float)m_pParticleInfoArray[i].cur_time;

						if( v > m_pParticleInfoArray[i].direction.z ) // �ӵ� ����
							v = m_pParticleInfoArray[i].direction.z;
			*/
			m_pParticleInfoArray[i].cur_Pos.x = m_pParticleInfoArray[i].first_Pos.x + ( m_pParticleInfoArray[i].gravity.x * v * (float)m_pParticleInfoArray[i].cur_time );
			m_pParticleInfoArray[i].cur_Pos.y = m_pParticleInfoArray[i].first_Pos.y + ( m_pParticleInfoArray[i].gravity.y * v * (float)m_pParticleInfoArray[i].cur_time );
			m_pParticleInfoArray[i].cur_Pos.z = m_pParticleInfoArray[i].first_Pos.z + ( m_pParticleInfoArray[i].gravity.z * v * (float)m_pParticleInfoArray[i].cur_time );
			/* // ���̵� ������ �� �ȵǼ� �׳� ��.
			//			m_pParticleInfoArray[i].fade = 255 - (m_pParticleInfoArray[i].cur_time * 0.1f);

						if( m_pParticleInfoArray[i].fade < 0 )
							m_pParticleInfoArray[i].fade = 0;
						if( m_pParticleInfoArray[i].fade > 255 )
							m_pParticleInfoArray[i].fade = 255;
			*/
			// Ÿ�Ժ� ������ ó��
			if( m_pParticleInfoArray[i].texFrame == 1 ) // �¿�� ����� ȸ��
			{
				if( m_pParticleInfoArray[i].direction.y == 1.0f ) // ������ ȸ�� ��
				{
					m_pParticleInfoArray[i].direction.z += 0.2f*nAddFrame;
					// ������ �̻��� �� ��������.
					if( m_pParticleInfoArray[i].direction.z > 0.5f )
					{
						m_pParticleInfoArray[i].direction.z = 0.5f;
						m_pParticleInfoArray[i].direction.y = 0.0f;
					}
				}
				else // ����ȸ����.
				{
					m_pParticleInfoArray[i].direction.z -= 0.2f*nAddFrame;
					// ������ �̻��� �� ��������.
					if( m_pParticleInfoArray[i].direction.z < -0.5f )
					{
						m_pParticleInfoArray[i].direction.z = -0.5f;
						m_pParticleInfoArray[i].direction.y = 1.0f;
					}
				}
			}
			else if( m_pParticleInfoArray[i].texFrame == 2 ) // Ŀ���� �۾�����.
			{
				if( m_pParticleInfoArray[i].direction.y == 1.0f ) // Ŀ���� ��
				{
					m_pParticleInfoArray[i].direction.z += 0.2f*nAddFrame;
					// ������ �̻��� �� ��������.
					if( m_pParticleInfoArray[i].direction.z > 0.5f )
					{
						m_pParticleInfoArray[i].direction.z = 0.5f;
						m_pParticleInfoArray[i].direction.y = 0.0f;
					}
				}
				else // �۾�������
				{
					m_pParticleInfoArray[i].direction.z -= 0.2f*nAddFrame;
					// ������ �̻��� �� ��������.
					if( m_pParticleInfoArray[i].direction.z < -0.5f )
					{
						m_pParticleInfoArray[i].direction.z = -0.5f;
						m_pParticleInfoArray[i].direction.y = 1.0f;
					}
				}
			}

		}

		if( ParticleDieCount == MAX_PARTICLE_FOR_HEARTLOAD ) // ��ƼŬ���� ���� ������ ��������...
		{
			effect_sort.nCurFrame = pCMyApp->m_pFxSet->m_pFrameCount[FX_TYPE_PARTICLE]; // �̰� ���̸� �������� TRUE�� ������ ����Ʈ�� ����� �ߴ�.
		}


		break;

	case FX_PARTICLE_TYPE_LUCKYPIG: // ���� ���� ȸ�� ����Ʈ

		if( !effect_sort.pNkChaTo )
			return;

		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // ������ effect_sort ������ �Ǿ������� �����͸� �޾ƿͼ� ó��.

		for( i = 0 ; i < MAX_PARTICLE_FOR_LUCKYPIG ; ++i )
		{
			dwElapsedTick = timeGetTime() - m_pParticleInfoArray[i].cur_time; // �������� �����ð�.
			m_pParticleInfoArray[i].cur_time = timeGetTime() - m_pParticleInfoArray[i].start_time; // ������ ��� �ð�.

			if( m_pParticleInfoArray[i].cur_time < 0 ) // �� ���� ���� �����ȵ� �༮.
				continue;

			if( m_pParticleInfoArray[i].cur_time > m_pParticleInfoArray[i].life_time ) // ������ ��������...
			{
				++ParticleDieCount; // ��ƼŬ ������ ���� ���ڸ� ���д�.
				continue;
			}

			float v = m_pParticleInfoArray[i].direction.x; // �ӵ��� ����.

			if( (m_pParticleInfoArray[i].cur_time)%1000 > 500 )
			{
				m_pParticleInfoArray[i].cur_Pos.x = effect_sort.pNkChaTo->m_wx + m_pParticleInfoArray[i].first_Pos.x - ( m_pParticleInfoArray[i].gravity.x * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
				m_pParticleInfoArray[i].cur_Pos.z = effect_sort.pNkChaTo->m_wz + m_pParticleInfoArray[i].first_Pos.z - ( m_pParticleInfoArray[i].gravity.z * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
			}
			else
			{
				m_pParticleInfoArray[i].cur_Pos.x = effect_sort.pNkChaTo->m_wx + m_pParticleInfoArray[i].first_Pos.x + ( m_pParticleInfoArray[i].gravity.x * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
				m_pParticleInfoArray[i].cur_Pos.z = effect_sort.pNkChaTo->m_wz + m_pParticleInfoArray[i].first_Pos.z + ( m_pParticleInfoArray[i].gravity.z * v * (float)m_pParticleInfoArray[i].cur_time * 1.5f );
			}

			m_pParticleInfoArray[i].cur_Pos.y = effect_sort.pNkChaTo->m_wy + m_pParticleInfoArray[i].first_Pos.y + ( m_pParticleInfoArray[i].gravity.y * v * (float)m_pParticleInfoArray[i].cur_time ); // ������ ��������.

			// ���̵� ������ �� �ȵǼ� �׳� ��.
//			m_pParticleInfoArray[i].fade = m_pParticleInfoArray[i].fade - ((float)nAddFrame*0.1f);

			if( m_pParticleInfoArray[i].fade < 0 )
				m_pParticleInfoArray[i].fade = 0;
			if( m_pParticleInfoArray[i].fade > 255 )
				m_pParticleInfoArray[i].fade = 255;

		}

		if( ParticleDieCount == MAX_PARTICLE_FOR_LUCKYPIG ) // ��ƼŬ���� ���� ������ ��������...
		{
			effect_sort.nCurFrame = pCMyApp->m_pFxSet->m_pFrameCount[FX_TYPE_PARTICLE]; // �̰� ���̸� �������� TRUE�� ������ ����Ʈ�� ����� �ߴ�.
		}


		break;
	}
}

BOOL CFxTypeParticle::Render( EffectSort &effect_sort )
{
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	DWORD dwLighting, dwSrc, dwDest, dwAlphaBlend;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	D3DMATRIX matTrans;
	D3DMATRIX matRotate;

	float rectwidth = 0.0f; // vSortPos�� �����ϰ� �ִ°� ������ �� x���� ũ�Ⱚ���� ����Ѵ�.
	float tu = 0.0f;
	float tv = 0.0f;

	int i = 0, k = 0;

	switch( effect_sort.Index ) // effect_sort �� Index�� Ÿ������ ����Ѵ�.
	{
	case FX_PARTICLE_TYPE_L    : // 2007.03.14 ȭ��Ʈ
	case FX_PARTICLE_TYPE_O    :
	case FX_PARTICLE_TYPE_V    :
	case FX_PARTICLE_TYPE_E    :
	case FX_PARTICLE_TYPE_LIP  :
	case FX_PARTICLE_TYPE_HEARTLORD: // �Ϻ� �߷�Ÿ�ε��� ������ ��Ʈ�ε�,���귯��ε� �� ����Ʈ. ��Ʈ���� �ȶ� �ȶ�~
	case FX_PARTICLE_TYPE_LUCKYPIG: // ���� ���� ȸ�� ����Ʈ. ��Ʈ�ε�� ���� ��ƾ�� Ÿ�� �ؽ��ĸ� �ٲ��൵ �ȴ�.

		// �ֺ� ����Ʈ ���
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		// ��ƼŬ ���.
		m_pParticleInfoArray = (Deco_Particle *)effect_sort.effects; // ������ effect_sort ������ �Ǿ������� �����͸� �޾ƿͼ� ó��.

		if( !m_pParticleInfoArray ) // ��ƼŬ�� ������ ���ϸ� �̰� NULL�� �ȴ�.
			return FALSE; // ����Ʈ�� ������ �˷��ش�.

		if( effect_sort.Index == FX_PARTICLE_TYPE_HEARTLORD )
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_HEART_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_LUCKYPIG )
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LUCKYPIG_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_LUCKYPIG;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_LIP) // 2007.03.14 ȭ��Ʈ
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LIP_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_L)   // 2007.03.14 ȭ��Ʈ
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_L_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_O)   // 2007.03.14 ȭ��Ʈ
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_O_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_V)   // 2007.03.14 ȭ��Ʈ
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_V_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}
		else if( effect_sort.Index == FX_PARTICLE_TYPE_E)   // 2007.03.14 ȭ��Ʈ
		{
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LOVE_E_PARTICLE ] ) );
			k = MAX_PARTICLE_FOR_HEARTLOAD;
		}

		for( i = 0 ; i < k ; ++i )
		{
			if( m_pParticleInfoArray[i].cur_time < 0 ) // �� ���� ���� �����ȵ� �༮.
				continue;
			if( m_pParticleInfoArray[i].cur_time > m_pParticleInfoArray[i].life_time ) // ������ ��������...
				continue;


			rectwidth = m_pParticleInfoArray[i].scale * 1.0f;

			if( m_pParticleInfoArray[i].texFrame == 2 ) // Ŀ���� �۾�����.
				rectwidth += m_pParticleInfoArray[i].direction.z;



			if( m_pRectPlane )
			{
				m_pRectPlane->ChangeVertexInfo( 0, D3DLVERTEX( D3DVECTOR(-rectwidth,-rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 0.0f, 1.0f )  );
				m_pRectPlane->ChangeVertexInfo( 1, D3DLVERTEX( D3DVECTOR(-rectwidth,rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 0.0f, 0.0f )  );
				m_pRectPlane->ChangeVertexInfo( 2, D3DLVERTEX( D3DVECTOR(rectwidth,-rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 1.0f, 1.0f )  );
				m_pRectPlane->ChangeVertexInfo( 3, D3DLVERTEX( D3DVECTOR(rectwidth,rectwidth,0.0f)
												, D3DRGBA(m_pParticleInfoArray[i].red,m_pParticleInfoArray[i].blue,m_pParticleInfoArray[i].green,m_pParticleInfoArray[i].fade), 0, 1.0f, 0.0f )  );
			}

			m_pRectPlane->SetBillBoard( vViewVec, FALSE, 0.0f );

			if( m_pParticleInfoArray[i].texFrame == 1 ) // �¿�� ����� ȸ��
				m_pRectPlane->RotateZ( m_pParticleInfoArray[i].direction.z );


			m_pRectPlane->SetTranslationXYZ( m_pParticleInfoArray[i].cur_Pos.x
											 , m_pParticleInfoArray[i].cur_Pos.y, m_pParticleInfoArray[i].cur_Pos.z );

			m_pRectPlane->Render();
		}

		break;
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return FALSE;
}