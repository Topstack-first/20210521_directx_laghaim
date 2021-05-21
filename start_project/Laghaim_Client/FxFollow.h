#ifndef _FX_FOLLOW_H_
#define _FX_FOLLOW_H_

// FOLLOW ����Ʈ�� ����.
/*#define FX_FOLLOW_TYPE_FAIRY		0 // 345���� ������ ��.
#define FX_FOLLOW_TYPE_LUCKYPIG     1 // ���� �̺�Ʈ ������
#define FX_FOLLOW_TYPE_BABYSOUL		2 // [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ������ �ҿ�
#define FX_FOLLOW_TYPE_VAMPIREBAT   3 ///�����̾���� ����
#define FX_FOLLOW_TYPE_RUDOLPH		4 ///��絹��
#define FX_FOLLOW_TYPE_RING_LIGHT	5 // [1/15/2008 �����] �Ϻ� �� ������ ����Ʈ*/
enum FXTYPE
{
	FX_FOLLOW_TYPE_FAIRY,		// 345���� ������ ��.
	FX_FOLLOW_TYPE_LUCKYPIG,	// ���� �̺�Ʈ ������
	FX_FOLLOW_TYPE_BABYSOUL,	// 6�ֳ� ��� �̺�Ʈ ������ �ҿ�
	FX_FOLLOW_TYPE_VAMPIREBAT,	// �����̾���� ����
	FX_FOLLOW_TYPE_RUDOLPH,		// ��絹��
	FX_FOLLOW_TYPE_RING_LIGHT,	// �Ϻ� �� ������ ����Ʈ
	FX_FOLLOW_TYPE_ARUPAKAPET,	// ����ī��
	FX_FOLLOW_TYPE_CATS,
	FX_FOLLOW_TYPE_ZORO,
	FX_FOLLOW_TYPE_VALO,
	FX_FOLLOW_TYPE_BEAR,
	FX_FOLLOW_TYPE_HORSE,
	FX_FOLLOW_TYPE_JEGUAR,
	FX_FOLLOW_TYPE_BONI,
	FX_FOLLOW_TYPE_AZUL,	//PABLO PET TEST
	FX_FOLLOW_TYPE_VERDE,	//PABLO PET TEST
	FX_FOLLOW_TYPE_ROXO,	//PABLO PET TEST
	FX_FOLLOW_TYPE_AMARELO,	//PABLO PET TEST
	FX_FOLLOW_TYPE_VERMELHO,	//PABLO PET TEST
	FX_FOLLOW_TYPE_CINZA,	//PABLO PET TEST
	FX_FOLLOW_TYPE_FESHIK,
	FX_FOLLOW_TYPE_EZULAN,
	FX_FOLLOW_TYPE_THUNDERBIRD,
	FX_FOLLOW_TYPE_THUNDERBIRD_P,
	FX_FOLLOW_TYPE_PETFAME1,
	FX_FOLLOW_TYPE_PETFAME2,
	FX_FOLLOW_TYPE_PETFAME3,
	FX_FOLLOW_TYPE_PETFAME4,
	FX_FOLLOW_TYPE_PETHELPER,
	FX_FOLLOW_TYPE_PETHALL1,
	FX_FOLLOW_TYPE_PETHALL2,
	FX_FOLLOW_TYPE_PETHALL3,
	FX_FOLLOW_TYPE_PETNATAL1,
	FX_FOLLOW_TYPE_PETNATAL2,
	FX_FOLLOW_TYPE_PETNATAL3,

	FX_FOLLOW_TYPE__ENDOFENUM,
};

//#define FX_FAIRY_SIZE 20.0f // �� ũ��

// ������ ���� ����
#define FX_FAIRY_MOVE_FRAME_TOTAL 16 // �� ������ �ִϸ��̼��� �� ������ ��
#define FX_FAIRY_MOVE_FRAME_WIDTH 8 // �� ������ �ִϸ��̼��� ���� ������ ��

#define FX_FAIRY_BLOCK_FRAME_TOTAL 16 // �� ��ŷ �ִϸ��̼��� �� ������ ��
#define FX_FAIRY_BLOCK_FRAME_WIDTH 8 // �� ��ŷ �ִϸ��̼��� ���� ������ ��

