#include "stdafx.h"
#define D3D_OVERLOADS

#include <tchar.h>
#include <math.h>
#include <d3d.h>
#include "d3dxmath.h"

#include "IndexedTexture.h"
#include "VirtualFile.h"

#include "CapSyong.h"

#include "d3dmath.h"
#include "d3dutil.h"

BOOL bShowLow = FALSE;

D3DMATRIX matEddy[EDDY_FRAME];
BOOL bCalced_matEddy = FALSE;
void Calc_matEddy();
D3DVERTEX loloVert[MAX_LOLO_VERT];

const int MAX_UV_FRAME = 1280;

CAniMatrix::CAniMatrix()
{
	m_Mats = NULL;
}

CAniMatrix::~CAniMatrix()
{
	if (m_Mats)
		delete[] m_Mats;
}

CBiped::CBiped()
{
	m_AnimCount = 0;
	m_BipedNum = -1;
	m_bHalfFrame = false;
	m_AniMat = NULL;
}

CBiped::~CBiped()
{
	if (m_AniMat)
		delete[] m_AniMat;
}

CCsyObj::CCsyObj()
{
	m_TextureNum = -1;
	m_CsyEffect = CSY_EFF_NONE;
	m_VertexCount = 0;
	m_FaceCount = 0;
	m_FaceCountLow = 0;
	m_Vertices = NULL;
	m_Vertices0 = NULL;
	m_BipedIdx = NULL;
	m_Indexed = NULL;
	m_IndexedLow = NULL;
	m_Physique = NULL; ///CSY2
}

CCsyObj::~CCsyObj()
{
	if (m_Vertices0)
		delete[] m_Vertices0;
	if (m_Vertices)
		delete[] m_Vertices;
	if (m_BipedIdx)
		delete[] m_BipedIdx;
	if (m_Indexed)
		delete[] m_Indexed;
	if (m_IndexedLow)
		delete[] m_IndexedLow;
	if (m_Physique) ///CSY2
		delete[] m_Physique;
}

CCapsyong::CCapsyong(IndexedTexture *pTexture)
{
	m_bCsy2 = FALSE; ///CSY2
	m_Csy = NULL;
	m_pTexture = pTexture;
	m_LineVector0 = NULL;
	m_LineVector = NULL;
	m_LineTextureNum = -1;
	m_bLightEffect = FALSE;


	m_BoundVert = NULL;
	m_BoundIndex = NULL;
	m_BoundFaceNum = 0;
	m_BoundNum = 0;
	m_BoundFaceNum = 0;


	m_effect_frame = 0;
}


CCapsyong::~CCapsyong()
{
	if (m_Csy)
		delete[] m_Csy;
	if (m_LineVector)
		delete[] m_LineVector;
	if (m_LineVector0)
		delete[] m_LineVector0;
	if (m_BoundNum > 0)
	{
		delete[] m_BoundVert;
		delete[] m_BoundIndex;
	}
}

CLoloObj::CLoloObj()
{
	m_bAlphablend = FALSE;

	m_AnimCount = 0;
	m_TextureNum = -1;
	m_FaceCount = 0;
	m_Vertices = NULL;
	m_Indexed = NULL;
	m_CenterPoint = NULL;

	m_FaceCountLow = 0;
	m_IndexedLow = NULL;
}

CLoloObj::~CLoloObj()
{
	if (m_Vertices)
	{
		for (int idx = 0; idx < m_AnimCount; idx++)
		{
			delete[] m_Vertices[idx];
		}
		delete[] m_Vertices;
	}
	if (m_CenterPoint)
		delete[] m_CenterPoint;
	if (m_Indexed)
		delete[] m_Indexed;

	if (m_IndexedLow)
		delete[] m_IndexedLow;
}

CLolos::CLolos()
	: CLolosBase()
{
}

CLolos::CLolos(IndexedTexture *pTexture)
	: CLolosBase(pTexture)
{
}

CLolos::~CLolos()
{
}

CLolosBase::CLolosBase()
{
	m_pTexture = NULL;

	m_LoloObjCount = 0;
	m_LoloObjs = NULL;
	m_LoloRoofCount = 0;
	m_LoloRoofs = NULL;
	m_LineVector0 = NULL;
	m_LineType = LINE_TYPE_BRIGHT;
	m_bMustRenderLast = FALSE;

	m_FloorVect = NULL;
	m_InDoorVect = NULL;
	m_OutDoorVect = NULL;
	m_WalkPlaneVect = NULL;
	m_WalkPlaneCount = 0;
}

CLolosBase::CLolosBase(IndexedTexture *pTexture)
{
	m_pTexture = pTexture;

	m_LoloObjCount = 0;
	m_LoloObjs = NULL;
	m_LoloRoofCount = 0;
	m_LoloRoofs = NULL;
	m_LineVector0 = NULL;
	m_LineType = LINE_TYPE_BRIGHT;
	m_bMustRenderLast = FALSE;

	m_FloorVect = NULL;
	m_InDoorVect = NULL;
	m_OutDoorVect = NULL;
	m_WalkPlaneVect = NULL;
	m_WalkPlaneCount = 0;
}

CLolosBase::~CLolosBase()
{
	if (m_LoloObjs)
		delete[] m_LoloObjs;
	if (m_LoloRoofs)
		delete[] m_LoloRoofs;
	if (m_LineVector0)
		delete[] m_LineVector0;
	if (m_FloorVect)
		delete[] m_FloorVect;
	if (m_InDoorVect)
		delete[] m_InDoorVect;
	if (m_OutDoorVect)
		delete[] m_OutDoorVect;
	if( m_WalkPlaneVect )
		delete [] m_WalkPlaneVect;
}

void CLolosBase::SetIndexedTexture(IndexedTexture *pTexture)
{
	m_pTexture = pTexture;
}

void GetHalfMat(D3DMATRIX &mat, CBiped *pBiped, int bpd_idx, int CurFrame)
{
	mat._11 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._11 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._11) / 2;
	mat._12 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._12 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._12) / 2;
	mat._13 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._13 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._13) / 2;
	mat._14 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._14 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._14) / 2;
	mat._21 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._21 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._21) / 2;
	mat._22 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._22 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._22) / 2;
	mat._23 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._23 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._23) / 2;
	mat._24 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._24 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._24) / 2;
	mat._31 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._31 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._31) / 2;
	mat._32 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._32 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._32) / 2;
	mat._33 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._33 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._33) / 2;
	mat._34 = 0;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._34 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._34) / 2;
	mat._41 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._41 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._41) / 2;
	mat._42 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._42 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._42) / 2;
	mat._43 = (pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._43 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._43) / 2;
	mat._44 = 1.0f;//(pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]._44 + pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame+1]._44) / 2;
}


#define EDDY_HEIGHT			(15.0f)
#define EDDY_SCALE			(-0.5f)
#define LIGHTSWORD_SCALE	(0.5f)
#define LIGHTSWORD_Z		(3.0f)
#define LASERGUN_SCALE		(0.1f)
#define BLEEDSWORD_SCALE	(1.5f)

D3DMATRIX g_TempMat[100];

