#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d.h>
#include "Frustum.h"
#include "D3DMath.h"
#include "NKMath.h"


/* -----------------------------------------------------------
// CalCoefs()
// Description:
// �� ���� ���͸� �̿��Ͽ� ����� �������� ����� ����.
// �� ���� ���ʹ� ��Ȯ���� �������� �� ���̴�.
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

	// D�� AB*A=0
	//fD = -DotProduct(AB, A);
	fD = 0.0f - (fN[0]*A.x + fN[1]*A.y + fN[2]*A.z);
};


// -----------------------------------------------------------
// Name: InitFrustum()
// Description:
// ���尡 �����Ǵ� ������ �ʱ� ���������� �����ϴ� ���̴�.
// ī�޶��� ��ġ�� (0, 0, 0) �ִٰ� �����ϰ� �����Ѵ�.
// ------------------------------------------------------------
void CFrustum::InitFrustum(float Fov, float Aspect, float Near, float Far)
{
	float xFov, half_width, half_height;

	//m_Fov = Fov + 0.01745f;
	m_Fov = Fov;
	m_Aspect = Aspect;
	m_Near = Near;
	m_Far = Far;

	// xFov���ϱ�
	xFov = 1/Aspect * Fov;
	// ���������� half_width�� half_height���ϱ�. : far�÷��ο� ���ؼ�.
	half_width = (float) (Far * tan(xFov/2.0));
	half_height = (float) (Far * tan(Fov/2.0));
	m_vecPoint[0] = D3DVECTOR(-half_width, half_height, Far);
	m_vecPoint[1] = D3DVECTOR(half_width, half_height, Far);
	m_vecPoint[2] = D3DVECTOR(half_width, -half_height, Far);
	m_vecPoint[3] = D3DVECTOR(-half_width, -half_height, Far);

	// Near�� ī�޶� �����ǰ� ������ �־�� ����� �����ȴ�.(?)
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
// ���Ʈ������ ���ڷ� �޾�, ���Ʈ������ ������� ���Ͽ� ī�޶� �̵�, ȸ��
// ��Ʈ������ ���Ѵ�.
// �������� ���Ҷ����� ���������� �ʱ� 8���� ���� ī�޶� �̵�, ȸ�� ��Ʈ������
// ��ȭ�� ���Ѽ� ���� �������� ���������� �����Ѵ�.
// ------------------------------------------------------------
void CFrustum::SetFrustum(D3DMATRIX &matView, D3DVECTOR &camera)
{
	int i;
	D3DMATRIX matCamera;

	// ���Ʈ������ �̿��Ͽ�, ī�޶��� �̵�, ȸ�� ��Ʈ������ ���Ѵ�.
	D3DMath_MatrixInvert(matCamera, matView);

	// �ʱ��� ������������ 8���� ������ ī�޶� ��Ʈ������ �̿��Ͽ� ����������� ����� �ٲ۴�.
	for (i=0; i < 4; i++)
	{
		D3DMath_VectorMatrixMultiply(m_vecWorkPoint[i], m_vecPoint[i], matCamera);
	}
	for( i = 8; i < 12; i++ )
		D3DMath_VectorMatrixMultiply( m_vecWorkPoint[i], m_vecPoint[i], matCamera );

	// Near plane�� �����ϴ� ���
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
	// �䷯������ ������ ī�޶� �̵�,ȸ�� ��Ʈ������ ���ؼ� ����� �̵�ȸ���Ǵ��� �˾ƺ��� ���� �����
	// ī�޶��� ��ġ�� m_vecWorkPoint[0](Far plane�� left_top ����Ʈ) �׸���
	// m_vecWorkPoint[4](Near plane�� left_top ����Ʈ) �� ������
	// v01(ī�޶󿡼� far plane�� left_top����Ʈ�� ���� ����)
	// v04(ī�޶󿡼� near plane�� left_top�����Ϳ� ���� ����)�� ���� ��
	// Normaliz(v01) == Normalize(v04) ���� Ȯ���Ѵ�.
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

	// Near�� Far�� ������ 4���� ��� �����..
	m_plane[0].CalCoefs(camera, m_vecWorkPoint[0], m_vecWorkPoint[1]);	// top
	m_plane[1].CalCoefs(camera, m_vecWorkPoint[1], m_vecWorkPoint[2]);	// right
	m_plane[2].CalCoefs(camera, m_vecWorkPoint[2], m_vecWorkPoint[3]);	// bottom
	m_plane[3].CalCoefs(camera, m_vecWorkPoint[3], m_vecWorkPoint[0]);	// left

	// Far plane�����
	m_plane[4].CalCoefs(m_vecWorkPoint[0], m_vecWorkPoint[2], m_vecWorkPoint[1]);

	// Far plane for object culling
	m_plane[6].CalCoefs( m_vecWorkPoint[8], m_vecWorkPoint[10], m_vecWorkPoint[9] );

	// Near plane�� �����Ѵٸ�
	if (m_Near > 0.0f)
	{
		m_plane[5].CalCoefs(m_vecWorkPoint[4], m_vecWorkPoint[5], m_vecWorkPoint[6]);
	}
}

// -----------------------------------------------------------
// Name: ClassifySpace()
// Parameter:
//  CPlane - �˻��� ���
//  D3DVECTOR - �˻��� ���� ���� ��
//  int - �������� ������ ����
// Return Value:
//  ����� ���⿡ �ִ� ������ ����
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
//  D3DVECTOR - �˻��� ���� ���� ��
//  int - �������� ������ ����
// Return Value:
//  #define VFCULL_OUTSIDE	0
//  #define VFCULL_INSIDE	1
//  #define VFCULL_OVERLAP	2
// Description:
//  �˻��� �������� ����� ������ �Ķ���ͷ� �޾Ƽ� �� 6���� ��鿡 ���ؼ�
//  ��� ������ �� ���������� ���ʿ� �ִ��� �ٱ��ʿ� �ִ��� �ƴϸ� ���ʰ� �ٱ��ʿ�
//  ���ļ� �ִ����� �˻��Ѵ�.
// ��� ������ ��� ������� �ٱ��ʿ� ������ �� ������ �������� �ٱ��ʿ� �ִ�.
// ��� ������ ��� ��鿡 ���Ͽ� ����(����� �ݴ����)�� ������ �� ������ �������� ���� �ִ�.
// ------------------------------------------------------------
int	CFrustum::FrustumCull(D3DVECTOR *vec, int num)
{
	int i, res, overlap_count=0;
	// �� �÷��ΰ��� �׽�Ʈ�� �Ѵ�.
	// top, right, bottom, left, far, near plane ��
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
	// �� �÷��ΰ��� �׽�Ʈ�� �Ѵ�.
	// top, right, bottom, left, far, near plane ��
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

// �ٿ�� ���Ǿ �̿��� ���.
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
//  �亼���� 8���� ���Ϳ� ���� ������.
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
