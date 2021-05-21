#ifndef _BUFFITEMMGR_H_
#define _BUFFITEMMGR_H_

#include "singleton.h"

enum Buff_Index
{
	Buff_Index__Hp_Up,				// 생명력 상승
	Buff_Index__Mp_Up,				// 마나 상승
	Buff_Index__Sp_Up,				// 스테미너 상승
	Buff_Index__Ep_Up,				// 전기력 상승
	Buff_Index__Att_Up,				// 공격력 상승
	Buff_Index__Def_Up,				// 방어력 상승
	Buff_Index__Exp_Up,				// 경험치 상승
	Buff_Index__Laim_Up,			// 라임획득 상승
	Buff_Index__Att_Exp_Up,			// 공격력&경험치 상승
	Buff_Index__Def_Exp_Up,			// 방어력&경험치 상승
	Buff_Index__Att_Def_Up,			// 공격력&방어력 상승
	Buff_Index__Att_Def_Exp_Up,		// 공격력&방어력&경험치 상승
	Buff_Index__InstanceAtt_Up,		// 순간 공격력 상승
	Buff_Index__Critical_Up,		// [미구현] 크리티컬 적중률 상승
	Buff_Index__MonHp_Dn,			// [미구현] 몬스터 체력 하락
	Buff_Index__Dam_dn,				// 데미지 감소
	Buff_Index__Speed_up,			// 이속증가
	Buff_Index__Dyor,				// 퀘스트 포인트 획득량 상승
	Buff_Index__Party,				// 파티원 추가당 전체 능력치 상승

	Buff_Index__Count,
};

enum Buff_Type
{
	POTION_UPDAM = 0,				// 공격력 상승
	POTION_UPDEF,					// 방어력 상승
	POTION_UPDEF_MAGIC,				// 마법,방어력 상승
	POTION_EXP,						// 경험치 상승
	POTION_MONEY,					// 라임획득 상승
	POTION_DYOR,					// 퀘스트 포인트 획득량 상승
	POTION_PARTY,					// 파티원 추가당 전체 능력치 상승
	POTION_VITAL,					// 생명력 상승
	POTION_MANA,					// 마나 상승
	POTION_STAMINA,					// 스테미너 상승
	POTION_ENERGE,					// 전기력 상승
	POTION_CRITICAL,				// 크리티컬 상승
	POTION_STAMINADOWN_MOB,			// 몬스터 체력 감소
	POTION_DAM_DOWN,				// 데미지 감소
	POTION_MOVE,					// 이속 증가
	POTION_INSTANTDAM,				// 순간데미지 증가

	POTION_EVENT_NUM,
};

struct BuffItem
{
	Buff_Type type;
	long remainTime;	// 남은 시간(밀리초)
	int value;			// 툴팁 표시할 효과값
	bool percent;		// 퍼센트 표기
};

class BuffItemMgr : public CSingleton<BuffItemMgr>
{
public:
	typedef std::vector<BuffItem> VEC_BUFFITEM;

	BuffItemMgr();
	~BuffItemMgr();

public:
	void Update();
	void Clear();

	void AddBuff(int type, unsigned long remainTime, int value);
	void RemoveBuff(int type);

	const VEC_BUFFITEM& GetBuffList()
	{
		return m_vecBuffItem;
	}

	unsigned int GetBuffCount();
	int GetBuffType(int index);
	long GetBuffRemainTime(int index);
	int GetBuffValue(int index);
	bool GetBuffPercent(int index);

private:
	unsigned long m_curTime;
	VEC_BUFFITEM m_vecBuffItem;
};

#endif