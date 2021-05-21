#ifndef _GUILD_STOREHOUSE_
#define _GUILD_STOREHOUSE_

#include "Config.h"

#define GUILD_STORE_PACK_NUM		10

#define GUILD_STORE_PACK_INTERVAL	20
#define GUILD_STORE_SLOT_X_NUM		4
#define GUILD_STORE_SLOT_Y_NUM		3
#define GUILD_STORE_SLOT_WIDTH		54
#define GUILD_STORE_SLOT_HEIGHT		80


class CSurface;
class CBasicButton;
class CTcpIpConnection;
class CItemRender;
class CItem;
class CMsgPopUp;
class CTextOutBox;

class CControlGuildSHouse
{
protected:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	// 기본 이미지.
	CSurface *m_pBack;			// 인벤 이미지
	CSurface *m_pItemBack;
	CSurface *m_pMoneyBk;		// 그리기만 한다 의미 없음

	CBasicButton m_InMoneyBtn;	// 입금 버튼
	CBasicButton m_OutMoneyBtn;	// 출금
	CBasicButton m_CloseBtn;	// 종료

	CBasicButton* m_pBtnTab;

	CSurface* m_pSlot;

	// Helper Class
	CNkCharacter	*m_pNkCha;

	// 상태변수
	char m_MsgBuf[1024];
	BOOL m_bActivate;
	BOOL m_bGetStashEnd;

	// nate : 창고 아이템 다수 이동시 딜레이 타일
	BOOL m_bWareLock;
	DWORD m_timeWareLock;
	void SetWareLock( BOOL bLock );

	int m_wareslot_start_x, m_wareslot_start_y;
	int m_ScreenWidth;
	int m_ScreenHeight;

	// 창고 변수
	CItem *m_WareInven[GUILD_STORE_PACK_NUM][GUILD_STORE_SLOT_X_NUM][GUILD_STORE_SLOT_Y_NUM];
	int m_WareSlot[GUILD_STORE_PACK_NUM][GUILD_STORE_SLOT_X_NUM][GUILD_STORE_SLOT_Y_NUM];		// slot의 숫자가 가르키는 것은 갯수.
	__int64	m_nSaveMoney;
	int m_nPackNum;

	int	m_using_store;

	void SendMsg();

public:

	CControlGuildSHouse();
	~CControlGuildSHouse();

	void Init();
	void LoadRes();
	void SetNkCha(CNkCharacter *pNkCha)
	{
		m_pNkCha = pNkCha;
	};
	void DeleteRes();
	CItem *Draw();

	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL IsInside(int x, int y);
	void IsInsideBtn(int nBtNum, int x, int y);
	BOOL IsPlayArea(int x, int y);

	//창고가 보이는가
	BOOL IsActivate()
	{
		return m_bActivate;
	};
	void ActivateGStoreHouse(BOOL bActivate, int gLevel = 0);

//=======================================================================================
	// 수정요망 사항 : 창고 - From Network
	void WareInvenTo(int x, int y, int index, int vnum, int gem_num, int special);
	void WareInvenFrom(int x, int y, int w = 1, int h = 1);
//=======================================================================================

	// 창고 아이템에 대한 관련 함수.
	int  GetWareSlotIndex(int x, int y);
	int  GetCurPackNum()
	{
		return m_nPackNum;
	}

	// 서버상에서 온 메시지.
	void Stash(int slot_num, int vnum, int plus, int special, int special2,
			   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ); ///시간제아이템 시간값 추가(050711 by 원석) ///신방어구강화시스템
	// [6/7/2007 Theodoric] 내구도 수리 :: 함수 인자 추가
	void StashTo(int slot_num, int vnum, int plus, int special, int special2,
				 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag); ///신방어구강화시스템
	// [6/7/2007 Theodoric] 내구도 수리 :: 함수 인자 추가
	void StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
				   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count); ///신방어구강화시스템
	void StashEnd();

	// nate
	// 인자 : 창고팩번호 창고슬롯번호 인벤팩번호 인번좌표들
	void StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y );
	void StashGet( int nItemNum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y );

	void RemoveAll();

	// 길드 레벨이 사용할수 있는 창고의 Num 임.
	void setusingstore(int using_store)
	{
		m_using_store = using_store;
	}
	int	 getusingstoreNum() const
	{
		return m_using_store;
	}

	int m_iCurSlot_X;
	int m_iCurSlot_Y;
	CTextOutBox m_MoneyText;
	CTextOutBox m_pNumText[GUILD_STORE_PACK_NUM][GUILD_STORE_SLOT_X_NUM][GUILD_STORE_SLOT_Y_NUM];
	BOOL CheckMoneyStr(char* szMoney);
	char m_strMoney1[12];
	BOOL InMoneyMsg(char* money);
	BOOL OutMoneyMsg(char* money);
	BYTE m_byInOutMoneyState;	//nate 2006-02-23 : 입출금 상태(0:비활성화 상태, 1:입금, 2:출금)
	BOOL m_bMsgWaiting;
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
};
#endif // _GUILD_STOREHOUSE_