int CCapsyong::Animate(CBiped *pBiped, int CurFrame, BOOL bTrace, int whatEff, D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat,
					   D3DVECTOR *pTargetVector, int csyEff)
{
	D3DMATRIX *pLineMat, *pLineMat2;
	D3DVECTOR lineVector, rectVector;
	int oldCurFrame = CurFrame;
	int bpd_idx;

	// Biped를 이용한 Animation, Biped가 없는 경우는 회오리 칼이라고 사료되옵니다....
	if (pBiped)
	{
		CurFrame %= pBiped->m_AnimCount;
		// 중간 동작을 만드는데, Frame이 홀수이고 마지막 숫자만 아니면 중간 동작을 계산해서 쓴다.
		if (pBiped->m_bHalfFrame && CurFrame % 2)
		{
			CurFrame /= 2;

			// 중간 프레임을 위한 Matrix를 만든다.
			for (bpd_idx = 0; bpd_idx < pBiped->m_BipedNum; bpd_idx++)
			{
				GetHalfMat(g_TempMat[bpd_idx], pBiped, bpd_idx, CurFrame);
			}

			// Vertex들을 Animation 시킨다.
			for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++)
			{
				for (int i = 0; i < m_Csy[csy_idx].m_VertexCount; i++)
				{
					if( m_bCsy2 && m_Csy[csy_idx].m_Physique ) ///CSY2
					{
						D3DVECTOR TempVecSrc; // 이 녀석엔 원본 벡터를 담아둔다.(LAGVERTEX엔 쓸모없는 데이터도 있기때문)(혹시 노말도 스킨 계산을 해야한다면 LAGVERTEX로 바꾸자)
						TempVecSrc.x = m_Csy[csy_idx].m_Vertices0[i].x;
						TempVecSrc.y = m_Csy[csy_idx].m_Vertices0[i].y;
						TempVecSrc.z = m_Csy[csy_idx].m_Vertices0[i].z;
						D3DVECTOR TempVecSrc2 = D3DVECTOR(0.0f,0.0f,0.0f); // 이 녀석엔 노말라이즈 이전의 계산된 노말 벡터를 담아둔다.(LAGVERTEX엔 쓸모없는 데이터도 있기때문)(혹시 노말도 스킨 계산을 해야한다면 LAGVERTEX로 바꾸자)


						m_Csy[csy_idx].m_Vertices[i].x = 0.0f; // 이녀석을 초기화 해줘야한다.
						m_Csy[csy_idx].m_Vertices[i].y = 0.0f;
						m_Csy[csy_idx].m_Vertices[i].z = 0.0f;

						for( int j = 0 ; j < m_Csy[csy_idx].m_Physique[i].UseBpdNum ; ++j )
						{
							D3DVECTOR TempVecDest = D3DVECTOR( 0.0f, 0.0f, 0.0f ); // 임시로 쓸 벡터
							D3DVECTOR TempVecDest2 = D3DVECTOR( 0.0f, 0.0f, 0.0f ); // 임시로 쓸 벡터

							bpd_idx = m_Csy[csy_idx].m_Physique[i].BpdIndex[j];
							if (bpd_idx >= pBiped->m_BipedNum)
								bpd_idx = pBiped->m_BipedNum - 1;

							/////////////////////////////////////////////////////////
							// 피직 버택스 계산
							NKMath_VectorMatrixMultiply( TempVecDest, m_Csy[csy_idx].m_Physique[i].LocalPosFromBone[j], g_TempMat[bpd_idx] );

							// 가중치를 적용시킨다.
							TempVecDest.x *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest.y *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest.z *= m_Csy[csy_idx].m_Physique[i].Weight[j];

							// 그리고 누적
							m_Csy[csy_idx].m_Vertices[i].x += TempVecDest.x;
							m_Csy[csy_idx].m_Vertices[i].y += TempVecDest.y;
							m_Csy[csy_idx].m_Vertices[i].z += TempVecDest.z;

							/////////////////////////////////////////////////////////
							// 노말 계산
							D3DMATRIX matRotateOnly = g_TempMat[bpd_idx];
							matRotateOnly._41 = 0.0f;
							matRotateOnly._42 = 0.0f;
							matRotateOnly._43 = 0.0f;

							NKMath_VectorMatrixMultiply( TempVecDest2, m_Csy[csy_idx].m_Physique[i].LocalNormalFromBone[j], matRotateOnly );

							TempVecDest2.x *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest2.y *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest2.z *= m_Csy[csy_idx].m_Physique[i].Weight[j];

							NKMath_VectorMatrixMultiply( TempVecDest2, m_Csy[csy_idx].m_Physique[i].LocalNormalFromBone[j], matRotateOnly );

							TempVecDest2.x *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest2.y *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest2.z *= m_Csy[csy_idx].m_Physique[i].Weight[j];

							// 그리고 누적
							TempVecSrc2.x += TempVecDest2.x;
							TempVecSrc2.y += TempVecDest2.y;
							TempVecSrc2.z += TempVecDest2.z;
						}
						TempVecSrc2 = Normalize(TempVecSrc2);
						m_Csy[csy_idx].m_Vertices[i].nx = TempVecSrc2.x;
						m_Csy[csy_idx].m_Vertices[i].ny = TempVecSrc2.y;
						m_Csy[csy_idx].m_Vertices[i].nz = TempVecSrc2.z;
					}
					else
					{
						bpd_idx = m_Csy[csy_idx].m_BipedIdx[i];
						if (bpd_idx >= pBiped->m_BipedNum)
							bpd_idx = pBiped->m_BipedNum - 1;
						NKMath_LagVertexMatrixMultiply(m_Csy[csy_idx].m_Vertices[i],
													   m_Csy[csy_idx].m_Vertices0[i],
													   g_TempMat[bpd_idx]);
					}
				}
			}
			// Line 계산...
			if (m_LineVector0)
			{
				NKMath_VectorMatrixMultiply(m_LineVector[0],
											m_LineVector0[0], g_TempMat[m_LineBiped[0]]);
				NKMath_VectorMatrixMultiply(m_LineVector[1],
											m_LineVector0[1], g_TempMat[m_LineBiped[1]]);
			}
		}
		else
		{
			if (pBiped->m_bHalfFrame)
				CurFrame /= 2;

			for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++)
			{
				// Vertex들을 Animation 시킨다.
				for (int i = 0; i < m_Csy[csy_idx].m_VertexCount; i++)
				{
					if( m_bCsy2 && m_Csy[csy_idx].m_Physique ) ///CSY2
					{
						D3DVECTOR TempVecSrc; // 이 녀석엔 원본 벡터를 담아둔다.(LAGVERTEX엔 쓸모없는 데이터도 있기때문)(혹시 노말도 스킨 계산을 해야한다면 LAGVERTEX로 바꾸자)
						TempVecSrc.x = m_Csy[csy_idx].m_Vertices0[i].x;
						TempVecSrc.y = m_Csy[csy_idx].m_Vertices0[i].y;
						TempVecSrc.z = m_Csy[csy_idx].m_Vertices0[i].z;
						D3DVECTOR TempVecSrc2 = D3DVECTOR(0.0f,0.0f,0.0f); // 이 녀석엔 노말라이즈 이전의 계산된 노말 벡터를 담아둔다.(LAGVERTEX엔 쓸모없는 데이터도 있기때문)(혹시 노말도 스킨 계산을 해야한다면 LAGVERTEX로 바꾸자)

						m_Csy[csy_idx].m_Vertices[i].x = 0.0f; // 이녀석을 초기화 해줘야한다.
						m_Csy[csy_idx].m_Vertices[i].y = 0.0f;
						m_Csy[csy_idx].m_Vertices[i].z = 0.0f;

						m_Csy[csy_idx].m_Vertices[i].nx = 0.0f; // 이녀석을 초기화 해줘야한다.
						m_Csy[csy_idx].m_Vertices[i].ny = 0.0f;
						m_Csy[csy_idx].m_Vertices[i].nz = 0.0f;

						for( int j = 0 ; j < m_Csy[csy_idx].m_Physique[i].UseBpdNum ; ++j )
						{
							D3DVECTOR TempVecDest = D3DVECTOR( 0.0f, 0.0f, 0.0f ); // 임시로 쓸 벡터
							D3DVECTOR TempVecDest2 = D3DVECTOR( 0.0f, 0.0f, 0.0f ); // 임시로 쓸 벡터

							bpd_idx = m_Csy[csy_idx].m_Physique[i].BpdIndex[j];
							if (bpd_idx >= pBiped->m_BipedNum)
								bpd_idx = pBiped->m_BipedNum - 1;

							if( bpd_idx < 0 )
								bpd_idx = 0;

							/////////////////////////////////////////////////////////
							// 피직 버택스 계산
							NKMath_VectorMatrixMultiply( TempVecDest, m_Csy[csy_idx].m_Physique[i].LocalPosFromBone[j], pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame] );

							// 가중치를 적용시킨다.
							TempVecDest.x *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest.y *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest.z *= m_Csy[csy_idx].m_Physique[i].Weight[j];

							// 그리고 누적
							m_Csy[csy_idx].m_Vertices[i].x += TempVecDest.x;
							m_Csy[csy_idx].m_Vertices[i].y += TempVecDest.y;
							m_Csy[csy_idx].m_Vertices[i].z += TempVecDest.z;


							/////////////////////////////////////////////////////////
							// 노말 계산
							D3DMATRIX matRotateOnly = pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame];
							matRotateOnly._41 = 0.0f;
							matRotateOnly._42 = 0.0f;
							matRotateOnly._43 = 0.0f;

							NKMath_VectorMatrixMultiply( TempVecDest2, m_Csy[csy_idx].m_Physique[i].LocalNormalFromBone[j], matRotateOnly );

							TempVecDest2.x *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest2.y *= m_Csy[csy_idx].m_Physique[i].Weight[j];
							TempVecDest2.z *= m_Csy[csy_idx].m_Physique[i].Weight[j];

							// 그리고 누적
							TempVecSrc2.x += TempVecDest2.x;
							TempVecSrc2.y += TempVecDest2.y;
							TempVecSrc2.z += TempVecDest2.z;
						}
						TempVecSrc2 = Normalize(TempVecSrc2);
						m_Csy[csy_idx].m_Vertices[i].nx = TempVecSrc2.x;
						m_Csy[csy_idx].m_Vertices[i].ny = TempVecSrc2.y;
						m_Csy[csy_idx].m_Vertices[i].nz = TempVecSrc2.z;


					}
					else
					{
						bpd_idx = m_Csy[csy_idx].m_BipedIdx[i];
						if (bpd_idx >= pBiped->m_BipedNum)
							bpd_idx = pBiped->m_BipedNum - 1;

						NKMath_LagVertexMatrixMultiply(m_Csy[csy_idx].m_Vertices[i],
													   m_Csy[csy_idx].m_Vertices0[i],
													   pBiped->m_AniMat[bpd_idx].m_Mats[CurFrame]);
					}
				}
			}
			// Line 계산...
			if (m_LineVector0)
			{
				NKMath_VectorMatrixMultiply(m_LineVector[0],
											m_LineVector0[0], pBiped->m_AniMat[m_LineBiped[0]].m_Mats[CurFrame]);
				NKMath_VectorMatrixMultiply(m_LineVector[1],
											m_LineVector0[1], pBiped->m_AniMat[m_LineBiped[1]].m_Mats[CurFrame]);
			}
		}

		if (bTrace && m_LineVector0)
		{
			// 검 Trace Line 계산
			oldCurFrame--;
			if (oldCurFrame < 0)
			{
				m_LineVector[2] = m_LineVector[0];
				m_LineVector[3] = m_LineVector[1];
			}
			else
			{
				if (pBiped->m_bHalfFrame)
				{
					if (oldCurFrame % 2)
					{
						GetHalfMat(g_TempMat[0], pBiped, m_LineBiped[0], oldCurFrame / 2);
						GetHalfMat(g_TempMat[1], pBiped, m_LineBiped[1], oldCurFrame / 2);
						pLineMat = &g_TempMat[0];
						pLineMat2 = &g_TempMat[1];
					}
					else
					{
						pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame / 2]);
						pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame / 2]);
					}
				}
				else
				{
					pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame]);
					pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame]);
				}

				NKMath_VectorMatrixMultiply(m_LineVector[2], m_LineVector0[0], *pLineMat);
				NKMath_VectorMatrixMultiply(m_LineVector[3], m_LineVector0[1], *pLineMat2);
			}

			oldCurFrame--;
			if (oldCurFrame < 0)
			{
				m_LineVector[4] = m_LineVector[2];
				m_LineVector[5] = m_LineVector[3];
			}
			else
			{
				if (pBiped->m_bHalfFrame)
				{
					if (oldCurFrame % 2)
					{
						GetHalfMat(g_TempMat[0], pBiped, m_LineBiped[0], oldCurFrame / 2);
						GetHalfMat(g_TempMat[1], pBiped, m_LineBiped[1], oldCurFrame / 2);
						pLineMat = &g_TempMat[0];
						pLineMat2 = &g_TempMat[1];
					}
					else
					{
						pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame / 2]);
						pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame / 2]);
					}
				}
				else
				{
					pLineMat = &(pBiped->m_AniMat[m_LineBiped[0]].m_Mats[oldCurFrame]);
					pLineMat2 = &(pBiped->m_AniMat[m_LineBiped[1]].m_Mats[oldCurFrame]);
				}

				NKMath_VectorMatrixMultiply(m_LineVector[4], m_LineVector0[0], *pLineMat);
				NKMath_VectorMatrixMultiply(m_LineVector[5], m_LineVector0[1], *pLineMat2);
			}

		}
	}
	else
	{
		// 회오리 무기 Animation
		CurFrame %= EDDY_FRAME;
		if (!bCalced_matEddy)
			Calc_matEddy();

		for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++)
		{
			CCsyObj *pCsy = &(m_Csy[csy_idx]);

			// 무기를 회오리 Matrix로 Animation 시킨다.
			for (int i = 0; i < pCsy->m_VertexCount; i++)
			{
				NKMath_LagVertexMatrixMultiply(pCsy->m_Vertices[i],
											   pCsy->m_Vertices0[i], matEddy[CurFrame]);
			}
		}

		if (m_LineVector0)
		{
			// 검 Trace Line 계산
			NKMath_VectorMatrixMultiply(m_LineVector[0],
										m_LineVector0[0], matEddy[CurFrame]);
			NKMath_VectorMatrixMultiply(m_LineVector[1],
										m_LineVector0[1], matEddy[CurFrame]);

			CurFrame--;
			if (CurFrame < 0)
				CurFrame = 0;
			NKMath_VectorMatrixMultiply(m_LineVector[2],
										m_LineVector0[0], matEddy[CurFrame]);
			NKMath_VectorMatrixMultiply(m_LineVector[3],
										m_LineVector0[1], matEddy[CurFrame]);

			CurFrame--;
			if (CurFrame < 0)
				CurFrame = 0;
			NKMath_VectorMatrixMultiply(m_LineVector[4],
										m_LineVector0[0], matEddy[CurFrame]);
			NKMath_VectorMatrixMultiply(m_LineVector[5],
										m_LineVector0[1], matEddy[CurFrame]);
		}
	}

	// 상대방이 너무 가까우면 총 안쏴!!
	if (pTargetVector && pTargetVector->x < 30)
	{
		pViewMat = NULL;
	}

	m_bLightEffect = FALSE;
	if (m_LineVector && pWorldMat && pViewMat)
	{
		D3DMATRIX matView, matInv;
		if (pTargetVector)
		{
			// 레이저 총을 위한 Rect 계산
			// View Matrix를 구한 후 View의 Inv를 구한다.
			D3DMATRIX matView, matInv;
			D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
			D3DMath_MatrixInvert(matInv, matView);

			m_LineVector[1].x = pTargetVector->x;
			m_LineVector[1].y = pTargetVector->y;
			m_LineVector[1].z = pTargetVector->z;

			// LineVector를 View 좌표로..
			D3DVECTOR lineVector[2];
			D3DMath_VectorMatrixMultiply(lineVector[0], m_LineVector[0], matView);
			D3DMath_VectorMatrixMultiply(lineVector[1], m_LineVector[1], matView);

			// line에 직각인 벡터를 구한다.
			D3DVECTOR lineInv;
			lineInv = lineVector[0];
			lineInv -= lineVector[1];

			FLOAT x_temp = lineInv.x;
			lineInv.x = lineInv.y;
			lineInv.y = x_temp;
			lineInv.z = 0;
			lineInv = Normalize(lineInv);
			lineInv *= LASERGUN_SCALE;

			// View 좌표의 사각형
			m_RectVector[0] = D3DVECTOR(lineVector[0].x + lineInv.x,
										lineVector[0].y - lineInv.y,
										lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[1] = D3DVECTOR(lineVector[1].x + lineInv.x,
										lineVector[1].y - lineInv.y,
										lineVector[1].z - LIGHTSWORD_Z);
			m_RectVector[2] = D3DVECTOR(lineVector[0].x - lineInv.x,
										lineVector[0].y + lineInv.y,
										lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[3] = D3DVECTOR(lineVector[1].x - lineInv.x,
										lineVector[1].y + lineInv.y,
										lineVector[1].z - LIGHTSWORD_Z);

			// 사각형을 World 좌표로...
			D3DMath_VectorMatrixMultiply(m_RectVector[0], m_RectVector[0], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[1], m_RectVector[1], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[2], m_RectVector[2], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[3], m_RectVector[3], matInv);
		}

		//------------------------------------------------------------------------
		//-- LINE_EFF_LIGHTSWORD || BLEEDSWORD_SCALE
		//-- 광선검 빛 Bleed (xy빌보드)

		else if (whatEff == LINE_EFF_LIGHTSWORD || whatEff == LINE_EFF_BLEED)
		{

			// 광선검이나 불검등을 위한 Rect 계산

			// View Matrix를 구한 후 View의 Inv를 구한다.
			D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
			D3DMath_MatrixInvert(matInv, matView);

			// LineVector를 View 좌표로..
			D3DVECTOR lineVector[2];
			D3DMath_VectorMatrixMultiply(lineVector[0], m_LineVector[0], matView);
			D3DMath_VectorMatrixMultiply(lineVector[1], m_LineVector[1], matView);

			// line에 직각인 벡터를 구한다.
			D3DVECTOR lineInv;
			lineInv = lineVector[0];
			lineInv -= lineVector[1];

			FLOAT x_temp = lineInv.x;
			lineInv.x = lineInv.y;
			lineInv.y = x_temp;
			lineInv.z = 0;
			lineInv = Normalize(lineInv);

			//-- Scale
			if(whatEff == LINE_EFF_LIGHTSWORD) lineInv *= LIGHTSWORD_SCALE;
			else if(whatEff == LINE_EFF_BLEED) lineInv *= BLEEDSWORD_SCALE;

			// View 좌표의 사각형
			m_RectVector[0] = D3DVECTOR(lineVector[0].x + lineInv.x,
										lineVector[0].y - lineInv.y,
										lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[1] = D3DVECTOR(lineVector[1].x + lineInv.x,
										lineVector[1].y - lineInv.y,
										lineVector[1].z - LIGHTSWORD_Z);
			m_RectVector[2] = D3DVECTOR(lineVector[0].x - lineInv.x,
										lineVector[0].y + lineInv.y,
										lineVector[0].z - LIGHTSWORD_Z);
			m_RectVector[3] = D3DVECTOR(lineVector[1].x - lineInv.x,
										lineVector[1].y + lineInv.y,
										lineVector[1].z - LIGHTSWORD_Z);

			// 사각형을 World 좌표로...
			D3DMath_VectorMatrixMultiply(m_RectVector[0], m_RectVector[0], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[1], m_RectVector[1], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[2], m_RectVector[2], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[3], m_RectVector[3], matInv);
		}
		//------------------------------------------------------------------------
		//-- LINE_EFF_BRIGHT
		//-- 검에 표시되는 반짝이 에 관한 계산 ( 완전 빌보드 )
		else if (whatEff == LINE_EFF_BRIGHT)
		{
			// View Matrix를 구한 후 View의 Inv를 구한다.
			D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
			D3DMath_MatrixInvert(matInv, matView);			

			D3DVECTOR lineCenter;

			// Line의 길이의 절반을 구한다. (근사값)
			float lineLen = MAX(PLUS(m_LineVector[0].x - m_LineVector[1].x),
								MAX(PLUS(m_LineVector[0].y - m_LineVector[1].y),
									PLUS(m_LineVector[0].z - m_LineVector[1].z))) / 2;

			// 라인의 중간값을 구한다.
			lineCenter.x = (m_LineVector[0].x + m_LineVector[1].x) / 2;
			lineCenter.y = (m_LineVector[0].y + m_LineVector[1].y) / 2;
			lineCenter.z = (m_LineVector[0].z + m_LineVector[1].z) / 2;
			D3DMath_VectorMatrixMultiply(lineCenter, lineCenter, matView);

			// View 좌표의 사각형
			m_RectVector[0] = D3DVECTOR(lineCenter.x - lineLen,
										lineCenter.y - lineLen,
										lineCenter.z - LIGHTSWORD_Z);
			m_RectVector[1] = D3DVECTOR(lineCenter.x - lineLen,
										lineCenter.y + lineLen,
										lineCenter.z - LIGHTSWORD_Z);
			m_RectVector[2] = D3DVECTOR(lineCenter.x + lineLen,
										lineCenter.y - lineLen,
										lineCenter.z - LIGHTSWORD_Z);
			m_RectVector[3] = D3DVECTOR(lineCenter.x + lineLen,
										lineCenter.y + lineLen,
										lineCenter.z - LIGHTSWORD_Z);

			// 사각형을 World 좌표로...
			D3DMath_VectorMatrixMultiply(m_RectVector[0], m_RectVector[0], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[1], m_RectVector[1], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[2], m_RectVector[2], matInv);
			D3DMath_VectorMatrixMultiply(m_RectVector[3], m_RectVector[3], matInv);
		}
		m_bLightEffect = TRUE;
	}

	return S_OK;
}

void Calc_matEddy()
{
	for (int eddy_idx = 0; eddy_idx < EDDY_FRAME; eddy_idx++)
	{
		D3DMATRIX mat_pos, mat_rot;
		// Frame에 맞게 matrix를 만든다.
		D3DUtil_SetRotateZMatrix(mat_pos, (g_2_PI / 12) * eddy_idx);
		mat_pos._43 = (float) eddy_idx * EDDY_SCALE;
		mat_pos._42 = EDDY_HEIGHT;
		D3DUtil_SetRotateYMatrix(mat_rot, (g_2_PI / 12) * eddy_idx);
		D3DMath_MatrixMultiply(matEddy[eddy_idx], mat_pos, mat_rot);
	}

	bCalced_matEddy = TRUE;
}

unsigned short g_TraceIndex[24] = {0, 2, 1, 1, 2, 3,
								   4, 6, 5, 5, 6, 7,
								   0, 1, 2, 1, 3, 2,
								   4, 5, 6, 5, 7, 6
								  };


//-- renderopt - 0 : 기존의 Capsyong 및 광선검 (default)
//--           - 1 : 하이브리더 Bleed 검
void CCapsyong::Render( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum , BOOL bPicking, BOOL bShowLow ,
						LPDIRECTDRAWSURFACE7 traceTexture , int effFrame , int renderopt,bool is_old)
{
	CCsyObj *pCsy;
	LPDIRECTDRAWSURFACE7 lpTexture;
	DWORD srcBlend, destBlend, srcBlend2, destBlend2;

	pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend2);
	pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend2 );



	//------------------------------------------------------------------------
	//-- Capsyong Render
	//------------------------------------------------------------------------
	for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++)
	{
		pCsy = &(m_Csy[csy_idx]);
		int tNum;

		if (textureNum >= 0)
			tNum = textureNum;
		else
			tNum = pCsy->m_TextureNum;

		//------------------------------------------------------------------------
		//-- Set Texture ( 피킹버퍼에 그릴 경우 제외 )
		if (tNum >= 0 && !bPicking)
		{
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(tNum)) != NULL)
			{
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
				{
					pd3dDevice->SetTexture(0, NULL);
				}
			}
			else
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}

		if( is_old )
		{
			if (textureNum >= 0 && effFrame >= 0)
			{
				float uv_add = (1.0f / 40) * (effFrame % 40);
				for (int i = 0; i < pCsy->m_VertexCount; i++)
				{
					pCsy->m_Vertices[i].tu += uv_add;
					pCsy->m_Vertices[i].tv += uv_add;
				}
			}
		}
		else
		{
			if (textureNum >= 0 && effFrame >= 0)
			{
				int max_frame = 256;
				m_effect_frame++;

				if( m_effect_frame > max_frame )
					m_effect_frame = 0;

				float uv_add = (1.0f / max_frame) * (m_effect_frame%max_frame);

				for (int i = 0; i < pCsy->m_VertexCount; i++)
				{
					pCsy->m_Vertices[i].tu += uv_add;
					pCsy->m_Vertices[i].tv += uv_add;
				}
			}
		}

		DWORD blendEnable;
		//------------------------------------------------------------------------
		//-- CSY_EFF_ONEONE
		if (pCsy->m_CsyEffect == CSY_EFF_ONEONE)
		{
			pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &blendEnable);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend );

			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		}

		// 그려라!
		if (bShowLow && pCsy->m_IndexedLow)
		{
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
											  pCsy->m_Vertices, pCsy->m_VertexCount,
											  pCsy->m_IndexedLow, pCsy->m_FaceCountLow * 3, NULL );
		}
		else
		{
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
											  pCsy->m_Vertices, pCsy->m_VertexCount,
											  pCsy->m_Indexed, pCsy->m_FaceCount * 3, NULL );
		}
		if (pCsy->m_CsyEffect == CSY_EFF_ONEONE)
		{
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, blendEnable);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend );
		}

		if (textureNum >= 0 && effFrame >= 0)
		{
			for (int i = 0; i < pCsy->m_VertexCount; i++)
			{
				pCsy->m_Vertices[i].tu = pCsy->m_Vertices0[i].tu;
				pCsy->m_Vertices[i].tv = pCsy->m_Vertices0[i].tv;
			}
		}
	}//-- end of for Capsyong render


	//------------------------------------------------------------------------
	//-- EFFECT 표현
	//------------------------------------------------------------------------
	if (m_LineVector && !bPicking)
	{
		DWORD alphaBlend, alphaTest, alphaRef;
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF   , 0x0000001);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );

		//------------------------------------------------------------------------
		//-- 광선검일 경우만 검의 자취를 표시하는 부분...
		//-- 사실은 텍스쳐가 자취 형으로 그려저 있어야 한다. 그러지 않을 경우 그냥 텍스쳐 표현
		if (traceTexture && renderopt ==0)
		{
			D3DLVERTEX traceVertex[8];

			traceVertex[0] = D3DLVERTEX(m_LineVector[0], 0x55ffffff, 0L, 0, 1);
			traceVertex[1] = D3DLVERTEX(m_LineVector[1], 0x55ffffff, 0L, 0, 0);
			traceVertex[2] = D3DLVERTEX(m_LineVector[2], 0x55ffffff, 0L, 1, 1);
			traceVertex[3] = D3DLVERTEX(m_LineVector[3], 0x55ffffff, 0L, 1, 0);
			traceVertex[4] = D3DLVERTEX(m_LineVector[2], 0x55ffffff, 0L, 0, 1);
			traceVertex[5] = D3DLVERTEX(m_LineVector[3], 0x55ffffff, 0L, 0, 0);
			traceVertex[6] = D3DLVERTEX(m_LineVector[4], 0x55ffffff, 0L, 1, 1);
			traceVertex[7] = D3DLVERTEX(m_LineVector[5], 0x55ffffff, 0L, 1, 0);			

			pd3dDevice->SetTexture(0, traceTexture);
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_LVERTEX ,
											  traceVertex, 8, g_TraceIndex, 24, NULL );			
		}

		//------------------------------------------------------------------------
		// 광선검이나, 불검, 또는 빤짝이등 검의 Effect를 표시하는 부분...

		if (m_bLightEffect)
		{
			//------------------------------------------------------------------------
			//-- Set Renderstate
			pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, FALSE );
			pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend );
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );

			D3DLVERTEX rectVertex[4];

			//------------------------------------------------------------------------
			//-- Texture Animaiton
			// 총 16Frame 동안 8Frame을 보여준다.
			int lightswordFrame = effFrame % 16 / 2;
			// 5->3, 6->2, 7->1
			if (lightswordFrame > 4)
				lightswordFrame = 8 - lightswordFrame;

			//------------------------------------------------------------------------
			//--  Set Vertext
			float textureU = lightswordFrame / 5.0f;

			rectVertex[0] = D3DLVERTEX(m_RectVector[0], 0x00ffffff, 0L, textureU, 1);
			rectVertex[1] = D3DLVERTEX(m_RectVector[1], 0x00ffffff, 0L, textureU, 0);
			rectVertex[2] = D3DLVERTEX(m_RectVector[2], 0x00ffffff, 0L, textureU + 0.2f, 1);
			rectVertex[3] = D3DLVERTEX(m_RectVector[3], 0x00ffffff, 0L, textureU + 0.2f, 0);
			
			//------------------------------------------------------------------------
			//-- Set texture

			//-- Effect ( Bleed )
			if( renderopt && traceTexture)
			{
				if (pd3dDevice->SetTexture(0, traceTexture) != DD_OK)
					pd3dDevice->SetTexture(0, NULL);
			}
			//-- Effect ( 반짝이 / 광선검 )
			else if ((lpTexture = m_pTexture->GetSurfaceByIdx(m_LineTextureNum)) != NULL)
			{
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
				{
					pd3dDevice->SetTexture(0, NULL);
				}
			}
			//-- Set NULL Texture
			else
			{
				pd3dDevice->SetTexture(0, NULL);
			}

			//------------------------------------------------------------------------
			//-- Draw Primitive
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX ,rectVertex, 4, NULL );

			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend );
			pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
		}

		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF   , alphaRef);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	}
	pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend2);
	pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend2 );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
}

