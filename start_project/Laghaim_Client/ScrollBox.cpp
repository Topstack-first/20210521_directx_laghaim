#include "stdafx.h"


#define D3D_OVERLOADS

#include <math.h>
#include <d3d.h>
#include <assert.h>
#include <Def.h>
#include "d3dutil.h"
#include "IndexedTexture.h"
#include "scrollbox.h"
#include "TextOutBox.h"
#include "main.h"
#include "basicbutton.h"
#include "textbox.h"
#include "headers.h"
#include "Nk2DFrame.h"
#include "LoadEffectSound.h"
//===============================
// nate 2004 - 7
// Image Manager
#include "ImageDefine.h"
//===============================
#include "Country.h"
#include "Mouse.h"


#define TEXT_SIZE		12
#define TEXT_CY_GAP		5
#define LIMIT_STR_CNT   15
#define ONE_LINE_CY     (TEXT_SIZE + TEXT_CY_GAP)
#define ONE_LINE_CX     (TEXT_SIZE*LIMIT_STR_CNT)
#define BAR_CX			TEXT_SIZE
#define BAR_CY			ONE_LINE_CY

#define NAME_COLOR			RGB(255, 255, 255)
#define TEXT_OVER_COLOR		RGB(255, 255, 0)
#define TEXT_CLICK_COLOR	RGB(255, 0, 0)





extern int g_EffectTextr[];
extern DWORD g_dwLangType;

