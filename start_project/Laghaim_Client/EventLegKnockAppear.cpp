#include "stdafx.h"

#define D3D_OVERLOADS
#include <d3d.h>
#include <assert.h>
#include "main.h"
#include "Frustum.h"
#include "Nk2DFrame.h"
#include "NkMob.h"
#include "Capsyong.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "DirtSnd.h"
#include "Land.h"
#include "ClrAlloc.h"
#include "ImageDefine.h"
#include "LoadEffectSound.h"

#include "EventLegKnockAppear.h"


extern IndexedTexture *g_pMobTexture;

extern float g_fDSDistance;



CEventLegKnockAppear::CEventLegKnockAppear()
	: m_PosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_dir(0)
	, m_fCamDist(0)
	, m_fCamEyeHeight(0)
	, m_fCamAtHeight(0)
{
	m_pCapSyong = NULL;
	m_pBiped = NULL;
	m_pFadeEffect = NULL;
	m_pNameOutput = NULL;
}

CEventLegKnockAppear::~CEventLegKnockAppear()
{
	DeleteRes(); // Ȥ�� �𸣴� �ѹ��� Ȯ��.(UiMgr ���� EndEventScene() ������ �ѹ� ȣ����.)
}

void CEventLegKnockAppear::Init( D3DVECTOR vOldEye, D3DVECTOR vOldAt ) // ȣ��ɶ� ���� �ʱ�ȭ�ؾ��ϴ� ������ �͵�.
{
	m_vOldEye = vOldEye; // ������ ī�޶� ����ϱ� ���� ���.
	m_vOldAt = vOldAt;

	m_vEye.x = m_PosX + ( cos(m_dir) * 55.0f ); // ������ ���� �߻������� ���� �ݴ�� ������.
	m_vEye.y = m_PosY + 30.0f; // ���� ���̿��� ������ ��.
	m_vEye.z = m_PosZ + ( sin(m_dir) * 55.0f ); // ������ ���� �߻������� ���� �ݴ�� ������.
	m_vAt.x = m_PosX; // ������ ���� �߻������� ���� �ݴ�� ������.
	m_vAt.y = m_PosY + 30.0f; // ���� ���̿��� ������ ��.
	m_vAt.z = m_PosZ; // ������ ���� �߻������� ���� �ݴ�� ������.

	pCMyApp->SetCameraInitPos( m_vEye, m_vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.
}

void CEventLegKnockAppear::LoadRes()
{

	m_pCapSyong = new CCapsyong(g_pMobTexture);
	if( VF_ERR_NONE > m_pCapSyong->Read( "./data/Npc/csy/legknock.CSY" ) )
	{
		SAFE_DELETE(m_pCapSyong);
		return;
	}

	m_pBiped = new CBiped;

	if( VF_ERR_NONE > m_pBiped->Read("./data/Npc/bpd/legknock_jump00.BPD") )
	{
		SAFE_DELETE(m_pBiped);
		return;
	}


	m_pFadeEffect = new CRectPlane;
	if( m_pFadeEffect )
		m_pFadeEffect->Create( 1024.0f, 1024.0f, FALSE ); // ȭ���� �� ���� �� �ִ� ũ��.

	m_pNameOutput = new CRectPlane;
	if( m_pNameOutput )
		m_pNameOutput->Create( 100.0f, 100.0f, FALSE );
}

void CEventLegKnockAppear::DeleteRes()
{
	SAFE_DELETE( m_pNameOutput );
	SAFE_DELETE( m_pFadeEffect );
	SAFE_DELETE( m_pCapSyong );
	SAFE_DELETE( m_pBiped );
}

BOOL CEventLegKnockAppear::Render( DWORD dwEventStartTime )
{
	static int PrevFrame = 0;

	D3DMATRIX matTrans, matRotate, matWorld;
	DWORD alphaTest, alphaBlend, srcBlendOld, destBlendOld, dwLighting, dwAmbient, dwFogEnable;

	DWORD dwNowTick = timeGetTime();
	DWORD dwElapsedTick = dwNowTick - dwEventStartTime; // �̺�Ʈ�� ���۵��� ����� ƽ.(�� �������� �ƴ�.)
	DWORD nCurFrame = (dwElapsedTick-1000) * 20 / 1000; // 1�ʸ� 20���������� ����ؼ� ���� �� ������������ ����Ѵ�. (���� ���̵� �κ��� �����ϰ� ���)

	// ī�޶� �ܿ� ���� �þ߰� ���� �޶����� ������ �������� ��Ʈ������ �÷����ҵ� �ʱ�ȭ���ش�.
	D3DMATRIX matProj;

	float Fov = 0.75f;
	float Aspect = 0.75f;
	float zNear = 0.1f;
	float zFar = 10000.0f;
	D3DUtil_SetProjectionMatrix( matProj, Fov, Aspect, zNear, zFar );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj);
	pCMyApp->GetFrustum()->InitFrustum( Fov, Aspect, zNear, 10000.0f );


	// ���°� ����.
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_FOGENABLE, &dwFogEnable );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &srcBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &destBlendOld );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwAmbient);

	if( dwElapsedTick < 1000 ) // ���̵� ��
	{
		D3DTLVERTEX TLVec[4];
		DWORD Alpha = (float)dwElapsedTick * 0.3f;

		if( Alpha > 255 )
			Alpha = 255;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(1024,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(1024,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);

		GET_D3DDEVICE()->SetTexture(0, NULL );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}
	else if( dwElapsedTick < 13000 ) // ���׳� ���� ��.
	{
		if( nCurFrame >= 220 )
			nCurFrame = 219; // �Ѿ�� ���� ����.


		// ī�޶� ����.
		float fDist = 55.0f; // �Ÿ� ������.
		float fEyeHeight = 30.0f; // ���� ������.
		float fAtHeight = 30.0f; // ���� ������.

		if( nCurFrame > 20 ) // �־�����.
		{
			int nTemp = nCurFrame-20;

			if( nTemp > 50 )
				nTemp = 50;

			fDist += (float)(nTemp)*0.5f;
		}

		if( nCurFrame > 110 && nCurFrame <= 120 ) // ī�޶� ������ ���缭�ö󰡰�.
		{
			int nTemp = nCurFrame-110;

			fDist += (float)(nTemp*10);
			fEyeHeight += (float)(nTemp*4);
			fAtHeight += (float)(nTemp*4);

			// ȿ���� ���.
			if( PrevFrame <= 110 && nCurFrame > 110 )
			{
				if (g_pDSound)
				{
					// ������ �Ѹ� ���ǵǸ� �ȴ�.
					float x, y, z;
					x = 0.0f; // ȭ���� ���߹Ƿ� ��ǥ�� ��������..
					y = 0.0f;
					z = 0.0f;

					g_pDSound->Play(g_EffectSound[EFF_SND_STAR_FLARE], x, y, z);
				}
			}
		}
		else if( nCurFrame > 120 && nCurFrame <= 125 ) // ī�޶� ������ ���缭 ��������.
		{
			int nTemp = 125- nCurFrame;

			fEyeHeight += (float)(nTemp*6);
			fAtHeight += (float)(nTemp*6);
		}
		else if( nCurFrame > 125 && nCurFrame <= 138 ) // ���ϰ� ��鸮�� ī�޶� ��ũ
		{
			int nTemp = (rand()%10)-5; // �����ϰ�..

			fEyeHeight += (float)nTemp; // ���̸� ����
			fAtHeight += (float)nTemp;
		}

		if( nCurFrame > 140 )
		{
			int nTemp = nCurFrame-140;

			if( nTemp > 25 )
				nTemp = 25;

			fDist -= (float)(nTemp)*0.5f;
			fEyeHeight -= (float)(nTemp)*1.5f; // Eye�� ���߰�
			fAtHeight += (float)(nTemp)*0.8f; // At�� �ø���.

			// ȿ���� ���.
			if( PrevFrame <= 170 && nCurFrame > 170 )
			{
				if (g_pDSound)
				{
					// ������ �Ѹ� ���ǵǸ� �ȴ�.
					float x, y, z;
					x = 0.0f; // ȭ���� ���߹Ƿ� ��ǥ�� ��������..
					y = 0.0f;
					z = 0.0f;

					g_pDSound->Play(g_EffectSound[EFF_SND_FIRE_PILLAR], x, y, z);
				}
			}
		}

		// ī�޶� ����.
		m_vEye.x = m_PosX + ( cos(m_dir) * fDist ); // ������ ���� �߻������� ���� �ݴ�� ������.
		m_vEye.y = m_PosY + fEyeHeight; // ���� ���̿��� ������ ��.
		m_vEye.z = m_PosZ + ( sin(m_dir) * fDist ); // ������ ���� �߻������� ���� �ݴ�� ������.
		m_vAt.x = m_PosX; // ������ ���� �߻������� ���� �ݴ�� ������.
		m_vAt.y = m_PosY + fAtHeight; // ���� ���̿��� ������ ��.
		m_vAt.z = m_PosZ; // ������ ���� �߻������� ���� �ݴ�� ������.

		pCMyApp->SetCameraInitPos( m_vEye, m_vAt ); // ���� �����ϱ� �����Ƽ� pCMyApp �� ���� ���Ĺ���. ���� ������ ����ص� ���� ī�޶� ��ǥ�� �ٽ� �ٲ�����Ѵ�.


		if( m_pCapSyong && m_pBiped )
		{
			// ���� ��ġ ����
			D3DUtil_SetTranslateMatrix( matTrans, m_PosX, m_PosY, m_PosZ);
			Nk3DUtil_SetRotateCCWYMatrix( matRotate, m_dir);
			D3DMath_MatrixMultiply( matWorld, matRotate, matTrans );
			GET_D3DDEVICE()->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld );

			m_pCapSyong->Animate( m_pBiped, nCurFrame );
			m_pCapSyong->Render( GET_D3DDEVICE() );
		}



		// �����ӿ� ���缭 �� Ƣ��� ����Ʈ �߰�
		if( PrevFrame < 1 && nCurFrame >= 1 )
		{
			// �� Ƣ��� ����Ʈ �߰�.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX + (cos(m_dir-g_PI_DIV_2)*13.0f), m_PosY, m_PosZ + (sin(m_dir-g_PI_DIV_2)*13.0f) );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			if (g_pDSound)
			{
				// ������ �Ѹ� ���ǵǸ� �ȴ�.
				float x, y, z;
				x = 0.0f; // ȭ���� ���߹Ƿ� ��ǥ�� ��������..
				y = 0.0f;
				z = 0.0f;

				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if( PrevFrame < 28 && nCurFrame >= 28 )
		{
			// �� Ƣ��� ����Ʈ �߰�.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX, m_PosY, m_PosZ );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			if (g_pDSound)
			{
				// ������ �Ѹ� ���ǵǸ� �ȴ�.
				float x, y, z;
				x = 0.0f; // ȭ���� ���߹Ƿ� ��ǥ�� ��������..
				y = 0.0f;
				z = 0.0f;

				g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
			}
		}
		if( PrevFrame < 64 && nCurFrame >= 64 )
		{
			// �� Ƣ��� ����Ʈ �߰�.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX + (cos(m_dir+g_PI_DIV_2)*13.0f), m_PosY, m_PosZ + (sin(m_dir+g_PI_DIV_2)*13.0f) );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			/*
						if (g_pDSound)
						{
							// ������ �Ѹ� ���ǵǸ� �ȴ�.
							float x, y, z;
							x = 0.0f; // ȭ���� ���߹Ƿ� ��ǥ�� ��������..
							y = 0.0f;
							z = 0.0f;

							g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
						}
			*/
		}
		if( PrevFrame < 110 && nCurFrame >= 110 )
		{
			// �� Ƣ��� ����Ʈ �߰�.
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_STONERANCE;
			ef_sort.Index = 2;

			ef_sort.vPos = D3DVECTOR( m_PosX, m_PosY, m_PosZ );
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			/*
						if (g_pDSound)
						{
							// ������ �Ѹ� ���ǵǸ� �ȴ�.
							float x, y, z;
							x = 0.0f; // ȭ���� ���߹Ƿ� ��ǥ�� ��������..
							y = 0.0f;
							z = 0.0f;

							g_pDSound->Play(g_EffectSound[EFF_SND_MISSLEBOMB], x, y, z);
						}
			*/
		}
	}

	if( dwElapsedTick > 8000 ) // �̸� ���
	{
		D3DTLVERTEX TLVec[4];
		DWORD Alpha = 255;

		float ScreenCenterX = (float)(g_pNk2DFrame->GetClientWidth() / 2);
		float PosY = 100.0f;
//		ScreenCenterX = 150;
		float rectwidth = 120.0f;
		float rectheight = 0.0f;

		rectheight = ( dwElapsedTick-8000 ) * 0.07f;

		if( rectheight > 100.0f )
			rectheight = 100.0f;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX-rectwidth,PosY+rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX-rectwidth,PosY-rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX+rectwidth,PosY-rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(ScreenCenterX+rectwidth,PosY+rectheight,0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

		GET_D3DDEVICE()->SetTexture(0, g_pCapsyongTexture->GetSurfaceByIdx( g_EffectTextr[ EFF_FX_LEGKNOCK_NAME ] ) );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}

	if( dwElapsedTick > 12000 ) // ���̵� �ƿ� �κ�. (else if �� �ƴ� ������ ���� ��µǴ� �߿� ���̵� �ƿ��� �Ǿ��ϱ� ����.)
	{
		D3DTLVERTEX TLVec[4];
		DWORD Alpha = 255 - (float)(dwElapsedTick-12000) * 0.3f;

		if( Alpha > 255 )
			Alpha = 0;

		TLVec[0] = D3DTLVERTEX( D3DVECTOR(0,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX( D3DVECTOR(0,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX( D3DVECTOR(1024,0,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX( D3DVECTOR(1024,1024,0), 1, RGBA_MAKE(0,0,0,Alpha), 0, 1.0f, 1.0f );

		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCCOLOR);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);

		GET_D3DDEVICE()->SetTexture(0, NULL );
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_FOGENABLE, dwFogEnable );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, srcBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, destBlendOld);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_AMBIENT, dwAmbient);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );


	PrevFrame = nCurFrame;

	if( dwElapsedTick >= 13000 ) // �����ʰ� �������� �����Ѵ�. // �տ� ���̵� 1��, �ڿ� ����1��, ���̵� 1�ʰ� �߰��� ��ġ.
	{
		pCMyApp->SetCameraInitPos( m_vOldEye, m_vOldAt ); // ���� �����ϱ� ���� ī�޶� ��ǥ�� �����ش�.
		pCMyApp->UpdateProjectMatrix(); // �������� ��Ʈ������ �÷����� ������ �ǵ帮�� ������ �̰� ������� ���ƿ´�.

		return TRUE;
	}

	return FALSE; // ���� ������ �ʾҴ�.
}