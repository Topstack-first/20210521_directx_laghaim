#include "stdafx.h"
// ControlDocument.cpp: implementation of the CControlDocument class.
//
//////////////////////////////////////////////////////////////////////


#include "headers.h"
#include "main.h"
#include "ControlDocument.h"
#include "NkCharacter.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "ListBox.h"
#include "UIMgr.h"
#include "g_stringmanager.h"
#include "skill.h"








extern HINSTANCE			g_hDllInst;


CControlDocument::CControlDocument()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_slot_index(0)
{
	memset(m_MsgBuf, 0, sizeof(m_MsgBuf));
	memset(m_SocialActNum, 0, sizeof(m_SocialActNum));

	m_pSurfaceBack = NULL;
	m_DocumentList = NULL;
	m_nDocument = 0;
	m_nSel = -1;
	Init();
}

CControlDocument::~CControlDocument()
{
	DeleteRes();
	SAFE_DELETE( m_DocumentList );
}

void CControlDocument::Init()
{
	m_CloseBtn.SetFileName("common/btn_close_01");

	m_DocumentList = new CListBox(" ", 4, 54, 210+20, 52 );
	if( m_DocumentList )
	{
		m_DocumentList->SetImage("common/scroll_down", "common/scroll_up", "interface/common/scroll_thumb_02.bmp");
		m_DocumentList->SetSelectedLine(-1);
		m_DocumentList->SetTextColor(DOCUMENT_LIST_FCOLOR);
	}
}

void CControlDocument::LoadRes()
{
	if( g_pNk2DFrame )
	{
		m_ScreenWidth = g_pNk2DFrame->GetClientWidth();
		m_ScreenHeight = g_pNk2DFrame->GetClientHeight();
	}

	SAFE_DELETE(m_pSurfaceBack);

	g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/Document/Document_back.bmp" );
	if( m_pSurfaceBack )
	{
		m_pSurfaceBack->SetColorKey(RGB(0, 0, 0));
		m_pSurfaceBack->Xpos = (g_pNk2DFrame->GetClientWidth()-m_pSurfaceBack->GetWidth())>>1;
		m_pSurfaceBack->Ypos = (g_pNk2DFrame->GetClientHeight()-m_pSurfaceBack->GetHeight())>>1;;

		m_CloseBtn.PosX = m_pSurfaceBack->Xpos+202;
		m_CloseBtn.PosY = m_pSurfaceBack->Ypos+5;
		m_CloseBtn.LoadRes();
	}

	if(m_DocumentList)
	{
		m_DocumentList->SetPosDim(m_pSurfaceBack->Xpos+4, m_pSurfaceBack->Ypos+54, 210, 52);
		m_DocumentList->LoadRes();
	}
}

void CControlDocument::DeleteRes()
{
	SAFE_DELETE(m_pSurfaceBack);

	m_CloseBtn.DeleteRes();
	m_DocumentList->DeleteRes();
}

void CControlDocument::Draw()
{
	if (g_pDisplay && m_pSurfaceBack)
		g_pDisplay->Blt(m_pSurfaceBack->Xpos, m_pSurfaceBack->Ypos, m_pSurfaceBack);

	m_CloseBtn.Draw();
	if( m_DocumentList )	m_DocumentList->Draw();
}

BOOL CControlDocument::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return false;

	return m_pSurfaceBack->IsIn(x,y);
}

