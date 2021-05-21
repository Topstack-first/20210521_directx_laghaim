#include "stdafx.h"

#include "MsgPopUp.h"
#include "Basicbutton.h"
#include "headers.h"
#include "ddutil.h"
#include "main.h"
#include "2dresmgr.h"
#include "textbox.h"
#include "LoadEffectSound.h"
#include "WindowMgr.h"
#include "tcpipcon.h"
#include "Nk2DFrame.h"
#include "Mouse.h"
#include "CheckBox.h"

// 어떤 버튼이 클릭 됬는가??
#define OK_CLICK			0
#define CANCLE_CLICK		1
#define STR_LIMIT			20
const int MSG_FONT_SIZE = 12;
const int MSG_FONT_SIZE2 = 12;

extern char					g_strFontName[];
extern DWORD				g_dwLangType;
extern HHOOK				g_hHook;

extern LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////////////////////////////

CaruResultData::CaruResultData()
	: m_listCount(0)
	, m_FontColor(_FCOLOR_GREEN)
	, m_Font(NULL)
	, m_posX(0)
	, m_posY(0)
{
	memset(m_pSurfIndex, 0 , sizeof(m_pSurfIndex));
	memset(m_pSurfItemName, 0 , sizeof(m_pSurfItemName));
	memset(m_pSurfItemCount, 0 , sizeof(m_pSurfItemCount));	
}

CaruResultData::~CaruResultData()
{
	for(int i = 0; i < MAX_TAKEBOX; i++)
	{
		SAFE_DELETE(m_pSurfIndex[i]);
		SAFE_DELETE(m_pSurfItemName[i]);
		SAFE_DELETE(m_pSurfItemCount[i]);
	}
}

void CaruResultData::SetListCount(const int count)
{
	m_listCount = count;
}

void CaruResultData::SetIndex(const int num, const std::string& index)
{
	if( num < 0 || num >= MAX_TAKEBOX )
		return;

	if( m_Font == NULL )
		return;

	m_Index[num] = index;
	if( m_pSurfIndex[num] )
	{
		SAFE_DELETE(m_pSurfIndex[num]);
	}

	TCHAR* szTmp = const_cast<char*>(m_Index[num].c_str());
	g_pDisplay->CreateSurfaceFromText(&m_pSurfIndex[num], m_Font, szTmp, RGB(0, 0, 1), m_FontColor);
	if( m_pSurfIndex[num] )
	{
		m_pSurfIndex[num]->SetColorKey(RGB(0, 0, 1));
	}
}

void CaruResultData::SetItemName(const int num, const std::string& itemName)
{
	if( num < 0 || num >= MAX_TAKEBOX )
		return;

	if( m_Font == NULL )
		return;

	m_ItemName[num] = itemName;
	if( m_pSurfItemName[num] )
	{
		SAFE_DELETE(m_pSurfItemName[num]);
	}

	TCHAR* szTmp = const_cast<char*>(m_ItemName[num].c_str());
	g_pDisplay->CreateSurfaceFromText(&m_pSurfItemName[num], m_Font, szTmp, RGB(0, 0, 1), m_FontColor);
	if( m_pSurfItemName[num] )
	{
		m_pSurfItemName[num]->SetColorKey(RGB(0, 0, 1));
	}
}

void CaruResultData::SetItemCount(const int num, const std::string& itemCount)
{
	if( num < 0 || num >= MAX_TAKEBOX )
		return;

	if( m_Font == NULL )
		return;

	m_ItemCount[num] = itemCount;
	if( m_pSurfItemCount[num] )
	{
		SAFE_DELETE(m_pSurfItemCount[num]);
	}

	TCHAR* szTmp = const_cast<char*>(m_ItemCount[num].c_str());
	g_pDisplay->CreateSurfaceFromText(&m_pSurfItemCount[num], m_Font, szTmp, RGB(0, 0, 1), m_FontColor);
	if( m_pSurfItemCount[num] )
	{
		m_pSurfItemCount[num]->SetColorKey(RGB(0, 0, 1));
	}
}

void CaruResultData::SetFont(int f_w, int f_h, char* fontname)
{
	m_Font = G_GetFont(f_h,0,fontname);
}

void CaruResultData::SetTextColor(DWORD color)
{
	m_FontColor = color;
}

void CaruResultData::SetPos(int x, int y)
{
	m_posX = x;
	m_posY = y;
}

void CaruResultData::Draw()
{
	int len = 0;
	for(int i = 0; i < MAX_TAKEBOX; i++)
	{
		if( m_pSurfIndex[i] == NULL )
			continue;
		if( m_pSurfItemName[i] == NULL )
			continue;
		if( m_pSurfItemCount[i] == NULL )
			continue;

		len = m_Index[i].length();
		len = len * 6 / 2;
		g_pDisplay->Blt(m_posX + 59 - len, m_posY + 70 + (i * 20), m_pSurfIndex[i]);
		len = m_ItemName[i].length();
		len = len * 6 / 2;
		g_pDisplay->Blt(m_posX + 234 - len, m_posY + 70 + (i * 20), m_pSurfItemName[i]);
		len = m_ItemCount[i].length();
		len = len * 6 / 2;
		g_pDisplay->Blt(m_posX + 402 - len, m_posY + 70 + (i * 20), m_pSurfItemCount[i]);
	}
}

void CaruResultData::Restore()
{
	for(int i = 0; i < MAX_TAKEBOX; i++)
	{
		if( m_Index[i].length() <= 0 )
			continue;

		SetIndex(i, m_Index[i]);
		SetItemName(i, m_ItemName[i]);
		SetItemCount(i, m_ItemCount[i]);
	}
}

//////////////////////////////////////////////////////////////////////////

CMsgPopUp::CMsgPopUp()
	: m_FontColor(0)
	, m_FontSize(0)
	, m_StrLimit(0)
	, m_bMultLine(false)
	, m_nCurExistBtn(0)
	, m_CurType(0)
{
	memset(m_Tmp1, 0, sizeof(m_Tmp1));
	memset(m_Tmp2, 0, sizeof(m_Tmp2));
	memset(m_Tmp3, 0, sizeof(m_Tmp3));

	m_pSurf		 = NULL;
	m_pSurf2	 = NULL;
	m_pSurf3	 = NULL; ///메세지박스3줄확장
	m_pBack  	 = NULL;	// 일반적인 팝업창
	m_Font		 = NULL;
	m_hWnd		 = NULL;
	m_pCaruResultData = NULL;
	m_bActive    = FALSE;	
}

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- m_Font 가 handle 를 가져온다. 따라서 파괴할 필요가 없다.
CMsgPopUp::~CMsgPopUp()
{
	DeleteRes();
}


