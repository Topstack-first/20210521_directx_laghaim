#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "Frustum.h"
#include "D3DMath.h"
#include "NKMath.h"


/* -----------------------------------------------------------
// CalCoefs()
// Description:
// 세 개의 벡터를 이용하여 평면의 방정식을 만들어 낸다.
// 세 개의 벡터는 정확히는 공간상의 세 점이다.
--------------------------------------------------------------*/
void CPlane::CalCoefs(D3DVECTOR A, D3DVECTOR B, D3DVECTOR C)
{
	D3DVECTOR AB, AC, temp;
	// AB
	AB.x = B.x - A.x;
	AB.y = B.y -A.y;
	AB.z = B.z - A.z;
	// AC
	AC.x = C.x - A.x;
	AC.y = C.y -A.y;
	AC.z = C.z - A.z;

	// AB * AC = NORMAL
	temp = CrossProduct(AB, AC);
	temp = Normalize(temp);
	fN[0] = temp.x;
	fN[1] = temp.y;
	fN[2] = temp.z;

	// D는 AB*A=0
	//fD = -DotProduct(AB, A);
	fD = 0.0f - (fN[0]*A.x + fN[1]*A.y + fN[2]*A.z);
};


// -----------------------------------------------------------
// Name: InitFrustum()
// Description:
// 월드가 구성되는 시점의 초기 프러스텀을 설정하는 것이다.
// 카메라의 위치는 (0, 0, 0) 있다고 가정하고 설정한다.
// ------------------------------------------------------------
void CFrustum::InitFrustum(float Fov, float Aspect, float Near, float Far)
{
	float xFov, half_width, half_height;

	//m_Fov = Fov + 0.01745f;
	m_Fov = Fov;
	m_Aspect = Aspect;
	m_Near = Near;
	m_Far = Far;

	// xFov구하기
	xFov = 1/Aspect * Fov;
	// 프러스텀의 half_width와 half_height구하기. : far플레인에 대해서.
	half_width = (float) (Far * tan(xFov/2.0));
	half_height = (float) (Far * tan(Fov/2.0));
	m_vecPoint[0] = D3DVECTOR(-half_width, half_height, Far);
	m_vecPoint[1] = D3DVECTOR(half_width, half_height, Far);
	m_vecPoint[2] = D3DVECTOR(half_width, -half_height, Far);
	m_vecPoint[3] = D3DVECTOR(-half_width, -half_height, Far);

	// Near가 카메라 포지션과 떨어져 있어야 평면이 형성된다.(?)
	if (Near > 0.0f)
	{
		half_width = (float) (Near * tan(xFov/2.0));
		half_height = (float) (Near * tan(Fov/2.0));
		m_vecPoint[4] = D3DVECTOR(-half_width, half_height, Near);
		m_vecPoint[5] = D3DVECTOR(half_width, half_height, Near);
		m_vecPoint[6] = D3DVECTOR(half_width, -half_height, Near);
		m_vecPoint[7] = D3DVECTOR(-half_width, -half_height, Near);
	}
}


// -----------------------------------------------------------
// Name : InitObjFrustum()
// Desc :
// ------------------------------------------------------------
void CFrustum::InitObjFrustum( float fObjFar )
{
	float xFov, half_width, half_height;
	m_fObjFar = fObjFar;
	xFov = 1 / m_Aspect * m_Fov;

	// For far plane (using object culling)
	half_width = (float)( m_fObjFar * tan( xFov / 2.0 ) );
	half_height = (float)( m_fObjFar * tan( m_Fov / 2.0 ) );
	m_vecPoint[8] = D3DVECTOR( -half_width, half_height, m_fObjFar );
	m_vecPoint[9] = D3DVECTOR( half_width, half_height, m_fObjFar );
	m_vecPoint[10] = D3DVECTOR( half_width, -half_height, m_fObjFar );
	m_vecPoint[11] = D3DVECTOR( -half_width, -half_height, m_fObjFar );
}