//-------------------------------------------------------------------
// name : 생성자
// desc : Type를 설정할수 있다 bLink는 아직 사용하지 않는다
//-------------------------------------------------------------------
CScrollBox::CScrollBox(int Type, BOOL bLink)
	: m_bMainActive(FALSE)
	, m_bDown(FALSE)
	, m_RenderXpos(0)
	, m_RenderYpos(0)
	, m_RenderWidth(0)
	, m_RenderHeight(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_iListBox_X(0)
	, m_iListBox_Y(0)
	, m_iListBox_Cx(0)
	, m_iListBox_Cy(0)
	, m_iStartViewCnt(0)
	, m_iEndViewCnt(0)
	, m_iViewCnt(0)
	, m_eState(NONE)
{
	memset(m_sPreVal, 0, sizeof(m_sPreVal));

	m_iMaxListCnt = 0;
	m_hWnd		  = NULL;

	m_pTitleSur   = NULL;
	m_pListSur    = NULL;
	m_sListStr    = NULL;
	m_sTitleStr   = NULL;
	m_pArrowUp    = NULL;
	m_pArrowDown  = NULL;
	m_pBarSur	  = NULL;
	m_pListSur    = NULL;
	m_pOverSur    = NULL;
	m_pClickSur   = NULL;
	m_pBtn1		  = NULL;
	m_pBtn2		  = NULL;
	m_pBtn3       = NULL;
	m_pBtn4       = NULL;
	m_pInBox	  = NULL;
	m_pBox		  = NULL;

	m_bLink = bLink;
	m_bTitle = TRUE;
	m_iCurListNum = 0;
	m_iCurType    = Type;

	m_IsVisible		= TRUE;

	InitBtn(m_iCurType);
}



CScrollBox::~CScrollBox()
{
	DeleteRes();
}


//-------------------------------------------------------------------
// name : Init
// desc : 직접입력방식에서 쓰인다 인자로는 생성할 리스트의 갯수와
//		  보여줄 리스트의 갯수를 인자로 받는다
//-------------------------------------------------------------------
void CScrollBox::Init(int MaxListCnt, int ViewCnt)
{
	SetMaxListCnt(MaxListCnt);

	ViewCnt > m_iMaxListCnt ? m_iViewCnt = m_iMaxListCnt :	m_iViewCnt = ViewCnt;

	m_iStartViewCnt = 0;
	m_iEndViewCnt   = m_iStartViewCnt+m_iViewCnt;
	m_bMainActive = TRUE;
}


void CScrollBox::InitFromFile(char*fname, int ViewCnt)
{
	if( fname == NULL )
		return;

	m_bMainActive = ReadInfoFromTxt(fname);

	// Scroll Button Scroll Bar..
	if(ViewCnt > m_iMaxListCnt)
		ViewCnt = m_iMaxListCnt;

	m_iStartViewCnt = 0;
	m_iViewCnt      = ViewCnt;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;

	m_iMaxListCnt - m_iViewCnt > 0 ? m_bScroll = TRUE : m_bScroll = FALSE;

	m_eState = NONE;
}


//-------------------------------------------------------------------
// name : InitBtn
// desc : "ok", "cancle", "back", "next" 버튼들을 초기화 시킨다
//-------------------------------------------------------------------
void CScrollBox::InitBtn(int type)
{
	m_iCurType = type;

	switch(type)
	{
	case TYPE_REDISTRIBUTION:
		m_pBtn1 = new CBasicButton();
		m_pBtn2 = new CBasicButton();

		m_pBtn1->SetFileName("menu/btn_accept");
		m_pBtn1->SetDisable( false );
		m_pBtn2->SetFileName("menu/btn_cancel");
		m_pBtn2->SetDisable( false );

		m_pInBox = new CTextBox();
		m_pInBox->Init(0, 0, 14, NULL, RGB(0, 255, 0));
		m_pInBox->SetEnable(TRUE);

		break;

	case TYPE_DELIV_BOX:
		m_pBtn1 = new CBasicButton();
		m_pBtn2 = new CBasicButton();

		m_pBtn1->SetFileName("menu/btn_accept");
		m_pBtn1->SetDisable( false );
		m_pBtn2->SetFileName("menu/btn_cancel");
		m_pBtn2->SetDisable( false );
		m_bScroll = false;
		break;

	case TYPE_GUILD_NOTICE:
	case TYPE_HELP_BOX://1128
		m_pBtn1 = new CBasicButton();
		m_pBtn1->SetFileName("menu/btn_accept");
		m_pBtn1->SetDisable( false );
		m_bScroll = false;
		break;

	case TYPE_MULTI_PG:
		m_pBtn1 = new CBasicButton();
		m_pBtn3 = new CBasicButton();
		m_pBtn4 = new CBasicButton();

		m_pBtn1->SetFileName("menu/btn_accept");
		m_pBtn1->SetDisable( false );
		m_pBtn3->SetFileName("menu/info_back");
		m_pBtn3->SetDisable( false );
		m_pBtn4->SetFileName("menu/info_next");
		m_pBtn4->SetDisable(false);
		break;
	}
}




//-------------------------------------------------------------------
// name : SetMaxListCnt
// desc : 생성할 리스트의 총갯수만큼만 스트링 버퍼를 생성한다
//-------------------------------------------------------------------
void CScrollBox::SetMaxListCnt(int cnt)
{
	m_iMaxListCnt = cnt;

	m_sListStr = new char*[m_iMaxListCnt];

	for(int i=0; i<m_iMaxListCnt; i++)
	{
		m_sListStr[i] = new char[200];
		m_sListStr[i][0] = '\0'; ///국가대항전 시에 버그 발견하여 수정.릴리즈에서 쓰레기값 들어가자나!
	}
}


//-------------------------------------------------------------------
// name : SetTitleStr
// desc : "타이틀" 부분을 위한 스트링을 얻는다
//-------------------------------------------------------------------
BOOL CScrollBox::SetTitleStr(char* title)
{
	if(title == NULL)
		return FALSE;

	int len = strlen(title);

	if (m_sTitleStr)
		delete[] m_sTitleStr;
	m_sTitleStr = new char[len+1];
	strcpy(m_sTitleStr, title);

	m_bTitle = TRUE;

	return true;
}

//-------------------------------------------------------------------
// name : SetAddStr
// desc : 리스트를 생성한다
//-------------------------------------------------------------------
BOOL CScrollBox::SetAddStr(char* str)
{
	if(!str)
		return FALSE;

	if(m_iCurListNum >= m_iMaxListCnt)
		return false;

	strcpy(m_sListStr[m_iCurListNum++], str);
	return true;
}


void CScrollBox::AddStringMulti(char *string, int len, BOOL flag, BOOL Enable_Enter)
{
	if (!string)
		return;

	char descr_buf[102] = {0,};

	int idx_tmp=0;

	int descr_len = strlen(string);
	if (descr_len <= 0)
		return;
	if (len > 100)
		len = 100;
	int pos = 0;

	while (pos < descr_len)
	{
		int idx = 0;
		BOOL is_han = FALSE;
		while ((idx < len || is_han) && pos < descr_len)
		{
			if (string[pos] < 0 || string[pos] > 127)
			{
				if (is_han)
					is_han = FALSE;
				else
					is_han = TRUE;
			}
			else
			{
				is_han = FALSE;
			}

			Enable_Enter = 1;

			if( !Enable_Enter )
			{
				if( string[pos] != '\n' && string[pos] != '\r' && string[pos] != '\t' )
					descr_buf[idx++] = string[pos++];
				else
				{
					pos++;
					descr_buf[idx++] = ' ';
				}
			}
			else
			{
				if( string[pos] == '\n' )
				{
					pos++;
					idx = len;
					is_han = FALSE;
				}
				else if( string[pos] != '\r' && string[pos] != '\t' )
					descr_buf[idx++] = string[pos++];
				else
				{
					pos++;
					descr_buf[idx++] = ' ';
				}
			}
		}

		if(g_dwLangType == 2 || g_dwLangType == 5 || g_dwLangType == LANG_DEU || g_dwLangType == LANG_FR)
		{
			if(flag && pos != descr_len)
			{
				idx_tmp = idx;
				if(string[pos] != ' ')
				{
					while(idx_tmp > 0 && descr_buf[idx_tmp-1] != ' ')
						--idx_tmp;

					if(idx_tmp > 0)
					{
						pos -= (idx - idx_tmp);
						idx = idx_tmp;
					}
				}
				else
					pos++;
			}
		}

		descr_buf[idx] = '\0';
		SetAddStr(descr_buf);
		ZeroMemory(descr_buf, sizeof(descr_buf));
		//AddString(descr_buf, color);
	}
}

void CScrollBox::PreSetValForInBox(int Str, int Int, int Dex, int Con, int Cha)
{
	char tmp[100];
	sprintf(tmp, "%d %d %d %d %d", Str, Int, Dex, Con, Cha);
	strcpy(m_sPreVal, tmp);
}

//-------------------------------------------------------------------
// naem : LoadRes
// desc : 로딩은 CutePopUp의 호출시에 로드하며 사용이 끝나면 제거한다 (매순간마다 반복)
//        Title, List, Scroll(up,down) Button, ScrollBar를 생성한다
//-------------------------------------------------------------------
void CScrollBox::LoadRes()
{
	if( !m_bMainActive || !g_pDisplay )
		return;

	RECT ClientRc;
	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &ClientRc);
	m_ScreenWidth = ClientRc.right;
	m_ScreenHeight = ClientRc.bottom;

	m_background.Init("menu/menuback1");
	m_background.Align(6, g_pNk2DFrame->GetClientWidth(), g_pNk2DFrame->GetClientHeight());

	// 타이틀이 존재하면
	if(m_bTitle)
	{
		CreTitleSurface();
	}

	// List 의 개수가 1개 이상이라면
	if(m_iMaxListCnt>0)
	{
		CreListSurface();
	}

	// 스크롤 기능을 사용하는가??
	m_iMaxListCnt - m_iViewCnt > 0 ? m_bScroll = TRUE : m_bScroll = FALSE;

	if(m_bScroll)
	{

		if(m_pArrowUp) SAFE_DELETE(m_pArrowUp);
		m_pArrowUp = new CBasicButton();

		if(m_pArrowDown) SAFE_DELETE(m_pArrowDown);
		m_pArrowDown = new CBasicButton();

		// Arrow Button FileName
		m_pArrowUp->SetFileName("common/scroll_up_t01");
		m_pArrowUp->SetSize( TEXT_SIZE, ONE_LINE_CY );
		m_pArrowUp->SetDisable( false );

		m_pArrowDown->SetFileName("common/scroll_dn_t01");
		m_pArrowDown->SetSize( TEXT_SIZE, ONE_LINE_CY );
		m_pArrowDown->SetDisable( false );

		m_pArrowUp->LoadRes();
		m_pArrowDown->LoadRes();
		/*
				m_pArrowUp->PosX = m_iListBox_X+m_iListBox_Cx - m_pArrowUp->Width+5;//1130 +5했음.
				m_pArrowUp->PosY = m_iListBox_Y;
				m_pArrowDown->PosX = m_iListBox_X+m_iListBox_Cx - m_pArrowUp->Width+5;
				m_pArrowDown->PosY = m_iListBox_Y + (ONE_LINE_CY*(m_iViewCnt-1));
		*/
		int a, b;
		a = m_iMaxListCnt - m_iViewCnt;
		b = m_iViewCnt - (a+2);

		if(b<=0) b = 1;

		g_pDisplay->CreateSurfaceFromBitmap (&m_pBarSur, "interface/common/scroll_thumb.bmp", m_pArrowUp->Width, (BAR_CY*b)); // scroll bar surface
		m_pBarSur->SetColorKey(RGB(0, 0, 0));
//		m_pBarSur->Xpos = m_pArrowUp->PosX;
//		m_pBarSur->Ypos = m_pArrowUp->PosY + m_pArrowUp->Height;
	}


	switch(m_iCurType)
	{
	case TYPE_REDISTRIBUTION:
		m_pBtn1->LoadRes();
		m_pBtn2->LoadRes();
		m_pInBox->SetfontSize(TEXT_SIZE);
		g_pDisplay->CreateSurfaceFromBitmap (&m_pBox, "interface/common/InBox.bmp");
		m_pBox->SetColorKey(RGB(0, 0, 0));
		m_pInBox->SetText(m_sPreVal);

		m_pBtn1->SetPosition( m_background.m_x + 79 , m_background.m_y + 230 );
		m_pBtn2->SetPosition( m_background.m_x + 139 , m_background.m_y + 230 );
		break;

	case TYPE_DELIV_BOX:
		m_pBtn1->LoadRes();
		m_pBtn2->LoadRes();
		m_pBtn1->SetPosition( m_background.m_x + 79 , m_background.m_y + 230 );
		m_pBtn2->SetPosition( m_background.m_x + 139 , m_background.m_y + 230 );
		break;

	case TYPE_GUILD_NOTICE:
	case TYPE_HELP_BOX://1128
		m_pBtn1->LoadRes();
		m_pBtn1->SetPosition( m_background.m_x + 114 , m_background.m_y + 230 );
		break;

	case TYPE_MULTI_PG:
		m_pBtn1->LoadRes();
		m_pBtn3->LoadRes();
		m_pBtn4->LoadRes();
		m_pBtn1->SetPosition( m_background.m_x + 57 , m_background.m_y + 230 );
		m_pBtn3->SetPosition( m_background.m_x + 114 , m_background.m_y + 230 );
		m_pBtn2->SetPosition( m_background.m_x + 171 , m_background.m_y + 230 );
		break;
	}

	m_bMainActive = TRUE;

	SetDrawPosition();
}



