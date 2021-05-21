#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "DecoObj.h"
#include "ImageDefine.h"

typedef struct Deco_Particle Deco_Particle;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxFireParticle::CFxFireParticle()
{
	fire1				= NULL;
	fire2				= NULL;
	fire3				= NULL;
	fire4				= NULL;
	fire5				= NULL;
	fire6				= NULL;
	fire7				= NULL;
	fire8				= NULL;
	fire9				= NULL;
	fire10				= NULL;

	Create();
}

CFxFireParticle::~CFxFireParticle()
{
	SAFE_DELETE(fire1);
	SAFE_DELETE(fire2);
	SAFE_DELETE(fire3);
	SAFE_DELETE(fire4);
	SAFE_DELETE(fire5);
	SAFE_DELETE(fire6);
	SAFE_DELETE(fire7);
	SAFE_DELETE(fire8);
	SAFE_DELETE(fire9);
	SAFE_DELETE(fire10);
}


void CFxFireParticle::Create()
{
	SAFE_DELETE(fire1);
	SAFE_DELETE(fire2);
	SAFE_DELETE(fire3);
	SAFE_DELETE(fire4);
	SAFE_DELETE(fire5);
	SAFE_DELETE(fire6);
	SAFE_DELETE(fire7);
	SAFE_DELETE(fire8);
	SAFE_DELETE(fire9);
	SAFE_DELETE(fire10);

	fire1 = new CRectPlane;
	if( fire1 )
		fire1->Create( 3.0f, 3.0f, FALSE );

	fire2 = new CRectPlane;
	if( fire2 )
		fire2->Create( 3.0f, 3.0f, FALSE );

	fire3 = new CRectPlane;
	if( fire3 )
		fire3->Create( 3.0f, 3.0f, FALSE );

	fire4 = new CRectPlane;
	if( fire4 )
		fire4->Create( 3.0f, 3.0f, FALSE );

	fire5 = new CRectPlane;
	if( fire5 )
		fire5->Create( 3.0f, 3.0f, FALSE );

	fire6 = new CRectPlane;
	if( fire6 )
		fire6->Create( 3.0f, 3.0f, FALSE );

	fire7 = new CRectPlane;
	if( fire7 )
		fire7->Create( 3.0f, 3.0f, FALSE );

	fire8 = new CRectPlane;
	if( fire8 )
		fire8->Create( 3.0f, 3.0f, FALSE );

	fire9 = new CRectPlane;
	if( fire9 )
		fire9->Create( 3.0f, 3.0f, FALSE );

	fire10 = new CRectPlane;
	if( fire10 )
		fire10->Create( 3.0f, 3.0f, FALSE );
}



