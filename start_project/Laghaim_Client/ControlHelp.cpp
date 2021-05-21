#include "stdafx.h"
#include "headers.h"
#include "controlhelp.h"
#include "Country.h"



extern DWORD		g_dwClientCountry;
///아케론심의등급

#define		_NONE			-1  // 처음의 상태
#define		SAFE_DELETE(p)	{ if(p) { delete (p); (p)=NULL; } }

CControlHelp::CControlHelp()
{
	m_cur_index = _NONE;

	// 등급심의
	m_bViewKrServiceLv = FALSE;
	m_pSyntexKrServiceLv = NULL;
	m_pServiceLvCloseBtn = NULL;
}

CControlHelp::~CControlHelp()
{
	DeleteRes();
}

void CControlHelp::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_background.Init( "help/help" );
	m_btn_close.Init( "common/btn_close_01" );

	m_tab_01.Init( "common/btn_tab_01" );
	m_tab_02.Init( "common/btn_tab_02" );

	m_btn_list.Init( "help/btn_list" );
	m_btn_prev.Init( "common/btn_larrow" );
	m_btn_next.Init( "common/btn_rarrow" );


	int i;
	char buf[256];
	for( i=0 ; i<SYNTEX_ALL_NUM ; i++ )
	{
		sprintf( buf , "help/btn_link%d" , i+1 );
		m_btn_link[i].Init( buf );

		sprintf( buf , "help/help%d" , i+1 );
		m_help[i].Init( buf );
	}

	if( g_dwClientCountry == CTRY_KOR ) // 한국만...
	{
		m_LinkKrServiceLv.SetFileName( "help/btn_link20" );
		m_LinkKrServiceLv.LoadRes();
	}

	m_background.Align(3 , g_pNk2DFrame->GetClientWidth(),g_pNk2DFrame->GetClientHeight() );

	int bgx = m_background.m_x;
	int bgy = m_background.m_y;

	m_btn_close.SetPosition( bgx + 257 , bgy + 6 );
	m_tab_01.SetPosition( bgx + 70 , bgy + 58 ); // 17 68
	m_tab_02.SetPosition( bgx + 115 , bgy + 58 ); // 96 68

	m_btn_list.SetPosition( bgx + 168 , bgy + 378 ); // 131  378
	m_btn_prev.SetPosition( bgx + 145 , bgy + 378 ); //97 378
	m_btn_next.SetPosition( bgx + 191 , bgy + 378 ); // 164 378


	int gap_h = 22;

	for( i=0 ; i<SYNTEX_ALL_NUM ; i++ )
	{
		m_btn_link[i].SetPosition( bgx + 16+50 , bgy + 93 + 10 + gap_h * (i%10) );
		m_help[i].SetPos( bgx + 16 + 50 , bgy + 93 + 10 );
	}


	m_tab_01.SetState(BTN_DOWN);

	if( g_dwClientCountry == CTRY_KOR ) // 한국만...
		m_LinkKrServiceLv.SetPosition(bgx + 16+50 , bgy + 93 + 10 + gap_h * 9);
}


void CControlHelp::Draw()
{
	if( !g_pDisplay )
		return;

	m_background.Draw();
	m_btn_close.Draw();

	if(m_cur_index != _NONE)
	{
		m_help[m_cur_index].Draw();
	}
	else
	{
		if(m_tab_01.GetState() == BTN_DOWN)
		{
			for(int i=0; i<10; i++)
				m_btn_link[i].Draw();
		}
		else if(m_tab_02.GetState() == BTN_DOWN)
		{
			for(int i=10; i<SYNTEX_ALL_NUM; i++)
				m_btn_link[i].Draw();

			if( g_dwClientCountry == CTRY_KOR )
				m_LinkKrServiceLv.Draw();
		}
	}

	m_tab_01.Draw();
	m_tab_02.Draw();

	m_btn_list.Draw();
	m_btn_prev.Draw();
	m_btn_next.Draw();

	DrawSyntex(m_cur_index);
}


