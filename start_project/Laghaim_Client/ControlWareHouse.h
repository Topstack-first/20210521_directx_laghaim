#ifndef _CONTROL_WAREHOUSE_H_
#define _CONTROL_WAREHOUSE_H_

#include "Config.h"

// nate
// 창고 슬롯 확장
//#define WARE_PACK_NUM		2
#define WARE_PACK_NUM		3

#define WARE_PACK_INTERVAL	20
#define WARE_SLOT_X_NUM		4
#define WARE_SLOT_Y_NUM		3
#define WARE_SLOT_WIDTH		53
#define WARE_SLOT_HEIGHT	83

class CSurface;
class CBasicButton;
class CTcpIpConnection;
class CItemRender;
class CItem;
class CMsgPopUp;
class CTextOutBox;


class CControlWareHouse//:public CTransStrFormat
{
protected:
	HWND	m_hWnd;
	RECT	m_ClientRc;

	// [4/16/2007 Theodoric] 태국 유료 창고 :: 관련 변수들
	CBasicButton	*m_pCashViewBtn;
	CBasicButton	*m_pNormalViewBtn;
	CTextOutBox		*m_pCashDateText;
	BOOL			m_bCash;			// 유료 창고 보여줄땐 TRUE
	int				m_nCashMaxPackNum;

	// 기본 이미지.
	CSurface *m_pBack;			// 바탕 이미지

	CSurface	*m_pStoreWidening_time_back;	// 확장창고 남은시간 표시
	bool		 m_bStoreWidening;				// 유료아이템을 사용중인지(true:사용, false:사용않함)

	CBasicButton m_InMoneyBtn;	// 입금 버튼
	CBasicButton m_OutMoneyBtn;	// 출금
	CBasicButton m_CloseBtn;	// 종료

	// nate : 창고 슬롯 확장 - 버튼 추가
	CBasicButton m_BtnWindow_1;
	CBasicButton m_BtnWindow_2;
	CBasicButton m_BtnWindow_3;

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
	CItem *m_WareInven[WARE_PACK_NUM][WARE_SLOT_X_NUM][WARE_SLOT_Y_NUM];
	int m_WareSlot[WARE_PACK_NUM][WARE_SLOT_X_NUM][WARE_SLOT_Y_NUM];		// slot의 숫자가 가르키는 것은 갯수.
	__int64	m_nSaveMoney;
	int m_nPackNum;


	void SendMsg();

public:
	bool IsGetList()
	{
		return m_bGetStashEnd?true:false;
	}

	CControlWareHouse();
	~CControlWareHouse();

	// [4/16/2007 Theodoric] 태국 유료 창고 :: 관련 함수들
	void SetCashInfo(int packMaxNumber, int seconds );
	void InvenSlotInfoInit();

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
	BOOL IsPlayArea(int x, int y);

	//창고가 보이는가
	BOOL IsActivate()
	{
		return m_bActivate;
	};
	void ActivateWareHouse(BOOL bActivate);

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
	CItem**	GetWareInven()
	{
		return &m_WareInven[0][0][0];
	}
	int*	GetWareSlotPtr()
	{
		return &m_WareSlot[0][0][0];
	}

	void SetCurPackNum(int pack_num)
	{
		m_nPackNum = pack_num;
	}

	// 서버상에서 온 메시지.
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void Stash(int slot_num, int vnum, int plus, int special, int special2,
			   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int time_limit ); ///시간제아이템 시간값 추가(050711 by 원석) ///신방어구강화시스템
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void StashTo(int slot_num, int vnum, int plus, int special, int special2,
				 int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag); ///신방어구강화시스템
	// [6/1/2007 Theodoric] 내구도 수리 :: 함수 파라미터 내구도 포함
	void StashFrom(int slot_num, int index, int vnum, int plus, int &special, int special2,
				   int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count); ///신방어구강화시스템
	void StashEnd();

	// nate
	// 인자 : 창고팩번호 창고슬롯번호 인벤팩번호 인번좌표들
	void StashPut( int nVnum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nSlot_X, int nSlot_Y );
	void StashGet( int nItemNum, int nInvenPackNum, int nWarePackNum, int nWareSlotNum, int nItemIndex, int nSlot_X, int nSlot_Y );


	void StashPuts(int vnum , int ware_slot , int inven_pack , int item_index );
	void StashPuts(int vnum , int ware_slot , int inven_pack , int item_index , int plus, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count, int swap_flag);
	void StashGets(int vnum , int ware_slot , int inven_pack , int item_index , int inven_x, int inven_y);
	void StashGets(int vnum , int ware_slot , int inven_pack , int item_index , int inven_x, int inven_y, int plus, int special, int special2, int UpEndur, int MaxUpdEndur, int Endur, int MaxEndur, int count);


	void RemoveAll();

	int m_iCurSlot_X;
	int m_iCurSlot_Y;
	CTextOutBox m_MoneyText;
	CTextOutBox m_pNumText[WARE_PACK_NUM][WARE_SLOT_X_NUM][WARE_SLOT_Y_NUM];
	BOOL CheckMoneyStr(char* szMoney);
	char m_strMoney1[12];
	BOOL InMoneyMsg(char* money);
	BOOL OutMoneyMsg(char* money);
	BYTE m_byInOutMoneyState;	//nate 2006-02-23 : 입출금 상태(0:비활성화 상태, 1:입금, 2:출금)
	BOOL m_bMsgWaiting;
	LRESULT MoneyPopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, char* szMoney);
	LRESULT PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);
	std::string m_strStashPacketTemp;

	//  [11/26/2008 parkmj] 유료창고시 더블클릭
	BOOL IsCashStoreUsing()
	{
		return m_bCash;
	}; ///071003 추가


public:
	int GetEmptySlot(int slot_num);
	int GetEmptySlot();
	int FindSlot(int vnum, bool add_item);
	int FindSlot(int vnum , int pack , bool add_item);

	int SlotToPack(int slot_index);
	int SlotToX(int slot_index);
	int SlotToY(int slot_index);

	CItem * GetWareSlotItem(int slot_index);
	int GetWareSlotCount(int slot_index);

	int Push_Available(CItem* pItem);

public:
	void SetNpcIndex(const int& index);
	const int& GetNpcIndex();
	BOOL IsFromSummonNpc();

private:
	int m_indexNpc;
};

#endif // _CONTROL_WAREHOUSE_H_
