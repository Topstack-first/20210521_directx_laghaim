#include "stdafx.h"
#define D3D_OVERLOADS

#include <assert.h>
#include <ddraw.h>
#include <tchar.h>
#include "headers.h"
#include "ddutil.h"
#include "dxutil.h"
#include "main.h"
#include "D3DEnum.h"
#include "LHRenderTarget_Character.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "IndexedTexture.h"




extern float g_fTimeKey;
extern BOOL g_bCsyMipmap;

static HRESULT WINAPI EnumZBufferFormatsCallback2( DDPIXELFORMAT* pddpf,
		VOID* pContext )
{
	DDPIXELFORMAT* pddpfOut = (DDPIXELFORMAT*)pContext;

	if( pddpfOut->dwRGBBitCount == pddpf->dwRGBBitCount )
	{
		(*pddpfOut) = (*pddpf);
		return D3DENUMRET_CANCEL;
	}

	return D3DENUMRET_OK;
}


LHRenderTarget_Character::LHRenderTarget_Character(void)
	:	m_initialized(false)
	,	m_w(800)
	,	m_h(600)
	,	m_pddsRT(NULL)
	,	m_pddsZBuffer(NULL)
	,	m_character(new CNkCharacter(NULL,NULL,NULL))
	, m_race(0)
	, m_sex(0)
{
	for( int i=0 ; i<EquipPos__EndOfEnum ; i++ )
		m_wearing[i] = EquipPos__None;
}

LHRenderTarget_Character::~LHRenderTarget_Character(void)
{
	Release();
}

void LHRenderTarget_Character::Init(int w, int h, int race, int sex)
{
	if( S_OK != Create(w,h) )
		return;

	m_w = w;
	m_h = h;
	m_race = race;
	m_sex = sex;
	m_initialized = true;
}

HRESULT LHRenderTarget_Character::Create(int w, int h)
{
// 	HRESULT hr;
// 	DDSURFACEDESC2 ddsd;
//
// 	ZeroMemory( &ddsd, sizeof(ddsd) );
// 	ddsd.dwSize         = sizeof(ddsd);
// 	ddsd.dwFlags        = DDSD_CAPS;
//
// 	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
// 	ddsd.dwWidth        = w;
// 	ddsd.dwHeight       = h;
// 	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
//
//
// 	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsRT, NULL ) ) )
// 	{
// 		DEBUG_ERR( hr, "Error: Couldn't create Small Rendering Target" );
// 		if( hr != DDERR_OUTOFVIDEOMEMORY )
// 			return D3DFWERR_NOBACKBUFFER;
// 		DEBUG_MSG( "Error: Out of video memory" );
// 		return DDERR_OUTOFVIDEOMEMORY;
// 	}
//
// 	D3DDEVICEDESC7 ddDesc;
// 	GET_D3DDEVICE()->GetCaps( &ddDesc );
// 	if( ddDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
// 	{
// 		MessageBox(NULL, "Z-Buffer 생성 실패", "경고", MB_OK);
// 		return S_OK;
// 	}
//
// 	ddsd.dwSize = sizeof(ddsd);
// 	m_pddsRT->GetSurfaceDesc( &ddsd );
//
// 	DWORD dwDeviceMemType;
// 	dwDeviceMemType = pCMyApp->GetFramework()->GetDeviceMemType();
// 	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
// 	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | dwDeviceMemType;
// 	ddsd.ddpfPixelFormat.dwSize = 0;
//
// 	D3DEnum_DeviceInfo* pDeviceInfo;
// 	pDeviceInfo = pCMyApp->GetDeviceInfo();
// 	GET_D3D()->EnumZBufferFormats( *(pDeviceInfo->pDeviceGUID) , EnumZBufferFormatsCallback2,
// 		(VOID*)&ddsd.ddpfPixelFormat );
// 	if( 0 == ddsd.ddpfPixelFormat.dwSize )
// 	{
// 		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
// 		GET_D3D()->EnumZBufferFormats( *(pDeviceInfo->pDeviceGUID), EnumZBufferFormatsCallback2,
// 			(VOID*)&ddsd.ddpfPixelFormat );
//
// 		if( 0 == ddsd.ddpfPixelFormat.dwSize )
// 		{
// 			DEBUG_MSG( _T("Device doesn't support requested zbuffer format") );
// 			return D3DFWERR_NOZBUFFER;
// 		}
// 	}
//
// 	// Create and attach a z-buffer
// 	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsZBuffer, NULL ) ) )
// 	{
// 		DEBUG_MSG( _T("Error: Couldn't create a ZBuffer surface") );
// 		if( hr != DDERR_OUTOFVIDEOMEMORY )
// 			return D3DFWERR_NOZBUFFER;
// 		DEBUG_MSG( _T("Error: Out of video memory") );
// 		return DDERR_OUTOFVIDEOMEMORY;
// 	}
//
// 	if( FAILED( m_pddsSmRenderTarget->AddAttachedSurface( m_pddsZBuffer ) ) )
// 	{
// 		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
// 		return D3DFWERR_NOZBUFFER;
// 	}

	return S_OK;
}

void LHRenderTarget_Character::InitCharacter(int race, int sex)
{
	if( !m_character )
		return;

	// g_pRoh;
	// m_character->SetBiped();

	m_character->SetInitPos(0,0);
	m_character->m_Race = race;
	m_character->m_Sex;
}


