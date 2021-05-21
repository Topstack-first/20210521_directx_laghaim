#ifndef _HEADERS_H_
#define _HEADERS_H_

#include "Config.h"
#include <iostream>

#define STARTPAGE		11
#define LOGINPAGE		21
#define SELCHARPAGE		31
#define CRTCHARPAGE		41
#define SETVALPAGE		51
#define SELVILPAGE		61
#define LOGOPAGE		71
#define OPTIONPAGE		81
#define MAKEACCPAGE		91

//................................추가부분
#define CHATPAGE		81
//.......................................

#define BTN_NORMAL		1
#define BTN_ON			2
#define BTN_DOWN		3
#define BTN_DISABLE		4
#define BTN_ACTION		8

#define TRANS_COLOR		RGB(0, 0, 0)

#define MOUSE_ARROW		12
#define MOUSE_KNIFE		22


// popup macro number
typedef enum Pop_Process
{
	POPUP_OK = 1,
	POPUP_GUILDOUT,
	POPUP_TEMP,
	POPUP_WINDOW_CLOSE,
	POPUP_SHOP,			// 5
	POPUP_USESHOP_YESNO,
	POPUP_EVENTANCIENT,
	POPUP_GUILDPK,
	POPUP_INTERCHANGE,
	POPUP_EXCHANGE,			// 10
	POPUP_PARTY,
	POPUP_REFINE,
	POPUP_PROCESS,
	POPUP_QUEST,
	POPUP_GATE,				// 15
	POPUP_MONEYEXCHANGE,
	POPUP_REQUESTPARTY,
	POPUP_MONEYUSERSHOP,
	POPUP_MONEYWAREHOUSE,
	POPUP_MONEYPROCESS,	// 20
	POPUP_MONEYREFINE,
	POPUP_PETNAME,
	POPUP_ERRORMSG,
	POPUP_MONEYWARTEX,
	POPUP_PASSMATLIST,		// 25
	POPUP_WARNINGINVEN,
	POPUP_DOCUMENT,
	POPUP_TIMEITEM,
	POPUP_CASHSHOP_BUY,
	POPUP_NPCUPGRADE_BROKEN,       // 30 ///유료상점
	POPUP_PET_UPGRADE,
	POPUP_NPCUPGRADE_RESULT,
	POPUP_HELPER,
	POPUP_TRAINEEGIVEUP,
	POPUP_RESURRECTION,	    // 35
	POPUP_GO_DEKAREN1,
	POPUP_GO_DEKAREN2,
	POPUP_GO_DEKAREN3,
	POPUP_LETS_PVP,
	POPUP_MONEYGUILDSTOREHOUSE, // 40
	POPUP_GUILD_NORMAL,
	POPUP_GUILD_CREATE,
	POPUP_GUILD_CONFIRM,
	POPUP_GUILD_NAMEINPUT,
	POPUP_GUILD_RECOMINPUT,  // 45
	POPUP_GUILD_CONFIRM_REQUEST,
	POPUP_GUILD_CONFIRM_CANCEL_PROMOTION,
	POPUP_GUILD_CONFIRM_CACEL_DISSOLVE,
	POPUP_GUILD_CONFIRM_CACEL_BREAK,
	POPUP_GUILD_RECOMMAND,	// 50
	POPUP_GUILD_ACCEPT,
	POPUP_GUILD_BATTLE_GO,
	POPUP_GUILD_INVASION_FINISH,
	POPUP_GUILD_RANKWAR_INVITE,
	POPUP_GUILD_WAR_MOVE, // 55
	POPUP_GUILD_MONEYEXCHANGE,
	POPUP_GUILD_JOIN,
	POPUP_WANTED_NORMAL,
	POPUP_GUILD_REALLY_DISPERSE,
	POPUP_GUILD_RECOM_MEMBER, // 60
	POPUP_GUILD_INVASION_FINISH_RES_OK,
	POPUP_MULTI_SALE, ///다중판매
	POPUP_MULTI_SALE_WARRING, ///다중판매
	POPUP_WAREHOUSE_CHECK,	// [1/15/2008 반재승] : 일본 버그 요청사항-기간제 아이템일때 띄우는 메세지
	POPUP_DMITRON_BATTLE_REQUEST, //드미트론 점령전 - 신청
	POPUP_DMITRON_TAX_PAYMENT,//드미트론 점령전 - 세율
	POPUP_PREMIUM_CHA_MOVE_FROM,
	POPUP_PREMIUM_CHA_MOVE_TO,
	POPUP_PREMIUM_CHA_MOVE_NAME_INPUT,
	POPUP_PREMIUM_CHA_MOVE_WAIT,
	POPUP_PREMIUM_CHA_MOVE_SUC_EXIT,
	POPUP_PREMIUM_CHA_MOVE_FAIL,
	POPUP_EVOLUTIONBALL_USE,
	POPUP_HELPER_REJECT, // 후견인 포기
	POPUP_LAGRUSH_START,
	POPUP_STASH_FROM_INVEN_PROC,				// stash_click, stash_get	// warehouse proc 처리
	POPUP_STASH_FROM_WAREHOUSE_PROC,			// stash_put				// inven proc 처리
	POPUP_NPCUPGRADE_BROKEN_CHECK,
	POPUP_FRIEND_ADD_REQ,
	POPUP_CHANGE_CHAR_NAME,
	POPUP_RECYCLE_ITEM,
	POPUP_GMCOMMAND_LOADITEM,
	POPUP_GMCOMMAND_LOADITEM_10,
	POPUP_GMCOMMAND_GOMOB,
	POPUP_GMCOMMAND_GONPC,
	POPUP_CHECK_SUMMON_NPC_NOTIFY,
	POPUP_EVENT_MAD_CONFIRM,
	POPUP_EVENT_MAD_INPUT,
	POPUP_EVENT_MAD_SURE,
	POPUP_EVENT_MAD_ERROR,
	POPUP_CARU_RESULT,
	POPUP_MASTER_DELEGATION,
	POPUP_CHAT_SHOUT,
	POPUP_CHAT_SHOUT_CONFIRM,
	POPUP_BATTLELEAGUE,
	POPUP_BINGO_TEN_TIME,
	POPUP_GIFTTIME,
	POPUP_PET_EQUIP,
	POPUP_910_ENTER,
	POPUP_910_OUT,

} POPUPPROC, POPUP_PROC;

