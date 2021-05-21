#include "stdafx.h"
#include "headers.h"
#include "ControlGuildT.h"
#include "Country.h"
#include "2DResMgr.h"
#include "TextBoard.h"
#include "multilistbox.h"
#include "Tcpipcon.h"
#include "NkCharacter.h"
#include "time.h"
#include "WindowMgr.h"
#include "window_resource.h"
#include "g_stringmanager.h"



extern DWORD				g_dwClientCountry;





#define		_NONE			-1  // 처음의 상태
#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

#define		BOARD_MAX_ROW		3
#define		BOUNDARY_LINE_POSY_TOP	   (m_nBackPosY + 110)
#define		BOUNDARY_LINE_POSY_BOTTOM  (m_nBackPosY + 412)

#define		CLASS_CONTROL_IN_NAME	CControlGuild::CBunch

#define TEXTYPOS 60 + 85
#define TEXTIPOS_BLANK 41

extern	int InsertPriceComma( char *DestString, char *SrcString, char FindChar ); // 가격 스트링들의 숫자 부에 콤마를 넣어주는 함수. ///가격표시개선
extern	SVR_TYPE g_SvrType;

CControlGuild::CControlGuild()
	: m_nMoveInterval(0)
	, m_nMouseClickPos(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));

	// 변수초기화
	m_hWnd = NULL;

	m_vMemberIndexlist.clear();

	m_nMemberLine = 0;

	m_nBackPosX = 0;
	m_nBackPosY = 0;

	m_pGuildInfo = NULL;

	for(int i = 0; i < BACK_NUM; i++)
		m_pBack[i] = NULL;

	m_vMemberIndexlist.clear();

	for(int i = 0 ; i < GRADE_TOTAL; i++)
		m_pGuildManager[i] = NULL;

	m_revision_text.Init(12, RGB(255, 255, 0), 0, 0);

	m_GName.Init(  12, RGB(250, 235, 0), 0, 0);
	m_GLevel.Init( 12, RGB(255, 186, 0), 0, 0);
	m_GRank.Init(  12, RGB(255, 186, 0), 0, 0);
	m_GPoint.Init( 12, RGB(255, 186, 0), 0, 0);

	m_bRevisionDraw = FALSE;
	m_bRecom		= FALSE;

}

CControlGuild::~CControlGuild()
{
	DeleteRes();

	SAFE_DELETE(m_pGuildInfo);

	for(int i = 1; i < BACK_NUM; i++)
		SAFE_DELETE(m_pBack[i]);

	vector<CLASS_CONTROL_IN_NAME*>::iterator _iter;

	for(_iter = m_vMemberIndexlist.begin(); _iter != m_vMemberIndexlist.end(); _iter++)
	{
		CLASS_CONTROL_IN_NAME* pTemp = (*_iter);

		SAFE_DELETE(pTemp);
	}

	m_vMemberIndexlist.clear();

	for( int i = 0; i < GRADE_TOTAL; i++)
		SAFE_DELETE(m_pGuildManager[i]);

	m_pBack[BACK_COMMON] = NULL;
}

void CControlGuild::all_reset()
{
	vector<CLASS_CONTROL_IN_NAME*>::iterator _iter;

	for(_iter = m_vMemberIndexlist.begin(); _iter != m_vMemberIndexlist.end(); _iter++)
	{
		CLASS_CONTROL_IN_NAME* pTemp = (*_iter);

		SAFE_DELETE(pTemp);
	}

	m_vMemberIndexlist.clear();

	for(int i = 0; i < GRADE_TOTAL; i++)
		SAFE_DELETE(m_pGuildManager[i]);

	m_nMemberLine = 0;

	Set_btn_status(false,0);
}

