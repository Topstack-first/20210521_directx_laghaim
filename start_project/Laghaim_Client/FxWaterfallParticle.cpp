#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"
#include "ImageDefine.h"
#include "GUtil.h"
#include "NkCharacter.h"


typedef struct Deco_Particle Deco_Particle;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxWaterfallParticle::CFxWaterfallParticle()
{
	m_Plane = NULL;
	Create();
}

CFxWaterfallParticle::~CFxWaterfallParticle()
{
	SAFE_DELETE(m_Plane);
}

void CFxWaterfallParticle::Create()
{
	SAFE_DELETE(m_Plane);

	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 1.0f, 1.0f, FALSE );
}


void CFxWaterfallParticle::Update_Particle(EffectSort &ef_sort)
{

	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( ef_sort.vPos.x, ef_sort.vPos.y, ef_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;

	float dist = DistVecToVec( ef_sort.vTargetPos, ef_sort.vPos ) ;

	for( int i = 0; i<MAX_WATERFALL_PARTICLE; i++ )
	{
		switch( deco_particle[i].start_time )
		{
		case 0:
			{
				// 라인 선상을 중심을 일정 거리 이상 떨어져 시작 하지 않도록 시작 위치를 아래와 같이 잡아준다.
				D3DVECTOR vTemp;
				D3DVECTOR fFirst = ef_sort.vSortPos * ((float)(rand() % (DWORD)dist)+1.0f);

				vTemp = ef_sort.vPos + fFirst;

				// 시작할 위치를 결정한자.
				vTemp.x += ((float)(rand()%6)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f));
				vTemp.z += ((float)(rand()%6)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f));
				//vTemp.y = ef_sort.vPos.y;

				deco_particle[i].cur_Pos = vTemp;

				// 도착 지점은 도착할 지점을 결정 한다.
				vTemp.x = vTemp.x + ((float)(rand()%10)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f));
				vTemp.z = vTemp.z + ((float)(rand()%10)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f));
				vTemp.y = vTemp.y + ((float)(rand()%6)+(float)(rand()%10)*0.1f) + 10.0f;

				deco_particle[i].first_Pos = vTemp;

				// 방향값을 정해준다.
				vTemp = deco_particle[i].first_Pos - deco_particle[i].cur_Pos ;
				deco_particle[i].direction = Normalize(vTemp);

				// 증감(가) 속도를 정해준다.
				deco_particle[i].gravity.x = 0.0f;

				if( i >	MAX_ELCTRIC_PARTICLE / 2 )
					deco_particle[i].scale = 10.0f;
				else
					deco_particle[i].scale = 25.0f;

				deco_particle[i].fade = 0.5f;
				deco_particle[i].start_time = 1;

			}
			break;
		case 1:// 위로 솟구치는 계산
			{
				D3DVECTOR direction = deco_particle[i].direction * ( 0.6f - deco_particle[i].gravity.x);
				deco_particle[i].cur_Pos = deco_particle[i].cur_Pos + direction;
				deco_particle[i].gravity.x += 0.005f;

				if( i >	MAX_ELCTRIC_PARTICLE / 2 )
					deco_particle[i].scale += 1.5f;
				else
					deco_particle[i].scale += 1.5f;


				deco_particle[i].fade -= 0.0051f;
				if(deco_particle[i].fade < 0.0f)
					deco_particle[i].fade = 0.0f;

				if( deco_particle[i].first_Pos.y <  deco_particle[i].cur_Pos.y )
					deco_particle[i].start_time = 0;
			}
			break;
		case 2: // 아래로 떨어지기 위한 셋팅
			{

			}
			break;
		case 3: // 아래로 떨어지는 계산
			{


				if(deco_particle[i].life_time >= timeGetTime() - ef_sort.dwColor  )
					deco_particle[i].start_time = 0;
			}
			break;
		}
	}
}

void CFxWaterfallParticle::LoadRes()
{}

void CFxWaterfallParticle::DeleteRes()
{}

BOOL CFxWaterfallParticle::Render(EffectSort &ef_sort)
{
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);

	// Set any appropiate state
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	// nate 2004 - 7 - image

	Update_Particle(ef_sort);

	m_Plane->SetColor(1.0f, 1.0f, 1.0f, 1.0f);//deco_particle[i].fade);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;

	if( m_Plane )
	{
		for( int i = 0; i < MAX_ELCTRIC_PARTICLE; i++ ) // max 고쳐야 됨...
		{
			if( deco_particle[i].start_time == 1 )
			{
				if( i >	MAX_ELCTRIC_PARTICLE / 2 )
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WATERFALLPARTICLE ] ) );
				else
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_WATERFALLPARTICLE_01 ] ) );

				float fScale = deco_particle[i].scale;// + (((float)((rand()%1)+2)) * ((rand()%2 == 0? -1.0f:1.0f)));


				m_Plane->SetBillBoard(vViewVec, TRUE, 0.0f);
				m_Plane->Scale( fScale, fScale, fScale );
				m_Plane->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				m_Plane->Render();
			}
		}
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	ef_sort.nCurFrame = 0;

	return FALSE;
}


