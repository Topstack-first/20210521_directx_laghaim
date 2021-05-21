#ifndef DMITRON_BATTLE_STATUS_H
#define DMITRON_BATTLE_STATUS_H

#include "Config.h"
#include "DmitronBattle.h"

class CTextOutBox;
class IndexedTexture;

class CDmitronBattleStatus : public CUIBaseWindow
{
public:

	int		ZoneId;
	char	OwnerName[256]; // 드미트론 길드 이름
	int		OwnerPoint;
	char	ChalengerName[256]; // 도전자(최고 점수)
	int		ChalengerPoint;	// 도전자가 입힌 데미지
	int		LastTime;	// 점령 시간
	int		OccpTime;   // 점령 시간


	int m_nPosX, m_nPosY;

	CTextOutMultiBox	m_StatusBox1, m_StatusBox2;
	int					m_ZoneStatus[DMITRON_BATTLE_ZONE];

	CDmitronBattleStatus();
	virtual ~CDmitronBattleStatus();

	int Draw();
	void LoadRes();
	void DeleteRes();

	void Init();
	void MakeDrawPopup();
	void SetDmitronInfo1(int index, int status);
	void SetDmitronInfo2(int index, int status, char* myGuildName, int myGuildPoint,
						 char* topGuildName, int topGuildPoint, int lastTime, int elapsedTime);

};

#endif // DMITRON_BATTLE_STATUS_H

