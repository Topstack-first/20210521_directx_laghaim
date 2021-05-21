#include "stdafx.h"
#define D3D_OVERLOADS


#include <assert.h>
#include <d3d.h>
#include "main.h"
#include "Land.h"
#include "LandDemo.h"
#include "D3DMath.h"
#include "IndexedTexture.h"

int		gLdMapWidth;					// 맵의 x축 넓이
int		gLdMapHeight;
int		gLdHFWidth;
int		gLdHFHeight;
int		gLdMapExtention;					// 스케일 팩터 - 정수형
float	gfLdMapExtention;				// 스케일 팩터 - 실수형
int		gLdMapYExtention;
float	gfLdMapYExtention;
int		gLdHeightScale = 2;

// 맵을 셀로 나눈 것에 대한 정의.
int g_nLdMapCellWidth;						// 한 셀의 크기.
int g_nLdMapCellHeight;
int g_nLdMapCellWidthCount;
int g_nLdMapCellHeightCount;

extern BOOL g_bDungeon;

static DWORD	RGB_BITMASK_R = 0x00FF0000;
static DWORD	RGB_BITMASK_G = 0x0000FF00;
static DWORD	RGB_BITMASK_B = 0x000000FF;
D3DLVERTEX CLandDemo::BaseTri[4];					// 텍스쳐 좌표를 위한 기본 삼각형.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLandDemo::CLandDemo()
	: m_nAttrMapWidth(0)
	, m_nAttrMapByteWidth(0)
	, m_nAttrMapHeight(0)
{
	memset(&m_MapAttr, 0, sizeof(m_MapAttr));

	m_fCenterX = 0.0f;
	m_fCenterZ = 0.0f;
	m_pHeightMap = NULL;
	m_pMapIdx = NULL;					// 맵의 텍스처 인덱스
	m_pTileTextures = NULL;
	m_pdwColor = NULL;

	// 신맵을 쓰는 로그인존은 아래 요소들을 직접 가지고 있게 변경. (구맵은 그대로 전역변수를 쓴다.)///2007로딩변경
	m_pL1TexID = NULL;
	m_pL2TexID = NULL;
	m_pL3TexID = NULL;
	m_pIndexedTexture = NULL;         // 신맵용 텍스쳐 셋 ///2007로딩변경

	m_LdMapWidth = 0;					// 맵의 x축 넓이
	m_LdMapHeight = 0;
	m_LdHFWidth = 0;
	m_LdHFHeight = 0;
	m_LdMapExtention = 0;					// 스케일 팩터 - 정수형
	m_fLdMapExtention= 0.0f;				// 스케일 팩터 - 실수형
	m_LdMapYExtention = 0;
	m_fLdMapYExtention = 0.0f;
	m_LdHeightScale = 2;

	m_nLdMapCellWidth = 0;						// 한 셀의 크기.
	m_nLdMapCellHeight = 0;
	m_nLdMapCellWidthCount = 0;
	m_nLdMapCellHeightCount = 0;
}

CLandDemo::~CLandDemo()
{
	SAFE_DELETE_ARRAY(m_pHeightMap); ///2007로딩변경 시 ARRAY 삭제로 바꿈.
	SAFE_DELETE_ARRAY(m_pMapIdx);
	SAFE_DELETE(m_pTileTextures);
	SAFE_DELETE_ARRAY(m_pdwColor);

	///2007로딩변경
	SAFE_DELETE_ARRAY(m_pL3TexID);
	SAFE_DELETE_ARRAY(m_pL2TexID);
	SAFE_DELETE_ARRAY(m_pL1TexID);

	SAFE_DELETE(m_pIndexedTexture);

}

void CLandDemo::Init(float x, float z)
{
	m_fCenterX = x;
	m_fCenterZ = z;
}

