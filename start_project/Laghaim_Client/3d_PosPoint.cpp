#include "stdafx.h"
//----------------------------------------------------------------------
//		INCLUDE
//----------------------------------------------------------------------
#define D3D_OVERLOADS
#include <math.h>
#include <d3d.h>
#include "sheet.h"
#include "d3dutil.h"
#include "3d_PosPoint.h"
#include "IndexedTexture.h"
#include "D3DMath.h"
#include "LoadEffectSound.h"
// nate 2004 - 7 - Image Manager
#include "ImageDefine.h"
#include "Nk2DFrame.h"
#include "main.h"


extern int				g_nCurWorldIdx;				// Current World Index ///암호화변경060925


D3DMATRIX	Matrix_3D;			// 공간 조작 행렬
//----------------------------------------------------------------------
//		DEFINE
//----------------------------------------------------------------------
#define IMG_WIDTH 12            // 화살표 이미지 가로 세로 크기
#define IMG_HEIGHT 5

C3d_PosPoint::C3d_PosPoint()
	: m_ImgWidth(0)
	, m_ImgHeight(0)
	, m_byShare(0)
	, m_nShareNum(0)
{
	Init();
}

C3d_PosPoint::~C3d_PosPoint()
{
	DeleteRes();
}

void C3d_PosPoint::Init()
{}

void C3d_PosPoint::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_ImgWidth  = IMG_WIDTH;
	m_ImgHeight  = IMG_HEIGHT;
}

void C3d_PosPoint::LoadRes(int img_cx, int img_cy)
{
	if( !g_pDisplay )
		return;

	m_ImgWidth  = img_cx;
	m_ImgHeight  = img_cy;
}

void C3d_PosPoint::DeleteRes()
{
}

void C3d_PosPoint::RenderOj(int xpos, int ypos, float tan_val)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;
	D3DMATERIAL7 mtrl;
	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);
	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = g_pNk2DFrame->GetClientWidth();
	vpNew.dwHeight = g_pNk2DFrame->GetClientHeight();
	GET_D3DDEVICE()->SetViewport(&vpNew);
	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	OriginRotate(tan_val);  // 회전시키구
	MoveMat(xpos, ypos);    // 옮긴다

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_3DPOSPOINT_POSDIR]);

		GET_D3DDEVICE()->SetTexture(0, lpTexture);
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}


// nate 2004 - 3 : 숫자 확인 시스템
void C3d_PosPoint::RenderOj(int xpos, int ypos, float tan_val, int textr_id, BYTE byShare, int ShareNum )
{
	if( !GET_D3DDEVICE() )
		return;

	// nate 2004 - 3 : 숫자 확인 시스템
	m_byShare = byShare;
	m_nShareNum = ShareNum;
	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	// 머티리얼 설정
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = g_pNk2DFrame->GetClientWidth();
	vpNew.dwHeight = g_pNk2DFrame->GetClientHeight();
	GET_D3DDEVICE()->SetViewport(&vpNew);
	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	OriginRotate(tan_val);  // 회전시키구
	MoveMat(xpos, ypos);    // 옮긴다

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[textr_id]);

		GET_D3DDEVICE()->SetTexture(0, lpTexture);
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}



void C3d_PosPoint::RenderOj_Fade(int xpos, int ypos, float tan_val, int textr_id)
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	OriginRotate(tan_val);  // 회전시키구
	MoveMat(xpos, ypos);    // 옮긴다
	static int cnt = 0;
	static int Alpha = 255;
	static bool m_bInc = false;
	static DWORD startTime = timeGetTime();
	DWORD  curTime = timeGetTime();

	if(! m_bInc)
	{
		if((curTime - startTime) > 1)
		{
			Alpha-=30;
			if(Alpha <0)
			{
				Alpha = 0;
				m_bInc = true;
			}
			startTime = curTime;
		}
	}
	else
	{
		if((curTime - startTime) > 1)
		{
			Alpha+=30;
			if(Alpha >255)
			{
				Alpha = 255;
				m_bInc = false;
			}
		}
		startTime = curTime;
	}

	TLVec[0].color = RGBA_MAKE ( 255, 255, 255, Alpha );
	TLVec[1].color = RGBA_MAKE ( 255, 255, 255, Alpha );
	TLVec[2].color = RGBA_MAKE ( 255, 255, 255, Alpha );
	TLVec[3].color = RGBA_MAKE ( 255, 255, 255, Alpha );

	lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[textr_id]);
	GET_D3DDEVICE()->SetTexture(0, lpTexture);
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
}

