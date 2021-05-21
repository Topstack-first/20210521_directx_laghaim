#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Gutil.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



#define FX_VAMPIRE_MAX_NUM		3
#define FX_VAMPIRE_NAX_TAILPOS	16
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxVampire::CFxVampire()
{
	m_vampire				= NULL;
	m_ptailRGB				= NULL;

	Create();
}

CFxVampire::~CFxVampire()
{
	SAFE_DELETE_ARRAY(m_ptailRGB);
	SAFE_DELETE_ARRAY(m_vampire);
}

void CFxVampire::Create()
{
	SAFE_DELETE_ARRAY(m_ptailRGB);
	SAFE_DELETE_ARRAY(m_vampire);

	m_vampire = new CTailPlane[ FX_VAMPIRE_MAX_NUM ];
	m_vampire[0].Create( FX_VAMPIRE_NAX_TAILPOS-1, 2.0f, 1.5f, 1.0f );
	m_vampire[1].Create( FX_VAMPIRE_NAX_TAILPOS-1, 2.5f, 2.0f, 1.0f );
	m_vampire[2].Create( FX_VAMPIRE_NAX_TAILPOS-1, 3.0f, 2.5f, 1.0f );

	m_ptailRGB = new D3DXCOLOR[ FX_VAMPIRE_MAX_NUM ];
	m_ptailRGB[0] = D3DXCOLOR( 1.0f, 0.5f, 0.5f, 1.0f );
	m_ptailRGB[1] = D3DXCOLOR( 1.0f, 0.25f, 0.25f, 1.0f );
	m_ptailRGB[2] = D3DXCOLOR( 1.0f, 0.75f, 0.75f, 1.0f );
}

void CFxVampire::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxVampire::DeleteRes()
{}

