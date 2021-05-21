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




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxAssault::CFxAssault()
	: mb_sound(false)
{
	m_board = NULL;
	m_board =  new CRectPlane;
	m_board->Create( 10.0f, 10.0f, FALSE );
}

CFxAssault::~CFxAssault()
{
	SAFE_DELETE(m_board);
}

//-- num 1 : assault
//--     2 : D Assault
BOOL CFxAssault::Render(EffectSort &effect_sort)
{
	//-- ��� ĳ���� ����� ���
	if(!GET_D3DDEVICE() || !m_board)
		return true;

	DWORD  dwTextureState[3];
	DWORD  dwSrc, dwDest;
	float x = 0;
	float z = 0;

	float fade = 1.0f;
	float scale = 1.0f;

	int st_inframe = 0;
	int st_outframe = 0;
	int ed_inframe = 0;
	int ed_outframe = 0;

	int i = 0,j = 0;
	int vFrame = 0;

	D3DMATRIX matR, matT, matWorld;
	D3DVECTOR	vtestVec;
	D3DVECTOR		vDrawPos;
	vtestVec = pCMyApp->GetEyeDirection();

	vDrawPos =  effect_sort.vTargetPos;
	vDrawPos.y += 14.0f;

	//--------------------------------------------------------------------------
	//-- Set Render State
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	//--------------------------------------------------------------------------
	//-- Virframe
	//-- Dassault
	if(effect_sort.num == 1)
	{
		vFrame = effect_sort.nCurFrame - 16;
		m_flame[0].tv = 1.0f;
		m_flame[2].tv = 1.0f;
		m_flame[3].tv = 0.0f;
		m_flame[1].tv = 0.0f;
	}
	//-- assault
	else
	{
		vFrame = effect_sort.nCurFrame - 9;
		m_flame[0].tv = 0.0f;
		m_flame[2].tv = 0.0f;
		m_flame[3].tv = 1.0f;
		m_flame[1].tv = 1.0f;
	}

	//----------------------------------------------------------------------
	//-- Board 1

	if(vFrame >0 && vFrame < 5)
	{
		if( m_board )
		{
			if(g_pCapsyongTexture)
				GET_D3DDEVICE()->SetTexture(0,g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_ASSAULT_BOOM ] ));
			//-- Dev1
			m_board->SetBillBoard(vtestVec, false, 0.0f);       //-- Set Billboard
			m_board->Scale(vFrame*2, vFrame*2, 0.0f);           //-- Scale
			m_board->SetColor(1.0f, 1.0f, 1.0f, vFrame / 10.0f);          //-- color
			m_board->SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
			m_board->Render();
		}
	}
	//--------------------------------------------------------------------------
	//-- Set Texture
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0,g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_ASSAULT ] ));

	//--------------------------------------------------------------------------
	//-- Effect �� ��ǥ�� �����Ѵ�.
	x = 28.0f;
	z = 0.0f;

	//--------------------------------------------------------------------------
	//-- World Matrix
	// ������ : �̿���
	// ������ : 05-03-24
	// �������� : pNkChaFrom�� ���� �ƴ� ���� �����Ͱ� �����Ǿ� ƨ��� ���װ� �־���.
	// ������ �Ʈ ��ų�� ����� ���� ������ ���� �����Ϳ��� ������ ��ƾ�� �׳� From�� To��
	// ���� ����ؼ� ���� ������� �ٲپ���. ( InsertMagic���� �־��ٶ� TargetPos�� Pos�� ����ϵ��� ���� )
	D3DVECTOR vDir;
	get_normal_vector(vDir, effect_sort.vPos, effect_sort.vTargetPos);

	float fDir = 0.0f;
	fDir = atan2f(vDir.z, vDir.x);

	Nk3DUtil_SetRotateCCWYMatrix( matR, fDir );
	D3DUtil_SetTranslateMatrix( matT, effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );
	D3DMath_MatrixMultiply( matWorld, matR, matT );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	//--------------------------------------------------------------------------
	//-- Render
	//--------------------------------------------------------------------------
	if(vFrame>0)
	{
		//--------------------------------------------------------------------------
		//-- Two Side
		for(j=0; j<2; j++)
		{
			if(j)
				//--cw
			{
				m_flame[2].x = x-40;
				m_flame[2].y = -5.0f;
				m_flame[2].z = -20.0f;
				m_flame[3].x = x-40;
				m_flame[3].y = 35.0f;
				m_flame[3].z = 20;
				m_flame[0].x = x;
				m_flame[0].y = -5.0f;
				m_flame[0].z = -20.0f;
				m_flame[1].x = x;
				m_flame[1].y = 35.0f;
				m_flame[1].z = 20;
			}
			//-- ccw
			else
			{
				m_flame[0].x = x-40;
				m_flame[0].y = -5.0f;
				m_flame[0].z = -20.0f;
				m_flame[1].x = x-40;
				m_flame[1].y = 35.0f;
				m_flame[1].z = 20;
				m_flame[2].x = x;
				m_flame[2].y = -5.0f;
				m_flame[2].z = -20.0f;
				m_flame[3].x = x;
				m_flame[3].y = 35.0f;
				m_flame[3].z = 20;
			}

			//-- Texture animation
			for(i =0 ; i< 4 ; i++)
			{
				fade = 1.0f;
				scale = 30.0f;

				st_inframe = i*1;
				ed_inframe = st_inframe + 2;
				st_outframe = i*1 + 2;
				ed_outframe = st_outframe + 2;


				//-- �ൿ ��� ����
				if(vFrame >=st_inframe && vFrame <ed_inframe )fade = (vFrame - st_inframe) /2.0f;
				else if(vFrame >=st_outframe && vFrame <ed_outframe )fade = (ed_outframe-vFrame) /2.0f;
				else fade =0.0f;

				if(vFrame > 18)
					fade = 0.0f;

				//-- Texture Animation ����.
				if(j)
				{
					m_flame[2].tu = i*0.25;
					m_flame[3].tu = i*0.25;
					m_flame[1].tu = (i+1)*0.25;
					m_flame[0].tu = (i+1)*0.25;
				}
				else
				{
					m_flame[0].tu = i*0.25;
					m_flame[1].tu = i*0.25;
					m_flame[2].tu = (i+1)*0.25;
					m_flame[3].tu = (i+1)*0.25;
				}

				//-- Color ����
				for(int t=0; t<4; t++)m_flame[t].color = D3DRGBA(1.0f,1.0f,1.0f,fade);

				//-- Render
				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
			}
		}
	}
	//--------------------------------------------------------------------------
	//--Restore Render State
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );


	return FALSE;
}

