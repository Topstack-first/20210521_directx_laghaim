#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include <time.h>
#include <assert.h>
#include "FxSet.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "sfxUtil.h"
#include "SFXBaseShape.h"
#include "IndexedTexture.h"
#include "ShadowRect.h"
#include "WeatherMgr.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "ImageDefine.h"
#include "CapSyong.h"
#include "GUtil.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Land.h"
//#include "FxPebbleShot.h"
#include "FX_BOSSRAID_LastBoss_02.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFxBossRaidLastBoss_02::CFxBossRaidLastBoss_02()
{
	m_Plane = 0;
	m_Floor = 0;

	// 바위.
	m_lverRect[0] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.0f, 1.0f );
	m_lverRect[1] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.0f, 0.0f );
	m_lverRect[2] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[3] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );

	m_lverRect[4] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.25f, 1.0f );
	m_lverRect[5] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.25f, 0.0f );
	m_lverRect[6] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[7] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );

	m_lverRect[8] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.5f, 1.0f );
	m_lverRect[9] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.5f, 0.0f );
	m_lverRect[10] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[11] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );

	m_lverRect[12] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 0.75f, 1.0f );
	m_lverRect[13] = D3DLVERTEX( D3DVECTOR(-FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 0.75f, 0.0f );
	m_lverRect[14] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, 0, 0), 0xffffffff, 0, 1.0f, 1.0f );
	m_lverRect[15] = D3DLVERTEX( D3DVECTOR( FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH, FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT, 0), 0xffffffff, 0, 1.0f, 0.0f );

	float fWidth, fHeight;
	// 바위에 나는 연기.
	fWidth = FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH + 1.0f;
	fHeight = FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT + 1.0f;

	m_lverRectSmoke[0] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffd29f, 0, 0.0f, 1.0f );
	m_lverRectSmoke[1] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffd29f, 0, 0.0f, 0.0f );
	m_lverRectSmoke[2] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffd29f, 0, 0.25f, 1.0f );
	m_lverRectSmoke[3] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffd29f, 0, 0.25f, 0.0f );

	fWidth = FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH - 0.0f;
	fHeight = FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT - 0.0f;
	m_lverRectSmoke[4] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffd29f, 0, 0.25f, 1.0f );
	m_lverRectSmoke[5] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffd29f, 0, 0.25f, 0.0f );
	m_lverRectSmoke[6] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffd29f, 0, 0.5f, 1.0f );
	m_lverRectSmoke[7] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffd29f, 0, 0.5f, 0.0f );

	fWidth = FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH - 1.0f;
	fHeight = FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT - 1.0f;
	m_lverRectSmoke[8] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffd29f, 0, 0.5f, 1.0f );
	m_lverRectSmoke[9] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffd29f, 0, 0.5f, 0.0f );
	m_lverRectSmoke[10] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffd29f, 0, 0.75f, 1.0f );
	m_lverRectSmoke[11] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffd29f, 0, 0.75f, 0.0f );

	fWidth = FX_BOSSRAID_LASTBOSS_02_STONE_WIDTH - 2.0f;
	fHeight = FX_BOSSRAID_LASTBOSS_02_STONE_HEIGHT - 2.0f;
	m_lverRectSmoke[12] = D3DLVERTEX( D3DVECTOR(-fWidth, 0, 0), 0xffffd29f, 0, 0.75f, 1.0f );
	m_lverRectSmoke[13] = D3DLVERTEX( D3DVECTOR(-fWidth, fHeight, 0), 0xffffd29f, 0, 0.75f, 0.0f );
	m_lverRectSmoke[14] = D3DLVERTEX( D3DVECTOR( fWidth, 0, 0), 0xffffd29f, 0, 1.0f, 1.0f );
	m_lverRectSmoke[15] = D3DLVERTEX( D3DVECTOR( fWidth, fHeight, 0), 0xffffd29f, 0, 1.0f, 0.0f );

	m_Texcode[0].tu = 0.0f;
	m_Texcode[0].tv = 0.0f;
	m_Texcode[1].tu = 0.25f;
	m_Texcode[1].tv = 0.0f;
	m_Texcode[2].tu = 0.5f;
	m_Texcode[2].tv = 0.0f;
	m_Texcode[3].tu = 0.75f;
	m_Texcode[3].tv = 0.0f;

	m_Texcode[4].tu = 0.0f;
	m_Texcode[4].tv = 0.25f;
	m_Texcode[5].tu = 0.25f;
	m_Texcode[5].tv = 0.25f;
	m_Texcode[6].tu = 0.5f;
	m_Texcode[6].tv = 0.25f;
	m_Texcode[7].tu = 0.75f;
	m_Texcode[7].tv = 0.25f;

	m_Texcode[8].tu = 0.0f;
	m_Texcode[8].tv = 0.5f;
	m_Texcode[9].tu = 0.25f;
	m_Texcode[9].tv = 0.5f;
	m_Texcode[10].tu = 0.5f;
	m_Texcode[10].tv = 0.5f;
	m_Texcode[11].tu = 0.75f;
	m_Texcode[11].tv = 0.5f;

	m_nFrameIdx[0] = 0;
	m_nFrameIdx[1] = 1;
	m_nFrameIdx[2] = 1;
	m_nFrameIdx[3] = 2;
	m_nFrameIdx[4] = 3;

	m_nFrameIdx[5] = 4;
	m_nFrameIdx[6] = 5;
	m_nFrameIdx[7] = 5;
	m_nFrameIdx[8] = 4;
	m_nFrameIdx[9] = 4;

	m_nFrameIdx[10] = 5;
	m_nFrameIdx[11] = 6;
	m_nFrameIdx[12] = 7;
	m_nFrameIdx[13] = 7;

	m_nFrameIdx[14] = 8;

	m_nFrameIdx[15] = 8;
	m_nFrameIdx[16] = 9;
	m_nFrameIdx[17] = 9;
	m_nFrameIdx[18] = 11;
	m_nFrameIdx[19] = 11;

	MakeLVertexRect(m_lverBoom, FX_BOSSRAID_LASTBOSS_02_SIZE, FX_BOSSRAID_LASTBOSS_02_SIZE, TRUE);
}


