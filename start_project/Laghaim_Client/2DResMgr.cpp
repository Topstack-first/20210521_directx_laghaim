#include "stdafx.h"

#include "Config.h"
#include "ddutil.h"
#include "2dresmgr.h"
#include "headers.h"

//


C2DResMgr::C2DResMgr()
{
	InitAll();
	LoadResAll();
}

C2DResMgr::~C2DResMgr()
{
	DeleteResAll();

	SAFE_DELETE(m_sResMgr.msgbox.pBack_Nor);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Esc);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Input);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_PriceInput); ///개인상점.
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Nor_Large);

	SAFE_DELETE(m_sResMgr.msgbox.pBack_Guild_Nor);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Guild_Input);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Guild_NameInput);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Guild_RecoInput);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Guild_Create);

	SAFE_DELETE(m_sResMgr.msgbox.pBack_Wanted_Nor);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Caru_Result);
	SAFE_DELETE(m_sResMgr.msgbox.pBack_Shout);

	SAFE_DELETE(m_sResMgr.guild_back.pBack_guild_bk);
	SAFE_DELETE(m_sResMgr.guild_back.pBack_guild_bk2);
	SAFE_DELETE(m_sResMgr.guild_back.pBack_guild_admin_bk);
	SAFE_DELETE(m_sResMgr.guild_back.pBack_guild_battle_bk);

	SAFE_DELETE(m_sResMgr.premium.pBack_cha_move_name);
}

void C2DResMgr::InitAll()
{
	m_sResMgr.msgbox.pBack_Nor			= NULL;
	m_sResMgr.msgbox.pBack_Esc			= NULL;
	m_sResMgr.msgbox.pBack_Input		= NULL;
	m_sResMgr.msgbox.pBack_PriceInput	= NULL; ///개인상점.
	m_sResMgr.msgbox.pBack_Nor_Large	= NULL;

	m_sResMgr.msgbox.pBack_Guild_Nor			= NULL;
	m_sResMgr.msgbox.pBack_Guild_Input			= NULL;
	m_sResMgr.msgbox.pBack_Guild_NameInput		= NULL;
	m_sResMgr.msgbox.pBack_Guild_RecoInput		= NULL;
	m_sResMgr.msgbox.pBack_Guild_Create			= NULL;

	m_sResMgr.guild_back.pBack_guild_bk			= NULL;
	m_sResMgr.guild_back.pBack_guild_bk2		= NULL;
	m_sResMgr.guild_back.pBack_guild_admin_bk	= NULL;
	m_sResMgr.guild_back.pBack_guild_battle_bk	= NULL;
	m_sResMgr.msgbox.pBack_Wanted_Nor			= NULL;
	m_sResMgr.msgbox.pBack_Caru_Result			= NULL;
	m_sResMgr.msgbox.pBack_Shout				= NULL;

	m_sResMgr.premium.pBack_cha_move_name		= NULL;
}

void C2DResMgr::LoadResAll()
{
	if( !g_pDisplay )
		return;

	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Nor,   "interface/msgbox/back_msgbox.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Esc,   "interface/msgbox/back_escback.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Input, "interface/msgbox/back_delback.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_PriceInput, "interface/msgbox/back_priceinput.bmp");  ///개인상점
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Nor_Large, "interface/msgbox/back_msgbox_large.bmp");

	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Guild_Nor		, "interface/guild/guild_msg/msg_bk.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Guild_Input	, "interface/guild/guild_msg/msg_input.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Guild_NameInput, "interface/guild/guild_msg/msg_guildname.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Guild_RecoInput, "interface/guild/guild_msg/msg_characname.bmp" );


	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Guild_Create	, "interface/guild/guild_msg/msg_guildmake.bmp" );

	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.guild_back.pBack_guild_bk	, "interface/guild/guild_common/guild_bk.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.guild_back.pBack_guild_bk2	, "interface/guild/guild_common/guildmember_bk.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.guild_back.pBack_guild_admin_bk	, "interface/guild/guild_common/guild_admin_bk.bmp" );
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.guild_back.pBack_guild_battle_bk, "interface/guild/guild_common/guild_battle_bk.bmp" );

	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Wanted_Nor, "interface/wanted/wanted_msg/message_bk.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Caru_Result, "interface/msgbox/back_takebox.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.msgbox.pBack_Shout, "interface/msgbox/back_shout.bmp");

	m_sResMgr.msgbox.pBack_Nor->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Esc->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Input->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_PriceInput->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Nor_Large->SetColorKey(TRANS_COLOR);

	m_sResMgr.msgbox.pBack_Guild_Nor->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Guild_Input->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Guild_NameInput->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Guild_RecoInput->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Guild_Create->SetColorKey(TRANS_COLOR);

	m_sResMgr.guild_back.pBack_guild_bk->SetColorKey(TRANS_COLOR);
	m_sResMgr.guild_back.pBack_guild_bk2->SetColorKey(TRANS_COLOR);
	m_sResMgr.guild_back.pBack_guild_admin_bk->SetColorKey(TRANS_COLOR);
	m_sResMgr.guild_back.pBack_guild_battle_bk->SetColorKey(TRANS_COLOR);

	m_sResMgr.msgbox.pBack_Wanted_Nor->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Caru_Result->SetColorKey(TRANS_COLOR);
	m_sResMgr.msgbox.pBack_Shout->SetColorKey(TRANS_COLOR);

	g_pDisplay->CreateSurfaceFromBitmap( &m_sResMgr.premium.pBack_cha_move_name, "interface/premium/chamove/char_exchage_msg.bmp");
	if( m_sResMgr.premium.pBack_cha_move_name )
	{
		m_sResMgr.premium.pBack_cha_move_name->SetColorKey(TRANS_COLOR);
	}	
}