void CMsgPopUp::Init(int type)
{
	m_CurType = type;

	switch(type)
	{
	case TYPE_NOR_OK:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn1.SetDisable( false );
		break;


	case TYPE_NOR_OKCANCLE:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn2.SetFileName("common/btn_cancel");
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;


	case TYPE_ESC:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_exit");
		m_Btn2.SetFileName("common/btn_cancel_03");
		m_Btn3.SetFileName("common/btn_restart");

		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		m_Btn3.SetDisable( false );
		break;

	case TYPE_DEL:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn2.SetFileName("common/btn_cancel");
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;

	case TYPE_EXCHANGE:
	case TYPE_SHOUT:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn2.SetFileName("common/btn_cancel");
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;

	case TYPE_EXCHANGE_EX: ///개인상점
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn2.SetFileName("common/btn_cancel");
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;

	case TYPE_NOR_OK_LARGE:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn1.SetDisable( false );
		break;

	case TYPE_NOR_OKCANCLE_LARGE:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok");
		m_Btn2.SetFileName("common/btn_cancel");
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;

	case TYPE_NOR_ACCEPTREJECT:
		TextOutInit();
		m_Btn1.SetFileName("common/btn_ok"); // 수락
		m_Btn2.SetFileName("common/btn_cancel"); // 거절
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;

	case TYPE_PREMIUM_CHA_MOVE_NAME_INPUT:
		TextOutInit();
		m_Btn1.SetFileName("premium/ChaMove/ok");
		m_Btn2.SetFileName("premium/ChaMove/cancel");
		m_Btn1.SetDisable( false );
		m_Btn2.SetDisable( false );
		break;

	case TYPE_PREMIUM_CHA_MOVE_WAIT:
		TextOutInit();
		break;

	case TYPE_NOR_OKCANCELCHECK:
		{
			TextOutInit();
			m_Btn1.SetFileName("common/btn_ok");
			m_Btn2.SetFileName("common/btn_cancel");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );
			m_chkBox.Init("interface/common/check.bmp", "interface/common/check_none.bmp");
			m_chkBox.SetEnable(true);
			m_chkBox.SetVisible(true);
		}
		break;

	case TYPE_CARU_RESULT:
		{
			TextOutInit();
			m_Btn1.SetFileName("common/btn_ok");
			m_Btn1.SetDisable(false);
			m_pCaruResultData = new CaruResultData();
		}
		break;
	}
	{
		switch( type)
		{
		case TYPE_GUILD_INVASION_CONFIRM_CANCEL :
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_accept");
			m_Btn2.SetFileName("guild/msg_refuse");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );
			break;
		case TYPE_GUILD_CREATE_PROMPT :
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_ok");
			m_Btn2.SetFileName("guild/msg_back");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );
			break;
		case TYPE_GUILD_CONFIRM :
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_ok");
			m_Btn1.SetDisable( false );
			break;
		case TYPE_GUILD_CREATE_NAME_INPUT :
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_ok");
			m_Btn2.SetFileName("guild/Exit");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );
			break;
		case TYPE_GUILD_CONFIRM_CANCEL :
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_ok");
			m_Btn2.SetFileName("guild/msg_no");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );
			break;
		case TYPE_GUILD_RECOMMAND_JOIN :
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_ok");
			m_Btn2.SetFileName("guild/msg_no");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );
			break;

		case TYPE_GUILD_EXCHANGE:
			TextOutInit();
			m_Btn1.SetFileName("guild/msg_ok");
			m_Btn2.SetFileName("guild/Exit");
			m_Btn1.SetDisable( false );
			m_Btn2.SetDisable( false );

			break;

		case TYPE_WANTED_CONFIRM :
			if( g_SvrType != ST_ADULT_ONLY)
				break;

			TextOutInit();
			m_Btn1.SetFileName("wanted/ok");
			m_Btn1.SetDisable( false );
			break;
		}
	}

}



void CMsgPopUp::ChangeFName(CBasicButton*	pBtn, char* name)
{
	if(!pBtn || name == NULL)
		return;
	if( pBtn )	pBtn->SetFileName(name);
}
//----------------------------------------------------------------------------------
//		INITIARIZE
//----------------------------------------------------------------------------------

void CMsgPopUp::TextOutInit()
{
	*m_Tmp1 = '\0';
	*m_Tmp2 = '\0';
	*m_Tmp3 = '\0'; ///메세지박스3줄확장

	m_FontSize	= MSG_FONT_SIZE;					    // 폰트 크기 지정
	m_FontColor = _FCOLOR_GREEN;			// 폰트의 색상 지정

	if( g_dwLangType != LANG_TH )
		m_StrLimit	= STR_LIMIT;				// 20 바이트에서 자른다!!!
	else // 태국 일 경우는 약간 더 넉넉하게 줄을 잘라 준다. (by 원석)
		m_StrLimit	= 30;				// 30 바이트에서 자른다!!!

	m_bMultLine = FALSE;					// 여러줄 찍기는 FALSE로 초기값설정
}

