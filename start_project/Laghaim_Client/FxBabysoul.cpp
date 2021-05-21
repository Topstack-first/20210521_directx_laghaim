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
	//-- Move Vector �ʱ�ȭ - Large
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
	//-- Move Vector �ʱ�ȭ - Small
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
	//-- Move Vector �ʱ�ȭ - Fountain
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

	//-- small ȣ��
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

		//-- �����带 �����غ�.
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

		int nFrame = effect_sort.nCurFrame;		//-- ���� Frame
		D3DVECTOR vPosFrom;                   // ���⺤�͸� ���Ѵ�.

		//-- ���� ��ü�� ������ ���� �ֱ� ������.
		vPosFrom = effect_sort.vPos;
		vPosFrom.y += 25.0f;

		//-- �ؽ�ó ���ϱ�.
		if( g_pCapsyongTexture )
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ;
		GET_D3DDEVICE()->SetTexture(0, lpSurface);

		float fire_fade = 0.3f;
		float fade_color = 0.0;
		int random_co;
		float random_color[3];


		//--------------------------------------------------------------------------
		//-- ���� ȿ��.
		//--------------------------------------------------------------------------
		else
		{
			//-- ��� Flame �� ���Ͽ�
			for(i=1;i<BABYSOUL_NUM;i++)
			{
				//-- 3���� ȿ�� - -125 * 3 �̴�.
				for(int k=0;k<3;k++)
				{
					//-- ���� ������ ����.
					int virframe = nFrame -5 - k;

					//-- ��ǥ �ʱ�ȭ
					m_position.x = 0.0f;
					m_position.y = 42.0f;
					m_position.z = 0.0f;

					//-- ���� ������ ��ŭ ���ϸ��̼� ����
					for(int j=0;j<virframe;j++)
					{
						m_position.x += m_flamemove[i].x;
						m_position.y += m_flamemove[i].y - (0.03f*virframe); //-- �Ҳ� �귯����
						m_position.z += m_flamemove[i].z;

						//-- ������ ��� ���� ����.
						if(j<5)
						{
							m_position.x += 4*m_flamemove[i].x;
							m_position.y += 4*m_flamemove[i].y;
							m_position.z += 4*m_flamemove[i].z;
						}
					}

					//-- �� Flame ���� �̵�.
					matWorld._41 = vPosFrom.x + m_position.x;
					matWorld._42 = vPosFrom.y + m_position.y;
					matWorld._43 = vPosFrom.z + m_position.z;

					//-- ������ ����.
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
					MakeLVertexRect(m_flame, 7.0f, 7.0f, TRUE);

					//-- Texture Animation ����.
					m_flame[0].tu = (virframe%4) * 0.25f;
					m_flame[1].tu = (virframe%4) * 0.25f;
					m_flame[2].tu = ((virframe%4)+1) * 0.25f;
					m_flame[3].tu = ((virframe%4)+1) * 0.25f;

					//-- Color ����
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

					//-- Color ����
					for(int t=0;t<4;t++)
						m_flame[t].color = D3DRGBA(random_color[0],random_color[1],random_color[2],fire_fade);//fade_color, 0x00 ,fade_color);
					//-- Render
					GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
				}
			}
		}
		//-- Render State ����. ����������. �̷��� �����ؾ� �Ǵٴ�.
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

	//-- �����带 �����غ�.
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

	int nFrame = effect_sort.nCurFrame;		//-- ���� Frame
	D3DVECTOR vPosFrom;                   // ���⺤�͸� ���Ѵ�.

	//-- ���� ��ü�� ������ ���� �ֱ� ������.
	vPosFrom = effect_sort.vPos;
	vPosFrom.y += 25.0f;

	//-- �ؽ�ó ���ϱ�.
	if( g_pCapsyongTexture )
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ; // ���� ����.

	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float fire_fade = 0.3f;
	float fade_color = 0.0;
	int random_co;
	float random_color[3];

	{
		//-- 4���� Color
		for(i=0; i<6; i++)
		{
			//-- 10 ���� ȿ��
			for(int j=0; j<14; j++)
			{
				//-- �߻縦 ������ ���� ȿ���� ������ ����.
				int virframe = nFrame -2;

				//-- ��ǥ �ʱ�ȭ
				m_position.x = 0.0f;
				m_position.y = 0.0f;
				m_position.z = 0.0f;

				//-- ����
				//	if(j <= virframe)
				{
					//-- ���� ������ ��ŭ ���ϸ��̼� ����
					float tail = j/2.0f; //-- �󸶳� ���� �������� ������ ���̳�
					float angle = float(virframe/1.0f - tail + float(i*4))*(3.1416/8);

					m_position.x = 7.0f * sin(-angle) ;
					m_position.y += m_flamemove[i].y + ((0.3f*(virframe-tail) + 1.5f) * (virframe-tail) ); //-- ����
					m_position.z = 7.0f * cos(-angle) ;


					//-- �� Flame ���� �̵�.
					matWorld._41 = vPosFrom.x + m_position.x;
					matWorld._42 = vPosFrom.y + m_position.y;
					matWorld._43 = vPosFrom.z + m_position.z;

					//-- ������ ����.
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
					MakeLVertexRect(m_flame, 14.0f, 14.0f, TRUE);

					//-- Texture Animation ����.
					m_flame[0].tu = ((virframe*(j+4)*i)%4) * 0.25f;
					m_flame[1].tu = ((virframe*(j+4)*i)%4) * 0.25f;
					m_flame[2].tu = (((virframe*(j+4)*i)%4)+1) * 0.25f;
					m_flame[3].tu = (((virframe*(j+4)*i)%4)+1) * 0.25f;

					//-- Color ����
					//-- Color ����
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

					//-- �������
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

					//-- Color ����
					for(int t=0; t<4; t++)
						m_flame[t].color = D3DRGBA(random_color[0],random_color[1],random_color[2],fire_fade);//fade_color, 0x00 ,fade_color);
					//-- Render
					GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
				}
			}
		}
	}
	//-- Render State ����. ����������. �̷��� �����ؾ� �Ǵٴ�.
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, dwstate[0] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, dwstate[1] );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, dwstate[2] );

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwTextureState[2] );

	return FALSE;
}