//-------------------------------------------------------------------
// name : CreTitleSurface
// desc : Title TextSurface 생성
//-------------------------------------------------------------------
void CScrollBox::CreTitleSurface()
{
	if( !g_pDisplay )
		return;

	SAFE_DELETE(m_pTitleSur);
	m_pTitleSur = new CTextOutBox();

	m_pTitleSur->Init(TEXT_SIZE, NAME_COLOR, m_iListBox_X, m_iListBox_Y);
	if (m_sTitleStr)
		m_pTitleSur->SetString(m_sTitleStr);
}

//-------------------------------------------------------------------
// name : CreListSurface
// desc : List  TextSurface 생성 (m_iMaxListCnt 만큼 생성한다)
//        만약 Link기능이 존제한다면 서피스 생성은 NORMAL, OVER, CLICK 종류별로 생성한다.
//-------------------------------------------------------------------
void CScrollBox::CreListSurface()
{
	if( !g_pDisplay )
		return;

	int i;

	// NorSurface
	if(m_pListSur)
		SAFE_DELETE_ARRAY(m_pListSur);
	m_pListSur  = new CTextOutBox*[m_iMaxListCnt];

	for (i=0; i < m_iMaxListCnt; i++)
	{
		m_pListSur[i] = NULL;
	}

	if(m_bLink)
	{
		// OverSurface
		if(m_pOverSur)
			SAFE_DELETE_ARRAY(m_pOverSur);
		m_pOverSur  = new CTextOutBox*[m_iMaxListCnt];

		// ClickSurface
		if(m_pClickSur)		SAFE_DELETE_ARRAY(m_pClickSur);
		m_pClickSur = new CTextOutBox*[m_iMaxListCnt];
	}


	for(i=0; i<m_iMaxListCnt; i++)
	{

		if (m_pListSur[i])
			SAFE_DELETE(m_pListSur[i]);

		// NorSurface
		m_pListSur[i] = new CTextOutBox();
		m_pListSur[i]->Init(TEXT_SIZE, NAME_COLOR, 0, 0);
		m_pListSur[i]->SetString(m_sListStr[i]);

		if(m_bLink)
		{
			// OverSurface
			m_pOverSur[i] = new CTextOutBox();
			m_pOverSur[i]->Init(TEXT_SIZE, TEXT_OVER_COLOR, 0, 0);
			m_pOverSur[i]->SetString(m_sListStr[i]);
			m_pOverSur[i]->SetVisible(false);

			// ClickSurface
			m_pClickSur[i] = new CTextOutBox();
			m_pClickSur[i]->Init(TEXT_SIZE, TEXT_CLICK_COLOR, 0, 0);
			m_pClickSur[i]->SetString(m_sListStr[i]);
			m_pClickSur[i]->SetVisible(false);
		}
	}
}

