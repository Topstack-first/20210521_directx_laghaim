#include "stdafx.h"

#define D3D_OVERLOADS

#include <assert.h>
#include <d3d.h>
#include <ddraw.h>
#include <time.h>
#include "FxSet.h"
#include "ddutil.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "sfxUtil.h"
#include "GUtil.h"
#include "SFXBaseShape.h"
#include "main.h"
#include "D3DMath.h"
#include "D3DUtil.h"
#include "IndexedTexture.h"
#include "ImageDefine.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "Skill.h"
#include "UIMgr.h"
#include "land.h"
#include "Tcpipcon.h"
#include "TcpUtil.h"
#include "ShadowRect.h"
#include "NKObjQuadNode.h"

#include "FxMuramasa.h"








CFxMuramasaGhost::CFxMuramasaGhost() ///�Ϻ����������׽�Ʈ ������ ���󸶻� �� �ͽ� ����Ʈ.
{
	m_pMainObject = NULL;
	m_pEffect1 = NULL;
	m_pLolo1 = NULL;
}

CFxMuramasaGhost::~CFxMuramasaGhost()
{
	DeleteRes();

}

void CFxMuramasaGhost::DeleteRes()
{
	SAFE_DELETE( m_pLolo1 );
	SAFE_DELETE( m_pEffect1 );
	SAFE_DELETE( m_pMainObject );
}

void CFxMuramasaGhost::LoadRes()
{
	DeleteRes();

	m_pMainObject = new CRectPlane;
	if( m_pMainObject )
		m_pMainObject->Create( 5.0f, 5.0f, FALSE ); // ũ�⸦ ���Ƿ� �����س��´�.(�� ����Ʈ���� �������� ũ�⸦ �������ش�.)

	m_pEffect1 = new CRectPlane;
	if( m_pEffect1 )
		m_pEffect1->Create( 5.0f, 5.0f, FALSE ); // ũ�⸦ ���Ƿ� �����س��´�.(�� ����Ʈ���� �������� ũ�⸦ �������ش�.)


	PFILE *fpLolo = NULL;

	if (!m_pLolo1)
	{
		m_pLolo1 = new CLolos;
		g_pCapsyongTexture->SetTexturePath("data/effect/textr/");
		m_pLolo1->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo1->Read("data/effect/lolos/muramasa_ghost.LOL");
	}


}

