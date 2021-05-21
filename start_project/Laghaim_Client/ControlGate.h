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

#define GATE_TYPE_MATRIX_CENTER   1010  // ����Ʈ Ÿ�Ժ��� �ε���. ///���������� �� ���� �߰�.

class CListBox;
struct DmitronGate						// ���Ʈ�� ���༼
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
	BYTE			m_byWarp[ 6 ];	// nate 2005-03-11 : Matrix - ���� ����Ʈ ���ؽ� �Ҵ�
	BYTE			m_byPopupType;

	int             m_GateType; // ����Ʈ ������ ����صα� ���ؼ� ���. ///���������� �� ��Ʈ���� ���� ����Ʈ ������ �ʿ��ؼ� �߰��ߴ�. ���� ����Ʈ���� �̰ɷ� üũ�ؼ� �޽��� ó���� ������. 07-03-13 ����

	int				m_GateSubType; // ���Ʈ�� ������ - ���� ��Ż
	int				m_ItemCurrent; // ���Ʈ�� ������ - ���� ��Ż
	void*			m_ItemArray[100]; // ���Ʈ�� ������ - ���� ��Ż

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

	void RemoveAll(); // ���Ʈ�� ������ - ���� ��Ż
	void EndItem(); // ���Ʈ�� ������ - ���� ��Ż
	void AddItem(char* name, void* value); // ���Ʈ�� ������ - ���� ��Ż

	int  GetSubType(); // ���Ʈ�� ������ - ���� ��Ż ���� Ÿ��
	void SetSubType(int nSubType); // ���Ʈ�� ������ - ���� ��Ż ���� Ÿ��

	bool IsJapan(int sel); //  [12/1/2008 ppmmjj83] �Ϻ����� üũ

	bool CheckUpdateVersionID(unsigned int zoneID);
};

#endif // _CONTROL_GATE_H_