//----------------------------------------------------------------------------------
//		LOAD RESOURCE !!!
//----------------------------------------------------------------------------------
void CMsgPopUp::LoadRes()
{
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);
	SetFont(m_FontSize/2, m_FontSize, g_strFontName);		// 폰트 생성

	switch(m_CurType)
	{
	case TYPE_NOR_OK:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_msgbox");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		break;

	case TYPE_NOR_OKCANCLE:
	case TYPE_NOR_ACCEPTREJECT:	
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_msgbox");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		m_Btn2.LoadRes();
		break;

	case TYPE_NOR_OKCANCELCHECK:
		{
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_msgbox");
			if( m_pBack )
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}
			m_Btn1.LoadRes();
			m_Btn2.LoadRes();
			m_chkBox.LoadRes();
		}
		break;

	case TYPE_ESC:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_escback");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		m_Btn2.LoadRes();
		m_Btn3.LoadRes();

		break;

	case TYPE_DEL:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_delback");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		m_Btn2.LoadRes();

		m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, TRUE, MSG_POPUP);
		m_InputLine.SetEnable(TRUE);
		m_InputLine.SetMaxLen(18);
		//채팅 에디트 수정 삭제시 비번..
		m_InputLine.SetMaxLen(15);
		m_InputLine.SetfontSize(12);
		m_InputLine.SetPos(m_pBack->Xpos+61, m_pBack->Ypos+64);
		m_FontColor = _FCOLOR_GREEN;
		break;

	case TYPE_EXCHANGE:
	case TYPE_SHOUT:
		{
			if( m_CurType == TYPE_SHOUT )
			{
				m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_shout");
			}
			else
			{
				m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_delback");
			}
			
			if( m_pBack )
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}
			m_Btn1.LoadRes();
			m_Btn2.LoadRes();

			if( m_CurType == TYPE_SHOUT )
			{
				m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);			
				m_InputLine.SetMaxLen(50);
				m_InputLine.SetfontSize(12);
				m_InputLine.SetPos(m_pBack->Xpos+45, m_pBack->Ypos+64);
			}
			else
			{
				m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);			
				m_InputLine.SetMaxLen(16);
				m_InputLine.SetfontSize(12);
				m_InputLine.SetPos(m_pBack->Xpos+61, m_pBack->Ypos+64);
			}

			m_InputLine.SetEnable(TRUE);
			
			*m_Tmp1 = '\0';
			*m_Tmp2 = '\0';
			*m_Tmp3 = '\0';

			m_FontSize	= 12;
			m_FontColor = _FCOLOR_GREEN;
			m_bMultLine = FALSE;
		}
		break;

	case TYPE_EXCHANGE_EX:	// 금괴가 추가된 가격입력용. ///개인상점
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_priceinput");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		m_Btn2.LoadRes();

		m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		m_InputLine.SetEnable(TRUE);
		m_InputLine.SetMaxLen(16);
		m_InputLine.SetfontSize( 12);
		m_InputLine.SetPos(m_pBack->Xpos+61, m_pBack->Ypos+102); // linha lime

		m_InputValue1.Init(0, 0, 4, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		//		m_InputValue1.SetEnable(TRUE);
		m_InputValue1.SetMaxLen(5);
		m_InputValue1.SetfontSize(12);
		m_InputValue1.SetPos(m_pBack->Xpos+50, m_pBack->Ypos+71); // linha gold

		m_InputValue2.Init(0, 0, 4, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		//		m_InputValue2.SetEnable(TRUE);
		m_InputValue2.SetMaxLen(5);
		m_InputValue2.SetfontSize(12);
		m_InputValue2.SetPos(m_pBack->Xpos+103, m_pBack->Ypos+71); // linha silver

		m_InputValue3.Init(0, 0, 4, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		//		m_InputValue3.SetEnable(TRUE);
		m_InputValue3.SetMaxLen(5);
		m_InputValue3.SetfontSize(12);
		m_InputValue3.SetPos(m_pBack->Xpos+156, m_pBack->Ypos+71); // linha bronze

		m_InputValue4.Init(0, 0, 4, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		//		m_InputValue3.SetEnable(TRUE);
		m_InputValue4.SetMaxLen(5);
		m_InputValue4.SetfontSize(12);
		m_InputValue4.SetPos(m_pBack->Xpos+76, m_pBack->Ypos+42); // linha platium

		m_InputValue5.Init(0, 0, 4, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		//		m_InputValue3.SetEnable(TRUE);
		m_InputValue5.SetMaxLen(5);
		m_InputValue5.SetfontSize(12);
		m_InputValue5.SetPos(m_pBack->Xpos+131, m_pBack->Ypos+42); // linha red

		// 여기서 TextOutInit 의 내용을 바꾼다.
		*m_Tmp1 = '\0';
		*m_Tmp2 = '\0';
		*m_Tmp3 = '\0'; ///메세지박스3줄확장

		m_FontSize	= MSG_FONT_SIZE;					    // 폰트 크기 지정
		m_FontColor = _FCOLOR_GREEN;//_FCOLOR_GREEN;			// 폰트의 색상 지정
		m_bMultLine = FALSE;					// 여러줄 찍기는 FALSE로 초기값설정

		break;

	case TYPE_NOR_OK_LARGE:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_msgbox_large");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		break;

	case TYPE_NOR_OKCANCLE_LARGE:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_msgbox_large");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		m_Btn1.LoadRes();
		m_Btn2.LoadRes();
		break;

	case TYPE_PREMIUM_CHA_MOVE_NAME_INPUT:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_cha_move_name");

		if(m_pBack)
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}

		m_Btn1.LoadRes();
		m_Btn2.LoadRes();

		m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
		m_InputLine.SetEnable(TRUE);
		m_InputLine.SetMaxLen(16);
		m_InputLine.SetfontSize(9);
		m_InputLine.SetPos(m_pBack->Xpos+122, m_pBack->Ypos+94);

		*m_Tmp1 = '\0';
		*m_Tmp2 = '\0';
		*m_Tmp3 = '\0';

		m_FontSize	= MSG_FONT_SIZE;
		m_FontColor = _FCOLOR_GREEN;
		m_bMultLine = FALSE;

		break;

	case TYPE_PREMIUM_CHA_MOVE_WAIT:
		m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_msgbox");
		if( m_pBack )
		{
			m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
			m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
			m_pBack->SetColorKey(TRANS_COLOR);
		}
		break;

	case TYPE_CARU_RESULT:
		{
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_takebox");
			if( m_pBack )
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);

				if( m_pCaruResultData )
				{
					m_pCaruResultData->SetPos(m_pBack->Xpos, m_pBack->Ypos);
					m_pCaruResultData->SetFont(m_FontSize/2, m_FontSize, g_strFontName);
				}
			}

			m_Btn1.LoadRes();			
		}
		break;
	}

	{
		switch( m_CurType )
		{
		case TYPE_WANTED_CONFIRM :
			if( g_SvrType != ST_ADULT_ONLY)
				break;

			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_wanted_normal");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}

			m_Btn1.LoadRes();
			break;
		case TYPE_GUILD_INVASION_CONFIRM_CANCEL :
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_normal");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}
			m_Btn1.LoadRes();
			m_Btn2.LoadRes();
			break;
		case TYPE_GUILD_CREATE_PROMPT :
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_create");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}
			m_Btn1.LoadRes();
			m_Btn2.LoadRes();
			break;
		case TYPE_GUILD_CONFIRM :
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_normal");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}

			m_Btn1.LoadRes();
			break;
		case TYPE_GUILD_CREATE_NAME_INPUT :
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_nameinput");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}

			m_Btn1.LoadRes();
			m_Btn2.LoadRes();

			m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
			m_InputLine.SetEnable(TRUE);
			m_InputLine.SetMaxLen(11);
			m_InputLine.SetfontSize(12);
			m_InputLine.SetPos(m_pBack->Xpos+97, m_pBack->Ypos+82);


			*m_Tmp1 = '\0';
			*m_Tmp2 = '\0';
			*m_Tmp3 = '\0';

			m_FontSize	= MSG_FONT_SIZE;
			m_FontColor = _FCOLOR_GREEN;
			m_bMultLine = FALSE;
			break;
		case TYPE_GUILD_RECOMMAND_JOIN:
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_recominput");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}

			m_Btn1.LoadRes();
			m_Btn2.LoadRes();

			m_InputLine.Init(0, 0, 10, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
			m_InputLine.SetEnable(TRUE);
			m_InputLine.SetMaxLen(10);
			m_InputLine.SetfontSize(12);
			m_InputLine.SetPos(m_pBack->Xpos+111, m_pBack->Ypos+82);

			*m_Tmp1 = '\0';
			*m_Tmp2 = '\0';
			*m_Tmp3 = '\0';

			m_FontSize	= MSG_FONT_SIZE2;
			m_FontColor = _FCOLOR_GREEN;
			m_bMultLine = FALSE;
			break;
		case TYPE_GUILD_CONFIRM_CANCEL :
			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_normal");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}

			m_Btn1.LoadRes();
			m_Btn2.LoadRes();
			break;

		case TYPE_GUILD_EXCHANGE:

			m_pBack = pCMyApp->Get2DResMgr()->GetSurfaceFromMsgBox("back_guild_input");

			if(m_pBack)
			{
				m_pBack->Xpos = ((m_ClientRc.right/2) - (m_pBack->GetWidth()/2));
				m_pBack->Ypos = ((m_ClientRc.bottom/2) - (m_pBack->GetHeight()/2));
				m_pBack->SetColorKey(TRANS_COLOR);
			}

			m_Btn1.LoadRes();
			m_Btn2.LoadRes();

			m_InputLine.Init(0, 0, 17*2, RGB(255, 255, 255), _FCOLOR_GREEN, FALSE, MSG_POPUP);
			m_InputLine.SetEnable(TRUE);
			m_InputLine.SetMaxLen(11);
			m_InputLine.SetfontSize(12);
			m_InputLine.SetPos(m_pBack->Xpos+97, m_pBack->Ypos+94);

			*m_Tmp1 = '\0';
			*m_Tmp2 = '\0';
			*m_Tmp3 = '\0';

			m_FontSize	= MSG_FONT_SIZE;
			m_FontColor = _FCOLOR_GREEN;
			m_bMultLine = FALSE;
			break;		
		}
	}
	RestoreAll();
}


//----------------------------------------------------------------------------------
//		DRAW!!!
//----------------------------------------------------------------------------------
void CMsgPopUp::Draw()
{
	if( !m_pBack )
		return;

	if(m_bActive)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);

		if (pCMyApp->m_pMouse && m_pBack->IsIn(point.x, point.y))
			pCMyApp->m_pMouse->SetMouseType(M_NORMAL);

		switch(m_CurType)
		{
		case TYPE_NOR_OK:
			{			
				DrawText();
				m_Btn1.Draw(m_pBack->Xpos+132, m_pBack->Ypos+88);
			}
			break;

		case TYPE_NOR_OKCANCLE:
		case TYPE_NOR_ACCEPTREJECT:
			{
				DrawText();
				m_Btn1.Draw(m_pBack->Xpos+101, m_pBack->Ypos+88);
				m_Btn2.Draw(m_pBack->Xpos+155, m_pBack->Ypos+88);
			}			
			break;

		case TYPE_NOR_OKCANCELCHECK:
			{
				DrawText();
				m_Btn1.Draw(m_pBack->Xpos+101, m_pBack->Ypos+81);
				m_Btn2.Draw(m_pBack->Xpos+155, m_pBack->Ypos+81);
				m_chkBox.SetPosition(m_pBack->Xpos+70, m_pBack->Ypos+103);
				m_chkBox.Draw();
			}
			break;

		case TYPE_ESC:
			{
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);

				if(pCMyApp->GetCurGameMode() == NKGRAPHIC3D)
				{

					m_Btn1.Draw(m_pBack->Xpos+44, m_pBack->Ypos+35);
					m_Btn2.Draw(m_pBack->Xpos+44, m_pBack->Ypos+65);
					m_Btn3.Draw(m_pBack->Xpos+44, m_pBack->Ypos+95); // 기능이 추가되면 실행해도 무관하다

				}
				else
				{
					m_Btn1.Draw(m_pBack->Xpos+44, m_pBack->Ypos+47);
					m_Btn2.Draw(m_pBack->Xpos+44, m_pBack->Ypos+77);
				}
			}
			break;

		case TYPE_DEL:
			{			
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				m_Btn1.Draw(m_pBack->Xpos+68, m_pBack->Ypos+90);
				m_Btn2.Draw(m_pBack->Xpos+127, m_pBack->Ypos+90);

				// Putstring 출력용
				if(m_bMultLine)
				{
					if( m_pSurf3 ) // 3줄째가 있으면... ///메세지박스3줄확장
					{
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+10, m_pSurf);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+10+20, m_pSurf2);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp3)/2, m_pBack->Ypos+10+40, m_pSurf3);
					}
					else
					{
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+20, m_pSurf);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+20+20, m_pSurf2);
					}
				}
				else
					g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+20, m_pSurf);

				m_InputLine.SetEnable(true);
				m_InputLine.Draw();
			}
			break;

		case TYPE_EXCHANGE:
		case TYPE_SHOUT:
			{
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				if( m_CurType == TYPE_SHOUT )
				{
					m_Btn1.Draw(m_pBack->Xpos+188, m_pBack->Ypos+90);
					m_Btn2.Draw(m_pBack->Xpos+247, m_pBack->Ypos+90);
				}
				else
				{
					m_Btn1.Draw(m_pBack->Xpos+68, m_pBack->Ypos+90);
					m_Btn2.Draw(m_pBack->Xpos+127, m_pBack->Ypos+90);
				}				

				// Putstring 출력용.
				if(m_bMultLine)
				{
					if( m_pSurf3 ) // 3줄째가 있으면... ///메세지박스3줄확장
					{
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+10, m_pSurf);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+10+20, m_pSurf2);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp3)/2, m_pBack->Ypos+10+40, m_pSurf3);
					}
					else
					{
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+15, m_pSurf);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+15+20, m_pSurf2);
					}
				}
				else
					g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+20, m_pSurf);

				m_InputLine.SetEnable(true);
				m_InputLine.Draw();
			}
			break;

		case  TYPE_EXCHANGE_EX:  ///개인상점
			{			
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				m_Btn1.Draw(m_pBack->Xpos+68, m_pBack->Ypos+123);
				m_Btn2.Draw(m_pBack->Xpos+127, m_pBack->Ypos+123);

				// Putstring 출력용.
				if(m_bMultLine)
				{
					if( m_pSurf3 ) // 3줄째가 있으면... ///메세지박스3줄확장
					{
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+10, m_pSurf);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+10+20, m_pSurf2);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp3)/2, m_pBack->Ypos+10+40, m_pSurf3);
					}
					else
					{
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+15, m_pSurf);
						g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+15+20, m_pSurf2);
					}
				}
				else
					g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+20, m_pSurf);

				//			m_InputLine.SetEnable(true);
				m_InputLine.Draw();

				m_InputValue1.Draw();
				m_InputValue2.Draw();
				m_InputValue3.Draw();
				m_InputValue4.Draw();
				m_InputValue5.Draw();
			}
			break;

		case TYPE_NOR_OK_LARGE:
			{			
				DrawText();
				m_Btn1.Draw(m_pBack->Xpos+151, m_pBack->Ypos+83);
			}
			break;

		case TYPE_NOR_OKCANCLE_LARGE:
			{			
				DrawText();
				m_Btn1.Draw(m_pBack->Xpos+105, m_pBack->Ypos+83);
				m_Btn2.Draw(m_pBack->Xpos+205, m_pBack->Ypos+83);
			}
			break;

		case TYPE_PREMIUM_CHA_MOVE_NAME_INPUT:
			{			
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);

				m_Btn1.Draw(m_pBack->Xpos+56, m_pBack->Ypos+136);
				m_Btn2.Draw(m_pBack->Xpos+154, m_pBack->Ypos+136);

				m_InputLine.SetEnable(true);
				m_InputLine.Draw();
			}
			break;

		case TYPE_PREMIUM_CHA_MOVE_WAIT:
			{			
				DrawText();
			}
			break;

		case TYPE_CARU_RESULT:
			{
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				if( m_pCaruResultData )
				{
					m_pCaruResultData->Draw();
				}
				m_Btn1.Draw(m_pBack->Xpos+210, m_pBack->Ypos+303);
			}
			break;
		}

		{
			switch(m_CurType)
			{
			case TYPE_WANTED_CONFIRM :
				if( g_SvrType != ST_ADULT_ONLY )
					break;
				DrawText();
				m_Btn1.Draw(m_pBack->Xpos+160, m_pBack->Ypos+125);
				break;
			case TYPE_GUILD_INVASION_CONFIRM_CANCEL :
				DrawText();
				m_Btn1.PosX = m_pBack->Xpos+65;
				m_Btn1.PosY = m_pBack->Ypos+103;
				m_Btn2.PosX = m_pBack->Xpos+155;
				m_Btn2.PosY = m_pBack->Ypos+103;
				m_Btn1.Draw();
				m_Btn2.Draw();
				break;
			case TYPE_GUILD_CREATE_NAME_INPUT :
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				m_Btn1.PosX = m_pBack->Xpos+62;
				m_Btn1.PosY = m_pBack->Ypos+125;
				m_Btn2.PosX = m_pBack->Xpos+158;
				m_Btn2.PosY = m_pBack->Ypos+125;
				m_Btn1.Draw(m_Btn1.PosX, m_Btn1.PosY);
				m_Btn2.Draw(m_Btn2.PosX, m_Btn2.PosY);
				m_InputLine.SetEnable(true);
				m_InputLine.Draw();
				break;
			case TYPE_GUILD_CREATE_PROMPT:
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				m_Btn1.PosX = m_pBack->Xpos+62;
				m_Btn1.PosY = m_pBack->Ypos+125;
				m_Btn2.PosX = m_pBack->Xpos+158;
				m_Btn2.PosY = m_pBack->Ypos+125;
				m_Btn1.Draw(m_Btn1.PosX, m_Btn1.PosY);
				m_Btn2.Draw(m_Btn2.PosX, m_Btn2.PosY);
				break;
			case TYPE_GUILD_CONFIRM :
				DrawText();
				m_Btn1.PosX = m_pBack->Xpos+113;
				m_Btn1.PosY = m_pBack->Ypos+125;
				m_Btn1.Draw(m_Btn1.PosX, m_Btn1.PosY);
				break;
			case TYPE_GUILD_CONFIRM_CANCEL :
				DrawText();
				m_Btn1.PosX = m_pBack->Xpos+65;
				m_Btn1.PosY = m_pBack->Ypos+103;
				m_Btn2.PosX = m_pBack->Xpos+155;
				m_Btn2.PosY = m_pBack->Ypos+103;
				m_Btn1.Draw();
				m_Btn2.Draw();
				break;
			case TYPE_GUILD_RECOMMAND_JOIN :
				g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				m_Btn1.PosX = m_pBack->Xpos+62;
				m_Btn1.PosY = m_pBack->Ypos+125;
				m_Btn2.PosX = m_pBack->Xpos+158;
				m_Btn2.PosY = m_pBack->Ypos+125;
				m_Btn1.Draw(m_Btn1.PosX, m_Btn1.PosY);
				m_Btn2.Draw(m_Btn2.PosX, m_Btn2.PosY);
				m_InputLine.Draw();
				//m_InputValue1.Draw();
				break;

			case TYPE_GUILD_EXCHANGE :

				DrawText();
				//g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack,NULL);
				m_Btn1.PosX = m_pBack->Xpos+62;
				m_Btn1.PosY = m_pBack->Ypos+125;
				m_Btn2.PosX = m_pBack->Xpos+158;
				m_Btn2.PosY = m_pBack->Ypos+125;
				m_Btn1.Draw(m_Btn1.PosX, m_Btn1.PosY);
				m_Btn2.Draw(m_Btn2.PosX, m_Btn2.PosY);

				m_InputLine.SetEnable(true);
				m_InputLine.Draw();

				break;
			}
		}
	}
}