void CCapsyong::RenderOutline(LPDIRECT3DDEVICE7 pd3dDevice, int textureNum, float width, DWORD color, IndexedTexture* pIT)
{
	CCsyObj *pCsy;
	LPDIRECTDRAWSURFACE7 lpTexture;

	DWORD cullMode;
	DWORD light;
	DWORD dwAlphaOp, dwAlphaArg1, dwAlphaArg2;
	DWORD dwColorOp, dwColorArg1, dwColorArg2;

	pd3dDevice->GetRenderState(D3DRENDERSTATE_CULLMODE, &cullMode);
	pd3dDevice->GetRenderState(D3DRENDERSTATE_LIGHTING, &light);
	pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOp);
	pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg1);
	pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG2, &dwAlphaArg2);
	pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
	pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg1);
	pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dwColorArg2);

	pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
	pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	for (int csy_idx = 0; csy_idx < m_ObjectCount; csy_idx++)
	{
		pCsy = &(m_Csy[csy_idx]);

		pd3dDevice->SetTexture(0, NULL);

		if( pIT != NULL )
		{
			lpTexture = pIT->GetSurfaceByIdx(textureNum);
		}
		else
		{
			lpTexture = m_pTexture->GetSurfaceByIdx(textureNum);
		}

		if( lpTexture )
		{
			pd3dDevice->SetTexture(0, lpTexture);

			pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);
			pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

			pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
			pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		}

		LAGVERTEX* vertices = new LAGVERTEX[pCsy->m_VertexCount];
		memcpy(vertices, pCsy->m_Vertices, sizeof(LAGVERTEX) * pCsy->m_VertexCount);

		for(int i = 0; i < pCsy->m_VertexCount; i++)
		{
			vertices[i].x += (vertices[i].nx * width);
			vertices[i].y += (vertices[i].ny * width);
			vertices[i].z += (vertices[i].nz * width);

			vertices[i].diffuse = color;
		}

		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  vertices, pCsy->m_VertexCount,
										  pCsy->m_Indexed, pCsy->m_FaceCount * 3, NULL );

		delete [] vertices;
	}

	pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg1);
	pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dwColorArg2);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOp);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg1);
	pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, dwAlphaArg2);
	pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, light);
	pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, cullMode);
	pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
}