void CControlGuild::LoadRes()
{
	if(!g_pDisplay)
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	//DeleteRes();
	m_pBack[BACK_COMMON] = NULL;

	for(int i = 1; i < BACK_NUM; i++)
		SAFE_DELETE(m_pBack[i]);

	m_Bts[BT_MANAGE].SetFileName("guild/mem_admin");
	m_Bts[BT_RECOMMENT].SetFileName("guild/recommend");
	m_Bts[BT_CLOSE].SetFileName("guild/close");
	m_Bts[BT_RECOMCANCEL].SetFileName("guild/request_cancel");
	m_Bts[BT_NOTICE].SetFileName("guild/notice");

	if( m_pBack[BACK_COMMON] == NULL)
	{
		m_pBack[BACK_COMMON] = pCMyApp->Get2DResMgr()->GetSurfaceFrom_guildback("guild_back2");

		if( m_pBack[BACK_COMMON] )
		{
			// scroll
			m_Scroll.LoadRes("guild/scr_up", "guild/scr_down", "interface/guild/scr_bar.bmp");
			m_Scroll.SetRange(1);

			g_pDisplay->CreateSurfaceFromBitmap(&m_pGuildInfo,					"interface/guild/guild_member/guild_info.bmp");

			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_MASTER],			"interface/guild/guild_member/mem_master_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_ASSIST_MASTER],	"interface/guild/guild_member/mem_submaster_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_BULKAN],			"interface/guild/guild_member/mem_bulkan_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_HUMAM	],			"interface/guild/guild_member/mem_human_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_AIDA],			"interface/guild/guild_member/mem_aidia_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_KAI],				"interface/guild/guild_member/mem_kailipton_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_HYBRID],			"interface/guild/guild_member/mem_hybrider_bk.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_PEROM],			"interface/guild/guild_member/MEM_PEROM_BK.bmp");			 
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_MEMBER],			"interface/guild/guild_member/mem_member.bmp");
			g_pDisplay->CreateSurfaceFromBitmap(&m_pBack[BACK_MEMBER_BK],		"interface/guild/guild_member/member_bk.bmp", 229, BOUNDARY_LINE_POSY_BOTTOM - BOUNDARY_LINE_POSY_TOP);

			for(int i = 0; i < BT_NUM; i++)
			{
				m_Bts[i].LoadRes();
				m_Bts[i].SetDisable( false );
			}

			m_revision_text.SetPos(m_nBackPosX+34, m_nBackPosY+21);

			SetPosition();

		}
	}
}

void CControlGuild::Insert_list(guild_member& g_member)
{
	if(g_member.s_glevel == -1 || g_member.s_glevel == 0)
		return;

	switch(g_member.s_glevel)
	{
	case JOB_GENERIC :
		Insert_member(g_member);
		break;
	case JOB_PEROM :
		Insert_perom(g_member);
		break;
	case JOB_HIV	 :
		Insert_hybrid(g_member);
		break;
	case JOB_HUMAN   :
		Insert_human(g_member);
		break;
	case JOB_EIDA    :
		Insert_aida(g_member);
		break;
	case JOB_KAI	 :
		Insert_kai(g_member);
		break;
	case JOB_VULCAN	 :
		Insert_vulkan(g_member);
		break;
	case JOB_ASSISTANTMASTER :
		Insert_assistmaster(g_member);
		break;
	case JOB_MASTER :
		Insert_master(g_member);
		break;
	}
}

void CControlGuild::CheckData()
{
	CGuildData* pGuildData = g_pRoh->GetMyGuildData();

	if(!pGuildData)	return;

	pGuildData->Member_list_check();

	if(!pGuildData->GetGeneralMember().size() && pGuildData->GetCaptainMember().size())
		return;

	vector<CBunch*>::iterator _iter    = m_vMemberIndexlist.begin();

	list_guildmember_ptr::iterator _iter_  = pGuildData->GetGeneralMember().begin();

	for(; _iter_ != pGuildData->GetGeneralMember().end(); _iter_++)
	{
		guild_member* member = *_iter_;

		if(member->s_check == CHECK_ADD)
		{
			Insert_list(*member);

			member->s_check = CHECK_OK;
		}
		else if(member->s_check == CHECK_UPDATE)
		{
			Update_list(*member);

			member->s_check = CHECK_OK;
		}
	}
}

void CControlGuild::Restore()
{

}

void CControlGuild::Refresh_list(guild_member& g_member)
{
	int nWho = 0;

	if(g_member.s_glevel == -1)	// 삭제
	{
		if(nWho = Delete_list(g_member))
		{
			SetReposition(nWho);
		}
	}
	else
	{
		if(!Update_list(g_member))
			Insert_list(g_member);
	}

}

void CControlGuild::UpdateData(guild_member& g_member)
{
	if(!Update_list(g_member))
		Insert_list(g_member);
}

BOOL CControlGuild::SetReposition(int nWho)
{
	if(nWho == 2)
	{
		m_nMemberLine = 0;

		vector<CBunch*>::iterator _iter_;

		m_nMemberLine = 0;

		_iter_ = m_vMemberIndexlist.begin();

		for(; _iter_ != m_vMemberIndexlist.end(); _iter_++, m_nMemberLine++)
		{
			CBunch* pTemp = *_iter_;

			pTemp->SetRePosition(m_nMemberLine, TEXTYPOS + TEXTIPOS_BLANK*8 + m_nMemberLine*26);
		}
	}
	return TRUE;
}

