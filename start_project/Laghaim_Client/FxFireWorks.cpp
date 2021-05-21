#include "stdafx.h"
#include "sfx.h"
#include "main.h"
#include "NkCharacter.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "GUtil.h"
#include "assert.h"
#include "stdlib.h" //-- rand

//----------------------------------------------------------------
#define PI 0.017453      //3.1416/180
#define random(num) (rand()%(num))


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxFireWorks::CFxFireWorks()
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
				assert(i<FIREWORKS_NUM);
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
				assert(i<FIREWORKS_NUM);
			}
		}
	}
	//--------------------------------------------------------------------------
	//-- Move Vector �ʱ�ȭ - Fountain
	for( i = 0; i<FIREWORKS_NUM_FON; i++)
	{
		int algle = random(360);
		m_flamemove_Fountain[i].x = float(random(45)+5)/10.0f  * cos(double(double(algle)*PI));
		m_flamemove_Fountain[i].z = float(random(45)+5)/10.0f  * sin(double(double(algle)*PI));
		m_flamemove_Fountain[i].y = float(random(8)+10) / 3.0f + 1.0f ;
		m_startframe[i] = random(20);
	}

}

CFxFireWorks::~CFxFireWorks()
{}

BOOL CFxFireWorks::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	//-- small ȣ��
	if(effect_sort.num ==0)
	{
		Render_Small(effect_sort);
		return FALSE;
	}

	//-- Render_Screw
	if(effect_sort.num ==2)
	{
		Render_Screw(effect_sort);
		return FALSE;
	}

	//-- Fountain
	if(effect_sort.num ==3)
	{
		Render_Fountain(effect_sort);
		return FALSE;
	}

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
	//-- �߻� ȿ��.
	//--------------------------------------------------------------------------
	if(nFrame<6)
	{
		//-- �� Flame ���� �̵�.
		matWorld._41 = vPosFrom.x; //+ m_position[i].x;
		matWorld._42 = vPosFrom.y + nFrame * 7.0f;
		matWorld._43 = vPosFrom.z;// + m_position[i].z;

		//-- ������ ����.
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		MakeLVertexRect(m_flame, 4.0f, 15.5f, TRUE);

		//-- Texture Animation ����.
		m_flame[0].tu = (nFrame%4) * 0.25f;
		m_flame[1].tu = (nFrame%4) * 0.25f;
		m_flame[2].tu = ((nFrame%4)+1) * 0.25f;
		m_flame[3].tu = ((nFrame%4)+1) * 0.25f;
		//-- Color ����
		for(int t=0; t<4; t++)m_flame[t].color = D3DRGBA(1.0f,0.2f,0.1f,1.0f);
		//-- Render
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
	}
	//--------------------------------------------------------------------------
	//-- ���� ȿ��.
	//--------------------------------------------------------------------------
	else
	{
		//-- ��� Flame �� ���Ͽ�
		for(i=1; i<FIREWORKS_NUM; i++)
		{
			//-- 3���� ȿ�� - -125 * 3 �̴�.
			for(int k=0; k<3; k++)
			{
				//-- ���� ������ ����.
				int virframe = nFrame -5 - k;

				//-- ��ǥ �ʱ�ȭ
				m_position.x = 0.0f;
				m_position.y = 42.0f;
				m_position.z = 0.0f;

				//-- ���� ������ ��ŭ ���ϸ��̼� ����
				for(int j=0; j<virframe; j++)
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
				case 0:
					random_color[0] = 1.0f;
					random_color[1] = 0.0f;
					random_color[2] = 0.0f;
					break;
				case 1:
					random_color[0] = 1.0f;
					random_color[1] = 1.0f;
					random_color[2] = 0.0f;
					break;
				case 6:
					random_color[0] = 1.0f;
					random_color[1] = 1.0f;
					random_color[2] = 0.0f;
					break;
				case 2:
					random_color[0] = 0.0f;
					random_color[1] = 1.0f;
					random_color[2] = 0.0f;
					break;
				case 3:
					random_color[0] = 0.0f;
					random_color[1] = 1.0f;
					random_color[2] = 1.0f;
					break;
				case 4:
					random_color[0] = 0.0f;
					random_color[1] = 0.0f;
					random_color[2] = 1.0f;
					break;
				case 5:
					random_color[0] = 1.0f;
					random_color[1] = 0.0f;
					random_color[2] = 1.0f;
					break;
				default :
					random_color[0] = 1.0f;
					random_color[1] = 1.0f;
					random_color[2] = 1.0f;
					break;
				}
				//-- fade_color
				fire_fade = 1.0f;
				if(virframe  >FX_FIREWORKS_FRAMECOUNT -25)
					fire_fade = float(FX_FIREWORKS_FRAMECOUNT - virframe)/25.0f;

				//-- Color ����
				for(int t=0; t<4; t++)
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
}
//----------------------------------------------------------------------------
//-- Render Screw
//----------------------------------------------------------------------------
BOOL CFxFireWorks::Render_Screw(EffectSort &effect_sort)
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
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float fire_fade = 0.3f;
	float fade_color = 0.0;
	int random_co;
	float random_color[3];

	//--------------------------------------------------------------------------
	//-- �߻� ȿ��. Screw
	//--------------------------------------------------------------------------
	if(nFrame<6)
	{
		//-- �� Flame ���� �̵�.
		matWorld._41 = vPosFrom.x; //+ m_position[i].x;
		matWorld._42 = vPosFrom.y + nFrame * 7.0f;
		matWorld._43 = vPosFrom.z;// + m_position[i].z;

		//-- ������ ����.
		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		MakeLVertexRect(m_flame, 4.0f, 15.5f, TRUE);

		//-- Texture Animation ����.
		m_flame[0].tu = (nFrame%4) * 0.25f;
		m_flame[1].tu = (nFrame%4) * 0.25f;
		m_flame[2].tu = ((nFrame%4)+1) * 0.25f;
		m_flame[3].tu = ((nFrame%4)+1) * 0.25f;
		//-- Color ����
		for(int t=0; t<4; t++)
			m_flame[t].color = D3DRGBA(1.0f,0.2f,0.1f,1.0f);

		//-- Render
		GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
	}
	//--------------------------------------------------------------------------
	//-- ���� ȿ��. Screw
	//--------------------------------------------------------------------------
	else
	{
		//-- 4���� Color
		for(i=0; i<6; i++)
		{
			//-- 10 ���� ȿ��
			for(int j=0; j<14; j++)
			{
				//-- �߻縦 ������ ���� ȿ���� ������ ����.
				int virframe = nFrame -5;

				//-- ��ǥ �ʱ�ȭ
				m_position.x = 0.0f;
				m_position.y = 52.0f;
				m_position.z = 0.0f;

				//-- ����
				if(j <= virframe)
				{
					//-- ���� ������ ��ŭ ���ϸ��̼� ����
					float tail = j/2.0f; //-- �󸶳� ���� �������� ������ ���̳�
					float angle = float(virframe/1.0f - tail + float(i*4))*(3.1416/12);

					m_position.x += float(virframe-tail) * 1.5f * sin(angle) ;
					m_position.y += m_flamemove[i].y - ((0.09f*(virframe-tail) - 0.3f)* (virframe-tail) ); //-- �Ҳ� �귯����
					m_position.z += float(virframe-tail) * 1.5f * cos(angle) ;


					//-- �� Flame ���� �̵�.
					matWorld._41 = vPosFrom.x + m_position.x;
					matWorld._42 = vPosFrom.y + m_position.y;
					matWorld._43 = vPosFrom.z + m_position.z;

					//-- ������ ����.
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
					MakeLVertexRect(m_flame, 14.0f, 14.0f, TRUE);

					//-- Texture Animation ����.
					m_flame[0].tu = ((virframe*j*i)%4) * 0.25f;
					m_flame[1].tu = ((virframe*j*i)%4) * 0.25f;
					m_flame[2].tu = (((virframe*j*i)%4)+1) * 0.25f;
					m_flame[3].tu = (((virframe*j*i)%4)+1) * 0.25f;

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
					fire_fade = (20.0f - j)/20.0f;

					if(virframe  >FX_FIREWORKS_FRAMECOUNT -25)
						fire_fade = float(FX_FIREWORKS_FRAMECOUNT - virframe)/25.0f;

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
//----------------------------------------------------------------------------
//-- Render Heart
//----------------------------------------------------------------------------
BOOL CFxFireWorks::Render_Fountain(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

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
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float fire_fade = 0.3f;
	float fade_color = 0.0;
	int random_co;
	float random_color[3];

	//--------------------------------------------------------------------------
	//-- ���� ȿ��. Screw
	//--------------------------------------------------------------------------

	//-- Move Vector �ʱ�ȭ - Fountain
	for(i=0; i<FIREWORKS_NUM_FON; i++) //
	{
		//-- ��ǥ �ʱ�ȭ
		m_position.x = 02.0f;
		m_position.y = -10.0f;
		m_position.z = 0.0f;

		//-- ���� ��ƼŬ ���� Frame�� �����ٸ�
		if(nFrame>m_startframe[i])
		{
			//-- ���� ��ƼŬ�� ����
			int virframe = nFrame - m_startframe[i];

			m_position.x += m_flamemove_Fountain[i].x * virframe/2.0f;
			m_position.y += m_flamemove_Fountain[i].y * virframe - (virframe * virframe * 0.18f);
			m_position.z += m_flamemove_Fountain[i].z * virframe/2.0f;

			//-- �� Flame ���� �̵�.
			matWorld._41 = vPosFrom.x + m_position.x;
			matWorld._42 = vPosFrom.y + m_position.y;
			matWorld._43 = vPosFrom.z + m_position.z;

			//-- ������ ����.
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			MakeLVertexRect(m_flame, 10.0f, 10.0f, TRUE);

			//-- Texture Animation ����.
			m_flame[0].tu = ((virframe*i*nFrame)%4) * 0.25f;
			m_flame[1].tu = ((virframe*i*nFrame)%4) * 0.25f;
			m_flame[2].tu = (((virframe*i*nFrame)%4)+1) * 0.25f;
			m_flame[3].tu = (((virframe*i*nFrame)%4)+1) * 0.25f;

			//-- �������
			random_co = random(6);
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
				random_color[1] = 1.0f;
				random_color[2] = 1.0f;
				break;
			case 4:
				random_color[0] = 0.4f;
				random_color[1] = 0.4f;
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
			//-- fade_color
			fire_fade = 1.0f;

			if(virframe  >FX_FIREWORKS_FRAMECOUNT -25)
				fire_fade = float(FX_FIREWORKS_FRAMECOUNT - virframe)/25.0f;

			//-- Color ����
			for(int t=0; t<4; t++)
				m_flame[t].color = D3DRGBA(random_color[0],random_color[1],random_color[2],fire_fade);//fade_color, 0x00 ,fade_color);
			//-- Render
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
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


//----------------------------------------------------------------------------
//-- Render Small
//----------------------------------------------------------------------------
//-- �Ҳ��� ���� Item �� Render
BOOL CFxFireWorks::Render_Small(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	vViewVec = pCMyApp->GetEyeDirection();

	int	i = 0;
	int inst = 0; //-- Small �� ��� �� 3���� �߻�ȴ�.
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
		lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_FIREWORK ] ) ;
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	float fire_fade = 0.3f;
	float fade_color = 0.0;
	int random_co;
	float random_color[3];
	int inst_frame = 0; //-- ������ instance �� ���� Frame

	//--------------------------------------------------------------------------
	//-- �� 4���� �Ҳ��� ������.
	//--------------------------------------------------------------------------

	for(inst = 0; inst  <4; inst ++)
	{
		inst_frame  =  nFrame - inst*6;//-- �� ��ü�� 2Frame�� �ڿ� ǥ��.
		//--------------------------------------------------------------------------
		//-- �߻� ȿ��.
		//--------------------------------------------------------------------------
		if(inst_frame > 0 && inst_frame <4 && inst == 0)
		{
			//-- �� Flame ���� �̵�.
			matWorld._41 = vPosFrom.x ;
			matWorld._42 = vPosFrom.y + inst_frame * 9.0f;
			matWorld._43 = vPosFrom.z ;

			//-- ������ ����.
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
			MakeLVertexRect(m_flame, 4.0f, 15.5f, TRUE);

			//-- Texture Animation ����.
			m_flame[0].tu = (inst_frame%4) * 0.25f;
			m_flame[1].tu = (inst_frame%4) * 0.25f;
			m_flame[2].tu = ((inst_frame%4)+1) * 0.25f;
			m_flame[3].tu = ((inst_frame%4)+1) * 0.25f;
			//-- Color ����
			for(int t=0; t<4; t++)
				m_flame[t].color = D3DRGBA(1.0f,0.2f,0.1f,1.0f);
			//-- Render
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX,m_flame, 4, 0 );
		}
		//--------------------------------------------------------------------------
		//-- ���� ȿ��.
		//--------------------------------------------------------------------------
		else if(inst_frame > 4 && inst_frame < 26)
		{
			//-- ��� Flame �� ���Ͽ�
			for(i=1; i<FIREWORKS_NUM_SM; i++)
			{
				//-- 3���� ȿ��
				for(int k=0; k<5; k++)
				{
					//-- ���� ������ ����.
					int virframe = inst_frame -4 - k;

					//-- ��ǥ �ʱ�ȭ
					m_position.x = 0.0f;
					m_position.y = 36.0f;
					m_position.z = 0.0f;
					switch(inst)
					{
					case 0:
						m_position.x  = 15.0f;
						m_position.z  = 15.0f;
						break;
					case 1:
						m_position.x  = -15.0f;
						m_position.z  = -15.0f;
						break;
					case 2:
						m_position.x  = 15.0f;
						m_position.z  = -15.0f;
						break;
					case 3:
						m_position.x  = -15.0f;
						m_position.z  = 15.0f;
						break;
					default :
						break;
					}

					//-- ���� ������ ��ŭ ���ϸ��̼� ����
					for(int j=0; j<virframe; j++)
					{
						m_position.x += ((33-virframe)/6)*m_flamemove_small[i].x;
						m_position.y += ((33-virframe)/6)*m_flamemove_small[i].y;
						m_position.z += ((33-virframe)/6)*m_flamemove_small[i].z;
					}

					//-- �� Flame ���� �̵�.
					matWorld._41 = vPosFrom.x + m_position.x;
					matWorld._42 = vPosFrom.y + m_position.y;
					matWorld._43 = vPosFrom.z + m_position.z;

					//-- ������ ����.
					GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
					MakeLVertexRect(m_flame, 5.0f, 5.0f, TRUE);

					//-- Texture Animation ����.
					m_flame[0].tu = (virframe%4) * 0.25f;
					m_flame[1].tu = (virframe%4) * 0.25f;
					m_flame[2].tu = ((virframe%4)+1) * 0.25f;
					m_flame[3].tu = ((virframe%4)+1) * 0.25f;

					//-- Color ����
					switch(inst)
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
					default :
						random_color[0] = 1.0f;
						random_color[1] = 1.0f;
						random_color[2] = 1.0f;
						break;
					}
					//-- random ���
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
					fire_fade = 1.0f;
					if(virframe  >FX_FIREWORKS_FRAMECOUNT -30)
						fire_fade = float(FX_FIREWORKS_FRAMECOUNT - virframe)/30.0f;

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