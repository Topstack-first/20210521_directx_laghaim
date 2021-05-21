#ifndef _CLockSmith_H_
#define _CLockSmith_H_
#pragma once


#include "Config.h"

#include "ddutil.h"
#include "NkCharacter.h"

class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;

#define CUBE_ANICOUNT	17
#define BOXWH			54
#define KEYWH			27
#define EXTRASLOT_BOX_X	0
#define EXTRASLOT_BOX_Y	0
#define EXTRASLOT_KEY_X	2
#define EXTRASLOT_KEY_Y	0
#define BOXSTARTINDEX	2373
#define BOXENDINDEX		2376
#define KEYSTARTINDEX	2368
#define KEYENDINDEX		2372

#define BACKIMAGE		"interface/gamble/LockSmith/LockSmith.bmp"
#define ANIIMAGE		"interface/gamble/LockSmith/Ani_result/cube_%02d.bmp"
#define RESULTIMAGE		"interface/gamble/LockSmith/Ani_result/result_%02d.bmp"
#define BTN_CANCEL		"common/btn_close_01"
#define BTN_DECISION	"gamble/LockSmith/btn_decision"

enum RESULT {RS_11, RS_12, RS_13, RS_22, RS_14, RS_23, RS_15, RS_24, RS_33, RS_16, RS_25, RS_34,
			 RS_26, RS_35, RS_44, RS_36, RS_45, RS_46, RS_55, RS_56, RS_66, RS_END
			};


class CLockSmith
{
	HWND				m_hWnd;
	RECT				m_ClientRc;

	CSurface*			m_pBack;						// 빽판 이미지
	CSurface*			m_pCubeAni[CUBE_ANICOUNT];		// 애니메이션 이미지
	CSurface*			m_pResult[RS_END];				// 결과 이미지
	CSurface*			m_pExistSlot;
	CSurface*			m_pEnableSlot;
	CSurface*			m_pDisableSlot;

	CBasicButton*		m_pBtnCancel;					// X 버튼
	CBasicButton*		m_pBtnDecision;					// 결정 버튼

	CItemRender*		m_pItemRender;

	int					m_iResult;						// 결과값
	int					m_iAniCount;
	int					m_slotbox_x, m_slotbox_y;		// 보물상자 인벤
	int					m_slotkey_x, m_slotkey_y;		// 키 인벤
	int					m_iBoxIndex[4];					// 박스 인덱스
	int					m_iKeyIndex[6];					// 키 인덱스

	bool				m_bProcess;						// 주사위가 돌아가는가를 판단
	bool				m_bResultDraw;					// 결과를 그릴지 판단
	bool				m_bMsg;
	int					m_box_vnum;					// 보물상자 vnum

	DWORD				m_dwStartTime;
	DWORD				m_dwMsgTime;
	DWORD				m_dwMouseClickTickTime;

	CItem*				m_pOverItem;


	BOOL IsInside(int x, int y);
	void DrawMySlot(void);



	int GetResultNum(int rv);

public:
	char				m_MsgBuf[256];

	bool IsProcess(void)
	{
		return m_bProcess;
	}

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void LoadRes( CItemRender *pItemRender);
	void DeleteRes(void);
	CItem* Draw(void);

	CLockSmith(void);
	virtual ~CLockSmith(void);
};
#endif // _CLockSmith_H_