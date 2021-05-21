#include "stdafx.h"
#define D3D_OVERLOADS

#include <assert.h>
#include <ddraw.h>
#include <tchar.h>
#include <mmsystem.h>
#include "headers.h"
#include "ddutil.h"
#include "dxutil.h"
#include "main.h"
#include "D3DEnum.h"
#include "D3DMath.h"
#include "ItemRender.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "IndexedTexture.h"
//#include "Nk2DFrame.h"
#include "ControlInven.h"



//
extern float g_fTimeKey;

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

CItemRender::CItemRender()
	: m_Xpos(0)
	, m_Ypos(0)
	, m_Width(0)
	, m_Height(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_dwAlphaTest(0)
	, m_dwAlphaBlend(0)
	, m_lightEnable0(FALSE)
	, m_lightEnable1(FALSE)
	, m_lightEnable2(FALSE)
	, m_dwColor(0)
{
	memset(m_nCurFrame, 0, sizeof(m_nCurFrame));
	memset(m_fTimeElapsed, 0, sizeof(m_fTimeElapsed));

	m_pddsRenderTarget = NULL;
	m_pddsZBuffer = NULL;
	m_pddsBlt = NULL;

	m_lastTime = timeGetTime();
	m_Frame = 0;

	MakeLight();
}

CItemRender::~CItemRender()
{
	DeleteRes();
}


void CItemRender::Init()
{
	LoadResources();
}

//-----------------------------------------------------------------------
// LoadResources()
// Desc : 리소스와 오브젝트를 로딩한다.
//     화면의 크기나 전화면 전환시 호출되어야 한다.
//-----------------------------------------------------------------------
void CItemRender::LoadResources()
{
	CreateDevices();
	Load3DResources();
}

//-----------------------------------------------------------------------
// CreateDevices()
// Desc : 3D Rendering을 위한 디바이스를 생성한다.
//-----------------------------------------------------------------------
HRESULT CItemRender::CreateDevices()
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;

	// 렌더링 버퍼 만들기.
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth        = ITEM_RENDER_WIDTH;
	ddsd.dwHeight       = ITEM_RENDER_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;

	m_Width = ITEM_RENDER_WIDTH;
	m_Height =ITEM_RENDER_HEIGHT;

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
	ddsd.dwWidth        = ITEM_RENDER_WIDTH;
	ddsd.dwHeight       = ITEM_RENDER_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	if( FAILED( hr = GET_DD7()->CreateSurface( &ddsd, &m_pddsBlt, NULL ) ) )
	{
		DEBUG_ERR( hr, "Error: Couldn't create surface for blt" );
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
		return S_OK;
	// Get z-buffer dimensions from the render target
	//DDSURFACEDESC2 ddsd;
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

	if( FAILED( m_pddsRenderTarget->AddAttachedSurface( m_pddsZBuffer ) ) )
	{
		DEBUG_MSG( _T("Error: Couldn't attach zbuffer to render surface") );
		return D3DFWERR_NOZBUFFER;
	}

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue  = ConvertGDIColor( ITEM_KEY_COLOR_REF );
	ddck.dwColorSpaceHighValue = ConvertGDIColor( ITEM_KEY_COLOR_REF );
	m_pddsBlt->SetColorKey( DDCKEY_SRCBLT, &ddck );

	return S_OK;
}

DWORD CItemRender::ConvertGDIColor( COLORREF dwGDIColor )
{
	if( m_pddsRenderTarget == NULL )
		return 0x00000000;

	COLORREF       rgbT;
	HDC            hdc;
	DWORD          dw = CLR_INVALID;
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	if (m_pddsRenderTarget->IsLost() == DDERR_SURFACELOST)
	{
		m_pddsRenderTarget->Restore();
	}

	//  Use GDI SetPixel to color match for us
	if( dwGDIColor != CLR_INVALID && m_pddsRenderTarget->GetDC(&hdc) == DD_OK)
	{
		rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
		SetPixel(hdc, 0, 0, dwGDIColor);       // Set our value
		m_pddsRenderTarget->ReleaseDC(hdc);
	}

	// Now lock the surface so we can read back the converted color
	ddsd.dwSize = sizeof(ddsd);
	hr = m_pddsRenderTarget->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
	if( hr == DD_OK)
	{
		dw = *(DWORD *) ddsd.lpSurface;
		if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 ) // Mask it to bpp
			dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;
		m_pddsRenderTarget->Unlock(NULL);
	}

	//  Now put the color that was there back.
	if( dwGDIColor != CLR_INVALID && m_pddsRenderTarget->GetDC(&hdc) == DD_OK )
	{
		SetPixel( hdc, 0, 0, rgbT );
		m_pddsRenderTarget->ReleaseDC(hdc);
	}

	return dw;
}