void CLandDemo::LoadNewMap( const TCHAR* szFilePath, const TCHAR* szFileName ) // 신맵툴 데이터를 읽어와서 CLandDemo 데이터를 만든다. ///2007로딩변경
{
	TCHAR	szFilePathName[MAX_PATH];
	strcpy( szFilePathName, szFilePath );
	strcat( szFilePathName, szFileName );
	PFILE*	fp = pfopen( szFilePathName, "rb" );
	if( fp == NULL )
		return;

	int MapExtention, HeightScale, HFWidth, HFHeight, MapWidth, MapHeight;

	// Magic( MAP + Version ) - Version: 0.1 -> 1, 1.0 -> 10
	DWORD	dwMagic;
	pfread( &dwMagic, sizeof(DWORD), 1, fp );
	if( dwMagic != ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 10 )
		return;

	// Patch size
	pfread( &MapExtention, sizeof(int), 1, fp );

	// Height scale
	pfread( &HeightScale, sizeof(int), 1, fp );

	// Height map
	pfread( &HFWidth, sizeof(int), 1, fp );
	m_pHeightMap = new unsigned char[HFWidth * HFWidth];
	pfread( m_pHeightMap, sizeof(unsigned char), HFWidth * HFWidth, fp );
	HFHeight = HFWidth;
	MapWidth = MapHeight = HFWidth - 1;


	// 이 위부턴 텍스쳐 외엔 일단 안쓴다.

	// Attribute map
	int nAttrMapWidth = MapWidth * MapExtention / 10;
	int nAttrMapByteWidth = ( nAttrMapWidth - 1 ) / 8 + 1;
	int nAttrMapHeight = nAttrMapWidth;
	BYTE *pAttrMap;
	pAttrMap = new unsigned char[nAttrMapWidth * nAttrMapHeight];
	pfread( pAttrMap, sizeof(unsigned char), nAttrMapByteWidth * nAttrMapHeight, fp );
	SAFE_DELETE_ARRAY( pAttrMap ); // 실제론 안쓰이니 바로 지운다.

	// 알파 맵과 라이트 맵도 무조건 스킵
	int	nWidth = MapWidth * 2;
	// Alpha map
	pfseek( fp, sizeof(unsigned char) * nWidth * nWidth * 2, SEEK_CUR );
	// Light map
	pfseek( fp, sizeof(unsigned char) * nWidth * nWidth * 18, SEEK_CUR );


	// Texture IDs
	m_pL1TexID = new unsigned char[MapWidth * MapWidth];
	m_pL2TexID = new unsigned char[MapWidth * MapWidth];
	m_pL3TexID = new unsigned char[MapWidth * MapWidth];
	pfread( m_pL1TexID, sizeof(unsigned char), MapWidth * MapWidth, fp );
	pfread( m_pL2TexID, sizeof(unsigned char), MapWidth * MapWidth, fp );
	pfread( m_pL3TexID, sizeof(unsigned char), MapWidth * MapWidth, fp );


	// Index buffers // 스킵
	int i;
	DWORD	dwIndexCount;
	int nTexCount;
	pfread( &nTexCount, sizeof(int), 1, fp );

	for( i = 0; i <= nTexCount; i++ )
	{
		pfread( &dwIndexCount, sizeof(DWORD), 1, fp );
		pfread( &dwIndexCount, sizeof(DWORD), 1, fp );
		pfread( &dwIndexCount, sizeof(DWORD), 1, fp );
	}

	// Textures
	TCHAR	szTexName[128];
	int		nLength;
	pfread( &nTexCount, sizeof(int), 1, fp );
	m_pIndexedTexture = new IndexedTexture( nTexCount );
	m_pIndexedTexture->SetTexturePath( szFilePath );
	for( i = 0; i < nTexCount; i++ )
	{
		pfread( &nLength, sizeof(int), 1, fp );
		pfread( szTexName, sizeof(TCHAR), nLength, fp );
		szTexName[nLength] = NULL;
		if( m_pIndexedTexture->CreateTextureFromFile( szTexName, i ) < 0 )
		{
			SAFE_DELETE_ARRAY( m_pL1TexID );
			SAFE_DELETE_ARRAY( m_pL2TexID );
			SAFE_DELETE_ARRAY( m_pL3TexID );
			pfclose( fp );
			return;
		}
	}

	m_pIndexedTexture->RestoreAllTextures(GET_D3DDEVICE());

	pfclose( fp );

	// 여기서 부턴 CLandDemo에서 필요한 수치들의 셋팅.
	m_LdMapWidth = MapWidth;					// 맵의 x축 넓이
	m_LdMapHeight = MapHeight;
	m_LdHFWidth = m_LdMapWidth + 1;
	m_LdHFHeight = m_LdMapHeight + 1;
	m_LdMapExtention = MapExtention;					// 스케일 팩터 - 정수형
	m_fLdMapExtention = (float)MapExtention;				// 스케일 팩터 - 실수형
	m_LdHeightScale = HeightScale; // 높이 스케일

	m_nLdMapCellWidth = m_LdMapExtention;						// 한 셀의 크기.
	m_nLdMapCellHeight = m_LdMapExtention;

	m_nLdMapCellWidthCount = m_LdMapWidth / (m_nLdMapCellWidth/m_LdMapExtention);
	m_nLdMapCellHeightCount = m_LdMapHeight / (m_nLdMapCellHeight/m_LdMapExtention);
}