CFxBossRaidLastBoss_02::~CFxBossRaidLastBoss_02()
{
	DeleteRes();
}

void CFxBossRaidLastBoss_02::LoadRes()
{
	m_Plane = new CRectPlane;
	if( m_Plane )
		m_Plane->Create( 2.0f, 2.0f, FALSE );

	m_Floor = NULL;
	m_Floor = new CRectPlane;
	if( m_Floor )
		m_Floor->Create(15.0f, 15.0f, FALSE);
}

void CFxBossRaidLastBoss_02::DeleteRes()
{
	SAFE_DELETE( m_Plane );
	SAFE_DELETE( m_Floor );
}

void CFxBossRaidLastBoss_02::RenderSmoke(D3DMATRIX matDecal,
		D3DVECTOR vDrawPos, int nFrame)
{
	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld;

	float scale = powf(1.07f, nFrame);
	float alpha = powf(0.80f, nFrame);

	DWORD dwLighting, dwSrc, dwDest;
	DWORD dwTextureState;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING,	&dwLighting);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND,   &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND,  &dwDest );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING,	FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState);
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	///////////////////////////////////////////////////////////////////////
	// 폭파 그리기.

	matDecal._11 *= 2 * scale;
	matDecal._22 *= 2 * scale;
	matDecal._33 *= 2 * scale;

	matDecal._41 = vDrawPos.x;
	matDecal._42 = vDrawPos.y + FX_BOSSRAID_LASTBOSS_02_HEIGHT;
	matDecal._43 = vDrawPos.z;

	m_lverBoom[0].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
	m_lverBoom[0].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
	m_lverBoom[1].tu = m_Texcode[m_nFrameIdx[nFrame]].tu;
	m_lverBoom[1].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;
	m_lverBoom[2].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
	m_lverBoom[2].tv = m_Texcode[m_nFrameIdx[nFrame]].tv+0.25f;
	m_lverBoom[3].tu = m_Texcode[m_nFrameIdx[nFrame]].tu+0.25f;
	m_lverBoom[3].tv = m_Texcode[m_nFrameIdx[nFrame]].tv;

	m_lverBoom[0].color = D3DRGBA(1.00, 0.76, 0.65, alpha);
	m_lverBoom[1].color = D3DRGBA(1.00, 0.76, 0.65, alpha);
	m_lverBoom[2].color = D3DRGBA(1.00, 0.76, 0.65, alpha);
	m_lverBoom[3].color = D3DRGBA(1.00, 0.76, 0.65, alpha);

	// nate 2004 - 7 - image
	lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTBOOM ] );
	GET_D3DDEVICE()->SetTexture(0, lpSurface);

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matDecal );

	GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_lverBoom, 4, 0 );

	//상태 복원
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState);
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState);

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  dwDest );
}