int CControlGuild::Delete_list(guild_member& g_member)
{
	for(int i = 0 ; i < GRADE_TOTAL; i++)
	{
		if(m_pGuildManager[i] && m_pGuildManager[i]->m_nCharidx == g_member.s_index )
		{
			if(Delete_captain(m_pGuildManager[i]))
			{
				//  [6/29/2009 ppmmjj83] 길드내 직책있는 캐릭터 탈퇴시 null값이 않되어 팅기는 문제 수정
				m_pGuildManager[i] = NULL;
				return 1;
			}
		}
	}

	vector<CBunch*>::iterator _iter_;

	int nSize = m_vMemberIndexlist.size();

	for(_iter_ = m_vMemberIndexlist.begin(); _iter_ != m_vMemberIndexlist.end(); _iter_++)
	{
		CBunch* pTemp = *_iter_;

		if(pTemp && pTemp->m_nCharidx == g_member.s_index)
		{
			Delete_member(pTemp);

			m_vMemberIndexlist.erase(_iter_);

			m_nMemberLine--;

			SetReposition(2);

			return 2;
		}
	}

	return 0;
}

BOOL CControlGuild::Delete_captain(CBunch* g_member)
{
	SAFE_DELETE(g_member);

	return TRUE;
}

BOOL CControlGuild::Delete_member(CBunch* g_member)
{
	SAFE_DELETE(g_member);
	return TRUE;
}

BOOL CControlGuild::Update_list(guild_member& g_member)
{
	// 갱신은 지웠다가 다시 생성한다..귀찮아서 갱신 없음
	int nWho = 0;
	if(nWho = Delete_list(g_member))
	{
		SetReposition(nWho);
		Insert_list(g_member);

		return TRUE;
	}

	return FALSE;
}


void CControlGuild::Set_btn_status(BOOL bEnable, int nLevel)
{

	m_Bts[BT_RECOMCANCEL].SetDisable(true);
	m_Bts[BT_RECOMCANCEL].SetState(BTN_DISABLE);

	if(bEnable && nLevel == JOB_MASTER ) // 길드 권리 권한을 가지고 있다.
	{
		m_Bts[BT_MANAGE].SetDisable( false );
		m_Bts[BT_MANAGE].SetState(BTN_NORMAL);

		m_Bts[BT_RECOMMENT].SetDisable( false );
		m_Bts[BT_RECOMMENT].SetState(BTN_NORMAL);

		m_Bts[BT_NOTICE].SetDisable2( false );
		m_Bts[BT_NOTICE].SetState(BTN_DISABLE);
	}
	else if(bEnable &&  nLevel == JOB_ASSISTANTMASTER ) // 길드 권리 권한을 가지고 있다.
	{
		m_Bts[BT_MANAGE].SetDisable( false );
		m_Bts[BT_MANAGE].SetState(BTN_NORMAL);

		m_Bts[BT_RECOMMENT].SetDisable( false );
		m_Bts[BT_RECOMMENT].SetState(BTN_NORMAL);

		m_Bts[BT_NOTICE].SetDisable( false );
		m_Bts[BT_NOTICE].SetState(BTN_NORMAL);
	}
	else if( bEnable && (nLevel == JOB_HIV || nLevel == JOB_HUMAN || nLevel == JOB_EIDA
						 || nLevel ==  JOB_KAI || nLevel == JOB_VULCAN || nLevel == JOB_PEROM) )
	{
		m_Bts[BT_MANAGE].SetDisable2( true );
		m_Bts[BT_MANAGE].SetState(BTN_DISABLE);

		m_Bts[BT_RECOMMENT].SetDisable( false );
		m_Bts[BT_RECOMMENT].SetState(BTN_NORMAL);

		m_Bts[BT_NOTICE].SetDisable( false );
		m_Bts[BT_NOTICE].SetState(BTN_NORMAL);
	}
	else if(bEnable && (nLevel == JOB_GENERIC ))
	{
		m_Bts[BT_MANAGE].SetDisable2( true );
		m_Bts[BT_MANAGE].SetState(BTN_DISABLE);

		m_Bts[BT_RECOMMENT].SetDisable2( true );
		m_Bts[BT_RECOMMENT].SetState(BTN_DISABLE);

		m_Bts[BT_NOTICE].SetDisable( false );
		m_Bts[BT_NOTICE].SetState(BTN_NORMAL);

	}
	else if( m_bRecom )
	{
		m_Bts[BT_MANAGE].SetDisable2( true );
		m_Bts[BT_MANAGE].SetState(BTN_DISABLE);

		m_Bts[BT_RECOMMENT].SetDisable2( true );
		m_Bts[BT_RECOMMENT].SetState(BTN_DISABLE);

		m_Bts[BT_NOTICE].SetDisable2( true );
		m_Bts[BT_NOTICE].SetState(BTN_DISABLE);

		m_Bts[BT_RECOMCANCEL].SetDisable(false);
		m_Bts[BT_RECOMCANCEL].SetState(BTN_NORMAL);
	}
	else
	{
		m_Bts[BT_MANAGE].SetDisable2( true );
		m_Bts[BT_MANAGE].SetState(BTN_DISABLE);

		m_Bts[BT_RECOMMENT].SetDisable2( true );
		m_Bts[BT_RECOMMENT].SetState(BTN_DISABLE);

		m_Bts[BT_NOTICE].SetDisable2( true );
		m_Bts[BT_NOTICE].SetState(BTN_DISABLE);
	}

}