void LHRenderTarget_Character::Reset()
{
}

void LHRenderTarget_Character::Release()
{
	SAFE_DELETE(m_character);
	SAFE_RELEASE(m_pddsRT);
	SAFE_RELEASE(m_pddsZBuffer);
}

void LHRenderTarget_Character::Invalid()
{
	SAFE_RELEASE(m_pddsRT);
	SAFE_RELEASE(m_pddsZBuffer);
}

void LHRenderTarget_Character::Restore()
{
	Invalid();
	Create(m_w, m_h);
}

void LHRenderTarget_Character::Render(int x, int y)
{
	if( !m_character )
		return;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	LPDIRECTDRAWSURFACE7 lpRenderTarget;
	D3DVIEWPORT7 vpOld, vpNew;
	D3DMATERIAL7 mtrl;
	DWORD	dwColor;

	GET_D3DDEVICE()->GetRenderTarget(&lpRenderTarget);
	GET_D3DDEVICE()->SetRenderTarget(m_pddsRT, 0);

	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwX = 0;
	vpNew.dwY = 0;
	vpNew.dwWidth = m_w;
	vpNew.dwHeight = m_h;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 0.4f, fAspect, 1.0f, 500.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );

	float fBaseHeight = 100.0f;						// 90밑의 높이는 수영동작이다.
	D3DUtil_SetIdentityMatrix(matWorld);

	vEyePt = D3DVECTOR(50, 30+fBaseHeight, -60);
	vLookatPt = D3DVECTOR(0, 10+fBaseHeight, 0);
	vUpVec = D3DVECTOR(0, 1, 0);

	D3DUtil_SetViewMatrix(matView,  vEyePt,     vLookatPt, vUpVec );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView );

	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00, 1.0f, 0L );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFF333333 );

	D3DLIGHT7 lightOld, d3dLight;

	GET_D3DDEVICE()->GetLight(0, &lightOld);

	ZeroMemory( &d3dLight, sizeof(D3DLIGHT7) );
	d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	d3dLight.dcvDiffuse.r   = 1.0f;
	d3dLight.dcvDiffuse.g   = 1.0f;
	d3dLight.dcvDiffuse.b   = 1.0f;
	d3dLight.dcvSpecular    = d3dLight.dcvDiffuse;
	d3dLight.dvDirection = Normalize(D3DVECTOR(0, -1000, 0));

	d3dLight.dvAttenuation0 = 1.0f;
	d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
	GET_D3DDEVICE()->SetLight(0, &d3dLight);

	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		if (GET_D3DDEVICE() == NULL)
		{
			MessageBox(NULL, "Error : Deivce is Null.", "Caution!", MB_OK);
			return;
		}

		m_character->Render(NULL,FALSE,TRUE);

		GET_D3DDEVICE()->EndScene();
	}

	GET_D3DDEVICE()->SetLight(0, &lightOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetRenderTarget(lpRenderTarget, 0);
	lpRenderTarget->Release();
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->LightEnable(0, TRUE);

// 	if( g_pDisplay->GetBackBuffer() )
// 	{
// 		g_pDisplay->Blt(x, y, m_pddsRT);
// 		g_pDisplay->GetBackBuffer()->Blt(&rcDest, m_pddsRT, &rcSrc, 0, NULL);
// 	}
}

void LHRenderTarget_Character::FrameMove()
{
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	char fCharPosDir[] = {-1.23f, -1.4, -1.57f, -1.74f, -1.91f};

	static int lastTickCount = 0;
	static int	 frame = 0;

	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	mtrl.diffuse.a = 0.5f;
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	vEyePt    = D3DVECTOR(0, 140, -140);
	vLookatPt = D3DVECTOR(0, 110, 0);
	vUpVec    = D3DVECTOR(0, 1, 0);


	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );

	D3DLIGHT7 m_d3dLight;

	ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
	m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	m_d3dLight.dcvDiffuse.a   = 0.5f;
	m_d3dLight.dcvDiffuse.r   = 1.0f;
	m_d3dLight.dcvDiffuse.g   = 1.0f;
	m_d3dLight.dcvDiffuse.b   = 1.0f;
	m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
	m_d3dLight.dvDirection = Normalize(D3DVECTOR(1000, -1000, -1000));

	m_d3dLight.dvAttenuation0 = 1.0f;
	m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;

	GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
	GET_D3DDEVICE()->LightEnable(0, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

// 	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
// 	D3DUtil_SetProjectionMatrix(m_matProjSel, m_FovSel, fAspect, m_zNear, m_zFar );
// 	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProjSel);

	D3DUtil_SetIdentityMatrix( matWorld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

	DWORD dwAmbientOld, dwAmbient;

	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbientOld );

// 	m_character->m_wx = 0;
// 	m_character->m_wy = 0;
// 	m_character->m_wz = 0;
// 	m_character->m_dir = -1.57f; //fCharPosDir[idx];

	dwAmbient = 0xFF555555;

	m_character->SetAmbient(TRUE, dwAmbient);
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		m_character->Render(NULL, FALSE);

		GET_D3DDEVICE()->EndScene();
	}
	m_character->SetAmbient(FALSE, 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbientOld );
}



void LHRenderTarget_Character::Equip(int wear, int vnum)
{

}

void LHRenderTarget_Character::UnEquip(int wear, int vnum)
{

}