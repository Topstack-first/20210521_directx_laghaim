#include "stdafx.h"
#define D3D_OVERLOADS
#include <boost/format.hpp>
#include <ddraw.h>
#include <d3d.h>
#include <direct.h>//1115
#include <ctime>
#include <boost/format.hpp>
#include "LH_SelectCharacterPage.h"
#include "headers.h"
#include "Sheet.h"
#include "createcharacterpage.h"

#include "ProgressBox.h"
#include "TextOutBox.h"
#include "Tcpipcon.h"
#include "main.h"
#include "direct.h"
#include "skill.h"
#include "FxSet.h"
#include "DirtSnd.h"
#include "Loginpage.h"
#include "Country.h"
#include "g_stringmanager.h"
#include "WebWorld.h"


extern BOOL g_bTestServer;
extern BOOL g_bFinishFxCharWave;

extern int		g_MatrixSvr;
extern BOOL     g_bMatrixMatch;
extern DWORD	g_dwClientCountry;

#define	GOSTART		1
#define GOCREATE	2
#define DELETECHAR	3
#define GOLOGIN		4

LH_SelectCharacterPage::LH_SelectCharacterPage()
	: m_pSheet(NULL)
	, m_selected_slot(0)
	, m_UserIndex(0)
	, m_MeterRateTime(0)
{
	memset(m_strFlatRateTime, 0, sizeof(m_strFlatRateTime));

	m_pFlatRateText = NULL;
	m_pMeterRateText = NULL;

}

LH_SelectCharacterPage::LH_SelectCharacterPage(CSheet* pSheet)
	: m_pSheet(pSheet)
{
	m_pFlatRateText = NULL;
	m_pMeterRateText = NULL;
	m_selected_slot = -1;

	Init();
}

LH_SelectCharacterPage::~LH_SelectCharacterPage(void)
{
	DeleteRes();
}

void LH_SelectCharacterPage::Action( int i )
{
	if (g_pDSound)
	{
		g_pDSound->Play(g_SheetSound[SHEET_SND_CLICK]);
	}
	switch ( i )
	{
	case GOSTART:
		{
			m_pSheet->StartGame(START_GAME);
			strcpy(pCMyApp->m_SaveChaName, m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_Name);
			pCMyApp->m_bSelectCha=TRUE;

			char path[1024];
			char *strPath;
			strPath = pCMyApp->GetExePath();
			_chdir(strPath);
			sprintf(path, "%s/lag_lcg/", strPath);


			FILE *stream;
			int _list[30] = {0,};
			int _list2[30] = {0,};
			char temp[50] = {0,};
			char strTemp[128] = {0,};

			strcpy(strTemp, pCMyApp->m_SaveChaName);
			int count;

			count=0;

			while(strTemp[count]!='\0')
			{
				switch(strTemp[count])
				{
				case '\\':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"0",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '/':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"1",1);
					strcpy(strTemp+count+2,temp);
					break;
				case ':':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"2",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '*':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"3",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '?':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"4",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '"':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"5",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '<':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"6",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '>':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"7",1);
					strcpy(strTemp+count+2,temp);
					break;
				case '|':
					strncpy(strTemp+count,"'",1);
					strcpy(temp,&strTemp[count+1]);
					strncpy(strTemp+count+1,"8",1);
					strcpy(strTemp+count+2,temp);
					break;
				default:
					break;
				}
				count++;
			}

			strcat(strTemp, ".lcg");

			char tmp[800];
			sprintf(tmp, "%s%s", path, strTemp);

			if( (stream = fopen( tmp, "rb" )) != NULL )
			{
				fread( _list, sizeof( int ), 8, stream );
				fread( _list2, sizeof( int ), 8, stream );

				for (count = 0; count < FUNCTION_NUM; count++ )
				{
					if( g_pNk2DFrame->GetControlBottom() )
					{
						g_pNk2DFrame->GetControlBottom()->m_QuickSkill[count].skill = _list[count];
						g_pNk2DFrame->GetControlBottom()->m_QuickSkill[count].type = _list2[count];
						g_pNk2DFrame->GetControlBottom()->m_FunctionKey[count].m_Type = _list2[count];
					}
				}
				fclose( stream );
			}
		}
		return;

	case GOCREATE:
		{	
			char buf[64] = {0};
			g_pTcpIp->SendNetMessage("char_new_check\n", TRUE);
			if( m_pSheet->GetOneLine(buf, sizeof(buf)) == false )
			{
				return;
			}

			char buf2[64] = {0};
			char *arg = m_pSheet->AnyOneArg(buf, buf2);
			if( strcmp(buf2, "char_new_check") != 0 )
			{
				return;
			}

			arg = m_pSheet->AnyOneArg(arg, buf2);
			int second = atoi(buf2);

			if( second <= 0 )
			{
				m_pSheet->GoPage( CRTCHARPAGE );
				m_pSheet->GoCreateChaPage();
				m_pSheet->m_pCrtCharPg->OutoSetValue(0);// 최초 불칸

				return;
			}
			
			int hour = (second / 60) / 60;
			int min = (second / 60) % 60;
			int sec = (second % 60);
			std::string msg;
			if( hour > 0 )
			{
				msg = boost::str(boost::format("Character creation limit Remaining time:%d hours %d minutes %d seconds") % hour % min % sec);
			}
			else if( min > 0 )
			{
				msg = boost::str(boost::format("Character creation limit Remaining time:%d minutes %d seconds") % min % sec);
			}
			else
			{
				msg = boost::str(boost::format("Character creation limit Remaining time:%d seconds") % sec);
			}


			
			m_MsgPopUpNew.PutString((char*)msg.c_str());
		}
		return;

	case DELETECHAR:
		{
			// 기본 비번 & 2차 비번
			m_MsgPopUp.PutString((char*)G_STRING(IDS_SELCHA_CHA_DELETE));
		}
		break;

	case GOLOGIN:
		{
			m_pSheet->m_SlotNum = -1;
			g_pTcpIp->SetTerminate();

			m_pSheet->m_pLoginPg->m_bShowMatrixList = FALSE;
			m_pSheet->GoPage( LOGINPAGE );
		}
		return ;
	}
}