void CControlGuild::SetPosition()
{
	m_nBackPosX = m_ClientRc.right - m_pBack[BACK_COMMON]->GetWidth();
	m_nBackPosY = 29;

	m_pGuildInfo->Xpos = m_nBackPosX + 32;
	m_pGuildInfo->Ypos = m_nBackPosY + 26;

	int nHeight = 0;

	int temp = m_pGuildInfo->Ypos + m_pGuildInfo->GetHeight();

	m_Scroll.SetDrawPos( m_nBackPosX + 248 , temp + 10  , 305);

	for(int i = 1; i < BACK_NUM - 1; i++, nHeight += 41)
	{
		m_pBack[i]->Xpos = m_nBackPosX + 17;
		m_pBack[i]->Ypos = m_nBackPosY + temp + nHeight;
	}

	m_pBack[BACK_MEMBER_BK]->Xpos = m_nBackPosX + 17;
	m_pBack[BACK_MEMBER_BK]->Ypos = m_pBack[BACK_MEMBER]->Ypos + 19;

	m_Bts[BT_MANAGE].SetPosition( m_nBackPosX + 13, m_nBackPosY + 459);
	m_Bts[BT_RECOMCANCEL].SetPosition( m_nBackPosX + 13, m_nBackPosY + 459);
	m_Bts[BT_RECOMMENT].SetPosition(m_Bts[BT_RECOMCANCEL].PosX + m_Bts[BT_RECOMCANCEL].Width + 2, m_nBackPosY + 459);
	m_Bts[BT_NOTICE].SetPosition(m_Bts[BT_RECOMMENT].PosX + m_Bts[BT_RECOMMENT].Width+ 2, m_nBackPosY + 459);

	m_Bts[BT_CLOSE].SetPosition(m_nBackPosX + 259, m_nBackPosY + 4);

	m_GName.SetPos( m_pGuildInfo->Xpos + 53,  m_pGuildInfo->Ypos + 12 );
	m_GLevel.SetPos(m_pGuildInfo->Xpos + 54,  m_pGuildInfo->Ypos + 32 );
	m_GRank.SetPos( m_pGuildInfo->Xpos + 150, m_pGuildInfo->Ypos + 32 );
	m_GPoint.SetPos(m_pGuildInfo->Xpos + 53,  m_pGuildInfo->Ypos + 52 );
}

void CControlGuild::Draw()
{
	if( !g_pDisplay )
		return;

	int i = 0;
	if( m_pBack[BACK_COMMON] )
	{
		g_pDisplay->Blt(m_nBackPosX, m_nBackPosY, m_pBack[BACK_COMMON]);
		g_pDisplay->Blt(m_pGuildInfo->Xpos, m_pGuildInfo->Ypos, m_pGuildInfo );

		DrawGuildInfo();

		if( m_bRevisionDraw)
			m_revision_text.Draw();

		if( m_bRecom )
		{
			m_Bts[BT_RECOMCANCEL].Draw();
			m_Bts[BT_CLOSE].Draw(m_nBackPosX,m_nBackPosY);
		}
		else
		{
			for( i = 0; i < BT_NUM; i++)
			{
				if( i != BT_RECOMCANCEL )
					m_Bts[i].Draw();
			}
		}

		for( i = 1; i < BACK_NUM - 1; i++)
		{
			DrawToBlt(m_pBack[i]);
		}

		DrawToBltBlank(m_pBack[BACK_MEMBER_BK]);

		for(i = 0; i < GRADE_TOTAL; i++)
		{
			if(m_pGuildManager[i])
				m_pGuildManager[i]->Draw(m_Scroll.GetPos());
		}

		vector<CLASS_CONTROL_IN_NAME*>::iterator _iter;

		for(_iter = m_vMemberIndexlist.begin(); _iter != m_vMemberIndexlist.end(); _iter++)
		{
			CBunch* pTemp = *_iter;

			pTemp->Draw(m_Scroll.GetPos());
		}

		m_Scroll.SetRange(m_pBack[BACK_COMMON]->GetHeight() + (m_nMemberLine) * 20);
		m_Scroll.Draw();

	}
}


