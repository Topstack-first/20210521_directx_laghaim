#ifndef _EVENT_ANCIENT_H_
#define _EVENT_ANCIENT_H_

#include "ddutil.h"
#include "NkCharacter.h"

class CSurface;
class CBasicButton;
class CItemRender;
class CMsgPopUp;
class CAlphaPopUp;
class LHParam_10YearEvent;

#define NUM_PANNEL 6

#define COMMON_ESTIMATE					0		// ����
#define EVENT_TYPE_CHILDDAY				1
#define EVENT_TYPE_PARENTSDAY			2

#define EVENT_TYPE_WORLDCUP				8		// ������ �߰��Ǵ� �̺�Ʈ�� ��ũ�θ� �־�����.
#define EVENT_TYPE_THAI_MOTHERS_DAY		9		// �±� �պ� ���� �̺�Ʈ. 
#define EVENT_TYPE_THAI_XMAS_TREE		10 		// �±� ũ�������� Ʈ������ �� ��ȯ
#define EVENT_TYPE_LAGHAIM_STRING		11 		// ������� ���� ��ȯ.
#define EVENT_TYPE_THAI_VALENTINE		12 		// �±� �߷�Ÿ�� �̺�Ʈ, ����,��ũ ���ݸ� ���� ���� ��ȯ.
#define EVENT_TYPE_KOR2007_VALENTINE	13 		// �ѱ� 2007�⵵ �߷�Ÿ�� �̺�Ʈ, �±����� �Ȱ��� ����,��ũ ���ݸ� ���� ���� ��ȯ. ������ �ٸ��Ƿ� ������ ����
#define EVENT_TYPE_KOR2007_NEWYEAR		14 		// �ѱ� 2007�⵵ ���� �̺�Ʈ, �޴��� ��Ƽ� ���� ���Ʈ�� ��ȯ. 
#define EVENT_TYPE_KOR2007_WHITEDAY		15 		// 2007.03.14 ȭ��Ʈ
#define EVENT_TYPE_KOR_6YEAR			16 		// [8/6/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ
#define EVENT_TYPE_KOR2007_PEPERO		17 		// 2007.11.01 ������ ���� �̺�Ʈ
#define EVENT_TYPE_KOR2008_NEWYEAR		18 		// �ѱ� 2008�⵵ ���� �̺�Ʈ
#define EVENT_TYPE_STARSTONE_EXCH		19		// �������̵� ��Ÿ���� ��ȯ ����� �̺�Ʈ �������� ó���ߴ�.

#define EVENT_TYPE_GIRASI_EXCH			20 		// ����콺 �Ʒ��� ������ 3�� ���� ���� �̺�Ʈ
#define EVENT_TYPE_SK_200804			21 		// [2008/4/7 Theodoric] SK�ΰ����� ������ ���� �̺�Ʈ
#define EVENT_TYPE_BP_CAMPAIN_200804	22 		// [2008/4/14 Theodoric] BP �Ҹ� ķ����
#define EVENT_TYPE_LP_GIFTBOX_CHANGE	23