void CFxMuramasaGhost::FrameMove( EffectSort &effect_sort, int nAddFrame ) // ���� �������� ��������.
{
	DWORD dwElapsedTick = timeGetTime() - effect_sort.dwColor; // ����� ƽ
	D3DVECTOR vMoveDir;  // �̵��� ����.
	float fTargetDir; // ����� �ٶ󺸰� �ִ� ����.
	int TargetHeight; // ��� ����.

	D3DVECTOR vTempVec;
	D3DVECTOR vTempVec2;
	D3DVECTOR vTempVec3;
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DMATRIX matRotate;
	float speed = 0.0f;

	// ��� ����.
	if( effect_sort.pNkChaTo )
	{
		vTempVec = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz );
		fTargetDir = effect_sort.pNkChaTo->m_dir;
		if( effect_sort.pNkChaTo->m_Race == RACE_AIDIA ) // ���̵�ƴ�...
			TargetHeight = MOB_HEIGHT_LOW_NUM;
		else
			TargetHeight = MOB_HEIGHT_MIDDLE_VAL;
	}
	else if( effect_sort.pMobTo )
	{
		vTempVec = D3DVECTOR( effect_sort.pMobTo->m_wx, effect_sort.pMobTo->m_wy, effect_sort.pMobTo->m_wz );
		fTargetDir = effect_sort.pMobTo->m_dir;
		TargetHeight = effect_sort.pMobTo->m_Height;
	}
	/* // �׾� ������ ���� �ƿ� �������� ����Ʈ�� �����ع�������. ��, �� �κ� �ǹ̰� ����.
		else // Ȥ�� �׾� �������� ��쵵 ����.
		{
			vTempVec = effect_sort.vCurPos; // ���� �ִ� ��ġ
			fTargetDir = effect_sort.vSortPos.x; // ���� ���� �״��.
			TargetHeight = MOB_HEIGHT_MIDDLE_VAL; // �߰� ����� ��������.

			// �ٷ� ������ ������� ����
			effect_sort.num = 2;
			effect_sort.dwColor = timeGetTime();

			effect_sort.vSortPos.x = (float)atan2( vTempVec.z - effect_sort.vCurPos.z, vTempVec.x - effect_sort.vCurPos.x ); // �̶� ���� ��ȯ.
			effect_sort.vTargetPos = D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
			effect_sort.vSortPos.y = Magnitude( effect_sort.vTargetPos ); // Ÿ�ٱ����� ���� �Ÿ��� ������.
			effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

		}
	*/

	switch( effect_sort.Index ) // effect_sort �� Index �� Ÿ������ ����Ѵ�.
	{
	case 0: // ����� �� Ÿ�Թۿ� ����.

		if( !effect_sort.pNkChaFrom ) // ���༮�� �ݵ�� �־���Ѵ�.
			return;

		if( !effect_sort.pNkChaTo && !effect_sort.pMobTo ) // �´� �༮�� �ݵ�� �־���Ѵ�.
			return;

		// ��ġ���.
		vTempVec2 = D3DVECTOR( 1.0f, 0.0f, 0.0f );
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, fTargetDir);
		D3DMath_VectorMatrixMultiply( vTempVec2, vTempVec2, matRotate ); // ����� �ٶ󺸴� ���� ����.
		vTempVec2 = Normalize( vTempVec2 );

		switch( effect_sort.num )
		{
		case 0: // ó�� ���� ��.

			effect_sort.vSortPos.x = fTargetDir; // ���Ⱚ���� ���. ó���� ����� �ڿ��� �����Ƿ� ���� ��������.

			switch( TargetHeight )
			{
			case MOB_HEIGHT_HIGH_NUM:
				vTempVec3 = vTempVec - (vTempVec2*15.0f);
				vTempVec3.y += MOB_HEIGHT_MIDDLE_VAL;
				break;
			case MOB_HEIGHT_LOW_NUM:
				vTempVec3 = vTempVec - (vTempVec2*8.0f);
				vTempVec3.y -= MOB_HEIGHT_LOW_VAL;
				break;
			default: // MOB_HEIGHT_MIDDLE_NUM:
				vTempVec3 = vTempVec - (vTempVec2*10.0f);
				vTempVec3.y += MOB_HEIGHT_LOW_VAL;
				break;
			}

			effect_sort.vCurPos = vTempVec3;

			if( dwElapsedTick > 500 ) // ���� ��ȭ
			{
				effect_sort.num = 1;
				effect_sort.dwColor = timeGetTime();

				effect_sort.vSortPos.y = RandomNum( 2.0f, 4.0f ); // ��. (�ּ����� �Ÿ��� �ش�.)
				effect_sort.vSortPos.z = 0.0f; // ���� ���η� ���� ����. �ʱ�ȭ.

				effect_sort.vTargetPos = D3DVECTOR(vTempVec.x+RandomNum(-15.0f,15.0f),0.0f,vTempVec.z+RandomNum(-15.0f,15.0f)) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
				effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

			}

			break;

		case 1: // �ֺ��� ����.

			// �̵������� �׻� Ÿ���� ���ϴ� ��������...
			effect_sort.vCurPos += (effect_sort.vTargetPos*nAddFrame*effect_sort.vSortPos.y*0.5f);

			effect_sort.vCurPos.y += sinf(dwElapsedTick*0.002f) * ((float)nAddFrame * 0.7f); // ���Ʒ� ��鸮��.

			effect_sort.vSortPos.x = (float)atan2( vTempVec.z - effect_sort.vCurPos.z, vTempVec.x - effect_sort.vCurPos.x ); // �̶� ���� ��ȯ.

			effect_sort.vSortPos.z += sinf(dwElapsedTick*0.002f) * ((float)nAddFrame * 0.03f); // ���� ����.

			// �� ����.
			effect_sort.vSortPos.y -= nAddFrame * 0.1f;
			if( effect_sort.vSortPos.y <= 0.0f )
			{

				effect_sort.vSortPos.y = RandomNum( 2.0f, 4.0f ); // ��. (�ּ����� �Ÿ��� �ش�.)

				effect_sort.vTargetPos = D3DVECTOR(vTempVec.x+RandomNum(-2.0f,2.0f),0.0f,vTempVec.z+RandomNum(-2.0f,2.0f)) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
				effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

				effect_sort.subNum++; // �Դٰ����� Ƚ��.
			}

			if( effect_sort.subNum > 2 && (rand()%100)>90 ) // �ּ��� 2���̻�. �����ϰ� �Դٰ����ϵ���...
			{
				effect_sort.num = 2;
				effect_sort.dwColor = timeGetTime();

				effect_sort.vSortPos.x = (float)atan2( vTempVec.z - effect_sort.vCurPos.z, vTempVec.x - effect_sort.vCurPos.x ); // �̶� ���� ��ȯ.
				effect_sort.vTargetPos = D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
				effect_sort.vSortPos.y = Magnitude( effect_sort.vTargetPos ); // Ÿ�ٱ����� ���� �Ÿ��� ������.
				effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );
			}

			break;

		case 2: // ������.
			// �׳� õõ�� �ٰ����� ���Ȱ�
			effect_sort.vTargetPos = D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
			effect_sort.vSortPos.y = Magnitude( effect_sort.vTargetPos ); // Ÿ�ٱ����� ���� �Ÿ��� ������.
			effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

			if( effect_sort.vSortPos.y > 5.0f ) // ���� ������������ �̵�.
			{
				effect_sort.vCurPos += (effect_sort.vTargetPos*nAddFrame*effect_sort.vSortPos.y*0.5f);
				effect_sort.vSortPos.y -= nAddFrame * 0.1f;
			}

			break;

		case 3: // ���� ���
			break;
		}

		break;
	}
}