void CControlGuild::SetGuildInfo()
{
	CGuildData* pGData = g_pRoh->GetMyGuildData();

	if( pGData == NULL )
		return;

	char temp[256] = {0,};

	// 길드 이름
	sprintf( temp, "%s", pGData->GetMyGuildInfo().s_guildName );
	m_GName.SetString(temp);

	// 길드 레벨
	m_GLevel.SetString( g_pNk2DFrame->GetWindowRes()->GetGuildLevel(pGData->GetMyGuildInfo().s_glevel-1) );

	// 길드 랭킹
	sprintf( temp, "%d", pGData->GetMyGuildInfo().s_rank);
	m_GRank.SetString(temp);

	// 길드 점수
	char temp2[128] = {0,};
	sprintf( temp, "%d", pGData->GetMyGuildInfo().s_gPoint );
	InsertPriceComma( temp2, temp, '-' ); // 콤마를 넣은 스트링으로 바꾼다.
	// IDS_GUILD_POINT "%s Point"
	sprintf( temp, (char *)G_STRING(IDS_GUILD_POINT), temp2);
	m_GPoint.SetString(temp);

	// 길드 리스트 갱신

}

void CControlGuild::ChangeGuildInfo(char* guild_name)
{
	CGuildData* pGData = g_pRoh->GetMyGuildData();
	if( pGData == NULL)
		return;

	//일단은 길드 이름만 바꾼다. 추후에 필요한 내용이 있으면 추가한다.
	strcpy(pGData->GetMyGuildInfo().s_guildName, guild_name);

	SetGuildInfo();
}

void CControlGuild::DrawGuildInfo()
{
	CGuildData* pGData = g_pRoh->GetMyGuildData();

	if( pGData == NULL && !m_bRecom )
		return;

	if( m_bRecom )
	{
		m_GName.Draw();		// 길드 이름
		m_GPoint.Draw();	// 길드 점수
	}
	else
	{
		m_GName.Draw();		// 길드 이름
		m_GLevel.Draw();	// 길드 레벨
		m_GRank.Draw();		// 길드 랭킹
		m_GPoint.Draw();	// 길드 점수
	}
}


void CControlGuild::set_revision(t_Info time)
{
	if( time.nYear	== -1 && time.nMon	== -1 && time.nDay	== -1 &&
			time.nHour	== -1 && time.nMin	== -1 && time.nSec	== -1)
	{
//		m_bRevisionDraw = FALSE;
//		return;
	}

	m_bRevisionDraw = TRUE;

	char szTmp[256] = {0,};

	if( time.nMon != -1 )
	{
		// IDS_GUILD_LVL_DOWN_D_DAY		"길드레벨 하락 예정일 %02d/%02d %02d:%02d"
		sprintf(szTmp, (char *)G_STRING(IDS_GUILD_LVL_DOWN_D_DAY), time.nMon, time.nDay, time.nHour, time.nMin);
	}

	m_revision_text.SetString(szTmp );

	m_revision_text.SetPos(m_nBackPosX + ( m_pBack[BACK_COMMON]->GetWidth() - m_revision_text.GetBoxWidth() ) / 2 ,
						   m_nBackPosY + 424 );
}


BOOL CControlGuild::DrawToBlt(CSurface* pSurface)
{
	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = pSurface->GetWidth();
	src.bottom = pSurface->GetHeight();

	dest.left   = pSurface->Xpos;
	dest.top    = pSurface->Ypos;
	dest.right  = pSurface->Xpos + pSurface->GetWidth();
	dest.bottom = pSurface->Ypos + pSurface->GetHeight();

	if(dest.top - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_TOP &&
			dest.bottom - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_TOP)	// 위쪽  밖에 있음..
	{
		return FALSE;
	}
	else if(dest.top - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_TOP &&		// 윗 영역에 걸쳤음.
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_TOP)
	{

		src.top  = abs((dest.top - m_Scroll.GetPos()) - BOUNDARY_LINE_POSY_TOP );
		dest.top = BOUNDARY_LINE_POSY_TOP;
		dest.bottom = (dest.bottom - m_Scroll.GetPos());
	}
	else if(dest.top - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_TOP &&		// 모두 영역 안에 있음.
			dest.bottom - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_BOTTOM/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.top    = pSurface->Ypos - m_Scroll.GetPos();
		dest.bottom = dest.top + pSurface->GetHeight();

	}
	else if(dest.top - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_BOTTOM &&
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_BOTTOM)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( BOUNDARY_LINE_POSY_BOTTOM - (dest.top - m_Scroll.GetPos()) );
		dest.top    = (dest.top- m_Scroll.GetPos());
		dest.bottom = BOUNDARY_LINE_POSY_BOTTOM;
	}
	else if(dest.top - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_BOTTOM &&
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_BOTTOM) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, pSurface, &src);


	return TRUE;
}

