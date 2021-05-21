#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Gutil.h"
// nate 2004 - 7 - image
#include "ImageDefine.h"



#define MAX_GHOST_RISE_NUM	5
#define MAX_TAIL_POS		26

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxGhostRise::CFxGhostRise()
{
	m_ghostrise	= NULL;
	m_ptailRGB	= NULL;

	Create();
}

CFxGhostRise::~CFxGhostRise()
{
	SAFE_DELETE_ARRAY(m_ghostrise);
	SAFE_DELETE_ARRAY(m_ptailRGB);
}

void CFxGhostRise::Create()
{
	SAFE_DELETE_ARRAY(m_ghostrise);
	SAFE_DELETE_ARRAY(m_ptailRGB);

	m_ghostrise = new CTailPlane[ MAX_GHOST_RISE_NUM ];
	m_ghostrise[0].Create( MAX_TAIL_POS - 1, 2.0f, 1.5f, 1.0f );
	m_ghostrise[1].Create( MAX_TAIL_POS - 1, 3.0f, 2.5f, 1.0f );
	m_ghostrise[2].Create( MAX_TAIL_POS - 1, 2.0f, 1.5f, 1.0f );
	m_ghostrise[3].Create( MAX_TAIL_POS - 1, 3.0f, 2.5f, 1.0f );
	m_ghostrise[4].Create( MAX_TAIL_POS - 1, 3.5f, 3.0f, 1.0f );

	m_ptailRGB = new D3DXCOLOR[ MAX_GHOST_RISE_NUM ];
	m_ptailRGB[0] = D3DXCOLOR( 0.5f, 0.5f, 1.0f, 1.0f );
	m_ptailRGB[1] = D3DXCOLOR( 0.25f, 0.25f, 1.0f, 1.0f );
	m_ptailRGB[2] = D3DXCOLOR( 0.5f, 0.5f, 1.0f, 1.0f );
	m_ptailRGB[3] = D3DXCOLOR( 0.25f, 0.25f, 1.0f, 1.0f );
	m_ptailRGB[4] = D3DXCOLOR( 0.75f, 0.75f, 1.0f, 1.0f );
}

void CFxGhostRise::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxGhostRise::DeleteRes()
{}

