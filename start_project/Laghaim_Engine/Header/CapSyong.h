#ifndef __CAPSYONG_H__
#define __CAPSYONG_H__

#include <string>
#include "Config.h"
#include "Def.h"
#include "MemoryPoolBase.h"

#define E_VERTEX_MATRIX_MULTIPLY	(-1)
#define E_NO_MATRIX					(-2)

#define MAX_LOLO_VERT		1000
#define SWING_FRAME			80

#define LINE_EFF_NONE			0
#define LINE_EFF_LIGHTSWORD		1
#define LINE_EFF_GUN			2
#define LINE_EFF_BRIGHT			3
#define LINE_EFF_BLEED			4

#define LINE_TYPE_BRIGHT		0
#define LINE_TYPE_FIRE			1
#define LINE_TYPE_SMOKE			2
#define LINE_TYPE_BLUEFIRE		3
#define LINE_TYPE_VOLCANO		4
#define LINE_TYPE_POISON		5
#define LINE_TYPE_GAS			6
#define LINE_TYPE_ELECTRIC		7	// bossraid �޸ռ� �߾� �Ϸ�Ʈ�� �̹��� ����Ʈ
#define LINE_TYPE_CLYSTAL_RED	8	// bossraid ī�̸��� ���� ũ����Ż
#define LINE_TYPE_CLYSTAL_WINEB	9	// bossraid ī�̸��� ���λ� ũ����Ż ū�� 
#define LINE_TYPE_CLYSTAL_WINES	10	// bossraid ī�̸��� ���λ� ũ����Ż ������ 
#define LINE_TYPE_CLYSTAL_CYON	11	// bossraid ī�̸��� ���λ� ũ����Ż
#define LINE_TYPE_WATERFALLD	12	// bossraid ī�̸��� ���� ����Ʈ
#define LINE_TYPE_WATERFALL		13	// bossraid ī�̸��� ���� �� Ƣ�� ����Ʈ
#define LINE_TYPE_PORTAL		14	// bossraid �������� �Ա� ��Ż ����Ʈ
#define LINE_TYPE_FIRESMOKE		15  // bossraid �������� ���� ����Ʈ

#define CSY_EFF_NONE			0
#define CSY_EFF_SKIN			1	// ������� ����.
#define CSY_EFF_ONEONE			2

#define FVF_LAGHAIM (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct CVF
{
	D3DVALUE x;
	D3DVALUE y;
	D3DVALUE z;

	D3DVALUE nx;
	D3DVALUE ny;
	D3DVALUE nz;

	D3DCOLOR  diffuse;
	D3DVALUE tu, tv;
} LAGVERTEX;

#define MAX_BPD_WEIGHT_NUM      4 // �����ϴ� ����ġ ���� ///CSY2

struct sPhysique ///CSY2
{
	BYTE UseBpdNum; // ����� �� ����
	D3DVECTOR LocalPosFromBone[MAX_BPD_WEIGHT_NUM];  // �� �������� ��� ��ġ.(skin TM �� ��� ��)
	D3DVECTOR LocalNormalFromBone[MAX_BPD_WEIGHT_NUM];  // �� �������� ��� �븻.(skin TM �� ��� ��)
	short BpdIndex[MAX_BPD_WEIGHT_NUM];
	float Weight[MAX_BPD_WEIGHT_NUM];
};

class IndexedTexture;

typedef struct CVertexTemp
{
	D3DVERTEX	m_Vert;
	short	m_Int;
} VertexTemp;

class CAniMatrix
{
public:
	CAniMatrix();
	~CAniMatrix();	

	D3DMATRIX *m_Mats;
};

class CBiped : public MemoryPoolBase<CBiped>
{
public:
	std::string m_filename;

public:
	CBiped();
	~CBiped();

	void Read( PFILE *fp, BOOL decode );
	int  Read( char* szFileName );

	int m_AnimCount;
	int m_BipedNum;
	BOOL m_bHalfFrame;

