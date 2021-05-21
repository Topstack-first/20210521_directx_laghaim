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

//#define NPC_UPGRADE_CAN_ARMOR_LEVEL 275 // ����� 275���� �̻� ���� ��ȭ�� ���� // ���� ����üũ�� �׳� �ε����� �˻�� �ٲپ���.
#define NPC_UPGRADE_NEED_REGENT_LEVEL 8 // �ʿ� ������ ���̾ƿ��� ����Ʈ�� �ٲ�� +����



//#define MIN_ING_ANI_TIME 2000  // ��ȭ �ִϸ��̼��� �ּ��� ���� �ϴ� ƽ Ÿ��.(��Ŷ�� ������ �� ������ �ִ�.)
//#define MAX_ING_ANI_TIME 5000  // ��ȭ �ִϸ��̼��� �ִ��� ���� �ϴ� ƽ �Ѱ�.(��ǻ�� ��翡 ���� �������� ��ŵ�� ���ֱ⶧���� �Ѱ踦 �ξ���Ѵ�.)

enum { UPGRADE_ANI_NON = -1, UPGRADE_ANI_ING, UPGRADE_ANI_SUCCESS, UPGRADE_ANI_FAIL, UPGRADE_ANI_BROKEN, MAX_UPGRADE_ANI };
enum { UPGRADE_NEED_DIA = 0, UPGRADE_NEED_REGENT_DIA, UPGRADE_NEED_LEVELDOWN_DIA, MAX_NEED_ITEM_IN_NPC_UPGRADE };

class CSurface;
class CTcpIpConnection;
class CItemRender;
class CTextOutBox;
class CItem;
class CBasicButton;
class CSprite; // 2D ��������Ʈ �ִϸ��̼� Ŭ����.


class LHItemSystem_Manager;




// NPC�� ���� ��� ��ȭ,�����������̽�
class CControlUpgrade
{
public:
	CControlUpgrade();
	virtual ~CControlUpgrade();

	void	Init();

	void	DeleteRes();
	void	LoadRes( int Type, CItemRender *pItemRender);
	HRESULT RestoreSurfaces();

