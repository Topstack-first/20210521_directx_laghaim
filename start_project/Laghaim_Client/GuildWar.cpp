#include "stdafx.h"


#include "main.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "msgpopup.h"
#include "nkcharacter.h"
#include "headers.h"
#include "msgpopup.h"
#include "tcpipcon.h"
#include "GuildWar.h"
#include "nk2dframe.h"
#include "controlbottom.h"
#include "g_stringmanager.h"

static DWORD back_color = 0x000000;
static DWORD fore_color = RGB(231, 203, 102);
static DWORD gray_color = RGB(155, 155, 155);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildWar::CGuildWar(int mode)
	: m_pMsgPopUp(NULL)
	, m_bActive(FALSE)
{
	memset(m_strA, 0, sizeof(m_strA));
	memset(m_strB, 0, sizeof(m_strB));
	memset(m_strMoney, 0, sizeof(m_strMoney));

	int i;
	if(!g_pDisplay || mode < 0)
		return;

	// ----------------------------------------------------------
	// GET SCREEN SIZE
	RECT rc;
	HWND hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(hWnd, &rc);
	m_ScreenWidth = rc.right;
	m_ScreenHeight = rc.bottom;			

	//------------------------------------------------------------
	//	BACKGROUND
	int X = 0, Y = 0;
	m_pBack		=  NULL;		
	g_pDisplay->CreateSurfaceFromBitmap(&m_pBack, "interface/GuildWar/guild_war.bmp");
	if( m_pBack )
	{
		m_pBack->Xpos = m_ScreenWidth/2 - m_pBack->GetWidth()/2;
		m_pBack->Ypos = m_ScreenHeight/2 - m_pBack->GetHeight()/2;
		X = m_iMainX = m_pBack->Xpos;
		Y = m_iMainY =m_pBack->Ypos;

		m_pBack->SetColorKey(TRANS_COLOR);
	}

	//------------------------------------------------------------
	//	BUTTON
	m_pBtn_Req		= NULL;
	m_pBtn_Close	= NULL;
	m_pBtn_Cancel	= NULL;
	m_pBtn_Agree	= NULL;
	m_pBtn_reject	= NULL;
	m_pBtn_Start	= NULL;
	m_pBtn_ArrowUp	 = NULL;
	m_pBtn_ArrowDown = NULL;

	m_pBtn_Req		= new CBasicButton();
	m_pBtn_Close	= new CBasicButton();
	m_pBtn_Cancel	= new CBasicButton();
	m_pBtn_Agree	= new CBasicButton();
	m_pBtn_reject	= new CBasicButton();
	m_pBtn_Start	= new CBasicButton();	
	m_pBtn_ArrowUp		= new CBasicButton();
	m_pBtn_ArrowDown	= new CBasicButton();

	if( m_pBtn_Req )
		m_pBtn_Req->Init("GuildWar/btn_request");
	if( m_pBtn_Close )
		m_pBtn_Close->Init("common/btn_ok_02");
	if( m_pBtn_Cancel )
		m_pBtn_Cancel->Init("common/btn_close_01");
	if( m_pBtn_Agree )
		m_pBtn_Agree->Init("common/btn_ok_02");
	if( m_pBtn_reject )
		m_pBtn_reject->Init("GuildWar/btn_refusal");
	if( m_pBtn_Start )
		m_pBtn_Start->Init("GuildWar/btn_start");
	if( m_pBtn_ArrowUp )
		m_pBtn_ArrowUp->Init("common/uarrow");	
	if( m_pBtn_ArrowDown )
		m_pBtn_ArrowDown->Init("common/darrow");


	//------------------------------------------------------------
	//	TEXTOUT	
	m_pTextOut = NULL;
	m_pTextOut = new CTextOutBox();
	if( m_pTextOut )
	{
		m_pTextOut->Init(12, _FCOLOR_YELLOW,0, 0);
		m_pTextOut->SetPos(X+175, Y+151);
	}
	m_iPersonNum = 8; // Default number of people

	//------------------------------------------------------------
	//	TEXTIN	
	for(i=0; i<3; i++){
		m_pTextIn[i] = NULL;
		m_pTextIn[i] = new CTextBox();
		if( !m_pTextIn[i] )
			continue;
		m_pTextIn[i]->Init(0, 0, 20, back_color, fore_color);
		m_pTextIn[i]->SetfontSize();
		m_pTextIn[i]->SetEnable(false);
	}
	if( m_pTextIn[0] )
		m_pTextIn[0]->SetPos(X+175, Y+75);
	if( m_pTextIn[1] )
		m_pTextIn[1]->SetPos(X+175, Y+113);
	if( m_pTextIn[2] )
		m_pTextIn[2]->SetPos(X+175, Y+189);
	if( m_pTextIn[0] )
		m_pTextIn[0]->SetStringLength(14); // 
	if( m_pTextIn[1] )
		m_pTextIn[1]->SetStringLength(12); // 
	if( m_pTextIn[2] )
		m_pTextIn[2]->SetStringLength(12); // 
	
	if( m_pBtn_Req )
		m_pBtn_Req->SetVisible(FALSE);
	if( m_pBtn_Req )
		m_pBtn_Req->SetDisable(true);		
	if( m_pBtn_Close )
		m_pBtn_Close->SetVisible(FALSE);
	if( m_pBtn_Close )
		m_pBtn_Close->SetDisable(true);
	if( m_pBtn_Cancel )
		m_pBtn_Cancel->SetVisible(FALSE);
	if( m_pBtn_Cancel )
		m_pBtn_Cancel->SetDisable(true);
	if( m_pBtn_Agree )
		m_pBtn_Agree->SetVisible(FALSE);
	if( m_pBtn_Agree )
		m_pBtn_Agree->SetDisable(true);
	if( m_pBtn_reject )
		m_pBtn_reject->SetVisible(FALSE);
	if( m_pBtn_reject )
		m_pBtn_reject->SetDisable(true);
	if( m_pBtn_Start )
		m_pBtn_Start->SetVisible(FALSE);
	if( m_pBtn_Start )
		m_pBtn_Start->SetDisable(true);

	if( m_pBtn_ArrowUp )
		m_pBtn_ArrowUp->PosX	= X+267;
	if( m_pBtn_ArrowUp )
		m_pBtn_ArrowUp->PosY	= Y+148;
	if( m_pBtn_ArrowDown )
		m_pBtn_ArrowDown->PosX	= X+267;
	if( m_pBtn_ArrowDown )
		m_pBtn_ArrowDown->PosY	= Y+148+m_pBtn_ArrowDown->Height+5;

	m_iMode = mode;
}