void CCapsyong::RenderUnit( LPDIRECT3DDEVICE7 pd3dDevice, int UnitNum, int textureNum ) // 캡숑을 오브젝트 단위로 렌더하기 위한 함수.( 06-01-12 원석 )
{
	CCsyObj *pCsy;
	LPDIRECTDRAWSURFACE7 lpTexture;
	DWORD srcBlend, destBlend, srcBlend2, destBlend2;

	pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend2);
	pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend2 );

	//------------------------------------------------------------------------
	//-- Capsyong Render
	//------------------------------------------------------------------------
	if( UnitNum >= 0 && UnitNum < m_ObjectCount  ) // 정상적인 값일때만.
	{
		pCsy = &(m_Csy[UnitNum]);
		int tNum;

		if (textureNum >= 0)
			tNum = textureNum;
		else
			tNum = pCsy->m_TextureNum;

		//------------------------------------------------------------------------
		//-- Set Texture ( 이녀석은 피킹버퍼용으로 쓰진 않는다. )
		if (tNum >= 0 )
		{
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(tNum)) != NULL)
			{
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
				{
					pd3dDevice->SetTexture(0, NULL);
				}
			}
			else
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
		
		DWORD blendEnable;
		//------------------------------------------------------------------------
		//-- CSY_EFF_ONEONE
		if (pCsy->m_CsyEffect == CSY_EFF_ONEONE)
		{
			pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, &blendEnable);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_SRCBLEND, &srcBlend);
			pd3dDevice->GetRenderState(D3DRENDERSTATE_DESTBLEND, &destBlend );

			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
		}

		// 그려라!
		if (bShowLow && pCsy->m_IndexedLow)
		{
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
											  pCsy->m_Vertices, pCsy->m_VertexCount,
											  pCsy->m_IndexedLow, pCsy->m_FaceCountLow * 3, NULL );
		}
		else
		{
			pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
											  pCsy->m_Vertices, pCsy->m_VertexCount,
											  pCsy->m_Indexed, pCsy->m_FaceCount * 3, NULL );
		}
		if (pCsy->m_CsyEffect == CSY_EFF_ONEONE)
		{
			pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, blendEnable);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend);
			pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend );
		}		
	}//-- end of for Capsyong render


	pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, srcBlend2);
	pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, destBlend2 );
	pd3dDevice->SetRenderState( D3DRENDERSTATE_ZWRITEENABLE, TRUE );
}


void CCapsyong::RenderBound( LPDIRECT3DDEVICE7 pd3dDevice )
{
	if (m_BoundNum <= 0)
		return;
	pd3dDevice->SetTexture(0, NULL);

	pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX ,
									  m_BoundVert, m_BoundVertNum, m_BoundIndex, m_BoundFaceNum*3, NULL );
}
int loloZcompare( const void *arg1, const void *arg2 )
{
	if ((*(CLoloObj **)arg1)->m_CenterWorld.z < (*(CLoloObj **)arg2)->m_CenterWorld.z)
		return 1;
	else
		return -1;
}

void CLolosBase::Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, BOOL bPicking, BOOL bNoRoof, BOOL bShowLow,
					 D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, BOOL bSort, BOOL bNoLight, DWORD dwAlpha )
{
	int lolo_idx;
	D3DMATRIX matView;

	int oldFrame = CurFrame;
	if(m_AnimCount){
		CurFrame %= m_AnimCount;
	}
	else CurFrame = 0;

	if (pWorldMat && pViewMat)
	{
		D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
	}

	if (bSort && pWorldMat && pViewMat)
	{
		int lolo_count;
		if (bNoRoof)
		{
			lolo_count = m_LoloObjCount;
		}
		else
		{
			lolo_count = m_LoloObjCount + m_LoloRoofCount;
		}
		CLoloObj **pLoloList = new CLoloObj*[lolo_count];

		// 중심 좌표 회전!
		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++)
		{
			int nFrame = CurFrame;
			CLoloObj *pLoloObj;
			if (lolo_idx < m_LoloObjCount)
				pLoloObj = &m_LoloObjs[lolo_idx];
			else
				pLoloObj = &m_LoloRoofs[lolo_idx];

			if (pLoloObj->m_AnimCount == 1)
				nFrame = 0;

			D3DMath_VectorMatrixMultiply(pLoloObj->m_CenterWorld,
										 pLoloObj->m_CenterPoint[nFrame], matView);
			pLoloList[lolo_idx] = pLoloObj;
		}
		// 중심 좌표로 소팅!
		qsort((void *) pLoloList, (size_t) lolo_count, sizeof(CLoloObj *), loloZcompare);

		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++)
		{
			pLoloList[lolo_idx]->Render(pd3dDevice, CurFrame,
										m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}

		delete[] pLoloList;

	}
	else
	{
		for (lolo_idx = 0; lolo_idx < m_LoloObjCount; lolo_idx++)
		{
			int nFrame = CurFrame;
			if (m_LoloObjs[lolo_idx].m_AnimCount == 1)
				nFrame = 0;

			m_LoloObjs[lolo_idx].Render(pd3dDevice, nFrame,
										m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}
		if (!bNoRoof)
		{
			for (lolo_idx = 0; lolo_idx < m_LoloRoofCount; lolo_idx++)
			{
				int nFrame = CurFrame;
				if (m_LoloRoofs[lolo_idx].m_AnimCount == 1)
					nFrame = 0;

				m_LoloRoofs[lolo_idx].Render(pd3dDevice, nFrame,
											 m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
			}
		}
	}

	if (!bPicking && m_LineVector0 && pWorldMat && pViewMat && m_LineType == LINE_TYPE_BRIGHT)
	{
		D3DLVERTEX lvertex[4];
		D3DVECTOR lineCenter;

		D3DMATRIX matInv;
		D3DMath_MatrixInvert(matInv, matView);
				
		// Line의 길이의 절반을 구한다. (근사값)
		float lineLen = MAX(PLUS(m_LineVector0[0].x - m_LineVector0[1].x),
							MAX(PLUS(m_LineVector0[0].y - m_LineVector0[1].y),
								PLUS(m_LineVector0[0].z - m_LineVector0[1].z))) / 2;

		// 라인의 중간값을 구한다.
		lineCenter.x = (m_LineVector0[0].x + m_LineVector0[1].x) / 2;
		lineCenter.y = (m_LineVector0[0].y + m_LineVector0[1].y) / 2;
		lineCenter.z = (m_LineVector0[0].z + m_LineVector0[1].z) / 2;
		D3DMath_VectorMatrixMultiply(lineCenter, lineCenter, matView);

		// 총 16Frame 동안 8Frame을 보여준다.
		int brightFrame = oldFrame % 16 / 2;
		// 5->3, 6->2, 7->1
		if (brightFrame > 4)
			brightFrame = 8 - brightFrame;

		float textureU = brightFrame / 5.0f;

		// View 좌표의 사각형
		lvertex[0] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										  lineCenter.y - lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU, 1);
		lvertex[1] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										  lineCenter.y + lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU, 0);
		lvertex[2] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										  lineCenter.y - lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU + 0.2f, 1);
		lvertex[3] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										  lineCenter.y + lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU + 0.2f, 0);

		// 사각형을 World 좌표로...
		NKMath_LVertexMatrixMultiply(lvertex[0], lvertex[0], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[1], lvertex[1], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[2], lvertex[2], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[3], lvertex[3], matInv);

		unsigned short lineIndex[6];
		lineIndex[0] = 0;
		lineIndex[1] = 1;
		lineIndex[2] = 2;
		lineIndex[3] = 1;
		lineIndex[4] = 3;
		lineIndex[5] = 2;

		DWORD alphaBlend;
		DWORD alphaTest;

		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		
		// 그려라!
		LPDIRECTDRAWSURFACE7 lpTexture;
		if (m_LineTextureNum >= 0)
		{
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(m_LineTextureNum)) != NULL)
			{
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
				{
					pd3dDevice->SetTexture(0, NULL);
				}
			}
			else
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX , lvertex, 4, NULL );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	}
}

void CLolosBase::Render2( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum, int CurFrame, BOOL bPicking, BOOL bNoRoof, BOOL bShowLow,
					  D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, BOOL bSort, BOOL bNoLight, DWORD dwAlpha ) ///롤로텍스쳐변경 을 위한 그리기 함수를 추가. 오버로드하면 오히려 복잡할것 같아서 이름을 달리했다. 아이템 툴에서 별도로 입력한 텍스쳐를 롤로에 반영하기 위해 추가. 07-07-04 원석
{
	int lolo_idx;
	D3DMATRIX matView;

	int oldFrame = CurFrame;

	CurFrame %= m_AnimCount;

	if (pWorldMat && pViewMat)
	{
		D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
	}

	if (bSort && pWorldMat && pViewMat)
	{
		int lolo_count;
		if (bNoRoof)
		{
			lolo_count = m_LoloObjCount;
		}
		else
		{
			lolo_count = m_LoloObjCount + m_LoloRoofCount;
		}
		CLoloObj **pLoloList = new CLoloObj*[lolo_count];

		// 중심 좌표 회전!
		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++)
		{
			int nFrame = CurFrame;
			CLoloObj *pLoloObj;
			if (lolo_idx < m_LoloObjCount)
				pLoloObj = &m_LoloObjs[lolo_idx];
			else
				pLoloObj = &m_LoloRoofs[lolo_idx];

			if (pLoloObj->m_AnimCount == 1)
				nFrame = 0;

			D3DMath_VectorMatrixMultiply(pLoloObj->m_CenterWorld,
										 pLoloObj->m_CenterPoint[nFrame], matView);
			pLoloList[lolo_idx] = pLoloObj;
		}
		// 중심 좌표로 소팅!
		qsort((void *) pLoloList, (size_t) lolo_count, sizeof(CLoloObj *), loloZcompare);

		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++)
		{
			pLoloList[lolo_idx]->Render2(pd3dDevice, textureNum, CurFrame,
										 m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}

		delete[] pLoloList;

	}
	else
	{
		for (lolo_idx = 0; lolo_idx < m_LoloObjCount; lolo_idx++)
		{
			int nFrame = CurFrame;
			if (m_LoloObjs[lolo_idx].m_AnimCount == 1)
				nFrame = 0;

			m_LoloObjs[lolo_idx].Render2(pd3dDevice, textureNum, nFrame,
										 m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}
		if (!bNoRoof)
		{
			for (lolo_idx = 0; lolo_idx < m_LoloRoofCount; lolo_idx++)
			{
				int nFrame = CurFrame;
				if (m_LoloRoofs[lolo_idx].m_AnimCount == 1)
					nFrame = 0;

				m_LoloRoofs[lolo_idx].Render2(pd3dDevice, textureNum, nFrame,
											  m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
			}
		}
	}

	if (!bPicking && m_LineVector0 && pWorldMat && pViewMat && m_LineType == LINE_TYPE_BRIGHT)
	{
		D3DLVERTEX lvertex[4];
		D3DVECTOR lineCenter;

		D3DMATRIX matInv;
		D3DMath_MatrixInvert(matInv, matView);
				
		// Line의 길이의 절반을 구한다. (근사값)
		float lineLen = MAX(PLUS(m_LineVector0[0].x - m_LineVector0[1].x),
							MAX(PLUS(m_LineVector0[0].y - m_LineVector0[1].y),
								PLUS(m_LineVector0[0].z - m_LineVector0[1].z))) / 2;

		// 라인의 중간값을 구한다.
		lineCenter.x = (m_LineVector0[0].x + m_LineVector0[1].x) / 2;
		lineCenter.y = (m_LineVector0[0].y + m_LineVector0[1].y) / 2;
		lineCenter.z = (m_LineVector0[0].z + m_LineVector0[1].z) / 2;
		D3DMath_VectorMatrixMultiply(lineCenter, lineCenter, matView);

		// 총 16Frame 동안 8Frame을 보여준다.
		int brightFrame = oldFrame % 16 / 2;
		// 5->3, 6->2, 7->1
		if (brightFrame > 4)
			brightFrame = 8 - brightFrame;

		float textureU = brightFrame / 5.0f;

		// View 좌표의 사각형
		lvertex[0] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										  lineCenter.y - lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU, 1);
		lvertex[1] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										  lineCenter.y + lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU, 0);
		lvertex[2] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										  lineCenter.y - lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU + 0.2f, 1);
		lvertex[3] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										  lineCenter.y + lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU + 0.2f, 0);

		// 사각형을 World 좌표로...
		NKMath_LVertexMatrixMultiply(lvertex[0], lvertex[0], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[1], lvertex[1], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[2], lvertex[2], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[3], lvertex[3], matInv);

		unsigned short lineIndex[6];
		lineIndex[0] = 0;
		lineIndex[1] = 1;
		lineIndex[2] = 2;
		lineIndex[3] = 1;
		lineIndex[4] = 3;
		lineIndex[5] = 2;

		DWORD alphaBlend;
		DWORD alphaTest;
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		
		// 그려라!
		LPDIRECTDRAWSURFACE7 lpTexture;
		if (m_LineTextureNum >= 0)
		{
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(m_LineTextureNum)) != NULL)
			{
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
				{
					pd3dDevice->SetTexture(0, NULL);
				}
			}
			else
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX , lvertex, 4, NULL );		
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	}
}

