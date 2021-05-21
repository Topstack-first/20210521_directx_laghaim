#ifndef _CONTROL_UPGRADE_H_
#define _CONTROL_UPGRADE_H_

#include "Config.h"

#include "BasicStatic.h"
#include "DefenseValMgr.h"

#include <dsound.h>

enum INTERFACE_TYPE
{
	INTERFACE_TYPE_UPARMOR ,
	INTERFACE_TYPE_UPWEAPON ,
	INTERFACE_TYPE_CONVERSION ,
	INTERFACE_TYPE_CONFUSION ,
	INTERFACE_TYPE_PROCESS	,
	INTERFACE_TYPE_320LV_WEAPON_CONVERSION ,
	INTERFACE_TYPE_320LV_WEAPON_UPGRADE ,
	INTERFACE_TYPE_320LV_WEAPON_GODWPOWER ,
	INTERFACE_TYPE_LEVELDOWN ,
	INTERFACE_TYPE_ITEMMAKE ,
	INTERFACE_TYPE_ITEMSEPARATE ,
	INTERFACE_TYPE_GMAGICSTONE ,
	INTERFACE_TYPE_GOD_WEAPON_UPGRADE,
	INTERFACE_TYPE_UPARMOR_400,
	INTERFACE_TYPE_BUFFMAKE_MAKE,
	INTERFACE_TYPE_BUFFMAKE_GATCHA,
	INTERFACE_TYPE_BUFFMAKE_MEDAL,

	INTERFACE_TYPE_END_OF_ENUM
};

//#define NPC_UPGRADE_CAN_ARMOR_LEVEL 275 // 현재는 275레벨 이상 방어구만 강화가 가능 // 현재 조건체크를 그냥 인덱스로 검사로 바꾸었다.
#define NPC_UPGRADE_NEED_REGENT_LEVEL 8 // 필요 보석이 다이아에서 리젠트로 바뀌는 +레벨



//#define MIN_ING_ANI_TIME 2000  // 강화 애니메이션이 최소한 돌게 하는 틱 타입.(패킷이 늦으면 더 돌수는 있다.)
//#define MAX_ING_ANI_TIME 5000  // 강화 애니메이션이 최대한 돌게 하는 틱 한계.(컴퓨터 사양에 따라서 프레임이 스킵될 수있기때문에 한계를 두어야한다.)

enum { UPGRADE_ANI_NON = -1, UPGRADE_ANI_ING, UPGRADE_ANI_SUCCESS, UPGRADE_ANI_FAIL, UPGRADE_ANI_BROKEN, MAX_UPGRADE_ANI };
enum { UPGRADE_NEED_DIA = 0, UPGRADE_NEED_REGENT_DIA, UPGRADE_NEED_LEVELDOWN_DIA, MAX_NEED_ITEM_IN_NPC_UPGRADE };

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CTextOutBox;
class CItem;
class CBasicButton;
class CSprite; // 2D 스프라이트 애니메이션 클래스.


class LHItemSystem_Manager;




// NPC를 통한 장비 강화,가공인터페이스
class CControlUpgrade
{
public:
	CControlUpgrade();
	virtual ~CControlUpgrade();

	void	Init();

	void	DeleteRes();
	void	LoadRes( int Type, CItemRender *pItemRender);
	HRESULT RestoreSurfaces();

	void	ProcessState(); // 상태 전환용. m_ChangeState가 m_NowState 랑 다를때 작동한다.
	CItem*  Draw(); // 이 함수의 리턴은 DrawItemInfo()함수에서 표시되는 아이템 정보를 갱신하기 위해 쓰인다.
	void	DrawNeedInfo(); // 아이템을 강화에 필요한 보석과 금액을 출력.
	void	DrawMySlot(); // 자신의 교환창 인벤을 그려주는 함수.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	HRESULT BrokenPopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);

	BOOL	IsInside( int x, int y );
	BOOL	IsPlayArea( int x, int y ); // 인터페이스를 벗어나 땅을 클릭했는지의 체크.

	void	UpdateTextInfo(int nType, int TextInfoIndex ); // TextInfo 서피스를 바꿔로드해주는 녀석.

	int		CheckUpgradeNeed(); // 강화에 필요한 젬(종류,갯수)과 라임을 구한다. 측정한 아이템의 포인터를 리턴.(아이템이 동시에 여러개 올라가있으면 NULL을 리턴한다. 오로지 방어구 하나만 올라가있을때 제대로 리턴. NULL일 경우 유저가 잘못 올린 경우다.)
	int		CheckSlotPure(); // 업그레이드를 시도하는데 부적절한 아이템이 포함되어있는 지의 체크.
	BOOL	CheckBrokenItem(); // 아이템이 깨질 우려가 있을땐 경고 메세지를 출력한다. 깨질 우려의 체크 함수.