//----------------------------------------------------------------------------------
//		DELETE RESOURCE !!!
//----------------------------------------------------------------------------------
void CMsgPopUp::DeleteRes()
{
//	SAFE_DELETE(m_pBack);
	SAFE_DELETE(m_pSurf);
	SAFE_DELETE(m_pSurf2);
	SAFE_DELETE(m_pSurf3);	 ///메세지박스3줄확장
	m_InputLine.DeleteRes();
	m_InputValue1.DeleteRes();
	m_InputValue2.DeleteRes();
	m_InputValue3.DeleteRes();
	m_InputValue4.DeleteRes();
	m_InputValue5.DeleteRes();

	m_Btn1.DeleteRes();
	m_Btn2.DeleteRes();
	m_Btn3.DeleteRes();
	m_InputLine.DeleteRes();
	SAFE_DELETE(m_pCaruResultData);
}

void CMsgPopUp::DrawText()
{
	if( !g_pDisplay || !m_pBack )
		return;

	g_pDisplay->Blt(m_pBack->Xpos, m_pBack->Ypos, m_pBack, NULL );

	int nInterval = 0;

	if(m_CurType == TYPE_WANTED_CONFIRM && g_SvrType == ST_ADULT_ONLY )
		nInterval = 20;
	
	if( m_pCaruResultData )
	{
		m_pCaruResultData->Draw();
	}
	else if(m_bMultLine)
	{
		if( m_pSurf3 ) // 3줄째가 있으면... ///메세지박스3줄확장
		{
			g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp1)/2, m_pBack->Ypos+(60 / 3)-(m_FontSize/2)+15 + nInterval, m_pSurf);
			g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp2)/2, m_pBack->Ypos+(60 / 3)-(m_FontSize/2)+15+18 + nInterval, m_pSurf2);
			g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_FontSize/2)*strlen(m_Tmp3)/2, m_pBack->Ypos+(60 / 3)-(m_FontSize/2)+15+36 + nInterval, m_pSurf3);
		}
		else
		{
			g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_pSurf->GetWidth()/2), m_pBack->Ypos+(60 / 3)-(m_FontSize/2)+20 + nInterval, m_pSurf);
			g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_pSurf2->GetWidth()/2), m_pBack->Ypos+(60 / 3)-(m_FontSize/2)+20+20 + nInterval, m_pSurf2);
		}
	}
	else
	{
		// 출력 위치를 글자 수와 폰트 사이즈로 계산하지 않고 생성된 최종 서피스의 길이로 부터 셋팅하게 변경했음.( by 원석 )
		g_pDisplay->Blt(m_pBack->Xpos+(m_pBack->GetWidth()/2)-(m_pSurf->GetWidth()/2), m_pBack->Ypos+(60 / 2)-(m_FontSize/2)+20 + nInterval, m_pSurf);
	}
}