void CLolosBase::RenderRaw( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, BOOL bPicking, BOOL bNoRoof, BOOL bShowLow,
						D3DMATRIX *pWorldMat, D3DMATRIX *pViewMat, BOOL bSort, BOOL bNoLight, DWORD dwAlpha )
{
	int lolo_idx;
	D3DMATRIX matView;

	int oldFrame = CurFrame;

	CurFrame %= m_AnimCount;

	if (pWorldMat && pViewMat)
	{
		D3DMath_MatrixMultiply(matView, *pWorldMat, *pViewMat);
	}

	if (bSort && pWorldMat && pViewMat)
	{
		int lolo_count;
		if (bNoRoof)
		{
			lolo_count = m_LoloObjCount;
		}
		else
		{
			lolo_count = m_LoloObjCount + m_LoloRoofCount;
		}
		CLoloObj **pLoloList = new CLoloObj*[lolo_count];

		// 중심 좌표 회전!
		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++)
		{
			int nFrame = CurFrame;
			CLoloObj *pLoloObj;
			if (lolo_idx < m_LoloObjCount)
				pLoloObj = &m_LoloObjs[lolo_idx];
			else
				pLoloObj = &m_LoloRoofs[lolo_idx];

			if (pLoloObj->m_AnimCount == 1)
				nFrame = 0;

			D3DMath_VectorMatrixMultiply(pLoloObj->m_CenterWorld,
										 pLoloObj->m_CenterPoint[nFrame], matView);
			pLoloList[lolo_idx] = pLoloObj;
		}
		// 중심 좌표로 소팅!
		qsort((void *) pLoloList, (size_t) lolo_count, sizeof(CLoloObj *), loloZcompare);

		for (lolo_idx = 0; lolo_idx < lolo_count; lolo_idx++)
		{
			pLoloList[lolo_idx]->RenderRaw(pd3dDevice, CurFrame,
										   m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}

		delete[] pLoloList;

	}
	else
	{
		for (lolo_idx = 0; lolo_idx < m_LoloObjCount; lolo_idx++)
		{
			int nFrame = CurFrame;
			if (m_LoloObjs[lolo_idx].m_AnimCount == 1)
				nFrame = 0;

			m_LoloObjs[lolo_idx].RenderRaw(pd3dDevice, nFrame,
										   m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
		}
		if (!bNoRoof)
		{
			for (lolo_idx = 0; lolo_idx < m_LoloRoofCount; lolo_idx++)
			{
				int nFrame = CurFrame;
				if (m_LoloRoofs[lolo_idx].m_AnimCount == 1)
					nFrame = 0;

				m_LoloRoofs[lolo_idx].RenderRaw(pd3dDevice, nFrame,
												m_BoundVect[7].y - m_BoundVect[0].y, bPicking, m_pTexture, bShowLow, dwAlpha);
			}
		}
	}

	if (!bPicking && m_LineVector0 && pWorldMat && pViewMat && m_LineType == LINE_TYPE_BRIGHT)
	{
		D3DLVERTEX lvertex[4];
		D3DVECTOR lineCenter;

		D3DMATRIX matInv;
		D3DMath_MatrixInvert(matInv, matView);

		// Line의 길이의 절반을 구한다. (근사값)
		float lineLen = MAX(PLUS(m_LineVector0[0].x - m_LineVector0[1].x),
							MAX(PLUS(m_LineVector0[0].y - m_LineVector0[1].y),
								PLUS(m_LineVector0[0].z - m_LineVector0[1].z))) / 2;

		// 라인의 중간값을 구한다.
		lineCenter.x = (m_LineVector0[0].x + m_LineVector0[1].x) / 2;
		lineCenter.y = (m_LineVector0[0].y + m_LineVector0[1].y) / 2;
		lineCenter.z = (m_LineVector0[0].z + m_LineVector0[1].z) / 2;
		D3DMath_VectorMatrixMultiply(lineCenter, lineCenter, matView);

		// 총 16Frame 동안 8Frame을 보여준다.
		int brightFrame = oldFrame % 16 / 2;
		// 5->3, 6->2, 7->1
		if (brightFrame > 4)
			brightFrame = 8 - brightFrame;

		float textureU = brightFrame / 5.0f;

		// View 좌표의 사각형
		lvertex[0] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										  lineCenter.y - lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU, 1);
		lvertex[1] = D3DLVERTEX(D3DVECTOR(lineCenter.x - lineLen,
										  lineCenter.y + lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU, 0);
		lvertex[2] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										  lineCenter.y - lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU + 0.2f, 1);
		lvertex[3] = D3DLVERTEX(D3DVECTOR(lineCenter.x + lineLen,
										  lineCenter.y + lineLen,
										  lineCenter.z - LIGHTSWORD_Z),
								0x55ffffff, 0L, textureU + 0.2f, 0);

		// 사각형을 World 좌표로...
		NKMath_LVertexMatrixMultiply(lvertex[0], lvertex[0], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[1], lvertex[1], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[2], lvertex[2], matInv);
		NKMath_LVertexMatrixMultiply(lvertex[3], lvertex[3], matInv);

		unsigned short lineIndex[6];
		lineIndex[0] = 0;
		lineIndex[1] = 1;
		lineIndex[2] = 2;
		lineIndex[3] = 1;
		lineIndex[4] = 3;
		lineIndex[5] = 2;

		DWORD alphaBlend;
		DWORD alphaTest;
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		
		// 그려라!
		LPDIRECTDRAWSURFACE7 lpTexture;
		if (m_LineTextureNum >= 0)
		{
			if ((lpTexture = m_pTexture->GetSurfaceByIdx(m_LineTextureNum)) != NULL)
			{
				if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
				{
					pd3dDevice->SetTexture(0, NULL);
				}
			}
			else
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , D3DFVF_LVERTEX , lvertex, 4, NULL );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	}
}


void CLoloObj::Render( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, float height, BOOL bPicking,
					   IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha )
{
	if (m_VertexCount == 0 || m_FaceCount == 0)
		return;
	
	if( CurFrame < 0 )
	{
		CurFrame = 0;
	}
	CurFrame %= m_AnimCount;	

	LPDIRECTDRAWSURFACE7 lpTexture;
	// Texture가 있으면 Texture 출력
	if (m_TextureNum >= 0 && !bPicking)
	{
		if ((lpTexture = pTexture->GetSurfaceByIdx(m_TextureNum)) != NULL)
		{
			if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
	}
	else
	{
		pd3dDevice->SetTexture(0, NULL);
	}

	LAGVERTEX *pVert;
	pVert = m_Vertices[CurFrame];

	// 그려라!
	DWORD alphaBlend;
	DWORD alphaTest;
	DWORD alphaRef;
	DWORD alphaFunc;
	if (m_bAlphablend && !bPicking )
	{
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		pd3dDevice->GetRenderState(D3DRENDERSTATE_ALPHAFUNC, &alphaFunc);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , 0x0000001);
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
	}
	if (dwAlpha != 0xff)
	{
		DWORD diffuse = 0x00ffffff | dwAlpha;
		for (int idx = 0; idx < m_VertexCount; idx++)
		{
			pVert[idx].diffuse = diffuse;
		}
	}
	if (bShowLow && m_IndexedLow)
	{
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  pVert, m_VertexCount,
										  m_IndexedLow, m_FaceCountLow * 3, NULL);
	}
	else
	{
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  pVert, m_VertexCount,
										  m_Indexed, m_FaceCount * 3, NULL);
	}
	if (m_bAlphablend && !bPicking )
	{
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF   , alphaRef);
		pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunc);
	}
}

void CLoloObj::Render2( LPDIRECT3DDEVICE7 pd3dDevice, int textureNum, int CurFrame, float height, BOOL bPicking,
						IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha ) ///롤로텍스쳐변경 을 위한 그리기 함수를 추가. 오버로드하면 오히려 복잡할것 같아서 이름을 달리했다. 아이템 툴에서 별도로 입력한 텍스쳐를 롤로에 반영하기 위해 추가. 07-07-04 원석
{
	if (m_VertexCount == 0 || m_FaceCount == 0)
		return;

	CurFrame %= m_AnimCount;

	int tNum;

	if (textureNum >= 0)
		tNum = textureNum;
	else
		tNum = m_TextureNum;

	LPDIRECTDRAWSURFACE7 lpTexture;
	// Texture가 있으면 Texture 출력
	if (m_TextureNum >= 0 && !bPicking)
	{
		if ((lpTexture = pTexture->GetSurfaceByIdx(tNum)) != NULL)
		{
			if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
	}
	else
	{
		pd3dDevice->SetTexture(0, NULL);
	}

	LAGVERTEX *pVert;
	pVert = m_Vertices[CurFrame];

	// 그려라!
	DWORD alphaBlend;
	DWORD alphaTest;
	DWORD alphaRef;
	DWORD alphaFunc;
	if (m_bAlphablend)
	{
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, &alphaBlend );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest );
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHAREF   , &alphaRef);
		pd3dDevice->GetRenderState( D3DRENDERSTATE_ALPHAFUNC, &alphaFunc);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF   , 0x0000001);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
	}
	if (dwAlpha != 0xff)
	{
		DWORD diffuse = 0x00ffffff | dwAlpha;
		for (int idx = 0; idx < m_VertexCount; idx++)
		{
			pVert[idx].diffuse = diffuse;
		}
	}
	if (bShowLow && m_IndexedLow)
	{
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  pVert, m_VertexCount,
										  m_IndexedLow, m_FaceCountLow * 3, NULL);
	}
	else
	{
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  pVert, m_VertexCount,
										  m_Indexed, m_FaceCount * 3, NULL);
	}
	if (m_bAlphablend)
	{
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, alphaBlend );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, alphaTest );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF   , alphaRef);
		pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, alphaFunc);
	}
}

void CLoloObj::RenderRaw( LPDIRECT3DDEVICE7 pd3dDevice, int CurFrame, float height, BOOL bPicking,
						  IndexedTexture *pTexture, BOOL bShowLow, DWORD dwAlpha )
{
	if (m_VertexCount == 0 || m_FaceCount == 0)
		return;

	CurFrame %= m_AnimCount;

	LPDIRECTDRAWSURFACE7 lpTexture;
	// Texture가 있으면 Texture 출력
	if (m_TextureNum >= 0 && !bPicking)
	{
		if ((lpTexture = pTexture->GetSurfaceByIdx(m_TextureNum)) != NULL)
		{
			if (pd3dDevice->SetTexture(0, lpTexture) != DD_OK)
			{
				pd3dDevice->SetTexture(0, NULL);
			}
		}
		else
		{
			pd3dDevice->SetTexture(0, NULL);
		}
	}
	else
	{
		pd3dDevice->SetTexture(0, NULL);
	}

	LAGVERTEX *pVert;
	pVert = m_Vertices[CurFrame];

	if (dwAlpha != 0xff)
	{
		for (int idx = 0; idx < m_VertexCount; idx++)
		{
			pVert[idx].diffuse = dwAlpha;
		}
	}
	if (bShowLow && m_IndexedLow)
	{
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  pVert, m_VertexCount,
										  m_IndexedLow, m_FaceCountLow * 3, NULL);
	}
	else
	{
		pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, FVF_LAGHAIM,
										  pVert, m_VertexCount,
										  m_Indexed, m_FaceCount * 3, NULL);
	}
}

