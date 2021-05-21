#ifndef _CONTROL_SOCIAL_H_
#define _CONTROL_SOCIAL_H_

#include "Config.h"

#include "NkCharacter.h"
#include "BasicButton.h"
#include "BaseInterface.h"

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CNkCharacter;
class CListBox;

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

#define SOCIAL_WIDTH	(219)		// * m_ScreenWidth / BASE_X_SIZE)
#define SOCIAL_HEIGHT	(344)		// * m_ScreenHeight / BASE_Y_SIZE)
#define SOCIAL_START_X	(200 * m_ScreenWidth / BASE_X_SIZE)
#define SOCIAL_START_Y	(50 * m_ScreenHeight / BASE_Y_SIZE)

#define SOCIAL_LIST_X		(SOCIAL_START_X+17)	// * m_ScreenWidth / BASE_X_SIZE)
#define SOCIAL_LIST_Y		(SOCIAL_START_Y+40)	// * m_ScreenHeight / BASE_Y_SIZE)
#define SOCIAL_LIST_WIDTH	(192)				// * m_ScreenWidth / BASE_X_SIZE)
#define SOCIAL_LIST_HEIGHT	(250)				// * m_ScreenHeight / BASE_Y_SIZE)
#define SOCIAL_LIST_FCOLOR	(RGB(255, 255, 0))

#define SOCIAL_CLOSE_BTN_X		(370)// * m_ScreenWidth / BASE_X_SIZE)
#define SOCIAL_CLOSE_BTN_Y		(353)// * m_ScreenHeight / BASE_Y_SIZE)

#define SOCIAL_NUM				100

class CControlSocial : public CBaseInterface
{
public:
	CControlSocial();
	virtual ~CControlSocial();

	void Init();
	void LoadRes();
	void DeleteRes();
	void Draw();
//	void SetNkCha(CNkCharacter *pNkCha);
	BOOL IsInside(int x, int y);
	void SendMsg();
	void SetEquipArea();
	void SetSocialActNum(int sel);
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	CSurface		*m_pSurfaceBack;
//	CNkCharacter	*m_pNkCha;

	CBasicButton	m_CloseBtn;
	CListBox		*m_SocialList;

	char			m_MsgBuf[256];

	int				m_ScreenWidth;
	int				m_ScreenHeight;

	int				m_SocialActNum[SOCIAL_NUM];
};


#endif // _CONTROL_SOCIAL_H_