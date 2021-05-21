#include "stdafx.h"
#include <boost/format.hpp>
#include "LHUI_TakeBox.h"
#include "BasicButton.h"
#include "BasicStatic.h"
#include "MultiListBox.h"
#include "TextOutBox.h"
#include "ControlBottom.h"

#include "main.h"
#include "UIMgr.h"
#include "Nk2DFrame.h"
#include "Tcpipcon.h"
#include "ItemProto.h"
#include "g_stringmanager.h"

LHUI_TakeBox::LHUI_TakeBox(void)
	:	m_background(NULL)
	,	m_ImgPage(NULL)
	,	m_ImgSearch(NULL)
	,	m_btn_close(NULL)
	,	m_btn_get(NULL)
	,	m_btn_getall(NULL)
	,	m_btn_first(NULL)
	,	m_btn_prev(NULL)
	,	m_btn_next(NULL)
	,	m_btn_end(NULL)
	,	m_btn_search(NULL)
	,	m_lst_item(NULL)
	,	m_textout_page(NULL)
	,	m_send_message(false)
	,	m_recv_message(false)
	,	m_current_page(-1)
	,	m_max_page(-1)
	,	m_selected_item(-1)
	,	m_num_item(0)
	,	m_takebox_type(TakeBox_Type__Character)
	,	m_takebox_state(TakeBox_State__None)
	,	m_send_delay(0)
{
	_SetVisible(true);
	_SetDisable(false);

	Init();
}

LHUI_TakeBox::LHUI_TakeBox(TakeBox_Type type)
	:	m_background(NULL)
	,	m_ImgPage(NULL)
	,	m_ImgSearch(NULL)
	,	m_btn_close(NULL)
	,	m_btn_get(NULL)
	,	m_btn_getall(NULL)
	,	m_btn_first(NULL)
	,	m_btn_prev(NULL)
	,	m_btn_next(NULL)
	,	m_btn_end(NULL)
	,	m_btn_search(NULL)
	,	m_lst_item(NULL)
	,	m_textout_page(NULL)
	,	m_send_message(false)
	,	m_recv_message(false)
	,	m_current_page(-1)
	,	m_max_page(-1)
	,	m_selected_item(-1)
	,	m_num_item(0)
	,	m_takebox_type(type)
	,	m_takebox_state(TakeBox_State__None)
	,	m_send_delay(0)
{
	_SetVisible(true);
	_SetDisable(false);

	Init();
}

LHUI_TakeBox::~LHUI_TakeBox(void)
{
	DeleteRes();
	ResetAll();	
}

void LHUI_TakeBox::Init()
{
	DeleteRes();
	ResetAll();

	m_background	= new CBasicStatic;
	m_ImgPage		= new CBasicStatic;
	m_ImgSearch		= new CBasicStatic;
	m_btn_close 	= new CBasicButton;
	m_btn_get		= new CBasicButton;
	m_btn_getall	= new CBasicButton;
	m_btn_first		= new CBasicButton;
	m_btn_prev 		= new CBasicButton;
	m_btn_next 		= new CBasicButton;
	m_btn_end		= new CBasicButton;
	m_btn_search	= new CBasicButton;
	m_lst_item		= new CMutiListBox(" ",0, 0, 0, 10, 4 ,12);
	m_textout_page	= new CTextOutBox();

	m_lst_item->AddLine(0 , 74.f);
	m_lst_item->AddLine(1 , 235.f);
	m_lst_item->AddLine(2 , 106.f );
	m_lst_item->AddLine(3 , 126.f );
	m_lst_item->SetImage("guild/scr_down", "guild/scr_up", "interface/guild/scr_bar.bmp");
	m_lst_item->SetSelectedLine(-1);
	LoadRes();
}