#define FX_FAIRY_SHOW1_FRAME_TOTAL 8 // �� ���� ������ �ִϸ��̼��� �� �����Ӽ�
#define FX_FAIRY_SHOW1_FRAME_WIDTH 8 // �� ���� ������ �ִϸ��̼��� ���� ������ ��

// AI ������Ʈ
#define FX_FAIRY_STATE_STAND		0  // �յ� ���ִ� ����.
#define FX_FAIRY_STATE_SIDESTEP		1  // �¿�� ������ �̵�.
#define FX_FAIRY_STATE_TURNROUND	2  // ������ ���� ���� ����.
#define FX_FAIRY_STATE_FOLLOW		3  // ������ �i�ư��� ����.
#define FX_FAIRY_STATE_BLOCKING		4  // ��ġ��.
#define FX_FAIRY_STATE_SHOW1		5  // ê���� ���� ���.

#define MAX_PARTICLE_FOR_FXFOLLOW  15 // ��ƼŬ �ִ� ����.

// ������ AI ������Ʈ
#define FX_LUCKYPIG_STATE_STAND			0  // �׳� ���ִ� ����.
#define FX_LUCKYPIG_STATE_RANDOMWALK	1  // �ƹ� �������γ� �ɾ.(õõ��)
#define FX_LUCKYPIG_STATE_FOLLOW		2  // ������ �i�ư��� ����.
#define FX_LUCKYPIG_STATE_SOCIAL1		3  // ���� �ֱ�����
#define FX_LUCKYPIG_STATE_SOCIAL2		4  // Ŭ���̾�Ʈ���� �ֺ� ������ ��
#define FX_LUCKYPIG_STATE_HEAL			5  // ȸ�� ���

#define KIND_LUCKY_PIG_CSY 4 // �������� ���� ����.
#define KIND_LUCKY_PIG_BPD 5 // �������� ���� ����.

// ������ �ִϸ��̼� ����
#define FX_LUCKYPIG_ANI_STAND		0  // �׳� ���ִ� ����.
#define FX_LUCKYPIG_ANI_WALK		1  // �ƹ� �������γ� �ɾ.(õõ��)
#define FX_LUCKYPIG_ANI_SOCIAL1		2  // ���� �ֱ�����
#define FX_LUCKYPIG_ANI_SOCIAL2		3  // Ŭ���̾�Ʈ���� �ֺ� ������ ��
#define FX_LUCKYPIG_ANI_HEAL		4  // ȸ�� ���

// [1/15/2008 �����] �Ϻ� �� ������ ����
#define FX_RING_WHITE			0  // ���̴� ��
#define FX_RING_DARK			1  // ������ ��
#define FX_RING_RED				2  // �����Ʈ ���� ��
#define FX_RING_BLUE			3  // �����Ʈ ��� ��
#define FX_RING_GREEN			4  // �����Ʈ �׸� ��
#define FX_RING_GOLD			5  // by evilkiki 2010.07.09 ��� ����Ʈ



//////////////////////////////////////////////////////////////////////////
// [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ���̺� �ҿ�

// ���� ����
enum babysoul_state
{
	FX_BABYSOUL_STATE_NONE = -1,
	FX_BABYSOUL_STATE_STAND,			// ��������
	FX_BABYSOUL_STATE_STAND_TO,			// ���ִ�   ���ۿ��� ���� �������� �̾����� ���� ���� �۾�
	FX_BABYSOUL_STATE_STAND_UP,			// �������� �������� 1 : ������ ������
	FX_BABYSOUL_STATE_STAND_UP_TO,		// ���ִ�   ���ۿ��� ���� �������� �̾����� ���� ���� �۾�
	FX_BABYSOUL_STATE_STAND_TELE,		// �������� �������� 2 : �׷���Ʈ
	FX_BABYSOUL_STATE_STAND_TELE_TO,	// ���ִ�   ���ۿ��� ���� �������� �̾����� ���� ���� �۾�
	FX_BABYSOUL_STATE_WALK,				// �ɾ�� ���
	FX_BABYSOUL_STATE_WALK_TO,			// �ȴ�     ���ۿ��� ���� �������� �̾����� ���� ���� �۾�
	FX_BABYSOUL_STATE_CRITICAL,			// ũ��Ƽ��
};