void CLandDemo::Render()
{
	if( !GET_D3DDEVICE() )
		return;

	LPDIRECTDRAWSURFACE7 lpDDsurface;
	int map_center_x, map_center_z;
	int sw, sh, ew, eh, w, h;
	int h_mul_gHFWidth, h1_mul_gHFWidth;
	float fH_mul_gMap, fW_mul_gfMap;

	// 중심점의 맵 좌표를 찾는다.
	map_center_x = (int) (m_fCenterX / gfLdMapExtention);
	map_center_z = (int) (m_fCenterZ / gfLdMapExtention);

	sw = map_center_x - LANDDEMO_RENDER_RANGE;
	ew = map_center_x + LANDDEMO_RENDER_RANGE;
	sh = map_center_z - LANDDEMO_RENDER_RANGE;
	eh = map_center_z + LANDDEMO_RENDER_RANGE;

	// 렌더링 영역 범위 제한.
	if (sw < 0)
		sw = 0;
	if (ew > gLdMapWidth)
		ew = gLdMapWidth;
	if (sh < 0)
		sh = 0;
	if (eh > gLdMapHeight)
		eh = gLdMapHeight;

	for (h = sh; h < eh; h++)
	{
		fH_mul_gMap = (float) (h * gfLdMapExtention);
		BaseTri[0].z = fH_mul_gMap + gLdMapExtention;
		BaseTri[1].z = fH_mul_gMap + gLdMapExtention;
		BaseTri[2].z = fH_mul_gMap;
		BaseTri[3].z = fH_mul_gMap;

		for (w = sw; w < ew; w++)
		{
			fW_mul_gfMap = w * gfLdMapExtention;
			BaseTri[0].x = fW_mul_gfMap;
			BaseTri[1].x = fW_mul_gfMap + gfLdMapExtention;
			BaseTri[2].x = fW_mul_gfMap;
			BaseTri[3].x = fW_mul_gfMap + gfLdMapExtention;

			h_mul_gHFWidth = h*gLdHFWidth;
			h1_mul_gHFWidth = (h+1)*gLdHFWidth;
			BaseTri[0].y = (float) m_pHeightMap[h1_mul_gHFWidth+w] * gLdHeightScale;
			BaseTri[1].y = (float) m_pHeightMap[h1_mul_gHFWidth+w+1] * gLdHeightScale;
			BaseTri[2].y = (float) m_pHeightMap[h_mul_gHFWidth+w] * gLdHeightScale;
			BaseTri[3].y = (float) m_pHeightMap[h_mul_gHFWidth+w+1] * gLdHeightScale;

			BaseTri[0].color = CalColorFactor(m_pdwColor[h1_mul_gHFWidth+w], 1.0f);
			BaseTri[1].color = CalColorFactor(m_pdwColor[h1_mul_gHFWidth+w+1], 1.0f);
			BaseTri[2].color = CalColorFactor(m_pdwColor[h_mul_gHFWidth+w], 1.0f);
			BaseTri[3].color = CalColorFactor(m_pdwColor[h_mul_gHFWidth+w+1], 1.0f);

			int idx = h*gLdMapWidth+w;
			WORD tidx = m_pMapIdx[idx];
			if( m_pTileTextures )
				lpDDsurface = m_pTileTextures->GetSurfaceByIdx(tidx);

			GET_D3DDEVICE()->SetTexture(0, lpDDsurface);
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
		}
	}
}