void LHUI_TakeBox::LoadRes()
{
	m_background->Init( "takebox/bg_takebox" );
	m_ImgPage->Init( "takebox/BG_TAKEBOX_03" );
	m_ImgSearch->Init( "takebox/BG_TAKEBOX_03" );
	m_btn_close->Init( "common/btn_cancel" );	
	m_btn_get->Init( "takebox/btn_get" );
	m_btn_getall->Init( "takebox/btn_get1" );
	m_btn_first->Init( "shop/btn_llarrow" );
	m_btn_prev->Init( "shop/btn_larrow" );
	m_btn_next->Init( "shop/btn_rarrow" );
	m_btn_end->Init( "shop/btn_rrarrow" );
	m_btn_search->Init( "takebox/SEARCH_BT");
	m_lst_item->LoadRes();

	int bgx = ( g_pNk2DFrame->GetClientWidth() - m_background->m_w ) >> 1;
	int bgy = ( g_pNk2DFrame->GetClientHeight() - m_background->m_h ) >> 1;

	m_background->SetPos( bgx, bgy );
	m_ImgPage->SetPos( bgx + 223, bgy + 303 );
	m_ImgSearch->SetPos( bgx + 17, bgy + 303 );
	m_btn_close->SetPosition( bgx + 509 , bgy + 302 );
	m_btn_get->SetPosition( bgx + 368 , bgy + 302 );
	m_btn_getall->SetPosition( bgx + 426, bgy + 302 );
	m_btn_first->SetPosition( bgx + 174, bgy + 304 );
	m_btn_prev->SetPosition( bgx + 198, bgy + 304 );
	m_btn_next->SetPosition( bgx + 320, bgy + 304 );
	m_btn_end->SetPosition( bgx + 344, bgy + 304 );
	m_btn_search->SetPosition( bgx + 117, bgy + 302 );

	m_lst_item->SetPosDim(bgx+18, bgy+70, 540, 220);
	m_textout_page->Init(12, RGB(255,255,255), bgx+ 225 , bgy+310 );

	m_lst_item->m_DecBtn.SetVisible(false);
	m_lst_item->m_IncBtn.SetVisible(false);

	for( int i=0 ; i<MAX_TAKEBOX ; i++ )
	{
		m_box_list[i].initialized = false;
	}

	m_InputSearch.Init(bgx + 22, bgy + 307, 10, RGB(255,255,255), RGB(255,179,0), FALSE, MSG_POPUP);
	m_InputSearch.SetfontSize(12);
}

void LHUI_TakeBox::DeleteRes()
{
	SAFE_DELETE(m_background);
	SAFE_DELETE(m_ImgPage);
	SAFE_DELETE(m_ImgSearch);
	SAFE_DELETE(m_btn_close);
	SAFE_DELETE(m_btn_get);
	SAFE_DELETE(m_btn_getall);
	SAFE_DELETE(m_btn_first);
	SAFE_DELETE(m_btn_prev);
	SAFE_DELETE(m_btn_next);
	SAFE_DELETE(m_btn_end);
	SAFE_DELETE(m_btn_search);
	SAFE_DELETE(m_lst_item);
	SAFE_DELETE(m_textout_page);
	m_InputSearch.DeleteRes();
}

void LHUI_TakeBox::Draw()
{
	if( !_GetVisible() )
		return;

	m_background->Draw();
	m_ImgPage->Draw();	
	m_btn_close->Draw();
	m_btn_get->Draw();	
	m_btn_first->Draw();
	m_btn_prev->Draw();
	m_btn_next->Draw();
	m_btn_end->Draw();	

	if( m_takebox_state == TakeBox_State__Finish )
		m_lst_item->Draw();

	m_textout_page->Draw(87);

	if( m_takebox_type != TakeBox_Type__User )
	{
		m_ImgSearch->Draw();
		m_btn_search->Draw();		
		m_InputSearch.Draw();
		m_btn_getall->Draw();
	}	
}

