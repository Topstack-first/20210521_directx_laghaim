#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include <d3d.h>

#define VFCULL_OUTSIDE	0
#define VFCULL_INSIDE	1
#define VFCULL_OVERLAP	2

class CPlane
{
public:
	float fN[3];	// x, y, z
	float fD;		// distance

	CPlane() {};
	CPlane(D3DVECTOR A, D3DVECTOR B, D3DVECTOR C)
	{
		CalCoefs(A, B, C);
	};

	void CalCoefs(D3DVECTOR A, D3DVECTOR B, D3DVECTOR C);
};

class CFrustum
{
public:
	// uleft, uright, lright, lleft of Far plane and uleft, uright, lright, lleft of Near plane
	D3DVECTOR m_vecPoint[12];		// near�� ������ܿ��� �ð�������� �׸��� far�� �������..
	D3DVECTOR m_vecWorkPoint[12];  // view matrix�� ���� transformation�� ����.
	CPlane m_plane[7];			// top, right, bottom, left, far, near, far(for object culling) plane

	float m_Fov, m_Aspect, m_Near, m_Far, m_fObjFar;

	CFrustum() {};
	~CFrustum() {};

	void	InitFrustum(float Fov, float Aspect, float Near, float Far);
	void	InitObjFrustum( float fObjFar );
	void	SetFrustum(D3DMATRIX &matView, D3DVECTOR &camera);
	D3DVECTOR *GetFrustumPoints();

	// ������ �з�(completely one side, overlap)
	int		ClassifySpace(CPlane &plane, D3DVECTOR *vec, int num);
	int		ClassifySpaceLand(CPlane &plane, D3DVECTOR *vec, int num);
	int		ClassifySpace(CPlane &plane, float x, float y, float z, float fRadius);

	// ���鿡 ���� �������� �ø�
	int		FrustumCull(D3DVECTOR *vec, int num);
	int		FrustumCullLand(D3DVECTOR *vec, int num);
	int		FrustumCullPoint(float x, float y, float z, float fRadius);
	int		MinMaxCull(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z);
	int		MinMaxCull(D3DVECTOR &vecMin, D3DVECTOR &vecMax);

	// 2004
	int		CullAABB( const D3DVECTOR& rMin, const D3DVECTOR& rMax );
	int		CullAABBObj( const D3DVECTOR& rMin, const D3DVECTOR& rMax );
};

#endif