void CMsgPopUp::RestoreAll()
{
	if(g_pDisplay && m_bActive==TRUE)
	{
		int len = strlen( m_Tmp1);

		if(len == m_StrLimit+1)
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf )	m_pSurf->SetColorKey(RGB(0, 0, 1));
			m_bMultLine = FALSE;
		}
		else if(m_bMultLine == TRUE)
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf )	m_pSurf->SetColorKey(RGB(0, 0, 1));
			g_pDisplay->CreateSurfaceFromText(&m_pSurf2, m_Font, m_Tmp2, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf2 )	m_pSurf2->SetColorKey(RGB(0, 0, 1));
			g_pDisplay->CreateSurfaceFromText(&m_pSurf3, m_Font, m_Tmp3, RGB(0, 0, 1), m_FontColor); ///메세지박스3줄확장
			if( m_pSurf3 )	m_pSurf3->SetColorKey(RGB(0, 0, 1));		 ///메세지박스3줄확장
		}
		else
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf )	m_pSurf->SetColorKey(RGB(0, 0, 1));
		}
	}

	if( m_pCaruResultData )
	{
		m_pCaruResultData->Restore();
	}
	m_InputLine.RestoreSurfaces();
	m_InputValue1.RestoreSurfaces();
	m_InputValue2.RestoreSurfaces();
	m_InputValue3.RestoreSurfaces();
	m_InputValue4.RestoreSurfaces();
	m_InputValue5.RestoreSurfaces();
}



