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

CFxElectricParticle::CFxElectricParticle()
{
	electric = NULL;
	Create();
}

CFxElectricParticle::~CFxElectricParticle()
{
	SAFE_DELETE(electric);
}

void CFxElectricParticle::Create()
{
	SAFE_DELETE(electric);

	electric = new CRectPlane;
	if( electric )
		electric->Create( 1.0f, 1.0f, FALSE );
}


void CFxElectricParticle::Update_Particle(EffectSort &ef_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( ef_sort.vPos.x, ef_sort.vPos.y, ef_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;

	float dist = DistVecToVec( ef_sort.vTargetPos, ef_sort.vPos );

	for( int i = 0; i < MAX_ELCTRIC_PARTICLE; i++ )
	{

		if( ef_sort.vTargetPos.y <= deco_particle[i].cur_Pos.y - deco_particle[i].scale) // Ÿ�� ��ǥ�� ���� ���ٸ� �ٽ� ���� ���ش�.
			deco_particle[i].start_time = 0;

		if(deco_particle[i].start_time == 0 )
		{
			// �������� ������ ���̸� ��������
			deco_particle[i].scale = dist * RandomNum(0.7f, 0.9f);

			// ������ �� �ؿ��� ������ ũ�⿡ ���缭 ���� ��ġ�� ����ش�.
			D3DVECTOR vTemp;

			vTemp.x = ef_sort.vPos.x + ((float)(rand()%19)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f));
			vTemp.z = ef_sort.vPos.z + ((float)(rand()%19)+(float)(rand()%10)*0.1f) * ((rand()%2 == 0? -1.0f:1.0f));
			vTemp.y = ef_sort.vPos.y - deco_particle[i].scale;

			//������ ��ġ�� ���� ��ġ�� ����ȭ �����ش�.
			deco_particle[i].cur_Pos = deco_particle[i].first_Pos = vTemp;

			deco_particle[i].life_time = (float)(rand()%3) + 2.0f + ((float)(rand()%10)*0.1f); // �����Ӵ� �̵� �ӵ� �ð����� ������� �ʴ´�.
			deco_particle[i].start_time = 1;
		}
		else // ���� ���������ϴ� ���¶�� ��� �����δ�.
		{
			deco_particle[i].cur_Pos.y += deco_particle[i].life_time;
		}
	}
}

void CFxElectricParticle::LoadRes()
{}
void CFxElectricParticle::DeleteRes()
{}

BOOL CFxElectricParticle::Render(EffectSort &ef_sort)
{
	if( !GET_D3DDEVICE() )
		return FALSE;

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
	GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_ELECTRICPARTICLE ] ));

	Update_Particle(ef_sort);

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) ef_sort.effects;

	if( electric )
	{
		for( int i = 0; i < MAX_ELCTRIC_PARTICLE; i++ ) // max ���ľ� ��...
		{
			if( deco_particle[i].start_time == 1 )
			{
				float fScale = 7.0f + (((float)((rand()%1)+2)) * ((rand()%2 == 0? -1.0f:1.0f)));
				electric->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				electric->SetBillBoard(vViewVec, TRUE, 0.0f);
				electric->Scale( fScale, deco_particle[i].scale, 0.0f );
				electric->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				electric->Render();

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


