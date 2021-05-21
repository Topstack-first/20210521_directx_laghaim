#ifndef _SHEET_H_
#define _SHEET_H_

#include "Config.h"

#include "IndexedTexture.h"
#include "Capsyong.h"
#include "NkCharacter.h"
#include "MsgPopUp.h"
#include "ChaAct.h"
#include "Tcpipcon.h"

class LHUI_TakeBox;

#define HORIZ_BTN_X			413
//#define HORIZ_BTN_Y			375
#define HORIZ_BTN_Y			423
#define HORIZ_BTN_WIDTH		96
#define HORIZ_BTN_HEIGHT	39
#define HORIZ_BTN_GAP		110

#define SEL_GATE_UP			0
#define SEL_GATE_DOWN_ING	1
#define SEL_GATE_UP_ING		2
#define SEL_GATE_DOWN		3

#define SHEET_SND_STONE		0
#define SHEET_SND_WIND		1
#define SHEET_SND_CLICK		2
#define SHEET_SND_APP		3
#define SHEET_SND_NUM		4


#define MAX_SLOT			5

extern int g_SheetSound[];

class CLogoPage;
class CStartPage;
class CLoginPage;
class CSelectCharacterPage;
class CCreateCharacterPage;
class CSetValuePage;
class CSelectVillagePage;
class CSMRender;
class CChatPage;
class COptionPg;
class CMakeAccPage;

class CItem;
class CWeatherMgr;

class LH_SelectCharacterPage;

class CTcpIpConnection;
class CNkCharacter;
class CBiped;
class CSkyPlane;
class CSphere;

#define MAX_LOGO_TEXTR 20 // ������ �ΰ��� �����Ӽ�. ///2007�ε�����

#define SLOT_COUNT	5

#define IS_NEWLINE(c)	(c == '\n' || c == '\r')
#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

class CSlotChar
{
public:
	CSlotChar();
	~CSlotChar();

	void Clone(CSlotChar* pClone);

	char *m_Name;		// ĳ���� �̸�
	int m_Race;			// ����
	int m_Sex;			// ����
	int m_Hair;			// �Ӹ� ��Ÿ��

	int m_Level;		// ����

	int m_MaxVital;	// ����� �ִ�ġ
	int m_MaxMana;		// ������ �ִ�ġ
	int m_MaxStamina;	// ������ �ִ�ġ
	int m_MaxEpower;	// ����� �ִ�ġ

	int m_Vital;		// �����
	int m_Mana;			// ������
	int m_Stamina;		// ������
	int m_Epower;		// �����

	int m_Str;
	int m_Int;
	int m_Dex;
	int m_Con;
	int m_Cha;

	int m_BlockedTime;

	int m_Wearing[WEARING_SHOW_NUM];

	int m_GuilMa;
	int m_GuildIndex;

	int m_Support;
};

typedef enum {GAME_MENU, START_GAME} START_OPTION;


#define LOLO_INDEX 7

#define STAR_SCALE	(2.0f)
#define STAR_COUNT	100
#define STAR_START_Z	(400)
#define STAR_END_Z		(-150)
#define STAR_AREA		(150)
#define STAR_SPEED		(1.0f)

// ������ ���� �ڵ絥..���� �޴� ��� �׽�Ʈ�빮�� ���δ�.....
class CSurface;
class CLandDemo;
class CLand; ///2007�ε�����
class CFrustum; ///2007�ε�����
///////////////////////////////////////////////////////////////

class CSheet
{
public:
	//	CLogoPage				*m_pLogoPg;		// ���� ȭ��
	//	CStartPage				*m_pStartPg;	// ���� ȭ��
	CLoginPage				*m_pLoginPg;	// �α��� ȭ��
	CSelectCharacterPage	*m_pSelCharPg;  // �ɸ��� ���� ȭ��
	CCreateCharacterPage	*m_pCrtCharPg;  // �ɸ��� ����� ȭ��
	//CSetValuePage			*m_pSetValPg;	// �Ӽ����ֱ� ȭ��
	//CSelectVillagePage		*m_pSelVilPg;	// ���� ���� ȭ��
	COptionPg				*m_pOptionPg;	// �ɼ� ���� ȭ��
	LH_SelectCharacterPage * m_pSelCharPg2;


	LHUI_TakeBox * m_testui;

	LPDIRECTDRAWSURFACE7	m_pddsBackBuffer;

	CMsgPopUp	 m_GoHomePage;	// ����������� �˾�â
	CSMRender				*m_pSMRender;
	//void					RenderCsy(int slot_num, int x, int y, BOOL bSelected);
	void					SetBasicCha(int nRace, int nSex, int nHair);
	void					SetSlotCha(int idx);
	void					RenderBasicCha(int x, int y, int width, int height, BOOL bSelected = TRUE);
	void					RenderSlotCha2(int idx, int x, int y, int width, int height, BOOL bSelected = TRUE);