void CBiped::Read(PFILE *fp, BOOL decode)
{
	int read_start = 1;
	int i;
	D3DMATRIX mat_temp;

	long fileSize = getFileSize(fp);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, fp);
	if(decode)
		pDecode(stream, fileSize);

	// Half Frame인지를 읽는다..
	readData(&m_bHalfFrame, sizeof(BOOL), 1, &stream);
	// Biped 의 Object 갯수를 읽는다.
	readData(&m_BipedNum, sizeof(int), 1, &stream);

	m_AniMat = new CAniMatrix[m_BipedNum];
	// Biped 의 Animation 갯수를 읽는다.
	readData(&(m_AnimCount), sizeof(int), 1, &stream);

	for (int bi = 0; bi < m_BipedNum; bi++)
	{
		// Animation Matrix 읽기.
		int anim_count;
		if (m_bHalfFrame)
			anim_count = (m_AnimCount+1)/2;
		else
			anim_count = m_AnimCount;

		m_AniMat[bi].m_Mats = new D3DMATRIX[anim_count];
		mat_temp._14 = mat_temp._24 = mat_temp._34 = 0.0f;
		mat_temp._44 = 1.0f;
		for (i = 0; i < anim_count; i++)
		{
			readData(&(mat_temp._11), sizeof(float), 1, &stream);
			readData(&(mat_temp._12), sizeof(float), 1, &stream);
			readData(&(mat_temp._13), sizeof(float), 1, &stream);
			readData(&(mat_temp._21), sizeof(float), 1, &stream);
			readData(&(mat_temp._22), sizeof(float), 1, &stream);
			readData(&(mat_temp._23), sizeof(float), 1, &stream);
			readData(&(mat_temp._31), sizeof(float), 1, &stream);
			readData(&(mat_temp._32), sizeof(float), 1, &stream);
			readData(&(mat_temp._33), sizeof(float), 1, &stream);
			readData(&(mat_temp._41), sizeof(float), 1, &stream);
			readData(&(mat_temp._42), sizeof(float), 1, &stream);
			readData(&(mat_temp._43), sizeof(float), 1, &stream);

			memcpy(&(m_AniMat[bi].m_Mats[i]), &mat_temp, sizeof(D3DMATRIX));
		}
	}
	SAFE_DELETE_ARRAY(streamOrg);
}

int CBiped::Read( char* szFileName )
{
	if( !strlen(szFileName) )
		return VF_ERR_NULL_STRING;

	m_filename = szFileName;

	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(szFileName, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();


	PFILE* fp = pfopen( m_filename.c_str(), "rb" );
	if( !fp )
		return VF_ERR_NULL_PT;
	Read( fp, decode );
	pfclose( fp );
	return VF_ERR_NONE;
}

#define CSY_ALPHA	(0xffffffff)
int CCapsyong::Read( PFILE *fp, BOOL decode )
{
	int i;

	char name[100];
	int name_len;

	int object_num;
	int all_face_count = 0;
	int low_face_count = 0;

	long fileSize = getFileSize(fp);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, fp);
	if(decode)
		pDecode(stream, fileSize);

	CCsyObj *pCsy;

	readData(&object_num, sizeof(int), 1, &stream);
	if (object_num < 1)
	{
		SAFE_DELETE_ARRAY(streamOrg);
		return FALSE;
	}

	if (object_num == 181818)
	{
		int i_temp;
		for (i = 0; i < 8; i++)
		{
			readData(&i_temp, sizeof(int), 1, &stream);
		}
		readData(&object_num, sizeof(int), 1, &stream);
	}
	else if( object_num == 282828 ) ///CSY2
	{
		m_bCsy2 = TRUE;

		int i_temp;
		for (i = 0; i < 8; i++)
		{
			readData(&i_temp, sizeof(int), 1, &stream);
		}
		readData(&object_num, sizeof(int), 1, &stream);
	}


	m_ObjectCount = object_num;
	m_Csy = new CCsyObj[object_num];

	// Object의 갯수만큼 읽어온다.
	for (int obj_idx = 0; obj_idx < object_num; obj_idx++)
	{
		pCsy = &(m_Csy[obj_idx]);

		// Material의 Texture파일 이름을 읽는다.
		readData(&name_len, sizeof(int), 1, &stream);
		if (name_len)
		{
			readData(name, name_len, 1, &stream);
			name[name_len] = '\0';

			if (!strncmp(name, "oo_", 3))
			{
				pCsy->m_CsyEffect = CSY_EFF_ONEONE;
			}

			// Texture 읽어오기.
			int TextureNum;
			if ((TextureNum = m_pTexture->CreateTextureFromFile(name)) < 0)
			{
				pCsy->m_TextureNum = -1;
			}
			else
			{
				pCsy->m_TextureNum = TextureNum;
			}
		}

		// Read Vertices
		int vertex_num;

		readData(&vertex_num, sizeof(int), 1, &stream);

		pCsy->m_Vertices0 = new LAGVERTEX[vertex_num];
		pCsy->m_Vertices = new LAGVERTEX[vertex_num];
		pCsy->m_BipedIdx = new short[vertex_num];

		if( m_bCsy2 ) ///CSY2
			pCsy->m_Physique = new sPhysique[vertex_num];

		pCsy->m_VertexCount = vertex_num;

		for (i = 0; i < vertex_num; i++)
		{
			if( m_bCsy2 ) ///CSY2
			{
				if( pCsy->m_Physique )
				{
					readData(&(pCsy->m_Physique[i].UseBpdNum), sizeof(BYTE), 1, &stream);

					if( pCsy->m_Physique[i].UseBpdNum > MAX_BPD_WEIGHT_NUM )
						pCsy->m_Physique[i].UseBpdNum = MAX_BPD_WEIGHT_NUM;

					for( int j = 0 ; j < pCsy->m_Physique[i].UseBpdNum ; ++j )
					{
						readData(&(pCsy->m_Physique[i].LocalPosFromBone[j].x), sizeof(float), 1, &stream);
						readData(&(pCsy->m_Physique[i].LocalPosFromBone[j].y), sizeof(float), 1, &stream);
						readData(&(pCsy->m_Physique[i].LocalPosFromBone[j].z), sizeof(float), 1, &stream);

						readData(&(pCsy->m_Physique[i].LocalNormalFromBone[j].x), sizeof(float), 1, &stream);
						readData(&(pCsy->m_Physique[i].LocalNormalFromBone[j].y), sizeof(float), 1, &stream);
						readData(&(pCsy->m_Physique[i].LocalNormalFromBone[j].z), sizeof(float), 1, &stream);


						readData(&(pCsy->m_Physique[i].BpdIndex[j]), sizeof(short), 1, &stream);
						readData(&(pCsy->m_Physique[i].Weight[j]), sizeof(float), 1, &stream);
					}

					readData(&(pCsy->m_Vertices0[i].tu), sizeof(float), 1, &stream);
					readData(&(pCsy->m_Vertices0[i].tv), sizeof(float), 1, &stream);

					// 혹시 모르니 첫 피직정보로 이전 정보들을 셋팅.
					pCsy->m_Vertices0[i].x = pCsy->m_Physique[i].LocalPosFromBone[0].x;
					pCsy->m_Vertices0[i].y = pCsy->m_Physique[i].LocalPosFromBone[0].y;
					pCsy->m_Vertices0[i].z = pCsy->m_Physique[i].LocalPosFromBone[0].z;

					pCsy->m_Vertices0[i].nx = pCsy->m_Physique[i].LocalNormalFromBone[0].x;
					pCsy->m_Vertices0[i].ny = pCsy->m_Physique[i].LocalNormalFromBone[0].y;
					pCsy->m_Vertices0[i].nz = pCsy->m_Physique[i].LocalNormalFromBone[0].z;

					pCsy->m_BipedIdx[i] = pCsy->m_Physique[i].BpdIndex[0];

					pCsy->m_Vertices0[i].diffuse = CSY_ALPHA;
				}

			}
			else
			{
				readData(&(pCsy->m_Vertices0[i].x), sizeof(float), 1, &stream);
				readData(&(pCsy->m_Vertices0[i].y), sizeof(float), 1, &stream);
				readData(&(pCsy->m_Vertices0[i].z), sizeof(float), 1, &stream);

				readData(&(pCsy->m_Vertices0[i].nx), sizeof(float), 1, &stream);
				readData(&(pCsy->m_Vertices0[i].ny), sizeof(float), 1, &stream);
				readData(&(pCsy->m_Vertices0[i].nz), sizeof(float), 1, &stream);

				readData(&(pCsy->m_Vertices0[i].tu), sizeof(float), 1, &stream);
				readData(&(pCsy->m_Vertices0[i].tv), sizeof(float), 1, &stream);

				pCsy->m_Vertices0[i].diffuse = CSY_ALPHA;

				readData(&(pCsy->m_BipedIdx[i]), sizeof(short), 1, &stream);
			}
		}

		// Read Faces
		int face_num;
		readData(&(face_num), sizeof(int), 1, &stream);
		pCsy->m_FaceCount = face_num;
		all_face_count += face_num;

		pCsy->m_Indexed = new unsigned short[face_num*3];
		int vert_idx;

		for (i = 0; i < face_num * 3; i++)
		{
			readData(&vert_idx, sizeof(int), 1, &stream);
			pCsy->m_Indexed[i] = (unsigned short) vert_idx;
		}

		// Read Faces Low
		readData(&(face_num), sizeof(int), 1, &stream);
		pCsy->m_FaceCountLow = face_num;
		low_face_count += face_num;

		if (face_num > 0)
		{
			pCsy->m_IndexedLow = new unsigned short[face_num*3];

			for (i = 0; i < face_num * 3; i++)
			{
				readData(&vert_idx, sizeof(int), 1, &stream);
				pCsy->m_IndexedLow[i] = (unsigned short) vert_idx;
			}
		}

		// Vertex들 Copy
		for (i = 0; i < vertex_num; i++)
		{
			pCsy->m_Vertices[i] = pCsy->m_Vertices0[i];
		}
	}

	int hasLine;
	readData(&hasLine, sizeof(int), 1, &stream);
	if (hasLine > 0)
	{
		m_LineVector0 = new D3DVECTOR[2];
		m_LineVector = new D3DVECTOR[6];

		readData(&(m_LineVector0[0].x), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[0].y), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[0].z), sizeof(float), 1, &stream);
		readData(&(m_LineBiped[0]), sizeof(int), 1, &stream);

		readData(&(m_LineVector0[1].x), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[1].y), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[1].z), sizeof(float), 1, &stream);
		readData(&(m_LineBiped[1]), sizeof(int), 1, &stream);

		readData(&name_len, sizeof(int), 1, &stream);
		if (name_len)
		{
			readData(name, name_len, 1, &stream);
			name[name_len] = '\0';

			// Texture 읽어오기.
			int TextureNum;
			if ((TextureNum = m_pTexture->CreateTextureFromFile(name)) < 0)
			{
				m_LineTextureNum = -1;
			}
			else
			{
				m_LineTextureNum = TextureNum;
			}
		}
	}

	readData(&m_BoundNum, sizeof(int), 1, &stream);
	if (m_BoundNum > 0)
	{
		readData(&m_BoundVertNum, sizeof(int), 1, &stream);
		m_BoundVert = new D3DVERTEX[m_BoundVertNum];
		for (int vert = 0; vert < m_BoundVertNum; vert++)
		{
			float x, y, z;
			readData(&x, sizeof(float), 1, &stream);
			readData(&y, sizeof(float), 1, &stream);
			readData(&z, sizeof(float), 1, &stream);
			m_BoundVert[vert] = D3DVERTEX(D3DVECTOR(x, y, z), D3DVECTOR(0, 0, 0), 0, 0);
		}
		readData(&m_BoundFaceNum, sizeof(int), 1, &stream);
		m_BoundIndex = new unsigned short[m_BoundFaceNum*3];
		for (int idx = 0; idx < m_BoundFaceNum*3; idx++)
		{
			int indexed;
			readData(&indexed, sizeof(int), 1, &stream);
			m_BoundIndex[idx] = indexed;
		}
	}
	SAFE_DELETE_ARRAY(streamOrg);

	return all_face_count;
}