LRESULT LHUI_TakeBox::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !_GetVisible() || _GetDisalbe() )
		return 0;

	if( m_takebox_state != TakeBox_State__Finish && m_takebox_state != TakeBox_State__None )
		return 0;
		
	if( m_InputSearch.GetEnable() && g_pNk2DFrame->GetControlBottom()->HasFocus() )
	{
		m_InputSearch.SetEnable( FALSE );
		pCMyApp->ConvertImmStatus();
	}

	m_btn_close->MsgProc( hWnd , msg , wParam , lParam );
	m_btn_get->MsgProc( hWnd , msg , wParam , lParam );	
	m_btn_first->MsgProc( hWnd , msg , wParam , lParam );
	m_btn_prev->MsgProc( hWnd , msg , wParam , lParam );
	m_btn_next->MsgProc( hWnd , msg , wParam , lParam );
	m_btn_end->MsgProc( hWnd , msg , wParam , lParam );
	
	if( m_takebox_type != TakeBox_Type__User )
	{
		m_btn_search->MsgProc( hWnd, msg, wParam, lParam );
		m_btn_getall->MsgProc( hWnd, msg, wParam, lParam );
	}	

	if( E_FAIL != m_lst_item->GetMessage(hWnd, msg, wParam, lParam) )
		return 1;

	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt = { LOWORD(lParam) , HIWORD(lParam) };
			if( !m_background->IsIn( pt.x , pt.y) )
				return 0;

			if( m_btn_close->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_get->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_getall->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_first->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_prev->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_next->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_end->GetState() == BTN_DOWN )
				return 1;
			if( m_btn_search->GetState() == BTN_DOWN )
				return 1;

			if( m_takebox_type != TakeBox_Type__User )
			{
				int bInputEnable = m_InputSearch.GetEnable();

				if( E_FAIL != m_InputSearch.MsgProc( hWnd, msg, wParam, lParam ) )				
					return 1;

				if( bInputEnable == FALSE && m_InputSearch.GetEnable() )
				{
					pCMyApp->RestoreImmStatus();
				}
			}			

			return 1;
		}
		break;

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		{
			POINT pt = { LOWORD(lParam) , HIWORD(lParam) };
			if( !m_background->IsIn( pt.x , pt.y) )
				return 0;

			if( m_takebox_type != TakeBox_Type__User )
			{
				m_InputSearch.MsgProc(hWnd, msg, wParam, lParam);
			}			

			if( m_btn_close->GetState() == BTN_ACTION )
			{
				return 2;
			}

			if( m_btn_get->GetState() == BTN_ACTION )
			{
				int cur_sel = m_lst_item->GetSelectedLine();
				if( cur_sel == -1 )
					return 1;

				Request_Item(cur_sel);

				return 1;
			}
			if( m_btn_getall->GetState() == BTN_ACTION )
			{
				Request_ItemAll();

				return 1;
			}
			if( m_btn_search->GetState() == BTN_ACTION )
			{
				if( Request_List_Search() )
					m_recived_data = true;

				return 1;
			}
			if( m_btn_first->GetState() == BTN_ACTION )
			{
				if( Request_List(0) )
					m_recived_data = true;

				return 1;
			}
			if( m_btn_prev->GetState() == BTN_ACTION )
			{
				if( Request_List(m_current_page-1) )
					m_recived_data = true;

				return 1;
			}
			if( m_btn_next->GetState() == BTN_ACTION )
			{
				if( Request_List(m_current_page+1) )
					m_recived_data = true;

				return 1;
			}
			if( m_btn_end->GetState() == BTN_ACTION )
			{
				if( Request_List(m_max_page) )
					m_recived_data = true;

				return 1;
			}

			return 1;
		}
		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		{
			if( m_takebox_type == TakeBox_Type__User )
				return 0;

			if( m_InputSearch.GetEnable() )
			{
				m_InputSearch.MsgProc( hWnd, msg, wParam, lParam );
				return 1;
			}
		}
		break;
	}

	return 0;
}

void LHUI_TakeBox::ResetAll()
{
	ZeroMemory( m_box_list , sizeof(LHUIBoxSet)*MAX_TAKEBOX );
	if(m_lst_item)
		m_lst_item->all_Reset();

	m_takebox_state = TakeBox_State__None;
}