BOOL CControlGuild::DrawToBltBlank(CSurface* pSurface)
{
	RECT src, dest;

	src.left = 0;
	src.top  = 0;
	src.right  = pSurface->GetWidth();
	src.bottom = pSurface->GetHeight();

	dest.left   = pSurface->Xpos;
	dest.top    = pSurface->Ypos;
	dest.right  = pSurface->Xpos + pSurface->GetWidth();
	dest.bottom = pSurface->Ypos + pSurface->GetHeight();

	if(dest.top - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_TOP &&
			dest.bottom - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_TOP)		// 위쪽  밖에 있음..
	{
		src.top  = 0;
		dest.top = BOUNDARY_LINE_POSY_TOP;
		dest.bottom = BOUNDARY_LINE_POSY_BOTTOM;
	}
	else if(dest.top - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_TOP &&		// 윗 영역에 걸쳤음.
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_TOP)
	{
		src.top  = 0;
		dest.top = BOUNDARY_LINE_POSY_TOP;
		dest.bottom = BOUNDARY_LINE_POSY_BOTTOM;
	}
	else if(dest.top - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_TOP &&		// 모두 영역 안에 있음.
			dest.bottom - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_BOTTOM/*BOUNDARY_LINE_POSY_TOP*/)
	{
		dest.top    = pSurface->Ypos - m_Scroll.GetPos();
		dest.bottom = dest.top + pSurface->GetHeight();

	}
	else if(dest.top - m_Scroll.GetPos() <= BOUNDARY_LINE_POSY_BOTTOM &&
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_BOTTOM)	// 아랫 영역에 걸쳤음
	{
		src.bottom  = abs( BOUNDARY_LINE_POSY_BOTTOM - (dest.top - m_Scroll.GetPos()) );
		dest.top    = (dest.top- m_Scroll.GetPos());
		dest.bottom = BOUNDARY_LINE_POSY_BOTTOM;
	}
	else if(dest.top - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_BOTTOM &&
			dest.bottom - m_Scroll.GetPos() >= BOUNDARY_LINE_POSY_BOTTOM) // 아랫영역 밖으로 모두 나갔삼.
	{
		return FALSE;
	}

	g_pDisplay->Blt(dest.left, dest.top, dest.right, dest.bottom, pSurface, &src);


	return TRUE;
}

void CControlGuild::DeleteRes()
{
	SAFE_DELETE(m_pGuildInfo);

	// 배경화면
	for(int i = 1; i < BACK_NUM; i++)
		SAFE_DELETE(m_pBack[i]);

	for( int i = 0; i < BT_NUM; i++)
		m_Bts[i].DeleteRes();

	m_Scroll.DeleteRes();
	m_revision_text.DeleteRes();
}

LRESULT CControlGuild::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if(!IsInside(LOWORD(lParam), HIWORD(lParam)))
		return 0;

	for(int i = 0; i < BT_NUM; i++)
		m_Bts[i].MsgProc(hWnd, msg, wParam, lParam);

	m_Scroll.MsgProc(hWnd, msg, wParam, lParam);

	switch ( msg )
	{
	case WM_LBUTTONDOWN :
		{
		}
		return 1;
	case WM_MOUSEMOVE:
		{
		}
		return 1;
	case WM_LBUTTONUP:
		{
			if( m_bRecom )
			{

				if(m_Bts[BT_CLOSE].GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->ShowControlGuild(FALSE);
					m_Bts[BT_CLOSE].SetState(BTN_NORMAL);
				}

				if(m_Bts[BT_RECOMCANCEL].GetState() == BTN_ACTION)
				{
					g_pTcpIp->SendNetMessage("mg rinfo\n");
					m_Bts[BT_RECOMCANCEL].SetState(BTN_NORMAL);
				}
			}
			else
			{
				if(m_Bts[BT_MANAGE].GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->ToggleControlGuildManageWindow();
				}

				if(m_Bts[BT_RECOMMENT].GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->InsertPopup(" ", TYPE_GUILD_RECOMMAND_JOIN, POPUP_GUILD_RECOM_MEMBER);
				}

				if(m_Bts[BT_CLOSE].GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->ShowControlGuild(FALSE);
					m_Bts[BT_CLOSE].SetState(BTN_NORMAL);
				}

				if(m_Bts[BT_NOTICE].GetState() == BTN_ACTION)
				{
					g_pNk2DFrame->GetUIWindow()->OnOpenWindow(WINDOW_GUILD_MEMBER_NOTICE);
				}
			}
		}
		return 1;
	}


	return 0;
}

LRESULT CControlGuild::MsgPopUpProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg, int nProcess, char* szString)
{
	return 0;
}

void CControlGuild::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

BOOL CControlGuild::IsInside(int x, int y)
{
	if(!m_pBack)
		return FALSE;

	if( x >= m_nBackPosX
			&& x < m_nBackPosX + m_pBack[BACK_COMMON]->GetWidth()
			&& y >= m_nBackPosY
			&& y < m_nBackPosY + m_pBack[BACK_COMMON]->GetHeight() )
		return TRUE;

	return FALSE;
}