int CCapsyong::Read(char* szFileName )
{
	if( !strlen( szFileName ) )
		return VF_ERR_NULL_STRING;
	int all_face_count = 0;

	m_filename = szFileName;

	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(m_filename.c_str(), "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();

	PFILE* fp = pfopen( szFileName, "rb" );
	if( !fp )
		return VF_ERR_NULL_PT;

	all_face_count = Read( fp, decode );
	pfclose( fp );
	return all_face_count;
}

void CCapsyong::RestoreTextures( LPDIRECT3DDEVICE7 pd3dDevice )
{
	if( m_pTexture == NULL )
		return;

	for(int i = 0; i < m_ObjectCount; i++)
	{
		if( m_Csy[i].m_TextureNum == -1 )
			continue;

		m_pTexture->RestoreByIdx(m_Csy[i].m_TextureNum, pd3dDevice);
	}

	if( m_LineTextureNum == -1 )
	{
		m_pTexture->RestoreByIdx(m_LineTextureNum, pd3dDevice);
	}
}

#define LOLO_ALPHA	(0xFE)
int CLolosBase::Read( PFILE *fp, BOOL bAlpha, BOOL bLoadWalkPlane, BOOL decode )
{
	int i;

	char name[100];
	int name_len;

	int normal_obj_num;
	int ani_obj_num;
	int normal_roof_num;
	int ani_roof_num;
	int anim_num;
	int obj_idx;
	CLoloObj *pLoloObj;
	int TextureNum;
	int vertex_num;
	int vert_idx;

	int all_face_count = 0;
	int low_face_count = 0;

	D3DVERTEX vertTemp;

	long fileSize = getFileSize(fp);
	unsigned char* stream = new unsigned char[fileSize];
	unsigned char* streamOrg = stream;
	pfread(stream, fileSize, 1, fp);
	if(decode)
		pDecode(stream, fileSize);

	// 버전 확인!
	unsigned int version;
	readData(&version, sizeof(unsigned int), 1, &stream);
	if (version != 100)
	{
		SAFE_DELETE_ARRAY(streamOrg);
		return 0;
	}

	readData(&anim_num, sizeof(int), 1, &stream);
	readData(&normal_obj_num, sizeof(int), 1, &stream);
	readData(&ani_obj_num, sizeof(int), 1, &stream);
	readData(&normal_roof_num, sizeof(int), 1, &stream);
	readData(&ani_roof_num, sizeof(int), 1, &stream);
	if (normal_obj_num + ani_obj_num + normal_roof_num + ani_roof_num < 1)
	{
		SAFE_DELETE_ARRAY(streamOrg);
		return FALSE;
	}

	m_AnimCount = anim_num;
	m_LoloObjCount = normal_obj_num + ani_obj_num;
	m_LoloRoofCount = normal_roof_num + ani_roof_num;

	if( m_LoloObjCount > 0 )
	{
		m_LoloObjs = new CLoloObj[m_LoloObjCount];
		if( normal_obj_num > 0 )
		{
			// Normal Object의 갯수만큼 읽어온다.
			for (obj_idx = 0; obj_idx < normal_obj_num; obj_idx++)
			{
				// 오브젝트 초기화
				pLoloObj = &(m_LoloObjs[obj_idx]);

				pLoloObj->m_AnimCount = 1;
				pLoloObj->m_Vertices = new LAGVERTEX*[1];
				pLoloObj->m_CenterPoint = new D3DVECTOR[1];

				// Material의 Texture파일 이름을 읽는다.
				readData(&name_len, sizeof(int), 1, &stream);
				if (name_len)
				{
					readData(name, name_len, 1, &stream);
					name[name_len] = '\0';

					if (!strncmp(name, "ap_", 3))
					{
						pLoloObj->m_bAlphablend = TRUE;
						m_bMustRenderLast = TRUE;
					}
					// Texture 읽어오기.
					if (bAlpha)
						TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
					else
						TextureNum = m_pTexture->CreateTextureFromFile(name);

					if (TextureNum < 0)
					{
						pLoloObj->m_TextureNum = -1;
					}
					else
					{
						pLoloObj->m_TextureNum = TextureNum;
					}
				}

				// Read Vertices
				readData(&vertex_num, sizeof(int), 1, &stream);
				pLoloObj->m_VertexCount = vertex_num;

				pLoloObj->m_Vertices[0] = new LAGVERTEX[vertex_num];

				for (i = 0; i < vertex_num; i++)
				{
					readData(&vertTemp, sizeof(D3DVERTEX), 1, &stream);
					pLoloObj->m_Vertices[0][i].x = vertTemp.x;
					pLoloObj->m_Vertices[0][i].y = vertTemp.y;
					pLoloObj->m_Vertices[0][i].z = vertTemp.z;
					pLoloObj->m_Vertices[0][i].nx = vertTemp.nx;
					pLoloObj->m_Vertices[0][i].ny = vertTemp.ny;
					pLoloObj->m_Vertices[0][i].nz = vertTemp.nz;
					pLoloObj->m_Vertices[0][i].tu = vertTemp.tu;
					pLoloObj->m_Vertices[0][i].tv = vertTemp.tv;
					pLoloObj->m_Vertices[0][i].diffuse = 0xffffffff;
				}
				// Read CenterPoint
				readData(&(pLoloObj->m_CenterPoint[0]), sizeof(D3DVECTOR), 1, &stream);

				// Read Faces
				readData(&(pLoloObj->m_FaceCount), sizeof(int), 1, &stream);
				pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
				all_face_count += pLoloObj->m_FaceCount;

				for (i = 0; i < pLoloObj->m_FaceCount * 3; i++)
				{
					readData(&vert_idx, sizeof(int), 1, &stream);
					pLoloObj->m_Indexed[i] = vert_idx;
				}

				// Read Low Faces
				readData(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, &stream);
				if (pLoloObj->m_FaceCountLow > 0)
				{
					pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
					low_face_count += pLoloObj->m_FaceCountLow;

					//int vert_idx;
					for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++)
					{
						readData(&vert_idx, sizeof(int), 1, &stream);
						pLoloObj->m_IndexedLow[i] = vert_idx;
					}
				}
			}
		}

		if( ani_obj_num > 0 )
		{
			// Animation Object의 갯수만큼 읽어온다.
			for (obj_idx = normal_obj_num; obj_idx < m_LoloObjCount; obj_idx++)
			{
				// 오브젝트 초기화
				pLoloObj = &(m_LoloObjs[obj_idx]);

				pLoloObj->m_AnimCount = anim_num;
				pLoloObj->m_Vertices = new LAGVERTEX*[anim_num];
				pLoloObj->m_CenterPoint = new D3DVECTOR[anim_num];

				// Material의 Texture파일 이름을 읽는다.
				readData(&name_len, sizeof(int), 1, &stream);
				if (name_len)
				{
					readData(name, name_len, 1, &stream);
					name[name_len] = '\0';

					if (!strncmp(name, "ap_", 3))
					{
						pLoloObj->m_bAlphablend = TRUE;
						m_bMustRenderLast = TRUE;
					}
					// Texture 읽어오기.
					if (bAlpha)
						TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
					else
						TextureNum = m_pTexture->CreateTextureFromFile(name);

					if (TextureNum < 0)
					{
						pLoloObj->m_TextureNum = -1;
					}
					else
					{
						pLoloObj->m_TextureNum = TextureNum;
					}
				}

				// Read Vertices
				readData(&vertex_num, sizeof(int), 1, &stream);
				pLoloObj->m_VertexCount = vertex_num;

				for (int anim_idx = 0; anim_idx < anim_num; anim_idx++)
				{

					pLoloObj->m_Vertices[anim_idx] = new LAGVERTEX[vertex_num];

					for (i = 0; i < vertex_num; i++)
					{
						readData(&vertTemp, sizeof(D3DVERTEX), 1, &stream);
						pLoloObj->m_Vertices[anim_idx][i].x = vertTemp.x;
						pLoloObj->m_Vertices[anim_idx][i].y = vertTemp.y;
						pLoloObj->m_Vertices[anim_idx][i].z = vertTemp.z;
						pLoloObj->m_Vertices[anim_idx][i].nx = vertTemp.nx;
						pLoloObj->m_Vertices[anim_idx][i].ny = vertTemp.ny;
						pLoloObj->m_Vertices[anim_idx][i].nz = vertTemp.nz;
						pLoloObj->m_Vertices[anim_idx][i].tu = vertTemp.tu;
						pLoloObj->m_Vertices[anim_idx][i].tv = vertTemp.tv;
						pLoloObj->m_Vertices[anim_idx][i].diffuse = 0xffffffff;
					}
					// Read CenterPoint
					readData(&(pLoloObj->m_CenterPoint[anim_idx]), sizeof(D3DVECTOR), 1, &stream);
				}

				// Read Faces
				readData(&(pLoloObj->m_FaceCount), sizeof(int), 1, &stream);
				pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
				all_face_count += pLoloObj->m_FaceCount;

				for (i = 0; i < pLoloObj->m_FaceCount * 3; i++)
				{
					readData(&vert_idx, sizeof(int), 1, &stream);
					pLoloObj->m_Indexed[i] = vert_idx;
				}

				readData(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, &stream);
				if (pLoloObj->m_FaceCountLow > 0)
				{
					pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
					low_face_count += pLoloObj->m_FaceCountLow;

					//int vert_idx;
					for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++)
					{
						readData(&vert_idx, sizeof(int), 1, &stream);
						pLoloObj->m_IndexedLow[i] = vert_idx;
					}
				}
			}
		}
	}

	if( m_LoloRoofCount > 0 )
	{
		m_LoloRoofs = new CLoloObj[m_LoloRoofCount];

		if( normal_roof_num > 0 )
		{
			// 지붕 Object의 갯수만큼 읽어온다.
			for (obj_idx = 0; obj_idx < normal_roof_num; obj_idx++)
			{
				// 오브젝트 초기화
				pLoloObj = &(m_LoloRoofs[obj_idx]);

				pLoloObj->m_AnimCount = 1;
				pLoloObj->m_Vertices = new LAGVERTEX*[1];
				pLoloObj->m_CenterPoint = new D3DVECTOR[1];

				// Material의 Texture파일 이름을 읽는다.
				readData(&name_len, sizeof(int), 1, &stream);
				if (name_len)
				{
					readData(name, name_len, 1, &stream);
					name[name_len] = '\0';

					if (!strncmp(name, "ap_", 3))
					{
						pLoloObj->m_bAlphablend = TRUE;
					}
					// Texture 읽어오기.
					if (bAlpha)
						TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
					else
						TextureNum = m_pTexture->CreateTextureFromFile(name);

					if (TextureNum < 0)
					{
						pLoloObj->m_TextureNum = -1;
					}
					else
					{
						pLoloObj->m_TextureNum = TextureNum;
					}
				}

				// Read Vertices
				readData(&vertex_num, sizeof(int), 1, &stream);
				pLoloObj->m_VertexCount = vertex_num;

				pLoloObj->m_Vertices[0] = new LAGVERTEX[vertex_num];

				for (i = 0; i < vertex_num; i++)
				{
					readData(&vertTemp, sizeof(D3DVERTEX), 1, &stream);
					pLoloObj->m_Vertices[0][i].x = vertTemp.x;
					pLoloObj->m_Vertices[0][i].y = vertTemp.y;
					pLoloObj->m_Vertices[0][i].z = vertTemp.z;
					pLoloObj->m_Vertices[0][i].nx = vertTemp.nx;
					pLoloObj->m_Vertices[0][i].ny = vertTemp.ny;
					pLoloObj->m_Vertices[0][i].nz = vertTemp.nz;
					pLoloObj->m_Vertices[0][i].tu = vertTemp.tu;
					pLoloObj->m_Vertices[0][i].tv = vertTemp.tv;
					pLoloObj->m_Vertices[0][i].diffuse = 0xffffffff;
				}
				// Read CenterPoint
				readData(&(pLoloObj->m_CenterPoint[0]), sizeof(D3DVECTOR), 1, &stream);

				// Read Faces
				readData(&(pLoloObj->m_FaceCount), sizeof(int), 1, &stream);
				pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
				all_face_count += pLoloObj->m_FaceCount;

				for (i = 0; i < pLoloObj->m_FaceCount * 3; i++)
				{
					readData(&vert_idx, sizeof(int), 1, &stream);
					pLoloObj->m_Indexed[i] = vert_idx;
				}

				// Read Low Faces
				readData(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, &stream);
				if (pLoloObj->m_FaceCountLow > 0)
				{
					pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
					low_face_count += pLoloObj->m_FaceCountLow;

					//int vert_idx;
					for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++)
					{
						readData(&vert_idx, sizeof(int), 1, &stream);
						pLoloObj->m_IndexedLow[i] = vert_idx;
					}
				}
			}
		}

		if( ani_roof_num > 0 )
		{
			// 지붕 Animation Object의 갯수만큼 읽어온다.
			for (obj_idx = normal_roof_num; obj_idx < m_LoloRoofCount; obj_idx++)
			{
				// 오브젝트 초기화
				pLoloObj = &(m_LoloRoofs[obj_idx]);

				pLoloObj->m_AnimCount = anim_num;
				pLoloObj->m_Vertices = new LAGVERTEX*[anim_num];
				pLoloObj->m_CenterPoint = new D3DVECTOR[anim_num];

				// Material의 Texture파일 이름을 읽는다.
				readData(&name_len, sizeof(int), 1, &stream);
				if (name_len)
				{
					readData(name, name_len, 1, &stream);
					name[name_len] = '\0';

					if (!strncmp(name, "ap_", 3))
					{
						pLoloObj->m_bAlphablend = TRUE;
					}
					// Texture 읽어오기.
					if (bAlpha)
						TextureNum = m_pTexture->CreateTextureFromFile(name, -1, 0L, D3DTEXTR_TRANSPARENTALPHA, LOLO_ALPHA);
					else
						TextureNum = m_pTexture->CreateTextureFromFile(name);
					if (TextureNum < 0)
					{
						pLoloObj->m_TextureNum = -1;
					}
					else
					{
						pLoloObj->m_TextureNum = TextureNum;
					}
				}

				// Read Vertices
				readData(&vertex_num, sizeof(int), 1, &stream);
				pLoloObj->m_VertexCount = vertex_num;

				for (int anim_idx = 0; anim_idx < anim_num; anim_idx++)
				{
					pLoloObj->m_Vertices[anim_idx] = new LAGVERTEX[vertex_num];

					for (i = 0; i < vertex_num; i++)
					{
						readData(&vertTemp, sizeof(D3DVERTEX), 1, &stream);
						pLoloObj->m_Vertices[anim_idx][i].x = vertTemp.x;
						pLoloObj->m_Vertices[anim_idx][i].y = vertTemp.y;
						pLoloObj->m_Vertices[anim_idx][i].z = vertTemp.z;
						pLoloObj->m_Vertices[anim_idx][i].nx = vertTemp.nx;
						pLoloObj->m_Vertices[anim_idx][i].ny = vertTemp.ny;
						pLoloObj->m_Vertices[anim_idx][i].nz = vertTemp.nz;
						pLoloObj->m_Vertices[anim_idx][i].tu = vertTemp.tu;
						pLoloObj->m_Vertices[anim_idx][i].tv = vertTemp.tv;
						pLoloObj->m_Vertices[anim_idx][i].diffuse = 0xffffffff;
					}
					// Read CenterPoint
					readData(&(pLoloObj->m_CenterPoint[anim_idx]), sizeof(D3DVECTOR), 1, &stream);
				}

				// Read Faces
				readData(&(pLoloObj->m_FaceCount), sizeof(int), 1, &stream);
				pLoloObj->m_Indexed = new unsigned short[pLoloObj->m_FaceCount * 3];
				all_face_count += pLoloObj->m_FaceCount;

				for (i = 0; i < pLoloObj->m_FaceCount * 3; i++)
				{
					readData(&vert_idx, sizeof(int), 1, &stream);
					pLoloObj->m_Indexed[i] = vert_idx;
				}

				readData(&(pLoloObj->m_FaceCountLow), sizeof(int), 1, &stream);
				if (pLoloObj->m_FaceCountLow > 0)
				{
					pLoloObj->m_IndexedLow = new unsigned short[pLoloObj->m_FaceCountLow * 3];
					low_face_count += pLoloObj->m_FaceCountLow;

					//int vert_idx;
					for (i = 0; i < pLoloObj->m_FaceCountLow * 3; i++)
					{
						readData(&vert_idx, sizeof(int), 1, &stream);
						pLoloObj->m_IndexedLow[i] = vert_idx;
					}
				}
			}
		}
	}

	readData(&m_CenterPoint, sizeof(D3DVECTOR), 1, &stream);
	float MinX, MaxX, MinY, MaxY, MinZ, MaxZ;
	readData(&MinX, sizeof(float), 1, &stream);
	readData(&MaxX, sizeof(float), 1, &stream);
	readData(&MinY, sizeof(float), 1, &stream);
	readData(&MaxY, sizeof(float), 1, &stream);
	readData(&MinZ, sizeof(float), 1, &stream);
	readData(&MaxZ, sizeof(float), 1, &stream);

	// Line Export
	int hasLine;
	readData(&hasLine, sizeof(int), 1, &stream);
	if (hasLine > 0)
	{
		readData(&name_len, sizeof(int), 1, &stream);
		if (name_len)
		{
			readData(name, name_len, 1, &stream);
			name[name_len] = '\0';

			if (!strncmp(name, "hb_", 3))
			{
				m_LineType = LINE_TYPE_FIRE;
			}
			else if (!strncmp(name, "jk_", 3))
			{
				m_LineType = LINE_TYPE_SMOKE;
			}
			else if (!strncmp(name, "yn_", 3))
			{
				m_LineType = LINE_TYPE_BLUEFIRE;
			}
			else if (!strncmp(name, "kr_", 3))
			{
				m_LineType = LINE_TYPE_VOLCANO;
			}
			else if (!strncmp(name, "os_", 3))
			{
				m_LineType = LINE_TYPE_POISON;
			}
			else if (!strncmp(name, "se_", 3))
			{
				m_LineType = LINE_TYPE_GAS;
			}
			else if (!strncmp(name, "if_", 3))
			{
				m_LineType = LINE_TYPE_ELECTRIC;
			}
			else if (!strncmp(name, "glr_", 4))
			{
				m_LineType = LINE_TYPE_CLYSTAL_RED;
			}
			else if (!strncmp(name, "glwb_", 5))
			{
				m_LineType = LINE_TYPE_CLYSTAL_WINEB;
			}
			else if (!strncmp(name, "glws_", 5))
			{
				m_LineType = LINE_TYPE_CLYSTAL_WINES;
			}
			else if (!strncmp(name, "glc_", 4))
			{
				m_LineType = LINE_TYPE_CLYSTAL_CYON;
			}
			else if (!strncmp(name, "wfd_", 4))
			{
				m_LineType = LINE_TYPE_WATERFALLD;
			}
			else if (!strncmp(name, "wf_", 3))
			{
				m_LineType = LINE_TYPE_WATERFALL;
			}
			else if (!strncmp(name, "pt_", 3))
			{
				m_LineType = LINE_TYPE_PORTAL;
			}
			else if (!strncmp(name, "fsm_", 4))
			{
				m_LineType = LINE_TYPE_FIRESMOKE;
			}

			// Texture 읽어오기.
			if ((TextureNum = m_pTexture->CreateTextureFromFile(name)) < 0)
			{
				m_LineTextureNum = -1;
			}
			else
			{
				m_LineTextureNum = TextureNum;
			}
		}

		m_LineVector0 = new D3DVECTOR[2];

		readData(&(m_LineVector0[0].x), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[0].y), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[0].z), sizeof(float), 1, &stream);

		readData(&(m_LineVector0[1].x), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[1].y), sizeof(float), 1, &stream);
		readData(&(m_LineVector0[1].z), sizeof(float), 1, &stream);

		if (m_LineVector0[0].x < MinX)
			MinX = m_LineVector0[0].x;
		if (m_LineVector0[0].y < MinY)
			MinY = m_LineVector0[0].y;
		if (m_LineVector0[0].z < MinZ)
			MinZ = m_LineVector0[0].z;
		if (m_LineVector0[0].x > MaxX)
			MaxX = m_LineVector0[0].x;
		if (m_LineVector0[0].y > MaxY)
			MaxY = m_LineVector0[0].y;
		if (m_LineVector0[0].z > MaxZ)
			MaxZ = m_LineVector0[0].z;

		if (m_LineVector0[1].x < MinX)
			MinX = m_LineVector0[1].x;
		if (m_LineVector0[1].y < MinY)
			MinY = m_LineVector0[1].y;
		if (m_LineVector0[1].z < MinZ)
			MinZ = m_LineVector0[1].z;
		if (m_LineVector0[1].x > MaxX)
			MaxX = m_LineVector0[1].x;
		if (m_LineVector0[1].y > MaxY)
			MaxY = m_LineVector0[1].y;
		if (m_LineVector0[1].z > MaxZ)
			MaxZ = m_LineVector0[1].z;
	}
	m_BoundVect[0] = D3DVECTOR(MinX, MinY, MinZ);
	m_BoundVect[1] = D3DVECTOR(MinX, MinY, MaxZ);
	m_BoundVect[2] = D3DVECTOR(MaxX, MinY, MaxZ);
	m_BoundVect[3] = D3DVECTOR(MaxX, MinY, MinZ);
	m_BoundVect[4] = D3DVECTOR(MinX, MaxY, MinZ);
	m_BoundVect[5] = D3DVECTOR(MinX, MaxY, MaxZ);
	m_BoundVect[6] = D3DVECTOR(MaxX, MaxY, MaxZ);
	m_BoundVect[7] = D3DVECTOR(MaxX, MaxY, MinZ);


	// Floor 읽기
	readData(&m_FloorCount, sizeof(int), 1, &stream);
	if( m_FloorCount > 0 )
	{
		stream += sizeof(float) * 4 * m_FloorCount;
	}

	// InDoor 읽기
	readData(&m_InDoorCount, sizeof(int), 1, &stream);
	if( m_InDoorCount > 0 )
	{
		stream += sizeof(float) * 4 * m_InDoorCount;
	}

	// OutDoor 읽기
	readData(&m_OutDoorCount, sizeof(int), 1, &stream);
	if( m_OutDoorCount > 0 )
	{
		stream += sizeof(float) * 4 * m_OutDoorCount;
	}

	int size = stream - streamOrg;
	size += sizeof(int);
	// WalkPlane 읽기
	if( bLoadWalkPlane && size < fileSize)
	{
		readData( &m_WalkPlaneCount, sizeof(int), 1, &stream );
		if( m_WalkPlaneCount > 0 )
		{
			D3DVECTOR	vTemp;
			m_WalkPlaneVect = new D3DVECTOR[m_WalkPlaneCount * 4];
			for( i = 0; i < m_WalkPlaneCount * 4; i++ )
			{
				readData( &vTemp, sizeof(D3DVECTOR), 1, &stream );
				m_WalkPlaneVect[i] = vTemp;
			}
		}
	}
	SAFE_DELETE_ARRAY(streamOrg);

	return all_face_count;
}