bool LHUI_TakeBox::Request_List()
{
	char msg_buf[256] = {0};
	if( m_takebox_type == TakeBox_Type__Character )
		sprintf( msg_buf , "gift_present_list %d\n" , 0 );
	else if( m_takebox_type == TakeBox_Type__User )
		sprintf( msg_buf , "gift_goods_list %d\n" , 0 );
	else if( m_takebox_type == TakeBox_Type__ServerMove )
		sprintf( msg_buf , "combine_stash_list %d\n" , 0 );	

	LH_SEND_NET_MSG( msg_buf );
	m_send_delay = GetTickCount();

	return true;
}

bool LHUI_TakeBox::Request_List_Search()
{
	if( m_send_delay != 0 && GetTickCount() - m_send_delay < 3000 )
	{
		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1842) , -1 );
		return false;
	}

	char msg_buf[256] = {0};
	m_strSearch = m_InputSearch.GetStrBuf();	
	m_InputSearch.ClearText();
	m_InputSearch.SetEnable(FALSE);
	if( m_strSearch.length() >= 4 && m_strSearch.length() <= 10 )
	{
		if( m_strSearch.find(" ") != std::string::npos )
		{
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SEARCH_TEXT_NO_SPACE_ENABLE), TYPE_NOR_OK, 1);
			return false;
		}

		if( m_takebox_type == TakeBox_Type__Character )
			sprintf( msg_buf , "gift_present_list_search %d %s\n" , 0, m_strSearch.c_str() );
		else if( m_takebox_type == TakeBox_Type__User )
			sprintf( msg_buf , "gift_goods_list %d\n" , 0 );
		else if( m_takebox_type == TakeBox_Type__ServerMove )
			sprintf( msg_buf , "combine_stash_list_search %d %s\n" , 0, m_strSearch.c_str() );		
	}
	else if( m_strSearch.length() == 0 )
	{
		Request_List();
		return true;
	}
	else
	{
		m_strSearch = "";
		g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SEARCH_TEXT_WRONG_LENGTH), TYPE_NOR_OK, 1);		
		return false;
	}

	LH_SEND_NET_MSG( msg_buf );
	m_send_delay = GetTickCount();

	return true;
}

bool LHUI_TakeBox::Request_List(int page)
{
	if( m_send_delay != 0 && GetTickCount() - m_send_delay < 3000 )
	{
		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1842) , -1 );
		return false;
	}

	int temp_max_page = m_max_item_prev / MAX_TAKEBOX;
	if( m_max_item_prev % MAX_TAKEBOX )
		temp_max_page++;

	m_max_page = m_max_item / MAX_TAKEBOX;
	if( m_max_item % MAX_TAKEBOX )
		m_max_page++;
		
	if( page >= m_max_page )	page = m_max_page-1;
	if( page < 0 )				page = 0;

	if( m_max_page == temp_max_page )
	{
		if( m_current_page == page )
			return false;
	}

	ResetAll();

	char msg_buf[256] = {0};
	
	if( m_strSearch.length() >= 4 && m_strSearch.length() <= 10 )
	{
		if( m_strSearch.find(" ") != std::string::npos )
		{
			g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_SEARCH_TEXT_NO_SPACE_ENABLE), TYPE_NOR_OK, 1);
			return false;
		}

		if( m_takebox_type == TakeBox_Type__Character )
			sprintf( msg_buf , "gift_present_list_search %d %s\n" , page, m_strSearch.c_str() );
		else if( m_takebox_type == TakeBox_Type__User )
			sprintf( msg_buf , "gift_goods_list %d\n" , page );
		if( m_takebox_type == TakeBox_Type__ServerMove )
			sprintf( msg_buf , "combine_stash_list_search %d %s\n" , page, m_strSearch.c_str() );
	}
	else
	{
		if( m_takebox_type == TakeBox_Type__Character )
			sprintf( msg_buf , "gift_present_list %d\n" , page );
		else if( m_takebox_type == TakeBox_Type__User )
			sprintf( msg_buf , "gift_goods_list %d\n" , page );
		else if( m_takebox_type == TakeBox_Type__ServerMove )
			sprintf( msg_buf , "combine_stash_list %d\n" , page );		
	}

	LH_SEND_NET_MSG( msg_buf );
	m_send_delay = GetTickCount();

	m_takebox_state = TakeBox_State__Send;

	return true;
}

