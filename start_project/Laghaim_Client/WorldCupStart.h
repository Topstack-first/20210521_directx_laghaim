#ifndef _WORLDCUP_START_H_
#define _WORLDCUP_START_H_

#include "Config.h"

#include "TextOutBox.h"

class IndexedTexture;
class CSurface;

class CWorldCupStart
{
public:
	CWorldCupStart();
	virtual ~CWorldCupStart();

	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_nRemainTime;
	int m_nScoreA;
	int m_nScoreB;
	int m_nScoreA10;
	int m_nScoreB10;

	char *m_chAGuildName;
	char *m_chBGuildName;

	CSurface		*m_pSurTime;
	CSurface		*m_pSurScoreA[10];
	CSurface		*m_pSurScoreB[10];
	CSurface		*m_pSurScoreA10[10];
	CSurface		*m_pSurScoreB10[10];
	CSurface		*m_pSurScoreBoard;
	CTextOutBox		m_RemainTimeTex;
	CTextOutBox		m_AGuildNameTex;
	CTextOutBox		m_BGuildNameTex;

	void LoadRes();
	void DeleteRes();
	void Draw();
	void DrawScore();

	void SetRemainTime(int ntime);
	void SetScore(int nScoreA, int nScoreB);
	void SetGuildName(char *ch_AGuild, char *ch_BGuild);
};

#endif // _WORLDCUP_START_H_
