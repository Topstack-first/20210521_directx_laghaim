#include "stdafx.h"


#include "main.h"
#include "resource.h"
#include <commctrl.h> //-- Tab Control
#include "UIMgr.h"
#include "g_devlogbox.h"
#include "Skill.h"
#include <direct.h>

#define MAX_NUM_OF_SEARCH 14
#define MAX_NUM_OF_LIST   400
#define COODI_FLAG_Y 18


bool g_bWndNetMessage = false;     //-- 창을 보일 것인가.

extern HWND	g_hWnd;                //-- Parent Hwnd
//-- App (instanc / Skill 를 위해)

extern BOOL g_bAdmin2;
HWND	hWndDevWnd;                  //-- main Window HWND
HWND  hTabDlg;                     //-- Tab 에 따라 Loading 될Dlg


int l_tab_sel = 0;       //-- 선택된 Tab 번호

bool b_pause	= false;    //-- Message list 추가 중지
bool b_srh_svr	= false;  //-- Server Message list 검색
bool b_srh_cli	= false;  //-- Client Message list 검색
char comp_s[100] = {0,}; //-- 클라이언트 검색 String
char comp_c[100] = {0,}; //-- 서버 검색 String

// [2008/5/8 Theodoric] tab 3 -> point log
bool b_EXP	= false;
bool b_Lime = false;
bool b_Damage = false;
bool b_PointListPause = false;
DWORD	tab3_EXP		= 0;
DWORD	tab3_Lime		= 0;
DWORD	tab3_Dmg_base	= 0;
DWORD	tab3_Dmg_dam	= 0;
DWORD	tab3_Dmg_Last	= 0;


int  num_stop = 0;       //-- 검색하는 문자열 발견 후 멈추기 까지 Step
//-- 2 이상이면 감소, 0이면 Dont care
//-- 1 이면 Pause 후 0으로
bool chk_silence = false;//-- msg 싸일런스
//----------------------------------------------------------------------------
//-- Skill
int Selected_Skill = 0;
int Selected_Skill_Distance = 0;

//-- GUI 를 위한 각종 변수.
HFONT hfont, oldfont;
HBRUSH bRed,bBlue,bGreen,bYellow,bNormal,bBrown,bWblue,oldbrush;

void SaveListString(HWND hWnd, int idDlgItem);

//----------------------------------------------------------------------------
//-- G_CreateDlg
//----------------------------------------------------------------------------
//-- Dlg를 생성한다.
//-- Property Sheet 생성에 관련된 작업
void G_DevCreateDlg()
{

	TCITEM tie;

	if(g_bWndNetMessage)return;

	//-- Main Dialog Wnd 를 생성한다.
	hWndDevWnd = CreateDialog(pCMyApp->hInstApp, MAKEINTRESOURCE(IDD_DEV_MAIN),
							  g_hWnd, (DLGPROC)Proc_Main);


	//-- Tab 버튼에 String 추가.
	HWND hwndtab = GetDlgItem(hWndDevWnd, IDC_TAB1);

	tie.mask=TCIF_TEXT;

	tie.pszText="Net Log";  //-- Network Message Log
	TabCtrl_InsertItem(hwndtab,0,&tie);

	tie.pszText="Skill Flag ";      //-- Skill Flag View
	TabCtrl_InsertItem(hwndtab,1,&tie);

	tie.pszText="String ";      //-- Strng Message Log
	TabCtrl_InsertItem(hwndtab,2,&tie);

	tie.pszText="Point Log ";      //-- Strng Message Log
	TabCtrl_InsertItem(hwndtab,3,&tie);

	//-- Default Page
	hTabDlg=CreateDialog(pCMyApp->hInstApp,MAKEINTRESOURCE(IDD_DEV_NETLOG),hWndDevWnd,Proc_DevNet);
	ShowWindow(hTabDlg,SW_SHOW);


	//-- Flag Setting
	g_bWndNetMessage = true;
	l_tab_sel = 0;
	ShowWindow(hWndDevWnd,SW_SHOW);
}