// ���ϸ޿��� ����
enum babysoul_ani
{
	FX_BABYSOUL_ANI_STAND = 0,	// ��������
	FX_BABYSOUL_ANI_STAND_UP,	// ������ ������
	FX_BABYSOUL_ANI_WALK,		// �ɾ�� ���
	FX_BABYSOUL_ANI_CRITICAL,	// ũ��Ƽ��
};

enum babysoul_tele_state
{
	BS_TELE_SHOW = -1,
	BS_TELE_FADEIN,
	BS_TELE_HIDE,
	BS_TELE_FADEOUT,
};

#define KIND_BABYSOUL_CSY 3 // CSY ����.
#define KIND_BABYSOUL_BPD (FX_BABYSOUL_ANI_CRITICAL+1) // BPD ����.

//////////////////////////////////////////////////////////////////////////

///�����̾���� ���� AI ������Ʈ
#define FX_VAPIREBAT_STATE_STAND			0  // �׳� ���ִ� ����. �Ĵ��Ĵ�, �� ��鸰��.
#define FX_VAPIREBAT_STATE_TURNROUND		1  // ������ ���� ���� ����.
#define FX_VAPIREBAT_STATE_FOLLOW			2  // ������ �i�ư��� ����.

///�����̾���� ���� �ִϸ��̼� ����
#define FX_VAPIREBAT_STAND_FRAME_TOTAL 12 // �� ������ �ִϸ��̼��� �� ������ ��
#define FX_VAPIREBAT_STAND_FRAME_WIDTH 4 // �� ������ �ִϸ��̼��� ���� ������ ��
#define FX_VAPIREBAT_MOVE_FRAME_TOTAL 12 // �� ������ �ִϸ��̼��� �� ������ ��
#define FX_VAPIREBAT_MOVE_FRAME_WIDTH 4 // �� ������ �ִϸ��̼��� ���� ������ ��

#define MAX_VAMPIREBAT_TRACE 25 ///�����̾���� ���� ��������Ʈ ���ý� ��
#define MAX_RING_LIGHT_TRACE 25 // [1/15/2008 �����] �Ϻ� �������� ����Ʈ ���ؽ� ��

///��絹�� AI ������Ʈ
#define FX_RUDOLPH_STATE_STAND			0  // �׳� ���ִ� ����.
#define FX_RUDOLPH_STATE_RANDOMWALK	1  // �ƹ� �������γ� �ɾ.(õõ��)
#define FX_RUDOLPH_STATE_FOLLOW		2  // ������ �i�ư��� ����.
#define FX_RUDOLPH_STATE_SOCIAL1		3  // ���� �ֱ�����(�ٸ� ����)
#define FX_RUDOLPH_STATE_SOCIAL2		4  // Ŭ���̾�Ʈ���� �ֺ� ������ ��,(���ݸ�� �̿�.����.)

#define KIND_RUDOLPH_CSY 6 // �絹�� �ִϸ��̼� CSY ����. // [2008/11/3 Theodoric] ���� �����̾� �̺�Ʈ������ �ϳ� ���� 5+1
#define KIND_RUDOLPH_BPD 4 // �絹�� �ִϸ��̼� ���� ����.
///��絹�� �ִϸ��̼� ����
#define FX_RUDOLPH_ANI_STAND		0  // �׳� ���ִ� ����.
#define FX_RUDOLPH_ANI_WALK			1  // �ƹ� �������γ� �ɾ.(õõ��)
#define FX_RUDOLPH_ANI_SOCIAL1		2  // ���ݵ���
#define FX_RUDOLPH_ANI_SOCIAL2		3  // �״� ����

// [1/15/2007 �����] �Ϻ� �������� ������Ʈ
#define FX_RING_LIGHT_STATE_FOLLOW		1  // ������ �i�ư��� ����.


#define ARUPAKA_CSY_MAX		3	// ĸ�� ����

enum ARUPAKAANI
{
	FX_ARUPAKA_ANI_WALK,		// �ȴ� ����
	FX_ARUPAKA_ANI_DOWN,		// �Ѿ����� ����
	FX_ARUPAKA_ANI_SPIT,		// ħ��� ����
	FX_ARUPAKA_ANI_MAX,			// �����е� ����

	FX_ARUPAKA_ANI_RANDOMWALK	= 10,
	FX_ARUPAKA_ANI_STAND		= 20
};


