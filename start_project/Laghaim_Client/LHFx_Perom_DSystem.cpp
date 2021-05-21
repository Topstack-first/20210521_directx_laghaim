#include "stdafx.h"
#include "main.h"
#include "FxSet.h"
#include "NkCharacter.h"
#include "NkMob.h"

#include "LHFx_Perom_DSystem.h"
#include "ItemProto.h"

const int male_frame = 2;
const int fmale_frame = 2;

LHFx_Perom_DSystem::LHFx_Perom_DSystem(void)
	:	m_lolo(NULL)
{
}

LHFx_Perom_DSystem::~LHFx_Perom_DSystem(void)
{
	SAFE_DELETE(m_lolo);
}

void LHFx_Perom_DSystem::LoadRes()
{
// 	if (!m_lolo)
// 	{
// 		m_lolo = new CLolos;
// 		m_lolo->SetIndexedTexture( g_pCapsyongTexture );
// 		m_lolo->Read("data/effect/lolos/pm_skill_d_system.LOL");
// 	}
}

void LHFx_Perom_DSystem::DeleteRes()
{

}

bool LHFx_Perom_DSystem::Render(EffectSort &effect_sort)
{
// 	if( !GET_D3DDEVICE() || !m_lolo )
// 		return false;
//
// 	if( effect_sort.pNkChaFrom && !effect_sort.pNkChaFrom->m_Wearing[WEARING_CLOAK] )
// 		return false;
//
// 	CCapsyong * csy = GET_CSY(effect_sort.pNkChaFrom->m_Wearing[WEARING_CLOAK] , effect_sort.pNkChaFrom->m_BpdRace);
// 	if( !csy )
// 		return false;
//
// 	int bpd_index = csy->m_Csy[0].m_BipedIdx[0];
// 	CBiped * bpd = effect_sort.pNkChaFrom->GetBiped();
// 	D3DMATRIX * mat = &bpd->m_AniMat[bpd_index].m_Mats[effect_sort.nCurFrame];
//
// 	D3DMATRIX matWorld, matView, matRot;
// 	DWORD dwLighting, dwZWrite, dwSrc, dwDest;
//
// 	CopyMemory( &matWorld, &bpd->m_AniMat[bpd_index].m_Mats[effect_sort.nCurFrame] , sizeof(D3DMATRIX) );
//
// 	matWorld._41 = effect_sort.pNkChaFrom->m_wx;
// 	matWorld._42 = effect_sort.pNkChaFrom->m_wy;
// 	matWorld._43 = effect_sort.pNkChaFrom->m_wz;
//
//
//
//
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );
//
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
//
// 	pCMyApp->SetViewMatrix(matView);
//
// 	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
// 	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
//
//  	if( effect_sort.pNkChaFrom )
//  		m_lolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame%2, FALSE, FALSE);
//
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );
//
//


	/// pm_d-system01.CSY

// 	if( !effect_sort.pNkChaFrom->IsActiveDSystem() )
// 		return false;


// 	D3DMATRIX matTrans;
// 	D3DVECTOR vScr, vTemp;
//
// 	int actNum = effect_sort.pNkChaFrom->GetBiped()->m_AnimCount;
// 	//actNum = 0;
//
// 	vTemp = D3DVECTOR( effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()]._41 + 0.0f,
// 		effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()]._42 - 0.0f,
// 		effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()]._43 - 0.0f );
//
// 	D3DUtil_SetIdentityMatrix(matTrans);
// 	D3DUtil_SetTranslateMatrix( matTrans, vTemp );
// 	Nk3DUtil_SetRotateCCWYMatrix( matTrans, effect_sort.pNkChaFrom->m_dir );
// 	D3DMath_VectorMatrixMultiply(vScr, vTemp, matTrans);
//
// 	effect_sort.vCurPos.x = effect_sort.pNkChaFrom->m_wx + vScr.x;
// 	effect_sort.vCurPos.y = effect_sort.pNkChaFrom->m_wy + vScr.y;
// 	effect_sort.vCurPos.z = effect_sort.pNkChaFrom->m_wz + vScr.z;
//
// 	effect_sort.vSortPos.y = 0.0f;
// 	effect_sort.vPos.y = 0.0f;
// 	effect_sort.subNum = 0;
//
// 	int frame = 0;
// 	if( effect_sort.pNkChaFrom->m_Sex == 0)
// 		frame = male_frame;
// 	else
// 		frame = fmale_frame;
//

//
// 	D3DUtil_SetIdentityMatrix( matWorld );
// //	D3DMath_MatrixMultiply( matWorld, effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()], matWorld );
// 	D3DUtil_SetTranslateMatrix( matWorld, effect_sort.vCurPos);
// 	D3DUtil_SetIdentityMatrix( matRot );
// 	D3DUtil_SetRotateYMatrix( matRot, -effect_sort.pNkChaFrom->m_dir );
// 	D3DMath_MatrixMultiply( matWorld, effect_sort.pNkChaFrom->GetBiped()->m_AniMat[actNum].m_Mats[effect_sort.pNkChaFrom->Get_m_CurFrame()], matWorld );
// 	D3DMath_VectorMatrixMultiply(vScr, vTemp, matTrans);
//
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
// 	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );
//
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
//
// // 	D3DUtil_SetTranslateMatrix( matWorld, effect_sort.pNkChaFrom->m_wx-0.0f, effect_sort.pNkChaFrom->m_wy-0.0f, effect_sort.pNkChaFrom->m_wz+0.0f);
// //
// // 	D3DUtil_SetIdentityMatrix( matRot );
// // 	D3DUtil_SetRotateYMatrix( matRot, -effect_sort.pNkChaFrom->m_dir );
// // 	D3DMath_MatrixMultiply( matWorld, matRot, matWorld );
//
// 	pCMyApp->SetViewMatrix(matView);
//
// 	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
// 	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
//
//  	if( effect_sort.pNkChaFrom )
//  		m_lolo->Render(GET_D3DDEVICE(), effect_sort.nCurFrame, FALSE, FALSE);
//
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
// 	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return false;
}