#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Gutil.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



#define FX_VAMPIREATTACK_MAXBALL	2
#define FX_VAMPIREATTACK_MAXTAILPOS	9
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxVampireAttack::CFxVampireAttack()
{
	m_VampireBall			= NULL;
	m_ptailRGB				= NULL;

	Create();
}

CFxVampireAttack::~CFxVampireAttack()
{
	SAFE_DELETE_ARRAY(m_VampireBall);
	SAFE_DELETE_ARRAY(m_ptailRGB);
}

void CFxVampireAttack::Create()
{
	SAFE_DELETE_ARRAY(m_VampireBall);
	SAFE_DELETE_ARRAY(m_ptailRGB);

	m_VampireBall = new CTailPlane[ FX_VAMPIREATTACK_MAXBALL ];
	m_VampireBall[0].Create( FX_VAMPIREATTACK_MAXTAILPOS-1, 3.0f, 2.0f, 1.0f, FALSE );
	m_VampireBall[1].Create( FX_VAMPIREATTACK_MAXTAILPOS-1, 3.0f, 2.0f, 1.0f, FALSE );

	m_ptailRGB = new D3DXCOLOR[ FX_VAMPIREATTACK_MAXBALL ];
	m_ptailRGB[0] = D3DXCOLOR( 1.0f, 0.35f, 0.35f, 1.0f );
	m_ptailRGB[1] = D3DXCOLOR( 1.0f, 0.35f, 0.35f, 1.0f );
}

void CFxVampireAttack::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxVampireAttack::DeleteRes()
{}

