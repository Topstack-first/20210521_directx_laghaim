#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "uimgr.h"
#include "NkMob.h"
#include "NkPet.h"
#include "ItemProto.h"
#include "NkCharacter.h"
#include "GUtil.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "ShadowRect.h"
#include "FxDragonEffects.h"
#include "CsyManager.h"


extern int g_EffectSound[];
extern float g_fDSDistance;
extern BOOL	g_bDungeon;

extern CPetBpd *g_PetBiped;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDragonEffects::CFxDragonEffects()
{
	m_fSpeed = 6.0f;
}

CFxDragonEffects::~CFxDragonEffects()
{
	DeleteRes();
}

void CFxDragonEffects::LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect)
{}

void CFxDragonEffects::DeleteRes()
{}

BOOL CFxDragonEffects::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo
			&& !effect_sort.pMobFrom )
		return TRUE;

	CNkMob* pMob = pCMyApp->m_pUIMgr->FindMobById(effect_sort.Index);
	CNkPet *pPet = NULL;

	if( pMob == NULL )
	{
		pPet = pCMyApp->m_pUIMgr->FindPetById(effect_sort.Index); // 몹중에 없었다면 펫에서 찾는다.(펫 스킬이 추가되서 펫이 쓸수도있다.)
		if( pPet == NULL ) // 펫도 없으면 진짜 문제가 있는거다.
		{
			effect_sort.nCurFrame = FX_DRAGONEFFECTS_FRAMECOUNT;
			return TRUE; // 이거 사실 문제 있는듯. 마더드래곤 브레스 등에 튕김 문제 해결을 위한 코드로 보이나 캐릭터가 이 이펙트를 사용시를 고려안되어있는 것 같다. ( 나중에 캐릭터에 사용할 필요가 있을때 수정하자.)
		}
	}

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	int nFrame = 0;
	nFrame = effect_sort.nCurFrame;

	// 방향벡터를 구한다.
	D3DVECTOR vDir, vPosFrom, vPosTo;
	D3DVECTOR vDrawPos; //, vDrawPosFrom;

	// 화이어볼 마법은 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;

	D3DMATRIX	matRot;
	D3DVECTOR	vTmp;

	if( effect_sort.num == 2 ) // 펫 브레스는 펫의 헤드에서 라인을 찾아야한다.
	{
		CNkPet *pPet = (CNkPet *)effect_sort.pMobFrom;

		int HeadItemIndex = pPet->m_BodyItem[PET_PIECE_HEAD]; // 라비용 헤드 파츠의 아이템 인덱스.

		if( HeadItemIndex == -1 )
			return TRUE;

		CCapsyong *pCapSyong = 	CsyManager::GetInstance()->GetCsy(g_ItemList.m_ItemArray[HeadItemIndex].m_CsyFile[0]);

		if( !pCapSyong || !pCapSyong->m_LineVector || !pCapSyong->m_LineVector0 )
			return TRUE;

		NKMath_VectorMatrixMultiply(   pCapSyong->m_LineVector[0], pCapSyong->m_LineVector0[0],
									   g_PetBiped[pPet->m_ActNum].m_Bpd.m_AniMat[ pCapSyong->m_LineBiped[0] ].m_Mats[ pPet->GetCurFrame() ]    );

		Nk3DUtil_SetRotateCCWYMatrix( matRot, pPet->m_dir );
		D3DMath_VectorMatrixMultiply( vTmp, pCapSyong->m_LineVector[0], matRot );
		vPosFrom = D3DVECTOR(	vTmp.x + pPet->m_wx,
								vTmp.y + pPet->m_wy,
								vTmp.z + pPet->m_wz );
	}
	else if( effect_sort.pMobFrom )
	{
		if( !(effect_sort.pMobFrom->m_nMobNum == 222 ||
				effect_sort.pMobFrom->m_nMobNum == 223 ||
				effect_sort.pMobFrom->m_nMobNum == 265 ||
				effect_sort.pMobFrom->m_nMobNum == 224||effect_sort.pMobFrom->m_nMobNum == 333 ||
				effect_sort.pMobFrom->m_nMobNum ==  737 ||
				effect_sort.pMobFrom->m_nMobNum ==  738 ||
				effect_sort.pMobFrom->m_nMobNum ==  739
			 )
		  )
		{
			//강화 마더드래곤
			effect_sort.nCurFrame = FX_DRAGONEFFECTS_FRAMECOUNT;
			return TRUE;
		}

		// 이부분에서 에이디아의 순간이동시 메모리 참조 오류 발생
		NKMath_VectorMatrixMultiply(effect_sort.pMobFrom->m_pCapSyong->m_LineVector[0],
									effect_sort.pMobFrom->m_pCapSyong->m_LineVector0[0],
									effect_sort.pMobFrom->GetBiped(effect_sort.pMobFrom->GetMotionState())->m_AniMat[effect_sort.pMobFrom->m_pCapSyong->m_LineBiped[0]].m_Mats[effect_sort.pMobFrom->GetCurFrame()]);

		Nk3DUtil_SetRotateCCWYMatrix(matRot, effect_sort.pMobFrom->m_dir);
		D3DMath_VectorMatrixMultiply(vTmp, effect_sort.pMobFrom->m_pCapSyong->m_LineVector[0], matRot);
		vPosFrom = D3DVECTOR(	vTmp.x + effect_sort.pMobFrom->m_wx,
								vTmp.y + effect_sort.pMobFrom->m_wy,
								vTmp.z + effect_sort.pMobFrom->m_wz );
	}
	else
		vPosFrom = effect_sort.vPos;

	vPosTo = effect_sort.vTargetPos;
	vPosTo.y += 15.0f;
	get_normal_vector(vDir, vPosFrom, vPosTo);

	// 프레임당 스피드.
	if( effect_sort.num == 2 ) // 펫 브레스는 기존 것보다 느리게.
		vDir *= m_fSpeed * RandomNum(1.0f, 2.0f);
	else
		vDir *= m_fSpeed * RandomNum(3.0f, 5.0f);

	if( g_pCapsyongTexture )
	{
		if( effect_sort.num == 0 || effect_sort.num == 2 ) // Fire Flame Effects...
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DRAGON_FIRETAIL ] ) ;
		else if( effect_sort.num == 1 ) // Ice Flame Effects...
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_DRAGON_ICETAIL ] ) ;
	}

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	if( effect_sort.num == 2 ) // 펫 브레스는 기존 것보다 짧고 작게 해준다.
	{
		int	temp = nFrame / 5;
		int start_frame = temp * 5;
		int end_frame = nFrame - start_frame;

		int	i = 0;
		float fire_scale = 5.0f;
		float fire_fade = 0.5f;

		for( i = 0; i < end_frame; i++ )
		{
			vDrawPos.x = vPosFrom.x + (vDir.x+RandomNum(-1.75f, 1.75f)) * (i+1);
			vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
			vDrawPos.z = vPosFrom.z + (vDir.z+RandomNum(-1.75f, 1.75f)) * (i+1);

			matWorld._41 = vDrawPos.x;
			matWorld._42 = vDrawPos.y;
			matWorld._43 = vDrawPos.z;

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

			MakeLVertexRect(m_lverTail, fire_scale, fire_scale, TRUE);

			m_lverTail[0].tu = i * 0.125f;
			m_lverTail[1].tu = i * 0.125f;
			m_lverTail[2].tu = (i+1) * 0.125f;
			m_lverTail[3].tu = (i+1) * 0.125f;

			if( effect_sort.num == 0 )
			{
				// Fire Flame Effects...
				m_lverTail[0].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
				m_lverTail[1].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
				m_lverTail[2].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
				m_lverTail[3].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
			}
			else if( effect_sort.num == 1 )
			{
				// Ice Flame Effects...
				m_lverTail[0].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
				m_lverTail[1].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
				m_lverTail[2].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
				m_lverTail[3].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
			}

			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
											m_lverTail, 4, 0 );
			fire_scale += 1.5f;
			fire_fade += 0.04f;
		}
	}
	else
	{
		int	temp = nFrame / 8;
		int start_frame = temp * 8;
		int end_frame = nFrame - start_frame;

		int	i = 0;
		float fire_scale = 13.0f;
		float fire_fade = 0.5f;

		for( i = 0; i < end_frame; i++ )
		{
			vDrawPos.x = vPosFrom.x + (vDir.x+RandomNum(-1.75f, 1.75f)) * (i+1);
			vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
			vDrawPos.z = vPosFrom.z + (vDir.z+RandomNum(-1.75f, 1.75f)) * (i+1);

			matWorld._41 = vDrawPos.x;
			matWorld._42 = vDrawPos.y;
			matWorld._43 = vDrawPos.z;

			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

			MakeLVertexRect(m_lverTail, fire_scale, fire_scale, TRUE);

			m_lverTail[0].tu = i * 0.125f;
			m_lverTail[1].tu = i * 0.125f;
			m_lverTail[2].tu = (i+1) * 0.125f;
			m_lverTail[3].tu = (i+1) * 0.125f;

			if( effect_sort.num == 0 )
			{
				// Fire Flame Effects...
				m_lverTail[0].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
				m_lverTail[1].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
				m_lverTail[2].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
				m_lverTail[3].color = D3DRGBA(1.0f, fire_fade, fire_fade, fire_fade);
			}
			else if( effect_sort.num == 1 )
			{
				// Ice Flame Effects...
				m_lverTail[0].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
				m_lverTail[1].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
				m_lverTail[2].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
				m_lverTail[3].color = D3DRGBA(fire_fade, fire_fade, 1.0f, fire_fade);
			}

			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,
											m_lverTail, 4, 0 );
			fire_scale += 5.5f;
			fire_fade += 0.04f;
		}
	}

	BOOL bFinished = FALSE;

	if(effect_sort.nCurFrame >= 8)
	{

		if (effect_sort.pMobTo)
			effect_sort.pMobTo->Attacked();
		else if (effect_sort.pNkChaTo)
			effect_sort.pNkChaTo->Attacked();

		bFinished = TRUE;
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return bFinished;
}

