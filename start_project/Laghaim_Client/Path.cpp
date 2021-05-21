#include "stdafx.h"

#include "Path.h"
#include "Land.h"
#include "NkCharacter.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define PATH_WIDTH 256
#define PATH_HEIGHT 256
#define PATH_WIDTH_DIV 5
#define PATH_HEIGHT_DIV 5
#define PATHMAP_SIZE PATH_WIDTH*PATH_HEIGHT*PATH_WIDTH_DIV*PATH_HEIGHT_DIV


extern BOOL g_bAdmin2;
extern BOOL g_bGhost;

CPath::CPath()
	: m_pLand(NULL)
	, m_tWidth(0)
	, m_tHeight(0)
{
	m_PathMap = new char[PATH_WIDTH*PATH_HEIGHT*PATH_WIDTH_DIV*PATH_HEIGHT_DIV];
	ZeroMemory(m_PathMap, PATH_WIDTH*PATH_HEIGHT*PATH_WIDTH_DIV*PATH_HEIGHT_DIV);
}

CPath::~CPath()
{
	SAFE_DELETE_ARRAY(m_PathMap);
}

void CPath::Init(CLand *pLand)
{
	m_pLand = pLand;
	m_tWidth = PATH_WIDTH * PATH_WIDTH_DIV;
	m_tHeight = PATH_HEIGHT * PATH_HEIGHT_DIV;
}

void CPath::ClearAll()
{
	if (m_PathMap)
		ZeroMemory(m_PathMap, PATH_WIDTH*PATH_HEIGHT*PATH_WIDTH_DIV*PATH_HEIGHT_DIV);
}

BOOL CPath::IsMoveArea(float x, float z)
{
	static BOOL bLandMovable, bChaMovable;

	if (m_pLand)
		bLandMovable = m_pLand->IsMovableArea(x, z);

	int nx = x / 10;
	int nz = z / 10;

	int pathMapIndex = m_tWidth * nz + nx;
	if( pathMapIndex < 0 || pathMapIndex >= PATHMAP_SIZE )
		bChaMovable = FALSE;
	else if (m_PathMap[pathMapIndex] == 0 || (pCMyApp->m_bFastMove && g_bAdmin2))
		bChaMovable = TRUE;
	else
		bChaMovable = FALSE;

	if (pCMyApp->IsPeaceArea(NULL, x, z)) // 평화지역에서는 캐릭터한테 안막힌다.
		bChaMovable = TRUE;

	return (bLandMovable & bChaMovable);
}

void CPath::SetMapMovable(float x, float z, BOOL bMove)
{
	int nx = x / 10;
	int nz = z / 10;

	int pathMapIndex = m_tWidth * nz + nx;
	if( pathMapIndex < 0 || pathMapIndex >= PATHMAP_SIZE )
		return;

	if (bMove)
	{
		m_PathMap[pathMapIndex] = 0;
	}
	else
	{
		m_PathMap[pathMapIndex] = 1;
	}
}
