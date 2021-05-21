#ifndef _NK_PET_H_
#define _NK_PET_H_

#include "Config.h"
#include "NkMob.h"

class CClrAlloc;
#include"item.h"

#define PET_PIECE_HORN	0
#define PET_PIECE_HEAD	1
#define PET_PIECE_BODY	2
#define PET_PIECE_WINGS	3
#define PET_PIECE_LEGS	4
#define PET_PIECE_TAIL	5
#define PET_PIECE_NUM	6

// �ֿϵ��� ���� ����
#define PET_STAND			0
#define PET_MOVE			1
#define PET_ATTACK			2
#define PET_DEAD			3
#define PET_KNOCK			4
#define PET_PICKUP			5
#define PET_SOCIAL			6
#define PET_USE_SKILL1		7

#define PET_ACT_INDEX_NUM	25 // ���� �ൿ ����.
#define PET_CLASSTYPE_NUM	4  // ���� ���� ����( ����� �⺻��(0), �����(1), ������(2) )

#include "Capsyong.h"

#define PET_EVOLUTION_TYPENUM 3
#define PET_SKILL_MAX 4

extern int g_PetSkillGroup[PET_EVOLUTION_TYPENUM][PET_SKILL_MAX];

//	by evilkiki 2008.08.04
//	return value ���輺���� �Լ� ����
extern void GetPetSkillName( int skill_index , OUT char * rv);
//extern char *GetPetSkillName( int skill_index ); // �� ��ų �ε����� ���� �̸��� ������ �Լ�.


class CPetBpd
{
public:
	CBiped m_Bpd;
	int m_AttFrame;
	int m_SoundFrame;
	int m_SoundNum;
};

#define MAX_PET_NAME	45

class CNkPet  : public CNkMob
{
public:
	CNkPet();
	CNkPet(CClrAlloc *pClrAlloc);
	virtual ~CNkPet();

	virtual void SetBody(int horn, int head, int body, int wings, int legs, int tail);
	void SetPiece(int v_num, int piece_num = -1);
	void	Kill();
	BOOL	Move(float tx, float tz, int nFrameCount = 20);
	BOOL	Attack(float tx, float tz);
	void	SetMotionState(unsigned short nState, int act_num = 0);
	int		GetClassActNum( int OriActNum ); // ActNum(�ൿ ��ȣ)�� ���� Ŭ���� ���� ���� ���� BpdIndex�� ���´�.
	int		GetActNumFromClassBpd( int ClassBpdIndex ); // �� Ŭ������ BpdIndex ActNum�� �ൿ ��ȣ�� ��ȯ���ִ� �༮.
	void	FrameMove(unsigned int nInc, BOOL bVisible = TRUE);
	void	Render(BOOL bMouseOn = FALSE, BOOL bVisible = TRUE, BOOL bDontPick = FALSE);
	BOOL	IsStanding()
	{
		return (m_nMotionState == PET_STAND);
	}
	BOOL	Pickup(float tx, float tz);
	void	SetName(char *name);
	void	SetMetaPet(BOOL bMeta)
	{
		m_bMetaPet = bMeta;
	}
	void	SetPetEffect( int skill_idx, CNkMob *pMob = NULL, CNkCharacter *pCha = NULL ); // CNkCharater �� SetMagicMob,SetMagicCha ����� �Լ�. ��ų�� ����� ��� ��󿡴��� ����Ʈ,��ǵ��� �����Ѵ�.

	int m_BodyItem[PET_PIECE_NUM];
	int m_BodyItem_Vnum[PET_PIECE_NUM];
	int m_PetId;
	int m_ActNum;
	int m_NextActNum; // ��&�ٿ� ó�� �ٸ� ��ǰ� �̾������� ��� ���.
	float m_was_wx, m_was_wz;
	float m_item_wx, m_item_wz;
	char m_strName[MAX_PET_NAME+1];
	BOOL m_bMetaPet;

	int m_PetClass; // ������ Ÿ�� : �⺻ 0, ��ȭ1�ܰ� ����� 1, ��ȭ1�ܰ� ������ 2

	CRectPlane* m_pSkillEffect1; // ��ų ����Ʈ ��.

	// [3/6/2007 Theodoric] ���� ���ǰ ǥ�� : ������ �Ӹ��� ����� �׷��ش�.
	float GetAppleYPod();
	void RenderApple(float mX, float mY, float mZ);

	DWORD	m_beforTime;
	BOOL	m_bview;
	CRectPlane	eat_smile; // ��� ���� �̹���
	int m_BodyItem_Class3[PET_PIECE_NUM];
};

#endif // _NK_PET_H_
