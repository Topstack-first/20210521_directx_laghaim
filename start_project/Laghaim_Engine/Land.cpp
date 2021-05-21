#include "stdafx.h"
#define D3D_OVERLOADS
#include <list>
#include <stack>
#include <commdlg.h>
#include <tchar.h>
#include <ddraw.h>
#include <d3d.h>
#include <crtdbg.h>
#include "Config.h"
#include "d3dmath.h"
#include "Frustum.h"
#include "Land.h"
#include "QuadTreeNode.h"
#include "DFloor.h"
#include "IndexedTexture.h"
#include "Nk3DMipMap.h"
#include "WaterObj.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "NKTerQuadNode.h"
#include "VirtualFile.h"

using namespace std;

// Terrain 에 대한 전역 변수
int		gMapWidth;					// 맵의 x축 넓이
int		gMapHeight;
int		gHFWidth;
int		gHFHeight;
int		gMapExtention = 50;					// 스케일 팩터 - 정수형
float	gfMapExtention = 50.0f;				// 스케일 팩터 - 실수형
int		gHeightScale = 2;
float	gfMapRangeWidth;					// 좌표의 범위.
float   gfMapRangeHeight;					// 좌표의 범위.

// 맵을 셀로 나눈 것에 대한 정의.
int g_nMapCellWidth;						// 한 셀의 크기.
int g_nMapCellHeight;
int g_nMapCellWidthCount;
int g_nMapCellHeightCount;

//
DWORD	RGB_BITMASK_A = 0xFF000000;
DWORD	RGB_BITMASK_R = 0x00FF0000;
DWORD	RGB_BITMASK_G = 0x0000FF00;
DWORD	RGB_BITMASK_B = 0x000000FF;

int		CLand::m_nVisibleCount;
int		CLand::m_nMaxVisibleCount;
float	CLand::m_fWaterAltitude = 95.0f;				// 50 level * gHeightScale

extern BOOL g_bDungeon;
extern BOOL g_bMipmap;
extern BOOL g_bDetailMipmap;

extern int g_nCurWorldIdx;
extern DWORD g_dwBaseTime;
extern int g_RohFlyLevel;

extern BOOL g_bMapAttrEncrypt;

BOOL g_bNewField = FALSE;

#define WATER_ANIM_TIME		10000	// 텍스춰 한장이 돌아가는 시간
#define WATER_UV_FACTOR		1		// 한 텍스춰가 차지하는 타일
#define LAVA_UV_FACTOR		4		// 한 텍스춰가 차지하는 타일
#define WAVE_ANIM_TIME		10000	// 텍스춰 한장이 돌아가는 시간
#define WAVE_UV_FACTOR		1		// 한 텍스춰가 차지하는 타일
#define WAVE_ALPHA_TIME		100		// 알파값이 변하는 주기

extern BOOL	g_bNewTerrain;

CLand::CLand()
{
	m_pHeightMap = NULL;
	m_pAttrMap = NULL;
	m_pAttrMapDummy = NULL;
	m_pTileDrawMap = NULL;
	m_pQuadTreeNode = NULL;
	m_pDFloor = NULL;
	m_pMapIdx = NULL;
	m_pDungeonIdx = NULL;
	m_pTileTexIdx = NULL;
	m_pTileTextures = NULL;
	m_pNk3DMipMap = NULL;
	m_pIndexedTexture = NULL;
	m_pWaterObj = NULL;	
	m_pdwColor = NULL;
	m_puAlpha = NULL;
	m_pnVisibleIndex = NULL;
	m_pfFarWaterFactor = NULL;

	m_dwDayLight = 0x00000000;
	m_fLightFactor = 1.0f;

	m_pd3dDevice = NULL;

	m_bCanDoModulate = FALSE;
	m_bCanDoModulate2X = FALSE;
	m_bCanHighQuality = TRUE;

	m_bHourLight = FALSE;

	// 지면의 spot light 초기값 읽기.
	// 파일을 읽어서 바닥라이트의 거리범위와 밝기인자를 읽는다.
	// 밝기 인자의 최대값은 1.0f
	FILE *fp = NULL;

	char temp[256];
	if ( (fp = fopen("dunLight.txt", "rt")) != NULL)
	{
		fgets(temp, sizeof(temp), fp);		// read dummy
		fgets(temp, sizeof(temp), fp);		// read two factor
		sscanf(temp, "%f %f", &m_fDunDistRange, &m_fDunSpotFactor);
		fclose(fp);
	}
	else
	{
		m_fDunDistRange = 50.0f;
		m_fDunSpotFactor = 0.9f;
	}

	fp = NULL;
	// 필드.
	if ( (fp = fopen("LandLight.txt", "rt")) != NULL)
	{
		fgets(temp, sizeof(temp), fp);		// read dummy
		fgets(temp, sizeof(temp), fp);		// read two factor
		sscanf(temp, "%f %f", &m_fLandDistRange, &m_fLandSpotFactor);
		fclose(fp);
	}
	else
	{
		m_fLandDistRange = 110.0f;
		m_fLandSpotFactor = 0.6f;
	}

	D3DVECTOR normal = Normalize(D3DVECTOR(-1, -1, 0));
	
	m_WaterRect[0].x = 0.0f;
	m_WaterRect[0].y = m_fWaterAltitude;
	m_WaterRect[0].z = 0.0f;
	m_WaterRect[0].nx = normal.x;
	m_WaterRect[0].ny = normal.y;
	m_WaterRect[0].nz = normal.z;
	m_WaterRect[0].tu = 0.0f;
	m_WaterRect[0].tv = 0.0f;
	m_WaterRect[0].diffuse = 0xffffffff;
	m_WaterRect[1] = m_WaterRect[2] = m_WaterRect[3] = m_WaterRect[0];

	CalcWaterNoise();
	
	m_pD3D = NULL;
	int i = 0;
	for( i = 0; i < 4; i++ )
	{
		m_aRootNode[i] = NULL;
		m_aVB[i] = NULL;
		m_aClrMapTexL2[i] = NULL;
		m_aClrMapTexL3[i] = NULL;
	}
	m_pAlphaMapL2 = NULL;
	m_pAlphaMapL3 = NULL;
	for( i = 0; i < 6; i++ )
	{
		m_pLightMapsR[i] = NULL;
		m_pLightMapsG[i] = NULL;
		m_pLightMapsB[i] = NULL;
	}
	m_ppTerIBL1 = NULL;
	m_ppTerIBL2 = NULL;
	m_ppTerIBL3 = NULL;
	m_pTerIBCountL1 = NULL;
	m_pTerIBCountL2 = NULL;
	m_pTerIBCountL3 = NULL;
	m_nTimeID = 0;
}

CLand::~CLand()
{
	Destroy();
}