#define CATS_CSY_MAX		3
enum
{
	FX_CATS_ANI_STAND,
	FX_CATS_ANI_STAND2,
	FX_CATS_ANI_WALK,

	FX_CATS_ANI_MAX
};

enum
{
	CAT_STATE__STAND,
	CAT_STATE__SOCIAL1,
	CAT_STATE__RANDOMMOVE,
	CAT_STATE__FOLLOW,
};

#define ZORO_CSY_MAX		3
enum
{
	FX_ZORO_ANI_STAND,
	FX_ZORO_ANI_STAND2,
	FX_ZORO_ANI_MOVE,
	FX_ZORO_ANI_MAX
};

enum
{
	ZORO_STATE_STAND,
	ZORO_STATE_SOCIAL1,
	ZORO_STATE_RANDOMMOVE,
	ZORO_STATE_FOLLOW,
};

#define VALO_CSY_MAX		3

enum
{
	FX_VALO_ANI_STAND,
	FX_VALO_ANI_STAND2,
	FX_VALO_ANI_MOVE,
	FX_VALO_ANI_MAX
};
enum
{
	VALO_STATE_STAND,
	VALO_STATE_SOCIAL1,
	VALO_STATE_RANDOMMOVE,
	VALO_STATE_FOLLOW,
};

#define BEAR_CSY_MAX		2
enum
{
	FX_BEAR_ANI_IDLE,
	FX_BEAR_ANI_MOVE,
	FX_BEAR_ANI_STAND,
	FX_BEAR_ANI_MAX
};

enum
{
	BEAR_STATE_STAND,
	BEAR_STATE_IDLE,
	BEAR_STATE_RANDOMMOVE,
	BEAR_STATE_FOLLOW,
};

#define HORSE_CSY_MAX		2
enum
{
	FX_HORSE_ANI_STAND,
	FX_HORSE_ANI_STAND2,
	FX_HORSE_ANI_MOVE,
	FX_HORSE_ANI_MAX
};

enum
{
	HORSE_STATE_STAND,
	HORSE_STATE_SOCIAL1,
	HORSE_STATE_RANDOMMOVE,
	HORSE_STATE_FOLLOW,
};

#define JEGUAR_CSY_MAX		3
enum
{
	FX_JEGUAR_ANI_STAND,
	FX_JEGUAR_ANI_STAND2,
	FX_JEGUAR_ANI_MOVE,
	FX_JEGUAR_ANI_MAX
};

enum
{
	JEGUAR_STATE_STAND,
	JEGUAR_STATE_SOCIAL1,
	JEGUAR_STATE_SOCIAL2,
	JEGUAR_STATE_RANDOMMOVE,
	JEGUAR_STATE_FOLLOW,
};

#define BONI_CSY_MAX	3
enum
{
	FX_BONI_ANI_STAND,
	FX_BONI_ANI_STAND2,
	FX_BONI_ANI_MOVE,
	FX_BONI_ANI_MAX
};

enum
{
	BONI_STATE_STAND,
	BONI_STATE_SOCIAL1,
	BONI_STATE_RANDOMMOVE,
	BONI_STATE_FOLLOW,
};

#define AZUL_CSY_MAX		3	//PABLO PET TEST
enum
{
	FX_AZUL_ANI_STAND,
	FX_AZUL_ANI_STAND2,
	FX_AZUL_ANI_MOVE,
	FX_AZUL_ANI_MAX
};

enum
{
	AZUL_STATE_STAND,
	AZUL_STATE_SOCIAL1,
	AZUL_STATE_RANDOMMOVE,
	AZUL_STATE_FOLLOW,
};

#define VERDE_CSY_MAX		3	//PABLO PET TEST
enum
{
	FX_VERDE_ANI_STAND,
	FX_VERDE_ANI_STAND2,
	FX_VERDE_ANI_MOVE,
	FX_VERDE_ANI_MAX
};

enum
{
	VERDE_STATE_STAND,
	VERDE_STATE_SOCIAL1,
	VERDE_STATE_RANDOMMOVE,
	VERDE_STATE_FOLLOW,
};

#define ROXO_CSY_MAX		3	//PABLO PET TEST
enum
{
	FX_ROXO_ANI_STAND,
	FX_ROXO_ANI_STAND2,
	FX_ROXO_ANI_MOVE,
	FX_ROXO_ANI_MAX
};

