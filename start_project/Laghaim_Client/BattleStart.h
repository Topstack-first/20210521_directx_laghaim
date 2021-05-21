#ifndef _BATTLE_START_H_
#define _BATTLE_START_H_

#include "Config.h"

class IndexedTexture;

class CBattleStart
{
	LPD3DTLVERTEX		m_rectBattle;
	LPD3DTLVERTEX		m_rectStart;

	DWORD				m_BattleTexId;
	DWORD				m_StartTexId;
	DWORD				m_CountDownId[10];

	LPD3DTLVERTEX		m_rectCountDown;
	LPD3DTLVERTEX		m_rectWinLose;

	DWORD				m_WinTexId;
	DWORD				m_LoseTexId;

	IndexedTexture*		m_pIndexedTexture;
	int					m_CurFrame;
	int					m_TexNum;

public:
	CBattleStart();
	virtual ~CBattleStart();

	void LoadRes();
	void DeleteRes();

	void Render(int RendNum);

};

#endif // _BATTLE_START_H_
