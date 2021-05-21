#include "stdafx.h"
#include "SetValuePage.h"
#include "Sheet.h"
#include "headers.h"
#include "DragBox.h"
#include "TextOutBox.h"
#include "createcharacterpage.h"
#include "SelectCharacterpage.h"
#include "g_stringmanager.h"

#include "main.h"

#define COMPLETE	1
#define GOBACK	2
#define AVERAGE_RVALUE  80
#define BACKGROUND_SETVALUEPAGE		"interface/common/back_setvaluepage.bmp" // 미사용(없음)

#define LIMITBAR_IMAGE				"interface/MakeChar/limitbar.bmp"
#define DRAGBTN_NORMAL				"interface/MakeChar/dragbtn_normal.bmp"
#define DRAGBTN_CLICK				"interface/MakeChar/dragbtn_click.bmp"





//--------------------------------------------------------------------------------------------
CSetValuePage::CSetValuePage()
{
	m_pSheet = NULL;	

	Init();
}

CSetValuePage::CSetValuePage( CSheet* pSheet )
{
	m_pSheet = NULL;
	SetSheetPtr( pSheet );	

	Init();
}

CSetValuePage::~CSetValuePage()
{
	DeleteRes();
}

void CSetValuePage::SetSheetPtr( CSheet* pSheet )
{
	m_pSheet = pSheet;

}

void CSetValuePage::Init()
{
	// Drag Box
	m_StrDrag.Init(587, 148+12 , SETVAL_DRAG_WIDTH, DRAGBTN_NORMAL, DRAGBTN_CLICK,  LIMITBAR_IMAGE);
	m_StrDrag.SetValues(5, 30, 15);

	m_IntDrag.Init(587, 178+12 , SETVAL_DRAG_WIDTH, DRAGBTN_NORMAL, DRAGBTN_CLICK,  LIMITBAR_IMAGE);
	m_IntDrag.SetValues(5, 30, 20);

	m_DexDrag.Init(587, 210+12 , SETVAL_DRAG_WIDTH, DRAGBTN_NORMAL, DRAGBTN_CLICK,  LIMITBAR_IMAGE);
	m_DexDrag.SetValues(5, 30, 15);

	m_ConDrag.Init(587, 240+12 , SETVAL_DRAG_WIDTH, DRAGBTN_NORMAL, DRAGBTN_CLICK,  LIMITBAR_IMAGE);
	m_ConDrag.SetValues(5, 30, 15);

	m_ChaDrag.Init(587, 271+12 , SETVAL_DRAG_WIDTH, DRAGBTN_NORMAL, DRAGBTN_CLICK,  LIMITBAR_IMAGE);
	m_ChaDrag.SetValues(5, 30, 15);

	// State
	m_StrBox.Init(SETVAL_STAT_FSIZE, SETVAL_STAT_FCOLOR,	749, 154);
	m_IntBox.Init(SETVAL_STAT_FSIZE, SETVAL_STAT_FCOLOR,	749, 185);
	m_DexBox.Init(SETVAL_STAT_FSIZE, SETVAL_STAT_FCOLOR,	749, 216 );
	m_ConBox.Init(SETVAL_STAT_FSIZE, SETVAL_STAT_FCOLOR,	749, 247 );
	m_ChaBox.Init(SETVAL_STAT_FSIZE, SETVAL_STAT_FCOLOR,	749, 278 );

	m_LeftPoint.Init(SETVAL_STAT_FSIZE, SETVAL_STAT_FCOLOR,	628, 306);

	// Value Box
	m_VitalBox.Init(SETVAL_VAL_FSIZE, SETVAL_VITL_FCOLOR,
					233, 438 );
	m_ManaBox.Init(SETVAL_VAL_FSIZE, SETVAL_MANA_FCOLOR,
				   472, 438 );
	m_StaminaBox.Init(SETVAL_VAL_FSIZE, SETVAL_STAM_FCOLOR,
					  233, 474 );
	m_EpowerBox.Init(SETVAL_VAL_FSIZE, SETVAL_EPOW_FCOLOR,
					 472, 474 );

	m_MsgPopUp.Init(TYPE_NOR_OK);
}



void CSetValuePage::SetBackground()
{
}


void CSetValuePage::LoadRes()
{
	SetBackground();	

	m_StrDrag.LoadRes();
	m_IntDrag.LoadRes();
	m_DexDrag.LoadRes();
	m_ConDrag.LoadRes();
	m_ChaDrag.LoadRes();

	// Stat Box
	m_MsgPopUp.LoadRes();

	CalcVal();
}