BOOL CLand::Init(char *strFilePath, IndexedTexture *pIndexedTexture, int nMapNum)
{
	Destroy();

	PFILE *fp = NULL;
	int len;
	char strMapFileName[255], strHeightFileName[255], strWaveFileName[255], strColorTblName[255], strAttribFileName[100];
	char strAlphaFileName[255];
	char strTileTblName[255];
	char strTemp[1024];
	char *strFileName = NULL;

	strTileTblName[0] = NULL;
	
	m_nVisibleCount = 0;

	if (strFilePath == NULL)
		return FALSE;

	strFileName = new char[255];

	// 불러들일 화일의 목록을 읽는다.
	strcpy(strFileName, strFilePath);
	strcat(strFileName, "LoadFile.txt");

	fp = pfopen(strFileName, "rt");
	pfgets(strTemp, 1024, fp);					// 헤더를 읽는다.
	pfgets(strTemp, 1024, fp);					// 일반 맵인가 던젼인가?

	int d;
	sscanf(strTemp, "%d", &d);
	if (d == 0 || d == 3)
		g_bDungeon = FALSE;
	else
		g_bDungeon = TRUE;

	g_bNewField = FALSE;
	if (d == 3 || nMapNum == 6)
		g_bNewField = TRUE;

	pfgets(strTemp, 1024, fp);
	sscanf(strTemp, "%s", strMapFileName);
	pfgets(strTemp, 1024, fp);
	sscanf(strTemp, "%s", strHeightFileName);
	pfgets(strTemp, 1024, fp);
	sscanf(strTemp, "%s", strWaveFileName);
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

	//////////////////////////////////////////////////////
	// 스카이 존을 위한 추가 속성 파일
	if (g_bNewField)
	{
		pfgets(strTemp, 1024, fp);
		sscanf(strTemp, "%s", strTileTblName);
	}
	//////////////

	if (g_bDungeon)
	{
		if (pfgets(strTemp, 1024, fp) != NULL)
			sscanf(strTemp, "%s", strAlphaFileName);
	}

	pfclose(fp);
	
	fp = pfopen(strMapFileName, "rb");
	_ASSERTE(fp != NULL);

	// 먼저 화일 이름의 길이를 읽어 들인다.
	if (g_bDungeon)
	{
		m_MapAttr.nMapWidthNum = 256;
		m_MapAttr.nMapHeightNum = 256;
		m_MapAttr.nTileWidthNum = 256;
		m_MapAttr.nTileHeightNum = 256;
		m_MapAttr.nTilePixelWidth = 256;
		m_MapAttr.nTilePixelHeight = 256;

		strcpy(m_MapAttr.sTileFileName, "d1_tile.bmp");
		m_pDungeonIdx = new unsigned char[m_MapAttr.nMapWidthNum*m_MapAttr.nMapHeightNum];
		len = pfread(m_pDungeonIdx, sizeof(unsigned char)*m_MapAttr.nMapWidthNum*m_MapAttr.nMapHeightNum, 1, fp);
	}
	else
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
		_ASSERTE(m_pMapIdx == NULL);
		m_pMapIdx = new WORD[m_MapAttr.nMapWidthNum*m_MapAttr.nMapHeightNum];
		len = pfread(m_pMapIdx, sizeof(WORD)*m_MapAttr.nMapWidthNum*m_MapAttr.nMapHeightNum, 1, fp);
	}

	pfclose(fp);
	// 맵화일 읽기 끝

	// 전역 변수 설정 - 고쳐야 할 텐데... 헐
	gMapWidth = m_MapAttr.nMapWidthNum;					// 맵의 x축 넓이
	gMapHeight = m_MapAttr.nMapHeightNum;
	gHFWidth = gMapWidth + 1;
	gHFHeight = gMapHeight + 1;
	if (g_bDungeon)
	{
		gMapExtention = 20;
		gfMapExtention = 20.0f;
	}
	else if (g_bNewField)  				// 뉴존.
	{
		gMapExtention = 50;					// 스케일 팩터 - 정수형
		gfMapExtention = 50.0f;				// 스케일 팩터 - 실수형
	}
	else
	{
		gMapExtention = 50;					// 스케일 팩터 - 정수형
		gfMapExtention = 50.0f;				// 스케일 팩터 - 실수형
	}

	gfMapRangeWidth = gMapWidth * gfMapExtention;
	gfMapRangeHeight = gMapHeight * gfMapExtention;

	if (g_bDungeon)  						// 던전에서 셀크기 조절 필요.
	{
		g_nMapCellWidth = 80;						// 한 셀의 크기.
		g_nMapCellHeight = 80;
	}
	else
	{
		g_nMapCellWidth = 200;						// 한 셀의 크기.
		g_nMapCellHeight = 200;
	}
	g_nMapCellWidthCount = gMapWidth / (g_nMapCellWidth/gMapExtention);
	g_nMapCellHeightCount = gMapHeight / (g_nMapCellHeight/gMapExtention);

	// 내부적으로 사용되는 속성맵.
	// 속성맵은 타일 하나가 한 비트를 나타낸다.
	if (g_bDungeon)
	{
		m_nAttrMapWidth = gMapWidth * 2;
		m_nAttrMapByteWidth = (m_nAttrMapWidth-1) / 8 + 1;
		m_nAttrMapHeight = gMapHeight * 2;
	}
	else
	{
		m_nAttrMapWidth = gMapWidth * 5;
		m_nAttrMapByteWidth = (m_nAttrMapWidth-1) / 8 + 1;
		m_nAttrMapHeight = gMapHeight * 5;
	}

	// 에러 점검..
	if ( ((gMapWidth*5) % 8) != 0)
	{
		MessageBox(NULL, "he width of the map should be an integer multiple of 8", "WARning", MB_OK);
	}

	// 타일 텍스처 로딩
	m_pTileTextures = new TileTextures;
	m_pTileTextures->SetTexturePath(strFilePath);
	m_pTileTextures->CreateTextureFromFile(m_MapAttr.sTileFileName, m_MapAttr.nTilePixelWidth,
										   m_MapAttr.nTilePixelHeight);
	m_pTileTextures->Restore(m_pd3dDevice);

	// 디테일 텍스처 로딩.
	if (g_bDetailMipmap && g_bDungeon!=TRUE)  	// 현재 던젼에서 디테일 텍스처를 사용하지 않는다.
	{
		m_pNk3DMipMap = new CNk3DMipMap();
		m_pNk3DMipMap->SetTexturePath(strFilePath);
		CHAR* strMipmapNames[] = { "Land_Detail.bmp", "Land_Detail_128.bmp", "Land_Detail2.bmp",
								   "Land_Detail3.bmp", NULL
								 };
		if( FAILED( m_pNk3DMipMap->Create( strMipmapNames, 2 ) ) )
		{
			DEBUG_MSG( "Could not load mipmap bitmap images" );
			// [4/2/2009 D.K ] : failed 이면 false를 넘겨줘야지 왜 E_FAIL이냐..;;
			return FALSE;
		}
		m_pIndexedTexture = new IndexedTexture(1);
		m_pIndexedTexture->SetTexturePath(strFilePath);
		m_nNoneDetailIdx = m_pIndexedTexture->CreateTextureFromFile("none.bmp");

		m_pIndexedTexture->RestoreAllTextures(m_pd3dDevice);
		m_pNk3DMipMap->Restore(m_pd3dDevice);
	}
	else if (g_bDungeon == TRUE)  				// 잠재적 에러 가능성.. 디테일맵에 대한.
	{
		m_pIndexedTexture = new IndexedTexture(1);
		m_pIndexedTexture->SetTexturePath(strFilePath);
		m_nDBottomIdx = m_pIndexedTexture->CreateTextureFromFile("d1_bottom.bmp");
		m_pIndexedTexture->RestoreAllTextures(m_pd3dDevice);
	}
	else
	{
		m_pIndexedTexture = new IndexedTexture(1);
		m_pIndexedTexture->SetTexturePath(strFilePath);
		m_nDetailIdx = m_pIndexedTexture->CreateTextureFromFile("Land_Detail.bmp");
		m_nDetailIdxLow = m_pIndexedTexture->CreateTextureFromFile("Land_Detail_128.bmp");
		m_pIndexedTexture->RestoreAllTextures(m_pd3dDevice);
	}
		
	int i;
	unsigned char check_read, check_cmp;

	// height field 화일을 연다.
	if ((fp = pfopen(strHeightFileName, "r+b")) != NULL)
	{
		m_pHeightMap = new unsigned char[gHFWidth*gHFHeight];
		int count = pfread(m_pHeightMap, sizeof(unsigned char), gHFWidth*gHFHeight, fp);
		if (count != gHFWidth*gHFHeight)
		{
			// 잘못된 하이트파일일때에는 하이트 필드를 초기화 시킨다.
			ZeroMemory(m_pHeightMap, sizeof(m_pHeightMap));
		}
		// 속성체크바이트를 읽는다.
		pfread(&check_read, sizeof(unsigned char), 1, fp);
		pfclose(fp);

		check_cmp = 0;
		for (i=0; i < gHFWidth*gHFHeight; i++)
		{
			check_cmp += m_pHeightMap[i];
		}

		if ((check_cmp == 256) && (check_read != 256))
		{
			return FALSE;
		}
		else if (check_read != (check_cmp+1))
		{
			return FALSE;
		}
	}

	// 맵에서 갈 수 있고 없고를 나타내는 속성 맵을 읽어들인다.
	if ((fp = pfopen(strAttribFileName, "rb")) != NULL)
	{
		m_pAttrMap = new unsigned char[m_nAttrMapByteWidth * m_nAttrMapHeight];

		m_pAttrMapDummy = new unsigned char[m_nAttrMapByteWidth];
		//  첫번째 파트를 읽어 들인다.
		pfread(m_pAttrMap, sizeof(unsigned char)*m_nAttrMapByteWidth*m_nAttrMapHeight/2, 1, fp);
		// 더미 파트를 읽어 들인다.
		pfread(m_pAttrMapDummy, sizeof(unsigned char)*m_nAttrMapByteWidth, 1, fp);
		// 두번째 파트를 읽어 들인다.
		pfread(m_pAttrMap+m_nAttrMapByteWidth*m_nAttrMapHeight/2, sizeof(unsigned char)*m_nAttrMapByteWidth*m_nAttrMapHeight/2, 1, fp);
		// 속성바이트를 읽는다. CRC체크 비슷한것.
		pfread(&check_read, sizeof(unsigned char), 1, fp);
		pfclose(fp);

		// 해외 버전 일치를 위해서.
		if (g_bMapAttrEncrypt)
		{
			int ii;
			for (ii=0; ii < m_nAttrMapByteWidth*m_nAttrMapHeight; ii++)
			{
				m_pAttrMap[ii] -= 10;
			}

			for (ii=0; ii < m_nAttrMapByteWidth; ii++)
			{
				m_pAttrMapDummy[ii] -= 10;
			}

			check_read -= 10;
		}

		check_cmp = 0;
		for (i=0; i < m_nAttrMapByteWidth * m_nAttrMapHeight; i++)
		{
			check_cmp += m_pAttrMap[i];
		}
		for (i=0; i < m_nAttrMapByteWidth; i++)
		{
			check_cmp += m_pAttrMapDummy[i];
		}

		if (m_pAttrMapDummy[0] != (unsigned char) nMapNum)
			return FALSE;

		if (check_read == 0)
			return FALSE;

		if (check_cmp == 255 && check_read != 255)
			return FALSE;

		if (check_cmp != 255)
			if (check_read != (check_cmp+1))
				return FALSE;
	}
	else
	{
		MessageBox(NULL, " have not read the property data", "Warning", MB_OK);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////
	// 뉴존에서만 필요한.
	if (g_bNewField)
	{
		if (strTileTblName[0] != NULL)
		{
			if ((fp = pfopen(strTileTblName, "r+b")) != NULL)
			{
				m_pTileDrawMap = new unsigned char[gMapWidth*gMapHeight];
				int count = pfread(m_pTileDrawMap, sizeof(unsigned char), gMapWidth*gMapHeight, fp);
				if (count != gMapWidth*gMapHeight)
				{
					// 잘못된 하이트파일일때에는 하이트 필드를 초기화 시킨다.
					ZeroMemory(m_pTileDrawMap, sizeof(m_pTileDrawMap));
				}
				pfclose(fp);
			}
		}
	}

	if (g_bDungeon)
	{
		D3DVECTOR from = D3DVECTOR(30.0f, GetHFHeight(30.0f, 30.0f) + 90.0f, -60.0f);
		D3DVECTOR at = D3DVECTOR(30.0f, GetHFHeight(30.0f, 30.0f), 30.0f);
		// 카메라 위치 설정
	}
	else
	{
		D3DVECTOR from = D3DVECTOR(10000.0f, GetHFHeight(10000.0f, 10500.0f) + 100.0f, 10500.0f-100);
		D3DVECTOR at = D3DVECTOR(10000.0f, GetHFHeight(10000.0f, 10500.0f), 10500.0f);
		// 카메라 위치 설정
	}
	// 물 오브젝트 리소스 로딩

	PFILE* fptemp = new PFILE_NFS();
	bool decode = true;
	
	if(!fptemp->Open(strWaveFileName, "rb"))
	{
		decode = false;
	}
	else
		fptemp->Close();

	if ((fp = pfopen(strWaveFileName, "r+b")) != NULL)
	{
		m_pWaterObj = new CWaterObj(pIndexedTexture);
		m_pWaterObj->Read(fp, FALSE, FALSE, decode);
		pfclose(fp);
	}

	// 칼라 테이블..
	PFILE *fpClr = NULL;
	fpClr = pfopen(strColorTblName, "rb");
	_ASSERTE(fpClr != NULL);
	m_pdwColor = new DWORD[gHFWidth*gHFHeight];

	pfread(m_pdwColor, sizeof(DWORD)*gHFWidth*gHFHeight, 1, fpClr);
	pfclose(fpClr);

	// 알파 테이블
	if (g_bDungeon)
	{
		PFILE *fpAlpha = NULL;
		fpAlpha = pfopen(strAlphaFileName, "rb");
		_ASSERTE(fpAlpha!= NULL);
		m_puAlpha = new unsigned char[gHFWidth*gHFHeight];

		pfread(m_puAlpha, sizeof(unsigned char)*gHFWidth*gHFHeight, 1, fpAlpha);
		pfclose(fpAlpha);
	}

	if (g_bDungeon != TRUE)
	{
		// quad tree의 초기 구조체를 만든다.
		m_pQuadTreeNode = new CQuadTreeNode;
		CQuadTreeNode::m_pLand = this;				// 확인함수를 넘겨준다.
		m_pQuadTreeNode->MakeInitStruct(0, LAND_VISIBLE_DEPTH);

		// 타일 하나에 대한 인덱스를 집어 넣은 템플릿.
		m_pQuadTreeNode->BaseTri[0] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,256.0f), 0x00777777, 0, 0.05f, 0.05f);
		m_pQuadTreeNode->BaseTri[1] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,256.0f),0x00777777, 0, 0.95f, 0.05f);
		m_pQuadTreeNode->BaseTri[2] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,0.0f), 0x00777777, 0, 0.05f, 0.95f);
		m_pQuadTreeNode->BaseTri[3] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,0.0f), 0x00777777, 0, 0.95f, 0.95f);
	}
	else
	{
		m_pDFloor = new CDFloor();
		CDFloor::m_pLand = this;

		m_pDFloor->BaseTri[0] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,256.0f), 0x55FFFFFF, 0, 0.01f, 0.01f);
		m_pDFloor->BaseTri[1] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,256.0f),0x55FFFFFF, 0, 0.99f, 0.01f);
		m_pDFloor->BaseTri[2] = D3DLVERTEX(D3DVECTOR( 0.0f,0.0f,0.0f), 0x55FFFFFF, 0, 0.01f, 0.99f);
		m_pDFloor->BaseTri[3] = D3DLVERTEX(D3DVECTOR( 256.0f,0.0f,0.0f), 0x55FFFFFF, 0, 0.99f, 0.99f);
	}

	// 보이는 노드 보관용 버퍼
	if (g_bDungeon)
	{
		m_pnVisibleIndex = new int[(MAP_RENDER_RANGE+1)*(MAP_RENDER_RANGE+1)*4];
		m_pfFarWaterFactor = new float[(MAP_RENDER_RANGE+1)*(MAP_RENDER_RANGE+1)*4];
		m_nMaxVisibleCount = (MAP_RENDER_RANGE+1)*(MAP_RENDER_RANGE+1)*4+1;
	}
	else
	{
		m_pnVisibleIndex = new int[MAX_VISIBLE_NODE+LAND_VISIBLE_WIDTH*4];
		m_pfFarWaterFactor = new float[MAX_VISIBLE_NODE+LAND_VISIBLE_WIDTH*4];
		m_nMaxVisibleCount = MAX_VISIBLE_NODE+LAND_VISIBLE_WIDTH*4;
	}

	SAFE_DELETE_ARRAY(strFileName);

	return TRUE;
}

void CLand::CalcWaterNoise()
{
	int water_uv_factor = WATER_UV_FACTOR;
	int wave_uv_factor = WAVE_UV_FACTOR;
	
	for (int i = 0; i < WATER_NOISE_NUM; i++)
	{
		BOOL bSame = FALSE;
		m_WaterNoise[i] = (rand() % 101) / 101.0f * 0.317f / water_uv_factor;		
		m_WaveNoise[i] = (rand() % 213) / 213.0f * 0.317f / wave_uv_factor;
		for (int j = 0; j < i; j++)
		{
			if (m_WaterNoise[j] == m_WaterNoise[i] || m_WaveNoise[j] == m_WaveNoise[i])
			{
				bSame = TRUE;
				break;
			}
		}
		if (bSame)
		{
			i--;
		}
	}
}

void CLand::InvalidateAllTextures()
{
	if (m_pTileTextures != NULL)
		m_pTileTextures->Invalidate();
	if (m_pIndexedTexture != NULL)
		m_pIndexedTexture->InvalidateAllTextures();
	if (m_pNk3DMipMap != NULL)
		m_pNk3DMipMap->Invalidate();
}

void CLand::RestoreAllTextures()
{
	if (m_pTileTextures != NULL)
		m_pTileTextures->Restore(m_pd3dDevice);
	if (m_pIndexedTexture != NULL)
		m_pIndexedTexture->RestoreAllTextures(m_pd3dDevice);
	if (m_pNk3DMipMap != NULL)
		m_pNk3DMipMap->Restore(m_pd3dDevice);
}

void CLand::QuadInit(CFrustum *pFrustum, float x, float z)
{
	int dx, dz;
	int max_x, max_z, min_x, min_z;			// 스케일이 없는 좌표.

	// visible list count
	m_nVisibleCount = 0;
	//
	dx = (int) (x / gfMapExtention);
	dz = (int) (z / gfMapExtention);

	max_x = dx + LAND_VISIBLE_WIDTH/2;
	min_x = dx - LAND_VISIBLE_WIDTH/2;

	max_z = dz + LAND_VISIBLE_WIDTH/2;
	min_z = dz - LAND_VISIBLE_WIDTH/2;

	// 검사 대상 영역 바운더리 확인.
	if (max_x <= LAND_VISIBLE_WIDTH-1)
	{
		max_x = LAND_VISIBLE_WIDTH-1;
		min_x = 0;
	}
	if (max_x >= gHFWidth-1)
	{
		max_x = gHFWidth-1;
		min_x = gHFWidth-LAND_VISIBLE_WIDTH-1;
	}

	if (max_z <= LAND_VISIBLE_HEIGHT-1)
	{
		max_z = LAND_VISIBLE_HEIGHT-1;
		min_z = 0;
	}

	if (max_z >= gHFHeight-1)
	{
		max_z = gHFHeight-1;
		min_z = gHFHeight-LAND_VISIBLE_HEIGHT-1;
	}
		
	// 맵 좌표를 넘겨 준다.
	m_pQuadTreeNode->Init(min_x, min_z, max_x, max_z, 0);	
	m_pQuadTreeNode->Culling(pFrustum);	
}

void CLand::DungeonInit(float x, float z)
{
	m_fCenterX = x;
	m_fCenterZ = z;

	m_nVisibleCount = 0;

	m_pDFloor->Init(x, z);
}

