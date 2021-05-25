#include "stdafx.h"
//----------------------------------------------------------------------
//		INCLUDE
//----------------------------------------------------------------------
#define D3D_OVERLOADS
#include <math.h>
#include <d3d9.h>
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


extern int				g_nCurWorldIdx;				// Current World Index ///��ȣȭ����060925


D3DMATRIX	Matrix_3D;			// ���� ���� ���
//----------------------------------------------------------------------
//		DEFINE
//----------------------------------------------------------------------
#define IMG_WIDTH 12            // ȭ��ǥ �̹��� ���� ���� ũ��
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
	// ��Ƽ���� ����
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);
	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = g_pNk2DFrame->GetClientWidth();
	vpNew.dwHeight = g_pNk2DFrame->GetClientHeight();
	GET_D3DDEVICE()->SetViewport(&vpNew);
	//// �������� ��Ʈ���� ����.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	// ������
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	OriginRotate(tan_val);  // ȸ����Ű��
	MoveMat(xpos, ypos);    // �ű��

	//----------------�׸��� ���� / �� --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX �� �̿��� ������ ���(ȭ���� ���� ��ǥ�� �Է��Ѵ�)
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

	// ���󺹱�.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}


// nate 2004 - 3 : ���� Ȯ�� �ý���
void C3d_PosPoint::RenderOj(int xpos, int ypos, float tan_val, int textr_id, BYTE byShare, int ShareNum )
{
	if( !GET_D3DDEVICE() )
		return;

	// nate 2004 - 3 : ���� Ȯ�� �ý���
	m_byShare = byShare;
	m_nShareNum = ShareNum;
	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	D3DVECTOR vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT7 vpOld, vpNew;
	DWORD	dwColor;
	D3DMATERIAL7 mtrl;

	// ��Ƽ���� ����
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	GET_D3DDEVICE()->SetMaterial(&mtrl);

	// ������ ����Ʈ�� �����ϰ� ���Ӱ� ����Ʈ�� �����Ѵ�.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = g_pNk2DFrame->GetClientWidth();
	vpNew.dwHeight = g_pNk2DFrame->GetClientHeight();
	GET_D3DDEVICE()->SetViewport(&vpNew);
	//// �������� ��Ʈ���� ����.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);
	// ������
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	OriginRotate(tan_val);  // ȸ����Ű��
	MoveMat(xpos, ypos);    // �ű��

	//----------------�׸��� ���� / �� --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX �� �̿��� ������ ���(ȭ���� ���� ��ǥ�� �Է��Ѵ�)
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

	// ���󺹱�.
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
	OriginRotate(tan_val);  // ȸ����Ű��
	MoveMat(xpos, ypos);    // �ű��
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

void C3d_PosPoint::RenderGuildPos( int xpos, int ypos, float ani_val, int textr_id ) ///�̴ϸʱ���ã��3D
{
	if( !GET_D3DDEVICE() )
		return;

	int OrginalWidth = 0, OriginalHeight = 0; // ����� ���������� �ٲ��ٶ� ��������� ����صα� ���� �༮(ex: �����)
	if( textr_id == EFF_GUILDMASTER )
	{
		OrginalWidth = m_ImgWidth;
		OriginalHeight = m_ImgHeight; // ��� �صΰ�...

		m_ImgWidth = 10; // �ٲ۴�.
		m_ImgHeight = 10;
	}

	LPDIRECTDRAWSURFACE7 lpTexture = NULL;
	int i = 0;
	int Alpha = 255;

	// 0.0f���� 1.0f ���� �� ������ ani_val �� ���� ���İ�. �ֺ� ȿ�� ũ����� ����ؼ� �������ش�.
	float PlusSize = 25.0f * ( ani_val*(ani_val*0.5f) ); // ó�� ����� ���� ũ�⿡ ����, �ι�° ����� �ӵ��� ����



	//////////////////////////////////////////////////////////////////////////////////////
	// �ֺ� ȿ�� 1 (�ĵ��� �Ǵ� �� ȿ��)
	Alpha = 250;
	Alpha -= (int)(30.0f*ani_val); // ���İ��� ����. (�� ����� ���� �ϸ� ���������� �ӵ��� ��������.)

	if( PlusSize < 25.0f && PlusSize > 5.0F ) // �̳༮�� ���� ũ�⸸ŭ�� ����ش�.(�̰ɷ� ���ĵ� ȿ���� �����ش�.)
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
	// �ֺ� ȿ�� 2 (�ֺ� ���� �ܻ� ȿ��)
	Alpha = 140;
	Alpha -= (int)(70.0f*ani_val); // ���İ��� ����. (�� ����� ���� �ϸ� ���������� �ӵ��� ��������.)

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
	// ���� ����Ʈ
	Alpha = 230;
	Alpha += (int)(50.0f*ani_val*ani_val*ani_val);

	if( ani_val >= 0.5f ) // �����̰� ����.
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

	// ����� �ٲ���ٸ� �����ش�.
	if( OrginalWidth != 0 || OriginalHeight != 0 )
	{
		m_ImgWidth = OrginalWidth;
		m_ImgHeight = OriginalHeight; // �����ش�.
	}

}


// nate : ���� Ȯ�� �ý��� - ���� �������� ȸ����Ų��..
void C3d_PosPoint::OriginRotate( float tan_val )
{
	// nate : ���� Ȯ�� �ý���
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


// ����ȸ����Ų ������ ���ؽ����� ��ǥ�� ���ϴ� ��ġ�� �̵���Ų��
void C3d_PosPoint::MoveMat(int xpos, int ypos)
{
	for( int i = 0 ; i < 4 ; i++ )
	{
		TLVec[i].sx += xpos;
		TLVec[i].sy += ypos;
	}
}


// ���� �࿭�� �ʱ�ȭ..
void C3d_PosPoint::Identity_Matrix( D3DMATRIX& m)
{
	//  1 �� ����
	m._11 = m._22 = m._33 = m._44 = 1.0f;
	//  0 �� ����
	m._12 = m._13 = m._14 = 0.0f;
	m._21 = m._23 = m._24 = 0.0f;
	m._31 = m._32 = m._34 = 0.0f;
	m._41 = m._42 = m._43 = 0.0f;
}


// ���⼭�� z�࿡���� ȸ������ ����ϰ�..
void C3d_PosPoint::Rotate_Matrix(D3DMATRIX& m, float Angl)
{
	float temp_sin,temp_cos;
	temp_sin = (float)sin(3.141592/180*Angl);
	temp_cos = (float)cos(3.141592/180*Angl);
	Identity_Matrix(m);

	// Z ��ȸ��
	m._11 = temp_cos;
	m._12 = temp_sin;
	m._21 =-temp_sin;
	m._22 = temp_cos;
}

//  ����� ����� �� ��ȯ�� ���� �����Ѵ�
BOOL C3d_PosPoint::Vertex_Matrix_Multiply(D3DTLVERTEX & Vert,D3DMATRIX& m)
{
	// �ӽ� ����
	float temp_x,temp_y,temp_z,temp_w;


	temp_x = m._11*Vert.sx + m._21*Vert.sy + m._31*Vert.sz + m._41;
	temp_y = m._12*Vert.sx + m._22*Vert.sy + m._32*Vert.sz + m._42;
	temp_z = m._13*Vert.sx + m._23*Vert.sy + m._33*Vert.sz + m._43;
	temp_w = m._14*Vert.sx + m._24*Vert.sy + m._34*Vert.sz + m._44;

	// �и� 0�� �ٻ����� Ȯ��
	if(fabs(temp_w)<0.00001)
		return FALSE;

	// ��� �� ��ȯ
	Vert.sx = temp_x/temp_w;
	Vert.sy = temp_y/temp_w;
	Vert.sz = temp_z/temp_w;

	return TRUE;
}