void CSetValuePage::DeleteRes()
{
	// DragBar
	m_StrDrag.DeleteRes();
	m_IntDrag.DeleteRes();
	m_DexDrag.DeleteRes();
	m_ConDrag.DeleteRes();
	m_ChaDrag.DeleteRes();

	m_StrBox.DeleteRes();
	m_IntBox.DeleteRes();
	m_DexBox.DeleteRes();
	m_ConBox.DeleteRes();
	m_ChaBox.DeleteRes();
	m_LeftPoint.DeleteRes();

	m_VitalBox.DeleteRes();
	m_ManaBox.DeleteRes();
	m_StaminaBox.DeleteRes();
	m_EpowerBox.DeleteRes();
	
	m_MsgPopUp.DeleteRes();
}

void CSetValuePage::Draw()
{	
	m_StrBox.SetString(m_StrDrag.GetCurPos());
	m_IntBox.SetString(m_IntDrag.GetCurPos());
	m_DexBox.SetString(m_DexDrag.GetCurPos());
	m_ConBox.SetString(m_ConDrag.GetCurPos());
	m_ChaBox.SetString(m_ChaDrag.GetCurPos());

	int Res = AVERAGE_RVALUE-(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos());
	if(Res >=10) 	// 숫자가 두자리수가 됬을때는 위치를 약간 바꾸어주어야 한다{
	{
		m_LeftPoint.m_PosX=360;
		m_LeftPoint.m_PosY=387;
	}
	else
	{
		m_LeftPoint.m_PosX=360;
		m_LeftPoint.m_PosY=387;
	}

	m_LeftPoint.SetString(Res);

	m_StrDrag.Draw();
	m_IntDrag.Draw();
	m_DexDrag.Draw();
	m_ConDrag.Draw();
	m_ChaDrag.Draw();

	m_StrBox.Draw();
	m_IntBox.Draw();
	m_DexBox.Draw();
	m_ConBox.Draw();
	m_ChaBox.Draw();


	m_LeftPoint.Draw(628, 310, 140);
	m_VitalBox.Draw(233, 442, 140);
	m_ManaBox.Draw(472, 442, 140);
	m_StaminaBox.Draw(233, 478, 140);
	m_EpowerBox.Draw(472, 478, 140);

	if (timeGetTime() - m_pSheet->m_dwDummyBaseTime > 180000)
		m_pSheet->SendDummyMsg();

	m_MsgPopUp.Draw();
}

void CSetValuePage::CalcVal()
{
	m_VitalBox.SetString(m_StrDrag.GetCurPos()+m_ConDrag.GetCurPos()*2);
	m_ManaBox.SetString(m_IntDrag.GetCurPos()*2+m_DexDrag.GetCurPos());
	m_StaminaBox.SetString(m_StrDrag.GetCurPos()*2+m_ConDrag.GetCurPos());
	m_EpowerBox.SetString(m_DexDrag.GetCurPos()*2+m_IntDrag.GetCurPos());
}

void CSetValuePage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int ret;
	// 현제 메세지가 들어왔는가??
	if(m_MsgPopUp.CheckMsgExist() == TRUE)
	{	
		if ((ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam)) == 1)
		{	
		}
		else if (ret == -1)
		{

			return;
		}
		return ;
	}

	switch ( msg )
	{

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		m_StrDrag.MsgProc( hWnd, msg, wParam, lParam );
		m_IntDrag.MsgProc( hWnd, msg, wParam, lParam );
		m_DexDrag.MsgProc( hWnd, msg, wParam, lParam );
		m_ConDrag.MsgProc( hWnd, msg, wParam, lParam );
		m_ChaDrag.MsgProc( hWnd, msg, wParam, lParam );
		break;

	case WM_MOUSEMOVE:
		int ret;
		if ((ret = m_StrDrag.MsgProc( hWnd, msg, wParam, lParam )) >= 0)
		{
			m_StrBox.SetString(ret);
			Complex(0);
			CalcVal();
		}
		else if ((ret = m_IntDrag.MsgProc( hWnd, msg, wParam, lParam )) >= 0)
		{
			m_IntBox.SetString(ret);
			Complex(1);
			CalcVal();
		}
		else if ((ret = m_DexDrag.MsgProc( hWnd, msg, wParam, lParam )) >= 0)
		{
			m_DexBox.SetString(ret);
			Complex(2);
			CalcVal();
		}
		else if ((ret = m_ConDrag.MsgProc( hWnd, msg, wParam, lParam )) >= 0)
		{
			m_ConBox.SetString(ret);
			Complex(3);
			CalcVal();
		}
		else if ((ret = m_ChaDrag.MsgProc( hWnd, msg, wParam, lParam )) >= 0)
		{
			m_ChaBox.SetString(ret);
			Complex(4);
			CalcVal();
		}

		break;
	}
}



