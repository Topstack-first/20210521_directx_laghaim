#ifndef _SFX_BASE_SHAPE_H_
#define _SFX_BASE_SHAPE_H_

#include "Config.h"
#include "MemoryPoolBase.h"

class CTriPlane : public MemoryPoolBase<CTriPlane>
{
	LPD3DLVERTEX			m_TriPlane;

	D3DMATRIX				matWorld;

	D3DMATERIAL7			mtrl;

	int						max_triplane_vertexs;


public:
	CTriPlane();
	virtual ~CTriPlane();

	void Create(float width, float height, bool bothFaces);
	void SetRotationX(float angle)
	{
		D3DUtil_SetRotateXMatrix(matWorld, angle);
	}
	void SetRotationY(float angle)
	{
		D3DUtil_SetRotateYMatrix(matWorld, angle);
	}
	void SetRotationZ(float angle)
	{
		D3DUtil_SetRotateZMatrix(matWorld, angle);
	}
	void RotateX(float angle)
	{
		D3DMATRIX		matRx;
		D3DUtil_SetRotateXMatrix(matRx, angle);
		D3DMath_MatrixMultiply(matWorld, matRx, matWorld);
	}
	void RotateY(float angle)
	{
		D3DMATRIX		matRy;
		D3DUtil_SetRotateYMatrix(matRy, angle);
		D3DMath_MatrixMultiply(matWorld, matRy, matWorld);
	}
	void RotateZ(float angle)
	{
		D3DMATRIX		matRz;
		D3DUtil_SetRotateZMatrix(matRz, angle);
		D3DMath_MatrixMultiply(matWorld, matRz, matWorld);
	}

	/*	D3DVALUE	_11, _12, _13, _14;
	    D3DVALUE	_21, _22, _23, _24;
	    D3DVALUE	_31, _32, _33, _34;
	    D3DVALUE	_41, _42, _43, _44;

		float		m00, m01, m02, m03;
	    float		m10, m11, m12, m13;
	    float		m20, m21, m22, m23;
	    float		m30, m31, m32, m33; */

	void SetScaling(float sx, float sy, float sz)
	{
		matWorld._11 = sx, matWorld._22 = sy, matWorld._33 = sz;
	} //{ D3DUtil_SetScaleMatrix(matWorld, sx, sy, sz); }
	void Scale(float sx, float sy, float sz)
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, sx, sy, sz);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}
	void SetTranslationXYZ(float x, float y, float z)
	{
		matWorld._41 = x, matWorld._42 = y, matWorld._43 = z;
	}
//	void SetColor(float Red, float Green, float Blue, float Alpha) { D3DUtil_InitMaterial(mtrl, Red, Green, Blue, Alpha); }
	void SetColor(float Red, float Green, float Blue, float Alpha)
	{
		for(int i = 0; i < max_triplane_vertexs; i++)
			m_TriPlane[i].color = D3DRGBA(Red, Green, Blue, Alpha);
	}
	void SetBillBoard(D3DVECTOR target, bool onlyRotateY, float rxBillBoard)
	{
		BillBoard( &matWorld, target, onlyRotateY, rxBillBoard );
	}

	void Render();
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CRectPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

class CRectPlane : public MemoryPoolBase<CRectPlane>
{
	LPD3DLVERTEX			m_RectPlane;

	D3DMATRIX				matWorld;

	D3DMATERIAL7			mtrl;

	int						max_rectplane_vertexs;


public:
	CRectPlane();
	virtual ~CRectPlane();

	void Create(float width, float height, bool bothFaces);
	void Create(int frame, float width, float height, float cap, bool bTemp = FALSE);
	void Create(float width, float height, float distance, bool bothFaces);
	void ReCreate(float zangle);
	void ChangeVertexInfo( int Index, D3DLVERTEX vertex ); // 버택스 정보를 조정해주는 함수.(06-03-09 원석) ///페어리 시 추가