BOOL CFxGhostRise::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	Tail_Info *tail_Info = (Tail_Info* )effect_sort.effects;
	effect_sort.Index = MAX_GHOST_RISE_NUM;
	int num = 0;
	bool end_Frame = FALSE;

	if( tail_Info )
		end_Frame = tail_Info[ MAX_GHOST_RISE_NUM - 1 ].end_Tail;

	if(effect_sort.nCurFrame >= FX_GHOSTRISE_FRAMECOUNT ||
			(end_Frame && !(tail_Info[ MAX_GHOST_RISE_NUM - 1 ].curTailPosNum)) )
	{
		if( tail_Info )
		{
			for(num = 0; num < MAX_GHOST_RISE_NUM ; num++)
				SAFE_DELETE_ARRAY(tail_Info[num].pTailPos);

			SAFE_DELETE_ARRAY(tail_Info);
			effect_sort.effects = tail_Info;
		}
		return TRUE;
	}

	if( tail_Info == NULL )
	{
		tail_Info = new Tail_Info[ MAX_GHOST_RISE_NUM ];
		ZeroMemory(tail_Info, sizeof(Tail_Info) * MAX_GHOST_RISE_NUM );
		for(num = 0; num < MAX_GHOST_RISE_NUM ; num++)
		{
			tail_Info[num].pTailPos = new D3DVECTOR[ MAX_TAIL_POS ];
			ZeroMemory(tail_Info[num].pTailPos, sizeof(D3DVECTOR) * MAX_TAIL_POS );
			tail_Info[num].curTailPosNum = 0;
			tail_Info[num].end_Tail = FALSE;
		}

		effect_sort.effects = tail_Info;
	}

	float maxScale = 3.0f;
	float m_fScale = (maxScale * effect_sort.nCurFrame) / FX_GHOSTRISE_FRAMECOUNT;
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

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR(effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z);

	// nate 2004 - 7 - image
	if( g_pCapsyongTexture )
		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_GHOSTRISE ] ) );

	for(num = 0; num < MAX_GHOST_RISE_NUM ; num++)
	{
		if( tail_Info[num].end_Tail )
			continue;

		if( effect_sort.nCurFrame > 135 )
			tail_Info[num].end_Tail = TRUE;
	}

	for( num = 0; num < MAX_GHOST_RISE_NUM ; num++ )
	{
		if( effect_sort.num )
		{
			if( tail_Info[num].end_Tail )
			{
				for( int add = effect_sort.num; add > 0 ; add-- )
					m_ghostrise[num].SubTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec, m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
			}
			else
			{
				for( int add = effect_sort.num; add > 0 ; add-- )
				{
					D3DVECTOR	vGhostRisePos;
					float xpos, ypos, zpos, step;

					if( num == 0 )
					{
						xpos = ypos = zpos = 0.0f;
						step = g_2_PI / 60;

						ypos = (effect_sort.nCurFrame - add) * 1.5f;
						xpos += (effect_sort.nCurFrame - add) * 0.35f * (float)sin(step*(effect_sort.nCurFrame - add));
						zpos += (effect_sort.nCurFrame - add) * 0.35f* (float)cos(step*(effect_sort.nCurFrame - add));
						vGhostRisePos = D3DVECTOR(xpos, ypos, zpos);
					}
					else if( num == 1 )
					{
						ypos = 0.0f;
						xpos = zpos = 5.0f;
						step = g_2_PI / 60;

						ypos = (effect_sort.nCurFrame - add) * 1.5f;
						xpos += (effect_sort.nCurFrame - add) * 0.55f * (float)sin(step*(effect_sort.nCurFrame - add) + (step*12.0f));
						zpos += (effect_sort.nCurFrame - add) * 0.55f* (float)cos(step*(effect_sort.nCurFrame - add) + (step*12.0f));
						vGhostRisePos = D3DVECTOR(xpos, ypos, zpos);
					}
					else if( num == 2 )
					{
						xpos = ypos = zpos = 0.0f;
						step = g_2_PI / 60;

						ypos = (effect_sort.nCurFrame - add) * 1.75f;
						xpos += (effect_sort.nCurFrame - add) * 0.35f * (float)sin(step*(effect_sort.nCurFrame - add) + (step*24.0f));
						zpos += (effect_sort.nCurFrame - add) * 0.35f* (float)cos(step*(effect_sort.nCurFrame - add) + (step*24.0f));
						vGhostRisePos = D3DVECTOR(xpos, ypos, zpos);
					}
					else if( num == 3 )
					{
						ypos = 0.0f;
						xpos = zpos = -5.0f;
						step = g_2_PI / 60;

						ypos = (effect_sort.nCurFrame - add) * 1.75f;
						xpos += (effect_sort.nCurFrame - add) * 0.55f * (float)sin(step*(effect_sort.nCurFrame - add) + (step*36.0f));
						zpos += (effect_sort.nCurFrame - add) * 0.55f* (float)cos(step*(effect_sort.nCurFrame - add) + (step*36.0f));
						vGhostRisePos = D3DVECTOR(xpos, ypos, zpos);
					}
					else if( num == 4 )
					{
						xpos = ypos = zpos = 0.0f;
						step = g_2_PI / 60;

						ypos = (effect_sort.nCurFrame - add) * 1.5f;
						xpos += (effect_sort.nCurFrame - add) * 0.35f * (float)sin(step*(effect_sort.nCurFrame - add) + (step*48.0f));
						zpos += (effect_sort.nCurFrame - add) * 0.35f* (float)cos(step*(effect_sort.nCurFrame - add) + (step*48.0f));
						vGhostRisePos = D3DVECTOR(xpos, ypos, zpos);
					}

					m_ghostrise[num].AddTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vGhostRisePos, vViewVec, m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
				}
			}
		}
		else
		{
			if( tail_Info[num].end_Tail )
				m_ghostrise[num].ReSetSubTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec,  m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);
			else
				m_ghostrise[num].ReSetAddTailPos(tail_Info[num].pTailPos, &(tail_Info[num].curTailPosNum), vViewVec,  m_ptailRGB[num].r, m_ptailRGB[num].g, m_ptailRGB[num].b);

		}

		m_ghostrise[num].SetTranslationXYZ(vDrawPos.x, vDrawPos.y, vDrawPos.z);
		m_ghostrise[num].Render(&(tail_Info[num].curTailPosNum));
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	effect_sort.num = 0;

	return FALSE;
}






