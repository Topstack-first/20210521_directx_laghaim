#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "ImageDefine.h"
#include "FxBleed.h"
#include "GUtil.h"
#include "NkCharacter.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBleed::CFxBleed()
{}

CFxBleed::~CFxBleed()
{}


//----------------------------------------------------------------------------
//-- ���� Boom
//----------------------------------------------------------------------------
//-- 2004.12.06
//-- Lyul
BOOL CFxBleed::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;
	//-- ��� ĳ���� ����� ���
	if(!effect_sort.pNkChaFrom)
		return true;

	DWORD  dwTextureState[3];
	int i;
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	//-- �����带 �����غ�.
	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	//--------------------------------------------------------------------------
	//-- Set Render State
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	//--------------------------------------------------------------------------
	//-- Effect �� ��ǥ�� �����Ѵ�.
	vPosFrom.x = effect_sort.pNkChaFrom->m_wx;
	vPosFrom.y = effect_sort.pNkChaFrom->m_wy;
	vPosFrom.z = effect_sort.pNkChaFrom->m_wz;
	vPosFrom.y += 15.0f;

	float fade = 1.0f;
	float scale = 1.0f;
	//--------------------------------------------------------------------------
	//-- Set Texture
	if(g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0,g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_BLEED_BOOM ] ));

	for(i =0 ; i< 4 ; i++)
	{
		fade = 1.0f;
		scale = 1.0f;
		//-- ������ ����.
		matWorld._41 = vPosFrom.x;
		matWorld._42 = vPosFrom.y;
		matWorld._43 = vPosFrom.z;

		//-- �ൿ ��� ����
		switch(i)
		{
		case 1:
		case 2:
			scale += effect_sort.nCurFrame*effect_sort.nCurFrame/5.0f;
			if(effect_sort.nCurFrame > 6)
				fade = float(10 - effect_sort.nCurFrame )/4.0f;
			break;

		case 3 :
		default:
			scale += effect_sort.nCurFrame*2.7f;
			if(effect_sort.nCurFrame > 6)
				fade = float(10 - effect_sort.nCurFrame )/4.0f;
			break;
		}

		if(effect_sort.nCurFrame > 10)
			fade = 0.0f;

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		MakeLVertexRect(m_flame, scale, scale, TRUE);

		//-- Texture Animation ����.
		m_flame[0].tu = i*0.25;
		m_flame[1].tu = i*0.25;
		m_flame[2].tu = (i+1)*0.25;//((effect_sort.nCurFrame%4)+1) * 0.25f;
		m_flame[3].tu = (i+1)*0.25;//((effect_sort.nCurFrame%4)+1) * 0.25f;
		//-- Color ����
		for(int t=0; t<4; t++)
			m_flame[t].color = D3DRGBA(1.0f,1.0f,1.0f,fade);

		//-- Render
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
	}
	//--------------------------------------------------------------------------
	//--Restore Render State
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	return FALSE;
}

