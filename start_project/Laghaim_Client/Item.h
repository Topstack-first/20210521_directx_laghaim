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
	void SetTimeLimitItem( int LimitTime ); // �ð��� ���������� ����. ///�ð���������
	void SetExPrice( int PriceType, int Price ); // Ư������ ������.

	BOOL IsSameItem( CItem *DestItem ); // ���� ���������� �����ִ� �Լ�. ///���λ���
	BOOL IsEventEnchantItem();

	int m_Index;		// �������� ������ȣ
	int m_Vnum;			// ������ DB��ȣ
	int m_ProtoNum;		// ������ ����Ʈ������ ��ȣ

	int m_PlusNum;		// �÷��� ��
	int m_GemNum;		// ��������

	CItem *m_Next;		// ���� �������� ����Ű�� ������
	int m_SlotX;		// �ڽô� ���Կ����� ��ġ
	int m_SlotY;

	// �ڽ��� 3D ��ǥ�󿡼��� ��ġ�� ���Ⱚ
	float m_X;
	float m_Y;
	float m_Z;
	float m_Dir;

	int m_Special;

	// �����۸��� ������ �Ӽ��� �����ȴ�.
	union
	{
		int m_Num0;
		int m_Defense; // Armor
		int m_Dam1; // Weapon
		int m_AppTo;
		int m_ToGemVnum;	// ������
		int m_GemMinLevel;	// ������
	};
	union
	{
		int m_Num1;
		int m_Dam2; // Weapon
		int m_RecValue;
		int m_MagicDefense; // ����
		int m_NeedKitNum;	// ������
		int m_GemMaxLevel;	// ������
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

	int m_Weight;			// ����
	int m_Int;				// ���� intelligence
	int m_Dex;				// ���
	int m_MinLevel;			// �ּҷ���
	int m_MinLevel_Org;		// �ּҷ��� ������

	int m_nDamageUp2;
	int m_nDeadHit;

	// �ð��� �����ۿ� �׸� �߰�.(050711 ����) ///�ð���������
	BOOL m_bTimeLimit; // �ð����� ���������� �ƴ��� �÷���.
	int  m_LimitTime; // ���� ���� ���� �ð�. �ʴ���

	// Ư���������� �����ۿ� �׸� �߰�.(by ����) // ������ item.dta�� ������ ���� ���� ���������� ������ ���ְ� �Ѵ�.
	int m_bExPrice; // Ư�� ���� ���� ������������ �÷���. // m_Special�� ���� �ι�° ��Ʈ�� Ư���������� ������������ ���õǾ��ֵ�. ������ �������� ������ �� ���� 0���� �ʱ�ȭ�Ѵ�.(������������ �״�� �θ� �ٸ��� m_Special ������ ����ϴ� ������ ���۵��� �������Ƿ� �÷��׷� �Űܵΰ� ���� �����ش�.)
	int m_PriceType; // Ư�� ���� ���� �������� ���� Ÿ��. ���� �������� BP�������� ���� �˷��ش�. // m_Special�� ���� 3��° ��Ʈ�� 4��° ��Ʈ 2��Ʈ�� ǥ��.(��, ���� Ÿ���� 4���� �Ѿ�� �ȵȴ�.) ������ �������� ������ �� ���� 0���� �ʱ�ȭ�Ѵ�.(������������ �״�� �θ� �ٸ��� m_Special ������ ����ϴ� ������ ���۵��� �������Ƿ� �÷��׷� �Űܵΰ� ���� �����ش�.)
	__int64 m_ExPrice; // ��������

	int m_NeedPlatinumLime;
	int m_NeedRedLime;
	int m_NeedGoldLime; // �ʿ� �ݱ� ///���λ���
	int m_NeedSilverLime; // �ʿ� �ݱ� ///���λ���
	int m_NeedBronzeLime; // �ʿ� �ݱ� ///���λ���

	int m_LimitCount; // ���� ����.

	///�Ź���ȭ�ý���
	int m_Special2; // �ι�° �ɼ� �÷���, ���� �ɼ� �÷��׸����� �����ؼ� Ȯ��. ���� �ɼ� �������� �� �����̴�.( 275�����̻� ������ ���� ���������� �� �⺻ ����  -10% ~ +10%�� �ٰ� �ȴ�. �� ����ġ�� �� ������ ����. )
	int m_spbuff[6];

	int m_UpgradeEndurance; // ��ȭ ������. 275���� �̻� ���� ��ȭ �������� ������ ���� ��ȭ�� �ణ�� ������ �ΰ� �Ǿ���.
	int m_MaxUpgradeEndurance; // ��ȭ �������� �ʱ�ġ. �����۸��� 70~130�� ���̰� �ִ�. �������� �̿��� ��ȭ �������� ä����ִ� �Ѱ�ġ��� ���� �ȴ�.

	int m_Endurance;	// �Ϲ� ������
	int m_MaxEndurance; // �Ϲ� ������ Max


	WORD		m_wColorIdx;						// �÷� ���̺� �ε���
	DWORD		m_dwColorPick;						// �÷� �з�����.
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
