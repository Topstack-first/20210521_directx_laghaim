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
	// ���� Ȯ�� �ý���
	BYTE	m_byShare;
	int		m_nShareNum;

public:
	C3d_PosPoint();
	~C3d_PosPoint();
	void Init();
	void DeleteRes();
	void LoadRes();
	void LoadRes(int img_cx, int img_cy);
	void Rotate_Matrix(D3DMATRIX& m, float Angl); // Y�� ȸ��
	void Identity_Matrix( D3DMATRIX& m); // �����࿭
	BOOL Vertex_Matrix_Multiply(D3DTLVERTEX& Vert,D3DMATRIX& m);
	void OriginRotate(float tan_val) ; // ȸ���� ���� �Է¹޾� ���� ȸ���ǽ��Ѵ�
	void MoveMat(int xpos, int ypos);  // ����ȸ���� ������ ���ؽ����� ��ǥ�� ���ϴ� ��ġ�� �̵��Ѵ�
	void RenderOj(int xpos, int ypos, float tan_val);

	// nate 2004 - 3
	// ���� Ȯ�� �ý���
	void RenderOj(int xpos, int ypos, float tan_val, int textr_id, BYTE byShare = 0, int nShareNum = 0 );

	void RenderOj_Fade(int xpos, int ypos, float tan_val, int textr_id);
	void RenderGuildPos( int xpos, int ypos, float ani_val, int textr_id ); ///�̴ϸʱ���ã��3D

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