	CAniMatrix *m_AniMat;
};

class CCsyObj : public MemoryPoolBase<CCsyObj>
{
public:
	CCsyObj();
	~CCsyObj();

	int m_TextureNum;
	int m_CsyEffect;

	int m_VertexCount;
	int m_FaceCount;
	int m_FaceCountLow;
	LAGVERTEX *m_Vertices0;
	LAGVERTEX *m_Vertices;
	short *m_BipedIdx;
	sPhysique *m_Physique; ///CSY2
	unsigned short *m_Indexed;
	unsigned short *m_IndexedLow;

};

class CCapsyong : public MemoryPoolBase<CCapsyong>
{
public:
	std::string m_filename;

	int m_effect_frame;


public:
	CCapsyong(IndexedTexture *pTexture);
	~CCapsyong();


	int Read(PFILE *fp, BOOL decode);
	int Read(char* szFileName);
	int Animate(CBiped *pBiped, int CurFrame, BOOL bTrace = FALSE, int whatEff = LINE_EFF_NONE, D3DMATRIX *pWorldMat = NULL, D3DMATRIX *pViewMat = NULL,
				D3DVECTOR *pTargetVector = NULL, int csyEff = CSY_EFF_NONE);		
	
	void Render( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum = -1, BOOL bPicking = FALSE, BOOL bShowLow = FALSE, LPDIRECTDRAWSURFACE7 traceTexture = NULL,
				 int effFrame = -1, int renderopt = 0,bool is_old=true);

	void RenderUnit( LPDIRECT3DDEVICE7 pd3dDevice, int UnitNum, int textureNum = -1 );
	void RenderBound( LPDIRECT3DDEVICE7 pd3dDevice );
	void RestoreTextures( LPDIRECT3DDEVICE7 pd3dDevice );
	void RenderOutline(LPDIRECT3DDEVICE7 pd3dDevice, int textureNum, float width, DWORD color, IndexedTexture* pIT = NULL);

	IndexedTexture *m_pTexture;
	int m_ObjectCount;
	BOOL m_bCsy2; ///CSY2
	CCsyObj *m_Csy;
	BOOL m_bLightEffect;

	int m_LineBiped[2];
	D3DVECTOR *m_LineVector0;
	D3DVECTOR *m_LineVector;
	int m_LineTextureNum;

	D3DVECTOR m_RectVector[4];

	int m_BoundNum;
	int m_BoundVertNum;
	D3DVERTEX *m_BoundVert;
	int m_BoundFaceNum;
	unsigned short *m_BoundIndex;
};

class CLoloObj : public MemoryPoolBase<CLoloObj>
{
public:
	CLoloObj();
	~CLoloObj();

	void Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, float height, BOOL bPicking, IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha);
	void Render2( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum, int CurFrame, float height, BOOL bPicking, IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha); ///�ѷ��ؽ��ĺ��� �� ���� �׸��� �Լ��� �߰�. �����ε��ϸ� ������ �����Ұ� ���Ƽ� �̸��� �޸��ߴ�. ������ ������ ������ �Է��� �ؽ��ĸ� �ѷο� �ݿ��ϱ� ���� �߰�. 07-07-04 ����
	void RenderRaw( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, float height, BOOL bPicking, IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha);

	int m_bAlphablend;
	int m_AnimCount;
	int m_TextureNum;

	int m_VertexCount;
	int m_FaceCount;
	int m_FaceCountLow;
	LAGVERTEX **m_Vertices;
	unsigned short *m_Indexed;
	unsigned short *m_IndexedLow;

	D3DVECTOR *m_CenterPoint;
	D3DVECTOR m_CenterWorld;
};

class CLolosBase
{
public:
	std::string m_filename;


public:
	CLolosBase();
	CLolosBase(IndexedTexture *pTexture);
	virtual ~CLolosBase();

