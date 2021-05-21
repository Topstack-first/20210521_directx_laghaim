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

#define MAX_LOGO_TEXTR 20 // 오프닝 로고의 프레임수. ///2007로딩수정

#define SLOT_COUNT	5

#define IS_NEWLINE(c)	(c == '\n' || c == '\r')
#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

class CSlotChar
{
public:
	CSlotChar();
	~CSlotChar();

	void Clone(CSlotChar* pClone);

	char *m_Name;		// 캐릭터 이름
	int m_Race;			// 종족
	int m_Sex;			// 성별
	int m_Hair;			// 머리 스타일

	int m_Level;		// 레벨

	int m_MaxVital;	// 생명력 최대치
	int m_MaxMana;		// 마법력 최대치
	int m_MaxStamina;	// 지구력 최대치
	int m_MaxEpower;	// 전기력 최대치

	int m_Vital;		// 생명력
	int m_Mana;			// 마법력
	int m_Stamina;		// 지구력
	int m_Epower;		// 전기력

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

// 원래는 없던 코든데..게임 메뉴 배경 테스트대문에 쓰인다.....
class CSurface;
class CLandDemo;
class CLand; ///2007로딩변경
class CFrustum; ///2007로딩변경
///////////////////////////////////////////////////////////////

class CSheet
{
public:
	//	CLogoPage				*m_pLogoPg;		// 최초 화면
	//	CStartPage				*m_pStartPg;	// 접속 화면
	CLoginPage				*m_pLoginPg;	// 로그인 화면
	CSelectCharacterPage	*m_pSelCharPg;  // 케릭터 선택 화면
	CCreateCharacterPage	*m_pCrtCharPg;  // 케릭터 만들기 화면
	//CSetValuePage			*m_pSetValPg;	// 속성값주기 화면
	//CSelectVillagePage		*m_pSelVilPg;	// 마을 선택 화면
	COptionPg				*m_pOptionPg;	// 옵션 선택 화면
	LH_SelectCharacterPage * m_pSelCharPg2;


	LHUI_TakeBox * m_testui;

	LPDIRECTDRAWSURFACE7	m_pddsBackBuffer;

	CMsgPopUp	 m_GoHomePage;	// 새계정만들기 팝업창
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

	// 메뉴 장면 렌더링
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

	int m_iPageState;				// 현제 장면을 가지고 있는 변수
	START_OPTION m_eStartOption;	// 게임 스타팅
	CSlotChar *m_Slot[SLOT_COUNT];	// 각각의 수치(속성값)를 담을 변수들의 클레스
	CSlotChar m_TempSlot;

	int	m_CharsIndex[SLOT_COUNT];	// 캐릭터 인덱스를 받아서 길드 가입 승인을 기다리는 캐릭터를 검출하는 데 쓰인다.
	int m_SlotNum;					// 어떤 케릭터인가
	int m_SelRace;					// 어떤 종족인가
	int m_SelSex;					// 남자? / 여자?
	char m_strSvrAddr[20];			// 서버 연결 주소.
	int  m_nSvrPort;				// 서버 연결 포트
	int  m_nMatrixChaSvrIdx;

	BOOL	m_bEmptySloat;			/* "로긴"버튼이 눌렸을때 현재 만들어진 케릭터가 있는지 확인한다
									만약 아무런 케릭터도 만들어진것이 없다면 "새케릭터 만들기"버튼을
	활성화 시켜야한다*/
	BOOL	m_bCreatedCha;			/*  새케릭터를 만든후 "완료" 버튼이 눌려졌는가*/
	char	m_sUserId[128];
	char	m_sUserPW[2048];			// 유저의 페스워드를 담는 변수..



	BOOL	m_bNeedAcct;		//계정 전환 필요여부
	BOOL    m_bPaidUser;        // 브라질 쪽 결제한 유저인지 정보. ///브라질결제

	CSheet();
	~CSheet();
	void GoPage( int x );  // 장면전환
	START_OPTION IsStarting()
	{
		return m_eStartOption;
	};
	int GetPage()
	{
		return m_iPageState;    // 현재 장면은??
	}
	void GoCreateChaPage();		// 캐릭터 생성을 할때 기본 페이지를 보여주기 위한 페이지.

	// CDisplay객체의 포인터를 받아서 Image Load 및 CreateSurface를 실행한다
	void LoadRes();

	void DeleteRes();			// 장면 delete시키기
	void Draw();					// 화면에 출력
	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM ); // 현재장면의 MsgProc()를 받는다

	char *SetSvrAddr(char *str, int port);
	BOOL Connect(char *id_name, char *passwd,bool flags=true);	// 메세지 (id, password) 보내기	
	bool GetOneLine(char* msg, int maxSize);
	char *SendNewChar(int town_num);
	char *SendDelChar(bool& outFail, char * str = NULL);
	void StartGame(START_OPTION option);
	int UpdateChars();
	int ConfirmExist(char *name);

	// 자동 로긴.
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
	// 2002년 10월 1일
	// 로긴 페이지 변경 작업
	//CSkyPlane		*m_pSkyPlane;	// 네모하늘
	CLandDemo		*m_pLandDemo;
	CLandDemo		*m_pLandSelChar;

	// 환경 변수
	DWORD	m_dwAmbientLogin, m_dwAmbientSel;
	DWORD	m_dwRL, m_dwGL, m_dwBL;
	DWORD	m_dwRS, m_dwGS, m_dwBS;
	float	m_zFar, m_zNear;

	// 카메라 조정.
	D3DVECTOR m_vEye, m_vLookAt, m_vLookAtStart, m_vLookAtEnd, m_vUpVec;
	D3DVECTOR m_vLoginLookAt, m_vSelLookAt;
	D3DVECTOR m_vStartEye, m_vEndEye, m_vEyeSelStart, m_vEyeSelEnd;
	D3DMATRIX m_matView;
	float	m_FovLogin, m_FovSel;

	void GoForward();
	void GoBackward();
	void GoLeft();
	void GoRight();
	void TurnLeft(); 	///2007로딩변경
	void TurnRight(); 	///2007로딩변경
	void TurnUp(); 	///2007로딩변경
	void TurnDown(); 	///2007로딩변경
	void UpEye();
	void DownEye();

	D3DVECTOR GetEyeDirection();

	DWORD	CombineRGB(DWORD R, DWORD G, DWORD B);

	void RenderDeco(int nStage);


	// void DisplayInfo(); 사용하지 않는 함수.
	void OutputText( DWORD x, DWORD y, TCHAR* str );

	// alternative sky
	CSphere*		sky;
	CSphere*		sky_cloud;
	WORD			m_wSky, m_wSky_Cloud;
	//WORD			m_LogoTextrIndex[MAX_LOGO_TEXTR]; // 로고 텍스쳐의 인덱스 ///2007로딩변경
	WORD			m_LogoTextrIndex; // 로고 텍스쳐의 인덱스 // [2009/3/31 Theodoric] 바른손게임즈로 로고 변경
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


	// logine page servlist 반투명배경을 위한.
	D3DTLVERTEX TLVec[4];
	WORD		m_wBlok;
	void RenderOj(int xpos, int ypos, int cx, int cy);

	// 황금율 변수
	float		m_fGoldRate;

	// 클리핑 디스플레이.
	RECT m_cliRect;
	void	BltClip(CSurface *pSurface, int dest_x, int dest_y);
	// 캐릭터 실렉트 애니메이션
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