CGuildWar::~CGuildWar()
{	
	SAFE_DELETE(m_pBack);	
	SAFE_DELETE(m_pBtn_Req);
	SAFE_DELETE(m_pBtn_Close);
	SAFE_DELETE(m_pBtn_Cancel);
	SAFE_DELETE(m_pBtn_Agree);
	SAFE_DELETE(m_pBtn_reject);
	SAFE_DELETE(m_pBtn_Start);	
	SAFE_DELETE(m_pBtn_ArrowUp);	
	SAFE_DELETE(m_pBtn_ArrowDown);	
	SAFE_DELETE(m_pTextOut);	

	for(int i=0; i<3; i++)
		SAFE_DELETE(m_pTextIn[i]);				
	
	m_bActive=FALSE;
}


void CGuildWar::Draw()
{ 
	if(!m_bActive || !g_pDisplay || !m_pBack)
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack);
	if( m_pBtn_Req )
		m_pBtn_Req->Draw();
	if( m_pBtn_Close )
		m_pBtn_Close->Draw();
	if( m_pBtn_Cancel )
		m_pBtn_Cancel->Draw();
	if( m_pBtn_Agree )
		m_pBtn_Agree->Draw();
	if( m_pBtn_reject )
		m_pBtn_reject->Draw();
	if( m_pBtn_Start )
		m_pBtn_Start->Draw();
	if( m_pBtn_ArrowUp )
		m_pBtn_ArrowUp->Draw();
	if( m_pBtn_ArrowDown )
		m_pBtn_ArrowDown->Draw();		
	if( m_pTextOut )
		m_pTextOut->Draw();

	for(int i=0; i<3; i++){
		if( m_pTextIn[i] )
			m_pTextIn[i]->Draw();				
	}
}