//-----------------------------------------------------------------------
// Load3DResources()
// Desc : lolo와 IndexedTexture는 다시 로드할 필요는 없지만.. 그래도 한다.
//-----------------------------------------------------------------------
HRESULT CItemRender::Load3DResources()
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
void CItemRender::DeleteRes()
{
	SAFE_RELEASE( m_pddsZBuffer );
	SAFE_RELEASE(m_pddsRenderTarget);
	SAFE_RELEASE(m_pddsBlt);
}

void CItemRender::BeginRenderLolo()
{
	D3DMATERIAL7 mtrl;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;

	DWORD dwPass = 0;
	HRESULT hr = GET_D3DDEVICE()->ValidateDevice(&dwPass);
	if( hr != S_OK )
	{
		char chkValid[MAX_PATH] = {0,};
		sprintf(chkValid, "DEVICE VALID ERROR : %X\n", hr);
		OutputDebugString(chkValid);
		Sleep(20);

		return;
	}

	// 현재의 렌더 타겟을 가져온다.
	GET_D3DDEVICE()->SetRenderTarget(m_pddsRenderTarget, 0);

	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&m_vpOld);
	m_vpNew = m_vpOld;
	m_vpNew.dwWidth = ITEM_RENDER_WIDTH;
	m_vpNew.dwHeight = ITEM_RENDER_HEIGHT;
	GET_D3DDEVICE()->SetViewport(&m_vpNew);

	// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) m_vpNew.dwHeight / (double) m_vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &m_matProjOld);
	D3DUtil_SetProjectionMatrix(m_matProjNew, 0.6f, fAspect, 1.0f, 500.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProjNew);

	vEyePt = D3DVECTOR(0.1f, 33, 0);
	vLookatPt = D3DVECTOR(0, 0, 0);
	vUpVec = D3DVECTOR(0, 1, 0);

	D3DUtil_SetViewMatrix(m_matView,  vEyePt, vLookatPt, vUpVec );

	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorldOld );
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_VIEW, &m_matViewOld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &m_matView );

	// 알파 설정...
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &m_dwAlphaBlend );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &m_dwAlphaTest );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE );

	// 조명 설정...
	GET_D3DDEVICE()->GetLight(0, &m_lightOld);
	GET_D3DDEVICE()->SetLight(0, &m_d3dLight);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &m_dwColor );
	GET_D3DDEVICE()->GetLightEnable(0, &m_lightEnable0);
	GET_D3DDEVICE()->GetLightEnable(1, &m_lightEnable1);
	GET_D3DDEVICE()->GetLightEnable(2, &m_lightEnable2);
	GET_D3DDEVICE()->LightEnable(0, TRUE);
	GET_D3DDEVICE()->LightEnable(1, FALSE);
	GET_D3DDEVICE()->LightEnable(2, FALSE);
}

void CItemRender::EndRenderLolo()
{
	DWORD dwPass = 0;
	HRESULT hr = GET_D3DDEVICE()->ValidateDevice(&dwPass);
	if( hr != S_OK )
	{
		char chkValid[MAX_PATH] = {0,};
		sprintf(chkValid, "DEVICE VALID ERROR : %X\n", hr);
		OutputDebugString(chkValid);
		Sleep(20);

		return;
	}

	// 원상복귀.
	m_lpOldRenderTarget = pCMyApp->GetRenderTarget();
	GET_D3DDEVICE()->SetRenderTarget(m_lpOldRenderTarget, 0);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, m_dwColor);
	GET_D3DDEVICE()->SetViewport(&m_vpOld);
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &m_matProjOld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorldOld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_VIEW, &m_matViewOld );
	GET_D3DDEVICE()->SetLight(0, &m_lightOld);
	GET_D3DDEVICE()->LightEnable(0, m_lightEnable0);
	GET_D3DDEVICE()->LightEnable(1, m_lightEnable1);
	GET_D3DDEVICE()->LightEnable(2, m_lightEnable2);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, m_dwAlphaTest );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, m_dwAlphaBlend );
}

