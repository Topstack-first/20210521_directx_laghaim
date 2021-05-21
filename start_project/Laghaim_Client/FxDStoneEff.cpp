#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "Land.h"
#include "skill.h"
#include "ImageDefine.h"
#include "FxDStoneEff.h"
#include "NkMob.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxDStoneEff::CFxDStoneEff()
{
	corona1 = new CRectPlane;
	corona2 = new CRectPlane;
	corona3 = new CRectPlane;  //-- Dummy

	if( corona1 )
		corona1->Create( 3.0f, 3.0f, FALSE );
	if( corona2 )
		corona2->Create( 3.0f, 3.0f, FALSE );
	if( corona3 )
		corona3->Create( 3.0f, 3.0f, FALSE );

	m_fScale0 = 0.0f;
	m_fScale1 = 0.0f;
	m_fFade0 = 0.0f;
	m_fFade1 = 0.0f;
	m_rotang = 0.0f;
}

CFxDStoneEff::~CFxDStoneEff()
{
	SAFE_DELETE(corona1);
	SAFE_DELETE(corona2);
	SAFE_DELETE(corona3);
}

//-- 0 ~ 10 frame : Ȯ�� Effect
//-- 10 ~ 70 frame : ȸ��
BOOL CFxDStoneEff::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	float angle = 0.0f;
	int y = 0.0f;

	//-- Position : ���� ������ ��� ����Ʈ �������� �����־ ���� ����
	if(!effect_sort.pMobFrom || effect_sort.pMobFrom->m_RunSmokeFrame ) // ����转��(����转�� ���� ���� Ÿ�ֺ̹���)�� ����Ʈ �����ϵ���.. ///�����޺�
	{
		effect_sort.nCurFrame = FX_DSTONEEFF_FRAMECOUNT;
		return TRUE;
	}

	//-- Vector
	D3DVECTOR	vViewVec;
	D3DVECTOR	vtestVec;
	D3DVECTOR		vAdj;
	D3DVECTOR		vDrawPos;

	vDrawPos = D3DVECTOR( effect_sort.pMobFrom->m_wx, effect_sort.pMobFrom->m_wy + 4.0f, effect_sort.pMobFrom->m_wz  );

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];
	//-- Store State
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//-- ������ �ʱ�ȭ �� ���ѹݺ�
	if( effect_sort.nCurFrame > 90 && effect_sort.pMobFrom)
		effect_sort.nCurFrame = 10;

	//--------------------------------------------------------------------------
	//-- Scale , Alpha
	if( effect_sort.nCurFrame < 90 ) //-- �ܰ� 2 : ȸ�� 62 �� ���� ������.
	{
		m_fScale1 =(effect_sort.nCurFrame % 40) / 0.8f; //-- scale
		m_fFade1 = (40-(effect_sort.nCurFrame % 40 )) / 50.0f;   //-- scale fade
		y = effect_sort.nCurFrame % 40 - 20;
		m_fScale0 = 10.0f; //-- rot
		m_fFade0 = (40-(effect_sort.nCurFrame % 40 )) / 40.0f;   //-- rot
	}

	vtestVec.dvX=0;
	vtestVec.dvY =-10;
	vtestVec.dvZ = 0;
	// �Ÿ��� ����߸���.
	vAdj = effect_sort.vTargetPos - effect_sort.vPos;
	vAdj = Normalize(vAdj);

	//------------------------------------------------------------------------
	//-- Rotation Plane
	angle = float(effect_sort.nCurFrame - 20)*3.1416f / 62.0f ;

	//-- Set Texture _0	:rot
	if( g_pCapsyongTexture )
	{
		if(effect_sort.subNum  == 5086)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_5_0 ] ) );
		if(effect_sort.subNum  == 5085)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_4_0 ] ) );
		if(effect_sort.subNum  == 5081)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_3_0 ] ) );
		if(effect_sort.subNum  == 5080)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_2_0 ] ) );
		if(effect_sort.subNum  == 367  || effect_sort.subNum  == 778 )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_1_0 ] ) );
		if(effect_sort.subNum  == 368)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_0_0 ] ) );
	}

	if( corona1 )
	{
		corona1->SetBillBoard(vtestVec, FALSE, 0.0f);	//-- Make Billboard
		corona1->Scale(m_fScale0, m_fScale0, 0.0f);   	//-- Scale
		corona1->SetColor(1.0f, 1.0f, 1.0f,m_fFade0);	//-- color
		corona1->RotateZ(angle);//-- Rotation
		corona1->SetTranslationXYZ(vDrawPos.x, vDrawPos.y + y, vDrawPos.z);
		corona1->Render();
	}

	//------------------------------------------------------------------------
	//-- Scaling Plane
	//-- Set Texture _1	:scale
	if( g_pCapsyongTexture )
	{
		if(effect_sort.subNum  == 367  || effect_sort.subNum  == 778 )
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_0_1 ] ) );
		if(effect_sort.subNum  == 368)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_SMOKE ] ) );
		if(effect_sort.subNum  == 5080)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_SMOKE2 ] ) );
		if(effect_sort.subNum  == 5081)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_0_2 ] ) );
		if(effect_sort.subNum  == 5085)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_0_3 ] ) );
		if(effect_sort.subNum  == 5086)
			GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_DSTONE_EFF_0_4 ] ) );
	}

	if( corona2 )
	{
		corona2->SetBillBoard(vtestVec, FALSE, 0.0f);	//-- Make Billboard
		corona2->Scale(m_fScale1, m_fScale1, 0.0f);	    //-- Scale
		corona2->SetColor(1.0f, 1.0f, 1.0f, m_fFade1);	//-- color
		corona2->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		corona2->Render();
	}

	//-- Restore Render State
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	return false;
}

