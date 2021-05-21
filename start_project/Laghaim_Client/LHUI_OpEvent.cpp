#include "stdafx.h"
#include "LHUI_OpEvent.h"
#include "main.h"
#include "Nk2DFrame.h"
#include "Compas.h"
#include "main.h"
#include "UIMgr.h"
#include "AutoPlay_test.h"
#include "string_res.h"
#include "g_stringmanager.h"





LHUI_OpEvent::LHUI_OpEvent(void)
	:	m_activate(false)
	,	m_activate_ex(false)
	,	m_over_info(NULL)
{
	Init();
}

LHUI_OpEvent::~LHUI_OpEvent(void)
{
	DeleteRes();

	SAFE_DELETE(m_over_info);
}

void LHUI_OpEvent::Init()
{
	char buf[128];
	for( int i=0 ; i<OpEvent_Type__EndOfEnum ; i++ )
	{
		sprintf( buf , "family_event/op_event_%d" , i );
		m_op_event[i].value = 0;

		if( i == OpEvent_Type__Pc || i == OpEvent_Type__Pre || i == OpEvent_Type__Rush
				|| i == OpEvent_Type__Normal
				|| i == OpEvent_Type__LuckyTime
				|| i == OpEvent_Type__Fever_Time
		  )
			m_op_event[i].image.Init( buf ,1 , 1);
		else
			m_op_event[i].image.Init( buf ,9 , 1);
	}

	for( int i=0 ; i<OpEvent_Ex_Type__EndOfEnum ; i++ )
	{
		sprintf( buf , "family_event/op_event_ex_%d" , i );
		m_op_event_ex[i].value = 0;
		m_op_event_ex[i].image.Init( buf ,1 , 1);
	}


	m_over_info = new CAlphaPopUp;
	m_over_info->Init( 12 );
	if( m_over_info )
		m_over_info->LoadRes( g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight() );

	m_dwTime	= 0;
	m_bImgDraw	= false;
	m_fPcLpTime	= 0.0f;
}

void LHUI_OpEvent::LoadRes()
{
}

void LHUI_OpEvent::DeleteRes()
{
	for( int i=0 ; i<OpEvent_Type__EndOfEnum ; i++ )
		m_op_event[i].image.DeleteRes();
	for( int i=0 ; i<OpEvent_Ex_Type__EndOfEnum ; i++ )
		m_op_event_ex[i].image.DeleteRes();

	SAFE_DELETE(m_over_info);
}

