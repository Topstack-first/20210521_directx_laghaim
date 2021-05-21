#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

class CLand;
class CFrustum;

class CQuadTreeNode
{
public:
	CQuadTreeNode();
	virtual ~CQuadTreeNode();

	void		Init(int LeftX, int LeftZ, int RightX, int RightZ, int Level);
	void		MakeInitStruct(int nLevle, int nDepth);
	void		Culling(CFrustum *pFrustum);
	void		PropagateInsideFlag(CQuadTreeNode *pQuadTreeNode);
	DWORD		CalColorFactor(DWORD dwColor, float fColorFactor);

	void		SetCenterPos(float x, float z)
	{
		m_center_x = x;
		m_center_z = z;
	};
	void		SetDevice(LPDIRECT3DDEVICE7 lpd3dDevice)
	{
		m_pd3dDevice = lpd3dDevice;
	};

	void		Render();

public:
	static LPDIRECT3DDEVICE7 m_pd3dDevice;
	static CLand *m_pLand;							// 전역 정보를 유지하기 위해서.
	static D3DLVERTEX BaseTri[4];
	static float m_center_x, m_center_z;

protected:
	CQuadTreeNode	*m_pLeftTop;
	CQuadTreeNode	*m_pLeftBottom;
	CQuadTreeNode	*m_pRightTop;
	CQuadTreeNode	*m_pRightBottom;

	float		fmin[3];
	float		fmax[3];

	int			m_bVisibleState;
	int			m_nLevel;
};

#endif // _QUADTREENODE_H_