void CControlGuild::Insert_master(guild_member& g_member)
{
	m_pGuildManager[MASTER]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[MASTER]->Init();
	m_pGuildManager[MASTER]->LoadRes();
	m_pGuildManager[MASTER]->SetPosition(0, TEXTYPOS + TEXTIPOS_BLANK*0, 0);

	m_pGuildManager[MASTER]->SetCharName(g_member.s_name);
	m_pGuildManager[MASTER]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[MASTER]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[MASTER]->SetCharIdx(g_member.s_index);
}
void CControlGuild::Insert_assistmaster(guild_member& g_member)
{
	m_pGuildManager[ASSISTMASTER]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[ASSISTMASTER]->Init();
	m_pGuildManager[ASSISTMASTER]->LoadRes();
	m_pGuildManager[ASSISTMASTER]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*1, 1);

	m_pGuildManager[ASSISTMASTER]->SetCharName(g_member.s_name);
	m_pGuildManager[ASSISTMASTER]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[ASSISTMASTER]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[ASSISTMASTER]->SetCharIdx(g_member.s_index);
}
void CControlGuild::Insert_vulkan(guild_member& g_member)
{
	m_pGuildManager[VULKAN]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[VULKAN]->Init();
	m_pGuildManager[VULKAN]->LoadRes();
	m_pGuildManager[VULKAN]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*2, 2);

	m_pGuildManager[VULKAN]->SetCharName(g_member.s_name);
	m_pGuildManager[VULKAN]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[VULKAN]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[VULKAN]->SetCharIdx(g_member.s_index);
}
void CControlGuild::Insert_human(guild_member& g_member)
{
	m_pGuildManager[HUMAN]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[HUMAN]->Init();
	m_pGuildManager[HUMAN]->LoadRes();
	m_pGuildManager[HUMAN]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*3, 3);

	m_pGuildManager[HUMAN]->SetCharName(g_member.s_name);
	m_pGuildManager[HUMAN]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[HUMAN]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[HUMAN]->SetCharIdx(g_member.s_index);
}
void CControlGuild::Insert_aida(guild_member& g_member)
{
	m_pGuildManager[AIDA]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[AIDA]->Init();
	m_pGuildManager[AIDA]->LoadRes();
	m_pGuildManager[AIDA]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*4, 4);

	m_pGuildManager[AIDA]->SetCharName(g_member.s_name);
	m_pGuildManager[AIDA]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[AIDA]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[AIDA]->SetCharIdx(g_member.s_index);
}
void CControlGuild::Insert_kai(guild_member& g_member)
{
	m_pGuildManager[KAI]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[KAI]->Init();
	m_pGuildManager[KAI]->LoadRes();
	m_pGuildManager[KAI]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*5, 5);

	m_pGuildManager[KAI]->SetCharName(g_member.s_name);
	m_pGuildManager[KAI]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[KAI]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[KAI]->SetCharIdx(g_member.s_index);
}
void CControlGuild::Insert_hybrid(guild_member& g_member)
{
	m_pGuildManager[HYBRID]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[HYBRID]->Init();
	m_pGuildManager[HYBRID]->LoadRes();
	m_pGuildManager[HYBRID]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*6, 6);

	m_pGuildManager[HYBRID]->SetCharName(g_member.s_name);
	m_pGuildManager[HYBRID]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[HYBRID]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[HYBRID]->SetCharIdx(g_member.s_index);
}

void CControlGuild::Insert_perom(guild_member& g_member)
{
	m_pGuildManager[PEROM]  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	m_pGuildManager[PEROM]->Init();
	m_pGuildManager[PEROM]->LoadRes();
	m_pGuildManager[PEROM]->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*7, 7);

	m_pGuildManager[PEROM]->SetCharName(g_member.s_name);
	m_pGuildManager[PEROM]->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	m_pGuildManager[PEROM]->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	m_pGuildManager[PEROM]->SetCharIdx(g_member.s_index);
}

void CControlGuild::Insert_member(guild_member& g_member)
{
	CLASS_CONTROL_IN_NAME* pTemp  = CLASS_CONTROL_IN_NAME::Create(m_nBackPosX, m_nBackPosY);

	pTemp->Init();
	pTemp->LoadRes();
	pTemp->SetPosition(0,  TEXTYPOS + TEXTIPOS_BLANK*8 + m_nMemberLine*26, m_nMemberLine);

	pTemp->SetCharName(g_member.s_name);
	pTemp->SetRaceName(g_pNk2DFrame->GetWindowRes()->GetRaceShortName(g_member.s_race));
	pTemp->SetZoneName(g_pNk2DFrame->GetWindowRes()->GetZoneName(g_member.s_connect));

	pTemp->SetCharIdx(g_member.s_index);

	m_Scroll.SetRange(m_nMemberLine*28);

	m_nMemberLine++;

	m_vMemberIndexlist.push_back(pTemp);
}

