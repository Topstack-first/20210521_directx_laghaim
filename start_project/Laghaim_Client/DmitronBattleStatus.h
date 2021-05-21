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
	char	OwnerName[256]; // ���Ʈ�� ��� �̸�
	int		OwnerPoint;
	char	ChalengerName[256]; // ������(�ְ� ����)
	int		ChalengerPoint;	// �����ڰ� ���� ������
	int		LastTime;	// ���� �ð�
	int		OccpTime;   // ���� �ð�


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