int CFxBossRaidLastBoss_02::ActiveStone(int nFrame)
{
	int startFrame[FX_BOSSRAID_LASTBOSS_02_MAX_PARTICLE] =
	{
		54, 54, 54, 54, 59,
		59, 59, 63, 63, 63,
		68, 68, 68, 68, 72,
	};

	int idx = 0;
	int sum = 0;
	for(; idx < FX_BOSSRAID_LASTBOSS_02_MAX_PARTICLE; ++idx)
	{
		if ( nFrame < startFrame[idx])
			return sum;
		else
			++sum;
	}
	return sum;
}

bool CFxBossRaidLastBoss_02::Render(EffectSort &effect_sort)
{
	if( !GET_D3DDEVICE() )
		return true;

	if(!effect_sort.pNkChaTo && !effect_sort.pMobTo)
		return true;

	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	D3DVECTOR vViewVec;
	D3DMATRIX matRx, matRy, matWorld, matDecal;
	unsigned int offset;
	int i;

	vViewVec = pCMyApp->GetEyeDirection();

	float ry = atan2f(vViewVec.x, vViewVec.z);
	float rx = atan2f(vViewVec.x*sinf(ry)+vViewVec.z*cosf(ry), vViewVec.y) - (g_PI / 2.0f) ;

	D3DUtil_SetRotateXMatrix(matRx, rx);
	D3DUtil_SetRotateYMatrix(matRy, ry);
	D3DMath_MatrixMultiply(matWorld, matRx, matRy);
	matDecal = matWorld;

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_SRCALPHA );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	D3DVECTOR Eye;
	pCMyApp->GetEyePos( Eye );
	D3DVECTOR At;
	pCMyApp->GetEyeAtPos( At );

	if( effect_sort.nCurFrame >= 50 && effect_sort.nCurFrame < 54 )
	{
		float delta = effect_sort.nCurFrame - 49;
		float theta = delta * g_PI * 0.025f;
		float power = 16 / sqrt(delta);
		Eye.y += power * sin(theta);
		pCMyApp->SetCameraInitPos(Eye, At);
	}
	else if(effect_sort.nCurFrame >= 54 && effect_sort.nCurFrame < 68)
	{
		float delta = effect_sort.nCurFrame - 53;
		float theta = delta * g_PI * 0.50f;
		float power = 8 / sqrt(delta);
		Eye.y += power * sin(theta);
		pCMyApp->SetCameraInitPos(Eye, At);
	}
	else if(effect_sort.nCurFrame >= 69 && effect_sort.nCurFrame < 92)
	{
		float delta = effect_sort.nCurFrame - 68;
		float theta = delta * g_PI * 0.50f;
		float power = 2 / sqrt(delta);
		Eye.y += power * sin(theta);
		pCMyApp->SetCameraInitPos(Eye, At);
	}

	//바닥 이펙트
	if( effect_sort.nCurFrame >= 54 && effect_sort.nCurFrame < 120 )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = NULL;

		D3DMATRIX matRotate;
		D3DVECTOR vDrawPos(70, 0, 0);
		D3DUtil_SetRotateYMatrix(matRotate, -effect_sort.pMobFrom->m_dir);
		D3DMath_VectorMatrixMultiply( vDrawPos, vDrawPos, matRotate );
		vDrawPos += effect_sort.vCurPos;

		if( g_pShadowRect )
		{
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_STONERANCE2 ] );
			g_pShadowRect->SetTileCoord(vDrawPos, 120, FALSE);		// 부울값은 시간인가 아닌가
			g_pShadowRect->Render(lpSurface, FALSE);	// 부울값은 리얼그림자인가 아닌가?
		}
	}

	if( effect_sort.nCurFrame >= 60 && effect_sort.nCurFrame < 90 )
	{
		int nFrame = effect_sort.nCurFrame - 60;

		Deco_Particle* deco_particle = effect_sort.pParticle;

		int nObject = 0;

		int nActiveStone = ActiveStone(effect_sort.nCurFrame);
		for( nObject = 0; nObject < nActiveStone; ++nObject)
		{
			if (nFrame == 0) offset = 0;
			else offset = nFrame % 4;

			// 방향벡터를 구한다.
			D3DVECTOR vDir, vPosFrom, vPosTo;
			D3DVECTOR vDrawPos;

			// 메테오 마법은 시작위치가 고정. 시전 객체가 움직일 수도 있기 때문에.
			vPosFrom = effect_sort.vCurPos + D3DVECTOR(0, 120, 0) + deco_particle [nObject].cur_Pos;

			// 타겟 좌표를 구한다.
			//vPosTo = effect_sort.vTargetPos;
			vPosTo = effect_sort.vCurPos + deco_particle [nObject].cur_Pos;

			get_normal_vector(vDir, vPosFrom, vPosTo);

			float m_fSpeed = 8.0f;

			// 프레임당 스피드.
			vDir *= m_fSpeed;

			// 현재 프레임에서 8프레임 전까지 그린다.
			int start_frame, end_frame;

			start_frame = nFrame - 4;		// 총 8프레임.
			if (start_frame < 0)
				start_frame = 0;
			end_frame = nFrame;

			// 텍스처 택하기.
			// nate 2004 - 7 - image
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTHEAD ] );
			GET_D3DDEVICE()->SetTexture(0, lpSurface);

			// 마지막 프레임 전까지 그림을 그린다.
			// 여기서 start_frame은 화이어볼의 꼬리쪽.
			int nTex = 0;
			int nFrameNum = 0;

			// nate 2004 - 7 - image
			lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTTAIL ] );
			GET_D3DDEVICE()->SetTexture(0, lpSurface);

			for (i= end_frame - 1; i >= start_frame; i--)
			{
				vDrawPos.x = vPosFrom.x + vDir.x * (i+1);
				vDrawPos.y = vPosFrom.y + vDir.y * (i+1);
				vDrawPos.z = vPosFrom.z + vDir.z * (i+1);

				matWorld._41 = vDrawPos.x;
				matWorld._42 = vDrawPos.y + FX_BOSSRAID_LASTBOSS_02_HEIGHT;
				matWorld._43 = vDrawPos.z;

				GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRectSmoke[nFrameNum*4], 4, 0 );
				nFrameNum++;
				if (nFrameNum == 4) nFrameNum = 0;
			}

			// 마지막 프레임이 목적지에 도착했는가 확인? - 거리를 측정해서 한다.
			vDrawPos.x = vPosFrom.x + vDir.x * (end_frame+1);
			vDrawPos.y = vPosFrom.y + vDir.y * (end_frame+1);
			vDrawPos.z = vPosFrom.z + vDir.z * (end_frame+1);

			// 3차원 공간상에서 거리를 재야 한다.
			float fDist = Dist3PTo3P(vPosTo.x, vPosTo.y, vPosTo.z, vPosFrom.x, vPosFrom.y, vPosFrom.z) - \
						  Dist3PTo3P(vDrawPos.x, vDrawPos.y, vDrawPos.z, vPosFrom.x, vPosFrom.y, vPosFrom.z);

			if ( fDist <= 0.0f || fabsf(fDist) <= 10.0f)
			{
				vDrawPos = vPosTo;

				matWorld._41 = vDrawPos.x;
				matWorld._42 = vDrawPos.y;
				matWorld._43 = vDrawPos.z;

				GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

				// 연기 부분.
				// nate 2004 - 7 - image
				lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTTAIL ] );
				GET_D3DDEVICE()->SetTexture(0, lpSurface);
				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRectSmoke[offset*4], 4, 0 );

				// 돌부분.
				// nate 2004 - 7 - image
				lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTHEAD ] );
				GET_D3DDEVICE()->SetTexture(0, lpSurface);
				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRect[offset*4], 4, 0 );

				// 연기 그리기
				if (deco_particle[nObject].cur_time == -1) //최초로 그리면
				{
					RenderSmoke( matDecal, vDrawPos, 0);
					deco_particle[nObject].cur_time = 0;
				}
				else if(deco_particle[nObject].cur_time >= 0 &&
						deco_particle[nObject].cur_time < 20)
				{
					RenderSmoke( matDecal, vDrawPos, deco_particle[nObject].cur_time++);
				}
			}
			else
			{
				matWorld._41 = vDrawPos.x;
				matWorld._42 = vDrawPos.y;
				matWorld._43 = vDrawPos.z;

				GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

				// 연기 부분.
				// nate 2004 - 7 - image
				lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTTAIL ] );
				GET_D3DDEVICE()->SetTexture(0, lpSurface);
				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRectSmoke[offset*4], 4, 0 );

				// 돌 부분.
				// nate 2004 - 7 - image
				lpSurface = g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_PEBBLESHOTHEAD ] );
				GET_D3DDEVICE()->SetTexture(0, lpSurface);
				GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &m_lverRect[offset*4], 4, 0 );
			}
		}
	}
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);

	return FALSE;
}