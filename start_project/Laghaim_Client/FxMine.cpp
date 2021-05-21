#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"
#include "Land.h"
#include "FxMine.h"




extern int g_EffectSound[];
extern float g_fDSDistance;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxMine::CFxMine()
{
	m_pCapsyong = NULL;
	ZeroMemory( m_byState, sizeof(m_byState) );
	m_nArrivalFrame = 0;

	for( int i = 0 ; i < 3 ; i ++ )
		m_pBpd[ i ] = NULL;
}

CFxMine::~CFxMine()
{
	DeleteRes();
}

void CFxMine::LoadRes()
{
	DeleteRes();

	m_pCapsyong = new CCapsyong(g_pCapsyongTexture);
	if( VF_ERR_NONE > m_pCapsyong->Read( "data/effect/csy/H_Skill_bug.CSY" ) )
	{
		SAFE_DELETE(m_pCapsyong);
		return;
	}

	char szTemp[ 200 ] = {0,};
	for( int i = 0 ; i < 3 ; i ++ )
	{
		sprintf( szTemp, "data/effect/bpd/H_Skill_bug_%d.BPD", i );
		m_pBpd[ i ] = new CBiped;
		if( VF_ERR_NONE > m_pBpd[i]->Read( szTemp ) )
		{
			SAFE_DELETE(m_pBpd[i]);
			return;
		}
	}

}

void CFxMine::DeleteRes()
{
	SAFE_DELETE( m_pCapsyong );

	for( int i = 0 ; i < 3 ; i ++ )
		SAFE_DELETE( m_pBpd[ i ] );
}


BOOL CFxMine::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	if( effect_sort.nCurFrame > 70 )
		return TRUE;

	BOOL bFinished = FALSE;
	D3DMATRIX matTrans, matView, matWorld;
	DWORD dwLighting;
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos;
	int end_frame = 0, nPlayFrame = 0;
	end_frame = effect_sort.nCurFrame;
	vPosFrom = effect_sort.vPos;

//===========================================================================
	// ���⺤�͸� ���Ѵ�.
	// ������ġ�� ����. ���� ��ü�� ������ ���� �ֱ� ������.
	// Ÿ�� ��ǥ�� ���Ѵ�.
	if (effect_sort.pMobTo)
		vPosTo = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz);
	else if (effect_sort.pNkChaTo)
		vPosTo = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz);
	else
		vPosTo = effect_sort.vTargetPos;

	// ����.
	get_normal_vector(vDir, vPosFrom, vPosTo);
	// ���� �����ӿ��� 8������ ������ �׸���.
	if( effect_sort.subNum )	// ���� ����
	{
		m_byState[ effect_sort.Index ] = 4;
		vPosFrom.x += vDir.x * 5 * effect_sort.subNum;
		vPosFrom.z += vDir.z * 5 * effect_sort.subNum;
		vDrawPos = vPosFrom;
	}
	else if( !m_byState[ effect_sort.Index ] )	// ��ǥ�� ������
	{
		vDir *= 4.0f;	// �̵�
		vDrawPos.x = vPosFrom.x + vDir.x * (end_frame + 1);
		vDrawPos.y = vPosFrom.y + vDir.y * (end_frame + 1);
		vDrawPos.z = vPosFrom.z + vDir.z * (end_frame + 1);
	}
	else	// ��ǥ�� ������
		vDrawPos = vPosTo;

	// �Ÿ� ���
	float fDist = DistPlToPlAbs(vPosTo.x, vPosTo.z, vPosFrom.x, vPosFrom.z) -
				  DistPlToPlAbs(vDrawPos.x, vDrawPos.z, vPosFrom.x, vPosFrom.z);

	CBiped* pBiped = NULL;
	if( m_byState[ effect_sort.Index ] == 4 )	// ���� ���н�
	{
		pBiped = m_pBpd[ 2 ];
		nPlayFrame = effect_sort.nCurFrame;
		if( nPlayFrame > 40 )	// ���� ������ ����
			return FALSE;
	}
	else if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)	// ��ǥ���� ����������
	{
		if( m_byState[ effect_sort.Index ] == 0 )
		{
			m_nArrivalFrame = effect_sort.nCurFrame;	// ��ǥ���� ������������ �������� ����
			m_byState[ effect_sort.Index ] = 1;
		}

		pBiped = m_pBpd[ 1 ];	// ���� �ξ�
		nPlayFrame = effect_sort.nCurFrame - m_nArrivalFrame;	// �����ĺ��� �ٽ� ������ ���
	}
	else
	{
		m_byState[ effect_sort.Index ] = 0;
		pBiped = m_pBpd[ 0 ];	// �̵�
		nPlayFrame = effect_sort.nCurFrame;
	}

	if( nPlayFrame < 0 )
		nPlayFrame = 0;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );
//===========================================================================
	// ����
	if( m_pCapsyong
			&& pBiped
			&& m_byState[ effect_sort.Index ] != 2 )
	{
		D3DUtil_SetIdentityMatrix( matTrans );
		D3DUtil_SetTranslateMatrix( matTrans, vDrawPos.x, vDrawPos.y, vDrawPos.z);
		pCMyApp->SetViewMatrix(matView);
		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matTrans );
		m_pCapsyong->Animate(pBiped, nPlayFrame,
							 TRUE, LINE_EFF_NONE, &matWorld, &matView);
		m_pCapsyong->Render(GET_D3DDEVICE(), -1, FALSE, FALSE, NULL, effect_sort.nCurFrame);
	}
//===========================================================================
	//����
	if ( m_byState[ effect_sort.Index ] == 1	// ���װ� ������ �����ߴ�.
			&& nPlayFrame > 20 )				// ������ 20������(���ߺξ���) ������
	{
		// ���� ������ �� �¾Ҵٰ� �˷�e�ش�.
		// �׷����ν� ���� ���� ������ �ִ� effect_num�� �����ȴ�.
		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		// ���� ȿ���� ȣ���Ѵ�.
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_FIREBALLBOOM;	// ���� �Ҳ�
		ef_sort.vPos = vPosTo;
		ef_sort.vTargetPos = vPosTo;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		if (g_pDSound)
			g_pDSound->Play(g_EffectSound[EFF_SND_BOOM1], (vPosTo.x- g_pRoh->m_wx)/g_fDSDistance, (vPosTo.y- g_pRoh->m_wy)/g_fDSDistance, (vPosTo.z-g_pRoh->m_wz)/g_fDSDistance);

		m_byState[ effect_sort.Index ] = 2;
		bFinished = TRUE;
	}
//===========================================================================
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);

	return bFinished;
}