LRESULT CGuildWar::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y, i;

	if(!m_bActive)
		return 0;

	x = LOWORD (lParam);
	y = HIWORD (lParam);
	
	switch ( msg )
	{			
	case WM_LBUTTONDOWN:

		if(IsInside(x, y)){	
			if( m_pBtn_Req )
				m_pBtn_Req->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Close )
				m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Cancel )
				m_pBtn_Cancel->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Agree )
				m_pBtn_Agree->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_reject )
				m_pBtn_reject->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Start )
				m_pBtn_Start->MsgProc(hWnd, msg, wParam, lParam);					
			if( m_pBtn_ArrowUp )
				m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);					
			if( m_pBtn_ArrowDown )
				m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);					

			// Depending on the mode, lines that receive input and lines that do not receive are distinguished.
			if(m_iMode == 0)
			{
				if( m_pTextIn[0] )
					m_pTextIn[0]->MsgProc(hWnd, msg, wParam, lParam);
				if( m_pTextIn[1] )
					m_pTextIn[1]->MsgProc(hWnd, msg, wParam, lParam);
				if( m_pTextIn[2] )
					m_pTextIn[2]->MsgProc(hWnd, msg, wParam, lParam);
			}
			else if(m_iMode ==1)
			{			
			}
			else if(m_iMode ==2)
			{
				if( m_pTextIn[0] )
					m_pTextIn[0]->MsgProc(hWnd, msg, wParam, lParam);			
			}
			else if(m_iMode ==3)
			{
			}
			return 4;
			break;
		}
		else
		{
			return 2;
		}

		break;
	case WM_LBUTTONUP:		
		if(m_bActive)
		{
			char tmp[10];
			
			if( m_pBtn_Req )
				m_pBtn_Req->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Close )
				m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Cancel )
				m_pBtn_Cancel->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Agree )
				m_pBtn_Agree->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_reject )
				m_pBtn_reject->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Start )
				m_pBtn_Start->MsgProc(hWnd, msg, wParam, lParam);	
			if( m_pBtn_ArrowUp )
				m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);					
			if( m_pBtn_ArrowDown )
				m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);							
			
			
			if(m_pBtn_Req && m_pBtn_Req->m_iBtnFlg == BTN_ACTION){
				if(SendMsg_Req())
					return 2;
			}
			else if(m_pBtn_Close && m_pBtn_Close->m_iBtnFlg == BTN_ACTION){
				// Just kill it.
				return 2;
			}
			else if(m_pBtn_Cancel && m_pBtn_Cancel->m_iBtnFlg == BTN_ACTION){
				if(SendMsg_Cancel())
					return 2;
			}			
			else if(m_pBtn_Agree && m_pBtn_Agree->m_iBtnFlg == BTN_ACTION){
				if(SendMsg_Accept())
					return 2;
			}
			else if(m_pBtn_reject && m_pBtn_reject->m_iBtnFlg == BTN_ACTION){
				if(SendMsg_Cancel())
					return 2;
			}
			else if(m_pBtn_Start && m_pBtn_Start->m_iBtnFlg == BTN_ACTION){
				return 2;
			}
			else if(m_pBtn_ArrowUp && m_pBtn_ArrowUp->m_iBtnFlg == BTN_ACTION){
				m_iPersonNum+=1;
				if(m_iPersonNum > 8)
					m_iPersonNum = 8;
				if( m_pTextOut )
					m_pTextOut->SetString(itoa(m_iPersonNum, tmp, 10));
				return 1;
			}
			else if(m_pBtn_ArrowDown && m_pBtn_ArrowDown->m_iBtnFlg == BTN_ACTION){
				m_iPersonNum-=1;
				if(m_iPersonNum < 2)
					m_iPersonNum = 2;
				if( m_pTextOut )
					m_pTextOut->SetString(itoa(m_iPersonNum, tmp, 10));
				return 1;
			}
		}
		return 1;

		break;		
	case WM_MOUSEMOVE:
		if(IsInside(x, y)){			
			if( m_pBtn_Req )
				m_pBtn_Req->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Close )
				m_pBtn_Close->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Cancel )
				m_pBtn_Cancel->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Agree )
				m_pBtn_Agree->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_reject )
				m_pBtn_reject->MsgProc(hWnd, msg, wParam, lParam);		
			if( m_pBtn_Start )
				m_pBtn_Start->MsgProc(hWnd, msg, wParam, lParam);	
			if(m_pBtn_ArrowUp && m_pBtn_ArrowUp->m_iBtnFlg != BTN_DISABLE){
				m_pBtn_ArrowUp->MsgProc(hWnd, msg, wParam, lParam);	
				if( m_pBtn_ArrowDown )
					m_pBtn_ArrowDown->MsgProc(hWnd, msg, wParam, lParam);	
			}
			return 1;
		}
		break;
		
	case WM_KEYDOWN:
		if(wParam ==VK_TAB || wParam ==VK_RETURN )
		{	
			
			for(i=0; i<3; i++){
				if(m_pTextIn[i]->GetEnable())
				{
					m_pTextIn[i]->SetEnable(false);					
					
					if(m_pTextIn[i+1] && i+1<3)
						m_pTextIn[i+1]->SetEnable(true);
					else if( m_pTextIn[0] ) 
						m_pTextIn[0]->SetEnable(true);

					break;
				}					
			}
			if(m_iMode == 0){				
			}else if(m_iMode ==1){			
				if( m_pTextIn[0] )
					m_pTextIn[0]->SetEnable(false);					
				if( m_pTextIn[1] )
					m_pTextIn[1]->SetEnable(false);					
				if( m_pTextIn[2] )
					m_pTextIn[2]->SetEnable(false);					
			}else if(m_iMode ==2){
				if( m_pTextIn[1] )
					m_pTextIn[1]->SetEnable(false);					
				if( m_pTextIn[2] )
					m_pTextIn[2]->SetEnable(false);					
			}else if(m_iMode ==3){
			}
			return 1;
		}	
		break;
		

	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:		
		// Depending on the mode, lines that receive input and lines that do not receive are distinguished.
		if(m_iMode == 0){
			if( m_pTextIn[0] )
				m_pTextIn[0]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[1] )
				m_pTextIn[1]->MsgProc(hWnd, msg, wParam, lParam);
			if( m_pTextIn[2] )
				m_pTextIn[2]->MsgProc(hWnd, msg, wParam, lParam);
		}else if(m_iMode ==1){			
		}else if(m_iMode ==2){
			if( m_pTextIn[0] )
				m_pTextIn[0]->MsgProc(hWnd, msg, wParam, lParam);			
		}else if(m_iMode ==3){
		}
		return 1;
		break;
	}		
	return 0;
}

