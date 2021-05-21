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

// 애완동물 상태 정의
#define PET_STAND			0
#define PET_MOVE			1
#define PET_ATTACK			2
#define PET_DEAD			3
#define PET_KNOCK			4
#define PET_PICKUP			5
#define PET_SOCIAL			6
#define PET_USE_SKILL1		7

#define PET_ACT_INDEX_NUM	25 // 펫의 행동 갯수.
#define PET_CLASSTYPE_NUM	4  // 펫의 형태 갯수( 현재는 기본형(0), 방어형(1), 공격형(2) )

#include "Capsyong.h"

#define PET_EVOLUTION_TYPENUM 3
#define PET_SKILL_MAX 4

extern int g_PetSkillGroup[PET_EVOLUTION_TYPENUM][PET_SKILL_MAX];

//	by evilkiki 2008.08.04
//	return value 위험성으로 함수 수정
extern void GetPetSkillName( int skill_index , OUT char * rv);
//extern char *GetPetSkillName( int skill_index ); // 펫 스킬 인덱스로 부터 이름을 얻어오는 함수.


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
	int		GetClassActNum( int OriActNum ); // ActNum(행동 번호)를 토대로 클래스 별로 실제 사용될 BpdIndex를 얻어온다.
	int		GetActNumFromClassBpd( int ClassBpdIndex ); // 각 클래스의 BpdIndex ActNum을 행동 번호로 변환해주는 녀석.
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
	void	SetPetEffect( int skill_idx, CNkMob *pMob = NULL, CNkCharacter *pCha = NULL ); // CNkCharater 의 SetMagicMob,SetMagicCha 기능의 함수. 스킬을 사용한 펫과 대상에대한 이펙트,모션등을 셋팅한다.

	int m_BodyItem[PET_PIECE_NUM];
	int m_BodyItem_Vnum[PET_PIECE_NUM];
	int m_PetId;
	int m_ActNum;
	int m_NextActNum; // 업&다운 처럼 다른 모션과 이어져야할 경우 사용.
	float m_was_wx, m_was_wz;
	float m_item_wx, m_item_wz;
	char m_strName[MAX_PET_NAME+1];
	BOOL m_bMetaPet;

	int m_PetClass; // 라비용의 타입 : 기본 0, 진화1단계 방어형 1, 진화1단계 공격형 2

	CRectPlane* m_pSkillEffect1; // 스킬 이펙트 용.

	// [3/6/2007 Theodoric] 라비용 배고품 표시 : 라비용의 머리위 사과를 그려준다.
	float GetAppleYPod();
	void RenderApple(float mX, float mY, float mZ);

	DWORD	m_beforTime;
	BOOL	m_bview;
	CRectPlane	eat_smile; // 사과 바탕 이미지
	int m_BodyItem_Class3[PET_PIECE_NUM];
};

#endif // _NK_PET_H_