//----------------------------------------------------------------------------------
//		MSG PROCEDURE
//----------------------------------------------------------------------------------
LRESULT CMsgPopUp::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	pCMyApp->m_IsMsgPopupInputEnable = m_InputLine.GetEnable();

	switch ( msg )
	{
	case WM_MOUSEMOVE:
		m_Btn1.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn2.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn3.MsgProc(hWnd, msg, wParam, lParam);		
		break;

	case WM_LBUTTONDOWN:
		m_Btn1.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn2.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn3.MsgProc(hWnd, msg, wParam, lParam);		

		m_InputLine.MsgProc(hWnd, msg, wParam, lParam);
		m_InputValue1.MsgProc(hWnd, msg, wParam, lParam); ///개인상점
		m_InputValue2.MsgProc(hWnd, msg, wParam, lParam);
		m_InputValue3.MsgProc(hWnd, msg, wParam, lParam);
		m_InputValue4.MsgProc(hWnd, msg, wParam, lParam);
		m_InputValue5.MsgProc(hWnd, msg, wParam, lParam);


		m_chkBox.MsgProc(hWnd, msg, wParam, lParam);
		break;

	case WM_LBUTTONUP:
		m_Btn1.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn2.MsgProc(hWnd, msg, wParam, lParam);
		m_Btn3.MsgProc(hWnd, msg, wParam, lParam);		
		
		if ( m_Btn1.GetState() == BTN_ACTION )
		{
			m_Btn1.m_iBtnFlg = BTN_NORMAL;			
			Action( OK_CLICK );
			return 1;
		}
		else if ( m_Btn2.GetState() == BTN_ACTION )
		{
			m_Btn2.m_iBtnFlg = BTN_NORMAL;
			Action( CANCLE_CLICK );
			return -1;
		}
		else if ( m_Btn3.GetState() == BTN_ACTION )
		{
			{
				m_Btn3.m_iBtnFlg = BTN_NORMAL;
				m_bActive = FALSE;
				return 2;
			}
		}
		break;

	case WM_KEYDOWN:
		if(wParam == VK_RETURN)
		{
			if(pCMyApp->m_bInverseReturn)//0124 +아템 버릴때나 상점에 팔때 리턴은 취소.
				return -1;
			if(m_CurType == TYPE_ESC)//0318
				return -1;

			m_Btn1.m_iBtnFlg = BTN_NORMAL;
			Action( OK_CLICK );
			return 1;
		}
		else if (wParam == VK_ESCAPE)
		{
			return -1;
		}
		else if(wParam ==VK_TAB  ) // 탭을 누르면 입력창 이동할수있도록...///개인상점 시 작업.
		{
			if( m_InputLine.GetEnable() )
			{
				m_InputLine.SetEnable( FALSE );
				m_InputValue4.SetEnable( TRUE );
			}
			else if( m_InputValue4.GetEnable() )
			{
				m_InputValue4.SetEnable( FALSE );
				m_InputValue5.SetEnable( TRUE );
			}
			else if( m_InputValue5.GetEnable() )
			{
				m_InputValue5.SetEnable( FALSE );
				m_InputValue1.SetEnable( TRUE );
			}
			else if( m_InputValue1.GetEnable() )
			{
				m_InputValue1.SetEnable( FALSE );
				m_InputValue2.SetEnable( TRUE );
			}
			else if( m_InputValue2.GetEnable() )
			{
				m_InputValue2.SetEnable( FALSE );
				m_InputValue3.SetEnable( TRUE );
			}
			else if( m_InputValue3.GetEnable() )
			{
				m_InputValue3.SetEnable( FALSE );
				m_InputLine.SetEnable( TRUE );
			}
			
			
		}


	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_InputLine.GetEnable() )	///개인상점 시 작업.
		{
			{
				g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, NULL, GetCurrentThreadId());// nate 2006-03-15 : 메세지 훅 설정
			}
			m_InputLine.MsgProc( hWnd, msg, wParam, lParam );
		}
		else if( m_InputValue1.GetEnable() )
			m_InputValue1.MsgProc(hWnd, msg, wParam, lParam);
		else if( m_InputValue2.GetEnable() )
			m_InputValue2.MsgProc(hWnd, msg, wParam, lParam);
		else if( m_InputValue3.GetEnable() )
			m_InputValue3.MsgProc(hWnd, msg, wParam, lParam);
		else if( m_InputValue4.GetEnable() )
			m_InputValue4.MsgProc(hWnd, msg, wParam, lParam);
		else if( m_InputValue5.GetEnable() )
			m_InputValue5.MsgProc(hWnd, msg, wParam, lParam);
		break;

	}
	return 0;
}

//----------------------------------------------------------------------------------
//		BUTTON  ACTION
//----------------------------------------------------------------------------------
// [6/4/2007 Theodoric] 여기서 길드
void CMsgPopUp::Action( int i )
{
	SAFE_DELETE(m_pSurf);
	SAFE_DELETE(m_pSurf2);
	SAFE_DELETE(m_pSurf3);	 ///메세지박스3줄확장
	m_bActive = FALSE;
	pCMyApp->m_IsMsgPopupInputEnable = FALSE;

	switch(i)
	{
	case OK_CLICK:
		switch( m_CurType )
		{
		case TYPE_GUILD_CREATE_PROMPT:
			g_pTcpIp->SendNetMessage("mg create t\n");
			break;
		}
		break;

	case CANCLE_CLICK:
		break;

	}
}