BOOL CGuildWar::IsInside(int x, int y)
{
	if( !m_pBack )
		return false;

	return true;
}

void CGuildWar::GetMsg(int type, char* a, char* b, char* money, char* person_num)
{
	
	char tmp[80];
	int i;	

	if(type == 0){ // Guild Application Team
		if( m_pBtn_Req )
			m_pBtn_Req->SetVisible(TRUE);
		if( m_pBtn_Req )
			m_pBtn_Req->SetDisable(false);		
		if( m_pBtn_Close )
			m_pBtn_Close->SetVisible(TRUE);
		if( m_pBtn_Close )
			m_pBtn_Close->SetDisable(false);

		if( m_pBtn_Req )
			m_pBtn_Req->SetPosition(m_iMainX + 36, m_iMainY+ 223);
		if( m_pBtn_Close )
			m_pBtn_Close->SetPosition(m_iMainX + 193, m_iMainY+ 223);			
		
		// 길원인가?
		if(g_pRoh->m_pstrGuildName){
			strcpy(tmp, g_pRoh->m_pstrGuildName);			
		}else{			
			strcpy(tmp, g_pRoh->m_pstrName);			
		} 
		if( m_pTextIn[0] )
			m_pTextIn[0]->SetEnable(true);// 최초 커서의 위치
		if( m_pTextIn[0] )
			m_pTextIn[0]->SetText(tmp);			// a팀		
		if( m_pTextOut )
			m_pTextOut->SetString("8");  // 인원

	}
	else if(type == 1){ // 길드전 취소 (a팀이 수정가능)
		if( m_pBtn_Cancel )
			m_pBtn_Cancel->SetVisible(TRUE);
		if( m_pBtn_Cancel )
			m_pBtn_Cancel->SetDisable(false);		
		if( m_pBtn_Close )
			m_pBtn_Close->SetVisible(TRUE);
		if( m_pBtn_Close )
			m_pBtn_Close->SetDisable(false);

		if( m_pBtn_Cancel )
			m_pBtn_Cancel->SetPosition(m_iMainX + 36, m_iMainY+ 223);
		if( m_pBtn_Close )
			m_pBtn_Close->SetPosition(m_iMainX + 193, m_iMainY+ 223);

		if( m_pBtn_ArrowUp )
			m_pBtn_ArrowUp->m_iBtnFlg		= BTN_DISABLE;
		if( m_pBtn_ArrowDown )
			m_pBtn_ArrowDown->m_iBtnFlg	= BTN_DISABLE;
		if( m_pBtn_ArrowUp )
			m_pBtn_ArrowUp->SetDisable(true);
		if( m_pBtn_ArrowDown )
			m_pBtn_ArrowDown->SetDisable(true);		

		
		if( m_pTextIn[0] )
			m_pTextIn[0]->SetText(a);			// a팀
		if( m_pTextIn[2] )
			m_pTextIn[2]->SetText(b);			// b팀
		if( m_pTextIn[1] )
			m_pTextIn[1]->SetText(money);		// 길드전 금액
		if( m_pTextOut )
			m_pTextOut->SetString(person_num, gray_color);  // 인원

		// 입력제한과 함께 텍스트는 그레이처리를 한다..
		for(i=0; i<3; i++){
			if( m_pTextIn[i] )
				m_pTextIn[i]->ChangeColor(gray_color);
		}
		
	}
	else if(type == 2){ // 길드전 승인
		if( m_pBtn_Agree )
			m_pBtn_Agree->SetVisible(TRUE);
		if( m_pBtn_Agree )
			m_pBtn_Agree->SetDisable(false);		
		if( m_pBtn_reject )
			m_pBtn_reject->SetVisible(TRUE);
		if( m_pBtn_reject )
			m_pBtn_reject->SetDisable(false);
		if( m_pBtn_Close )
			m_pBtn_Close->SetVisible(TRUE);
		if( m_pBtn_Close )
			m_pBtn_Close->SetDisable(false);

		if( m_pBtn_Agree )
			m_pBtn_Agree->SetPosition(m_iMainX + 21, m_iMainY+ 223);
		if( m_pBtn_reject )
			m_pBtn_reject->SetPosition(m_iMainX + 113, m_iMainY+ 223);		
		if( m_pBtn_Close )
			m_pBtn_Close->SetPosition(m_iMainX + 210, m_iMainY+ 223);

		if( m_pBtn_ArrowUp )
			m_pBtn_ArrowUp->m_iBtnFlg		= BTN_DISABLE;
		if( m_pBtn_ArrowDown )
			m_pBtn_ArrowDown->m_iBtnFlg	= BTN_DISABLE;
		if( m_pBtn_ArrowUp )
			m_pBtn_ArrowUp->SetDisable(true);
		if( m_pBtn_ArrowDown )
			m_pBtn_ArrowDown->SetDisable(true);		

		if(g_pRoh->m_pstrGuildName){
			strcpy(tmp, g_pRoh->m_pstrGuildName);			
		}else{			
			strcpy(tmp, g_pRoh->m_pstrName);			
		}

		if( m_pTextIn[0] )
			m_pTextIn[0]->SetText(tmp);			// a팀
		if( m_pTextIn[2] )
			m_pTextIn[2]->SetText(b);			// b팀
		if( m_pTextIn[1] )
			m_pTextIn[1]->SetText(money);		// 길드전 금액
		if( m_pTextOut )
			m_pTextOut->SetString(person_num, gray_color);  // 인원

		if( m_pTextIn[1] )
			m_pTextIn[1]->ChangeColor(gray_color);
		if( m_pTextIn[2] )
			m_pTextIn[2]->ChangeColor(gray_color);

	}
	else if(type == 3){ // 준비완료 (이미 신청중인 상태)
		if( m_pBtn_Start )
			m_pBtn_Start->SetVisible(TRUE);
		if( m_pBtn_Start )
			m_pBtn_Start->SetDisable(false);		
		if( m_pBtn_Close )
			m_pBtn_Close->SetVisible(TRUE);
		if( m_pBtn_Close )
			m_pBtn_Close->SetDisable(false);

		if( m_pBtn_Start )
			m_pBtn_Start->SetPosition(m_iMainX + 36, m_iMainY+ 223);
		if( m_pBtn_Close )
			m_pBtn_Close->SetPosition(m_iMainX + 193, m_iMainY+ 223);

		if( m_pBtn_ArrowUp )
			m_pBtn_ArrowUp->m_iBtnFlg		= BTN_DISABLE;
		if( m_pBtn_ArrowDown )
			m_pBtn_ArrowDown->m_iBtnFlg	= BTN_DISABLE;
		if( m_pBtn_ArrowUp )
			m_pBtn_ArrowUp->SetDisable(true);
		if( m_pBtn_ArrowDown )
			m_pBtn_ArrowDown->SetDisable(true);
	}	
	m_bActive = TRUE;

}