bool LHUI_TakeBox::Request_Item(int index)
{
	if( !m_box_list[index].initialized )
	{
		g_pNk2DFrame->AddChatStr( (char*)G_STRING(IDS_LHSTRING1843) , -1 );
		return false;
	}

	char msg_buf[256];
	if( m_takebox_type == TakeBox_Type__Character )
		sprintf( msg_buf , "gift_present_get 1 %d\n" , m_box_list[index].db_index );
	else if( m_takebox_type == TakeBox_Type__User )
		sprintf( msg_buf , "gift_goods_get %d\n" , m_box_list[index].db_index );
	else if( m_takebox_type == TakeBox_Type__ServerMove )
		sprintf( msg_buf , "combine_stash_get 1 %d\n" , m_box_list[index].db_index );	

	LH_SEND_NET_MSG( msg_buf );

	m_send_delay = GetTickCount();

	// »πµÊ ø‰√ª
	m_takebox_state = TakeBox_State__Send;

	return true;
}

bool LHUI_TakeBox::Request_ItemAll()
{
	std::string msg_buf;
	if( m_takebox_type == TakeBox_Type__Character || m_takebox_type == TakeBox_Type__ServerMove )
	{	
		int count = 0;
		for(int i = 0; i < m_num_item; i++)
		{
			if( m_box_list[i].initialized == false )
				continue;

			msg_buf += boost::str(boost::format(" %1%") % m_box_list[i].db_index);
			++count;
		}

		if( count == 0 )
		{
			// 
			return false;
		}

		std::string header_buf = boost::str(boost::format("gift_present_get %1%") % count);
		if( m_takebox_type == TakeBox_Type__ServerMove )
		{
			header_buf = boost::str(boost::format("combine_stash_get %1%") % count);
		}

		msg_buf = header_buf + msg_buf;
		msg_buf += "\n";
	}
	else if( m_takebox_type == TakeBox_Type__User )
	{
		return false;		
	}

	LH_SEND_NET_MSG( const_cast<char*>(msg_buf.c_str()) );

	m_send_delay = GetTickCount();

	// »πµÊ ø‰√ª
	m_takebox_state = TakeBox_State__Send;

	return true;
}

void LHUI_TakeBox::SetTakebox(int num_page, int page, int num_item)
{
	m_max_page = num_page / MAX_TAKEBOX;
	if( num_page % MAX_TAKEBOX )
		m_max_page++;

	m_current_page = page;
	m_num_item = 0;
}

void LHUI_TakeBox::SetTakebox(TakeBox_Type type, int num_page, int page, int num_item)
{
	m_takebox_type = (TakeBox_Type)type;

	m_max_item = num_page;
	m_max_item_prev = num_page;
	m_max_page = num_page / MAX_TAKEBOX;
	if( num_page % MAX_TAKEBOX )
		m_max_page++;

	m_current_page = page;
	m_num_item = 0;

	if( m_textout_page )
	{
		char buf[32];
		sprintf( buf, "%d/%d" , (m_current_page+1) , m_max_page );
		m_textout_page->SetString(buf);
	}

	m_send_delay = GetTickCount();
}

void LHUI_TakeBox::InsertItem(int db_index, int vnum, int count, char* date)
{
	m_box_list[m_num_item].initialized	= true;
	m_box_list[m_num_item].db_index		= db_index;
	m_box_list[m_num_item].item_vnum	= vnum;
	m_box_list[m_num_item].item_count	= count;
	strcpy( m_box_list[m_num_item].get_date , date );


	char buf[256];
	sprintf( buf , "%d" , db_index );
	m_lst_item->AddString(m_num_item, 0 , buf);

	int idx = g_ItemList.FindItem(vnum);
	if( idx != -1 )
		m_lst_item->AddString(m_num_item, 1 , (char*)GET_ITEM_NAME_VNUM(vnum));
	else
	{
		char temp[32];
		temp[0] = 0;
		sprintf( temp, "%d" , vnum );
		m_lst_item->AddString(m_num_item, 1 , temp);
	}

	sprintf( buf , "%d" , count );
	m_lst_item->AddString(m_num_item, 2 , buf);
	m_lst_item->AddString(m_num_item, 3 , date);
	m_lst_item->SetIncLine();

	m_num_item++;
}


