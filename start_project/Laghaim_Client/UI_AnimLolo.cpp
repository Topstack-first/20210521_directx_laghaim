#include "stdafx.h"
#define D3D_OVERLOADS
#include "UI_AnimLolo.h"
#include "ddutil.h"
#include "dxutil.h"
#include "main.h"
#include "IndexedTexture.h"
#include "CapSyong.h"

#define OFFSCREEN_SIZE	1024

static HRESULT WINAPI EnumZBufferFormatsCallback2(DDPIXELFORMAT* pddpf, VOID* pContext)
{
	DDPIXELFORMAT* pddpfOut = (DDPIXELFORMAT*)pContext;

	if( pddpfOut->dwRGBBitCount == pddpf->dwRGBBitCount )
	{
		(*pddpfOut) = (*pddpf);
		return D3DENUMRET_CANCEL;
	}

	return D3DENUMRET_OK;
}

//////////////////////////////////////////////////////////////////////////
UI_AnimLolo::UI_AnimLolo()
	: m_pddsBlt(NULL)
	, m_pddsRenderTarget(NULL)
	, m_pddsZBuffer(NULL)
	, m_pLolo(NULL)
	, m_pTexture(NULL)
	, m_posX(0)
	, m_posY(0)
	, m_width(128)
	, m_height(128)
	, m_curFrame(0)
	, m_maxFrame(0)	
	, m_bPlaying(false)
	, m_bLoop(false)	
{
	m_vEyePt = D3DVECTOR(120, 10, 0);
	m_vLookatPt = D3DVECTOR(0, 10, 0);
	m_vUpVec = D3DVECTOR(0, 1, 0);
}

UI_AnimLolo::~UI_AnimLolo()
{
	DeleteRes();
}