LRESULT CControlDocument::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y, sel;

	switch ( msg )
	{
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);
		m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
		if (m_CloseBtn.GetState() == BTN_ACTION)
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterfaceWindow(FALSE, DOCUMENT);
			m_CloseBtn.SetState(BTN_NORMAL);
		}
		if( m_DocumentList )
			m_DocumentList->GetMessage(hWnd, msg, wParam, lParam );

		if (!IsInside(x, y))
			return 0;

		return 1;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (IsInside(x, y))
		{
			// Msg Popup창이 떠있을때는 존 리스트가 클릭이 안되게 리턴~
			if( g_pNk2DFrame->Check_All_MsgPopUp() )
				return 1;

			if( g_pRoh->m_LevelupPoint <= 0)
			{
				// IDS_POINT_NOTENOUGH	필요 스킬 포인트가 부족합니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_POINT_NOTENOUGH), TYPE_NOR_OK, 1 );
				g_pNk2DFrame->ShowInterfaceWindow(FALSE,DOCUMENT);
				return 1;
			}

			if (m_CloseBtn.IsInside(x, y))
			{
				m_CloseBtn.MsgProc(hWnd, msg, wParam, lParam);
			}

			if (m_DocumentList
					&& (sel = m_DocumentList->GetMessage(hWnd, msg, wParam, lParam )) >= 0)
			{
				if( g_pNk2DFrame )
					g_pNk2DFrame->ShowInterfaceWindow(FALSE,DOCUMENT);
				m_CloseBtn.SetState(BTN_NORMAL);

				if (!(pCMyApp->GetHeroSwimming()))
				{
					pCMyApp->StopHero();

					if ( g_pTcpIp && !g_pRoh->IsDead() )
					{
						if( m_nDocument == 1 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[107].GetMoney(g_ChaSkill.m_SkillLevel[17] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								// IDS_LESSON_REALY 정말 배우시겠습니까?
								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 2 )
						{
							if(sel == 0)
							{

								{
									if( g_pRoh->m_Money < g_Skills[119].GetMoney(g_ChaSkill.m_SkillLevel[19] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								// IDS_LESSON_REALY 정말 배우시겠습니까?
								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 3 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[107].GetMoney( g_ChaSkill.m_SkillLevel[17] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}
								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if(sel == 1)
							{
								{
									if( g_pRoh->m_Money < g_Skills[119].GetMoney( g_ChaSkill.m_SkillLevel[19] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[108].GetMoney(g_ChaSkill.m_SkillLevel[27] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 11 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[120].GetMoney(g_ChaSkill.m_SkillLevel[29] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 12 )
						{
							if( sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[108].GetMoney( g_ChaSkill.m_SkillLevel[27] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[120].GetMoney( g_ChaSkill.m_SkillLevel[29] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 100 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[110].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 101 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[122].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 102 )
						{
							if( sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[110].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[122].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 1000 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[109].GetMoney( g_ChaSkill.m_SkillLevel[17] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 1001 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[121].GetMoney( g_ChaSkill.m_SkillLevel[19] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 1002 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[109].GetMoney( g_ChaSkill.m_SkillLevel[17] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if(sel == 1)
							{
								{
									if( g_pRoh->m_Money < g_Skills[121].GetMoney( g_ChaSkill.m_SkillLevel[19] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10000 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10001 )
						{
							if(sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10002 )
						{
							if( sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10003 )
						{
							if( sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10004 )
						{
							if( sel == 0)
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10005 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10006 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10007 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10008 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10009 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10010 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 2 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10011 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 2 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10012 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 2 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10013 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 2 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10014 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 2 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 3 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
						else if( m_nDocument == 10015 )
						{
							if( sel == 0 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[111].GetMoney( g_ChaSkill.m_SkillLevel[20] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 1 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[112].GetMoney( g_ChaSkill.m_SkillLevel[21] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 2 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[123].GetMoney( g_ChaSkill.m_SkillLevel[22] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
							else if( sel == 3 )
							{
								{
									if( g_pRoh->m_Money < g_Skills[124].GetMoney( g_ChaSkill.m_SkillLevel[23] + 1 ) )
									{
										g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_INTERCHA_NOT_ENOUGH_MONEY), TYPE_NOR_OK, 1 );
										return 1;
									}
								}

								g_pNk2DFrame->InsertPopup((char*)G_STRING(IDS_LESSON_REALY),TYPE_NOR_OKCANCLE, 27, 30, m_nDocument);
							}
						}
					}
				}
				m_nSel = sel;
				pCMyApp->SetCurVillageID(sel);
			}
		}
		else
		{
			if( g_pNk2DFrame )
				g_pNk2DFrame->ShowInterfaceWindow(FALSE, DOCUMENT);
			return 0;
		}
		return 1;
	}
	return 0;
}

void CControlDocument::SendMsg()
{
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(m_MsgBuf);
}


void CControlDocument::SetDocument( int nDocument )
{
	char strTemp[255];
	m_nDocument = nDocument;
	SAFE_DELETE(m_DocumentList);
	if ( m_DocumentList == NULL )
	{
		m_DocumentList = new CListBox(" ", 4, 54, 210, 52 );
		if( m_DocumentList )
		{
			m_DocumentList->SetImage("Document/Document_arrowdown", "Document/Document_arrowup", "interface/Document/Document_ScrollBar.bmp");
			m_DocumentList->SetSelectedLine(-1);
			m_DocumentList->SetTextColor(DOCUMENT_LIST_FCOLOR);
		}

		if( !m_DocumentList )
			return;

		if ( nDocument == 1 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_B_FLYINGCRASH].GetName());
			m_DocumentList->AddString(strTemp);
		}
		else if( nDocument == 2 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_B_PROTECTION_SHIELDS].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if( nDocument == 3 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_B_FLYINGCRASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_B_PROTECTION_SHIELDS].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_K_POISON].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 11 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_K_ICEBOLT].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 12 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_K_POISON].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_K_ICEBOLT].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 100 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_A_SUMMON_FLOWER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 101 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_A_RESURRECTION].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 102 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_A_SUMMON_FLOWER].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_A_RESURRECTION].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 1000 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_H_ELECTRICSHOT].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 1001 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_H_FROZENSHOT].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 1002 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_H_ELECTRICSHOT].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_H_FROZENSHOT].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10000 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10001 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10002 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10003 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10004 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10005 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10006 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10007 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10008 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10009 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10010 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10011 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10012 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10013 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10014 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		else if ( nDocument == 10015 )
		{
			sprintf(strTemp, "%s", g_Skills[SKILL_F_SHADOWSLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_ADRENALINE].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_MERCERNERY_SLASH].GetName() );
			m_DocumentList->AddString(strTemp);
			sprintf(strTemp, "%s", g_Skills[SKILL_F_TORNADO_HAMMER].GetName() );
			m_DocumentList->AddString(strTemp);
		}
		//m_DocumentList->SetPosDim(DOCUMENT_LIST_X, DOCUMENT_LIST_Y, DOCUMENT_LIST_WIDTH, DOCUMENT_LIST_HEIGHT);
		m_DocumentList->SetPosDim(m_pSurfaceBack->Xpos+4, m_pSurfaceBack->Ypos+54, 220, 56);
		m_DocumentList->LoadRes();
	}
}

LRESULT CControlDocument::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if( nMsg == 1 && g_pTcpIp )
	{
		if( m_slot_index < 0 )
			return 0;

		int skill_no = -1;

		if( m_nDocument == 1 )
			skill_no = 107;
		else if( m_nDocument == 2 )
			skill_no = 119;
		else if( m_nDocument == 3 )
		{
			if( m_nSel == 0 )
				skill_no = 107;
			else if( m_nSel == 1 )
				skill_no = 119;
		}
		else if( m_nDocument == 10 )
			skill_no = 108;
		else if( m_nDocument == 11 )
			skill_no = 120;
		else if( m_nDocument == 12 )
		{
			if( m_nSel == 0 )
				skill_no = 108;
			else if( m_nSel == 1 )
				skill_no = 120;
		}
		else if( m_nDocument == 100 )
			skill_no = 110;
		else if( m_nDocument == 101 )
			skill_no = 122;
		else if( m_nDocument == 102 )
		{
			if( m_nSel == 0 )
				skill_no = 110;
			else if( m_nSel == 1 )
				skill_no = 122;
		}
		else if( m_nDocument == 1000 )
			skill_no = 109;
		else if( m_nDocument == 1001 )
			skill_no = 121;
		else if( m_nDocument == 1002 )
		{
			if( m_nSel == 0 )
				skill_no = 109;
			else if( m_nSel == 1 )
				skill_no = 121;
		}
		else if( m_nDocument == 10000 )
			skill_no = 111;
		else if( m_nDocument == 10001 )
			skill_no = 112;
		else if( m_nDocument == 10002 )
			skill_no = 123;
		else if( m_nDocument == 10003 )
			skill_no = 124;
		else if( m_nDocument == 10004 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 112;
		}
		else if( m_nDocument == 10005 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 123;
		}
		else if( m_nDocument == 10006 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 124;
		}
		else if( m_nDocument == 10007 )
		{
			if( m_nSel == 0 )
				skill_no = 112;
			else if( m_nSel == 1 )
				skill_no = 123;
		}
		else if( m_nDocument == 10008 )
		{
			if( m_nSel == 0 )
				skill_no = 112;
			else if( m_nSel == 1 )
				skill_no = 124;
		}
		else if( m_nDocument == 10009 )
		{
			if( m_nSel == 0 )
				skill_no = 123;
			else if( m_nSel == 1 )
				skill_no = 124;
		}
		else if( m_nDocument == 10010 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 112;
			else if( m_nSel == 2 )
				skill_no = 123;
		}
		else if( m_nDocument == 10011 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 112;
			else if( m_nSel == 2 )
				skill_no = 124;
		}
		else if( m_nDocument == 10012 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 123;
			else if( m_nSel == 2 )
				skill_no = 124;
		}
		else if( m_nDocument == 10013 )
		{
			if( m_nSel == 0 )
				skill_no = 112;
			else if( m_nSel == 1 )
				skill_no = 123;
			else if( m_nSel == 2 )
				skill_no = 124;
		}
		else if( m_nDocument == 10014 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 112;
			else if( m_nSel == 2 )
				skill_no = 123;
			else if( m_nSel == 3 )
				skill_no = 124;
		}
		else if( m_nDocument == 10015 )
		{
			if( m_nSel == 0 )
				skill_no = 111;
			else if( m_nSel == 1 )
				skill_no = 112;
			else if( m_nSel == 2 )
				skill_no = 123;
			else if( m_nSel == 3 )
				skill_no = 124;
		}

		if( skill_no == -1 )
			return 0;

		char param[32];
		param[0] = 0;
		sprintf( param , "%d" , skill_no );
		pCMyApp->m_pUIMgr->UseDocument( m_slot_index , param );
	}
	return 0L;
}