BOOL CFxVampire::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if( !effect_sort.pNkChaTo )
		return TRUE;

	Tail_Info *tail_Info = (Tail_Info* )effect_sort.effects;

	effect_sort.Index = FX_VAMPIRE_MAX_NUM;

	int num = 0;
	bool end_Frame = FALSE;

	if( tail_Info )
		end_Frame = tail_Info[ FX_VAMPIRE_MAX_NUM - 1 ].end_Tail;

	if(effect_sort.nCurFrame >= FX_VAMPIRE_FRAMECOUNT ||
			(end_Frame && !(tail_Info[ FX_VAMPIRE_MAX_NUM - 1 ].curTailPosNum)) )
	{

		if( tail_Info )
		{
			for(num = 0; num < FX_VAMPIRE_MAX_NUM ; num++)
				SAFE_DELETE_ARRAY(tail_Info[num].pTailPos);

			delete [] tail_Info;
			tail_Info = NULL;
			effect_sort.effects = tail_Info;
		}
		return TRUE;
	}

	if( tail_Info == NULL )
	{
		tail_Info = new Tail_Info[ FX_VAMPIRE_MAX_NUM ];
		ZeroMemory(tail_Info, sizeof(Tail_Info) * FX_VAMPIRE_MAX_NUM );
		for(num = 0; num < FX_VAMPIRE_MAX_NUM ; num++)
		{
			tail_Info[num].pTailPos = new D3DVECTOR[ FX_VAMPIRE_NAX_TAILPOS ];
			ZeroMemory(tail_Info[num].pTailPos, sizeof(D3DVECTOR) * FX_VAMPIRE_NAX_TAILPOS );
			tail_Info[num].curTailPosNum = 0;
			tail_Info[num].end_Tail = FALSE;
		}

		effect_sort.effects = tail_Info;
	}

	float maxScale = 3.0f;
	float m_fScale = (maxScale * effect_sort.nCurFrame) / FX_VAMPIRE_FRAMECOUNT;
	float m_fFade = 1.0f - (m_fScale * 0.3f);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	D3DVECTOR	vChaVec, vMobVec;
	if( effect_sort.pNkChaTo )
		vChaVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 12.0f, effect_sort.pNkChaTo->m_wz);
	vMobVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_VAMPIRE ] ) );

	for(num = 0; num < FX_VAMPIRE_MAX_NUM ; num++)
	{

		if( tail_Info[num].end_Tail )
			continue;

		switch( num )
		{
		case 0:
			if( effect_sort.nCurFrame > 36 )
			{
				tail_Info[num].end_Tail = TRUE;
			}
			break;
		case 1:
			if( effect_sort.nCurFrame > 41 )
			{
				tail_Info[num].end_Tail = TRUE;
			}
			break;
		case 2:
			if( effect_sort.nCurFrame > 46 )
			{
				tail_Info[num].end_Tail = TRUE;
			}
			break;
		}
	}

	for( num = 0; num < FX_VAMPIRE_MAX_NUM ; num++ )
	{

		if( effect_sort.num )
		{

			if( tail_Info[num].end_Tail )
			{
				for( int add = effect_sort.num; add > 0 ; add-- )
					m_vampire[num].SubTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec, m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
			}
			else
			{
				for( int add = effect_sort.num; add > 0 ; add-- )
				{

					D3DVECTOR	vStartPos, vEndPos, vControlPos1, vControlPos2, vDirControl, vGhostHuntPos;
					float		HuntTime = 0; //(float)(effect_sort.nCurFrame - add) / GHOSTHUNT_SPEED;

					vStartPos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					vEndPos =  vChaVec - vMobVec;

					D3DMATRIX	matRy;
					D3DVECTOR	vDirControl_Rotate = vEndPos;

					if( num == 0 )
					{
						HuntTime = (float)(effect_sort.nCurFrame - add) / 35.0f;

						if( HuntTime > 1.0f )
						{
							HuntTime = 1.0f;
						}

						vDirControl = -vEndPos * 1.2f;
						vControlPos1 = D3DVECTOR( vDirControl.x, 50.0f, vDirControl.z );
						vDirControl = vEndPos * 0.3f;
						vControlPos2 = D3DVECTOR( vDirControl.x, 50.0f, vDirControl.z );
						vGhostHuntPos = PointOnCurve(vStartPos, vControlPos1, vControlPos2, vEndPos, HuntTime);
					}
					else if( num == 1 )
					{
						HuntTime = (float)(effect_sort.nCurFrame - add) / 40.0f;

						if( HuntTime > 1.0f )
						{
							HuntTime = 1.0f;
						}

						D3DUtil_SetRotateYMatrix(matRy, g_PI * 0.25f);
						D3DMath_VectorMatrixMultiply( vDirControl_Rotate, vDirControl_Rotate, matRy );

						vDirControl = -vDirControl_Rotate * 1.2f;
						vControlPos1 = D3DVECTOR( vDirControl.x, 20.0f, vDirControl.z );
						vDirControl = vDirControl_Rotate * 0.5f;
						vControlPos2 = D3DVECTOR( vDirControl.x, 20.0f, vDirControl.z );
						vGhostHuntPos = PointOnCurve(vStartPos, vControlPos1, vControlPos2, vEndPos, HuntTime);
					}
					else if( num == 2 )
					{
						HuntTime = (float)(effect_sort.nCurFrame - add) / 45.0f;

						if( HuntTime > 1.0f )
						{
							HuntTime = 1.0f;
						}

						D3DUtil_SetRotateYMatrix(matRy, g_PI * -0.25f);
						D3DMath_VectorMatrixMultiply( vDirControl_Rotate, vDirControl_Rotate, matRy );

						vDirControl = -vDirControl_Rotate * 1.2f;
						vControlPos1 = D3DVECTOR( vDirControl.x, 25.0f, vDirControl.z );
						vDirControl = vDirControl_Rotate * 0.5f;
						vControlPos2 = D3DVECTOR( vDirControl.x, 25.0f, vDirControl.z );
						vGhostHuntPos = PointOnCurve(vStartPos, vControlPos1, vControlPos2, vEndPos, HuntTime);
					}
					m_vampire[num].AddTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vGhostHuntPos, vViewVec, m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
				}
			}
		}
		else
		{
			if( tail_Info[num].end_Tail )
				m_vampire[num].ReSetSubTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec,  m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
			else
				m_vampire[num].ReSetAddTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec,  m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);

		}

		m_vampire[num].SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		m_vampire[num].Render(&(tail_Info[num].curTailPosNum));
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	effect_sort.num = 0;

	return FALSE;
}