enum
{
	ROXO_STATE_STAND,
	ROXO_STATE_SOCIAL1,
	ROXO_STATE_RANDOMMOVE,
	ROXO_STATE_FOLLOW,
};

#define AMARELO_CSY_MAX		3	//PABLO PET TEST
enum
{
	FX_AMARELO_ANI_STAND,
	FX_AMARELO_ANI_STAND2,
	FX_AMARELO_ANI_MOVE,
	FX_AMARELO_ANI_MAX
};

enum
{
	AMARELO_STATE_STAND,
	AMARELO_STATE_SOCIAL1,
	AMARELO_STATE_RANDOMMOVE,
	AMARELO_STATE_FOLLOW,
};

#define VERMELHO_CSY_MAX		3	//PABLO PET TEST
enum
{
	FX_VERMELHO_ANI_STAND,
	FX_VERMELHO_ANI_STAND2,
	FX_VERMELHO_ANI_MOVE,
	FX_VERMELHO_ANI_MAX
};

enum
{
	VERMELHO_STATE_STAND,
	VERMELHO_STATE_SOCIAL1,
	VERMELHO_STATE_RANDOMMOVE,
	VERMELHO_STATE_FOLLOW,
};

#define CINZA_CSY_MAX		2	//PABLO PET TEST
enum
{
	FX_CINZA_ANI_STAND,
	FX_CINZA_ANI_STAND2,
	FX_CINZA_ANI_MOVE,
	FX_CINZA_ANI_MAX
};

enum
{
	CINZA_STATE_STAND,
	CINZA_STATE_SOCIAL1,
	CINZA_STATE_RANDOMMOVE,
	CINZA_STATE_FOLLOW,
};

#define FESHIK_CSY_MAX		2
enum
{
	FX_FESHIK_ANI_STAND,
	FX_FESHIK_ANI_STAND2,
	FX_FESHIK_ANI_MOVE,
	FX_FESHIK_ANI_MAX
};

enum
{
	FESHIK_STATE_STAND,
	FESHIK_STATE_SOCIAL1,
	FESHIK_STATE_RANDOMMOVE,
	FESHIK_STATE_FOLLOW,
};

#define THUNDERBIRD_CSY_MAX		2
enum
{
	FX_THUNDERBIRD_ANI_STAND,
	FX_THUNDERBIRD_ANI_STAND2,
	FX_THUNDERBIRD_ANI_MOVE,
	FX_THUNDERBIRD_ANI_MAX
};

enum
{
	THUNDERBIRD_STATE_STAND,
	THUNDERBIRD_STATE_SOCIAL1,
	THUNDERBIRD_STATE_RANDOMMOVE,
	THUNDERBIRD_STATE_FOLLOW,
};

#define THUNDERBIRD_P_CSY_MAX		2
enum
{
	FX_THUNDERBIRD_P_ANI_STAND,
	FX_THUNDERBIRD_P_ANI_STAND2,
	FX_THUNDERBIRD_P_ANI_MOVE,
	FX_THUNDERBIRD_P_ANI_MAX
};

enum
{
	THUNDERBIRD_P_STATE_STAND,
	THUNDERBIRD_P_STATE_SOCIAL1,
	THUNDERBIRD_P_STATE_RANDOMMOVE,
	THUNDERBIRD_P_STATE_FOLLOW,
};

#define EZULAN_CSY_MAX		2
enum
{
	FX_EZULAN_ANI_STAND,
	FX_EZULAN_ANI_STAND2,
	FX_EZULAN_ANI_MOVE,
	FX_EZULAN_ANI_MAX
};

enum
{
	EZULAN_STATE_STAND,
	EZULAN_STATE_SOCIAL1,
	EZULAN_STATE_RANDOMMOVE,
	EZULAN_STATE_FOLLOW,
};

#define PETFAME1_CSY_MAX		2
enum
{
	FX_PETFAME1_ANI_STAND,
	FX_PETFAME1_ANI_STAND2,
	FX_PETFAME1_ANI_MOVE,
	FX_PETFAME1_ANI_MAX
};

enum
{
	PETFAME1_STATE_STAND,
	PETFAME1_STATE_SOCIAL1,
	PETFAME1_STATE_RANDOMMOVE,
	PETFAME1_STATE_FOLLOW,
};