void C2DResMgr::DeleteResAll()
{
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Nor);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Esc);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Input);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_PriceInput); ///개인상점.
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Nor_Large);

	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Guild_Nor);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Guild_Input);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Guild_NameInput);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Guild_RecoInput);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Guild_Create);

	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Wanted_Nor);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Caru_Result);
	SAFE_SURFACE_DESTROY(m_sResMgr.msgbox.pBack_Shout);

	SAFE_SURFACE_DESTROY(m_sResMgr.guild_back.pBack_guild_bk);
	SAFE_SURFACE_DESTROY(m_sResMgr.guild_back.pBack_guild_bk2);
	SAFE_SURFACE_DESTROY(m_sResMgr.guild_back.pBack_guild_admin_bk);
	SAFE_SURFACE_DESTROY(m_sResMgr.guild_back.pBack_guild_battle_bk);

	SAFE_SURFACE_DESTROY(m_sResMgr.premium.pBack_cha_move_name);
}

CSurface* C2DResMgr::GetSurfaceFromMsgBox(char* img_name)
{
	if(img_name == NULL)
		return NULL;

	static int Exgist = 0;

	if(strcmp(img_name, "back_guild_normal") == Exgist)
		return m_sResMgr.msgbox.pBack_Guild_Nor;

	if(strcmp(img_name, "back_guild_input") == Exgist)
		return m_sResMgr.msgbox.pBack_Guild_Input;

	if(strcmp(img_name, "back_guild_nameinput") == Exgist)
		return m_sResMgr.msgbox.pBack_Guild_NameInput;

	if(strcmp(img_name, "back_guild_recominput") == Exgist)
		return m_sResMgr.msgbox.pBack_Guild_RecoInput;

	if(strcmp(img_name, "back_guild_create") == Exgist)
		return m_sResMgr.msgbox.pBack_Guild_Create;

	if(strcmp(img_name, "back_wanted_normal") == Exgist)
		return m_sResMgr.msgbox.pBack_Wanted_Nor;

	if(strcmp(img_name, "back_msgbox") == Exgist)
		return m_sResMgr.msgbox.pBack_Nor;

	if(strcmp(img_name, "back_escback") == Exgist)
		return m_sResMgr.msgbox.pBack_Esc;

	if(strcmp(img_name, "back_delback") == Exgist)
		return m_sResMgr.msgbox.pBack_Input;

	if(strcmp(img_name, "back_priceinput") == Exgist) ///개인상점.
		return m_sResMgr.msgbox.pBack_PriceInput;

	if(strcmp(img_name, "back_msgbox_large") == Exgist) //큰 매세지 박스
		return m_sResMgr.msgbox.pBack_Nor_Large;

	if(strcmp(img_name, "back_cha_move_name") == Exgist) //큰 매세지 박스
		return m_sResMgr.premium.pBack_cha_move_name;

	if(strcmp(img_name, "back_takebox") == Exgist)
		return m_sResMgr.msgbox.pBack_Caru_Result;

	if(strcmp(img_name, "back_shout") == Exgist)
		return m_sResMgr.msgbox.pBack_Shout;

	return NULL;
}

CSurface* C2DResMgr::GetSurfaceFrom_guildback(char* img_name)
{
	if(img_name == NULL )
		return NULL;

	static int Exgist1 = 0;

	if(strcmp(img_name, "guild_back") == Exgist1)
		return m_sResMgr.guild_back.pBack_guild_bk;
	if(strcmp(img_name, "guild_back2") == Exgist1)
		return m_sResMgr.guild_back.pBack_guild_bk2;
	if(strcmp(img_name, "guild_back_admin") == Exgist1)
		return m_sResMgr.guild_back.pBack_guild_admin_bk;
	if(strcmp(img_name, "guild_back_battle") == Exgist1)
		return m_sResMgr.guild_back.pBack_guild_battle_bk;

	return NULL;
}
