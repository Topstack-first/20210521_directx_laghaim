#include "stdafx.h"

#define D3D_OVERLOADS

#include "ShadowRect.h"
#include "Land.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "D3dUtil.h"
#include "Main.h"
#include "UIMgr.h"

CShadowRect * g_pShadowRect = NULL;

// 외부 전역변수
extern int		gMapWidth;							// 맵의 x축 넓이
extern int		gMapHeight;
extern int		gHFWidth;
extern int		gHFHeight;
extern int		gMapExtention;					// 스케일 팩터 - 정수형
extern float	gfMapExtention;				// 스케일 팩터 - 실수형


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadowRect::CShadowRect()
	: m_tileIdx(0)
	, m_ShadowXSize(0)
	, m_ShadowZSize(0)
	, m_Sign(0)
{
	m_pLand = NULL;

	for (int i = 0; i < 9; i++)
	{
		m_TileIndex[i*6+0] = i*4+0;
		m_TileIndex[i*6+1] = i*4+1;
		m_TileIndex[i*6+2] = i*4+2;
		m_TileIndex[i*6+3] = i*4+2;
		m_TileIndex[i*6+4] = i*4+3;
		m_TileIndex[i*6+5] = i*4+0;
	}

	for (int i = 0; i < 36; i++)
	{
		m_TileRectVert[i].dcColor = 0xffffffff;
		m_TileRectVert[i].dcSpecular = 1;
	}
	m_TileNum = 0;
}

CShadowRect::~CShadowRect()
{
	m_pLand = NULL;
}

// Desc : 지형에 대한 정보를 알기 위해서 필요한 클래스 포인터 연결
void CShadowRect::Init(CLand *pLand )
{
	m_pLand = pLand;
}

void CShadowRect::SetTileCoord(float wx, float wy, float wz, int size_or_hour, BOOL bIsHour)
{
	D3DVECTOR vecWorld;
	vecWorld = D3DVECTOR(wx, wy, wz);
	SetTileCoord(vecWorld, size_or_hour, bIsHour);
}

#define SHADOW_MIN_SIZE		40
#define SHADOW_MAX_SIZE		90

void CShadowRect::SetTileCoord(D3DVECTOR &vecWorld, int size_or_hour, BOOL bIsHour)
{
	m_vSelSpot = vecWorld;
	m_tileIdx = m_pLand->GetMapIndex(m_vSelSpot);

	if (bIsHour)
	{
		m_ShadowZSize = SHADOW_MIN_SIZE;
		if (size_or_hour < 5 || size_or_hour > 24)
			m_ShadowXSize = SHADOW_MIN_SIZE;
		else if (size_or_hour < 15)
			m_ShadowXSize = 90 - (50 * (size_or_hour-5) / 10);
		else
			m_ShadowXSize = 40 + (50 * (size_or_hour-15) / 9);

		if (size_or_hour < 15)
			m_Sign = 1;
		else
			m_Sign = -1;
	}
	else
	{
		m_ShadowXSize = size_or_hour;
		m_ShadowZSize = size_or_hour;
		m_Sign = 1;
	}

	SetupTileRect();
}


HRESULT CShadowRect::Render(LPDIRECTDRAWSURFACE7 lpDDsurface, BOOL bReal)
{
	HRESULT res;
	D3DMATRIX matWorldOld;
	DWORD lightingOld, alphaEnableOld, srcBlendOld, destBlendOld, tssAddressOld, alphaTest, zwrite;

	if (!lpDDsurface)
	{
		if (g_EffectTextr[ EFF_SHADOWRECT ] == -1)
			lpDDsurface = NULL;
		else
			lpDDsurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_SHADOWRECT ] );
	}

	res = GET_D3DDEVICE()->SetTexture(0, lpDDsurface);

	D3DUtil_SetIdentityMatrix(m_matWorld);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_WORLD, &matWorldOld );
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &m_matWorld );
	// 알파테스팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	// 라이팅 끄고...
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_LIGHTING, &lightingOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
	// 알파블렌딩
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &alphaEnableOld);
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlendOld); // Test 용...
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlendOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	// z-func
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &zwrite);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);

	if (bReal)
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ZERO);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCCOLOR);
	}
	else
	{
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// 여기서 부터 스테이지 설정
	GET_D3DDEVICE()->GetTextureStageState(0 ,D3DTSS_ADDRESSU, &tssAddressOld);
	GET_D3DDEVICE()->GetTextureStageState(0 ,D3DTSS_ADDRESSV, &tssAddressOld);
	GET_D3DDEVICE()->SetTextureStageState(0 ,D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	GET_D3DDEVICE()->SetTextureStageState(0 ,D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

	res = GET_D3DDEVICE()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_LVERTEX,
			m_TileRectVert, m_TileNum*4,
			m_TileIndex, m_TileNum*6, 0);

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, lightingOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, alphaEnableOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, zwrite);
	GET_D3DDEVICE()->SetTextureStageState(0 ,D3DTSS_ADDRESSU, tssAddressOld);
	GET_D3DDEVICE()->SetTextureStageState(0 ,D3DTSS_ADDRESSV, tssAddressOld);
	GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorldOld );

	return res;
}


