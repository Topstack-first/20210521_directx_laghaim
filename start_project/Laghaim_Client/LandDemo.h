#ifndef _LAND_DEMO_H_
#define _LAND_DEMO_H_

#include "Config.h"

#include "Land.h"

#define LANDDEMO_RENDER_RANGE 29

class TileTextures;

class CLandDemo
{
public:
	CLandDemo();
	virtual ~CLandDemo();

protected:
	float		m_fCenterX, m_fCenterZ;

public:
	//static CLand		*m_pLand;							// 전역 정보를 유지하기 위해서.
	static D3DLVERTEX	BaseTri[4];


	unsigned char	*m_pHeightMap;
	WORD			*m_pMapIdx;					// 맵의 텍스처 인덱스
	TileTextures	*m_pTileTextures;
	DWORD			*m_pdwColor;				// LVERTEX에 들어가는 칼라값들.

	int				m_nAttrMapWidth, m_nAttrMapByteWidth, m_nAttrMapHeight;

	// 타일 텍스처에 대한 인덱스
	MapAttr			m_MapAttr;

	void		SetDevice() {};
	BOOL		LandDemoInit(char *strFileName);
	void		LoadNewMap( const TCHAR* szFilePath, const TCHAR* szFileName ); // 신맵툴 데이터를 읽어와서 CLandDemo 데이터를 만든다. ///2007로딩변경
	void		Init(float x, float z);
	void		Render();
	void        Render2(); // 신맵으로 부터 업어온 데이터를 렌더링한다.(기본적으론 텍스쳐 방식만 틀리다.) ///2007로딩변경
	DWORD		CalColorFactor(DWORD dwColor, float fColorFactor);

	// 신맵을 쓰는 로그인존은 아래 요소들을 직접 가지고 있게 변경. (구맵은 그대로 전역변수를 쓴다.)///2007로딩변경
	int		m_LdMapWidth;					// 맵의 x축 넓이
	int		m_LdMapHeight;
	int		m_LdHFWidth;
	int		m_LdHFHeight;
	int		m_LdMapExtention;					// 스케일 팩터 - 정수형
	float	m_fLdMapExtention;				// 스케일 팩터 - 실수형
	int		m_LdMapYExtention;
	float	m_fLdMapYExtention;
	int		m_LdHeightScale;

	int m_nLdMapCellWidth;						// 한 셀의 크기.
	int m_nLdMapCellHeight;
	int m_nLdMapCellWidthCount;
	int m_nLdMapCellHeightCount;

	unsigned char*	m_pL1TexID;
	unsigned char*	m_pL2TexID;
	unsigned char*	m_pL3TexID;
	IndexedTexture  *m_pIndexedTexture;         // 신맵용 텍스쳐 셋 ///2007로딩변경

};

#endif // _LAND_DEMO_H_
