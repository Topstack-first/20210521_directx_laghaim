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








CFxMuramasaGhost::CFxMuramasaGhost() ///일본디자인컨테스트 아이템 무라마사 의 귀신 이펙트.
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
		m_pMainObject->Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)

	m_pEffect1 = new CRectPlane;
	if( m_pEffect1 )
		m_pEffect1->Create( 5.0f, 5.0f, FALSE ); // 크기를 임의로 생성해놓는다.(각 이펙트마다 렌더에서 크기를 조정해준다.)


	PFILE *fpLolo = NULL;

	if (!m_pLolo1)
	{
		m_pLolo1 = new CLolos;
		g_pCapsyongTexture->SetTexturePath("data/effect/textr/");
		m_pLolo1->SetIndexedTexture( g_pCapsyongTexture );
		m_pLolo1->Read("data/effect/lolos/muramasa_ghost.LOL");
	}


}

void CFxMuramasaGhost::FrameMove( EffectSort &effect_sort, int nAddFrame ) // 페어리의 움직임을 구현하자.
{
	DWORD dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 진행된 틱
	D3DVECTOR vMoveDir;  // 이동할 방향.
	float fTargetDir; // 대상이 바라보고 있는 방향.
	int TargetHeight; // 대상 높이.

	D3DVECTOR vTempVec;
	D3DVECTOR vTempVec2;
	D3DVECTOR vTempVec3;
	D3DVECTOR vViewVec = pCMyApp->GetEyeDirection();
	D3DMATRIX matRotate;
	float speed = 0.0f;

	// 기반 정보.
	if( effect_sort.pNkChaTo )
	{
		vTempVec = D3DVECTOR( effect_sort.pNkChaTo->m_wx, effect_sort.pNkChaTo->m_wy, effect_sort.pNkChaTo->m_wz );
		fTargetDir = effect_sort.pNkChaTo->m_dir;
		if( effect_sort.pNkChaTo->m_Race == RACE_AIDIA ) // 에이디아는...
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
	/* // 죽어 없어진 경우는 아예 없어질때 이펙트를 제거해버리더라. 즉, 이 부분 의미가 없다.
		else // 혹시 죽어 없어졌을 경우도 포함.
		{
			vTempVec = effect_sort.vCurPos; // 현재 있던 위치
			fTargetDir = effect_sort.vSortPos.x; // 현재 방향 그대로.
			TargetHeight = MOB_HEIGHT_MIDDLE_VAL; // 중간 사이즈를 기준으로.

			// 바로 마지막 페이즈로 이행
			effect_sort.num = 2;
			effect_sort.dwColor = timeGetTime();

			effect_sort.vSortPos.x = (float)atan2( vTempVec.z - effect_sort.vCurPos.z, vTempVec.x - effect_sort.vCurPos.x ); // 이때 방향 전환.
			effect_sort.vTargetPos = D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
			effect_sort.vSortPos.y = Magnitude( effect_sort.vTargetPos ); // 타겟까지의 남은 거리를 힘으로.
			effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

		}
	*/

	switch( effect_sort.Index ) // effect_sort 의 Index 을 타입으로 사용한다.
	{
	case 0: // 현재는 이 타입밖에 없다.

		if( !effect_sort.pNkChaFrom ) // 쓴녀석은 반드시 있어야한다.
			return;

		if( !effect_sort.pNkChaTo && !effect_sort.pMobTo ) // 맞는 녀석도 반드시 있어야한다.
			return;

		// 위치계산.
		vTempVec2 = D3DVECTOR( 1.0f, 0.0f, 0.0f );
		Nk3DUtil_SetRotateCCWYMatrix(matRotate, fTargetDir);
		D3DMath_VectorMatrixMultiply( vTempVec2, vTempVec2, matRotate ); // 대상이 바라보는 방향 벡터.
		vTempVec2 = Normalize( vTempVec2 );

		switch( effect_sort.num )
		{
		case 0: // 처음 등장 중.

			effect_sort.vSortPos.x = fTargetDir; // 방향값으로 사용. 처음엔 대상의 뒤에서 나오므로 같은 방향으로.

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

			if( dwElapsedTick > 500 ) // 상태 변화
			{
				effect_sort.num = 1;
				effect_sort.dwColor = timeGetTime();

				effect_sort.vSortPos.y = RandomNum( 2.0f, 4.0f ); // 힘. (최소한의 거리를 준다.)
				effect_sort.vSortPos.z = 0.0f; // 몸통 가로로 흔드는 각도. 초기화.

				effect_sort.vTargetPos = D3DVECTOR(vTempVec.x+RandomNum(-15.0f,15.0f),0.0f,vTempVec.z+RandomNum(-15.0f,15.0f)) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
				effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

			}

			break;

		case 1: // 주변을 휙휙.

			// 이동방향은 항상 타겟을 향하는 방향으로...
			effect_sort.vCurPos += (effect_sort.vTargetPos*nAddFrame*effect_sort.vSortPos.y*0.5f);

			effect_sort.vCurPos.y += sinf(dwElapsedTick*0.002f) * ((float)nAddFrame * 0.7f); // 위아래 흔들리게.

			effect_sort.vSortPos.x = (float)atan2( vTempVec.z - effect_sort.vCurPos.z, vTempVec.x - effect_sort.vCurPos.x ); // 이때 방향 전환.

			effect_sort.vSortPos.z += sinf(dwElapsedTick*0.002f) * ((float)nAddFrame * 0.03f); // 몸통 흔들게.

			// 힘 감소.
			effect_sort.vSortPos.y -= nAddFrame * 0.1f;
			if( effect_sort.vSortPos.y <= 0.0f )
			{

				effect_sort.vSortPos.y = RandomNum( 2.0f, 4.0f ); // 힘. (최소한의 거리를 준다.)

				effect_sort.vTargetPos = D3DVECTOR(vTempVec.x+RandomNum(-2.0f,2.0f),0.0f,vTempVec.z+RandomNum(-2.0f,2.0f)) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
				effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

				effect_sort.subNum++; // 왔다갔다한 횟수.
			}

			if( effect_sort.subNum > 2 && (rand()%100)>90 ) // 최소한 2번이상. 랜덤하게 왔다갔다하도록...
			{
				effect_sort.num = 2;
				effect_sort.dwColor = timeGetTime();

				effect_sort.vSortPos.x = (float)atan2( vTempVec.z - effect_sort.vCurPos.z, vTempVec.x - effect_sort.vCurPos.x ); // 이때 방향 전환.
				effect_sort.vTargetPos = D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
				effect_sort.vSortPos.y = Magnitude( effect_sort.vTargetPos ); // 타겟까지의 남은 거리를 힘으로.
				effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );
			}

			break;

		case 2: // 껴안음.
			// 그냥 천천히 다가가서 껴안게
			effect_sort.vTargetPos = D3DVECTOR(vTempVec.x,0.0f,vTempVec.z) - D3DVECTOR(effect_sort.vCurPos.x,0.0f,effect_sort.vCurPos.z);
			effect_sort.vSortPos.y = Magnitude( effect_sort.vTargetPos ); // 타겟까지의 남은 거리를 힘으로.
			effect_sort.vTargetPos = Normalize( effect_sort.vTargetPos );

			if( effect_sort.vSortPos.y > 5.0f ) // 힘이 남아있을때만 이동.
			{
				effect_sort.vCurPos += (effect_sort.vTargetPos*nAddFrame*effect_sort.vSortPos.y*0.5f);
				effect_sort.vSortPos.y -= nAddFrame * 0.1f;
			}

			break;

		case 3: // 제거 모션
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

	float rectwidth = 0.0f; // vSortPos를 사용안하고 있는것 같으니 그 x값을 크기값으로 사용한다.

	int TotalFrame = 0; // 현재 애니메이션의 총 프레임을 받아오기 위한 변수
	float tu = 0.0f, tv = 0.0f;
	float FrameWidthRatio = 1.0f;
	int NowFrame = 0;
	DWORD dwColor;
	DWORD dwAlpha;
	float fScaleX = 0.7f, fScaleY = 0.7f, fScaleZ = 0.7f; // 스케일 기본치.


	D3DMATRIX matTrans;
	D3DMATRIX matRotate;
	D3DMATRIX matRotate2;
	D3DMATRIX matScale;
	D3DMATRIX matWorld;

	DWORD dwElapsedTick = timeGetTime() - effect_sort.dwColor; // 진행된 틱

	BOOL bEndFlag = FALSE;

	switch( effect_sort.Index ) // effect_sort 의 Index 을 타입으로 사용한다.
	{
	case 0: // 현재는 이 타입밖에 없다.

		if( !m_pLolo1 )
		{
			bEndFlag = TRUE;
			break;
		}

		if( !effect_sort.pNkChaFrom ) // 쓴녀석은 반드시 있어야한다.
		{
			bEndFlag = TRUE;
			break;
		}

		if( !effect_sort.pNkChaTo && !effect_sort.pMobTo ) // 맞는 녀석도 반드시 있어야한다.
		{
			bEndFlag = TRUE;
			break;
		}


		// 주변 이펙트 출력
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
//		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVDESTCOLOR ); // 약간더 진한 버젼
//		GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_DESTCOLOR ); // 더 진한 버젼.

		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

		rectwidth = 9.0f;
		dwColor = 0xffffffff;

		rectwidth = dwElapsedTick * 0.015f;

		dwColor = RGBA_MAKE(255,255,255,255);
		NowFrame = 0;

		switch( effect_sort.num ) // 등장 씬
		{
		case 0: // 처음 등장 중.

			fScaleX += RandomNum(-0.1f,0.1f);
			fScaleY += RandomNum(-0.3f,0.2f);
			fScaleZ += RandomNum(-0.1f,0.1f);

			dwAlpha = (int)(dwElapsedTick*0.1f)%255;
			dwColor = RGBA_MAKE(255,255,255,dwAlpha);

			break;

		case 1: // 주변을 휙휙.

			NowFrame = 205 - ( sinf(dwElapsedTick*0.01f) * (50.0f) ); // 임시로 이녀석을 색상값 계산에 이용.
			dwColor = RGBA_MAKE(255,255,255,NowFrame);
			NowFrame = 0;

			break;

		case 2: // 확 껴안음.

			// 프레임 계산.
			NowFrame = 3; // 앞부분 약간 짤라주자.
			NowFrame += (float)dwElapsedTick * 0.02f;

			if( NowFrame >= m_pLolo1->m_AnimCount-4 )
			{
				NowFrame = m_pLolo1->m_AnimCount-4-1;
				effect_sort.num = 3;
				effect_sort.dwColor = timeGetTime();
			}

			break;

		case 3: // 제거 모션

			NowFrame = m_pLolo1->m_AnimCount-4-1;

			fScaleX -= (float)dwElapsedTick * 0.001f;
			fScaleY += (float)dwElapsedTick * 0.01f;
			fScaleZ -= (float)dwElapsedTick * 0.001f;
			dwAlpha = 255-(int)((float)dwElapsedTick*0.001f);
			dwColor = RGBA_MAKE(255,255,255,dwAlpha);


			if( dwElapsedTick > 700 ) // 일정시간 지나면 사라지게.
			{
				bEndFlag = TRUE;
				break;
			}

			break;
		}

		// 행렬 계산들.
		D3DUtil_SetIdentityMatrix(matWorld);

		// 좌우로 덩실덩실 처리.
		D3DUtil_SetRotationMatrix( matRotate2, D3DVECTOR(effect_sort.vCurPos.x,effect_sort.vCurPos.y+10.0f,effect_sort.vCurPos.z), effect_sort.vSortPos.z );
		D3DMath_MatrixMultiply( matWorld, matWorld, matRotate2 );

		// 다른 행렬 계산
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