//-----------------------------------------------------------------------------
//-- Proc_Main
//-----------------------------------------------------------------------------
//-- Wnd 의 main Proc
BOOL CALLBACK Proc_Main( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

	HWND htab = GetDlgItem(hWndDevWnd, IDC_TAB1);
	switch (nMsg)
	{
	case WM_COMMAND:
		//----------------------------------------------------------------------
		//-- COMMAND

		/*switch(LOWORD(wParam))
		{
		case IDC_CLOSE :
		  g_bWndNetMessage = false;
		  EndDialog(hWndDevWnd, FALSE);
		}*/
		break;

	case WM_CLOSE :
		//------------------------------------------------------------------------
		//-- Close
		g_bWndNetMessage = false;
		EndDialog(hWndDevWnd, FALSE);
		break;

	case WM_NOTIFY:
		//------------------------------------------------------------------------
		//-- tab 으로 부터의 Message
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			//-- 이미 존재하면 삭제
			if (hTabDlg) DestroyWindow(hTabDlg);

			//-- 현재 선택된 Tab
			l_tab_sel=TabCtrl_GetCurSel(htab);

			//-- Tab 에 따른 Wnd 변환
			switch(l_tab_sel)
			{
			case 0:
				hTabDlg=CreateDialog(pCMyApp->hInstApp,MAKEINTRESOURCE(IDD_DEV_NETLOG),hWndDevWnd,Proc_DevNet);
				ShowWindow(hTabDlg,SW_SHOW);
				break;

			case 1:
				hTabDlg=CreateDialog(pCMyApp->hInstApp,MAKEINTRESOURCE(IDD_DEV_SKILL),hWndDevWnd,Proc_DevSkill);
				ShowWindow(hTabDlg,SW_SHOW);
				break;

			case 2:
				hTabDlg=CreateDialog(pCMyApp->hInstApp,MAKEINTRESOURCE(IDD_DEV_STRING),hWndDevWnd,Proc_DevString);
				ShowWindow(hTabDlg,SW_SHOW);
				break;

			case 3:
				hTabDlg=CreateDialog(pCMyApp->hInstApp,MAKEINTRESOURCE(IDD_DEV_POINTLOG),hWndDevWnd,Proc_DevPointLog);
				ShowWindow(hTabDlg,SW_SHOW);
				break;
			}
			break;
		}
		break;



	}
	return 0;

}