BOOL CSetValuePage::Complete()
{
	m_pSheet->m_TempSlot.m_Str = m_StrDrag.GetCurPos();
	m_pSheet->m_TempSlot.m_Int = m_IntDrag.GetCurPos();
	m_pSheet->m_TempSlot.m_Dex = m_DexDrag.GetCurPos();
	m_pSheet->m_TempSlot.m_Con = m_ConDrag.GetCurPos();
	m_pSheet->m_TempSlot.m_Cha = m_ChaDrag.GetCurPos();	

	if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()
			+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() < 80)
	{
		//-- IDS_SETVALUE : 각 힘의 종합이 기본능력치(80)보다 적습니다
		m_MsgPopUp.PutString((char*)G_STRING(IDS_SETVALUE));
		return 0;
	}

	char *msg;
	msg = m_pSheet->SendNewChar(0);
		
	if(msg)
	{
		if( strcmp(msg, "fail") == 0 )
		{
			m_MsgPopUp.PutString((char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION));
			return 0;
		}

		m_pSheet->GoPage( SELCHARPAGE );
	}
	else
	{
		if( m_pSheet->UpdateChars() == -1 )
		{
			m_MsgPopUp.PutString((char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION));
			return 0;
		}
		m_pSheet->GoPage( SELCHARPAGE );
		m_pSheet->m_bEmptySloat = FALSE;		
	}

	return 1;
}




void CSetValuePage::SetCharitisics(int Str, int Int, int Dex, int Con, int Cha)
{
	m_StrDrag.SetCurPos(Str);
	m_IntDrag.SetCurPos(Int);
	m_DexDrag.SetCurPos(Dex);
	m_ConDrag.SetCurPos(Con);
	m_ChaDrag.SetCurPos(Cha);

	CalcVal();
}


void CSetValuePage::SetDragBarPos(int x0, int x1, int x2, int x3, int x4)
{
	m_StrDrag.SetDragBar(x0);
	m_IntDrag.SetDragBar(x1);
	m_DexDrag.SetDragBar(x2);
	m_ConDrag.SetDragBar(x3);
	m_ChaDrag.SetDragBar(x4);
}






void CSetValuePage::Complex(int Case)
{
	switch(Case)
	{
	case 0:
		if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
		{
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_IntDrag.SetCurPos(m_IntDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_DexDrag.SetCurPos(m_DexDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ConDrag.SetCurPos(m_ConDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ChaDrag.SetCurPos(m_ChaDrag.GetCurPos()-1) ;
			}
		}

		break;

	case 1:
		if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
		{
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_StrDrag.SetCurPos(m_StrDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_DexDrag.SetCurPos(m_DexDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ConDrag.SetCurPos(m_ConDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ChaDrag.SetCurPos(m_ChaDrag.GetCurPos()-1) ;
			}
		}
		break;

	case 2:
		if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
		{
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{

				if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
				{
					m_StrDrag.SetCurPos(m_StrDrag.GetCurPos()-1) ;
				}
				if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
				{
					m_IntDrag.SetCurPos(m_IntDrag.GetCurPos()-1) ;
				}
				if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
				{
					m_ConDrag.SetCurPos(m_ConDrag.GetCurPos()-1) ;
				}
				if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
				{
					m_ChaDrag.SetCurPos(m_ChaDrag.GetCurPos()-1) ;
				}
			}
		}
		break;

	case 3:
		if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
		{

			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_StrDrag.SetCurPos(m_StrDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_IntDrag.SetCurPos(m_IntDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_DexDrag.SetCurPos(m_DexDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ChaDrag.SetCurPos(m_ChaDrag.GetCurPos()-1) ;
			}
		}
		break;

	case 4:
		if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
		{
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_StrDrag.SetCurPos(m_StrDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_IntDrag.SetCurPos(m_IntDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_DexDrag.SetCurPos(m_DexDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ConDrag.SetCurPos(m_ConDrag.GetCurPos()-1) ;
			}
		}
		break;
	case 5:
		if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
		{
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_StrDrag.SetCurPos(m_StrDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_IntDrag.SetCurPos(m_IntDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_DexDrag.SetCurPos(m_DexDrag.GetCurPos()-1) ;
			}
			if(m_StrDrag.GetCurPos()+m_IntDrag.GetCurPos()+m_DexDrag.GetCurPos()+m_ConDrag.GetCurPos()+m_ChaDrag.GetCurPos() > 80)
			{
				m_ConDrag.SetCurPos(m_ConDrag.GetCurPos()-1) ;
			}
		}
		break;
	}
}

HRESULT CSetValuePage::Restore()
{
	HRESULT hr = S_OK;

	m_StrBox.Restore();
	m_IntBox.Restore();
	m_DexBox.Restore();
	m_ConBox.Restore();
	m_ChaBox.Restore();
	m_LeftPoint.Restore();
	m_VitalBox.Restore();
	m_ManaBox.Restore();
	m_StaminaBox.Restore();
	m_EpowerBox.Restore();

	return hr;
}