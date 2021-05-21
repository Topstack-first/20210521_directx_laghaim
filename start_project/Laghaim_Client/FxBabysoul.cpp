#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
//#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "GUtil.h"
#include "assert.h"
#include "stdlib.h" //-- rand
//#include "FxBabySoul.h"

//----------------------------------------------------------------
#define PI 0.017453      //3.1416/180
#define random(num) (rand()%(num))


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxBabySoul::CFxBabySoul()
{
	int i = 0;
	int anglex = 0;
	int angley = 0;
	int anglez = 0;

	//--------------------------------------------------------------------------
	//-- Move Vector 초기화 - Large
	for( angley = 0; angley<360; angley+=72)
	{
		for( anglex = 0 ; anglex<360; anglex+=72)
		{
			for( anglez =0 ; anglez<360; anglez+=72)
			{
				m_flamemove[i].x=float(1.0 * sin(double(double(angley)*PI)) * cos(double(double(anglez)*PI)));
				m_flamemove[i].y=float(1.0 * sin(double(double(anglez)*PI)) * cos(double(double(anglex)*PI)));
				m_flamemove[i].z=float(1.0 * sin(double(double(anglex)*PI)) * cos(double(double(angley)*PI)));
				i++;
				assert(i<BABYSOUL_NUM);
			}
		}
	}

	//--------------------------------------------------------------------------
	//-- Move Vector 초기화 - Small
	i =0;
	for( angley = 0; angley<360; angley+=120)
	{
		for(anglex = 0 ; anglex<360; anglex+=120)
		{
			for(anglez =0 ; anglez<360; anglez+=120)
			{
				m_flamemove_small[i].x=float(0.5 * sin(double(double(angley)*PI)) * cos(double(double(anglez)*PI)));
				m_flamemove_small[i].y=float(0.5 * sin(double(double(anglez)*PI)) * cos(double(double(anglex)*PI)));
				m_flamemove_small[i].z=float(0.5 * sin(double(double(anglex)*PI)) * cos(double(double(angley)*PI)));
				i++;
				assert(i<BABYSOUL_NUM);
			}
		}
	}
	//--------------------------------------------------------------------------
	//-- Move Vector 초기화 - Fountain
	for( i = 0; i<BABYSOUL_NUM_FON; i++)
	{
		int algle = random(360);
		m_flamemove_Fountain[i].x = float(random(45)+5)/10.0f  * cos(double(double(algle)*PI));
		m_flamemove_Fountain[i].z = float(random(45)+5)/10.0f  * sin(double(double(algle)*PI));
		m_flamemove_Fountain[i].y = float(random(8)+10) / 3.0f + 1.0f ;
		m_startframe[i] = random(20);
	}

}

CFxBabySoul::~CFxBabySoul()
{}

BOOL CFxBabySoul::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	//-- small 호출
//	if(effect_sort.num ==0)
//	{
//		Render_Small(GET_D3DDEVICE(),effect_sort);
//		return FALSE;
//	}

	//-- Render_Screw
	if(effect_sort.num == 0)
	{
		Render_Screw(effect_sort);
		return TRUE;
	}

	return TRUE;

	//-- Fountain