BOOL CFxVampireAttack::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo
			&& !effect_sort.pMobTo)
		return TRUE;

	Tail_Info *tail_Info = (Tail_Info* )effect_sort.effects;

	int num = 0;
	bool end_Frame = FALSE;

	if( tail_Info )
		end_Frame = tail_Info[ FX_VAMPIREATTACK_MAXBALL - 1 ].end_Tail;

	if(effect_sort.nCurFrame >= FX_VAMPIREATTACK_FRAMECOUNT ||
			(end_Frame && !(tail_Info[ FX_VAMPIREATTACK_MAXBALL - 1 ].curTailPosNum)) )
	{

		if( tail_Info )
		{
			for(num = 0; num < FX_VAMPIREATTACK_MAXBALL ; num++)
				SAFE_DELETE_ARRAY(tail_Info[num].pTailPos);

			delete [] tail_Info;
			tail_Info = NULL;
			effect_sort.effects = tail_Info;
		}
		return TRUE;
	}

	if( tail_Info == NULL )
	{
		tail_Info = new Tail_Info[ FX_VAMPIREATTACK_MAXBALL ];
		ZeroMemory(tail_Info, sizeof(Tail_Info) * FX_VAMPIREATTACK_MAXBALL );
		for(num = 0; num < FX_VAMPIREATTACK_MAXBALL ; num++)
		{
			tail_Info[num].pTailPos = new D3DVECTOR[ FX_VAMPIREATTACK_MAXTAILPOS ];
			ZeroMemory(tail_Info[num].pTailPos, sizeof(D3DVECTOR) * FX_VAMPIREATTACK_MAXTAILPOS );
			tail_Info[num].curTailPosNum = 0;
			tail_Info[num].end_Tail = FALSE;
		}

		effect_sort.effects = tail_Info;
	}

	float maxScale = 3.0f;
	float m_fScale = (maxScale * effect_sort.nCurFrame) / FX_VAMPIREATTACK_FRAMECOUNT;
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
	if( effect_sort.pMobTo )
	{
		vChaVec = D3DVECTOR(effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy + 12.0f, effect_sort.pMobTo->m_wz);
		vMobVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y + 10.0f, effect_sort.vPos.z);
	}
	else if( effect_sort.pNkChaTo )
	{
		vChaVec = D3DVECTOR(effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy + 12.0f, effect_sort.pNkChaTo->m_wz);
		vMobVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y + 10.0f, effect_sort.vPos.z);
	}
	else
	{
		vChaVec = D3DVECTOR(effect_sort.vTargetPos.x, effect_sort.vTargetPos.y +12.0f, effect_sort.vTargetPos.z );
		vMobVec = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y + 10.0f, effect_sort.vPos.z);
	}

	D3DVECTOR	vDrawPos;
	vDrawPos = vMobVec;

	// nate 2004 - 7 - image
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_VAMPIREATTACK_BALL ] ) );
	for(num = 0; num < FX_VAMPIREATTACK_MAXBALL ; num++)
	{
		if( tail_Info[num].end_Tail )
			continue;

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		switch( num )
		{
		case 0:
			if( effect_sort.nCurFrame > 16 )
			{
				tail_Info[num].end_Tail = TRUE;

				ef_sort.nType = FX_HITPETEFFECTS;
				ef_sort.vTargetPos = vChaVec;
				ef_sort.vPos = vMobVec;

				if( effect_sort.pMobTo )
					effect_sort.pMobTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				else if( effect_sort.pNkChaTo )
					effect_sort.pNkChaTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}
			break;
		case 1:
			if( effect_sort.nCurFrame > 21 )
			{
				tail_Info[num].end_Tail = TRUE;

				ef_sort.nType = FX_HITPETEFFECTS;
				ef_sort.vTargetPos = vChaVec;
				ef_sort.vPos = vMobVec;

				if( effect_sort.pMobTo )
					effect_sort.pMobTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
				else if( effect_sort.pNkChaTo )
					effect_sort.pNkChaTo->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
			}
			break;
		}
	}

	for( num = 0; num < FX_VAMPIREATTACK_MAXBALL ; num++ )
	{
		if( effect_sort.num )
		{
			if( tail_Info[num].end_Tail )
			{
				for( int add = effect_sort.num; add > 0 ; add-- )
				{
					m_VampireBall[num].SubTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec, m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
				}
			}
			else
			{
				for( int add = effect_sort.num; add > 0 ; add-- )
				{
					D3DVECTOR	vStartPos, vEndPos, vControlPos1, vControlPos2, vDirControl, vMagicSwordPos;
					float		HuntTime = 0; //(float)(effect_sort.nCurFrame - add) / MagicSword_SPEED;

					vStartPos = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					vEndPos =  vChaVec - vMobVec;

					D3DMATRIX	matRy;
					D3DVECTOR	vDirControl_Rotate = vEndPos;

					if( num == 0 )
					{
						HuntTime = (float)(effect_sort.nCurFrame - add) / 15.0f;

						if( HuntTime > 1.0f )
							HuntTime = 1.0f;

						vDirControl = vEndPos * 0.15f;
						vControlPos1 = D3DVECTOR( vDirControl.x, 10.0f, vDirControl.z );
						vDirControl = vEndPos * 0.25f;
						vControlPos2 = D3DVECTOR( vDirControl.x, 10.0f, vDirControl.z );
						vMagicSwordPos = PointOnCurve(vStartPos, vControlPos1, vControlPos2, vEndPos, HuntTime);
					}
					else if( num == 1 )
					{
						HuntTime = (float)(effect_sort.nCurFrame - add) / 20.0f;

						if( HuntTime > 1.0f )
							HuntTime = 1.0f;

						D3DUtil_SetRotateYMatrix(matRy, g_PI * 0.15f);
						D3DMath_VectorMatrixMultiply( vDirControl_Rotate, vDirControl_Rotate, matRy );

						vDirControl = vDirControl_Rotate * 0.05f;
						vControlPos1 = D3DVECTOR( vDirControl.x, 10.0f, vDirControl.z );
						vDirControl = vDirControl_Rotate * 0.15f;
						vControlPos2 = D3DVECTOR( vDirControl.x, 10.0f, vDirControl.z );
						vMagicSwordPos = PointOnCurve(vStartPos, vControlPos1, vControlPos2, vEndPos, HuntTime);
					}
					m_VampireBall[num].AddTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vMagicSwordPos, vViewVec, m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
				}
			}
		}
		else
		{
			if( tail_Info[num].end_Tail )
				m_VampireBall[num].ReSetSubTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec,  m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
			else
				m_VampireBall[num].ReSetAddTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec,  m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);

		}

		m_VampireBall[num].SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		m_VampireBall[num].Render(&(tail_Info[num].curTailPosNum));
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	effect_sort.num = 0;

	return FALSE;
}