#define PETFAME2_CSY_MAX		2
enum
{
	FX_PETFAME2_ANI_STAND,
	FX_PETFAME2_ANI_STAND2,
	FX_PETFAME2_ANI_MOVE,
	FX_PETFAME2_ANI_MAX
};

enum
{
	PETFAME2_STATE_STAND,
	PETFAME2_STATE_SOCIAL1,
	PETFAME2_STATE_RANDOMMOVE,
	PETFAME2_STATE_FOLLOW,
};

#define PETFAME3_CSY_MAX		2
enum
{
	FX_PETFAME3_ANI_STAND,
	FX_PETFAME3_ANI_STAND2,
	FX_PETFAME3_ANI_MOVE,
	FX_PETFAME3_ANI_MAX
};

enum
{
	PETFAME3_STATE_STAND,
	PETFAME3_STATE_SOCIAL1,
	PETFAME3_STATE_RANDOMMOVE,
	PETFAME3_STATE_FOLLOW,
};

#define PETFAME4_CSY_MAX		2
enum
{
	FX_PETFAME4_ANI_STAND,
	FX_PETFAME4_ANI_STAND2,
	FX_PETFAME4_ANI_MOVE,
	FX_PETFAME4_ANI_MAX
};

enum
{
	PETFAME4_STATE_STAND,
	PETFAME4_STATE_SOCIAL1,
	PETFAME4_STATE_RANDOMMOVE,
	PETFAME4_STATE_FOLLOW,
};

#define PETHELPER_CSY_MAX		2
enum
{
	FX_PETHELPER_ANI_STAND,
	FX_PETHELPER_ANI_STAND2,
	FX_PETHELPER_ANI_MOVE,
	FX_PETHELPER_ANI_MAX
};

enum
{
	PETHELPER_STATE_STAND,
	PETHELPER_STATE_SOCIAL1,
	PETHELPER_STATE_RANDOMMOVE,
	PETHELPER_STATE_FOLLOW,
};

#define PETHALL1_CSY_MAX		2
enum
{
	FX_PETHALL1_ANI_STAND,
	FX_PETHALL1_ANI_STAND2,
	FX_PETHALL1_ANI_MOVE,
	FX_PETHALL1_ANI_MAX
};

enum
{
	PETHALL1_STATE_STAND,
	PETHALL1_STATE_SOCIAL1,
	PETHALL1_STATE_RANDOMMOVE,
	PETHALL1_STATE_FOLLOW,
};

#define PETHALL2_CSY_MAX		2
enum
{
	FX_PETHALL2_ANI_STAND,
	FX_PETHALL2_ANI_STAND2,
	FX_PETHALL2_ANI_MOVE,
	FX_PETHALL2_ANI_MAX
};

enum
{
	PETHALL2_STATE_STAND,
	PETHALL2_STATE_SOCIAL1,
	PETHALL2_STATE_RANDOMMOVE,
	PETHALL2_STATE_FOLLOW,
};

#define PETHALL3_CSY_MAX		2
enum
{
	FX_PETHALL3_ANI_STAND,
	FX_PETHALL3_ANI_STAND2,
	FX_PETHALL3_ANI_MOVE,
	FX_PETHALL3_ANI_MAX
};

enum
{
	PETHALL3_STATE_STAND,
	PETHALL3_STATE_SOCIAL1,
	PETHALL3_STATE_RANDOMMOVE,
	PETHALL3_STATE_FOLLOW,
};
#define PETNATAL1_CSY_MAX		2
enum
{
	FX_PETNATAL1_ANI_STAND,
	FX_PETNATAL1_ANI_STAND2,
	FX_PETNATAL1_ANI_MOVE,
	FX_PETNATAL1_ANI_MAX
};

enum
{
	PETNATAL1_STATE_STAND,
	PETNATAL1_STATE_SOCIAL1,
	PETNATAL1_STATE_RANDOMMOVE,
	PETNATAL1_STATE_FOLLOW,
};

#define PETNATAL2_CSY_MAX		2
enum
{
	FX_PETNATAL2_ANI_STAND,
	FX_PETNATAL2_ANI_STAND2,
	FX_PETNATAL2_ANI_MOVE,
	FX_PETNATAL2_ANI_MAX
};

