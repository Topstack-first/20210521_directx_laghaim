#ifndef _WAR_INFO_H_
#define _WAR_INFO_H_

#include "Config.h"

#include "dxutil.h"
#include "ddutil.h"
#include "sheet.h"
#include "TextOutBox.h"

#define WARINFO_NUM		8
#define WARINFO2_NUM	4

const int WARINFO_BASE_X = 5;
const int WARINFO_BASE_Y = 106;

class CWarInfo
{
public:
	CWarInfo();
	virtual ~CWarInfo();

	void FontInit();
	void DeleteRes();
	void LoadRes(int screenWidth, int screenHeight);
	void Draw();

	void SetWarInfo(char *guild_name, char *owner_name, int nOwnerSec, char *first_guild_name, int nFirstSec, float owner_x, float owner_z, int point);
	// nate 2005-03-11
	// Matrix
	// Matrix server battle info
	void SetMatWarInfo( int nHawklawPoint, int nCoolrainPoint, int nShiberPoint,int nBattleTime );
	void SetMatWar2009Info(int nGuild1, int nGuild2,int nBattleTime );
	void SetMatBattlePoint( int nBattlePoint );

	void SetWarInfo2(int flag_num, char *guild_name, char *owner_name);
	void SetBattleCount(int nSec);

	CTextOutBox		m_WarInfoBox[WARINFO_NUM + WARINFO2_NUM];
	CSurface		*m_pWarInfoBase, *m_pWarInfoBase2;
	CSurface *m_pWarScoreBase[4];

	int				m_ScreenWidth, m_ScreenHeight;
	BOOL m_bTeamLose[3];

};



class CMatrixWarMessage
{
public:
	DWORD	m_dwBefTime;
	DWORD	m_dwCurTime;
	int		m_Second;
	int		m_OldCount;
	int		m_NewCount;

	CMatrixWarMessage();
	~CMatrixWarMessage();

	void	Init();
	void	MatrixMsgSet(int sec=0);
	void	MsgProc();
};

#endif // _WAR_INFO_H_