void LH_SelectCharacterPage::Init()
{
	m_MsgPopUp.Init(TYPE_DEL);
	m_MsgPopUpNew.Init(TYPE_NOR_OK);
	ResetAccountEnd();
}


#define NAME_TEXT_SIZE     14
#define LEVEL_TEXT_SIZE     12
#define NAME_TEXT_COLOR		RGB(244, 232, 48)
#define LEVEL_TEXT_COLOR	RGB(255, 255, 255)
#define DESC_TEXT_COLOR		RGB(148, 148, 148)

void LH_SelectCharacterPage::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_title.Init( "selectchar/sel_char_title" );
	m_bg_list.Init( "selectchar/select_charac" );
	m_btn_start.Init( "selectchar/sel_start" );
	m_blocked.Init( "selectchar/blocked" );


	for( int i=0 ; i<SLOT_COUNT ; i++ )
	{
		m_slot[i].slot.Init( "selectchar/namestic" );
		m_slot[i].namebox.Init( NAME_TEXT_SIZE , NAME_TEXT_COLOR , 0 , 0 );
		m_slot[i].levelbox.Init( LEVEL_TEXT_SIZE , LEVEL_TEXT_COLOR , 0 , 0 );
		m_slot[i].desc.Init( NAME_TEXT_SIZE , DESC_TEXT_COLOR , 0 , 0 );
	}

	m_btn_new.Init( "selectchar/sel_newcharacter" );
	m_btn_delete.Init( "selectchar/sel_delete" );
	m_btn_back.Init( "selectchar/sel_back" );
	m_btn_exit.Init( "selectchar/sel_exit" );

	m_servername.Init( 14 , DESC_TEXT_COLOR , 0 , 0 );
	m_blockedDesc.Init(14, NAME_TEXT_COLOR, 0, 0);
	m_blockedDesc2.Init(14, NAME_TEXT_COLOR, 0, 0);

	m_MsgPopUp.LoadRes();
	m_MsgPopUpNew.LoadRes();




	// 포지션 설정
	m_title.SetPos( (g_pNk2DFrame->GetClientWidth() - m_title.m_w)>>1 , 5 );
	m_bg_list.SetPos( g_pNk2DFrame->GetClientWidth() - m_bg_list.m_w - 17 , (g_pNk2DFrame->GetClientHeight() - m_bg_list.m_h)>>1 );

	m_blocked.SetPos(m_title.m_x - 14, 81);

	m_btn_start.SetPosition( (g_pNk2DFrame->GetClientWidth() - m_btn_start.Width)>>1, g_pNk2DFrame->GetClientHeight() - m_btn_start.Height - 70 );


	int bx = m_bg_list.m_x;
	int by = m_bg_list.m_y;

	m_btn_new.SetPosition( bx + 20 , by + 359 );
	m_btn_delete.SetPosition( bx + 114  , by + 359 );
	m_btn_back.SetPosition( bx + 20 , by + 397 );
	m_btn_exit.SetPosition( bx + 114  , by + 397 );



	m_btn_new.SetDisable( TRUE );
	m_btn_delete.SetDisable( TRUE );
	m_btn_start.SetDisable( TRUE );

}

