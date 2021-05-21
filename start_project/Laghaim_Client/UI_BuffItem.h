#ifndef _UI_BUFFITEM_H_
#define _UI_BUFFITEM_H_

#include "BaseInterface.h"
#include "BuffItemMgr.h"

class CSurface;
class CAlphaPopUp;
class UI_BuffItem : public CBaseInterface
{
public:
	UI_BuffItem();
	~UI_BuffItem();

	void LoadRes();
	void Draw();
	void DeleteRes();
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void ResetPosition();

private:
	void drawDesc();
	std::string getName(const int& index);
	std::string getEffectDesc(const int& index);
	std::string getRemainTimeDesc(const int& index);

private:
	CSurface* m_pSurIcon[POTION_EVENT_NUM];
	CAlphaPopUp* m_pPopDesc;

	int m_posX;
	int m_posY;
};

#endif