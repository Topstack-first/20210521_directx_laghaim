#ifndef _FX_HORN_H_
#define _FX_HORN_H_

#include "Config.h"

class CLolos;

class CFxHorn
{
public:
	CFxHorn();
	virtual ~CFxHorn();

	CLolos *m_pLolo;						// object 하나.
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

	CLolos *m_pLolo;						// object 하나.
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

	CLolos *m_pLolo;						// object 하나.
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

	CLolos *m_pLolo;						// object 하나.
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

	CLolos *m_pLolo;						// object 하나.
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

	CLolos *m_pLolo;						// object 하나.
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


	CLolos *m_pLolo;						// object 하나.
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
	CLolos				*m_pLolo2;	// 진 드래곤 링 용.
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxSummonRing // 에이디아 링중 뭔가를 소환해서 갑자기 튀어나와서 치고 나가는 종류의 이펙트, 기존에 그래픽이 로드되 있는 녀석을 사용할때는 이 녀석을 쓰자.....일본 요청 아이템 불칸링 만들때 제작.(기존 로드된 캡숑등을 이용해서 찍을때 사용하면 유용할 것이다.) by 원석 ///불칸링
{
public:
	CFxSummonRing();
	~CFxSummonRing();

	// 기존에 있는 녀석을 이펙트로 쓰는 녀석이므로 따로 로드도 필요없다.
	BOOL			Render(EffectSort &effect_sort);
};


#endif // _FX_HORN_H_