///////////////////////////////////////////////////////////////////
// C -> S  msg

BOOL CGuildWar::SendMsg_Open()
{
	char tmp[100];
	sprintf(tmp, "g_war\n");
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);	

	return TRUE;
}

// 최초 신청자가 보내는 메세지
BOOL CGuildWar::SendMsg_Req()
{	
	if(!CheckAllItem())
		return FALSE;

	char tmp[100];
	if( m_pTextIn[0] && m_pTextIn[1] && m_pTextIn[2] && m_pTextOut )
		sprintf(tmp, "g_war request %s %s %s %s\n"
			, m_pTextIn[0]->GetString()
			, m_pTextIn[2]->GetString()
			, m_pTextIn[1]->GetString()
			, m_pTextOut->m_str);
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);	

	return TRUE;
}


// 신청을 받은 팀이 "수락 " 메세지를 보낸다.
BOOL CGuildWar::SendMsg_Accept()
{
	if(!CheckAllItem())
		return FALSE;

	char tmp[100];
	sprintf(tmp, "g_war accept %s\n", m_pTextIn[0]->GetString());
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);	
	return TRUE;
}


// 신청 또는 신청 받는 팀이 취소할경우.
BOOL CGuildWar::SendMsg_Cancel()
{
	if(!CheckAllItem())
		return FALSE;

	char tmp[100];
	sprintf(tmp, "g_war cancel\n");
	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);	

	return TRUE;
}





