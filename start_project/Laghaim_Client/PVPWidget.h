#ifndef _PVP_WIDGET_H_
#define _PVP_WIDGET_H_

#include "Config.h"

#include "BasicButton.h"

class CTextOutBox;
class IndexedTexture;

class CPVPWidget
{
	bool				bActive;

	LPD3DTLVERTEX		m_rectFire;
	LPD3DTLVERTEX		m_rectBack;
	LPD3DTLVERTEX		m_rectEnemyHP[3];
	LPD3DTLVERTEX		m_rectPlayerHP[3];

	int					m_EnemyHP;
	int					m_EnemyHPOld;
	int					m_EnemyLosing;
	int					m_EnemyWinning;

	int					m_PlayerHP;
	int					m_PlayerHPOld;
	int					m_PlayerLosing;
	int					m_PlayerWinning;

	DWORD				m_BackTexId;
	DWORD				m_BarTexId[2];
	DWORD				m_FireTexId[5];

	CTextOutBox*		m_pEnemyName;
	CTextOutBox*		m_pEnemyRecord;

	CTextOutBox*		m_pPlayerName;
	CTextOutBox*		m_pPlayerRecord;

	IndexedTexture*		m_pIndexedTexture;
	IndexedTexture*		m_pTextureAnimation;

	int					m_CurFrame;

	char				m_szEnemyName[256];
	char				m_szPlayerName[256];

	char				m_szEnemyRecord[256];
	char				m_szPlayerRecord[256];

	float				m_fAlpha;
	int					m_ViewWidth;
	int					m_ViewHeight;

	float				m_fSaveTime;
	float				m_fDelta1, m_fDelta2;

	int					m_BarSize;
	int					m_NameWidth;
	int					m_RecordWidth;

	int					m_Bar1X, m_Bar2X;
	int					m_Name1X, m_Name2X;
	int					m_Record1X, m_Record2X;

public:
	CPVPWidget();
	virtual ~CPVPWidget();

	void InitVariables();
	void AdjustVariables(int width, int height);

	void Draw();
	void LoadRes();
	void DeleteRes();

	bool IsActive();
	void Activate(bool status);
	int  Lerp(int x0, int x1, float s);

	void SetPVPStatus(int PlayerHP, int EnemyHP);
	void SetPVPInfo(CNkCharacter *pPlayer, CNkCharacter *pEnemy);
	void SetPVPRecord(int PlayerWin, int PlayerLose, int EnemyWin, int EnemyLose);

};
#endif // _PVP_WIDGET_H_