#ifndef PREMIUM_EXCHANGE_H
#define PREMIUM_EXCHANGE_H

#include "Config.h"
#include "basewindow.h"

class CTextOutBox;
class IndexedTexture;

class CPremiumExchange
{
	int					m_nType;
	int					m_nSlot;
	CSurface*			m_pBack;
	CSurface*			m_pBackSource[2];
	int					m_nScreenWidth;
	int					m_nScreenHeight;

	char**				m_pszProperList;		// 캐릭명 필터링 리스트
	int					m_nProperListCount;		// 캐릭명 필터링 갯수

	CTextBox			m_boxName;
	CBasicButton		m_btOk;
	CBasicButton		m_btCancel;

	//입력 후 후처리 관련 함수
	BOOL				CheckCharName(char* string);
	BOOL				CheckGuildName(char* string);
public:
	CPremiumExchange();
	~CPremiumExchange();

	void SetInfo(int nType, int nSlot);

	int  Draw();
	void LoadRes();
	void DeleteRes();

	void SetDrawPos();

	void SetStringPos();
	void SetButtonPos();

	BOOL IsInside( int x, int y );
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void ReadProperList(char* szFileName);

	void SendChangeCharName();
};

#endif//PREMIUM_EXCHANGE_H