	void SetRotationX(float angle)
	{
		D3DUtil_SetRotateXMatrix(matWorld, angle);
	}
	void SetRotationY(float angle)
	{
		D3DUtil_SetRotateYMatrix(matWorld, angle);
	}
	void SetRotationZ(float angle)
	{
		D3DUtil_SetRotateZMatrix(matWorld, angle);
	}
	void RotateX(float angle)
	{
		D3DMATRIX		matRx;
		D3DUtil_SetRotateXMatrix(matRx, angle);
		D3DMath_MatrixMultiply(matWorld, matRx, matWorld);
	}
	void RotateY(float angle)
	{
		D3DMATRIX		matRy;
		D3DUtil_SetRotateYMatrix(matRy, angle);
		D3DMath_MatrixMultiply(matWorld, matRy, matWorld);
	}
	void RotateZ(float angle)
	{
		D3DMATRIX		matRz;
		D3DUtil_SetRotateZMatrix(matRz, angle);
		D3DMath_MatrixMultiply(matWorld, matRz, matWorld);
	}
	void RotateAxis(D3DVECTOR axis,float angle)
	{
		D3DMATRIX		matR;
		D3DUtil_SetRotationMatrix(matR, axis, angle);
		D3DMath_MatrixMultiply(matWorld, matR, matWorld);
	} ///플레임스피어 때 추가.
	void SetScaling(float sx, float sy, float sz)
	{
		matWorld._11 = sx, matWorld._22 = sy, matWorld._33 = sz;
	} //{ D3DUtil_SetScaleMatrix(matWorld, sx, sy, sz); }
	void Scale(float sx, float sy, float sz)
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, sx, sy, sz);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}
	void SetTranslationXYZ(float x, float y, float z)
	{
		matWorld._41 = x, matWorld._42 = y, matWorld._43 = z;
	}
//	void SetColor(float Red, float Green, float Blue, float Alpha) { D3DUtil_InitMaterial(mtrl, Red, Green, Blue, Alpha); }
	void SetColor(float Red, float Green, float Blue, float Alpha)
	{
		for(int i = 0; i < max_rectplane_vertexs; i++)
			m_RectPlane[i].color = D3DRGBA(Red, Green, Blue, Alpha);
	}
	void SetColor(DWORD color)
	{
		for(int i = 0; i < max_rectplane_vertexs; i++)
			m_RectPlane[i].color = color;
	}
	void SetBillBoard(D3DVECTOR target, bool onlyRotateY, float rxBillBoard)
	{
		BillBoard( &matWorld, target, onlyRotateY, rxBillBoard );
	}

	void Render();
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CRoundPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

class CRoundPlane : public MemoryPoolBase<CRoundPlane>
{
	LPD3DLVERTEX			m_RoundPlane;

	D3DMATRIX				matWorld;

	D3DMATERIAL7			mtrl;

	int						max_roundplane_vertexs;


public:
	CRoundPlane();
	virtual ~CRoundPlane();

	void Create(float radius, float depth, int division);

	void SetRotationX(float angle)
	{
		D3DUtil_SetRotateXMatrix(matWorld, angle);
	}
	void SetRotationY(float angle)
	{
		D3DUtil_SetRotateYMatrix(matWorld, angle);
	}
	void SetRotationZ(float angle)
	{
		D3DUtil_SetRotateZMatrix(matWorld, angle);
	}
	void RotateX(float angle)
	{
		D3DMATRIX		matRx;
		D3DUtil_SetRotateXMatrix(matRx, angle);
		D3DMath_MatrixMultiply(matWorld, matRx, matWorld);
	}
	void RotateY(float angle)
	{
		D3DMATRIX		matRy;
		D3DUtil_SetRotateYMatrix(matRy, angle);
		D3DMath_MatrixMultiply(matWorld, matRy, matWorld);
	}
	void RotateZ(float angle)
	{
		D3DMATRIX		matRz;
		D3DUtil_SetRotateZMatrix(matRz, angle);
		D3DMath_MatrixMultiply(matWorld, matRz, matWorld);
	}
	void SetScaling(float sx, float sy, float sz)
	{
		matWorld._11 = sx, matWorld._22 = sy, matWorld._33 = sz;
	} //{ D3DUtil_SetScaleMatrix(matWorld, sx, sy, sz); }
	void Scale(float sx, float sy, float sz)
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, sx, sy, sz);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}
	void SetTranslationXYZ(float x, float y, float z)
	{
		matWorld._41 = x, matWorld._42 = y, matWorld._43 = z;
	}