void LH_SelectCharacterPage::DeleteRes()
{
	SAFE_DELETE( m_pFlatRateText );
	SAFE_DELETE( m_pMeterRateText );
}

void LH_SelectCharacterPage::Draw()
{
	if( !g_pDisplay )
		return;

	m_pSheet->TestSceneRender3();

	BOOL bBlocked = FALSE;

	if( m_selected_slot != -1 && m_pSheet->m_Slot[m_selected_slot] )
	{		
		if( m_pSheet->m_Slot[m_selected_slot]->m_BlockedTime != 0 )
		{
			bBlocked = TRUE;
		}

		m_pSheet->RenderSlotCha(m_selected_slot,0,0,0,0,TRUE, bBlocked);

		if (pCMyApp->m_pFxSet)
		{
			if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
			{
				pCMyApp->m_pFxSet->Render();
				GET_D3DDEVICE()->EndScene();
			}
		}
	}

	m_title.Draw();
	m_bg_list.Draw();
	m_servername.Draw( m_bg_list.m_x + 53 , m_bg_list.m_y + 37 , 123 );

	if( bBlocked == TRUE )
	{
		DrawBlockedUI();
	}

	for( int i=0 ; i<SLOT_COUNT ; i++ )
	{
		m_slot[i].slot.Draw(m_bg_list.m_x + 33 , m_bg_list.m_y + 89 + 49*i , (m_selected_slot==i)?true:false ); // 138////49

		if( m_pSheet->m_Slot[i] )
		{
			m_slot[i].namebox.Draw(m_bg_list.m_x + 33 , m_bg_list.m_y + 100 + 49*i  , 156);
			m_slot[i].levelbox.Draw(m_bg_list.m_x + 33 , m_bg_list.m_y + 116 + 49*i  , 156);
		}
		else
		{
			m_slot[i].desc.Draw(m_bg_list.m_x + 33 , m_bg_list.m_y + 105 + 49*i  , 156);
		}
	}

	m_btn_new.Draw();
	m_btn_delete.Draw();
	m_btn_back.Draw();
	m_btn_exit.Draw();

	m_btn_start.Draw();

	m_MsgPopUpNew.Draw();
	m_MsgPopUp.Draw();


	if (timeGetTime() - m_pSheet->m_dwDummyBaseTime > 120000)
		m_pSheet->SendDummyMsg();

	Sleep(1);

	if( g_dwClientCountry == CTRY_KOR ) // 계정만료일은 일단 한국만 지퓖E
		DrawAccountEnd();


}

