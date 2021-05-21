#include "stdafx.h"
#include <assert.h>
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxAssault.h"
#include "GUtil.h"
#include "IndexedTexture.h"
#include "NkMob.h"
#include "NkCharacter.h"
#include "FX_BOSSRAID_KAI1_02.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBossRaidKai1_02::CFxBossRaidKai1_02()
{
	m_pLolo = NULL;
}

CFxBossRaidKai1_02::~CFxBossRaidKai1_02()
{
	SAFE_DELETE(m_pLolo);
}

void CFxBossRaidKai1_02::LoadRes()
{
	if (!m_pLolo)
	{
		m_pLolo = new CLolos;
		m_pLolo->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo->Read("data/effect/lolos/boss_k01_attack02_eff.LOL");
	}



}

void CFxBossRaidKai1_02::DeleteRes()
{
}

//-- num 1 : assault
//--     2 : D Assault
bool CFxBossRaidKai1_02::Render(EffectSort &effect_sort)
{
	//-- 사용 캐릭이 사라질 경우
	if(!GET_D3DDEVICE() )
		return true;

	DWORD  dwTextureState[3];
	DWORD  dwSrc, dwDest;
	DWORD alphaBlend, dwLighting, dwAmbient, dwFogEnable;

	D3DVECTOR vEyeVec = pCMyApp->GetEyeDirection();

	D3DMATRIX matTemp1, matTemp2;
	D3DMATRIX matTrans, matRotate, matScale, matWorld;
	D3DVECTOR vDrawPos;

	vDrawPos =  effect_sort.vTargetPos;
	vDrawPos.y += 14.0f;

	//--------------------------------------------------------------------------
	//-- Set Render State
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_AMBIENT, &dwAmbient);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//if ( effect_sort.nCurFrame >= 20 && effect_sort.nCurFrame < 70)
	{
		D3DUtil_SetIdentityMatrix(matScale);
		D3DUtil_SetIdentityMatrix(matWorld);
		D3DUtil_SetIdentityMatrix(matTrans);
		D3DUtil_SetIdentityMatrix(matRotate);

		/*float fTime = effect_sort.nCurFrame - 20;
				D3DVECTOR vDiff = Normalize(effect_sort.vTargetPos - effect_sort.vCurPos) * 8.0f * (fTime+2.5);
				D3DVECTOR vDiff2 = effect_sort.vTargetPos - effect_sort.vCurPos;
				float theta = atan2(vDiff2.z, vDiff2.x);
				D3DVECTOR vPos = effect_sort.vCurPos + vDiff + D3DVECTOR(0, 16, 0);

				D3DUtil_SetRotateXMatrix(matTemp2, g_PI * 0.20f);
				Nk3DUtil_SetRotateCCWYMatrix(matRotate, theta );
				D3DMath_MatrixMultiply(matRotate, matTemp2, matRotate);

				D3DUtil_SetTranslateMatrix( matTrans, vPos );
				D3DMath_MatrixMultiply(matTemp1, matRotate, matTrans);
				matScale._11 = matScale._22 = matScale._33 = sqrt(fTime) * 0.2f + 0.6f;
				D3DMath_MatrixMultiply(matWorld, matScale, matTemp1);
		*/

		D3DUtil_SetTranslateMatrix( matWorld, effect_sort.vCurPos );
		//D3DMath_MatrixMultiply(matTemp1, matRotate, matTrans);

		DWORD a = (DWORD)0xFF;
		DWORD w = (a << 24) | (a << 16) | (a << 8) | (a);

		GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
		m_pLolo->RenderRaw(GET_D3DDEVICE(), effect_sort.nCurFrame,
						   FALSE, FALSE, FALSE, NULL, NULL, FALSE, FALSE, w);
	}

	//--------------------------------------------------------------------------
	//--Restore Render State
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );

	return FALSE;
}