// -----------------------------------------------------------
// Name: SetFrustum()
// Description:
// 뷰매트릭스를 인자로 받아, 뷰매트릭스의 역행렬을 구하여 카메라 이동, 회전
// 매트릭스를 구한다.
// 프레임이 변할때마다 프러스텀의 초기 8개의 점을 카메라 이동, 회전 매트릭스에
// 변화를 시켜서 월드 공간상의 프러스텀을 설정한다.
// ------------------------------------------------------------
void CFrustum::SetFrustum(D3DMATRIX &matView, D3DVECTOR &camera)
{
	int i;
	D3DMATRIX matCamera;

	// 뷰매트릭스를 이용하여, 카메라의 이동, 회전 매트릭스를 구한다.
	D3DMath_MatrixInvert(matCamera, matView);

	// 초기의 뷰프러스텀의 8개의 점들을 카메라 매트릭스를 이용하여 월드공간상의 점들로 바꾼다.
	for (i=0; i < 4; i++)
	{
		D3DMath_VectorMatrixMultiply(m_vecWorkPoint[i], m_vecPoint[i], matCamera);
	}
	for( i = 8; i < 12; i++ )
		D3DMath_VectorMatrixMultiply( m_vecWorkPoint[i], m_vecPoint[i], matCamera );

	// Near plane이 존재하는 경우
	if (m_Near > 0.0f)
	{
		for (i=4; i < 8; i++)
		{
			D3DMath_VectorMatrixMultiply(m_vecWorkPoint[i], m_vecPoint[i], matCamera);
		}
	}

	// Debugging
	D3DVECTOR viewCamera = D3DVECTOR(0.0f, 0.0f, 0.0f);
	D3DVECTOR worldCamera;
	D3DMath_VectorMatrixMultiply(worldCamera, viewCamera, matCamera);

	// End of Debugging
	// Debugging
	// 뷰러스텀의 점들이 카메라 이동,회전 매트릭스에 의해서 제대로 이동회전되는지 알아보기 위한 디버깅
	// 카메라의 위치와 m_vecWorkPoint[0](Far plane의 left_top 포인트) 그리고
	// m_vecWorkPoint[4](Near plane의 left_top 포인트) 를 가지고
	// v01(카메라에서 far plane의 left_top포인트에 대한 벡터)
	// v04(카메라에서 near plane의 left_top포인터에 대한 벡터)를 구한 후
	// Normaliz(v01) == Normalize(v04) 임을 확인한다.
	/*
	D3DVECTOR v01, v04;
	for (i=0; i < 4; i++) {
		v01 = m_vecWorkPoint[i] - camera;
		v04 = m_vecWorkPoint[i+4] - camera;
		v01 = Normalize(v01);
		v04 = Normalize(v04);
	}
	*/
	// End of Debugging

	// Near와 Far를 제외한 4개의 평면 만들기..
	m_plane[0].CalCoefs(camera, m_vecWorkPoint[0], m_vecWorkPoint[1]);	// top
	m_plane[1].CalCoefs(camera, m_vecWorkPoint[1], m_vecWorkPoint[2]);	// right
	m_plane[2].CalCoefs(camera, m_vecWorkPoint[2], m_vecWorkPoint[3]);	// bottom
	m_plane[3].CalCoefs(camera, m_vecWorkPoint[3], m_vecWorkPoint[0]);	// left

	// Far plane만들기
	m_plane[4].CalCoefs(m_vecWorkPoint[0], m_vecWorkPoint[2], m_vecWorkPoint[1]);

	// Far plane for object culling
	m_plane[6].CalCoefs( m_vecWorkPoint[8], m_vecWorkPoint[10], m_vecWorkPoint[9] );

	// Near plane이 존재한다면
	if (m_Near > 0.0f)
	{
		m_plane[5].CalCoefs(m_vecWorkPoint[4], m_vecWorkPoint[5], m_vecWorkPoint[6]);
	}
}

