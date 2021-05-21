#ifndef _BUFFITEMMGR_H_
#define _BUFFITEMMGR_H_

#include "singleton.h"

enum Buff_Index
{
	Buff_Index__Hp_Up,				// ����� ���
	Buff_Index__Mp_Up,				// ���� ���
	Buff_Index__Sp_Up,				// ���׹̳� ���
	Buff_Index__Ep_Up,				// ����� ���
	Buff_Index__Att_Up,				// ���ݷ� ���
	Buff_Index__Def_Up,				// ���� ���
	Buff_Index__Exp_Up,				// ����ġ ���
	Buff_Index__Laim_Up,			// ����ȹ�� ���
	Buff_Index__Att_Exp_Up,			// ���ݷ�&����ġ ���
	Buff_Index__Def_Exp_Up,			// ����&����ġ ���
	Buff_Index__Att_Def_Up,			// ���ݷ�&���� ���
	Buff_Index__Att_Def_Exp_Up,		// ���ݷ�&����&����ġ ���
	Buff_Index__InstanceAtt_Up,		// ���� ���ݷ� ���
	Buff_Index__Critical_Up,		// [�̱���] ũ��Ƽ�� ���߷� ���
	Buff_Index__MonHp_Dn,			// [�̱���] ���� ü�� �϶�
	Buff_Index__Dam_dn,				// ������ ����
	Buff_Index__Speed_up,			// �̼�����
	Buff_Index__Dyor,				// ����Ʈ ����Ʈ ȹ�淮 ���
	Buff_Index__Party,				// ��Ƽ�� �߰��� ��ü �ɷ�ġ ���

	Buff_Index__Count,
};

enum Buff_Type
{
	POTION_UPDAM = 0,				// ���ݷ� ���
	POTION_UPDEF,					// ���� ���
	POTION_UPDEF_MAGIC,				// ����,���� ���
	POTION_EXP,						// ����ġ ���
	POTION_MONEY,					// ����ȹ�� ���
	POTION_DYOR,					// ����Ʈ ����Ʈ ȹ�淮 ���
	POTION_PARTY,					// ��Ƽ�� �߰��� ��ü �ɷ�ġ ���
	POTION_VITAL,					// ����� ���
	POTION_MANA,					// ���� ���
	POTION_STAMINA,					// ���׹̳� ���
	POTION_ENERGE,					// ����� ���
	POTION_CRITICAL,				// ũ��Ƽ�� ���
	POTION_STAMINADOWN_MOB,			// ���� ü�� ����
	POTION_DAM_DOWN,				// ������ ����
	POTION_MOVE,					// �̼� ����
	POTION_INSTANTDAM,				// ���������� ����

	POTION_EVENT_NUM,
};

struct BuffItem
{
	Buff_Type type;
	long remainTime;	// ���� �ð�(�и���)
	int value;			// ���� ǥ���� ȿ����
	bool percent;		// �ۼ�Ʈ ǥ��
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