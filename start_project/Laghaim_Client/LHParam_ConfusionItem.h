#ifndef _LHParam_ConfusionItem_H_
#define _LHParam_ConfusionItem_H_
#pragma once

#include "ItemProto.h"

enum eConfusion_Result
{
	eConfusion_Result__Done,			// ����
	eConfusion_Result__Empty,			// ��ȯâ�� ������ ����
	eConfusion_Result__Invalied_Item,	// �߸��� ������ ����
	eConfusion_Result__Disable,			// �߸��� ������ ����
	eConfusion_Result__Equipments,		// ��񰳼��� �߸���
	eConfusion_Result__Laim,			// ���� ����
	eConfusion_Result__Gem,				// �ʿ亸���ƴ�
	eConfusion_Result__Gem_Count,		// �ʿ亸�������� Ʋ��

	eConfusion_Result__EndOfEnum
};


class LHParam_ConfusionItem
{
public:
	struct sData
	{
		int		grade;			// ��� Ȯ�ο�
		bool	enable;			// �������
		int 	need_laim;		// �ʿ����
		int 	need_gem;		// �ʿ亸��
		int 	need_gem_count;	// �ʿ亸�� ����
		int		value_range;	// ������( for server )
	};


private:
	bool m_initialized;
	sData m_infolist[eItem_Grade__EndOfEnum];


protected:
	void Init();
	void Release();

	bool Open();
	void Close();


protected:
	bool _CheckLaim(CItem * item);
	bool _CheckEquip(CItem * item);
	bool _CheckJewelry(CItem * item);


public:
	bool CheckItem(CItem * item);


public:
	sData * GetParam(eItem_Grade_Type item_grade);

	eConfusion_Result Execute();



public:
	LHParam_ConfusionItem(void);
	~LHParam_ConfusionItem(void);
};
#endif // _LHParam_ConfusionItem_H_