void CLand::AddVisibleList(int idx, float farWaterFactor)
{
	m_pnVisibleIndex[m_nVisibleCount] = idx;
	m_pfFarWaterFactor[m_nVisibleCount] = (farWaterFactor > 1.0f) ? 1.0f : farWaterFactor;
	m_nVisibleCount++;
	_ASSERTE(m_nVisibleCount < m_nMaxVisibleCount);
}

void CLand::Render(float x, float z)
{
	BOOL bPrevState = FALSE;

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING , FALSE);

	if (g_bDungeon)
	{
		m_pDFloor->Render();
	}
	else
	{
		if (g_bMipmap)
		{
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );
		}

		if (g_bDetailMipmap)
			m_pd3dDevice->SetTextureStageState(1, D3DTSS_MIPFILTER, D3DTFP_POINT );

		m_pQuadTreeNode->SetDevice(m_pd3dDevice);
		m_pQuadTreeNode->SetCenterPos(x, z);
		m_pQuadTreeNode->Render();

		if (g_bMipmap)
		{
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE );
		}

		if (g_bDetailMipmap)
			m_pd3dDevice->SetTextureStageState(1, D3DTSS_MIPFILTER, D3DTFP_NONE );
	}

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING , TRUE);	
}

#define WATER_ALPHA		(0xCC)
void CLand::RenderWaterArea(LPDIRECTDRAWSURFACE7 lpTexture, LPDIRECTDRAWSURFACE7 lpTexture2)
{
	LPDIRECT3DDEVICE7 lpd3dDevice;
	static int count = 0;
	int w, h, i;
	BOOL bRendered = FALSE;

	lpd3dDevice = m_pd3dDevice;

	BOOL lightEnable;
	lpd3dDevice->GetLightEnable(1, &lightEnable);
	lpd3dDevice->LightEnable(1, FALSE);

	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	DWORD alphaArg1, alphaOp;
	lpd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &alphaArg1);
	lpd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,  &alphaOp);
	lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	lpd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);

	// 필드 물은 월드 트랜스폼을 한번 해준다.
	D3DMATRIX matWorld;
	D3DUtil_SetIdentityMatrix( matWorld );
	lpd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matWorld);

	for (i = 0; i < m_nVisibleCount; i++)
	{
		// 타일의 좌표를 가져온다.
		// m_pnVisibleIndex[i] 에 있는 값은 사각타일에 대한 인덱스이다.
		// 그러므로 w와 h는 맵타일의 가로세로 좌표이다.
		// 하이트필드의 가로세로 위치가 아니다.
		w = m_pnVisibleIndex[i] % gMapWidth;
		h = m_pnVisibleIndex[i] / gMapWidth;

		// 타일의 네 개의 꼭지점 중에서 하나라도 수면보다 낮은 높이를 가지고 있는
		// 하이트필드 값이 있으면 워터객체를 그린다.

		DWORD alpha = (DWORD) ((0xff - WATER_ALPHA) * m_pfFarWaterFactor[i] + WATER_ALPHA) << 24;
		if (g_bDungeon)
		{
			alpha = 0xff << 24;
		}
		// 검사는 좌하에서 시계방향으로.
		if ( ( (float) m_pHeightMap[h*gHFWidth+w]*gHeightScale) < m_fWaterAltitude )
		{
			if (g_bDungeon)
				m_pWaterObj->Render(lpd3dDevice, w*gfMapExtention, m_fWaterAltitude, h*gfMapExtention, alpha);
			else
				RenderWaterRect((float)w, (float)h, alpha, lpTexture, lpTexture2);

			bRendered = TRUE;
			continue;
		}

		if ( ( (float) m_pHeightMap[(h+1)*gHFWidth+w]*gHeightScale) < m_fWaterAltitude )
		{
			if (g_bDungeon)
				m_pWaterObj->Render(lpd3dDevice, w*gfMapExtention, m_fWaterAltitude, h*gfMapExtention, alpha);
			else
				RenderWaterRect((float)w, (float)h, alpha, lpTexture, lpTexture2);

			bRendered = TRUE;
			continue;
		}

		if ( ( (float) m_pHeightMap[(h+1)*gHFWidth+w+1]*gHeightScale) < m_fWaterAltitude )
		{
			if (g_bDungeon)
				m_pWaterObj->Render(lpd3dDevice, w*gfMapExtention, m_fWaterAltitude, h*gfMapExtention, alpha);
			else
				RenderWaterRect((float)w, (float)h, alpha, lpTexture, lpTexture2);

			bRendered = TRUE;
			continue;
		}

		if ( ( (float) m_pHeightMap[h*gHFWidth+w+1]*gHeightScale) < m_fWaterAltitude )
		{
			if (g_bDungeon)
				m_pWaterObj->Render(lpd3dDevice, w*gfMapExtention, m_fWaterAltitude, h*gfMapExtention, alpha);
			else
				RenderWaterRect((float)w, (float)h, alpha, lpTexture, lpTexture2);

			bRendered = TRUE;
			continue;
		}
	}
	lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, alphaArg1);
	lpd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, alphaOp);
	lpd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);

	lpd3dDevice->LightEnable(1, lightEnable);

	// 물 애니메이션 프레임을 조절하는 부분을 호출한다.
	if (bRendered)
	{
		m_pWaterObj->AdjustAnimFrame();		
	}
}

void CLand::RenderWaterRect(float x, float z, DWORD alpha, LPDIRECTDRAWSURFACE7 lpTexture, LPDIRECTDRAWSURFACE7 lpTexture2)
{
	int water_uv_factor = WATER_UV_FACTOR;
	int wave_uv_factor = WAVE_UV_FACTOR;
	float u = (1.0f / water_uv_factor) * ((int)x % water_uv_factor);
	float v = (1.0f / water_uv_factor) * ((int)z % water_uv_factor)
			  + (g_dwBaseTime % WATER_ANIM_TIME) * (1.0f / WATER_ANIM_TIME);
	float u2 = (1.0f / wave_uv_factor) * ((int)x % wave_uv_factor);
	float v2 = (1.0f / wave_uv_factor) * ((int)z % wave_uv_factor);

	int w = (int)x;
	int h = (int)z;
	x *= gfMapExtention;
	z *= gfMapExtention;

	// 물
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);

	m_WaterRect[0].diffuse = m_WaterRect[1].diffuse
							 = m_WaterRect[2].diffuse = m_WaterRect[3].diffuse = alpha;

	m_WaterRect[0].x = x;
	m_WaterRect[0].z = z;
	m_WaterRect[0].tu = u + m_WaterNoise[(w+(h*2)+0)%WATER_NOISE_NUM];
	m_WaterRect[0].tv = v + m_WaterNoise[(h+(w*3)-4)%WATER_NOISE_NUM];

	m_WaterRect[1].x = x;
	m_WaterRect[1].z = z+gfMapExtention;
	m_WaterRect[1].tu = u + m_WaterNoise[(w+(h*2)+2)%WATER_NOISE_NUM];
	m_WaterRect[1].tv = v + (1.0f / water_uv_factor) + m_WaterNoise[(h+(w*3)-3)%WATER_NOISE_NUM];

	m_WaterRect[2].x = x+gfMapExtention;
	m_WaterRect[2].z = z;
	m_WaterRect[2].tu = u + (1.0f / water_uv_factor) + m_WaterNoise[(w+(h*2)+1)%WATER_NOISE_NUM];
	m_WaterRect[2].tv = v + m_WaterNoise[(h+(w*3)-1)%WATER_NOISE_NUM];

	m_WaterRect[3].x = x+gfMapExtention;
	m_WaterRect[3].z = z+gfMapExtention;
	m_WaterRect[3].tu = u + (1.0f / water_uv_factor) + m_WaterNoise[(w+(h*2)+3)%WATER_NOISE_NUM];
	m_WaterRect[3].tv = v + (1.0f / water_uv_factor) + m_WaterNoise[(h+(w*3)-0)%WATER_NOISE_NUM];

	m_pd3dDevice->SetTexture(0, lpTexture);
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, FVF_LAGHAIM,
								 m_WaterRect, 4, 0 );

	if (g_nCurWorldIdx == WORLD_FIELD) // 데카르디는 한장만 그린다.
		return;

	// 파도
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);

	m_WaterRect[0].diffuse = m_WaterRect[1].diffuse
							 = m_WaterRect[2].diffuse = m_WaterRect[3].diffuse = 0xff << 24; //0xffffffff;

	m_WaterRect[0].tu = u2 + m_WaveNoise[(w-(h*2)+2)%WATER_NOISE_NUM];
	m_WaterRect[0].tv = v2 + m_WaveNoise[(h-(w*3)-1)%WATER_NOISE_NUM];

	m_WaterRect[1].tu = u2 + m_WaveNoise[(w-(h*2)+0)%WATER_NOISE_NUM];
	m_WaterRect[1].tv = v2 + (1.0f / wave_uv_factor) + m_WaveNoise[(h-(w*3)-0)%WATER_NOISE_NUM];

	m_WaterRect[2].tu = u2 + (1.0f / wave_uv_factor) + m_WaveNoise[(w-(h*2)+3)%WATER_NOISE_NUM];
	m_WaterRect[2].tv = v2 + m_WaveNoise[(h-(w*3)-4)%WATER_NOISE_NUM];

	m_WaterRect[3].tu = u2 + (1.0f / wave_uv_factor) + m_WaveNoise[(w-(h*2)+1)%WATER_NOISE_NUM];
	m_WaterRect[3].tv = v2 + (1.0f / wave_uv_factor) + m_WaveNoise[(h-(w*3)-3)%WATER_NOISE_NUM];

	m_pd3dDevice->SetTexture(0, lpTexture2);
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, FVF_LAGHAIM,
								 m_WaterRect, 4, 0 );
}

int CLand::GetHF(int x, int z)
{
	int y;

	y = m_pHeightMap[z*gHFWidth+x]*gHeightScale;

	return y;
}

int CLand::GetMapIndex(D3DVECTOR vCoord)
{
	int nTileX, nTileZ;

	// 어떤 타일인가 알아낸다.
	nTileX = (int) (vCoord.x / gfMapExtention);
	nTileZ = (int) (vCoord.z / gfMapExtention);

	if (nTileX < 0 || nTileZ < 0 || nTileX >= gMapWidth || nTileZ >= gMapHeight)
		return -1;

	return (nTileZ * gMapWidth + nTileX);
}

BOOL CLand::TranslateTileToWorld(D3DVECTOR &reVec, int w, int h)
{
	reVec.x = w * gfMapExtention;
	reVec.z = h * gfMapExtention;
	reVec.y = GetHFHeight(reVec.x, reVec.z);

	if (IsValidWorldCoord(reVec))
		return TRUE;
	else
		return FALSE;
}

BOOL CLand::IsValidWorldCoord(D3DVECTOR &vec)
{
	if (vec.x >= 0 && vec.x < gHFWidth*gfMapExtention && vec.z >= 0 && vec.z < gHFHeight*gfMapExtention)
		return TRUE;
	else
		return FALSE;
}

#define ADJUSTHEIGHT_AREA 4
float adjust_pos[ADJUSTHEIGHT_AREA][5] =
{
	{220.0f, 7600.0f,6230.0f, 7960.0f, 6355.0f},	// 동쪽
	{220.0f, 5190.0f,6230.0f, 5560.0f, 6370.0f},	// 서쪽
	{220.0f, 6530.0f,7150.0f, 6670.0f, 7500.0f},	// 북쪽
	{220.0f, 6530.0f,5100.0f, 6670.0f, 5450.0f}		// 남쪽
};

float CLand::GetHFHeight(float x, float z)
{
	float y = 0.0f;

	int nTileX;			// x번째 타일 (x축에서)
	int nTileZ;			// z번째 타일 (z축에서)
	BOOL bLeft;			// 타일내에 왼쪽 삼각형인가 오른쪽 삼각형인가...
	int rx, rz;

	D3DVECTOR vTemp[3];
	D3DVECTOR U, V, N;

	if ( x < 0.0f || z < 0.0f || x > gfMapExtention*gMapWidth || z > gfMapExtention*gMapHeight)
		return -1.0f;

	// 높이 조정 부분.
	if( !g_bNewTerrain && g_nCurWorldIdx == WORLD_FIELD )
	{
		for( int i = 0; i < ADJUSTHEIGHT_AREA; i++ )
		{
			if( x >= adjust_pos[i][1] && x <= adjust_pos[i][3] && z >= adjust_pos[i][2] && z <= adjust_pos[i][4] )
				return adjust_pos[i][0];
		}
	}


	// 어떤 타일인가 알아낸다.
	nTileX = (int) (x / gfMapExtention);
	nTileZ = (int) (z / gfMapExtention);

	// 타일내의 두 개의 삼각형중에 왼쪽과 오른쪽 어느쪽에 x, y가 속해 있는지 판단.
	rx = (int) x % gMapExtention;
	rz = (int) z % gMapExtention;

	bLeft = abs(rx) < abs(rz) ? TRUE : FALSE;


	if (bLeft)
	{
		vTemp[0] = D3DVECTOR(nTileX*gfMapExtention, m_pHeightMap[(int)(nTileZ*gHFWidth+nTileX)], nTileZ*gfMapExtention);
		vTemp[1] = D3DVECTOR((nTileX+1)*gfMapExtention, m_pHeightMap[(int)((nTileZ+1)*gHFWidth+nTileX+1)], (nTileZ+1)*gfMapExtention);
		vTemp[2] = D3DVECTOR(nTileX*gfMapExtention, m_pHeightMap[(int)((nTileZ+1)*gHFWidth+nTileX)], (nTileZ+1)*gfMapExtention);
	}
	else
	{
		vTemp[0] = D3DVECTOR(nTileX*gfMapExtention, m_pHeightMap[(int)(nTileZ*gHFWidth+nTileX)], nTileZ*gfMapExtention);
		vTemp[1] = D3DVECTOR((nTileX+1)*gfMapExtention, m_pHeightMap[(int) (nTileZ*gHFWidth+nTileX+1)], nTileZ*gfMapExtention);
		vTemp[2] = D3DVECTOR((nTileX+1)*gfMapExtention, m_pHeightMap[(int) ((nTileZ+1)*gHFWidth+nTileX+1)], (nTileZ+1)*gfMapExtention);
	}

	Nk3DUtil_VectorMake(U, vTemp[1], vTemp[0]);
	Nk3DUtil_VectorMake(V, vTemp[2], vTemp[0]);

	N = CrossProduct(U, V);
	N = Normalize(N);

	// 평면의 방정식을 이용해서 Y좌표를 얻어낸다.
	y = -(N.x*vTemp[2].x + N.y*vTemp[2].y + N.z*vTemp[2].z);
	y = -(N.x*x + N.z*z + y) / N.y;


	return y * gHeightScale;
}

