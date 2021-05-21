#ifndef _SET_VALUE_PAGE_H_
#define _SET_VALUE_PAGE_H_

#include "Config.h"

#include "headers.h"
#include "BasicButton.h"
#include "DragBox.h"
#include "TextOutBox.h"
#include "MsgPopUp.h"


#define SETVAL_BTN_X			HORIZ_BTN_X
#define SETVAL_BTN_Y			HORIZ_BTN_Y
#define SETVAL_BTN_WIDTH		HORIZ_BTN_WIDTH
#define SETVAL_BTN_HEIGHT		HORIZ_BTN_HEIGHT
#define SETVAL_BTN_GAP			HORIZ_BTN_GAP

#define SETVAL_DRAG_X		198
#define SETVAL_DRAG_Y		127
//#define SETVAL_DRAG_WIDTH	231


#define SETVAL_DRAG_WIDTH	114 -15// �߿��ϴ�..<----------------------> <--��ũ�⿡�� DragButton�̹����� ����ũ�⸦ �����
#define SETVAL_DRAG_BTN_W	15
#define SETVAL_DRAG_BTN_H	14
#define SETVAL_DRAG_GAP		40

#define SETVAL_STAT_X		386
#define SETVAL_STAT_Y		120
//#define SETVAL_STAT_FSIZE	9
#define SETVAL_STAT_FSIZE	14
#define SETVAL_STAT_FCOLOR	RGB(255, 228, 255)

#define SETVAL_VAL_X		189
#define SETVAL_VAL_Y		303
#define SETVAL_VAL_GAP		39
#define SETVAL_VAL_FSIZE	14
#define SETVAL_VITL_FCOLOR	RGB(255, 255, 255)//RGB(246, 11, 61)
#define SETVAL_MANA_FCOLOR	RGB(255, 255, 255)//RGB(123, 110, 244)
#define SETVAL_STAM_FCOLOR	RGB(255, 255, 255)//RGB(49, 220, 85)
#define SETVAL_EPOW_FCOLOR	RGB(255, 255, 255)//RGB(168, 196, 36)

#define SETVAL_LEFTPOINT_X 398
#define SETVAL_LEFTPOINT_X2 398
#define SETVAL_LEFTPOINT_Y 314


class CSheet;
class CSetValuePage;
class CMsgPopUp;

class CSetValuePage
{
public:
	CSheet*			m_pSheet;
	//CSurface*		m_pBatangSur;

	//CBasicButton	m_NextBtn;
	//CBasicButton	m_BackBtn;

	// nate 2004 - 11
	// new race
	// �ּ�ó��
	CDragBox		m_StrDrag;	// ��
	CDragBox		m_IntDrag;	// ����
	CDragBox		m_DexDrag;	// ���
	CDragBox		m_ConDrag;	// ü��
	CDragBox		m_ChaDrag;	// ���

	CTextOutBox		m_StrBox;	// ��
	CTextOutBox		m_IntBox;	// ����
	CTextOutBox		m_DexBox;	// ���
	CTextOutBox		m_ConBox;	// ü��
	CTextOutBox		m_ChaBox;	// ���
	CTextOutBox		m_LeftPoint;

	CTextOutBox		m_VitalBox;		// �����
	CTextOutBox		m_ManaBox;		// ����
	CTextOutBox		m_StaminaBox;	// ���׹̳�
	CTextOutBox		m_EpowerBox;	// �����

	CMsgPopUp		m_MsgPopUp;


	CSetValuePage();
	CSetValuePage( CSheet* pSheet );
	~CSetValuePage();
	void SetSheetPtr( CSheet* pSheet );
	void SetCharitisics(int Str, int Int, int Dex, int Con, int Cha);
	void Init();
	void SetBackground();
	void LoadRes();

	void DeleteRes();
	void Draw();
	void CalcVal();
	void MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Action( int i);
	void Complex(int Case);
	void SetDragBarPos(int x0, int x1, int x2, int x3, int x4);
	BOOL Complete();

	HRESULT Restore();
};

#endif // _SET_VALUE_PAGE_H_