	void	SetNkCha( CNkCharacter *pNkCha )
	{
		m_pNkCha = pNkCha;
	}

	int		GetUpgradeType()
	{
		return m_iType;
	}

	HWND			m_hWnd;
	RECT			m_ClientRc;
	CItemRender		*m_pItemRender;  // 아이템을 렌더링 하기 위해선 이게 필요하다.
	CItem	 *m_pOverItem;
	CNkCharacter *m_pNkCha; // 주인공의 참조 포인터.

	CSurface *m_pBackSur; // 인터페이스 배경
	CSurface *m_pBack_Title;
	CSurface *m_pTextInfo;     // 설명용 판때기.

	CSurface *m_pEnableSlot;   // 활성화된 슬롯 배경 이미지용.
	CSurface *m_pDisableSlot;  // 비활성화된 슬롯 배경 이미지용.
	CSurface *m_pExistSlot;    // 아이템이 놓인 곳의 슬롯 배경 이미지용.

	CBasicButton	m_UpgradeBtn;		// 강화 버튼
	CBasicButton	m_BtnCancel;         // 취소 버튼

	CSurface *m_pDiaGem;     // 다이아몬드 그림.(필요 보석 표시시 사용)
	CSurface *m_pRegentDiaGem;     // 리젠트 다이아몬드 그림.(필요 보석 표시시 사용)
	CSurface *m_pRegOrGrtDiaGem;
	CSurface *m_pCapellaGem;  //Upgrade 850+ weapon

	CTextOutBox m_NeedGemText;  // "필요 보석"이란 글자용
	CTextOutBox m_NeedGemNumText;  // 필요 보석 갯수 출력용
	CTextOutBox m_NeedMoneyText1;  // 필요 금액란. (info부분)
	CTextOutBox m_NeedMoneyText2;  // 필요 금액란. (보유금액 윗 부분)
	CTextOutBox m_HaveMoneyText;  // 보유 금액란.

	int m_iNeedGemVnum[NEEDGEM_MAX];
	int m_iNeedGemCount[NEEDGEM_MAX];

	CSprite m_SpriteAni[MAX_UPGRADE_ANI]; // 강화 관련 스프라이트 애니메이션.
	DWORD m_dwLastTickTime; // 애니메이션 진행용 틱 저장 변수.(프레임 갱신 용.)
	DWORD m_dwStartTickTime; // 애니메이션이 시작된 시간을 기억하기 위한 변수. 일정 시간 기달리게 하기위해서.
	DWORD m_dwMinAniTime; // 강화 애니메이션이 최소한 돌게 하는 틱 타입.(패킷이 늦으면 더 돌수는 있다.)
	DWORD m_dwMaxAniTime;  // 강화 애니메이션이 최대한 돌게 하는 틱 한계.(컴퓨터 사양에 따라서 프레임이 스킵될 수있기때문에 한계를 두어야한다.)
	int m_iAniChangeFrame; // 애니메이션이 'ing'에서 다른 걸로 전환되는 프레임. 'ing'애니메이션중 이 프레임에서 다른 애니메이션으로 넘어간다.


	LPDIRECTSOUNDBUFFER m_pAniSound[MAX_UPGRADE_ANI]; // 애니메이션당 효과음.

	int m_iType; // 강화인지 가공인지 플래그.
	int m_my_slotstart_x, m_my_slotstart_y;

	BOOL m_bSlotLock; // 교체시 슬롯에서 빼는 것을 방지시키기 위한 플래그.
	int m_NowState; // 현재 상태.( 0 - 대기, 1 - 강화중 애니메이션, 2 - 성공 애니메이션, 3 - 실패 애니메이션 )
//	BOOL m_bUpgrading; // 강화 애니메이션 중인지의 플래그.(강화 애니메이션 중엔 인터페이스를 끄거나 다른 짓을 못하게 막는 역활을 한다.)(성공,실패 애니메이션은 예외.)

	__int64 NeedItem[MAX_NEED_ITEM_IN_NPC_UPGRADE]; // 강화 또는 가공시 아이템 종류별로 필요한 갯수.
	__int64 NeedLaim; // 필요 라임.

	double m_iOldMoney; // 이전 보유금액 ///최대금액 늘리기


	int m_Result; // 강화 결과,(0-성공,1-실패,2-깨짐)

	int m_iTextInfoIndex, m_iOldTextInfoIndex; // 컨버젼시엔 텍스트 인포를 상황에 따라서 이미지를 다르게 로드해서 쓴다. 어떤 이미지가 로드되어있는지와 이전 이미지의 번호 인덱스를 기억하는 변수.

	DWORD	m_dwMouseClickTickTime;


	LHItemSystem_Manager * m_pItemSystemMgr;

	int				m_need_gem[3];
	CBasicStaticEx	m_gem_image;

	int GetGemToIndex(int item_index);

	void ResetText();
};

#endif // _CONTROL_UPGRADE_H_