void CLandDemo::Render2() // 신맵으로 부터 업어온 데이터를 렌더링한다.(기본적으론 텍스쳐 방식만 틀리다.) ///2007로딩변경
{
	if( !GET_D3DDEVICE() )
		return;

	int map_center_x, map_center_z;
	int sw, sh, ew, eh, w, h;
	int h_mul_gHFWidth, h1_mul_gHFWidth;
	float fH_mul_gMap, fW_mul_gfMap;

	// 중심점의 맵 좌표를 찾는다.
	map_center_x = (int) (m_fCenterX / m_fLdMapExtention);
	map_center_z = (int) (m_fCenterZ / m_fLdMapExtention);

	sw = map_center_x - LANDDEMO_RENDER_RANGE;
	ew = map_center_x + LANDDEMO_RENDER_RANGE;
	sh = map_center_z - LANDDEMO_RENDER_RANGE;
	eh = map_center_z + LANDDEMO_RENDER_RANGE;

	// 렌더링 영역 범위 제한.
	if (sw < 0)
		sw = 0;
	if (ew > m_LdMapWidth)
		ew = m_LdMapWidth;
	if (sh < 0)
		sh = 0;
	if (eh > m_LdMapHeight)
		eh = m_LdMapHeight;

	for (h = sh; h < eh; h++)
	{
		fH_mul_gMap = (float) (h * m_fLdMapExtention);
		BaseTri[0].z = fH_mul_gMap + m_LdMapExtention;
		BaseTri[1].z = fH_mul_gMap + m_LdMapExtention;
		BaseTri[2].z = fH_mul_gMap;
		BaseTri[3].z = fH_mul_gMap;

		for (w = sw; w < ew; w++)
		{
			fW_mul_gfMap = w * m_fLdMapExtention;
			BaseTri[0].x = fW_mul_gfMap;
			BaseTri[1].x = fW_mul_gfMap + m_fLdMapExtention;
			BaseTri[2].x = fW_mul_gfMap;
			BaseTri[3].x = fW_mul_gfMap + m_fLdMapExtention;

			h_mul_gHFWidth = h * m_LdHFWidth;
			h1_mul_gHFWidth = (h+1) * m_LdHFWidth;
			BaseTri[0].y = (float) m_pHeightMap[h1_mul_gHFWidth+w] * m_LdHeightScale;
			BaseTri[1].y = (float) m_pHeightMap[h1_mul_gHFWidth+w+1] * m_LdHeightScale;
			BaseTri[2].y = (float) m_pHeightMap[h_mul_gHFWidth+w] * m_LdHeightScale;
			BaseTri[3].y = (float) m_pHeightMap[h_mul_gHFWidth+w+1] * m_LdHeightScale;

			BaseTri[0].color = 0xffffffff;
			BaseTri[1].color = 0xffffffff;
			BaseTri[2].color = 0xffffffff;
			BaseTri[3].color = 0xffffffff;

			/*
						BaseTri[0].color = CalColorFactor(m_pdwColor[h1_mul_gHFWidth+w], 1.0f);
						BaseTri[1].color = CalColorFactor(m_pdwColor[h1_mul_gHFWidth+w+1], 1.0f);
						BaseTri[2].color = CalColorFactor(m_pdwColor[h_mul_gHFWidth+w], 1.0f);
						BaseTri[3].color = CalColorFactor(m_pdwColor[h_mul_gHFWidth+w+1], 1.0f);
			*/
			// 텍스쳐 혼합 방식쪽은 좀더 생각해보자.
			int idx = h*m_LdMapWidth+w;
			WORD tidx = m_pL1TexID[idx];
			GET_D3DDEVICE()->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx(tidx) );
			tidx = m_pL2TexID[idx];
			GET_D3DDEVICE()->SetTexture( 1, m_pIndexedTexture->GetSurfaceByIdx(tidx) );
			GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, BaseTri, 4, 0);
		}
	}
}