//-----------------------------------------------------------------------------
//-- Proc_DevSkill
//-----------------------------------------------------------------------------
//-- Property sheet 중
//-- String Load sheet 의 Msg 를 처리한다.
BOOL CALLBACK Proc_DevSkill( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

	char strtmp[100]= {0,};

	HWND htab = GetDlgItem(hWndDevWnd, IDC_TAB1);

	RECT prt;

	switch (nMsg)
	{

	case WM_DESTROY:
		//------------------------------------------------------------------------
		//-- Destroy
		KillTimer(hWnd,1); //-- Settimer

		DeleteObject(bRed);
		DeleteObject(bBlue);
		DeleteObject(bGreen);
		DeleteObject(bYellow);
		DeleteObject(bBrown);
		DeleteObject(hfont);
		DeleteObject(bWblue);
		break;

	case WM_INITDIALOG:
		//------------------------------------------------------------------------
		//-- InitDialog
		GetWindowRect(htab,&prt);
		TabCtrl_AdjustRect(htab,FALSE,&prt);
		ScreenToClient(hWndDevWnd,(LPPOINT)&prt);
		SetWindowPos(hWnd,HWND_TOP,prt.left,prt.top,0,0,SWP_NOSIZE);

		SetTimer(hWnd,1,50,NULL); //-- Timer

		//-- GDI Prepare
		bRed =CreateSolidBrush( RGB(255,90,90));
		bBlue =CreateSolidBrush( RGB(90,90,255));
		bGreen =CreateSolidBrush( RGB(90,255,90));
		bYellow =CreateSolidBrush(RGB(255,255,50));
		bBrown =CreateSolidBrush(RGB(155,155,50));
		bNormal=CreateSolidBrush( RGB(255,255,255));
		bWblue =CreateSolidBrush( RGB(180,180,255));
		//bNormal=CreateHatchBrush(HS_BDIAGONAL, RGB(212,208,200));

		hfont = CreateFont(11,0,0,0,0,0,0,0,0,0,0,0,0,"돋움체");

		break;
	default:
		//------------------------------------------------------------------------
		//-- Skill 에 관련된 정보를 표시한다.

		if(pCMyApp->m_pUIMgr)
		{
			//-- Prepare
			HDC dc;
			dc = GetDC(hWnd);

			oldbrush = (HBRUSH)SelectObject(dc,bNormal);
			oldfont  =(HFONT)SelectObject(dc,hfont);
			SetBkMode(dc,TRANSPARENT);
			//----------------------------------------------------------------------
			//-- Status Image
			Rectangle(dc,8,25 +COODI_FLAG_Y,100,150 +COODI_FLAG_Y);

			if(pCMyApp->m_pUIMgr->m_b_Attack)
			{
				SelectObject(dc,bYellow);
				Rectangle(dc,10,28 +COODI_FLAG_Y,98,42  +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_b_MotionForAttack)
			{
				SelectObject(dc,bBrown);
				Rectangle(dc,10,43 +COODI_FLAG_Y,98,57 +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_b_MovingForAttack)
			{
				SelectObject(dc,bGreen);
				Rectangle(dc,10,58 +COODI_FLAG_Y,98,72 +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_b_SecAttack)
			{
				SelectObject(dc,bRed);
				Rectangle(dc,10,73 +COODI_FLAG_Y,98,87 +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_bChakram)
			{
				SelectObject(dc,bWblue);
				Rectangle(dc,10,88 +COODI_FLAG_Y,98,102 +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_bChakramAtt)
			{
				SelectObject(dc,bBlue);
				Rectangle(dc,10,103 +COODI_FLAG_Y,98,117 +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_bBookSpell)
			{
				SelectObject(dc,bWblue);
				Rectangle(dc,10,118 +COODI_FLAG_Y,98,132 +COODI_FLAG_Y);
			}
			if(pCMyApp->m_pUIMgr->m_bBookSpellAtt)
			{
				SelectObject(dc,bBlue);
				Rectangle(dc,10,133 +COODI_FLAG_Y,98,147 +COODI_FLAG_Y);
			}

			//----------------------------------------------------------------------
			//-- Status
			sprintf(strtmp,">> Flags");
			TextOut(dc,10,30,strtmp,strlen(strtmp)); //-- Title

			sprintf(strtmp,"b_AT  : %d",pCMyApp->m_pUIMgr->m_b_Attack);
			TextOut(dc,30,30 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_AMo : %d",pCMyApp->m_pUIMgr->m_b_MotionForAttack);
			TextOut(dc,30,45 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_AMv : %d",pCMyApp->m_pUIMgr->m_b_MovingForAttack);
			TextOut(dc,30,60 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_SeA : %d",pCMyApp->m_pUIMgr->m_b_SecAttack);
			TextOut(dc,30,75 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_Chr : %d",pCMyApp->m_pUIMgr->m_bChakram);
			TextOut(dc,30,90 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_ChAT: %d",pCMyApp->m_pUIMgr->m_bChakramAtt);
			TextOut(dc,30,105 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_Scr: %d",pCMyApp->m_pUIMgr->m_bBookSpell);
			TextOut(dc,30,120 +COODI_FLAG_Y,strtmp,strlen(strtmp));

			sprintf(strtmp,"b_ScAT: %d",pCMyApp->m_pUIMgr->m_bBookSpellAtt);
			TextOut(dc,30,135 +COODI_FLAG_Y,strtmp,strlen(strtmp));
			//----------------------------------------------------------------------
			//-- Skill
			sprintf(strtmp,">> Skill");
			TextOut(dc,110,30,strtmp,strlen(strtmp));  //-- Title
			SelectObject(dc,bNormal);
			Rectangle(dc,110,25 +COODI_FLAG_Y,230,54 +COODI_FLAG_Y); //-- Box


			//-- right
			if(pCMyApp->m_nCurSkill)
			{
				sprintf(strtmp,"Right");
				SelectObject(dc,bYellow);
				Rectangle(dc,170,25 +COODI_FLAG_Y,230,54 +COODI_FLAG_Y); //-- Box
				TextOut(dc,180,50,strtmp,strlen(strtmp));
			}
			//-- left
			else
			{
				sprintf(strtmp,"Left");
				SelectObject(dc,bYellow);
				Rectangle(dc,110,25 +COODI_FLAG_Y,170,54 +COODI_FLAG_Y); //-- Box
				TextOut(dc,120,50,strtmp,strlen(strtmp));
			}



			//-- Property of Selected Skill

			SelectObject(dc,bNormal);
			Rectangle(dc,110,65 +COODI_FLAG_Y,230,120 +COODI_FLAG_Y); //-- Box
			sprintf(strtmp,"CurSkill : %d" ,Selected_Skill);
			TextOut(dc,120,90,strtmp,strlen(strtmp));
			sprintf(strtmp,"Distance : %d" ,Selected_Skill_Distance);
			TextOut(dc,120,105,strtmp,strlen(strtmp));


			//----------------------------------------------------------------------
			//-- END
			SelectObject(dc,oldbrush);
			SelectObject(dc,oldfont);
			ReleaseDC(hWnd,dc);

		}



		break;
	}
	return false;

}



//-----------------------------------------------------------------------------
//-- Proc_DevNet
//-----------------------------------------------------------------------------
//-- Property sheet 중 처음 Page
//-- Network Log sheet 의 Msg 를 처리한다.
BOOL CALLBACK Proc_DevNet( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

	int l_spos;
	char str[512];
	RECT prt;

	HWND hwndList2 = GetDlgItem(hWnd, IDC_LIST_COMM2);
	HWND hwndList = GetDlgItem(hWnd, IDC_LIST_COMM);
	HWND bt_s= GetDlgItem(hWnd, IDC_EDITSVR);
	HWND bt_c = GetDlgItem(hWnd, IDC_EDITCLI);
	HWND selected = GetDlgItem(hWnd, IDC_SELECTED);
	HWND htab = GetDlgItem(hWndDevWnd, IDC_TAB1);
	HWND hcheck = GetDlgItem(hWnd, IDC_CHK_SILENCE);


	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			GetWindowRect(htab,&prt);
			TabCtrl_AdjustRect(htab,FALSE,&prt);
			ScreenToClient(hWndDevWnd,(LPPOINT)&prt);
			SetWindowPos(hWnd,HWND_TOP,prt.left,prt.top,0,0,SWP_NOSIZE);

			//-- flag 초기화
			b_pause = false;
			b_srh_svr = false;  //-- Server Message list 검색
			b_srh_cli = false;  //-- Client Message list 검색
			num_stop = 0;       //-- 검색하는 문자열 발견 후 멈추기 까지 Step
		}
		break;

	case WM_COMMAND:
		{
			//----------------------------------------------------------------------
			//-- COMMAND
			switch(LOWORD(wParam))
			{

			//--------------------------------------------------------------------
			//-- TEXT EDIT
			case IDC_EDITSVR:
				if(HIWORD(wParam) == EN_CHANGE) GetWindowText((HWND)lParam,comp_s,100);
				break;

			case IDC_EDITCLI:
				if(HIWORD(wParam) == EN_CHANGE) GetWindowText((HWND)lParam,comp_c,100);
				break;

			//--------------------------------------------------------------------
			//-- Search Button
			case IDSRH_SVR:
				b_srh_svr = !b_srh_svr;
				if(b_srh_svr)
				{
					SetWindowText((HWND)lParam , "..cancel");
					EnableWindow(bt_s,false);
				}
				else
				{
					SetWindowText((HWND)lParam , "Search");
					EnableWindow(bt_s,true);
				}
				break;

			case IDSRH_CLI:
				b_srh_cli = !b_srh_cli;
				if(b_srh_cli)
				{
					SetWindowText((HWND)lParam , "..cancel");
					EnableWindow(bt_c,false);
				}
				else
				{
					SetWindowText((HWND)lParam , "Search");
					EnableWindow(bt_c,true);
				}
				break;

			case IDC_BTN_SAVE_SVR:
				{
					SaveListString(hWnd, IDC_LIST_COMM);
				}
				break;

			case IDC_BTN_SAVE_CLI:
				{
					SaveListString(hWnd, IDC_LIST_COMM2);
				}
				break;

			//--------------------------------------------------------------------
			//-- Server Mesage List
			case IDC_LIST_COMM2:

				//-- item 이 선택되면 Edit 박스에 전체 내용을 표현한다.
				if(HIWORD(wParam) == LBN_SELCHANGE)
				{
					int sel = SendMessage(hwndList2,  LB_GETCURSEL,0,0);
					SendMessage(hwndList2,  LB_GETTEXT,sel, (LPARAM)str);
					SetWindowText(selected  , str);
				}

				//-- 동기화
				l_spos = SendMessage(hwndList2,  LB_GETTOPINDEX ,0,0);
				SendMessage(hwndList,  LB_SETTOPINDEX ,l_spos,0);
				break;

			//--------------------------------------------------------------------
			//-- Server Mesage List
			case IDC_LIST_COMM:

				//-- item 이 선택되면 Edit 박스에 전체 내용을 표현한다.
				if(HIWORD(wParam) == LBN_SELCHANGE)
				{
					int sel = SendMessage(hwndList,  LB_GETCURSEL,0,0);
					SendMessage(hwndList,  LB_GETTEXT,sel, (LPARAM)str);
					SetWindowText(selected  , str);
				}

				//-- 동기화
				l_spos = SendMessage(hwndList,  LB_GETTOPINDEX ,0,0);
				SendMessage(hwndList2,  LB_SETTOPINDEX ,l_spos,0);
				break;

			case IDPAUSE:
				//--------------------------------------------------------------------
				//-- Pause
				b_pause  = !b_pause;
				//-- Text 변경
				if(b_pause)SetWindowText((HWND)lParam , "Resume");
				else SetWindowText((HWND)lParam , "Pause");
				break;

			//--------------------------------------------------------------------
			//-- Silence Check Button
			case IDC_CHK_SILENCE:
				{
					if(SendMessage(hcheck,  BM_GETCHECK,0,0))
						chk_silence = true;
					else
						chk_silence = false;
				}
				break;



			default:
				break;
			}
		}
	default:
		break;
	}

	return FALSE;

}

//-----------------------------------------------------------------------------
//-- Proc_DevString
//-----------------------------------------------------------------------------
//-- Property sheet 중
//-- String Load sheet 의 Msg 를 처리한다.
BOOL CALLBACK Proc_DevString( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

	HWND htab = GetDlgItem(hWndDevWnd, IDC_TAB1);
	RECT prt;

	switch (nMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(htab,&prt);
		TabCtrl_AdjustRect(htab,FALSE,&prt);
		ScreenToClient(hWndDevWnd,(LPPOINT)&prt);
		SetWindowPos(hWnd,HWND_TOP,prt.left,prt.top,0,0,SWP_NOSIZE);
		break;
	}
	return false;

}

void G_DevAddLogNetwork(char *a_svr_str, char *a_cli_str)
{
	int iCount = 0;

	//-- 추가 중지
	if(b_pause || l_tab_sel!=0)return;

	//--------------------------------------------------------------------------
	//-- Silence 
	if(chk_silence)
	{
		if(strstr(a_svr_str,"in n"))return;
		if(strstr(a_svr_str,"out n"))return;
		if(strstr(a_svr_str,"w n"))return;
		if(strstr(a_svr_str,"walk npc"))return;
		if(strstr(a_svr_str,"a n"))return;
		if(strstr(a_svr_str,"attack npc"))return;
		if(strstr(a_svr_str,"status"))return;
	}

	//-- HWND
	HWND hwnd_pause = GetDlgItem(hTabDlg, IDPAUSE);
	HWND hwndList_svr = GetDlgItem(hTabDlg, IDC_LIST_COMM);
	HWND hwndList_cli = GetDlgItem(hTabDlg, IDC_LIST_COMM2);  

	//--------------------------------------------------------------------------
	//-- Search 
	if(b_srh_svr || b_srh_cli)
	{
		//-- 검색 Server
		if(strstr(a_svr_str,comp_s) && num_stop ==0 && b_srh_svr)
		{      
			num_stop = MAX_NUM_OF_SEARCH; //-- 뒤로 MAX_NUM_OF_SEARCH개 추가.
		}
		//-- 검색 Client
		if(strstr(a_cli_str,comp_c) && num_stop ==0 && b_srh_cli)
		{      
			num_stop = MAX_NUM_OF_SEARCH; //-- 뒤로 MAX_NUM_OF_SEARCH개 추가.
		}

		//-- Pause
		if(num_stop==1)
		{
			num_stop = 0;      
			b_pause =true;
			SetWindowText(hwnd_pause , "Resume");
		}
		//-- num_stop 감소
		if(num_stop>0) num_stop--;
	}


	//--------------------------------------------------------------------------
	//-- Add String 

	//-- 현재 List 의 총 개수를 알아온다.
	iCount = SendMessage(hwndList_svr, LB_GETCOUNT, 0, 0);	  

	//-- 400 이 넘을 경우 처음 List 를 지운다.
	if (iCount >MAX_NUM_OF_LIST ) 
	{
		SendMessage(hwndList_svr, LB_DELETESTRING, 0, 0);	  		// clearing list box 
		SendMessage(hwndList_cli, LB_DELETESTRING, 0, 0);	  		// clearing list box 
	}

	//-- String 추가..
	int idx = SendMessage(hwndList_svr, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) a_svr_str);	
	SendMessage(hwndList_cli, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) a_cli_str);	

	//-- Search 결과라면
	if(num_stop == MAX_NUM_OF_SEARCH-1 && (b_srh_svr || b_srh_cli))
	{
		SendMessage(hwndList_cli, LB_SETCURSEL, (WPARAM)idx, 0);	
		SendMessage(hwndList_svr, LB_SETCURSEL, (WPARAM)idx, 0);	
	}

	//--------------------------------------------------------------------------
	//-- Scroll Bar 설정
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_RANGE;
	GetScrollInfo( hwndList_cli, SB_VERT, &si );  
	SendMessage(hwndList_cli,  LB_SETTOPINDEX, (WPARAM) idx, (LPARAM)0);

	GetScrollInfo( hwndList_svr, SB_VERT, &si );  
	SendMessage(hwndList_svr,  LB_SETTOPINDEX, (WPARAM) idx, (LPARAM)0);


}

void G_DevAddSkillProp(int val_a, int val_b)
{

	switch(val_a)
	{

	//-- Selected Skill
	case 0:
		Selected_Skill = val_b;
		break;

	//-- Skill Distance
	case 1:
		Selected_Skill_Distance = val_b;
		break;

		//-- Skill Distance
		//case 2: Selected_Skill_Req = val_b;break;

	}

}

//----------------------------------------------------------------------------
//-- G_DevAddLogString
//----------------------------------------------------------------------------
//-- Client String 을 Loading 하는 부분
//-- 2004.07.05
//-- Lyul
void G_DevAddLogString(char *a_str)
{

	int iCount = 0;

	if(l_tab_sel!=2)return;

	//-- HWND
	HWND hwndList_str = GetDlgItem(hTabDlg, IDC_LIST_STRING);




	//--------------------------------------------------------------------------
	//-- Add String

	//-- 현재 List 의 총 개수를 알아온다.
	iCount = SendMessage(hwndList_str, LB_GETCOUNT, 0, 0);

	//-- 400 이 넘을 경우 처음 List 를 지운다.
	if (iCount >MAX_NUM_OF_LIST )
	{
		SendMessage(hwndList_str, LB_DELETESTRING, 0, 0);	  		// clearing list box
	}

	//-- String 추가..
	int idx = SendMessage(hwndList_str, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) a_str);


	//--------------------------------------------------------------------------
	//-- Scroll Bar 설정
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_RANGE;
	GetScrollInfo( hwndList_str, SB_VERT, &si );
	SendMessage(hwndList_str,  LB_SETTOPINDEX, (WPARAM) idx, (LPARAM)0);

}




//-----------------------------------------------------------------------------
//-- Proc_PointLog
//-----------------------------------------------------------------------------
//-- Point Log sheet 의 Msg 를 처리한다.
BOOL CALLBACK Proc_DevPointLog( HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

//	int l_spos;
//	char str[256];
	RECT prt;

	HWND hwndList2 = GetDlgItem(hWnd, IDC_LIST_COMM2);
	HWND hwndList = GetDlgItem(hWnd, IDC_LIST_COMM);
	HWND bt_s= GetDlgItem(hWnd, IDC_EDITSVR);
	HWND bt_c = GetDlgItem(hWnd, IDC_EDITCLI);
	HWND selected = GetDlgItem(hWnd, IDC_SELECTED);
	HWND htab = GetDlgItem(hWndDevWnd, IDC_TAB1);
	HWND hcheck = GetDlgItem(hWnd, IDC_CHK_SILENCE);


	switch (nMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(htab,&prt);
		TabCtrl_AdjustRect(htab,FALSE,&prt);
		ScreenToClient(hWndDevWnd,(LPPOINT)&prt);
		SetWindowPos(hWnd,HWND_TOP,prt.left,prt.top,0,0,SWP_NOSIZE);

		_mkdir("LhLog/");

		tab3_EXP		= 0;
		tab3_Lime		= 0;
		tab3_Dmg_base	= 0;
		tab3_Dmg_dam	= 0;
		tab3_Dmg_Last	= 0;

		break;

	case WM_DESTROY:

		tab3_EXP		= 0;
		tab3_Lime		= 0;
		tab3_Dmg_base	= 0;
		tab3_Dmg_dam	= 0;
		tab3_Dmg_Last	= 0;

		b_Lime		= false;
		b_EXP		= false;
		b_Damage	= false;
		b_PointListPause = false;

	case WM_COMMAND:
		//----------------------------------------------------------------------
		//-- COMMAND
		switch(LOWORD(wParam))
		{
		case ID_LIME: // 라임 기록
			{
				b_Lime = !b_Lime;

				FILE* fp = NULL;
				fp = fopen( "LhLog/LimeLog.txt", "a+" );

				if( b_Lime )
				{
					tab3_Lime = 0;
					fprintf( fp, "\n\n## start ##\n");
					fprintf( fp, "---------------------------------------\n");
					SetWindowText((HWND)lParam , "■ Lime Stop ");
				}
				else
				{
					fprintf( fp, "---------------------------------------\n");
					fprintf( fp, "total [ %d ]\n\n", (int)tab3_Lime );
					SetWindowText((HWND)lParam , "▶ Lime Start");
					tab3_Lime = 0;
				}

				fclose( fp );
			}
			break;

		case ID_EXP: // 경험치 기록
			{
				b_EXP = !b_EXP;

				FILE* fp = NULL;
				fp = fopen( "LhLog/ExpLog.txt", "a+" );

				if( b_EXP )
				{
					tab3_EXP = 0;
					fprintf( fp, "\n\n## start ##\n");
					fprintf( fp, "---------------------------------------\n");
					SetWindowText((HWND)lParam , "■ EXP Stop ");
				}
				else
				{
					fprintf( fp, "---------------------------------------\n");
					fprintf( fp, "Total [ %d ]\n\n", (int)tab3_EXP);
					SetWindowText((HWND)lParam , "▶ EXP Start");
					tab3_EXP = 0;
				}

				fclose( fp );
			}
			break;

		case ID_DAMAGE: // 데미지 기록
			{
				b_Damage = !b_Damage;

				FILE* fp = NULL;
				fp = fopen( "LhLog/DamageLog.txt", "a+" );

				if( b_Damage )
				{
					tab3_Dmg_base = 0;
					tab3_Dmg_dam  = 0;
					tab3_Dmg_Last = 0;
					fprintf( fp, "\n\n## start ##\n");
					fprintf( fp, "-------------------------------------------------------------------\n");
					SetWindowText((HWND)lParam , "■ Damage Stop ");
				}
				else
				{
					fprintf( fp, "------------------------------------------------------------------\n");
					fprintf( fp, "Total [ base : %d => Dam : %d => Last : %d ]\n\n", (int)tab3_Dmg_base, (int)tab3_Dmg_dam, (int)tab3_Dmg_Last );
					SetWindowText((HWND)lParam , "▶ Damage Start");
					tab3_Dmg_base = 0;
					tab3_Dmg_dam  = 0;
					tab3_Dmg_Last = 0;
				}

				fclose( fp );
			}
			break;

		case ID_POINTLOG_PAUSE:

			b_PointListPause = !b_PointListPause;

			if( b_PointListPause )
				SetWindowText((HWND)lParam , "▶ Resume");
			else
				SetWindowText((HWND)lParam , "■ Pause ");

			break;

		case ID_POINTLOG_ALL_SAVE:
			{
				if( !g_bAdmin2 )
					break;

				HWND hwndPointLogList = GetDlgItem(hTabDlg, IDC_POINTLOG_LIST);
				int iCount = 0;
				iCount = SendMessage(hwndPointLogList, LB_GETCOUNT, 0, 0);

				if( iCount == 0 )
					break;

				char strTemp1[512] = {0,};
				char strTemp2[512] = {0,};
				G_CatLocalTimeToStr(strTemp1, NULL );

				sprintf( strTemp2,"LhLog/PointAllLog_%s.txt", strTemp1);
				FILE* fp = NULL;
				fp = fopen( strTemp2, "a+" );

				if( fp )
				{
					for(int i=0; i<iCount; i++)
					{
						SendMessage(hwndPointLogList,  LB_GETTEXT,i, (LPARAM)strTemp1);
						fprintf( fp, "%s\n", strTemp1 );
					}

					fclose( fp );
				}
			}
			break;

		default:
			break;
		}

	default:
		break;
	}

	return FALSE;

}



void G_DevAddPointString(char *str)
{

	int iCount = 0;


	if( b_PointListPause )
		return;

	HWND hwndPointLogList = GetDlgItem(hTabDlg, IDC_POINTLOG_LIST);


	iCount = SendMessage(hwndPointLogList, LB_GETCOUNT, 0, 0);

	//-- 400 이 넘을 경우 처음 List 를 지운다.
	if (iCount > 3*200 )
		SendMessage(hwndPointLogList, LB_DELETESTRING, 0, 0);

	//-- String 추가..
	//char strTemp[1024];
	//G_CatLocalTimeToStr(strTemp, str);
	int idx = SendMessage(hwndPointLogList, LB_ADDSTRING, 0, (LPARAM) (LPCTSTR) str);

	//--------------------------------------------------------------------------
	//-- Scroll Bar 설정
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_RANGE;
	GetScrollInfo( hwndPointLogList, SB_VERT, &si );
	SendMessage(hwndPointLogList,  LB_SETTOPINDEX, (WPARAM) idx, (LPARAM)0);


}


void G_CatLocalTimeToStr( char* OUT strOUT, char* IN strIN)
{
	SYSTEMTIME	lt;
	ZeroMemory( &lt, sizeof(LPSYSTEMTIME));
	GetLocalTime( &lt );

	if( strIN != NULL)
		sprintf( strOUT, "[%02d%02d%02d_%02d%02d%02d] %s",lt.wYear-2000, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, strIN);
	else
		sprintf( strOUT, "%02d%02d%02d_%02d%02d%02d",lt.wYear-2000, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond );
}

void SaveListString(HWND hWnd, int idDlgItem)
{
	if( idDlgItem != IDC_LIST_COMM && idDlgItem != IDC_LIST_COMM2 )
		return;

	char str[1024];
	char filename[MAX_PATH] = {0,};

	if( idDlgItem == IDC_LIST_COMM )
	{
		sprintf(filename, "%s", "./netlog_server.txt");
	}
	else
	{
		sprintf(filename, "%s", "./netlog_client.txt");
	}

	HWND hwndList = GetDlgItem(hWnd, idDlgItem);
	int count = SendMessage(hwndList, LB_GETCOUNT, 0, 0);

	FILE* fp = fopen(filename, "wt");
	if( fp == NULL )
		return;

	for( int i = 0; i < count; i++ )
	{
		SendMessage(hwndList, LB_GETTEXT, i, (LPARAM)str);
		if( idDlgItem == IDC_LIST_COMM )
		{
			fprintf(fp, "%s\n", str);
		}
		else
		{
			fprintf(fp, "%s", str);
		}
	}

	fclose(fp);
}