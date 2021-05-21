#ifndef _ITEM_H_
#define _ITEM_H_

#include "Config.h"

class CClrAlloc;
class CTextOutBox;

// nate
class CItem
{
public:
	CItem();
	CItem(CClrAlloc *pClrAlloc);
	~CItem();

	void SetColorAlloc(CClrAlloc *pClrAlloc);
	void SetInitPos(float x, float z, float r);
	void SetInfo(int item_index, int vnum, int proto_num);
	void Render(D3DMATRIX *pMatView = NULL, BOOL bVisible=FALSE, BOOL bDontPick = FALSE);
	void Drop()
	{
		m_Dropping = 15;
	}
	void SetGemNum(int gem_num);
	void SetDiaGemNum(int gem_num);
	void SetTimeLimitItem( int LimitTime ); // 시간제 아이템으로 셋팅. ///시간제아이템
	void SetExPrice( int PriceType, int Price ); // 특정가격 지정용.

	BOOL IsSameItem( CItem *DestItem ); // 같은 아이템인지 비교해주는 함수. ///개인상점
	BOOL IsEventEnchantItem();

	int m_Index;		// 아이템의 고유번호
	int m_Vnum;			// 아이템 DB번호
	int m_ProtoNum;		// 아이템 리스트에서의 번호

	int m_PlusNum;		// 플러스 값
	int m_GemNum;		// 보석갯수

	CItem *m_Next;		// 다음 아이템을 가르키는 포인터
	int m_SlotX;		// 자시늬 슬롯에서의 위치
	int m_SlotY;

	// 자신의 3D 좌표상에서의 위치와 방향값
	float m_X;
	float m_Y;
	float m_Z;
	float m_Dir;

	int m_Special;

	// 아이템마다 다음의 속성이 결정된다.
	union
	{
		int m_Num0;
		int m_Defense; // Armor
		int m_Dam1; // Weapon
		int m_AppTo;
		int m_ToGemVnum;	// 원석용
		int m_GemMinLevel;	// 보석용
	};
	union
	{
		int m_Num1;
		int m_Dam2; // Weapon
		int m_RecValue;
		int m_MagicDefense; // 망토
		int m_NeedKitNum;	// 원석용
		int m_GemMaxLevel;	// 보석용
	};
	union
	{
		int m_Num2;
		int m_BonusDam1;
		int m_ReduceManaUsage;
		int m_MagicDamBonus;
	};
	union
	{
		int m_Num3;
		int m_BonusDam2;
		int m_MagicAttackSpeed;
	};
	union
	{
		int m_Num4;
		int m_Watt; // Armor, Weapon
		int m_Mana;
	};
	union
	{
		int m_Num5;
		int m_AttRange;
	};
	union
	{
		int m_Num6;
		int m_Speed;
	};
	union
	{
		int m_Num7;
	};
	union
	{
		int m_Num8;
	};
	union
	{
		int m_Num9;
	};

	int m_Weight;			// 무게
	int m_Int;				// 지혜 intelligence
	int m_Dex;				// 기민
	int m_MinLevel;			// 최소레벨
	int m_MinLevel_Org;		// 최소레벨 원본값

	int m_nDamageUp2;
	int m_nDeadHit;

	// 시간제 아이템용 항목 추가.(050711 원석) ///시간제아이템
	BOOL m_bTimeLimit; // 시간제한 아이템인지 아닌지 플래그.
	int  m_LimitTime; // 실제 남은 제한 시간. 초단위

	// 특별가격지정 아이템용 항목 추가.(by 원석) // 가격을 item.dta에 고정된 가격 말고 유동적으로 셋팅할 수있게 한다.
	int m_bExPrice; // 특별 가격 지정 아이템인지의 플래그. // m_Special의 상위 두번째 비트에 특별가격지정 아이템인지가 셋팅되어있따. 하지만 서버에서 받은후 이 값은 0으로 초기화한다.(가격지정값을 그대로 두면 다른쪽 m_Special 변수를 사용하는 곳에서 오작동할 수있으므로 플래그로 옮겨두고 값을 지워준다.)
	int m_PriceType; // 특별 가격 지정 아이템의 가격 타입. 라임 가격인지 BP가격인지 등을 알려준다. // m_Special의 상위 3번째 비트와 4번째 비트 2비트로 표시.(즉, 지불 타입이 4개를 넘어가선 안된다.) 하지만 서버에서 받은후 이 값은 0으로 초기화한다.(가격지정값을 그대로 두면 다른쪽 m_Special 변수를 사용하는 곳에서 오작동할 수있으므로 플래그로 옮겨두고 값을 지워준다.)
	__int64 m_ExPrice; // 지정가격

	int m_NeedPlatinumLime;
	int m_NeedRedLime;
	int m_NeedGoldLime; // 필요 금괴 ///개인상점
	int m_NeedSilverLime; // 필요 금괴 ///개인상점
	int m_NeedBronzeLime; // 필요 금괴 ///개인상점

	int m_LimitCount; // 제한 갯수.

	///신방어구강화시스템
	int m_Special2; // 두번째 옵션 플래그, 기존 옵션 플래그만으론 부족해서 확장. 가공 옵션 정보등이 들어갈 예정이다.( 275레벨이상 방어구에는 방어력 보정개념이 들어가 기본 방어도에  -10% ~ +10%가 붙게 된다. 이 보정치도 이 변수에 들어간다. )
	int m_spbuff[6];

	int m_UpgradeEndurance; // 강화 내구도. 275레벨 이상 방어구는 강화 내구도의 개념이 들어가서 강화에 약간의 제약을 두게 되었다.
	int m_MaxUpgradeEndurance; // 강화 내구도의 초기치. 아이템마다 70~130의 차이가 있다. 아이템을 이용해 강화 내구도를 채울수있는 한계치라고 보면 된다.

	int m_Endurance;	// 일반 내구도
	int m_MaxEndurance; // 일반 내구도 Max


	WORD		m_wColorIdx;						// 컬러 테이블 인덱스
	DWORD		m_dwColorPick;						// 컬러 패러미터.
	CClrAlloc	*m_pClrAlloc;

	BOOL m_Dropping;		//
	int m_AniFrame;			//
	int m_nFlyFrame;		//
	DWORD m_dwBaseTime;		//

	D3DVERTEX *m_BoundVert;
	unsigned short m_BoundIndex[30];
	CTextOutBox *m_pTextOutBox;

	int m_item_grade;
};

#endif // _ITEM_H_