#define EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH	24	// [2008/6/16 Theodoric] �ű� �� ��ȯ �ý���.
#define EVENT_TYPE_2008_BEIJING_OLYMPIC		25	// [2008/8/1 Theodoric] ����¡ �ø��� �̺�Ʈ (��,��,�� �Ҵ�Ʈ ��ȯ)
#define EVENT_TYPE_2008_GEMSTONE			26	// 2008.10.15 Ƽ����� �»�
#define EVENT_TYPE_HALLOWEEN			27		// 2008�� �ҷ��� �̺�Ʈ 
#define EVENT_TYPE_081114_PREMIUM_EVENT	28		// �ű� �����̾� �̺�Ʈ
#define EVENT_TYPE_BOSSRAID_SEAL		29		// 2009 �������̵� �����̺�Ʈ
#define EVENT_TYPE_LP_GIFTBOX_CHANGE_JP	30  	//  [10/29/2008 parkmj] �Ϻ������� �ý���
#define EVENT_TYPE_KOR_8YEAR			31		// by evilkiki �������8�ֳ� �̺�Ʈ
#define EVENT_TYPE_BALLOON_EXCH			32		// by evilkiki ������ �̺�Ʈ(�ູ��ǳ��)
#define EVENT_TYPE_CUSUK_2010			33		// by evilkiki 2009.09.08 �߼� �̺�Ʈ
#define EVENT_TYPE_HALLOWEEN_2009		34		// by evilkiki 2009.09.08 �ҷ��� �̺�Ʈ 
#define EVENT_TYPE_BIRTH_2010			42	// by evilkiki 2010.07.07 ��������
#define EVENT_TYPE_PRESENT_2010			43  // by evilkiki 2010.07.07 ������ȯ
#define EVENT_TYPE_BIRTH_CELEBRATE_2010     44 // by hyuni 2010.7.14  ���� ���� ����
#define EVENT_LP_PRESENT_BOX				46	// 2010.10.21 LP�����Ǹſ�
#define EVENT_GOD_WEAPON_EXPERIENCE			47
#define EVENT_TYPE_NGA_OPEN					48
#define EVENT_TYPE_AWARD_2010				49
#define EVENT_TYPE_NEWYEAR_2011				50
#define EVENT_TYPE_NEWYEAR_LUCKYBAG			51
#define EVENT_TYPE_FORLOM_MAP				52
#define EVENT_TYPE_FORLOM_DEVICE			53
#define EVENT_TYPE_FORLOM_DEVICE_DM			54
#define EVENT_TYPE_GREENBUD_2011			55			// by evilkiki 2011 ���� ������ �̺�Ʈ
#define EVENT_TYPE_NEW_SUPPORT_2011			56			// �ű����� ����.
#define EVENT_TYPE_SUMMER_2011				57			// �Ϻ� 2011 ���� �̺�Ʈ
#define EVENT_TYPE_10YEAR_ITEM				58
#define EVENT_TYPE_S1TOS2					61			// 20���� ������ ��ȯ
#define EVENT_TYPE_JPN_9YEAR_ITEM			62
#define EVENT_TYPE_PCBANG_ARUPAKA			63
#define EVENT_BULLION_EXCHANGE				64
#define EVENT_TYPE_NEWYEAR_CARD		65
#define EVENT_TYPE_NATAL		66


class CEventAncient
{
public:
	CSurface	*   m_pBack;
	CSurface	*	m_pBack_Title;
	CBasicButton*	m_pBtnCancel;	 // "cancle" ��ư
	CBasicButton*	m_pBtnExchange;	 // "��ȯ" ��ư
	CSurface*       m_pTextInfo;
	CMsgPopUp*		m_pMsgPopUp_OkCancel;
	CAlphaPopUp*    m_pAlphaPopUp;
	CBasicButton*	m_pBtnExchangePaper;	 // "��������" ��ư
	CBasicButton*	m_pBtnExchangeRing;	 // "����ȸ��" ��ư
	int				m_iType; // 0=��빮�� , 1=����̳� �̺�Ʈ

	BOOL    m_bMainActive;
	BOOL    m_bClick, m_bMove, m_bMoveEnd;
	BOOL    m_bExchange;

	LHParam_10YearEvent *	m_p10YearEvent;

	DWORD m_dwMouseClickTickTime;

public:
	CEventAncient();
	~CEventAncient();

	void LoadRes(int Type, CItemRender *pItemRender);
	void DeleteRes();
	CItem* Draw();
	LRESULT	MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	void LoadExchange();
	void DeleteExchange();
	void DrawExchange();
	void DrawMySlot();

	void Exchange(int result, int type, int vnum_money, int index, int gem_num);
	void ExchangeTree(int result, int type, int vnum, int index);
	void MultiExchange( int NumItem, char* Packet ); // �̺�Ʈ �������̽� �󿡼� ������ ������ ��ȯ ������ ������ �Լ�. (by ����) ///���߱�ȯ

	void RemoveAllMyInven();

public:
	CNkCharacter *m_pNkCha;
	CSurface *m_pEnableSlot;
	CSurface *m_pDisableSlot;
	CSurface *m_pExistSlot;
	CItemRender		*m_pItemRender;
	CItem			*m_pOverItem;

	int m_my_slotstart_x, m_my_slotstart_y;
	char m_MsgBuf[256];

	void SetNkCha(CNkCharacter *pNkCha)
	{
		m_pNkCha = pNkCha;
	}
	LRESULT ExgMessage( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT parentsDayMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT santaExgMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL IsInside(int x, int y);
	BOOL IsPlayArea(int x, int y);
	void InterCancel(BOOL bSend);
	void SendMsg();
	int  CheckMyslot_Exg();
	int  Check2Myslot_Exg();
	int  CheckMyslot_ParentsDay();

	int  CheckMyslot_Santa();
	void ChangeTo(int vnum, int svr_idx, int plus);

	void QuestExchange(int money);
	void GetMsgParentsDay(BOOL bsuccess, int vnum, int sver_idx);

	BOOL m_bQuestWindow;
	HRESULT PopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);

	int GetExItemNum(int item_no);

	bool LoadScript();
};

#endif // _EVENT_ANCIENT_H_