void CControlHelp::DrawSyntex(int index)
{
	if( g_dwClientCountry == CTRY_KOR && m_bViewKrServiceLv ) // 한국만...
	{
		if( m_pSyntexKrServiceLv == NULL )
		{
			if( g_SvrType != ST_ADULT_ONLY )
				g_pDisplay->CreateSurfaceFromBitmap(&m_pSyntexKrServiceLv, "interface/help/help20.bmp" );
			else
				g_pDisplay->CreateSurfaceFromBitmap(&m_pSyntexKrServiceLv, "interface/help/help20-2.bmp" );

			if( m_pSyntexKrServiceLv )
			{
				m_pSyntexKrServiceLv->SetColorKey(RGB(0,  0, 0));

				{
					m_pSyntexKrServiceLv->Xpos = 312;
					m_pSyntexKrServiceLv->Ypos = 313;
				}
			}
		}

		if( !m_pServiceLvCloseBtn ) // 버튼도 새로 로드.
		{
			m_pServiceLvCloseBtn = new CBasicButton;

			if( m_pServiceLvCloseBtn )
			{
				m_pServiceLvCloseBtn->SetFileName("common/btn_close_01");
				m_pServiceLvCloseBtn->LoadRes();

				if( m_pSyntexKrServiceLv )
					m_pServiceLvCloseBtn->SetPosition(m_pSyntexKrServiceLv->Xpos+361, m_pSyntexKrServiceLv->Ypos+104);
			}
		}

		if( m_pSyntexKrServiceLv )
			g_pDisplay->Blt(m_pSyntexKrServiceLv->Xpos, m_pSyntexKrServiceLv->Ypos, m_pSyntexKrServiceLv);

		if( m_pServiceLvCloseBtn )
			m_pServiceLvCloseBtn->Draw();
	}
}

void CControlHelp::DeleteRes()
{
	int i = 0;

	m_background.DeleteRes();
	m_btn_close.DeleteRes();

	m_tab_01.DeleteRes();
	m_tab_02.DeleteRes();

	m_btn_list.DeleteRes();
	m_btn_prev.DeleteRes();
	m_btn_next.DeleteRes();

	for(i=0; i<SYNTEX_ALL_NUM; i++)
	{
		m_btn_link[i].DeleteRes();
		m_help[i].DeleteRes();
	}

	m_cur_index = _NONE;
}

void CControlHelp::SelectDeleteRes()
{
	m_cur_index = _NONE;

	// 심의 등급 표시
	if( m_pSyntexKrServiceLv )
		SAFE_DELETE( m_pSyntexKrServiceLv );

	// 심의 등급 표시
	if( m_pServiceLvCloseBtn )
		SAFE_DELETE( m_pServiceLvCloseBtn );
}


