#ifndef _LD_DECO_OBJ_H_
#define _LD_DECO_OBJ_H_

#include "Config.h"

#include "DecoObj.h"

class CLdDecoObj
{
// ĳ���Ϳ� ���õ� effects
	list<int>		m_listEffect;
	void			InsertEffectNum(int num);

protected:
	BOOL		m_bOpenedRoof;
public:
	CLdDecoObj();
	~CLdDecoObj();

	CLolos *m_pLolo;						// object �ϳ�.
	int		m_nLoloId;

	float m_wx, m_wy, m_wz, m_dir;			// ������� ��ǥ�� ����

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

	HRESULT Render(BOOL bNoRoof = FALSE, int whichAlpha=DECO_ALPHA_NONE, BOOL bAnimate=TRUE ); ///2007�ε����� �� �ִϸ���Ʈ ���� �÷��� �߰�.

	// �ָ� �ִ� �� ���� ������ ���� Factor, 0.0f: �հ�, 1.0f: ������
	float m_farFactor;
};

class CLdDecoObjSet
{
protected:
	IndexedTexture	*m_pIndexedTexture;
	char	*m_pLolosDir;

public:
	CLolos	*m_pLoloSet;					// �迭 ,,, ���������Ͱ� �ƴϴ�.

	char	**m_FileName;
	char	**m_NickName;
	int		*m_pnObjIdx;						// object ���� �ѹ�.
	int		m_nLoloCount;
	int		*m_pnObjType;						// ������Ʈ�� ����.

	CLdDecoObjSet(IndexedTexture *pTexture);
	virtual ~CLdDecoObjSet();

	void SetLolosDir(char *dir_name);
	void ReadDataFile(char *file_name);
	char *GetSentence(char *dest, int n, FILE *fp);
	char *PGetSentence(char *dest, int n, PFILE *fp);
};

#endif // _LD_DECO_OBJ_H_
