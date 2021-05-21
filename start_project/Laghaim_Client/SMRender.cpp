#include "stdafx.h"
// SMRender.cpp: implementation of the CSMRender class.
//
//////////////////////////////////////////////////////////////////////
#define D3D_OVERLOADS


#include <assert.h>
#include <ddraw.h>
#include <tchar.h>
#include "headers.h"
#include "ddutil.h"
#include "dxutil.h"
#include "main.h"
#include "D3DEnum.h"
#include "SMRender.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "IndexedTexture.h"




extern float g_fTimeKey;
extern BOOL g_bCsyMipmap;
//extern CBiped *g_pBipedList[];

//-----------------------------------------------------------------------------
// Name: EnumZBufferFormatsCallback2()
// Desc: Simply returns the first matching enumerated z-buffer format
//      d3dframe.cpp에 있는 것 가지고 와서 함수 이름만 바꾼것.
//-----------------------------------------------------------------------------
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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSMRender::CSMRender()
	: m_Xpos(0)
	, m_Ypos(0)
	, m_Width(0)
	, m_Height(0)
{
	memset(m_nCurFrame, 0, sizeof(m_nCurFrame));
	memset(m_fTimeElapsed, 0, sizeof(m_fTimeElapsed));

	m_pddsSmRenderTarget = NULL;
	m_pddsZBuffer = NULL;
	m_bWireframe = FALSE;
}

CSMRender::~CSMRender()
{
	DeleteRes();
}

void CSMRender::Init(int width, int height)
{
	m_Width = width;
	m_Height = height;
	LoadResources();
}

//-----------------------------------------------------------------------
// LoadResources()
// Desc : 리소스와 오브젝트를 로딩한다.
//     화면의 크기나 전화면 전환시 호출되어야 한다.
//-----------------------------------------------------------------------
void CSMRender::LoadResources()
{
	CreateDevices();
	Load3DResources();
}

