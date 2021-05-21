#ifndef _CLOUD_SEA_H_
#define _CLOUD_SEA_H_

#include "Config.h"

#define CLOUDSEA_NOISE_NUM	11

class CLand;
class CFrustum;

class CCloudSea
{
protected:
	CCloudSea	*m_pLeftTop;
	CCloudSea	*m_pLeftBottom;
	CCloudSea	*m_pRightTop;
	CCloudSea	*m_pRightBottom;

	float		fmin[3];
	float		fmax[3];

	int			m_bVisibleState;
	int			m_nLevel;


public:
	static D3DLVERTEX BaseTri[4];
	static float m_center_x, m_center_z;
	static BOOL		m_bCanDoModulate2X;
	static DWORD	m_SkyFrame;
	static DWORD	m_CloudFrame;
	static float	m_SNoise[CLOUDSEA_NOISE_NUM];
	static float	m_CNoise[CLOUDSEA_NOISE_NUM];
//==================================================
	// nate 2004 - 7
	// Image Manager
//	static int		m_textrSky, m_textrCloud;
//==================================================
	//static D3DLVERTEX TriArray[MAX_VISIBLE_NODE*6];

	CCloudSea();
	virtual ~CCloudSea();

	void		Init(int LeftX, int LeftZ, int RightX, int RightZ, int Level);
	void		MakeInitStruct(int nLevle, int nDepth);
	void		Culling(CFrustum *pFrustum);
	void		PropagateInsideFlag(CCloudSea *pQuadTreeNode);

	void		SetCenterPos(float x, float z)
	{
		m_center_x = x;
		m_center_z = z;
	};
	void		SetDevice() {};
	void		RenderMain();
	void		SetFog(BOOL bFog);
	void		MakeNoise();
//==================================================
	// nate 2004 - 7
	// Image Manager
//	void		LoadTexture();
//==================================================

	// CLand 에서 가져온것...
	void		DoModulate2X(BOOL bCanDoModulate2X)
	{
		m_bCanDoModulate2X = bCanDoModulate2X;
	};
	BOOL		CanDoModulate2X()
	{
		return m_bCanDoModulate2X;
	};
	void		AddVisibleList(int idx, float farWaterFactor = 0.0f);
	void		QuadInit(CFrustum *pFrustum, float x, float z);					// 중심 점..
	void		Render(float x, float z, BOOL bFog);
	////

};

#endif // _CLOUD_SEA_H_