////////// 마우스 이미지 종류에대한 정의////////////
#define M_ATTACT		0
#define M_TALK			1
#define M_UNABLE_AREA	2
#define M_HOLD			3
#define M_NORMAL		4
#define M_NORMAL_CLICK	5
#define M_ATTACT_GUN	6
#define M_ZOOM_IN		7
#define M_ZOOM_OUT		8
#define M_DOOR			9
#define M_COMPAS_INOUT  10
#define M_MAGIC_BOOK	11
#define M_CHAKRAM		12
#define	M_GHOST			13
#define	M_REPAIR		14  // [6/7/2007 Theodoric] 내구도 수리 
#define	M_NON			999


#define TYPE_NOR_OK			10
#define TYPE_NOR_OKCANCLE   20
#define TYPE_ESC			30
#define TYPE_DEL			40
#define TYPE_EXCHANGE       50
#define TYPE_EXCHANGE_EX    60 // 금괴가 추가된 가격입력용. ///개인상점
#define TYPE_NOR_OK_LARGE	70
#define TYPE_NOR_OKCANCLE_LARGE	80
#define TYPE_NOR_ACCEPTREJECT	90 //기존 TYPE_NOR_OKCANCLE 과 같고 단지 수락, 거절 버튼으로 대체
#define TYPE_NOR_OKCANCELCHECK	100
#define TYPE_SHOUT			110

#define TYPE_GUILD_CREATE_PROMPT			200
#define TYPE_GUILD_CONFIRM					201
#define TYPE_GUILD_CREATE_NAME_INPUT		202
#define TYPE_GUILD_CONFIRM_CANCEL			203
#define TYPE_GUILD_RECOMMAND_JOIN			204
#define TYPE_GUILD_INVASION_CONFIRM_CANCEL	205
#define TYPE_GUILD_EXCHANGE					206

#define TYPE_WANTED_CONFIRM			300

#define TYPE_PREMIUM_CHA_MOVE_NAME_INPUT		400
#define TYPE_PREMIUM_CHA_MOVE_WAIT				401

#define TYPE_CARU_RESULT					500

// 스크롤 팝업창의 타입 정의
#define TYPE_REDISTRIBUTION	70
#define TYPE_MULTI_PG		60
#define TYPE_DELIV_BOX   	71
#define TYPE_HELP_BOX   	72//1128
#define TYPE_GUILD_NOTICE	73




// 단축키 지정하기..에쓰이는 상수..
//#define FUNCTION_NUM   7  // 사용할 단축키 개수
#define FUNCTION_NUM   8  // 1030
#define _TYPE_NONE   0
#define _TYPE_RSKILL 1
#define _TYPE_LSKILL 2
#define _STATE_LOCK   33 // 단축키 지정하기
#define _STATE_UNLOCK 44 // 단축키 해제하기
#define _STATE_CALL   55 // 단축키에해당하는 아이콘 호출하기

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_BPP      16

#define SPRITE_DIAMETER 48
#define NUM_SPRITES     25

#define WINDOWED_HELPTEXT   TEXT("Press Escape to quit.  Press Alt-Enter to switch to Full-Screen mode.")
#define FULLSCREEN_HELPTEXT TEXT("Press Escape to quit.  Press Alt-Enter to switch to Windowed mode.")


#define BACKGROUND_MSGBOX	"interface/back_msgbox.bmp"

// 스크롤 팝업 창에 쓰일 정의
#define _TYPE_SCROLL_OK			101
#define _TYPE_SCROLL_OKCANCLE	102
#define _TYPE_ONLY_INFO_OK		103
#define _TYPE_ONLY_INFO_OKCANCLE 104


//#define _FCOLOR_WHITE	RGB(152, 152, 152)
#define _FCOLOR_WHITE	RGB(192, 192, 192)
#define _FCOLOR_RED		RGB(223, 150, 152)
#define _FCOLOR_SKY		RGB(164, 219, 218)
#define _FCOLOR_YELLOW  RGB(231, 203, 102)
#define _FCOLOR_GREEN   RGB(140, 204, 138)

typedef struct _timeinfo
{
	int nYear;
	int nMon;
	int nDay;
	int nHour;
	int nMin;
	int nSec;

	_timeinfo()
	{
		nYear = -1;
		nMon = -1;
		nDay = -1;
		nHour = -1;
		nMin = -1;
		nSec= -1;
	}
	_timeinfo(int y, int mt, int d, int h, int m, int s)
	{
		nYear = y;
		nMon = mt;
		nDay = d;
		nHour = h;
		nMin = m;
		nSec= s;
	}
	_timeinfo& operator=(const _timeinfo& n)
	{
		nYear	= n.nYear;
		nMon	= n.nMon;
		nDay	= n.nDay;
		nHour	= n.nHour;
		nMin	= n.nMin;
		nSec	= n.nSec;

		return (*this);
	}
} TIME_INFO, t_Info;


#endif // _HEADERS_H_