void CScrollBox::DeleteRes()
{
	int i;

	m_bMainActive = FALSE;

	if(m_bTitle)
	{
		SAFE_DELETE(m_sTitleStr);
		SAFE_DELETE(m_pTitleSur);
	}
	//m_bTitle = FALSE;

	if(m_iMaxListCnt >0)
	{
		for(i=0; i<m_iMaxListCnt; i++)
		{
			SAFE_DELETE(m_pListSur[i]);
			SAFE_DELETE_ARRAY(m_sListStr[i]); // delete char**

			if(m_bLink)
			{
				SAFE_DELETE(m_pOverSur[i]);
				SAFE_DELETE(m_pClickSur[i]);
			}
		}
		SAFE_DELETE_ARRAY(m_pListSur);
		SAFE_DELETE_ARRAY(m_sListStr); // delete char**
		if(m_bLink)
		{
			SAFE_DELETE_ARRAY(m_pOverSur);
			SAFE_DELETE_ARRAY(m_pClickSur);
		}
	}

	if(m_bScroll)
	{
		SAFE_DELETE(m_pArrowUp);
		SAFE_DELETE(m_pArrowDown);
		SAFE_DELETE(m_pBarSur);
	}

	switch(m_iCurType)
	{
	case TYPE_REDISTRIBUTION:
		SAFE_DELETE(m_pBtn1);
		SAFE_DELETE(m_pBtn2);
		SAFE_DELETE(m_pInBox);
		SAFE_DELETE(m_pBox);
		break;

	case TYPE_DELIV_BOX:
		SAFE_DELETE(m_pBtn1);
		SAFE_DELETE(m_pBtn2);
		break;

	case TYPE_GUILD_NOTICE:
	case TYPE_HELP_BOX://1128
		SAFE_DELETE(m_pBtn1);
		break;

	case TYPE_MULTI_PG:
		SAFE_DELETE(m_pBtn1);
		SAFE_DELETE(m_pBtn2);
		SAFE_DELETE(m_pBtn3);
		break;
	}
}






//-------------------------------------------------------------------
// name : Draw
// desc : 출력한다
//-------------------------------------------------------------------
void CScrollBox::Draw()
{
	if(!m_bMainActive)
		return;

	BOOL IsVisible = FALSE;

	if(	g_pNk2DFrame->IsHelpVisible()			|| g_pNk2DFrame->IsInvenVisible()
			|| g_pNk2DFrame->IsPartyVisible()		|| g_pNk2DFrame->IsInterfaceVisible(SCORE)
			|| g_pNk2DFrame->IsSkillVisible()		|| g_pNk2DFrame->IsInterfaceVisible(MAP)
			|| g_pNk2DFrame->IsPetStatusVisible()	|| g_pNk2DFrame->IsInterfaceVisible(QUEST)
			|| g_pNk2DFrame->IsControlGuild()
			|| g_pNk2DFrame->IsControlGuildManage()
	  )
		IsVisible = TRUE;
	else
		IsVisible = FALSE;

	if( m_IsVisible != IsVisible )
	{
		m_IsVisible = IsVisible;
		SetDrawPosition();
	}

	//RenderOj(m_RenderXpos, m_RenderYpos, m_RenderWidth, m_RenderHeight);

	m_background.Draw();

	switch(m_iCurType)
	{
	case TYPE_REDISTRIBUTION:
// 		86, 230, MENU_ACTION_USERSHOP_SEARCH ); // 검색
// 		m_pControlMenu->AddButton( "Menu/btn_cancel", 226, 230



		m_pBtn1->Draw();
		m_pBtn2->Draw();
		g_pDisplay->Blt(m_pBox->Xpos, m_pBox->Ypos, m_pBox);
		m_pInBox->Draw();
		break;

	case TYPE_MULTI_PG:
		m_pBtn1->Draw();
		m_pBtn3->Draw();
		m_pBtn4->Draw();
		break;

	case TYPE_GUILD_NOTICE:
	case TYPE_HELP_BOX://1128
		m_pBtn1->Draw();
		break;

	case TYPE_DELIV_BOX:
		m_pBtn1->Draw();
		m_pBtn2->Draw();
		break;
	}

	if(m_bTitle)
	{
		Show_Title();
	}

	if(m_iMaxListCnt>0)
	{
		Show_List();
	}

	if(m_bScroll)
	{
		Show_ArrowBtn(); //1130
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_pDisplay->GetHWnd(), &point);

	if (pCMyApp->m_pMouse && IsInside_BackGround(point.x, point.y))
		pCMyApp->m_pMouse->SetMouseType(M_NORMAL);
}

