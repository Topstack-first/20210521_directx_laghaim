#ifndef _FX_HORN_H_
#define _FX_HORN_H_

#include "Config.h"

class CLolos;

class CFxHorn
{
public:
	CFxHorn();
	virtual ~CFxHorn();

	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

class CFxSilverFly
{
public:
	CFxSilverFly();
	virtual ~CFxSilverFly();

	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

class CFxRaven
{
public:
	CFxRaven();
	virtual ~CFxRaven();

	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

class CFxKines
{
public:
	CFxKines();
	virtual ~CFxKines();

	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

class CFxNeo
{
public:
	CFxNeo();
	virtual ~CFxNeo();

	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

class CFxKaisen
{
public:
	CFxKaisen();
	virtual ~CFxKaisen();

	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture, CShadowRect *pShadowRect);
	void		LoadRes();

	void		DeleteRes();

	void		Render(EffectSort &effect_sort);
};

class CFxPhenix
{
	CCylinderPlane*			ring;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexIdx;

public:
	CFxPhenix();
	virtual ~CFxPhenix();


	CLolos *m_pLolo;						// object �ϳ�.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxGryphon
{
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;
	CSphere*				eng_sphere;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexIdx;
//	DWORD					m_shieldTexId;
//	DWORD					m_coronaTexId;

public:
	CFxGryphon();
	virtual ~CFxGryphon();


	CLolos				*m_pLolo;
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxSphinx
{
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;
	CSphere*				eng_sphere;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexIdx;
//	DWORD					m_shieldTexId;
//	DWORD					m_coronaTexId;

public:
	CFxSphinx();
	virtual ~CFxSphinx();


	CLolos				*m_pLolo;
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxDragon
{
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexIdx;
//	DWORD					m_shieldTexId;
//	DWORD					m_coronaTexId;

public:
	CFxDragon();
	virtual ~CFxDragon();


	CLolos				*m_pLolo;
	CLolos				*m_pLolo2;	// �� �巡�� �� ��.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxSummonRing // ���̵�� ���� ������ ��ȯ�ؼ� ���ڱ� Ƣ��ͼ� ġ�� ������ ������ ����Ʈ, ������ �׷����� �ε�� �ִ� �༮�� ����Ҷ��� �� �༮�� ����.....�Ϻ� ��û ������ ��ĭ�� ���鶧 ����.(���� �ε�� ĸ������ �̿��ؼ� ������ ����ϸ� ������ ���̴�.) by ���� ///��ĭ��
{
public:
	CFxSummonRing();
	~CFxSummonRing();

	// ������ �ִ� �༮�� ����Ʈ�� ���� �༮�̹Ƿ� ���� �ε嵵 �ʿ����.
	BOOL			Render(EffectSort &effect_sort);
};


#endif // _FX_HORN_H_