void CFxFireParticle::Update_Fire_Particle(EffectSort &effect_sort)
{
	D3DVECTOR	vDrawPos;
	vDrawPos = D3DVECTOR( effect_sort.vPos.x, effect_sort.vPos.y, effect_sort.vPos.z );

	Deco_Particle*	deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ )
	{
		switch( i )
		{
		case 0:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 6 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 1:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 7 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 2:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 8 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 3:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 10 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 4:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 8 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 5:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 2 )
			{
				if( ++(deco_particle[i].texIndex) >= 10 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 6:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 6 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 7:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 7 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 8:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 8 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		case 9:
			deco_particle[i].texFrame += effect_sort.nCurFrame;

			if( deco_particle[i].texFrame > 1 )
			{
				if( ++(deco_particle[i].texIndex) >= 6 )
				{
					deco_particle[i].cur_time = 0;
					deco_particle[i].texIndex = 0;
				}

				deco_particle[i].texFrame = 0;
			}
			break;
		default:
			break;
		}
	}

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ )
	{
		deco_particle[i].cur_time -= effect_sort.nCurFrame;
		if( deco_particle[i].cur_time < 0 ) deco_particle[i].cur_time = 0;

		if( deco_particle[i].cur_time == 0 )
		{
			switch ( effect_sort.Index )
			{
			default:
			case 1:	// 기존 불 효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 1.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 1.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.25f, 0.27f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 1.5f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.1f, 0.12f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;

			case 10: // 기존의 2배 불효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 2.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 2.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.5f, 0.54f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 3.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.2f, 0.24f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;

			case 15: // 기존의 3배 불효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 3.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 3.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.5f, 0.54f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 4.5f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.2f, 0.24f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;

			case 20: // 기존의 4배 불효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 4.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 4.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(1.0f, 1.1f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 6.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.4f, 0.48f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;

			case 25: // 기존의 6배 불효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 6.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 6.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(1.0f, 1.1f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 8.5f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.4f, 0.48f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;

			case 30: // 기존의 8배 불효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 8.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 8.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(1.0f, 1.1f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 10.5f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.4f, 0.48f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;

			case 35: // 기존의 10배 불효과
				switch( i )
				{
				case 6:
				case 7:
				case 8:
				case 9:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 10.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, // + RandomNum(-1.0f, 1.0f),
															vDrawPos.y + 10.0f,
															vDrawPos.z ); // + RandomNum(-1.0f, 1.0f) );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(1.0f, 1.1f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;

				default:
					deco_particle[i].life_time = RandomNum( 100, 100 );
					deco_particle[i].cur_time = deco_particle[i].life_time;
					deco_particle[i].scale = 14.0f;
					deco_particle[i].fade = 1.0f;
					deco_particle[i].red = 1.0f;
					deco_particle[i].green = 1.0f;
					deco_particle[i].blue = 1.0f;
					deco_particle[i].first_Pos = D3DVECTOR( vDrawPos.x, vDrawPos.y, vDrawPos.z );
					deco_particle[i].cur_Pos = D3DVECTOR( deco_particle[i].first_Pos.x, deco_particle[i].first_Pos.y, deco_particle[i].first_Pos.z );
					deco_particle[i].direction = D3DVECTOR( 0.0f, RandomNum(0.4f, 0.48f), 0.0f );
					deco_particle[i].gravity = D3DVECTOR( 0.0f, 0.0f, 0.0f );
					break;
				}
				break;
			}
		}
		else
		{
			if( effect_sort.nCurFrame != 0 )
				deco_particle[i].cur_Pos += ( (deco_particle[i].direction * effect_sort.nCurFrame )
											  + (deco_particle[i].gravity * effect_sort.nCurFrame) );
		}
	}

	// 사그러지는 불꽃 구현 ///플레임스피어 때 추가.
	if( effect_sort.num == REDFIRE_ONETIME )
	{
		if( effect_sort.dwColor == 0 ) // 분모로 사용하니까 0이면 튕긴다.
			effect_sort.dwColor = 1; // 최소값으로 셋팅.

		int ScalePhase = (int)effect_sort.subNum / effect_sort.dwColor; // 남은 라이프(dwColor를 진행된 시간, subNum 처음 할당된 라이프로  사용)에 비례하여 스케일을 조정. (생성시에 크게 커졌다가 사그러지면서 사라지도록)

		for( int i = 0; i < MAX_FIRE_PARTICLE; i++ )
		{
			if( ScalePhase > 3 ) // 진행도가 1/3 이 안되었을때까진.
			{
				// 스케일을 키운다.
				deco_particle[i].scale += effect_sort.vSortPos.x*effect_sort.nCurFrame; // vSortPos.x를 커지는 속도로 사용.
			}
			else // 그 이상이면 줄인다.
			{
				deco_particle[i].scale -= effect_sort.vSortPos.y*effect_sort.nCurFrame; // vSortPos.y를 꺼지는 속도로 사용.
			}
		}
	}
}

void CFxFireParticle::LoadRes(IndexedTexture *pIndexedTexture)
{}
void CFxFireParticle::DeleteRes()
{}

BOOL CFxFireParticle::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

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

	Deco_Particle* deco_particle;
	deco_particle = (Deco_Particle*) effect_sort.effects;