enum
{
	PETNATAL2_STATE_STAND,
	PETNATAL2_STATE_SOCIAL1,
	PETNATAL2_STATE_RANDOMMOVE,
	PETNATAL2_STATE_FOLLOW,
};

#define PETNATAL3_CSY_MAX		2
enum
{
	FX_PETNATAL3_ANI_STAND,
	FX_PETNATAL3_ANI_STAND2,
	FX_PETNATAL3_ANI_MOVE,
	FX_PETNATAL3_ANI_MAX
};

enum
{
	PETNATAL3_STATE_STAND,
	PETNATAL3_STATE_SOCIAL1,
	PETNATAL3_STATE_RANDOMMOVE,
	PETNATAL3_STATE_FOLLOW,
};

// 345���� ������ ����Ʈ. ����.
class CFxFollow // ���𰡸� ����ٴϴ� ������ ����Ʈ. AI�� ���� ���� �ִ�.
{
public:
	CFxFollow();
	~CFxFollow();

	void DeleteRes();
	void LoadRes();

	void FrameMove( EffectSort &effect_sort, int nAddFrame ); // effect_sort�� �� �� ��Ʈ���� �����ȴ�. �� effect_sort�� �����ذ��鼭 ���������.
	void Render( EffectSort &effect_sort ); // �ְ�� ����� ��ο�.(����� 2D�� ����ӿ� ����. �����ú��� 3D�� �ƴ϶� 2D��ǥ��.)

	// �� ��
	CRectPlane* m_pMainObject;    // ������� �� ���� �ִ� Ŭ����.
	CRectPlane* m_pEffect1;
	CRectPlane* m_pEffect2;
	CLolos*		m_pLolo1;

	Deco_Particle *m_pParticleInfoArray; // Tail_Info Ŭ������ ��ƼŬ ���� Ŭ������ ����.


	// ���� �̺�Ʈ �� ������.
	CCapsyong *m_pCsyLuckyPig[KIND_LUCKY_PIG_CSY];
	CBiped	  *m_pBpdLuckyPig[KIND_LUCKY_PIG_BPD];


	// [7/31/2007 Theodoric] 6�ֳ� ��� �̺�Ʈ ���̺� �ҿ�
	int			m_BabySoulTextrIndex[KIND_BABYSOUL_CSY-1];
	CCapsyong*	m_pCsyBabySoul; //[KIND_BABYSOUL_CSY];
	CBiped*		m_pBpdBabySoul[KIND_BABYSOUL_BPD];

	///��絹��
	CCapsyong *m_pCsyRudolph[KIND_RUDOLPH_CSY];
	CBiped	  *m_pBpdRudolph[KIND_RUDOLPH_BPD];


	CCapsyong	*m_pCsyArupaka[ARUPAKA_CSY_MAX];
	CBiped		*m_pBpdArupaka[FX_ARUPAKA_ANI_MAX];
	bool		m_bSpit;
	CRectPlane	*m_pRectPlane;
	D3DVECTOR	m_vMasterPosDir;

	void SpitRender(EffectSort &effect_sort);


