#include "stdafx.h"
#include ".\lh_decal.h"

#include "UIMgr.h"
#include "Land.h"
#include "NkCharacter.h"
#include "ImageDefine.h "




extern int					g_EffectTextr[];


LH_Decal::LH_Decal(void)
	:	m_initialized(false)
	,	m_vertices(NULL)
	,	m_indices(NULL)
	,	m_num_vertex(0)
	,	m_num_index(0)
	,	m_x(0)
	,	m_z(0)
	,	m_half_size(0)
	, m_size(0)
	, m_gap(0)
{
	Initial();
}

LH_Decal::LH_Decal(float x, float z, float half_size, int gap)
	:	m_initialized(false)
	,	m_vertices(NULL)
	,	m_indices(NULL)
	,	m_num_vertex(0)
	,	m_num_index(0)
	,	m_size(0)
	,	m_x(x)
	,	m_z(z)
	,	m_half_size(half_size)
	,	m_gap(gap)
{
	Init(m_x, m_z , m_half_size , m_gap );
}

LH_Decal::~LH_Decal(void)
{
	Release();
}

void LH_Decal::Initial()
{
}

void LH_Decal::Release()
{
	SAFE_DELETE_ARRAY(m_vertices);
	SAFE_DELETE_ARRAY(m_indices);
}

void LH_Decal::Init(float x, float z, float half_size, int gap)
{	
	(void)gap;
	m_x = x;
	m_z = z;	

	if( half_size < 30.0f )
		half_size = 30.0f;
	else if( half_size > 500.0f )
		half_size = 500.0f;

	int halfTemp = (int)half_size;
	int halfMod = halfTemp % 10;
	halfTemp -= halfMod;

	m_half_size = (float)halfTemp;

	m_size			= m_half_size * 2;							// 나타내야할 총 영역 사이즈(지름)
	m_gap			= m_size / 10.0f;
	m_num_vertex	= 121;	// 버텍스 수
	m_num_index		= 600;	// 인덱스 수

	m_vertices		= new D3DVERTEX[m_num_vertex];
	m_indices		= new WORD[m_num_index];

	int nCnt	= 10;								// 지름에 대한 버텍스 수
	int n		= 0;

	float fu=0.0f, fv=0.0f;
	float uvLen = 0.1f;
	float nMapX=0.0f, nMapY=0.0f, nMapZ=0.0f;

	float m_fx = m_x - m_half_size;
	float m_fz = m_z + m_half_size;

	// 버텍스 설정 //
	for( int i = 0; i < nCnt+1; i++ )
	{
		for( int j = 0; j < nCnt+1; j++ )
		{
			n = i * (nCnt+1) + j;

			nMapX = m_fx + (j*m_gap);
			nMapZ = m_fz - (i*m_gap);			
			nMapY = pCMyApp->m_pUIMgr->m_pLand->GetHFHeight(nMapX, nMapZ) + 1.0f;

			m_vertices[n] = D3DVERTEX( D3DVECTOR(nMapX, nMapY ,nMapZ), D3DVECTOR(0, 0, 1), 0.0f+(i*uvLen), 0.0f+(j*uvLen) );			
		}
	}

	// 인덱스 설정 //
	n = 0;
	for( int i = 0; i < nCnt; i++ )
	{
		for( int j = 0; j < nCnt; j++ )
		{
			int _0 = (nCnt+1)*(i+0) +j;
			int _1 = (nCnt+1)*(i+0) +j+1;
			int _2 = (nCnt+1)*(i+1) +j ;
			int _3 = (nCnt+1)*(i+1) +j+1;

			m_indices[n] = _0;
			++n;
			m_indices[n] = _1;
			++n;
			m_indices[n] = _2;
			++n;
			m_indices[n] = _1;
			++n;
			m_indices[n] = _3;
			++n;
			m_indices[n] = _2;
			++n;
		}
	}

	m_initialized = true;
}

void LH_Decal::ReInit(float x, float z, float half_size, int gap)
{
	//	if( m_initialized )
	Release();

	Init(x,z,half_size,gap);
}

void LH_Decal::UpdateTransform()
{

}

void LH_Decal::Render(float dt)
{
	if( !m_initialized )
		return;

	D3DMATRIX matWorld;
	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[EFF_LAGRUSH_AREA]);

	D3DUtil_SetIdentityMatrix(matWorld);

	// 	matWorld._41 = m_x;
	// 	matWorld._42 = g_pRoh->m_wy+0.2f;
	// 	matWorld._43 = m_z;

	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );


	DWORD dwTextureState[3];

	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwTextureState[0] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &dwTextureState[1] );
	GET_D3DDEVICE()->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwTextureState[2] );


	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_SRCBLEND,   D3DBLEND_ONE );
	GET_D3DDEVICE()->SetRenderState( D3DRENDERSTATE_DESTBLEND,  D3DBLEND_ONE );

	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );


	GET_D3DDEVICE()->SetTexture(0, lpTexture);
	GET_D3DDEVICE()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX, m_vertices, m_num_vertex, m_indices, m_num_index, NULL);


	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);

	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwTextureState[0] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwTextureState[1] );
	GET_D3DDEVICE()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwTextureState[2] );
}