// Draw TitleSurface
void CScrollBox::Show_Title()
{
	if(m_pTitleSur)
	{
		if( m_iCurType == TYPE_GUILD_NOTICE ) // 175, 12
			m_pTitleSur->Draw(m_background.m_x+72, m_background.m_y+12);
		else //태국 폰트가 넘어가서 조금 수정
			m_pTitleSur->Draw(m_background.m_x+72, m_background.m_y+12);
	}
}

// Draw FunLineTextSurface
void CScrollBox::Show_List()
{
	for(int i=m_iStartViewCnt; i<m_iEndViewCnt; i++)
	{
		if(m_pListSur[i])
		{
			m_pListSur[i]->m_StringPosX = m_background.m_x + 27;
			m_pListSur[i]->m_StringPosY = m_background.m_y + 67 + (TEXT_SIZE+TEXT_CY_GAP)*(i-m_iStartViewCnt);
			m_pListSur[i]->Draw();

			if(m_bLink)
			{
				m_pOverSur[i]->m_StringPosX = m_background.m_x + 27;
				m_pOverSur[i]->m_StringPosY = m_background.m_y + 67 + ((TEXT_SIZE+TEXT_CY_GAP)*(i-m_iStartViewCnt));
				m_pOverSur[i]->Draw();
				m_pClickSur[i]->m_StringPosX = m_background.m_x + 27;
				m_pClickSur[i]->m_StringPosY = m_background.m_y + 67 + ((TEXT_SIZE+TEXT_CY_GAP)*(i-m_iStartViewCnt));
				m_pClickSur[i]->Draw();
			}
		}
	}
}

// Draw Scroll Button
void CScrollBox::Show_ArrowBtn()
{
	if(m_bScroll)
	{
		m_pArrowUp->Draw();
		m_pArrowDown->Draw();

		g_pDisplay->Blt(m_pBarSur->Xpos, m_pBarSur->Ypos, m_pBarSur);
	}
}



//------------------------------------------------------------
//	Name : ReadInfoFromTxt ()
//	Desc : 파일로부터 스트링을 얻어와서
//         1. List 의 카운트를 얻는다
//         2. Title 가 존재한다면 Title를 얻는다
//		   3. List 카운트만큼 List String를 얻틑다
//------------------------------------------------------------


BOOL CScrollBox::ReadInfoFromTxt(char *filename)
{
	PFILE *fp = NULL;
	char buf[256];

	// 서버 이름  , 주소

	// get max line..
	m_iMaxListCnt = GetFileLineCnt(filename);


	if(m_iMaxListCnt < 1)
		return false;

	char *sentence=NULL;
	int i;

	if((fp = pfopen(filename, "rt")) == NULL)
		return false;

	if(m_bTitle)
	{
		m_iMaxListCnt = m_iMaxListCnt-1;
		sentence  = PGetSentence(buf, sizeof(buf), fp);
		if(!SetTitleStr(sentence)) return FALSE;
	}

	// List String
	if(m_iMaxListCnt>0)
	{
		m_sListStr = new char*[m_iMaxListCnt];
		for(int i=0; i<m_iMaxListCnt; i++)
		{
			m_sListStr[i] = new char[100];
		}
	}


	// Set List String..
	for (i = 0; i < m_iMaxListCnt; i++)
	{
		// 첫번째 읽은줄을 sentence에 넣는다
		sentence = PGetSentence(buf, sizeof(buf), fp);
		if(!SetAddStr(sentence)) return FALSE;

	}
	pfclose(fp);

	return true;
}




int CScrollBox::GetFileLineCnt(char* fname)
{
	char buf[100];
	int line = 0;

	PFILE* fp = NULL;

	fp = pfopen(fname, "rt");


	if(fp==NULL)
		return -1;

	while ((pfgets(buf, sizeof(buf), fp)) != NULL)
	{
		if(buf[0] == '#') continue;
		line++;
	}
	pfclose(fp);
	return line;
}

char *CScrollBox::GetSentence(char *dest, int n, FILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = fgets(dest, n, fp)) == NULL)
			return tmp;

		//while ((*tmp == 0x20 || (*tmp >= 0x09 && *tmp <= 0x0D)) && *tmp != 0)
		//while (iswspace(*tmp) && *tmp != 0)
		//	tmp++;
		//while (iswspace(*tmp) && *tmp != 0)
		//	tmp++;
	}
	while (*tmp == '#' || *tmp == 0);


	int len = strlen(tmp);
	if(tmp[len-1] == '\n')
		tmp[len-1] = '\0';

	return tmp;
}

char *CScrollBox::PGetSentence(char *dest, int n, PFILE *fp)
{
	char *tmp = NULL;

	if (fp == NULL || dest == NULL)
		return NULL;

	dest[0] = NULL;

	do
	{
		if ( (tmp = pfgets(dest, n, fp)) == NULL)
			return tmp;

		//while ((*tmp == 0x20 || (*tmp >= 0x09 && *tmp <= 0x0D)) && *tmp != 0)
		//while (iswspace(*tmp) && *tmp != 0)
		//	tmp++;
		//while (iswspace(*tmp) && *tmp != 0)
		//	tmp++;
	}
	while (*tmp == '#' || *tmp == 0);


	int len = strlen(tmp);
	if(tmp[len-1] == '\n')
		tmp[len-1] = '\0';

	return tmp;
}