void LHUI_OpEvent::Draw()
{
	if( !g_pDisplay || !pCMyApp )
		return;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if( m_activate )
	{
		int sx = pCMyApp->m_pCompas->GetX()+15;
		int offset = 0;

		for( int i=0 ; i<OpEvent_Type__EndOfEnum ; i++ )
		{
			if( m_op_event[i].value )
			{

				m_op_event[i].image.SetPos(sx - (m_op_event[i].image.m_w/m_op_event[i].image.m_num_col)*offset+2 ,5);

				if( i == OpEvent_Type__Rush )
				{
					if( GetTickCount() > m_dwTime + 500 )
					{
						if( m_bImgDraw )		m_bImgDraw = false;
						else					m_bImgDraw = true;

						m_dwTime = GetTickCount();
					}

					if( m_bImgDraw )			m_op_event[i].image.Draw();
				}
				else
					m_op_event[i].image.Draw();
				offset++;
			}
		}
	}

	if( m_activate_ex )
	{
		int sx = pCMyApp->m_pCompas->GetX()+15;
		int offset = 0;
		int sy = 5;

		if( m_activate )
			sy = m_op_event[0].image.m_h + 10;

		for( int i=0 ; i<OpEvent_Ex_Type__EndOfEnum ; i++ )
		{
			if( m_op_event_ex[i].value )
			{
				m_op_event_ex[i].image.SetPos(sx - (m_op_event_ex[i].image.m_w/m_op_event_ex[i].image.m_num_col)*offset+2 ,sy );
				m_op_event_ex[i].image.Draw();
				offset++;
			}
		}
	}


	if( m_over_info )
	{
		if(m_activate)
		{
			char buf[256] = {0,};
			CMultiText multiText;

			for( int i=0 ; i<OpEvent_Type__EndOfEnum ; i++ )
			{
				if( m_op_event[i].value && m_op_event[i].image.IsIn( point.x , point.y ) )
				{
					switch( i )
					{
					/*case OpEvent_Type__LuckyTime:
						{
							sprintf( buf , (char*)G_STRING(IDS_LHSTRING1919) );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Pc:
					case OpEvent_Type__Pre:
					case OpEvent_Type__Normal:
						{
							sprintf( buf , G_STRING(IDS_LHSTRING1749), m_fPcLpTime );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Drop:
						{
							sprintf( buf , G_STRING(IDS_EVENTBUFF_MSG1) );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Att:
						{
							sprintf( buf , G_STRING(IDS_EVENTBUFF_MSG2));
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Def:
						{
							sprintf( buf , G_STRING(IDS_EVENTBUFF_MSG3) );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Hp:
						{
							sprintf( buf , G_STRING(IDS_EVENTBUFF_MSG4));
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Exp:
						{
							sprintf( buf , G_STRING(IDS_EVENTBUFF_MSG5) );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;

					case OpEvent_Type__Less_Exp:
						{
							sprintf( buf , G_STRING(IDS_EVENTBUFF_MSG5) );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;*/

					case OpEvent_Type__Rush:
						{
							if( pCMyApp->m_pUIMgr->m_AutoPlay->m_dwGameMode == 0 )
								strcpy(buf, "Potion automatic");
							else if( pCMyApp->m_pUIMgr->m_AutoPlay->m_dwGameMode == 1 )
								strcpy(buf, "Semi-automatic");
							else if( pCMyApp->m_pUIMgr->m_AutoPlay->m_dwGameMode == 2 )
								strcpy(buf, "Full automatic");
							else
								strcpy(buf, "Bot");

							strcat(buf, G_STRING(IDS_LHSTRING1752) );
							multiText.AddString( buf , RGB(255,255,255) );

							sprintf( buf , G_STRING(IDS_LHSTRING1750) );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );

						}
						break;

					case OpEvent_Type__Fever_Time:
						{
							// 최대 생명력 3000 상승\n공격력 10% 상승\n방어력 10% 상승\n경험치 10% 상승
							strcpy( buf , G_STRING(IDS_LHSTRING1956) );						// % 출력을 위해 sprintf말고 strcpy

							char temp[128] = {0,};
							char* t = 0;

							if (t = strtok(buf, "\\n"))		// \n가 토큰
							{
								strcpy(temp, t);
								multiText.AddString( temp , RGB(255,255,255) );
							}

							while (t = strtok(NULL, "\\n"))	// 두번재 부턴 NULL
							{
								strcpy(temp, t);
								multiText.AddString( temp , RGB(255,255,255) );
							}

							m_over_info->PutString( &multiText );
						}
						break;
					}
					m_over_info->Draw(point.x, point.y);
					return;
				}
			}
		}

	/*	if( m_activate_ex )
		{
			for( int i=0 ; i<OpEvent_Ex_Type__EndOfEnum ; i++ )
			{
				if( m_op_event_ex[i].value && m_op_event_ex[i].image.IsIn( point.x , point.y ) )
				{
					CMultiText multiText;
					char buf[32] = {0,};
					switch(i)
					{
					case OpEvent_Ex_Type__Att:
						{
							sprintf( buf , G_STRING(IDS_LHSTRING1753) , m_op_event_ex[i].value * 10 );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;
					case OpEvent_Ex_Type__Def:
						{
							sprintf( buf , G_STRING(IDS_LHSTRING1754) , m_op_event_ex[i].value * 10 );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;
					case OpEvent_Ex_Type__Exp:
						{
							sprintf( buf , G_STRING(IDS_LHSTRING1755) , m_op_event_ex[i].value * 10 );
							multiText.AddString( buf , RGB(255,255,255) );
							m_over_info->PutString( &multiText );
						}
						break;
					}

					m_over_info->Draw(point.x, point.y);
					return;
				}
			}
		}*/

	}
}

bool LHUI_OpEvent::IsInside( int x, int y )
{
	return false;
}

LRESULT LHUI_OpEvent::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

void LHUI_OpEvent::SetEvent( int type , int value )
{
	m_op_event[type].value = value;

	if( m_op_event[type].value != 0 )
	{
		m_op_event[type].image.SetCurrent( value - 1 );
		m_activate = true;
		return;
	}

	for( int i=0 ; i<OpEvent_Type__EndOfEnum ; i++ )
	{
		if( m_op_event[i].value )
		{
			m_activate = true;
			return;
		}
	}

	m_activate = false;
}

// 시간값
void LHUI_OpEvent::SetEvent_Ex( int type , int value )
{
	m_op_event_ex[type].value = value;

	if( m_op_event_ex[type].value != 0 )
	{
		// m_op_event_ex[type].image.SetCurrent( value - 1 );
		m_activate_ex = true;
		return;
	}

	for( int i=0 ; i<OpEvent_Ex_Type__EndOfEnum ; i++ )
	{
		if( m_op_event_ex[i].value )
		{
			m_activate_ex = true;
			return;
		}
	}

	m_activate_ex = false;
}

bool LHUI_OpEvent::IsActivate()
{
	return m_activate|m_activate_ex;
}