	CCapsyong				**m_dpChaCapsyong;
	CBiped					*m_dpBipedList[NUM_RACE*NUM_SEX];
	//IndexedTexture			*m_pChaTexture;
	void					SetChaSet(CCapsyong **dpCapsyong, IndexedTexture *pChaTexture);
	void					LoadBipedList();

	// �޴� ��� ������
	IndexedTexture			*m_pIndexedTexture;
	CLolos					*m_pLolos[LOLO_INDEX];
	CNkCharacter			*m_pCha;
	CNkCharacter			*m_pSlotCha[SLOT_COUNT];
	CNkCharacter			*m_pOpeningCha[NUM_RACE];
	int						m_nOpeningWear[NUM_RACE*CAPSYONG_NUM];

	void TestSceneRender(int);
	void TestSceneRender2();
	void TestSceneRender3();
	void TestSceneRender_back(int);
	void LoadTexture();
	void LoadTexture2();

	int m_iPageState;				// ���� ����� ������ �ִ� ����
	START_OPTION m_eStartOption;	// ���� ��Ÿ��
	CSlotChar *m_Slot[SLOT_COUNT];	// ������ ��ġ(�Ӽ���)�� ���� �������� Ŭ����
	CSlotChar m_TempSlot;

	int	m_CharsIndex[SLOT_COUNT];	// ĳ���� �ε����� �޾Ƽ� ��� ���� ������ ��ٸ��� ĳ���͸� �����ϴ� �� ���δ�.
	int m_SlotNum;					// � �ɸ����ΰ�
	int m_SelRace;					// � �����ΰ�
	int m_SelSex;					// ����? / ����?
	char m_strSvrAddr[20];			// ���� ���� �ּ�.
	int  m_nSvrPort;				// ���� ���� ��Ʈ
	int  m_nMatrixChaSvrIdx;

	BOOL	m_bEmptySloat;			/* "�α�"��ư�� �������� ���� ������� �ɸ��Ͱ� �ִ��� Ȯ���Ѵ�
									���� �ƹ��� �ɸ��͵� ����������� ���ٸ� "���ɸ��� �����"��ư��
	Ȱ��ȭ ���Ѿ��Ѵ�*/
	BOOL	m_bCreatedCha;			/*  ���ɸ��͸� ������ "�Ϸ�" ��ư�� �������°�*/
	char	m_sUserId[128];
	char	m_sUserPW[2048];			// ������ �佺���带 ��� ����..



	BOOL	m_bNeedAcct;		//���� ��ȯ �ʿ俩��
	BOOL    m_bPaidUser;        // ����� �� ������ �������� ����. ///���������

	CSheet();
	~CSheet();
	void GoPage( int x );  // �����ȯ
	START_OPTION IsStarting()
	{
		return m_eStartOption;
	};
	int GetPage()
	{
		return m_iPageState;    // ���� �����??
	}
	void GoCreateChaPage();		// ĳ���� ������ �Ҷ� �⺻ �������� �����ֱ� ���� ������.

	// CDisplay��ü�� �����͸� �޾Ƽ� Image Load �� CreateSurface�� �����Ѵ�
	void LoadRes();