//------------------------------------------------------------
//	Name : SetBarPosToGo ()
//	Desc :	Scroll Bar의 위치를 계산한다.
//			Scroll Bar를 이동시킨다. (마우스 Event에 의하여)
//------------------------------------------------------------

void CScrollBox :: SetBarPosToGo (int y)
{
	if (m_iMaxListCnt > m_iViewCnt)
	{
		if (y < m_pArrowUp->PosY+m_pArrowUp->Height+m_pBarSur->GetHeight()/2)
			GoHome ();
		else if (y > m_pArrowDown->PosY-m_pBarSur->GetHeight()/2)
			GoEnd ();
		else
		{
			int HeightCanBeScrolled = m_background.m_y - (2*m_pArrowUp->Height);

			// 한 line당의 pixel수
			int interval = HeightCanBeScrolled / (m_iMaxListCnt-m_iViewCnt);



			m_iStartViewCnt = (y-m_pArrowUp->PosY-m_pArrowUp->Height) / interval;
			if( m_iStartViewCnt > 0 )
				m_iStartViewCnt -= 1;
			m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;

			CalculateBarPos ();
		}
	}
}

//------------------------------------------------------------
//	Name : CalculateBarPos ()
//	Desc :
//------------------------------------------------------------
//
void CScrollBox::CalculateBarPos ()
{
	if (m_iMaxListCnt > m_iViewCnt)
	{
		int n1 = m_iMaxListCnt - m_iViewCnt;
		int n2 = m_iStartViewCnt;

		if (n1 != n2)
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함하지 않음)
			int HeightCanBeScrolled = m_background.m_y - 2*m_pArrowUp->Height - m_pBarSur->GetHeight();

			// 움직여지는 양..
			int	interval = HeightCanBeScrolled * n2 / n1;
			m_pBarSur->Ypos = m_pArrowUp->PosY+m_pArrowUp->Height+interval;
		}
		else
		{
			// 맨 밑으로 내려간 경우이다.
			// 이경우는 바닥에 정확히 붙이기 위한 계산을 한다.
			m_pBarSur->Ypos = m_pArrowDown->PosY - m_pBarSur->GetHeight();
		}
	}
}


//------------------------------------------------------------
//	Name : GoHome () / GoEnd()
//	Desc :	처음과 끝으로..
//------------------------------------------------------------
//
void CScrollBox::GoHome ()
{
	m_iStartViewCnt = 0;
	m_pBarSur->Ypos = m_pArrowUp->PosY+m_pArrowUp->Height;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
}


void CScrollBox::GoEnd ()
{
	if (m_iMaxListCnt > m_iViewCnt)
	{
		m_iStartViewCnt = m_iMaxListCnt-m_iViewCnt;
		m_pBarSur->Ypos = m_pArrowDown->PosY-m_pBarSur->GetHeight();
		m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
	}
	else
	{
		m_iStartViewCnt = 0;
		m_pBarSur->Ypos = m_pArrowUp->PosY+m_pArrowUp->Height;
		m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
	}
}


//-------------------------------------------------------------------
// name : List_UpScroll
// desc : 스크롤버튼(up)이 눌린경우 보여주는 list의 변화..
//-------------------------------------------------------------------
void CScrollBox::List_UpScroll()
{
	if(m_iEndViewCnt >= m_iMaxListCnt) return;
	m_iStartViewCnt++;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
	CalculateBarPos ();
}

//-------------------------------------------------------------------
// name : List_UpScroll
// desc : 스크롤버튼(up)이 눌린경우 보여주는 list의 변화..
//-------------------------------------------------------------------
void CScrollBox::List_DownScroll()
{
	if(m_iStartViewCnt <= 0) return;
	m_iStartViewCnt--;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
	CalculateBarPos ();
}


//-------------------------------------------------------------------
// name : IsInside_Text
// desc : Check Mouse Action!!  In Text Area
//-------------------------------------------------------------------
BOOL CScrollBox::IsInside_Text(int index, int x, int y)
{
	if (x >= m_pListSur[index]->m_StringPosX&& x <= m_pListSur[index]->m_StringPosX + m_pListSur[index]->GetBoxWidth() &&
			y >= m_pListSur[index]->m_StringPosY && y < m_pListSur[index]->m_StringPosY + m_pListSur[index]->GetBoxHeight() )
	{
		return TRUE;
	}
	return FALSE;
}



int CScrollBox::IsInside_ScrollArea(int x, int y)
{
	if (x >= m_pArrowUp->PosX && x <= m_pArrowUp->PosX + m_pArrowUp->Width &&
			y >= m_pArrowUp->PosY+m_pArrowUp->Height && y < m_pArrowDown->PosY)
	{

		if (x >= m_pBarSur->Xpos && x <= m_pBarSur->Xpos + m_pBarSur->GetWidth() &&
				y >= m_pBarSur->Ypos && y < m_pBarSur->Ypos+m_pBarSur->GetHeight())
		{
			return SCROLLBAR_POLL;
		}
		return SCROLLBAR_FLAT;
	}
	return false;
}