//-----------------------------------------------------------------------
// CreateDevices()
// Desc : 3D Rendering을 위한 디바이스를 생성한다.
//-----------------------------------------------------------------------
HRESULT CSMRender::CreateDevices()
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;

	// 렌더링 버퍼 만들기.
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS;

	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = m_Width;
	ddsd.dwHeight       = m_Height;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;


	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsSmRenderTarget, NULL ) ) )
	{
		DEBUG_ERR( hr, "Error: Couldn't create Small Rendering Target" );
		if( hr != DDERR_OUTOFVIDEOMEMORY )
			return D3DFWERR_NOBACKBUFFER;
		DEBUG_MSG( "Error: Out of video memory" );
		return DDERR_OUTOFVIDEOMEMORY;
	}

	// z-Buffer 만들기.
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
	//DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(ddsd);
	m_pddsSmRenderTarget->GetSurfaceDesc( &ddsd );

	// Setup the surface desc for the z-buffer.
	DWORD dwDeviceMemType;
	dwDeviceMemType = pCMyApp->GetFramework()->GetDeviceMemType();
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | dwDeviceMemType;
	ddsd.ddpfPixelFormat.dwSize = 0;  // Tag the pixel format as unitialized.

	// Get an appropiate pixel format from enumeration of the formats. On the
	// first pass, we look for a zbuffer dpeth which is equal to the frame
	// buffer depth (as some cards unfornately require this).

	// 내가 넣은 코드
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

	if( FAILED( m_pddsSmRenderTarget->AddAttachedSurface( m_pddsZBuffer ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
		return D3DFWERR_NOZBUFFER;
	}

	return S_OK;
}


//-----------------------------------------------------------------------
// Load3DResources()
// Desc : lolo와 IndexedTexture는 다시 로드할 필요는 없지만.. 그래도 한다.
//-----------------------------------------------------------------------
HRESULT CSMRender::Load3DResources()
{
	for (int i=0; i < NUM_RACE; i++)
	{
		m_nCurFrame[i] = 0;
		m_fTimeElapsed[i] = 0.0f;
	}

	return S_OK;
}

//-----------------------------------------------------------------------
// DeleteRes()
// Desc : 지우고 싶은 것 대충 대충 지우자.
//-----------------------------------------------------------------------
void CSMRender::DeleteRes()
{
	SAFE_RELEASE( m_pddsZBuffer );
	SAFE_RELEASE(m_pddsSmRenderTarget);
}

void CSMRender::RenderCha(int x, int y, int width, int height, CNkCharacter *pNkCha, int page_num, BOOL bSelected)
{
	if( !pNkCha )
		return;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	LPDIRECTDRAWSURFACE7 lpRenderTarget;
	D3DVIEWPORT7 vpOld, vpNew;
	D3DMATERIAL7 mtrl;
	DWORD	dwColor;

	// 현재의 렌더 타겟을 가져온다.
	GET_D3DDEVICE()->GetRenderTarget(&lpRenderTarget);				// 참조카운트가 1증가한다.
	GET_D3DDEVICE()->SetRenderTarget(m_pddsSmRenderTarget, 0);

	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwX = 0;
	vpNew.dwY = 0;
	vpNew.dwWidth = RENDER_TARGET_WIDTH;
	vpNew.dwHeight = RENDER_TARGET_HEIGHT;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 0.4f, fAspect, 1.0f, 500.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );

	// 월드 트랜스폼
	float fBaseHeight = 100.0f;						// 90밑의 높이는 수영동작이다.
	D3DUtil_SetIdentityMatrix(matWorld);
	if (page_num == SELCHARPAGE)
	{
		vEyePt = D3DVECTOR(50, 40+fBaseHeight, -70);
		vLookatPt = D3DVECTOR(0, 10+fBaseHeight, 0);
		vUpVec = D3DVECTOR(0, 1, 0);
	}
	else
	{
		vEyePt = D3DVECTOR(50, 30+fBaseHeight, -60);
		vLookatPt = D3DVECTOR(0, 10+fBaseHeight, 0);
		vUpVec = D3DVECTOR(0, 1, 0);
	}

	D3DUtil_SetViewMatrix(matView,  vEyePt,     vLookatPt, vUpVec );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &matView );

	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00, 1.0f, 0L );

	// 라이팅
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFF333333 );

	// 라이트.
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

	if (g_bCsyMipmap)
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );

	// Begin the scene
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		if (GET_D3DDEVICE() == NULL)
		{
			MessageBox(NULL, "Device board", "warning", MB_OK);
		}
		assert(pNkCha != NULL);

		if (m_bWireframe)
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);

		pNkCha->Render(NULL, bSelected);

		if (m_bWireframe)
			GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);

		// End the scene.
		GET_D3DDEVICE()->EndScene();
	}

	if (g_bCsyMipmap)
		GET_D3DDEVICE()->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );

	GET_D3DDEVICE()->SetLight(0, &lightOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	// 원상복귀.
	GET_D3DDEVICE()->SetRenderTarget(lpRenderTarget, 0);
	lpRenderTarget->Release();			// 중요함.. 중요하쥐..
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->LightEnable(0, TRUE);

	if (width == RENDER_TARGET_WIDTH && height == RENDER_TARGET_HEIGHT)
		g_pDisplay->Blt(x, y, m_pddsSmRenderTarget);
	else if (width < RENDER_TARGET_WIDTH && height < RENDER_TARGET_HEIGHT)
	{
		RECT rcSrc, rcDest;

		assert(width <= RENDER_TARGET_WIDTH);
		assert(height <= RENDER_TARGET_HEIGHT);

		rcSrc.left = (RENDER_TARGET_WIDTH - width) / 2;
		rcSrc.top = (RENDER_TARGET_HEIGHT - height) / 2;
		rcSrc.right = rcSrc.left + width;
		rcSrc.bottom = rcSrc.top + height;

		rcDest.left = x;
		rcDest.top = y;
		rcDest.right = x+width;
		rcDest.bottom = y+height;

		if( g_pDisplay->GetBackBuffer() )
			g_pDisplay->GetBackBuffer()->Blt(&rcDest, m_pddsSmRenderTarget, &rcSrc, 0, NULL);
	}
}

bool CSMRender::CheckRestore()
{
	if( m_pddsZBuffer && m_pddsZBuffer->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}
	if( m_pddsSmRenderTarget && m_pddsSmRenderTarget->IsLost() == DDERR_SURFACELOST )
	{
		return false;
	}

	return true;
}

HRESULT	CSMRender::Restore()
{
	HRESULT hr = S_OK;

	if( m_pddsZBuffer )
	{
		m_pddsZBuffer->Restore();
	}
	if( m_pddsSmRenderTarget )
	{
		m_pddsSmRenderTarget->Restore();
	}

	return hr;
}