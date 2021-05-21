#include "stdafx.h"
#define D3D_OVERLOADS

#include "D3DUtil.h"
#include "LightCtrl.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightCtrl::CLightCtrl()
{
	m_dwAmbiColor = 0x00777777;
	m_fCurRange = 0.0f;
}

CLightCtrl::~CLightCtrl()
{
}

void CLightCtrl::InitLight(int light_num, D3DLIGHTTYPE ltType, FLOAT x, FLOAT y, FLOAT z )
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM)
		MessageBox(NULL, "I passed the parameters wrong.", "Warning", MB_OK);

	ZeroMemory( &m_d3dLight[light_num], sizeof(D3DLIGHT7) );
	m_d3dLight[light_num].dltType        = ltType;
	m_d3dLight[light_num].dcvDiffuse.r   = 1.0f;
	m_d3dLight[light_num].dcvDiffuse.g   = 1.0f;
	m_d3dLight[light_num].dcvDiffuse.b   = 1.0f;
	m_d3dLight[light_num].dcvSpecular    = m_d3dLight[light_num].dcvDiffuse;
	m_d3dLight[light_num].dvPosition.x   = x;
	m_d3dLight[light_num].dvPosition.y   = y;
	m_d3dLight[light_num].dvPosition.z   = z;
	m_d3dLight[light_num].dvDirection = Normalize(D3DVECTOR(x, y, z));
	m_d3dLight[light_num].dvAttenuation0 = 1.0f;
	m_d3dLight[light_num].dvRange        = D3DLIGHT_RANGE_MAX;
}

void CLightCtrl::InitLight( int light_num, D3DLIGHTTYPE ltType, FLOAT x, FLOAT y, FLOAT z,
							FLOAT r, FLOAT g, FLOAT b )
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM)
		MessageBox(NULL, "I passed the parameters wrong.", "warning", MB_OK);

	ZeroMemory( &m_d3dLight[light_num], sizeof(D3DLIGHT7) );
	m_d3dLight[light_num].dltType			= ltType;
	m_d3dLight[light_num].dcvDiffuse.r		= r;
	m_d3dLight[light_num].dcvDiffuse.g		= g;
	m_d3dLight[light_num].dcvDiffuse.b		= b;
	m_d3dLight[light_num].dcvSpecular		= m_d3dLight[light_num].dcvDiffuse;
	m_d3dLight[light_num].dvPosition.x		= x;
	m_d3dLight[light_num].dvPosition.y		= y;
	m_d3dLight[light_num].dvPosition.z		= z;
	m_d3dLight[light_num].dvDirection		= Normalize(D3DVECTOR(x, y, z));
	m_d3dLight[light_num].dvAttenuation0	= 1.0f;
	m_d3dLight[light_num].dvRange			= D3DLIGHT_RANGE_MAX;
}



void CLightCtrl::LightingEnalbe(BOOL bEnable)
{
	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, bEnable);
}

void CLightCtrl::LightEnable(int light_num, BOOL bEnable)
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM)
		MessageBox(NULL, "I passed the parameters wrong.", "Warning", MB_OK);

	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->LightEnable(light_num, bEnable);
}

void CLightCtrl::LightAllEnalbe(BOOL bEnable)
{
	if( !GET_D3DDEVICE() )
		return;

	for (int i=0; i < NKLIGHT_NUM; i++)
		GET_D3DDEVICE()->LightEnable(i, bEnable);
}

void CLightCtrl::SetLight(int light_num)
{
	if (light_num < 0 || light_num >= NKLIGHT_NUM)
		MessageBox(NULL, "I passed the parameters wrong.", "Warning", MB_OK);

	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->SetLight(light_num, &m_d3dLight[light_num]);
}

void CLightCtrl::SetAmbientLight(DWORD dwColor)
{
	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT,  dwColor);
}

void CLightCtrl::SetCurLandLight()
{
	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT,  m_dwAmbiColor);
}

void CLightCtrl::IncAmbientLight()
{
	m_dwAmbiColor += 0x00050505;

	if (m_dwAmbiColor >= 0x00EEEEEE)
		m_dwAmbiColor = 0x00EEEEEE;
}

void CLightCtrl::DecAmbientLight()
{
	m_dwAmbiColor -= 0x00050505;

	if (m_dwAmbiColor <= 0x00101010)
		m_dwAmbiColor = 0x00101010;
}

void CLightCtrl::SetDayLight()
{
	m_dwAmbiColor = 0x00777777;
	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT,  m_dwAmbiColor);
}

void CLightCtrl::SetNightLight()
{
	m_dwAmbiColor = 0x00233081;
	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT,  m_dwAmbiColor);
}

BOOL CLightCtrl::IsEnabled(int light_num)
{
	BOOL bEnabled = FALSE;

	if( GET_D3DDEVICE() )
		GET_D3DDEVICE()->GetLightEnable(light_num, &bEnabled);

	return bEnabled;
}