//-------------------------------------------------------------------
// name : MsgProc
// desc : 리턴되는 값은
//        -1 = 팝업창 영역 내에서의 이벤트
//         0 = 팝업창 영역 외에서의 이벤트
//         1 = "ok"		버튼 클릭
//         2 = "cancle" 버튼 클릭
//         3 = "back"	버튼 클릭
//         4 = "next"	버튼 클릭
//-------------------------------------------------------------------
LRESULT CScrollBox::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int i;
	int ret;

	x = LOWORD (lParam);
	y = HIWORD (lParam);

	switch ( msg )
	{

	case WM_KEYDOWN:
		if(wParam == VK_BACK) // for...tw/chi     backspace
		{
			switch(m_iCurType)
			{
			case TYPE_REDISTRIBUTION:
				m_pInBox->MsgProc( hWnd, msg, wParam, lParam );
				break;
			}
			break;
		}
		break;

	case WM_CHAR:
		switch(m_iCurType)
		{
		case TYPE_REDISTRIBUTION:
			m_pInBox->MsgProc( hWnd, msg, wParam, lParam );
			break;
		}
		break;


	case WM_LBUTTONDOWN:
		if( !IsInside_BackGround(x,y) )
		{
			return 0;
		}
		if(m_iMaxListCnt<0) break;


		switch(m_iCurType)
		{
		case TYPE_REDISTRIBUTION:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			m_pInBox->MsgProc( hWnd, msg, wParam, lParam );
			break;

		case TYPE_DELIV_BOX:
			if(m_pBtn1->GetState() != BTN_DISABLE)
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn2->GetState() != BTN_DISABLE)
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			break;
		case TYPE_GUILD_NOTICE:
		case TYPE_HELP_BOX://1128
			if(m_pBtn1->GetState() != BTN_DISABLE)
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			break;

		case TYPE_MULTI_PG:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn3->MsgProc(hWnd, msg, wParam, lParam);
			break;
		}

		if(m_bScroll)
		{
			m_pArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pArrowDown->MsgProc(hWnd, msg, wParam, lParam);

			ret = IsInside_ScrollArea(x, y);

			if (ret == SCROLLBAR_POLL)
				m_eState = SCROLLBAR_POLL;

			else if(ret ==SCROLLBAR_FLAT)
			{
				m_eState = SCROLLBAR_FLAT;
				SetBarPosToGo (y);
			}
			else
				m_eState = NONE;
		}

		if(m_bLink)
		{
			for(i=0; i<m_iMaxListCnt; i++)
			{
				if(IsInside_Text(i, x, y))
				{
					m_pListSur[i]->SetVisible(FALSE);
					m_pOverSur[i]->SetVisible(FALSE);
					m_pClickSur[i]->SetVisible(TRUE);
					m_bDown = TRUE;
				}
				else
				{
					m_pListSur[i]->SetVisible(TRUE);
					m_pOverSur[i]->SetVisible(FALSE);
					m_pClickSur[i]->SetVisible(FALSE);
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		if(m_iMaxListCnt<0) break;

		switch(m_iCurType)
		{

		case TYPE_REDISTRIBUTION:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn1->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn1->m_iBtnFlg = BTN_NORMAL;
				return 1;
			}
			if(m_pBtn2->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn2->m_iBtnFlg = BTN_NORMAL;
				return 2;
			}
			break;

		case TYPE_GUILD_NOTICE:
		case TYPE_HELP_BOX://1128
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn1->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn1->m_iBtnFlg = BTN_NORMAL;
				return 2;
			}
			break;

		case TYPE_DELIV_BOX:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn1->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn1->m_iBtnFlg = BTN_NORMAL;
				return 1;
			}
			if(m_pBtn2->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn2->m_iBtnFlg = BTN_NORMAL;
				return 2;
			}
			break;

		case TYPE_MULTI_PG:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn3->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn4->MsgProc(hWnd, msg, wParam, lParam);

			// GO!!  BACK PAGE
			if(m_pBtn3->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn3->m_iBtnFlg = BTN_NORMAL;
				return 3;
			}
			// GO!!  NEXT PAGE
			if(m_pBtn4->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn4->m_iBtnFlg = BTN_NORMAL;
				return 4;
			}
			// ok..
			if(m_pBtn1->m_iBtnFlg == BTN_ACTION)
			{
				m_pBtn1->m_iBtnFlg = BTN_NORMAL;
				return 2;
			}
		}

		if(m_bScroll)
		{
			m_pArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pArrowDown->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pArrowUp->GetState() == BTN_ACTION)
			{
				List_DownScroll();
			}
			if(m_pArrowDown->GetState() == BTN_ACTION)
			{
				List_UpScroll();
			}
			m_eState = NONE;
		}

		if(m_bLink)
		{
			for(i=0; i<m_iMaxListCnt; i++)
			{
				if(IsInside_Text(i, x, y)) // Type Btn Over
				{
					m_pListSur[i]->SetVisible(FALSE);
					m_pOverSur[i]->SetVisible(TRUE);
					m_pClickSur[i]->SetVisible(FALSE);
					m_bDown = FALSE;
				}
				else if(! IsInside_Text(i, x, y)) // Type Btn Nor
				{
					m_pListSur[i]->SetVisible(TRUE);
					m_pOverSur[i]->SetVisible(FALSE);
					m_pClickSur[i]->SetVisible(FALSE);
					m_bDown = FALSE;
				}
			}
		}
		break;

	case WM_MOUSEMOVE:
		if(m_iMaxListCnt<0) break;

		switch(m_iCurType)
		{
		case TYPE_REDISTRIBUTION:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			break;

		case TYPE_DELIV_BOX:
			if(m_pBtn1->GetState() != BTN_DISABLE)
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn2->GetState() != BTN_DISABLE)
				m_pBtn2->MsgProc(hWnd, msg, wParam, lParam);
			break;

		case TYPE_GUILD_NOTICE:
		case TYPE_HELP_BOX:
			if(m_pBtn1->GetState() != BTN_DISABLE)
				m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			break;

		case TYPE_MULTI_PG:
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn3->MsgProc(hWnd, msg, wParam, lParam);
			m_pBtn4->MsgProc(hWnd, msg, wParam, lParam);
			break;
		}

		if(m_bScroll)
		{
			m_pArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pArrowDown->MsgProc(hWnd, msg, wParam, lParam);

			if(m_eState == SCROLLBAR_POLL)
			{
				SetBarPosToGo (y);
			}
		}

		if(m_bLink)
		{
			for(i=0; i<m_iMaxListCnt; i++)
			{
				if ( (IsInside_Text(i, x, y) && (!m_bDown)))
				{
					m_pListSur[i]->SetVisible(FALSE);
					m_pOverSur[i]->SetVisible(TRUE);
					m_pClickSur[i]->SetVisible(FALSE);
				}

				else if ( (IsInside_Text(i, x, y))&& (m_bDown))
				{
					m_pListSur[i]->SetVisible(FALSE);
					m_pOverSur[i]->SetVisible(FALSE);
					m_pClickSur[i]->SetVisible(TRUE);
				}
				else
				{
					m_pListSur[i]->SetVisible(TRUE);
					m_pOverSur[i]->SetVisible(FALSE);
					m_pClickSur[i]->SetVisible(FALSE);
				}
			}
		}
		break;

	default:
		break;
	}

	if(IsInside_BackGround(x, y))
		return -1;
	else
		return 0;
}