void CShadowRect::SetupTileRect()
{
	int tx, tz;
	float wx, wz, tu, tv;

	// 넘겨온 좌표 부분 그리기.
	wx = m_vSelSpot.x;
	wz = m_vSelSpot.z;

	// 텍스처 좌표 넣기
	// tv 값 계산시 맵 좌표는 위로 증가 이고 택스처 좌표는 아래로 증가이다.
	tx = ((int) wx) % gMapExtention;
	tz = ((int) wz) % gMapExtention;

	tu = ((float)tz) / m_ShadowZSize;				// 맵 좌표를 타일 한칸의 폭으로 나누어 몫을 구한다.
	tv = ((float)tx) / m_ShadowXSize;

	// z값의 나머지가 클수록 타일의 윗쪽으로 이동하는 것이지만 텍스처 좌표는 아래쪽으로 이동한다.
	// 따라서 값을 보정해 주어야 한다.

	// 현재까지의 tu와 tv는 텍스처 상단 위치. 원점 이동을 통한 보정.
	tu = 0.5f - (m_Sign * tu);
	tv = 0.5f - (m_Sign * tv);

	float tile_shadow_x_num = m_Sign * (gfMapExtention/m_ShadowXSize);
	float tile_shadow_z_num = m_Sign * (gfMapExtention/m_ShadowZSize);

	float x = (float) (m_tileIdx % gMapWidth);
	float z = (float) (m_tileIdx / gMapWidth);

	m_TileRectVert[0].tu = tu;
	m_TileRectVert[0].tv = tv;
	m_TileRectVert[0].x = x;
	m_TileRectVert[0].z = z;

	m_TileNum = 1;
	if (tz < m_ShadowZSize/2 && z > 0)
	{
		// 하
		m_TileRectVert[m_TileNum*4].tu = tu - tile_shadow_z_num;
		m_TileRectVert[m_TileNum*4].tv = tv;
		m_TileRectVert[m_TileNum*4].x = x;
		m_TileRectVert[m_TileNum*4].z = z-1;
		m_TileNum++;

		if (tx < m_ShadowXSize/2 && x > 0)
		{
			// 좌하
			m_TileRectVert[m_TileNum*4].tu = tu - tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv - tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x-1;
			m_TileRectVert[m_TileNum*4].z = z-1;
			m_TileNum++;
		}
		if (tx > gMapExtention - (m_ShadowXSize/2) && x < gMapWidth-1)
		{
			// 우하
			m_TileRectVert[m_TileNum*4].tu = tu - tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv + tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x+1;
			m_TileRectVert[m_TileNum*4].z = z-1;
			m_TileNum++;
		}
	}

	if (tx < m_ShadowXSize/2 && x > 0)
	{
		// 좌
		m_TileRectVert[m_TileNum*4].tu = tu;
		m_TileRectVert[m_TileNum*4].tv = tv - tile_shadow_x_num;
		m_TileRectVert[m_TileNum*4].x = x-1;
		m_TileRectVert[m_TileNum*4].z = z;
		m_TileNum++;
	}
	if (tx > gMapExtention - (m_ShadowXSize/2) && x < gMapWidth-1)
	{
		// 우
		m_TileRectVert[m_TileNum*4].tu = tu;
		m_TileRectVert[m_TileNum*4].tv = tv + tile_shadow_x_num;
		m_TileRectVert[m_TileNum*4].x = x+1;
		m_TileRectVert[m_TileNum*4].z = z;
		m_TileNum++;
	}

	if (tz > gMapExtention - (m_ShadowZSize/2) && z < gMapHeight-1)
	{
		// 상
		m_TileRectVert[m_TileNum*4].tu = tu + tile_shadow_z_num;
		m_TileRectVert[m_TileNum*4].tv = tv;
		m_TileRectVert[m_TileNum*4].x = x;
		m_TileRectVert[m_TileNum*4].z = z+1;
		m_TileNum++;

		if (tx < m_ShadowXSize/2 && x > 0)
		{
			// 좌상
			m_TileRectVert[m_TileNum*4].tu = tu + tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv - tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x-1;
			m_TileRectVert[m_TileNum*4].z = z+1;
			m_TileNum++;
		}
		if (tx > gMapExtention - (m_ShadowXSize/2) &&  x < gMapWidth-1)
		{
			// 우상
			m_TileRectVert[m_TileNum*4].tu = tu + tile_shadow_z_num;
			m_TileRectVert[m_TileNum*4].tv = tv + tile_shadow_x_num;
			m_TileRectVert[m_TileNum*4].x = x+1;
			m_TileRectVert[m_TileNum*4].z = z+1;
			m_TileNum++;
		}
	}

	// 기본 정보를 사용하여 vertex들을 만든다.
	for (int i = 0; i < m_TileNum; i++)
	{
		int x = (int) m_TileRectVert[i*4].x;
		int z = (int) m_TileRectVert[i*4].z;

		if (pCMyApp->m_nCurWorldIdx == WORLD_SKY && !m_pLand->IsDrawTile(x * gfMapExtention, z * gfMapExtention))
		{
			m_TileRectVert[i*4+0].x = 0.0f;
			m_TileRectVert[i*4+0].y = 0.0f;
			m_TileRectVert[i*4+0].z = 0.0f;
			m_TileRectVert[i*4+1].x = 0.0f;
			m_TileRectVert[i*4+1].y = 0.0f;
			m_TileRectVert[i*4+1].z = 0.0f;
			m_TileRectVert[i*4+2].x = 0.0f;
			m_TileRectVert[i*4+2].y = 0.0f;
			m_TileRectVert[i*4+2].z = 0.0f;
			m_TileRectVert[i*4+3].x = 0.0f;
			m_TileRectVert[i*4+3].y = 0.0f;
			m_TileRectVert[i*4+3].z = 0.0f;
		}
		else
		{
			m_TileRectVert[i*4].x = x * gfMapExtention;
			m_TileRectVert[i*4].y = (float) m_pLand->GetHF(x, z) + 0.1f;
			m_TileRectVert[i*4].z = z * gfMapExtention;

			m_TileRectVert[i*4+1].tu = m_TileRectVert[i*4].tu + tile_shadow_z_num;
			m_TileRectVert[i*4+1].tv = m_TileRectVert[i*4].tv;
			m_TileRectVert[i*4+1].x = x * gfMapExtention;
			m_TileRectVert[i*4+1].y = (float) m_pLand->GetHF(x, z+1) + 0.1f;
			m_TileRectVert[i*4+1].z = (z+1) * gfMapExtention;

			m_TileRectVert[i*4+2].tu = m_TileRectVert[i*4].tu + tile_shadow_z_num;
			m_TileRectVert[i*4+2].tv = m_TileRectVert[i*4].tv + tile_shadow_x_num;
			m_TileRectVert[i*4+2].x = (x+1) * gfMapExtention;
			m_TileRectVert[i*4+2].y = (float) m_pLand->GetHF(x+1, z+1) + 0.1f;
			m_TileRectVert[i*4+2].z = (z+1) * gfMapExtention;

			m_TileRectVert[i*4+3].tu = m_TileRectVert[i*4].tu;
			m_TileRectVert[i*4+3].tv = m_TileRectVert[i*4].tv + tile_shadow_x_num;
			m_TileRectVert[i*4+3].x = (x+1) * gfMapExtention;
			m_TileRectVert[i*4+3].y = (float) m_pLand->GetHF(x+1, z) + 0.1f;
			m_TileRectVert[i*4+3].z = z * gfMapExtention;
		}
	}
}