void C3d_PosPoint::RenderGuildPos( int xpos, int ypos, float ani_val, int textr_id ) ///미니맵길드원찾기3D
{
	if( !GET_D3DDEVICE() )
		return;

	int OrginalWidth = 0, OriginalHeight = 0; // 사이즈를 예외적으로 바꿔줄때 원래사이즈를 기억해두기 위한 녀석(ex: 길드장)
	if( textr_id == EFF_GUILDMASTER )
	{
		OrginalWidth = m_ImgWidth;
		OriginalHeight = m_ImgHeight; // 기억 해두고...

		m_ImgWidth = 10; // 바꾼다.
		m_ImgHeight = 10;
	}

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	int i = 0;
	int Alpha = 255;

	// 0.0f부터 1.0f 까지 로 들어오는 ani_val 로 부터 알파값. 주변 효과 크기등을 계산해서 적용해준다.
	float PlusSize = 25.0f * ( ani_val*(ani_val*0.5f) ); // 처음 계수는 최종 크기에 영향, 두번째 계수는 속도에 영향



	//////////////////////////////////////////////////////////////////////////////////////
	// 주변 효과 1 (파동이 되는 원 효과)
	Alpha = 250;
	Alpha -= (int)(30.0f*ani_val); // 알파값을 조정. (이 계수를 높게 하면 투명해지는 속도가 빨라진다.)

	if( PlusSize < 25.0f && PlusSize > 5.0F ) // 이녀석은 일정 크기만큼만 찍어준다.(이걸로 후파동 효과가 나와준다.)
	{
		TLVec[0] = D3DTLVERTEX(D3DVECTOR( -(m_ImgWidth+PlusSize), (m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX(D3DVECTOR( -(m_ImgWidth+PlusSize), -(m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX(D3DVECTOR( (m_ImgWidth+PlusSize), -(m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX(D3DVECTOR( (m_ImgWidth+PlusSize), (m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );
		for( i = 0 ; i < 4 ; i++ )
		{
			TLVec[i].sx += xpos;
			TLVec[i].sy += ypos;
		}
		lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_GUILDPOS_SIDE]);
		GET_D3DDEVICE()->SetTexture(0, lpTexture);
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// 주변 효과 2 (주변 원의 잔상 효과)
	Alpha = 140;
	Alpha -= (int)(70.0f*ani_val); // 알파값을 조정. (이 계수를 높게 하면 투명해지는 속도가 빨라진다.)

	TLVec[0] = D3DTLVERTEX(D3DVECTOR( -(m_ImgWidth+PlusSize), (m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
	TLVec[1] = D3DTLVERTEX(D3DVECTOR( -(m_ImgWidth+PlusSize), -(m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX(D3DVECTOR( (m_ImgWidth+PlusSize), -(m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
	TLVec[3] = D3DTLVERTEX(D3DVECTOR( (m_ImgWidth+PlusSize), (m_ImgHeight+PlusSize), 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );
	for( i = 0 ; i < 4 ; i++ )
	{
		TLVec[i].sx += xpos;
		TLVec[i].sy += ypos;
	}
	lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_GUILDPOS_SIDE2]);
	GET_D3DDEVICE()->SetTexture(0, lpTexture);
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);




	//////////////////////////////////////////////////////////////////////////////////////
	// 메인 포인트
	Alpha = 230;
	Alpha += (int)(50.0f*ani_val*ani_val*ani_val);

	if( ani_val >= 0.5f ) // 깜빡이게 설정.
	{
		Alpha -= (int)( 50.0f*((ani_val-0.5f)*(ani_val-0.5f)*(ani_val-0.5f)) );
	}

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(-m_ImgWidth,  m_ImgHeight, 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 1.0f );
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(-m_ImgWidth, -m_ImgHeight, 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 0, 0 );
	TLVec[2] = D3DTLVERTEX(D3DVECTOR( m_ImgWidth, -m_ImgHeight, 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 0 );
	TLVec[3] = D3DTLVERTEX(D3DVECTOR( m_ImgWidth,  m_ImgHeight, 0), 1, RGBA_MAKE(255,255,255,Alpha), 0, 1.0f, 1.0f );
	for( i = 0 ; i < 4 ; i++ )
	{
		TLVec[i].sx += xpos;
		TLVec[i].sy += ypos;
	}
	lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[textr_id]);
	GET_D3DDEVICE()->SetTexture(0, lpTexture);
	GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLEFAN    , D3DFVF_TLVERTEX, TLVec, 4, 0);

	// 사이즈를 바꿨었다면 돌려준다.
	if( OrginalWidth != 0 || OriginalHeight != 0 )
	{
		m_ImgWidth = OrginalWidth;
		m_ImgHeight = OriginalHeight; // 돌려준다.
	}

}


// nate : 숫자 확인 시스템 - 최초 원점에서 회전시킨다..
void C3d_PosPoint::OriginRotate( float tan_val )
{
	// nate : 숫자 확인 시스템
	if( m_byShare )
	{
		float fResult = 0.1f * m_nShareNum;
		TLVec[0] = D3DTLVERTEX(D3DVECTOR(-m_ImgWidth,  m_ImgHeight, 0),
							   1, 0xffffffff, 0, fResult, 1.0f );
		TLVec[1] = D3DTLVERTEX(D3DVECTOR(-m_ImgWidth, -m_ImgHeight, 0),
							   1, 0xffffffff, 0, fResult, 0 );
		TLVec[2] = D3DTLVERTEX(D3DVECTOR( m_ImgWidth, -m_ImgHeight, 0),
							   1, 0xffffffff, 0, fResult + 0.1f, 0 );
		TLVec[3] = D3DTLVERTEX(D3DVECTOR( m_ImgWidth,  m_ImgHeight, 0),
							   1, 0xffffffff, 0, fResult + 0.1f, 1.0f );
	}
	else
	{
		TLVec[0] = D3DTLVERTEX(D3DVECTOR(-m_ImgWidth,  m_ImgHeight, 0), 1, 0xffffffff, 0, 0, 1.0f );
		TLVec[1] = D3DTLVERTEX(D3DVECTOR(-m_ImgWidth, -m_ImgHeight, 0), 1, 0xffffffff, 0, 0, 0 );
		TLVec[2] = D3DTLVERTEX(D3DVECTOR( m_ImgWidth, -m_ImgHeight, 0), 1, 0xffffffff, 0, 1.0f, 0 );
		TLVec[3] = D3DTLVERTEX(D3DVECTOR( m_ImgWidth,  m_ImgHeight, 0), 1, 0xffffffff, 0, 1.0f, 1.0f );
	}

	float angle= (180/3.141592)*(tan_val);
	Rotate_Matrix(Matrix_3D, -(angle));
	for(int i=0; i<VERTEX_CNT; i++)
		Vertex_Matrix_Multiply(TLVec[i],Matrix_3D);
}


// 원점회전시킨 각각의 버텍스들의 좌표를 원하는 위치로 이동시킨다
void C3d_PosPoint::MoveMat(int xpos, int ypos)
{
	for( int i = 0 ; i < 4 ; i++ )
	{
		TLVec[i].sx += xpos;
		TLVec[i].sy += ypos;
	}
}


// 단위 행열로 초기화..
void C3d_PosPoint::Identity_Matrix( D3DMATRIX& m)
{
	//  1 값 설정
	m._11 = m._22 = m._33 = m._44 = 1.0f;
	//  0 값 설정
	m._12 = m._13 = m._14 = 0.0f;
	m._21 = m._23 = m._24 = 0.0f;
	m._31 = m._32 = m._34 = 0.0f;
	m._41 = m._42 = m._43 = 0.0f;
}


// 여기서는 z축에서의 회전만을 계산하고..
void C3d_PosPoint::Rotate_Matrix(D3DMATRIX& m, float Angl)
{
	float temp_sin,temp_cos;
	temp_sin = (float)sin(3.141592/180*Angl);
	temp_cos = (float)cos(3.141592/180*Angl);
	Identity_Matrix(m);

	// Z 축회전
	m._11 = temp_cos;
	m._12 = temp_sin;
	m._21 =-temp_sin;
	m._22 = temp_cos;
}

//  행렬의 곱계산 후 변환된 값을 리턴한다
BOOL C3d_PosPoint::Vertex_Matrix_Multiply(D3DTLVERTEX & Vert,D3DMATRIX& m)
{
	// 임시 변수
	float temp_x,temp_y,temp_z,temp_w;


	temp_x = m._11*Vert.sx + m._21*Vert.sy + m._31*Vert.sz + m._41;
	temp_y = m._12*Vert.sx + m._22*Vert.sy + m._32*Vert.sz + m._42;
	temp_z = m._13*Vert.sx + m._23*Vert.sy + m._33*Vert.sz + m._43;
	temp_w = m._14*Vert.sx + m._24*Vert.sy + m._34*Vert.sz + m._44;

	// 분모가 0에 근사한지 확인
	if(fabs(temp_w)<0.00001)
		return FALSE;

	// 계산 값 반환
	Vert.sx = temp_x/temp_w;
	Vert.sy = temp_y/temp_w;
	Vert.sz = temp_z/temp_w;

	return TRUE;
}