//-------------------------------------------------------------------
// name : RenderOj
// desc : 3d 기능을 사용하여 팝업창의 반투명한 바탕(BACK)를 그린다.
//-------------------------------------------------------------------
void CScrollBox::RenderOj(int xpos, int ypos, int cx, int cy)
{
	//xpos = X1, ypos = Y1; // TextSur의 위치를 알파 배경 위치와 같게 한다
	LPDIRECTDRAWSURFACE7 lpTexture = NULL;

	D3DMATRIX matWorld, matView, matProjOld, matProjNew;
	//D3DVECTOR vEyePt, vLookatPt, vUpVec;
	D3DVIEWPORT7 vpOld, vpNew;

	DWORD	dwColor;

	// 현재의 뷰포트를 저장하고 새롭게 뷰포트를 설정한다.
	GET_D3DDEVICE()->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.dwWidth  = m_ScreenWidth;
	vpNew.dwHeight = m_ScreenHeight;
	GET_D3DDEVICE()->SetViewport(&vpNew);

	//// 프로젝션 매트릭스 수정.
	float fAspect = (float) ((double) vpNew.dwHeight / (double) vpNew.dwWidth);
	GET_D3DDEVICE()->GetTransform(D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	D3DUtil_SetProjectionMatrix(matProjNew, 1.0f, fAspect, 1.0f, 2000.0f );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjNew);

	TLVec[0] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos,      0), 1, 0x00ffffff, 0, 0, 0);
	TLVec[1] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos,     0), 1, 0x00ffffff, 0, 1, 0);
	TLVec[2] = D3DTLVERTEX(D3DVECTOR(xpos,    ypos+cy,  0), 1, 0x00ffffff, 0, 0, 1);
	TLVec[3] = D3DTLVERTEX(D3DVECTOR(xpos+cx, ypos+cy,  0), 1, 0x00ffffff, 0, 1, 1);

	// 라이팅
	GET_D3DDEVICE()->GetRenderState(D3DRENDERSTATE_AMBIENT, &dwColor );
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, 0x00FFFFFF);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);

	//----------------그리기 시작 / 끝 --------------
	if( SUCCEEDED( GET_D3DDEVICE()->BeginScene() ) )
	{
		//-----------------------TLVERTEX 를 이용한 맵핑후 출력(화면의 실제 좌표를 입력한다)

		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , TRUE);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE   , FALSE);

		if( m_iCurType == TYPE_GUILD_NOTICE )
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_GUILD_NOTICE ]);
		else
			lpTexture = g_pCapsyongTexture->GetSurfaceByIdx(g_EffectTextr[ EFF_CUTE_BACK ]);

		GET_D3DDEVICE()->SetTexture(0, lpTexture);
		//GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
		GET_D3DDEVICE()->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, TLVec, 4, 0);
		//GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
		//GET_D3DDEVICE()->SetTexture(0, NULL);
		GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE   , FALSE);

		GET_D3DDEVICE()->EndScene();
	}

	// 원상복귀.
	GET_D3DDEVICE()->SetViewport(&vpOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_AMBIENT, dwColor );
	GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProjOld);
	GET_D3DDEVICE()->SetRenderState(D3DRENDERSTATE_LIGHTING, TRUE);
}



//-------------------------------------------------------------------
// name : IsInside_BackGround
// desc : Check Mouse Action!!  In PopUp BackGround
//-------------------------------------------------------------------
BOOL CScrollBox::IsInside_BackGround(int x, int y)
{
	return m_background.IsIn(x, y);
}

void CScrollBox::SetDrawPosition()
{
	if(m_bScroll)
	{
		m_pArrowUp->SetPosition(m_background.m_x + 327 , m_background.m_y + 67 );
		m_pArrowUp->SetPosition(m_background.m_x + 327 , m_background.m_y + 67 + 145 - 18 );
		m_pBarSur->Xpos = m_pArrowUp->PosX;
		m_pBarSur->Ypos = m_pArrowUp->PosY + m_pArrowUp->Height;
	}
	switch(m_iCurType)
	{
	case TYPE_REDISTRIBUTION:
		m_pInBox->SetPos(m_background.m_x+82, m_background.m_y+190);
		m_pBox->Xpos = m_background.m_x+77;
		m_pBox->Ypos = m_background.m_y+190;
		break;
	}
}




