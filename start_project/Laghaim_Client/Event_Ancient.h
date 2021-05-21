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

#define COMMON_ESTIMATE					0		// 감정
#define EVENT_TYPE_CHILDDAY				1
#define EVENT_TYPE_PARENTSDAY			2

#define EVENT_TYPE_WORLDCUP				8		// 앞으로 추가되는 이벤트라도 매크로를 넣어주자.
#define EVENT_TYPE_THAI_MOTHERS_DAY		9		// 태국 왕비 생신 이벤트. 
#define EVENT_TYPE_THAI_XMAS_TREE		10 		// 태국 크리스마스 트리에게 벨 교환
#define EVENT_TYPE_LAGHAIM_STRING		11 		// 라그하임 글자 교환.
#define EVENT_TYPE_THAI_VALENTINE		12 		// 태국 발렌타인 이벤트, 레드,핑크 쵸콜릿 성별 구분 교환.
#define EVENT_TYPE_KOR2007_VALENTINE	13 		// 한국 2007년도 발렌타인 이벤트, 태국꺼랑 똑같이 레드,핑크 쵸콜릿 성별 구분 교환. 갯수가 다르므로 별도로 만듬
#define EVENT_TYPE_KOR2007_NEWYEAR		14 		// 한국 2007년도 구정 이벤트, 메달을 모아서 돼지 펜던트로 교환. 
#define EVENT_TYPE_KOR2007_WHITEDAY		15 		// 2007.03.14 화이트
#define EVENT_TYPE_KOR_6YEAR			16 		// [8/6/2007 Theodoric] 6주년 기념 이벤트
#define EVENT_TYPE_KOR2007_PEPERO		17 		// 2007.11.01 빼빼로 데이 이벤트
#define EVENT_TYPE_KOR2008_NEWYEAR		18 		// 한국 2008년도 구정 이벤트
#define EVENT_TYPE_STARSTONE_EXCH		19		// 보스레이드 스타스톤 교환 방법을 이벤트 형식으로 처리했다.

#define EVENT_TYPE_GIRASI_EXCH			20 		// 뫼비우스 아레나 한일전 3차 찌라시 수거 이벤트
#define EVENT_TYPE_SK_200804			21 		// [2008/4/7 Theodoric] SK부가서비스 아이템 지급 이벤트
#define EVENT_TYPE_BP_CAMPAIN_200804	22 		// [2008/4/14 Theodoric] BP 소모 캠패인
#define EVENT_TYPE_LP_GIFTBOX_CHANGE	23

#define EVENT_TYPE_NEW_DEFENITEM_ITEM_EXCH	24	// [2008/6/16 Theodoric] 신규 방어구 교환 시스템.
#define EVENT_TYPE_2008_BEIJING_OLYMPIC		25	// [2008/8/1 Theodoric] 베이징 올림픽 이벤트 (금,은,동 팬던트 교환)
#define EVENT_TYPE_2008_GEMSTONE			26	// 2008.10.15 티끌모아 태산
#define EVENT_TYPE_HALLOWEEN			27		// 2008년 할로윈 이벤트 
#define EVENT_TYPE_081114_PREMIUM_EVENT	28		// 신규 프리미엄 이벤트
#define EVENT_TYPE_BOSSRAID_SEAL		29		// 2009 보스레이드 인장이벤트
#define EVENT_TYPE_LP_GIFTBOX_CHANGE_JP	30  	//  [10/29/2008 parkmj] 일본보석함 시스템
#define EVENT_TYPE_KOR_8YEAR			31		// by evilkiki 라그하임8주년 이벤트
#define EVENT_TYPE_BALLOON_EXCH			32		// by evilkiki 전단지 이벤트(축복의풍선)
#define EVENT_TYPE_CUSUK_2010			33		// by evilkiki 2009.09.08 추석 이벤트
#define EVENT_TYPE_HALLOWEEN_2009		34		// by evilkiki 2009.09.08 할로윈 이벤트 
#define EVENT_TYPE_BIRTH_2010			42	// by evilkiki 2010.07.07 글자조합
#define EVENT_TYPE_PRESENT_2010			43  // by evilkiki 2010.07.07 선물교환
#define EVENT_TYPE_BIRTH_CELEBRATE_2010     44 // by hyuni 2010.7.14  생일 축하 상자
#define EVENT_LP_PRESENT_BOX				46	// 2010.10.21 LP상점판매용
#define EVENT_GOD_WEAPON_EXPERIENCE			47
#define EVENT_TYPE_NGA_OPEN					48
#define EVENT_TYPE_AWARD_2010				49
#define EVENT_TYPE_NEWYEAR_2011				50
#define EVENT_TYPE_NEWYEAR_LUCKYBAG			51
#define EVENT_TYPE_FORLOM_MAP				52
#define EVENT_TYPE_FORLOM_DEVICE			53
#define EVENT_TYPE_FORLOM_DEVICE_DM			54
#define EVENT_TYPE_GREENBUD_2011			55			// by evilkiki 2011 새싹 모으기 이벤트
#define EVENT_TYPE_NEW_SUPPORT_2011			56			// 신규지원 서비스.
#define EVENT_TYPE_SUMMER_2011				57			// 일본 2011 여름 이벤트
#define EVENT_TYPE_10YEAR_ITEM				58
#define EVENT_TYPE_S1TOS2					61			// 20번존 출입증 교환
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
	CBasicButton*	m_pBtnCancel;	 // "cancle" 버튼
	CBasicButton*	m_pBtnExchange;	 // "교환" 버튼
	CSurface*       m_pTextInfo;
	CMsgPopUp*		m_pMsgPopUp_OkCancel;
	CAlphaPopUp*    m_pAlphaPopUp;
	CBasicButton*	m_pBtnExchangePaper;	 // "문서감정" 버튼
	CBasicButton*	m_pBtnExchangeRing;	 // "반지회수" 버튼
	int				m_iType; // 0=고대문서 , 1=어버이날 이벤트

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
	void MultiExchange( int NumItem, char* Packet ); // 이벤트 인터페이스 상에서 여러개 아이템 교환 받을때 들어오는 함수. (by 원석) ///다중교환

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