//	void SetColor(float Red, float Green, float Blue, float Alpha) { D3DUtil_InitMaterial(mtrl, Red, Green, Blue, Alpha); }
	void SetColor(float Red, float Green, float Blue, float Alpha)
	{
		for(int i = 0; i < max_roundplane_vertexs; i++)
			m_RoundPlane[i].color = D3DRGBA(Red, Green, Blue, Alpha);
	}
	void SetBillBoard(D3DVECTOR target, bool onlyRotateY, float rxBillBoard)
	{
		BillBoard( &matWorld, target, onlyRotateY, rxBillBoard );
	}

	void Render();
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CCylinderPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

class CCylinderPlane : public MemoryPoolBase<CCylinderPlane>
{
	LPD3DLVERTEX			m_CylinderPlane;

	D3DMATRIX				matWorld;

	D3DMATERIAL7			mtrl;

	int						max_cylinderplane_vertexs;


public:
	CCylinderPlane();
	virtual ~CCylinderPlane();

	void Create(float topRadius, float bottomRadius, float depth, int division, bool bothFaces, float pattern_tu = 1.0f);
	void Create(float topRadius, float bottomRadius, float depth, int division, float distance, bool bothFaces, float pattern_tu = 1.0f);
	void Create2(float innerRadius, float outerRadius, float depth, int division, int revolution, float gapStep, float radiusStep, float heightStep, float pattern_tu = 1.0f);
	void ReCreate(float topRadius, float bottomRadius, float depth, int division, float distance, float pattern_tu = 1.0f);
	void SkyCreate(float topRadius, float bottomRadius, float depth, int division, float distance, float pattern_tu);

	void SetRotation( D3DMATRIX	matR )
	{
		matWorld = matR;    // 외부에서 회전행렬을 셋팅할 수 있도록...(짐벌락 때문에) ///플레임 스피어 때 추가
	}
	void SetRotationX(float angle)
	{
		D3DUtil_SetRotateXMatrix(matWorld, angle);
	}
	void SetRotationY(float angle)
	{
		D3DUtil_SetRotateYMatrix(matWorld, angle);
	}
	void SetRotationZ(float angle)
	{
		D3DUtil_SetRotateZMatrix(matWorld, angle);
	}
	void RotateX(float angle)
	{
		D3DMATRIX		matRx;
		D3DUtil_SetRotateXMatrix(matRx, angle);
		D3DMath_MatrixMultiply(matWorld, matRx, matWorld);
	}
	void RotateY(float angle)
	{
		D3DMATRIX		matRy;
		D3DUtil_SetRotateYMatrix(matRy, angle);
		D3DMath_MatrixMultiply(matWorld, matRy, matWorld);
	}
	void RotateZ(float angle)
	{
		D3DMATRIX		matRz;
		D3DUtil_SetRotateZMatrix(matRz, angle);
		D3DMath_MatrixMultiply(matWorld, matRz, matWorld);
	}
	void SetScaling(float sx, float sy, float sz)
	{
		matWorld._11 = sx, matWorld._22 = sy, matWorld._33 = sz;
	} //{ D3DUtil_SetScaleMatrix(matWorld, sx, sy, sz); }
	void Scale(float sx, float sy, float sz)
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, sx, sy, sz);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}
	void SetTranslationXYZ(float x, float y, float z)
	{
		matWorld._41 = x, matWorld._42 = y, matWorld._43 = z;
	}
//	void SetColor(float Red, float Green, float Blue, float Alpha) { D3DUtil_InitMaterial(mtrl, Red, Green, Blue, Alpha); }
	void SetColor(float Red, float Green, float Blue, float Alpha)
	{
		for(int i = 0; i < max_cylinderplane_vertexs; i++)
			m_CylinderPlane[i].color = D3DRGBA(Red, Green, Blue, Alpha);
	}

	void SetBillBoard(D3DVECTOR target, bool onlyRotateY, float rxBillBoard)
	{
		BillBoard( &matWorld, target, onlyRotateY, rxBillBoard );
	}

	void Render();
	void Render2();
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CSphere Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

#define FULL_SPHERE 1
#define HALF_OF_HEIGTH_SPHERE 2
#define HALF_OF_WIDTH_SPHERE 3


class CSphere : public MemoryPoolBase<CSphere>
{
	LPD3DLVERTEX			m_Sphere;

	D3DMATRIX				matWorld;