void LH_SelectCharacterPage::DrawBlockedUI()
{
	time_t blockedTime = m_pSheet->m_Slot[m_selected_slot]->m_BlockedTime;

	if( blockedTime == 0 )
		return;
	
	m_blocked.Draw();

	bool blockedForever = false;
	if( blockedTime == -1 )
	{
		blockedForever = true;
	}

	std::string strBlocked;

	int blockedReason = 0;

	if( blockedTime == -1 )
	{	
		strBlocked = boost::str(boost::format(G_STRING(IDS_CHAR_BLOCK_FOREVER)) 
			% G_STRING(IDS_CHAR_BLOCK_REASON0 + blockedReason));

		m_blockedDesc.SetString(const_cast<char*>(strBlocked.c_str()));
		m_blockedDesc.Draw( m_title.m_x + 73 , m_title.m_y + 94 , 123 );
	}
	else
	{
		tm* pTime = NULL;
		pTime = localtime(&blockedTime);

		strBlocked = boost::str(boost::format(G_STRING(IDS_CHAR_BLOCK_TIME)) 
			% (pTime->tm_year + 1900)
			% (pTime->tm_mon + 1)
			% pTime->tm_mday
			% pTime->tm_hour
			% pTime->tm_min);

		std::string strReason = boost::str(boost::format(G_STRING(IDS_CHAR_BLOCK_TIME2))
			% G_STRING(IDS_CHAR_BLOCK_REASON0 + blockedReason));

		m_blockedDesc.SetString(const_cast<char*>(strBlocked.c_str()));
		m_blockedDesc2.SetString(const_cast<char*>(strReason.c_str()));
		m_blockedDesc.Draw( m_title.m_x + 73 , m_title.m_y + 87 , 123 );
		m_blockedDesc2.Draw( m_title.m_x + 73 , m_title.m_y + 87 + 15 , 123 );
	}
}

int LH_SelectCharacterPage::GetSlotNum(int x, int y)
{
	int i=0;
	for( i=0 ; i<SLOT_COUNT ; i++ )
	{
		if( m_slot[i].slot.IsInside( x, y ) )
			return i;
	}

	return -1;
}

void LH_SelectCharacterPage::ChangeSlot(int slot_num)
{
	int prev_slot = m_pSheet->m_SlotNum;
	m_selected_slot = slot_num;
	m_pSheet->m_SlotNum = slot_num;

	if( prev_slot >= 0 && prev_slot<5 && m_pSheet->m_pSlotCha[prev_slot] )
	{
		if( m_pSheet->m_pSlotCha[prev_slot]->GetBossRaidHeroEffectIndex() != -1 )
		{
			pCMyApp->m_pFxSet->DeleteEffect( m_pSheet->m_pSlotCha[prev_slot]->GetBossRaidHeroEffectIndex() );
			m_pSheet->m_pSlotCha[prev_slot]->DeleteEffectNum( m_pSheet->m_pSlotCha[prev_slot]->GetBossRaidHeroEffectIndex() );
			m_pSheet->m_pSlotCha[prev_slot]->SetBossRaidHeroEffectIndex( -1 );
		}

		m_pSheet->m_pSlotCha[m_selected_slot]->SetSetItem();


		if( m_pSheet->m_pSlotCha[m_selected_slot]->GetSetItem() == HEROSET || m_pSheet->m_pSlotCha[m_selected_slot]->GetSetItem() == GM_HEROSET )
		{
			if( m_pSheet->m_pSlotCha[m_selected_slot]->GetBossRaidHeroEffectIndex() != -1 )
				return;

			EffectSort ef;

			ZeroMemory( &ef, sizeof(EffectSort));

			ef.nType		= FX_HERO_EFFECT;
			ef.num			= 0;
			ef.nCurFrame	= 0;
			ef.pNkChaTo		= m_pSheet->m_pSlotCha[m_selected_slot];

			int index = pCMyApp->m_pFxSet->InsertFxEffect(ef);
			m_pSheet->m_pSlotCha[m_selected_slot]->InsertEffectNum( index );
			m_pSheet->m_pSlotCha[m_selected_slot]->SetBossRaidHeroEffectIndex( index );
		}
	}

	if (slot_num < 0 || slot_num >= SLOT_COUNT)
	{
		return;
	}

	if (m_pSheet->m_Slot[slot_num])
	{
		if( g_dwClientCountry == CTRY_CN )	// 중국은 삭제 제한을 없앴다. (by 원석)
		{
		}
		else if( g_dwClientCountry == CTRY_TH && m_pSheet->m_Slot[ slot_num ]->m_Level >= 250 )
		{
		}
		else if( !g_bTestServer &&
				 (( m_pSheet->m_Slot[ slot_num ]->m_Race == RACE_FREAK	&& m_pSheet->m_Slot[ slot_num ]->m_Level >= 250 ) ||
				  ( m_pSheet->m_Slot[ slot_num ]->m_Race != RACE_FREAK && m_pSheet->m_Slot[ slot_num ]->m_Level >= 200 )) )
		{
		}
		else
		{
		}
	}
	else
	{
	}

}