BOOL CFxMuramasaGhost::Render( EffectSort &effect_sort )
{
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DVECTOR vViewAt = pCMyApp->GetEyeAtPos();
	D3DVECTOR vViewFrom;
	pCMyApp->GetEyePos( vViewFrom );
	float fViewDist = Magnitude( vViewFrom-vViewAt );

	DWORD dwLighting,dwAlphaBlend , dwSrc, dwDest, dwZWrite;

	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_ZWRITEENABLE, &dwZWrite );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_LIGHTING, &dwLighting );
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &dwAlphaBlend);
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrc );
	GET_D3DDEVICE()->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDest );

	DWORD  dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );

	float rectwidth = 0.0f; // vSortPos�� �����ϰ� �ִ°� ������ �� x���� ũ�Ⱚ���� ����Ѵ�.

	int TotalFrame = 0; // ���� �ִϸ��̼��� �� �������� �޾ƿ��� ���� ����
	float tu = 0.0f, tv = 0.0f;
	float FrameWidthRatio = 1.0f;
	int NowFrame = 0;
	DWORD dwColor;
	DWORD dwAlpha;
	float fScaleX = 0.7f, fScaleY = 0.7f, fScaleZ = 0.7f; // ������ �⺻ġ.


	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matRotate2;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	DWORD dwElapsedTick = timeGetTime() - effect_sort.dwColor; // ����� ƽ

	BOOL bEndFlag = FALSE;

	switch( effect_sort.Index ) // effect_sort �� Index �� Ÿ������ ����Ѵ�.
	{
	case 0: // ����� �� Ÿ�Թۿ� ����.

		if( !m_pLolo1 )
		{
			bEndFlag = TRUE;
			break;
		}

		if( !effect_sort.pNkChaFrom ) // ���༮�� �ݵ�� �־���Ѵ�.
		{
			bEndFlag = TRUE;
			break;
		}

		if( !effect_sort.pNkChaTo && !effect_sort.pMobTo ) // �´� �༮�� �ݵ�� �־���Ѵ�.
		{
			bEndFlag = TRUE;
			break;
		}


		// �ֺ� ����Ʈ ���
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
//		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVDESTCOLOR ); // �ణ�� ���� ����
//		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTCOLOR ); // �� ���� ����.

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		rectwidth = 9.0f;
		dwColor = 0xffffffff;

		rectwidth = dwElapsedTick * 0.015f;

		dwColor = RGBA_MAKE(255,255,255,255);
		NowFrame = 0;

		switch( effect_sort.num ) // ���� ��
		{
		case 0: // ó�� ���� ��.

			fScaleX += RandomNum(-0.1f,0.1f);
			fScaleY += RandomNum(-0.3f,0.2f);
			fScaleZ += RandomNum(-0.1f,0.1f);

			dwAlpha = (int)(dwElapsedTick*0.1f)%255;
			dwColor = RGBA_MAKE(255,255,255,dwAlpha);

			break;

		case 1: // �ֺ��� ����.

			NowFrame = 205 - ( sinf(dwElapsedTick*0.01f) * (50.0f) ); // �ӽ÷� �̳༮�� ���� ��꿡 �̿�.
			dwColor = RGBA_MAKE(255,255,255,NowFrame);
			NowFrame = 0;

			break;

		case 2: // Ȯ ������.

			// ������ ���.
			NowFrame = 3; // �պκ� �ణ ©������.
			NowFrame += (float)dwElapsedTick * 0.02f;

			if( NowFrame >= m_pLolo1->m_AnimCount-4 )
			{
				NowFrame = m_pLolo1->m_AnimCount-4-1;
				effect_sort.num = 3;
				effect_sort.dwColor = timeGetTime();
			}

			break;

		case 3: // ���� ���

			NowFrame = m_pLolo1->m_AnimCount-4-1;

			fScaleX -= (float)dwElapsedTick * 0.001f;
			fScaleY += (float)dwElapsedTick * 0.01f;
			fScaleZ -= (float)dwElapsedTick * 0.001f;
			dwAlpha = 255-(int)((float)dwElapsedTick*0.001f);
			dwColor = RGBA_MAKE(255,255,255,dwAlpha);


			if( dwElapsedTick > 700 ) // �����ð� ������ �������.
			{
				bEndFlag = TRUE;
				break;
			}

			break;
		}

		// ��� ����.
		D3DUtil_SetIdentityMatrix(matWorld);

		// �¿�� ���ǵ��� ó��.
		D3DUtil_SetRotationMatrix( matRotate2, D3DVECTOR(effect_sort.vCurPos.x,effect_sort.vCurPos.y+10.0f,effect_sort.vCurPos.z), effect_sort.vSortPos.z );
		D3DMath_MatrixMultiply( matWorld, matWorld, matRotate2 );

		// �ٸ� ��� ���
		D3DUtil_SetScaleMatrix( matScale, fScaleX, fScaleY, fScaleZ );
		Nk3DUtil_SetRotateCCWYMatrix( matRotate, effect_sort.vSortPos.x );
		D3DUtil_SetTranslateMatrix( matTrans, effect_sort.vCurPos );

		D3DMath_MatrixMultiply( matWorld, matWorld, matScale );
		D3DMath_MatrixMultiply( matWorld, matWorld, matRotate );
		D3DMath_MatrixMultiply( matWorld, matWorld, matTrans );

		GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );
		m_pLolo1->Render( GET_D3DDEVICE(), NowFrame, FALSE, FALSE, FALSE, NULL, NULL, FALSE, TRUE, dwColor );

		break;
	}

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, dwZWrite );

	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, dwLighting );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, dwAlphaBlend);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrc );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDest );

	return bEndFlag;
}