void CControlGuild::SetRecomInfo(BOOL bRecom, char* GuildName /* = NULL */)
{
	m_bRecom = bRecom;

	if( m_bRecom )
	{
		m_GName.SetString(GuildName);
		// IDS_GUILD_RECOM_ING "길드 가입 요청중"s
		m_GPoint.SetString((char*)G_STRING(IDS_GUILD_RECOM_ING));

		m_Bts[BT_RECOMCANCEL].SetDisable(false);
		m_Bts[BT_RECOMCANCEL].SetState(BTN_NORMAL);
	}
	else
	{
		m_GName.SetString("");
		m_GPoint.SetString("");

		m_Bts[BT_RECOMCANCEL].SetDisable(true);
		m_Bts[BT_RECOMCANCEL].SetState(BTN_NORMAL);
	}
}



/*****************************************************************************************************************/
CLASS_CONTROL_IN_NAME*	CLASS_CONTROL_IN_NAME::Create(int nBaseX, int nBaseY)
{
	return new CBunch(nBaseX, nBaseY);
}

CLASS_CONTROL_IN_NAME::CBunch(int nBaseX, int nBaseY)
{
	m_hWnd		= NULL;

	m_nBackPosX = nBaseX;
	m_nBackPosY = nBaseY;

	m_nSequence   = 0;

	ZeroMemory(m_MsgBuf, sizeof(m_MsgBuf));
}

CLASS_CONTROL_IN_NAME::~CBunch()
{
	DeleteRes();
}

void CLASS_CONTROL_IN_NAME::Init()
{

}

void CLASS_CONTROL_IN_NAME::LoadRes()
{
	if(!g_pDisplay)	return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);
}

void CLASS_CONTROL_IN_NAME::Draw(int nScrollPos)
{
	if(!g_pDisplay)	return;

	for(int i = 0; i < CControlGuild::TEXT_TOTAL; i++)
		m_CharInfoText[i].DrawClipText(m_CharInfoText[i].m_StringPosY, BOUNDARY_LINE_POSY_TOP, BOUNDARY_LINE_POSY_BOTTOM, nScrollPos);
}

void CLASS_CONTROL_IN_NAME::DeleteRes()
{
	for(int i = 0; i < CControlGuild::TEXT_TOTAL; i++)
		m_CharInfoText[i].DeleteRes();
}

void CLASS_CONTROL_IN_NAME::SetCharName(char* pName)
{
	m_CharInfoText[CControlGuild::TEXT_CHAR].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_CONTROL_IN_NAME::SetRaceName(char* pName)
{
	m_CharInfoText[CControlGuild::TEXT_RACE].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_CONTROL_IN_NAME::SetZoneName(char* pName)
{
	m_CharInfoText[CControlGuild::TEXT_ZONE].SetString_Center_Arrange(pName, RGB(0, 255, 64), RGB(255, 255, 255), TRUE);
}

void CLASS_CONTROL_IN_NAME::SetPosition(int nPosX, int nPosY, int nNum)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_CharInfoText[CControlGuild::TEXT_CHAR].Init(12, RGB(255, 255, 255), m_nBackPosX + 17 , m_nBackPosY + nPosY, 128, 30, TRUE);
	m_CharInfoText[CControlGuild::TEXT_RACE].Init(12, RGB(255, 255, 255), m_nBackPosX + 147, m_nBackPosY + nPosY,  32, 30, TRUE);
	m_CharInfoText[CControlGuild::TEXT_ZONE].Init(12, RGB(255, 255, 255), m_nBackPosX + 177, m_nBackPosY + nPosY,  67, 30, TRUE);
}

void CLASS_CONTROL_IN_NAME::SetRePosition(int nNum, int nPosY)
{
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_nSequence = nNum;

	m_CharInfoText[CControlGuild::TEXT_CHAR].SetPos_Center(m_nBackPosX + 17 , m_nBackPosY + nPosY);
	m_CharInfoText[CControlGuild::TEXT_RACE].SetPos_Center(m_nBackPosX + 147, m_nBackPosY + nPosY);
	m_CharInfoText[CControlGuild::TEXT_ZONE].SetPos_Center(m_nBackPosX + 177, m_nBackPosY + nPosY);
}

void CLASS_CONTROL_IN_NAME::Restore()
{

}

void CLASS_CONTROL_IN_NAME::SendMsg()
{
	g_pTcpIp->SendNetMessage(m_MsgBuf);
}

LRESULT CLASS_CONTROL_IN_NAME::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nScroll)
{

	return -1;
}