void CLand::GetRay(D3DVECTOR &org, D3DVECTOR &dir, long mx, long my)
{
	D3DVECTOR v;
	D3DMATRIX matProj, matView;
	D3DVIEWPORT7 vp;

	m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProj);
	m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);

	m_pd3dDevice->GetViewport(&vp);
	v.x = ( ( ( 2.0f * mx ) / vp.dwWidth  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * my ) / vp.dwHeight ) - 1 ) / matProj._22;
	v.z =  1.0f;

	// 뷰매트릭의 인버스 매트릭스 구하기
	D3DMATRIX m;
	D3DMath_MatrixInvert(m, matView);

	// Transform the screen space pick ray into 3D space
	dir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	dir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	dir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	org.x = m._41;
	org.y = m._42;
	org.z = m._43;
}

void CLand::GetPipelineMatrix(D3DMATRIX &matPipe)
{
	D3DMATRIX matView, matProj, matVPort;
	D3DVIEWPORT7 vp;

	m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProj);
	m_pd3dDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, &matView);
	m_pd3dDevice->GetViewport(&vp);

	D3DUtil_SetIdentityMatrix(matVPort);
	matVPort._11 = (float) vp.dwWidth/2;
	matVPort._22 = (float) vp.dwHeight/2;
	matVPort._22 = 0.0f - matVPort._22;
	matVPort._33 = vp.dvMaxZ - vp.dvMinZ;
	matVPort._41 = vp.dwX + (float) vp.dwWidth/2;
	matVPort._42 = (float) vp.dwHeight/2 + vp.dwY;
	matVPort._43 = vp.dvMinZ;

	D3DMath_MatrixMultiply(matPipe, matView, matProj);
	D3DMath_MatrixMultiply(matPipe, matPipe, matVPort);
}

BOOL CLand::PickPoint(D3DVECTOR &reVec, long mx, long my)
{
	D3DVECTOR vPickRayDir;
	D3DVECTOR vPickRayOrig;

	// ray를 구한다.
	GetRay(vPickRayOrig, vPickRayDir, mx, my);

	if( g_bNewTerrain )
	{
		float	fDist = FLT_MAX;

		m_aRootNode[0]->GetPickedPoint( vPickRayOrig, vPickRayDir, fDist );
		if( gMapWidth == 256 )
		{
			for( int nIndex = 1; nIndex < 4; nIndex++ )
				m_aRootNode[nIndex]->GetPickedPoint( vPickRayOrig, vPickRayDir, fDist );
		}

		if( fDist != FLT_MAX )
		{
			vPickRayDir *= fDist;
			reVec = vPickRayOrig + vPickRayDir;
			return TRUE;
		}

		return FALSE;
	}
	else
	{
		D3DMATRIX matPipe;
		D3DVECTOR vecOrg[4];
		int w, h, i;
		BOOL bLeft, bInsideMap = FALSE;

		// pipeline matrix를 구한다.
		GetPipelineMatrix(matPipe);

		// Visible List에서 마우스가 선택한 타일을 찾는다.
		for (i = 0; i < m_nVisibleCount && !bInsideMap; i++)
		{
			w = m_pnVisibleIndex[i] % gMapWidth;
			h = m_pnVisibleIndex[i] / gMapWidth;
			// 타일에 해당하는 네 개의 벡터를 만든다.
			// 왼쪽 하단 점에서 시계 방향으로.
			// 좌하
			vecOrg[0].x = w * gfMapExtention;
			vecOrg[0].y = (float) (m_pHeightMap[h*gHFWidth+w]*gHeightScale);
			vecOrg[0].z = h * gfMapExtention;
			if (vecOrg[0].y < m_fWaterAltitude)
			{
				vecOrg[0].y = m_fWaterAltitude;
			}

			// 좌상
			vecOrg[1].x = vecOrg[0].x;
			vecOrg[1].y = (float) (m_pHeightMap[(h+1)*gHFWidth+w]*gHeightScale);
			vecOrg[1].z = (h+1)*gfMapExtention;
			if (vecOrg[1].y < m_fWaterAltitude)
			{
				vecOrg[1].y = m_fWaterAltitude;
			}

			//우상
			vecOrg[2].x = (w+1)*gfMapExtention;
			vecOrg[2].y = (float) (m_pHeightMap[(h+1)*gHFWidth+w+1]*gHeightScale);
			vecOrg[2].z = vecOrg[1].z ;
			if (vecOrg[2].y < m_fWaterAltitude)
			{
				vecOrg[2].y = m_fWaterAltitude;
			}

			//우하
			vecOrg[3].x = (w+1)*gfMapExtention;
			vecOrg[3].y = (float) (m_pHeightMap[h*gHFWidth+w+1]*gHeightScale);
			vecOrg[3].z = vecOrg[0].z;
			if (vecOrg[3].y < m_fWaterAltitude)
			{
				vecOrg[3].y = m_fWaterAltitude;
			}

			bInsideMap = IsInsideMap(vecOrg, matPipe, bLeft, mx, my);
			if (bInsideMap)
				break;
		}

		float plane[4];				// 평면의 방정식
		if (bInsideMap)
		{
			D3DVECTOR AB, AC, temp;
			// 삼각형의 평면의 방정식을 구한다. (월드공간상의 타일 좌표를 가지고)
			// vecOrg 에는 선택된 타일의 월드좌표가 있다.
			if (bLeft)  			// 정점 0, 1, 2로 평면의 방정식을 구한다.
			{
				AB.x = vecOrg[1].x - vecOrg[0].x;
				AB.y = vecOrg[1].y - vecOrg[0].y;
				AB.z = vecOrg[1].z - vecOrg[0].z;

				AC.x = vecOrg[2].x - vecOrg[0].x;
				AC.y = vecOrg[2].y - vecOrg[0].y;
				AC.z = vecOrg[2].z - vecOrg[0].z;

			}
			else  				// 정점 1, 2, 3으로 평면의 방정식을 구한다.
			{
				AB.x = vecOrg[2].x - vecOrg[0].x;
				AB.y = vecOrg[2].y - vecOrg[0].y;
				AB.z = vecOrg[2].z - vecOrg[0].z;

				AC.x = vecOrg[3].x - vecOrg[0].x;
				AC.y = vecOrg[3].y - vecOrg[0].y;
				AC.z = vecOrg[3].z - vecOrg[0].z;
			}

			// 두 벡터를 이용하여 평면의 방정식 구하기.
			temp = CrossProduct(AB, AC);
			temp = Normalize(temp);
			plane[0] = temp.x;
			plane[1] = temp.y;
			plane[2] = temp.z;
			// vecOrg[0]는 두 삼각형에 공통인 점
			plane[3] = 0.0f - (plane[0]*vecOrg[0].x + plane[1]*vecOrg[0].y + plane[2]*vecOrg[0].z);

			// 평면의 방정식과 ray와의 교차점 구하기 ---
			/*
						Ax + By + Cz + D
				t = -  ------------------
						  Ai + Bj + Ck

				a = ray.direction.x * A + ray.direction.y * B +ray.direction.z * C;
				b = - ( ray.origin.x * A + ray.origin.y * B + ray.origin.z * C + D );
				t = b / a;
			*/
			float a, b, t = 1.0f;
			a = vPickRayDir.x * plane[0] + vPickRayDir.y * plane[1] + vPickRayDir.z*plane[2];
			b = - (vPickRayOrig.x * plane[0] + vPickRayOrig.y * plane[1] + vPickRayOrig.z*plane[2] + plane[3]);

			if (fabs( a ) > g_EPSILON)
				t = b / a;
			else
				t = t;

			/*
				A ray is a parametric form of a 3D line
				vStart + vDir * t
			*/

			D3DVECTOR x;
			reVec.x = vPickRayOrig.x + vPickRayDir.x * t;
			reVec.y = vPickRayOrig.y + vPickRayDir.y * t;
			reVec.z = vPickRayOrig.z + vPickRayDir.z * t;

			return TRUE;
		}

		return FALSE;
	}
}

BOOL CLand::IsInsideMap(D3DVECTOR *vecOrg, D3DMATRIX &matPipe, BOOL &bLeft, long mx, long my)
{
	BOOL bInsideMap = FALSE;
	long bCCW[4];
	D3DVECTOR vecTrans[4];

	int k;
	for (k = 0; k < 4; k++)
	{
		D3DMath_VectorMatrixMultiply(vecTrans[k], vecOrg[k], matPipe);
	}

	// 화면 좌표가 마이너스로 나올 경우에는 다음으로 진행.
	//if (vecTrans[0].x < 0.0f || vecTrans[0].y < 0 || vecTrans[
	// (AB) 와 (AC) 벡터의 외적을 구하여 방향을 세 점의 위치 관계를 판다.
	bCCW[0] = Nk3DMath_CCW(vecTrans[0].x, vecTrans[0].y, vecTrans[1].x, vecTrans[1].y, mx, my);
	bCCW[1] = Nk3DMath_CCW(vecTrans[1].x, vecTrans[1].y, vecTrans[2].x, vecTrans[2].y,  mx,  my);
	bCCW[2] = Nk3DMath_CCW(vecTrans[2].x, vecTrans[2].y, vecTrans[3].x, vecTrans[3].y,  mx,  my);
	bCCW[3] = Nk3DMath_CCW(vecTrans[3].x, vecTrans[3].y, vecTrans[0].x, vecTrans[0].y,  mx,  my);

	if ((bCCW[0] + bCCW[1] + bCCW[2] + bCCW[3]) == 4)
	{
		// 화면 좌표 안으로 들어오는지 체크한다.
		// x좌표만 체크.
		bInsideMap = TRUE;

		for (k=0; k < 4; k++)
		{
			if ( vecTrans[k].x <= -1024.0f || vecTrans[k].x > 2048.0f)
			{
				bInsideMap = FALSE;
				break;
			}			
		}

		if (bInsideMap == FALSE)
			return FALSE;

		// 사각 타일 안에 존재한다. 왼쪽 삼각형에 있는지 오른쪽 삼각형에 있는지 판단.
		if (Nk3DMath_CCW(vecTrans[0].x, vecTrans[0].y, vecTrans[2].x, vecTrans[2].y,  mx,  my) > 0)
			bLeft = FALSE;
		else
			bLeft = TRUE;
	}

	return bInsideMap;
}