	void DeleteRes();			// ��� delete��Ű��
	void Draw();					// ȭ�鿡 ���
	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM ); // ��������� MsgProc()�� �޴´�

	char *SetSvrAddr(char *str, int port);
	BOOL Connect(char *id_name, char *passwd,bool flags=true);	// �޼��� (id, password) ������	
	bool GetOneLine(char* msg, int maxSize);
	char *SendNewChar(int town_num);
	char *SendDelChar(bool& outFail, char * str = NULL);
	void StartGame(START_OPTION option);
	int UpdateChars();
	int ConfirmExist(char *name);

	// �ڵ� �α�.
	BOOL AutoLogin(int nSvrIdx, char *strUserId, char *strUserPw);


	char *AnyOneArg(char *argument, char *first_arg);
	void SkipSpaces(char **string);
	char *_StrDup(char *str);
	char m_Buf[MAX_RECV_BUF_LEN];
	char m_Buf2[1024];

	DWORD m_dwDummyBaseTime;
	DWORD GetDummyTime();
	void SendDummyMsg();

	////////////////////////////////////////////////////
	// 2002�� 10�� 1��
	// �α� ������ ���� �۾�
	//CSkyPlane		*m_pSkyPlane;	// �׸��ϴ�
	CLandDemo		*m_pLandDemo;
	CLandDemo		*m_pLandSelChar;

	// ȯ�� ����
	DWORD	m_dwAmbientLogin, m_dwAmbientSel;
	DWORD	m_dwRL, m_dwGL, m_dwBL;
	DWORD	m_dwRS, m_dwGS, m_dwBS;
	float	m_zFar, m_zNear;

	// ī�޶� ����.
	D3DVECTOR m_vEye, m_vLookAt, m_vLookAtStart, m_vLookAtEnd, m_vUpVec;
	D3DVECTOR m_vLoginLookAt, m_vSelLookAt;
	D3DVECTOR m_vStartEye, m_vEndEye, m_vEyeSelStart, m_vEyeSelEnd;
	D3DMATRIX m_matView;
	float	m_FovLogin, m_FovSel;

	void GoForward();
	void GoBackward();
	void GoLeft();
	void GoRight();
	void TurnLeft(); 	///2007�ε�����
	void TurnRight(); 	///2007�ε�����
	void TurnUp(); 	///2007�ε�����
	void TurnDown(); 	///2007�ε�����
	void UpEye();
	void DownEye();

	D3DVECTOR GetEyeDirection();

	DWORD	CombineRGB(DWORD R, DWORD G, DWORD B);

	void RenderDeco(int nStage);


	// void DisplayInfo(); ������� �ʴ� �Լ�.
	void OutputText( DWORD x, DWORD y, TCHAR* str );

	// alternative sky
	CSphere*		sky;
	CSphere*		sky_cloud;
	WORD			m_wSky, m_wSky_Cloud;
	//WORD			m_LogoTextrIndex[MAX_LOGO_TEXTR]; // �ΰ� �ؽ����� �ε��� ///2007�ε�����
	WORD			m_LogoTextrIndex; // �ΰ� �ؽ����� �ε��� // [2009/3/31 Theodoric] �ٸ��հ������ �ΰ� ����
	BOOL			m_bTwoPassSky;

	float			m_zSkyNear, m_zSkyFar, m_zLandNear, m_zLandFar;
	DWORD			m_dwFogSkyColor, m_dwFogLandColor;
	DWORD			m_dwFogSkyR, m_dwFogSkyG, m_dwFogSkyB;
	DWORD			m_dwFogLandR, m_dwFogLandG, m_dwFogLandB;

	float			m_zSkyNearLogin, m_zSkyFarLogin, m_zLandNearLogin, m_zLandFarLogin;
	DWORD			m_dwFogSkyLoginColor, m_dwFogLandLoginColor;
	DWORD			m_dwFogSkyLoginR, m_dwFogSkyLoginG, m_dwFogSkyLoginB;
	DWORD			m_dwFogLandLoginR, m_dwFogLandLoginG, m_dwFogLandLoginB;

	float			m_zSkyNearSel, m_zSkyFarSel, m_zLandNearSel, m_zLandFarSel;
	DWORD			m_dwFogSkySelColor, m_dwFogLandSelColor;
	DWORD			m_dwFogSkySelR, m_dwFogSkySelG, m_dwFogSkySelB;
	DWORD			m_dwFogLandSelR, m_dwFogLandSelG, m_dwFogLandSelB;

	void			SkyRender(float x, float y, float z);
	void			SetFog(int nStage, BOOL bFog, BOOL bSky = TRUE, float zNear = 0.0f, float zFar=1.0f);

	// select character page animation
	void			AnimateSelChar();

	// select character coordination
	D3DVECTOR		m_vCharPos[SLOT_COUNT], m_vSelCharPos;
	D3DMATRIX		m_matProjSel;
	void			RenderSlotCha(int idx, int x, int y, int width, int height, BOOL bSelected, BOOL bBlocked);


	// logine page servlist ���������� ����.
	D3DTLVERTEX TLVec[4];
	WORD		m_wBlok;
	void RenderOj(int xpos, int ypos, int cx, int cy);

	// Ȳ���� ����
	float		m_fGoldRate;

	// Ŭ���� ���÷���.
	RECT m_cliRect;
	void	BltClip(CSurface *pSurface, int dest_x, int dest_y);
	// ĳ���� �Ƿ�Ʈ �ִϸ��̼�
	BOOL	m_bSlotAniIng[SLOT_COUNT];
	DWORD	m_dwSlotAniStart[SLOT_COUNT];
	BOOL	m_bSlotOpenAni[SLOT_COUNT];
	void	AnimateGate (int nGateNum, int nState);
	void	OpenNewGate(int nGateNum);
	void	ResetAllGate();
	void	LoadSheetWav();	
	
	void	SetOpeningChar( int nIndex, int nRace, int nSex );
	void	SetOpeningCharWear();

	HRESULT	Restore();

	CWeatherMgr		*m_pWeatherMgr;
	int		m_ChaCount;
	int		m_EmptySlotCheck[SLOT_COUNT];

	bool m_bIsChangeInfo;
};

#endif // _SHEET_H_