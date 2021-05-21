#ifndef _LD_DECO_OBJ_H_
#define _LD_DECO_OBJ_H_

#include "Config.h"

#include "DecoObj.h"

class CLdDecoObj
{
// 캐릭터와 관련된 effects
	list<int>		m_listEffect;
	void			InsertEffectNum(int num);

protected:
	BOOL		m_bOpenedRoof;
public:
	CLdDecoObj();
	~CLdDecoObj();

	CLolos *m_pLolo;						// object 하나.
	int		m_nLoloId;

	float m_wx, m_wy, m_wz, m_dir;			// 월드공간 좌표와 방향

	BOOL  m_bDoAni;
	BOOL  m_bUpAni;
	float m_fAdjustY, m_fDownAdjustY;
	DWORD m_dwAdjustTime;
	void  AdjustYPos(float fAdjust, BOOL bUpAni, BOOL bDoAni);

	D3DVECTOR	m_vBoundBox[8];
	WORD		m_wColorIdx;
	DWORD		m_dwColorPick;
	int			m_nCurFrame;
	float		m_fTimeElapsed;

	static D3DMATRIX	m_matWorld, m_matTrans, m_matRotate;
	static float		m_fAniRate;

	// Particle Effects...
	BOOL			m_LineEffectsLoad;
	void			DeleteEffectNum();

	Deco_Particle*	Fire_Particle;
	Deco_Particle*	Smoke_Particle;
	Deco_Particle*	Electric_Particle;
	Deco_Particle*	ClystalGlow;
	Deco_Particle*	Waterfall;
	Deco_Particle*	Waterfall_Particle;
	Deco_Particle*	PortalBossraid;
	Deco_Particle*	Firesmoke_Particle;

	void		CalBoundBox();

	void		OpenRoof()
	{
		m_bOpenedRoof = TRUE;
	};
	void		CloseRoof()
	{
		m_bOpenedRoof = FALSE;
	};
	BOOL		IsRoofOpend()
	{
		return m_bOpenedRoof;
	};
	BOOL		IsInside(float wx, float wz);
	BOOL		MustRenderLast()
	{
		return (m_pLolo->MustRenderLast());
	}

	HRESULT Render(BOOL bNoRoof = FALSE, int whichAlpha=DECO_ALPHA_NONE, BOOL bAnimate=TRUE ); ///2007로딩변경 때 애니메이트 여부 플래그 추가.

	// 멀리 있는 것 알파 블렌딩을 위한 Factor, 0.0f: 먼거, 1.0f: 가까운거
	float m_farFactor;
};

class CLdDecoObjSet
{
protected:
	IndexedTexture	*m_pIndexedTexture;
	char	*m_pLolosDir;

public:
	CLolos	*m_pLoloSet;					// 배열 ,,, 이중포인터가 아니다.

	char	**m_FileName;
	char	**m_NickName;
	int		*m_pnObjIdx;						// object 고유 넘버.
	int		m_nLoloCount;
	int		*m_pnObjType;						// 오브젝트의 종류.

	CLdDecoObjSet(IndexedTexture *pTexture);
	virtual ~CLdDecoObjSet();

	void SetLolosDir(char *dir_name);
	void ReadDataFile(char *file_name);
	char *GetSentence(char *dest, int n, FILE *fp);
	char *PGetSentence(char *dest, int n, PFILE *fp);
};

#endif // _LD_DECO_OBJ_H_