//----------------------------------------------------------------------------------
//		만약 메세지를 받았다면 출력해라!!!
//----------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// 단어 단위로 줄 바꿈. [2/6/2007 Theodoric]
void CMsgPopUp::PutString(char* String)
{
//////////////////////////////////////////////////////////////////////////
// test string
//
//	char teststring[1024]="당신은 새를 기르고 있지 않습니다. 당신은 새를 기르고 있지 않습니다. 당신은 새를 기로고 있지 않습니다. 당신은 새를 기르고 있지 않습니다. 당신은 새를 기르고 있지 않습니다. 당신은 새를 기로고 있지 않습니다.";
//	char teststring[1024]="당신은새를기르고있지않습니다. 당신은새를기르고있지않습니다.당신은새를기로고있지않습니다.당신은 새를 기르고 있지 않습니다. 당신은 새를 기르고 있지 않습니다. 당신은 새를 기로고 있지 않습니다.";
//	char teststring[1024]="I love You! Do you understandingthing? Are u Okay? I'm fine, and you? i'm okay. *^^*I love You! Do you understandingthing? Are u Okay? I'm fine, and you? i'm okay. *^^*";
//	char teststring[1024]="IloveYou!Doyouunderstandingthing?AreyouOkay? I'm fine, andyou?i'mokay. *^^* I love You!Doyouunderstandingthing?Areu Okay? I'm fine, and you? i'm okay. *^^*";
//	String = teststring;
//
	//////////////////////////////////////////////////////////////////////////

	if(String == NULL)
		return;

	int descr_len = strlen(String);

	if (descr_len <= 0)
		return;

	if (!g_pDisplay )
		return;

	if(!m_Font)
		return;

	ZeroMemory(m_Tmp1, strlen(m_Tmp1)+1);
	ZeroMemory(m_Tmp2, strlen(m_Tmp2)+1);
	ZeroMemory(m_Tmp3, strlen(m_Tmp3)+1);

	if (m_pSurf)
	{
		SAFE_DELETE(m_pSurf);
	}
	if (m_pSurf2)
	{
		SAFE_DELETE(m_pSurf2);
	}
	if (m_pSurf3) ///메세지박스3줄확장
	{
		SAFE_DELETE(m_pSurf3);
	}

	int		pos = 0;
	int		idx = 0;
	int		idx_tmp = 0;
	int		Tmpcount = 0;
	int		CheckPoint = 0;
	BOOL	is_han = FALSE;
	char	descr_buf[102] = {0,};
	char	print_buf[102] = {0,};

	if( descr_len < m_StrLimit+2 ) // 출력할 문장이 출력 제한 사이즈 보다 작으면 그냥 한줄 출력한다.
	{
		strcpy(m_Tmp1, String);
		m_Tmp1[m_StrLimit+1] = '\0';
		g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
		if( m_pSurf )	m_pSurf->SetColorKey(RGB(0, 0, 1));
		m_bMultLine = FALSE;
	}
	else // 한줄이상 출력해야 하면
	{
		m_bMultLine = TRUE;

		while (pos < descr_len) // 문장 길이 만큼 무조건 루프.
		{
			Tmpcount ++;

// 			if( Tmpcount > 3) // 3줄이 넘어가면 무조건 종료.
// 			{	break;	}

			idx			= 0;
			is_han		= FALSE;
			CheckPoint	= 0;

			ZeroMemory(descr_buf, strlen(descr_buf)+1);
			ZeroMemory(print_buf, strlen(print_buf)+1);

			while(1)// 문장 시작시 공백이 있음 지워준다.
			{
				BOOL bBreakTime = FALSE;

				switch( g_dwLangType )
				{
				case LANG_JPN:	// 일본
					if( String[pos] == 0x20)  // 반각 공백일때.
					{
						pos++;
					}
					else if( String[pos] == -127 && String[pos+1] == 0x40)  // 전각 공백일때
					{
						pos += 2;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;

				case LANG_TWN:	// 대만
					if( String[pos] == 0x20)  // 반각 공백일때.
					{
						pos++;
					}
					else if( String[pos] == -95 && String[pos+1] == 0x40)  // 전각 공백일때
					{
						pos += 2;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;

				case LANG_CN:	// 중국
					if( String[pos] == 0x20)  // 반각 공백일때.
					{
						pos++;
					}
					else if( String[pos] == -95 && String[pos+1] == -95)  // 전각 공백일때
					{
						pos += 2;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;

				default:
					if( String[pos] == 0x20)
					{
						pos++;
					}
					else
					{
						bBreakTime = TRUE;
					}
					break;
				}
				if(bBreakTime)
				{
					break;
				}
			}

			// 3번째 줄은 m_tmp3 크기 만큼만 문자열을 출력한다.
			if( Tmpcount > 2)
			{
				int size = sizeof(m_Tmp3)-2;
				int len = strlen(String) - pos;

				if( size < len )
				{
					if( String[pos+size] < 0 || String[pos+size] > 127 )
					{
						size--;
					}
				}
				else
				{
					size = len;
				}

				memcpy(m_Tmp3, &String[pos], size);
				break;
			}

			while((idx < m_StrLimit || is_han) && pos < descr_len) // 지정된 길이 만큼 복사 후 멈 춘다.
			{
				//단어 사이의 공백을 체크 한다.
				switch( g_dwLangType)
				{
				case LANG_JPN:	// 일본
					if((String[pos-1] != 0x20  && String[pos] == 0x20)		// 반각일때.
							|| ((String[pos-2] != -127 && String[pos-1] != 0x40)	// 전각일때.
								&& (String[pos] == -127 && String[pos+1] == 0x40)))
					{
						CheckPoint = idx;
					}
					break;

				case LANG_TWN:	// 대만
					if((String[pos-1] != 0x20  && String[pos] == 0x20)		// 반각일때.
							|| ((String[pos-2] != -95 && String[pos-1] != 0x40)		// 전각일때.
								&& (String[pos] == -95 && String[pos+1] == 0x40)))
					{
						CheckPoint = idx;
					}
					break;

				case LANG_CN:  // 중국
					if((String[pos-1] != 0x20  && String[pos] == 0x20)		// 반각일때.
							|| (String[pos-1] != -95 && String[pos] == -95))		// 전각일때.
					{
						CheckPoint = idx;
					}
					break;

				default:
					if(String[pos-1] != 0x20  && String[pos] == 0x20)
					{
						CheckPoint = idx;
					}
					break;
				}

				if (String[pos] < 0 || String[pos] > 127)
				{
					if (is_han) // 한글인지 체크
					{
						is_han = FALSE;
					}
					else
					{
						is_han = TRUE;
					}
				}
				else
				{
					is_han = FALSE;
				}

				// 인텍스 증가, 포지션 증가
				descr_buf[idx++] = String[pos++];

				// 			if( string[pos] == 0x0a0d) // enter 발견
				// 			{
				// 				pos++;
				// 				idx++;
				// 				break;
				//			}
			}

			if( CheckPoint > 0 && pos < descr_len )  // 한 단어의 길이가 출력할 폼의 사이즈 보다 길때.
			{
				memcpy(print_buf, descr_buf, CheckPoint);
				pos = pos - (idx - CheckPoint) +1;

				switch( g_dwLangType )
				{
				case LANG_JPN:
					if( String[pos-1] == -127 )
					{
						pos ++;
					}
					break;	// 일본
				case LANG_TWN:
					if( String[pos-1] == -95  )
					{
						pos ++;
					}
					break;	// 대만
				case LANG_CN:
					if( String[pos-1] == -95  )
					{
						pos ++;
					}
					break;	// 중국
				}

				idx = CheckPoint;
			}
			else
			{
				memcpy(print_buf, descr_buf, idx);
			}

			print_buf[strlen(print_buf)+1] = '\0';

			switch(Tmpcount)
			{
			case 1:
				ZeroMemory(m_Tmp1, strlen(print_buf)+2);
				memcpy(m_Tmp1, print_buf, strlen(print_buf)+1);
				break;
			case 2:
				ZeroMemory(m_Tmp2, strlen(print_buf)+2);
				memcpy(m_Tmp2, print_buf, strlen(print_buf)+1);
				break;
			case 3:
				ZeroMemory(m_Tmp3, strlen(print_buf)+2);
				memcpy(m_Tmp3, print_buf, strlen(print_buf)+1);
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 문장이 3줄을 넘어가면 "..." 로 여운을 남기자... 필요없음 바로 삭제 하자
// 	if( Tmpcount == 4 )
// 	{
// 		if( (print_buf[strlen(print_buf)-1] < 0 || print_buf[strlen(print_buf)-1] > 127)  &&
// 		    (print_buf[strlen(print_buf)-2] < 0 || print_buf[strlen(print_buf)-2] > 127) )
// 		{
// 			print_buf[strlen(print_buf)-2] = '.';
// 		}
//
// 		print_buf[strlen(print_buf)-1] = '.';
// 		print_buf[strlen(print_buf)] = '.';
//
// 		ZeroMemory(m_Tmp3, strlen(print_buf)+2);
// 		memcpy(m_Tmp3, print_buf, strlen(print_buf)+1);
// 	}
	//////////////////////////////////////////////////////////////////////////

	if( strlen(m_Tmp1) )
	{
		if( !m_pSurf ) // surface를 생성했는데 또 생성하면 충돌이 일어난다. 생성 되어있다면 생성하지 말자.
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
			if(m_pSurf)
			{
				m_pSurf->SetColorKey(RGB(0, 0, 1));
			}
		}
	}

	if( strlen(m_Tmp2) )
	{
		if( !m_pSurf2 )
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf2, m_Font, m_Tmp2, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf2 )
			{
				m_pSurf2->SetColorKey(RGB(0, 0, 1));
			}
		}
	}

	if( strlen(m_Tmp3) )
	{
		if( !m_pSurf3 )
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf3, m_Font, m_Tmp3, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf3 )
			{
				m_pSurf3->SetColorKey(RGB(0, 0, 1));
			}
		}
	}

	m_bActive = TRUE;
}
//////////////////////////////////////////////////////////////////////////


/*
void CMsgPopUp::PutString(char* String)
{
	if(String == NULL)
		return;

	int len = strlen(String);

	int Count = 0;
	if (len <= 0)
		return;

	if (!g_pDisplay || !m_Font)
		return;

	for(int i=0; i<m_StrLimit; i++)
	{
		if(String[i] > 0 )
			Count++;
	}

	if (m_pSurf)
		SAFE_DELETE(m_pSurf);
	if (m_pSurf2)
		SAFE_DELETE(m_pSurf2);
	if (m_pSurf3) ///메세지박스3줄확장
		SAFE_DELETE(m_pSurf3);

	// 스트링의 길이가 20바이트를 넘으면 자른다
	if(len >m_StrLimit+2)
	{
		m_bMultLine = TRUE ;// 여러줄 찍기
		int OnebyteCnt = 0;
		int cutline = 0;

		if(g_dwLangType == 1)
		{ // for japan

			BOOL bFirst = FALSE;
			for (int i = 0; i < m_StrLimit; i++)
			{
				if (bFirst)
					bFirst = FALSE;
				else if (String[i] < 0)
					bFirst = TRUE;
			}
			if (bFirst)
			{
				strncpy(m_Tmp1, String, m_StrLimit-1);
				m_Tmp1[m_StrLimit-1] = '\0';
				strcpy(m_Tmp2, &String[m_StrLimit-1]);
			}
			else
			{
				strncpy(m_Tmp1, String, m_StrLimit);
				m_Tmp1[m_StrLimit] = '\0';
				strcpy(m_Tmp2, &String[m_StrLimit]);
			}

			// 세줄까지 확장. ///메세지박스3줄확장
			if( strlen(m_Tmp2) > m_StrLimit+2 ) // 둘째줄도 한계치를 넘으면...
			{
				// 한번더 짜른다. // 그냥 위의 루틴으로...
				BOOL bFirst = FALSE;
				for (int i = 0; i < m_StrLimit; i++)
				{
					if (bFirst)
						bFirst = FALSE;
					else if (m_Tmp2[i] < 0)
						bFirst = TRUE;
				}
				if (bFirst)
				{
					strcpy(m_Tmp3, &m_Tmp2[m_StrLimit-1]);	// m_Tmp3에 뒤쪽을 넣어주고.
					m_Tmp2[m_StrLimit-1] = '\0';            // m_Tmp2를 끊어준다.
				}
				else
				{
					strcpy(m_Tmp2, &m_Tmp2[m_StrLimit]); 	// m_Tmp3에 뒤쪽을 넣어주고.
					m_Tmp2[m_StrLimit] = '\0';			    // m_Tmp2를 끊어준다.
				}
			}
		}
		else if(g_dwLangType == 0 || g_dwLangType == 3 || g_dwLangType == 4)// 한국 /중국
		{
			bool bf = false;
			for(int i = 0; i<m_StrLimit; i++)
			{
				if((unsigned char)String[i] & 0x80)
				{
					if(bf)
						bf = false;
					else
						bf = true;
				}else
					bf = false;
			}
			if(bf)
			{
				strcpy(m_Tmp1, String);
				m_Tmp1[m_StrLimit-1] = '\0';
				strcpy(m_Tmp2, &String[m_StrLimit-1]);
			}
			else
			{
				strcpy(m_Tmp1, String);
				m_Tmp1[m_StrLimit] = '\0';
				strcpy(m_Tmp2, &String[m_StrLimit]);
			}

			// 세줄까지 확장. ///메세지박스3줄확장
			if( strlen(m_Tmp2) > m_StrLimit+2 ) // 둘째줄도 한계치를 넘으면...
			{
				// 한번더 짜른다. // 그냥 위의 루틴으로...
				bool bf = false;
				for(int i = 0; i<m_StrLimit; i++)
				{
					if((unsigned char)m_Tmp2[i] & 0x80)
					{
						if(bf)
							bf = false;
						else
							bf = true;
					}else
						bf = false;
				}
				if(bf)
				{
					strcpy(m_Tmp3, &m_Tmp2[m_StrLimit-1]);	// m_Tmp3에 뒤쪽을 넣어주고.
					m_Tmp2[m_StrLimit-1] = '\0';            // m_Tmp2를 끊어준다.
				} else {
					strcpy(m_Tmp3, &m_Tmp2[m_StrLimit]); 	// m_Tmp3에 뒤쪽을 넣어주고.
					m_Tmp2[m_StrLimit] = '\0';			    // m_Tmp2를 끊어준다.
				}
			}
		}
		else
		{
			strcpy(m_Tmp1, String);
			m_Tmp1[m_StrLimit] = '\0';
			strcpy(m_Tmp2, &String[m_StrLimit]);

			// 세줄까지 확장. ///메세지박스3줄확장
			if( strlen(m_Tmp2) > m_StrLimit+2 ) // 둘째줄도 한계치를 넘으면...
			{
				strcpy(m_Tmp3, &m_Tmp2[m_StrLimit]);
				m_Tmp2[m_StrLimit] = '\0';
			}
		}

		if( g_pDisplay )
		{
			g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf )	m_pSurf->SetColorKey(RGB(0, 0, 1));
			g_pDisplay->CreateSurfaceFromText(&m_pSurf2, m_Font, m_Tmp2, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf2 )	m_pSurf2->SetColorKey(RGB(0, 0, 1));

			if( strlen(m_Tmp3) ) // 셋째줄이 잇을때만 m_pSurf3을 만들자. ///메세지박스3줄확장
			{
				g_pDisplay->CreateSurfaceFromText(&m_pSurf3, m_Font, m_Tmp3, RGB(0, 0, 1), m_FontColor);
				if( m_pSurf3 )	m_pSurf3->SetColorKey(RGB(0, 0, 1));
			}
		}
	}
	else
	{
		if( g_pDisplay )
		{
			strcpy(m_Tmp1, String);
			m_Tmp1[m_StrLimit+1] = '\0';
			g_pDisplay->CreateSurfaceFromText(&m_pSurf, m_Font, m_Tmp1, RGB(0, 0, 1), m_FontColor);
			if( m_pSurf )	m_pSurf->SetColorKey(RGB(0, 0, 1));
			m_bMultLine = FALSE;
		}
	}
	m_bActive = TRUE;
}
*/






//----------------------------------------------------------------------------------
//		메세지가 들어와서 SURFACE != NULL 이아니면 로드된 리소스들을 DRAW시켜도 좋다!!
//----------------------------------------------------------------------------------
BOOL CMsgPopUp::CheckMsgExist()
{
	if(m_bActive == TRUE)
		return TRUE;
	else
		return FALSE;
}



//----------------------------------------------------------------------------------
//		TEXT를 출력할때 쓰일 SURFACE를 최초 클리어해주자..!!
//----------------------------------------------------------------------------------
void CMsgPopUp::ClearSurface(CSurface* surface)
{
	ZeroMemory(&surface, sizeof(surface));
}


//----------------------------------------------------------------------------
//-- Set Font
//----------------------------------------------------------------------------
//-- Global Font 중 하나를 받아온다.

//-- 2004.03.15 - 수정완료
//-- Lyul
//-- 자체 생성에서 Global 사용으로 변경
void CMsgPopUp::SetFont (int f_w, int f_h, char *fontname)
{
	// Font 생성
	m_Font = G_GetFont(f_h,0,fontname);
}

char* CMsgPopUp::GetCheckPW()
{
	return m_InputLine.GetStrBuf();
}

char* CMsgPopUp::GetInputValue1() ///개인상점
{
	return m_InputValue1.GetStrBuf();
}

char* CMsgPopUp::GetInputValue2() ///개인상점
{
	return m_InputValue2.GetStrBuf();
}

char* CMsgPopUp::GetInputValue3() ///개인상점
{
	return m_InputValue3.GetStrBuf();
}
char* CMsgPopUp::GetInputValue4() ///개인상점
{
	return m_InputValue4.GetStrBuf();
}
char* CMsgPopUp::GetInputValue5() ///개인상점
{
	return m_InputValue5.GetStrBuf();
}

void CMsgPopUp::SetTextColor(DWORD color)
{
	m_FontColor = color;
}

void CMsgPopUp::SetCheckBoxText(const std::string& text)
{
	m_chkBox.SetText(text);
}

bool CMsgPopUp::IsCheckBoxChecked()
{
	if( m_CurType != TYPE_NOR_OKCANCELCHECK )
		return false;

	return m_chkBox.GetChecked();
}