void CItemRender::RenderLolo(int x, int y, CLolos *pLolo, int TextureNum, DWORD dwAmbient, BOOL bMove, int width, int height, BOOL bBeginEnd, int sizeX, int sizeY, BOOL bIgronreSize, float fScale)
{
	if (!pLolo)
		return;

	D3DMATRIX matWorld, matWorldOld;

	if (bBeginEnd == FALSE)
		BeginRenderLolo();

	// 월드 트랜스폼
	D3DUtil_SetIdentityMatrix(matWorld);

	float fScaleVal = 1.0f;
	if( width > 0 && height > 0 && bIgronreSize == FALSE )
	{
		int iScaleVal = width > height ? width : height;
		fScaleVal = 1.0f / (float)iScaleVal;
		D3DUtil_SetScaleMatrix(matWorld, fScaleVal, fScaleVal, fScaleVal);

		fScaleVal = (fScaleVal - 1.0f) * 2;
		if( fScaleVal < 1.0f )
		{
			fScaleVal = 1.0f;
		}
	}

	if( fScale != 1.0f )
	{
		matWorld._11 *= fScale;
		matWorld._22 *= fScale;
		matWorld._33 *= fScale;
	}

	matWorld._43 = -0.2f;
	if (bMove)
	{
		DWORD curTime = timeGetTime();
		if (curTime > m_lastTime + 100)
		{
			m_lastTime = curTime;
			m_Frame++;
			if (m_Frame >= 10)
				m_Frame = 0;
		}
		if (m_Frame < 5)
			matWorld._42 = (9-m_Frame) / 0.8f / fScaleVal;
		else
			matWorld._42 = m_Frame / 0.8f / fScaleVal;
	}
	else
	{
		matWorld._42 = 10.0f;
	}

	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwAmbient);

	GET_D3DDEVICE()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ITEM_KEY_COLOR, 1.0f, 0L );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
	// Begin the scene
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		static int aniFrame = 0;
		aniFrame++;
		pLolo->Render2(GET_D3DDEVICE(),TextureNum); ///롤로텍스쳐변경
		// End the scene.
		GET_D3DDEVICE()->EndScene();
	}
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	if (bBeginEnd == FALSE)
		EndRenderLolo();

	// 렌더링된 캐릭터를 목적지에 그린다.
	// 수정이 필요하다.. 음냐..
	if( bIgronreSize == FALSE )
	{
		if( x >=0 && y >= 0 &&
				x+SLOT_WIDTH*1 <= GET_CLIENT_WIDTH() &&
				y+SLOT_HEIGHT*1 <= GET_CLIENT_HEIGHT() )
		{
			RECT rcSrc;

			rcSrc.left = (ITEM_RENDER_WIDTH - (SLOT_WIDTH*1)) / 2;
			rcSrc.top = (ITEM_RENDER_HEIGHT - (SLOT_HEIGHT*1)) / 2;
			rcSrc.right = rcSrc.left+(SLOT_WIDTH*1);
			rcSrc.bottom = rcSrc.top+(SLOT_HEIGHT*1);
			m_pddsBlt->BltFast(rcSrc.left, rcSrc.top, m_pddsRenderTarget, &rcSrc, 0);
			g_pDisplay->GetBackBuffer()->BltFast(x, y, m_pddsBlt, &rcSrc, DDBLTFAST_SRCCOLORKEY);
		}
		else
		{
			m_pddsBlt->BltFast(0, 0, m_pddsRenderTarget, NULL, 0);
			g_pDisplay->Blt(x, y, m_pddsBlt, NULL, DDBLTFAST_SRCCOLORKEY);
		}
	}
	else
	{
		RECT rcSrc;
		rcSrc.left = 0;
		rcSrc.top = 0;
		rcSrc.right = ITEM_RENDER_WIDTH;
		rcSrc.bottom = ITEM_RENDER_HEIGHT;

		if (x < 0)
		{
			rcSrc.left -= x;
			x = 0;
		}
		else if (x + ITEM_RENDER_WIDTH > GET_CLIENT_WIDTH())
		{
			rcSrc.right -= (x+ITEM_RENDER_WIDTH) - GET_CLIENT_WIDTH();
		}

		if (y < 0)
		{
			rcSrc.top -= y;
			y = 0;
		}
		else if (y + ITEM_RENDER_HEIGHT > GET_CLIENT_HEIGHT())
		{
			rcSrc.bottom -= (y+ITEM_RENDER_HEIGHT) - GET_CLIENT_HEIGHT();
		}

		m_pddsBlt->BltFast(0, 0, m_pddsRenderTarget, NULL, 0);
		if( sizeX > -1 || sizeY > -1 )
		{
			g_pDisplay->Blt(x, y, x+sizeX, y+sizeY, m_pddsBlt, &rcSrc, DDBLT_KEYSRC);
		}
		else
		{
			g_pDisplay->Blt(x, y, m_pddsBlt, &rcSrc, DDBLTFAST_SRCCOLORKEY);
		}
	}
}

void CItemRender::MakeLight()
{
	ZeroMemory( &m_d3dLight, sizeof(D3DLIGHT7) );
	m_d3dLight.dltType        = D3DLIGHT_DIRECTIONAL;
	m_d3dLight.dcvDiffuse.r   = 1.0f;
	m_d3dLight.dcvDiffuse.g   = 1.0f;
	m_d3dLight.dcvDiffuse.b   = 1.0f;
	m_d3dLight.dcvSpecular    = m_d3dLight.dcvDiffuse;
	m_d3dLight.dvDirection = Normalize(D3DVECTOR(0, -1000, 0));
	m_d3dLight.dvAttenuation0 = 1.0f;
	m_d3dLight.dvRange        = D3DLIGHT_RANGE_MAX;
}

bool CItemRender::CheckRestore()
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

HRESULT	CItemRender::Restore()
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