// ---------------------------------------------------------------------
// Name : CalColorFactor()
// Desc : Color값의 r, g, b값을 추출한 후에 color factor를 각 요소에 곱한 후
//      새로 계산된 칼라 값을 리턴.
// ---------------------------------------------------------------------
DWORD CLandDemo::CalColorFactor(DWORD dwColor, float fColorFactor)
{
	DWORD dwR, dwG, dwB;
	DWORD dwResColor;

	dwR = (dwColor & RGB_BITMASK_R) >> 16;
	dwG = (dwColor & RGB_BITMASK_G) >> 8;
	dwB = (dwColor & RGB_BITMASK_B);

	dwR = (DWORD) (dwR * fColorFactor);
	dwG = (DWORD) (dwG * fColorFactor);
	dwB = (DWORD) (dwB * fColorFactor);

	dwResColor = 0;
	dwResColor |= dwR;
	dwResColor = dwResColor << 8;
	dwResColor |= dwG;
	dwResColor = dwResColor << 8;
	dwResColor |= dwB;

	return dwResColor;
}

BOOL CLandDemo::LandDemoInit(char *strFilePath)
{
	PFILE *fp = NULL;
	int len;
	int i, w, h, pos, idx;
	char strMapFileName[255], strHeightFileName[255], strColorTblName[255], strAttribFileName[100];//, strWaveFileName[255];
	char strTemp[1024];
	char *strFileName = NULL;

	strMapFileName[0] = NULL;

	i = w = h = pos = idx = 0;

	if (strFilePath == NULL)
		return FALSE;

	strFileName = new char[255];
	ZeroMemory(strFileName, 255);

	// 불러들일 화일의 목록을 읽는다.
	strcpy(strFileName, strFilePath);
	strcat(strFileName, "LoadFile.txt");

	fp = pfopen(strFileName, "rt");

	if( !fp ) return FALSE;

	pfgets(strTemp, 1024, fp);					// 헤더를 읽는다.

	pfgets(strTemp, 1024, fp);					// 일반 맵인가 던젼인가?

	int d;
	sscanf(strTemp, "%d", &d);

	int g_maptype;
	g_maptype = 2;

	pfgets(strTemp, 1024, fp);
	sscanf(strTemp, "%s", strMapFileName);
	pfgets(strTemp, 1024, fp);
	sscanf(strTemp, "%s", strHeightFileName);
	pfgets(strTemp, 1024, fp);
	sscanf(strTemp, "%s", strColorTblName);
	if (pfgets(strTemp, 1024, fp) != NULL)
	{
		sscanf(strTemp, "%s", strAttribFileName);
	}
	else
	{
		MessageBox(NULL, "Failed to read property map", "warning", MB_OK);
		strAttribFileName[0] = NULL;
	}

	while (pfgets(strTemp, 1024, fp) != NULL)
		;
	pfclose(fp);
	fp = NULL;

	// 맵화일을 연다.
	fp = pfopen(strMapFileName, "rb");
	assert(fp != NULL);

	// 먼저 화일 이름의 길이를 읽어 들인다.
	if (g_maptype == 2)
	{
		pfread(&len, sizeof(unsigned int), 1, fp);
		pfread(m_MapAttr.sTileFileName, sizeof(char)*len, 1, fp);
		m_MapAttr.sTileFileName[len] = NULL;
		pfread(&(m_MapAttr.nMapWidthNum), sizeof(int), 1, fp);		// 맵의 가로 크기
		pfread(&(m_MapAttr.nMapHeightNum), sizeof(int), 1, fp);
		pfread(&(m_MapAttr.nTileWidthNum), sizeof(int), 1, fp);		// 타일 비트맵의 가로 갯수
		pfread(&(m_MapAttr.nTileHeightNum), sizeof(int), 1, fp);
		pfread(&(m_MapAttr.nTilePixelWidth), sizeof(int), 1, fp);
		pfread(&(m_MapAttr.nTilePixelHeight), sizeof(int), 1, fp);

		// 타일 인덱스를 연다.
		SAFE_DELETE_ARRAY(m_pMapIdx);
		assert(m_pMapIdx == NULL);
		m_pMapIdx = new WORD[m_MapAttr.nMapWidthNum*m_MapAttr.nMapHeightNum];
		len = pfread(m_pMapIdx, sizeof(WORD)*m_MapAttr.nMapWidthNum*m_MapAttr.nMapHeightNum, 1, fp);
	}

	pfclose(fp);
	// 맵화일 읽기 끝

	// 전역 변수 설정 - 고쳐야 할 텐데... 헐
	gLdMapWidth = m_MapAttr.nMapWidthNum;					// 맵의 x축 넓이
	gLdMapHeight = m_MapAttr.nMapHeightNum;
	gLdHFWidth = gLdMapWidth + 1;
	gLdHFHeight = gLdMapHeight + 1;

	if(g_maptype == 2)
	{
		gLdMapExtention = 20;					// 스케일 팩터 - 정수형
		gfLdMapExtention = 20.0f;				// 스케일 팩터 - 실수형
	}

	g_nLdMapCellWidth = gLdMapExtention;						// 한 셀의 크기.
	g_nLdMapCellHeight = gLdMapExtention;

	g_nLdMapCellWidthCount = gLdMapWidth / (g_nLdMapCellWidth/gLdMapExtention);
	g_nLdMapCellHeightCount = gLdMapHeight / (g_nLdMapCellHeight/gLdMapExtention);

	if (g_maptype == 2)
	{
		m_nAttrMapWidth = gLdMapWidth * gLdMapExtention/10;
		m_nAttrMapByteWidth = (m_nAttrMapWidth-1) / 8 + 1;
		m_nAttrMapHeight = gLdMapHeight * gLdMapExtention/10;
	}

	// 타일 텍스처 로딩
	SAFE_DELETE(m_pTileTextures);
	g_bDungeon = FALSE;
	m_pTileTextures = new TileTextures;
	if( m_pTileTextures )
	{
		m_pTileTextures->SetTexturePath(strFilePath);
		if (m_pTileTextures->CreateTextureFromFile(m_MapAttr.sTileFileName, m_MapAttr.nTilePixelWidth,
				m_MapAttr.nTilePixelHeight) < 0)
		{
			m_pTileTextures->SetTexturePath("map/room/startzone/");
			m_pTileTextures->CreateTextureFromFile("room64.bmp", 64, 64);
		}
		m_pTileTextures->Restore(GET_D3DDEVICE());
	}

	// height field 화일을 연다.
	SAFE_DELETE_ARRAY(m_pHeightMap);
	if ((fp = pfopen(strHeightFileName, "r+b")) != NULL)
	{
		m_pHeightMap = new unsigned char[gLdHFWidth*gLdHFHeight];
		int count = pfread(m_pHeightMap, sizeof(unsigned char), gLdHFWidth*gLdHFHeight, fp);
		if (count != gLdHFWidth*gLdHFHeight)
			// 잘못된 하이트파일일때에는 하이트 필드를 초기화 시킨다.
			ZeroMemory(m_pHeightMap, gLdHFWidth*gLdHFHeight);

		pfclose(fp);
	}

	// 칼라 테이블..
	PFILE *fpClr = NULL;
	fpClr = pfopen(strColorTblName, "rb");
	assert(fpClr != NULL);
	SAFE_DELETE_ARRAY(m_pdwColor);
	m_pdwColor = new DWORD[gLdHFWidth*gLdHFHeight];

	pfread(m_pdwColor, sizeof(DWORD)*gLdHFWidth*gLdHFHeight, 1, fpClr);
	pfclose(fpClr);

	if (g_maptype == 2)
	{
		// 타일 하나에 대한 인덱스를 집어 넣은 템플릿.
		BaseTri[0] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,256.0f), 0x00777777, 0, 0.01f, 0.01f);
		BaseTri[1] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,256.0f),0x00777777, 0, 0.99f, 0.01f);
		BaseTri[2] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,0.0f), 0x00777777, 0, 0.01f, 0.99f);
		BaseTri[3] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,0.0f), 0x00777777, 0, 0.99f, 0.99f);
	}

	SAFE_DELETE_ARRAY(strFileName);

	return TRUE;
}