void LHUI_TakeBox::InsertItem_Test(int db_index, int vnum, int count, char* date)
{
	m_box_list[m_num_item].initialized	= true;
	m_box_list[m_num_item].db_index		= db_index;
	m_box_list[m_num_item].item_vnum	= vnum;
	m_box_list[m_num_item].item_count	= count;
	strcpy( m_box_list[m_num_item].get_date , date );


	char buf[256];
	sprintf( buf , "%d" , db_index );
	m_lst_item->AddString(m_num_item, 0 , buf);

	int idx = g_ItemList.FindItem(vnum);
	if( idx != -1 )
		m_lst_item->AddString(m_num_item, 1 , (char*)GET_ITEM_NAME_VNUM(vnum));
	else
	{
		char temp[32];
		temp[0] = 0;
		sprintf( temp, "%d" , vnum );
		m_lst_item->AddString(m_num_item, 1 , temp);
	}

	sprintf( buf , "%d" , count );
	m_lst_item->AddString(m_num_item, 2 , buf);
	m_lst_item->AddString(m_num_item, 3 , date);
	m_lst_item->SetIncLine();

	m_num_item++;
}

void LHUI_TakeBox::EditItem(int db_index, int count)
{
	int list_index = -1;
	for( int i=0 ; i<MAX_TAKEBOX ; i++ )
	{
		if( m_box_list[i].initialized && m_box_list[i].db_index == db_index )
		{
			list_index = i;
			m_box_list[i].item_count = count;
			break;
		}
	}

	if( list_index < 0 )
	{
		g_pNk2DFrame->AddChatStr((char*)G_STRING(IDS_LHSTRING1845) , -1 );
		return;
	}

	if( count == 0 )
	{
		m_box_list[list_index].initialized = false;

		// ¡ˆ±ﬁµ 
		m_lst_item->AddString(list_index, 2 , (char*)G_STRING(IDS_LHSTRING1853) );
		m_max_item_prev = m_max_item;
		m_max_item--;
		m_max_item = max(0 , m_max_item);
	}
	else
	{
		// ≥≤æ∆¿÷¿Ω
		char buf[256];
		sprintf( buf , "%d" , count );
		m_lst_item->AddString(list_index, 2 , buf);
	}
}

void LHUI_TakeBox::SetRecived(int finished)
{
	m_takebox_state = (TakeBox_State)finished;
}

void LHUI_TakeBox::Refresh()
{
	if(m_lst_item)
		m_lst_item->all_Reset();

	char buf[256];
	for( int i=0 ; i<m_num_item ; i++ )
	{
		sprintf( buf , "%d" , m_box_list[i].db_index );
		m_lst_item->AddString(i, 0 , buf);

		int idx = g_ItemList.FindItem(m_box_list[i].item_vnum);
		if( idx != -1 )
			m_lst_item->AddString(i, 1 , (char*)GET_ITEM_NAME_VNUM(m_box_list[i].item_vnum));
		else
			m_lst_item->AddString(i, 1 , (char*)G_STRING(IDS_LHSTRING1771) );

		if( m_box_list[i].item_count == 0 )
			sprintf( buf , G_STRING(IDS_LHSTRING1772)  );
		else
			sprintf( buf , "%d" , m_box_list[i].item_count );

		m_lst_item->AddString(i, 2 , buf);
		m_lst_item->AddString(i, 3 , m_box_list[i].get_date);

		m_lst_item->SetIncLine();
	}
}

void LHUI_TakeBox::ClearList()
{
	ZeroMemory( m_box_list , sizeof(LHUIBoxSet)*MAX_TAKEBOX );
	if( m_lst_item )
		m_lst_item->all_Reset();

	SetTakebox(m_takebox_type, 0, 0, 0);
}