	D3DMATERIAL7			mtrl;

	int						max_sphere_vertexs;


public:
	CSphere();
	virtual ~CSphere();

	void Create();
	void SkyCreate(int division, float ring_TileTex, float seg_TileTex);

	void SetRotationX(float angle)
	{
		D3DUtil_SetRotateXMatrix(matWorld, angle);
	}

	void SetRotationY(float angle)
	{
		D3DUtil_SetRotateYMatrix(matWorld, angle);
	}

	void SetRotationZ(float angle)
	{
		D3DUtil_SetRotateZMatrix(matWorld, angle);
	}

	void RotateX(float angle)
	{
		D3DMATRIX		matRx;
		D3DUtil_SetRotateXMatrix(matRx, angle);
		D3DMath_MatrixMultiply(matWorld, matRx, matWorld);
	}

	void RotateY(float angle)
	{
		D3DMATRIX		matRy;
		D3DUtil_SetRotateYMatrix(matRy, angle);
		D3DMath_MatrixMultiply(matWorld, matRy, matWorld);
	}

	void RotateZ(float angle)
	{
		D3DMATRIX		matRz;
		D3DUtil_SetRotateZMatrix(matRz, angle);
		D3DMath_MatrixMultiply(matWorld, matRz, matWorld);
	}

	void SetScaling(float sx, float sy, float sz)
	{
		matWorld._11 = sx;
		matWorld._22 = sy;
		matWorld._33 = sz;
	}

	void Scale(float sx, float sy, float sz)
	{
		D3DMATRIX		matS;
		D3DUtil_SetScaleMatrix(matS, sx, sy, sz);
		D3DMath_MatrixMultiply(matWorld, matS, matWorld);
	}

	void SetTranslationXYZ(float x, float y, float z)
	{
		matWorld._41 = x, matWorld._42 = y, matWorld._43 = z;
	}

	void SetColor(float Red, float Green, float Blue, float Alpha)
	{
		for(int i = 0; i < max_sphere_vertexs; i++)
		{
			m_Sphere[i].color = D3DRGBA(Red, Green, Blue, Alpha);
		}
	}

	void SetColor(DWORD color)
	{
		for(int i = 0; i < max_sphere_vertexs; i++)
		{
			m_Sphere[i].color = color;
		}
	}

	void SetBillBoard(D3DVECTOR target, bool onlyRotateY, float rxBillBoard)
	{
		BillBoard( &matWorld, target, onlyRotateY, rxBillBoard );
	}

	void trans_texture(float tu)
	{
		int i = 0;
		for( i = 0; i < max_sphere_vertexs; i++)
		{
			m_Sphere[i].tu += tu;

			if( m_Sphere[i].tu > 1.0f )
				m_Sphere[i].tu = 1.0f - m_Sphere[i].tu;
		}
	}

	void Render();
	void SkyRender();
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// CTailPlane Class Method
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Tail_Info
{
	LPD3DVECTOR				pTailPos;

	int						curTailPosNum;

	bool					end_Tail;

} Tail_Info;


class CTailPlane : public MemoryPoolBase<CTailPlane>
{
	LPD3DLVERTEX			m_TailPlane;

	D3DMATRIX				m_matWorld;

	int						max_tailplane_vertexs;
	int						max_tailpos_vectors;

	float 					m_headsize;
	float 					m_tailsize;
	int						m_pattern_tu;

	bool					m_bIsFade;

public:
	CTailPlane();
	virtual ~CTailPlane();

	void Create(int division, float head_size, float tail_size, float pattern_tu, bool bIsFade = TRUE);

	void AddTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR addPos, D3DVECTOR viewVec, float org_red, float org_green, float org_blue);
	void SubTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR viewVec, float org_red, float org_green, float org_blue);
	void ReSetAddTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR viewVec, float org_red, float org_green, float org_blue);
	void ReSetSubTailPos(LPD3DVECTOR TailPos, int *CurTailPosNum, D3DVECTOR viewVec, float org_red, float org_green, float org_blue);

	void SetTranslationXYZ(float x, float y, float z)
	{
		m_matWorld._41 = x, m_matWorld._42 = y, m_matWorld._43 = z;
	}

	void Render(int *CurTailPosNum);
};

#endif // _SFX_BASE_SHAPE_H_








