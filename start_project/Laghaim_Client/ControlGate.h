#ifndef _CONTROL_GATE_H_
#define _CONTROL_GATE_H_

#include "Config.h"

#define BASE_X_SIZE		640
#define BASE_Y_SIZE		480
#define BASE_X_SIZE_F	(640.0f)
#define BASE_Y_SIZE_F	(480.0f)

#define GATE_WIDTH	(219)	// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_HEIGHT	(344)	// * m_ScreenHeight / BASE_Y_SIZE)
#define GATE_START_X	((m_ScreenWidth/2) - (GATE_WIDTH/2))
#define GATE_START_Y	((m_ScreenHeight/2) - (GATE_HEIGHT/2))

#define GATE_LIST_X	(200 * m_ScreenWidth / BASE_X_SIZE)//(GATE_START_X+17)	// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_LIST_Y	(50 * m_ScreenHeight / BASE_Y_SIZE)//(GATE_START_Y+40)	// * m_ScreenHeight / BASE_Y_SIZE)
#define GATE_LIST_WIDTH	(192)			// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_LIST_HEIGHT	(250)		// * m_ScreenHeight / BASE_Y_SIZE)
#define GATE_LIST_FCOLOR	(RGB(255, 255, 0))

#define GATE_CLOSE_BTN_X		(370)	// * m_ScreenWidth / BASE_X_SIZE)
#define GATE_CLOSE_BTN_Y		(353)	// * m_ScreenHeight / BASE_Y_SIZE)

#define GATE_NUM				100

#define GATE_TYPE_MATRIX_CENTER   1010  // 게이트 타입별로 인덱스. ///국가대항전 시 부터 추가.

class CListBox;
struct DmitronGate						// 드미트론 통행세
{
	int Zone, Toll;
	DmitronGate(int Zone, int Toll) : Zone(Zone), Toll(Toll) {}
};


class CControlGate : public CBaseInterface
{
public:

	CSurface		*m_pSurfaceBack;
	CBasicButton	m_CloseBtn;
	CListBox		*m_GateList;
	char			m_MsgBuf[256];
	char			m_strFtoS[3][100];
	char			m_strStoF[4][100];
	int				m_ScreenWidth;
	int				m_ScreenHeight;
	BOOL			m_bSpecialGate;
	BYTE			m_byWarp[ 6 ];	// nate 2005-03-11 : Matrix - 동적 게이트 인텍스 할당
	BYTE			m_byPopupType;

	int             m_GateType; // 게이트 종류를 기억해두기 위해서 사용. ///국가대항전 시 매트릭스 광장 게이트 때문에 필요해서 추가했다. 이후 게이트들은 이걸로 체크해서 메시지 처리를 해주자. 07-03-13 원석

	int				m_GateSubType; // 드미트론 점령전 - 내부 포탈
	int				m_ItemCurrent; // 드미트론 점령전 - 내부 포탈
	void*			m_ItemArray[100]; // 드미트론 점령전 - 내부 포탈

public:
	CControlGate();
	virtual ~CControlGate();

	void Init(int nCurWorldNum=0);
	void SetCurWorld (int nCurWorldNum);
	void ResetCurWorld();
	void LoadRes();
	void DeleteRes();
	void Draw();
	BOOL IsInside(int x, int y);
	void SendMsg();
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);

	void RemoveAll(); // 드미트론 점령전 - 내부 포탈
	void EndItem(); // 드미트론 점령전 - 내부 포탈
	void AddItem(char* name, void* value); // 드미트론 점령전 - 내부 포탈

	int  GetSubType(); // 드미트론 점령전 - 내부 포탈 서브 타입
	void SetSubType(int nSubType); // 드미트론 점령전 - 내부 포탈 서브 타입

	bool IsJapan(int sel); //  [12/1/2008 ppmmjj83] 일본전용 체크

	bool CheckUpdateVersionID(unsigned int zoneID);
};

#endif // _CONTROL_GATE_H_
