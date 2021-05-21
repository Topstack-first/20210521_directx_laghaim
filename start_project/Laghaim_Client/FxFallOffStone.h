#ifndef _FX_FALLOFF_STONE_H_
#define _FX_FALLOFF_STONE_H_

#include "Config.h"

class CBiped;
class CCapsyong;
class IndexedTexture;

class CFxFallOffStone
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	// nate 2004 - 7 - image
//	IndexedTexture		*m_pIndexedTexture;

	// nate 2004 - 8 ImageManager
//	DWORD					m_ringTexIdx;

public:
	CFxFallOffStone();
	virtual ~CFxFallOffStone();

	// nate 2004 - 8 ImageManager
//	void		LoadRes(IndexedTexture *pIndexedTexture);
	void		LoadRes();

	void		DeleteRes();

	BOOL		Render(EffectSort &effect_sort);
};

class CFxDevil
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr1; // 원본 포인터 주소. 밑에꺼 넣었다가 돌려줘야한다. 그래야 메모리 해제시 문제가 없다.
	LAGVERTEX *OriginalVertexPtr2; // 원본 포인터 주소. 밑에꺼 넣었다가 돌려줘야한다. 그래야 메모리 해제시 문제가 없다.
	LAGVERTEX *m_GreenDevilVertexSet1; // 색상 변환용 버택스 셋 ///컨버젼무기
	LAGVERTEX *m_GreenDevilVertexSet2; // 색상 변환용 버택스 셋
	LAGVERTEX *m_RedDevilVertexSet1; // 색상 변환용 버택스 셋
	LAGVERTEX *m_RedDevilVertexSet2; // 색상 변환용 버택스 셋

public:
	CFxDevil();
	virtual ~CFxDevil();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class CFxSarngaMouel // [12/3/2008 Theodoric] 에이디아 신무기 사릉가 공격 이펙트
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; // 원본 포인터 주소. 밑에꺼 넣었다가 돌려줘야한다. 그래야 메모리 해제시 문제가 없다.
	LAGVERTEX *m_GreenMouelVertexSet; // 색상 변환용 버택스 셋 ///컨버젼무기
	LAGVERTEX *m_RedMouelVertexSet; // 색상 변환용 버택스 셋

public:
	CFxSarngaMouel();
	virtual ~CFxSarngaMouel();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_YetiRing // AIDIA LEVEL 450
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_YetiRing();
	virtual ~LHFX_YetiRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_PeakockRing // AIDIA LEVEL 500
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_PeakockRing();
	virtual ~LHFX_PeakockRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};
class LHFX_TigerRing // AIDIA LEVEL 550
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_TigerRing();
	virtual ~LHFX_TigerRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_UnicornRing // AIDIA LEVEL 600
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_UnicornRing();
	virtual ~LHFX_UnicornRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_650Ring // AIDIA LEVEL 650
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_650Ring();
	virtual ~LHFX_650Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_700Ring // AIDIA LEVEL 700
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_700Ring();
	virtual ~LHFX_700Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_750Ring // AIDIA LEVEL 750
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_750Ring();
	virtual ~LHFX_750Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_800Ring // AIDIA LEVEL 800
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_800Ring();
	virtual ~LHFX_800Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_850Ring // AIDIA LEVEL 850
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_850Ring();
	virtual ~LHFX_850Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_900Ring // AIDIA LEVEL 900
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_900Ring();
	virtual ~LHFX_900Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_950Ring // AIDIA LEVEL 950
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_950Ring();
	virtual ~LHFX_950Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_1000Ring // AIDIA LEVEL 1000
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_1000Ring();
	virtual ~LHFX_1000Ring();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_CHRRing // EVENT CHRISTMAS
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_CHRRing();
	virtual ~LHFX_CHRRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_CHINRing // AIDIA CHINESE RING
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; 
	LAGVERTEX *m_GreenMouelVertexSet; 
	LAGVERTEX *m_RedMouelVertexSet; 

public:
	LHFX_CHINRing();
	virtual ~LHFX_CHINRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};

class LHFX_GriffonRing
{
	CCapsyong				*m_pCapsyong;
	CBiped					*m_pBpd;
	CCylinderPlane*			ring;
	CRectPlane*				ground_corona;

	LAGVERTEX *OriginalVertexPtr; // 원본 포인터 주소. 밑에꺼 넣었다가 돌려줘야한다. 그래야 메모리 해제시 문제가 없다.
	LAGVERTEX *m_GreenMouelVertexSet; // 색상 변환용 버택스 셋 ///컨버젼무기
	LAGVERTEX *m_RedMouelVertexSet; // 색상 변환용 버택스 셋

public:
	LHFX_GriffonRing();
	virtual ~LHFX_GriffonRing();
	void		LoadRes();
	void		DeleteRes();
	BOOL		Render(EffectSort &effect_sort);
};
#endif // _FX_FALLOFF_STONE_H_
