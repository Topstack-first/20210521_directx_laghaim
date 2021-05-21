#ifndef _2D_RES_MGR_H_
#define _2D_RES_MGR_H_

#include "Config.h"

class CSurface;

// From  MsgPopUp..
typedef struct
{
	CSurface   *pBack_Nor;
	CSurface   *pBack_Esc;
	CSurface   *pBack_Input;
	CSurface   *pBack_PriceInput; ///개인상점
	CSurface   *pBack_Nor_Large; //글자수가 많을때 큰 매세지 박스
	CSurface   *pBack_Guild_Nor;
	CSurface   *pBack_Guild_Input;
	CSurface   *pBack_Guild_NameInput;
	CSurface   *pBack_Guild_RecoInput;
	CSurface   *pBack_Guild_Create;
	CSurface   *pBack_Wanted_Nor;
	CSurface   *pBack_Caru_Result;
	CSurface   *pBack_Shout;
} FROM_MSGBOX;

typedef struct
{
	CSurface	*pBack_guild_bk;
	CSurface	*pBack_guild_bk2;
	CSurface	*pBack_guild_admin_bk;
	CSurface	*pBack_guild_battle_bk;
} FROM_GUILD_COMMON_BACK;

typedef struct
{
	CSurface	*pBack_cha_move_name;
} FROM_PREMIUM;

typedef struct
{
	FROM_MSGBOX				msgbox;
	FROM_GUILD_COMMON_BACK	guild_back;
	FROM_PREMIUM			premium;


} RESOURCES_MGR;



class C2DResMgr
{

public:
	RESOURCES_MGR   m_sResMgr;

public:
	C2DResMgr();
	~C2DResMgr();

	void InitAll();
	void LoadResAll();
	void DeleteResAll();

public:

	CSurface* GetSurfaceFromMsgBox(char* img_name);

	CSurface* GetSurfaceFrom_guildback(char* img_name);
};

#endif // _2D_RES_MGR_H_