HRESULT	UI_AnimLolo::CreateDevices()
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS;

	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = OFFSCREEN_SIZE;
	ddsd.dwHeight       = OFFSCREEN_SIZE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;


	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsRenderTarget, NULL ) ) )
	{
		DEBUG_ERR( hr, "Error: Couldn't create Small Rendering Target" );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( "Error: Out of video memory" );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	// 블릿용 서피스 만들기.
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = OFFSCREEN_SIZE;
	ddsd.dwHeight       = OFFSCREEN_SIZE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsBlt, NULL ) ) )
	{
		DEBUG_ERR( hr, "Error: Couldn't create surface for blt" );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( "Error: Out of video memory" );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	
	// Check if the device supports z-bufferless hidden surface removal. If so,
	// we don't really need a z-buffer
	D3DDEVICEDESC7 ddDesc;
	GET_D3DDEVICE()->GetCaps( &ddDesc );
	if( ddDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
	{
		MessageBox(NULL, "Z-Buffer Failed to create", "warning", MB_OK);
		return S_OK;
	}
	// Get z-buffer dimensions from the render target	
	ddsd.dwSize = sizeof(ddsd);
	m_pddsRenderTarget->GetSurfaceDesc( &ddsd );

	// Setup the surface desc for the z-buffer.
	DWORD dwDeviceMemType;
	dwDeviceMemType = pCMyApp->GetFramework()->GetDeviceMemType();
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | dwDeviceMemType;
	ddsd.ddpfPixelFormat.dwSize = 0;  // Tag the pixel format as unitialized.

	// Get an appropiate pixel format from enumeration of the formats. On the
	// first pass, we look for a zbuffer dpeth which is equal to the frame
	// buffer depth (as some cards unfornately require this).

	D3DEnum_DeviceInfo* pDeviceInfo;
	pDeviceInfo = pCMyApp->GetDeviceInfo();
	GET_D3D()->EnumZBufferFormats( *(pDeviceInfo->pDeviceGUID) , EnumZBufferFormatsCallback2,
		(VOID*)&ddsd.ddpfPixelFormat );
	if( 0 == ddsd.ddpfPixelFormat.dwSize )
	{
		// Try again, just accepting any 16-bit zbuffer
		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
		GET_D3D()->EnumZBufferFormats( *(pDeviceInfo->pDeviceGUID), EnumZBufferFormatsCallback2,
			(VOID*)&ddsd.ddpfPixelFormat );

		if( 0 == ddsd.ddpfPixelFormat.dwSize )
		{
			DEBUG_MSG( _T("Device doesn't support requested zbuffer format") );
			return D3DFWERR_NOZBUFFER;
		}
	}

	// Create and attach a z-buffer
	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsZBuffer, NULL ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't create a ZBuffer surface") );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOZBUFFER;
		DEBUG_MSG( _T("Error: Out of video memory") );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	if( FAILED( m_pddsRenderTarget->AddAttachedSurface( m_pddsZBuffer ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
		return D3DFWERR_NOZBUFFER;
	}

 	DDCOLORKEY ddck;
 	ddck.dwColorSpaceLowValue  = RGB(0xff, 0xff, 0xff);
 	ddck.dwColorSpaceHighValue = RGB(0xff, 0xff, 0xff);
 	m_pddsBlt->SetColorKey( DDCKEY_SRCBLT, &ddck );

	return S_OK;
}

void UI_AnimLolo::DeleteRes()
{
	SAFE_RELEASE(m_pddsZBuffer);
	SAFE_RELEASE(m_pddsRenderTarget);
	SAFE_RELEASE(m_pddsBlt);
	SAFE_DELETE(m_pLolo);
}

void UI_AnimLolo::Init(const int width, const int height)
{
	m_width = width;
	m_height = height;	
	CreateDevices();
}

void UI_AnimLolo::SetPos(const int posX, const int posY)
{
	m_posX = posX;
	m_posY = posY;
}

void UI_AnimLolo::SetEyeAt(const D3DVECTOR& vEye, const D3DVECTOR& vAt)
{
	m_vEyePt = vEye;
	m_vLookatPt = vAt;
}

void UI_AnimLolo::LoadLolo(const std::string& fileName)
{
	m_pLolo = new CLolos;
	m_pTexture = new IndexedTexture(32);
	m_pTexture->SetTexturePath("data/effect/textr/");

	m_pLolo->SetIndexedTexture(m_pTexture);
	
	if( m_pLolo->Read((char*)fileName.c_str()) == FALSE )
	{
		SAFE_DELETE(m_pLolo);
		SAFE_DELETE(m_pTexture);
		return;
	}
	m_pLolo->RestoreTextures(GET_D3DDEVICE());

	m_curFrame = 0;
	m_maxFrame = m_pLolo->m_AnimCount;
}

void UI_AnimLolo::SetTexturePath(const std::string& path)
{
	if( m_pTexture == NULL )
		return;

	m_pTexture->SetTexturePath(path.c_str());
}

void UI_AnimLolo::SetLolo(CLolos* pLolo)
{
	SAFE_DELETE(m_pLolo);
	m_pLolo = pLolo;

	if( m_pLolo != NULL )
	{
		m_curFrame = 0;
		m_maxFrame = m_pLolo->m_AnimCount;
	}
}

void UI_AnimLolo::Play(const bool loop)
{
	m_bPlaying = true;
	m_bLoop = loop;
}

void UI_AnimLolo::Pause()
{
	m_bPlaying = false;
}

void UI_AnimLolo::Stop()
{
	m_curFrame = 0;
	m_bPlaying = false;
}

void UI_AnimLolo::Render()
{
	if( m_pLolo == NULL )
		return;
	
	if( m_bPlaying == false )
		return;

	++m_curFrame;

	if( m_curFrame > m_maxFrame )
	{
		m_curFrame = 0;

		if( m_bLoop == false )
		{
			m_bPlaying = false;		
		}		
	}	

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;	
	LPDIRECTDRAWSURFACE7 lpRenderTarget;
	D3DVIEWPORT7 vpOld, vpNew;
	D3DMATERIAL7 mtrl;
	DWORD	dwColor;
	
	GET_D3DDEVICE()->GetRenderTarget(&lpRenderTarget);				// 참조카운트가 1증가한다.
	GET_D3DDEVICE()->SetRenderTarget(m_pddsRenderTarget, 0);
	
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);
	
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwX = 0;
	vpNew.dwY = 0;
 	vpNew.dwWidth = OFFSCREEN_SIZE;
 	vpNew.dwHeight = OFFSCREEN_SIZE;
	GET_D3DDEVICE()->SetViewport(&vpNew);
	
	float fAspect = (float) ((double) 4.0 / (double) 3.0);	
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 0.4f, fAspect, 1.0f, 500.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	
	D3DUtil_SetIdentityMatrix(matWorld);
	D3DUtil_SetViewMatrix(matView, m_vEyePt, m_vLookatPt, m_vUpVec);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00ffffff, 1.0f, 0L );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);	
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

	// Begin the scene
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{	
		m_pLolo->Render(GET_D3DDEVICE(), m_curFrame);
		GET_D3DDEVICE()->EndScene();
	}

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	// 원상복귀.
	GET_D3DDEVICE()->SetRenderTarget(lpRenderTarget, 0);
	lpRenderTarget->Release();
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->LightEnable(0, TRUE);

	RECT rcSrc, rcDest;
	
	rcSrc.left = 0;
	rcSrc.top = 0;
	rcSrc.right = OFFSCREEN_SIZE;
	rcSrc.bottom = OFFSCREEN_SIZE;

	rcDest.left = m_posX;
	rcDest.top = m_posY;
	rcDest.right = m_posX + m_width;
	rcDest.bottom = m_posY + m_height;

	m_pddsBlt->BltFast(0, 0, m_pddsRenderTarget, NULL, 0);
	g_pDisplay->Blt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, m_pddsBlt, &rcSrc, DDBLT_KEYSRC);
}

bool UI_AnimLolo::CheckRestore()
{
	if( m_pddsZBuffer && m_pddsZBuffer->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}
	if( m_pddsBlt && m_pddsBlt->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}
	if( m_pddsRenderTarget && m_pddsRenderTarget->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	return true;
}

HRESULT UI_AnimLolo::Restore()
{
	HRESULT hr = S_OK;

	if( m_pddsZBuffer )
	{
		m_pddsZBuffer->Restore();
	}
	if( m_pddsBlt )
	{
		m_pddsBlt->Restore();
	}
	if( m_pddsRenderTarget )
	{
		m_pddsRenderTarget->Restore();
	}

	return hr;
}