void LH_SelectCharacterPage::SetCharNames(void)
{
	if( m_pSheet == NULL )
		return;

	for (int i = 0; i < SLOT_COUNT; i++)
	{
		if( m_pSheet->m_Slot[i] )
		{
			m_slot[i].namebox.SetString( m_pSheet->m_Slot[i]->m_Name );
			char buf[32];
			wsprintf( buf , "Lv.%3d" , m_pSheet->m_Slot[i]->m_Level );
			m_slot[i].levelbox.SetString( buf );
		}
		else
		{
			m_slot[i].desc.SetString( (char*)G_STRING(IDS_LHSTRING1823) );
		}
	}
}

void LH_SelectCharacterPage::SetCharLevel(void)
{
}

void LH_SelectCharacterPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if(m_MsgPopUp.m_bActive == TRUE)
	{
		int ret = -1;
		if( (ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam) ) == 1 )
		{
			if(strcmp(m_pSheet->m_sUserPW, m_MsgPopUp.GetCheckPW()) == 0)
			{
				char *msg;
				bool isFail = false;
				if (msg = m_pSheet->SendDelChar(isFail, m_MsgPopUp.GetCheckPW()))
				{
					if( isFail == true )
					{
						m_MsgPopUpNew.PutString(msg);
					}
				}

				if( m_pSheet->UpdateChars() == -1 )
				{
					m_MsgPopUpNew.PutString((char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION));
					return;
				}

				ChangeSlot(m_pSheet->m_SlotNum);

				m_MsgPopUp.m_InputLine.ClearText();
			}
			else
			{
				m_MsgPopUp.PutString((char*)G_STRING(IDS_SELCHA_FAULT_PASS));
				m_MsgPopUp.m_InputLine.ClearText();
			}
		}
		else if( ret == -1 )
		{
			m_MsgPopUp.m_InputLine.ClearText();
		}

		return;
	}

	if(m_MsgPopUpNew.m_bActive == TRUE)
	{
		int ret = -1;
		if ((ret = m_MsgPopUpNew.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{
		}
		else if (ret == -1)
		{
			m_MsgPopUpNew.m_InputLine.ClearText();
		}
		return ;
	}



	for( int i=0 ; i<SLOT_COUNT ; i++ )
		m_slot[i].slot.MsgProc( hWnd , msg , wParam , lParam );

	m_btn_start.MsgProc( hWnd , msg , wParam , lParam );
	m_btn_new.MsgProc( hWnd , msg , wParam , lParam );
	m_btn_delete.MsgProc( hWnd , msg , wParam , lParam );
	m_btn_exit.MsgProc( hWnd , msg , wParam , lParam );
	m_btn_back.MsgProc( hWnd , msg , wParam , lParam );

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		{
			int prev_selected = GetSlotNum(LOWORD(lParam), HIWORD(lParam) );
			if( prev_selected != -1 )
			{
				m_selected_slot = prev_selected;
				m_pSheet->m_SlotNum = m_selected_slot;
			}
			else
			{
				return;
			}

			if( m_selected_slot != -1 )
			{
				if( m_pSheet->m_Slot[m_selected_slot] )
				{
					m_btn_new.SetDisable(TRUE);
					m_btn_delete.SetDisable(FALSE);
					m_btn_start.SetDisable( FALSE );

					CheckBlock(m_selected_slot);
				}
				else
				{
					m_btn_new.SetDisable(FALSE);
					m_btn_delete.SetDisable(TRUE);
					m_btn_start.SetDisable( TRUE );
				}

				if (g_pDSound)
					g_pDSound->Play(g_SheetSound[SHEET_SND_APP]);

			}
		}
		return;

	case WM_MOUSEMOVE:
		{
		}
		break;

	case WM_LBUTTONUP:
		{
			int seleted_slot = GetSlotNum(LOWORD(lParam), HIWORD(lParam));
			if( m_selected_slot != -1 && seleted_slot == m_selected_slot )
				ChangeSlot(m_selected_slot);

			POINT pt = {LOWORD(lParam) , HIWORD(lParam) };


			if( m_btn_start.IsInside(pt.x, pt.y) && m_btn_start.GetState() == BTN_ACTION )
			{
				if( m_selected_slot == -1 )
					return;

				if( !m_pSheet->m_Slot[m_selected_slot] )
					return;

				g_bFinishFxCharWave = TRUE;
				Action( GOSTART );
			}
			else if( m_btn_delete.IsInside(pt.x, pt.y) && m_btn_delete.GetState() == BTN_ACTION )
			{
				if( m_selected_slot == -1 )
					return;

				if( !m_pSheet->m_Slot[m_selected_slot] )
					return;

				if( m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_GuilMa > 0 &&
						m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_GuildIndex > 0 )
				{
					m_MsgPopUpNew.PutString((char*)G_STRING(IDS_GUILD_PLZ_MEMBER_OUT));

					break;
				}
				else if( m_pSheet->m_Slot[m_pSheet->m_SlotNum]->m_Support > 0 )
				{
					m_MsgPopUpNew.PutString((char*)G_STRING(IDS_PLZ_SUPPORT_OUT));

					break;
				}
				else if(g_MatrixSvr != 0 )
				{
					char buf[256];
					sprintf(buf,(char*)G_STRING(IDS_CANT_DEL_MATRIX));
					if(g_bMatrixMatch)
						sprintf(buf,(char*)G_STRING(IDS_CANT_DEL_GAME));

					m_MsgPopUpNew.PutString(buf);
				}
				else
// 삭제버튼 클릭시 일본은 삭제 않되도록(웝滂팀에게 문의하라는 메시햨E
					g_bFinishFxCharWave = TRUE;
				Action( DELETECHAR );
			}
			else if( m_btn_new.IsInside(pt.x, pt.y) &&  m_btn_new.GetState() == BTN_ACTION )
			{
				if(g_MatrixSvr != 0)
				{
					char buf[256];
					sprintf(buf,(char*)G_STRING(IDS_CANT_CRE_MATRIX));
					if(g_bMatrixMatch)
						sprintf(buf,(char*)G_STRING(IDS_CANT_CRE_GAME));

					m_MsgPopUpNew.PutString(buf);
				}
				else
				{
					if( m_selected_slot == -1 )
						return;

					if( m_pSheet->m_Slot[m_selected_slot] )
						return;

					Action( GOCREATE );
				}
			}
			else if( m_btn_back.IsInside(pt.x, pt.y) && m_btn_back.GetState() == BTN_ACTION )
			{
				Action( GOLOGIN );
			}
			else if(  m_btn_exit.IsInside(pt.x, pt.y) && m_btn_exit.GetState() == BTN_ACTION )
			{
				g_pTcpIp->SetTerminate();
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			break;
		}
	}
}

void LH_SelectCharacterPage::ResetAccountEnd()
{
	m_UserIndex = 0;
	ZeroMemory( &m_FlatRateDay, sizeof(SYSTEMTIME) );
	ZeroMemory( m_strFlatRateTime, sizeof(m_strFlatRateTime) );
	m_MeterRateTime = 0;
	SAFE_DELETE( m_pFlatRateText );
	SAFE_DELETE( m_pMeterRateText );
}

void LH_SelectCharacterPage::GetAccountEndString( int UserIndex )
{
	m_UserIndex = UserIndex;

	CWebWorld	webworld;

	char address[256] = "";
	char temp[256] = "";

	sprintf(address, "http://payment.laghaim.com/ForGame/getUserGameTimeInfo.asp?");
	sprintf(temp, "userId=%d", (int)m_UserIndex);
	strcat( address, temp );

	if( webworld.GetWebPage(address) )
	{
		if( webworld.m_UrlData )
			SetAccountEndInfo( webworld.m_UrlData );
	}
}
void LH_SelectCharacterPage::SetAccountEndInfo( char *strAccountEnd )
{
	ResetAccountEnd(); // 초기화.


	// 캐릭툈E따男病 여기서 설정하자!?
	m_servername.SetString( g_pNk2DFrame->GetConnectServerName() );

	for( int i=0 ; i<SLOT_COUNT ; i++ )
	{
		if( m_pSheet->m_Slot[i] )
		{
			m_slot[i].namebox.SetString( m_pSheet->m_Slot[i]->m_Name );
			char buf[32];
			wsprintf( buf , "Lv.%3d" , m_pSheet->m_Slot[i]->m_Level );
			m_slot[i].levelbox.SetString( buf );
		}
		else
		{
			m_slot[i].desc.SetString( "빈 슬 롯" );
		}
	}

	char strTemp1[256];
	char strTemp2[256];
	char msg[256];

	int OriginalStringLen = 0;
	int SectionPoint = -1; // 갈라지는 표시인 "||"의 위치.

	if( !strAccountEnd ) // 잘툈E들엉邦툈E..
		return;

	OriginalStringLen = strlen(strAccountEnd);

	if( OriginalStringLen == 0 || OriginalStringLen <= 2 ) // 내퓖E?없거나 0 이거나 결제 정보가 없으툈E"||"만 있으툈E
		return;

	// 문자열을 "||"을 기준으로 자른다.
	for( int i = 0 ; i < OriginalStringLen-2 ; ++i ) // 문자열 길이만큼 돌면서...
	{
		if( strncmp(&strAccountEnd[i],"||",2) == 0 ) // 구분 점이 발견되었으툈E..
			SectionPoint = i; // 구분점을 기푳E巒畇?
	}

	if( SectionPoint == -1 ) // 구분점이 업으툈E..
		return;

	if( SectionPoint != 0 ) // OriginalStringLen이 0인 경퓖E?정량제는 이퓖E舊갋않은 경퓖E?
	{
		// 정량제 부분 스트링 셋팅.
		strncpy( strTemp1, strAccountEnd, SectionPoint );
		strTemp1[SectionPoint] = NULL;

		char temp1[10];
		char temp2[10];
		char temp3[10];

		sscanf( strTemp1, "%s %s %s %s", temp1, temp2, temp3, m_strFlatRateTime ); // 구분지엉幕 읽엉龐다.

		// 스트링으로 끊엉幕 변환해야한다. 07일 <- 이런경퓖E%d로는 제큱E?못읽엉邦더턿E
		m_FlatRateDay.wMonth = atoi(temp1);
		m_FlatRateDay.wDay = atoi(temp2);
		m_FlatRateDay.wYear = atoi(temp3);
	}

	if( SectionPoint+2 <= OriginalStringLen ) // 이 경퓖E?종량제를 이퓖E構갋있는 경퓖E
	{
		// 종량제 부분 스트링 셋팅
		strncpy( strTemp2, &strAccountEnd[SectionPoint+2], OriginalStringLen-SectionPoint+2 );
		strTemp2[OriginalStringLen-SectionPoint+2] = NULL;

		m_MeterRateTime = atoi( strTemp2 ); // 종량제 시간 셋팅.
	}

	if( m_FlatRateDay.wMonth ) // 이게 0이 아니툈E정량제를 쓰과復는 거다.
	{
		// 5일 이하일때만 표시
		SYSTEMTIME NowTime;
		GetSystemTime( &NowTime );
		//		if( NowTime.wYear == m_FlatRateDay.wYear && NowTime.wMonth == m_FlatRateDay.wMonth && NowTime.wDay >= m_FlatRateDay.wDay-5 )
		{
			int font_size = 14;

			sprintf( msg, G_STRING(IDS_LHSTRING1747), m_FlatRateDay.wYear, m_FlatRateDay.wMonth, m_FlatRateDay.wDay, m_strFlatRateTime );

			int len;
			if ((len = strlen(msg)) <= 0)
				return;

			m_pFlatRateText = new CTextOutBox;
			if( m_pFlatRateText )
			{
				m_pFlatRateText->Init(font_size, RGB(0, 200, 0), 5, 7); // 이 상태에선 가로 사이짊瞼 항퍊E800이다.
				m_pFlatRateText->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(220, 100, 135) ); // 인자 : 글, 글자퍊E 외곽선 퍊E
			}
		}
	}

	if( m_MeterRateTime && m_MeterRateTime < 60 ) // 이게 0이 아니툈E종제를 쓰과復는 거다.(한시간 이하로 남았을 때만 표시)
	{
		int font_size = 14;

		sprintf( msg, G_STRING(IDS_LHSTRING1748), m_MeterRateTime );

		int len;
		if ((len = strlen(msg)) <= 0)
			return;

		m_pMeterRateText = new CTextOutBox;
		if( m_pMeterRateText )
		{
			m_pMeterRateText->Init(font_size, RGB(0, 200, 0), 5, 27 ); // 이 상태에선 가로 사이짊瞼 항퍊E800이다.
			m_pMeterRateText->SetString_withOutLine( msg, RGB(255, 255, 255), RGB(220, 100, 135) ); // 인자 : 글, 글자퍊E 외곽선 퍊E
		}
	}
}
void LH_SelectCharacterPage::DrawAccountEnd()
{
	if( m_pFlatRateText && m_pMeterRateText ) // 정량제 스트링컖E종량제 스트링이 둘다 있는 경퓖E
	{
		m_pFlatRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
		m_pMeterRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
	else if( m_pFlatRateText ) // 정량제만 있는 경퓖E
	{
		m_pFlatRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
	else if( m_pMeterRateText ) // 종량제만 잇는 경퓖E
	{
		m_pMeterRateText->Draw(/* 0, 0, m_pFlatRateText->m_Width/2 */);
	}
}

void LH_SelectCharacterPage::UpdateButton(int slot_num)
{
	m_selected_slot = slot_num;
	m_pSheet->m_SlotNum = slot_num;

	if( m_selected_slot != -1 )
	{
		m_btn_new.SetState(BTN_NORMAL);
		m_btn_delete.SetDisable(BTN_NORMAL);
		m_btn_start.SetDisable( BTN_NORMAL );

		m_btn_new.SetDisable(TRUE);
		m_btn_delete.SetDisable(FALSE);
		m_btn_start.SetDisable( FALSE );
	}
	else
	{
		m_btn_new.SetState(BTN_NORMAL);
		m_btn_delete.SetDisable(BTN_NORMAL);
		m_btn_start.SetDisable( BTN_NORMAL );

		m_btn_new.SetDisable(TRUE);
		m_btn_delete.SetDisable(TRUE);
		m_btn_start.SetDisable( TRUE );
	}
}

HRESULT LH_SelectCharacterPage::Restore()
{
	HRESULT hr = S_OK;

	if( g_pNk2DFrame->GetConnectServerName() )
		m_servername.SetString( g_pNk2DFrame->GetConnectServerName() );
	SetCharNames();
	m_MsgPopUp.RestoreAll();
	m_MsgPopUpNew.RestoreAll();

	return hr;
}

void LH_SelectCharacterPage::CheckBlock(int select_slot)
{
	if( m_pSheet->m_Slot[select_slot]->m_BlockedTime == 0 )
		return;

	m_btn_new.SetDisable(TRUE);
	m_btn_delete.SetDisable(TRUE);
	m_btn_start.SetDisable(TRUE);
}
