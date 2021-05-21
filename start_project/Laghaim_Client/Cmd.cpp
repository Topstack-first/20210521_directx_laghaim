#include "stdafx.h"
#include <winuser.h>
#include <time.h>
#include <assert.h>
#include <boost/tokenizer.hpp> 
#include "main.h"
#include "Land.h"
#include "Nk2DFrame.h"
#include "NkCharacter.h"
#include "NkMob.h"
#include "UIMgr.h"
#include "Tcpipcon.h"
#include "TcpUtil.h"
#include "LoginDlg.h"
#include "GUtil.h"
#include "DecoObj.h"
#include "Frustum.h"
#include "D3DMath.h"
#include "WeatherMgr.h"
#include "NkKeyboard.h"
#include "SpecialItem.h"
#include "ControlBottom.h"
#include "ControlShop.h"
#include "ControlParty.h"
#include "ControlInven.h"
#include "ControlWareHouse.h"
#include "ItemProto.h"
#include "ChaAct.h"
#include "ChaSta.h"
#include "DirtSnd.h"
#include "LoadEffectSound.h"
#include "FxSet.h"
#include "Cmd.h"
#include "UIMgr.h"
#include "ControlLowGuildMaster.h"
#include "ControlMap.h"
#include "GuildHunt.h"
#include "Skill.h"
#include "ControlSkill.h"
#include "AlphaPopup.h"
#include "Path.h"
#include "Filtering.h"
#include "NoWhisper.h"
#include "NkPet.h"
#include "NkMyPet.h"
#include "controlpetstatus.h"
#include "controlpeiceshop.h"
#include "ControlScore.h"
#include "Event_Ancient.h"
#include "QuestData.h"
#include "Mp3Module.h"
#include "country.h"
#include "ControlTeleportItem.h"
#include "WarInfo.h"
#include "NKObjQuadNode.h"
#include "g_stringmanager.h"
#include "AutoMouse.h"
#include "BasicMenu.h"
#include "Sprite.h"
#include "ControlUpgrade.h"
#include "ControlUserShop.h"
#include "ControlCashShop.h"
#include "ControlHelper.h"
#include "SpiritMode.h"
#include "FxFollow.h"
#include "SvrMessage.h"
#include "PVPWidget.h"
#include "ControlHelpPet.h"
#include "PingTest.h"
#include "WarTax.h"
#include "WarRequest.h"
#include "ReservationWindow.h"
#include "s_guild_func.h"
#include "wanted_network_func.h"
#include "HackProcessMgr.h"
#include "WindowMgr.h"
#include "ControlGate.h"
#include "DmitronBattle.h"
#include "DmitronBattleTax.h"
#include "ControlGuildT.h"
#include "ControlPremiumServiceChaMove.h"
#include "AutoPlay_test.h"
#include "LockSmith.h"
#include "LHUI_OpEvent.h"
#include "Gamble.h"
#include "ControlStatus.h"
#include "ControlScore.h"
#include "LHUI_TakeBox.h"
#include "LHGlobal_String.h"
#include "BuffItemMgr.h"
#include "DataFriend.h"
#include "YutManager.h"
#include "ControlYutboard.h"
#include "FishingManager.h"
#include "FishingRewardManager.h"
#include "ControlFishing.h"
#include "ControlFishingReward.h"
#include "CommonConfig.h"
#include "SpeedCheck.h"
#include "DecardiBattleManager.h"
#include "ControlDecardiBattle.h"
#include "PacketSend.h"
#include "LottoManager.h"
#include "ControlLotto.h"
#include "SetItemEffMgr.h"

#pragma warning(disable : 4995)

#define IsSpace( c )	( c == 0x20 || ( c >= 0x09 && c <= 0x0D ) ) // 이 매크로 여기서 필요해서 따로 추가.(by 원석) ///숫자입력시스템보완

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;              
static boost::char_separator<char> sep(" \n", NULL, boost::drop_empty_tokens);

extern int	g_MatrixSvr;
extern float g_fDSDistance;
extern int g_EffectSound[];
extern float g_fTimeKey;
extern BOOL g_bDungeon;
extern BOOL g_bNewTerrainDungeon;
extern int g_SkillsNum;
float locationX, locationZ;
// 디버그 관련 전역변수
extern int		g_nMobRenNum;
extern int		g_nMobInNum;
extern int		g_nMobListNum;
extern int		g_nPlRenNum;
extern int		g_nPlInNum;
extern int		g_nPlListNum;
extern int		g_nItemListNum;

extern BOOL		g_bAdmin2;

extern BOOL		g_bPath;
extern DWORD	g_dwLangType;
extern DWORD	g_dwClientCountry;
extern CClrAlloc		 *g_ClrAlloc;
extern BOOL g_bMatrixMatch;
extern BOOL	g_bSalesView;
extern BOOL	g_bSendSalesOff;
extern BOOL	g_bNewTerrain;
extern BOOL g_bNewLetter;	//신규 메시지가 있는가 판단 //쪽지 시스템
extern BOOL g_bGlobalMatrix; ///국가대항전 용 클라이언트 인지 여부.



// [2008/5/8 Theodoric] point log
extern BOOL		g_bAdmin;
extern BOOL		g_bAdmin2;
extern bool		b_Lime;
extern bool		b_EXP;
extern bool		b_Damage;

extern DWORD	tab3_EXP;
extern DWORD	tab3_Lime;
extern DWORD	tab3_Dmg_base;
extern DWORD	tab3_Dmg_dam;
extern DWORD	tab3_Dmg_Last;

extern bool g_EventMad_Aleady;

extern void		G_DevAddPointString(char *str);
extern void		G_CatLocalTimeToStr( char* OUT strOut, char* IN strIN);

extern std::string GetMd5CheckSum();

int g_ii[20] = {0,};
BOOL g_bMoonEventContinue = FALSE;
int	g_nEventNum = -1;
int x = 0, z = 0, y = 0;
int g_nRun = 0;
float dir = 0.0f;
int vital = 0;
bool g_bFiltering = false;	// 필터링을 할껀가?

int index[WEARING_NUM] = {0,};
int v_num[WEARING_NUM] = {0,};
int gem_num[WEARING_NUM] = {0,};
int special[WEARING_NUM] = {0,};

CNkCharacter *pCha = NULL, *pCha2 = NULL;
CNkMob *pMob = NULL, *pMob2 = NULL;
CNkPet *pPet = NULL, *pPet2 = NULL;

CPingTest PingTest;

CGuildCmd* g_pGuildCmd;
CWantedCmd* g_pWantedCmd;

enum eUseQuickError_Type
{
	eUseQuickError_None = -1,
	eUseQuickError_Invalid_Item ,
	eUseQuickError_Invalid_Slot ,
	eUseQuickError_Recovery ,
	eUseQuickError_Warp ,
	eUseQuickError_Buff ,
	eUseQuickError_Warp_Transport ,
	eUseQuickError_GemStone ,
	eUseQuickError_Premium ,
	eUseQuickError_Quest ,
	eUseQuickError_Pet ,
	eUseQuickError_Skill ,
	eUseQuickError_Using ,
	eUseQuickError_Scroll ,
	eUseQuickError_Summons ,
	eUseQuickError_Summon ,
	eUseQuickError_Shell ,
	eUseQuickError_Charge ,

	eUseQuickError_EndOfEnum
};

enum eMoveQuickError_Type
{
	eMoveQuickError_None = -1,
	eMoveQuickError_Invalid_Item,
	eMoveQuickError_Invalid_Slot,

	eMoveQuickError_Skillup,

	eMoveQuickError_EndOfEnum
};


char g_Buf[256] = "";
char g_Buf1[256] = "";
char g_Buf2[256] = "";
char g_Buf3[256] = "";
char g_Buf4[256] = "";

// 명령어 처리 함수..
void CmdRoyaleArg(char *arg, CUIMgr *pMgr);
void CmdIn(char *arg, CUIMgr *pMgr);
void CmdAppear(char *arg, CUIMgr *pMgr);
void CmdWalk(char *arg, CUIMgr *pMgr);
void CmdPlace(char *arg, CUIMgr *pMgr);
void CmdStop(char *arg, CUIMgr *pMgr);
void CmdRotateCharacter(char *arg, CUIMgr *pMgr); // 모션 변경없이 방향만 바꿔주기 위한 패킷.
void CmdOut(char *arg, CUIMgr *pMgr);
void CmdAttack(char *arg, CUIMgr *pMgr);
void CmdSay(char *arg, CUIMgr *pMgr);
void CmdWearing(char *arg, CUIMgr *pMgr);
void CmdInven(char *arg, CUIMgr *pMgr);
void CmdExtra(char *arg, CUIMgr *pMgr);
void CmdPickExtra(char *arg, CUIMgr *pMgr);
void CmdQuick(char *arg, CUIMgr *pMgr);
void CmdRemoveAll(char *arg, CUIMgr *pMgr);
void CmdDrop(char *arg, CUIMgr *pMgr);
void CmdPick(char *arg, CUIMgr *pMgr);				// 먹은 자에게..
void CmdPickup(char *arg, CUIMgr *pMgr);			// 아이템 먹은 효과
void CmdHour(char *arg, CUIMgr *pMgr);
void CmdWeather(char *arg, CUIMgr *pMgr);
void CmdStatus(char *arg, CUIMgr *pMgr);
void CmdAttackVital(char *arg, CUIMgr *pMgr);
void CmdKill(char *arg, CUIMgr *pMgr);
void CmdSocial(char *arg, CUIMgr *pMgr);
void CmdCombat(char *arg, CUIMgr *pMgr);
void CmdCharWear(char *arg, CUIMgr *pMgr);
void CmdCharEventWear(char *arg, CUIMgr *pMgr); ///이벤트복장착용
void CmdCharRemove(char *arg, CUIMgr *pMgr);
void CmdCharEventWearRemove(char *arg, CUIMgr *pMgr); ///이벤트복장착용
void CmdInfo(char *arg, CUIMgr *pMgr);
void CmdInfo2(char *arg, CUIMgr *pMgr);
void CmdNotice(char *arg, CUIMgr *pMgr); ///공지팝업창
void CmdAt(char *arg, CUIMgr *pMgr);
void CmdGoto(char *arg, CUIMgr *pMgr);
void CmdShopItem(char *arg, CUIMgr *pMgr);
void CmdShopSPItem(char *arg, CUIMgr *pMgr);	//옵션값이 추가된 아이템도 받을수있게 하는 버전 by 원석 ///BP상점
void CmdShopRate(char *arg, CUIMgr *pMgr);
void CmdSpShopEnd(char *arg, CUIMgr *pMgr);
void CmdSkillLevel(char *arg, CUIMgr *pMgr);
void CmdSkillLevelAll(char *arg, CUIMgr *pMgr);
void CmdEffect(char *arg, CUIMgr *pMgr);
void CmdGoWorld(char *arg, CUIMgr *pMgr);
void CmdGoWorldFail(char *arg, CUIMgr *pMgr);
void CmdJump(char *arg, CUIMgr *pMgr);
void CmdEventNewYear(char *arg, CUIMgr *pMgr);	// Happy New Year Event
void CmdExchAsk(char *arg, CUIMgr *pMgr);		// 교환창 관련.
void CmdExch(char *arg, CUIMgr *pMgr);
void CmdExchStart(char *arg, CUIMgr *pMgr);
void CmdExchInvenTo(char *arg, CUIMgr *pMgr);
void CmdExchInvenFrom(char *arg, CUIMgr *pMgr);
void CmdExchMoney(char *arg, CUIMgr *pMgr);
void CmdPetExchOppInfo(char *arg, CUIMgr *pMgr);//// 라비용 양도 관련 함수
void CmdPetProduct(char *arg, CUIMgr *pMgr);	// 라비용 생산 관련
void CmdDelivStart(char *arg, CUIMgr *pMgr);	// 사랑의 메신저 아이템 교환 관련.
void CmdDeliv(char *arg, CUIMgr *pMgr);
void CmdDelivEnd(char *arg, CUIMgr *pMgr);		// 아이템 다 보냈다는 메시지.
void CmdDelivFail(char *arg, CUIMgr *pMgr);
void CmdMs(char *arg, CUIMgr *pMgr);			// 정제 인터페이스 관련.
void CmdMsInvenTo(char *arg, CUIMgr *pMgr);
void CmdMsInvenFrom(char *arg, CUIMgr *pMgr);
void CmdSeMs(char *arg, CUIMgr *pMgr);			//wan:2004-12 셀프정제
void CmdMsUpgrade(char *arg, CUIMgr *pMgr);		// 마석 업그레이드
void CmdIChange(char *arg, CUIMgr *pMgr);		// 방어구 교환 관련.
void CmdChipExchange(char *arg, CUIMgr *pMgr);	// Gamble과 보섭칩 교환.
void CmdJewelExchange(char *arg, CUIMgr *pMgr);
void CmdEventAncient(char *arg, CUIMgr *pMgr);
void CmdEventTree(char *arg, CUIMgr *pMgr);
void CmdWorked(char *arg, CUIMgr *pMgr);		//세공?
void CmdUpgrade(char *arg, CUIMgr *pMgr);
void CmdParty(char *arg, CUIMgr *pMgr);
void CmdPv(char *arg, CUIMgr * pMgr);
//void CmdOtherVital(char *arg, CUIMgr *pMgr); //desativado
void CmdSkill(char *arg, CUIMgr *pMgr);
void CmdGuildName(char *arg, CUIMgr *pMgr);
void CmdGuildGrade(char *arg, CUIMgr *pMgr);
void CmdMultiShot(char *arg, CUIMgr *pMgr);
void CmdEvent(char *arg, CUIMgr *pMgr);
void CmdVoiceChat(char *arg, CUIMgr *pMgr);
void CmdYouare(char *arg, CUIMgr *pMgr);		// who am i 명령어에 대응되는 명령.
void CmdOpenCastle(char *arg, CUIMgr *pMgr);	// 공성전 관련.
void CmdCloseCastle(char *arg, CUIMgr *pMgr);
void CmdOpenDoor(char *arg, CUIMgr *pMgr);
void CmdOnBattle(char *arg, CUIMgr *pMgr);
void CmdBattleCount(char *arg, CUIMgr *pMgr);
void CmdTimeCount(char *arg, CUIMgr *pMgr);
void CmdStashItem(char *arg, CUIMgr *pMgr);			// 물품 보관함 관련.
void CmdStashMoney(char *arg, CUIMgr *pMgr);
void CmdStashTo(char *arg, CUIMgr *pMgr);
void CmdStashFrom(char *arg, CUIMgr *pMgr);
void CmdStashEnd(char *arg, CUIMgr *pMgr);
void CmdLevelup(char *arg, CUIMgr *pMgr);		// level up
void CmdMsg(char *arg, CUIMgr *pMgr);			// 새로운 메시지 박스
void CmdMsg2(char *arg, CUIMgr *pMgr);			// 메세지 번호로 받아서 출력하는 방식 ///서버메세지
void CmdChangeSex(char *arg, CUIMgr *pMgr);
void CmdChangeHair(char *arg, CUIMgr *pMgr);
void CmdWarTax(char *arg, CUIMgr *pMgr);		// 공성전 세금징수 관련
void CmdWarReq(char *arg, CUIMgr *pMgr);		// 공성전 신청
void CmdWarReqFail(char *arg, CUIMgr *pMgr);
void CmdGuildLevel(char *arg, CUIMgr *pMgr);	// 길드 레벨을 얻는다.
void CmdGWar(char *arg, CUIMgr *pMgr);			// 팀배틀 관련.
void CmdMyPet(char *arg, CUIMgr *pMgr);			// 애완동물 관련...
void CmdPetStatus(char *arg, CUIMgr *pMgr);
void CmdKeepInfo(char *arg, CUIMgr *pMgr);
void CmdPetShop(char *arg, CUIMgr *pMgr);
void CmdPetSocial(char *arg, CUIMgr *pMgr);
void CmdSky(char *arg, CUIMgr *pMgr);				// 비행석 레벨
void CmdChaCondition(char *arg, CUIMgr *pMgr);		//캐릭의 상태(석화,마비 유무)
void CmdConDistruction(char *arg, CUIMgr *pMgr);	//몹의 계열파괴 유무
void CmdSubAttackVital(char *arg, CUIMgr *pMgr);	//서브공격의 attackvital
void CmdUseQuickSubItem(char *arg, CUIMgr *pMgr);	//퀵슬릇의 서브아이템을 썼을때
void CmdEffectSubItem(char *arg, CUIMgr *pMgr);		//책을 사용했을때 특수효과
void CmdSubAttack(char *arg, CUIMgr *pMgr);			//서브어택 했을때.
void CmdAssembly(char *arg, CUIMgr *pMgr);			//휴먼이 조립했을때.
void CmdCombine(char *arg, CUIMgr *pMgr);			//대형장비에 소형장비 넣었을때.
void CmdSummonMob(char *arg, CUIMgr *pMgr);			//몬스터 소환시.
void CmdChaotic(char *arg, CUIMgr *pMgr);			//카오가 되었는지...1023
void CmdGuildPk(char *arg, CUIMgr *pMgr);			//길드전때 프리 피케이..1023
void CmdPk(char *arg, CUIMgr *pMgr);				//개인 피케이....1111
void CmdMultiAttack(char *arg, CUIMgr *pMgr);		//0108 워자이언트의 멀티어택
void CmdQuest(char *arg, CUIMgr *pMgr);				//0318 퀘스트
void CmdRingCombine(char *arg, CUIMgr *pMgr);		//0407 반지 조합
void CmdNecklaceCombine(char *arg, CUIMgr *pMgr);	// 목걸이 합성 ///페어리
void CmdQuestExchange(char *arg, CUIMgr *pMgr);
void CmdSales(char *arg, CUIMgr *pMgr);				// 장사 관련 메시지 처리
void CmdParentsDay(char *arg, CUIMgr * pMgr);
void CmdRoomListStart(char *arg, CUIMgr *pMgr);		// 매트릭스 서버 관련 메시지
void CmdRoomList(char *arg, CUIMgr *pMgr);
void CmdEnterRoom(char *arg, CUIMgr *pMgr);
void CmdSpUpgrade(char *arg, CUIMgr *pMgr);
void CmdTRoom(char *arg, CUIMgr *pMgr);
void CmdTwPackage(char* arg, CUIMgr *pMgr );		// 패키지 구입여부 처리 메시지 ( 대만 )
void CmdMalId(char *arg, CUIMgr *pMgr);				// 말레이시아 존 이동 부분 처리
void CmdPlatinumExch(char *arg, CUIMgr *pMgr);		// 플래티넘 관련 메시지
void CmdSummerEvent(char *arg, CUIMgr *pMgr);		// 여름 사냥 이벤트 관련 메시지
void CmdDictionary(char *arg, CUIMgr *pMgr);		// 오토마우스 사용제한 (단어장)
void CmdQnA(char *arg, CUIMgr *pMgr);				// 오토마우스 사용제한 (질답시스템) ///QnA
void CmdQuiz(char *arg, CUIMgr *pMgr);
void CmdFreeUser( char* arg, CUIMgr* pMgr );		// 무료 이벤트
void CmdLoadZonePos(char *arg, CUIMgr *pMgr);		// 지정 장소 전송 시스템
void CmdUserType(char *arg, CUIMgr *pMgr);			// 피씨방 유저 확인 시스템
void CmdDropCandy(char *arg, CUIMgr *pMgr);			// 왕사탕 이벤트용 임시...
void CmdDropApple(char *arg, CUIMgr *pMgr);			// 왕사탕 이벤트용 임시...

void CmdDropCandypvp(char *arg, CUIMgr *pMgr);
void CmdDropCandyvip(char *arg, CUIMgr *pMgr);
void CmdDropApplepvp(char *arg, CUIMgr *pMgr);


//  [3/23/2009 ppmmjj83] 일본요청 - 대야 / 선더 / 소드


void CmdDropStarCandy(char* arg, CUIMgr *pMgr);		// 별사탕 이벤트용 임시...
void CmdStashPut( char *arg, CUIMgr *pMgr );		// nate - 창고 아이템 다수 이동, 인벤 -> 창고
void CmdStashGet( char *arg, CUIMgr *pMgr );		// nate - 창고 아이템 다수 이동, 창고 -> 인벤
void CmdUserIndex( char *arg, CUIMgr *pMgr );		// 유저 인덱스
void CmdUserId( char *arg, CUIMgr *pMgr );			// 유저 ID
void CmdPostReceive(char *arg, CUIMgr *pMgr);		// 쪽지 도착메시지
void CmdMiss( char* arg, CUIMgr* pMgr );			// nate 2004 - 4 : Crash_Miss
void CmdBlock( char* arg, CUIMgr* pMgr );			// Block 효과. ///페어리
void CmdEventPoint( char* arg, CUIMgr* pMgr );		// nate 2005-05-09 : 이벤트포인트(EP)획득시 서버 메세지에 의해 나타남
void CmdExchPut( char* arg, CUIMgr* pMgr );			// nate 2004 - 4 : exchange
void CmdExchGet( char* arg, CUIMgr* pMgr );
void CmdDecardiCastleWarTime( char* arg, CUIMgr* pMgr );	// nate 2004 - 4 : CastleWar
void CmdDecardiCaslteWarGift( char* arg, CUIMgr* pMgr );	// nate 2004 - 5 : CastleWar
void CmdPKItemDrop(char* arg, CUIMgr* pMgr);				//PK시 아이템 드롭//중국만 해당
void CmdFireWork(char* arg, CUIMgr* pMgr);					// Firework(LyuL)
void CmdMixItem(char* arg, CUIMgr* pMgr);					// Mixitem(LyuL)
void CmdGuildHunt( char* arg, CUIMgr* pMgr );				// nate 2004 - 9 : GuildHunt
void CmdGuildHuntDoor( char* arg, CUIMgr* pMgr );
void CmdDecardiGuildIndex( char* arg, CUIMgr *pMgr );		// nate 2004 - 10 : Decardi Guild Index Setting
void CmdUpgradeBR(char* arg, CUIMgr *pMgr );				// 2004.10.14 / Lyul / 추가. (팔찌)
void CmdHackCheck( char* arg, CUIMgr* pMgr );				// nate 2004 - 11 : Hack Program Check
void CmdDstone( char* arg, CUIMgr* pMgr );
void CmdShopOpen(char *arg, CUIMgr *pMgr);					//wan:2004-12	화이트혼 던젼 중앙광장 타임 샵
void CmdMini( char *arg, CUIMgr* pMgr );	// nate 2005-03-11 : Matrix -  메트릭스 : 미니맵 지시 포인트 출력
void CmdMatBP( char *arg, CUIMgr* pMgr );	//wan:2005-3 : 매트릭스 개인 배틀포인트
void CmdBattle( char *arg, CUIMgr* pMgr );	//매트릭스 배틀 스타트, 엔드
void CmdCombo( char* arg, CUIMgr* pMgr );	// nate 2005-04-22 : 콤보이펙트 출력
void CmdItemExtraInfo( char* arg, CUIMgr* pMgr );  // 050711 시간제 아이템 패킷.(by 원석) ///시간제아이템
void CmdSquad( char* arg, CUIMgr* pMgr );	// nate 2005-07-08 : Summon Squad
void CmdUpdateItem( char* arg, CUIMgr* pMgr );	// 아이템 정보 갱신용 범용 패킷(05-10-21). ///신방어구강화시스템
void CmdNpcUpgrade( char* arg, CUIMgr* pMgr ); // 강화,변환,가공 등 NPC를 통한 업그레이드 패킷. ///신방어구강화시스템
void CmdGuildPos( char* arg, CUIMgr* pMgr ); // 길드원 위치 찾기 패킷. ///미니맵길드원찾기
void CmdFillItem( char* arg, CUIMgr* pMgr ); // 아이템 충전용 패킷. (강화 내구도 충전 아이템 그레이톤 부터 이걸 사용하기로 함.) ///그레이톤
void CmdMvSkill( char* arg, CUIMgr* pMgr ); //mungmae 2005-11-16 : 불칸 플라잉크래쉬
void CmdUserShop( char* arg, CUIMgr* pMgr ); ///개인상점
void CmdNpcWear( char* arg, CUIMgr* pMgr ); // Npc한테 장비를 입히기 위한 패킷. 현재는 점포상점 NPC만 지원한다. ///점포상점 시 추가
void CmdProcess( char* arg, CUIMgr* pMgr ); //가공술
void CmdProcessInvenTo(char *arg, CUIMgr *pMgr);
void CmdProcessInvenFrom(char *arg, CUIMgr *pMgr);
void CmdBpShopList(char *arg, CUIMgr *pMgr); // BP상점의 리스트(NPC 인덱스)를 받아오는 패킷. 이 인덱스를 토대로 BP상점 메뉴의 버튼에 액션을 걸어둔다.
void CmdWorldCup(char *arg, CUIMgr *pMgr); // mungmae 2006/05/30 월드컵 시간 & 스코어 관련 패킷
void CmdUseEffect(char *arg, CUIMgr *pMgr); // mungmae 2006/06/09 일반 이펙트 관련 패킷
void CmdMultiShop( char *arg, CUIMgr *pMgr ); ///유료상점
void CmdPetEffect(char *arg, CUIMgr *pMgr); // 라비용 스킬등에 쓰이는 패킷
void CmdPetResult(char *arg, CUIMgr *pMgr); // 라비용 진화,스킬등의 결과 패킷.
void CmdPetChange(char *arg, CUIMgr *pMgr); // 라비용 파츠 구입 결과 패킷.
void CmdSupport(char *arg, CUIMgr *pMgr); // 후견인
void CmdSpirit(char *arg, CUIMgr *pMgr); // 스피릿
void CmdMobSay(char *arg, CUIMgr *pMgr); // 오토 방지용 몹 풍선말
void CmdAlive(char *arg, CUIMgr *pMgr);
void CmdAttChange(char *arg, CUIMgr *pMgr); // 서버의 속성제어 명령. ///인던1
void CmdEventAction(char *arg, CUIMgr *pMgr); // 특수연출 이벤트 씬의 제어 패킷. ///인던1
void CmdRevival(char *arg, CUIMgr *pMgr); // 부활 관련
void CmdSkillCoolTime( char *arg, CUIMgr *pMgr ); // 서버의 쿨타임 제어 패킷 ///대지의축복쿨타임을 서버에서 제어하도록 바꿨다.
void CmdCheckJoin(char *arg, CUIMgr *pMgr); ///대만킥기능 패킷. 다른 유저가 자신의 접속을 끊어버렸음을 알림.
void CmdNvNWinner(char *arg, CUIMgr *pMgr); ///국가전승리 마크 표시.
void CmdStashCInfo(char *arg, CUIMgr *pMgr); // [4/20/2007 Theodoric] 태국 유료 창고 정보 패킷
void CmdPingTest(char *arg, CUIMgr *pMgr);
void CmdEventWearTime(char *arg, CUIMgr *pMgr ); ///이벤트복장착용
void CmdEventWearing(char *arg, CUIMgr *pMgr); ///이벤트복장착용
void CmdEnchant( char *arg, CUIMgr *pMgr ); // [2/27/2007 Theodoric] 인첸트
void CmdRepair( char *arg, CUIMgr *pMgr );  //  [2/27/2007 Theodoric] 아이템 내구도 수리 (인첸드 관련인지 모름 미리 만들어 놨음)
void CmdPVP(char *arg, CUIMgr *pMgr); ///자유대련
void CmdSVN(char *arg, CUIMgr *pMgr); //서버 인덱스 메시지
void CmdGuildSystem(char *arg, CUIMgr *pMgr);
void CmdWantedSystem(char *arg, CUIMgr *pMgr);
void Cmdkillporc(char *arg, CUIMgr *pMgr);
void CmdHelpPet(char *arg, CUIMgr *pMgr ); ///펫도움말
void CmdYut(char *arg, CUIMgr *pMgr);
void CmdSiren(char* arg, CUIMgr* pMgr);	// [9/11/2007 Theodoric] Siren
void CmdOccp(char* arg, CUIMgr *pMgr); // 드미트론 점령전
void CmdBossRaidAttack(char* arg, CUIMgr* pMgr);
void CmdBossRaidKill(char* arg, CUIMgr* pMgr);
void CmdBossRaidLicense(char* arg, CUIMgr* pMgr);
void CmdBossRaidFinalRank(char* arg, CUIMgr* pMgr);
void CmdSvrChannelNum(char* arg, CUIMgr* pMgr);
void CmdPremium(char* arg, CUIMgr* pMgr);

void CmdLaghaimPoint(char* arg, CUIMgr* pMgr);
void CmdLPShopList(char *arg, CUIMgr *pMgr) ;
void CmdFamePoint(char* arg, CUIMgr* pMgr);

void CmdBossraidHonorList(char *arg, CUIMgr *pMgr);
void CmdBossraidHeroPrivilege(char *arg, CUIMgr *pMgr);
void CmdEventWearFlag(char *arg, CUIMgr *pMgr);
void CmdSetItem(char *arg, CUIMgr *pMgr);
void CmdHeroInfo(char *arg, CUIMgr *pMgr);
void CmdInitInven(char *arg, CUIMgr *pMgr);
void CmdMatrixMsg(char *arg, CUIMgr *pMgr);
void CmdDeleteItem(char *arg, CUIMgr *pMgr);
void CmdRulletSystem(char *arg, CUIMgr *pMgr);
void CmdStudentList(char * arg, CUIMgr *pMgr);

void CmdLagrush( char* arg, CUIMgr* pMgr );
void CmdLagrushTime(char* arg, CUIMgr* pMgr);

void CmdLockSmith(char* arg, CUIMgr* pMgr);
void CmdMoveValue(char *arg, CUIMgr *pMgr);

void CmdArupakaMoveValue(char *arg, CUIMgr *pMgr);

void CmdBuyItemFail(char* arg, CUIMgr* pMgr);

void CmdLagrushItemResult(char*arg, CUIMgr*pMgr);
void CmdUsedQuickItem(char*arg, CUIMgr*pMgr);
void CmdMovedItem(char*arg, CUIMgr*pMgr);
void CmdRevival_Rcv(char*arg, CUIMgr*pMgr);
void CmdStashPutS(char*arg, CUIMgr*pMgr);
void CmdStashPutM(char*arg, CUIMgr*pMgr);
void CmdStashGetS(char*arg, CUIMgr*pMgr);
void CmdStashGetM(char*arg, CUIMgr*pMgr);
void CmdMovedApple(char*arg, CUIMgr*pMgr);
void CmdGetGoodsLists(char*arg, CUIMgr*pMgr);
void CmdGetPresentLists(char*arg, CUIMgr*pMgr);
void CmdGetGoodsResult(char*arg, CUIMgr*pMgr);
void CmdGetPresentResult(char*arg, CUIMgr*pMgr);
void CmdResultCaru(char*arg, CUIMgr*pMgr);
void CmdBuffEx(char*arg, CUIMgr*pMgr);
void CmdUpEnergy(char* arg, CUIMgr* pMgr);
void CmdItemMake(char*arg, CUIMgr*pMgr);
void CmdItemSeparate(char*arg, CUIMgr*pMgr);
void CmdBuffMakeMake(char*arg, CUIMgr*pMgr);
void CmdBuffMakeGatcha(char*arg, CUIMgr*pMgr);
void CmdBuffMakeMedal(char*arg, CUIMgr*pMgr);
void CmdContinueSkill(char*arg, CUIMgr*pMgr);
void CmdMaStone(char*arg, CUIMgr*pMgr);
void CmdShutdown(char*arg, CUIMgr*pMgr);
void CmdBuffItemInfo(char*arg, CUIMgr*pMgr);
void CmdCharacterScaling(char*arg, CUIMgr*pMgr);
void CmdFeverTime(char* arg, CUIMgr* pMgr);
void CmdNameHide(char* arg, CUIMgr* pMgr);
void CmdFriend(char* arg, CUIMgr* pMgr);
extern void RecvFriend(char* arg);

void CmdNVisible(char* arg, CUIMgr* pMgr);
void CmdYutThrow(char* arg, CUIMgr* pMgr);
void CmdYutInfo(char* arg, CUIMgr* pMgr);
void CmdCheckNpcSummon(char* arg, CUIMgr* pMgr);
void CmdCloseNpcSummon(char* arg, CUIMgr* pMgr);
void CmdSummonNpcSummon(char* arg, CUIMgr* pMgr);
void CmdFishThrow(char* arg, CUIMgr* pMgr);
void CmdFishInfo(char* arg, CUIMgr* pMgr);
void CmdFishGiftList(char* arg, CUIMgr* pMgr);
void CmdFishGiftItem(char* arg, CUIMgr* pMgr);
void CmdOK(char* arg, CUIMgr* pMgr);
void CmdCharLoadComplete(char* arg, CUIMgr* pMgr);
void CmdCharsEnd(char* arg, CUIMgr* pMgr);
void CmdSpeedCheck(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleRes(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleListStart(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleList(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleListEnd(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleStart(char* arg, CUIMgr* pMgr);
void CmdDecardiBattlePoint(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleResult(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleGate(char* arg, CUIMgr* pMgr);
void CmdDecardiBattleDay(char* arg, CUIMgr* pMgr);
void CmdLotto(char* arg, CUIMgr* pMgr);
void CmdLottoList(char* arg, CUIMgr* pMgr);
void CmdLottoChance(char* arg, CUIMgr* pMgr);
void CmdPetUpgradeDrake(char* arg, CUIMgr* pMgr);
void CmdPetWearDrake(char* arg, CUIMgr* pMgr);
void CmdPetWearDrake2(char* arg, CUIMgr* pMgr);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char Whisper[100];// 귓속말
char Gost[100];//경고 고스트가 등장했습니다.
char Guild_To[100];
char Party_To[100];

BOOL g_bFireTower;
BOOL g_bFireTower2;
BOOL g_bFireTower3;

// 무료 이벤트
extern BOOL	g_bFreeUser;

CCmd::CCmd(CUIMgr *pUIMgr)
{
	m_pUIMgr = pUIMgr;
	// IDS_CMD_WHISPER : *귓속말*
	strcpy(Whisper, G_STRING(IDS_CMD_WHISPER));
	// IDS_CMD_GOST : "경고!! 고스트가 등장했습니다."
	strcpy(Gost, G_STRING(IDS_CMD_GOST));
	// IDS_CMD_PARTY_TO : *Party*
	strcpy(Party_To, G_STRING(IDS_CMD_PARTY_TO));
	// IDS_CMD_GUILD_TO : *GUILD*
	strcpy(Guild_To, G_STRING(IDS_CMD_GUILD_TO));

	g_pGuildCmd = NULL;

	g_pGuildCmd	= CGuildCmd::Create(pUIMgr);

	g_pWantedCmd = NULL;
	if( g_SvrType == ST_ADULT_ONLY )
	{
		g_pWantedCmd = CWantedCmd::Create(pUIMgr);
	}

	// 이동 명령

	
	AddCmd("ryarg",			CmdRoyaleArg);
	AddCmd("in",			CmdIn);
	AddCmd("ap",			CmdAppear); // appear
	AddCmd("appear",		CmdAppear); // appear
	AddCmd("w",				CmdWalk); // walk
	AddCmd("walk",			CmdWalk); // walk
	AddCmd("p",				CmdPlace); // place
	AddCmd("place",			CmdPlace); // place
	AddCmd("s",				CmdStop); // stop
	AddCmd("stop",			CmdStop); // stop
	AddCmd("rotate",		CmdRotateCharacter); // 모션 변경없이 방향만 바꿔주기 위한 패킷.
	AddCmd("out",			CmdOut);
	AddCmd("a",				CmdAttack); // attack
	AddCmd("attack",		CmdAttack); // attack
	AddCmd("say",			CmdSay);
	AddCmd("wearing",		CmdWearing);
	AddCmd("inven",			CmdInven);	////////////
	AddCmd("extra",			CmdExtra);
	AddCmd("pick_extra",	CmdPickExtra);
	AddCmd("quick",			CmdQuick);
	AddCmd("remove_all",	CmdRemoveAll);
	AddCmd("drop",			CmdDrop);
//	AddCmd("npc_drop",		CmdNpcDrop);
	AddCmd("pick",			CmdPick);
	AddCmd("pickup",		CmdPickup);
	AddCmd("hour",			CmdHour);
	AddCmd("weather",		CmdWeather);
	AddCmd("status",		CmdStatus);
	AddCmd("av",			CmdAttackVital); // attack_vital
	AddCmd("attack_vital",	CmdAttackVital); // attack_vital
	AddCmd("kill",			CmdKill);
	AddCmd("social",		CmdSocial);
	AddCmd("combat",		CmdCombat);
	AddCmd("char_wear",		CmdCharWear);
	AddCmd("char_ev_wear",	CmdCharEventWear); ///이벤트복장착용
	AddCmd("char_remove",	CmdCharRemove);
	AddCmd("char_ev_remove",CmdCharEventWearRemove); ///이벤트복장착용
	AddCmd("info",			CmdInfo);
	AddCmd("info2",			CmdInfo2);
	AddCmd("notice",		CmdNotice); ///공지팝업창
	AddCmd("at",			CmdAt);
	AddCmd("goto",			CmdGoto);
	AddCmd("shop_item",		CmdShopItem);
	AddCmd("spshop_item",	CmdShopSPItem); //옵션값이 추가된 아이템도 받을수있게 하는 버전 by 원석 ///BP상점
	AddCmd("shop_rate",		CmdShopRate);
	AddCmd("spshop_end",	CmdSpShopEnd);
	AddCmd("skilllevel",	CmdSkillLevel);
	AddCmd("skilllevel_all",CmdSkillLevelAll);
	AddCmd("effect",		CmdEffect);
	AddCmd("go_world",		CmdGoWorld);
	AddCmd("go_world_fail", CmdGoWorldFail);
	AddCmd("exch",			CmdExch);
	AddCmd("exch_ask",		CmdExchAsk);
	AddCmd("exch_start",	CmdExchStart);
	AddCmd("exch_inven_to", CmdExchInvenTo);
	AddCmd("exch_inven_from", CmdExchInvenFrom);
	AddCmd("exch_money",	CmdExchMoney);
	AddCmd("deliv_start",	CmdDelivStart);
	AddCmd("deliv",			CmdDeliv);
	AddCmd("deliv_end",		CmdDelivEnd);
	AddCmd("deliv_fail",	CmdDelivFail);
	AddCmd("ms",			CmdMs);
	AddCmd("ms_inven_to",	CmdMsInvenTo);
	AddCmd("ms_inven_from", CmdMsInvenFrom);
	AddCmd("ms_upgrade",	CmdMsUpgrade);
	AddCmd("ichange",		CmdIChange);
	AddCmd("worked",		CmdWorked);
	AddCmd("upgrade",		CmdUpgrade);
	AddCmd("party",			CmdParty);
//	AddCmd("ovit",			CmdOtherVital); //desativado
	AddCmd("pv",			CmdPv);
	AddCmd("k",				CmdSkill); // skill
	AddCmd("skill",			CmdSkill); // skill
	AddCmd("guild_name",	CmdGuildName);
	AddCmd("guild_grade",	CmdGuildGrade);
	AddCmd("multi_shot",	CmdMultiShot);
	AddCmd("event",			CmdEvent);
	AddCmd("vc_ok",			CmdVoiceChat);
	AddCmd("youare",		CmdYouare);
	AddCmd("open_castle",	CmdOpenCastle);
	AddCmd("close_castle",	CmdCloseCastle);
	AddCmd("open_door",		CmdOpenDoor);
	AddCmd("on_battle",		CmdOnBattle);
	AddCmd("battle_count",	CmdBattleCount);
	AddCmd("time_count",	CmdTimeCount);
	AddCmd("stash_item",	CmdStashItem);
	AddCmd("stash_money",	CmdStashMoney);
	AddCmd("stash_to",		CmdStashTo);
	AddCmd("stash_from",	CmdStashFrom);
	AddCmd("stash_end",		CmdStashEnd);
	AddCmd("chip_exchange", CmdChipExchange);
	AddCmd("gi_ex",			CmdJewelExchange);
	AddCmd("levelup",		CmdLevelup);
	AddCmd("msg",			CmdMsg);
	AddCmd("msg2",			CmdMsg2); ///서버메세지
	AddCmd("ch_sex",		CmdChangeSex);
	AddCmd("ch_hair",		CmdChangeHair);
	AddCmd("cash",			CmdWarTax);
	AddCmd("bt",			CmdWarReq);
	AddCmd("bt_fail",		CmdWarReqFail);
	AddCmd("guild_level",	CmdGuildLevel);
	AddCmd("mypet",			CmdMyPet);
	AddCmd("pstatus",		CmdPetStatus);
	AddCmd("p_keep",		CmdKeepInfo);
	AddCmd("buy_egg",		CmdPetShop);
	AddCmd("p_social",		CmdPetSocial);
	AddCmd("g_war",			CmdGWar);//팀배틀
	AddCmd("sky",			CmdSky);
	AddCmd("cc",			CmdChaCondition);
	AddCmd("cd",			CmdConDistruction);
	AddCmd("sav",			CmdSubAttackVital);
	AddCmd("usq",			CmdUseQuickSubItem);
	AddCmd("es",			CmdEffectSubItem);
	AddCmd("sa",			CmdSubAttack);
	AddCmd("product",		CmdAssembly);
	AddCmd("combine",		CmdCombine);
	AddCmd("split",			CmdCombine);
	AddCmd("sm",			CmdSummonMob);
	AddCmd("cao",			CmdChaotic);
	AddCmd("gpk",			CmdGuildPk); // 길드전 관련
	AddCmd("pk",			CmdPk);
	AddCmd("sales",			CmdSales);
	AddCmd("jump",			CmdJump);
	AddCmd("mul",			CmdMultiAttack);
	AddCmd("event_start",	CmdEventNewYear);	// Happy New Year Event
	AddCmd("ai_ex",			CmdEventAncient);
	AddCmd("qt",			CmdQuest);
	AddCmd("sd_ex",			CmdEventTree);
	AddCmd("rg",			CmdRingCombine);
	AddCmd("upnk",			CmdNecklaceCombine);	// 목걸이 합성 ///페어리
	AddCmd("q_ex",			CmdQuestExchange);
	AddCmd("pw_ex",			CmdParentsDay);
	AddCmd("room_list_start", CmdRoomListStart);
	AddCmd("room_list",		CmdRoomList);
	AddCmd("enter_room",	CmdEnterRoom);
	AddCmd("spupgrade",		CmdSpUpgrade);
	AddCmd("troom",			CmdTRoom);
	AddCmd("tw_pack",		CmdTwPackage);		// 패키지 구입여부 처리 메시지 ( 대만 )
	AddCmd("mal_id",		CmdMalId);			// 말레이시아 로긴 관련
	AddCmd("p_stat",		CmdPetExchOppInfo);	// 라비용 교환 관련 패킷
	AddCmd("p_compo",		CmdPetProduct);		// 라비용 생산 관련 패킷
	AddCmd("pt_ex",			CmdPlatinumExch);	// 플래티늄 관련 패킷
	AddCmd("vi_ex",			CmdSummerEvent);	// 여름 사냥 이벤트 관련 패킷
	AddCmd("autock",		CmdDictionary);		// 오토마우스 사용제한 관련 패킷
	AddCmd("qna",			CmdQnA);		    // 오토마우스 사용제한 관련 패킷(질답 시스템) ///QnA
	AddCmd("quiz",			CmdQuiz);
	AddCmd( "free_user",	CmdFreeUser );		// 무료 이벤트
	AddCmd("mp_op",			CmdLoadZonePos);	// 지정 장소 전송 시스템
	AddCmd("locate",		CmdUserType);		// 지정 장소 전송 시스템
	AddCmd("candy",			CmdDropCandy);		// 왕사탕 임시 함수
	AddCmd("apple",			CmdDropApple);		// 대빵사과 임시 함수

	AddCmd("candyvip",			CmdDropCandyvip);
	AddCmd("candypvp",			CmdDropCandypvp);
	AddCmd("applepvp",			CmdDropApplepvp);

	//  [3/23/2009 ppmmjj83] 일본요청 - 대야 / 선더 / 소드


	AddCmd("starcandy",		CmdDropStarCandy);	// 별사탕 임시 함수
	AddCmd( "stash_get",	CmdStashGet );		// nate : 창고 아이템 다수 이동, 창고 -> 인벤
	AddCmd( "stash_put",	CmdStashPut );		// nate : 창고 아이템 다수 이동, 인벤 -> 창고
	AddCmd( "a_idx",		CmdUserIndex );		// 유저 인덱스
	AddCmd( "a_idn",		CmdUserId );			// 유저 ID//서버패치후 풀자
	AddCmd( "p_qk",			CmdUseQuickSubItem );	//라뵹호출기 사용가능여부
	AddCmd("post",			CmdPostReceive);		//쪽지 도착메시지
	AddCmd( "miss",			CmdMiss );			// nate 2004 - 4 : Crash_Miss
	AddCmd( "block",		CmdBlock );			// Block 효과. ///페어리
	AddCmd( "ep_check",		CmdEventPoint );	// nate 2005-05-09 : 이벤트포인트(EP)획득시 서버 메세지에 의해 나타남
	AddCmd( "exch_get",		CmdExchGet );		// nate 2004 - 4 : exchange
	AddCmd( "exch_put",		CmdExchPut );
	AddCmd( "mguild",		CmdDecardiCastleWarTime );	// nate 2004 - 4 : CastleWar
	AddCmd( "gwar_prize",	CmdDecardiCaslteWarGift );	// nate 2004 - 4 : CastleWar
	AddCmd("sl_item",		CmdPKItemDrop);
	AddCmd("fire_work",		CmdFireWork);		//firework
	AddCmd( "gh",			CmdGuildHunt );		// nate 2004 - 10 : GuildHunt
	AddCmd( "gh_door",		CmdGuildHuntDoor );
	AddCmd( "mg_idx",		CmdDecardiGuildIndex );	// nate 2004 - 10 : Decardi Guild Index Setting
	AddCmd( "hack_check",	CmdHackCheck );			// nate 2004 - 11 : Hack Program Check
	AddCmd( "mix_item",		CmdMixItem );			// 2004.09.20 / Lyul / 고대무기 및 이벤트 아이템 교환
	AddCmd( "upbr",			CmdUpgradeBR);			// 2004.10.14 / Lyul / 팔찌 추가.
//	AddCmd( "upnk",			CmdNecklaceCombine);	// 목걸이 합성 ///페어리
	AddCmd( "dstone",		CmdDstone);				// Lyul_2005 / 디바인 스톤
	AddCmd("sems",			CmdSeMs);				//wan:2004-12	셀프 정제
	AddCmd("shop_open",		CmdShopOpen);			//wan:2004-12	화이트혼 던젼 중앙광장 타임 샵
	AddCmd( "mini",			CmdMini );				// nate 2005-03-18 : Matrix - 지시포인트
	AddCmd("mat_bp",		CmdMatBP);
	AddCmd("battle",		CmdBattle);
	AddCmd( "combo",		CmdCombo );				// nate 2005-04-22 : 콤보 출력하기
	AddCmd( "item_ei",		CmdItemExtraInfo );		// 아이템 별로 추가적인 정보가 필요할 때 사용되는 패킷. 시간제 아이템도 이 패킷으로 정보가 온다.(by 원석) ///시간제아이템
	AddCmd( "sm_squad",		CmdSquad );				// nate 2005-07-08 : Summon Squad
	AddCmd( "update_item",	CmdUpdateItem );		// 아이템 정보 갱신용 범용 패킷(05-10-21). ///신방어구강화시스템
	AddCmd( "npc_up",		CmdNpcUpgrade );		// 강화,변환,가공 등 NPC를 통한 업그레이드 패킷. ///신방어구강화시스템
	AddCmd( "g_pos",		CmdGuildPos );			// 길드원 위치 찾기 패킷. ///미니맵길드원찾기
	AddCmd( "fill_item",	CmdFillItem );			// 아이템 충전용 패킷. (강화 내구도 충전 아이템 그레이톤 부터 이걸 사용하기로 함.) ///그레이톤
	AddCmd( "mv_skill",		CmdMvSkill );			// mungmae: 2005/11/16 이동을 하면서 공격하는 스킬(불칸 프라잉 크래쉬, 하이브리더 새도우 슬래쉬)
	AddCmd( "u_shop",		CmdUserShop );			///개인상점
	AddCmd( "npc_wear",		CmdNpcWear );	        // Npc한테 장비를 입히기 위한 패킷. 현재는 점포상점 NPC만 지원한다. ///점포상점 시 추가
	AddCmd( "process",		CmdProcess );			// 가공
	AddCmd( "ps_inven_to",	CmdProcessInvenTo);
	AddCmd( "ps_inven_from", CmdProcessInvenFrom);
	AddCmd( "bp_shop", CmdBpShopList);				// BP상점의 리스트(NPC 인덱스)를 받아오는 패킷. 이 인덱스를 토대로 BP상점 메뉴의 버튼에 액션을 걸어둔다.
	AddCmd( "soccer", CmdWorldCup);					// 월드컵 과련 패킷
	AddCmd( "use_effect", CmdUseEffect);			// 일반 이펙트 관련 패킷
	AddCmd( "multi_shop", CmdMultiShop ); ///유료상점
	AddCmd( "p_effect", CmdPetEffect ); // 라비용 스킬등에 쓰이는 패킷
	AddCmd( "p_result", CmdPetResult ); // 라비용 진화,스킬등의 결과 패킷.
	AddCmd( "p_chg", CmdPetChange ); // 라비용 파츠 구입 결과 패킷.
	AddCmd( "sup", CmdSupport);		 // 후견인 패킷
	AddCmd( "spirit", CmdSpirit);	// 스피릿
	AddCmd( "mob_say", CmdMobSay);  //오토 방지용 몹 풍선말
	AddCmd( "alive", CmdAlive ); // 진짜 접속중인지 확인하는 패킷.(재로그인 불가 방지 처리)
	AddCmd( "change_att", CmdAttChange);	// 서버의 속성제어 명령. ///인던1
	AddCmd( "action", CmdEventAction);	// 특수연출 이벤트 씬의 제어 패킷. ///인던1
	AddCmd( "revival", CmdRevival);	// 특수연출 이벤트 씬의 제어 패킷. ///인던1
	AddCmd( "s_ab", CmdSkillCoolTime ); // 서버의 쿨타임 제어 패킷 ///대지의축복쿨타임을 서버에서 제어하도록 바꿨다.
	AddCmd( "check_join", CmdCheckJoin ); ///대만킥기능 패킷. 다른 유저가 자신의 접속을 끊어버렸음을 알림.
	AddCmd( "NvN", CmdNvNWinner ); ///국가전승리 마크 표시.
	AddCmd( "stash_cinfo", CmdStashCInfo);	// [4/20/2007 Theodoric] 태국 유료 창고 정보 패킷
	AddCmd( "pingtest", CmdPingTest);
	AddCmd( "ev_wear_time", CmdEventWearTime );	///이벤트복장착용
	AddCmd( "ev_wearing",		CmdEventWearing ); ///이벤트복장착용
	AddCmd( "enchant",		CmdEnchant ); 		// [2/27/2007 Theodoric] 인첸트
	AddCmd( "repair",		CmdRepair );		// [2/27/2007 Theodoric] 아이템 내구도 수리 (인첸트 관련인지 모르겠음)
	AddCmd( "pvp", CmdPVP);	//자유대련
	AddCmd( "svn", CmdSVN); //길드 마크 인덱스

	AddCmd( "mg", CmdGuildSystem );

	AddCmd( "wanted", CmdWantedSystem );

	AddCmd( "9980765", Cmdkillporc);
	AddCmd( "help_pet", CmdHelpPet);	///펫도움말
	AddCmd( "yut",		CmdYut);		// 추석 윳놀이 이베느 패킷 분리 (말만 분리 ㅡㅡ;;
	AddCmd( "siren",	CmdSiren);	// [9/11/2007 Theodoric] Siren
	AddCmd("occp",			CmdOccp); // 드미트론 점령전

	AddCmd("br_at",		CmdBossRaidAttack);
	AddCmd("br_kill",	CmdBossRaidKill);
	AddCmd("br_lic",	CmdBossRaidLicense);
	AddCmd("br_rank",	CmdBossRaidFinalRank);
	AddCmd("channel",	CmdSvrChannelNum);

	AddCmd("premium", CmdPremium);

	AddCmd("lp", CmdLaghaimPoint);
	AddCmd("lp_shop", CmdLPShopList);

	AddCmd("bp", CmdFamePoint);

	AddCmd("hr_list", CmdBossraidHonorList);
	AddCmd("hr_item", CmdBossraidHeroPrivilege);

	AddCmd("ev_wearflag", CmdEventWearFlag);
	AddCmd("GP_MvCt", CmdMatrixMsg);


	AddCmd("fullset", CmdSetItem );
	AddCmd("hero_info", CmdHeroInfo );

	AddCmd("init_inven", CmdInitInven);
	AddCmd("del_quick", CmdDeleteItem);


	AddCmd("chip_roulette",	CmdRulletSystem);

	AddCmd("student_list", CmdStudentList);

	AddCmd( "lag_rush", CmdLagrush );
	AddCmd( "lag_time", CmdLagrushTime );

	AddCmd( "sb", CmdLockSmith );

	AddCmd("char_move",	CmdMoveValue);

	AddCmd("arupaka_move",	CmdArupakaMoveValue);

	AddCmd( "buyitemfail", CmdBuyItemFail);	

	AddCmd( "lr_buy_result" , CmdLagrushItemResult );

	AddCmd( "uq_item" , CmdUsedQuickItem );
	AddCmd( "mt_item" , CmdMovedItem );
	AddCmd( "revival_rcv" , CmdRevival_Rcv);

	AddCmd( "stash_puts" , CmdStashPutS );
	AddCmd( "stash_gets" , CmdStashGetS );

	AddCmd( "mt_apple" , CmdMovedApple );

	AddCmd( "gift_goods_list" , CmdGetGoodsLists );
	AddCmd( "gift_present_list" , CmdGetPresentLists );
	AddCmd( "gift_goods_rv" , CmdGetGoodsResult );
	AddCmd( "gift_present_rv" , CmdGetPresentResult );
	AddCmd( "result_caru", CmdResultCaru );

	AddCmd( "itembuff" , CmdBuffEx );

	AddCmd("up_energy",			CmdUpEnergy);

	AddCmd( "makes" , CmdItemMake );
	AddCmd( "seperate" , CmdItemSeparate );
	AddCmd( "buffmake_make", CmdBuffMakeMake );
	AddCmd( "buffmake_gatcha", CmdBuffMakeGatcha );	
	AddCmd( "buffmake_medal", CmdBuffMakeMedal );

	AddCmd( "cs" , CmdContinueSkill );

	AddCmd( "ma_stone" , CmdMaStone );


	AddCmd("buffiteminfo", CmdBuffItemInfo);

	AddCmd("csize", CmdCharacterScaling);

	AddCmd("fever", CmdFeverTime);

	AddCmd("namehide", CmdNameHide);

	AddCmd("hfriend", CmdFriend);

	AddCmd("nvisible", CmdNVisible);

	AddCmd("yut_throw", CmdYutThrow);
	AddCmd("yut_info", CmdYutInfo);

	AddCmd("check_npc_summon", CmdCheckNpcSummon);
	AddCmd("close_npc_summon", CmdCloseNpcSummon);
	AddCmd("summon_npc_summon", CmdSummonNpcSummon);

	AddCmd("fish_throw", CmdFishThrow);
	AddCmd("fish_info", CmdFishInfo);
	AddCmd("fish_gift_list", CmdFishGiftList);
	AddCmd("fish_gift_item", CmdFishGiftItem);

	AddCmd("OK", CmdOK);
	AddCmd("charloadcomplete", CmdCharLoadComplete);
	AddCmd("chars_end", CmdCharsEnd);
	AddCmd("speedcheck", CmdSpeedCheck);

	AddCmd("dcb_res", CmdDecardiBattleRes);
	AddCmd("dcb_list_start", CmdDecardiBattleListStart);
	AddCmd("dcb_list", CmdDecardiBattleList);
	AddCmd("dcb_list_end", CmdDecardiBattleListEnd);
	AddCmd("dcb_start", CmdDecardiBattleStart);
	AddCmd("dcb_point", CmdDecardiBattlePoint);
	AddCmd("dcb_result", CmdDecardiBattleResult);
	AddCmd("dcb_gate", CmdDecardiBattleGate);
	AddCmd("dcb_day", CmdDecardiBattleDay);
	AddCmd("lotto", CmdLotto);
	AddCmd("lotto_list", CmdLottoList);
	AddCmd("lotto_chance", CmdLottoChance);
	AddCmd("p_upgrade_drake", CmdPetUpgradeDrake);
	AddCmd("p_wear_drake", CmdPetWearDrake);
	AddCmd("pweardrake", CmdPetWearDrake2);
	
	if(g_dwLangType == 3 || g_dwClientCountry ==3)
	{
		ReadFilterList("filtering_tw.txt");
		g_bFiltering = true;
	}
	else if(g_dwLangType == 4 || g_dwClientCountry ==4)
	{
		ReadFilterList("filtering_chi.txt");
		g_bFiltering = true;
	}
	else if(g_dwLangType == 1 || g_dwClientCountry ==1)
	{
		if( g_bGlobalMatrix ) ///국가대항전 전용 클라이언트의 경우 일본도 필터링을 한다.
		{
			ReadFilterList("filtering_jpn.txt",1); // 오토 리피트로...
			g_bFiltering = TRUE;
		}
		else
		{
			g_bFiltering = false; // 일본은 아직 필터링 하지 않는다.
		}
	}
	else if(	g_dwClientCountry ==7)
	{
		ReadFilterList("filtering_th.txt", TRUE);
		g_bFiltering = true;
	}
	else if(g_dwClientCountry ==2)
	{
		ReadFilterList("filtering_USA.txt",1);//미국도 필터링시작
		g_bFiltering = true;
	}
	else if( g_dwClientCountry == CTRY_BRA )
	{
		ReadFilterList("filtering_bra.txt");//브라질도 필터링시작
		g_bFiltering = true;
	}
	else if( g_dwLangType == 5
			 || g_dwClientCountry ==5
			 || g_dwClientCountry ==6
			 || g_dwClientCountry ==7
			 || g_dwClientCountry == CTRY_TEN
			 || g_dwClientCountry == CTRY_DENG
			 || g_dwClientCountry == CTRY_MENG
			 || g_dwClientCountry == CTRY_FR
		   )
	{
		g_bFiltering = false;
	}
	else
	{
		ReadFilterList("filtering.ftr");
		g_bFiltering = true;
	}
}

CCmd::~CCmd()
{
	DeleteFilterList();

	SAFE_DELETE(g_pGuildCmd);
	SAFE_DELETE(g_pWantedCmd);
}

BOOL CCmd::AddCmd(char *cmd, void cmd_p(char *, CUIMgr *))
{
	m_mapFunc.insert(std::make_pair(cmd, &cmd_p));
	return TRUE;
}

void CCmd::Run(char* cmd, char *arg)
{
	MAP_FUNC::iterator itFind = m_mapFunc.find(cmd);
	if( itFind == m_mapFunc.end() )
		return;

	itFind->second(arg, m_pUIMgr);
}

// nate : 대만 훈장 관련
// 캐릭터 레벨 추가

void CmdRoyaleArg(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_pRoh->m_bBrLv = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	g_pRoh->m_bpexp = _atoi64( g_Buf );

		arg = AnyOneArg(arg, g_Buf);
	g_pRoh->m_bpmax = _atoi64( g_Buf );

}
void CmdIn(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	int combat_state;

	arg = AnyOneArg(arg, g_Buf);
	if (!strncmp(g_Buf, "c", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// id
		arg = AnyOneArg(arg, g_Buf2);	// g_Buf2 -> Name
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// race
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// sex

		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// hair

		if (g_ii[3] >= 10)
			g_ii[3] = (g_ii[3] % 10) + 5;
		if (g_ii[3] < 0 || g_ii[3] >= 10)
			g_ii[3] = 0;

		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);				//
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);				//

		arg = AnyOneArg(arg, g_Buf);
		int lev = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);

		__int64 fame = _atoi64(g_Buf);


		if( y == 0 )	// 대만 훈장 관련
			y = 1;		// Here the client set it to 1
		// 270 레벨이하는 1로 처리

		arg = AnyOneArg(arg, g_Buf);
		dir = atof(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[3] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[4] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[5] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[6] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[7] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[8] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[WEARING_WEAPON] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		vital = atof(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		combat_state = atof(g_Buf);	// 0이 평화, 1이 전투, 2가 잠복, 67이 앉기, 87이 머리박기
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// 스킬번호
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// 상태
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);			// extra
		arg = AnyOneArg(arg, g_Buf);
		g_ii[7] = atoi(g_Buf);			// 카오 등급
		arg = AnyOneArg(arg, g_Buf);
		g_ii[8] = atoi(g_Buf);			// 프리 피케이 유무
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);			// 비행석 번호
		arg = AnyOneArg(arg, g_Buf3);	// 길드 이름.

		//==============================================
		// nate 2005-03-23
		// Matrix : 서버와 조건채크를 일치시킴
		// 기존 메트릭스서버와는 다르게 현재는 길드 인덱스가 팔요하여 추가하였음
		int nGuildType = 0;	// 0은 아무것도 아님, 1은 수정소유자, 3은 수정소유 길드원
		int nGuildIndex = 0;

		BOOL nShowMark = FALSE;
		if (g_Buf3[0] != NULL)
		{
			if( arg[1] == ' ' )
			{
				sprintf(g_Buf4, "");
			}
			else
			{
				arg = AnyOneArg(arg, g_Buf4);	// 길드 그레이드 이름
			}

			arg = AnyOneArg(arg, g_Buf);	// 길드 레벨
			g_ii[10] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// 길드 인덱스
			nGuildIndex = atoi( g_Buf );	// 메트리스에서는 길드 인덱스
			if( g_MatrixSvr != 0 ) //MATRIXSVR_NONE )
			{
				if (nGuildIndex < 0) //
				{
					nGuildIndex *= -1;
					nShowMark = TRUE;
				}
			}
			else
			{
				arg = AnyOneArg(arg, g_Buf);	// 길드 타입
				if (strlen(g_Buf)) nGuildType = atoi(g_Buf);	// 본섭에서는 길드 타입으로 쓰임
			}
		}
		else
		{
			g_Buf4[ 0 ] = NULL;
			g_ii[ 10 ] = 0;
		}

		if( g_MatrixSvr != 0 )
		{
			pMgr->AddPlayer( g_ii[0], g_Buf2,lev,fame, nShowMark, (float) x, (float) z,
							 dir, g_ii[1], g_ii[2], g_ii[3], v_num, combat_state,
							 g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8], g_ii[9],
							 g_Buf3, g_Buf4, g_ii[10], nGuildIndex ); // 길드 등급 이름, 등급 인덱스 추가
		}
		else
		{
			pMgr->AddPlayer(/*Index*/ g_ii[0],
							/*Name*/g_Buf2,
							lev,
							fame,
							nShowMark, 
							(float) x, 
							(float) z, 
							// Meant to be level
							
							 /*dir*/dir,
							 /*race*/g_ii[1],
							 /*Gender*/g_ii[2], 
							 /*Hair*/g_ii[3], 
							 /*Wearing*/v_num, 
							 combat_state,
							 /*Condition*/g_ii[4], 
							 /*Skill*/g_ii[5], 
							 /*Extra*/g_ii[6], 
							 /*PK Status*/g_ii[7], 
							 /*PK Mode on*/g_ii[8], 
							 /*Fly Level (sky)*/g_ii[9],
							 /*Guild name*/g_Buf3, 
							 /*Guild Grade*/g_Buf4, 
							 /*Guild Index*/g_ii[10], 
							 /*Guild type*/nGuildType ,
							 /*Not sure why it needs two..*/nGuildIndex
							  ); // 길드 등급 이름, 등급 인덱스 추가
		}

		pCha = NULL;
		pCha = pMgr->FindChaById(g_ii[0], FALSE);

		if (g_MatrixSvr == 0 && g_bGlobalMatrix == FALSE)
		{
			pMgr->isguildmark(nGuildIndex);
		}

		if( vital == 0 ) // 멀리있던 캐릭터의 피가 0이면 죽어있던 상태.(쓰러져있게 만들어야 한다.) 07-08-01 수정
		{
			if (pCha)
			{
				pCha->Kill();
				pCha->FrameMove( 60 ); // 임의로 시간을 진행시켜 쓰러지는 모양을 없애준다.
			}

		}

		if( pCha )
		{
			pCha->OnBossRaidHeroEffect();
		}
	}
	else if (!strncmp(g_Buf, "n", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// id
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// vnum
		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		y = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		dir = atof(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		vital = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// 뮤턴트 몹인감?
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// 속성몹 attribute
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// 계열몹 class
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// 몹의 상태.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);			// 워자이언트의 리젠
//================================================================================
		// nate 2004 - 4
		// quest level
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 7 ] = atoi( g_Buf );

		g_ii[3] = y;	//현재까지 속성 플래그 안쓰임 -> y값에 이벤트 몹 플래그 줌

		pMgr->AddMob(g_ii[1], g_ii[0], x, z, dir, vital, g_ii[2],g_ii[3],g_ii[4], g_ii[ 7 ], g_ii[6] ); // 리젠 플레그 추가 ///인던1
//================================================================================
		//----------------------------------------------------------------------
		// 디바인 스톤이 in 이라면
		if(g_ii[4] == 9)
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			pMob = pMgr->FindMobById(g_ii[0]);

			ef_sort.nType = FX_DSTONEEFF;
			ef_sort.pMobFrom = pMob;
			ef_sort.num = pMob->m_nVirId;
			ef_sort.subNum = pMob->m_nMobNum;
			pMob->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}

		if( g_ii[1] == 497 ) ////드미트론 점령전 - 크리스탈
		{
			// 빛 기둥
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			pMob = pMgr->FindMobById(g_ii[0]);

			ef_sort.nType = FX_DMITRON_CRYSTAL;
			ef_sort.vPos.x = pMob->m_wx;
			ef_sort.vPos.y = pMob->m_wy;
			ef_sort.vPos.z = pMob->m_wz;
			ef_sort.subNum = pMob->m_nMobNum;
			pMob->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect(ef_sort) );
		}

		if( g_ii[1] == 504)
		{
			// 빛 기둥
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			pMob = pMgr->FindMobById(g_ii[0]);

			ef_sort.nType = FX_DMITRON_PORTAL;
			ef_sort.vPos.x = pMob->m_wx;
			ef_sort.vPos.y = pMob->m_wy;
			ef_sort.vPos.z = pMob->m_wz;
			ef_sort.subNum = pMob->m_nMobNum;
			pMob->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}

		if(pCMyApp->m_pFxSet && g_ii[6] == 1 ) // g_ii[6] 등장 타입이 1, 즉, 기존의 뿅방식일때만 ShowUp 펙트를 출력한다. ///인던1
		{
			//리젠 메시지
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));
			ef_sort.nType = FX_MOBSHOWUP;
			ef_sort.num = g_ii[1];
			ef_sort.vPos.x = x;
			ef_sort.vPos.z = z;
			if( pCMyApp->GetLandPtr() )
				ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight(x, z);
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

			ef_sort.nCurFrame = -3;
			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		}

	}
	else if (!strncmp(g_Buf, "p", 1))
	{
		//sscanf(msg, "%s %s %d %d %d %d %d %f", first_arg, second_arg, &g_ii[0], &g_ii[1], &x, &z, &y, &dir);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// id
		arg = AnyOneArg(arg, g_Buf2);	// name

		arg = AnyOneArg(arg, g_Buf);  // class
		g_ii[4] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		dir = atof(g_Buf);

		arg = AnyOneArg(arg, g_Buf);  // horn
		v_num[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // head
		v_num[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // body
		v_num[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // wings
		v_num[3] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // legs
		v_num[4] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // tail
		v_num[5] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		vital = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf); // mode
		arg = AnyOneArg(arg, g_Buf3);	// 주인 이름
		// IDS_CMD_PET_NAME : %s의 %s
		sprintf(g_Buf, G_STRING(IDS_CMD_PET_NAME), g_Buf3, g_Buf2);
		arg = AnyOneArg(arg, g_Buf2);
		g_ii[3] = atoi(g_Buf); // 메타레벨?

		pMgr->AddPet(g_ii[0], g_Buf, g_ii[4], x, z, dir, vital,
					 v_num[0], v_num[1], v_num[2], v_num[3], v_num[4], v_num[5], g_ii[3]);
	}
	else     // 아이템...
	{
		//sscanf(msg, "%s %s %d %d %d %d %d %f", first_arg, second_arg, &g_ii[0], &g_ii[1], &x, &z, &y, &dir);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// id
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// vnum
		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		y = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		dir = atof(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		gem_num[0] = atof(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		special[0] = atof(g_Buf);

		///신방어구강화시스템
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atof(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf); // 시간제 아이템인지의 플래그.

		pMgr->AddItem(g_ii[0], g_ii[1], x, z, dir, gem_num[0], special[0], g_ii[2], g_ii[3] ); ///신방어구강화시스템
	}
}

// nate
// 대만 훈장 관련
// y 값을 캐릭의 레벨로 사용
void CmdAppear(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	int combat_state;
	arg = AnyOneArg(arg, g_Buf);
	if (!strncmp(g_Buf, "c", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// id
		arg = AnyOneArg(arg, g_Buf2);	// g_Buf2 -> Name
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// race
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// sex

		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// hair

		if (g_ii[3] >= 10)
			g_ii[3] = (g_ii[3] % 10) + 5;
		if (g_ii[3] < 0 || g_ii[3] >= 10)
			g_ii[3] = 0;

		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);				// Charater X좌표
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);				// Charater Z좌표
		arg = AnyOneArg(arg, g_Buf);
		//y = atoi(g_Buf);				// Charater Y좌표
		//arg = AnyOneArg(arg, g_Buf);
		
		int lev = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		
		__int64 fame = _atoi64(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		dir = atof(g_Buf);				// Charater Direction
		arg = AnyOneArg(arg, g_Buf);
		v_num[0] = atoi(g_Buf);			// Charater 착용 장비들
		arg = AnyOneArg(arg, g_Buf);
		v_num[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[3] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[4] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[5] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[6] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[7] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[8] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[WEARING_WEAPON] = atoi(g_Buf);	// 들고있는 무기
		arg = AnyOneArg(arg, g_Buf);
		vital = atof(g_Buf);			// 에너지
		arg = AnyOneArg(arg, g_Buf);
		combat_state = atof(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// 스킬번호
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// 상태
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);			// extra
		arg = AnyOneArg(arg, g_Buf);
		g_ii[7] = atoi(g_Buf);			// 카오 등급
		arg = AnyOneArg(arg, g_Buf);
		g_ii[8] = atoi(g_Buf);			// 프리 피케이 유무
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);			// 비행석 레벨
		arg = AnyOneArg(arg, g_Buf3);	// 길드 이름

		//==============================================
		// nate 2005-03-23
		// Matrix : 서버와 조건채크를 일치시킴
		// 기존 메트릭스서버와는 다르게 현재는 길드 인덱스가 팔요하여 추가하였음
		int nGuildType = 0;
		int nGuildIndex = 0;
		BOOL nShowMark = FALSE;

		if (g_Buf3[0] != NULL)
		{
			if( arg[1] == ' ' )
			{
				sprintf(g_Buf4, "");
			}
			else
			{
				arg = AnyOneArg(arg, g_Buf4);	// 길드 그레이드 이름
			}

			arg = AnyOneArg(arg, g_Buf);	// 길드 레벨
			g_ii[10] = atoi(g_Buf);

			arg = AnyOneArg(arg, g_Buf);	// 길드 인덱스
			nGuildIndex = atoi( g_Buf );	// 메트리스에서는 길드 인덱스
			if( g_MatrixSvr != 0 )
			{
				if (nGuildIndex < 0)
				{
					nGuildIndex *= -1;
					nShowMark = TRUE;
				}
			}
			else
			{
				arg = AnyOneArg(arg, g_Buf);	// 길드 타입
				if (strlen(g_Buf)) nGuildType = atoi(g_Buf);	// 본섭에서는 길드 타입으로 쓰임

			}
		}
		else
		{
			g_Buf4[ 0 ] = NULL;
			g_ii[ 10 ] = 0;
		}

		// nate
		// 대만 훈장 관련, y 값은 캐릭터의 레벨값으로 쓰인다.
		pMgr->AddPlayer( g_ii[0], g_Buf2,lev,fame, nShowMark, (float) x, (float) z, dir, g_ii[1], g_ii[2], g_ii[3], v_num, combat_state,
						 g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8], g_ii[9], g_Buf3, g_Buf4, g_ii[10], g_MatrixSvr != 0 ? nGuildIndex : nGuildType
						 , nGuildIndex	); // 길드 등급 이름, 등급 인덱스 추가
		//==============================================

		pCha = NULL;
		pCha = pMgr->FindChaById(g_ii[0], FALSE);

		if (g_MatrixSvr == 0 && g_bGlobalMatrix == FALSE)
		{
			pMgr->isguildmark(nGuildIndex);
		}

		if( pCha )
		{
			pCha->OnBossRaidHeroEffect();
		}
	}
	else if (!strncmp(g_Buf, "p", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// id
		arg = AnyOneArg(arg, g_Buf2);	// name

		arg = AnyOneArg(arg, g_Buf);  // class
		g_ii[4] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		dir = atof(g_Buf);

		arg = AnyOneArg(arg, g_Buf);  // horn
		v_num[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // head
		v_num[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // body
		v_num[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // wings
		v_num[3] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // legs
		v_num[4] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);  // tail
		v_num[5] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		vital = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf); // mode
		arg = AnyOneArg(arg, g_Buf3);	// 주인 이름
		// IDS_CMD_PET_NAME : %s의 %s
		sprintf(g_Buf, G_STRING(IDS_CMD_PET_NAME), g_Buf3, g_Buf2);
		arg = AnyOneArg(arg, g_Buf2);
		g_ii[3] = atoi(g_Buf2); // 메타 레벨?

		pMgr->AddPet(g_ii[0], g_Buf, g_ii[4], x, z, dir, vital,
					 v_num[0], v_num[1], v_num[2], v_num[3], v_num[4], v_num[5], g_ii[3]);
	}

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	ef_sort.nType = FX_TELEPORT;
	ef_sort.vPos.x = x;
	ef_sort.vPos.z = z;
	if( g_bNewTerrain )
	{
		float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( x, z );
		ef_sort.vPos.y = CNKObjQuadNode::GetHeightOnObject( z, x, y );
		if( ef_sort.vPos.y < fHeight )
			ef_sort.vPos.y = fHeight;
	}
	else
		ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( x, z );

	if( pCMyApp->m_pFxSet )
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
}

void CmdWalk(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	// npc인가 char인가 구별.
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	if (!strncmp(g_Buf2, "c", 1))
	{
		// 플레이어 워킹
		arg = AnyOneArg(arg, g_Buf);
		g_nRun = atoi(g_Buf);
		if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		{
			if (g_nRun == 0)
			{
				pCha->m_bRun = FALSE;
				pCha->SetMotionState(CHA_WALK);
			}
			else
			{
				pCha->m_bRun = TRUE;
				pCha->SetMotionState(CHA_RUN);
			}

			pCha->Move((float) x, (float) z);
		}
	}
	else if (!strncmp(g_Buf2, "p", 1))
	{
		// 라비용
		// 애완동물 워킹
		if (pPet = pMgr->FindPetById(g_ii[0]))
			pPet->Move((float) x, (float) z);
		else
		{
			if(g_dwLangType == 0)
			{
				char strTemp2[200] = "";
				sprintf(strTemp2, (char*)G_STRING(IDS_LHSTRING1724), g_ii[0]);

				// 서버로 패킷을 보내서 In 메세지를 요청한다.(고스트 몹 현상이 In 패킷이 제대로 안와서 그런 걸로 예상되므로..) 05-08-17 원석
				if( g_pTcpIp )
				{
					sprintf(strTemp2, "ghost_ck p %d\n", g_ii[0]);
					g_pTcpIp->SendNetMessage( strTemp2 );
				}
			}
		}
	}
	else
	{
		//npc
		// 맙 워킹
		if (pMob = pMgr->FindMobById(g_ii[0]))
			pMob->Move((float) x, (float) z);
		else
		{
			if(g_dwLangType == 0)
			{
				char strTemp3[200];
				sprintf(strTemp3, (char*)G_STRING(IDS_LHSTRING1724), g_ii[0]);

				// 서버로 패킷을 보내서 In 메세지를 요청한다.(고스트 몹 현상이 In 패킷이 제대로 안와서 그런 걸로 예상되므로..) 05-08-17 원석
				if( g_pTcpIp )
				{
					sprintf(strTemp3, "ghost_ck n %d\n", g_ii[0]);
					g_pTcpIp->SendNetMessage( strTemp3 );
				}
			}
		}
	}
}

void CmdPlace(char *arg, CUIMgr *pMgr)
{
	// npc인가 char인가 구별.
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	dir = atof(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	if (!strncmp(g_Buf2, "c", 1))
	{
		// 플레이어 워킹
		arg = AnyOneArg(arg, g_Buf);
		g_nRun = atoi(g_Buf);
		if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		{
			if (g_nRun == 0)
			{
				pCha->m_bRun = FALSE;
				pCha->SetMotionState(CHA_WALK);
			}
			else
			{
				pCha->m_bRun = TRUE;
				pCha->SetMotionState(CHA_RUN);
			}

			pCha->Place((float) x, (float) z, (float) dir, g_ii[1]);
		}
	}
	else
	{
		// 맙 워킹
		if (pMob = pMgr->FindMobById(g_ii[0]))
			pMob->Place((float) x, (float) z, (float) dir, g_ii[1]);
	}
}

void CmdStop(char *arg, CUIMgr *pMgr)
{
	// npc인가 char인가 구별.
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	dir = atof(g_Buf);
	if (!strncmp(g_Buf2, "c", 1))
	{
		// 플레이어 워킹
		if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		{
			pCha->Stop((float) x, (float) z, dir);
		}
	}
	else
	{
		// 맙 워킹
		if (pMob = pMgr->FindMobById(g_ii[0]))
			pMob->Stop((float) x, (float) z, dir);
	}
}

void CmdRotateCharacter(char *arg, CUIMgr *pMgr) // 모션 변경없이 방향만 바꿔주기 위한 패킷.
{
	// npc인가 char인가 구별.
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	dir = atof(g_Buf);
	if (!strncmp(g_Buf2, "c", 1))
	{
		// 플레이어 워킹
		if (pCha = pMgr->FindChaById(g_ii[0], TRUE))
			pCha->m_toDir = dir; // 방향만 바꿔준다.
	}
	else
	{
		// 맙 워킹
		if (pMob = pMgr->FindMobById(g_ii[0]))
			pMob->m_toDir = dir; // 방향만 바꿔준다.
	}
}

void CmdOut(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	if (!strncmp(g_Buf2, "c", 1))
		pMgr->DeletePlayer(g_ii[0]);
	else if (!strncmp(g_Buf2, "p", 1))
		pMgr->DeletePet(g_ii[0]);
	else if (!strncmp(g_Buf2, "n", 1))
		pMgr->DelMob(g_ii[0]);
	else
		pMgr->DeleteItem(g_ii[0]);
}

void CmdAttack(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->CmdAttack(arg);
}

///2003 서브어택메시지 처리
void CmdSubAttack(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->CmdSubAttack(arg);
}

#define SAY_TO		Whisper//"*귓속말*"
#define GUILD_TO	Guild_To
#define PARTY_TO	Party_To
#define GG			"*GM*"

void CmdSay(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	pCha = NULL;

	char TempBuf[256]; // 텍스트 중간 수정용 버퍼.(by 원석)
	int say_type = -1;

	// [2008/12/16 Theodoric] WORLD_MATRIX_2009
	bool bMatrixMaster = false;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	pCha = pMgr->FindChaById(g_ii[0]);

	if (g_ii[0] >= 0)
	{
		SkipSpaces(&arg);
		int len = strlen(arg);
		BOOL bAdmin = FALSE;
		BOOL bNormal = FALSE;

		if(len >= 1 && arg[len-1] == '1')
		{
// by evilkiki 2010.11.10. 채팅색상 조정.
			if (g_dwLangType != 0 && g_dwClientCountry != 0)
				say_type = 4;

			bAdmin = TRUE;
		}
		if(len >= 1 && arg[len-1] == '0')
			bNormal = TRUE;

		char *arg2 = AnyOneArg(arg, g_Buf);

		int nSayto_len = strlen(SAY_TO);
		int nSayto_len2 = strlen(&g_Buf[2]);
		char * test = &g_Buf[2];
		if (!strncmp(&g_Buf[2], SAY_TO, nSayto_len)	&& strlen(&g_Buf[2]) > nSayto_len)
//		if (!strncmp(g_Buf, SAY_TO, nSayto_len)	&& strlen(g_Buf) > nSayto_len)
		{
			say_type = 1;
			if( g_pNk2DFrame->GetControlBottom() )
				g_pNk2DFrame->GetControlBottom()->InsertSender(&g_Buf[nSayto_len+2]);	// nate 2006-03-10 : Whisper List
		}
		else if( ( !strncmp(g_Buf, GUILD_TO, strlen(GUILD_TO))
				   && strlen(g_Buf) > strlen(GUILD_TO) )
				 || ( !strncmp(g_Buf, "*TEAM*", 6)
					  && strlen(g_Buf) > 6 )) // 신 매트릭스에서는 길드채팅 대신 팀(연합)채팅이 온다. (어차피 영어고 시스템 스트링이니 DLL에 넣지 않고 처리했다.) (by 원석)
			say_type = 2;
		else if (!strncmp(g_Buf, PARTY_TO, strlen(PARTY_TO))
				 && strlen(g_Buf) > strlen(PARTY_TO))
			say_type = 3;
		else if( !strncmp(g_Buf, "*SHOUT*", 7)
				 && strlen(g_Buf) >= 7 ) // 외침.(매트릭스 등에서 지휘자가 보내는 말)
		{
			if (g_pDSound)
				g_pDSound->Play(g_EffectSound[EFF_SND_LEVELUP]); // 사운드 종류는 후에 바꿔주자.

			// 외침은 "*SHOUT*"부분을 바꿔준다.
			strcpy( TempBuf, &arg[7] );
			//- IDS_COMMANDER:   지휘자:
			strcpy( arg, G_STRING(IDS_COMMANDER) ); // 띄어쓰기 넣으면 안됨에 유의!
			strcat( arg, TempBuf );

			say_type = 6;

			if( pCMyApp->GetCurWorld() == 19 )
				bMatrixMaster = true;

		}
		else
			say_type = 0;
// by evilkiki 2010.11.10. 채팅색상 조정.
		if(bAdmin && g_dwLangType != 0 && g_dwClientCountry != 0)
			say_type = 4;

		len = strlen(arg);
		if (len >= 2 && (bAdmin || bNormal))
			arg[len-2] = '\0';

		// 귓말 제거 대상자인지 확인한다.
		if (say_type == 1
				&& g_pNk2DFrame
				&& g_pNk2DFrame->GetControlBottom()
				&& g_pNk2DFrame->GetControlBottom()->GetNoWhisper()
				&& g_pNk2DFrame->GetControlBottom()->GetNoWhisper()->IsIntheList(&g_Buf[nSayto_len+2]))
//			&& g_pNk2DFrame->GetControlBottom()->GetNoWhisper()->IsIntheList(&g_Buf[nSayto_len]))
		{
			char buf[256];
			strcpy(buf, "nochat ");
			strcat(buf, &g_Buf[nSayto_len+2]);
//			strcat(buf, &g_Buf[nSayto_len]);
			strcat(buf, "\n");
			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage(buf);
			return;
		}

		char *arg3 = arg2;
		SkipSpaces(&arg3);

		if(g_bFiltering)
			CheckAndRepair(arg3);

		arg2 = AnyOneArg(arg, g_Buf); // 이 녀석 위치가 굉장히 중요함에 유의! 위의 "char *arg3 = arg2;" 이 줄 다음에 와야한다.// 여기서 이걸 다시해주는 이유는 위에서 arg의 스트링 앞부분을 고쳐버리는 루틴이 있다. 그걸 감안해서 정상적인 arg2를 얻기위해선 다시 얻어야한다.( by 원석 )
		if( bMatrixMaster )
		{
			g_pNk2DFrame->SetInfoBox2(arg2);
		}
		if (pCha)
		{
			if( say_type == 1 )
			{
				pCha->SetChatMsg(&g_Buf[2], arg3);
			}
			else if( say_type == 0 )
			{
				if( DataFriend::GetInstance()->IsDenyFriend(g_ii[0]) == true )
					return;

				pCha->SetChatMsg(g_Buf, arg3);
			}
			else
			{
				pCha->SetChatMsg(g_Buf, arg3);
			}
		}

		// [5/6/2008 ppmmjj83] 브라질 요청으로 채팅창에 표시되는 아이디 구분 텍스트 변경
		if( g_dwClientCountry == CTRY_BRA )
		{
			*arg2 = '>';
		}

		else
		{
			*arg2 = ')';
		}
	}

	if( g_ii[0] == -2 ) // say -2 타입은 서버메세지가 번호로 넘어오는 타임. ///서버메세지
	{
		arg = AnyOneArg(arg, g_Buf); // 이 녀석 위치가 굉장히 중요함에 유의! 위의 "char *arg3 = arg2;" 이 줄 다음에 와야한다.// 여기서 이걸 다시해주는 이유는 위에서 arg의 스트링 앞부분을 고쳐버리는 루틴이 있다. 그걸 감안해서 정상적인 arg2를 얻기위해선 다시 얻어야한다.( by 원석 )
		g_ii[1] = atoi(g_Buf);			// 서버 메세지 번호.

		if( g_pNk2DFrame )
			g_pNk2DFrame->AddChatStr((char*)G_STRING(g_ii[1]), say_type);	 // 서버에서 바로 메세지 번호를 받도로 수정 ///07-04-27
	}

	// [5/6/2008 ppmmjj83] 태국 노란색 티셔츠 지급 이벤트 관련.
	if( g_ii[0] == -3 )
	{
		char strTemp[512] = {0, };

		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);

		switch( g_ii[1] )
		{
		case 0:
			{
				arg = AnyOneArg(arg, g_Buf);
				g_ii[2] = atoi(g_Buf);

				sprintf( strTemp, (char*)G_STRING(IDS_YELOWWT_EVENTMSG0), g_ii[2] );
			}
			break;

		case 1:
			{
				arg = AnyOneArg(arg, g_Buf);
				g_ii[2] = atoi(g_Buf);

				sprintf( strTemp, (char*)G_STRING(IDS_YELOWWT_EVENTMSG1), g_ii[2] );
			}
			break;

		case 2:
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_YELOWWT_EVENTMSG2), say_type);
			}
		case 3:
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_YELOWWT_EVENTMSG3), say_type);
			}
			break;

		case 4:
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_YELOWWT_EVENTMSG4), say_type);
			}
			break;

		default:
			break;
		}

		if( strcmp( strTemp, "" ) != 0 )
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->AddChatStr(strTemp, say_type);
		}
	}

	else
	{
		SkipSpaces(&arg);
		//////////////////////////////////////////////////////////////////////////
		// [2008/5/8 Theodoric] log남기기
		if( g_bAdmin )
		{
			char strTemp[1024] = {0,};
			char strWrite[1024] = {0,};
			sprintf(strTemp, "%s", arg);

			char* strTemp1 = AnyOneArg(strTemp, g_Buf);
			if( !strnicmp(g_Buf, "Base:", 5) )
			{
				// 패킷 분해
				int dmg_base = 0;
				int dmg_dam  = 0;

				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				dmg_base = atoi( g_Buf);

				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				dmg_dam  = atoi( g_Buf);

				sprintf(strTemp, "Base : %d  =>  Dam : %d  ", dmg_base, dmg_dam);
				G_CatLocalTimeToStr(strWrite, strTemp);

				if( b_Damage && g_bAdmin2 )
				{
					tab3_Dmg_base += dmg_base;
					tab3_Dmg_dam  += dmg_dam;

					FILE* fp = NULL;
					fp = fopen( "LhLog/DamageLog.txt", "a+" );
					fprintf( fp, strWrite );
					fclose( fp );
				}

				G_DevAddPointString(strWrite);
			}
			else if( !strnicmp(g_Buf, "*", 1) )
			{
				strTemp1 = AnyOneArg(strTemp1, g_Buf);

				if( !strnicmp(g_Buf, "Counter_DAM:", 12) )
				{
					int dmg = atoi(strTemp1);
					sprintf(strTemp, "=>  Counter_DAM : %d", dmg);

					if( b_Damage && g_bAdmin2 )
					{
						FILE* fp = NULL;
						fp = fopen( "LhLog/DamageLog.txt", "a+" );
						fprintf( fp, "%s\n", strTemp );
						fclose( fp );
					}

					G_CatLocalTimeToStr(strWrite, strTemp);
					G_DevAddPointString(strWrite);
				}
				else if( !strnicmp(g_Buf, "Critical_DAM::", 13) )
				{
					int dmg = atoi(strTemp1);

					sprintf(strTemp, "=>  Critical_DAM : %d", dmg);

					if( b_Damage && g_bAdmin2 )
					{
						FILE* fp = NULL;
						fp = fopen( "LhLog/DamageLog.txt", "a+" );
						fprintf( fp, "%s\n", strTemp );
						fclose( fp );
					}

					G_CatLocalTimeToStr(strWrite, strTemp);
					G_DevAddPointString(strWrite);

				}
			}
			else if( !strnicmp(g_Buf, "Last", 4) )
			{
				int dmg_last = 0;
				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				dmg_last = atoi( g_Buf);

				sprintf(strTemp, "=>  Last : %d", dmg_last);

				if( b_Damage && g_bAdmin2 )
				{
					tab3_Dmg_Last += dmg_last;

					FILE* fp = NULL;
					fp = fopen( "LhLog/DamageLog.txt", "a+" );
					fprintf( fp, "%s\n", strTemp );
					fclose( fp );
				}

				G_CatLocalTimeToStr(strWrite, strTemp);
				G_DevAddPointString(strWrite);
			}
			else if( !strnicmp(g_Buf, "Obtained", 4) )
			{
				__int64 exp  = 0;
				__int64 lime = 0;

				char strTemp2[512];

				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				exp = _atoi64(g_Buf);

				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				strTemp1 = AnyOneArg(strTemp1, g_Buf);
				lime = _atoi64(g_Buf);

				if( b_EXP && g_bAdmin2 )
				{
					tab3_EXP += exp;

					FILE* fp = NULL;
					fp = fopen( "LhLog/ExpLog.txt", "a+" );
					sprintf(strTemp2, "EXP : %8d", exp);
					G_CatLocalTimeToStr(strWrite, strTemp2);
					fprintf( fp, "%s\n", strWrite );
					fclose( fp );
				}

				if( b_Lime && g_bAdmin2 )
				{
					tab3_Lime += lime;

					FILE* fp = NULL;
					fp = fopen( "LhLog/LimeLog.txt", "a+" );
					sprintf(strTemp2, "Lime : %8d", lime);
					G_CatLocalTimeToStr(strWrite, strTemp2);
					fprintf( fp, "%s\n", strWrite );
					fclose( fp );
				}
				G_CatLocalTimeToStr(strWrite, strTemp);
				G_DevAddPointString(strWrite);
			}

			//////////////////////////////////////////////////////////////////////////

		}

		if( g_pNk2DFrame && pCha && say_type == 0 && pCha->IsPressMan() )
			say_type = 	7;
		// [4/10/2009 D.K ] : 라그러시 이용시 라임 부족의 경우 라그러시 정지.
		if( g_pNk2DFrame )
		{
			if( strstr( arg, "Lack of money" ) && pCMyApp->m_pUIMgr->m_bAutoHunt )
				pCMyApp->m_pUIMgr->m_AutoPlay->StopProc(AutoPlay_StopProc__Laim_Not);
		}
		if( g_pNk2DFrame )
			g_pNk2DFrame->AddChatStr(arg, say_type);

	}
}



void CmdWearing(char *arg, CUIMgr *pMgr)
{
	for(int i=0; i<WEARING_NUM; i++)
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);
		///신방어구강화시스템
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf); // 추가 옵션 m_Special2.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf); // 현재 강화 내구도 수치.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf); // 맥스 강화 내구도.

		if( g_SvrType == ST_ADULT_ONLY)// [6/1/2007 Theodoric] 내구도 수리
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[7] = atoi(g_Buf);		// 일반 내구도
			arg = AnyOneArg(arg, g_Buf);
			g_ii[8] = atoi(g_Buf);		// 일반 내구도 max
		}

		if(g_pRoh)
		{
			if( g_SvrType == ST_ADULT_ONLY)
				g_pRoh->Wear(i, g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8] ); ///신방어구강화시스템
			else
				g_pRoh->Wear(i, g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], 0, 0 ); ///신방어구강화시스템
		}
	}

	g_pRoh->OnBossRaidHeroEffect();
}

void CmdInven(char *arg, CUIMgr *pMgr)
{
	//sscanf(msg, "%s %d %d %d %d %d", first_arg, &g_ii[0], &g_ii[1], &g_ii[2], &g_ii[3], &g_ii[4]);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);			// ???
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);		// ???

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);	// ???
	g_ii[5] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[8] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);			// 일반 내구도 Max
	}

	if( g_pRoh )
	{
		if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
			g_pRoh->AddToInven(g_ii[0], g_ii[3], g_ii[4], g_ii[1], g_ii[2], gem_num[0], special[0], g_ii[5], g_ii[6], g_ii[7], g_ii[8], g_ii[9] ); ///신방어구강화시스템
		else
			g_pRoh->AddToInven(g_ii[0], g_ii[3], g_ii[4], g_ii[1], g_ii[2], gem_num[0], special[0], g_ii[5], g_ii[6], g_ii[7], 0 , 0 ); ///신방어구강화시스템
	}
}

void CmdExtra(char *arg, CUIMgr *pMgr)
{
	//sscanf(msg, "%s %d %d", first_arg, &g_ii[0], &g_ii[1]);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf); // 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);		// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);		// 일반 내구도 Max
	}

	if( g_pRoh )
	{
		if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
			g_pRoh->SetExtraSlot(g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6] ); ///신방어구강화시스템
		else
			g_pRoh->SetExtraSlot(g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[2], g_ii[3], g_ii[4], 0, 0 ); ///신방어구강화시스템
	}
}

void CmdPickExtra(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		// 공간부족
		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1833) , -1 );

		if( CommonConfig::Instance()->GetLagrush() == true )
		{
			pMgr->m_AutoPlay->OnDontPick();
		}	

		return;
	}

	g_ii[0] = rv;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf); // 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);		// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);		// 일반 내구도 Max
	}

	if (g_pNk2DFrame
			&& g_pNk2DFrame->GetControlInven())
	{
		g_pNk2DFrame->GetControlInven()->SetPickExtraTime();
	}

	if( g_pRoh )
	{
		if( g_SvrType == ST_ADULT_ONLY )
			g_pRoh->SetExtraSlot(g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6] ); ///신방어구강화시스템
		else
			g_pRoh->SetExtraSlot(g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[2], g_ii[3], g_ii[4], 0, 0 ); ///신방어구강화시스템
	}

	// 패킷을 받은 후에 pick 플래그를 해제( by 원석 )
	if( pMgr )
		pMgr->m_bPickLock = FALSE; ///pick_bug
}

void CmdQuick(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[5] = atoi(g_Buf); // special2 (06-04-29 추가)

	if( g_pRoh )
		g_pRoh->ToQuickSlot(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5]);
}

void CmdRemoveAll(char *arg, CUIMgr *pMgr)
{
	if( g_pRoh )
		g_pRoh->RemoveAll();
}

void CmdDrop(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	dir = atof(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atof(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atof(g_Buf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf); // 추가 옵션 m_Special2.

	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf); // 시간제 아이템인지의 플래그.

	if( pMgr )
		pMgr->AddItem(g_ii[0], g_ii[1], x, z, dir, gem_num[0], special[0], g_ii[2], g_ii[3], TRUE); ///신방어구강화시스템
}
/*
void CmdNpcDrop(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	dir = atof(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);

	if (pMob = pMgr->FindMobById(g_ii[2]))
		pMob->SetDropItem(g_ii[0], g_ii[1], x, z, dir);
}
*/
void CmdPick(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// 인덱스
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// vnum
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);			// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);			// y
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);			// 슬롯 넘버
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[5] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[8] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);			// 일반 내구도 Max.
	}

	if( g_pRoh )
	{
		if( g_SvrType == ST_ADULT_ONLY )
			g_pRoh->AddToInven(g_ii[4], g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[5], g_ii[6], g_ii[7], g_ii[8], g_ii[9]); ///신방어구강화시스템
		else
			g_pRoh->AddToInven(g_ii[4], g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[5], g_ii[6], g_ii[7], 0, 0 ); ///신방어구강화시스템

		// [1/6/2009 D.K ] : 아이템 습득시 습득 아이템 메시지 출력.
		char szMessage[128] = {0,};
		if( g_dwClientCountry == CTRY_KOR )
		{
			sprintf( szMessage, G_STRING(IDS_LHSTRING1725), GET_ITEM_NAME_VNUM(g_ii[1]) );
			g_pNk2DFrame->AddChatStr( szMessage, -1 );
		}
		else if( g_dwClientCountry == CTRY_JPN )
		{
			if(g_pNk2DFrame->GetControlShop() && g_pNk2DFrame->GetControlShop()->m_bMultibuy )
			{
				g_pNk2DFrame->GetControlShop()->m_iMulitCnt++;

				if( g_pNk2DFrame->GetControlShop()->m_iMulitCnt == 10 )
				{
					// IDS_LHSTRING1725
					// sprintf( szMessage, G_STRING(IDS_LHSTRING1725) , g_ItemList.m_ItemArray[g_ItemList.FindItem( g_ii[1])].m_Name );
					sprintf( szMessage, "%s 10뙿롦벦.", GET_ITEM_NAME_VNUM(g_ii[1]) );
					g_pNk2DFrame->GetControlShop()->m_iMulitCnt = 0;
					g_pNk2DFrame->GetControlShop()->m_bMultibuy = false;
					g_pNk2DFrame->AddChatStr( szMessage, -1 );
				}
			}
			else
			{
				sprintf( szMessage, "%s 롦벦.", GET_ITEM_NAME_VNUM(g_ii[1]) );
				// G_STRING(IDS_LHSTRING1725)
				g_pNk2DFrame->AddChatStr( szMessage, -1 );
			}
		}
		else
		{
			sprintf( szMessage, G_STRING(IDS_LHSTRING1725), GET_ITEM_NAME_VNUM( g_ii[1]) );
			g_pNk2DFrame->AddChatStr( szMessage, -1 );
		}

		g_pRoh->CountQuickAll();
	}

	if( g_pNk2DFrame && g_pNk2DFrame->IsHelpPetVisible() && g_pNk2DFrame->GetControlHelpPet() ) ///펫도움말 의 발동 조건을 체크한다.
		g_pNk2DFrame->GetControlHelpPet()->CheckHelpCondition( 2, g_ii[1], TRUE ); // 아이템 습득 조건의 헬프가 있는지 체크한다.

	// 패킷을 받은 후에 pick 플래그를 해제( by 원석 ) (사실 이 플래그 셋팅이 해결하는 버그는 CmdPickExtra() 에 들어올때 해당하는 버그이지만 플래그의 해제는 여기서도 해줘야한다.)
	if( pMgr )
		pMgr->m_bPickLock = FALSE; ///pick_bug
}


void CmdPickup(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	ef_sort.nType = FX_EATEFFECTS;

	pCha = pMgr->FindChaById(g_ii[0], FALSE);

	if( pCha )
	{
		ef_sort.pNkChaTo = pCha;
		pCha->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
	}
	else if (g_pRoh->m_nCharIndex == g_ii[0])
	{
		ef_sort.pNkChaTo = g_pRoh;
		if( g_pRoh )
			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
	}
}

void CmdHour(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if( pCMyApp->GetCurWorldID() == WORLD_BOSSRAID_BULKAN )
		pCMyApp->ChangeHour(23); // 마지막 보스는 고정한다.
	else
		pCMyApp->ChangeHour(g_ii[0]);

	pCMyApp->SetWaterRender(TRUE);

	if( pCMyApp->GetCurWorldID() == WORLD_BOSSRAID_BULKAN && ( g_ii[0] >= 28 || g_ii[0] <= 3 ) )
		pCMyApp->SetWaterRender(FALSE);
}

void CmdWeather(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if(pCMyApp->IsAcropolis())
		g_ii[0] = 0;

	if (!(g_bDungeon || g_bNewTerrainDungeon )&& !pCMyApp->m_ProOption.bWeather)
		if( pCMyApp->m_pWeatherMgr )
			pCMyApp->m_pWeatherMgr->SetWeather(g_ii[0]);
}

void CmdStatus(char *arg, CUIMgr *pMgr)
{
	if( !g_pRoh || !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = _atoi64(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	// nate 2005-09-08 : 경험치
	__int64 n64Exp = 0.0f;
	if( g_ii[0] == STATUS_EXP || g_ii[0] == STATUS_NEED_EXP || g_ii[0] == STATUS_MONEY )
		n64Exp = _atoi64( g_Buf );
	else
		g_ii[1] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);

	switch (g_ii[0])
	{
	case STATUS_VITAL:
		g_pRoh->SetVital(g_ii[1], g_ii[2]);
		break;
	case STATUS_MANA:
		g_pRoh->SetMana(g_ii[1], g_ii[2]);
		break;
	case STATUS_STAMINA:
		g_pRoh->SetStamina(g_ii[1], g_ii[2]);
		break;
	case STATUS_EPOWER:
		g_pRoh->SetEpower(g_ii[1], g_ii[2]);
		break;
	case STATUS_LEVEL:
		g_pRoh->SetLevel(g_ii[1]);
		break;
	case STATUS_LEVELUP_POINT:
		g_pRoh->SetLevelupPoint (g_ii[1]);
		break;
	case STATUS_EXP:
		g_pRoh->SetExp(n64Exp);
		break;
	case STATUS_NEED_EXP:
		g_pRoh->SetNeedExp(n64Exp);
		break;
	case STATUS_STR:
		g_pRoh->SetStr(g_ii[1]);
		break;
	case STATUS_INT:
		g_pRoh->SetInt(g_ii[1]);
		break;
	case STATUS_DEX:
		g_pRoh->SetDex(g_ii[1]);
		break;
	case STATUS_CON:
		g_pRoh->SetCon(g_ii[1]);
		break;
	case STATUS_CHA:
		g_pRoh->SetCha(g_ii[1]);
		break;
	case STATUS_MONEY:
		g_pRoh->SetMoney(n64Exp);
		break;
	case STATUS_PENALTY://1024
		g_pRoh->m_Chaotic_Num = g_ii[1];
		g_pRoh->SetPenalty();
		g_pRoh->SetDefense();	//우선은 여기에서 방어력 표시 해주기.
		if(g_pRoh->m_Chaotic_Num<=-5000)
		{
				g_pRoh->m_Chaotic_Grade = 2;
		}
		else if(g_pRoh->m_Chaotic_Num <= -10 && g_pRoh->m_Chaotic_Num >= -4999)
		{
			if(g_pRoh->m_Chaotic_Grade <10)
				g_pRoh->m_Chaotic_Grade = 1;
		}
		break;

	case STATUS_CONFIRMCODE: // 오토마우스 체크 인증 코드. 해커를 속이기 위해 status 패킷으로 들어온다.(by 원석) ///숫자입력시스템보완

		if( g_pNk2DFrame->GetAutoMouse() )
			g_pNk2DFrame->GetAutoMouse()->SetConfirmNumber( g_ii[1], g_ii[2] );

		break;

	case STATUS_CASH: ///유료상점.
		g_pRoh->SetCash( g_ii[1] );
		break;

	case STATUS_FAME:
		g_pRoh->SetFame(g_ii[1]); //후견인
		break;

	}

	//쪽시 시스템 신규쪽지 버튼
	//처음 로그인 했을때 한번 신규메시지가있는지 체크
	if(g_dwClientCountry == CTRY_KOR && pMgr->isFirstNewLetter)
	{
		pMgr->isNewLetter();
		pMgr->isFirstNewLetter = FALSE;
	}
}

void CmdAttackVital(char *arg, CUIMgr *pMgr)
{
	int hitpoint = 0, attacker = 0;
	arg = AnyOneArg(arg, g_Buf2);//p(pet) n(npc) c(player)
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);//index
	arg = AnyOneArg(arg, g_Buf);
	vital = atoi(g_Buf);//hp
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);   //캐릭터의 스페셜 공격//style
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2]= atoi(g_Buf); //계열의 해제여부.//class
	if(arg){
		arg = AnyOneArg(arg, g_Buf);
		hitpoint = atoi(g_Buf);//attack points to show
	}
	if(arg){
		arg = AnyOneArg(arg, g_Buf);
		attacker = atoi(g_Buf);//attacker idx
	}
	
	if( g_pRoh )
		g_pRoh->SetAttackType(g_ii[1]);

	if (!strncmp(g_Buf2, "n", 1))
	{
		if (pMob = pMgr->FindMobById(g_ii[0]))
		{
//			pMob->addhitpoint(hitpoint, attacker);
			if(g_ii[2]==0)
				pMob->SetClassType(g_ii[2]);

			pMob->SetAttackVital(vital);

			if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
			{
				if(vital == 0)
					pMgr->m_AutoPlay->KillMob(g_ii[0]);
			}
		}
	}
	else if (!strncmp(g_Buf2, "p", 1)) // 다른 펫이 죽었을때의 처리가 기존에 없었다. 그래서 추가(05-10-27 원석)
	{
		if( vital <= 0 ) // 현재는 죽었을 때의 처리만 필요.
		{
			if (pPet = pMgr->FindPetById(g_ii[0]))
			{
				pPet->Kill(); // 펫은 바로 죽여버린다.
			}
		}
	}
	else
	{
		// "c"
		if (pCha = pMgr->FindChaById(g_ii[0]))
		{
			if (pMgr->FindChaById(attacker))
				pCha->addhitpoint(hitpoint, attacker);
			pCha->SetAttackVital(vital);
		}
	}

	// [8/7/2007 Theodoric] 6주년 기념 이벤트 베이비 소울 :: 스페셜 공격 효과 및 에니
	if( g_pRoh && ( g_ii[1] == 1 || g_ii[1] == 2 ) )
	{
		EffectSort *TempEffect = NULL;
		for(int i=0; i<3; i++)
		{
			TempEffect = g_pRoh->FindMyFairy( SKILL_BABYSOUL_1+i);

			if(TempEffect)
			{
				bool bAction = false;
				switch( SKILL_BABYSOUL_1+i )
				{
				// 포치 :: 크리티컬, 카운트 어택
				case SKILL_BABYSOUL_1 :
					bAction = true;
					break;
				// 히치 :: 크리티컬
				case SKILL_BABYSOUL_2 :
					if(g_ii[1] == 2)	bAction = true;
					break;
				// 에치 :: 카운트 어택
				case SKILL_BABYSOUL_3 :
					if(g_ii[1] == 1)	bAction = true;
					break;
				}

				if( bAction )
				{
					TempEffect->num = FX_BABYSOUL_STATE_CRITICAL;
					TempEffect->vSortPos.y = 0.0f; // 프레임 초기화.

					EffectSort ef_sort;
					ZeroMemory(&ef_sort, sizeof(EffectSort));

					ef_sort.nType		= FX_BABYSOUL;
					ef_sort.pNkChaTo	= g_pRoh;// g_pRoh;
					ef_sort.vPos		= D3DVECTOR(TempEffect->vCurPos.x, TempEffect->vCurPos.y-20.0f, TempEffect->vCurPos.z);
					ef_sort.num			= 0;

					g_pRoh->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect(ef_sort) );

					break;
				}

				continue;
			}

		}
	}

}

void CmdKill(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if (!strncmp(g_Buf2, "n", 1))
	{
		if (pMob = pMgr->FindMobById(g_ii[0]))
			pMob->Kill();
	}
	else if(!strncmp(g_Buf2, "p", 1))
	{
		if (pPet = pMgr->FindPetById(g_ii[0]))
			pPet->Kill();
	}
	else
	{
		// "c"
		if (pCha = pMgr->FindChaById(g_ii[0]))
			pCha->Kill();
	}
}

void CmdSocial(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	if ((g_ii[1] >= 67 && g_ii[1] <= 97) || g_ii[1] == 105 || (g_ii[1] >= 153 && g_ii[1] <= 158) )
	{
		if (!strncmp(g_Buf2, "c", 1))
		{
			if (pCha = pMgr->FindChaById(g_ii[0]))
				pCha->SetMotionState(CHA_SOCIAL, g_ii[1]);
		}
	}
}

void CmdCombat(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	if (pCha = pMgr->FindChaById(g_ii[0], TRUE)) // 자기 자신도 바뀔수 있게 수정(평화모드중 공격당하면 전투모드로 바뀌게를 서버에서 판단해줌) ///자유대련 시 수정
	{
		pCha->SetCombat(g_ii[1]);
		pCha->ResetMotionState();
	}
}

//--------------------------------------------------------------------------
// CmdCharWear
//--------------------------------------------------------------------------
// 다른 캐릭이 장비를 착용할 때 오는 메세지.
// Lyul - 2004.08.31
// 상대의 무기 Gem_Num 을 적용한다. (무기 추가에 따른 파급효괏!)
void CmdCharWear(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);    // Plus 수치
	g_ii[3] = atoi(g_Buf);

	if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		pCha->Wear(g_ii[1], -1, g_ii[2], g_ii[3] );
}

void CmdCharEventWear(char *arg, CUIMgr *pMgr) ///이벤트복장착용
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf); // where
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf); // vnum
	g_ii[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);    // Plus 수치
	g_ii[3] = atoi(g_Buf);

	if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		pCha->EventWear(g_ii[1], -1, g_ii[2], g_ii[3] );
}

void CmdCharRemove(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		pCha->Remove(g_ii[1]);
}

void CmdCharEventWearRemove(char *arg, CUIMgr *pMgr) ///이벤트복장착용
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		pCha->EventWearRemove(g_ii[1]);
}

void CmdInfo(char *arg, CUIMgr *pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	SkipSpaces(&arg);
	g_pNk2DFrame->SetInfoBox(arg);
	if (g_pDSound)
		g_pDSound->Play(g_EffectSound[EFF_SND_LEVELUP]);

	g_pNk2DFrame->AddChatStr(arg, 4);
}

void CmdInfo2(char *arg, CUIMgr *pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	SkipSpaces(&arg);
	
	g_pNk2DFrame->AddChatStr(arg, 4);
}

void CmdHeroInfo(char *arg, CUIMgr *pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	SkipSpaces(&arg);
	g_pNk2DFrame->SetInfoBox2(arg);
	if (g_pDSound)
		g_pDSound->Play(g_EffectSound[EFF_SND_LEVELUP]);

	g_pNk2DFrame->AddChatStr(arg, 8);
}

void CmdNotice(char *arg, CUIMgr *pMgr) ///공지팝업창
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	g_pNk2DFrame->ShowControlNoticeWindow( TRUE, 1, arg ); // 공지 인터페이스에 스트링을 추가.(기존에 안떠있었으면 새로 띄운다.) 두번째인자 타입 1은 뫼비우스 아레나 중계용. 나중엔 패킷으로 나누자.
	if (g_pDSound)
		g_pDSound->Play(g_EffectSound[EFF_SND_LEVELUP]);
}

void CmdAt(char *arg, CUIMgr *pMgr)
{
	if( !g_pRoh )
		return;	

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	dir = 0;

	// nate 2005-08-31 : 맵 영역 밖에서 생성되는 버그때문에
	if( pCMyApp->m_nCurWorldIdx == WORLD_START )
	{
		if( !pCMyApp->IsInsideMap( x, z ) )
		{
			char strTemp[50]; ///맵영역밖생성버그
			sprintf( strTemp, "Server send wrong position [%d, %d]\n", x, z );
			if( g_pNk2DFrame )
				g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OK, 1 );

			pCMyApp->SetHeroInitPos(7066, 0, 5257, 1.5707f); // 일단 라그라미아 가운데 쪽으로 보정해준다.
		}

		///펫도움말확장
		// 이전 접속 종료때 펫 도움말을 켜놨었는지 여부를 확인, 켜놨었고, 인벤에 지혜의 서(1596)이 있다면 펫도움말을 켜준다.
		if( g_pNk2DFrame && !g_pNk2DFrame->IsHelpPetVisible() ) // 다른 존에서 온걸수도 있으니 이걸 확인해주자.
		{
			FILE *fp;
			int OnOff = 0;
			fp = fopen( "data/etc/OnOff.txt", "rt" ); // 켰었는지 껏었는지 저장.
			if( fp ) // 만약 파일이 없다면 안키면 된다.
			{
				fscanf( fp, "%d\n", &OnOff );
				fclose( fp );

				if( OnOff ) // 켜있었다면...
				{
					CItem *pItem = g_pRoh->FindItemByVnumFromAll(1596); // 인벤을 검사한다.혹시 켜놓고 아이템 팔고 재접속한 경우 펫을 끌 방법이 없어지니까.
					if( pItem ) // 아이템도 있을 경우만 켜준다.
						g_pNk2DFrame->ToggleHelpePetWindow(0); // 켜준다.
					else // 만약 켜져있었는데 아이템이 없어졌다면..
					{
						FILE *fp;
						fp = fopen( "data/etc/OnOff.txt", "wt" ); // 켰었는지 껏었는지 저장.
						fprintf( fp, "0\n" ); // 플래그를 갱신해준다.
						fclose( fp );
					}
				}
			}
		}
	}

	g_pRoh->m_nCharIndex = g_ii[0];

	pCMyApp->SetHeroInitPos((float)x, (float)y, (float)z, (float)dir);	// 카메라 설정도 필요함으로.
	pCMyApp->StopHero(FALSE);
	g_pRoh->SetMotionState(CHA_STAND);

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	ef_sort.nType = FX_TELEPORT;
	ef_sort.vPos.x = x;
	ef_sort.vPos.z = z;
	if( g_bNewTerrain )
	{
		float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( x, z );
		ef_sort.vPos.y = CNKObjQuadNode::GetHeightOnObject( z, x, y );
		if( ef_sort.vPos.y < fHeight )
			ef_sort.vPos.y = fHeight;
	}
	else
		ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( x, z );

	pCMyApp->InitTimeCtrl();
	if( pCMyApp->m_pFxSet )
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	pCMyApp->m_bPlaying = TRUE;
	pCMyApp->m_bBeforeGoWorld = FALSE;						// connect되었기 때문이다.
	// 월드간 이동에서 사용되는 플래그. 장시간 At이 안 오면 프로그램 종료함.
	pCMyApp->m_bWaitingAt = FALSE;
	// nate 2005-05-05 : 팀배틀( 깃발전 )이후 존이동시 플레그 초기화
	g_pRoh->m_nTeamMemGrade = TEAM_BATTLE_NONE;

	if( g_dev_client )
	{
		char buf[MAX_PATH] = {0,};
		sprintf(buf, "LaghaimOnline New [IP : %s][PORT : %d][UserIdx : %d][CharIdx : %d]", g_pTcpIp->GetIp().c_str(), g_pTcpIp->GetPort(), g_pRoh->m_nUserIndex, g_pRoh->m_nCharIndex);
		::SetWindowText(pCMyApp->Get_hWnd(), buf);
	}

	if( CommonConfig::Instance()->GetRenewerCharInfo() == true )
	{
		if( g_pNk2DFrame && (CControlScore*)g_pNk2DFrame->GetInterface(SCORE) )
		{
			CControlScore* pScore = (CControlScore*)g_pNk2DFrame->GetInterface(SCORE);
			pScore->LoadRes_Char(true);
		}
	}
}

void CmdGoto(char *arg, CUIMgr *pMgr)
{
	// nate 2004 - 11 : 핵 체크를 위해 세버에서 확인
	g_pTcpIp->SendNetMessage( "hack_check goto\n" );
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	y = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	dir = atof(g_Buf);

	pMgr->EraseAllList();

	//wan:2005-04	 goto 를 받았다. 이제 DASH써도 된다.
	pMgr->m_bNextGoto = TRUE;
	pCMyApp->SetHeroInitPos(x, y, z, dir, FALSE);	// 카메라 설정도 필요함으로.
	if( g_pRoh )
		g_pRoh->SetMotionState(CHA_STAND);
	pCMyApp->StopHero(FALSE);

	if( pCMyApp->m_pUIMgr->m_bAutoHunt )
	{
		if( pCMyApp->m_pUIMgr->m_AutoPlay->m_bUseTrans )
			pCMyApp->m_pUIMgr->m_AutoPlay->SetPos( g_pRoh->m_wx , g_pRoh->m_wz );
	}

	if( !pCMyApp->m_pFxSet )
		return;

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	ef_sort.nType = FX_SHOWUP2;
	ef_sort.vPos.x = x;
	ef_sort.vPos.z = z;

	if( g_bNewTerrain )
	{
		float	fHeight = pCMyApp->GetLandPtr()->GetHFHeight( x, z );
		ef_sort.vPos.y = CNKObjQuadNode::GetHeightOnObject( z, x, y );
		if( ef_sort.vPos.y < fHeight )
			ef_sort.vPos.y = fHeight;
	}
	else
		ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight( x, z );

	pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	ef_sort.nCurFrame = -2;
	pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	ef_sort.nCurFrame = -4;
	pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

	if(pMgr->m_bTeleport)
	{
		g_pTcpIp->SendNetMessage("tp\n");
		pMgr->m_bTeleport = FALSE;
	}
}

void CmdShopItem(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int item_vnum = atoi(g_Buf);

	if (g_pNk2DFrame && g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop() )
		g_pNk2DFrame->GetControlShop()->AddItem(0, item_vnum);
}

void CmdShopSPItem(char *arg, CUIMgr *pMgr) //옵션값이 추가된 아이템도 받을수있게 하는 버전 by 원석 ///BP상점
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf); // index (아이템 고유번호. 추가되는 스페셜 상점은 사고 팔때 이값을 넣어 줘야한다.)

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf); // vnum

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf); // plus 수치

	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf); // option 값

	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf); // option2 값

	arg = AnyOneArg(arg, g_Buf);
	g_ii[5] = atoi(g_Buf); // 내구도 값

	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 제한시간(시간제 아아템일 경우)

	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 가격

	arg = AnyOneArg(arg, g_Buf);
	g_ii[8] = atoi(g_Buf); // 제한 개수.

	if (g_pNk2DFrame && g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop() )
		g_pNk2DFrame->GetControlShop()->AddSPItem( 0, g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8] );
}

void CmdShopRate(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf); // 상점 타입. 이 상점이 BP상점인지, 라임상점인지등을 알려준다.(by 원석) ///BP상점

	if(g_pNk2DFrame && g_pNk2DFrame->GetControlShop()->IsLpShop() && g_pNk2DFrame->GetControlShop() )
	{
		g_pNk2DFrame->GetControlShop()->RemoveAllItems();
	}

	if (g_pNk2DFrame && g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop() )
	{
		if (g_bMatrixMatch)
			g_pNk2DFrame->GetControlShop()->SetRate(0, 0);
		else
			g_pNk2DFrame->GetControlShop()->SetRate(g_ii[0], g_ii[1], (BYTE)g_ii[2]); ///BP상점
	}
}

void CmdSpShopEnd(char *arg, CUIMgr *pMgr)
{
	if( g_pNk2DFrame && g_pNk2DFrame->GetControlShop() && g_pNk2DFrame->IsShopVisible() && g_pNk2DFrame->GetControlShop()->IsLpShop() )
	{
		g_pNk2DFrame->GetControlShop()->SetLpShopReady();
	}
}

void CmdSkillLevel(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	g_ChaSkill.SetSkillLevel(g_ii[0], g_ii[1]);
	if( g_pNk2DFrame
			&& g_pNk2DFrame->GetControlSkill())
	{
		g_pNk2DFrame->GetControlSkill()->m_pSkillInfo->SetIndex(-100);
		g_pNk2DFrame->GetControlSkill()->SetSkillEnable();
	}

	CControlScore * score = (CControlScore*)g_pNk2DFrame->GetInterface(SCORE);
	if(score)
		score->SetAllScore();
}

void CmdSkillLevelAll(char *arg, CUIMgr *pMgr)
{
	while (TRUE)
	{
		arg = AnyOneArg(arg, g_Buf);
		if (*g_Buf == '\0')
			break;
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);

		g_ChaSkill.SetSkillLevel(g_ii[0], g_ii[1]);
	}
	if( g_pNk2DFrame && g_pNk2DFrame->GetControlSkill() )
		g_pNk2DFrame->GetControlSkill()->SetSkillEnable();

	if( !g_pNk2DFrame || !g_pNk2DFrame->GetControlBottom() )
		return;
// g_ii[0]    스킬 번호
// g_ii[1]	배웠는지 유무...
	for( int i  = 0 ; i < 8 ; i++ )
	{
		if(g_ChaSkill.m_SkillNum < g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill
				|| (g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].type<0
					&& g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].type>2)
				|| (g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill!=-1
					&& g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill!=-2
					&& g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill!=-99
					&& g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill<0))
		{
			for(int j=0; j<8; j++)
			{
				g_pNk2DFrame->GetControlBottom()->m_QuickSkill[j].skill=-99;
				g_pNk2DFrame->GetControlBottom()->m_QuickSkill[j].type=0;
			}
			return;
		}

		if( g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill!=-1
				&& g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill!=-2
				&& g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill!=-99)
		{
			if(g_ChaSkill.m_SkillLevel[g_pNk2DFrame->GetControlBottom()->m_QuickSkill[i].skill]<=0)
			{
				for(int j=0; j<8; j++)
				{
					g_pNk2DFrame->GetControlBottom()->m_QuickSkill[j].skill=-99;
					g_pNk2DFrame->GetControlBottom()->m_QuickSkill[j].type=0;
				}
				return;
			}
		}
	}

	g_pRoh->SetDeactiveField();
	g_pRoh->SetDeactiveField_Ex();
	g_pRoh->SetDeactiveCritical();
	g_pRoh->SetDeactiveEngage();
	g_pRoh->SetDeactiveDSystem();
	g_pRoh->m_activate_buff_skill = 0;

}

//--------------------------------------------------------------------------
// CmdEffect
//--------------------------------------------------------------------------
// 각종 이펙트 및. 캐릭 공격에 관한 모션을 여기로 받는다. 후아~
void CmdEffect(char *arg, CUIMgr *pMgr)
{
	char szOriBuffer[1024] = {};
	strcpy( szOriBuffer, arg );

	static EffectSort ef_sort;

	arg = AnyOneArg(arg, g_Buf);	// skill index
	int skill_idx = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);	// source kind

	//----------------------------------------------------------------------
	// From 캐릭
	if (!strncmp(g_Buf, "c", 1))
	{
		if ((skill_idx >= 0 && skill_idx < MAX_SKILL && g_Skills[skill_idx].m_Kind == SKILL_KIND_TO)
				|| skill_idx == SKILL_F_DASHSLASH || skill_idx == SKILL_F_SLASHSHADOW
				|| skill_idx == SKILL_F_STONEBASH1 || skill_idx == SKILL_F_STONEBASH2
				|| skill_idx == SKILL_F_BASHREFLECT1 || skill_idx == SKILL_F_BASHREFLECT2 )  // 일부 스킬 예외처리(SKILL_KIND_TO가 아닌녀석들) ///하이콤보
		{
			arg = AnyOneArg(arg, g_Buf);	// source index
			g_ii[0] = atoi(g_Buf);

			int source_index = g_ii[0];
			int target_index = -1;

			//서먼피션일때는 자기한테 이펙트 보이게
			if (pCha = pMgr->FindChaById(g_ii[0], pMgr->IsRohSkill( skill_idx )))
			{
				// from Player 일 경우
				// !!!! 임시. 스킬이 유얼 어썰트일 경우 모션을 변경해야 한다.
				if(pCha == g_pRoh && skill_idx==SKILLF_C_DASSAULT)
					g_pRoh->SetMotionState(CHA_F_DASSAULT);

				if(pCha == g_pRoh && skill_idx==SKILL_SUMMON_PARTITION) ///07-04-10 원석 : 서먼피션 쿨타임 딜레이가 서버랑 안맞는게 일본에서 말이 많아서 여기서 쿨타임 돌리게 변경했다.
					G_SK_SetCooltime(SKILL_SUMMON_PARTITION);

				arg = AnyOneArg(arg, g_Buf);	// target kind
				//----------------------------------------------------------------------
				// To 캐릭 From 캐릭
				if(!strncmp(g_Buf, "c", 1))
				{
					arg = AnyOneArg(arg, g_Buf);	// target index
					g_ii[0] = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);	// target vital
					g_ii[1] = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);	// special attack
					g_ii[2] = atoi(g_Buf);//1022
					pCha->SetAttackType(g_ii[2]);//특수공격 1022
					arg = AnyOneArg( arg, g_Buf );	// nate 2005-06-27 : npc effect와 패킷수 동기화위한 쓰레기 값
					arg = AnyOneArg( arg, g_Buf );	// nate 2005-06-17 : Skill Step
					g_ii[ 3 ] = atoi( g_Buf );
					arg = AnyOneArg( arg, g_Buf );	// nate 2005-06-17 : Skill Point Value
					g_ii[ 4 ] = atoi( g_Buf );

					target_index = g_ii[ 4 ];

					if (pCha2 = pMgr->FindChaById(g_ii[0], TRUE))
					{
						if (pCha2 != g_pRoh)
							pCha2->SetAttackVital(g_ii[1]);
						pCha->m_nSkillLevel = g_ii[ 4 ];			// 캐릭터 스킬 포인트 값
						pCha->SetSkillStep( g_ii[ 3 ] );			// 동작 구분
						pCha->SetAttackChaId(pCha2->m_nCharIndex);	// 공격 대상 저장
						pCha->SetMagicCha(pCha2, skill_idx );	// 이펙트 셋팅
					}
				}
				//----------------------------------------------------------------------
				// To 몹 from 캐릭
				// 서먼 피션일 경우 손소환몹으로 보내준다.
				else
				{
					// pCha To pMob
					arg = AnyOneArg(arg, g_Buf);	// target index
					g_ii[0] = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);	// target vital
					g_ii[1] = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);	// special attack
					g_ii[2] = atoi(g_Buf);			// 카운터 크리티컬 등의 특수타격 알림
					pCha->SetAttackType(g_ii[2]);
					arg = AnyOneArg(arg, g_Buf);	// Mob Class
					g_ii[3] = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);	// Skill Step
					g_ii[4] = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);	// Skill Level
					g_ii[5] = atoi(g_Buf);

					target_index = g_ii[ 0 ];


					if (pMob = pMgr->FindMobById(g_ii[0]))
					{
						pMob->SetAttackVital(g_ii[1]);
						if(g_ii[3]==0)
							pMob->SetClassType(g_ii[3]);
						pCha->m_nSkillLevel = g_ii[ 5 ];			// 캐릭터 스킬 포인트 값
						pCha->SetSkillStep( g_ii[ 4 ] );			// 동작 구분
						pCha->SetMagicMob(pMob, skill_idx);		// 이펙트 셋팅
						pCha->SetAttackMobId(pMob->m_nVirId);	// 공격 대상 저장

						if(skill_idx==73)
							pMob->SetFissionType(1);
					}
				}
			}

			if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
			{
				pMgr->m_AutoPlay->SetSurport( source_index , target_index );
			}

		}
	}
	//wan:2004-12	화이트혼 던젼 보스몹들의 공격
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		if (pMob = pMgr->FindMobById(g_ii[0]))
		{
			if( pMob->m_nMobNum == 430 || pMob->m_nMobNum == 432 )
			{
				pMgr->InsertMagic(NULL, pMob, NULL, NULL, skill_idx);
				return;
			}

			arg = AnyOneArg(arg, g_Buf);
			//중고렙스킬쓸때: attack2 모션
			if(skill_idx == 71 || skill_idx == 72 || skill_idx == 73 || skill_idx == 74 )
				pMgr->m_bMultiAttack = TRUE;
			if (!strncmp(g_Buf, "c", 1))
			{
				// pMob To pCha2
				arg = AnyOneArg(arg, g_Buf);
				g_ii[0] = atoi(g_Buf);
				arg = AnyOneArg(arg, g_Buf);
				g_ii[1] = atoi(g_Buf);
				arg = AnyOneArg(arg, g_Buf);	// special attack 쓰지 않는다.
				arg = AnyOneArg( arg, g_Buf );	// nate 2005-06-27 : npc effect와 패킷수 동기화위한 쓰레기 값
				arg = AnyOneArg( arg, g_Buf );	// Skill Step 몹이나 케릭한테 맞았을때 헬다운 취소하기 위해
				g_ii[2] = atoi( g_Buf );
				if (pCha2 = pMgr->FindChaById(g_ii[0], TRUE))
				{
					pCha2->SetFailStep( g_ii[2] );
					if (pCha2 != g_pRoh)
						pCha2->SetAttackVital(g_ii[1]);
					pMob->Attack(pCha2->m_wx,pCha2->m_wz);
					pMgr->InsertMagic(NULL, pMob, pCha2, NULL, skill_idx);
				}
			}
			else
			{
				// pMob To pMob2
				arg = AnyOneArg(arg, g_Buf);
				g_ii[0] = atoi(g_Buf);
				arg = AnyOneArg(arg, g_Buf);
				g_ii[1] = atoi(g_Buf);
				if (pMob2 = pMgr->FindMobById(g_ii[0]))
				{
					if(skill_idx==73)
						pMob2->SetFissionType(1);
					pMob->Attack(pMob2->m_wx,pMob2->m_wz);
					pMob->SetAttackVital(g_ii[1]);
				}
			}
		}
	}
}


void CmdGoWorld(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);			// ip_address

	arg = AnyOneArg(arg, g_Buf2);
	g_ii[0] = atoi(g_Buf2);					// port

	arg = AnyOneArg(arg, g_Buf2);			// world number
	if (g_Buf2[0] == NULL)
		g_ii[1] = 0;
	else
		g_ii[1] = atoi(g_Buf2);

	arg = AnyOneArg(arg, g_Buf2);
	if (g_Buf2[0] == NULL)
		g_ii[2] = 0;
	else
		g_ii[2] = atoi(g_Buf2);

	if( g_pRoh->GetMyGuildData() )
		g_pRoh->DeleteMyGuildData();

	g_pNk2DFrame->SetInfoBox_NextString("");
	g_pNk2DFrame->SetInfoBox_NextString2("");

	if( pMgr )
		pMgr->GoWorld(g_Buf, g_ii[0], g_ii[1], g_ii[2]);
}

void CmdGoWorldFail(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->GoWorldFail();
}

void CmdExch(char *arg, CUIMgr *pMgr) // Message "exch"
{
	arg = AnyOneArg(arg, g_Buf);	// lock, trade, change, cancel
	if( pMgr )
		pMgr->Exch(g_Buf);
}

// 다른 유저에게서 거래 요청이 들어왔다.
void CmdExchAsk(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);	// 유저 이름.
	if( pMgr )
		pMgr->AskExchange(g_Buf);
}

void CmdExchStart(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);	// 교환시작하라는 유저 이름.
	arg = AnyOneArg(arg, g_Buf1);	// 교환시작하라는 유저 레벨.
	g_ii[0] = atoi(g_Buf1);
	if( pMgr )
		pMgr->ExchStart(g_Buf, g_ii[0]);		// 조건에 맞으면 교환창을 띄운다
}

// exch_inven_to index vnum x y
void CmdExchInvenTo(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				//index
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);				// vnum
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);				// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);				// y
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);				// y
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);				// y

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[5] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[7] = atoi(g_Buf);		// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[8] = atoi(g_Buf);		// 일반 내구도 Max
	}

	if( pMgr )
	{
		if( g_SvrType == ST_ADULT_ONLY )
			pMgr->ExchInvenTo(g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8] ); ///신방어구강화시스템
		else
			pMgr->ExchInvenTo(g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], g_ii[4], g_ii[5], g_ii[6], 0, 0 ); ///신방어구강화시스템
	}

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
		g_pNk2DFrame->GetControlInven()->ResetItemInfo();/// 정보 갱신.

	if( g_pNk2DFrame && g_pNk2DFrame->IsHelpPetVisible() && g_pNk2DFrame->GetControlHelpPet() ) ///펫도움말 의 발동 조건을 체크한다.
		g_pNk2DFrame->GetControlHelpPet()->CheckHelpCondition( 2, g_ii[1], TRUE ); // 아이템 습득 조건의 헬프가 있는지 체크한다.
}

void CmdExchInvenFrom(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);				// y

	if( pMgr )
		pMgr->ExchInvenFrom(g_ii[0], g_ii[1]);
}

void CmdExchMoney(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	if( pMgr )
		pMgr->ExchMoney(g_Buf);
}

void CmdDelivStart(char *arg, CUIMgr *pMgr)
{}

void CmdDeliv(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);					// index
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);					// vnum
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);					// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);					// y

	if( !g_pRoh )
		return;
	// 위치를 파악하여 기존 아이템을 삭제한다.
	g_pRoh->DelFromExgInven(g_ii[2], g_ii[3]);
	// 교환창에 새 아이템을 넣는다.
	g_pRoh->AddToExgInven(g_ii[2], g_ii[3], g_ii[0], g_ii[1], 0, 0, 0, 0, 0, 0, TRUE); ///신방어구강화시스템
}

void CmdDelivEnd(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->CheckCarnation(_DELIV_SUCCEED);
}

void CmdDelivFail(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->CheckCarnation(_DELIV_FAIL);
}

void CmdMs(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );		/// 구분 플레그 - 0:정제, 1:가공
	g_ii[ 0 ] = atoi( g_Buf );

	arg = AnyOneArg(arg, g_Buf);		// 상태 구분

	if (!strcmp(g_Buf, "request"))
	{
		arg = AnyOneArg(arg, g_Buf);	// 요청자 이름
		if( pMgr )
			pMgr->CmdMsRequest(g_Buf);
	}
	else if (!strcmp(g_Buf, "start"))
	{
		arg = AnyOneArg(arg, g_Buf);	// 0: 요청한 사람, 1: 정제해줄 사람에게.
		g_ii[ 1 ] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);	// 상대방 이름
		if( pMgr )
			pMgr->CmdMsStart( g_ii[ 1 ], g_Buf, g_ii[ 0 ] );
	}
	else if (!strcmp(g_Buf, "money"))
	{
		arg = AnyOneArg( arg, g_Buf );	// 금액
		if( pMgr )
			pMgr->MsMoney( g_Buf, g_ii[ 0 ] );
	}
	else if (!strcmp(g_Buf, "success"))	// 성공
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// clear flag
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// index
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// vnum
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// x
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// y

		if( pMgr )
			pMgr->MsSuccess( g_ii[1], g_ii[3], g_ii[2], g_ii[4] ,g_ii[5], g_ii[ 0 ] );
	}
	else if (!strcmp(g_Buf, "fail"))	// 실페
	{
		if( pMgr && g_pNk2DFrame )
		{
			pMgr->MsFail( g_ii[ 0 ] );
			g_pNk2DFrame->ShowRefineWindow( false );
		}
	}
	else if (!strcmp(g_Buf, "cancel"))	// 가공 취소는 ms cancel로 온다.
	{
		if( pMgr && g_pNk2DFrame )
		{
			g_pNk2DFrame->ShowProcessWindow(FALSE, 1);
		}
	}
}

// exch_inven_to index vnum x y
void CmdMsInvenTo(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );		/// 구분 플레그 - 0:정제, 1:가공
	int nSection = atoi( g_Buf );

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			//index
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// vnum
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);			// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);			// y
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);

	if( g_SvrType == ST_ADULT_ONLY ) // [6/7/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// 강화 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// 강화 내구도 Max
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[7] = atoi(g_Buf);			// 일반 내구도 Max
	}

	if( pMgr )
	{
		if( g_SvrType == ST_ADULT_ONLY )
			pMgr->MsInvenTo(g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], nSection, g_ii[4], g_ii[5], g_ii[6], g_ii[7]);
		else
			pMgr->MsInvenTo(g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], nSection, 0, 0, 0, 0 );
	}
}

void CmdMsInvenFrom(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );		/// 구분 플레그 - 0:정제, 1:가공
	int nSection = atoi( g_Buf );

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// y

	if( pMgr )
		pMgr->MsInvenFrom(g_ii[0], g_ii[1], nSection );
}

// 마석 옵션 추가
// 0 : 추가 실패
// 1 : 추가 성공
// 2 : 다운 그레이드.
void CmdMsUpgrade(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);

	if( pMgr )
		pMgr->CmdMsUpgrade(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4]);
	if( g_pRoh )
		g_pRoh->SetDefense();//1114 마석업그레이드로 인한 방어력 상승했으므로 다시 표시
}

void CmdIChange(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// old_idx
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);				// new_idx
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);				// vnum
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);				// plus_point (x)
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);				// special (y)

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[5] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 맥스 강화 내구도.

	if( g_SvrType == ST_ADULT_ONLY )// [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[8] = atoi(g_Buf);		// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);		// 일반 내구도 Max
	}

	if( pMgr )
	{
		if( g_SvrType == ST_ADULT_ONLY )
			pMgr->InterchangeTo(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8], g_ii[9] ); ///신방어구강화시스템
		else
			pMgr->InterchangeTo(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], g_ii[7], 0, 0 ); ///신방어구강화시스템
	}
}

void CmdWorked(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// slot_num
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);				// 0은 실패, 1은 성공.

	if (g_ii[1] == 0)
	{
		pMgr->FailedWork(g_ii[0]);
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// 원석 Index (지울것)
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// 보석 Index (생성할것)
		pMgr->SuccessWork(g_ii[0], g_ii[2], g_ii[3]);
	}
}

void CmdUpgrade(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// 0은 실패, 1은 성공
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);				// where
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);				// Item Index
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);				// 보석 갯수.

	if( pMgr )
		pMgr->Upgrade(g_ii[0], g_ii[1], g_ii[2], g_ii[3]);
	if( g_pRoh )
		g_pRoh->SetDefense();//1114 업그레이드로 인해 방어력 상승했으므로 다시 표시
}

void CmdParty(char *arg, CUIMgr *pMgr) ///파티개편.
{
	if( !g_pNk2DFrame )
		return;

	char strTemp[200];
	arg = AnyOneArg(arg, g_Buf);

	if( !strcmp(g_Buf, "request") ) // 요청을 받을때.. 모드에 따라 파티장이 받는 경우와 신입이 받는 경우로 갈린다.
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 신청모드. 경우의 수에 따라 모드가 바뀐다.

		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf); // 신청한 사람의 인덱스
		arg = AnyOneArg(arg, g_Buf1); // 신청한 사람의 이름

		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf); // 파티장의 인덱스
		arg = AnyOneArg(arg, g_Buf2); // 파티장의 이름


		// 파티 거부가 되어있을땐 자동 취소.
		if( g_pNk2DFrame->GetControlParty()
				&& g_pNk2DFrame->GetControlParty()->m_RequestRejection.GetToggleState() )
		{
			char strTemp[ 100 ];

			if( g_pNk2DFrame->GetControlParty()->m_bIsMaster ) // 파티장만 dismiss
				sprintf( strTemp, "party dismiss %d 1\n", g_ii[1] ); // 뒤에 1은 자동취소(파티거부) 일때 셋팅.
			else
				sprintf( strTemp, "party secession %d 1\n", g_ii[1] ); // 뒤에 1은 자동취소(파티거부) 일때 셋팅.

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( strTemp );

			return; // request 처리를 하지 않는다.
		}

		g_pNk2DFrame->m_PartyReadyIndex = g_ii[1];

		switch( g_ii[0] ) // 현재 신청모드는 7개. 모드에따라 받는 인자의 숫자 및 메세지 박스의 코멘트가 바뀐다.
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:

			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf); // 경험치 분배 방식
			arg = AnyOneArg(arg, g_Buf);
			g_ii[4] = atoi(g_Buf); // 아이템 입수 모드

			if( pMgr->m_AutoPlay )
			{
				if( pMgr->m_AutoPlay->IsRun() )
				{
					if( pMgr->m_AutoPlay->IsAutoParty() )
					{
						char msgbuf[256] = {0,};
						sprintf(msgbuf , "party consist %d %d\n", g_ii[0], g_ii[1]);
						LH_SEND_NET_MSG( msgbuf );
						return;
					}
					else
					{
						if( g_pNk2DFrame->GetControlParty() && g_pNk2DFrame->GetControlParty()->m_PartyState == PARTY_STATE_MEMBER )
						{
							if( g_pNk2DFrame->GetControlParty()->m_bIsMaster )
							{
								char msgbuf[256] = {0,};
								sprintf(msgbuf , "party dismiss %d 0\n", g_ii[1]);
								LH_SEND_NET_MSG( msgbuf );
								return;
							}
						}

						char msgbuf[256] = {0,};
						sprintf(msgbuf , "party secession %d 0\n", g_ii[1]);
						LH_SEND_NET_MSG( msgbuf );
						return;
					}
				}
			} 

			if( g_pNk2DFrame->GetControlParty() )
				g_pNk2DFrame->GetControlParty()->AskParty( g_ii[0], g_Buf1, g_ii[2], g_Buf2, g_ii[3], g_ii[4] );

			break;

		case 5:
		case 6:
			if( pMgr->m_AutoPlay )
			{
				if( pMgr->m_AutoPlay->IsRun() )
				{
					if( pMgr->m_AutoPlay->IsAutoParty() )
					{
						char msgbuf[256] = {0,};
						sprintf(msgbuf , "party consist %d %d\n", g_ii[0], g_ii[1]);
						LH_SEND_NET_MSG( msgbuf );
						return;
					}
					else
					{
						if( g_pNk2DFrame->GetControlParty() && g_pNk2DFrame->GetControlParty()->m_PartyState == PARTY_STATE_MEMBER )
						{
							if( g_pNk2DFrame->GetControlParty()->m_bIsMaster )
							{
								char msgbuf[256] = {0,};
								sprintf(msgbuf , "party dismiss %d 0\n", g_ii[1]);
								LH_SEND_NET_MSG( msgbuf );
								return;
							}
						}

						char msgbuf[256] = {0,};
						sprintf(msgbuf , "party secession %d 0\n", g_ii[1]);
						LH_SEND_NET_MSG( msgbuf );
						return;
					}
				}
			}
 
			if( g_pNk2DFrame->GetControlParty() )
				g_pNk2DFrame->GetControlParty()->AskParty( g_ii[0], g_Buf1, g_ii[2], g_Buf2 );

			break;
		}

	}
	else if( !strcmp(g_Buf, "list") ) // 파티 멤버 리스트 갱신, 누가 새로 들어왔을때, 누가 나갔을때 리스트가 새로 들어온다.
	{
		// 이 패킷을 받은 시점에서 멤버들의 리스트를 싹 지운다.
		g_pNk2DFrame->GetControlParty()->RemoveMemberAll(); // 일단 다 지운다.(나중엔 업데이트 플래그를 셋팅해줘야한다.(안그러면 리스트가 실시간으로 막 바뀔지도.)
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 파티원의 수.
		g_pNk2DFrame->GetControlParty()->SetParty( TRUE );
		g_pRoh->m_bIsParty = TRUE;
	}
	else if( !strcmp(g_Buf, "list_end") ) // 파티 멤버 리스트 갱신, 누가 새로 들어왔을때, 누가 나갔을때 리스트가 새로 들어온다.
	{
		g_pNk2DFrame->GetControlParty()->SetPartyCount();
	}
	else if( !strcmp(g_Buf, "member") ) // 파티 멤버 리스트 갱신, 한명씩 이 패킷으로 들어온다.
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 파티원의 인덱스
		arg = AnyOneArg(arg, g_Buf); // 파티원의 캐릭명
		g_pNk2DFrame->GetControlParty()->AddMember(g_ii[0], g_Buf);
	}
	else if( !strcmp(g_Buf, "dismiss") ) // 누군가가 해고 당했을때. 자기자신이 해고 당했을때와 해고당한 사람의 이름을 출력해주기위해 사용하자.
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 해고된 사람의 인덱스(자기 자신인지도 체크해야한다.)

		if( g_ii[0] == g_pRoh->m_nCharIndex && g_pRoh->m_bIsParty ) // 자기 자신이 강퇴당한 경우. (파티가 있을 경우에만 처리)
		{
			g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
			g_pNk2DFrame->GetControlParty()->SetParty( FALSE );
			g_pRoh->m_bIsParty = FALSE;
			//IDS_PARTY_DISMISS_YOU : 당신은 파티에서 해고 당하셨습니다.
			sprintf( strTemp, G_STRING(IDS_PARTY_DISMISS_YOU) );
			g_pNk2DFrame->AddChatStr( strTemp, -1 );
		}
		else
		{
			//IDS_PARTY_DISMISS : %s 님이 파티에서 해고 당하셨습니다.
			sprintf( strTemp, G_STRING(IDS_PARTY_DISMISS), g_pNk2DFrame->GetControlParty()->GetMemberName(g_ii[0]) );
			g_pNk2DFrame->AddChatStr( strTemp, -1 );
		}
	}
	else if( !strcmp(g_Buf, "disband") ) // 해산. 파장이 해산을 눌렀을 때.
	{
		// 우리 파티 해체됬엉~~~
		if( g_pRoh->m_bIsParty ) // 파티가 있을 경우에만 처리
		{
			g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
			g_pNk2DFrame->GetControlParty()->SetParty( FALSE );
			g_pRoh->m_bIsParty = FALSE;

			char strTemp[200];
			sprintf( strTemp, G_STRING(IDS_PARTY_DISBAND) );
			// IDS_PARTY_DISBAND : 파티가 해체되었습니다.
			g_pNk2DFrame->AddChatStr( strTemp, -1 );
		}
	}
	else if( !strcmp(g_Buf, "change") ) // 경험치, 아이템 입수 방식 변경(또는 최초 결성시) 시 오느 패킷.(현재는 변경은 없고 최초 결성시에만 온다.)
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 뭘 바꾸는지...

		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf); // 바뀔 값.

		switch( g_ii[0] )
		{
		case 1: // 경험치 분배 방식.
			g_pNk2DFrame->GetControlParty()->m_ExpGetRule.SetSelect( g_ii[1] );
			break;

		case 2: // 아이템 배분 방식.
			g_pNk2DFrame->GetControlParty()->m_ItemGetRule.SetSelect( g_ii[1] );
			break;
		}
	}
	else if( !strcmp(g_Buf, "secession") ) // 누군가 나갔을 경우.
	{
		// 다른 사람이 나갔을 경우는 그냥 무시해도 된다.(어차피 멤버 리스트를 새로 받아 갱신함.)
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 인덱스

		if( g_ii[0] == g_pRoh->m_nCharIndex && g_pRoh->m_bIsParty ) // 자기 자신이면...
		{
			// 초기화를 해준다.
			g_pNk2DFrame->GetControlParty()->RemoveMemberAll();
			g_pNk2DFrame->GetControlParty()->SetParty( FALSE );
			g_pRoh->m_bIsParty = FALSE;

			char strTemp[200];
			sprintf( strTemp, G_STRING(IDS_PARTY_DISBAND) );
			// IDS_PARTY_DISBAND : 파티가 해체되었습니다.
			g_pNk2DFrame->AddChatStr( strTemp, -1 );
		}
	}
}
/*void CmdOtherVital(char *arg, CUIMgr *pMgr) //desativado
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf); //idx
	arg = AnyOneArg(arg, g_Buf);
	__int64 vit = _atoi64(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	__int64 max_vit = _atoi64(g_Buf);
	

	if (pCha = pMgr->FindChaById(g_ii[0], TRUE))
		{
			pCha->m_Vital = (long double)vit;
			pCha->m_MaxVital = (long double)max_vit;
		}
	
	


	
}*/
void CmdPv(char *arg, CUIMgr *pMgr)
{
	if( g_pRoh )
		g_pRoh->GetPvInfo(arg);
}

// skill <1/0> <char/npc> index skill_index
void CmdSkill(char *arg, CUIMgr *pMgr)
{
	BOOL bSucess = FALSE;
	BOOL bChar = FALSE;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// 0은 실패, 1은 성공
	arg = AnyOneArg(arg, g_Buf);		// npc or char
	arg = AnyOneArg(arg, g_Buf2);
	g_ii[1] = atoi(g_Buf2);				// npc or char index
	arg = AnyOneArg(arg, g_Buf2);
	g_ii[2] = atoi(g_Buf2);				// skill_index
	if( g_ii[2] == SKILLF_C_STONEBRUST)
	{
		//mungmae-2005/05/10 스톤버스트 이펙트 추가
		CNkMob *pMob = pMgr->FindMobById(g_ii[1]);
		pMgr->InsertMagic(NULL, pMob, NULL, NULL, EFFECT_SKILL_STONEBRUST);
	}

	if (strncmp(g_Buf, "c", 1) == 0)
		bChar = TRUE;
	else
		bChar = FALSE;

	pMgr->SkillOnOff(g_ii[0], bChar, g_ii[1], g_ii[2]);
}

//==============================
// nate 2004 - 4
// CastleWar
// 길드원 그림자 표시 버그 수정
void CmdGuildName(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// 0은 실패, 1은 성공
	arg = AnyOneArg(arg, g_Buf);		// npc or char

	CNkCharacter* pChar = NULL;
	LHCharacterMap_it prChar;

	if( !g_pRoh )
		return;

	if( g_pRoh->m_nCharIndex == g_ii[ 0 ] )
	{
		g_pRoh->SetGuildName( g_Buf );

		if( g_pNk2DFrame->GetControlGuild() )
			g_pNk2DFrame->GetControlGuild()->ChangeGuildInfo(g_Buf);

		if( strlen( g_Buf ) <= 0 )
			g_pRoh->m_GuildIndex = 0;

		for( prChar = pMgr->m_character_map.begin() ; prChar != pMgr->m_character_map.end() ; prChar ++ )
		{
			pChar = (*prChar).second;

			if( g_pRoh->m_pstrGuildName && pChar->m_pstrGuildName )
			{
				if( strcmp( g_pRoh->m_pstrGuildName, pChar->m_pstrGuildName ) )
					pChar->SetMyGuild( FALSE );
				else
					pChar->SetMyGuild( TRUE );
			}
			else
				pChar->SetMyGuild( FALSE );
		}
	}
	else
	{
		if( pChar = pMgr->FindChaById( g_ii[ 0 ], TRUE ) )
		{
			pChar->SetGuildName( g_Buf );

			/*      // 월드컵 이벤트 때 뫼비우스 맵에서 길드 이름으로 인덱스를 바꿔주는 코드. 해외를 고려안했기에 돌림. (서버에서 축구경기장과 뫼비우스 전장을 구분해 주기로 했음)
						if( g_bMatrixSvr ) // 뫼비우스는 길드 인덱스를 따로 처리해주기로 했음.
						{
							if(!strcmp(g_Buf, "호크"))
								pChar->m_GuildIndex = 1;
							else if(!strcmp(g_Buf, "쿨레인"))
								pChar->m_GuildIndex = 2;
							else if(!strcmp(g_Buf, "쉬버"))
								pChar->m_GuildIndex = 3;
							else
								pChar->m_GuildIndex = 0;
						}
			*/
			if( strlen( g_Buf ) <= 0 )
				pChar->m_GuildIndex = 0;

			if( g_pRoh->m_pstrGuildName && g_Buf )
			{
				if( strcmp( g_pRoh->m_pstrGuildName, g_Buf ) )
					pChar->SetMyGuild( FALSE );
				else
					pChar->SetMyGuild( TRUE );
			}
			else
				pChar->SetMyGuild( FALSE );

		}
	}	
}

void CmdGuildGrade(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);		// 캐릭터 인덱스
	g_ii[0] = atoi(g_Buf);

	if (pCha = pMgr->FindChaById(g_ii[0], TRUE))
	{
		arg = AnyOneArg(arg, g_Buf);	// 길드 고유 인덱스
		g_ii[1] = atoi(g_Buf);

		if( arg[1] == ' ' )
		{
			sprintf(g_Buf3, "");
		}
		else
		{
			arg = AnyOneArg(arg, g_Buf3);	// 길드 등급 이름
		}

		arg = AnyOneArg(arg, g_Buf);	// 길드 등급 인덱스
		g_ii[2] = atoi(g_Buf);			// 길마 : 10, 부길마 : 9, 부장 : 7, 길드원 : 1

		pCha->m_GuildIndex = g_ii[1];
		
		pMgr->isguildmark(g_ii[1]);
		pCha->SetGuildGrade(g_Buf3);

		if( g_MatrixSvr  != 0 )
		{
			if (g_ii[1] < 0)
			{
				g_ii[1] *= -1;
				pCha->ShowMatrixMark(TRUE);
			}
			else
				pCha->ShowMatrixMark(FALSE);

			pCha->SetGuildIndex( g_ii[1]);
		}
		else
		{
			pCha->ShowMatrixMark(FALSE);
			pCha->SetGuildIndex( g_ii[1]);
		}

//=========================================
		// nate 2004 - 5
		// m_GuildLevel과 m_GuildGradeIndex의 성질이 같은 것으로 보인다.
		pCha->m_GuildLevel = pCha->m_GuildGradeIndex = g_ii[2];
//=========================================
	}
}

//--------------------------------------------------------------------------
// CmdMultiShot
//--------------------------------------------------------------------------
// 휴먼 연사 및 저격술
// 2005.01.20 / Lyul
// 저격의 경우 -1로 온다ㅏ.
void CmdMultiShot(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	// Other Character : 스나이핑 처리 있음
	if (!strncmp(g_Buf, "c", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);

		if (pCha = pMgr->FindChaById(g_ii[0], FALSE))
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[1] = atoi(g_Buf);

			// 연사횟수
			if (g_ii[1] >= 2)
			{
				pCha->SetNextMultiShot(g_ii[1] - 1);
			}
			else if (g_ii[1] == -1)
			{
				pCha->mb_Snifing = true;
			}
			else if (g_ii[1] < 0)
			{
				pCha->SetSnifing(TRUE);
			}
		}
	}
	// Player : 스나이핑 처리 없음
	else if (!strcmp(g_Buf, "me"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);

		// 연사 횟수
		if (g_pRoh && g_ii[1] >= 2)
			g_pRoh->SetNextMultiShot(g_ii[1] - 1);
	}
}

const int FE_TYPE_DROP = 2;
const int FE_TYPE_ATT = 3;
const int FE_TYPE_DEF = 4;
const int FE_TYPE_HP = 5;
const int FE_TYPE_EXP = 6;
const int FE_TYPE_LUCKY = 9;
const int FE_TYPE_LESS_EXP = 10;

void CmdEvent(char *arg, CUIMgr *pMgr)
{
	strcpy(g_Buf2, arg);

	arg = AnyOneArg(arg, g_Buf);

	// Event 패킷을 통한 버프 효과 표시 기능 추가. ///이벤트버프표시
	if(!strcmp(g_Buf, "drop"))
	{
		arg = AnyOneArg(arg, g_Buf);

		g_pNk2DFrame->SetEventType(FE_TYPE_DROP, atoi(g_Buf));

		return;
	}
	else if(!strcmp(g_Buf, "att"))
	{
		arg = AnyOneArg(arg, g_Buf);

		g_pNk2DFrame->SetEventType(FE_TYPE_ATT, atoi(g_Buf));
		return;
	}
	else if(!strcmp(g_Buf, "def"))
	{
		arg = AnyOneArg(arg, g_Buf);

		g_pNk2DFrame->SetEventType(FE_TYPE_DEF, atoi(g_Buf));
		return;
	}
	else if(!strcmp(g_Buf, "hp"))
	{
		arg = AnyOneArg(arg, g_Buf);

		g_pNk2DFrame->SetEventType(FE_TYPE_HP, atoi(g_Buf));
		return;
	}
	else if(!strcmp(g_Buf, "exp"))
	{
		arg = AnyOneArg(arg, g_Buf);

		int value = atoi(g_Buf);
		
		if( CommonConfig::Instance()->GetEventLessExp() == false )
		{
			g_pNk2DFrame->SetEventType(FE_TYPE_EXP, value);
		}
		else
		{
			if (value > 0 && value < 10)
			{
				g_pNk2DFrame->SetEventType(FE_TYPE_EXP, value);
				g_pNk2DFrame->SetEventType(FE_TYPE_LESS_EXP, 0);
			}
			else if (value > 100 && value < 110)
			{
				g_pNk2DFrame->SetEventType(FE_TYPE_LESS_EXP, value - 100);
				g_pNk2DFrame->SetEventType(FE_TYPE_EXP, 0);
			}
			else if (value == 0)
			{
				g_pNk2DFrame->SetEventType(FE_TYPE_EXP, 0);
				g_pNk2DFrame->SetEventType(FE_TYPE_LESS_EXP, 0);
			}
		}
		return;
	}

	else if(!strcmp(g_Buf, "luckey")) //형식 event lucky 1  ,  event lucky 0
	{
		arg = AnyOneArg(arg, g_Buf);

		g_pNk2DFrame->SetEventType(FE_TYPE_LUCKY, atoi(g_Buf));
		return;
	}

	else if(!strcmp(g_Buf, "event"))
	{
		g_pTcpIp->SendNetMessage( g_Buf2 );
		return;
	}

	else
	{
		if( pMgr )
			pMgr->CmdEvent(g_Buf2);
	}
}

void CmdVoiceChat(char *arg, CUIMgr *pMgr)
{
}

void CmdYouare(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if (g_ii[0] > 2)
	{
		g_bAdmin2 = TRUE;
		pCMyApp->m_bFastMove = TRUE;
	}
}
//-----------------------------------------
// 공성전 관련 명령어
//-----------------------------------------
void CmdOpenCastle(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if( pMgr )
		pMgr->OpenCastle(g_ii[0]);
}

void CmdCloseCastle(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->CloseCastle();
}

// nate 2004 - 11
// 공성시 화이트혼의 외성문이 열릴때
// 사일론의 수호탑이 깨질때
void CmdOpenDoor(char *arg, CUIMgr *pMgr)
{
	int i;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );

	if( g_ii[ 0 ] == 7 )  		// 화이트혼
	{
		for ( i = 0 ; i < 8 ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ i ] = atoi( g_Buf );
		}
		if( pMgr )
			pMgr->OpenDoor( 7, g_ii );
	}
	else if( g_ii[ 0 ] == 3 )  	// 샤일론
	{
		for( i = 0 ; i < 4 ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ i ] = atoi( g_Buf );
		}
		if( pMgr )
			pMgr->OpenDoor( 3, g_ii );
	}
	// wan:2004-12 : whitehorn Dungeon
	else if( g_ii[ 0 ] == 8 )  	// 화이트혼 던젼
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf );
		if( pMgr )
			pMgr->OpenDoor( 8, g_ii );
	}
	else if( g_ii[ 0 ] == 0 ) // 데카르디 추가(by 원석)
	{
		for ( i = 0 ; i < 4 ; i ++ ) // 현재 데카르디의 수호탑은 4개
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ i ] = atoi( g_Buf );
		}
		if( pMgr )
			pMgr->OpenDoor( 0, g_ii );
	}
}

//=======================================
// nate 2004 - 4 : CastleWar
void CmdOnBattle(char *arg, CUIMgr *pMgr)
{
	if( pMgr == NULL )
		return;

	// nate 2005-03-11 : Matrix - 스톤의 상태값
	if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX )
	{
		// wan_mat : 패킷의 맨 앞에 공성 플래그 하나 추가
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf); // 공성 중이냐 아니냐.
		if(g_ii[0])
			pMgr->m_bOnBattle = TRUE;
		else
		{
			pMgr->m_bOnBattle = FALSE;
			g_bFireTower = 0;
			g_bFireTower2 = 0;
			g_bFireTower3 = 0;
		}

		for( int i = 0 ; i < 12 ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			pMgr->m_nStoneState[ i ] = atoi( g_Buf );
			pMgr->OpenMatrixGate();
		}
		//그랜드 크리스탈 관련된 처리(mungmae-3/22)
		// 깨졌다면 효과 나타나게
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_FIRETOWER;
		ef_sort.dwColor = 1; // 렌더 플래그로 사용.

		if( g_bFireTower == 0 && pMgr->m_nStoneState[6] == 2)
		{
			ef_sort.num = 1; // 어디 불기둥인지를 체크하는데 사용.
			ef_sort.vTargetPos = D3DVECTOR(6315, 350, 3015);

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			g_bFireTower = 1;
		}
		else if(  g_bFireTower != 0 && pMgr->m_nStoneState[6] != 2 )
			g_bFireTower = 0;

		if( g_bFireTower2 == 0 && pMgr->m_nStoneState[7] == 2 )
		{
			ef_sort.num = 2;
			ef_sort.vTargetPos = D3DVECTOR(3608, 350, 8058);

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			g_bFireTower2 = 1;
		}
		else if(  g_bFireTower2 != 0 && pMgr->m_nStoneState[7] != 2 )
			g_bFireTower2 = 0;

		if( g_bFireTower3 == 0 && pMgr->m_nStoneState[8] == 2)
		{
			ef_sort.num = 3;
			ef_sort.vTargetPos = D3DVECTOR(8980, 350, 8105);

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			g_bFireTower3 = 1;
		}
		else if(  g_bFireTower3 != 0 && pMgr->m_nStoneState[8] != 2 )
			g_bFireTower3 = 0;

		return;
	}

	if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009 )
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);

		if(g_ii[0]) // 공성 중이면
			pMgr->m_bOnBattle = TRUE;
		else
		{
			pMgr->m_bOnBattle	= FALSE;
			g_bFireTower		= 0;
			g_bFireTower2		= 0;
			g_bFireTower3		= 0;
		}
		/*
			//////////////////////////////////////////////////////////////////////
			// 설정 값
			// 0 :: 닫힘
			// 1 :: 열림

			0	:: 툰드마르 가드게이트
			1	:: 미미르 툰드마르 가드게이트
			2	:: 미미르 안그마르 가드게이트
			3	:: 안그마르 가드게이트


			//////////////////////////////////////////////////////////////////////
			// 설정 값
			// 1 	:: 남은체력 > 1/2
			// 2 	:: 남은체력 < 1/2
			// 0 	:: 그라운드 크리탈이 없을때

			4	:: 툰드마르 그라운드 크리스탈
			5	:: 안그마르 그라운드 크리스탈


			//////////////////////////////////////////////////////////////////////
			// 설정값
			// 0 	:: 점령한 길드가 없을 때
			// 0<n 	:: 점령한 연합 인덱스 번호 (자신의 길드 인덱스와 검사해본다.)

			6	:: 안들랑그 북부 G스톤 점령한 연합 인덱스 번호
			7	:: 툰드마르 남부 G스톤 점령한 연합 인덱스 번호
			8 	:: 미미르 G스톤 점령한 연합 인덱스 번호
			9	:: 안그마르 북부 G스톤 점령한 연합 인덱스 번호
			10	:: 안들랑그 남부 G스톤 점령한 연합 인덱스 번호
			11	:: 사용않함
		*/
		// 여기서는 7개만 사용한다. 가드게이트 4 + GP크리스탈 3
		for( int i = 0 ; i < 11 ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			pMgr->m_nStoneState[ i ] = atoi( g_Buf );
			pMgr->OpenMatrixGate();
		}

		// 크리스탈 깨졌다면 효과 나타나게
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		ef_sort.nType = FX_FIRETOWER;
		ef_sort.dwColor = 1; // 렌더 플래그로 사용.

		// 툰드마르 크리스탈
		if( g_bFireTower == 0 && pMgr->m_nStoneState[4] == 2)
		{
			ef_sort.num = 2; // 어디 불기둥인지를 체크하는데 사용.
			ef_sort.vTargetPos = D3DVECTOR(1942, 274, 11076);

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			g_bFireTower = 1;
		}
		else if(  g_bFireTower != 0 && pMgr->m_nStoneState[4] != 2 )
			g_bFireTower = 0;

		// 운드마르 크리스탈
		if( g_bFireTower2 == 0 && pMgr->m_nStoneState[5] == 2 )
		{
			ef_sort.num = 3;
			ef_sort.vTargetPos = D3DVECTOR(11027, 250, 1659);

			pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			g_bFireTower2 = 1;
		}
		else if(  g_bFireTower2 != 0 && pMgr->m_nStoneState[5] != 2 )
			g_bFireTower2 = 0;

		return;
	}
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf); // 공성 중이냐 아니냐.

	if( g_ii[ 0 ] == 1 )
		pMgr->m_byEndDecardiCastleWar = 1;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf); // 성주 1 존
	arg = AnyOneArg(arg, g_Buf); // 성주 1 길드명
	strcpy(pMgr->m_MasterGuild1, g_Buf);
	pMgr->m_MasterZone1 = g_ii[1];

	arg = AnyOneArg(arg, g_Buf2);
	g_ii[2] = atoi(g_Buf2); // 성주 2 존
	arg = AnyOneArg(arg, g_Buf2); // 성주 2 길드명
	strcpy(pMgr->m_MasterGuild2, g_Buf2);
	pMgr->m_MasterZone2 = g_ii[2];

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 3 ] = atoi( g_Buf ); // 성주 3 존

	if( g_ii[ 3 ]  < 700 )
	{
		arg = AnyOneArg( arg, g_Buf3 ); // 성주 3 길드명
		strcpy( pMgr->m_MasterGuild3, g_Buf3 );
		pMgr->m_MasterZone3 = g_ii[ 3 ];

		// 외성주 저장
		arg = AnyOneArg(arg, g_Buf);
	}

	while (*g_Buf)
	{
		g_ii[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf2);
		if (g_ii[1] >= 100)   // 외성주의 존과 성위치 정보는 100보다 크다.
		{
			g_ii[2] = g_ii[1] / 100; // 존 정보
			g_ii[3] = g_ii[1] % 100; // 성위치 (북,동,서,남)
			if (g_ii[3] < 4 && *g_Buf2)
			{
				strcpy(pMgr->m_SubGuild[g_ii[3]], g_Buf2);
			}
		}
		arg = AnyOneArg(arg, g_Buf);
	}

	if( pMgr )
		pMgr->OnBattle(g_ii[0]);
}
//=======================================

// 공성전 남은 시간, 초로 환산해서 보냄.
void CmdBattleCount(char *arg, CUIMgr *pMgr)
{
	//g_pNk2DFrame->AddChatStr(arg, 0);		// 임시.

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if( pMgr )
		pMgr->BattleCount(g_ii[0]);
}

// 수정 소유 길드 타임 카운트.
// 소유길드명, 소유자, 소유시간, 일등길드, 일등소유시간.
void CmdTimeCount(char *arg, CUIMgr *pMgr)
{
	// nate 2005-03-11
	// Matrix
	// 메트릭스 서버에서 점수출력관련
	if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX )
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf );	// 호크로우 누적 포인트
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf );	// 쿨레인 누적포인트
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 2 ] = atoi( g_Buf );	// 쉬버 누적포인트
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 3 ] = atoi( g_Buf );	// 배틀 남은 시간(초)

		if( pMgr )
			pMgr->MatTimeCount( g_ii[ 0 ], g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ] );

		return;
	}
	if( pCMyApp->m_nCurWorldIdx == WORLD_MATRIX_2009  ) // 점수 출력
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf );	// 툰드 누적 포인트
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf );	// 안그 누적포인트
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 2 ] = atoi( g_Buf );	// 배틀 남은 시간(초)

		if( pMgr )
			pMgr->MatTimeCount( g_ii[ 0 ], g_ii[ 1 ], g_ii[ 2 ] );

		return;
	}
	arg = AnyOneArg(arg, g_Buf);				// 길드 이름
	g_ii[0] = atoi(g_Buf);
	if( g_ii[0] == -1 )
	{
		arg = AnyOneArg(arg, g_Buf2);			// 떨어뜨린 깃발 방향
		g_ii[1] = atoi(g_Buf2);
		if( pMgr )
			pMgr->TimeCount_Flag(g_ii[1], "NOFLAG", "NOFLAG");
		return;
	}

//===========================================
	// nate 2004 - 4
	// CastleWar
	if( g_ii[ 0 ] == -2 )
	{
		if( pMgr )
			pMgr->m_byEndDecardiCastleWar = 0;
		return;
	}
//===========================================

	arg = AnyOneArg(arg, g_Buf2);				// 소유자 이름.
	arg = AnyOneArg(arg, g_Buf3);
	g_ii[0] = atoi(g_Buf3);						// 소유 시간
	arg = AnyOneArg(arg, g_Buf3);				// 1등 길드
	arg = AnyOneArg(arg, g_Buf4);
	g_ii[1] = atoi(g_Buf4);						// 1등 시간
	arg = AnyOneArg(arg, g_Buf4);
	g_ii[2] = atoi(g_Buf4);						// 소유자 x
	arg = AnyOneArg(arg, g_Buf4);
	g_ii[3] = atoi(g_Buf4);						// 소유자 z
	arg = AnyOneArg(arg, g_Buf4);
	g_ii[4] = atoi(g_Buf4);						// 자기 길드 점수

	if( pMgr )
		pMgr->TimeCount(g_Buf, g_Buf2, g_ii[0], g_Buf3, g_ii[1], (float) g_ii[2], (float) g_ii[3], g_ii[4]);

	if (pCMyApp->m_nCurWorldIdx == WORLD_WHITEHORN)
	{
		for (int i = 0; i < WARINFO2_NUM; i++)
		{
			arg = AnyOneArg(arg, g_Buf);
			arg = AnyOneArg(arg, g_Buf2);
			if( pMgr )
				pMgr->TimeCount_Flag(i, g_Buf2, g_Buf);
		}
	}
}

void CmdGuildLevel(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	if( g_pRoh )
		g_pRoh->m_GuildLevel = g_pRoh->m_GuildGradeIndex =g_ii[0];
}


//----------------------------------------
// 창고 관련 명령어
//----------------------------------------
void CmdStashItem(char *arg, CUIMgr *pMgr)
{
	enum
	{
		STASH_ITEM_LOOP_COUNT = 0,

		STASH_ITEM_SLOT,
		STASH_ITEM_VNUM,
		STASH_ITEM_PLUS,
		STASH_ITEM_FLAG,
		STASH_ITEM_FLAG2,
		STASH_ITEM_UPENDUR,
		STASH_ITEM_MAXUPENDUR,
		STASH_ITEM_COUNT,
		STASH_ITEM_ENDUR,
		STASH_ITEM_MAXENDUR,		

		STASH_ITEM_ELEMENT_COUNT,
	};

	std::string str(arg);
	tokenizer token(str, sep);
	std::vector<std::string> vecItem;
	vecItem.assign(token.begin(), token.end());

	int loopCount = std::stoi(vecItem[STASH_ITEM_LOOP_COUNT]);
	
	int slot = 0;
	int vnum = 0;
	int plus = 0;
	int flag = 0;
	int flag2 = 0;
	int upEndur = 0;
	int maxUpEndur = 0;
	int endur = 0;
	int maxEndur = 0;
	int count = 0;	

	int temp = 0;
	for( int i = 0; i < loopCount; ++i )
	{
		temp		= (STASH_ITEM_ELEMENT_COUNT - 1) * i;

		slot		= std::stoi(vecItem[STASH_ITEM_SLOT + temp]);
		vnum		= std::stoi(vecItem[STASH_ITEM_VNUM + temp]);
		plus		= std::stoi(vecItem[STASH_ITEM_PLUS + temp]);
		flag		= std::stoi(vecItem[STASH_ITEM_FLAG + temp]);
		flag2		= std::stoi(vecItem[STASH_ITEM_FLAG2 + temp]);
		upEndur		= std::stoi(vecItem[STASH_ITEM_UPENDUR + temp]);
		maxUpEndur	= std::stoi(vecItem[STASH_ITEM_MAXUPENDUR + temp]);
		count		= std::stoi(vecItem[STASH_ITEM_COUNT + temp]);
		endur		= std::stoi(vecItem[STASH_ITEM_ENDUR + temp]);
		maxEndur	= std::stoi(vecItem[STASH_ITEM_MAXENDUR + temp]);		

		pMgr->Stash(slot, vnum, plus, flag, flag2, upEndur, maxUpEndur, endur, maxEndur, count, 0);
	}	
}

void CmdStashMoney(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	__int64 money = _atoi64(g_Buf);

	pMgr->Stash(12, 0, money, 0, 0, 0, 0, 0, 0, 0, 0);
}

void CmdStashTo(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);	// 슬롯번호
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// item_vnum
	g_ii[1] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// item_plus
	g_ii[2] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// item_flag
	g_ii[3] = atoi(g_Buf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[8] = atoi(g_Buf); // 맥스 강화 내구도.

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[10] = atoi(g_Buf);			// 이반 내구도 Max
	}

	arg = AnyOneArg(arg, g_Buf);	// count
	g_ii[4] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// swap flag
	g_ii[5] = atoi(g_Buf);

	if( pMgr )
	{
		if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			pMgr->StashTo(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[6],
						  g_ii[7], g_ii[8], g_ii[9], g_ii[10], g_ii[4], g_ii[5]);	///신방어구강화시스템
		}
		else
		{
			pMgr->StashTo(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[6],
						  g_ii[7], g_ii[8], 0, 0, g_ii[4], g_ii[5]);	///신방어구강화시스템
		}
	}
}

void CmdStashFrom(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);	// 슬롯번호
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// item_index
	g_ii[1] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// item_vnum
	g_ii[2] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// item_plus
	g_ii[3] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// flag
	g_ii[4] = atoi(g_Buf);

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[8] = atoi(g_Buf); // 맥스 강화 내구도..

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);		// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[10] = atoi(g_Buf);		// 일반 내구도 Max
	}

	arg = AnyOneArg(arg, g_Buf);	// count
	g_ii[5] = atoi(g_Buf);

	if( pMgr )
	{
		if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			pMgr->StashFrom(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[6],
							g_ii[7], g_ii[8], g_ii[9], g_ii[10], g_ii[5]); ///신방어구강화시스템
		}
		else
		{
			pMgr->StashFrom(g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[6],
							g_ii[7], g_ii[8], 0, 0, g_ii[5]); ///신방어구강화시스템
		}
	}
}

void CmdStashEnd(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->StashEnd();

	if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
		pMgr->m_AutoPlay->OnRecvStash();
}

void CmdChipExchange(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);		//0은 칩 교환, 1은 겜블 (type)
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);		// 성공, 실패 메시지.

	if (g_ii[0] == 0)
	{
		if (!strcmp(g_Buf, "ok"))
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[1] = atoi(g_Buf);		// 바뀔 아이템 번호.
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);		// server index

			pMgr->ChipExchange(g_ii[0], TRUE, g_ii[1], g_ii[2], -1);
		}
		else
			pMgr->ChipExchange(g_ii[0], FALSE, -1, -1, -1);
	}
	else if (g_ii[0] == 1)
	{
		if (!strcmp(g_Buf, "ok"))
		{
			arg = AnyOneArg(arg, g_Buf);	// win or lose
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[1] = atoi(g_Buf2);			// vnum or -1
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[2] = atoi(g_Buf2);			// 당첨 번호
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[4] = atoi(g_Buf2);

			if( g_ii[4] == 1 )
			{
				arg = AnyOneArg(arg, g_Buf2);
				g_ii[3] = atoi(g_Buf2);
			}

			if (!strcmp(g_Buf, "win"))
			{
				if( g_ii[4] == 1 )
					pMgr->ChipExchange(g_ii[0], TRUE, g_ii[1], g_ii[3], g_ii[2]);
				else
					pMgr->ChipExchangeMulti(g_ii[0], TRUE, g_ii[1], g_ii[4], g_ii[2], arg);
			}
			else
			{
				if( g_ii[4] == 1 || g_ii[1] == -1 )
					pMgr->ChipExchange(g_ii[0], FALSE, g_ii[1], g_ii[3], g_ii[2]);
				else
					pMgr->ChipExchangeMulti(g_ii[0], FALSE, g_ii[1], g_ii[4], g_ii[2],arg);
			}
		}
		else if (!strcmp(g_Buf, "fail"))
		{}
		else
			return;
	}

	//connie

	else if (g_ii[0] == 2)
	{
		if (!strcmp(g_Buf, "ok"))
		{
			arg = AnyOneArg(arg, g_Buf);	// win or lose
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[1] = atoi(g_Buf2);			// vnum or -1
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[2] = atoi(g_Buf2);			// 당첨 번호
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[4] = atoi(g_Buf2);

			if( g_ii[4] == 1 )
			{
				arg = AnyOneArg(arg, g_Buf2);
				g_ii[3] = atoi(g_Buf2);
			}

			if (!strcmp(g_Buf, "win"))
			{
				if( g_ii[4] == 1 )
					pMgr->ChipExchange(g_ii[0], TRUE, g_ii[1], g_ii[3], g_ii[2]);
				else
					pMgr->ChipExchangeMulti(g_ii[0], TRUE, g_ii[1], g_ii[4], g_ii[2], arg);
			}
			else
			{
				if( g_ii[4] == 1 || g_ii[1] == -1 )
					pMgr->ChipExchange(g_ii[0], FALSE, g_ii[1], g_ii[3], g_ii[2]);
				else
					pMgr->ChipExchangeMulti(g_ii[0], FALSE, g_ii[1], g_ii[4], g_ii[2],arg);
			}
		}
		else if (!strcmp(g_Buf, "fail"))
		{}
		else
			return;
	}

	else
		return;
}

void CmdYut(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);		//0은 칩 교환, 1은 겜블 (type)
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);		// 성공, 실패 메시지.

	if (g_ii[0] == 0)
	{
		if (!strcmp(g_Buf, "ok"))
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[1] = atoi(g_Buf);		// 바뀔 아이템 번호.
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);		// server index

			pMgr->ChipExchange(g_ii[0], TRUE, g_ii[1], g_ii[2], -1);
		}
		else
			pMgr->ChipExchange(g_ii[0], FALSE, -1, -1, -1);
	}
	else if (g_ii[0] == 1)
	{
		if (!strcmp(g_Buf, "ok"))
		{
			arg = AnyOneArg(arg, g_Buf);	// win or lose
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[1] = atoi(g_Buf2);			// vnum or -1
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[2] = atoi(g_Buf2);			// 당첨 번호
			arg = AnyOneArg(arg, g_Buf2);
			g_ii[3] = atoi(g_Buf2);			// server index

			if (!strcmp(g_Buf, "win"))
				pMgr->ChipExchange(g_ii[0], TRUE, g_ii[1], g_ii[3], g_ii[2]);
			else
				pMgr->ChipExchange(g_ii[0], FALSE, g_ii[1], g_ii[3], g_ii[2]);
		}
		else if (!strcmp(g_Buf, "fail"))
		{}
		else
			return;
	}
	else
		return;
}
void CmdEventAncient(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);		//0은 실패, 1은 성공
	g_ii[0] = atoi(g_Buf);

	if(g_ii[0]==0)  //실패
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf); //어떤 실패인가.
		pMgr->EventAncientExchange(g_ii[0],g_ii[1],-1,-1,-1);

	}
	else   //성공
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);
		if(g_ii[1]==0)  //아이템
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);
			pMgr->EventAncientExchange(g_ii[0],g_ii[1],g_ii[2],g_ii[3],-1);
		}
		else if(g_ii[1]==1)  //+아이템
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[4] = atoi(g_Buf);
			pMgr->EventAncientExchange(g_ii[0],g_ii[1],g_ii[2],g_ii[3],g_ii[4]);
		}
		else if(g_ii[1]==2)  //마석
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);
			pMgr->EventAncientExchange(g_ii[0],g_ii[1],g_ii[2],g_ii[3],-1);
		}
		else if(g_ii[1]==4)  //여러개 받을때 들어오는 패킷. (by 원석) ///다중교환
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf); // 갯수.
			// 이후엔 패킷 채로 넘겨준다.
			pMgr->EventMultiExchange( g_ii[2], arg );
		}
	}
}

void CmdEventTree(char *arg, CUIMgr *pMgr)//0401 식목일 이벤트
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);		//0은 실패, 1은 성공
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	if(g_ii[0]==0)	//실패
		pMgr->EventTreeExchange(g_ii[0],g_ii[1],-1,-1);
	else if(g_ii[0]==1)	//성공
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);
		pMgr->EventTreeExchange(g_ii[0],-1,g_ii[1],g_ii[2]);
	}
}

// nate
// 대만 훈장 관련
// 레벨값 추가
void CmdLevelup(char *arg, CUIMgr *pMgr)
// 캐릭이 레벨업하면 전송되는 메세지
{
	CNkCharacter *pCha;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// 캐릭터 인덱스

	if( g_dwClientCountry == CTRY_TWN )
	{
		arg = AnyOneArg( arg, g_Buf );	// 대만 훈장 관련
		g_ii[ 1 ] = atoi( g_Buf );		// 270레벨의 경우에만 레벨값 전송
	}									// 그 외에는 1이 전송

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	ef_sort.nType = FX_LEVELUP;

	pCha = pMgr->FindChaById(g_ii[0], FALSE);

	// 대만 훈장 관련
	// 레벨 셋팅
	// 레벨 270이상은 모두 270으로 레벨이 옴
	// 270레벨 미만하는 모두 1
	if( g_dwClientCountry == CTRY_TWN )
	{
		if( g_ii[ 1 ] == 270 && pCha )
			pCha->SetLevel( g_ii[ 1 ] );
	}

	// 캐릭터를 조회하고 캐릭이 현재 있을 경우랑 없을경우 분류
	if( !pCMyApp->m_pFxSet || !pCMyApp->GetLandPtr() )
		return;

	if (pCha)
	{
		ef_sort.pNkChaTo = pCha;

		ef_sort.vPos = D3DVECTOR( pCha->m_wx,
								  pCMyApp->GetLandPtr()->GetHFHeight(pCha->m_wx, pCha->m_wz),
								  pCha->m_wz);

		pCha->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

		ef_sort.nCurFrame = -5;
		pCha->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

		if ( g_pDSound )
			g_pDSound->Play(g_EffectSound[EFF_SND_LEVELUP]);		// 레벨업시에는 축하 빵빠레 나옴
	}
	else if( g_pRoh->m_nCharIndex == g_ii[0] )
	{
		ef_sort.pNkChaTo = g_pRoh;

		ef_sort.vPos = D3DVECTOR( g_pRoh->m_wx,
								  pCMyApp->GetLandPtr()->GetHFHeight(g_pRoh->m_wx, g_pRoh->m_wz),
								  g_pRoh->m_wz );

		g_pRoh->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

		ef_sort.nCurFrame = -5;
		g_pRoh->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect( ef_sort ) );

		if ( g_pDSound )
			g_pDSound->Play(g_EffectSound[ EFF_SND_LEVELUP ]);

		if( g_pNk2DFrame && g_pNk2DFrame->IsHelpPetVisible() && g_pNk2DFrame->GetControlHelpPet() ) ///펫도움말 의 발동 조건을 체크한다.
			g_pNk2DFrame->GetControlHelpPet()->CheckHelpCondition( 0, g_pRoh->m_Level, TRUE ); // 레벨 조건의 헬프가 있는지 체크한다.
	}

}

void CmdMsg(char *arg, CUIMgr *pMgr)
{
	if( g_pNk2DFrame )
		g_pNk2DFrame->InsertPopup( arg, TYPE_NOR_OK, 1 );
}

void CmdMsg2(char *arg, CUIMgr *pMgr) ///서버메세지
{
	// UI Manager의 함수 호출.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// 서버 메세지 번호.

	if( g_pNk2DFrame )
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(g_ii[0]), TYPE_NOR_OK, 1 ); // 서버에서 바로 메세지 번호를 받도로 수정 ///07-04-27
}

void CmdChangeSex(char *arg, CUIMgr *pMgr)
{
	// npc인가 char인가 구별.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// sex
	if (g_ii[1] != 0 && g_ii[1] != 1)
		return;

	if (pCha = pMgr->FindChaById(g_ii[0]))
	{
		pCha->m_Sex = g_ii[1];
		pCha->MakeBasicBody();
		if( g_pNk2DFrame )
			((CControlScore*)g_pNk2DFrame->GetInterface(SCORE))->SetSex();
		pCha->ResetMotionState();
		g_pNk2DFrame->GetControlStatus()->SetNkChar();
	}
}

void CmdChangeHair(char *arg, CUIMgr *pMgr)
{
	//sscanf(msg, "%s %s %d %d %d %d", first_arg, second_arg, &g_ii[0], &x, &z, &y);
	// npc인가 char인가 구별.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// sex
	if (g_ii[1] >= 10)
		g_ii[1] = (g_ii[1] % 10) + 5;

	if (g_ii[1] < 0 || g_ii[1] >= 10)
		return;

	if (pCha = pMgr->FindChaById(g_ii[0]))
	{
		pCha->m_Hair = g_ii[1];
		pCha->MakeBasicBody();
		pCha->ResetMotionState();
	}
}


void CmdWarTax(char *arg, CUIMgr * pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// 세금징수액
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// 세금율

	if( g_ii[0] == -1 ) // -1로 들어오면 열리기 실패처리.(서있는 위치라던지 여러 조건에의해서..)
	{
		// 이 경우 창을 닫는다.
		if( pMgr->m_pWarTax)
		{
			delete pMgr->m_pWarTax;
			pMgr->m_pWarTax = NULL;
		}
		pMgr->m_bIsWarTax = FALSE;
	}

	if( pMgr )
		pMgr->GetServMsg_WarTax(g_ii[0], g_ii[1]);
}


void CmdWarReq(char *arg, CUIMgr * pMgr)
{
	arg = AnyOneArg(arg, g_Buf);  // 날자
	arg = AnyOneArg(arg, g_Buf2); // 시간
	arg = AnyOneArg(arg, g_Buf3); // 시간
	g_ii[0] = atoi(g_Buf3);

	arg = AnyOneArg(arg, g_Buf4); // 타입
	g_ii[1] = atoi(g_Buf4);

	if( pMgr )
		pMgr->GetServMsg_WarReq(g_Buf2, g_ii[0], g_ii[1]);
}

void CmdWarReqFail(char *arg, CUIMgr * pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	arg = AnyOneArg(arg, g_Buf);  // "req" 가 올것이다..

	if (!strcmp(g_Buf, "req"))
	{
		// IDS_CANNOT_CASTLE1 : 공성 신청을 하실 수 없습니다.
		g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_CANNOT_CASTLE1),TYPE_NOR_OK, 1);
	}
	else if (!strcmp(g_Buf, "time"))
	{
		// IDS_CANNOT_CASTLE2 : "공성 신청 시간에 이상이 있습니다."
		g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_CANNOT_CASTLE2),TYPE_NOR_OK, 1);
	}

	pMgr->m_bIsWarReq = FALSE;
	SAFE_DELETE( pMgr->m_pWarReq );
}

void CmdMyPet(char *arg, CUIMgr * pMgr)
{
	arg = AnyOneArg(arg, g_Buf);  // del or index

	if (!strcmp(g_Buf, "del"))
	{
		SAFE_DELETE( pCMyApp->m_pMyPet );

		if(g_pNk2DFrame && g_pNk2DFrame->IsLShiftVisible())
			g_pNk2DFrame->SetShowLShiftFlag(FALSE);
		return;
	}
	if (!strcmp(g_Buf, "clear"))
	{
		SAFE_DELETE( pCMyApp->m_pMyPet );	// Pet삭제

		if(g_pNk2DFrame && g_pNk2DFrame->IsLShiftVisible())
			g_pNk2DFrame->SetShowLShiftFlag(FALSE);
		ZeroMemory( &g_MyPetStatus, sizeof( g_MyPetStatus ) );
		if( pMgr )
			pMgr->CmdExchPetClear();
		return;
	}

	g_ii[0] = atoi(g_Buf); // index
	arg = AnyOneArg(arg, g_Buf2);  // name

	arg = AnyOneArg(arg, g_Buf);  // class
	g_ii[1] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);  // x
	x = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // z
	z = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // dir
	dir = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // horn
	v_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // head
	v_num[1] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // body
	v_num[2] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // wings
	v_num[3] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // legs
	v_num[4] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);  // tail
	v_num[5] = atoi(g_Buf);

	SAFE_DELETE( pCMyApp->m_pMyPet );
	pCMyApp->m_pMyPet = new CNkMyPet(g_ClrAlloc);
	if( !pCMyApp->m_pMyPet )
		return;
	pCMyApp->m_pMyPet->m_bPet = TRUE;
	if (g_pRoh
			&& g_pRoh->m_Level >= 250)
		pCMyApp->m_pMyPet->SetMetaPet(TRUE);

	pCMyApp->m_pMyPet->m_PetClass = g_ii[1]; // 펫 타입(기본형0,방어형1,공격형2)
	g_MyPetStatus.SetClassType( g_ii[1] ); // 인터페이스 클래스에도 등록해둔다.

	pCMyApp->m_pMyPet->SetInitPos(x, z, dir);
	pCMyApp->m_pMyPet->m_nVirId = g_ii[0];
	pCMyApp->m_pMyPet->SetMotionState(PET_STAND);
	pCMyApp->m_pMyPet->SetBody(v_num[0], v_num[1], v_num[2], v_num[3], v_num[4], v_num[5]);
	// 애완동물 이름을 받는다.
	pCMyApp->m_pMyPet->SetName(g_Buf2);
	if( g_pNk2DFrame->GetControlPetStatus() )
	{
		g_pNk2DFrame->GetControlPetStatus()->Reposition();
		g_pNk2DFrame->GetControlPetStatus()->SetPetName(g_Buf2);
	}
	// 혹시 알을 가지고 있으면 없애자!
	g_pRoh->RemoveAllEggs();
}


void CmdPetStatus(char *arg, CUIMgr *pMgr)
{
	/*arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = _atoi64(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = _atoi64(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);*/

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = _atoi64(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	__int64 n64Exp = 0.0f;
	if( g_ii[0] == PET_EXP || g_ii[0] == PET_NEED_EXP || g_ii[0] == PET_NEED_MONEY )
		n64Exp = _atoi64( g_Buf );
	else

	g_ii[1] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);

	/*  // 이건 뭔가? 혹시 다른 사람 라비용 죽을때 잔상남는거 수정하려다 만것인가? 일단 소스 자체가 무의미하고 잘못된 소스니 주석처리함. (05-10-27 원석)
		if (pPet = pMgr->FindPetById(g_ii[2]))
		{
			if(g_ii[2] >0 )
				pPet->GetDeadFrame();
		}
	*/
	if( !g_pNk2DFrame->GetControlPetStatus() )
		return;

	switch (g_ii[0])
	{
	case PET_VITAL: // 생명력
		g_MyPetStatus.SetVital(g_ii[1], g_ii[2]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_VITAL, g_ii[1], g_ii[2]);
		break;
	case PET_DAM_RANGE: //원격공격
		g_MyPetStatus.SetRangeAtt(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_DAM_RANGE, g_ii[1]);
		break;
	case PET_DAM_MELEE: //근접공격
		g_MyPetStatus.SetMeleeAtt(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_DAM_MELEE, g_ii[1]);
		break;
	case PET_ARMOR_RANGE://원격방어
		g_MyPetStatus.SetRangeDef(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_ARMOR_RANGE, g_ii[1]);
		break;
	case PET_ARMOR_MELEE://근접방어
		g_MyPetStatus.SetMeleeDef(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_ARMOR_MELEE, g_ii[1]);
		break;
	case PET_SPEED://이동속도
		g_MyPetStatus.SetSpeed(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_SPEED, g_ii[1]);
		break;
	case PET_COMPOSE://조합레벨
		g_MyPetStatus.SetLevel_Compos(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_COMPOSE, g_ii[1]);
		break;
	case PET_PICK://획득레벨
		g_MyPetStatus.SetLevel_Pick(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_PICK, g_ii[1]);
		break;
	case PET_LOYALTY://충성도
		g_MyPetStatus.SetLoyalty(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_LOYALTY, g_ii[1]);
		break;
	case PET_HUNGER://포만도
		g_MyPetStatus.SetHunger(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_HUNGER, g_ii[1]);
		break;
	case PET_LEVEL://레벨
		g_MyPetStatus.SetLevel(g_ii[1]);
		g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_LEVEL, g_ii[1]);
		break;
	case PET_EXP://경험치
			g_MyPetStatus.SetExp(n64Exp);
			g_pNk2DFrame->GetControlPetStatus()->SetPetStatus(PET_EXP, n64Exp);
			g_pNk2DFrame->GetControlPeiceShop()->SetMyPetExp(n64Exp);
			g_pNk2DFrame->GetControlPeiceShop()->SetPieceName();
		break;
	case PET_MODE://모드
		if( g_ii[2] )
		{
			if( pCMyApp->m_pMyPet == pMgr->FindPetById(g_ii[2]) )
			{
				g_MyPetStatus.SetMode(g_ii[1]);
				if(g_pNk2DFrame->IsLShiftVisible()
						&& g_ii[1] == MYPET_MODE_KEEP)	//보관상태(죽은경우)
					g_pNk2DFrame->SetShowLShiftFlag(FALSE);
			}
		}
		else
		{
			g_MyPetStatus.SetMode(g_ii[1]);
			if( g_pNk2DFrame->IsLShiftVisible()
					&& g_ii[1] == MYPET_MODE_KEEP)	//보관상태(죽은경우)
				g_pNk2DFrame->SetShowLShiftFlag(FALSE);
		}
		break;
	case PET_BAG://바구니 바깠당!
		g_MyPetStatus.SetBag(g_ii[1]);
		break;
	case PET_BAG_COUNT://물건이 몇개 들어 있소?
		g_MyPetStatus.SetFoodCount(g_ii[1]);
		break;
	case PET_NEED_MONEY://레벨업시에 필요한 돈
		g_MyPetStatus.SetNeedMoney(n64Exp);
		break;
	case PET_NEED_EXP://레벨업시에 필요한 경험
		g_MyPetStatus.SetNeedExp(n64Exp);
		break;
	case PET_NECKLACE://목걸이 바까땅 이뿌지? 이뿌자나~~!
		g_MyPetStatus.SetNecklace(g_ii[1]);
		break;
	case PET_SKILL: // 스킬 레벨. 안 배웠으면 아예 안 올것이다.
		g_MyPetStatus.SetSkill( g_ii[1],g_ii[2] );
		break;
	}
}


void CmdKeepInfo(char *arg, CUIMgr *pMgr)
{
	char Name[255] = {0,};

	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "info"))
	{
		arg = AnyOneArg(arg, Name);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// 보관상태
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// 레벨
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// 체력
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// 충성도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// 포만도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// 찾는금액
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);			// 시간

		if( pMgr )
			pMgr->CmdSetKeepInfo(Name, g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6]);
	}
	else if (!strcmp(g_Buf, "fail"))
	{
		if( g_MyPetStatus.mode == MYPET_MODE_KEEP
				|| g_MyPetStatus.mode == MYPET_MODE_HOLD)
			// IDS_CMD_NOTYET_EGG : 아직 찾으실 수 없습니다.
			pMgr->CmdErrorMsg((char *)G_STRING(IDS_CMD_NOTYET_EGG));
		else if( g_MyPetStatus.mode == MYPET_MODE_EGG )
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[0] = atoi(g_Buf);			// 남은시간

			char tmp[255];
			int hour = 0, min = 0;
			int total = g_ii[0] + 1;

			hour = total / 60 / 60;
			min = (total - (hour*60*60)) / 60;

			// IDS_CMD_LIVE_TIME : 부화까지 %d시간 %d분 남았습니다
			sprintf(tmp, G_STRING(IDS_CMD_LIVE_TIME), hour, min);
			if( pMgr )
				pMgr->CmdErrorMsg(tmp);
		}
	}
}

void CmdPetShop(char *arg, CUIMgr *pMgr)
{
	char Name[255] = {0,};

	arg = AnyOneArg(arg, g_Buf);
	if (pMgr && !strcmp(g_Buf, "fail"))
		// IDS_CMD_UNABLE_BUY : 더이상 구매하실 수 없습니다
		pMgr->CmdErrorMsg((char *)G_STRING(IDS_CMD_UNABLE_BUY));
}

void CmdPetSocial(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);		// 애완동물 ID
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);		// 동작 Index
	arg = AnyOneArg(arg, g_Buf);
	x = atoi(g_Buf);			// x
	arg = AnyOneArg(arg, g_Buf);
	z = atoi(g_Buf);			// z

	if (pPet = pMgr->FindPetById(g_ii[0]))
	{
		if (g_ii[1] == MYPET_SOCIAL_PICKUP)
			pPet->Pickup((float) x, (float) z);
	}
}

void CmdGWar(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	if (!strcmp(g_Buf, "0")
			|| !strcmp(g_Buf, "1")
			|| !strcmp(g_Buf, "2")
			|| !strcmp(g_Buf, "3"))
	{
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		arg = AnyOneArg(arg, g_Buf2);
		arg = AnyOneArg(arg, g_Buf3);
		arg = AnyOneArg(arg, g_Buf4);
		if( pMgr )
			pMgr->GuildWarStaus(g_ii[0], g_Buf, g_Buf2, g_Buf3, g_Buf4);
	}
	else if (!strcmp(g_Buf, "cancel"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		if( pMgr )
			pMgr->GWarCancel(g_ii[0]);
	}
	else if (!strcmp(g_Buf, "point"))
	{
		arg = AnyOneArg(arg, g_Buf1);//A팀 이름
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf2);//B팀 이름
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);
		if( pMgr )
		{
			pCha = pMgr->FindChaById(g_ii[2], TRUE);
			pMgr->GWarPoint(g_ii[0], g_ii[1], g_Buf1, g_Buf2,pCha);
		}
	}
	else if (!strcmp(g_Buf, "head"))
	{
		arg = AnyOneArg(arg, g_Buf);
		arg = AnyOneArg(arg, g_Buf2);
		if( pMgr )
			pMgr->GWarHead(g_Buf, g_Buf2);
	}
	else if (!strcmp(g_Buf, "effect"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		x = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		z = atoi(g_Buf);

		if( !pCMyApp->m_pFxSet )
			return;

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_STARFLAREBOOM;
		ef_sort.vPos.x = x;
		ef_sort.vPos.z = z;
		ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight(x, z);
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}
	else if (!strcmp(g_Buf, "char"))
	{
		arg = AnyOneArg(arg, g_Buf);
		arg = AnyOneArg(arg, g_Buf2);
		g_ii[0] = atoi(g_Buf2);
		if( pMgr )
			pMgr->AddTeamMem(g_Buf, g_ii[0]);
	}
	else if( pMgr )
		pMgr->GWarNotify(g_Buf);
}

///2003 캐릭터나 몹의 상태(석화,마비) 메시지
void CmdChaCondition(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	if (!strncmp(g_Buf, "c", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);		// 인덱스
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);		// 상태번호
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);		// 0/1.	1이 상태 On

		if(pCha = pMgr->FindChaById(g_ii[0]))
		{
			if(g_ii[1]==5)
			{
				//누적이라면,0925ore
				if(g_ii[2]==1)	//상태시작이라면,
					pCha->m_Accumulate = TRUE;
				else
					pCha->m_Accumulate = FALSE;
			}
			else
			{
				pCha->SetChaCondition(g_ii[1], g_ii[2]);
			}
		}

	}
	else
	{
		//몹일때.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);		// 인덱스
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);		// 상태번호
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// 0/1

		if(pMob = pMgr->FindMobById(g_ii[0]))
		{
			pMob->SetChaCondition(g_ii[1], g_ii[2]);
		}
	}
}


////2003 계열파괴 메시지
void CmdConDistruction(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	if (!strncmp(g_Buf, "n", 1))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);//계열파괴,또는 복귀

		if (pMob = pMgr->FindMobById(g_ii[0]))
			pMob->SetClassType(g_ii[1]);
	}
}

//--------------------------------------------------------------------------
// CmdSubAttackVital
//--------------------------------------------------------------------------
// 서브 아이템을 썼을 경우 내구력 메세지
// 2005.02.12
// Lyul / 디바인 스톤 추가.
// !!!! 디바인 스톤 부터  Get_shape 를 써서 처리
// 이전 루틴을 따르는 차크람 슬레이어 부분 수정바람!!!!!!!!!!
// 지송 시간이 없어서. ( 아마 드슬 빠져 있을 듯 )
// 슬레이어와 차크람은 처리루틴이 틀려야 한다.
void CmdSubAttackVital(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);     //인덱스
	arg = AnyOneArg(arg, g_Buf);
	vital = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);     //캐릭터의 스페셜 공격
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2]= atoi(g_Buf);      //계열의 해제여부.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3]= atoi(g_Buf);      //서브아이템의 정보
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4]= atoi(g_Buf);      //Wearing 위치.(어디 아이템인지.)

	CNkCharacter *pNkCha = g_pRoh;
	CItem *pItem = NULL;

	pItem = pNkCha->m_Wearing[ g_ii[4] ];
	if(pItem)
	{
		if(pItem->m_Vnum==693
				|| (pItem->m_Vnum>=731 && pItem->m_Vnum<=738)//챠크람
				|| (pItem->m_Vnum>=724 && pItem->m_Vnum<=730)
				|| (pItem->m_Vnum==947)
				|| (pItem->m_Vnum==948)
				|| (pItem->m_Vnum==3064)
				|| (pItem->m_Vnum==10371) // 불칸
				|| (pItem->m_Vnum==10414) 	
				|| (pItem->m_Vnum==11062)
				|| (pItem->m_Vnum==11083))
		{
			//슬레이어
			if(g_ii[3]==1)//내구도가 1이면 0이란뜻.
				pItem->m_Special = 0;
			else
				pItem->m_Special = g_ii[3]; //충전
		}
		else if( (pItem->m_Vnum>=740 && pItem->m_Vnum<=756)				   //대형장비
				 || (pItem->m_Vnum>=768 && pItem->m_Vnum<=775)
				 || (pItem->m_Vnum >= 949 && pItem->m_Vnum <= 955)
				 || (pItem->m_Vnum >= 1021 && pItem->m_Vnum <= 1023)
				 || (pItem->m_Vnum == 3068) // 휴먼
				 || (pItem->m_Vnum == 10374) // Update Elite 
				 || (pItem->m_Vnum == 10453)
				 || (pItem->m_Vnum == 11066)
				 || (pItem->m_Vnum == 11067)
				 || (pItem->m_Vnum == 11087)
				 || (pItem->m_Vnum == 3065) // 카이
				 || (pItem->m_Vnum == 10372) // Update Elite
				 || (pItem->m_Vnum == 10415)
				 || (pItem->m_Vnum == 11063)
				 || (pItem->m_Vnum == 11084)
				 || (pItem->m_Vnum == 3067 || pItem->m_Vnum == 10206 // 에이디아 // Update Elite 
				 || pItem->m_Vnum == 10417
				 || pItem->m_Vnum == 11065
				 || pItem->m_Vnum == 11086)
			   )
			pItem->m_PlusNum = g_ii[3]; //충전
		// 디바인 스톤
		else if ( (GET_SHAPE(pItem) == ISUB_DSTONE) )
			pItem->m_Special = g_ii[3]; //충전
		else if( GET_SHAPE(pItem) == ISUB_PANEL )
		{
			pItem->m_Special = g_ii[3];
		}
	}
	if( pMgr )
		g_pNk2DFrame->GetControlInven()->ResetItemInfo();

	if( g_ii[1] == -1 )
		return;

	if( g_pRoh )
		g_pRoh->SetAttackType(g_ii[1]);//스페셜 공격

	if (!strncmp(g_Buf2, "n", 1))
	{
		if (pMob = pMgr->FindMobById(g_ii[0]))
		{
			if(g_ii[2]==0)
				pMob->SetClassType(g_ii[2]);
			pMob->SetAttackVital(vital);
		}
	}
	else
	{
		// "c"
		if (pCha = pMgr->FindChaById(g_ii[0]))
			pCha->SetAttackVital(vital);
	}
}

//////2003 퀵슬롯의 서브아이템(스크롤,탄환,숫돌등)을 썼을때 메시지
void CmdUseQuickSubItem(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "fail"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		if(g_ii[0]!=-1)
			g_pRoh->m_bQuickSlotLock[g_ii[0]]=FALSE;

		for(int i=0; i<=5; i++)
			g_pRoh->m_bQuickSlotLock[i]=FALSE;
	}
	else if (!strcmp(g_Buf, "succ") || !strcmp(g_Buf, "remain"))
	{
		// 구조가 살짝 바껴야될듯!
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// del_slot
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// fill_slot
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2]= atoi(g_Buf);			// plus_num
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3]= atoi(g_Buf);			// special_num

		if( pMgr )
			pMgr->UseQuickSubItem(g_ii[0],g_ii[1],g_ii[2],g_ii[3]);
	}
}

/////2003  퀵슬롯의 마법책을 썼을때 효과 메세지 ,해제스킬효과 메시지.
void CmdEffectSubItem(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "1"))	//상대방에게 서브마법을 걸때.
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			//npc.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			//인덱스

		pCha = pMgr->FindChaById(g_ii[1]);//마법 거는 사람.항상 캐릭터.
		if(pCha==NULL)
			return;

		arg = AnyOneArg(arg, g_Buf);    //npc

		if (!strncmp(g_Buf, "n", 1))	//타켓
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);		//인덱스
			arg = AnyOneArg(arg, g_Buf);
			g_ii[4] = atoi(g_Buf);		//해제냐,책이냐..

			if (g_ii[4]==SKILL_REMOVE)
				pCha->SetUseBookEffect(EFFECT_REMOVE);		//해제를 사용한 효과
			else if (g_ii[4]==SKILL_LIFE_LIGHT)
				pCha->SetUseBookEffect(EFFECT_LIFE_LIGHT);	//생명의 빛 사용한 효과.
			else//책
				pCha->SetUseBookEffect(EFFECT_USE_BOOK);	//책을 사용한 효과

			if (pMob = pMgr->FindMobById(g_ii[3]))
			{
				if(g_ii[4]==0)
				{
					//블랙홀이라면
					if(pCha->m_nCharIndex != g_pRoh->m_nCharIndex)
					{
						pCha->SetAttackMobId(pMob->m_nVirId);
						pCha->Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz,SKILL_BOOK_READ);
					}
					pMob->SetUseBookEffect(EFFECT_BLACKHOLE);
				}
				else
				{
					if(pCha->m_nCharIndex != g_pRoh->m_nCharIndex)
					{
						pCha->SetAttackMobId(pMob->m_nVirId);
						pCha->Attack(pMob->m_wx, pMob->m_wy, pMob->m_wz,g_ii[4]);
					}
					pMob->SetUseBookEffect(EFFECT_BOOK); //몹이 책,해제 마법맞는 효과 On
				}
			}
		}
		else
		{
			// "c"
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);			//인덱스
			arg = AnyOneArg(arg, g_Buf);
			g_ii[4] = atoi(g_Buf);			//해제냐,책이냐..

			if (g_ii[4]==SKILL_REMOVE) 	//해제 52
				pCha->SetUseBookEffect(EFFECT_REMOVE);   //해제를 사용한 효과
			else if (g_ii[4]==SKILL_LIFE_LIGHT)
				pCha->SetUseBookEffect(EFFECT_LIFE_LIGHT);   //생명의 빛 사용한 효과.
			else//책
				pCha->SetUseBookEffect(EFFECT_USE_BOOK);   //책을 사용한 효과

			if (pCha2 = pMgr->FindChaById(g_ii[3]))
			{
				if(g_ii[4]==0)
				{
					//블랙홀이라면
					if(pCha->m_nCharIndex != g_pRoh->m_nCharIndex)
					{
						pCha->SetAttackChaId(pCha2->m_nCharIndex);
						pCha->Attack(pCha2->m_wx, pCha2->m_wy, pCha2->m_wz,SKILL_BOOK_READ);
					}
					pCha2->SetUseBookEffect(EFFECT_BLACKHOLE);//블랙홀 효과
				}
				else
				{
					if(pCha->m_nCharIndex != g_pRoh->m_nCharIndex)
					{
						pCha->SetAttackChaId(pCha2->m_nCharIndex);
						pCha->Attack(pCha2->m_wx, pCha2->m_wy, pCha2->m_wz,g_ii[4]);
					}
					pCha2->SetUseBookEffect(EFFECT_BOOK);//캐릭이 책,해제 마법 맞는 효과 On
				}

				// 헬다운 취소 예외 처리 07-03-14 원석
				if( pCha2 == g_pRoh ) // 맞는게 자신이면..
				{
					if( g_pRoh->GetMotionState() == CHA_C_HELLDOWN ) // 헬다운 기모으는 중이었다면...
					{
						g_pRoh->HellDownFail( g_pRoh, NULL ); // 헬다운을 취소시킨다.
					}
				}
			}
		}
	}
	else
	{
		//0으로 올때,자신에게 서브 마법 걸때.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			 //npc. 항상 c가 옴.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			 //인덱스
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2]= atoi(g_Buf);				 //타입(현재 처리 없음)
		if(pCha = pMgr->FindChaById(g_ii[1]))
		{
			//여긴 항상 캐릭만...
			if(g_ii[2])//1이라면 on
				pCha->SetUseBookEffect(EFFECT_SCANVIEW);
		}
	}
}
///2003 조립메시지.
void CmdAssembly(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;
	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "succ"))
	{
		arg = AnyOneArg(arg, g_Buf); //조립된 아이템 vnum
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf); //조립된 아이템 인덱스.
		g_ii[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf); //조립된 아이템 vnum
		g_ii[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf); //조립된 아이템 인덱스.
		g_ii[3] = atoi(g_Buf);

		pMgr->AssemblySuccess(g_ii[0],g_ii[1],g_ii[2],g_ii[3]);
		pMgr->AssemblyFail(FALSE);
	}
	else	// fiall
		pMgr->AssemblyFail(TRUE);
}

void CmdCombine(char *arg, CUIMgr *pMgr)//대형장비에 소형장비 넣기.빼기(split)
{
	CNkCharacter *pNkCha = g_pRoh;
	CItem *pItem = NULL;

	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "succ"))
	{
		arg = AnyOneArg(arg, g_Buf); //소형장비 (시프트)
		g_ii[0] = atoi(g_Buf);
		pNkCha->m_Wearing[3]->m_Special = g_ii[0];
		pCMyApp->m_bEquipLock = FALSE;//대형장비 락 해제 1009

		if( pMgr )
			g_pNk2DFrame->GetControlInven()->ResetItemInfo();///0928  정보 갱신.
	}
	else
		pCMyApp->m_bEquipLock = FALSE;//대형장비 락 해제

	for(int i=0; i<=5; i++)
	{
		if( g_pRoh )
			g_pRoh->m_bQuickSlotLock[i] = FALSE;
	}
}

//--------------------------------------------------------------------------
// CmdSummonMob
//--------------------------------------------------------------------------
// 소환수의 등장과 제거에 관련된 메세지
// sm  c 'ch' n '몹 인덱스' 좌표 등등
void CmdSummonMob(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	CNkCharacter *pNkCha = g_pRoh;

	arg = AnyOneArg(arg, g_Buf);//c
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			//인덱스
	pCha = pMgr->FindChaById(g_ii[0]);//소환하는 캐릭터

	arg = AnyOneArg(arg, g_Buf);    //n
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);		//인덱스
	pMob = pMgr->FindMobById(g_ii[1]);//소환하는 캐릭터

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);			//x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);			//z

	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);		//소환,역소환

	if(g_ii[4] == 1 || g_ii[4] == 11)	// 일반소환수 : 0(소멸)_1(생성), 서먼플라워 소환수 : 10(소멸)_11(생성)
	{
		// 내 소환수라면
		if(pCha == pNkCha)
		{
			if( g_ii[4] == 1 )
				pMgr->m_pSummonMob[ 0 ] = pMob;
			else if( g_ii[4] == 11 )
				pMgr->m_pSummonMob[ 1 ] = pMob;
		}

		if( !pCMyApp->m_pFxSet )
			return;

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_SHOWUP2;
		ef_sort.vPos.x = g_ii[2];
		ef_sort.vPos.z = g_ii[3];
		ef_sort.vPos.y = pCMyApp->GetLandPtr()->GetHFHeight(x, z);
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		ef_sort.nCurFrame = -2;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
		ef_sort.nCurFrame = -4;
		pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
	}
	else if( g_ii[4] == 0 || g_ii[4] == 10 )
	{
		//역소환,또는 시간 다되서 사라질때.
		if(pCha == pNkCha)
		{
			if( g_ii[4] == 0 )
				pMgr->m_pSummonMob[ 0 ] = NULL;
			else if( g_ii[4] == 10 )
				pMgr->m_pSummonMob[ 1 ] = NULL;
		}
	}
}

void CmdChaotic(char *arg, CUIMgr *pMgr) //1023
{

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			//인덱스
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			//등급

	if( pMgr )
	{
		pCha = pMgr->FindChaById(g_ii[0]);//
		if(pCha)
		{
			pCha->m_Chaotic_Grade = g_ii[1];
			pCha->SetChatMsg(pCha->m_pstrName, "");//1112
		}
	}
}

void CmdGuildPk(char *arg, CUIMgr *pMgr)
{
	if( !g_pRoh )
		return;

	int type = -1;
	int index = -1;

	arg = AnyOneArg(arg,g_Buf); // 서브 명령어
	if(strcmp(g_Buf,"end") == 0)
	{
		// 길드전 종료
		g_pRoh->m_bGuildPK = false;
		g_pRoh->m_nTeamMemGrade = 0;
	}
	else if(strcmp(g_Buf,"start") == 0)
	{
		// 길드전 시작 (머리위 주먹 표시 ?? )
		arg = AnyOneArg(arg,g_Buf1);  // 적길드이름
		g_pRoh->m_bGuildPK = true;
		g_pRoh->m_nTeamMemGrade= 9;
		ZeroMemory(g_pRoh->m_sEnemyGName,sizeof(g_pRoh->m_sEnemyGName));
		strcpy(g_pRoh->m_sEnemyGName,g_Buf1);
	}
	else if(strcmp(g_Buf,"request") == 0)
	{
		// 길드전 요청
		type = 0;
		arg = AnyOneArg(arg,g_Buf1);// 요청한 길드 이름
		if( pMgr )
			pMgr->GuildPK(type,g_Buf1,g_Buf2,g_ii[0]);// 서브명령어 type, 길드 //, 길드(or 길마), PK 상태 값
	}
	else if(strcmp(g_Buf,"sder") == 0)
	{
		// 길드전 항복
		type = 1; // surrender
		arg = AnyOneArg(arg,g_Buf1); // 항복한 길드 이름
		arg = AnyOneArg(arg,g_Buf2); // 항복한 길드의 길드 마스터 이름
		if( pMgr )
			pMgr->GuildPK(type,g_Buf1,g_Buf2,g_ii[0]);// 서브명령어 type, 길드 , 길드(or 길마) //, PK 상태 값
	}
	else if(strcmp(g_Buf,"play") == 0)
	{
		arg = AnyOneArg(arg,g_Buf1); //길드 이름
		arg = AnyOneArg(arg,g_Buf2); //길드 이름
		CNkCharacter *pCha;
		LHCharacterMap_it pr;
		for (pr = pMgr->m_character_map.begin(); pr != pMgr->m_character_map.end(); pr++)
		{
			pCha = (*pr).second;
			if(pCha->m_pstrGuildName)
			{
				if(strcmp(g_Buf1,pCha->m_pstrGuildName) == 0 || strcmp(g_Buf2,pCha->m_pstrGuildName)==0)
					pCha->m_nTeamMemGrade = 9;
			}
		}
	}
	else if(strcmp(g_Buf,"stop") == 0)
	{
		arg = AnyOneArg(arg,g_Buf1); //종료한 길드 이름
		CNkCharacter *pCha;
		LHCharacterMap_it pr;
		for (pr = pMgr->m_character_map.begin(); pr != pMgr->m_character_map.end(); pr++)
		{
			pCha = (*pr).second;
			if(pCha->m_pstrGuildName)
			{
				if(strcmp(g_Buf1,pCha->m_pstrGuildName) == 0 )
					pCha->m_nTeamMemGrade = 0;
			}
		}
	}
}

void CmdPk(char *arg, CUIMgr *pMgr) //1111
{
	arg = AnyOneArg(arg, g_Buf);		//캐릭index.
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			//0/1 1:시작 0:끝.

	if(pCha = pMgr->FindChaById(g_ii[0]))
	{
		if(g_ii[1]==2 && !pCha->m_bNonPK)//0305 non피케이 시작.
			pCha->m_bNonPK = TRUE;
		else if(g_ii[1]==0 && pCha->m_bNonPK)//non 피케이 해제.
			pCha->m_bNonPK = FALSE;
		else
			pCha->m_Free_Pk = g_ii[1];

		pCha->m_bPk_render = TRUE;
		pCha->m_bShowChatPopup = TRUE; //0310 피케이나 논피케이 선언시에 창 한번 띄워주기.
		pCha->DrawChatPopup(FALSE);
	}
}

void CmdSky(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// id
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// 비행석 레벨

	if (pCha = pMgr->FindChaById(g_ii[0], TRUE))
		pCha->SetFlyLevel(g_ii[1]);
}

void CmdSales(char *arg, CUIMgr *pMgr)
{
	if( !g_bSalesView )
	{
		if( g_bSendSalesOff )
			return ;

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage("sales_on 0\n");
		g_bSendSalesOff = TRUE;
		return ;
	}

	int say_type = 5;
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);


	if (g_ii[0] >= 0)
	{
		SkipSpaces(&arg);

		if(g_bFiltering)
			CheckAndRepair(arg);

		int len = strlen(arg);
		char *arg2 = AnyOneArg(arg, g_Buf);

		if (pCha = pMgr->FindChaById(g_ii[0]))
		{
			char *arg3 = arg;
			SkipSpaces(&arg3);
			pCha->SetChatMsg(g_Buf, arg2);
		}

		*arg2 = '>';
	}
	else if( g_ii[0] == -1 ) // -1이 들어오면 매트릭스에서 지휘관 등장 패킷이다.(색상 문제로 장사말 패킷으로 처리했다.) (by 원석)
	{
		SkipSpaces(&arg);
		g_pNk2DFrame->SetInfoBox2(arg); // 운영자 공지랑 비슷하게 화면 가운데에 문구를 3초정도 찍어준다.

		if (g_pDSound) // 징소리 출력.
			g_pDSound->Play(g_EffectSound[EFF_SND_ENTRANCE]);
	}

	SkipSpaces(&arg);
	g_pNk2DFrame->AddChatStr(arg, say_type);
}


//============================================
// nate 2004 - 5
// 존 이동시 원기둥과 채릭 좌표의 비동기화 수정
void CmdJump( char *arg, CUIMgr *pMgr )
{
	arg = AnyOneArg( arg, g_Buf );
	x = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );
	z = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );

	if( pCha = pMgr->FindChaById( g_ii[ 0 ] ) )
	{
		if( !pCMyApp->m_pFxSet )
			return;

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_TELEPORT;
		ef_sort.vPos.x = pCha->m_wx;
		ef_sort.vPos.y = pCha->m_wy;
		ef_sort.vPos.z = pCha->m_wz;

		pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
	}

	return;
}
//============================================

// Happy New Year Event
void CmdEventNewYear(char *arg, CUIMgr *pMgr)
{
	// nate 2004 - 11 : Happy New Year Event
	pCMyApp->CreateHappyNewYear();

	if( g_pDSound )
		g_pDSound->Play(g_EffectSound[EFF_SND_HAPPYNEWYEAR]);

	return;
}

void CmdMultiAttack(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->CmdMultiAttack(arg);
}

void CmdQuest(char *arg, CUIMgr *pMgr)	// qt
{
	if( !g_pNk2DFrame )
		return;

	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);		// 퀘스트 번호,몬스터번호,배열번호,에러번호

	if (!strcmp(g_Buf2, "get"))
	{
		g_pNk2DFrame->ShowInterfaceWindow(true, QUEST);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);		// 퀘스트 취소 패킷

		if(g_ii[1]==-1)	//퀘스트 취소
		{
			if( g_ii[0] >= 735 && g_ii[0] <= 801 )//mungmae-2006/06/22 스페셜 퀘스트를 끝내면 false
				g_pRoh->m_bSecialQuest = FALSE;

			// nate 2004 - 4 : quest
			pMgr->SetQuest2StartFlag( 0 );
			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetNoQuest();
			// 인벤에서 1053 아이템 몽땅 지운다.
			CItem *pItem = NULL;
			// Inven 에서 지우기
			do
			{
				pItem = NULL;
				for (int i = 0; i < PACK_NUM; i++)
				{
					pItem = g_pRoh->PickItemInPack(i, 1053, TRUE);
					if (pItem)
						break;
				}
				if (pItem)
					delete pItem;
			}
			while (pItem);
			// 교환창에서 지우기
			do
			{
				pItem = g_pRoh->PickItemInExgPack(1053, TRUE);
				if (pItem)
					delete pItem;
			}
			while (pItem);
			// 퀵슬롯에서 지우기
			for (int i = 0; i < QUICK_SLOT_NUM*QUICK_SLOT_PAGE; i++)
			{
				if (g_pRoh->m_QuickSlot[i]
						&& g_pRoh->m_QuickSlot[i]->m_Vnum == 1053)
					SAFE_DELETE(g_pRoh->m_QuickSlot[i]);
			}
		}
		else if(g_ii[0]>=0)	//퀘스트 정보
		{
			if( g_ii[0] >= 735 && g_ii[0] <= 801 )//mungmae-2006/06/22 인덱스 735~801이면 250~275 스폐셜 퀘스트
				g_pRoh->m_bSecialQuest = TRUE;

			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->Read("data/quest.dta", g_ii[0]);

			// nate 2004 - 4 : quest
			if( ((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_Type == 2 )
				pMgr->SetQuest2StartFlag( 1 );

			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetName();
		}
	}
	else if(!strcmp(g_Buf2, "end"))
	{
		if( g_ii[0] >= 735 && g_ii[0] <= 801 )//mungmae-2006/06/22 스페셜 퀘스트를 끝내면 false
			g_pRoh->m_bSecialQuest = FALSE;

		// nate 2004 - 4 : quest
		pMgr->SetQuest2StartFlag( 0 );
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetNoQuest();
	}
	else if(!strcmp(g_Buf2, "king"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);  //죽인숫자

		pMob = pMgr->FindMobById(g_ii[0]);//몹

		if(pMob)
		{
			if(g_ii[1]==0)	//킹->일반
				pMob->m_bQuestMob = FALSE;
			else if(g_ii[1]==1)	//일반->킹
			{
				pMob->m_bQuestMob = TRUE;

				EffectSort ef_sort;
				ZeroMemory(&ef_sort, sizeof(EffectSort));
				ef_sort.nType = FX_MOBSHOWUP;
				ef_sort.num = g_ii[0];
				ef_sort.vPos.x = pMob->m_wx;
				ef_sort.vPos.z = pMob->m_wz;
				ef_sort.vPos.y = pMob->m_wy;
				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);

				ef_sort.nCurFrame = -3;
				pCMyApp->m_pFxSet->InsertFxEffect(ef_sort);
			}
		}
	}
	else if(!strcmp(g_Buf2, "kill"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);  //죽인숫자

		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->Reload(g_ii[0],g_ii[1]);
	}
	else if(!strcmp(g_Buf2, "quick"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);  //퀵슬롯 번호
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);  //남은갯수

		if(g_ii[1]>=0)
			g_pRoh->m_QuickSlot[g_ii[0]]->m_Special = g_ii[1]; //퀘스트 남은 갯수 갱신
		g_pNk2DFrame->GetControlInven()->ResetItemInfo();
	}
	else if(!strcmp(g_Buf2, "fail"))
	{
		if(!g_pNk2DFrame->IsInterfaceVisible(QUEST))
			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->QuestMessage();

		g_pNk2DFrame->ShowInterfaceWindow(true, QUEST);
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->PopUpMsg(g_ii[0]);
	}
	else if(!strcmp(g_Buf2, "eff"))	//데카둔의 모험가만 온다.
	{
		if( !pCMyApp->m_pFxSet )
			return;

		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);  //Z값
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);  //캐릭 아이디

		pCha = pMgr->FindChaById(g_ii[2]);//캐릭터
		/////여기에 데카둔의 모험가 퀘스트 종료 효과 넣음....

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));

		if (g_pRoh->m_nCharIndex == g_ii[2])
		{
			ef_sort.nType = FX_GHOSTHUNT;
			ef_sort.pNkChaTo = g_pRoh;
			ef_sort.vPos = D3DVECTOR(g_ii[0], pCMyApp->GetLandPtr()->GetHFHeight(g_ii[0], g_ii[1]), g_ii[1]);
			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		else if (pCha)
		{
			ef_sort.nType = FX_GHOSTHUNT;
			ef_sort.pNkChaTo = pCha;
			ef_sort.vPos = D3DVECTOR(g_ii[0], pCMyApp->GetLandPtr()->GetHFHeight(g_ii[0], g_ii[1]), g_ii[1]);
			pCha->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
		else if(g_ii[2]== -1)
		{
			ef_sort.nType = FX_GHOSTRISE;
			ef_sort.vPos = D3DVECTOR(g_ii[0], pCMyApp->GetLandPtr()->GetHFHeight(g_ii[0], g_ii[1]), g_ii[1]);
			g_pRoh->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}

	}
	// nate 2004 - 4 : quest - 퀘스트에서 얻어야할 총 포인트
	else if( !strcmp( g_Buf2, "tp" ) )
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetTotalPoint( g_ii[ 0 ] );
	// 퀘스중에 몹을 잡으면서 얻게되는 누적 포인트
	else if( !strcmp( g_Buf2, "pt" ) )
	{
		// 남은 포인트
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetRemainPoint( g_ii[ 0 ] );
		arg = AnyOneArg( arg, g_Buf1 );
		g_ii[ 1 ] = atoi( g_Buf1 );

		// 누적 포인트
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetAddPoint( g_ii[ 1 ] );

		// 씽크가 안 맞을경우 로그를 남기기 위해 서버에게 알림
		// 씽크 : 얻어야할 토탈점수와 남은점수 + 획득누적점수 비교
		if( ((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_nQuestTotalPoint != ( g_ii[ 0 ] + g_ii[ 1 ] ) )
		{
			char strTemp[ 100 ];
			sprintf( strTemp, "qt_pt %d %d", g_ii[ 0 ], g_ii[ 1 ] );
			g_pTcpIp->SendNetMessage( strTemp );
		}
	}
	else if( !strcmp( g_Buf2, "nt" ) )
		((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->SetNextQuest();
	else if(!strcmp(g_Buf2, "sp"))
	{
		//퀘스트 수행 여부( 0, 1 );
		if( g_ii[0] )
			g_pRoh->m_bYanQuest = TRUE;
		else
			g_pRoh->m_bYanQuest = FALSE;
	}
	else if( !strcmp(g_Buf2, "bn") ) // 보상 x배의 표시.
	{
		switch( g_ii[0] )
		{
		case 0: // 보너스 텍스트 없애기
			SAFE_DELETE( ((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_pBonusText );
			break;
		case 1: // 얀의 팔찌 보너스 보상 x5배

			SAFE_DELETE( ((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_pBonusText );

			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_pBonusText = new CTextOutBox;
			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_pBonusText->Init( 20, RGB(250,100,100), 0, 0 );
			((CQuestData*)g_pNk2DFrame->GetInterface(QUEST))->m_pBonusText->SetString_withOutLine( "x5",RGB(250,250,250),RGB(250,50,50) );
			break;

		}
	}
}

void CmdRingCombine(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg( arg, g_Buf );
	g_ii[1] = atoi( g_Buf );  // Item_Flag

	if(g_ii[0]==-1) // 실패
	{
		if(g_pNk2DFrame
				&& g_pNk2DFrame->GetEventAncient())
		{
			g_pNk2DFrame->GetEventAncient()->m_bExchange =FALSE;
			// IDS_CMD_FAIL_RING : 고대반지 회수에 실패했습니다.
			if( g_pNk2DFrame )
			{
				if( g_ii[1] == 1 )
				{
					g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CMD_FAIL_RING_COMB), TYPE_NOR_OK, 1 );
				}
				else
				{
					if( g_dwClientCountry == CTRY_JPN )
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_COMBINE_FAIL), TYPE_NOR_OK, 1 );
					else
						g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CMD_FAIL_RING), TYPE_NOR_OK, 1 );
				}
			}
		}

	}
	else if(g_ii[0]==-2)	// 삭제성공
	{
		if( g_pRoh )
		{
			delete g_pRoh->m_Wearing[IARMOR_RING];
			g_pRoh->m_Wearing[IARMOR_RING] = NULL;	//인벤에서 반지 삭제...
		}
		if( g_pNk2DFrame && g_pNk2DFrame->GetEventAncient() )
			g_pNk2DFrame->GetEventAncient()->m_bExchange =FALSE;
	}
	else
	{
		//반지 추가
		if( g_pRoh )
		{
			g_pRoh->m_Wearing[IARMOR_RING]->m_Special = g_ii[0];
			g_pRoh->m_Wearing[IARMOR_RING]->m_PlusNum = g_ii[1];
		}
		if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			g_pNk2DFrame->GetControlInven()->ResetItemInfo();/// 정보 갱신.
	}

	pCMyApp->m_bRingLock = FALSE;//반지 락 해제

	for(int i=0; i<=5; i++)
		g_pRoh->m_bQuickSlotLock[i] = FALSE;
}

void CmdNecklaceCombine(char *arg, CUIMgr *pMgr) // 목걸이 합성 ///페어리
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// Item_Flag

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 2 ] = atoi( g_Buf );  // Item_Flag

	if( g_ii[0]== 1 ) // 성공. (뺐는지 꼈는지는 상관없다. 옵션값만 대입해주면 된다.(뺏을경우 Extra 슬롯 처리는 서버에서 알아서 해줌.)
	{
		if( g_pRoh )
		{
			g_pRoh->m_Wearing[IARMOR_NECKLACE]->m_Special = g_ii[1];
			g_pRoh->m_Wearing[IARMOR_NECKLACE]->m_PlusNum = g_ii[2];
		}
		if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			g_pNk2DFrame->GetControlInven()->ResetItemInfo();

		// status update
		if( g_pNk2DFrame->GetInterface(SCORE) )
			((CControlScore*)(g_pNk2DFrame->GetInterface(SCORE)))->SetAllScore();

	}
	else if( g_ii[0] == 0 ) // 실패 (합성된 목걸이에 합성하려는 경우 라던지 등..
	{
		_asm nop;
	}

	pCMyApp->m_bNecklaceLock = FALSE; // 락 해제

	for(int i=0; i<=5; i++)
		g_pRoh->m_bQuickSlotLock[i] = FALSE;
}

void CmdQuestExchange(char *arg, CUIMgr *pMgr)//역사학자가 돈으로 바꿔주기.
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if( pMgr )
		g_pNk2DFrame->GetEventAncient()->QuestExchange(g_ii[0]);
}

void CmdParentsDay(char * arg, CUIMgr * pMgr)
{
	if( !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if(g_ii[0] == 1 || g_ii[0] ==2)  // 성공
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);
		g_pNk2DFrame->GetEventAncient()->GetMsgParentsDay(TRUE, g_ii[0], g_ii[1]);
	}
	else
		g_pNk2DFrame->GetEventAncient()->GetMsgParentsDay(FALSE, 0, 0);
}

void CmdRoomListStart(char *arg, CUIMgr *pMgr)
{
	if( pMgr )
		pMgr->DoRoomListStart();
}

void CmdRoomList(char *arg, CUIMgr *pMgr)
{
	int room_num, room_type, count, max_count, min_level, max_level, fee;
	int state;

	arg = AnyOneArg(arg, g_Buf);		// 룸번호
	room_num = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	room_type = atoi(g_Buf);			// 방형식
	arg = GetSentence(arg, g_Buf);		// 방제
	arg = AnyOneArg(arg, g_Buf2);
	count = atoi(g_Buf2);
	arg = AnyOneArg(arg, g_Buf2);
	max_count = atoi(g_Buf2);
	arg = AnyOneArg(arg, g_Buf2);		// 방장 이름.
	arg = AnyOneArg(arg, g_Buf3);		// waiting, playing
	state = atoi(g_Buf3);
	arg = AnyOneArg(arg, g_Buf3);		// min level
	min_level = atoi(g_Buf3);
	arg = AnyOneArg(arg, g_Buf3);		// max level
	max_level = atoi(g_Buf3);
	arg = AnyOneArg(arg, g_Buf3);		// min fee
	fee = atoi(g_Buf3);
	arg = AnyOneArg(arg, g_Buf3);		// 비밀번호

	if( pMgr )
		pMgr->ShowRoomList(room_num, room_type, g_Buf, count, max_count, g_Buf2, state, min_level, max_level, fee, g_Buf3);
}

void CmdEnterRoom(char *arg, CUIMgr *pMgr)
{
	int room_type, user_type;

	arg = AnyOneArg(arg, g_Buf);
	room_type = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf2);
	arg = AnyOneArg(arg, g_Buf);
	user_type = atoi(g_Buf);

	if( pMgr )
		pMgr->EnterRoom(room_type, g_Buf2, user_type);
}
void CmdSpUpgrade(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);				// where
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);				// idx
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);				// addflag2
	
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);				// 1
	arg = AnyOneArg(arg, g_Buf);
	g_ii[4] = atoi(g_Buf);				// 2
	arg = AnyOneArg(arg, g_Buf);
	g_ii[5] = atoi(g_Buf);				// 3
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf);				// 4
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf);				// 5
	arg = AnyOneArg(arg, g_Buf);
	g_ii[8] = atoi(g_Buf);				// 6
	

	if( !g_pRoh )
		return;

	CNkCharacter *pNkCha = g_pRoh;

	// ว๖ภ็ ภๅบ๑ดย Pointer ธฆ ฐกมฎฟภนวทฮ ภำฝรทฮ ฟ๘ทก ภๅบ๑ ทนบงภป ภ๚ภๅวา ภงฤก
	int Ltemp_item_min_lev;
	// Up grade ตศ พฦภฬลภป รฃดยดู.
	CItem *pItem = pNkCha->FindItemByIndexFromAll(g_ii[1]);
	// รฃพาภธธ้
	if (pItem)
	{
		// วุด็ พฦภฬลภว ฟ๘ทก min_levภป ฐกมฎ ฟยดู.
		pItem->m_Special2 = g_ii[2];
		pItem->m_spbuff[0] = g_ii[3];
		pItem->m_spbuff[1] = g_ii[4];
		pItem->m_spbuff[2] = g_ii[5];
		pItem->m_spbuff[3] = g_ii[6];
		pItem->m_spbuff[4] = g_ii[7];
		pItem->m_spbuff[5] = g_ii[8];
		g_pNk2DFrame->GetControlInven()->m_pItemInfo->SetIndex(-1);
	}
}

void CmdTRoom(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	int which_loc;
	BOOL bWaiting, bOwner;

	arg = AnyOneArg(arg, g_Buf);			// command

	if (!strcmp("in", g_Buf))   //들어왔다.
	{
		arg = AnyOneArg(arg, g_Buf);		// 캐릭터 이름
		arg = AnyOneArg(arg, g_Buf2);		// 팀위치, 대기, 홍팀, 청팀
		which_loc = atoi(g_Buf2);

		arg = AnyOneArg(arg, g_Buf2);		// waiting, ready
		if (atoi(g_Buf2) == 0)
			bWaiting = FALSE;
		else
			bWaiting = TRUE;

		arg = AnyOneArg(arg, g_Buf2);		// 0: normal user, 1: master
		if (atoi(g_Buf2) == 0)
			bOwner = FALSE;
		else
			bOwner = TRUE;

		if( pMgr )
			pMgr->TRoomIn(g_Buf, which_loc, bWaiting, bOwner);

		return;
	}
	else if (!strcmp("out", g_Buf))
	{
		//나갔다.
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(1, g_Buf);

		return;
	}
	else if (!strcmp("ready", g_Buf))
	{
		//레디 상태이다.
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(2, g_Buf);
		return;
	}
	else if (!strcmp("wait", g_Buf))
	{
		//대기 상태이다.
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(3, g_Buf);
		return;
	}
	else if (!strcmp("shop", g_Buf))
	{
		//샾에 들어간 상태이다.
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(4, g_Buf);
		return;
	}
	else if (!strcmp("be_a", g_Buf))
	{
		//A팀 청팀
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(5, g_Buf);
		return;
	}
	else if (!strcmp("be_b", g_Buf))
	{
		//B팀 홍팀
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(6, g_Buf);
		return;
	}
	else if (!strcmp("master", g_Buf))
	{
		//방장이다.
		arg = AnyOneArg(arg, g_Buf);
		if( pMgr )
			pMgr->TRoomUpdate(7, g_Buf);
		return;
	}
	else if (!strcmp("start", g_Buf))
	{
		//경기 시작.(창 닫아주자)
		pMgr->TRoomClose(FALSE);
		//// Room에 들어갈때 Score 초기화
		if(pMgr->m_pTextScoreA)
			pMgr->m_pTextScoreA->SetString("0");
		if(pMgr->m_pTextScoreB)
			pMgr->m_pTextScoreB->SetString("0");
		return;
	}
	else if (!strcmp("kick", g_Buf))
	{
		pMgr->TRoomClose();
		// Close Score Board...
		if(pMgr->m_bShowScore)
			pMgr->DestroyScore();

		// If Shop Window is shown, Toggle Shop Window(Close)...
		if(g_pNk2DFrame && g_pNk2DFrame->IsShopVisible())
			g_pNk2DFrame->ToggleShopWindow();
		return;
	}

	arg = AnyOneArg(arg, g_Buf2);			// maybe character name

	pMgr->TRoom(g_Buf, g_Buf2);
}


void CmdTwPackage(char* arg, CUIMgr *pMgr )
{
	int	nDate;

	arg = AnyOneArg(arg, g_Buf);
	nDate = atoi(g_Buf);

	if( pMgr )
		pMgr->TwPackage( nDate );
}

void CmdMalId(char *arg, CUIMgr *pMar)
{
	arg = AnyOneArg(arg, g_Buf);
	strcpy(pCMyApp->m_strUserId, g_Buf);
}

//// 라비용 교환 관련 함수
void CmdPetExchOppInfo(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;

	//	"p_stat" : 이름 레벨 생명력 공격력 방어력  //다른사람 애완동물 상태 정보
	int cnt = 0;
	char strTemp[256];
	while(strlen(arg)>2 || strcmp(arg,"\0") != 0) //조건
	{
		switch(cnt)
		{
		case 0:
			arg = AnyOneArg(arg, g_Buf);
			break; //이름
		case 1:
			arg = AnyOneArg(arg, g_Buf1);
			break;//레벨
		case 2:
			arg = AnyOneArg(arg, g_Buf2);
			break;//생명력
		case 3:
			arg = AnyOneArg(arg, g_Buf3);
			break;//공격력
		case 4:
			arg = AnyOneArg(arg, g_Buf4);
			break;//방어력
		case 5: // 라비용 클래스(타입)
			arg = AnyOneArg(arg, strTemp);
			g_ii[0] = atoi( strTemp );
			break;
		case 6: // 스킬1 레벨
			arg = AnyOneArg(arg, strTemp);
			g_ii[1] = atoi( strTemp );
			break;
		case 7: // 스킬2 레벨
			arg = AnyOneArg(arg, strTemp);
			g_ii[2] = atoi( strTemp );
			break;
		case 8: // 스킬3 레벨
			arg = AnyOneArg(arg, strTemp);
			g_ii[3] = atoi( strTemp );
			break;
		case 9: // 스킬4 레벨
			arg = AnyOneArg(arg, strTemp);
			g_ii[4] = atoi( strTemp );
			break;
		}
		cnt++;
	}

	pMgr->CmdPetExchOppInfo(g_Buf,g_Buf1,g_Buf2,g_Buf3,g_Buf4,g_ii[0],g_ii[1],g_ii[2],g_ii[3],g_ii[4]);
}

void CmdPetProduct(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;
//	p_compo 0 //조합 실패
//  p_compo 1 vnum index slotX slotY //조합성공
	BOOL bSuccess = 0;
	arg = AnyOneArg(arg, g_Buf);
	bSuccess = atoi(g_Buf);
	if(bSuccess)
	{
		arg = AnyOneArg(arg, g_Buf); // vnum
		g_ii[0] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf); // index
		g_ii[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf); // slotX
		g_ii[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);// sloatY
		g_ii[3] = atoi(g_Buf);
		pMgr->PetAssemblySuccess(g_ii[0],g_ii[1],g_ii[2],g_ii[3] );
		pMgr->PetAssemblyFail(FALSE);
	}
	else
		pMgr->PetAssemblyFail(TRUE);
}

// 플래티늄관련 함수
void CmdPlatinumExch(char *arg, CUIMgr *pMgr)
{
	g_ii[0] = g_ii[1] = g_ii[2] = g_ii[3] = g_ii[4] = -1;
	arg = AnyOneArg(arg, g_Buf);		//0은 실패, 1은 성공
	g_ii[0] = atoi(g_Buf);

	if(g_ii[0]==0)
	{
		//실패
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf); //어떤 실패인가.
	}
	else
	{
		//성공
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);
		if(g_ii[1]==0)
		{
			//아이템
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[4] = atoi(g_Buf);
		}
		else if(g_ii[1]==1)
		{
			//+아이템
			arg = AnyOneArg(arg, g_Buf);
			g_ii[2] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[3] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_ii[4] = atoi(g_Buf);
		}
	}
	if( pMgr )
		pMgr->PlatimumExch(g_ii[0],g_ii[1],g_ii[2],g_ii[3],g_ii[4]);
}

// 여름 사냥 이벤트 관련 함수
void CmdSummerEvent(char *arg, CUIMgr *pMgr)
{
	g_ii[0] =g_ii[1] =g_ii[2] = g_ii[3] = g_ii[4] = -1;
	arg = AnyOneArg(arg, g_Buf);		//0은 실패, 1은 성공
	g_ii[0] = atoi(g_Buf);

	if(g_ii[0]==0)
	{
		//실패
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf); //어떤 실패인가.
	}
	else if(g_ii[0] == 1)
	{
		//성공
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);
	}
	if( pMgr )
		pMgr->SummerEvent(g_ii[0],g_ii[1],g_ii[2],g_ii[3]);
}

// nate
// 오토마우스 사용제한 관련 함수
void CmdDictionary(char *arg, CUIMgr *pMgr)
{
	// 두단어 입력값
	arg = AnyOneArg(arg,g_Buf);

	// 인증성공과 실패를 서버에서 체크해서 보내주도록 바꾸었다. g_Buf에는 "fail", "ok", 숫자가 들어올 수 있다. ( by 원석 ) ///숫자입력시스템보완
	if( strcmp(g_Buf,"error") == 0 ) // 이 실패는 입력 실패 또는 인증 실패. 마을로 보내지진 않았고 기회가 남아있는다.
	{
		// 실패시 한번만 다시 보내보도록 한다.(시간값이 갱신되는 타이밍시 서버와의 차이등으로 실패할수도 있으므로.)
		if( !g_pNk2DFrame->GetAutoMouse()->m_bOneFailed ) // 첫 실패시..
		{
			// 한번 더 보내준다.(서버와의 시간 경계선 때문에 실패할 경우를 생각.)
			char *buf;
			buf = g_pNk2DFrame->GetAutoMouse()->m_pTextIn->GetString();// or buf = m_pTextIn->GetStrBuf();

			int nSize = sizeof( buf ) / sizeof( char );
			char arStr[ 256 ] = {0,};

			int i = 0;
			while( *buf )
			{
				if( !IsSpace( *buf ) )
				{
					arStr[ i++ ] = *buf;
				}
				buf++;
			}

			g_pNk2DFrame->GetAutoMouse()->SendConfirm( arStr );

			g_pNk2DFrame->GetAutoMouse()->m_bOneFailed = TRUE;
		}
		else // 두번째 실패시
		{
			// 기존의 껏을 지워준다. (여기서 해야지 정상적인 타이밍이 나온다.)
			g_pNk2DFrame->GetAutoMouse()->m_pTextIn->ClearText();
			g_pNk2DFrame->GetAutoMouse()->SetTextInFocus();
		}

		return;
	}
	else if( strcmp(g_Buf,"fail") == 0 ) // 완전 실패. 마을로 보내졌음.
	{
		g_pNk2DFrame->GetControlBottom()->m_CurTextView = g_pNk2DFrame->GetAutoMouse()->m_CurTextView;//버튼
		g_pNk2DFrame->GetControlBottom()->m_bAct = g_pNk2DFrame->GetAutoMouse()->m_bCurAct;//창크기
		g_pNk2DFrame->ShowAutoMouseWindow(FALSE);
		g_pNk2DFrame->GetAutoMouse()->m_pTextIn->ClearText();
		g_pNk2DFrame->GetAutoMouse()->m_pTextIn->SetEnable(FALSE);

		return;
	}
	else if( strcmp(g_Buf,"ok") == 0 )
	{
		// 이 부분의 처리는 단순히 시스템 패널을 닫아줄 뿐이다. 만일 해커가 임의로 클라이언트에게 ok사인을 보냈어도 무의미하다.(벌칙은 서버에서 판단해서 내리니까)
		g_pNk2DFrame->GetControlBottom()->m_CurTextView = g_pNk2DFrame->GetAutoMouse()->m_CurTextView;//버튼
		g_pNk2DFrame->GetControlBottom()->m_bAct = g_pNk2DFrame->GetAutoMouse()->m_bCurAct;//창크기
		g_pNk2DFrame->ShowAutoMouseWindow(FALSE);
		g_pNk2DFrame->GetAutoMouse()->m_pTextIn->ClearText();
		g_pNk2DFrame->GetAutoMouse()->m_pTextIn->SetEnable(FALSE);

		return;
	}

	arg = AnyOneArg(arg,g_Buf1);

	// 오토마우스 확인
	pMgr->Dictionary(g_Buf,g_Buf1);
}

void CmdQnA(char *arg, CUIMgr *pMgr) ///QnA
{
	// 패킷명 다음엔 바로 질문 스트링이 따라온다.
	if( strcmp(arg," fail") == 0 ) // 실패. 창을 닫기 위해 사용.
	{
		g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // 창을 닫는다.

		return; // 단지 그것만..
	}

	g_pNk2DFrame->ShowAutoMouseWindow( TRUE, 1 ); // 질답 타입의 창을 보여준다.
	g_pNk2DFrame->GetAutoMouse()->m_pMunjaText->SetString( arg );
	g_pNk2DFrame->GetAutoMouse()->m_pHelpInfo->SetIndex(-1); // 툴팁을 새로 넣기 위해...

	CMultiText MultiText; // 이것을 이용해서 개행.
	MultiText.AddStringMulti( arg, 26, TRUE, RGB(255,255,0) );
	g_pNk2DFrame->GetAutoMouse()->m_pHelpInfo->PutString( &MultiText ); // 이쪽 서피스로 만든다.

	g_pNk2DFrame->GetAutoMouse()->SetTextInFocus();
}

void CmdQuiz(char *arg, CUIMgr *pMgr) // Quiz 이벤트.
{
	// 패킷명 다음엔 바로 질문 스트링이 따라온다.
	if( strcmp(arg," fail") == 0 ) // 실패. 창을 닫기 위해 사용.
	{
		g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // 창을 닫는다.

		return; // 단지 그것만..
	}

	g_pNk2DFrame->ShowAutoMouseWindow( TRUE, 2 ); // 질답 타입의 창을 보여준다.

	g_pNk2DFrame->GetAutoMouse()->m_pMunjaText->SetString( arg );
	g_pNk2DFrame->GetAutoMouse()->m_pHelpInfo->SetIndex(-1); // 툴팁을 새로 넣기 위해...

	CMultiText MultiText; // 이것을 이용해서 개행.
	MultiText.AddStringMulti( arg, 26, TRUE, RGB(255,255,0) );
	g_pNk2DFrame->GetAutoMouse()->m_pHelpInfo->PutString( &MultiText ); // 이쪽 서피스로 만든다.

	g_pNk2DFrame->GetAutoMouse()->SetTextInFocus();

//	g_pNk2DFrame->GetAutoMouse()->m_OpenTick = timeGetTime(); // 오픈한 시간을 기억해서 일정시간 입력없으면 닫히게 한다.
}

void CmdJewelExchange(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;
	arg = AnyOneArg(arg, g_Buf);		// 0은 실패, 1은 성공
	g_ii[0] = atoi(g_Buf);

	if (g_ii[0] == 0)
		pMgr->JewelExchange(0, -1, -1);
	else if ( g_ii[0] == 1 )
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);		// 바뀔 아이템 번호.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);		// server index

		pMgr->JewelExchange(1, g_ii[1], g_ii[2]);
	}
}

void CmdLoadZonePos(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;
	pMgr->CmdLoadZonePos();

	if( pMgr->m_bIsTeleportItem ) // 켜졌을 때만 이후 처리를 함.(by 원석)
	{
		arg = AnyOneArg(arg, g_Buf);
		int slot = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int item_index = atoi(g_Buf);

		for( int index = 0; index < MAX_TELEPORT_COUNT; index++ )
		{
			arg = AnyOneArg(arg, g_Buf);		// 존 인덱스
			g_ii[0] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);		// X 좌표
			g_ii[1] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);		// Y 좌표
			g_ii[2] = atoi(g_Buf);

			pMgr->m_pTeleportItem->SetZonePosString(index, g_ii[0], g_ii[1], g_ii[2]);
		}
		pMgr->m_pTeleportItem->SetSlot(slot);
	}
}

void CmdFreeUser( char* arg, CUIMgr* pMgr )
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf );

	if( g_ii[0] == 0 )
		g_bFreeUser = FALSE;
	else
		g_bFreeUser = TRUE;
}

void CmdUserType(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf );

	// 추가 필요
	// LP 양

	arg = AnyOneArg( arg, g_Buf );
	int lp = atoi( g_Buf );

	switch( g_ii[0] )
	{
	case 0:	// 일반결제 (30,90일)
		pCMyApp->m_IsPcBang_User = g_ii[0];
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pc , 0 );
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pre , 0 );
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Normal , 0 );
		break;

	case 1: // 피시방 결제( 정량,정액결제? )
		pCMyApp->m_IsPcBang_User = 1;
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pc , 1 );
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pre , 0 );
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Normal , 0 );
		pCMyApp->m_PcBangStartTime = timeGetTime(); // [2008/5/29 Theodoric] Pc방 30분 체크
		break;

	case 2:	// 피시방 30분경과 메세지
		pCMyApp->m_PcBangStartTime = timeGetTime(); // [2008/5/29 Theodoric] Pc방 30분 체크
		{
			char buf[128] = {0,};
			sprintf( buf , G_STRING(IDS_LHSTRING1727), lp );
			g_pNk2DFrame->AddChatStr( buf, -1 );
		}
		break;
	case 3:
		{
			g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Normal , 1 );
			g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pc , 0 );
			g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pre , 0 );
			pCMyApp->m_PcBangStartTime = timeGetTime();
		}
		break;
	case 10:	// 프리미엄 결제 사용자
		pCMyApp->m_IsPcBang_User = 2;
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pre , 1 );
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Pc , 0 );
		g_pNk2DFrame->GetNewEvent()->SetEvent( OpEvent_Type__Normal , 0 );
		pCMyApp->m_PcBangStartTime = timeGetTime();
		break;

	case 20: // 프리미엄 30분경과
		pCMyApp->m_PcBangStartTime = timeGetTime();
		{
			char buf[128] = {0,};
			sprintf( buf , G_STRING(IDS_LHSTRING1727) , lp );
			g_pNk2DFrame->AddChatStr( buf, -1 );
		}
		break;
	case 30: // 개인결제 30분경과메세지
		pCMyApp->m_PcBangStartTime = timeGetTime();
		{
			char buf[128] = {0,};
			sprintf( buf , G_STRING(IDS_LHSTRING1727) , lp );
			g_pNk2DFrame->AddChatStr( buf, -1 );
		}
		break;
	}
	return;
}

void CmdDropCandy(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	if(g_ii[0]==0)
	{
		if (pMob = pMgr->FindMobById(g_ii[1]))
		{
			pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPCANDY );
		}
	}
	else
	{
		if (pCha = pMgr->FindChaById(g_ii[1], TRUE))
		{
			pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_DROPCANDY );
		}
	}

	
}

//////////////////////////////////////////////////////////////////////////


void CmdDropApple(char *arg, CUIMgr *pMgr)		//대빵 사과 이벤트용 임시...
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);


	if(g_ii[0]==0)
	{
		if (pMob = pMgr->FindMobById(g_ii[1]))
		{
			pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPAPPLE );
		}
	}
	else
	{
		if (pCha = pMgr->FindChaById(g_ii[1], TRUE))
		{
			pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_DROPAPPLE );
		}
	}

	/*if (pMob = pMgr->FindMobById(g_ii[0]))
	pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPAPPLE );*/
}

void CmdDropApplepvp(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);


	if(g_ii[0]==0)
	{
		if (pMob = pMgr->FindMobById(g_ii[1]))
		{
			pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPAPPLEPVP );
		}
	}
	else
	{
		if (pCha = pMgr->FindChaById(g_ii[1], TRUE))
		{
			pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_DROPAPPLEPVP );
		}
	}
}

void CmdDropCandypvp(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);


	if(g_ii[0]==0)
	{
		if (pMob = pMgr->FindMobById(g_ii[1]))
		{
			pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPCANDY90 );
		}
	}
	else
	{
		if (pCha = pMgr->FindChaById(g_ii[1], TRUE))
		{
			pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_DROPCANDY90 );
		}
	}
}

void CmdDropCandyvip(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);


	if(g_ii[0]==0)
	{
		if (pMob = pMgr->FindMobById(g_ii[1]))
		{
			pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPCANDY30 );
		}
	}
	else
	{
		if (pCha = pMgr->FindChaById(g_ii[1], TRUE))
		{
			pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_DROPCANDY30 );
		}
	}
}


void CmdDropStarCandy(char *arg, CUIMgr *pMgr) // 별사탕 이벤트용
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);
	if(g_ii[0]==0)
	{
		if (pMob = pMgr->FindMobById(g_ii[1]))
		{
			pMgr->InsertMagic(NULL, NULL, NULL, pMob, EFFECT_DROPSTARCANDY );
		}
	}
	else
	{
		if (pCha = pMgr->FindChaById(g_ii[1], TRUE))
		{
			pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_DROPSTARCANDY );
		}
	}
}

// nate
void CmdStashPut( char *arg, CUIMgr *pMgr )
// 아이템 이동 : 인벤 -> 창고
// arg : 서버에서 전송된 패킷 전체가 있다.
// 기능 : 패킷을 분할하여 UIMgr의 stashPut() 에 전달
{
	if( !pMgr )
		return;
	// AmyOneArg() 함수는 공백을 기준으로 하나씩 잘라 나가는 함수.
	arg = AnyOneArg( arg, g_Buf );

	g_ii[ 0 ] = atoi( g_Buf );	// 성공 여부 확인 메세지
	if( g_ii[ 0 ] == 1 )
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf );		// 아이템 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 2 ] = atoi( g_Buf );		// 인벤 팩 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 3 ] = atoi( g_Buf );		// 창고 팩 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 4 ] = atoi( g_Buf );		// 창고 슬롯 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 5 ] = atoi( g_Buf );		// 아이템 갯수

		for( int i = 0 ; i < g_ii[ 5 ]  ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );		// X 좌표
			g_ii[ 6 ] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );		// Y 좌표
			g_ii[ 7 ] = atoi( g_Buf );

			pMgr->StashPut( g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ], g_ii[ 4 ], g_ii[ 6 ], g_ii[ 7 ] );
		}
	}
	else
	{
		_asm nop;
	}
}

// nate
void CmdStashGet( char *arg, CUIMgr *pMgr )
// 아이템 이동 : 창고 -> 인벤
// arg : 서버에서 전송된 패킷 전체가 있다.
// 기능 : 패킷을 분할하여 UIMgr의 stashGet() 에 전달
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf );

	g_ii[ 0 ] = atoi( g_Buf );	// 성공 여부 확인 메세지
	if( g_ii[ 0 ] == 1 )
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf );		// 아이템 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 2 ] = atoi( g_Buf );		// 인벤 팩 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 3 ] = atoi( g_Buf );		// 창고 팩 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 4 ] = atoi( g_Buf );		// 창고 슬롯 번호
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 5 ] = atoi( g_Buf );		// 아이템 카운트

		for( int i = 0 ; i < g_ii[ 5 ]  ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );		// Item Index
			g_ii[ 6 ] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );		// X 좌표
			g_ii[ 7 ] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );		// Y 좌표
			g_ii[ 8 ] = atoi( g_Buf );
			pMgr->StashGet( g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ], g_ii[ 4 ],  g_ii[ 6 ], g_ii[ 7 ], g_ii[ 8 ] );
		}
	}
	else
	{
		_asm nop;
		// error code....
	}
}

// 유저 인덱스
void CmdUserIndex( char *arg, CUIMgr *pMgr )
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );
	if( g_pRoh )
		g_pRoh->m_nUserIndex = g_ii[ 0 ];
}
// 유저 ID
void CmdUserId( char *arg, CUIMgr *pMgr )
{
	arg = AnyOneArg( arg, g_Buf );
	strcpy(pCMyApp->m_strLetterUserId,g_Buf);
}
//쪽지도착 메시지
void CmdPostReceive( char *arg, CUIMgr *pMgr )
{
	if( !pMgr )
		return;
	arg = AnyOneArg( arg, g_Buf );
	pMgr->CreLetterMsgBox();
	pMgr->m_bShowLetterMsgBox = TRUE;
	g_bNewLetter = TRUE;//새쪽지 알림버튼
}

// nate 2004 - 4 : Crash_Miss
void CmdMiss( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;
	arg = AnyOneArg( arg, g_Buf );
	arg = AnyOneArg( arg, g_Buf1 );
	g_ii[ 0 ] = atoi( g_Buf1 );

	pMgr->ComputeMiss( g_Buf, g_ii[ 0 ] );

	arg = AnyOneArg( arg, g_Buf );
	g_ii[1] = atoi( g_Buf ); // 스킬 인덱스
	if( g_ii[ 1 ] == SKILL_B_PROTECTION_SHIELDS )
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_SHIELDBLOCK;

		pCha = pMgr->FindChaById(g_ii[0], TRUE);

		if( pCha )
		{
			ef_sort.pNkChaFrom = pCha;
			pCha->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));
		}
	}
}

void CmdBlock( char* arg, CUIMgr* pMgr ) // Block 효과. ///페어리
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf1 ); // 블럭 캐릭터 타입.(c or n)
	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf ); // 블럭 캐릭터 인덱스.

	arg = AnyOneArg( arg, g_Buf2 ); // 공격 캐릭터 타입.(c or n)
	arg = AnyOneArg( arg, g_Buf );
	g_ii[1] = atoi( g_Buf ); // 공격 캐릭터 인덱스.

	pMgr->ComputeBlock( g_Buf1, g_ii[0], g_Buf2, g_ii[1] );
}

// nate 2004 - 4 : exchange
// Inven -> Exch
void CmdExchPut( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		// Who( Target or Source )

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// unum

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 2 ] = atoi( g_Buf );		// plus

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 3 ] = atoi( g_Buf );		// special

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[11] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[12] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[13] = atoi(g_Buf); // 맥스 강화 내구도.

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[14] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[15] = atoi(g_Buf);			// 일반 내구도 Max
	}

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 4 ] = atoi( g_Buf );		// count

	// Source
	if( g_ii[ 0 ] == 0 )
	{
		// Exch
		for( int i = 0 ; i < g_ii[ 4 ] ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 5 ] = atoi( g_Buf );	// item index

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 6 ] = atoi( g_Buf );	// X

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 7 ] = atoi( g_Buf );	// Y

			if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
			{
				// Conpute
				pMgr->ExchPutExch( g_ii[ 5 ], g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ], g_ii[11], g_ii[12], g_ii[13],
								   g_ii[ 6 ], g_ii[ 7 ], g_ii[14], g_ii[15] ); ///신방어구강화시스템
			}
			else
			{
				pMgr->ExchPutExch( g_ii[ 5 ], g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ], g_ii[11], g_ii[12], g_ii[13],
								   g_ii[ 6 ], g_ii[ 7 ], 0, 0 ); ///신방어구강화시스템
			}
		}

		// Inven
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 8 ] = atoi( g_Buf );		// inven pack num

		for( int i = 0 ; i < g_ii[ 4 ] ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 9 ] = atoi( g_Buf );	// X

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 10 ] = atoi( g_Buf );	// Y

			// Compute
			pMgr->ExchPutInven( g_ii[ 8 ],  g_ii[ 9 ], g_ii[ 10 ] );
		}
	}
	// Target
	else if( g_ii[ 0 ] == 1 )
	{
		for( int i = 0 ; i < g_ii[ 4 ] ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 5 ] = atoi( g_Buf );	// item index

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 6 ] = atoi( g_Buf );	// X

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 7 ] = atoi( g_Buf );	// Y

			// Compute
			if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
			{
				pMgr->ExchPutTarget( g_ii[ 5 ], g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ], g_ii[11], g_ii[12], g_ii[13],
									 g_ii[ 6 ], g_ii[ 7 ], g_ii[14], g_ii[15] ); ///신방어구강화시스템
			}
			else
			{
				pMgr->ExchPutTarget( g_ii[ 5 ], g_ii[ 1 ], g_ii[ 2 ], g_ii[ 3 ], g_ii[11], g_ii[12], g_ii[13],
									 g_ii[ 6 ], g_ii[ 7 ], 0, 0 ); ///신방어구강화시스템
			}
		}
	}
}

// Exch -> Inven
void CmdExchGet( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );			// Who( Target or Source )

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );			// count

	// Source
	if( g_ii[ 0 ] == 0 )
	{
		// Exch
		for( int i = 0 ; i < g_ii[ 1 ] ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 2 ] = atoi( g_Buf );		// X

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 3 ] = atoi( g_Buf );		// Y

			// Compute
			pMgr->ExchGetExch( g_ii[ 2 ], g_ii[ 3 ] );
		}

		// Inven
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 4 ] = atoi( g_Buf );		// Inven Pack Num

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 5 ] = atoi( g_Buf );		// unum

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 6 ] = atoi( g_Buf );		// plus

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 7 ] = atoi( g_Buf );		// special

		///신방어구강화시스템
		arg = AnyOneArg(arg, g_Buf);
		g_ii[11] = atoi(g_Buf); // 추가 옵션 m_Special2.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[12] = atoi(g_Buf); // 현재 강화 내구도 수치.
		arg = AnyOneArg(arg, g_Buf);
		g_ii[13] = atoi(g_Buf); // 맥스 강화 내구도.

		if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[14] = atoi(g_Buf);			// 일반 내구도
			arg = AnyOneArg(arg, g_Buf);
			g_ii[15] = atoi(g_Buf);			// 일반 내구도 Max
		}

		for( int i = 0 ; i < g_ii[ 1 ] ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 8 ] = atoi( g_Buf );		// item index

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 9 ] = atoi( g_Buf );		// X

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 10 ] = atoi( g_Buf );		// Y

			// Compute
			if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
			{
				pMgr->ExchGetInven( g_ii[ 4 ], g_ii[ 8 ], g_ii[ 5 ], g_ii[ 6 ], g_ii[ 7 ], g_ii[11], g_ii[12], g_ii[13],
									g_ii[ 9 ], g_ii[ 10 ], g_ii[14], g_ii[15] ); ///신방어구강화시스템
			}
			else
			{
				pMgr->ExchGetInven( g_ii[ 4 ], g_ii[ 8 ], g_ii[ 5 ], g_ii[ 6 ], g_ii[ 7 ], g_ii[11], g_ii[12], g_ii[13],
									g_ii[ 9 ], g_ii[ 10 ], 0, 0 ); ///신방어구강화시스템
			}
		}
	}
	// Target
	else if( g_ii[ 0 ] == 1 )
	{
		for( int i = 0 ; i < g_ii[ 1 ] ; i ++ )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 2 ] = atoi( g_Buf );		// X

			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 3 ] = atoi( g_Buf );		// Y

			// Compute
			pMgr->ExchGetTarget( g_ii[ 2 ], g_ii[ 3 ] );
		}
	}
}
//=================================================

//==============================================================================
// nate 2004 - 4
// CastleWar
void CmdDecardiCastleWarTime( char* arg, CUIMgr* pMgr )
{
	arg = AnyOneArg( arg, g_Buf );

	if( pMgr && pMgr->m_pLowGuildMaster )
		pMgr->m_pLowGuildMaster->SetSiegeDateString( arg );
}
//==============================================================================

//==============================================================================
// nate 2004 - 4
// CastleWar
void CmdDecardiCaslteWarGift( char* arg, CUIMgr* pMgr )
{
	if( !g_pRoh )
		return;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );

	if( g_ii[ 0 ] > 0 )
		g_pRoh->m_byShowGiftMessage = 1;
	else
		g_pRoh->m_byShowGiftMessage = 0;
}
//==============================================================================
void CmdPKItemDrop(char *arg, CUIMgr *pMgr)
{
	if( !pMgr )
		return;
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		// inven_num

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// slot_x

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 2 ] = atoi( g_Buf );		// slot_y

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 3 ] = atoi( g_Buf );		// width

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 4 ] = atoi( g_Buf );		// heith

	pMgr->PKItemDrop(g_ii[ 0 ],g_ii[ 1 ],g_ii[ 2 ],g_ii[ 3 ],g_ii[ 4 ]);
}
//--------------------------------------------------------------------------
// Firework
//--------------------------------------------------------------------------
// 2004.09.10
// Lyul / 추가.
// 불꽃 표현.
void CmdFireWork(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		// inven_num

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// char

	if( !pCMyApp->m_pFxSet )
		return;

	// 해당 캐릭을 찾는다.
	pCha = pMgr->FindChaById(g_ii[1], TRUE);

	EffectSort ef_sort;
	ZeroMemory(&ef_sort, sizeof(EffectSort));
	// 불꽃 추가.
	if( pCha )
	{
		ef_sort.nType = FX_FIREWORKS;
		ef_sort.pNkChaTo = pCha;// g_pRoh;
		ef_sort.vPos = D3DVECTOR(pCha->m_wx, pCha->m_wy, pCha->m_wz);
		ef_sort.num =g_ii[0];
		if( g_pDSound )
		{
			switch(ef_sort.num)
			{
			case 0:
				g_pDSound->Play(g_EffectSound[EFF_SND_FIREWORKS_4]);
				break;
			case 1:
				g_pDSound->Play(g_EffectSound[EFF_SND_FIREWORKS_RAINBOW]);
				break;
			case 2:
				g_pDSound->Play(g_EffectSound[EFF_SND_FIREWORKS_SCREW]);
				break;
			case 3:
				g_pDSound->Play(g_EffectSound[EFF_SND_FIREWORKS_FOUNTAIN]);
				break;
			}
		}

		pCha->InsertEffectNum( pCMyApp->m_pFxSet->InsertFxEffect(ef_sort) );
	}
}
//--------------------------------------------------------------------------
// MixItem
//--------------------------------------------------------------------------
// 2004.09.10
// Lyul / 추가.
// 교환창에 있는 모든 아이템을 지우고 새로오는 아이템을 넣는다.
void CmdMixItem(char* arg, CUIMgr* pMgr)
{
	CItem *pItem = NULL;
	CItem *nextItem = NULL;

	pItem = g_pRoh->m_ExgInven;

	//------------------------------------------------------------------------
	// Remove All
	while (pItem)
	{
		nextItem = pItem->m_Next;
		delete pItem;
		pItem = nextItem;
	}
	g_pRoh->m_ExgInven = NULL;

	int x_idx, y_idx;

	for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
	{
		for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
			g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
	}
	//------------------------------------------------------------------------
	// 교환창에 추가
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		// index

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// vnum

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 2 ] = atoi( g_Buf );		// slot_x

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 3 ] = atoi( g_Buf );		// slot_y

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 4 ] = atoi( g_Buf );		// gem_num

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 5 ] = atoi( g_Buf );		// special

	///신방어구강화시스템
	arg = AnyOneArg(arg, g_Buf);
	g_ii[6] = atoi(g_Buf); // 추가 옵션 m_Special2.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[7] = atoi(g_Buf); // 현재 강화 내구도 수치.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[8] = atoi(g_Buf); // 맥스 강화 내구도.

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[9] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[10] = atoi(g_Buf);			// 일반 내구도 Max
	}

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		g_pRoh->AddToExgInven(g_ii[2], g_ii[3], g_ii[0], g_ii[1], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8], g_ii[9], g_ii[10], false); ///신방어구강화시스템
	else
		g_pRoh->AddToExgInven(g_ii[2], g_ii[3], g_ii[0], g_ii[1], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8], 0, 0 , false); ///신방어구강화시스템
}
//=====================================================================================
// nate 2004 - 9
// GuildHunt
void CmdGuildHunt( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;

	int i;
	arg = AnyOneArg( arg, g_Buf );
	if( !strcmp( g_Buf, "op" ) )
	{
		if( pMgr->m_pGuildHunt )
		{
			arg = AnyOneArg( arg, g_Buf );	// 날짜 셋팅
			pMgr->m_pGuildHunt->SetGuildDate( g_Buf );

			for( i = 0 ; i < 5 ; i ++ )
			{
				arg = AnyOneArg( arg, g_Buf );	// 길드 이름
				arg = AnyOneArg( arg, g_Buf1 );	// 신청자
				arg = AnyOneArg( arg, g_Buf2 );	// 상태
				g_ii[ 0 ] = atoi( g_Buf2 );

				pMgr->m_pGuildHunt->SetReservationInform( g_Buf, g_Buf1, g_ii[ 0 ], i );
			}
		}
	}
	else if( !strcmp( g_Buf, "ok" ) )
	{
		if( !g_pNk2DFrame )
			return;

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf );

		if( g_ii[ 0 ] == 0 )
			// IDS_GUH_COMP_CANCLE : 취소 완료
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_COMP_CANCLE),TYPE_NOR_OK, 1);
		else if( g_ii[ 0 ] == 1 )
			// IDS_GUH_COMP_REQ : 신청 완료
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_COMP_REQ),TYPE_NOR_OK, 1);
	}
	else if( !strcmp( g_Buf, "err" ) )
	{
		if( !g_pNk2DFrame )
			return;
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf );

		if( g_ii[ 0 ] == 0 )
			// IDS_GUH_DENIDE	: 유저님은 권한이 없습니다.
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_DENIDE),TYPE_NOR_OK, 1);
		else if( g_ii[ 0 ] == 1 )
			// IDS_GUH_LAIM	: 소지금이 부녹합니다.
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_LAIM),TYPE_NOR_OK, 1);
		else if( g_ii[ 0 ] == 2 )
			// IDS_GUH_MIS_INFO	: 신청 정보가 맞지 않습니다.
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_MIS_INFO),TYPE_NOR_OK, 1);
		else if( g_ii[ 0 ] == 3 )
		{
			// IDS_GUH_LIMIT_GU	: 한 길드는 1일 2회 ㅇ이상 예약이 불가능 합니다.
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_LIMIT_GU),TYPE_NOR_OK, 1);
			SAFE_DELETE( pMgr->m_pReservationWindow );
			pMgr->m_bIsReservationWindow = FALSE;
		}
		else if( g_ii[ 0 ] == 4 )
			// IDS_GUH_LIMIT_MEMBER	: 입장인원 초과 입니다.
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_LIMIT_MEMBER),TYPE_NOR_OK, 1);
		else if( g_ii[ 0 ] == 5 )
			// IDS_GUH_MIS_TIME	: 신청 시간이 맞지 않습니다.
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_GUH_MIS_TIME),TYPE_NOR_OK, 1);

		SAFE_DELETE( pMgr->m_pReservationWindow );
		pMgr->m_bIsReservationWindow = FALSE;
	}
}

// 길드 사냥터 문열기
void CmdGuildHuntDoor( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );	// 문 개폐여부

	if( g_ii[ 0 ] == 0 )	// 닫힘
		pMgr->GuildHuntLandAttr( FALSE );
	else	// 열림
		pMgr->GuildHuntLandAttr( TRUE );
}
//=====================================================================================


//=====================================================================================
// nate 2004 - 10
// Decardi CastleWar
// 각 나라와 서버마다 감마, 델타 인덱스가 다르다
// 따라서, 서버에서 감마, 델타의 인덱스를 순서대로 보낸다
// 아래의 순서로 감마, 델타 인덱스를 구분하여
// 공성시 감마, 델타 구분 바닦호전 판을 그린다.
void CmdDecardiGuildIndex( char* arg, CUIMgr *pMgr )
{
	if( !pMgr )
		return;
	arg = AnyOneArg( arg, g_Buf );		// 감마 길드
	pMgr->SetDecardiGuildName( g_Buf, 1 );
	arg = AnyOneArg( arg, g_Buf );		// 델타 길드
	pMgr->SetDecardiGuildName( g_Buf, 2 );
}
//=====================================================================================

//--------------------------------------------------------------------------
// CmdUpgradeBR
//--------------------------------------------------------------------------
// 팔찌 업그레이드 결과를 받는다.
// 2004.10.18
// Lyul / 추가.
// 팔찌 업으 메카니즘은 다음과 같다.
// 1. 추가가 결정되면 Extra Slot 은 무조건 지운다.
// 2. 서버에서 추가가 되면 속성을 응답으로 준다.
// 3. 추가가 불 가능이 이유를 알리고 pick_up 으로 다시Extra Slot 에 쥐어 준다.
// 4. 팔찌를 뺄 경우에도 그냥 쥐어 준다.
// 5. 이 함수에서 아이템을 지울 경우는 팔찌가 깨질 경우이다.
void CmdUpgradeBR(char* arg, CUIMgr *pMgr )
{
	CItem *pItem = g_pRoh->m_Wearing[IARMOR_BRACELET];

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		// result

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// Item_Flag

	pCMyApp->m_bBrLock= FALSE;// 팔찌 락 해제

	for(int i=0; i<=5; i++)
		if( g_pRoh )
			g_pRoh->m_bQuickSlotLock[i] = FALSE;

	// 성공.
	if(g_ii[0] ==1)
	{
		if( pMgr
				&& g_pNk2DFrame
				&& g_pNk2DFrame->GetControlInven() )
		{
			g_pNk2DFrame->GetControlInven()->ResetItemInfo();
			pMgr->SetBRFalg(pItem, g_ii[1]);
		}
	}
	// 깨어짐.
	if(g_ii[0] == -1)
		if( g_pRoh )
			SAFE_DELETE(g_pRoh->m_Wearing[IARMOR_BRACELET]);
}

// nate 2004 - 11 : Hack Program Check
void CmdHackCheck( char* arg, CUIMgr* pMgr )
{
	arg = AnyOneArg( arg, g_Buf );
	if( !strcmp( g_Buf, "go_world" ) )
	{
		pCMyApp->m_bBeforeGoWorld = FALSE;		
	}
}

// 디바인 스톤에 관한 처리를 한다.
void CmdDstone( char* arg, CUIMgr* pMgr )
{
	float x,z;
	float dir;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		// flag

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		// char idx

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 2 ] = atoi( g_Buf );		// Mob idx

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 3 ] = atoi( g_Buf );		// Mob Vunm

	arg = AnyOneArg( arg, g_Buf );
	dir = atof( g_Buf );			// char dir
	dir /=10.0f;

	arg = AnyOneArg( arg, g_Buf );
	x = atof( g_Buf );				// char x

	arg = AnyOneArg( arg, g_Buf );
	z = atof( g_Buf );				// char z

	// 해당 캐릭을 찾는다.
	pCha = pMgr->FindChaById(g_ii[1], FALSE);

	// 생성
	// 이 시점에서는 만들어지지 않고 해당 캐릭의 FrameMove 에서 만들어진다.주의!!
	if(g_ii[0])
	{
		// 해당 캐릭의 디바인 스톤을 실행한다.
		if(pCha && pCha->m_Wearing[WEARING_CLOAK])
		{
			// Motion
			pCha->m_dir = dir;
			pCha->m_wx = x;
			pCha->m_wz = z;
			pCha->SetMotionState(CHA_F_DSTONE);

			// Effect
			pMgr->InsertMagic(pCha, NULL, pCha, NULL, SKILLF_A_DSTONE);

			// Add mob (char)
			pCha->m_bresstone = true;    // 디바인 스톤이 생성될 준비가 됐는가.
			pCha->m_resmobidx = g_ii[2];    // 생성될 몹의 인덱스
			pCha->m_resmobvnum = g_ii[3];   // 생성될 몹의 vnum
			pCha->m_resmobx =x + cos(dir)  * 15.0f;	  // 생성될 몹의 좌표
			pCha->m_resmobz =z + sin(dir)  * 15.0f;
			pCha->m_resmobdir = dir;
		}
		// 플레이어의 몹
		else if(g_ii[1] == g_pRoh->m_nCharIndex)
		{
			// Add mob (player)
			g_pRoh->m_bresstone = true;    // 디바인 스톤이 생성될 준비가 됐는가.
			g_pRoh->m_resmobidx = g_ii[2];    // 생성될 몹의 인덱스
			g_pRoh->m_resmobvnum = g_ii[3];   // 생성될 몹의 vnum
			g_pRoh->m_resmobx =x + cos(dir)  * 15.0f;	  // 생성될 몹의 좌표
			g_pRoh->m_resmobz =z + sin(dir)  * 15.0f;
			g_pRoh->m_resmobdir = dir;
			int cur_skill = g_pNk2DFrame->GetRSkill();
			G_SK_SetCooltime(SKILLF_A_DSTONE);
		}
	}
	// 삭제
	else
	{
		if(g_ii[1]  == g_pRoh->m_nCharIndex) // 플레이어
		{
			pMgr->DelMob(g_ii[2]);
			g_pRoh->m_bresstone = false;
			g_pRoh->m_bstone = false;

			G_SK_INVCooltime(SKILLF_A_DSTONE);
		}
		else // 다른 유저
		{
			pMgr->DelMob(g_ii[2]);
			if(pCha)
				pCha->m_bresstone = false;
			if(pCha)
				pCha->m_bstone = false;
		}
	}
}

//wan:2004-12 셀프정제
void CmdSeMs(char *arg, CUIMgr *pMgr)
{
	if( !g_pRoh || !pMgr )
		return;

	arg = AnyOneArg(arg, g_Buf);

	if (!strcmp(g_Buf, "success"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf); // clear falg
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf); // index
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf); // vnum
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf); // x
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf); // y

		if(g_ii[1])	//클리어
		{
			CItem *pItem = NULL;
			CItem *nextItem = NULL;
			pItem = g_pRoh->m_ExgInven;

			// Remove All
			while (pItem)
			{
				nextItem = pItem->m_Next;
				delete pItem;
				pItem = nextItem;
			}
			g_pRoh->m_ExgInven = NULL;

			int x_idx, y_idx;

			for (x_idx = 0; x_idx < EXG_SLOT_X_NUM; x_idx++)
			{
				for (y_idx = 0; y_idx < EXG_SLOT_Y_NUM; y_idx++)
					g_pRoh->m_ExgSlot[x_idx][y_idx] = -1;
			}
		}
		//아이템 삽입
		g_pRoh->AddToExgInven(g_ii[4], g_ii[5], g_ii[2],  g_ii[3], 0, 0, 0, 0, 0, 0, FALSE); ///신방어구강화시스템
	}
	else if (!strcmp(g_Buf, "cancel"))
		pMgr->MsFail();
}
//==========================================================
//wan:2004-12	화이트혼 던젼 중앙광장 타임 샵
void CmdShopOpen(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	if ( pMgr
			&& g_pNk2DFrame
			&& !strcmp(g_Buf, "0"))
		g_pNk2DFrame->ShowShopWindow(FALSE);
}

//==========================================================
// nate 2005-03-18
// Matrix
// 매트릭스 : 미니맵 지시 포인트 출력
void CmdMini( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;
	arg = AnyOneArg( arg, g_Buf );	// X 좌표
	g_ii[ 0 ] = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// Y 좌표
	g_ii[ 1 ] = atoi( g_Buf );

	if( g_pNk2DFrame
			&& g_pNk2DFrame->GetInterface(MAP) )
	{
		((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_bIndication_Point = 2;
		((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_dwTimeIndication = timeGetTime();
		((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_nCurClick_X = ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_minimap_x + g_ii[ 0 ];
		((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_nCurClick_Y = ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_minimap_y + g_ii[ 1 ];

		g_pNk2DFrame->AddChatStr( arg, 6 );	// arg : 지휘자가 위치명령을 하였습니다.
	}

	if (g_pDSound)
		g_pDSound->Play(g_EffectSound[EFF_SND_MINI]); // 사운드 종류는 후에 바꿔주자.
}
//==========================================================

void CmdMatBP(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );//오늘 획득 BP
	arg = AnyOneArg(arg, g_Buf);
	g_ii[ 1 ] = atoi(g_Buf); // 누적 BP
	arg = AnyOneArg(arg, g_Buf);
	g_ii[ 2 ] = atoi(g_Buf); // GHP

	if( pMgr )
		pMgr->MatBattlePoint( g_ii[ 0 ] );// 오늘 BP 출력

	if( g_pRoh )
	{
		g_pRoh->SetBattlePoint(g_ii[ 1 ]);// 누적 BP 저장
		g_pRoh->SetGHPoint(g_ii[ 2 ]);// 누적 GHP 저장
	}

	// 만약 상점이 열려 있으면 상점쪽 돈 표시부분을 갱신 해준다.(BP상점일 경우 자신의 BP잔량 갱신을 위해서) (by 원석)
	if( pMgr
			&& g_pNk2DFrame
			&& g_pNk2DFrame->IsShopVisible() )
		g_pNk2DFrame->GetControlShop()->SetMoney();  // 사실 라임 상점일 경우는 여기서 안해줘도 되긴한다.
}


//wan:2005-03 wan_mat
//매트릭스 배틀 스타트, 엔드(win,lose)
void CmdBattle(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "start"))
		pCMyApp->CreateBattleStart(2);
	else if(!strcmp(g_Buf,"end"))
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf );
		pCMyApp->CreateBattleStart(g_ii[ 0 ]);
	}
	else if(!strcmp(g_Buf,"cancel")) // 카운트 취소 ///인던1
	{
		pCMyApp->DeleteBattleStart();
		pCMyApp->DrawBattleStart = FALSE;
	}
}

// nate 2005-04-22 : 콤보 출력하기
void CmdCombo( char* arg, CUIMgr* pMgr )
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );
	arg = AnyOneArg(arg, g_Buf);
	g_ii[ 1 ] = atoi(g_Buf);		//성공한 프레임.

	pMgr->ComputeCombo( g_ii[ 0 ], g_ii[ 1 ] );
}

// nate 2005-05-09 : 이벤트포인트(EP)획득시 서버 메세지에 의해 나타남
void CmdEventPoint( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;
	arg = AnyOneArg( arg, g_Buf1 );	// 캐릭 인덱스
	g_ii[ 0 ] = atoi( g_Buf1 );

	pMgr->ComputeEventPoint( g_ii[ 0 ] );
}

void CmdItemExtraInfo( char* arg, CUIMgr* pMgr ) // 아이템 별로 추가적인 정보가 필요할 때 사용되는 패킷. 시간제 아이템도 이 패킷으로 정보가 온다.(by 원석) ///시간제아이템
{
	arg = AnyOneArg( arg, g_Buf );

	if( !strcmp(g_Buf,"remain") ) // 남은 시간 패킷.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf ); // 아이템 인덱스.(고유번호)

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf ); // 남은 시간 초단위.

		if( pMgr )
			pMgr->SetItemTimeLimit( g_ii[0], g_ii[1] ); // 시간제 아이템 셋팅.		
	}
	else if( !strcmp(g_Buf,"del") ) // 시간 만료시 아이템 삭제.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf ); // 아이템 인덱스.(고유번호)

		if( g_pRoh )
		{
			CItem *pItem = g_pRoh->FindItemByIndexFromAll( g_ii[0] );
			if( pItem ) // 아이템이 확실히 있다면..
			{
				if( pItem->m_bTimeLimit ) // 시간제 아이템만 사라졌다는 메세지 출력하게 수정 07-04-18 ///다중판매
				{
					char temp[256];
					sprintf( temp, (char*)G_STRING(IDS_ITEM_REMOVED), GET_ITEM_NAME_VNUM(pItem->m_Vnum) );
					g_pNk2DFrame->AddChatStr( temp, -1 );
				}

				g_pRoh->RemoveByIndex( g_ii[0] ); // 06-05-09 위치수정 원석.
				g_pRoh->CountQuickAll(); // 퀵슬롯 아이템을 넣었을 수도 있으니 다시 세어준다. RemoveByIndex() 안쪽에서 하기는 좀 무리가 있다. ///07-05-31
			}
		}
	}
	else if( !strcmp(g_Buf,"price") ) // 특별지정 가격.(속성 플래그에 특별지정가격 아이템인지 여부가 표시되어있다.)
	{
		// 여기서 속성 플래그를 체크해주는 것도 좋다.

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf ); // 아이템 인덱스.(고유번호)

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf ); // 가격타입 값, 라임(0), BP(1) 등이 온다.

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 2 ] = atoi( g_Buf ); // 가격 값.

		if( pMgr )
			pMgr->SetItemExPrice( g_ii[0], g_ii[1], g_ii[2] ); // 특정가격을 지정한다.
	}
	else if( !strcmp(g_Buf,"all") ) // 시간제 이면서 지정 가격 아이템인 경우. 패킷을 두번 받기 뭐하니까 한번에 받는다.(로그인등의 경우에만 들어오는 걸로. 위의 remain과는 약간 틀리다. 이 녀석이 시간제 아이템이다 라는 것을 알려주기 위해 쓰인다고 보면될듯.)
	{
		if( !pMgr )
			return;

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 0 ] = atoi( g_Buf ); // 아이템 인덱스.(고유번호)

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf ); // 남은 시간 초단위.

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 2 ] = atoi( g_Buf ); // 가격타입 값, 라임(0), BP(1) 등이 온다.

		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 3 ] = atoi( g_Buf ); // 가격 값.

		if( g_ii[1] != 0 )
			pMgr->SetItemTimeLimit( g_ii[0], g_ii[1] ); // 시간제 아이템 셋팅.

		pMgr->SetItemExPrice( g_ii[0], g_ii[2], g_ii[3] ); // 특정가격을 지정한다.
	}

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
	{
		g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE;
	}
}

// nate 2005-07-08 : Summon Squad - 서버에서 전달받은 메세지 처리
void CmdSquad( char* arg, CUIMgr* pMgr )
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf );	// Char Index
	g_ii[ 0 ] = atoi( g_Buf );

	pCha = pMgr->FindChaById(g_ii[0], TRUE);
	if( pCha )
		pMgr->InsertMagic( pCha, NULL, NULL, NULL, EFFECT_SKILL_SUMMONSQUAD );
}


void CmdUpdateItem( char* arg, CUIMgr* pMgr )	// 아이템 정보 갱신용 범용 패킷(05-10-21). ///신방어구강화시스템
{
	if( !pMgr )
		return;

	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf ); // 인덱스

	arg = AnyOneArg( arg, g_Buf );
	g_ii[1] = atoi( g_Buf ); // vnum

	arg = AnyOneArg( arg, g_Buf );
	g_ii[2] = atoi( g_Buf ); // plus

	arg = AnyOneArg( arg, g_Buf );
	g_ii[3] = atoi( g_Buf ); // special

	arg = AnyOneArg( arg, g_Buf );
	g_ii[4] = atoi( g_Buf ); // special2

	arg = AnyOneArg( arg, g_Buf );
	g_ii[5] = atoi( g_Buf ); // 강화 내구도 현재치

	arg = AnyOneArg( arg, g_Buf );
	g_ii[6] = atoi( g_Buf ); // 강화 내구도 맥스치

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[7] = atoi( g_Buf );		// 일반 내구도
		arg = AnyOneArg( arg, g_Buf );
		g_ii[8] = atoi( g_Buf );		// 일반 내구도 Max
	}

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		pMgr->UpdateItem( g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8] );
	else
		pMgr->UpdateItem( g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], 0, 0 );
}

void CmdNpcUpgrade(  char* arg, CUIMgr* pMgr ) // 강화,변환,가공 등 NPC를 통한 업그레이드 패킷. ///신방어구강화시스템
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetUpgrade() )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 강화 애니메이션으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_ING;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].m_AniType = SPRITE_ANITYPE_LOOP; // 강화중 애니메이션은 무한 반복으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].SetStart(); // 애니메이션 시작.
	g_pNk2DFrame->GetUpgrade()->m_dwStartTickTime = timeGetTime(); // 애니메이션이 시작된 시간을 기억.

	switch( pCMyApp->GetCurWorld() )
	{
	case 4:
		if( !strcmp(g_Buf,"armor") || !strcmp(g_Buf,"weapon") || !strcmp(g_Buf,"conv") || !strcmp(g_Buf,"reload") || !strcmp(g_Buf,"refine")) // 강화,가공,변환등 구분..
		{
			arg = AnyOneArg( arg, g_Buf ); // 결과값.

			// 여기서 결과값을 기억해둔다.
			if( !strcmp(g_Buf,"succ") )
			{
				g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS; // 애니상태 열거형을 그대로 넣어도 된다.(동일하게 맞춰서 사용.)
			}
			else if( !strcmp(g_Buf,"fail") )
			{
				g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_FAIL;
			}
			else if( !strcmp(g_Buf,"brk") )
			{
				g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_BROKEN;
			}
			else if( !strcmp(g_Buf,"error") ) // 뭔가 이상이 생긴 경우. 애니메이션을 돌리지 않고 초기화 한다.
			{
				g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.
				g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
				g_pNk2DFrame->GetUpgrade()->m_Result = 0; // 결과값 초기화...
			}
			else if( !strcmp(g_Buf, "check") )
			{
				g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.
				g_pNk2DFrame->GetUpgrade()->m_Result = 0; // 결과값 초기화...

				arg = AnyOneArg( arg, g_Buf );
				int type = atoi(g_Buf);
				if( 1 == type )
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_UPGRADE_11_WARNING),TYPE_NOR_OKCANCLE, POPUP_NPCUPGRADE_BROKEN_CHECK);
				}
				else if( 2 == type )
				{
					g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LHSTRING1955),TYPE_NOR_OKCANCLE, POPUP_NPCUPGRADE_BROKEN_CHECK);
				}
			}

			if( g_pNk2DFrame->GetControlInven() )
			{
				g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE; // DrawItemInfo() 강제 업데이트.
			}
		}
		break;

		// 스트링 추가

	case 7:
		if( !strcmp(g_Buf,"conv") || !strcmp(g_Buf,"weapon") || !strcmp(g_Buf,"godpower") )
		{
			arg = AnyOneArg( arg, g_Buf ); // 결과값.

			if( !strcmp(g_Buf,"succ") )
			{
				g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS; // 애니상태 열거형을 그대로 넣어도 된다.(동일하게 맞춰서 사용.)
			}
			else if( !strcmp(g_Buf,"fail") )
			{
				g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_FAIL;
			}
			else if( !strcmp(g_Buf,"brk") )
			{
				g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_BROKEN;
			}
			else if( !strcmp(g_Buf,"error") || !strcmp(g_Buf,"unsuccessful") ) // 뭔가 이상이 생긴 경우. 애니메이션을 돌리지 않고 초기화 한다.
			{
				g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.
				g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
				g_pNk2DFrame->GetUpgrade()->m_Result = 0; // 결과값 초기화...
			}

			if( g_pNk2DFrame->GetControlInven() )
			{
				g_pNk2DFrame->GetControlInven()->m_bDrawInfoUpdate = TRUE; // DrawItemInfo() 강제 업데이트.
			}
		}
		break;
	}


}

void CmdGuildPos( char* arg, CUIMgr* pMgr ) // 길드원 위치 찾기 패킷. ///미니맵길드원찾기
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetInterface(MAP) )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 여기서 결과값을 기억해둔다.
	if( !strcmp(g_Buf,"start") )
	{
		((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->GuildFindTick = timeGetTime(); // 시간 갱신. 이 시간을 기준으로 출력한다. 타이밍이 중요하다.

		// 리스트 초기화.  ///미니맵길드원찾기3D
		vector<sGuildPosInfo>::iterator prGuildPosVector;
		prGuildPosVector = ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_GuildPosVector.begin();
		while( prGuildPosVector != ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_GuildPosVector.end() )
		{
			prGuildPosVector = ((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_GuildPosVector.erase( prGuildPosVector );
		}
	}
	else if( !strcmp(g_Buf,"body") ) // 길드원
	{
		sGuildPosInfo TempInfo;

		arg = AnyOneArg( arg, g_Buf );
		TempInfo.m_Grade = atoi( g_Buf ); // 길드 등급.

		arg = AnyOneArg( arg, g_Buf );
		strcpy( TempInfo.m_strName, g_Buf ); // 캐릭터명.

		arg = AnyOneArg( arg, g_Buf );
		TempInfo.m_PosX = atoi( g_Buf ); // x 좌표.

		arg = AnyOneArg( arg, g_Buf );
		TempInfo.m_PosY = atoi( g_Buf ); // y 좌표

		// 집어넣는다.
		((CControlMap*)g_pNk2DFrame->GetInterface(MAP))->m_GuildPosVector.push_back( TempInfo ); // 끝에 집어넣어야한다.
	}
	else if( !strcmp(g_Buf,"end") )
	{
		/*      ///07-04-10 길드원찾기 테스트용.
				// 임시 테스트.
				if( g_pTcpIp )
				{
					char strTemp[200];

					sprintf( strTemp, "callback m g_pos start\n" ); // 소셜 픸E?이거 보내줘야하더턿E
					g_pTcpIp->SendNetMessage( strTemp );
					sprintf( strTemp, "callback m g_pos body 1 a %d %d\n", (int)g_pRoh->m_wx, (int)g_pRoh->m_wz ); // 소셜 픸E?이거 보내줘야하더턿E
					g_pTcpIp->SendNetMessage( strTemp );
				}
		*/
	}
}

void CmdFillItem( char* arg, CUIMgr* pMgr ) // 아이템 충전용 패킷. (강화 내구도 충전 아이템 그레이톤 부터 이걸 사용하기로 함.) ///그레이톤
{
	if( !pMgr || !pCMyApp )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 이건 성공 실패만 있다. (추후 다른 이 패킷을 사용하는 아이템이 나올시 성공/실패 말고 타입도 추가해서 처리해주자.)
	if( !strcmp(g_Buf,"succ") )
	{
		// 성공시 아이템 정보 갱신을 해준다.
		if( pMgr && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
			g_pNk2DFrame->GetControlInven()->ResetItemInfo();
	}
	else if( !strcmp(g_Buf,"fail") )
	{
		// 마땅한 메세지가 없어서 일단 메세지 보류.
	}

}

void CmdMvSkill( char* arg, CUIMgr* pMgr )
{

	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf );		//스킬 인덱스

	if( g_ii[ 0 ] == SKILL_KNOCKBACK2 ) // 스피릿 넉백처리. 몬스터의 넉백도 가능하다. 패킷 구조가 다른 녀석과 틀려져서 여기에서 부터 검사.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[ 1 ] = atoi( g_Buf );		//스킬 사용자 인덱스

		pCha = pMgr->FindChaById(g_ii[1], TRUE);
		if( !pCha ) return;

		if( pCha->m_nCharIndex == g_pRoh->m_nCharIndex )	// 스피릿이 발동된 캐릭이 주캐릭인 경우
		{
			// 넉백은 공격을 끊어준다.
			pMgr->m_b_MotionForAttack = false;
			pMgr->m_b_MovingForAttack= false;
			pMgr->m_b_Attack= false;
			pMgr->m_b_SecAttack = false;
			pMgr->m_bChakramAtt = false;
			pMgr->m_bChakram = false;
			pMgr->m_bBookSpellAtt = false;
			pMgr->m_bBookSpell = false;

			// 06-05-25 공격 캔슬 초기화 추가.
			pCha->m_MagicChaId[0] = -1;
			pCha->m_MagicMobId[0] =  -1;
			pCha->m_MagicChaCount = 0;
			pCha->m_MagicMobCount = 0;
			pCha->m_AttackChaId = -1;
			pCha->m_AttackMobId = -1;

			pCMyApp->SetHeroAttGoMob( NULL ); // 공격 대상 포인터를 초기화(이 안에서 공격대상 몹 외에 캐릭터도 초기화 된다.)
		}

		arg = AnyOneArg( arg, g_Buf );
		g_ii[1] = atoi( g_Buf );  // 넉백 대상의 갯수.

		for( int i = 0 ; i < g_ii[1] ; ++i ) // 넉백 대상 만큼 루프.
		{
			arg = AnyOneArg( arg, g_Buf2 );	//npc/char(타겟)
			arg = AnyOneArg( arg, g_Buf );	//타겟 인덱스
			g_ii[2] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );
			float x = atof( g_Buf );		// x	//이동할 X좌표
			arg = AnyOneArg( arg, g_Buf );
			float z = atof( g_Buf );		// z	//이동할 Z좌표

			if( !strcmp(g_Buf2,"n") ) // 타겟이 몹인 경우
			{
				pMob = pMgr->FindMobById( g_ii[2] );

				if( pMob )
					pMob->MoveSkill(x, z, g_ii[0] );
			}
			else
			{
				pCha2 = pMgr->FindChaById( g_ii[2], TRUE );

				if( pCha2 )
					pCha2->MoveSkill(x, z, g_ii[0] );
			}
		}


		return;
	}


	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 1 ] = atoi( g_Buf );		//스킬 사용자 인덱스
	arg = AnyOneArg( arg, g_Buf2 );	//npc/char(타겟)
	arg = AnyOneArg( arg, g_Buf );	//타겟 인덱스
	g_ii[ 2 ] = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );
	float x = atof( g_Buf );		// x	//이동할 X좌표
	arg = AnyOneArg( arg, g_Buf );
	float z = atof( g_Buf );		// z	//이동할 Z좌표
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 3 ] = atof( g_Buf );		//스텝

	pCha = pMgr->FindChaById(g_ii[1], TRUE);
	if( !pCha ) return;

	if( g_ii[ 0 ] == SKILL_KNOCKBACK ) // 넉백 처리 ///페어리
	{
		if( pCha->m_nCharIndex == g_pRoh->m_nCharIndex )	// 주캐릭의 경우
		{
			// 넉백은 공격을 끊어준다.
			pMgr->m_b_MotionForAttack = false;
			pMgr->m_b_MovingForAttack= false;
			pMgr->m_b_Attack= false;
			pMgr->m_b_SecAttack = false;
			pMgr->m_bChakramAtt = false;
			pMgr->m_bChakram = false;
			pMgr->m_bBookSpellAtt = false;
			pMgr->m_bBookSpell = false;

			// 06-05-25 공격 캔슬 초기화 추가.
			pCha->m_MagicChaId[0] = -1;
			pCha->m_MagicMobId[0] =  -1;
			pCha->m_MagicChaCount = 0;
			pCha->m_MagicMobCount = 0;
			pCha->m_AttackChaId = -1;
			pCha->m_AttackMobId = -1;

			pCMyApp->SetHeroAttGoMob( NULL ); // 공격 대상 포인터를 초기화(이 안에서 공격대상 몹 외에 캐릭터도 초기화 된다.)
		}
		pCha->MoveSkill(x, z, g_ii[ 0 ]);

		return;
	}

	if( g_ii[ 0 ] != SKILLF_A_DASH )	// 대쉬는 타겟 셋팅 안함
	{
		if(!strcmp(g_Buf2,"n"))
			pCha->SetAttackMobId(g_ii[ 2 ]);
		else
			pCha->SetAttackChaId(g_ii[ 2 ]);
	}

	if( pCha->m_nCharIndex == g_pRoh->m_nCharIndex )	// 주캐릭의 경우
	{
		if( g_ii[ 0 ] != SKILL_B_FLYINGCRASH ) // 플라잉크래쉬의 경우 렉 발생시 중복사용이 되는것땜에 사용시 쿨타임돌리기로 바꾸었다. 07-03-22 원석
			G_SK_SetCooltime(g_ii[ 0 ]);

		if( g_ii[ 0 ] != SKILLF_A_DASH )
		{
			pMgr->m_b_MotionForAttack = true;
			pMgr->m_b_MovingForAttack= false;
			pMgr->m_b_Attack= true;
			pMgr->m_b_SecAttack = false;
		}
		else
		{
			//어택 비활성 시킨다.
			pMgr->m_b_MotionForAttack = false;
			pMgr->m_b_MovingForAttack= false;
			pMgr->m_b_Attack= false;
			pMgr->m_b_SecAttack = false;
		}

		if( g_ii[0] == SKILLF_A_DASH ) ///하이콤보
		{
			// 콤보스킬 체크용 틱을 설정.
			pCha->m_dwComboTick = timeGetTime();
		}

	}

	pCha->MoveSkill(x, z, g_ii[ 0 ]);
	if( g_ii[ 0 ] == SKILL_B_FLYINGCRASH )
		pCMyApp->m_pUIMgr->InsertMagic( pCha, NULL, NULL, NULL, EFFECT_SKILL_FLYINGCRASH2 );
	else if( g_ii[ 0 ] == SKILL_F_SHADOWSLASH )
		pCMyApp->m_pUIMgr->InsertMagic( pCha, NULL, NULL, NULL, EFFECT_SKILL_SHADOWSLASH );
}

void CmdUserShop( char* arg, CUIMgr* pMgr ) ///개인상점
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetControlUserShop() )
		return;

	char strTemp[200];
	int i = 0;
	__int64 ii64;

	arg = AnyOneArg( arg, g_Buf ); // 패킷 종류

	// 여기서 결과값을 기억해둔다.
	if( !strcmp(g_Buf,"in") ) // 시야 중 캐릭터가 상점을 열었음. 또는 시야 안으로...
	{
		arg = AnyOneArg( arg, g_Buf2 ); // 캐릭터인지 npc인지...

		if( !strcmp(g_Buf2,"c") ) // 유저 상점.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[0] = atoi( g_Buf ); // 캐릭터 인덱스

			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 상점 레벨.

			pCha = pMgr->FindChaById( g_ii[0], FALSE );
			if( pCha )
			{
				pCha->m_UserShopLevel = g_ii[1];

				strcpy( pCha->m_strUserShopMent, arg ); // m_strUserShopMent의 버퍼길이는 g_Buf의 길이와 똑같이 256으로 설정되어있다.멘트의 한계길이를 200정도로 잡자.

				// 멘트 출력등을 위한 팝업 객체를 셋팅한다.
				if( pCha->m_pUserShopPopup )
				{
					CMultiText multiText;
					char strTemp[100];

					//-- IDS_WHOS_USERSHOP : %s의 개인상점
					sprintf( strTemp, (char*)G_STRING(IDS_WHOS_USERSHOP), pCha->m_pstrName );
					multiText.AddString( strTemp, RGB(252, 180, 180) );
					// 문자열이 길엉痺툈E라인을 내리면서 출력.
					// 문자열 길이를 15에서 35으로 늘려 출력해보자. [2/8/2007 Theodoric]
					multiText.AddStringMulti( pCha->m_strUserShopMent, 15, TRUE, RGB(255, 251, 115) );
					pCha->m_pUserShopPopup->PutString(&multiText, TRUE);
				}
			}
		}
		else if( !strcmp(g_Buf2,"n") ) // 점포 상점.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[0] = atoi( g_Buf ); // 상점주인 캐릭터 인덱스

			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // Vnum

			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf ); // NpcIndex

			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf ); // x

			arg = AnyOneArg( arg, g_Buf );
			g_ii[4] = atoi( g_Buf ); // z

			arg = AnyOneArg( arg, g_Buf );
			g_ii[5] = atof(g_Buf); // dir

			arg = AnyOneArg( arg, strTemp ); // 이름. (arg는 나머지 멘트가 들어있음 )

			pMgr->AddUserShopNpc( g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], strTemp, arg );
		}
	}
	else if( !strcmp(g_Buf,"out") ) // 시야 중 캐릭터가 상점을 닫음.또는 시야 밖으로...
	{
		arg = AnyOneArg( arg, g_Buf );

		if( !strcmp(g_Buf,"c") ) // 유저 상점.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[0] = atoi( g_Buf ); // 인덱스

			pCha = pMgr->FindChaById( g_ii[0], FALSE );
			if( pCha )
			{
				pCha->m_UserShopLevel = 0;
			}
		}
		else if( !strcmp(g_Buf,"n") ) // 점포 상점.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[0] = atoi( g_Buf ); // 인덱스

			pMgr->DeleteUserShopNpc( g_ii[0] );
		}
	}
	else if( !strcmp(g_Buf,"open") ) // 열기 확인.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부

		if( g_ii[0] ) // 성공
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 캐릭터 인덱스 -1이면 자기자신 개인상점 오픈. 자기 자신 인덱스 면 자기자신 점포상점 오픈이다.

			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf ); // 점포상점인지 개인상점인지 플래그.(현재로서 클라이언트는 신경 안써도 된다.) 개인 0, 점포 1

			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf ); // 상점 그레이드. 팩 몇개짜리 상점인가? 서버에서 0,1,2 로 보내주니 +1해서 쓴다.

			g_pNk2DFrame->ToggleUserShopWindow( g_ii[1], g_ii[2], g_ii[3]+1 ); // 상점을 열어준다.

			// 집어넣는다.
			if( g_ii[1] == -1 && g_ii[2] == 0 ) // 판매자인 경우.(점포 말고) ///점포상점
			{
				// 않게 하기 처리...
				if (g_pRoh
//					&& !(g_pRoh->IsCantMove()) // 공격중인지를 체크하는 것인데 공격중이더라도 공격을 멈추게 하자.
						&& !(pCMyApp->GetHeroSwimming())
						&& g_pRoh->IsShown()
				   )
				{
					// 만약 공격중이면 공격중지. (강제로 중지시키는 만큼 뭔가 묹가 있을수도 있다.)
					if( pMgr->m_b_Attack || pMgr->m_b_SecAttack || pMgr->m_bChakramAtt || pMgr->m_bBookSpellAtt )
					{
						pMgr->m_b_Attack = false;
						pMgr->m_b_MovingForAttack = false;
						pMgr->m_b_SecAttack = false;
						pMgr->m_bChakramAtt = false;
						pMgr->m_bChakram = false;
						pMgr->m_bBookSpellAtt = false;
						pMgr->m_bBookSpell = false;
					}

					pCMyApp->StopHero();

					g_pRoh->SetMotionState(CHA_SOCIAL, 67 );
					if( g_pTcpIp )
					{
						sprintf( strTemp, "combat 0\n" ); // 소셜 전에 이거 보내줘야하더라.
						g_pTcpIp->SendNetMessage( strTemp );
						sprintf( strTemp, "social 67\n" );
						g_pTcpIp->SendNetMessage( strTemp );
					}
				}
			}

//			g_pNk2DFrame->GetControlUserShop()->m_bOpened = TRUE; // ShowUserShop()에서 하도록 수정.
		}
	}
	else if( !strcmp(g_Buf,"reg") ) // 아이템 넣기. 클라이언트에서 요청한것에대한 성공실패 답변,이걸 받은후에 실제로 옮겨준다.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부

		if( g_ii[0] ) // 성공
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // ต๎ทฯ น๘ศฃ

			arg = AnyOneArg( arg, g_Buf );
			int red =0;
			red = atoi( g_Buf ); // ภฮบฅ ฦั น๘ศฃ.

			arg = AnyOneArg( arg, g_Buf );
			int plat =0;
			plat = atoi( g_Buf ); // ต๎ทฯ ผ๖ทฎ

			arg = AnyOneArg( arg, g_Buf );
			g_ii[7] = atoi( g_Buf ); // ฑฑซ

			arg = AnyOneArg( arg, g_Buf );
			g_ii[8] = atoi( g_Buf ); // ภบฑซ

			arg = AnyOneArg( arg, g_Buf );
			g_ii[9] = atoi( g_Buf ); // ตฟฑซ

			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf ); // ต๎ทฯ ฐกฐ
						
			
			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf ); // ภฮบฅ ฦั น๘ศฃ.

			arg = AnyOneArg( arg, g_Buf );
			g_ii[4] = atoi( g_Buf ); // ต๎ทฯ ผ๖ทฎ

			BOOL bSlotInResult = TRUE; // ฝบท็ธ้ มคป๓

			for( i = 0 ; i < g_ii[4] ; ++i )
			{
				arg = AnyOneArg( arg, g_Buf );
				g_ii[5] = atoi( g_Buf ); // 인벤 x

				arg = AnyOneArg( arg, g_Buf );
				g_ii[6] = atoi( g_Buf ); // 인벤 y

				bSlotInResult = g_pNk2DFrame->GetControlUserShop()->SlotIn( g_ii[1],red,plat, g_ii[7], g_ii[8], g_ii[9], g_ii[2], g_ii[3], g_ii[5], g_ii[6]);
			}

			if( !bSlotInResult ) // 만약 실패했다면...
			{
				// 서버로 패킷을 보내서 최신 정보로 갱신해줄것을 요구한다.
				if( g_pTcpIp )
				{
					sprintf(strTemp, "u_shop refresh req 1 %d %d %d %d %d %d %d %d \n",  g_ii[1], g_ii[7], g_ii[8], g_ii[9], g_ii[2], g_ii[3], g_ii[5], g_ii[6]);
					g_pTcpIp->SendNetMessage( strTemp );
				}
			}


			if( pCMyApp && g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
				g_pNk2DFrame->GetControlInven()->ResetItemInfo(); // DrawItemInfo()를 갱신.
		}
		else // 실패. 뒤에 에러 인덱스가 온다.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 에러 코드

			switch( g_ii[1] )
			{
			case -4: // 슬롯에 넣을 수 없는 요청인경우.(갯수제한이나 다른 아이템.)

				//-- IDS_CHECK_ITEM : 아이템을 확인해 주세요!
				pMgr->CmdErrorMsg( (char*)G_STRING(IDS_CHECK_ITEM) );

				break;
			}
		}

		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"unreg") ) // 아이템 빼기. 클라이언트에서 요청한것에대한 성공실패 답변,이걸 받은후에 실제로 옮겨준다.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부

		if( g_ii[0] ) // 성공
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 등록 번호

//			arg = AnyOneArg( arg, g_Buf );
//			g_ii[2] = atoi( g_Buf ); // 인벤 팩 번호.

			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf ); // 등록 수량

			if( !g_pNk2DFrame->GetControlUserShop()->SlotOut( g_ii[1], g_ii[3] ) ) // 카운트만 줄여주는 함수.
			{
				// 서버로 패킷을 보내서 최신 정보로 갱신해줄것을 요구한다.
				if( g_pTcpIp )
				{
					sprintf(strTemp, "u_shop refresh req 2 %d %d\n",  g_ii[1], g_ii[3] );
					g_pTcpIp->SendNetMessage( strTemp );
				}
			}

			// 클라이언트에서 직접 넣지 않고 아이템 하나하나는 서버에서 알아서 보내주고 클라이언트는 상점 슬롯의 갯수만 갱신하는 방식으로 바꿈.
			/*
						for( i = 0 ; i < g_ii[3] ; ++i )
						{
							arg = AnyOneArg( arg, g_Buf );
							g_ii[4] = atoi( g_Buf ); // 인벤 x

							arg = AnyOneArg( arg, g_Buf );
							g_ii[5] = atoi( g_Buf ); // 인벤 y

							if( g_pNk2DFrame->GetControlUserShop()->SlotOut( g_ii[1], g_ii[2], g_ii[4], g_ii[5] ) )
							{
								// 서버로 패킷을 보내서 최신 정보로 갱신해줄것을 요구한다.
								if( g_pTcpIp )
								{
									sprintf(strTemp, "u_shop refresh req 3\n");
									g_pTcpIp->SendNetMessage( strTemp );
								}
							}
						}
			*/
		}
		else // 실패. 뒤에 에러 인덱스가 온다.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 성공 여부

			switch( g_ii[1] )
			{
			case -3: // 엑스트라에 들어있는데 언레지를 요청한 경우.
				// 서버로 패킷을 보내서 최신 정보로 갱신해줄것을 요구한다.
				if( g_pTcpIp )
				{
					sprintf(strTemp, "u_shop refresh req 4 %d\n", g_ii[1]);
					g_pTcpIp->SendNetMessage( strTemp );
				}
				break;
			}

		}

		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"close") ) // 상점 닫기. 상점 닫기 패킷을 받은 다음에 닫자.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부
		if( g_ii[0] ) // 성공
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 클로즈 타입.( 0 : 정상, -1 : 구매자일때 판매자가 닫음(또는 수정), 1 : 아이템 회수 완료 못했음. )

			if( g_ii[1] == -1 ) // 보는중에 판매자가 닫아버렸을땐..
			{
				//-- IDS_USHOP_MASTERCLOSE : 상점주인이 상점을 닫았습니다.
				pMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_MASTERCLOSE) );
			}
			if( g_ii[1] == 1 )
			{
				//-- IDS_USHOP_CANT_RECEIVE_ALL : 상점의 아이템을 모두 회수하지 못하였습니다. NPC에게 찾아가세요.
				pMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_CANT_RECEIVE_ALL) );
			}

			g_pRoh->m_UserShopLevel = 0; // 구경하다가는 별 의미가 없지만 그냥 무조건 돗자리 끈다.

			g_pNk2DFrame->ShowUserShopWindow( FALSE ); // 상점을 닫아준다. // 이 안에서 m_bOpened를 FALSE시켜주는게 중요하다.
		}
		else // 실패시.. 성공을 받고도 인터페이스가 안꺼지는 경우가 있다. 이경우 5초후 클라이언트에서 다시 클로즈 요청을 하는데 클로즈가 실패로 올것이다. 이때에도 인터페이스는 닫아버린다.(왜냐면 안그러면 클라이언트가 다른 명령을 안받아들인다.)
		{
			// 결국 이 경우는 버그다. 버그를 원인 못찾아서 땜빵한 부분.
			//-- IDS_USHOP_CLOSE_ERROR : 개인상점이 비정상 종료되었습니다.
			pMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_CLOSE_ERROR) );

			g_pRoh->m_UserShopLevel = 0; // 구경하다가는 별 의미가 없지만 그냥 무조건 돗자리 끈다.

			g_pNk2DFrame->ShowUserShopWindow( FALSE ); // 상점을 닫아준다. // 이 안에서 m_bOpened를 FALSE시켜주는게 중요하다.
		}

		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"start") ) // 상점 열기 신청의 결과.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부
		if( g_ii[0] ) // 성공
		{
			g_pNk2DFrame->GetControlUserShop()->m_SellStartBtn.SetDisable( TRUE );
			g_pNk2DFrame->GetControlUserShop()->m_ModifyBtn.SetDisable( FALSE );

			g_pNk2DFrame->GetControlUserShop()->m_bSlotLock = TRUE; // 팔기 시작했으므로 함부로 슬롯을 못건드리게 해주는 락을 건다.

			if( g_pNk2DFrame->GetControlUserShop()->m_iType == USHOP_TYPE_SELLER ) // 일단 판매자의 경우 ///점포상점
			{
				g_pRoh->m_UserShopLevel = g_pNk2DFrame->GetControlUserShop()->m_iPackNum; // 자기자신은 스타트를 누른 시점에서 팩갯수에따라 가판대를 설치해준다

				// 멘트 출력등을 위한 팝업 객체를 셋팅한다.
				if( g_pRoh->m_pUserShopPopup )
				{
					CMultiText multiText;
					char strTemp[100];

					//-- IDS_WHOS_USERSHOP : %s의 개인상점
					sprintf( strTemp, (char*)G_STRING(IDS_WHOS_USERSHOP), g_pRoh->m_pstrName );
					multiText.AddString( strTemp, _FCOLOR_YELLOW );
					// 문자열이 길엉痺툈E라인을 내리면서 출력
					// 문자열 길이를 15에서 35으로 늘려 출력해보자. [2/8/2007 Theodoric]
					multiText.AddStringMulti( g_pNk2DFrame->GetControlUserShop()->m_ShopMentText.GetStrBuf(), 15, TRUE );
					g_pRoh->m_pUserShopPopup->PutString(&multiText, TRUE);
				}
			}
			else if( g_pNk2DFrame->GetControlUserShop()->m_iType == USHOP_TYPE_ADMIN
					 || g_pNk2DFrame->GetControlUserShop()->m_iType == USHOP_TYPE_STORE ) // 점포 관리자인 경우.
			{
				g_pNk2DFrame->ShowUserShopWindow( FALSE ); // 인터페이스를 닫아준다. 돌아다닐수있도록.

				if( g_pTcpIp )
				{
					sprintf( strTemp, "u_shop m_close\n" ); // 인터페이스를 닫았다고 서버에 알려주기 위한 패킷.
					g_pTcpIp->SendNetMessage( strTemp );
				}
			}
		}
		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"modify") ) // 수정 신청의 결과.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부
		if( g_ii[0] ) // 성공
		{
			g_pNk2DFrame->GetControlUserShop()->m_SellStartBtn.SetDisable( FALSE );
			g_pNk2DFrame->GetControlUserShop()->m_ModifyBtn.SetDisable( TRUE );

			g_pRoh->m_UserShopLevel = 0; // 구경하다가는 별 의미가 없지만 그냥 무조건 돗자리 끈다.
			g_pNk2DFrame->GetControlUserShop()->m_bSlotLock = FALSE; // 팔기 시작했으므로 함부로 슬롯을 못건드리게 해주는 락을 푼다.
		}

		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"list_start") ) // 상점 물품 품목 받기 시작
	{
		g_pNk2DFrame->GetControlUserShop()->ClearSlot(); // 우선 기존에 있던 슬롯들을 몽땅 지워버린다.
		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = TRUE; // 이 녀석은 예외적으로 락을 건다.
		g_pNk2DFrame->GetControlUserShop()->m_SrvMsgWaitTick = timeGetTime(); // 서버 메세지를 기다리고 있기 시작한 시점을 기억해두는 틱. 일정시간 서버메세지가 없으면 플래그를 풀어주기위해 둔다.(이렇게 풀릴경우 상점을 닫아버려야 할듯.)
	}
	else if( !strcmp(g_Buf,"list") ) // 상점 물품 품목 받음.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 슬롯 위치.

		arg = AnyOneArg( arg, g_Buf );
		g_ii[1] = atoi( g_Buf ); // vnum

		arg = AnyOneArg( arg, g_Buf );
		g_ii[2] = atoi( g_Buf ); // plus

		arg = AnyOneArg( arg, g_Buf );
		g_ii[3] = atoi( g_Buf ); // special1(옵션1)

		arg = AnyOneArg( arg, g_Buf );
		g_ii[4] = atoi( g_Buf ); // special2(옵션2)

		arg = AnyOneArg( arg, g_Buf );
		g_ii[5] = atoi( g_Buf ); // 현재 내구도

		arg = AnyOneArg( arg, g_Buf );
		g_ii[6] = atoi( g_Buf ); // 최대 내구도

		if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[12] = atoi( g_Buf );		// 일반 내구도
			arg = AnyOneArg( arg, g_Buf );
			g_ii[13] = atoi( g_Buf );		// 일반 내구도 Max
		}
		arg = AnyOneArg( arg, g_Buf );
		g_ii[14] = atoi( g_Buf ); // 금괴
		arg = AnyOneArg( arg, g_Buf );
		g_ii[15] = atoi( g_Buf ); // 은괴

		arg = AnyOneArg( arg, g_Buf );
		g_ii[9] = atoi( g_Buf ); // 금괴
		arg = AnyOneArg( arg, g_Buf );
		g_ii[10] = atoi( g_Buf ); // 은괴
		arg = AnyOneArg( arg, g_Buf );
		g_ii[11] = atoi( g_Buf ); // 동괴

		arg = AnyOneArg( arg, g_Buf );
		g_ii[7] = atoi( g_Buf ); // 가격

		arg = AnyOneArg( arg, g_Buf );
		g_ii[8] = atoi( g_Buf ); // 수량

		if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			g_pNk2DFrame->GetControlUserShop()->SetSlotItem( g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6]
					, g_ii[12], g_ii[13], g_ii[14], g_ii[15], g_ii[9], g_ii[10], g_ii[11], g_ii[7], g_ii[8] ); // 아이템 정보를 셋팅.
		}
		else
		{
			g_pNk2DFrame->GetControlUserShop()->SetSlotItem( g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6]
					, 0, 0 ,g_ii[14], g_ii[15], g_ii[9], g_ii[10], g_ii[11], g_ii[7], g_ii[8]); // 아이템 정보를 셋팅.
		}
	}
	else if( !strcmp(g_Buf,"list_end") ) // 상점 물품 품목 받기 끝.
	{
		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"buy") ) // 상품구매 요청 결과.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부
		if( g_ii[0] ) // 성공
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 등록 번호

			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf ); // 등록 수량

			if( !g_pNk2DFrame->GetControlUserShop()->SlotOut( g_ii[1], g_ii[2] ) ) // 카운트만 줄여주는 함수.
			{
				// 서버로 패킷을 보내서 최신 정보로 갱신해줄것을 요구한다.
				if( g_pTcpIp )
				{
					sprintf(strTemp, "u_shop refresh req 5 %d %d\n", g_ii[1], g_ii[2] );
					g_pTcpIp->SendNetMessage( strTemp );
				}
			}

			if( g_ii[0] == 1 ) // 자기 자신이 산 경우. 에만 메세지를 출력해준다.
			{
				//-- IDS_ITEM_BUY_SUCC : 아이템이 성공적으로 구매되었습니다.
				pMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_BUY_SUCC) );
			}
		}
		else
		{
			//-- IDS_ITEM_BUY_FAIL : 아이템 구매에 실패하였습니다.
			pMgr->CmdErrorMsg( (char*)G_STRING(IDS_ITEM_BUY_FAIL) );
		}

		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"sell") ) // 팔렸음.
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부
		if( g_ii[0] ) // 성공
		{
			// 돈과 슬롯을 갱신하자.
			arg = AnyOneArg( arg, g_Buf );
			g_ii[1] = atoi( g_Buf ); // 슬롯 번호.

			arg = AnyOneArg( arg, g_Buf );
			g_ii[7] = atoi( g_Buf ); // 총 입수 금괴.
			arg = AnyOneArg( arg, g_Buf );
			g_ii[8] = atoi( g_Buf ); // 총 입수 은괴.

			arg = AnyOneArg( arg, g_Buf );
			g_ii[4] = atoi( g_Buf ); // 총 입수 금괴.
			arg = AnyOneArg( arg, g_Buf );
			g_ii[5] = atoi( g_Buf ); // 총 입수 은괴.
			arg = AnyOneArg( arg, g_Buf );
			g_ii[6] = atoi( g_Buf ); // 총 입수 동괴.


			arg = AnyOneArg( arg, g_Buf );
			ii64 = _atoi64( g_Buf ); // 총 입수 라임.(아이템 정보 갱신이 아니라 판매금액란 갱신에 사용해야한다.) (지금까지 팔린 충 금액이 온다.)

			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf ); // 팔린 갯수.(남은 갯수가 아님에 유의.)(남은갯수-팔린갯수가 0이하면 아이템을 지워야한다.)

			g_pNk2DFrame->GetControlUserShop()->SelledItem( g_ii[1], g_ii[7], g_ii[8], g_ii[4], g_ii[5], g_ii[6], ii64, g_ii[3] );
		}

		g_pNk2DFrame->GetControlUserShop()->m_bSrvMsgWaiting = FALSE; // 기다리던 락 플래그를 해제.
	}
	else if( !strcmp(g_Buf,"req") ) // 회수요청 결과
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // 성공 여부
		switch( g_ii[0] ) // 성공
		{
		case 1: // 모두 회수
			//-- IDS_USHOP_RECEIVE_ALL : 모든 아이템을 돌려 받았습니다.
			pMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_RECEIVE_ALL) );
			break;
		case 0: // 회수받을거 없음.
			//-- IDS_USHOP_RECEIVE_NOT : 돌려 받을 아이템이 없습니다.
			pMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_RECEIVE_NOT) );
			break;
		case -1: // 모두 회수
			//-- IDS_USHOP_RECEIVE_MORE : 아직 받을 아이템이 남아있습니다. 인벤을 비우세요.
			pMgr->CmdErrorMsg( (char*)G_STRING(IDS_USHOP_RECEIVE_MORE) );
			break;
		}
	}
	else if( !strcmp(g_Buf,"slist_start") ) // 상점 검색 결과 받기 시작
	{
		g_pNk2DFrame->GetUserShopSearch()->ClearList(); // 우선 기존에 있던 리스트들을 몽땅 지워버린다.

		g_pNk2DFrame->GetUserShopSearch()->m_bNowSearching = TRUE; // 뭘 찾고있는지 표시용...
		g_pNk2DFrame->GetUserShopSearch()->m_NowSearchTick = timeGetTime(); // 스타트가 온 시간을 기억.
		g_pNk2DFrame->GetUserShopSearch()->m_pRefSelOnItem = NULL; // 이걸 초기화 안하면 튕기는 경우가 있다.
	}
	else if( !strcmp(g_Buf,"slist") ) // 상점 검색 결과 받기 받음.
	{
		if( !g_pNk2DFrame->GetUserShopSearch()->m_bNowSearching ) // 이게 FALSE면 리스트가 더 와도 무시한다.(시간 초과등의 경우.)
			return;

		sUShopSearchData *TempSearchData; // 임시로 넣어줄 항목.

		TempSearchData = new sUShopSearchData; // 메모리를 새로 잡아준다.
		CItem *TempItem = &(TempSearchData->Item);

		arg = AnyOneArg( arg, g_Buf ); // 상점 주인 명.(스트링 길이에 제한이 있음에 유의)
		int TempLen = strlen( g_Buf );
		if( TempLen < 29 ) // 버퍼 길이보다 짧을때만 넣는다.
		{
			strcpy( TempSearchData->CharName, g_Buf );
			TempSearchData->CharName[TempLen] = '\0';
		}
		else // 버퍼 길이보다 길었다면...
		{
			TempSearchData->CharName[0] = '\0';
		}

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->PosX = atoi( g_Buf ); // X 좌표

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->PosY = atoi( g_Buf ); // Y 좌표

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->ZoneNum = atoi( g_Buf ); // Zone

		// 이 이하는 아이템 정보.
		TempSearchData->Item.m_Index = 0;

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->Item.m_Vnum = atoi( g_Buf ); // vnum

		TempSearchData->Item.m_ProtoNum = g_ItemList.FindItem( TempSearchData->Item.m_Vnum );

		arg = AnyOneArg( arg, g_Buf );
		g_ii[0] = atoi( g_Buf ); // plus

		if( GET_TYPE(TempItem) == ITYPE_SUB )
			TempItem->m_PlusNum = g_ii[0];
		else if( g_ii[0] > 0 )
		{
			if( GET_ITEM_LEVEL(TempItem) >= 181 )
				TempItem->SetDiaGemNum( g_ii[0] );
			else
				TempItem->SetGemNum( g_ii[0] );
		}

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->Item.m_Special = atoi( g_Buf ); // special(옵션)

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->Item.m_Special2 = atoi( g_Buf ); // special2(옵션2)

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->Item.m_UpgradeEndurance = atoi( g_Buf ); // 현재 내구도

		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->Item.m_MaxUpgradeEndurance = atoi( g_Buf ); // 최대 내구도

		if( g_SvrType == ST_ADULT_ONLY) // 내구도 수리
		{
			arg = AnyOneArg( arg, g_Buf );
			TempSearchData->Item.m_Endurance = atoi( g_Buf);

			arg = AnyOneArg( arg, g_Buf );
			TempSearchData->Item.m_MaxEndurance = atoi( g_Buf);
		}

		// 아이템 값을 입력해보자!
		ResetItemValue( TempItem );
		SetSpecialValue( TempItem, GET_TYPE(TempItem), GET_SHAPE(TempItem), TempItem->m_Special, TempItem->m_Special2 );


		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->MinPrice = _atoi64( g_Buf ); // 최소 가격
		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->MaxPrice = _atoi64( g_Buf ); // 최대 가격

		/*      // 가격을 범위로 표시하기로 바꿨다.
				arg = AnyOneArg( arg, g_Buf );
				TempSearchData->GoldLime = atoi( g_Buf ); // 금괴
				arg = AnyOneArg( arg, g_Buf );
				TempSearchData->SilverLime = atoi( g_Buf ); // 은괴
				arg = AnyOneArg( arg, g_Buf );
				TempSearchData->BronzeLime = atoi( g_Buf ); // 동괴

				arg = AnyOneArg( arg, g_Buf );
				TempSearchData->Price = atoi( g_Buf ); // 가격
		*/
		arg = AnyOneArg( arg, g_Buf );
		TempSearchData->Count = atoi( g_Buf ); // 수량

		g_pNk2DFrame->GetUserShopSearch()->m_ResultList.push_back( TempSearchData ); // 하나씩 추가한다.

		if( g_pNk2DFrame->GetUserShopSearch()->m_ResultList.size() >= 99 ) // 99개까지만 받는다.
			g_pNk2DFrame->GetUserShopSearch()->m_bNowSearching = FALSE; // 이렇게 하면 그만 받게 됨.
	}
	else if( !strcmp(g_Buf,"slist_end") ) // 상점 검색 결과 받기 끝 (현재 서버에서 보내주기가 어렵다하여 실제 안쓰임. 하지만 나중을 위해 일단 놔둠.(서버에서 보내주면 좋다.)
	{
		if( g_pNk2DFrame->GetUserShopSearch()->m_ResultList.size() <= 0 ) // 들어온 리스트가 없다면...
		{
			// 에러메세지 출력
			char StrTemp[256];
			//-- IDS_USHOPSEARCH_NOTSELL : [%s] 아이템을 팔고있는 상점이 없습니다.
			sprintf( StrTemp, (char*)G_STRING(IDS_USHOPSEARCH_NOTSELL), g_pNk2DFrame->GetUserShopSearch()->m_strSearchName );

			pMgr->CmdErrorMsg( StrTemp );
		}

		g_pNk2DFrame->GetUserShopSearch()->m_bNowSearching = FALSE; // 뭘 찾고있는지 표시용...
	}
}

void CmdNpcWear( char* arg, CUIMgr* pMgr ) // Npc한테 장비를 입히기 위한 패킷. 현재는 점포상점 NPC만 지원한다. ///점포상점 시 추가
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf ); // Npc 인덱스. (점포상점 NPC도 일반 NPC랑 인덱스는 같이 쓴단다. 즉, 겹치지 않는다.

	arg = AnyOneArg( arg, g_Buf );
	g_ii[1] = atoi( g_Buf ); // 아이템 Vnum

	CUserShopNpc *TempUShopNpc = pMgr->FindUShopNpcById( g_ii[0] ); // 우선은 점포상점 NPC를 검사.

	if( TempUShopNpc ) // 있는 녀석이면...
	{
		// 아이템 플래그따라 렌더링 옵션을 셋팅.(얼굴없앰 같은것들..)
		int ItemProtoNum = g_ItemList.FindItem( g_ii[1] );

		if( ItemProtoNum == -1 ) // 없는 아이템임.
			return;

		int RenderFlag = g_ItemList.m_ItemArray[ItemProtoNum].m_RenderFlag;

		if( TempUShopNpc->m_bRemoveHair ) // 이미 1이면 갱신 안한다.(없앰 아이템과 안없앰 아이템은 없앰 아이템을 우선함.
			TempUShopNpc->m_bRemoveHair = ( RenderFlag & ARMOR_REMOVE_HAIR );
		if( TempUShopNpc->m_bRemoveFace ) // 이미 1이면 갱신 안한다.(없앰 아이템과 안없앰 아이템은 없앰 아이템을 우선함.
			TempUShopNpc->m_bRemoveFace = ( RenderFlag & ARMOR_REMOVE_FACE );
		if( TempUShopNpc->m_bRemoveBody ) // 이미 1이면 갱신 안한다.(없앰 아이템과 안없앰 아이템은 없앰 아이템을 우선함.
			TempUShopNpc->m_bRemoveBody = ( RenderFlag & ARMOR_REMOVE_BODY );
		if( TempUShopNpc->m_bRemoveArms ) // 이미 1이면 갱신 안한다.(없앰 아이템과 안없앰 아이템은 없앰 아이템을 우선함.
			TempUShopNpc->m_bRemoveArms = ( RenderFlag & ARMOR_REMOVE_ARMS );
		if( TempUShopNpc->m_bRemoveLegs ) // 이미 1이면 갱신 안한다.(없앰 아이템과 안없앰 아이템은 없앰 아이템을 우선함.
			TempUShopNpc->m_bRemoveLegs = ( RenderFlag & ARMOR_REMOVE_LEGS );
		if( TempUShopNpc->m_bRemoveFoot ) // 이미 1이면 갱신 안한다.(없앰 아이템과 안없앰 아이템은 없앰 아이템을 우선함.
			TempUShopNpc->m_bRemoveFoot = ( RenderFlag & ARMOR_REMOVE_FEET );

		TempUShopNpc->m_WearList.push_back( g_ItemList.FindItem(g_ii[1]) ); // 아이템의 ProtoNum을 추가한다.

		return; // 추후 일반 NPC에도 들어갈 경우를 위해 리턴.
	}

	// 현재는 점포상점 NPC만 지원. 추후 일반 NPC에도 필요할 경우 이 밑에서 인덱스로 검사해서 해주자.
}

void CmdProcess(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );

	if (!strcmp(g_Buf, "req"))
	{
		arg = AnyOneArg(arg, g_Buf);	// 요청자 이름
		if( pMgr )
			pMgr->CmdProcessRequest(g_Buf);
	}
	else if (!strcmp(g_Buf, "ans"))
	{
		arg = AnyOneArg(arg, g_Buf);
		if(!strcmp(g_Buf, "accept"))
		{
			arg = AnyOneArg(arg, g_Buf);	// 0: 요청한 사람, 1: 정제해줄 사람에게.
			g_ii[ 1 ] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// 상대방 이름
			if( pMgr )
				pMgr->CmdProcessAccept( g_ii[ 1 ], g_Buf);
		}
	}
	else if (!strcmp(g_Buf, "money"))
	{
		arg = AnyOneArg( arg, g_Buf );	// 금액
		if( pMgr )
			pMgr->ProcessMoney( g_Buf );
	}
	else if (!strcmp(g_Buf, "succ"))	// 성공
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);			// index
		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);			// vnum
		arg = AnyOneArg(arg, g_Buf);
		g_ii[3] = atoi(g_Buf);			// x
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// y

		if( pMgr )
			pMgr->ProcessSuccess( g_ii[1], g_ii[2], g_ii[3] ,g_ii[4]);
	}
	else if (!strcmp(g_Buf, "fail"))	// 실페
	{
		if( pMgr && g_pNk2DFrame )
		{
			pMgr->ProcessFail();
			g_pNk2DFrame->ShowProcessWindow( false );
		}
	}
}

// exch_inven_to index vnum x y
void CmdProcessInvenTo(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			//index
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// vnum
	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);			// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);			// y
	arg = AnyOneArg(arg, g_Buf);
	gem_num[0] = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	special[0] = atoi(g_Buf);

	if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[4] = atoi(g_Buf);			// 강화 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[5] = atoi(g_Buf);			// 강화 내구도 Max
		arg = AnyOneArg(arg, g_Buf);
		g_ii[6] = atoi(g_Buf);			// 일반 내구도
		arg = AnyOneArg(arg, g_Buf);
		g_ii[7] = atoi(g_Buf);			// 일반 내구도 Max
	}

	if( pMgr )
	{
		if(	g_SvrType == ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		{
			pMgr->ProcessInvenTo(g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0],
								 g_ii[4], g_ii[5], g_ii[6], g_ii[7]);
		}
		else
		{
			pMgr->ProcessInvenTo(g_ii[2], g_ii[3], g_ii[0], g_ii[1], gem_num[0], special[0], 0, 0, 0, 0);
		}
	}
}

void CmdProcessInvenFrom(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// x
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// y

	if( pMgr )
		pMgr->ProcessInvenFrom(g_ii[0], g_ii[1]);
}

void CmdBpShopList(char *arg, CUIMgr *pMgr) // BP상점의 리스트(NPC 인덱스)를 받아오는 패킷. 이 인덱스를 토대로 BP상점 메뉴의 버튼에 액션을 걸어둔다.
{
	// 이 녀석은 예외적으로 ShowControlMenuWindow 함수 호출 후 AddButton()과 LoadRes()를 해준다.
	CNkMob TempMobData;
	TempMobData.SetInfo( 380, 0 ); // 안나의 정보로 셋팅.

	g_pNk2DFrame->ShowControlMenuWindow( TRUE, MENU_TYPE_BPSHOP, TempMobData.GetMobName() );

	// 각 버튼의 등록(액션 인덱스를 서버에서 보내준 Npc 인덱스 + 10000 으로 셋팅한다.) (순서는 샵툴에 등록된 순서.)
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);	// 리스트 갯수

	for( int i = 0 ; i < g_ii[0] ; ++i )
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);	// 상점 타입(무슨 상점인지..)

		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);	// Npc 인덱스.
		CNkMob * npc = pMgr->FindMobById(g_ii[2]);
		locationX = npc->m_wx;
		locationZ = npc->m_wz;
		switch( g_ii[1] )
		{
		case 0: // 칼
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_sword", 50, 225, g_ii[2]+10000 ); // 칼 BP 상점.
			break;
		case 1: // 도끼
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_axe", 100, 225, g_ii[2]+10000 ); // 도끼 BP 상점.
			break;
		case 2: // 스태프
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_staff", 150, 225, g_ii[2]+10000 ); // 스태프 BP 상점.
			break;
		case 3: // 총
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_gun", 200, 225, g_ii[2]+10000 ); // 총 BP 상점.
			break;
		case 4: // 링
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_ring", 50, 250, g_ii[2]+10000 ); // 링 BP 상점.
			break;
		case 5: // 이도류
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_dsword", 100, 250, g_ii[2]+10000 ); // 이도류 BP 상점.
			break;
		case 6: // 해머
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_hammer", 150, 250, g_ii[2]+10000 ); // 해머 BP 상점.
			break;
		case 7: // 기타
			g_pNk2DFrame->GetControlMenu()->AddButton( "Menu/btn_etc", 200, 250, g_ii[2]+10000 ); // 기타 BP 상점.
			
			break;
		}
	}

	g_pNk2DFrame->GetControlMenu()->LoadRes(); // 필요한 리소스를 로드한다.
}




void CmdWorldCup(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	if (!strcmp(g_Buf, "time"))
	{
		arg = AnyOneArg( arg, g_Buf );	//시간
		g_ii[1] = atoi(g_Buf);
		if( pMgr )
			pMgr->WorldCupTime( g_ii[1] );
	}
	else if (!strcmp(g_Buf, "score"))
	{
		arg = AnyOneArg(arg, g_Buf);  //A팀 스코어
		g_ii[1] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);  //B팀 스코어
		g_ii[2] = atoi(g_Buf);

		if( pMgr )
			pMgr->WorldCupScore( g_ii[1], g_ii[2] );
	}
	else if (!strcmp(g_Buf, "name"))
	{
		arg = AnyOneArg(arg, g_Buf);	//A팀 길드 네임
		arg = AnyOneArg(arg, g_Buf2);	//B팀 길드 네임

		if( pMgr )
			pMgr->WorldCupGuildName( g_Buf, g_Buf2 );
	}
}

void CmdUseEffect(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );	//케릭 인덱스
	g_ii[0] = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	//이벡트 인덱스
	g_ii[1] = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );  //보이냐 마냐
	g_ii[2] = atoi( g_Buf );

	///하이콤보 시 수정
	pCha = pMgr->FindChaById(g_ii[0]);

	if( pCha )
	{
		switch( g_ii[1] )
		{
		case EFFECT_CHA_DONT_ATTACK_STATE:
			switch(g_ii[2])
			{
			case 1:
				pCha->ChaDontAttackEffectStart();
				return;
			case 0:
				pCha->ChaDontAttackEffectEnd();
				return;
			}
			break;

		default:
			break;
		}

		if( g_ii[1] == EFFECT_NOTSKILL_SPEEDUP ) // 월드컵 대쉬모드 이펙트.
		{
			if( pMgr )
				pMgr->SpeedUpEffect( pCha, g_ii[2] );
		}
		else if( g_ii[1] == EFFECT_NOTSKILL_SPIRITMODE )
		{
			if( pMgr )
				pMgr->SpiritModeEffect( pCha, g_ii[2] );
		}
		else if( g_ii[1] == EFFECT_SKILL_BASHREFLECTION ) // 하이브리더 해머 콤보 분신 이펙트의 타겟 리스트다.
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf ); // 타겟 갯수.(이 녀석은 On/Off 플래그를 안쓰므로 덮어쓴다.)

			pCha->m_MultiTargetNum = g_ii[2];

			arg = AnyOneArg( arg, g_Buf ); // 타겟 종류
			if( !strncmp(g_Buf, "c", 1) )
				pCha->m_MultiTargetKind = MULTITARGET_KIND_CHAR; // 타겟이 캐릭터임
			else
				pCha->m_MultiTargetKind = MULTITARGET_KIND_MOB; // 타겟이 몬스터임


			for( int i = 0 ; i < g_ii[2] ; ++i )
			{
				arg = AnyOneArg( arg, g_Buf );
				g_ii[3] = atoi( g_Buf ); // 타겟 인덱스.

				pCha->m_MultiTargetArray[i] = g_ii[3];

				if( pCha->m_MultiTargetKind == MULTITARGET_KIND_CHAR )
				{
					pCha2 = pMgr->FindChaById( g_ii[3], FALSE );

					if( pCha2 )
					{
						// 분신 이펙트를 삽입.
						pCMyApp->m_pUIMgr->InsertMagic( pCha, NULL, pCha2, NULL, EFFECT_SKILL_BASHREFLECTION );
					}
				}
				else if( pCha->m_MultiTargetKind == MULTITARGET_KIND_MOB )
				{
					pMob = pMgr->FindMobById( g_ii[3] );

					if( pMob )
					{
						// 분신 이펙트를 삽입.
						pCMyApp->m_pUIMgr->InsertMagic( pCha, NULL, NULL, pMob, EFFECT_SKILL_BASHREFLECTION );
					}
				}
			}
		}
		else if( g_ii[1] == EFFECT_POISON_FOG ) // 데카렌 보스방 독안개. 화면에 2D로 뿌려지는 독안개 느낌. ///인던1
		{
			EffectSort ef_sort;
			ZeroMemory(&ef_sort, sizeof(EffectSort));

			ef_sort.nType = FX_POISON_FOG;
			ef_sort.dwColor = timeGetTime();
			pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
		}
		else if( g_ii[1] >= SKILL_LUCKYPIG1 && g_ii[1] <= SKILL_LUCKYPIG4 ) // 설날 돼지 회복 스킬 사용.
		{
			EffectSort *TempEffect = NULL;
			TempEffect = pCha->FindMyFairy( g_ii[1] ); // 해당 캐릭터의 돼지를 찾아서...

			// 모션을 바꿔두고 이펙트를 추가해준다.
			if( TempEffect )
			{
				TempEffect->num = FX_LUCKYPIG_STATE_HEAL; // 회복 모션.
				TempEffect->vSortPos.y = 0; // 프레임 초기화.

				pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_PIG_HEALING );
			}
		}
		// [7/31/2007 Theodoric] 6주년 기념 이벤트 베이비 소울 :: 크리티컬 효과.(에니메이션)
//		else if( g_ii[1] >= SKILL_BABYSOUL_1 && g_ii[1] <= SKILL_BABYSOUL_3 )
//		{
//			EffectSort *TempEffect = NULL;
//			TempEffect = pCha->FindMyFairy( g_ii[1] ); // 해당 캐릭터의 돼지를 찾아서...
//
//			// 모션을 바꿔두고 이펙트를 추가해준다.
//			if( TempEffect )
//			{
//				TempEffect->num = FX_BABYSOUL_STATE_CRITICAL; // 회복 모션.
//				TempEffect->vSortPos.y = 0; // 프레임 초기화.
//
//				//pMgr->InsertMagic(NULL, NULL, pCha, NULL, EFFECT_PIG_HEALING );
//			}
//		}
	}
}

void CmdMultiShop( char *arg, CUIMgr *pMgr ) ///유료상점
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetControlCashShop() )
		return;


	arg = AnyOneArg( arg, g_Buf ); // 패킷 종류

	if( !strcmp(g_Buf,"buy") ) // 구입 성공 실패 여부
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);			// 탭 갯수.

		if( g_ii[0] == 0 ) // 구입 성공
		{
			if( !g_pNk2DFrame->GetControlCashShop()->m_pWantBuyCatalog ) // 혹시 없으면..
				return;

			// 구매내역 리스트에 추가.
			sSellCatalogInfo *TempCatalogInfo = NULL;
			TempCatalogInfo = new sSellCatalogInfo; // 새로 메모리 잡아서 해줘야한다.

			if( !TempCatalogInfo )
				return;

			memcpy( TempCatalogInfo, g_pNk2DFrame->GetControlCashShop()->m_pWantBuyCatalog, sizeof(*TempCatalogInfo) ); // 내용을 복사. 포인터 없으니 마음놓고...

			TempCatalogInfo->m_pTextSur = NULL; // 직접 초기화 한판. 복사 한후 해야함에 유의.

			GetSystemTime( &(TempCatalogInfo->m_BuyTime) ); // 구매시간을 받아놓는다.

			g_pNk2DFrame->GetControlCashShop()->MakeBuyListText( TempCatalogInfo ); // 해당 구매내역의 텍스트부를 미리 생성.

			g_pNk2DFrame->GetControlCashShop()->m_BuyCatalogList.push_back( TempCatalogInfo ); // 구매 내역에 추가.

			g_pNk2DFrame->GetControlCashShop()->m_TotalUseCash += TempCatalogInfo->m_Price; // 가격을 누적.
			g_pNk2DFrame->GetControlCashShop()->m_pWantBuyCatalog = NULL; // 구입대기는 풀어준다.

			if( g_pNk2DFrame->GetControlCashShop()->m_InfoTab.NowSelectedIndex == 1 ) // 구매 내역 보고있었다면 스크롤 조정.
			{
				if( g_pNk2DFrame->GetControlCashShop()->m_BuyCatalogList.size() >= VIEW_INFO2_LINE+1 ) // 스크롤 범위 갱
					g_pNk2DFrame->GetControlCashShop()->m_InfoScroll.SetRange( g_pNk2DFrame->GetControlCashShop()->m_BuyCatalogList.size() - VIEW_INFO2_LINE + 1 ); // 보여질 라인에 맡춰서 범위 지정.
				else
					g_pNk2DFrame->GetControlCashShop()->m_InfoScroll.SetRange( 1 );
			}
		}
		else // 구입 실패
		{
			char strTemp[200] = "";

			if( g_ii[0] == -1 ) // 잔액부족
			{
				//-- IDS_CASHSHOP_ERROR1 : 잔액이 부족합니다.
				strcpy( strTemp, (char*)G_STRING(IDS_CASHSHOP_ERROR1) );
			}
			else if( g_ii[0] == -2 ) // 에러 발생
			{
				// IDS_ERROR : 오류
				strcpy( strTemp, G_STRING(IDS_ERROR) );
			}
			else if( g_ii[0] == -3 ) // 결제 권한 없음
			{
				//-- IDS_CASHSHOP_ERROR2 : 결제 권한이 없습니다.
				strcpy( strTemp, (char*)G_STRING(IDS_CASHSHOP_ERROR2) );
			}
			else if( g_ii[0] == -4 ) // 인벤 부족
			{
				//-- IDS_CASHSHOP_ERROR3 : 인벤토리에 빈 공간이 부족합니다.
				strcpy( strTemp, (char*)G_STRING(IDS_CASHSHOP_ERROR3) );
			}

			if( g_pNk2DFrame )
				g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OK, 1 );

			g_pNk2DFrame->GetControlCashShop()->m_pWantBuyCatalog = NULL; // 기억 상품만 클리어.
		}
	}
}

void CmdPetEffect(char *arg, CUIMgr *pMgr) // 라비용 스킬등에 쓰이는 패킷
{
	arg = AnyOneArg(arg, g_Buf);	// skill index
	int skill_idx = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);	// source kind

	if( !strncmp(g_Buf, "p", 1) ) // 현재 사용 주체는 펫만 있다.
	{
		arg = AnyOneArg(arg, g_Buf);	// source index
		g_ii[0] = atoi(g_Buf);

		if( pPet = pMgr->FindPetById(g_ii[0]) )
		{
			arg = AnyOneArg( arg, g_Buf2 );	// target kind

			arg = AnyOneArg(arg, g_Buf);	// target index
			g_ii[0] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// target vital
			g_ii[1] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// special attack // 현재 라비용 스킬엔 안쓰임. 일반 effect 패킷과 구조를 맞추기위해 넣어둠.
			g_ii[2] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// Mob Class
			g_ii[3] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// Skill Step // 현재 라비용 스킬엔 안쓰임.
			g_ii[4] = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);	// Skill Level // 현재 라비용 스킬엔 안쓰임.
			g_ii[5] = atoi(g_Buf);

			if (!strncmp(g_Buf2, "n", 1)) // 몬스터에게 썼을 경우.
			{
				if( pMob = pMgr->FindMobById(g_ii[0]) )
				{
					pMob->SetAttackVital(g_ii[1]);
					if( g_ii[3]==0 )
						pMob->SetClassType(g_ii[3]);

					pPet->Attack( pMob->m_wx, pMob->m_wz );
					pPet->m_AttackMobId = pMob->m_nVirId;

//					if( pPet == pCMyApp->m_pMyPet ) // 자기 펫이면... // 이건 보류. 모드마다 달라야하기때문에...
//						pCMyApp->m_pMyPet->SetCombatMob( pMob->m_nVirId ); // 계속 때리게 셋팅.

					pPet->SetPetEffect( skill_idx, pMob, NULL ); // CNkCharater 의 SetMagicMob,SetMagicCha 기능의 함수. 스킬을 사용한 펫과 대상에대한 이펙트,모션등을 셋팅한다.

				}
			}
			else if (!strncmp(g_Buf2, "c", 1)) // 캐릭터에게 썼을 경우.
			{
				if( pCha = pMgr->FindChaById(g_ii[0], TRUE) )
				{
					if( pCha != g_pRoh )
						pCha->SetAttackVital( g_ii[1] );

					pPet->Attack( pCha->m_wx, pCha->m_wz );
					pPet->m_AttackChaId = pCha->m_nCharIndex;

//					if( pPet == pCMyApp->m_pMyPet ) // 자기 펫이면... // 이건 보류. 모드마다 달라야하기때문에...
//						pCMyApp->m_pMyPet->SetCombatMob( -1, pCha->m_nVirId ); // 공격중 캐릭터 계속 때리게 셋팅.

					pPet->SetPetEffect( skill_idx, NULL, pCha ); // CNkCharater 의 SetMagicMob,SetMagicCha 기능의 함수. 스킬을 사용한 펫과 대상에대한 이펙트,모션등을 셋팅한다.
				}
			}
		}
	}
	else // 사용한게 펫이아니면 뭔가 이상한거다.
	{

	}
}

void CmdPetResult(char *arg, CUIMgr *pMgr) // 라비용 진화,스킬등의 결과 패킷.
{
	arg = AnyOneArg(arg, g_Buf);	// 결과 종류
	g_ii[0] = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);	// 성공, 실패. 0보다 크면 성공, 0보다 작으면 실패.
	g_ii[1] = atoi(g_Buf);

	switch( g_ii[0] )
	{
	case 0: // 진화 결과.

		if( g_ii[1] > 0 ) // 성공.
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->SetShowPetLevelFlag(false); // 인터페이스 꺼준다.
		}
		else if( g_ii[1] < 0 ) // 실패.
		{
			if( g_ii[1] == -8 ) // 목걸이 차고있으면 진화 못하도록... ///070723 일본버그리포트 수정
			{
				// 메세지를 알려준다.
				// IDS_CANT_PETEVOL_NEC : 라비용의 목걸이를 제거한 후 다시 시도하세요.
				g_pNk2DFrame->InsertPopup( (char *)G_STRING(IDS_CANT_PETEVOL_NEC),TYPE_NOR_OK, 1 );
			}
		}

		break;

	case 1: // 스킬 배우기 결과

		if( g_ii[1] > 0 ) // 성공.
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->SetShowPetLevelFlag(false); // 인터페이스 꺼준다.
		}
		else if( g_ii[1] < 0 ) // 실패.
		{
		}

		break;

	}

}

void CmdPetChange(char *arg, CUIMgr *pMgr) // 라비용 파츠 구입 결과 패킷.
{
	// 현재는 경험치 부족 실패만 있다.
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetControlCashShop() )
		return;

	arg = AnyOneArg( arg, g_Buf ); // 패킷 종류

	if( !strcmp(g_Buf,"fail") ) // 실패 일 경우.
	{
		// IDS_MORE_NEED_EXP : 경험치가 부족합니다.
		g_pNk2DFrame->InsertPopup( (char *)G_STRING(IDS_MORE_NEED_EXP),TYPE_NOR_OK, 1 );
	}
}


void CmdSupport(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	if (!strcmp(g_Buf, "req"))
	{
		arg = AnyOneArg(arg, g_Buf1); // 견습생 캐릭명

		if( g_pNk2DFrame->GetControlHelper()->m_bHelperAccept == 0)
			g_pTcpIp->SendNetMessage( "support ans -14\n" );
		else
			pMgr->CmdHelperRequest(g_Buf1);
	}
	else if (!strcmp(g_Buf, "ans"))
	{
		arg = AnyOneArg( arg, g_Buf );	//결과
		g_ii[0] = atoi( g_Buf );

		switch( g_ii[0] )
		{
		case -14:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_STATE_REJECT_HELPER), TYPE_NOR_OK, 1);
			break;
		case -13:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CAUSE_TROUBLE), TYPE_NOR_OK, 1);
			break;
		case -12:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NOT_CONNECT_HELPER), TYPE_NOR_OK, 1);
			break;
		case -11:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CANT_ADD_TRAINEE), TYPE_NOR_OK, 1);
			break;
		case -10:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NOTENOUGHT_HELPER_LEVEL), TYPE_NOR_OK, 1);
			break;
		case -4:
			{
				char str[128] = {0,};
				arg = AnyOneArg(arg, g_Buf1);
				int	LimitTime = DWORD(atoi(g_Buf1));
				int Limit_Day	= ((LimitTime/60)/60 ) / 24;	// 남은 일수.
				int Limit_Hour	= ((LimitTime/60)/60 ) % 24;	// 남은 시간.
				int Limit_Min	= ( LimitTime/60)%60;		// 남은 분
				int hours = Limit_Day * 24 + Limit_Hour+1;
				sprintf(str, (char*)G_STRING(IDS_SUPPORT_MSG_01), hours);  // "%d시간 후에 후견인 관계를 맺을 수 있습니다."
				g_pNk2DFrame->InsertPopup( str, TYPE_NOR_OK, 1);
			}
			break;
		case -3:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ALREADY_CONNECTION_ANOTHER), TYPE_NOR_OK, 1);
			break;
		case -2:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NEGOTIATION_ANOTHER), TYPE_NOR_OK, 1);
			break;
		case -1:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_NOTENOUGH_TRAINEE_LEVEL), TYPE_NOR_OK, 1);
			break;
		case 0:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_REJECT_HELPER), TYPE_NOR_OK, 1);
			break;
		case 1:
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_CONNECT_HELPER_COMPLET), TYPE_NOR_OK, 1);
			break;
		}
		return;
	}
	else if ( !strcmp(g_Buf, "info") )
	{
		arg = AnyOneArg( arg, g_Buf );	//후견인 0, 견습생 1
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//개수만큼 이후 정보 반복
		g_ii[1] = atoi( g_Buf );
		if( g_pNk2DFrame->GetControlHelper() )
			g_pNk2DFrame->GetControlHelper()->SetMember( g_ii[0], g_ii[1] );

		for( int count = 0; count < g_ii[1]; count++ )
		{
			arg = AnyOneArg( arg, g_Buf );	//케릭 인덱스
			g_ii[2] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf1 );	//케릭터명
			arg = AnyOneArg( arg, g_Buf );	//케릭터레벨
			g_ii[3] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );	//케릭터종족
			g_ii[4] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );	//케릭터성별
			g_ii[5] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );	//접속 상태( -1 미접속 , 0~9 존번호 )
			g_ii[6] = atoi( g_Buf );
			arg = AnyOneArg( arg, g_Buf );	//관계 시작일.(초단위time_t) ///07-05-03 남은 날짜를 표기하기 위한 처리.
			g_ii[7] = (int)atol( g_Buf ); // unsigned 형임에 유의.
			if( g_pNk2DFrame->GetControlHelper() )
				g_pNk2DFrame->GetControlHelper()->Information(g_ii[2], count, g_Buf1, g_ii[3], g_ii[5], g_ii[4], g_ii[6], (DWORD)g_ii[7] );
		}
	}
	else if ( !strcmp(g_Buf, "disband") )
	{
		arg = AnyOneArg( arg, g_Buf );	//케릭 인덱스
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf1 ); //견습생 이름
		char strTemp[128] = "";
		sprintf(strTemp, (char*)G_STRING(IDS_GIVEUP_CONNET_HELPER), g_Buf1);
		g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_OK, 1);
	}
	else if (!strcmp(g_Buf, "succ"))	// 성공
	{
		arg = AnyOneArg( arg, g_Buf );	//후견인/견습생
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//케릭 인덱스
		g_ii[1] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf1 ); //후견인/견습생 이름

		char strTemp[128] = "";
		if( g_ii[0] == 0 )
		{
			sprintf(strTemp, (char*)G_STRING(IDS_CONNECT_HELPER_END_FAMEPLUS), g_Buf1); ///07-04-23 포기쪽 메세지가 출력되고 있었다.
			g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_OK_LARGE, 1);
		}
		else if( g_ii[0] == 1 )
		{
			sprintf(strTemp, (char*)G_STRING(IDS_CONNECT_END_HELPER), g_Buf1);
			g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_OK_LARGE, 1);
		}
	}
	else if (!strcmp(g_Buf, "fail"))	// 실패
	{
		arg = AnyOneArg( arg, g_Buf );	//후견인/견습생
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//케릭 인덱스
		g_ii[1] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf1 ); //견습생 이름
		char strTemp[128] = "";
		if( g_ii[0] == 0 )
		{
			sprintf(strTemp, (char*)G_STRING(IDS_TIMEOVER_FAIL_HELPER), g_Buf1);
			g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_OK, 1);
		}
		else if( g_ii[0] == 1 )
		{
			sprintf(strTemp, (char*)G_STRING(IDS_TIMEOVER_FAIL_HELPER), g_Buf1);
			g_pNk2DFrame->InsertPopup(strTemp, TYPE_NOR_OK, 1);
		}
	}
	else if(!strcmp(g_Buf, "conn"))	// 접속 상태 갱신
	{
		arg = AnyOneArg( arg, g_Buf );	//케릭 인덱스
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//접속 상태
		g_ii[1] = atoi( g_Buf );
		if( g_pNk2DFrame->GetControlHelper() )
			g_pNk2DFrame->GetControlHelper()->ConnectState( g_ii[0], g_ii[1] );
	}
	else if( !strcmp(g_Buf, "enable") )
	{
		arg = AnyOneArg( arg, g_Buf );	//1: 포기 2: 기간 만료 3: 가용 가능 레벨아님
		g_ii[0] = atoi( g_Buf );
		if( g_pNk2DFrame->GetControlHelper() )
			g_pNk2DFrame->GetControlHelper()->HelperExplan( g_ii[0] );
	}
}

void CmdSpirit(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	if (!strcmp(g_Buf, "start"))
	{
		arg = AnyOneArg( arg, g_Buf );	//케릭인덱스
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//목표 점수
		g_ii[1] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//남은 시간(분)
		g_ii[2] = atoi( g_Buf );

		int nTaget_Hundred = g_ii[1] / 100;
		int nTaget_Ten     = ( g_ii[1] - nTaget_Hundred * 100 ) / 10;
		int nTaget_One     = g_ii[1] - ( nTaget_Hundred * 100 ) - ( nTaget_Ten * 10 );

		if( g_pRoh->m_nCharIndex == g_ii[ 0 ] )
		{
			g_pNk2DFrame->m_bSpiritStart = TRUE;
			pCMyApp->m_SpiritRotateCamera = TRUE;
			pMgr->SpiritPoint( 0, 0, 0, nTaget_One, nTaget_Hundred, nTaget_Ten, nTaget_One);
			pMgr->SpiritTime(g_ii[2]);
		}

		pCha = pMgr->FindChaById(g_ii[0], TRUE);
		if( pCha )
		{
			pMgr->InsertMagic( pCha, NULL, NULL, NULL, EFFECT_SPIRITMODE );
			pCMyApp->StopHero();
			pCha->SetMotionState(CHA_SPIRITMODE);
		}
	}
	else if (!strcmp(g_Buf, "point"))
	{
		arg = AnyOneArg( arg, g_Buf );	//획득한 점수
		g_ii[0] = atoi( g_Buf );
		arg = AnyOneArg( arg, g_Buf );	//목표 점수
		g_ii[1] = atoi( g_Buf );

		int nGet_Thousand = g_ii[0] / 1000;
		int nGet_Hundred = ( g_ii[0] - nGet_Thousand * 1000 ) / 100;
		int nGet_Ten     = ( g_ii[0] - ( nGet_Thousand * 1000 ) - ( nGet_Hundred * 100 ) ) / 10;
		int nGet_One     = g_ii[0] - ( nGet_Thousand * 1000 ) - ( nGet_Hundred * 100 ) - ( nGet_Ten * 10 );

		int nTaget_Hundred = g_ii[1] / 100;
		int nTaget_Ten     = ( g_ii[1] - nTaget_Hundred * 100 ) / 10;
		int nTaget_One     = g_ii[1] - ( nTaget_Hundred * 100 ) - ( nTaget_Ten * 10 );

		pMgr->SpiritPoint( nGet_Thousand, nGet_Hundred, nGet_Ten, nGet_One, nTaget_Hundred, nTaget_Ten, nTaget_One);
	}
	else if (!strcmp(g_Buf, "time"))
	{
		arg = AnyOneArg( arg, g_Buf );	//남은 기간(분)
		g_ii[0] = atoi( g_Buf );
		pMgr->SpiritTime(g_ii[0]);
	}
	else if (!strcmp(g_Buf, "end"))
	{
		g_pNk2DFrame->m_bSpiritStart = FALSE;
		arg = AnyOneArg( arg, g_Buf );	//성공 여부(0:실패, 1:성공)
		g_ii[0] = atoi( g_Buf );
	}
}

void CmdMobSay(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);
	if (g_ii[0] >= 0)
	{
		if (pMob = pMgr->FindMobById(g_ii[0]))
		{
			SkipSpaces(&arg);
			char *arg2 = AnyOneArg(arg, g_Buf);
			char *arg3 = arg2;
			SkipSpaces(&arg3);

			arg2 = AnyOneArg(arg, g_Buf);
			pMob->SetMobChatMsg(arg3);
			*arg2 = ')';
			SkipSpaces(&arg);
			if( g_pNk2DFrame )
				g_pNk2DFrame->AddChatStr(arg, 1);
		}
	}
}

void CmdAlive(char *arg, CUIMgr *pMgr)
{
	// 서버에 끼인 상태를 풀어주기위해 유저가 재로그인을 시도하면 서버로 부터 alive가 날라온다. 이걸 받으면 서버로 alive를 날려서 진짜 접속중임을 표시해준다.(만약 끼인거라면 alive가 안날라 가고 이걸 기준으로 서버에서 끊어준다.)
	if( g_pTcpIp )
		g_pTcpIp->SendNetMessage( "alive\n" );
}

void CmdAttChange(char *arg, CUIMgr *pMgr) // 서버의 속성제어 명령. ///인던1
{
	arg = AnyOneArg( arg, g_Buf );	// 속성값.
	g_ii[0] = atoi( g_Buf );

	float x1,y1,x2,y2;

	arg = AnyOneArg( arg, g_Buf );	// 좌표
	x1 = atof( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// 좌표
	y1 = atof( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// 좌표
	x2 = atof( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// 좌표
	y2 = atof( g_Buf );

	pCMyApp->GetLandPtr()->SetFieldAttr( x1, y1, x2, y2, (BOOL)g_ii[0] ); // 속성 셋팅. // 1이면 이동가능, 0이면 이동 불능.
}

void CmdEventAction(char *arg, CUIMgr *pMgr) // 특수연출 이벤트 씬의 제어 패킷. ///인던1
{
	float x,z,dir;

	arg = AnyOneArg( arg, g_Buf );	// 연출번호
	g_ii[0] = atoi( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// x 좌표
	x = atof( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// z 좌표
	z = atof( g_Buf );
	arg = AnyOneArg( arg, g_Buf );	// 방향
	dir = atof( g_Buf );

	if( g_ii[0] == -1 ) // 연출 강제 캔슬 명령 ///튜토리얼 시 추가
	{
		pMgr->m_nNowPlayingEvent = 0;
	}
	else if( g_ii[0] == EVENTACTION_DEKAREN_BOSS_IN ) // 인던 데카렌 보스 등장 씬.
	{
		if( !pMgr->IsEventScenePlaying() ) // 이녀석은 카메라 좌표 기억때문에 중복해서 오면 곤란하다.(좌표가 엉망이 된다.)
			pMgr->StartEventScene( g_ii[0], x, z, dir );
	}
	else if( g_ii[0] >= EVENTACTION_DECKAREN_CUT_IN1 && g_ii[0] <= EVENTACTION_DECKAREN_CUT_IN3 ) // 데카렌 1층 컷인 이벤트이면...
	{
		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_TLVERTEX;
		ef_sort.Index = EFFECT_DECKAREN_CUT_IN;
		ef_sort.subNum = g_ii[0]-EVENTACTION_DECKAREN_CUT_IN1; // 이게 컷인 번호
		ef_sort.dwColor = timeGetTime(); // 시작 시간으로 사용.
		pCMyApp->m_pFxSet->InsertFxEffect( ef_sort );
	}
	else if( g_ii[0] == EVENTACTION_TUTORIAL ) ///튜토리얼
	{
		if( !pMgr->IsEventScenePlaying() ) // 이녀석은 카메라 좌표 기억때문에 중복해서 오면 곤란하다.(좌표가 엉망이 된다.)
			pMgr->StartEventScene( g_ii[0], x, z, dir );
	}
}

void CmdRevival(char *arg, CUIMgr *pMgr) // 부활 관련
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf );

	pCha = pMgr->FindChaById(g_ii[0]);

	if (pCha)
	{
		if (pCha == g_pRoh )
		{
			g_pNk2DFrame->DeletePopup();
		}

		EffectSort ef_sort;
		ZeroMemory(&ef_sort, sizeof(EffectSort));
		ef_sort.nType = FX_RESURRECTION;

		ef_sort.pNkChaTo = pCha;
		pCha->InsertEffectNum(pCMyApp->m_pFxSet->InsertFxEffect(ef_sort));

		pCha->m_DeadFrame = -1;
		pCha->m_Vital = 1;
		pCha->m_AttackVital = 1;

		pCha->m_dwReturnTime = 0;
		pCha->SetMotionState(CHA_STAND);

		if (g_pDSound)
			g_pDSound->Play(g_EffectSound[EFF_SND_REVIVAL]);
	}
}

void CmdSkillCoolTime(char *arg, CUIMgr *pMgr) // 서버의 쿨타임 제어 패킷 ///대지의축복쿨타임을 서버에서 제어하도록 바꿨다.
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// skill_idx
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// kind  // 0:비활성, 1:활성, 2:쿨타임 시작

	if( g_ii[1] == 0 ) // 비활성
	{
		// 이 기능은 일단 당장 쓰이지 않고, 구현이 복잡해서 보류. 나중에 필요하면 넣자.
	}
	else if( g_ii[1] == 1 ) // 활성
	{
		// 이 기능은 일단 당장 쓰이지 않고, 구현이 복잡해서 보류. 나중에 필요하면 넣자.
	}
	else if( g_ii[1] == 2 ) // 쿨타임 시작
	{
		G_SK_SetCooltime( g_ii[0] );
	}
}

void CmdCheckJoin(char *arg, CUIMgr *pMgr) ///대만킥기능 패킷. 다른 유저가 자신의 접속을 끊어버렸음을 알림.
{
	// 이 패킷을 받으면 메세지 박스 하나 띄워주고 끝.
	if( g_pNk2DFrame )
	{
		//-IDS_WARRING_LOGINKICK : 누군가 같은 계정으로 접속하였습니다. 접속을 해제합니다. 당신의 계정 암호가 노출되지 않았는지 주의하시기 바랍니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_WARRING_LOGINKICK), TYPE_NOR_OK, 4 ); // 4번 타입 메세지 박스로 해야함에 유의.(클라이언트 종료시 팝업이다. "서버연결이 끊어졌습니다."가 안뜨게 하기 위함.)
	}

	g_pTcpIp->SetTerminate();
}

void CmdNvNWinner(char *arg, CUIMgr *pMgr) ///국가전승리 마크 표시.
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// 캐릭터

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);			// 승리 횟수

	pCha = pMgr->FindChaById( g_ii[0] ); // 자기 포함해서 찾는다.
	if( pCha )
	{
		pCha->m_NvNWin = g_ii[1]; // 승리 횟수 셋팅.
	}
}


// [4/20/2007 Theodoric] 태국 유료 창고 정보 패킷
void CmdStashCInfo(char *arg, CUIMgr *pMgr )
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// skill_idx
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	if( g_pNk2DFrame )
		g_pNk2DFrame->GetControlWareHouse()->SetCashInfo( g_ii[0], g_ii[1]);
}


void CmdPingTest(char *arg, CUIMgr *pMgr )
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi( g_Buf);

	if( g_ii[0] == 1 )
		PingTest.start();
	else
		PingTest.check();

}



void CmdEnchant(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	switch( g_ii[0] )
	{
	case -4:
		break; // 잘못된 번호
	case -3:
		break; // 잘못된 아이템
	case -2:
		break; // 비용부족
	case -1:
		break; // 강화실패
	case  0:
		break; // 초기화 성공
	case  1:
		break; // 강화성공
	}

	if( pMgr )
	{
		pMgr->PlatimumExch(5,g_ii[0], 0, 0, 0);
	}
}

// [2/27/2007 Theodoric] 인첸트된 아이템 내구도 수리
void CmdRepair(char *arg, CUIMgr *pMgr)
{
	if(	g_SvrType != ST_ADULT_ONLY ) // [6/1/2007 Theodoric] 내구도 수리
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if( !pCMyApp->m_pUIMgr->m_bAutoHunt )
	{
		switch( g_ii[0] )
		{
		case -4: // IDS_REPAIR_FAIL_ITEM01 "수리할 수 없는 아이템입니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_REPAIR_FAIL_ITEM01),TYPE_NOR_OK, 1 );
			break;

		case -3: // IDS_REPAIR_FAIL_ITEM02 "방어구와 무기류만 수리할 수 있습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_REPAIR_FAIL_ITEM02),TYPE_NOR_OK, 1 );
			break;

		case -2: // IDS_REPAIR_FAIL_NOTHING "수리할 내구도가 없습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_REPAIR_FAIL_NOTHING),TYPE_NOR_OK, 1 );
			break;

		case -1: // IDS_WANTED_REG_FAIL_LAIM "라임이 부족합니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_WANTED_REG_FAIL_LAIM),TYPE_NOR_OK, 1 );
			break;

		case  1: // IDS_REPAIR_SUC "내구도 수리가 완료되었습니다."
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_REPAIR_SUC),TYPE_NOR_OK, 1 );
			break;
		}
	}
}

void CmdPVP(char *arg, CUIMgr *pMgr) ///자유대련
{
	char strTemp[256] = "";

	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf ); // 패킷 종류. 이 녀석은 단어가아닌 번호로 구분하자고 서버측에서 요청했음.

	if( g_ii[0] == 1 ) // req 요청 받음.
	{
		arg = AnyOneArg( arg, g_Buf );	//캐릭명

		// 단순히 물어보기만 하면됨.
		// IDS_F_PVP_REQ "%s 님이 겨루기를 요청 하였습니다. 수락하시겠습니까?"
		sprintf( strTemp, (char*)G_STRING(IDS_F_PVP_REQ), g_Buf );
		g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OKCANCLE, 39 );
	}
	else if( g_ii[0] == 2 ) // ans 요청에 대한 대답
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[1] = atoi( g_Buf );

		arg = AnyOneArg( arg, g_Buf ); // 상대 이름.

		if( g_ii[1] == 1 ) // 수락
		{
			// 현재로선 딱히 아무것도 안해도 될 듯.
		}
		else if( g_ii[1] == 0 ) // 거절
		{
			// IDS_F_PVP_REQ_FAIL "%s 님이 겨루기를 거절 하였습니다."
			sprintf( strTemp, (char*)G_STRING(IDS_F_PVP_REQ_FAIL), g_Buf );
			g_pNk2DFrame->InsertPopup( strTemp, TYPE_NOR_OK, 1 );
		}
	}
	else if( g_ii[0] == 3 ) // start 시작
	{
		arg = AnyOneArg( arg, g_Buf );	// 패킷 타입
		g_ii[1] = atoi( g_Buf );

		if( g_ii[1] == 0 ) // 링 생성 요청 ( 대결자 본인들에게만 보내짐 )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf );   // x좌표.
			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf );   // z좌표.

			g_pRoh->m_vPvpRingPos.x = (float)g_ii[2];
			if( pCMyApp->GetLandPtr() )
				g_pRoh->m_vPvpRingPos.y = pCMyApp->GetLandPtr()->GetHFHeight((float)g_ii[2], (float)g_ii[3]);
			else // 만약에라도 없다면.
				g_pRoh->m_vPvpRingPos.y = g_pRoh->m_wy; // 일단 자신 위치로..
			g_pRoh->m_vPvpRingPos.z = (float)g_ii[3];
			g_pRoh->m_dwRingCreateTick = timeGetTime(); // 시간도 기억.
			pCMyApp->CreateBattleStart(3); // 자유대련 시작 카운트.
		}
		else if( g_ii[1] == 1 ) // 대결자 인덱스 ( 룸 안에 있는 사람들에게 전부 보내짐 )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[2] = atoi( g_Buf );   // 인덱스1
			arg = AnyOneArg( arg, g_Buf );
			g_ii[3] = atoi( g_Buf );   // 인덱스2

			pCha = pMgr->FindChaById(g_ii[2], TRUE); // 자신도 포함해서 찾는다.
			pCha2 = pMgr->FindChaById(g_ii[3], TRUE); // 자신도 포함해서 찾는다.
			if( pCha )
				pCha->m_bNowFreePVP = TRUE;
			if( pCha2 )
				pCha2->m_bNowFreePVP = TRUE;

			if( pCha == g_pRoh ) // 자기 자신이면...
			{
				g_pRoh->m_dwPvpDestIndex = g_ii[3]; // 상대방이 있든 없든 인덱스를 기억해둔다.

				g_pNk2DFrame->CloseAllWindow();
				g_pNk2DFrame->GetPVPWidget()->SetPVPInfo(pCha, pCha2); // 에너지 바 UI를 그린다.
				g_pNk2DFrame->ShowPVPWidget(TRUE); // 에너지 바 UI를 그린다.
			}
			if( pCha2 == g_pRoh ) // 자기 자신이면...
			{
				g_pRoh->m_dwPvpDestIndex = g_ii[2]; // 상대방이 있든 없든 인덱스를 기억해둔다.

				g_pNk2DFrame->CloseAllWindow();
				g_pNk2DFrame->GetPVPWidget()->SetPVPInfo(pCha2, pCha); // 에너지 바 UI를 그린다.
				g_pNk2DFrame->ShowPVPWidget(TRUE); // 에너지 바 UI를 그린다.
			}
		}
	}
	else if( g_ii[0] == 5 ) // result 결과
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[1] = atoi( g_Buf );   // 패자 0 승자 1
		arg = AnyOneArg( arg, g_Buf );
		g_ii[2] = atoi( g_Buf );   // 인덱스

		// 승자 패자 이펙트만 보여주면 됨
		pCha = pMgr->FindChaById(g_ii[2], TRUE); // 자신도 포함해서 찾는다.

		if( pCha )
		{
			if( g_ii[1] == 0 ) // 패자일 경우
			{
				pCha->SetShowCrash_Miss( 22 );
			}
			else if( g_ii[1] == 1 ) // 승자일 경우
			{
				pCha->SetShowCrash_Miss( 21 );
				pCha->SetMotionState( CHA_STAND ); // 승자는 서있게. (공격을 끊기 위해 필요)
			}

			if( pCha == g_pRoh ) // 자기 자신이 포함되어있으면...
			{
				// 공격하던거 멈추도록.
				pMgr->m_b_MotionForAttack = false;
				pMgr->m_b_MovingForAttack= false;
				pMgr->m_b_Attack= false;
				pMgr->m_b_SecAttack = false;
				pMgr->m_bChakramAtt = false;
				pMgr->m_bChakram = false;
				pMgr->m_bBookSpellAtt = false;
				pMgr->m_bBookSpell = false;

				pCha->m_MagicChaId[0] = -1;
				pCha->m_MagicMobId[0] =  -1;
				pCha->m_MagicChaCount = 0;
				pCha->m_MagicMobCount = 0;
				pCha->m_AttackChaId = -1;
				pCha->m_AttackMobId = -1;

				pCMyApp->SetHeroAttGoMob( NULL ); // 공격 대상 포인터를 초기화(이 안에서 공격대상 몹 외에 캐릭터도 초기화 된다.)
				pCMyApp->SetHeroAttGoCha( NULL ); // 공격 대상 포인터를 초기화(이 안에서 공격대상 몹 외에 캐릭터도 초기화 된다.)

				if( pCMyApp->DrawBattleStart )
				{
					pCMyApp->DeleteBattleStart();
					pCMyApp->DrawBattleStart = FALSE;
				}
			}
		}
	}
	else if( g_ii[0] == 4 ) // end 대결 종료(각각 따로 온다. 즉, 2번 온다) (result 3초후에 옴)
	{
		arg = AnyOneArg( arg, g_Buf );
		g_ii[1] = atoi( g_Buf );   // 인덱스1

		// 링,반투명 없앰(플레그를 끄면 자동으로 안보임.따로 지울 필요는 없다.)
		pCha = pMgr->FindChaById(g_ii[1], TRUE); // 자신도 포함해서 찾는다.
		if( pCha )
		{
			pCha->m_bNowFreePVP = FALSE;
			pCha->m_dwPvpDestIndex = 0; // 얘도 무조건 초기화

			g_pNk2DFrame->ShowPVPWidget(FALSE); // 에너지 바 UI를 그린다.
		}
	}
	else if( g_ii[0] == 6 ) // In 패킷
	{
		arg = AnyOneArg( arg, g_Buf );	// 인덱스
		g_ii[1] = atoi( g_Buf );

		pCha = pMgr->FindChaById(g_ii[1], TRUE); // 자신도 포함해서 찾는다.
		if( pCha )
			pCha->m_bNowFreePVP = TRUE; // 이건 자기 자신에게 올일이 없으니 인덱스 셋팅은 필요없다.
	}
	else if( g_ii[0] == 7 ) // 에너지바 갱신
	{
		arg = AnyOneArg( arg, g_Buf );	// 플레이어 HP
		g_ii[1] = atoi( g_Buf );

		arg = AnyOneArg( arg, g_Buf );	// 적 HP
		g_ii[2] = atoi( g_Buf );

		g_pNk2DFrame->GetPVPWidget()->SetPVPStatus(g_ii[1], g_ii[2]);
	}
	else if( g_ii[0] == 8 ) // 승패 기록
	{
		int player_win = 0;
		int player_lose = 0;
		int enemy_win = 0;
		int enemy_lose = 0;

		arg = AnyOneArg( arg, g_Buf ); // 플레이어 승수
		player_win = atoi( g_Buf );

		arg = AnyOneArg( arg, g_Buf ); // 플레이어 패배수
		player_lose = atoi( g_Buf );

		arg = AnyOneArg( arg, g_Buf ); // 적 승수
		enemy_win = atoi( g_Buf );

		arg = AnyOneArg( arg, g_Buf ); // 적 패배수
		enemy_lose = atoi( g_Buf );

		g_pNk2DFrame->GetPVPWidget()->SetPVPRecord(player_win, player_lose, enemy_win, enemy_lose);
	}
	else if( g_ii[0] < 0 ) // fail 요청에 대한 오류
	{
		if( g_ii[0] == -1 ) // not response
		{
			// IDS_F_PVP_REQ_FAIL_DONT_MEG "상대방의 응답이 없어 자유 대련이 취소 되었습니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_REQ_FAIL_DONT_MEG), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -2 ) // not found target
		{
			// IDS_F_PVP_REQ_FAIL_MISS_CHA "상대방을 찾을 수 없습니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_REQ_FAIL_MISS_CHA), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -3 ) // not battle
		{
			// IDS_F_PVP_CANCEL_ATTACK "대련을 진행하지 않아 자유대련이 취소 되었습니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_CANCEL_ATTACK), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -4 ) // out area
		{
			// IDS_F_PVP_CANCEL_RANGE "자유대련 지역을 벗어나 자유대련이 취소 되었습니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_CANCEL_RANGE), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -5 ) // already doing
		{
			// IDS_F_PVP_REQ_FAIL_ING  "이미 다른 사람과 대련 중입니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_REQ_FAIL_ING), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -6 ) //
		{
			// IDS_F_PVP_FAIL_HEALTH "HP가 꽉 찾을때만 신청하실 수 있습니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_F_PVP_FAIL_HEALTH), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -7 ) //
		{
			// IDS_PVP_FAIL_LEVEL "레벨이 낮아 자유대련 신청에 실패했습니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PVP_FAIL_LEVEL), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -8 ) //
		{
			// IDS_PVP_FAIL_ZONE "자유대련 불가 지역입니다."
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_PVP_FAIL_ZONE), TYPE_NOR_OK, 1 );
		}
		if( g_ii[0] == -9 ) //
		{
			// IDS_PVP_FAIL_GUILD_INVASION "길드 침략전 중에는 자유대련 신청 불가입니다."
			g_pNk2DFrame->InsertPopup(  (char*)G_STRING(IDS_PVP_FAIL_GUILD_INVASION), TYPE_NOR_OK, 1 );
		}
	}
}

void CmdGuildSystem(char *arg, CUIMgr *pMgr)
{
	static char cmd[100];

	arg = AnyOneArg(arg, cmd);
	g_pGuildCmd->guild_run(cmd, arg);
}

void CmdSVN(char *arg, CUIMgr *pMgr)
{
	if( CommonConfig::Instance()->GetGuildMark() == false )
		return;

	char buffer[256];
	arg = AnyOneArg(arg, buffer);

	int serverIndex = atoi(buffer);

	if( pMgr->thread_create(serverIndex) )
		pMgr->m_phttpConnection = CHttpConnection::Create();	
}

void CmdWantedSystem(char *arg, CUIMgr *pMgr)
{
	if( g_SvrType != ST_ADULT_ONLY )
		return;

	static char cmd[100];

	arg = AnyOneArg(arg, cmd);
	g_pWantedCmd->wanted_run(cmd, arg);
}

void Cmdkillporc(char *arg, CUIMgr *pMgr)   // 9980765 filename
{
	CHackProcessMgr* HProcMgr = CHackProcessMgr::GetInstance();

	char EncString[512]	= {0,};
	char ProcName[64]	= {0,};


	{
		arg = AnyOneArg( arg, EncString );
		arg = AnyOneArg( arg, g_Buf );
		int quest = atoi( g_Buf );

		HProcMgr->DecodeAutoProcessName(ProcName, EncString );

		ZeroMemory( EncString, strlen(EncString));
		ZeroMemory( ProcName, strlen(ProcName));
	}
}


void CmdHelpPet(char *arg, CUIMgr *pMgr ) ///펫도움말
{
	arg = AnyOneArg(arg, g_Buf);

	if( !strcmp(g_Buf, "run") ) // 도움말 강제 발동 메세지인지 예외 체크
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);          // 발동 인자.

		if( g_pNk2DFrame && g_pNk2DFrame->IsHelpPetVisible() && g_pNk2DFrame->GetControlHelpPet() ) ///펫도움말 의 발동 조건을 체크한다.
			g_pNk2DFrame->GetControlHelpPet()->CheckHelpCondition( 3, g_ii[1], TRUE ); // 아이템 습득 조건의 헬프가 있는지 체크한다.


		return;
	}

	// 서버에서 받지 않고 클라이언트에서 직접 키는 방식으로 바꿨다.( 그래도 서버에서도 키고 끌수있도록 이쪽 소스도 남겨둔다. )
	g_ii[0] = atoi(g_Buf);			// 펫 타입.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);          // On/Off 플래그.

	if( g_pNk2DFrame )
		g_pNk2DFrame->ShowHelpPetWindow( g_ii[1], g_ii[0] ); // 인자 순서 반대임에 유의!
}

void CmdEventWearTime(char *arg, CUIMgr *pMgr ) ///이벤트복장착용
{
	arg = AnyOneArg(arg, g_Buf);

	g_ii[0] = atoi( g_Buf);

	if( g_pNk2DFrame && g_pNk2DFrame->GetControlInven() )
	{
		g_pNk2DFrame->GetControlInven()->m_EventWearTime = g_ii[0];
		g_pNk2DFrame->GetControlInven()->m_EventWearTimeUpdateTick = timeGetTime();	// 최종 갱신된 시간을 체크.
	}
}

void CmdEventWearing(char *arg, CUIMgr *pMgr) // 자기 자신 이벤트 복장 부분 착용 정보 ///이벤트복장착용
{
	int special2[WEARING_NUM], up_endur[WEARING_NUM], max_up_endur[WEARING_NUM], endur[WEARING_NUM], max_endur[WEARING_NUM];

	if( g_pRoh == NULL )
		return;

	for(int i = 0; i < WEARING_NUM; i++)
	{
		arg = AnyOneArg(arg, g_Buf);
		index[i] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		v_num[i] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		gem_num[i] = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		special[i] = atoi(g_Buf);
		///신방어구강화시스템
		arg = AnyOneArg(arg, g_Buf);
		special2[i] = atoi(g_Buf); // 추가 옵션 m_Special2.
		arg = AnyOneArg(arg, g_Buf);
		up_endur[i] = atoi(g_Buf); // 현재 강화 내구도 수치.
		arg = AnyOneArg(arg, g_Buf);
		max_up_endur[i] = atoi(g_Buf); // 맥스 강화 내구도.

		if( g_SvrType == ST_ADULT_ONLY )
		{
			arg = AnyOneArg(arg, g_Buf);
			endur[i] = atoi(g_Buf);		// 일반 내구도
			arg = AnyOneArg(arg, g_Buf);
			max_endur[i] = atoi(g_Buf); // 일반 내구도 max
		}
		else
		{
			endur[i] = 0;
			max_endur[i] = 0;
		}

		g_pRoh->EventWear(i, index[i], v_num[i], gem_num[i], special[i], special2[i], up_endur[i], max_up_endur[i], endur[i], max_endur[i] );
	}	
}


// [9/11/2007 Theodoric] Siren
void CmdSiren(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf );   // 캐릭터 인덱스

	arg = AnyOneArg( arg, g_Buf );
	g_ii[1] = atoi( g_Buf );   // 싸이렌 on/off

	arg = AnyOneArg( arg, g_Buf );
	g_ii[2] = atoi( g_Buf );   // 싸이렌 색상

	pMgr->Siren_flag(g_ii[1], g_ii[0], g_ii[2]);

}

void CmdOccp(char* arg, CUIMgr * pMgr) //드미트론 점령전
{
	arg = AnyOneArg(arg, g_Buf);

	//지역별 점령 정보
	if (strcmp(g_Buf , "info") == 0)
	{
		for(int i = 0; i < 5; ++i)
		{
			arg = AnyOneArg(arg, g_Buf);  // 지역 번호
			int nIndex = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf2); // 길드 이름
			arg = AnyOneArg(arg, g_Buf);
			int nStatus = atoi(g_Buf);	  // 현재 상태
			arg = AnyOneArg(arg, g_Buf);
			int nBuff1 = atoi(g_Buf); // 버프 상태1
			arg = AnyOneArg(arg, g_Buf);
			int nBuff2 = atoi(g_Buf); // 버프 상태2

			if (g_pNk2DFrame)
			{
				CDmitronInfo::SetZoneInfo1( nIndex, g_Buf2, nStatus, nBuff1, nBuff2 );//점령전 상태
			}
		}

		if (g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_DMITRON_BATTLE) == TRUE)
		{
			CDmitronBattle* pDmitronBattle = (CDmitronBattle*)
											 g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE);
			pDmitronBattle->UpdateState();
		}
	}
	//세율 조정 / 찾기
	else if (strcmp(g_Buf , "cash") == 0)
	{
		arg = AnyOneArg(arg, g_Buf);
		if (strcmp(g_Buf , "open") == 0)
		{
			arg = AnyOneArg(arg, g_Buf);
			g_ii[0] = atoi(g_Buf);			// 세금징수액
			arg = AnyOneArg(arg, g_Buf);
			g_ii[1] = atoi(g_Buf);			// 세금율

			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_DMITRON_BATTLE_TAX);
			CDmitronBattleTax* pDmitronBattleTax = (CDmitronBattleTax*)
												   g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE_TAX);
			pDmitronBattleTax->InitVariables(g_ii[0], g_ii[1]);
		}
	}
	// 버프
	else if (strcmp(g_Buf , "buff") == 0)
	{
		arg = AnyOneArg(arg, g_Buf); // 버프 종류
		g_ii[0] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf); // 버프 상태
		g_ii[1] = atoi(g_Buf);

		CDmitronInfo::SetBuff(g_ii[0], g_ii[1]);
	}
	//
	else if (strcmp(g_Buf , "bt") == 0)
	{
		arg = AnyOneArg(arg, g_Buf); // 서브 메시지

		// 포탈 이용
		if (strcmp(g_Buf , "trans") == 0)
		{
			char buff[256];

			arg = AnyOneArg(arg, g_Buf); // 카운트
			int nCount = atoi(g_Buf);

			if (nCount <= 0) return;

			CControlGate* pGate = (CControlGate*)g_pNk2DFrame->GetInterface(GATE);
			pGate->RemoveAll();

			g_pNk2DFrame->ShowInterfaceWindow(true, GATE, 200);

			for(int i = 0; i < nCount; ++i)
			{
				arg = AnyOneArg(arg, g_Buf); // 카운트
				int nZone = atoi(g_Buf);

				arg = AnyOneArg(arg, g_Buf); // 카운트
				int nToll = atoi(g_Buf);

				char format[256];
				// IDS_DMITRON_PORTAL_COST : %s(%d라임)
				strcpy(format, (char*)G_STRING(IDS_DMITRON_PORTAL_COST));
				sprintf(buff, format, CDmitronString::Portal(nZone), nToll);
				pGate->AddItem(buff, new DmitronGate(nZone, nToll));
			}

			if(pGate->GetSubType() == 5)
			{
				if (pCMyApp->m_nCurWorldIdx == WORLD_DMITRON)
				{
					pGate->AddItem((char*)G_STRING(IDS_GATE_LAGRAMIA), NULL); // 라그라미아 신전
					pGate->AddItem((char*)G_STRING(IDS_GATE_WHITEHORN_DUNGEON), NULL); // 제누스 레버너티
				}
				else if(pCMyApp->m_nCurWorldIdx == WORLD_DMITRON_BATTLE)
				{
					pGate->AddItem((char*)G_STRING(IDS_GATE_DMITRON), NULL); // 드미트론
				}
			}
			pGate->EndItem();
		}
		else if (!strcmp(g_Buf, "list"))
		{
			for(int i = 0; i < 5; ++i)
			{
				arg = AnyOneArg(arg, g_Buf);
				int nIndex = atoi(g_Buf);	  // 인덱스
				arg = AnyOneArg(arg, g_Buf2); // 길드 이름
				arg = AnyOneArg(arg, g_Buf);
				int nStatus = atoi(g_Buf);	  // 전투 현황
				arg = AnyOneArg(arg, g_Buf);
				int nHelper = atoi(g_Buf);
				CDmitronInfo::SetZoneInfo2( nIndex, g_Buf2, nStatus, nHelper );//점령전 상태
			}

			g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_DMITRON_BATTLE);
			CDmitronBattle* pWindow = (CDmitronBattle*)
									  g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE);
			pWindow->UpdateState();
			return;
		}
		else if(!strcmp(g_Buf, "info"))
		{
			if (g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_DMITRON_BATTLE) == TRUE)
			{
				CDmitronBattle* pDmitronBattle = (CDmitronBattle*)
												 g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE);

				arg = AnyOneArg(arg, g_Buf2);
				int nTime = atoi(g_Buf2);

				arg = AnyOneArg(arg, g_Buf2);
				g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_DMITRON_BATTLE_INFO);
				CDmitronBattleInfo* pWindow = (CDmitronBattleInfo*)
											  g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE_INFO);
				pWindow->ActivateInfoWindow(nTime, g_pRoh->m_pstrGuildName, g_Buf2);
			}
			return;
		}
		else if(!strcmp(g_Buf, "req"))
		{
			if (g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_DMITRON_BATTLE) == TRUE)
			{
				CDmitronBattle* pDmitronBattle = (CDmitronBattle*)
												 g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE);

				CDmitronInfo::UpdateZone(TRUE);
				pDmitronBattle->UpdateState();
			}
			return;
		}
		else if(!strcmp(g_Buf, "fail"))
		{
			arg = AnyOneArg(arg, g_Buf2); // 시간

			if (g_pNk2DFrame->GetUIWindow()->IsOpenWindow(WINDOW_DMITRON_BATTLE) == TRUE)
			{
				CDmitronBattle* pDmitronBattle = (CDmitronBattle*)
												 g_pNk2DFrame->GetUIWindow()->GetWindow(WINDOW_DMITRON_BATTLE);

				CDmitronInfo::UpdateZone(FALSE);
				pDmitronBattle->HandleError(atoi(g_Buf2));
			}
			return;
		}
		else if(!strcmp(g_Buf, "go"))
		{
			arg = AnyOneArg(arg, g_Buf);
			int nZone = atoi(g_Buf);

			arg = AnyOneArg(arg, g_Buf);
			int nSuccess = atoi(g_Buf);

			if (nSuccess)
			{
				pMgr->PrepareChangeWorld(TRUE, WORLD_DMITRON_BATTLE, nZone+1); //1을 더할 것
			}
		}
		else if(!strcmp(g_Buf, "end"))
		{
			arg = AnyOneArg(arg, g_Buf2); // 존번호

			if (pMgr)
				pMgr->PrepareChangeWorld(TRUE, WORLD_DMITRON, 0);
		}
		return;
	}
	else if(!strcmp(g_Buf, "count"))
	{
		char myGuild[256], topGuild[256];
		g_pNk2DFrame->InitDmitronInfo();

		for( int i = 0; i < DMITRON_BATTLE_ZONE; ++i )
		{
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 0 ] = atoi( g_Buf ); // 지역 번호
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 1 ] = atoi( g_Buf ); // 부가 정보
			arg = AnyOneArg( arg, g_Buf );
			g_ii[ 2 ] = atoi( g_Buf ); // 지역 상태
			if (g_ii[ 1 ] == FALSE)
			{
				g_pNk2DFrame->SetDmitronInfo1( g_ii[ 0 ], g_ii[ 2 ] );
			}
			else
			{
				arg = AnyOneArg( arg, myGuild ); // 소속 길드 이름
				arg = AnyOneArg( arg, g_Buf ); // 소속 길드 득점
				g_ii[ 3 ] = atoi( g_Buf );

				arg = AnyOneArg( arg, topGuild ); // 최고 길드 이름
				arg = AnyOneArg( arg, g_Buf ); // 최고 길드 득점
				g_ii[ 4 ] = atoi( g_Buf );

				arg = AnyOneArg( arg, g_Buf );
				g_ii[ 5 ] = atoi( g_Buf ); // 남은 시간

				arg = AnyOneArg( arg, g_Buf );
				g_ii[ 6 ] = atoi( g_Buf ); // 점령 시간

				g_pNk2DFrame->SetDmitronInfo2( g_ii[ 0 ], g_ii[ 2 ],
											   myGuild, g_ii[ 3 ], topGuild, g_ii[ 4 ], g_ii[5], g_ii[6] );
			}
		}
		g_pNk2DFrame->DisplayDmitronInfo();
		return;
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);  // 날자
		arg = AnyOneArg(arg, g_Buf2); // 시간
		arg = AnyOneArg(arg, g_Buf3); // 시간
		g_ii[0] = atoi(g_Buf3);

		arg = AnyOneArg(arg, g_Buf4); // 타입
		g_ii[1] = atoi(g_Buf4);

		if( pMgr )
			pMgr->GetServMsg_WarReq(g_Buf2, g_ii[0], g_ii[1]);
	}
}

void CmdBossRaidAttack(char* arg, CUIMgr* pMgr)
{
	// bossraid_at [boss Vnum] [skill index] [boss mob index] [character index]

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);	// skill index

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi(g_Buf);	// boss index

	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi(g_Buf);	// character index

	pMob = NULL;
	pCha = NULL;

	pMob = pMgr->FindMobById( g_ii[2] );
	pCha = pMgr->FindChaById( g_ii[3] );

	// 몬스터가 없으니 이펙트를 구현하면 안되다.
	if( pMob == NULL )
		return;

	switch( pMob->m_nMobNum ) // boss vnum
	{
	case 511:  // 불칸 보스 1
		// 몬스터가 없으니 이펙트를 구현하면 안되다.
		if( pCha == NULL )
			return;

		if( g_ii[1] == EFFECT_BOSSRAID_BULKAN1_02 || g_ii[1] == EFFECT_BOSSRAID_BULKAN1_03 )
		{
			pMob->SetMotionState( MOB_EMPTY1 + g_ii[1] - EFFECT_BOSSRAID_BULKAN1_02 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

	case 512: // 불칸 보스 2
		if( pCha == NULL )
			return;

		if( g_ii[1] == EFFECT_BOSSRAID_BULKAN2_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

	case 513: // 불칸 보스 3
		if( g_ii[1] == EFFECT_BOSSRAID_BULKAN3_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

//////////////////////////////////////////////////////////////////////////
	case 514: // 휴먼 보스 1
		if( pCha == NULL )
			return;

		if( g_ii[1] == EFFECT_BOSSRAID_HUMAN1_02 || g_ii[1] == EFFECT_BOSSRAID_HUMAN1_03 )
		{
			pMob->SetMotionState(MOB_EMPTY1 + g_ii[1] - EFFECT_BOSSRAID_HUMAN1_02 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;
	case 515: // 휴먼 보스 21
		if( pCha == NULL )
			return;

		if( g_ii[1] == EFFECT_BOSSRAID_HUMAN2_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		else if ( g_ii[1] == EFFECT_BOSSRAID_HUMAN2_03 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 6 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;
	case 516: // [12/13/2007 반재승] 보스레이드 휴먼보스3-02 스킬
		if( g_ii[1] == EFFECT_BOSSRAID_HUMAN3_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

//////////////////////////////////////////////////////////////////////////
	case 517: // 에이디아 보스 1
		if( g_ii[1] == EFFECT_BOSSRAID_AIDIA1_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

	case 518: // 에이디아 보스 2
		if( g_ii[1] == EFFECT_BOSSRAID_AIDIA2_02)
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		else if( g_ii[1] == EFFECT_BOSSRAID_AIDIA2_03)
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 6 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

	case 519: // 에이디아 보스 3
		if( g_ii[1] == EFFECT_BOSSRAID_AIDIA3_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;
//////////////////////////////////////////////////////////////////////////
	case 520: // 하이브리더 보스 1
	case 740: // 유피텔 보스 1
		if( g_ii[1] == EFFECT_BOSSRAID_HYBRI1_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;
	case 521: // 하이브리더 보스 2
		if( g_ii[1] == EFFECT_BOSSRAID_HYBRI2_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;
	case 522: // 하이브리더 보스 2

		if( g_ii[1] == EFFECT_BOSSRAID_HYBRI3_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}

		break;
//////////////////////////////////////////////////////////////////////////
	case 523: // 카이립톤 보스 1
		if( g_ii[1] == EFFECT_BOSSRAID_KAI1_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

	case 524: // 카이립톤 보스 2
		if( g_ii[1] == EFFECT_BOSSRAID_KAI2_02 || g_ii[1] == EFFECT_BOSSRAID_KAI2_03 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( MOB_EMPTY1 + g_ii[1] - EFFECT_BOSSRAID_KAI2_02 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}

		break;

	case 525: // 카이립톤 보스 3
		if( g_ii[1] == EFFECT_BOSSRAID_KAI3_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		else if( g_ii[1] == EFFECT_BOSSRAID_KAI3_03 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 6 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;

//////////////////////////////////////////////////////////////////////////
	case 526: // 최종 보스 1
	case 648: // 최종 보스 1
		if( g_ii[1] == EFFECT_BOSSRAID_LASTBOSS_02 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 5 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		else if( g_ii[1] == EFFECT_BOSSRAID_LASTBOSS_03 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 6 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		else if( g_ii[1] == EFFECT_BOSSRAID_LASTBOSS_04 )
		{
			pMob->SetAttackChaId( g_ii[3] );
			pMob->SetMotionState( 6 );
			pMgr->InsertMagic( NULL, pMob, pCha, NULL, g_ii[1] );
		}
		break;
	}
}


void CmdBossRaidKill(char* arg, CUIMgr* pMgr)
{
	BOOL KillFlag;
	g_pNk2DFrame->InitBossRaidKillCount();

	for(int i=0; i<5; i++)
		for(int j=0; j<3; j++)
		{
			arg = AnyOneArg(arg, g_Buf);
			KillFlag = ( atoi(g_Buf) == 0 )? FALSE : TRUE;
			g_pNk2DFrame->SetBossRaidInfo(i,j,KillFlag);
		}
}

void CmdBossRaidLicense(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	switch( g_ii[1] )
	{
	case 0:
		g_pNk2DFrame->InitBossRaidKillCount();
		break;
	}
}

// [2008/4/29 Theodoric] 마지막 보스 공헌도 순위 리스트
void CmdBossRaidFinalRank(char* arg, CUIMgr* pMgr)
{

// 최종 보스 공헌도 순위 전달 패킷 구조

// 구조
// br_rank list 종족별1,2,3위의 캐랙명 ... (불칸,카이립톤,에이디아,휴먼,하이브리더 순)

// ex) 불칸1위, 카이립톤1,2위, 하이브리더 1위가 존재할 경우
// br_rank list 불칸1위 -1 -1 카이립톤1위 카이립톤2위 -1 -1 -1 -1 -1 -1 -1 하이브리더 1위 -1 -1

	arg = AnyOneArg(arg, g_Buf);

	if( !strnicmp("list", g_Buf, 4) )
	{
		g_pNk2DFrame->SetHeroFinalRank( arg );
	}
}


void CmdSvrChannelNum(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf);

	g_pRoh->SetSvrChannelNum( g_ii[1]);
}

void CmdPremium(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	if(!strcmp(g_Buf, "chname_ack"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);

		switch( g_ii[1] )
		{
		case 0:	// 길드명 변경 관련 메시지
			{
				switch (g_ii[2])
				{
				case 1:		// IDS_PREMIUM_MSG_01 길드명이 변경 되었습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_01), TYPE_NOR_OK, 1);
					break;
				case 0:		// IDS_PREMIUM_MSG_02 길드명 변경권이 없습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_02), TYPE_NOR_OK, 1);
					break;
				case -1:	// IDS_CRECHA_ALLEADY_EXIST 그 이름은 이미 사용 중 입니다
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_CRECHA_ALLEADY_EXIST), TYPE_NOR_OK, 1);
					break;
				case -2:	// IDS_PREMIUM_MSG_03 길드명 변경 권한이 없습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_03), TYPE_NOR_OK, 1);
					break;
				case -3: // IDS_ENCHANT_FAIL_ITEM 잘못된 아이템입니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_ENCHANT_FAIL_ITEM), TYPE_NOR_OK, 1);
					break;
				case -4:	// IDS_PREMIUM_MSG_04 잘못된 이름입니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_04), TYPE_NOR_OK, 1);
					break;
				}
			}
			break;

		case 1: // 캐릭터명 과련 메시지
			{
				switch (g_ii[2])
				{
				case 1:		// IDS_PREMIUM_MSG_05 캐릭명이 변경 되었습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_05), TYPE_NOR_OK, 1);
					break;
				case 0:	// IDS_PREMIUM_MSG_06 캐릭명 변경권이 없습니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_06), TYPE_NOR_OK, 1);
					break;
				case -1:	// IDS_CRECHA_ALLEADY_EXIST 그 이름은 이미 사용 중 입니다
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_04), TYPE_NOR_OK, 1);
					break;
				case -3: // IDS_ENCHANT_FAIL_ITEM 잘못된 아이템입니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_ENCHANT_FAIL_ITEM), TYPE_NOR_OK, 1);
					break;
				case -4:	// IDS_PREMIUM_MSG_04 잘못된 이름입니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_04), TYPE_NOR_OK, 1);
					break;
				case -5:	// IDS_PREMIUM_MSG_07 사용 후 1개월 이후에 재사용이 가능합니다.
					g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_07), TYPE_NOR_OK, 1);
					break;
				}
			}
			break;
		}

		if( g_ii[2] == 1 )
		{
			arg = AnyOneArg(arg, g_Buf);
			int slot = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int index = atoi(g_Buf);
			g_pRoh->RemoveQuickItem(slot, index , false);
		}
	}
	else if(!strcmp(g_Buf, "chname_msg"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);


		pCha = NULL;

		switch( g_ii[1] )
		{
		case 0: // 길드명 변경
			{
				if( g_pRoh && g_pRoh->m_GuildIndex == g_ii[2] )
				{
					g_pRoh->SetGuildName(g_Buf);
					if( g_pNk2DFrame->GetControlGuild() )
						g_pNk2DFrame->GetControlGuild()->ChangeGuildInfo(g_Buf);
				}

				LHCharacterMap_it pr;
				for( pr = pMgr->m_character_map.begin(); pr != pMgr->m_character_map.end(); pr++ )
				{
					pCha = (*pr).second;
					if( pCha->m_GuildIndex == g_ii[2] )
						pCha->SetGuildName(g_Buf);
				}
			}
			break;

		case 1: // 캐릭터명 변경
			{
				pCha = pMgr->FindChaById(g_ii[2], TRUE);

				if (pCha)
					pCha->SetCharName(g_Buf);

				if( g_pNk2DFrame && g_pRoh->m_nCharIndex == g_ii[2])
				{
					((CControlScore*)g_pNk2DFrame->GetInterface(SCORE))->SetName();

					if( g_pNk2DFrame->GetControlStatus() )
						g_pNk2DFrame->GetControlStatus()->SetName();
				}
			}
			break;
		}
	}
	else if( !strcmp(g_Buf, "chmove_req_step") )
	{
		arg = AnyOneArg(arg, g_Buf);
		int step = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int flag = atoi(g_Buf);

		switch( step )
		{
		case 0:
			switch( flag )
			{
			case 0:	// IDS_PREMIUM_MSG_08 캐릭터 이동 마법서를 사용합니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_08), TYPE_NOR_OKCANCLE, POPUP_PREMIUM_CHA_MOVE_FROM);
				break;
			case 1:	// IDS_PREMIUM_MSG_09 캐릭터 이동 마법서를 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_09), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 2:		// IDS_PREMIUM_MSG_07 사용 후 1개월 이후에 재사용이 가능합니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_07), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 3:		// IDS_PREMIUM_MSG_10 개인 상점으로 아이템을 판매 중에는 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_10), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 4:	// IDS_PREMIUM_MSG_11 길드 가입되어 있어 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_11), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 5:	// IDS_PREMIUM_MSG_12 파티 중에는 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_12), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 6:	// IDS_PREMIUM_MSG_13 후견인 중에는 사용할 수 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_13), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			} // switch( flag )
			break;

		case 2:
			switch( flag )
			{
			case 0:	// IDS_PREMIUM_MSG_14 입력하신 캐릭터 정보를 찾을 수 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_14), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 1:	// IDS_PREMIUM_MSG_15 해당 캐릭터가 라그라미아 존에 없습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_15), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 2:	// IDS_PREMIUM_MSG_16 받으시는 분이 본인 인증이 되어있지 않습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_16), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 3:	// IDS_PREMIUM_MSG_17 받으시는 분의 슬롯이 비어있지 않습니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_17), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;
			case 4:		// IDS_PREMIUM_MSG_18 잠시만 기다려 주십시오. 응답을 기다리는 중 입니다.
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_18), TYPE_PREMIUM_CHA_MOVE_WAIT, POPUP_PREMIUM_CHA_MOVE_WAIT);
				break;
			case 5: // 받을 사람에게 응답 창을 띄운다.
				{
					// 캐릭터 정보를 미리 받아 셋팅하자.
					arg = AnyOneArg(arg, g_Buf);	// name from
					arg = AnyOneArg(arg, g_Buf1);	// name to
					arg = AnyOneArg(arg, g_Buf2);	// level
					int level = atoi(g_Buf2);
					arg = AnyOneArg(arg, g_Buf2);	// race
					int race = atoi(g_Buf2);

					// 기본 정보 셋팅
					g_pNk2DFrame->GetPremmiumChaMove()->ClearChaInfo();
					g_pNk2DFrame->GetPremmiumChaMove()->SetChaInfo( g_Buf, g_Buf1, level, race );

					// 입은 장비 셋팅
					//////////////////////////////////////////////////////////////////////////

					arg = AnyOneArg(arg, g_Buf2);	// wearing 을 보내준다. 그냥 여기서 걸러버리자.

					for(int i=0; i<WEARING_NUM; i++)
					{
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[0] = atoi(g_Buf2);
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[1] = atoi(g_Buf2);
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[2] = atoi(g_Buf2);
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[3] = atoi(g_Buf2);
						///신방어구강화시스템
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[4] = atoi(g_Buf2); // 추가 옵션 m_Special2.
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[5] = atoi(g_Buf2); // 현재 강화 내구도 수치.
						arg = AnyOneArg(arg, g_Buf2);
						g_ii[6] = atoi(g_Buf2); // 맥스 강화 내구도.

						if( g_SvrType == ST_ADULT_ONLY)// [6/1/2007 Theodoric] 내구도 수리
						{
							arg = AnyOneArg(arg, g_Buf2);
							g_ii[7] = atoi(g_Buf2);		// 일반 내구도
							arg = AnyOneArg(arg, g_Buf2);
							g_ii[8] = atoi(g_Buf2);		// 일반 내구도 max
						}

						if( g_pNk2DFrame->GetPremmiumChaMove() )
						{
							if( g_SvrType == ST_ADULT_ONLY)
								g_pNk2DFrame->PremiumChaMoveWear(i, g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], g_ii[7], g_ii[8] ); ///신방어구강화시스템
							else
								g_pNk2DFrame->PremiumChaMoveWear(i, g_ii[0], g_ii[1], g_ii[2], g_ii[3], g_ii[4], g_ii[5], g_ii[6], 0, 0 ); ///신방어구강화시스템
						}
					}
					//////////////////////////////////////////////////////////////////////////
					g_pNk2DFrame->SetProcLock(TRUE);
					// IDS_PREMIUM_MSG_19 %s님이 캐릭터 이동을 요청 하였습니다. 응하시겠습니까?
					sprintf( g_Buf2, (char *)G_STRING(IDS_PREMIUM_MSG_19), g_Buf );
					g_pNk2DFrame->InsertPopup(g_Buf2, TYPE_NOR_OKCANCLE, POPUP_PREMIUM_CHA_MOVE_TO );
				}
				break;
			case 7:
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_LHSTRING1945), TYPE_NOR_OK, POPUP_PREMIUM_CHA_MOVE_FAIL);
				break;

			} // switch( flag )
			break;

		case 4:
			switch( flag )
			{
			case 0: // 대기상태 해제
				g_pNk2DFrame->OnClosePremiumChaMove();
				break;
			case 1:
				g_pNk2DFrame->ClossePremiumMsgBox();
				// IDS_PREMIUM_MSG_20 캐릭터 이동이 완료 되었습니다. 잠시 후 게임이 자동 종료 됩니다.
				g_pNk2DFrame->SetForceExit();
				g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_20), TYPE_PREMIUM_CHA_MOVE_WAIT, POPUP_PREMIUM_CHA_MOVE_SUC_EXIT);

				break;
			case 2:
				{
					exit(1);
				}
				break;

			} // switch( flag )
			break;
		} // switch( step )

	}
	else if( !strcmp(g_Buf, "chmove_req_stop") ) // 캐릭터 이동 취소
	{
		g_pNk2DFrame->OnClosePremiumChaMove();
		// IDS_PREMIUM_MSG_21 캐릭터 이동이 종료 되었습니다.
		g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_PREMIUM_MSG_21), TYPE_NOR_OK );
	}
}

void CmdLaghaimPoint(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf ); //LP

	if( g_pRoh )
		g_pRoh->SetLaghaimPoint( g_ii[0] );
	if( pMgr && g_pNk2DFrame && g_pNk2DFrame->IsShopVisible() )
		g_pNk2DFrame->GetControlShop()->SetMoney();
}

void CmdFamePoint(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg( arg, g_Buf );
	g_ii[ 0 ] = atoi( g_Buf ); //LP

//	if( g_pRoh )
//		g_pRoh->SetLaghaimPoint( g_ii[0] );
	if( pMgr && g_pNk2DFrame && g_pNk2DFrame->IsShopVisible() )
		g_pNk2DFrame->GetControlShop()->SetMoney( true, g_ii[0] );
}


void CmdLPShopList(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg( arg, g_Buf);
	g_ii[0] = atoi(g_Buf);	// 리스트 갯수

	if(g_pNk2DFrame->GetControlShop())
	{
		g_pNk2DFrame->GetControlShop()->LPShopIndexClear();
		g_pNk2DFrame->GetControlShop()->SetLpShopFlag();
		g_pNk2DFrame->GetControlShop()->RemoveAllItems();
	}

	for( int i = 0 ; i < g_ii[0] ; ++i )
	{

		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi(g_Buf);	// 상점 타입(무슨 상점인지..)

		arg = AnyOneArg(arg, g_Buf);
		g_ii[2] = atoi(g_Buf);	// Npc 인덱스.

		if(g_pNk2DFrame->GetControlShop())
		{
			g_pNk2DFrame->GetControlShop()->AddLPShopIndex( g_ii[2] );
		}
	}

	// index를 모두 받았으면 창을 열자
	if(g_pNk2DFrame->GetControlShop())
	{
		g_pNk2DFrame->ShowShopWindow(TRUE);
		g_pNk2DFrame->GetControlShop()->SetLpShopFlag(TRUE);

	}



}

void CmdBossraidHonorList(char *arg, CUIMgr *pMgr)
{
	if( g_pNk2DFrame->GetHeroRankList() )
		g_pNk2DFrame->SetHeroRankList( arg );
}

void CmdBossraidHeroPrivilege(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	if( g_pNk2DFrame->GetHeroPrivilege() )
		g_pNk2DFrame->SetHeroPrivilege( atoi(g_Buf) );

}


void CmdEventWearFlag(char *arg, CUIMgr *pMgr)
{
	// ev_ewarflag [char_index] [flag] // flag 0:off 1:on

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf); // cha index

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi(g_Buf); // flag

	pCha = NULL;
	pCha = pMgr->FindChaById(g_ii[0], TRUE );

	if(pCha)
		pCha->m_bEventWearFlag = g_ii[1];
}


void CmdSetItem(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	if(!strcmp(g_Buf, "flag"))
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi( g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		g_ii[1] = atoi( g_Buf);

		if( g_pNk2DFrame && (CControlScore*)g_pNk2DFrame->GetInterface(SCORE) )
		{
			CControlScore* pScore = (CControlScore*)g_pNk2DFrame->GetInterface(SCORE);
			pScore->SetSetitemFlag(g_ii[0], g_ii[1]);
		}
	}
}



// [2008/8/14 Theodoric] 운영자가 인벤을 손쉽게 지우기 위해 만든 패킷
void CmdInitInven(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi( g_Buf);

	g_pRoh->RemoveAllInven(g_ii[0] );
}

void CmdMatrixMsg(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi( g_Buf);
	pMgr->m_pMatrixWarMsg->MatrixMsgSet( g_ii[0] );
}

// [3/30/2009 D.K ] : 아이템 삭제 루틴 추가
void CmdDeleteItem(char *arg, CUIMgr *pMgr)
{
	// by evilkiki 2010.12.09. 삭제루틴 제거
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi( g_Buf );			// quick slot No.
	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi( g_Buf );			// index No.
	g_pRoh->RemoveByIndex( g_ii[1] );
	g_pRoh->CountQuickAll();
}


/*
pMgr->ChipExchange(g_ii[0], TRUE, g_ii[1], g_ii[3], g_ii[2]);
else
pMgr->ChipExchangeMulti(g_ii[0], TRUE, g_ii[1], g_ii[4], g_ii[2], arg);

  g_pNk2DFrame->GetGamble()->ChipExchange(type, bResult, vnum, svr_idx, binggo_num);
*/
void CmdRulletSystem(char *arg, CUIMgr *pMgr)
{
	strcpy(g_pNk2DFrame->GetGamble()->m_szArg, arg);

	/*
	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi( g_Buf );		// 1 : 성공,   -1 : 실패

	arg = AnyOneArg(arg, g_Buf);
	g_ii[1] = atoi( g_Buf );		// 조건실패[ 오류번호 ], 결과[ 0:꽝, 1:한번더, 2:1단계up, 3:1단계down, 4:2단계down
									// 5:2배, 6:3배, 7:4배, 8:5배

	arg = AnyOneArg(arg, g_Buf);
	g_ii[2] = atoi( g_Buf );		// 보상 받을 아이템 vnum

	arg = AnyOneArg(arg, g_Buf);
	g_ii[3] = atoi( g_Buf );		// 보상 받을 보석 개수

	printf("state : %d,  result : %d,  Ite0mvNum : %d,  ItemCnt : %d,  ItemChar : %s\n",
		g_ii[0], g_ii[1], g_ii[2], g_ii[3], arg);
	*/

}


void CmdStudentList(char * arg, CUIMgr *pMgr)
{
	if( g_pNk2DFrame->ControlSelmenuSetMetuList(1, arg) )
		g_pNk2DFrame->ShowControlSelMenu(TRUE);
}

void CmdLagrush( char* arg, CUIMgr* pMgr )
{
	AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf );

	pMgr->SetLagrushPayment( g_ii[0] );

	if( pMgr->m_bAutoHunt && g_ii[0] == 0 )
	{
		// 결제?
		pMgr->m_AutoPlay->StopProc();
	}
}

void CmdLagrushTime(char* arg, CUIMgr* pMgr)
{
	AnyOneArg( arg, g_Buf );
	g_ii[0] = atoi( g_Buf );

	if( pMgr->m_bAutoHunt && g_ii[0] == 0 )
		pMgr->m_AutoPlay->StopProc(AutoPlay_StopProc__TimeOver);
	else
	{
		char buf[128];
		sprintf( buf , G_STRING(IDS_LHSTRING1728) , g_ii[0] );
		g_pNk2DFrame->GetControlBottom()->AddStrQlist( buf , -1 );
	}
}

void CmdLockSmith(char* arg, CUIMgr* pMgr)
{
	strcpy(g_pNk2DFrame->GetLockSmith()->m_MsgBuf, arg);
}

void CmdMoveValue(char *arg, CUIMgr *pMgr)
{
	arg = AnyOneArg(arg, g_Buf);

	g_ii[0] = atoi( g_Buf );		// 0 : 0%,   1 : 10%,   2 : 20%

	switch( g_ii[0] )
	{
	case 0:
		pCMyApp->m_fMoveValue = 0.0f;
		break;
	case 1:
		pCMyApp->m_fMoveValue = 0.25f;		// 0.1
		break;
	case 2:
		pCMyApp->m_fMoveValue = 0.5f;		// 0.2
		break;
	default:
		printf( "character move value error!!!");
		break;
	}
}

void CmdArupakaMoveValue(char *arg, CUIMgr *pMgr)
{
}

void CmdBuyItemFail(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int shop_no = atoi( g_Buf );
	arg = AnyOneArg(arg, g_Buf);
	int item_vnum = atoi( g_Buf );

	if( pMgr->m_AutoPlay->IsRun() )
	{
		pMgr->m_AutoPlay->_OnInterrupt(LagRush_Interrupt__Error_Stash_Get);
	}
	else
	{
		// 정상 플레이의 경우 진입이 불가!!!
		_asm nop;
	}
}

void CmdLagrushItemResult(char* arg, CUIMgr* pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int buy_result = atoi( g_Buf );

	if( buy_result == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		int failed_reason = atoi( g_Buf );

		enum
		{
			Buy_Fail_Error__03,	// Slot_Full
			Buy_Fail_Error__04,	// Slot_needmoney
			Buy_Fail_Error__05, // Dont_buy
			Error__EndOfEnum

		};

		failed_reason += (int)LagRush_Interrupt__InvenFull - 3;
		if( pMgr->m_AutoPlay->IsRun() )
		{
			pMgr->m_AutoPlay->_OnInterrupt((LagRush_Interrupt)failed_reason);
		}
		else
		{
			// 정상 플레이의 경우 진입이 불가!!!
			_asm nop;
		}
	}
	else
	{
		pMgr->m_AutoPlay->OnRecvShop();
	}
}


void CmdUsedQuickItem(char*arg, CUIMgr*pMgr)
{
	bool moveto = false;

	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		// error message
		arg = AnyOneArg(arg, g_Buf);
		int slot = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int index = atoi(g_Buf);


		arg = AnyOneArg(arg, g_Buf);
		eUseQuickError_Type err = (eUseQuickError_Type)atoi(g_Buf);
		// error no( 이미사용된 아이템이다 )

		CItem * pQuickSlotItem = g_pRoh->m_QuickSlot[slot];
		if( pQuickSlotItem && pQuickSlotItem->m_Index == index )
		{
			switch( GET_SHAPE(pQuickSlotItem) )
			{
			case IETC_GEMSTONE:
				{
				}
				break;
			case IETC_RECOVERY:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot
					  )
					{
						if( pQuickSlotItem->m_Num0 != 13 )
							moveto = true;
						g_pRoh->RemoveQuickItem(slot, index , moveto , true);
					}
				}
				break;
			case IETC_PREMIUM:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot
					  )
					{
						if( pQuickSlotItem->m_Num0 == 3 )
						{
						}
						else
						{
							moveto = true;
							g_pRoh->RemoveQuickItem(slot, index , moveto);
						}
					}
				}
				break;
			case IETC_QUICK:
				{
				}
				break;
			case IETC_QUEST:
				{
					enum Etc_Quest__Type
					{
						Etc_Quest__Random,
						Etc_Quest__Fix,
						Etc_Quest__Dependence,
						Etc_Quest__Document,


						Etc_Quest__EndOfEnum
					};

					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						switch( pQuickSlotItem->m_Num0 )
						{
						case Etc_Quest__Random:
							{
							}
							break;
						case Etc_Quest__Fix:
						case Etc_Quest__Dependence:
						case Etc_Quest__Document:
							{
								moveto = false;
								g_pRoh->RemoveQuickItem(slot, index , moveto);
							}
							break;
						}
					}
				}
				break;
			case IETC_PET:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot, index , moveto);
					}
				}
				break;

			case IETC_WARP:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						if( pQuickSlotItem->m_Num0 == 2 )
						{
							moveto = false;
						}

						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;

			case IETC_BUFF:
			case IETC_SUMMONNPC:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;
			case IETC_SKILL:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;

			case IETC_RESOURCE:
				break;
			case IETC_EFFECT:
				break;
			case IETC_USING:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;
			case IETC_SCROLL:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}


				}
				break;
			case IETC_SUMMON:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						// 퀵슬롯 에러
					}

					// 				if( state == 0 )
					// 				{
					// 				}
					// 				else if( state == 1 )
					// 				{
					// 				}
					// 				else if( state == 2 )
					// 				{
					// 					moveto = true;
					// 					g_pRoh->RemoveQuickItem(slot , index , moveto);
					// 				}
				}
				break;
			case IETC_SHELL:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;
			case IETC_CHARGE:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;
			case IECT_MOBSUMMON:
				{
					if( err == eUseQuickError_Invalid_Item
							|| err == eUseQuickError_Invalid_Slot )
					{
						moveto = false;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;
			}
		}
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int slot = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int index = atoi(g_Buf);

		CItem * pQuickSlotItem = g_pRoh->m_QuickSlot[slot];
		if( pQuickSlotItem && pQuickSlotItem->m_Index == index && rv )
		{
			switch( GET_SHAPE(pQuickSlotItem) )
			{
			case IETC_GEMSTONE:
				{
					arg = AnyOneArg(arg, g_Buf);
					int add_item = atoi(g_Buf);
					int gemToVnum = GET_TOGEM_VNUM(pQuickSlotItem);
					g_pRoh->RemoveQuickItem(slot, index , false);
					g_pRoh->ToQuickSlot(slot, add_item , gemToVnum ,0 , 0 , 0);
					g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1835) , -1 );
				}
				break;
			case IETC_RECOVERY:
				{
					arg = AnyOneArg(arg, g_Buf);
					int remain_count = atoi(g_Buf);
					if( remain_count > 0 )
					{
						g_pRoh->m_QuickSlot[slot]->m_Special = remain_count;
						g_pNk2DFrame->GetControlInven()->ResetItemInfo();
						return;
					}

					if( pQuickSlotItem->m_Num0 != 13 )
						moveto = true;
					g_pRoh->RemoveQuickItem(slot, index , moveto , true);
				}
				break;
			case IETC_PREMIUM:
				{
					if( pQuickSlotItem->m_Num0 == 3 )
					{
						// 몬가 복잡한 처리!!!
// 						arg = AnyOneArg(arg, g_Buf);
// 						int step = atoi(g_Buf);
					}
					else
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot, index , moveto);
					}
				}
				break;			
			
			case IETC_QUEST:
				{
					arg = AnyOneArg(arg, g_Buf);
					int remain_count = atoi(g_Buf);

					enum Etc_Quest__Type
					{
						Etc_Quest__Random,
						Etc_Quest__Fix,
						Etc_Quest__Dependence,
						Etc_Quest__Document,


						Etc_Quest__EndOfEnum
					};

					switch( pQuickSlotItem->m_Num0 )
					{
					case Etc_Quest__Random:
						{
							g_pRoh->m_QuickSlot[slot]->m_Special = remain_count;
							g_pNk2DFrame->GetControlInven()->ResetItemInfo();
						}
						break;
					case Etc_Quest__Fix:
					case Etc_Quest__Dependence:
					case Etc_Quest__Document:
						{
							moveto = false;
							g_pRoh->RemoveQuickItem(slot, index , moveto);
						}
						break;
					}
				}
				break;

			case IETC_PET:
				{
					moveto = true;
					g_pRoh->RemoveQuickItem(slot, index , moveto);
				}
				break;

			case IETC_QUICK:
				{
					moveto = true;
					g_pRoh->RemoveQuickItem(slot, index, moveto);
				}
				break;

			case IETC_WARP:
				{
					moveto = true;
					if( pQuickSlotItem->m_Num0 == 2 )
					{
						arg = AnyOneArg(arg, g_Buf);
						int zone_index = atoi(g_Buf);

						char msg_buf[64];
						msg_buf[0] = 0;
						sprintf( msg_buf , "go_zone %d %d 0\n" , slot , zone_index );
						LH_SEND_NET_MSG( msg_buf );

						pCMyApp->StopHero(TRUE);
						CPacketControl::EnableSend(FALSE);

						moveto = false;
					}

					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IETC_BUFF:
				{
					arg = AnyOneArg(arg, g_Buf);
					int remain_count = atoi(g_Buf);
					if( remain_count > 0 )
					{
						g_pRoh->m_QuickSlot[slot]->m_Special = remain_count;
						g_pNk2DFrame->GetControlInven()->ResetItemInfo();
						return;
					}

					// 이동필요
					CItem * buff_item = g_pRoh->m_QuickSlot[slot];

					if( !buff_item )
						return;

					moveto = true;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IETC_SUMMONNPC:
				{
					arg = AnyOneArg(arg, g_Buf);
					int remain_count = atoi(g_Buf);
					if( remain_count > 0 )
					{
						g_pRoh->m_QuickSlot[slot]->m_Special = remain_count;
						g_pNk2DFrame->GetControlInven()->ResetItemInfo();
						return;
					}

					moveto = true;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IETC_SKILL:
				{
					arg = AnyOneArg(arg, g_Buf);
					int skill_no = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int skill_level = atoi(g_Buf);

					g_ChaSkill.SetSkillLevel(skill_no , skill_level);
					if( g_pNk2DFrame
							&& g_pNk2DFrame->GetControlSkill())
					{
						g_pNk2DFrame->GetControlSkill()->m_pSkillInfo->SetIndex(-100);
						g_pNk2DFrame->GetControlSkill()->SetSkillEnable();
					}

					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;

			case IETC_RESOURCE:
				break;
			case IETC_EFFECT:
				break;
			case IETC_USING:
				{
					moveto = true;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IETC_SCROLL:
				{
					arg = AnyOneArg(arg, g_Buf);
					int plus_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int special_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int etc_slot = atoi(g_Buf);

					switch( g_pRoh->m_QuickSlot[slot]->m_Num0 )
					{
					case 0:
					case 7:
						{
							g_pRoh->UpdateEquipItem(plus_num , special_num , etc_slot);
						}
						break;
					default:
						{
							g_pRoh->UpdateQuickItem(plus_num , special_num , etc_slot);
						}
						break;
					}
					moveto = true;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IETC_SUMMON:
				{
					arg = AnyOneArg(arg, g_Buf);
					int plus_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int special_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);

					int state = atoi(g_Buf);

					g_pRoh->UpdateQuickItem(plus_num , special_num , slot);


					if( state == 0 )
					{
						// 솬됨
					}
					else if( state == 1 )
					{
						// 해제
					}
					else if( state == 2 )
					{
						moveto = true;
						g_pRoh->RemoveQuickItem(slot , index , moveto);
					}
				}
				break;
			case IETC_SHELL:
				{
					arg = AnyOneArg(arg, g_Buf);
					int plus_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int special_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int etc_slot = atoi(g_Buf);

					g_pRoh->UpdateEquipItem(plus_num , special_num , WEARING_CLOAK);
					moveto = true;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IETC_CHARGE:
				{
					arg = AnyOneArg(arg, g_Buf);
					int plus_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int special_num = atoi(g_Buf);
					arg = AnyOneArg(arg, g_Buf);
					int etc_slot = atoi(g_Buf);

					switch( g_pRoh->m_QuickSlot[slot]->m_Num0 )
					{
					case 0:
						{
							g_pRoh->UpdateQuickItem(plus_num , special_num , etc_slot);
						}
						break;
					case 1:
						g_pRoh->UpdateEquipItem(plus_num , special_num , WEARING_CLOAK);
						break;
					case 2:
						g_pRoh->UpdateEquipItem(plus_num , special_num , etc_slot);
						break;
					case 3:
						g_pRoh->UpdateEquipItem(plus_num , special_num , WEARING_BRACELET);
						break;
					default:
						{
							g_pRoh->UpdateEquipItem(plus_num , special_num , etc_slot);
						}
						break;
					}

					moveto = true;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			case IECT_MOBSUMMON:
				{
					moveto = false;
					g_pRoh->RemoveQuickItem(slot , index , moveto);
				}
				break;
			}
		}
	}
}

void CmdMovedItem(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		// error message
		arg = AnyOneArg(arg, g_Buf);
		int slot = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int index = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int err_no = atoi(g_Buf);

		// error 관련 함수 추가예정
		if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
		{
			switch( err_no )
			{
			case 1:
				pMgr->m_AutoPlay->AddLog( "*****Quick slot number error*****" , 1 );
				break;
			case 2:
				pMgr->m_AutoPlay->AddLog( "*****Pack number error*****" , 1 );
				break;
			case 3:
				pMgr->m_AutoPlay->AddLog( "*****Invalid item index*****" , 1 );
				break;
			}
		}
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int slot = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int index = atoi(g_Buf);

		if( !g_pRoh->InvenToQuick(slot , index) )
		{
			_asm nop;
		}
	}
}

void CmdRevival_Rcv(char*arg, CUIMgr*pMgr)
{
	// 살아났어요!!!
}

// 인벤->창고(속성있는)
void CmdStashPutS(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		int nothing = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int err_code = atoi(g_Buf);

		if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
		{
			if( g_pNk2DFrame->IsWareHouseVisible() )
				g_pNk2DFrame->ToggleWareHouseWindow();

			err_code += (int)LagRush_Interrupt__Wrong_Zone;
			pMgr->m_AutoPlay->_OnInterrupt((LagRush_Interrupt)err_code);
		}
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int vnum = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int ware_slot = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int inven_pack = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int count = atoi(g_Buf);

		for( int i=0 ; i<count ; i++ )
		{
			arg = AnyOneArg(arg, g_Buf);
			int item_index = atoi(g_Buf);
			pMgr->StashPuts(vnum , ware_slot , inven_pack , item_index );
			g_pRoh->CountQuickAll();
		}

		if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
		{
			pMgr->m_AutoPlay->OnRecvStash();
		}
	}
}

// 창고->인벤(단일)
void CmdStashGetS(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		int err_code = atoi(g_Buf);

		switch( err_code )
		{
		case 0:
			break;
		default:
			break;
		}

		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1836) , -1 );

		if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
		{			
			pMgr->m_AutoPlay->OnRecvStash();
		} 
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int vnum = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int ware_slot = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int inven_pack = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int count = atoi(g_Buf);

		for( int i=0 ; i<count ; i++ )
		{
			arg = AnyOneArg(arg, g_Buf);
			int item_index = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int slot_x = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int slot_y = atoi(g_Buf);
			pMgr->StashGets(vnum , ware_slot , inven_pack , item_index , slot_x , slot_y );
		}

		if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
		{
			pMgr->m_AutoPlay->OnRecvStash();
		} 
	}
}


void CmdMovedApple(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		int apple_count = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int item_index = atoi(g_Buf);

		g_MyPetStatus.SetFoodCount(apple_count);
		g_pRoh->RemoveInvenItem(item_index);
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int apple_count = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int item_index = atoi(g_Buf);

		g_MyPetStatus.SetFoodCount(apple_count);
		g_pRoh->RemoveInvenItem(item_index);

		if( pMgr->m_AutoPlay && pMgr->m_AutoPlay->IsRun() )
			pMgr->m_AutoPlay->OnRecvApple();
	}
}



void CmdGetGoodsLists(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int num_items = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int page_cur =  atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	int num_cur_item =  atoi(g_Buf);

	if( num_items == 0 )
	{
		g_pNk2DFrame->ShowTakeboxWindow(false,0);
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LHSTRING1854) , TYPE_NOR_OK );
		return;
	}

	if( g_pNk2DFrame->GetTakeOutBox() )
	{
		g_pNk2DFrame->GetTakeOutBox()->SetTakebox( TakeBox_Type__User , num_items , page_cur , num_items );

		for( int i=0 ; i<num_cur_item ; i++ )
		{
			arg = AnyOneArg(arg, g_Buf);
			int db_index =  atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int vnum =  atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int count =  atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			g_pNk2DFrame->GetTakeOutBox()->InsertItem( db_index , vnum , count , g_Buf );
		}

		g_pNk2DFrame->GetTakeOutBox()->SetRecived(2);
	}
}

void CmdGetPresentLists(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int type = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int num_items = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int page_cur =  atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	int num_cur_item =  atoi(g_Buf);

	if( num_items == 0 )
	{	
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LHSTRING1854) , TYPE_NOR_OK );		
	}
	
	LHUI_TakeBox* pTakeBox = g_pNk2DFrame->GetTakeOutBox();
	if( pTakeBox )
	{
		TakeBox_Type takeBoxType = TakeBox_Type__Character;
		if( type == 1 )
		{
			takeBoxType = TakeBox_Type__ServerMove;
		}		

		pTakeBox->ClearList();
		pTakeBox->SetTakebox( takeBoxType , num_items , page_cur , num_items );

		for( int i=0 ; i<num_cur_item ; i++ )
		{
			arg = AnyOneArg(arg, g_Buf);
			int db_index =  atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int vnum =  atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			int count =  atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			pTakeBox->InsertItem( db_index , vnum , count , g_Buf );
		}

		pTakeBox->SetRecived(2);
	}
}

// 실패 0 : 유효성 실패
// 성공 1 : 남은개수( 0일때 리스트 삭제, 0이상이면 갱신 )
void CmdGetGoodsResult(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		int err_code = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int db_index = atoi(g_Buf);

		if( g_pNk2DFrame->GetTakeOutBox() )
			g_pNk2DFrame->GetTakeOutBox()->SetRecived(3);
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int remain_count = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int db_index = atoi(g_Buf);

		if( g_pNk2DFrame->GetTakeOutBox() )
		{
			g_pNk2DFrame->GetTakeOutBox()->EditItem(db_index , remain_count);
			g_pNk2DFrame->GetTakeOutBox()->SetRecived(2);
		}

		if( remain_count !=0 )
		{
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LHSTRING1855) , TYPE_NOR_OK );
		}
	}
}

void CmdGetPresentResult(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int rv = atoi(g_Buf);

	if( rv == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		int err_code = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int db_index = atoi(g_Buf);

		if( g_pNk2DFrame->GetTakeOutBox() )
			g_pNk2DFrame->GetTakeOutBox()->SetRecived(3);
	}
	else
	{
		arg = AnyOneArg(arg, g_Buf);
		int remain_count = atoi(g_Buf);
		arg = AnyOneArg(arg, g_Buf);
		int db_index = atoi(g_Buf);


		if( g_pNk2DFrame->GetTakeOutBox() )
		{
			g_pNk2DFrame->GetTakeOutBox()->EditItem(db_index , remain_count);
			g_pNk2DFrame->GetTakeOutBox()->SetRecived(2);
		}		
	}
}

void CmdResultCaru(char*arg, CUIMgr*pMgr)
{
	arg = AnyOneArg(arg, g_Buf);
	int count = atoi(g_Buf);

	if( count == 0 )
	{
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LHSTRING1855) , TYPE_NOR_OK );
		return;
	}

	g_pNk2DFrame->InsertPopup( "caru", TYPE_CARU_RESULT, POPUP_CARU_RESULT);
	CMsgPopUp* p = g_pNk2DFrame->FindPopup(POPUP_CARU_RESULT);
	CaruResultData* data = p->m_pCaruResultData;
	if( data == NULL )
	{
		return;
	}

	for(int i = 0; i < count; i++)
	{
		arg = AnyOneArg(arg, g_Buf);
		data->SetIndex(i, g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		int vnum = atoi(g_Buf);
		data->SetItemName(i, GET_ITEM_NAME_VNUM(vnum));

		arg = AnyOneArg(arg, g_Buf);
		data->SetItemCount(i, g_Buf);
	}
	p->RestoreAll();	
}

void CmdBuffEx(char*arg, CUIMgr*pMgr)
{
	int bv = 0; // 수치
	int type = 0;
	arg = AnyOneArg(arg, g_Buf);
	if (!strcmp(g_Buf, "att"))
	{
		arg = AnyOneArg(arg, g_Buf);
		bv = atoi(g_Buf);
		type = 0;
	}
	else if (!strcmp(g_Buf, "def"))
	{
		arg = AnyOneArg(arg, g_Buf);
		bv = atoi(g_Buf);
		type = 1;
	}
	else if (!strcmp(g_Buf, "exp"))
	{
		arg = AnyOneArg(arg, g_Buf);
		bv = atoi(g_Buf);
		type = 2;
	}
	else
	{
		return;
	}

	if( g_pNk2DFrame->GetNewEvent() )
		g_pNk2DFrame->GetNewEvent()->SetEvent_Ex( type , bv );
}



void CmdItemMake(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetUpgrade() )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 강화 애니메이션으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_ING;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].m_AniType = SPRITE_ANITYPE_LOOP;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].SetStart();
	g_pNk2DFrame->GetUpgrade()->m_dwStartTickTime = timeGetTime();

	g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.


	if (!strcmp(g_Buf, "ok"))
	{
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS;
	}
	else if (!strcmp(g_Buf, "fail"))
	{
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_FAIL;
	}
	else
	{
		return;
	}
}

void CmdItemSeparate(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetUpgrade() )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 강화 애니메이션으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_ING;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].m_AniType = SPRITE_ANITYPE_LOOP;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].SetStart();
	g_pNk2DFrame->GetUpgrade()->m_dwStartTickTime = timeGetTime();

	g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.


	if (!strcmp(g_Buf, "ok"))
	{
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS;
	}
	else
	{
		return;
	}
}

void CmdBuffMakeMake(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetUpgrade() )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 강화 애니메이션으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_ING;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].m_AniType = SPRITE_ANITYPE_LOOP;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].SetStart();
	g_pNk2DFrame->GetUpgrade()->m_dwStartTickTime = timeGetTime();

	g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.

	int nResult = atoi(g_Buf);

	if( nResult == 0 )
	{
		g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_SUCCESS;
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS;
	}
	else
	{
		g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_FAIL;
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_FAIL;
	}
}

void CmdBuffMakeGatcha(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetUpgrade() )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 강화 애니메이션으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_ING;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].m_AniType = SPRITE_ANITYPE_LOOP;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].SetStart();
	g_pNk2DFrame->GetUpgrade()->m_dwStartTickTime = timeGetTime();

	g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.

	int nResult = atoi(g_Buf);

	if( nResult == 0 )
	{
		g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_SUCCESS;
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS;
	}
	else
	{
		g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_FAIL;
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_FAIL;
	}
}

void CmdBuffMakeMedal(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame || !g_pNk2DFrame->GetUpgrade() )
		return;

	arg = AnyOneArg( arg, g_Buf );

	// 강화 애니메이션으로 셋팅.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_ING;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].m_AniType = SPRITE_ANITYPE_LOOP;
	g_pNk2DFrame->GetUpgrade()->m_SpriteAni[UPGRADE_ANI_ING].SetStart();
	g_pNk2DFrame->GetUpgrade()->m_dwStartTickTime = timeGetTime();

	g_pNk2DFrame->GetUpgrade()->m_bSlotLock = FALSE; // 락을 해제.
	g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_NON; // 상태 복원.

	int nResult = atoi(g_Buf);

	if( nResult == 0 )
	{
		g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_SUCCESS;
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_SUCCESS;
	}
	else
	{
		g_pNk2DFrame->GetUpgrade()->m_NowState = UPGRADE_ANI_FAIL;
		g_pNk2DFrame->GetUpgrade()->m_Result = UPGRADE_ANI_FAIL;
	}
}

void CmdUpEnergy(  char* arg, CUIMgr* pMgr )
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	arg = AnyOneArg( arg, g_Buf );
	int rv_type = atoi(g_Buf);
	arg = AnyOneArg( arg, g_Buf );
	int item_index = atoi(g_Buf);

	if( rv_type == TRUE )
	{
		g_pRoh->RemoveExtraItem(item_index);
	}
	else
	{
		arg = AnyOneArg( arg, g_Buf );
		int err_code = atoi(g_Buf);
		if(err_code == 0 )
		{
			g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_LHSTRING1886) , -1);
		}
		else
		{
			// 원인불명?
		}
	}
}

// 지속형 스킬을 여기다가 넣어줄까나? 3개정도?
void CmdContinueSkill(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	arg = AnyOneArg( arg, g_Buf );
	int char_index = atoi(g_Buf);
	arg = AnyOneArg( arg, g_Buf );
	int skill_01 = atoi(g_Buf);		// 필드
	arg = AnyOneArg( arg, g_Buf );
	int skill_02 = atoi(g_Buf);		// 필드 ex
	arg = AnyOneArg( arg, g_Buf );
	int skill_03 = atoi(g_Buf);		// d system
	arg = AnyOneArg( arg, g_Buf );
	int skill_04 = atoi(g_Buf);		// engage
	arg = AnyOneArg( arg, g_Buf );
	int skill_05 = atoi(g_Buf);		// critical

	CNkCharacter * target = pMgr->GetCharacterByID(char_index);
	if( !target )
		return;


	if( target->m_Race == RACE_PEROM )
	{
		if( skill_01 )
			target->SetActiveField();
		else
			target->SetDeactiveField();

		if( skill_02 )
			target->SetActiveField_Ex();
		else
			target->SetDeactiveField_Ex();

		if( skill_03 )
			target->SetActiveDSystem();
		else
			target->SetDeactiveDSystem();

		if( skill_04 )
			target->SetActiveEngage();
		else
			target->SetDeactiveEngage();

		if( skill_05 )
			target->SetActiveCritical();
		else
			target->SetDeactiveCritical();
	}
}

void CmdMaStone(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	arg = AnyOneArg( arg, g_Buf );
	int nResult = atoi(g_Buf);

	struct E_RESULT
	{
		enum { FAILED = 0, SUCCESS = 1 };
	};

	switch ( nResult )
	{
	case E_RESULT::FAILED://정제에 실패하였습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_GMASUK_FAILED), TYPE_NOR_OK, 32 );
		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_ITEM_GMASUK_FAILED), -1);
		break;

	case E_RESULT::SUCCESS://정제에 성공하였습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_ITEM_GMASUK_SUCCESS), TYPE_NOR_OK, 32 );
		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_ITEM_GMASUK_SUCCESS), -1);
		break;
	}

}

void CmdShutdown(char*arg, CUIMgr*pMgr)
{
	if( !pMgr || !g_pNk2DFrame )
		return;

	g_pNk2DFrame->InsertPopup( "Shutdown", TYPE_NOR_OK, 0 );
}

void CmdBuffItemInfo(char* arg, CUIMgr* pMgr)
{
	if( !pMgr )
		return;

	BuffItemMgr* buffItemMgr = BuffItemMgr::GetInstance();

	arg = AnyOneArg(arg, g_Buf);
	if( strcmp(g_Buf, "add") == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);

		int buffIndex = 0;
		int buffRemainTime = 0;
		int buffVal = 0;
		for( int i = 0; i < g_ii[0]; i++ )
		{
			arg = AnyOneArg(arg, g_Buf);
			buffIndex = atoi(g_Buf);
			arg = AnyOneArg(arg, g_Buf);
			buffRemainTime = atoi(g_Buf);
			buffRemainTime *= 1000;
			arg = AnyOneArg(arg, g_Buf);
			buffVal = atoi(g_Buf);

			buffItemMgr->AddBuff(buffIndex, (unsigned long)buffRemainTime, buffVal);
		}
	}
	else if( strcmp(g_Buf, "del") == 0 )
	{
		arg = AnyOneArg(arg, g_Buf);
		g_ii[0] = atoi(g_Buf);

		buffItemMgr->RemoveBuff(g_ii[0]);
	}
}

void CmdCharacterScaling(char* arg, CUIMgr* pMgr)
{
	if (!pMgr)
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);			// index

	arg = AnyOneArg(arg, g_Buf);
	float scaleValue = atof(g_Buf);

	pCha = pMgr->FindChaById(g_ii[0]);

	if (pCha)
	{
		pCha->scaleCharacter(scaleValue);
	}
}

void CmdFeverTime(char* arg, CUIMgr* pMgr)
{
	if (!pMgr || !g_pNk2DFrame)
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	g_pNk2DFrame->GetNewEvent()->SetEvent(OpEvent_Type__Fever_Time, g_ii[0]);
}

void CmdNameHide(char* arg, CUIMgr* pMgr)
{
	if (!pMgr || !g_pNk2DFrame)
		return;

	arg = AnyOneArg(arg, g_Buf);
	g_ii[0] = atoi(g_Buf);

	if (g_ii[0])
	{
		g_SvrMode = eSVR_MASTER;
	}
	else
	{
		g_SvrMode = eSVR_NORMAL;
	}
}

void CmdFriend(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	RecvFriend(arg);
}

void CmdNVisible(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int vnum = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	BOOL bVisible = atoi(g_Buf);

	pMgr->SetVisibleMob(vnum, bVisible);
}

void CmdYutThrow(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	YutManager* yutMgr = YutManager::GetInstance();
	ControlYutboard* board = (ControlYutboard*)g_pNk2DFrame->GetInterface(YUTBOARD);

	arg = AnyOneArg(arg, g_Buf);
	int error = atoi(g_Buf);

	if( error != 0 )
	{
		board->SetEnableStartBtn(true);
		return;
	}

	arg = AnyOneArg(arg, g_Buf);
	int result = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	int pos = atoi(g_Buf);

	yutMgr->SetPosIndex(pos);
	board->SetAniYutStart(result);
}

void CmdYutInfo(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	YutManager* yutMgr = YutManager::GetInstance();
	ControlYutboard* board = (ControlYutboard*)g_pNk2DFrame->GetInterface(YUTBOARD);

	arg = AnyOneArg(arg, g_Buf);
	int complete = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	int pos = atoi(g_Buf);
	arg = AnyOneArg(arg, g_Buf);
	BOOL bMapIn = atoi(g_Buf);

	yutMgr->SetCompleteCount(complete);
	if( bMapIn == TRUE )
	{
		board->SetCurPos(pos);
		board->SetCompleteCount(complete);
	}
}

void CmdCheckNpcSummon(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int itemIndex = atoi(g_Buf);

	g_pNk2DFrame->InsertPopup(arg, TYPE_NOR_OKCANCLE, POPUP_CHECK_SUMMON_NPC_NOTIFY, 30, 0, (void*)itemIndex);
}

void CmdCloseNpcSummon(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	g_pNk2DFrame->CloseAllWindow();
}

void CmdSummonNpcSummon(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int npcIndex = atoi(g_Buf);

	if( g_pRoh == NULL )
		return;

	g_pRoh->SetSummonNpcIndex(npcIndex);
}

void CmdFishThrow(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int errType = atoi(g_Buf);
	if( errType != 0 )
	{
		ControlFishing* board = (ControlFishing*)g_pNk2DFrame->GetInterface(FISHING);
		board->Init();

		return;
	}

	arg = AnyOneArg(arg, g_Buf);
	int fishType = atoi(g_Buf);

	FishingManager* fishMgr = FishingManager::GetInstance();
	int count = fishMgr->GetFishCount(fishType);
	++count;
	fishMgr->SetFishCount(fishType, count);

	ControlFishing* board = (ControlFishing*)g_pNk2DFrame->GetInterface(FISHING);
	board->SetResult(fishType);
}

void CmdFishInfo(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int typeCount = atoi(g_Buf);

	int count = 0;
	int fishType = 0;
	FishingManager* fishMgr = FishingManager::GetInstance();
	fishMgr->Clear();
	for(int i = 0; i < typeCount; i++)
	{
		arg = AnyOneArg(arg, g_Buf);
		fishType = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		count = atoi(g_Buf);		

		fishMgr->SetFishCount(fishType, count);
	}

	ControlFishing* board = (ControlFishing*)g_pNk2DFrame->GetInterface(FISHING);
	board->Init();	
}

void CmdFishGiftList(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	FishingRewardManager* rewardMgr = FishingRewardManager::GetInstance();
	rewardMgr->Clear();
	
	arg = AnyOneArg(arg, g_Buf);
	int count = atoi(g_Buf);

	int grade = 0;
	int index = 0;
	int type = 0;
	for(int i = 0; i < count; i++)
	{
		arg = AnyOneArg(arg, g_Buf);
		grade = atoi(g_Buf);
		
		arg = AnyOneArg(arg, g_Buf);
		index = atoi(g_Buf);

		arg = AnyOneArg(arg, g_Buf);
		type = atoi(g_Buf);

		{
			// 서버와 클라이언트 등급 순서가 다름
			if( grade == 0 )
				grade = 2;
			else if( grade == 2 )
				grade = 0;
		}		

		if( type == 0 )
		{
			rewardMgr->AddCanGet(grade, index);
		}
		else if( type == 1 )
		{
			rewardMgr->AddAlready(grade, index);
		}
	}

	ControlFishingReward* board = (ControlFishingReward*)g_pNk2DFrame->GetInterface(FISHING_REWARD);
	board->Refresh();
}

void CmdFishGiftItem(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	FishingRewardManager* rewardMgr = FishingRewardManager::GetInstance();
	ControlFishingReward* board = (ControlFishingReward*)g_pNk2DFrame->GetInterface(FISHING_REWARD);

	arg = AnyOneArg(arg, g_Buf);
	int result = atoi(g_Buf);

	if( result != 0 )
	{
		board->Refresh();
		return;
	}

	arg = AnyOneArg(arg, g_Buf);
	int grade = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int index = atoi(g_Buf);

	{
		// 서버와 클라이언트 등급 순서가 다름
		if( grade == 0 )
			grade = 2;
		else if( grade == 2 )
			grade = 0;
	}

	rewardMgr->AddAlready(grade, index);

	board->Refresh();
}

void CmdOK(char* arg, CUIMgr* pMgr)
{
	CPacketControl::EnableSend(TRUE);

	char temp[256] = {0, };
	if (g_pTcpIp)
	{
		if( CommonConfig::Instance()->GetThaFlag() == true )
		{
			sprintf(temp, "start_game %s\n", GetMd5CheckSum().c_str());
			g_pTcpIp->SendNetMessage(temp, TRUE);		
		}
		else
		{
			g_pTcpIp->SendNetMessage("start_game\n", TRUE);
		}
	}

	CPacketControl::EnableSend(FALSE);

	SetItemEffMgr::GetInstance()->Clear();
}

void CmdCharLoadComplete(char* arg, CUIMgr* pMgr)
{
	CPacketControl::EnableSend(TRUE);	
}

void CmdCharsEnd(char* arg, CUIMgr* pMgr)
{
	CPacketControl::EnableSend(TRUE);	
	if (g_pTcpIp)
	{
		char strBuf[MAX_PATH] = {0,};
		sprintf(strBuf, "start %d %d 0 0 0 0 0 0\n", pCMyApp->m_SlotNum, pCMyApp->m_nAreaNum);
		g_pTcpIp->SendNetMessage(strBuf, TRUE);
	}
	CPacketControl::EnableSend(FALSE);
}

void CmdSpeedCheck(char* arg, CUIMgr* pMgr)
{
	SpeedChecker::GetInstance()->StartCheck();
}

void CmdDecardiBattleRes(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int err = atoi(g_Buf);

	g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_DCB_ERR_OK + err) , -1 );
	g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_DCB_ERR_OK + err) , TYPE_NOR_OK, 1 );
}

void CmdDecardiBattleListStart(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	dcbMgr->ClearList();
}

void CmdDecardiBattleList(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int team = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int count = atoi(g_Buf);

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();

	int charIndex = 0;
	for(int i = 0; i < count; i++)
	{
		arg = AnyOneArg(arg, g_Buf);
		charIndex = atoi(g_Buf);

		dcbMgr->AddTeam(team, charIndex);
	}
}

void CmdDecardiBattleListEnd(char* arg, CUIMgr* pMgr)
{
}

void CmdDecardiBattleStart(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	dcbMgr->Start();
	ControlDecardiBattle* uiDCB = g_pNk2DFrame->GetDecardiBattle();
	if( uiDCB )
	{
		uiDCB->Start();
	}	
}

void CmdDecardiBattlePoint(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int redPoint = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int bluePoint = atoi(g_Buf);

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	dcbMgr->SetPoint(0, redPoint);
	dcbMgr->SetPoint(1, bluePoint);
}

void CmdDecardiBattleResult(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int result = atoi(g_Buf);

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	dcbMgr->End();
	ControlDecardiBattle* uiDCB = g_pNk2DFrame->GetDecardiBattle();
	uiDCB->End(result);
}

void CmdDecardiBattleGate(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int nDraw = atoi(g_Buf);
	bool bDraw = (nDraw == 0 ? false : true);

	DecardiBattleManager* dcbMgr = DecardiBattleManager::GetInstance();
	dcbMgr->SetGate(bDraw);

	pCMyApp->GetLandPtr()->SetFieldAttr(dcbMgr->m_attackable[0].x1, dcbMgr->m_attackable[0].z1, dcbMgr->m_attackable[0].x2, dcbMgr->m_attackable[0].z2, nDraw);
	pCMyApp->GetLandPtr()->SetFieldAttr(dcbMgr->m_attackable[1].x1, dcbMgr->m_attackable[1].z1, dcbMgr->m_attackable[1].x2, dcbMgr->m_attackable[1].z2, nDraw);
	pCMyApp->GetLandPtr()->SetFieldAttr(dcbMgr->m_attackable[2].x1, dcbMgr->m_attackable[2].z1, dcbMgr->m_attackable[2].x2, dcbMgr->m_attackable[2].z2, nDraw);
	pCMyApp->GetLandPtr()->SetFieldAttr(dcbMgr->m_attackable[3].x1, dcbMgr->m_attackable[3].z1, dcbMgr->m_attackable[3].x2, dcbMgr->m_attackable[3].z2, nDraw);
	pCMyApp->GetLandPtr()->SetFieldAttr(dcbMgr->m_attackable[4].x1, dcbMgr->m_attackable[4].z1, dcbMgr->m_attackable[4].x2, dcbMgr->m_attackable[4].z2, nDraw);
}

void CmdDecardiBattleDay(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	g_pNk2DFrame->SetInfoBox((char*)G_STRING(IDS_DCB_DAY_INFO1));
	g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_DCB_DAY_INFO1), 4);
	g_pNk2DFrame->SetInfoBox2((char*)G_STRING(IDS_DCB_DAY_INFO2), RGB(0, 0, 0), RGB(100, 100, 250));
	g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_DCB_DAY_INFO2), 4);
	
	g_pNk2DFrame->SetInfoBox_NextString(G_STRING(IDS_DCB_DAY_INFO3));
	g_pNk2DFrame->SetInfoBox_NextString2(G_STRING(IDS_DCB_DAY_INFO4));
}

void CmdLotto(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int err = atoi(g_Buf);
	
	g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_LOTTO_ERR_OK + err) , TYPE_NOR_OK, 1 );

	ControlLotto* pLotto = (ControlLotto*)(g_pNk2DFrame->GetInterface(LOTTO));
	pLotto->RefreshListReq(0);	
}

void CmdLottoList(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int maxPage = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int curPage = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int count = atoi(g_Buf);

	LottoManager* lottoMgr = LottoManager::GetInstance();
	std::vector<int> numbers;
	int num = 0;
		
	lottoMgr->ClearList();

	for(int i = 0; i < count; i++)
	{
		numbers.clear();

		for(int j = 0; j < MAX_LOTTO_INPUT; j++)
		{
			arg = AnyOneArg(arg, g_Buf);
			num = atoi(g_Buf);
			numbers.push_back(num);
		}

		std::sort(numbers.begin(), numbers.end());
		lottoMgr->AddList(numbers);
	}

	ControlLotto* pLotto = (ControlLotto*)(g_pNk2DFrame->GetInterface(LOTTO));
	pLotto->RefreshList();
	pLotto->SetCurPage(curPage);
	pLotto->SetMaxPage(maxPage);
}

void CmdLottoChance(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int count = atoi(g_Buf);

	LottoManager::GetInstance()->SetTicketCount(count);
}

void CmdPetUpgradeDrake(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;	
}

void CmdPetWearDrake(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int w = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int vnum = atoi(g_Buf);

	int item_idx = g_ItemList.FindItem(vnum);
		if (item_idx >= 0)
			pCMyApp->m_pMyPet->m_BodyItem_Class3[w] = item_idx;

		


	//g_MyPetStatus.SetEquip(w, vnum);
}
void CmdPetWearDrake2(char* arg, CUIMgr* pMgr)
{
	if( pMgr == NULL || g_pNk2DFrame == NULL )
		return;

	arg = AnyOneArg(arg, g_Buf);
	int index = atoi(g_Buf);

	arg = AnyOneArg(arg, g_Buf);
	int where = atoi(g_Buf);


	arg = AnyOneArg(arg, g_Buf);
	int vnum = atoi(g_Buf);

	CNkPet *pPet = pMgr->FindPetById(index);

	if(pPet)
	{
	int item_idx = g_ItemList.FindItem(vnum);
		if (item_idx >= 0)
			pPet->m_BodyItem_Class3[where] = item_idx;
	}
		


	//g_MyPetStatus.SetEquip(w, vnum);
}

