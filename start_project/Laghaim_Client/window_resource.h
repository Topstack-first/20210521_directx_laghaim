#ifndef _WINDOW_UI_RESOURCE_H_
#define _WINDOW_UI_RESOURCE_H_

#include "Config.h"


enum WINDOW_RESOURCE
{
	WBK_BATTLE = 0,
	WBK_SEARCH,
	WBK_CREATE,
	WBK_MIX,
	WBK_PROMOTION,
	WBK_COMBINATION,
	WBK_NPCPAGE,
	WBK_BK,
	WBK_RANKSYSTEM,
	WBK_RANKPARICIPATE,
	WBK_INVASIONINFO,
	WBK_MEMBERNOTICE,
	WBK_DMITRON_MENU,	// 드미트론 점령전 - 신청
	WBK_DMITRON_INFO,	// 드미트론 점령전 - 신청 정보
	WBK_DMITRON_TAX,	// 드미트론 점령전 - 세율

	WBK_TOTAL_NUM
};

enum WINDOW_TGA_RESOURCE
{
	WBK_TGA_WANTED_PAGE = 0,
	WBK_TGA_WANTED_REGISTER,
	WBK_TGA_WANTED_MYMYNU,
	WBK_TGA_WANTED_REGISTER_LIST,
	WBK_TGA_TOTAL_NUM
};

class CSurface;
class IndexedTexture;

class CWINDOWUI
{
public:
	static CWINDOWUI* Create();
	~CWINDOWUI();
public:
	void LoadResource();
	void LoadRes();
	void DeleteRes();

	CSurface*	GetWindowUI(int nNum)
	{
		return m_pWindowUi[nNum];
	}

	char* GetRaceName(int nNum);
	char* GetRaceShortName(int nNum);

	char* GetZoneName(int nNum);
	char* GetGuildLevel(int nNum);

	IndexedTexture* GetWindowTgaUI()
	{
		return m_pTgaWindowUI;
	}
	int	GetTgaWindwResNum(int nNum)
	{
		return m_nTgaWindowNum[nNum];
	}


private:
	CSurface*	m_pWindowUi[WBK_TOTAL_NUM];

	IndexedTexture *m_pTgaWindowUI;
	int				m_nTgaWindowNum[WBK_TGA_TOTAL_NUM];

	CWINDOWUI();
};

#endif // _WINDOW_UI_RESOURCE_H_