#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

BOOL CGuildWar::CheckAllItem()
{
	if(m_iMode<0)
		return FALSE;

	if(m_pTextIn[0] && m_pTextIn[0]->GetStrBuf()[0] == NULL)
	{
		//-- IDS_GWAR_INPUT_NAME : 모임명을 입력해 주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_INPUT_NAME), TYPE_NOR_OK, 1 );
		return FALSE;;
	}
	else
	{
		if(m_pTextIn[0] && CheckSpace(m_pTextIn[0]->GetStrBuf()))
		{
			//-- IDS_GWAR_UNABLE_EMPTY : 모임명에 띄어쓰기는 사용할 수 없습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_UNABLE_EMPTY), TYPE_NOR_OK, 1 );
			return FALSE;;
		}
	}

	if(m_pTextIn[1] && m_pTextIn[1]->GetStrBuf()[0] == NULL)
	{
		//-- IDS_GWAR_INPUT_PAY : 팀배틀 금액을 입력해 주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_INPUT_PAY), TYPE_NOR_OK, 1 );
		return FALSE;;
	}
	else
	{
		if(m_pTextIn[1] && CheckSpace(m_pTextIn[1]->GetStrBuf()))
		{
			//-- IDS_GWAR_WRONG_MONEY : 잘못된 금액이 입력되었습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_WRONG_MONEY), TYPE_NOR_OK, 1 );
			return FALSE;;
		}

		int len = strlen(m_pTextIn[1]->GetStrBuf());
		for(int i=0; i<len; i++){  				
			if(m_pTextIn[1] && isdigit(m_pTextIn[1]->GetStrBuf()[i]) == FALSE)
			{
				//-- IDS_GWAR_WRONG_MONEY : 잘못된 금액이 입력되었습니다.
				g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_WRONG_MONEY), TYPE_NOR_OK, 1 );
				return FALSE;
			}
		}


		int money = 0;
		if( m_pTextIn[1] )
			money = atoi(m_pTextIn[1]->GetStrBuf());
		if(money < 100000 || money > 100000000)
		{
			//-- IDS_GWAR_MAX_PAY : 입금 한도액은 10만라임 ~ 1억라임 까지 입니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_MAX_PAY), TYPE_NOR_OK, 1 );
			return FALSE;
		}		
	}

	if(m_pTextIn[2] && m_pTextIn[2]->GetStrBuf()[0] == NULL)  
	{
		//-- IDS_GWAR_OTHER_NAME : 상대측 대표자 이름을 입력해 주세요
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_OTHER_NAME), TYPE_NOR_OK, 1 );
		return FALSE;;
	}
	else{
		if(m_pTextIn[2] && CheckSpace(m_pTextIn[2]->GetStrBuf()))
		{
			//-- IDS_GWAR_UNBLAE_EMPTY2 : 상대측 대표자 이름에 띄어쓰기는 사용할 수 없습니다.
			g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_GWAR_UNBLAE_EMPTY2), TYPE_NOR_OK, 1 );
			return FALSE;;
		}
	}

	return TRUE;
}




BOOL CGuildWar::CheckSpace(char* String)
{
	int len = strlen(String);
	for(int i=0; i<len; i++)
	{
		if(is_space(String[i]) == TRUE)
			return TRUE;
	}	
	return FALSE;
}