	CCapsyong * m_pCsyCats[CATS_CSY_MAX];
	CBiped * m_pBpdCats[FX_CATS_ANI_MAX];
	CCapsyong * m_pCsyZoro[ZORO_CSY_MAX];
	CBiped * m_pBpdZoro[FX_ZORO_ANI_MAX];
	CCapsyong * m_pCsyValo[VALO_CSY_MAX];
	CBiped * m_pBpdValo[FX_VALO_ANI_MAX];
	CCapsyong * m_pCsyBear[BEAR_CSY_MAX];
	CBiped * m_pBpdBear[FX_BEAR_ANI_MAX];
	CCapsyong * m_pCsyHorse[HORSE_CSY_MAX];
	CBiped * m_pBpdHorse[FX_HORSE_ANI_MAX];
	CCapsyong * m_pCsyJeguar[JEGUAR_CSY_MAX];
	CBiped * m_pBpdJeguar[FX_JEGUAR_ANI_MAX];
	CCapsyong * m_pCsyBoni[JEGUAR_CSY_MAX];
	CBiped * m_pBpdBoni[FX_JEGUAR_ANI_MAX];
	CCapsyong * m_pCsyAZUL[AZUL_CSY_MAX];			//PABLO TEST PET
	CBiped * m_pBpdAZUL[FX_AZUL_ANI_MAX];
	CCapsyong * m_pCsyVERDE[VERDE_CSY_MAX];			//PABLO TEST PET
	CBiped * m_pBpdVERDE[FX_VERDE_ANI_MAX];
	CCapsyong * m_pCsyROXO[ROXO_CSY_MAX];			//PABLO TEST PET
	CBiped * m_pBpdROXO[FX_ROXO_ANI_MAX];
	CCapsyong * m_pCsyAMARELO[AMARELO_CSY_MAX];			//PABLO TEST PET
	CBiped * m_pBpdAMARELO[FX_AMARELO_ANI_MAX];
	CCapsyong * m_pCsyVERMELHO[VERMELHO_CSY_MAX];			//PABLO TEST PET
	CBiped * m_pBpdVERMELHO[FX_VERMELHO_ANI_MAX];
	CCapsyong * m_pCsyCINZA[CINZA_CSY_MAX];			//PABLO TEST PET
	CBiped * m_pBpdCINZA[FX_CINZA_ANI_MAX];
	CCapsyong * m_pCsyFESHIK[FESHIK_CSY_MAX];			
	CBiped * m_pBpdFESHIK[FX_FESHIK_ANI_MAX];
	CCapsyong * m_pCsyEZULAN[EZULAN_CSY_MAX];			
	CBiped * m_pBpdEZULAN[FX_EZULAN_ANI_MAX];
	CCapsyong * m_pCsyTHUNDERBIRD[THUNDERBIRD_CSY_MAX];			
	CBiped * m_pBpdTHUNDERBIRD[FX_THUNDERBIRD_ANI_MAX];
	CCapsyong * m_pCsyTHUNDERBIRD_P[THUNDERBIRD_P_CSY_MAX];			
	CBiped * m_pBpdTHUNDERBIRD_P[FX_THUNDERBIRD_P_ANI_MAX];
	CCapsyong * m_pCsyPETFAME1[PETFAME1_CSY_MAX];			
	CBiped * m_pBpdPETFAME1[FX_PETFAME1_ANI_MAX];
	CCapsyong * m_pCsyPETFAME2[PETFAME2_CSY_MAX];			
	CBiped * m_pBpdPETFAME2[FX_PETFAME2_ANI_MAX];
	CCapsyong * m_pCsyPETFAME3[PETFAME3_CSY_MAX];			
	CBiped * m_pBpdPETFAME3[FX_PETFAME3_ANI_MAX];
	CCapsyong * m_pCsyPETFAME4[PETFAME4_CSY_MAX];			
	CBiped * m_pBpdPETFAME4[FX_PETFAME4_ANI_MAX];
	CCapsyong * m_pCsyPETHELPER[PETHELPER_CSY_MAX];			
	CBiped * m_pBpdPETHELPER[FX_PETHELPER_ANI_MAX];
	CCapsyong * m_pCsyPETHALL1[PETHALL1_CSY_MAX];			
	CBiped * m_pBpdPETHALL1[FX_PETHALL1_ANI_MAX];
	CCapsyong * m_pCsyPETHALL2[PETHALL2_CSY_MAX];			
	CBiped * m_pBpdPETHALL2[FX_PETHALL2_ANI_MAX];
	CCapsyong * m_pCsyPETHALL3[PETHALL3_CSY_MAX];			
	CBiped * m_pBpdPETHALL3[FX_PETHALL3_ANI_MAX];
	CCapsyong * m_pCsyPETNATAL1[PETNATAL1_CSY_MAX];			
	CBiped * m_pBpdPETNATAL1[FX_PETNATAL1_ANI_MAX];
	CCapsyong * m_pCsyPETNATAL2[PETNATAL2_CSY_MAX];			
	CBiped * m_pBpdPETNATAL2[FX_PETNATAL2_ANI_MAX];
	CCapsyong * m_pCsyPETNATAL3[PETNATAL3_CSY_MAX];			
	CBiped * m_pBpdPETNATAL3[FX_PETNATAL3_ANI_MAX];

	// [1/15/2008 �����] �Ϻ� �������� ����Ʈ ����.
	DWORD m_ring_Light_Color;
	DWORD m_ring_Light_TailColor;

	void	SetRing_Light_Color( int ringNum );

};

#endif // _FX_FOLLOW_H_