	void SetIndexedTexture(IndexedTexture *pTexture);
	int Read( PFILE *fp, BOOL bAlpha = FALSE, BOOL bLoadWalkPlane = FALSE, BOOL decode = FALSE );
	int Read( char* szFileName, BOOL bAlpha = FALSE, BOOL bLoadWalkPlane = FALSE );	
	void Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame = 0, BOOL bPicking = FALSE, BOOL bNoRoof = FALSE, BOOL bShowLow = FALSE, D3DMATRIX *pWorldMat = NULL,
				 D3DMATRIX *pViewMat = NULL, BOOL bSort = FALSE, BOOL bNoLight = FALSE, DWORD dwAlpha = 0xff);
	void Render2( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum = -1, int CurFrame = 0 , BOOL bPicking = FALSE
				  , BOOL bNoRoof = FALSE, BOOL bShowLow = FALSE, D3DMATRIX *pWorldMat = NULL, D3DMATRIX *pViewMat = NULL, BOOL bSort = FALSE
						  , BOOL bNoLight = FALSE, DWORD dwAlpha = 0xff ); ///�ѷ��ؽ��ĺ��� �� ���� �׸��� �Լ��� �߰�. �����ε��ϸ� ������ �����Ұ� ���Ƽ� �̸��� �޸��ߴ�. ������ ������ ������ �Է��� �ؽ��ĸ� �ѷο� �ݿ��ϱ� ���� �߰�. 07-07-04 ����

	void RenderRaw( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame = 0, BOOL bPicking = FALSE, BOOL bNoRoof = FALSE, BOOL bShowLow = FALSE, D3DMATRIX *pWorldMat = NULL,
					D3DMATRIX *pViewMat = NULL, BOOL bSort = FALSE, BOOL bNoLight = FALSE, DWORD dwAlpha = 0xff );

	void RestoreTextures( LPDIRECT3DDEVICE7 pd3dDevice );

	BOOL m_bMustRenderLast;
	BOOL MustRenderLast()
	{
		return (m_bMustRenderLast || (m_LineVector0 && (m_LineType == LINE_TYPE_BRIGHT)));
	}
	IndexedTexture *m_pTexture;

	int m_AnimCount;

	int m_LoloObjCount;
	CLoloObj *m_LoloObjs;
	int m_LoloRoofCount;
	CLoloObj *m_LoloRoofs;

	int m_LineType;
	int m_LineTextureNum;
	D3DVECTOR *m_LineVector0;

	D3DVECTOR m_CenterPoint;
	D3DVECTOR m_BoundVect[8];

	int m_FloorCount;
	D3DVECTOR *m_FloorVect;
	int m_InDoorCount;
	D3DVECTOR *m_InDoorVect;
	int m_OutDoorCount;
	D3DVECTOR *m_OutDoorVect;
	int	m_WalkPlaneCount;
	D3DVECTOR *m_WalkPlaneVect;
};

class CLolos : public CLolosBase, public MemoryPoolBase<CLolos>
{
public:
	CLolos();
	CLolos(IndexedTexture *pTexture);
	~CLolos();
};

HRESULT NKMath_VectorMatrixMultiply( D3DVECTOR& vDest, D3DVECTOR& vSrc, D3DMATRIX& mat);
HRESULT NKMath_LVertexMatrixMultiply( D3DLVERTEX& vDest, D3DLVERTEX& vSrc, D3DMATRIX& mat);
HRESULT NKMath_VertexMatrixMultiply( D3DVERTEX& vDest, D3DVERTEX& vSrc, D3DMATRIX& mat);
HRESULT NKMath_LagVertexMatrixMultiply( LAGVERTEX& vDest, LAGVERTEX& vSrc, D3DMATRIX& mat);

#define EDDY_FRAME		40

#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define PLUS(a)		((a) > 0 ? (a) : (-(a)))
void Decode(unsigned char* stream, int filesize);
#endif // __CAPSYONG_H__
