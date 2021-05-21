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
	//static CLand		*m_pLand;							// ���� ������ �����ϱ� ���ؼ�.
	static D3DLVERTEX	BaseTri[4];


	unsigned char	*m_pHeightMap;
	WORD			*m_pMapIdx;					// ���� �ؽ�ó �ε���
	TileTextures	*m_pTileTextures;
	DWORD			*m_pdwColor;				// LVERTEX�� ���� Į�󰪵�.

	int				m_nAttrMapWidth, m_nAttrMapByteWidth, m_nAttrMapHeight;

	// Ÿ�� �ؽ�ó�� ���� �ε���
	MapAttr			m_MapAttr;

	void		SetDevice() {};
	BOOL		LandDemoInit(char *strFileName);
	void		LoadNewMap( const TCHAR* szFilePath, const TCHAR* szFileName ); // �Ÿ��� �����͸� �о�ͼ� CLandDemo �����͸� �����. ///2007�ε�����
	void		Init(float x, float z);
	void		Render();
	void        Render2(); // �Ÿ����� ���� ����� �����͸� �������Ѵ�.(�⺻������ �ؽ��� ��ĸ� Ʋ����.) ///2007�ε�����
	DWORD		CalColorFactor(DWORD dwColor, float fColorFactor);

	// �Ÿ��� ���� �α������� �Ʒ� ��ҵ��� ���� ������ �ְ� ����. (������ �״�� ���������� ����.)///2007�ε�����
	int		m_LdMapWidth;					// ���� x�� ����
	int		m_LdMapHeight;
	int		m_LdHFWidth;
	int		m_LdHFHeight;
	int		m_LdMapExtention;					// ������ ���� - ������
	float	m_fLdMapExtention;				// ������ ���� - �Ǽ���
	int		m_LdMapYExtention;
	float	m_fLdMapYExtention;
	int		m_LdHeightScale;

	int m_nLdMapCellWidth;						// �� ���� ũ��.
	int m_nLdMapCellHeight;
	int m_nLdMapCellWidthCount;
	int m_nLdMapCellHeightCount;

	unsigned char*	m_pL1TexID;
	unsigned char*	m_pL2TexID;
	unsigned char*	m_pL3TexID;
	IndexedTexture  *m_pIndexedTexture;         // �Ÿʿ� �ؽ��� �� ///2007�ε�����

};

#endif // _LAND_DEMO_H_
