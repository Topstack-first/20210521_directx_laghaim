#ifndef _CONTROL_DOCUMENT_H_
#define _CONTROL_DOCUMENT_H_

#include "Config.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "BaseInterface.h"

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CNkCharacter;
class CListBox;

#define DOCUMENT_BASE_X_SIZE			640
#define DOCUMENT_BASE_Y_SIZE			480

#define DOCUMENT_WIDTH		 219
#define DOCUMENT_HEIGHT		 111
#define DOCUMENT_START_X	 215 * m_ScreenWidth / DOCUMENT_BASE_X_SIZE
#define DOCUMENT_START_Y	 140 * m_ScreenHeight / DOCUMENT_BASE_Y_SIZE

#define DOCUMENT_LIST_X		 DOCUMENT_START_X+15
#define DOCUMENT_LIST_Y		 DOCUMENT_START_Y+27
#define DOCUMENT_LIST_WIDTH	 214
#define DOCUMENT_LIST_HEIGHT 95
#define DOCUMENT_LIST_FCOLOR RGB(255, 255, 0)

#define DOCUMENT_NUM				10

class CControlDocument : public CBaseInterface
{
public:
	CControlDocument();
	virtual ~CControlDocument();

	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
	BOOL IsInside(int x, int y);
	void SendMsg();
	void SetDocument( int nCurWorldNum );
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);

	CSurface		*m_pSurfaceBack;

	CBasicButton	m_CloseBtn;
	CListBox		*m_DocumentList;

	char			m_MsgBuf[256];

	int				m_ScreenWidth;
	int				m_ScreenHeight;

	int				m_SocialActNum[DOCUMENT_NUM];
	int				m_nDocument;
	int				m_nSel;

protected:
	int m_slot_index;

public:
	void SetSlot(int slot_index)
	{
		m_slot_index = slot_index;
	}
};

#endif // _CONTROL_DOCUMENT_H_