//	if(effect_sort.num ==3)
//	{
//		Render_Fountain(GET_D3DDEVICE(),effect_sort);
//		return FALSE;
//	}
	/*
		LPDIRECTDRAWSURFACE7 lpSurface = NULL;
		D3DVECTOR vViewVec;
		D3DMATRIX matRx, matRy, matWorld;

		vViewVec = pCMyApp->GetEyeDirection();

		int	i = 0;
		float ry = atan2f(vViewVec.x, vViewVec.z);
		float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

		//-- 빌보드를 위한준비.
		D3DUtil_SetRotateXMatrix(matRx, rx);
		D3DUtil_SetRotateYMatrix(matRy, ry);
		D3DMath_MatrixMultiply(matWorld, matRx, matRy);

		DWORD  dwstate[5];
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwstate[0] );
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwstate[1] );
		GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwstate[2] );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

		DWORD  dwTextureState[3];
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
		GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwTextureState[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		int nFrame = effect_sort.nCurFrame;		//-- 현재 Frame
		D3DVECTOR vPosFrom;                   // 방향벡터를 구한다.

		//-- 시전 객체가 움직일 수도 있기 때문에.
		vPosFrom = effect_sort.vPos;
		vPosFrom.y += 25.0f;

		//-- 텍스처 택하기.
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ;
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float fire_fade = 0.3f;
		float fade_color = 0.0;
		int random_co;
		float random_color[3];


		//--------------------------------------------------------------------------
		//-- 폭파 효과.
		//--------------------------------------------------------------------------
		else
		{
			//-- 모든 Flame 에 관하여
			for(i=1;i<BABYSOUL_NUM;i++)
			{
				//-- 3장의 효과 - -125 * 3 이다.
				for(int k=0;k<3;k++)
				{
					//-- 꼬리 프레임 설정.
					int virframe = nFrame -5 - k;

					//-- 좌표 초기화
					m_position.x = 0.0f;
					m_position.y = 42.0f;
					m_position.z = 0.0f;

					//-- 현재 프레임 만큼 에니메이션 진행
					for(int j=0;j<virframe;j++)
					{
						m_position.x += m_flamemove[i].x;
						m_position.y += m_flamemove[i].y - (0.03f*virframe); //-- 불꽃 흘러내림
						m_position.z += m_flamemove[i].z;

						//-- 폭파일 경우 빨리 퍼짐.
						if(j<5)
						{
							m_position.x += 4*m_flamemove[i].x;
							m_position.y += 4*m_flamemove[i].y;
							m_position.z += 4*m_flamemove[i].z;
						}
					}

					//-- 각 Flame 별로 이동.
					matWorld._41 = vPosFrom.x + m_position.x;
					matWorld._42 = vPosFrom.y + m_position.y;
					matWorld._43 = vPosFrom.z + m_position.z;

					//-- 빌보드 생성.
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
					MakeLVertexRect(m_flame, 7.0f, 7.0f, TRUE);

					//-- Texture Animation 지정.
					m_flame[0].tu = (virframe%4) * 0.25f;
					m_flame[1].tu = (virframe%4) * 0.25f;
					m_flame[2].tu = ((virframe%4)+1) * 0.25f;
					m_flame[3].tu = ((virframe%4)+1) * 0.25f;

					//-- Color 지정
					random_co = random(8);
					switch(random_co)
					{
						case 0:random_color[0] = 1.0f;random_color[1] = 0.0f;random_color[2] = 0.0f;break;
						case 1:random_color[0] = 1.0f;random_color[1] = 1.0f;random_color[2] = 0.0f;break;
						case 6:random_color[0] = 1.0f;random_color[1] = 1.0f;random_color[2] = 0.0f;break;
						case 2:random_color[0] = 0.0f;random_color[1] = 1.0f;random_color[2] = 0.0f;break;
						case 3:random_color[0] = 0.0f;random_color[1] = 1.0f;random_color[2] = 1.0f;break;
						case 4:random_color[0] = 0.0f;random_color[1] = 0.0f;random_color[2] = 1.0f;break;
						case 5:random_color[0] = 1.0f;random_color[1] = 0.0f;random_color[2] = 1.0f;break;
						default :random_color[0] = 1.0f;random_color[1] = 1.0f;random_color[2] = 1.0f;break;
					}
					//-- fade_color
					fire_fade = 1.0f;
					if(virframe  >FX_FIREWORKS_FRAMECOUNT -25)
						fire_fade = float(FX_FIREWORKS_FRAMECOUNT - virframe)/25.0f;

					//-- Color 지정
					for(int t=0;t<4;t++)
						m_flame[t].color = D3DRGBA(random_color[0],random_color[1],random_color[2],fire_fade);//fade_color, 0x00 ,fade_color);
					//-- Render
					GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
				}
			}
		}
		//-- Render State 복귀. 가슴아프다. 이렇게 남발해야 되다니.
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwstate[0] );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwstate[1] );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwstate[2] );

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwTextureState[2] );

		return FALSE;
		*/
}
//----------------------------------------------------------------------------
//-- Render Screw
//----------------------------------------------------------------------------
BOOL CFxBabySoul::Render_Screw(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	vViewVec = pCMyApp->GetEyeDirection();

	int	i = 0,j = 0;
	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	//-- 빌보드를 위한준비.
	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);

	DWORD  dwstate[3];
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &dwstate[0] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &dwstate[1] );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &dwstate[2] );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE);

	DWORD  dwTextureState[3];
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwTextureState[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	int nFrame = effect_sort.nCurFrame;		//-- 현재 Frame
	D3DVECTOR vPosFrom;                   // 방향벡터를 구한다.

	//-- 시전 객체가 움직일 수도 있기 때문에.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 25.0f;

	//-- 텍스처 택하기.
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ; // 같이 쓴다.

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float fire_fade = 0.3f;
	float fade_color = 0.0;
	int random_co;
	float random_color[3];

	{
		//-- 4가지 Color
		for(i=0; i<6; i++)
		{
			//-- 10 장의 효과
			for(int j=0; j<14; j++)
			{
				//-- 발사를 제외한 폭파 효과의 프레임 설정.
				int virframe = nFrame -2;

				//-- 좌표 초기화
				m_position.x = 0.0f;
				m_position.y = 0.0f;
				m_position.z = 0.0f;

				//-- 꼬리
				//	if(j <= virframe)
				{
					//-- 현재 프레임 만큼 에니메이션 진행
					float tail = j/2.0f; //-- 얼마나 전의 프레임을 꼬리로 붙이냐
					float angle = float(virframe/1.0f - tail + float(i*4))*(3.1416/8);

					m_position.x = 7.0f * sin(-angle) ;
					m_position.y += m_flamemove[i].y + ((0.3f*(virframe-tail) + 1.5f) * (virframe-tail) ); //-- 위로
					m_position.z = 7.0f * cos(-angle) ;


					//-- 각 Flame 별로 이동.
					matWorld._41 = vPosFrom.x + m_position.x;
					matWorld._42 = vPosFrom.y + m_position.y;
					matWorld._43 = vPosFrom.z + m_position.z;

					//-- 빌보드 생성.
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
					MakeLVertexRect(m_flame, 14.0f, 14.0f, TRUE);

					//-- Texture Animation 지정.
					m_flame[0].tu = ((virframe*(j+4)*i)%4) * 0.25f;
					m_flame[1].tu = ((virframe*(j+4)*i)%4) * 0.25f;
					m_flame[2].tu = (((virframe*(j+4)*i)%4)+1) * 0.25f;
					m_flame[3].tu = (((virframe*(j+4)*i)%4)+1) * 0.25f;

					//-- Color 지정
					//-- Color 지정
					switch(i)
					{
					case 0:
						random_color[0] = 1.0f;
						random_color[1] = 0.4f;
						random_color[2] = 0.4f;
						break;
					case 1:
						random_color[0] = 1.0f;
						random_color[1] = 1.0f;
						random_color[2] = 0.4f;
						break;
					case 2:
						random_color[0] = 0.4f;
						random_color[1] = 1.0f;
						random_color[2] = 0.4f;
						break;
					case 3:
						random_color[0] = 0.4f;
						random_color[1] = 0.4f;
						random_color[2] = 1.0f;
						break;
					case 4:
						random_color[0] = 0.4f;
						random_color[1] = 1.0f;
						random_color[2] = 1.0f;
						break;
					case 5:
						random_color[0] = 1.0f;
						random_color[1] = 0.4f;
						random_color[2] = 1.0f;
						break;
					default :
						random_color[0] = 1.0f;
						random_color[1] = 1.0f;
						random_color[2] = 1.0f;
						break;
					}

					//-- 랜덤흰색
					random_co = random(12);
					switch(random_co )
					{
					case 0:
						random_color[0] = 1.0f;
						random_color[1] = 0.4f;
						random_color[2] = 0.4f;
						break;
					case 1:
						random_color[0] = 1.0f;
						random_color[1] = 1.0f;
						random_color[2] = 0.4f;
						break;
					case 2:
						random_color[0] = 0.4f;
						random_color[1] = 1.0f;
						random_color[2] = 0.4f;
						break;
					case 3:
						random_color[0] = 0.4f;
						random_color[1] = 0.4f;
						random_color[2] = 1.0f;
						break;
					case 4:
						random_color[0] = 1.0f;
						random_color[1] = 1.0f;
						random_color[2] = 1.0f;
						break;
					default :
						break;
					}
					//-- fade_color
					fire_fade = (20.0f - j) / 20.0f;

					if(virframe  >FX_BABYSOUL_FRAMECOUNT - 25)
						fire_fade = float(FX_BABYSOUL_FRAMECOUNT - virframe) / 25.0f;

					//-- Color 지정
					for(int t=0; t<4; t++)
						m_flame[t].color = D3DRGBA(random_color[0],random_color[1],random_color[2],fire_fade);//fade_color, 0x00 ,fade_color);
					//-- Render
					GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
				}
			}
		}
	}
	//-- Render State 복귀. 가슴아프다. 이렇게 남발해야 되다니.
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwstate[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwstate[1] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwstate[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwTextureState[2] );

	return FALSE;
}