//	if( !deco_particle )
//		return TRUE;

	Update_Fire_Particle(effect_sort);

	D3DVECTOR	vViewVec;
	vViewVec = pCMyApp->GetEyeDirection();

	for( int i = 0; i < MAX_FIRE_PARTICLE; i++ ) // max 고쳐야 됨...
	{
		switch( i )
		{
		case 0:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE1 + deco_particle[i].texIndex ]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE1 + deco_particle[i].texIndex]));
			}

			if( fire1 )
			{
				fire1->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire1->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire1->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire1->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire1->Render();
			}
			break;
		case 1:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE2 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE2 + deco_particle[i].texIndex]));
			}

			if( fire2 )
			{
				fire2->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire2->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire2->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire2->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire2->Render();
			}
			break;
		case 2:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE3 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE3 + deco_particle[i].texIndex]));
			}

			if( fire3 )
			{
				fire3->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire3->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire3->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire3->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire3->Render();
			}
			break;
		case 3:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE4 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE4 + deco_particle[i].texIndex]));
			}

			if( fire4 )
			{
				fire4->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire4->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire4->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire4->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire4->Render();
			}
			break;
		case 4:
			if(g_pCapsyongTexture)
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE5 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE5 + deco_particle[i].texIndex]));
			}

			if( fire5 )
			{
				fire5->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire5->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire5->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire5->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire5->Render();
			}
			break;
		case 5:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE6 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE6 + deco_particle[i].texIndex]));
			}

			if( fire6 )
			{
				fire6->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire6->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire6->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire6->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire6->Render();
			}
			break;
		case 6:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE1 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE1 + deco_particle[i].texIndex]));
			}

			if( fire7 )
			{
				fire7->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire7->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire7->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire7->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire7->Render();
			}
			break;
		case 7:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE2 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE2 + deco_particle[i].texIndex]));
			}

			if( fire8 )
			{
				fire8->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire8->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire8->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire8->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire8->Render();
			}
			break;
		case 8:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE5 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE5 + deco_particle[i].texIndex]));
			}

			if( fire9 )
			{
				fire9->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire9->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire9->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire9->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire9->Render();
			}
			break;
		case 9:
			if( g_pCapsyongTexture )
			{
				if( effect_sort.num == REDFIRE || effect_sort.num == REDFIRE_ONETIME ) ///플레임스피어 때 추가.
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREPARTICLE1 + deco_particle[i].texIndex]));
				else if( effect_sort.num == BLUEFIRE )
					// nate 2004 - 7 - image
					GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_BFIREPARTICLE1 + deco_particle[i].texIndex]));
			}

			if( fire10 )
			{
				fire10->SetColor(deco_particle[i].red, deco_particle[i].green, deco_particle[i].blue, deco_particle[i].fade);
				fire10->SetBillBoard(vViewVec, FALSE, 0.0f);
				fire10->Scale(deco_particle[i].scale, deco_particle[i].scale, 0.0f );
				fire10->SetTranslationXYZ(deco_particle[i].cur_Pos.x, deco_particle[i].cur_Pos.y, deco_particle[i].cur_Pos.z);
				fire10->Render();
			}
			break;
		default:
			break;
		}
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );

	// 사그러지는 불꽃 구현 ///플레임스피어 때 추가.
	if( effect_sort.num == REDFIRE_ONETIME )
	{
		// 남은 라이프(dwColor를 진행된 시간, subNum 처음 할당된 라이프로  사용)에 비례하여 스케일을 조정. (생성시에 크게 커졌다가 사그러지면서 사라지도록)
		effect_sort.dwColor += effect_sort.nCurFrame; // 진행된 프레임을 누적. (밑의 effect_sort.nCurFrame 초기화 이전에 해야함에 유의.)

		if( effect_sort.dwColor > effect_sort.subNum ) // 생명이 다했다면...
		{
			return TRUE; // 지우라고 보낸다.
		}
	}

	effect_sort.nCurFrame = 0;

	return FALSE;
}