int CLolosBase::Read( char* szFileName, BOOL bAlpha, BOOL bLoadWalkPlane )
{
	if( !strlen( szFileName ) )
		return VF_ERR_NULL_STRING;
	int all_face_count = 0;

	m_filename = szFileName;

	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(m_filename.c_str(), "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();

	PFILE* fp = pfopen( m_filename.c_str(), "rb" );
	if( !fp )
		return VF_ERR_NULL_PT;

	all_face_count = Read( fp, bAlpha, bLoadWalkPlane, decode );
	pfclose( fp );

	return all_face_count;
}

void CLolosBase::RestoreTextures( LPDIRECT3DDEVICE7 pd3dDevice )
{
	if( m_pTexture == NULL )
		return;

	for( int i = 0; i < m_LoloObjCount; i++ )
	{
		if( m_LoloObjs[i].m_TextureNum == -1 )
			continue;

		m_pTexture->RestoreByIdx(m_LoloObjs[i].m_TextureNum, pd3dDevice);
	}

	for( int i = 0; i < m_LoloRoofCount; i++ )
	{
		if( m_LoloRoofs[i].m_TextureNum == -1 )
			continue;

		m_pTexture->RestoreByIdx(m_LoloRoofs[i].m_TextureNum, pd3dDevice);
	}

	if( m_LineTextureNum != -1 )
		m_pTexture->RestoreByIdx(m_LineTextureNum, pd3dDevice);
}

HRESULT NKMath_VectorMatrixMultiply( D3DVECTOR& vDest, D3DVECTOR& vSrc, D3DMATRIX& mat)
{
	vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;

	return S_OK;
}

HRESULT NKMath_VertexMatrixMultiply( D3DVERTEX& vDest, D3DVERTEX& vSrc, D3DMATRIX& mat)
{
	vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;

	vDest.nx = vSrc.nx*mat._11 + vSrc.ny*mat._21 + vSrc.nz* mat._31;// + mat._41;
	vDest.ny = vSrc.nx*mat._12 + vSrc.ny*mat._22 + vSrc.nz* mat._32;// + mat._42;
	vDest.nz = vSrc.nx*mat._13 + vSrc.ny*mat._23 + vSrc.nz* mat._33;// + mat._43;

	return S_OK;
}

HRESULT NKMath_LagVertexMatrixMultiply( LAGVERTEX& vDest, LAGVERTEX& vSrc, D3DMATRIX& mat)
{
	vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;

	vDest.nx = vSrc.nx*mat._11 + vSrc.ny*mat._21 + vSrc.nz* mat._31;// + mat._41;
	vDest.ny = vSrc.nx*mat._12 + vSrc.ny*mat._22 + vSrc.nz* mat._32;// + mat._42;
	vDest.nz = vSrc.nx*mat._13 + vSrc.ny*mat._23 + vSrc.nz* mat._33;// + mat._43;

	return S_OK;
}

HRESULT NKMath_LVertexMatrixMultiply( D3DLVERTEX& vDest, D3DLVERTEX& vSrc, D3DMATRIX& mat)
{
	FLOAT x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	FLOAT y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	FLOAT z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
	FLOAT w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;

	if( fabs( w ) < g_EPSILON )
		return E_INVALIDARG;

	vDest.x = x/w;
	vDest.y = y/w;
	vDest.z = z/w;

	return S_OK;
}