// -----------------------------------------------------------
// Name: ClassifySpace()
// Parameter:
//  CPlane - 검사할 평면
//  D3DVECTOR - 검사할 공간 상의 점
//  int - 공간상의 점들의 갯수
// Return Value:
//  평면의 방향에 있는 점들의 갯수
// ------------------------------------------------------------
int	CFrustum::ClassifySpace(CPlane &plane, D3DVECTOR *vec, int num)
{
	int i, outcode = 0;

	for (i=0; i < num; i++)
	{
		if (plane.fN[0]*vec[i].x + plane.fN[1]*vec[i].y + plane.fN[2]*vec[i].z+plane.fD > 0)
		{
			outcode++;
		}
	}

	return outcode;
}

int	CFrustum::ClassifySpaceLand(CPlane &plane, D3DVECTOR *vec, int num)
{
	int i, outcode = 0;

	for (i=0; i < num; i++)
	{
		if (plane.fN[0]*vec[i].x + plane.fN[1]*vec[i].y + plane.fN[2]*vec[i].z+plane.fD > 0 )
		{
			outcode++;
		}
	}

	return outcode;
}

int CFrustum::ClassifySpace(CPlane &plane, float x, float y, float z, float fRadius)
{
	int outcode = 0;

	if (plane.fN[0]*x + plane.fN[1]*y + plane.fN[2]*z+plane.fD >= fRadius)
	{
		outcode++;
	}

	return outcode;
}

// -----------------------------------------------------------
// Name: FrustumCull()
// Parameter:
//  D3DVECTOR - 검사할 공간 상의 점
//  int - 공간상의 점들의 갯수
// Return Value:
//  #define VFCULL_OUTSIDE	0
//  #define VFCULL_INSIDE	1
//  #define VFCULL_OVERLAP	2
// Description:
//  검사할 공간상의 점들과 갯수를 파라미터로 받아서 각 6개의 평면에 대해서
//  모든 점들이 뷰 프러스텀의 안쪽에 있는지 바깥쪽에 있는지 아니면 안쪽과 바깥쪽에
//  겹쳐서 있는지를 검사한다.
// 모든 점들이 어느 한평면의 바깥쪽에 있으면 그 점들은 프러스텀 바깥쪽에 있다.
// 모든 점들이 모든 평면에 대하여 안쪽(평면의 반대방향)에 있으면 그 점들은 프러스텀 내에 있다.
// ------------------------------------------------------------
int	CFrustum::FrustumCull(D3DVECTOR *vec, int num)
{
	int i, res, overlap_count=0;
	// 각 플레인과의 테스트를 한다.
	// top, right, bottom, left, far, near plane 순
	for (i =0; i < 5; i++)
	{
		res = ClassifySpace(m_plane[i], vec, num);
		if (res == num)
		{
			return VFCULL_OUTSIDE;
		}
		overlap_count += res;
	}

	if (m_Near > 0.0f)
	{
		res = ClassifySpace(m_plane[5], vec, num);
		if (res == num)
		{
			return VFCULL_OUTSIDE;
		}
		overlap_count += res;
	}

	if (overlap_count > 0)
		return VFCULL_OVERLAP;
	else
		return VFCULL_INSIDE;				// Full-Inside

}

int CFrustum::FrustumCullLand(D3DVECTOR *vec, int num)
{
	int i, res, overlap_count=0;
	// 각 플레인과의 테스트를 한다.
	// top, right, bottom, left, far, near plane 순
	for (i =0; i < 5; i++)
	{
		res = ClassifySpaceLand(m_plane[i], vec, num);
		if (res == num)
		{
			return VFCULL_OUTSIDE;
		}
		overlap_count += res;
	}

	if (m_Near > 0.0f)
	{
		res = ClassifySpaceLand(m_plane[5], vec, num);
		if (res == num)
		{
			return VFCULL_OUTSIDE;
		}
		overlap_count += res;
	}

	if (overlap_count > 0)
		return VFCULL_OVERLAP;
	else
		return VFCULL_INSIDE;				// Full-Inside
}