BOOL CLand::PickPoint(D3DVECTOR &reVec, long mx, long my, int &tileIdx)
{
	D3DVECTOR vPickRayDir;
	D3DVECTOR vPickRayOrig;

	// ray를 구한다.
	GetRay(vPickRayOrig, vPickRayDir, mx, my);

	if( g_bNewTerrain )
	{
		float	fDist = FLT_MAX;

		m_aRootNode[0]->GetPickedPoint( vPickRayOrig, vPickRayDir, fDist );
		if( gMapWidth == 256 )
		{
			for( int nIndex = 1; nIndex < 4; nIndex++ )
				m_aRootNode[nIndex]->GetPickedPoint( vPickRayOrig, vPickRayDir, fDist );
		}

		if( fDist != FLT_MAX )
		{
			vPickRayDir *= fDist;
			reVec = vPickRayOrig + vPickRayDir;
			int	x = (int)( reVec.x / gfMapExtention );
			int	z = (int)( reVec.z / gfMapExtention );

			tileIdx = x + z * gMapWidth;
			return TRUE;
		}

		tileIdx = -1;
		return FALSE;
	}
	else
	{
		D3DMATRIX matPipe;
		D3DVECTOR vecOrg[4], vecTrans[4];
		int w, h, i;
		long bCCW[4];
		BOOL bLeft, bInsideMap = FALSE;

		// pipeline matrix를 구한다.
		GetPipelineMatrix(matPipe);

		// Visible List에서 마우스가 선택한 타일을 찾는다.
		for (i = 0; i < m_nVisibleCount; i++)
		{
			// 타일의 좌표를 가져온다.
			// m_pnVisibleIndex[i] 에 있는 값은 사각타일에 대한 인덱스이다.
			// 그러므로 w와 h는 맵타일의 가로세로 좌표이다.
			// 하이트필드의 가로세로 위치가 아니다.
			w = m_pnVisibleIndex[i] % gMapWidth;
			h = m_pnVisibleIndex[i] / gMapWidth;
			// 타일에 해당하는 네 개의 벡터를 만든다.
			// 왼쪽 하단 점에서 시계 방향으로.
			// 좌하
			vecOrg[0].x = w * gfMapExtention;
			vecOrg[0].y = (float) (m_pHeightMap[h*gHFWidth+w]*gHeightScale);
			vecOrg[0].z = h * gfMapExtention;

			// 좌상
			vecOrg[1].x = vecOrg[0].x;
			vecOrg[1].y = (float) (m_pHeightMap[(h+1)*gHFWidth+w]*gHeightScale);
			vecOrg[1].z = (h+1)*gfMapExtention;

			//우상
			vecOrg[2].x = (w+1)*gfMapExtention;
			vecOrg[2].y = (float) (m_pHeightMap[(h+1)*gHFWidth+w+1]*gHeightScale);
			vecOrg[2].z = vecOrg[1].z ;

			//우하
			vecOrg[3].x = (w+1)*gfMapExtention;
			vecOrg[3].y = (float) (m_pHeightMap[h*gHFWidth+w+1]*gHeightScale);
			vecOrg[3].z = vecOrg[0].z;

			// 네 점을 화면 좌표로 변환한다. vecTrans는 화면 좌표로 되어 있음.
			int k;
			for (k = 0; k < 4; k++)
			{
				D3DMath_VectorMatrixMultiply(vecTrans[k], vecOrg[k], matPipe);
			}


			// (AB) 와 (AC) 벡터의 외적을 구하여 방향을 세 점의 위치 관계를 판다.
			bCCW[0] = Nk3DMath_CCW(vecTrans[0].x, vecTrans[0].y, vecTrans[1].x, vecTrans[1].y, mx, my);
			bCCW[1] = Nk3DMath_CCW(vecTrans[1].x, vecTrans[1].y, vecTrans[2].x, vecTrans[2].y,  mx,  my);
			bCCW[2] = Nk3DMath_CCW(vecTrans[2].x, vecTrans[2].y, vecTrans[3].x, vecTrans[3].y,  mx,  my);
			bCCW[3] = Nk3DMath_CCW(vecTrans[3].x, vecTrans[3].y, vecTrans[0].x, vecTrans[0].y,  mx,  my);

			if ((bCCW[0] + bCCW[1] + bCCW[2] + bCCW[3]) == 4)
			{
				// 화면 좌표 안으로 들어오는지 체크한다.
				// x좌표만 체크.
				bInsideMap = TRUE;

				for (k=0; k < 4; k++)
				{
					if ( vecTrans[k].x <= -1024.0f || vecTrans[k].x > 2048.0f)
					{
						bInsideMap = FALSE;
					}
				}

				if (bInsideMap == FALSE)
					continue;

				tileIdx = m_pnVisibleIndex[i];						// 타일 인덱스 저장. (return paramer)
				// 사각 타일 안에 존재한다. 왼쪽 삼각형에 있는지 오른쪽 삼각형에 있는지 판단.
				if (Nk3DMath_CCW(vecTrans[0].x, vecTrans[0].y, vecTrans[2].x, vecTrans[2].y,  mx,  my) > 0)
					bLeft = FALSE;
				else
					bLeft = TRUE;

				break;
			}
		}

		float plane[4];				// 평면의 방정식
		if (bInsideMap)
		{
			D3DVECTOR AB, AC, temp;
			// 삼각형의 평면의 방정식을 구한다. (월드공간상의 타일 좌표를 가지고)
			// vecOrg 에는 선택된 타일의 월드좌표가 있다.
			if (bLeft)  			// 정점 0, 1, 2로 평면의 방정식을 구한다.
			{
				AB.x = vecOrg[1].x - vecOrg[0].x;
				AB.y = vecOrg[1].y - vecOrg[0].y;
				AB.z = vecOrg[1].z - vecOrg[0].z;

				AC.x = vecOrg[2].x - vecOrg[0].x;
				AC.y = vecOrg[2].y - vecOrg[0].y;
				AC.z = vecOrg[2].z - vecOrg[0].z;

			}
			else  				// 정점 1, 2, 3으로 평면의 방정식을 구한다.
			{
				AB.x = vecOrg[2].x - vecOrg[0].x;
				AB.y = vecOrg[2].y - vecOrg[0].y;
				AB.z = vecOrg[2].z - vecOrg[0].z;

				AC.x = vecOrg[3].x - vecOrg[0].x;
				AC.y = vecOrg[3].y - vecOrg[0].y;
				AC.z = vecOrg[3].z - vecOrg[0].z;
			}

			// 두 벡터를 이용하여 평면의 방정식 구하기.
			temp = CrossProduct(AB, AC);
			temp = Normalize(temp);
			plane[0] = temp.x;
			plane[1] = temp.y;
			plane[2] = temp.z;
			// vecOrg[0]는 두 삼각형에 공통인 점
			plane[3] = 0.0f - (plane[0]*vecOrg[0].x + plane[1]*vecOrg[0].y + plane[2]*vecOrg[0].z);

			// 평면의 방정식과 ray와의 교차점 구하기 ---
			/*
						Ax + By + Cz + D
				t = -  ------------------
						  Ai + Bj + Ck

				a = ray.direction.x * A + ray.direction.y * B +ray.direction.z * C;
				b = - ( ray.origin.x * A + ray.origin.y * B + ray.origin.z * C + D );
				t = b / a;
			*/
			float a, b, t = 1.0f;
			a = vPickRayDir.x * plane[0] + vPickRayDir.y * plane[1] + vPickRayDir.z*plane[2];
			b = - (vPickRayOrig.x * plane[0] + vPickRayOrig.y * plane[1] + vPickRayOrig.z*plane[2] + plane[3]);

			if (fabs( a ) > g_EPSILON)
				t = b / a;
			else
				t = t;

			/*
				A ray is a parametric form of a 3D line
				vStart + vDir * t
			*/

			D3DVECTOR x;
			reVec.x = vPickRayOrig.x + vPickRayDir.x * t;
			reVec.y = vPickRayOrig.y + vPickRayDir.y * t;
			reVec.z = vPickRayOrig.z + vPickRayDir.z * t;

			return TRUE;
		}

		tileIdx = -1;								// 스크린 좌표 상에 선택되는 타일이 존재하지 않음.
		return FALSE;
	}
}

BOOL CLand::FindNearPoint(long mx, long my, int count)
{
	D3DVECTOR	reVec;
	BOOL	bFound;
	int w, h, idx;

	m_selPtCount = 0;

	bFound = PickPoint(reVec, mx, my);

	if (bFound == FALSE)
		return FALSE;

	reVec.x += (gfMapExtention * 0.5f);
	reVec.z += (gfMapExtention * 0.5f);

	reVec.x /= gfMapExtention;
	reVec.z /= gfMapExtention;

	w = (int) (reVec.x);
	h = (int) (reVec.z);

	idx = h * gHFWidth + w;

	if (idx < 0)
		idx = idx;

	if (count == 1)  						// 선택한 점이 하나..
	{
		m_selPtIdx[m_selPtCount] = idx;
		m_selPtCount++;
	}

	return TRUE;
}

void CLand::DrawSelectedPoints()
{
	D3DLVERTEX lvertex[5];
	WORD	ptidx[8] = {0, 1, 0, 2, 0, 3, 0, 4};
	float   fHeight;

	int w, h, idx;

	lvertex[0] = D3DLVERTEX(D3DVECTOR(0.0f, 0.0f, 0.0f), 0x00ff0000, 0, 0.0f, 0.0f);
	for (int i = 1; i < 5; i++)
		lvertex[i] = lvertex[0];

	// 하나인 경우만 우선 생각을 한다.
	// src에서 dest로 네 번을 그린다.
	if (m_selPtCount == 1)
	{
		idx = m_selPtIdx[0];
		w = idx % gHFWidth;
		h = idx / gHFWidth;

		fHeight = (float) (m_pHeightMap[idx] * gHeightScale);
		lvertex[0].x = w * gfMapExtention;
		lvertex[0].y = fHeight;
		lvertex[0].z = h * gfMapExtention;

		lvertex[1].x = (w-1) * gfMapExtention;
		lvertex[1].y = fHeight;
		lvertex[1].z = h * gfMapExtention;

		lvertex[2].x = (w+1) * gfMapExtention;
		lvertex[2].y = fHeight;
		lvertex[2].z = h * gfMapExtention;

		lvertex[3].x = w * gfMapExtention;
		lvertex[3].y = fHeight;
		lvertex[3].z = (h-1) * gfMapExtention;

		lvertex[4].x = w * gfMapExtention;
		lvertex[4].y = fHeight;
		lvertex[4].z = (h+1) * gfMapExtention;

		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE);
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST, D3DFVF_LVERTEX, lvertex, 5, ptidx, 8, 0);
		m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE);
	}
}

void CLand::AdjustHeight(int oldy, int y)
{
	if (m_selPtCount == 1)
	{
		if (oldy < y && m_pHeightMap[m_selPtIdx[0]] != 0)
			m_pHeightMap[m_selPtIdx[0]] = m_pHeightMap[m_selPtIdx[0]] - 1;
		else if (oldy > y && m_pHeightMap[m_selPtIdx[0]] != 255)
			m_pHeightMap[m_selPtIdx[0]] = m_pHeightMap[m_selPtIdx[0]] + 1;
		else
			return;
	}
}

// 하나의 비트가 1미터 평방내에 갈 수 있는지 없는지를 나타낸다.
BOOL CLand::IsMovableArea(float x, float z)
{
	int nx, nz;
	int idx, offset, pos;
	unsigned char val = 1;

	if (m_pAttrMap == NULL)				// 속성맵이 없으면 움직이지 못하게 한다.
		return FALSE;

	// 영역 체크.
	// 영역을 벗어난 인자값이 들어오면 못 움직이는 영역이다.
	if ( x < 0.0f || z < 0.0f || x >= gfMapRangeWidth || z >= gfMapRangeHeight )
		return FALSE;

	nx = (int) (x / 10.0f);
	nz = (int) (z / 10.0f);

	pos = nz * m_nAttrMapWidth + nx;
	idx = pos / 8;
	offset = pos % 8;

	val = val << offset;

	BOOL bMovable = !(m_pAttrMap[idx] & val);
	if (g_nCurWorldIdx == WORLD_SKY && !IsDrawTile(x, z))
	{
		return (bMovable && (g_RohFlyLevel > 0));
	}

	return bMovable;
}