	void	ProcessState(); // ���� ��ȯ��. m_ChangeState�� m_NowState �� �ٸ��� �۵��Ѵ�.
	CItem*  Draw(); // �� �Լ��� ������ DrawItemInfo()�Լ����� ǥ�õǴ� ������ ������ �����ϱ� ���� ���δ�.
	void	DrawNeedInfo(); // �������� ��ȭ�� �ʿ��� ������ �ݾ��� ���.
	void	DrawMySlot(); // �ڽ��� ��ȯâ �κ��� �׷��ִ� �Լ�.
	LRESULT MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	HRESULT BrokenPopupMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg);

	BOOL	IsInside( int x, int y );
	BOOL	IsPlayArea( int x, int y ); // �������̽��� ��� ���� Ŭ���ߴ����� üũ.

	void	UpdateTextInfo(int nType, int TextInfoIndex ); // TextInfo ���ǽ��� �ٲ�ε����ִ� �༮.

	int		CheckUpgradeNeed(); // ��ȭ�� �ʿ��� ��(����,����)�� ������ ���Ѵ�. ������ �������� �����͸� ����.(�������� ���ÿ� ������ �ö������� NULL�� �����Ѵ�. ������ �� �ϳ��� �ö������� ����� ����. NULL�� ��� ������ �߸� �ø� ����.)
	int		CheckSlotPure(); // ���׷��̵带 �õ��ϴµ� �������� �������� ���ԵǾ��ִ� ���� üũ.
	BOOL	CheckBrokenItem(); // �������� ���� ����� ������ ��� �޼����� ����Ѵ�. ���� ����� üũ �Լ�.

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
	CItemRender		*m_pItemRender;  // �������� ������ �ϱ� ���ؼ� �̰� �ʿ��ϴ�.
	CItem	 *m_pOverItem;
	CNkCharacter *m_pNkCha; // ���ΰ��� ���� ������.

	CSurface *m_pBackSur; // �������̽� ���
	CSurface *m_pBack_Title;
	CSurface *m_pTextInfo;     // ����� �Ƕ���.

	CSurface *m_pEnableSlot;   // Ȱ��ȭ�� ���� ��� �̹�����.
	CSurface *m_pDisableSlot;  // ��Ȱ��ȭ�� ���� ��� �̹�����.
	CSurface *m_pExistSlot;    // �������� ���� ���� ���� ��� �̹�����.

	CBasicButton	m_UpgradeBtn;		// ��ȭ ��ư
	CBasicButton	m_BtnCancel;         // ��� ��ư

	CSurface *m_pDiaGem;     // ���̾Ƹ�� �׸�.(�ʿ� ���� ǥ�ý� ���)
	CSurface *m_pRegentDiaGem;     // ����Ʈ ���̾Ƹ�� �׸�.(�ʿ� ���� ǥ�ý� ���)
	CSurface *m_pRegOrGrtDiaGem;
	CSurface *m_pCapellaGem;  //Upgrade 850+ weapon

	CTextOutBox m_NeedGemText;  // "�ʿ� ����"�̶� ���ڿ�
	CTextOutBox m_NeedGemNumText;  // �ʿ� ���� ���� ��¿�
	CTextOutBox m_NeedMoneyText1;  // �ʿ� �ݾ׶�. (info�κ�)
	CTextOutBox m_NeedMoneyText2;  // �ʿ� �ݾ׶�. (�����ݾ� �� �κ�)
	CTextOutBox m_HaveMoneyText;  // ���� �ݾ׶�.

	int m_iNeedGemVnum[NEEDGEM_MAX];
	int m_iNeedGemCount[NEEDGEM_MAX];

	CSprite m_SpriteAni[MAX_UPGRADE_ANI]; // ��ȭ ���� ��������Ʈ �ִϸ��̼�.
	DWORD m_dwLastTickTime; // �ִϸ��̼� ����� ƽ ���� ����.(������ ���� ��.)
	DWORD m_dwStartTickTime; // �ִϸ��̼��� ���۵� �ð��� ����ϱ� ���� ����. ���� �ð� ��޸��� �ϱ����ؼ�.
	DWORD m_dwMinAniTime; // ��ȭ �ִϸ��̼��� �ּ��� ���� �ϴ� ƽ Ÿ��.(��Ŷ�� ������ �� ������ �ִ�.)
	DWORD m_dwMaxAniTime;  // ��ȭ �ִϸ��̼��� �ִ��� ���� �ϴ� ƽ �Ѱ�.(��ǻ�� ��翡 ���� �������� ��ŵ�� ���ֱ⶧���� �Ѱ踦 �ξ���Ѵ�.)
	int m_iAniChangeFrame; // �ִϸ��̼��� 'ing'���� �ٸ� �ɷ� ��ȯ�Ǵ� ������. 'ing'�ִϸ��̼��� �� �����ӿ��� �ٸ� �ִϸ��̼����� �Ѿ��.


	LPDIRECTSOUNDBUFFER m_pAniSound[MAX_UPGRADE_ANI]; // �ִϸ��̼Ǵ� ȿ����.

	int m_iType; // ��ȭ���� �������� �÷���.
	int m_my_slotstart_x, m_my_slotstart_y;

	BOOL m_bSlotLock; // ��ü�� ���Կ��� ���� ���� ������Ű�� ���� �÷���.
	int m_NowState; // ���� ����.( 0 - ���, 1 - ��ȭ�� �ִϸ��̼�, 2 - ���� �ִϸ��̼�, 3 - ���� �ִϸ��̼� )
//	BOOL m_bUpgrading; // ��ȭ �ִϸ��̼� �������� �÷���.(��ȭ �ִϸ��̼� �߿� �������̽��� ���ų� �ٸ� ���� ���ϰ� ���� ��Ȱ�� �Ѵ�.)(����,���� �ִϸ��̼��� ����.)

	__int64 NeedItem[MAX_NEED_ITEM_IN_NPC_UPGRADE]; // ��ȭ �Ǵ� ������ ������ �������� �ʿ��� ����.
	__int64 NeedLaim; // �ʿ� ����.

	double m_iOldMoney; // ���� �����ݾ� ///�ִ�ݾ� �ø���


	int m_Result; // ��ȭ ���,(0-����,1-����,2-����)

	int m_iTextInfoIndex, m_iOldTextInfoIndex; // �������ÿ� �ؽ�Ʈ ������ ��Ȳ�� ���� �̹����� �ٸ��� �ε��ؼ� ����. � �̹����� �ε�Ǿ��ִ����� ���� �̹����� ��ȣ �ε����� ����ϴ� ����.

	DWORD	m_dwMouseClickTickTime;


	LHItemSystem_Manager * m_pItemSystemMgr;

	int				m_need_gem[3];
	CBasicStaticEx	m_gem_image;

	int GetGemToIndex(int item_index);

	void ResetText();
};

#endif // _CONTROL_UPGRADE_H_