// 바운딩 스피어를 이용한 방법.
int CFrustum::FrustumCullPoint(float x, float y, float z, float fRadius)
{
	int i;

	for (i=0; i < 5; i++)
	{
		if (ClassifySpace(m_plane[i], x, y, z, fRadius) > 0)
			return VFCULL_OUTSIDE;
	}

	if (m_Near > 0.0f)
	{
		if (ClassifySpace(m_plane[5], x, y, z, fRadius) > 0)
			return VFCULL_OUTSIDE;
	}

	return VFCULL_INSIDE;
}

int CFrustum::MinMaxCull(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
{
	static D3DVECTOR vec[8];

	vec[0].x = min_x;
	vec[0].y = min_y;
	vec[0].z = min_z;
	vec[1].x = min_x;
	vec[1].y = min_y;
	vec[1].z = max_z;
	vec[2].x = max_x;
	vec[2].y = min_y;
	vec[2].z = max_z;
	vec[3].x = max_x;
	vec[3].y = min_y;
	vec[3].z = min_z;
	vec[4].x = min_x;
	vec[4].y = max_y;
	vec[4].z = min_z;
	vec[5].x = min_x;
	vec[5].y = max_y;
	vec[5].z = max_z;
	vec[6].x = max_x;
	vec[6].y = max_y;
	vec[6].z = max_z;
	vec[7].x = max_x;
	vec[7].y = max_y;
	vec[7].z = min_z;

	return FrustumCull(vec, 8);
}

int CFrustum::MinMaxCull(D3DVECTOR &vecMin, D3DVECTOR &vecMax)
{
	static D3DVECTOR vec[8];

	vec[0].x = vecMin.x;
	vec[0].y = vecMin.y;
	vec[0].z = vecMin.z;
	vec[1].x = vecMin.x;
	vec[1].y = vecMin.y;
	vec[1].z = vecMax.z;
	vec[2].x = vecMax.x;
	vec[2].y = vecMin.y;
	vec[2].z = vecMax.z;
	vec[3].x = vecMax.x;
	vec[3].y = vecMin.y;
	vec[3].z = vecMin.z;
	vec[4].x = vecMin.x;
	vec[4].y = vecMax.y;
	vec[4].z = vecMin.z;
	vec[5].x = vecMin.x;
	vec[5].y = vecMax.y;
	vec[5].z = vecMax.z;
	vec[6].x = vecMax.x;
	vec[6].y = vecMax.y;
	vec[6].z = vecMax.z;
	vec[7].x = vecMax.x;
	vec[7].y = vecMax.y;
	vec[7].z = vecMin.z;

	return FrustumCull(vec, 8);
}

// -----------------------------------------------------------
// Name: GetFrustumPoints()
// Return Value:
//  뷰볼륨의 8개의 벡터에 대한 포인터.
// Description:
// ------------------------------------------------------------
D3DVECTOR *CFrustum::GetFrustumPoints()
{
	return m_vecWorkPoint;
}


//-----------------------------------------------------------------------------
// Name : CullAABB()
// Desc : 2004 addition
//-----------------------------------------------------------------------------
int CFrustum::CullAABB( const D3DVECTOR& rMin, const D3DVECTOR& rMax )
{
	D3DVECTOR	vMinExtream, vMaxExtream;
	bool bOverlap = false;

	for( int i = 0; i < 6; i++ )
	{
		if( m_plane[i].fN[0] > NK_EPSILON4 )
		{
			vMinExtream.x = rMin.x;
			vMaxExtream.x = rMax.x;
		}
		else
		{
			vMinExtream.x = rMax.x;
			vMaxExtream.x = rMin.x;
		}
		if( m_plane[i].fN[1] > NK_EPSILON4 )
		{
			vMinExtream.y = rMin.y;
			vMaxExtream.y = rMax.y;
		}
		else
		{
			vMinExtream.y = rMax.y;
			vMaxExtream.y = rMin.y;
		}
		if( m_plane[i].fN[2] > NK_EPSILON4 )
		{
			vMinExtream.z = rMin.z;
			vMaxExtream.z = rMax.z;
		}
		else
		{
			vMinExtream.z = rMax.z;
			vMaxExtream.z = rMin.z;
		}
		if( ( m_plane[i].fN[0]*vMinExtream.x + m_plane[i].fN[1]*vMinExtream.y +
				m_plane[i].fN[2]*vMinExtream.z + m_plane[i].fD ) > NK_EPSILON4 )
			return VFCULL_OUTSIDE;
		if( ( m_plane[i].fN[0]*vMaxExtream.x + m_plane[i].fN[1]*vMaxExtream.y +
				m_plane[i].fN[2]*vMaxExtream.z + m_plane[i].fD ) > NK_EPSILON4 )
			bOverlap = true;
	}

	if( bOverlap )
		return VFCULL_OVERLAP;

	return VFCULL_INSIDE;
}


//-----------------------------------------------------------------------------
// Name : CullAABBObj()
// Desc : 2004 addition
//-----------------------------------------------------------------------------
int CFrustum::CullAABBObj( const D3DVECTOR& rMin, const D3DVECTOR& rMax )
{
	D3DVECTOR	vMinExtream, vMaxExtream;
	bool bOverlap = false;

	int i = 0;
	for( i = 0; i < 4; i++ )
	{
		if( m_plane[i].fN[0] > NK_EPSILON4 )
		{
			vMinExtream.x = rMin.x;
			vMaxExtream.x = rMax.x;
		}
		else
		{
			vMinExtream.x = rMax.x;
			vMaxExtream.x = rMin.x;
		}
		if( m_plane[i].fN[1] > NK_EPSILON4 )
		{
			vMinExtream.y = rMin.y;
			vMaxExtream.y = rMax.y;
		}
		else
		{
			vMinExtream.y = rMax.y;
			vMaxExtream.y = rMin.y;
		}
		if( m_plane[i].fN[2] > NK_EPSILON4 )
		{
			vMinExtream.z = rMin.z;
			vMaxExtream.z = rMax.z;
		}
		else
		{
			vMinExtream.z = rMax.z;
			vMaxExtream.z = rMin.z;
		}
		if( ( m_plane[i].fN[0]*vMinExtream.x + m_plane[i].fN[1]*vMinExtream.y +
				m_plane[i].fN[2]*vMinExtream.z + m_plane[i].fD ) > NK_EPSILON4 )
			return VFCULL_OUTSIDE;
		if( ( m_plane[i].fN[0]*vMaxExtream.x + m_plane[i].fN[1]*vMaxExtream.y +
				m_plane[i].fN[2]*vMaxExtream.z + m_plane[i].fD ) > NK_EPSILON4 )
			bOverlap = true;
	}

	for( i = 5; i < 7; i++ )
	{
		if( m_plane[i].fN[0] > NK_EPSILON4 )
		{
			vMinExtream.x = rMin.x;
			vMaxExtream.x = rMax.x;
		}
		else
		{
			vMinExtream.x = rMax.x;
			vMaxExtream.x = rMin.x;
		}
		if( m_plane[i].fN[1] > NK_EPSILON4 )
		{
			vMinExtream.y = rMin.y;
			vMaxExtream.y = rMax.y;
		}
		else
		{
			vMinExtream.y = rMax.y;
			vMaxExtream.y = rMin.y;
		}
		if( m_plane[i].fN[2] > NK_EPSILON4 )
		{
			vMinExtream.z = rMin.z;
			vMaxExtream.z = rMax.z;
		}
		else
		{
			vMinExtream.z = rMax.z;
			vMaxExtream.z = rMin.z;
		}
		if( ( m_plane[i].fN[0]*vMinExtream.x + m_plane[i].fN[1]*vMinExtream.y +
				m_plane[i].fN[2]*vMinExtream.z + m_plane[i].fD ) > NK_EPSILON4 )
			return VFCULL_OUTSIDE;
		if( ( m_plane[i].fN[0]*vMaxExtream.x + m_plane[i].fN[1]*vMaxExtream.y +
				m_plane[i].fN[2]*vMaxExtream.z + m_plane[i].fD ) > NK_EPSILON4 )
			bOverlap = true;
	}

	if( bOverlap )
		return VFCULL_OVERLAP;

	return VFCULL_INSIDE;
}