LRESULT CControlHelp::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i = 0, x = 0, y = 0;

	if( m_bViewKrServiceLv && m_pServiceLvCloseBtn && m_pSyntexKrServiceLv ) // 심의 등급 표시 창이 보이는 중이라면...
	{
		if( msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP )
		{
			x = LOWORD (lParam);
			y = HIWORD (lParam);

			if( IsInsideServiceLv(x, y) )
			{
				m_pServiceLvCloseBtn->MsgProc(hWnd, msg, wParam, lParam);
				if (m_pServiceLvCloseBtn->GetState() == BTN_ACTION)
				{
					m_bViewKrServiceLv = FALSE;

					// 리소스 지우자.
					// 심의 등급 표시
					if( m_pSyntexKrServiceLv )
					{
						SAFE_DELETE( m_pSyntexKrServiceLv );
						m_pSyntexKrServiceLv = NULL;
					}

					// 심의 등급 표시
					if( m_pServiceLvCloseBtn )
					{
						m_pServiceLvCloseBtn->DeleteRes();
						SAFE_DELETE( m_pServiceLvCloseBtn );
					}
				}
				return 1; // 영역안을 클릭했으면 딴거 무시.
			}
		}
		return 0;
	}

	switch ( msg )
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			if( m_tab_01.IsInside(x,y) )
			{
				// 1번창 버튼 클릭
				m_tab_01.SetState(BTN_DOWN);

				if(m_tab_02.GetState() != BTN_NORMAL)
					m_tab_02.SetState(BTN_NORMAL);
				m_cur_index = _NONE;
			}
			else if( m_tab_02.IsInside(x,y) )
			{
				// 2번창 버튼 클릭
				m_tab_02.SetState(BTN_DOWN);
				if(m_tab_01.GetState() != BTN_NORMAL)
					m_tab_01.SetState(BTN_NORMAL);
				m_cur_index = _NONE;
			}
		}

	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			m_btn_prev.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_next.MsgProc(hWnd, msg, wParam, lParam);
			m_btn_list.MsgProc(hWnd, msg, wParam, lParam);

			if(m_btn_prev.GetState() == BTN_ACTION)
			{
				if(m_tab_01.GetState() == BTN_DOWN)
				{
					//1번창일때.
					if(m_cur_index > -1)	// 뒤로이동-이전
						m_cur_index --;
				}
				else if(m_tab_02.GetState() == BTN_DOWN)
				{
					//2번창일때.
					if(m_cur_index > -1 && m_cur_index>(SYNTEX_ALL_NUM+1)/2)
						m_cur_index --;
					else if(m_cur_index <= (SYNTEX_ALL_NUM+1)/2)
						m_cur_index=-1;
				}
				m_btn_prev.m_iBtnFlg = BTN_NORMAL;
			}

			if(m_btn_next.GetState() == BTN_ACTION)
			{
				if(m_tab_01.GetState() == BTN_DOWN)
				{
					if(m_cur_index < (SYNTEX_ALL_NUM-1)/2)	// 앞으로이동-다음
						m_cur_index ++;					 //
				}
				else if(m_tab_02.GetState() == BTN_DOWN)
				{
					if(m_cur_index==-1)
						m_cur_index=9;

					if(m_cur_index < SYNTEX_ALL_NUM-1)
						m_cur_index ++;
				}

				m_btn_next.m_iBtnFlg = BTN_NORMAL;
			}


			if(m_btn_list.GetState() == BTN_ACTION)
			{
				m_cur_index = _NONE;
				m_btn_list.m_iBtnFlg = BTN_NORMAL;
			}


			m_btn_close.MsgProc(hWnd, msg, wParam, lParam);
			if (m_btn_close.GetState() == BTN_ACTION)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowHelpWindow(FALSE);
				m_btn_close.SetState(BTN_NORMAL);
				return 1;
			}


			if(m_tab_01.GetState() == BTN_DOWN)
			{
				//1번창일때.
				for(i=0; i<10; i++)
				{
					if(m_cur_index == _NONE)
					{
						m_btn_link[i].MsgProc(hWnd, msg, wParam, lParam);
						if(m_btn_link[i].GetState() ==BTN_ACTION)
						{
							m_cur_index = i;
							m_btn_link[i].m_iBtnFlg = BTN_NORMAL;
						}
					}
				}

			}
			else if(m_tab_02.GetState() == BTN_DOWN)
			{
				for(i=10; i<SYNTEX_ALL_NUM; i++)
				{
					if(m_cur_index == _NONE)
					{
						m_btn_link[i].MsgProc(hWnd, msg, wParam, lParam);
						if(m_btn_link[i].GetState() ==BTN_ACTION)
						{
							m_cur_index = i;
							m_btn_link[i].m_iBtnFlg = BTN_NORMAL;
						}
					}
				}

				if( g_dwClientCountry == CTRY_KOR ) // 한국만...
				{
					if(m_cur_index == _NONE)
					{
						m_LinkKrServiceLv.MsgProc(hWnd, msg, wParam, lParam);
						if(m_LinkKrServiceLv.GetState() ==BTN_ACTION ) ///아케론심의등급
						{
							m_bViewKrServiceLv = TRUE;
							m_LinkKrServiceLv.m_iBtnFlg = BTN_NORMAL;
						}
					}
				}
			}
		}
		else
			return 0;

		return 1;

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
			return 1;

		break;
	}

	return 0;
}


BOOL CControlHelp::IsInside(int x, int y)
{
	return m_background.IsIn( x, y );
}

BOOL CControlHelp::IsInsideServiceLv(int x, int y)
{
	if(!m_pSyntexKrServiceLv)
		return FALSE;

	return m_pSyntexKrServiceLv->IsIn( x, y );
}

