#ifndef _3D_POS_POINT_H_
#define _3D_POS_POINT_H_

#include "Config.h"

#include "dxutil.h"
#include "ddutil.h"
#include "sheet.h"


#define VERTEX_CNT	4


class CSheet;
class IndexedTexture;

class C3d_PosPoint
{
protected:

	D3DTLVERTEX				TLVec[VERTEX_CNT];

	int				m_ImgWidth, m_ImgHeight;

	// nate 2004 - 3
	// 숫자 확인 시스템
	BYTE	m_byShare;
	int		m_nShareNum;

public:
	C3d_PosPoint();
	~C3d_PosPoint();
	void Init();
	void DeleteRes();
	void LoadRes();
	void LoadRes(int img_cx, int img_cy);
	void Rotate_Matrix(D3DMATRIX& m, float Angl); // Y축 회전
	void Identity_Matrix( D3DMATRIX& m); // 단위행열
	BOOL Vertex_Matrix_Multiply(D3DTLVERTEX& Vert,D3DMATRIX& m);
	void OriginRotate(float tan_val) ; // 회전될 각을 입력받아 원점 회전실시한다
	void MoveMat(int xpos, int ypos);  // 원점회전된 각각의 버텍스들의 좌표를 원하는 위치로 이동한다
	void RenderOj(int xpos, int ypos, float tan_val);

	// nate 2004 - 3
	// 숫자 확인 시스템
	void RenderOj(int xpos, int ypos, float tan_val, int textr_id, BYTE byShare = 0, int nShareNum = 0 );

	void RenderOj_Fade(int xpos, int ypos, float tan_val, int textr_id);
	void RenderGuildPos( int xpos, int ypos, float ani_val, int textr_id ); ///미니맵길드원찾기3D

	int GetImagWidth()
	{
		return m_ImgWidth;
	}
	int GetImsgHeight()
	{
		return m_ImgHeight;
	}
};

#endif //_3D_POS_POINT_H_