// 하나의 비트가 1미터 평방내에 갈 수 있는지 없는지를 나타낸다.
BOOL CLand::IsDrawTile(float x, float z)
{
	int nx, nz;
	int pos; //, idx, offset;
	unsigned char val = 1;

	if (m_pTileDrawMap == NULL)				// 그리기 속성 타일 맵이 없으면 그린다.
		return TRUE;

	// 영역 체크.
	// 영역을 벗어난 인자값이 들어오면 못 움직이는 영역이다.
	if ( x < 0.0f || z < 0.0f || x >= gfMapRangeWidth || z >= gfMapRangeHeight )
		return FALSE;

	nx = (int) (x / gfMapExtention);
	nz = (int) (z / gfMapExtention);

	pos = nz * gMapWidth + nx;

	if (m_pTileDrawMap[pos] < 128)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 하나의 비트의 속성을 바꾸어, 갈수 있는 지역을 갈 수 없는 지역으로
// 갈 수 없는 지역을 갈 수 있는 지역으로 바꾼다.
BOOL CLand::SetFieldAttr(float fMinX, float fMinZ, float fMaxX, float fMaxZ, BOOL bPass)
{
	int nx, nz, mx, mz;
	int idx, offset, pos, i, j;
	unsigned char val;

	if (m_pAttrMap == NULL)
		return FALSE;

	// 영역 체크
	if ( fMinX < 0.0f || fMinZ < 0.0f || fMaxX >= gfMapRangeWidth || fMaxZ >= gfMapRangeHeight )
		return FALSE;

	nx = (int) (fMinX / 10.0f);
	nz = (int) (fMinZ / 10.0f);
	mx = (int) (fMaxX / 10.0f);
	mz = (int) (fMaxZ / 10.0f);

	// 루핑을 돌면서 새로운 값을 or한다.
	for (i=nz; i < mz; i++)
	{
		for (j=nx; j < mx; j++)
		{
			pos = i * m_nAttrMapWidth + j;
			idx = pos / 8;
			offset = pos % 8;

			val = 1;
			val = val << offset;

			if (bPass)
			{
				val = ~val;
				m_pAttrMap[idx] = m_pAttrMap[idx] & val;
			}
			else
			{
				m_pAttrMap[idx] = m_pAttrMap[idx] | val;
			}
		}
	}

	return TRUE;
}

void CLand::OnSaveHeightMap()
{
	static char szFilter[] = "Raw File (*.raw)\0*.raw\0\0";
	FILE *fp = NULL;
	OPENFILENAME ofn;
	char strFilePathName[255], strFileName[255];

	strFilePathName[0] = NULL;
	strFileName[0] = NULL;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = strFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = strFileName;;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "raw";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	// 파일 다이얼로그 박스를 호출하여 저장할 이름을 가져온다.
	BOOL bSuccess = GetSaveFileName(&ofn);

	if (m_pHeightMap != NULL && bSuccess)
	{
		fp = fopen(strFilePathName, "wb");
		int count = fwrite(m_pHeightMap, sizeof(unsigned char), gHFWidth*gHFHeight, fp);
		if (count != gHFWidth*gHFHeight)
			MessageBox(NULL, "OnSaveHeightMap - Not saved as map size.", "warning", MB_OK);
		fclose(fp);
	}
}

void CLand::IncreaseDayLight()
{
	m_dwDayLight += 0x00080808;

	while (m_dwDayLight > 0x007F7F7F)
	{
		m_dwDayLight -= 0x00080808;
	}
}

void CLand::DecreaseDayLight()
{
	m_dwDayLight -= 0x00080808;

	while (m_dwDayLight < 0x00080808)
	{
		m_dwDayLight += 0x00080808;
	}
}

void CLand::SetHourLightLand(float fRange, float fFactor)
{
	if (fFactor > 1.0f)
		m_fLandSpotFactor = 1.0f;
	else if (fFactor < 0.0f)
		m_fLandSpotFactor = 0.0f;
	else
		m_fLandSpotFactor = fFactor;

	m_fLandDistRange = fRange;
}

void CLand::SetHourLightDungeon(float fRange, float fFactor)
{
	if (fFactor > 1.0f)
		m_fDunSpotFactor = 1.0f;
	else if (fFactor < 0.0f)
		m_fDunSpotFactor = 0.0f;
	else
		m_fDunSpotFactor = fFactor;

	m_fDunDistRange = fRange;
}

DWORD AddColor(DWORD dwArg1, DWORD dwArg2)
{
	DWORD dwR, dwG, dwB, dwA;
	DWORD dwR2, dwG2, dwB2, dwA2;
	DWORD dwResColor;

	dwA = (dwArg1 & RGB_BITMASK_A) >> 24;
	dwR = (dwArg1 & RGB_BITMASK_R) >> 16;
	dwG = (dwArg1 & RGB_BITMASK_G) >> 8;
	dwB = (dwArg1 & RGB_BITMASK_B);

	dwA2 = (dwArg2 & RGB_BITMASK_A) >> 24;
	dwR2 = (dwArg2 & RGB_BITMASK_R) >> 16;
	dwG2 = (dwArg2 & RGB_BITMASK_G) >> 8;
	dwB2 = (dwArg2 & RGB_BITMASK_B);

	dwA += dwA2;
	dwR += dwR2;
	dwG += dwG2;
	dwB += dwB2;

	dwA = dwA > 255 ? 255 : dwA;
	dwR = dwR > 255 ? 255 : dwR;
	dwG = dwG > 255 ? 255 : dwG;
	dwB = dwB > 255 ? 255 : dwB;

	dwResColor = 0;
	dwResColor |= dwA;
	dwResColor = dwResColor << 8;
	dwResColor |= dwR;
	dwResColor = dwResColor << 8;
	dwResColor |= dwG;
	dwResColor = dwResColor << 8;
	dwResColor |= dwB;

	return dwResColor;
}

void CLand::Destroy()
{
	SAFE_DELETE_ARRAY( m_pDungeonIdx );
	SAFE_DELETE_ARRAY( m_pMapIdx );
	SAFE_DELETE( m_pTileTextures );
	SAFE_DELETE( m_pNk3DMipMap );
	SAFE_DELETE( m_pIndexedTexture );
	SAFE_DELETE_ARRAY( m_pHeightMap );
	SAFE_DELETE_ARRAY( m_pAttrMap );
	SAFE_DELETE_ARRAY( m_pAttrMapDummy );
	SAFE_DELETE_ARRAY( m_pTileDrawMap );
	SAFE_DELETE( m_pWaterObj );
	SAFE_DELETE_ARRAY( m_pdwColor );
	SAFE_DELETE_ARRAY( m_puAlpha );
	SAFE_DELETE( m_pQuadTreeNode );
	SAFE_DELETE( m_pDFloor );
	SAFE_DELETE_ARRAY( m_pnVisibleIndex );
	SAFE_DELETE_ARRAY( m_pfFarWaterFactor );
	SAFE_DELETE( m_pTileTexIdx );

	InvalidateDeviceObjects();

	SAFE_DELETE_ARRAY( m_pAlphaMapL2 );
	SAFE_DELETE_ARRAY( m_pAlphaMapL3 );
	int i = 0;
	for( i = 0; i < 4; i++ )
		SAFE_DELETE( m_aRootNode[i] );
	for( i = 0; i< 6; i++ )
	{
		SAFE_DELETE_ARRAY( m_pLightMapsR[i] );
		SAFE_DELETE_ARRAY( m_pLightMapsG[i] );
		SAFE_DELETE_ARRAY( m_pLightMapsB[i] );
	}
	if( m_ppTerIBL1 )
	{
		for( i = 0; i <= m_nTexCount; i++ )
			SAFE_DELETE_ARRAY( m_ppTerIBL1[i] );
		delete [] m_ppTerIBL1;
		m_ppTerIBL1 = NULL;
	}
	if( m_ppTerIBL2 )
	{
		for( i = 0; i <= m_nTexCount; i++ )
			SAFE_DELETE_ARRAY( m_ppTerIBL2[i] );
		delete [] m_ppTerIBL2;
		m_ppTerIBL2 = NULL;
	}
	if( m_ppTerIBL3 )
	{
		for( i = 0; i <= m_nTexCount; i++ )
			SAFE_DELETE_ARRAY( m_ppTerIBL3[i] );
		delete [] m_ppTerIBL3;
		m_ppTerIBL3 = NULL;
	}
	SAFE_DELETE_ARRAY( m_pTerIBCountL1 );
	SAFE_DELETE_ARRAY( m_pTerIBCountL2 );
	SAFE_DELETE_ARRAY( m_pTerIBCountL3 );
}

BOOL CLand::LoadMap( const TCHAR* szFilePath, const TCHAR* szFileName )
{
	Destroy();
	g_bDungeon = FALSE;
	g_bNewField = FALSE;

	CNKTerQuadNode::ms_pTerrain = this;

	// Load map
	TCHAR	szFilePathName[MAX_PATH];
	_tcscpy( szFilePathName, szFilePath );
	_tcscat( szFilePathName, szFileName );
	_tcscat( szFilePathName, _T(".map") );
	PFILE*	fp = pfopen( szFilePathName, "rb" );
	if( fp == NULL )
		return FALSE;

	// 텍스쳐 확장을 위해서 버전을 끊었다.
	// Magic( MAP + Version ) - Version: 0.1 -> 1, 1.0 -> 10
	DWORD	dwMagic;
	pfread( &dwMagic, sizeof(DWORD), 1, fp );
	if( (dwMagic == ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 10 ) ||
			(dwMagic == ( ( 'M' << 24 ) | ( 'A' << 16 ) | ( 'P' << 8 ) ) + 11 ) )
	{
		//
	}
	else
	{
		pfclose( fp );
		return FALSE;
	}

	// Patch size
	pfread( &gMapExtention, sizeof(int), 1, fp );

	// Height scale
	pfread( &gHeightScale, sizeof(int), 1, fp );

	// Height map
	pfread( &gHFWidth, sizeof(int), 1, fp );
	m_pHeightMap = new unsigned char[gHFWidth * gHFWidth];
	pfread( m_pHeightMap, sizeof(unsigned char), gHFWidth * gHFWidth, fp );
	gHFHeight = gHFWidth;
	gMapWidth = gMapHeight = gHFWidth - 1;

	// Attribute map
	m_nAttrMapWidth = gMapWidth * gMapExtention / 10;
	m_nAttrMapByteWidth = ( m_nAttrMapWidth - 1 ) / 8 + 1;
	m_nAttrMapHeight = m_nAttrMapWidth;
	m_pAttrMap = new unsigned char[m_nAttrMapByteWidth * m_nAttrMapHeight];
	pfread( m_pAttrMap, sizeof(unsigned char), m_nAttrMapByteWidth * m_nAttrMapHeight, fp );

	int	i;
	int	nWidth = gMapWidth * 2;
	if( m_bCanHighQuality )
	{
		// Alpha map
		m_pAlphaMapL2 = new unsigned char[nWidth * nWidth];
		pfread( m_pAlphaMapL2, sizeof(unsigned char), nWidth * nWidth, fp );
		m_pAlphaMapL3 = new unsigned char[nWidth * nWidth];
		pfread( m_pAlphaMapL3, sizeof(unsigned char), nWidth * nWidth, fp );

		// Light map
		for( i = 0; i < 6; i++ )
		{
			m_pLightMapsR[i] = new unsigned char[nWidth * nWidth];
			pfread( m_pLightMapsR[i], sizeof(unsigned char), nWidth * nWidth, fp );
			m_pLightMapsG[i] = new unsigned char[nWidth * nWidth];
			pfread( m_pLightMapsG[i], sizeof(unsigned char), nWidth * nWidth, fp );
			m_pLightMapsB[i] = new unsigned char[nWidth * nWidth];
			pfread( m_pLightMapsB[i], sizeof(unsigned char), nWidth * nWidth, fp );
		}
	}
	else
	{
		// Alpha map
		pfseek( fp, sizeof(unsigned char) * nWidth * nWidth * 2, SEEK_CUR );
		// Light map
		pfseek( fp, sizeof(unsigned char) * nWidth * nWidth * 18, SEEK_CUR );
	}

	// Texture IDs
	unsigned char*	pL1TexID = new unsigned char[gMapWidth * gMapWidth];
	unsigned char*	pL2TexID = new unsigned char[gMapWidth * gMapWidth];
	unsigned char*	pL3TexID = new unsigned char[gMapWidth * gMapWidth];
	pfread( pL1TexID, sizeof(unsigned char), gMapWidth * gMapWidth, fp );
	pfread( pL2TexID, sizeof(unsigned char), gMapWidth * gMapWidth, fp );
	pfread( pL3TexID, sizeof(unsigned char), gMapWidth * gMapWidth, fp );

	// Index buffers
	DWORD	dwIndexCount;
	pfread( &m_nTexCount, sizeof(int), 1, fp );
	m_ppTerIBL1 = new WORD*[m_nTexCount + 1];
	m_ppTerIBL2 = new WORD*[m_nTexCount + 1];
	m_ppTerIBL3 = new WORD*[m_nTexCount + 1];
	m_pTerIBCountL1 = new DWORD[m_nTexCount + 1];
	m_pTerIBCountL2 = new DWORD[m_nTexCount + 1];
	m_pTerIBCountL3 = new DWORD[m_nTexCount + 1];
	for( i = 0; i <= m_nTexCount; i++ )
	{
		pfread( &dwIndexCount, sizeof(DWORD), 1, fp );
		if( dwIndexCount > 0 )
			m_ppTerIBL1[i] = new WORD[dwIndexCount];
		else
			m_ppTerIBL1[i] = NULL;

		pfread( &dwIndexCount, sizeof(DWORD), 1, fp );
		if( dwIndexCount > 0 )
			m_ppTerIBL2[i] = new WORD[dwIndexCount];
		else
			m_ppTerIBL2[i] = NULL;

		pfread( &dwIndexCount, sizeof(DWORD), 1, fp );
		if( dwIndexCount > 0 )
			m_ppTerIBL3[i] = new WORD[dwIndexCount];
		else
			m_ppTerIBL3[i] = NULL;
	}

	// Textures
	TCHAR	szTexName[128];
	int		nLength;
	pfread( &m_nTexCount, sizeof(int), 1, fp );
	m_pIndexedTexture = new IndexedTexture( m_nTexCount );
	m_pIndexedTexture->SetTexturePath( szFilePath );
	for( i = 0; i < m_nTexCount; i++ )
	{
		pfread( &nLength, sizeof(int), 1, fp );
		pfread( szTexName, sizeof(TCHAR), nLength, fp );
		szTexName[nLength] = NULL;
		if( m_pIndexedTexture->CreateTextureFromFile( szTexName, i ) < 0 )
		{
			SAFE_DELETE_ARRAY( pL1TexID );
			SAFE_DELETE_ARRAY( pL2TexID );
			SAFE_DELETE_ARRAY( pL3TexID );
			Destroy();
			pfclose( fp );
			return FALSE;
		}
	}

	// Direcitonal light
	for( i = 0; i < 6; i++ )
		pfread( &m_aDirectLightDir[i], sizeof(D3DVECTOR), 1, fp );

	pfclose( fp );

	// Set variables
	m_MapAttr.nMapWidthNum = gMapWidth;
	m_MapAttr.nMapHeightNum = gMapHeight;
	gfMapExtention = (float)gMapExtention;
	gfMapRangeWidth = gMapWidth * gfMapExtention;
	gfMapRangeHeight = gMapHeight * gfMapExtention;
	g_nMapCellWidth = gMapExtention * 4;
	g_nMapCellHeight = g_nMapCellWidth;
	g_nMapCellWidthCount = gMapWidth / ( g_nMapCellWidth / gMapExtention );
	g_nMapCellHeightCount = gMapHeight / ( g_nMapCellHeight / gMapExtention );

	// Restore textures
	m_pIndexedTexture->RestoreAllTextures( m_pd3dDevice );

	// Vertex buffer width & size
	int	nWHSegment = gMapWidth == 256 ? 128 : gMapWidth;
	m_nVBWidth = nWHSegment * 3 / 2;
	m_dwVBSize = m_nVBWidth * m_nVBWidth;

	// Get a quad tree depth
	m_nQuadTreeDepth = 0;
	while( nWHSegment >>= 1 )
		m_nQuadTreeDepth++;

	// Initialize quad tree
	if( !InitQuadTree( pL1TexID, pL2TexID, pL3TexID ) )
	{
		Destroy();
		return FALSE;
	}
	SAFE_DELETE_ARRAY( pL1TexID );
	SAFE_DELETE_ARRAY( pL2TexID );
	SAFE_DELETE_ARRAY( pL3TexID );

	// Initialize vertex buffer
	if( !InitVertexBuffer() )
	{
		Destroy();
		return FALSE;
	}

	// Create color map textures
	if( m_bCanHighQuality )
	{
		if( FAILED( CreateClrMapTexture() ) )
		{
			Destroy();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CLand::InitQuadTree( unsigned char* pL1TexID, unsigned char* pL2TexID, unsigned char* pL3TexID )
{
	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;

	// Lower left vertex buffer
	m_aRootNode[0] = new CNKTerQuadNode( m_nQuadTreeDepth, 0, 0, nRightX, nRightZ,
										 pL1TexID, pL2TexID, pL3TexID );

	nCenterX = nRightX;
	nCenterZ = nRightZ;
	nRightX <<= 1;
	nRightZ <<= 1;

	if( gMapWidth == 256 )
	{
		// Lower right vertex buffer
		m_aRootNode[1] = new CNKTerQuadNode( m_nQuadTreeDepth, nCenterX, 0, nRightX, nCenterZ,
											 pL1TexID, pL2TexID, pL3TexID );

		// Upper left vertex buffer
		m_aRootNode[2] = new CNKTerQuadNode( m_nQuadTreeDepth, 0, nCenterZ, nCenterX, nRightZ,
											 pL1TexID, pL2TexID, pL3TexID );

		// Upper right vertex buffer
		m_aRootNode[3] = new CNKTerQuadNode( m_nQuadTreeDepth, nCenterX, nCenterZ, nRightX, nRightZ,
											 pL1TexID, pL2TexID, pL3TexID );
	}

	return TRUE;
}

BOOL CLand::InitVertexBuffer()
{
	if( m_pHeightMap == NULL )
		return TRUE;

	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;

	D3DVERTEXBUFFERDESC	VBDesc;
	VBDesc.dwSize = sizeof( VBDesc );
	VBDesc.dwCaps = D3DVBCAPS_WRITEONLY;
	VBDesc.dwFVF = NKVERTEXT2_FVF;
	VBDesc.dwNumVertices = m_nVBWidth * m_nVBWidth;

	NKVERTEXT2*	pVB = new NKVERTEXT2[m_nVBWidth * m_nVBWidth];

	CNKTerQuadNode::ms_fdt = 1.0f / (float)(nWidth * 2);

	if( !m_bCanHighQuality )
		return InitVertexBufferInSysMem( pVB );

	// Lower left vertex buffer
	CNKTerQuadNode::ms_nVBIdxCnt = 0;
	m_aRootNode[0]->InitVertexBuffer( 0, 0, nRightX, nRightZ, 0, 0, pVB );
	if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[0], 0 ) ) )
	{
		return InitVertexBufferInSysMem( pVB );
	}

	NKVERTEXT2*	pVert;
	if( FAILED( m_aVB[0]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
								(LPVOID*)(&pVert), NULL ) ) )
	{
		SAFE_RELEASE( m_aVB[0] );
		return InitVertexBufferInSysMem( pVB );
	}
	memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
	m_aVB[0]->Unlock();

	nCenterX = nRightX;
	nCenterZ = nRightZ;
	nRightX <<= 1;
	nRightZ <<= 1;

	if( gMapWidth == 256 )
	{
		// Lower right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[1]->InitVertexBuffer( nCenterX, 0, nRightX, nCenterZ, -1, 0, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[1], 0 ) ) )
		{
			SAFE_RELEASE( m_aVB[0] );
			return InitVertexBufferInSysMem( pVB );
		}

		if( FAILED( m_aVB[1]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			return InitVertexBufferInSysMem( pVB );
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[1]->Unlock();

		// Upper left vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[2]->InitVertexBuffer( 0, nCenterZ, nCenterX, nRightZ, 0, -1, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[2], 0 ) ) )
		{
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			return InitVertexBufferInSysMem( pVB );
		}

		if( FAILED( m_aVB[2]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			SAFE_RELEASE( m_aVB[2] );
			return InitVertexBufferInSysMem( pVB );
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[2]->Unlock();

		// Upper right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[3]->InitVertexBuffer( nCenterX, nCenterZ, nRightX, nRightZ, -1, -1, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[3], 0 ) ) )
		{
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			SAFE_RELEASE( m_aVB[2] );
			return InitVertexBufferInSysMem( pVB );
		}

		if( FAILED( m_aVB[3]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			SAFE_RELEASE( m_aVB[2] );
			SAFE_RELEASE( m_aVB[3] );
			return InitVertexBufferInSysMem( pVB );
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[3]->Unlock();
	}

	SAFE_DELETE_ARRAY( pVB );

	return TRUE;
}

BOOL CLand::InitVertexBufferInSysMem( NKVERTEXT2* pVB )
{
	int nWidth = 1 << m_nQuadTreeDepth;
	int nRightX = nWidth;
	int nRightZ = nRightX;
	int nCenterX, nCenterZ;

	D3DVERTEXBUFFERDESC	VBDesc;
	VBDesc.dwSize = sizeof( VBDesc );
	VBDesc.dwCaps = D3DVBCAPS_SYSTEMMEMORY;
	VBDesc.dwFVF = NKVERTEXT2_FVF;
	VBDesc.dwNumVertices = m_nVBWidth * m_nVBWidth;

	// Lower left vertex buffer
	CNKTerQuadNode::ms_nVBIdxCnt = 0;
	m_aRootNode[0]->InitVertexBuffer( 0, 0, nRightX, nRightZ, 0, 0, pVB );
	if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[0], 0 ) ) )
	{
		SAFE_DELETE_ARRAY( pVB );
		return FALSE;
	}

	NKVERTEXT2*	pVert;
	if( FAILED( m_aVB[0]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
								(LPVOID*)(&pVert), NULL ) ) )
	{
		SAFE_DELETE_ARRAY( pVB );
		SAFE_RELEASE( m_aVB[0] );
		return FALSE;
	}
	memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
	m_aVB[0]->Unlock();

	nCenterX = nRightX;
	nCenterZ = nRightZ;
	nRightX <<= 1;
	nRightZ <<= 1;

	if( gMapWidth == 256 )
	{
		// Lower right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[1]->InitVertexBuffer( nCenterX, 0, nRightX, nCenterZ, -1, 0, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[1], 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			SAFE_RELEASE( m_aVB[0] );
			return FALSE;
		}

		if( FAILED( m_aVB[1]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			return FALSE;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[1]->Unlock();

		// Upper left vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[2]->InitVertexBuffer( 0, nCenterZ, nCenterX, nRightZ, 0, -1, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[2], 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			return FALSE;
		}

		if( FAILED( m_aVB[2]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			SAFE_RELEASE( m_aVB[2] );
			return FALSE;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[2]->Unlock();

		// Upper right vertex buffer
		CNKTerQuadNode::ms_nVBIdxCnt = 0;
		m_aRootNode[3]->InitVertexBuffer( nCenterX, nCenterZ, nRightX, nRightZ, -1, -1, pVB );
		if( FAILED( m_pD3D->CreateVertexBuffer( &VBDesc, &m_aVB[3], 0 ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			SAFE_RELEASE( m_aVB[2] );
			return FALSE;
		}

		if( FAILED( m_aVB[3]->Lock( DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS,
									(LPVOID*)(&pVert), NULL ) ) )
		{
			SAFE_DELETE_ARRAY( pVB );
			SAFE_RELEASE( m_aVB[0] );
			SAFE_RELEASE( m_aVB[1] );
			SAFE_RELEASE( m_aVB[2] );
			SAFE_RELEASE( m_aVB[3] );
			return FALSE;
		}
		memcpy( pVert, pVB, m_nVBWidth * m_nVBWidth * sizeof(NKVERTEXT2) );
		m_aVB[3]->Unlock();
	}

	SAFE_DELETE_ARRAY( pVB );

	return TRUE;
}

static HRESULT CALLBACK TextureSearch( DDPIXELFORMAT* pddpf, VOID* param )
{
	DDSURFACEDESC2* pddsd = (DDSURFACEDESC2*)param;

	// Skip unwanted formats
	if( pddpf->dwRGBBitCount != pddsd->dwFlags )
		return DDENUMRET_OK;
	if( pddpf->dwFlags & ( DDPF_LUMINANCE | DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)  )
		return DDENUMRET_OK;
	if( !( pddpf->dwFlags & DDPF_ALPHAPIXELS ) )
		return DDENUMRET_OK;
	if( 0 != pddpf->dwFourCC )
		return DDENUMRET_OK;

	if( pddsd->dwFlags == 32 )
	{
		pddpf->dwRGBAlphaBitMask = 0xff000000;
		pddpf->dwRBitMask = 0x00ff0000;
		pddpf->dwGBitMask = 0x0000ff00;
		pddpf->dwBBitMask = 0x000000ff;
	}
	else
	{
		pddpf->dwRGBAlphaBitMask = 0x0000f000;
		pddpf->dwRBitMask = 0x00000f00;
		pddpf->dwGBitMask = 0x000000f0;
		pddpf->dwBBitMask = 0x0000000f;
	}

	memcpy( &pddsd->ddpfPixelFormat, pddpf, sizeof(DDPIXELFORMAT) );
	return DDENUMRET_CANCEL;
}

HRESULT	CLand::CreateClrMapTexture()
{
	if( m_pAlphaMapL2 == NULL )
		return S_OK;

	// Release any previously created objects
	int i = 0;
	for( i = 0; i < 4; i++ )
	{
		SAFE_RELEASE( m_aClrMapTexL2[i] );
		SAFE_RELEASE( m_aClrMapTexL3[i] );
	}

	// Get the device caps
	D3DDEVICEDESC7 ddDesc;
	if( FAILED( m_pd3dDevice->GetCaps( &ddDesc) ) )
		return E_FAIL;

	// Setup the new surface desc
	int nWidth = 1 << ( m_nQuadTreeDepth + 1 );
	DDSURFACEDESC2 ddsd;
	D3DUtil_InitSurfaceDesc( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.dwTextureStage = 1;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nWidth;

	// Turn on texture management for hardware devices
	if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	// Search a texture format
	DDSURFACEDESC2 ddsdSearch;
	ddsdSearch.dwFlags = 32;
	m_pd3dDevice->EnumTextureFormats( TextureSearch, &ddsdSearch );

	if( 32 != ddsdSearch.ddpfPixelFormat.dwRGBBitCount )
	{
		ddsdSearch.dwFlags = 16;
		m_pd3dDevice->EnumTextureFormats( TextureSearch, &ddsdSearch );

		if( 16 != ddsdSearch.ddpfPixelFormat.dwRGBBitCount )
			return E_FAIL;
	}

	// Use a searched texture format
	memcpy( &ddsd.ddpfPixelFormat, &ddsdSearch.ddpfPixelFormat, sizeof(DDPIXELFORMAT) );

	// Get the DirectDraw interface for creating surfaces
	LPDIRECTDRAW7        pDD;
	LPDIRECTDRAWSURFACE7 pddsRender;
	m_pd3dDevice->GetRenderTarget( &pddsRender );
	pddsRender->GetDDInterface( (VOID**)&pDD );
	pddsRender->Release();

	HRESULT hr;
	// Create a new surface for the texture
	if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL2[0], NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}
	if( FAILED( UpdateClrMapTexture( 2, 0 ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL3[0], NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}
	if( FAILED( UpdateClrMapTexture( 3, 0 ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	if( gMapWidth == 256 )
	{
		for( i = 1; i < 4; i++ )
		{
			if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL2[i], NULL ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}
			if( FAILED( UpdateClrMapTexture( 2, i ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}

			if( FAILED( hr = pDD->CreateSurface( &ddsd, &m_aClrMapTexL3[i], NULL ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}
			if( FAILED( UpdateClrMapTexture( 3, i ) ) )
			{
				pDD->Release();
				return E_FAIL;
			}
		}
	}

	// Done with DDraw
	pDD->Release();

	return S_OK;
}

HRESULT CLand::UpdateClrMapTexture( int nLayer, int nIndex )
{
	int	nWidth = 1 << ( m_nQuadTreeDepth + 1 );
	int	nSX = ( nIndex % 2 ) * nWidth;
	int	nSZ = ( nIndex / 2 ) * nWidth;
	int	nCount = 0;
	int	nX, nZ;

	// Get a DDraw object to create a temporary surface
	LPDIRECTDRAW7	pDD;
	if( nLayer == 2 )
		m_aClrMapTexL2[nIndex]->GetDDInterface( (VOID**)&pDD );
	else
		m_aClrMapTexL3[nIndex]->GetDDInterface( (VOID**)&pDD );

	// Setup the new surface desc
	DDSURFACEDESC2	ddsd;
	ddsd.dwSize = sizeof(ddsd);
	if( nLayer == 2 )
		m_aClrMapTexL2[nIndex]->GetSurfaceDesc( &ddsd );
	else
		m_aClrMapTexL3[nIndex]->GetSurfaceDesc( &ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	ddsd.ddsCaps.dwCaps2 = 0L;
	ddsd.dwTextureStage = 1;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nWidth;

	// Create a new surface for the texture
	LPDIRECTDRAWSURFACE7	pddsTempSurface;
	HRESULT hr;
	if( FAILED( hr = pDD->CreateSurface( &ddsd, &pddsTempSurface, NULL ) ) )
	{
		pDD->Release();
		return E_FAIL;
	}

	while( pddsTempSurface->Lock( NULL, &ddsd, 0, 0 ) == DDERR_WASSTILLDRAWING );
	BYTE*	pBits = (BYTE*)ddsd.lpSurface;
	if( nLayer == 2 )
	{
		int				nClrMapWidth = gMapWidth * 2;
		unsigned char*	pTempA;
		unsigned char*	pTempR;
		unsigned char*	pTempG;
		unsigned char*	pTempB;

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
		{
			DWORD*	pTempBits;
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (DWORD*)pBits;
				pTempA = &m_pAlphaMapL2[nSX + nZ * nClrMapWidth];
				pTempR = &m_pLightMapsR[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempG = &m_pLightMapsG[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempB = &m_pLightMapsB[m_nTimeID][nSX + nZ * nClrMapWidth];
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( *pTempA << 24 ) | ( *pTempR << 16 ) | ( *pTempG << 8 ) | *pTempB;
					pTempA++;
					pTempR++;
					pTempG++;
					pTempB++;
					pTempBits++;
				}
				pBits += ddsd.lPitch;
			}
		}
		else
		{
			WORD*	pTempBits;
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (WORD*)pBits;
				pTempA = &m_pAlphaMapL2[nSX + nZ * nClrMapWidth];
				pTempR = &m_pLightMapsR[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempG = &m_pLightMapsG[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempB = &m_pLightMapsB[m_nTimeID][nSX + nZ * nClrMapWidth];
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( ( *pTempA >> 4 ) << 12 ) | ( ( *pTempR >> 4 ) << 8 ) |
								 ( ( *pTempG >> 4 ) << 4 ) | ( *pTempB >> 4 );
					pTempA++;
					pTempR++;
					pTempG++;
					pTempB++;
					pTempBits++;
				}
				pBits += ddsd.lPitch;
			}
		}
	}
	else
	{
		int				nClrMapWidth = gMapWidth * 2;
		unsigned char*	pTempA;
		unsigned char*	pTempR;
		unsigned char*	pTempG;
		unsigned char*	pTempB;

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
		{
			DWORD*	pTempBits;
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (DWORD*)pBits;
				pTempA = &m_pAlphaMapL3[nSX + nZ * nClrMapWidth];
				pTempR = &m_pLightMapsR[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempG = &m_pLightMapsG[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempB = &m_pLightMapsB[m_nTimeID][nSX + nZ * nClrMapWidth];
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( *pTempA << 24 ) | ( *pTempR << 16 ) | ( *pTempG << 8 ) | *pTempB;
					pTempA++;
					pTempR++;
					pTempG++;
					pTempB++;
					pTempBits++;
				}
				pBits += ddsd.lPitch;
			}
		}
		else
		{
			WORD*	pTempBits;
			for( nZ = nSZ; nZ < nSZ + nWidth; nZ++ )
			{
				pTempBits = (WORD*)pBits;
				pTempA = &m_pAlphaMapL3[nSX + nZ * nClrMapWidth];
				pTempR = &m_pLightMapsR[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempG = &m_pLightMapsG[m_nTimeID][nSX + nZ * nClrMapWidth];
				pTempB = &m_pLightMapsB[m_nTimeID][nSX + nZ * nClrMapWidth];
				for( nX = nSX; nX < nSX + nWidth; nX++ )
				{
					*pTempBits = ( ( *pTempA >> 4 ) << 12 ) | ( ( *pTempR >> 4 ) << 8 ) |
								 ( ( *pTempG >> 4 ) << 4 ) | ( *pTempB >> 4 );
					pTempA++;
					pTempR++;
					pTempG++;
					pTempB++;
					pTempBits++;
				}
				pBits += ddsd.lPitch;
			}
		}
	}
	pddsTempSurface->Unlock( 0 );

	// Copy the temp surface to the real texture surface
	if( nLayer == 2 )
		m_aClrMapTexL2[nIndex]->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );
	else
		m_aClrMapTexL3[nIndex]->Blt( NULL, pddsTempSurface, NULL, DDBLT_WAIT, NULL );

	// Done with the temp objects
	pddsTempSurface->Release();
	pDD->Release();

	return S_OK;
}

HRESULT CLand::InvalidateDeviceObjects()
{
	// Release terrain vertex buffer & color map textures
	for( int i = 0; i < 4; i++ )
	{
		SAFE_RELEASE( m_aVB[i] );
		SAFE_RELEASE( m_aClrMapTexL2[i] );
		SAFE_RELEASE( m_aClrMapTexL3[i] );
	}

	// Release terrain textures
	InvalidateAllTextures();

	return S_OK;
}

HRESULT CLand::RestoreDeviceObjects( LPDIRECT3D7 pD3D, LPDIRECT3DDEVICE7 pd3dDevice )
{
	m_pD3D = pD3D;
	m_pd3dDevice = pd3dDevice;

	if( g_bNewTerrain )
	{
		// Restore terrain vertex buffer
		if( !InitVertexBuffer() )
			return E_FAIL;

		// Create color map texture
		if( m_bCanHighQuality )
		{
			if( FAILED( CreateClrMapTexture() ) )
				return E_FAIL;
		}
	}

	// Restore terrain textures
	RestoreAllTextures();

	return S_OK;
}

void CLand::SetWorldTime( int nWorldTime )
{
	int		nTimeID;
	bool	bUpdateLightMap = false;

	if( nWorldTime >= 12 && nWorldTime <= 16 )
		nTimeID = 0;
	else if( nWorldTime >=17 && nWorldTime <= 21 )
		nTimeID = 1;
	else if( nWorldTime >= 22 && nWorldTime <= 27 )
		nTimeID = 2;
	else if( nWorldTime >=2 && nWorldTime <= 6 )
		nTimeID = 4;
	else if( nWorldTime >= 7 && nWorldTime <= 11 )
		nTimeID = 5;
	else nTimeID = 3;

	if( nTimeID != m_nTimeID )
		bUpdateLightMap = true;

	m_nTimeID = nTimeID;

	if( m_bCanHighQuality && bUpdateLightMap && m_aClrMapTexL2[0] )
	{
		UpdateClrMapTexture( 2, 0 );
		UpdateClrMapTexture( 3, 0 );

		if( gMapWidth == 256 )
		{
			UpdateClrMapTexture( 2, 1 );
			UpdateClrMapTexture( 3, 1 );
			UpdateClrMapTexture( 2, 2 );
			UpdateClrMapTexture( 3, 2 );
			UpdateClrMapTexture( 2, 3 );
			UpdateClrMapTexture( 3, 3 );
		}
	}
}

void CLand::RenderTerrain( CFrustum* pFrustum )
{
	if( !m_bCanHighQuality )
	{
		RenderTerrainByLow( pFrustum );
		return ;
	}

	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	if( g_bMipmap )
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );

	// Get texture stage state
	DWORD	dwTexCoord0, dwTexCoord1;
	DWORD	dwColorOp0, dwColorArg01, dwColorArg02, dwAlphaOp0, dwAlphaArg01;
	DWORD	dwColorOp1, dwColorArg11, dwColorArg12, dwAlphaOp1, dwAlphaArg11, dwAddress1;
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, &dwTexCoord0 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, &dwTexCoord1 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg01 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG2, &dwColorArg02 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP, &dwColorOp1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLORARG1, &dwColorArg11 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLORARG2, &dwColorArg12 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP, &dwAlphaOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &dwAlphaArg01 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_ALPHAOP, &dwAlphaOp1 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_ALPHAARG1, &dwAlphaArg11 );
	m_pd3dDevice->GetTextureStageState( 1, D3DTSS_ADDRESS, &dwAddress1 );

	// Get render state ( alpha blending )
	DWORD	dwSrcBlend, dwDestBlend;
	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_SRCBLEND, &dwSrcBlend );
	m_pd3dDevice->GetRenderState( D3DRENDERSTATE_DESTBLEND, &dwDestBlend );

	// Set texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP );

	// Set render state ( alpha blending )
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// Lower left
	// Clear index buffer count
	int i = 0;
	for( i = 0; i <= m_nTexCount; i++ )
	{
		m_pTerIBCountL1[i] = 0;
		m_pTerIBCountL2[i] = 0;
		m_pTerIBCountL3[i] = 0;
	}

	// Collect triangles rendered
	m_aRootNode[0]->Render( pFrustum, VFCULL_OVERLAP );

	// Set texture ( stage 1 )
	m_pd3dDevice->SetTexture( 1, m_aClrMapTexL2[0] );

	// Set texture stage state for non-blending
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	// Set render state ( diable alpha blending )
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	// Render triangles
	for( i = 0; i < m_nTexCount; i++ )
	{
		if( m_pTerIBCountL1[i] > 0 )
		{
			m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
			m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
												  m_ppTerIBL1[i], m_pTerIBCountL1[i], 0 );
		}
	}

	// Set texture stage state for blending
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	// Set render state ( enable alpha blending )
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

	// Render triangles
	for( i = 0; i < m_nTexCount; i++ )
	{
		if( m_pTerIBCountL2[i] > 0 )
		{
			m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
			m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
												  m_ppTerIBL2[i], m_pTerIBCountL2[i], 0 );
		}
	}

	// Set texture ( stage 1 )
	m_pd3dDevice->SetTexture( 1, m_aClrMapTexL3[0] );

	// Render triangles
	for( i = 0; i < m_nTexCount; i++ )
	{
		if( m_pTerIBCountL3[i] > 0 )
		{
			m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
			m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
												  m_ppTerIBL3[i], m_pTerIBCountL3[i], 0 );
		}
	}

	// Lower right, upper left, upper right
	if( gMapWidth == 256 )
	{
		for( int nIndex = 1; nIndex < 4; nIndex++ )
		{
			// Clear index buffer count
			for( i = 0; i <= m_nTexCount; i++ )
			{
				m_pTerIBCountL1[i] = 0;
				m_pTerIBCountL2[i] = 0;
				m_pTerIBCountL3[i] = 0;
			}

			// Collect triangles rendered
			m_aRootNode[nIndex]->Render( pFrustum, VFCULL_OVERLAP );

			// Set texture ( stage 1 )
			m_pd3dDevice->SetTexture( 1, m_aClrMapTexL2[nIndex] );

			// Set texture stage state for non-blending
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

			// Set render state ( disable alpha blending )
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

			// Render triangles
			for( i = 0; i < m_nTexCount; i++ )
			{
				if( m_pTerIBCountL1[i] > 0 )
				{
					m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
					m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
														  m_ppTerIBL1[i], m_pTerIBCountL1[i], 0 );
				}
			}

			// Set texture stage state for blending
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

			// Set render state ( enable alpha blending )
			m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );

			// Render triangles
			for( i = 0; i < m_nTexCount; i++ )
			{
				if( m_pTerIBCountL2[i] > 0 )
				{
					m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
					m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
														  m_ppTerIBL2[i], m_pTerIBCountL2[i], 0 );
				}
			}

			// Set texture ( stage 1 )
			m_pd3dDevice->SetTexture( 1, m_aClrMapTexL3[nIndex] );

			// Render triangles
			for( i = 0; i < m_nTexCount; i++ )
			{
				if( m_pTerIBCountL3[i] > 0 )
				{
					m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
					m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
														  m_ppTerIBL3[i], m_pTerIBCountL3[i], 0 );
				}
			}
		}
	}

	// Restore render state
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, dwSrcBlend );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, dwDestBlend );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	// Restore texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, dwTexCoord0 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, dwTexCoord1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg01 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, dwColorArg02 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwColorOp1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, dwColorArg11 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, dwColorArg12 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, dwAlphaOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, dwAlphaArg01 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, dwAlphaOp1 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, dwAlphaArg11 );
	m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESS, dwAddress1 );

	if( g_bMipmap )
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_NONE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING , TRUE );
}

void CLand::RenderTerrainByLow( CFrustum* pFrustum )
{
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
	if( g_bMipmap )
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_LINEAR );

	// Get texture stage state
	DWORD	dwColorOp0, dwColorArg01;
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP, &dwColorOp0 );
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1, &dwColorArg01 );

	// Set texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

	// Lower left
	// Clear index buffer count
	int i = 0;
	for( i = 0; i <= m_nTexCount; i++ )
	{
		m_pTerIBCountL1[i] = 0;
		m_pTerIBCountL2[i] = 0;
		m_pTerIBCountL3[i] = 0;
	}

	// Collect triangles rendered
	m_aRootNode[0]->Render( pFrustum, VFCULL_OVERLAP );

	// Render triangles
	for( i = 0; i < m_nTexCount; i++ )
	{
		if( m_pTerIBCountL1[i] > 0 )
		{
			m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
			m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[0], 0, m_dwVBSize,
												  m_ppTerIBL1[i], m_pTerIBCountL1[i], 0 );
		}
	}

	// Lower right, upper left, upper right
	if( gMapWidth == 256 )
	{
		for( int nIndex = 1; nIndex < 4; nIndex++ )
		{
			// Clear index buffer count
			for( i = 0; i <= m_nTexCount; i++ )
			{
				m_pTerIBCountL1[i] = 0;
				m_pTerIBCountL2[i] = 0;
				m_pTerIBCountL3[i] = 0;
			}

			// Collect triangles rendered
			m_aRootNode[nIndex]->Render( pFrustum, VFCULL_OVERLAP );

			// Render triangles
			for( i = 0; i < m_nTexCount; i++ )
			{
				if( m_pTerIBCountL1[i] > 0 )
				{
					m_pd3dDevice->SetTexture( 0, m_pIndexedTexture->GetSurfaceByIdx( i ) );
					m_pd3dDevice->DrawIndexedPrimitiveVB( D3DPT_TRIANGLELIST, m_aVB[nIndex], 0, m_dwVBSize,
														  m_ppTerIBL1[i], m_pTerIBCountL1[i], 0 );
				}
			}
		}
	}

	// Restore texture stage state
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, dwColorOp0 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, dwColorArg01 );

	if( g_bMipmap )
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTFP_NONE );
	m_pd3dDevice->SetRenderState( D3DRENDERSTATE_LIGHTING , TRUE );
}

HRESULT CLand::InvalidateVB()
{
	// Release terrain vertex buffer
	for( int i = 0; i < 4; i++ )
		SAFE_RELEASE( m_aVB[i] );

	return S_OK;
}