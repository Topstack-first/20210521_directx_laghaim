#include "stdafx.h"

#include "QuestData.h"
#include "headers.h"
#include "sheet.h"
#include "ControlInven.h"
#include "Nk2DFrame.h"
#include "tcpipcon.h"
#include "textoutbox.h"
#include "ItemProto.h"
#include "textbox.h"
#include "main.h"
#include "resource.h"
#include "msgpopup.h"
#include "LoadEffectSound.h"
#include "NkMob.h"
#include "g_stringmanager.h"

#include "LHExcelReader.h"

const char * QUEST_DATA_FILENAME = "data/language/quest_table.txt";
const char * QUEST_DATA_FILENAME2= "data/language/quest_content_table.txt";

extern int UTF8ToUnicode(wchar_t* wcstr, const char * utfstr, size_t count);
extern int UniToMulti(char * mbstr, const wchar_t*wcstr , size_t count);


extern char g_strFontName[];





extern BOOL g_bFixedWidthFont;
extern DWORD			g_dwClientCountry;

MOBNAMESTRUCT *MobNameList; //퀘스트에 쓰일 몹 이름 담아놓기.

#define _640			33
#define _800			44
#define _1024			55

#define _MAIN_XPOS_640   435
#define _MAIN_XPOS_800   596
#define _MAIN_XPOS_1024  822

#define Q_ENC_INT_SEED		(1819)
#define Q_ENC_CHAR_SEED		(51)
#define Q_ENC_INT_VAL			(8756)
#define Q_ENC_CHAR_VAL		(117)
#define Q_ENC_DUMMY_COUNT		(13)

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


//#define _QUEST_ALIGN_ // by evilkiki 퀘스트정보 출력위치 조정...일단 일본만 제외(2011.02.22)

CQuestData::CQuestData()
	: m_fStretchX(0)
	, m_fStretchY(0)
	, m_FontWidth(0)
	, m_FontHeight(0)
	, m_CurClientMode(0)
	, m_TextColor(0)
	, m_TextBackColor(0)
	, m_ScreenWidth(0)
	, m_ScreenHeight(0)
	, m_Type(0)
	, m_Sub(0)
	, m_EndType(0)
	, m_MinLevel(0)
	, m_MaxLevel(0)
	, m_Zone(0)
	, m_Laim(0)
	, m_Exp(0)
	, m_Data0(0)
	, m_Data1(0)
	, m_Data2(0)
	, m_Data3(0)
	, m_Data4(0)
	, m_Data5(0)
	, m_Data6(0)
	, m_Data7(0)
	, m_Data8(0)
	, m_Data9(0)
	, m_bMainActive(FALSE)
	, m_bDown(FALSE)
	, m_iCurType(0)
	, m_RenderXpos(0)
	, m_RenderYpos(0)
	, m_RenderWidth(0)
	, m_RenderHeight(0)
	, m_iListBox_X(0)
	, m_iListBox_Y(0)
	, m_iListBox_Cx(0)
	, m_iListBox_Cy(0)
	, m_iStartViewCnt(0)
	, m_iEndViewCnt(0)
	, m_iViewCnt(0)
	, m_eState(NONE)
{
	memset(m_Tmp, 0, sizeof(m_Tmp));
	memset(m_sPreVal, 0, sizeof(m_sPreVal));
	memset(MobName1, 0, sizeof(MobName1));
	memset(MobName2, 0, sizeof(MobName2));
	memset(MobName3, 0, sizeof(MobName3));
	memset(MobName4, 0, sizeof(MobName4));
	memset(MobName5, 0, sizeof(MobName5));

	m_iMaxListCnt = 0;
	m_nQuestTotalPoint = 0;
	m_pToy = NULL;
	m_Index = -1;

	m_pSheet   = NULL;
	m_pBackSur = NULL;

	for(int i=0; i<QuestData_EndOfEnum; i++)
		m_pTextSur[i] = NULL;

	m_Font	   = NULL;
	m_pNkCha = NULL;
	m_hWnd		  = NULL;
	m_pListSur    = NULL;
	m_pBonusText = NULL;
	m_sListStr    = NULL;
	m_pArrowUp    = NULL;
	m_pArrowDown  = NULL;
	m_pBarSur    = NULL;
	m_iCurListNum = 0;
	m_bScroll = FALSE;
	m_bReload = FALSE;
	m_nNpcNameNum = -1;
	MobNameList = NULL;
	m_bNoQuest =TRUE;
	m_pAdventure = NULL;
	m_pIsumiron = NULL;
	ReadMobNameFile("data/Npc/npc_name.dta");
	ZeroMemory(m_nMobNameLength, sizeof(m_nMobNameLength));
}

CQuestData::~CQuestData()
{
	Clear();
	DeleteRes();
	if (m_nNpcNameNum)
	{
		for(int i=0; i<m_nNpcNameNum; i++)
		{
			if(MobNameList[i].mob_name)
			{
				SAFE_DELETE(MobNameList[i].mob_name);
			}
		}
	}

	if (MobNameList)
		delete [] MobNameList;
	MobNameList = NULL;
}

int CQuestData::Read(const char *fileName, int q_num)
{
	Clear();

	LHExcelReader er;

	if( false == er.Open( QUEST_DATA_FILENAME ) )
		return 0;


	char multibyte[1024];
	wchar_t unicode[1024];

	for( int row=0 ; row<er.GetRowSize() ; row++ )
	{
		int col = 0;
		int index = er.GetInt(row, col++);

		if( index != q_num )
			continue;

		const char * text = er.GetData(row, col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_Name = multibyte;

		m_Type = er.GetInt(row, col++);

		// a_enable
		col++;
		// a_sub
		m_Sub = er.GetInt(row, col++); // a_sub
		// a_get_type
		col++;
		// a_count
		col++;
		// a_min_level
		m_MinLevel = er.GetInt(row, col++);
		// a_max_level
		m_MaxLevel = er.GetInt(row, col++);
		// a_start_type
		col++;
		// a_end_type
		m_EndType = er.GetInt(row, col++);
		// a_start_data
		col++;
		// a_end_data
		col++;
		// a_link
		col++;
		// a_data0
		m_Data0 = er.GetInt(row, col++);
		// a_data1
		m_Data1 = er.GetInt(row, col++);
		// a_data2
		m_Data2 = er.GetInt(row, col++);
		// a_data3
		m_Data3 = er.GetInt(row, col++);
		// a_data4
		m_Data4 = er.GetInt(row, col++);
		// a_data5
		m_Data5 = er.GetInt(row, col++);
		// a_data6
		m_Data6 = er.GetInt(row, col++);
		// a_data7
		m_Data7 = er.GetInt(row, col++);
		// a_data8
		m_Data8 = er.GetInt(row, col++);
		// a_data9
		m_Data9 = er.GetInt(row, col++);
		// a_zone
		m_Zone = er.GetInt(row, col++);
		// a_prize
		col++;
		// a_laim
		m_Laim = er.GetInt(row, col++);
		// a_exp
		m_Exp = er.GetInt(row, col++);
		// a_item0
		col++;
		// a_item1
		col++;
		// a_item2
		col++;
		// a_item3
		col++;
		// a_item4
		col++;
		// a_item5
		col++;
		// a_item6
		col++;
		// a_item7
		col++;
		// a_item8
		col++;
		// a_string0
		text = er.GetData(row,col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_String0 = multibyte;
		// a_string1
		text = er.GetData(row,col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_String1 = multibyte;
		// a_string2
		text = er.GetData(row,col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_String2 = multibyte;
		// a_string3
		text = er.GetData(row,col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_String3 = multibyte;
		// a_string4
		text = er.GetData(row,col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_String4 = multibyte;
		// a_string5
		text = er.GetData(row,col++);
		UTF8ToUnicode( unicode , text , 1024 );
		UniToMulti( multibyte , unicode , 1024 );
		m_String5 = multibyte;

		{
			LHExcelReader er2;
			if( false == er2.Open( QUEST_DATA_FILENAME2 ) )
				return 0;


			for( int row2=0 ; row2<er2.GetRowSize() ; row2++ )
			{
				int col2 = 0;
				int ref_index = er2.GetInt(row2, col2++);
				if( ref_index == index )
				{
					text = er2.GetData(row2,col2++);
					UTF8ToUnicode( unicode , text , 1024 );
					UniToMulti( multibyte , unicode , 1024 );
					m_Content = multibyte;
					break;
				}
			}
		}

		m_bReload= TRUE;

		if( m_Data0 )
			strcpy(MobName1,(char*)GET_NPC_NAME_VNUM(m_Data0));
		if( m_Data1 )
			strcpy(MobName2,(char*)GET_NPC_NAME_VNUM(m_Data1));
		if( m_Data2 )
			strcpy(MobName3,(char*)GET_NPC_NAME_VNUM(m_Data2));
		if( m_Data3 )
			strcpy(MobName4,(char*)GET_NPC_NAME_VNUM(m_Data3));
		if( m_Data4 )
			strcpy(MobName5,(char*)GET_NPC_NAME_VNUM(m_Data4));

// 		for(int i=0;i<m_nNpcNameNum;i++)
// 		{
// 			if(MobNameList[i].nVnum == m_Data0)
// 				strcpy(MobName1,(char*)GET_NPC_NAME_VNUM(MobNameList[i].nVnum));
// 			else if(MobNameList[i].nVnum == m_Data1)
// 				strcpy(MobName2,(char*)GET_NPC_NAME_VNUM(MobNameList[i].nVnum));
// 			else if(MobNameList[i].nVnum == m_Data2)
// 				strcpy(MobName3,(char*)GET_NPC_NAME_VNUM(MobNameList[i].nVnum));
// 			else if(MobNameList[i].nVnum == m_Data3)
// 				strcpy(MobName4,(char*)GET_NPC_NAME_VNUM(MobNameList[i].nVnum));
// 			else if(MobNameList[i].nVnum == m_Data4)
// 				strcpy(MobName5,(char*)GET_NPC_NAME_VNUM(MobNameList[i].nVnum));
// 		}

		m_bNoQuest =FALSE;
		QuestMessage();

// 		테스트 코드
// 		{
// 			if( m_Data0 )	g_pNk2DFrame->AddChatStr( (char*)GET_NPC_NAME_VNUM(m_Data0), -1 );
// 			if( m_Data1 )	g_pNk2DFrame->AddChatStr( (char*)GET_NPC_NAME_VNUM(m_Data1), 0 );
// 			if( m_Data2 )	g_pNk2DFrame->AddChatStr( (char*)GET_NPC_NAME_VNUM(m_Data2), -1 );
// 			if( m_Data3 )	g_pNk2DFrame->AddChatStr( (char*)GET_NPC_NAME_VNUM(m_Data3), 0 );
// 			if( m_Data4 )	g_pNk2DFrame->AddChatStr( (char*)GET_NPC_NAME_VNUM(m_Data4), -1 );
// 		}

		return 1;
	}

	return 0;
}


void CQuestData::Clear()
{
	m_Index = -1;

	//  [9/24/2008 parkmj] 불칸퀘스트 - 받아도 수행중인게 없다고 나오는 버그
	int x=0, y=0;
	x = _MAIN_XPOS_800-50, y= 300+44+15;

	PutString(3,  "" , x,y);
	//////////////////////////////////////////////////////////////////////////


}

void CQuestData::ReadInt(int *i_data, PFILE *fp, int &int_seed)
{
	if( !fp )
		return;

	pfread(i_data, sizeof(int), 1, fp);
	int seed_temp = *i_data;
	*i_data = *i_data - int_seed - Q_ENC_INT_VAL;
	int_seed = seed_temp;
}


void CQuestData::ReadString(char *str, PFILE *fp, int &int_seed, char &char_seed)
{
	if( str== NULL || !fp)
		return;

	int len;

	pfread(&len, sizeof(int), 1, fp);
	// 길이 암호 풀기

	int seed_temp = len;
	len = len - int_seed - Q_ENC_INT_VAL;
	int_seed = seed_temp;

	if (len > 255)
	{
		*str = '\0';
		return;
	}
	pfread(str, len, 1, fp);
	str[len] = '\0';
	// 문자열 암호 풀기

	for (int i = 0; i < len; i++)
	{
		int char_seed_temp = str[i];
		str[i] = str[i] - char_seed - Q_ENC_CHAR_VAL;
		char_seed = char_seed_temp;
	}
}

void CQuestData::ReadStringDup(char* &str, PFILE *fp, int &int_seed, char &char_seed)
{
	if( &str == NULL || !fp )
		return;

	int len;
	char *str_temp;

	pfread(&len, sizeof(int), 1, fp);
	// 길이 암호 풀기

	int seed_temp = len;
	len = len - int_seed - Q_ENC_INT_VAL;
	int_seed = seed_temp;

	str_temp = new char[len+1];
	pfread(str_temp, len, 1, fp);
	str_temp[len] = '\0';
	str = str_temp;
	// 문자열 암호 풀기

	for (int i = 0; i < len; i++)
	{
		int char_seed_temp = str[i];
		str[i] = str[i] - char_seed - Q_ENC_CHAR_VAL;
		char_seed = char_seed_temp;
	}
	str_temp = NULL;
}

/////
//--------------------------------------------------------------------
//		INITIALIZE
//--------------------------------------------------------------------
void CQuestData::Init(int FontSize)
{
	HDC hDC = GetDC(NULL);
	int ptHeight = -MulDiv(FontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	SetFont(FontSize/2, FontSize, g_strFontName);

	SIZE	sizeFont;
	getFontSize(&sizeFont, m_Font, "A", 1);

	m_FontWidth  = sizeFont.cx;
	m_FontHeight = sizeFont.cy;
}




//--------------------------------------------------------------------
//		LOAD RESOURCE
//--------------------------------------------------------------------
void CQuestData::LoadRes()
{
	/////////////////////////////
	if( !g_pDisplay )
		return;

	m_hWnd =  g_pDisplay->GetHWnd();
	GetClientRect(m_hWnd, &m_ClientRc);

	m_VerifyBtn.Init("common/btn_close_01"); //x버튼
	m_VerifyBtn.SetDisable(false);
	m_VerifyBtn.LoadRes();

	m_CheckBtn.Init("common/btn_ok"); //확인
	m_CheckBtn.SetDisable(false);
	m_CheckBtn.LoadRes();

	m_GiveupBtn.SetFileName("quest/btn_g");	//포기
	m_GiveupBtn.SetDisable(false);
	m_GiveupBtn.LoadRes();

	g_pDisplay->CreateSurfaceFromBitmap(&m_pAdventure, "interface/quest/QUEST_T_1.bmp");
	g_pDisplay->CreateSurfaceFromBitmap(&m_pIsumiron, "interface/quest/QUEST_T_2.bmp");
	g_pDisplay->CreateSurfaceFromBitmap( &m_pToy, "interface/quest/QUEST_T_3.bmp" );
	//===================================================================================================
	//////////////////////////////////
	Init(0, 0);

	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pBackSur, "interface/quest/quest.bmp");
		m_pBackSur->SetColorKey(RGB(0, 0, 0));
		m_pBackSur->Xpos = g_pNk2DFrame->GetClientWidth() - m_pBackSur->GetWidth();
		m_pBackSur->Ypos = 0;

		m_pToy->Xpos = m_pAdventure->Xpos = m_pIsumiron->Xpos = m_pBackSur->Xpos + ((m_pBackSur->GetWidth() - m_pToy->GetWidth())>>1);
		m_pToy->Ypos = m_pAdventure->Ypos = m_pIsumiron->Ypos = 68;

		m_VerifyBtn.SetPosition( m_pBackSur->Xpos+272 , m_pBackSur->Ypos+7 );



		Init(9);

		m_iListBox_X    = m_pBackSur->Xpos+12;
		m_iListBox_Y    = m_pBackSur->Ypos+134;
		m_iListBox_Cx   = 272;
		m_iListBox_Cy   = 149;
	}

	if (g_pNk2DFrame->IsInterfaceVisible(QUEST))
	{
		m_bReload = TRUE;
		QuestMessage();
	}
}


//--------------------------------------------------------------------
//		DELETE
//--------------------------------------------------------------------
void CQuestData::DeleteRes()
{
	CloseRes();

	for(int i=0; i<QuestData_EndOfEnum; i++)
		SAFE_DELETE(m_pTextSur[i]);

	SAFE_DELETE(m_pBonusText);
	SAFE_DELETE(m_pBackSur);
	SAFE_DELETE(m_pAdventure);
	SAFE_DELETE(m_pIsumiron);
	//===================================================================================================
	// nate 2004 - 4 : quest
	SAFE_DELETE( m_pToy );
	//===================================================================================================
	m_GiveupBtn.DeleteRes();
	m_VerifyBtn.DeleteRes();
	m_CheckBtn.DeleteRes();
	m_bMainActive = FALSE;
}

//--------------------------------------------------------------------
//		DRAW
//--------------------------------------------------------------------
void CQuestData::Draw()
{
	g_pDisplay->Blt(m_pBackSur->Xpos, m_pBackSur->Ypos, m_pBackSur);
	//	m_CloseBtn.DrawToBlt();

	m_GiveupBtn.Draw(m_pBackSur->Xpos+165,m_pBackSur->Ypos+400);
	m_VerifyBtn.Draw();
	m_CheckBtn.Draw(m_pBackSur->Xpos+165+55, m_pBackSur->Ypos+400);

	if(m_Type==0)
		g_pDisplay->Blt(m_pIsumiron->Xpos, m_pIsumiron->Ypos, m_pIsumiron);
	else if(m_Type==1)
		g_pDisplay->Blt(m_pAdventure->Xpos, m_pAdventure->Ypos, m_pAdventure);
	//===================================================================================================
	// nate 2004 - 4
	// quest
	else if( m_Type == 2 )
		g_pDisplay->Blt( m_pToy->Xpos, m_pToy->Ypos, m_pToy );
	//===================================================================================================

	if( m_pBonusText && m_Type != -1 && (timeGetTime()%1000) > 300 ) // 깜빡이도록...(퀘스트가 세팅되었을때만 출력)
		m_pBonusText->Draw( m_pToy->Xpos+210, m_pToy->Ypos+45 ); // 퀘스트 명을 기준으로 일정 좌표에 보너스 여부를 출력.


	for(int i = 0 ; i< QuestData_EndOfEnum; i++)
	{
		if (m_pTextSur[i])
		{
			if( i == 2 )
				g_pDisplay->Blt(m_pTextSur[i]->Xpos+5, m_pTextSur[i]->Ypos + pCMyApp->Font_AddPosY+5, m_pTextSur[i]);
			else
				g_pDisplay->Blt(m_pTextSur[i]->Xpos, m_pTextSur[i]->Ypos + pCMyApp->Font_AddPosY, m_pTextSur[i]);
		}
	}
	//////0325
	if(!m_bMainActive)
		return;

	if(m_iMaxListCnt>0)
	{
		Show_List();
	}

	if(m_bScroll)
	{
		Show_ArrowBtn();
	}
}





//--------------------------------------------------------------------------------
//		텍스트를 그린다!!!
//--------------------------------------------------------------------------------
void CQuestData::PutString(int idx, char* String, int x, int y)
{
	if(idx < 0 || idx >= QuestData_EndOfEnum)
		return;

	if ( !g_pDisplay || !m_Font || String == NULL )
		return;

	int let = strlen(String);

	if(m_pTextSur[idx])
		SAFE_DELETE(m_pTextSur[idx]);

	if (let > 0)
	{
		// 서피스에 쓰고...
		if (g_bFixedWidthFont)
			g_pDisplay->CreateSurface( &m_pTextSur[idx], let*m_FontWidth,  m_FontHeight);
		else
		{
			HDC hdc;
			hdc = GetDC(pCMyApp->Get_hWnd());
			HFONT hFont = (HFONT) SelectObject(hdc, m_Font);

			SIZE size;
			GetTextExtentPoint32(hdc, String, let, &size);
			g_pDisplay->CreateSurface( &m_pTextSur[idx], size.cx,  m_FontHeight);

			SelectObject(hdc, hFont);
		}

		m_pTextSur[idx]->DrawText(m_Font, String, 0, 0, RGB(0, 0, 0), RGB(231, 203, 102),0);
		m_pTextSur[idx]->SetColorKey(RGB(0, 0, 0));

		// 화면에 맞게 위치를 정한다.
		m_pTextSur[idx]->Xpos = ((x));
		m_pTextSur[idx]->Ypos = ((y));
	}
}

void CQuestData::SetFont (int f_w, int f_h, char *fontname)
{
	if(fontname == NULL )
		return;

	HDC hDC = GetDC(NULL);

	int ptHeight = -MulDiv(f_h, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	// Font 생성
	m_Font = G_GetFont(ptHeight,f_w,fontname);
}





BOOL CQuestData::IsInside(int x, int y)
{
	// 아이텀 박스의 영역또한 마우스체크를 체크했다..
	if( !m_pBackSur )
		return false;

	return m_pBackSur->IsIn(x, y);
}

LRESULT CQuestData::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int x = 0, y = 0;
	char buf[128] = "";

	switch ( msg )
	{

	case WM_RBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		if (IsInside(x, y))
			return 1;
		break;

	case WM_LBUTTONDOWN:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		m_GiveupBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_VerifyBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CheckBtn.MsgProc(hWnd, msg, wParam, lParam);

		if(m_VerifyBtn.GetState() == BTN_ACTION)	//x버튼
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			m_VerifyBtn.m_iBtnFlg = BTN_NORMAL;
		}
		if( m_CheckBtn.GetState() == BTN_ACTION )  // 확인
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			m_CheckBtn.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_GiveupBtn.GetState() == BTN_ACTION && !m_bNoQuest)	//포기
		{
			m_GiveupBtn.m_iBtnFlg = BTN_NORMAL;

			// IDS_QUEST_REALLY_ABANDOM : 정말로 퀘스트를 포기하시겠습니까?
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_QUEST_REALLY_ABANDOM),TYPE_NOR_OKCANCLE, 14);

			return 1;
		}

		if(m_iMaxListCnt<0)
			break;

		if(m_bScroll)
		{
			m_pArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pArrowDown->MsgProc(hWnd, msg, wParam, lParam);

			int ret = IsInside_ScrollArea(x, y);

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

		return 1;

	case WM_LBUTTONUP:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if (!IsInside(x, y))
			return 0;

		m_GiveupBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_VerifyBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CheckBtn.MsgProc(hWnd, msg, wParam, lParam);

		if(m_VerifyBtn.GetState() == BTN_ACTION)	// x버튼
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			m_VerifyBtn.m_iBtnFlg = BTN_NORMAL;
		}
		if( m_CheckBtn.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			m_CheckBtn.m_iBtnFlg = BTN_NORMAL;
		}
		if(m_GiveupBtn.GetState() == BTN_ACTION && !m_bNoQuest)	// 포기
		{
			m_GiveupBtn.m_iBtnFlg = BTN_NORMAL;

			// IDS_QUEST_REALLY_ABANDOM : 정말로 퀘스트를 포기하시겠습니까?
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_QUEST_REALLY_ABANDOM),TYPE_NOR_OKCANCLE, 14);
			return 1;
		}

		if(m_bScroll)
		{
			m_pArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pArrowDown->MsgProc(hWnd, msg, wParam, lParam);

			if(m_pArrowUp->GetState() == BTN_ACTION)
				List_DownScroll();
			if(m_pArrowDown->GetState() == BTN_ACTION)
				List_UpScroll();

			m_eState = NONE;
		}
		return 1;

	case WM_MOUSEMOVE:

		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 마우스가 CControlScore 패널 위에 놓여 있지 않으면
		// 메시지를 처리하지 않는다.
		if (!IsInside(x, y))
			return 0;

		m_GiveupBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_VerifyBtn.MsgProc(hWnd, msg, wParam, lParam);
		m_CheckBtn.MsgProc(hWnd, msg, wParam, lParam);

		if(m_VerifyBtn.GetState() == BTN_ACTION)	// 확인
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			m_VerifyBtn.m_iBtnFlg = BTN_NORMAL;
		}

		if( m_CheckBtn.GetState() == BTN_ACTION )
		{
			g_pNk2DFrame->ShowInterfaceWindow(false, QUEST);
			m_CheckBtn.m_iBtnFlg = BTN_NORMAL;
		}

		if(m_GiveupBtn.GetState() == BTN_ACTION && !m_bNoQuest)	// 포기
		{
			m_GiveupBtn.m_iBtnFlg = BTN_NORMAL;

			// IDS_QUEST_REALLY_ABANDOM : 정말로 퀘스트를 포기하시겠습니까?
			g_pNk2DFrame->InsertPopup((char *)G_STRING(IDS_QUEST_REALLY_ABANDOM),TYPE_NOR_OKCANCLE, 14);
			return 1;
		}

		if(m_bScroll)
		{
			m_pArrowUp->MsgProc(hWnd, msg, wParam, lParam);
			m_pArrowDown->MsgProc(hWnd, msg, wParam, lParam);

			if(m_eState == SCROLLBAR_POLL)
				SetBarPosToGo (y);
		}
		return 1;
	}
	return 0;
}

HRESULT CQuestData::RestoreSurfaces()
{
	if (g_pDisplay)
		QuestMessage();

	return S_OK;
}

void CQuestData::SetName()
{
	int x = 0, y = 0;
	char strTemp[100] = "";
	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	PutString(1,  (char*)m_Name.c_str() , x,y);


	sprintf(strTemp, "%d - %d",m_MinLevel,m_MaxLevel);
	PutString(2,  strTemp , m_pBackSur->Xpos+138,m_pBackSur->Ypos+98);

	if( g_pRoh->m_bSecialQuest && m_EndType >= 6) //스폐셜 퀘스트면 퀘스트 하단 초기화
		PutString(3,  "" , x,y+16*2);


	if( m_Type ==1 && m_EndType < 7)
	{
		//데카둔의 모험가.
		if( m_Index == 27 ) // 데카둔의 팔찌는 하단에 아무 메세지 안나오도록 예외처리.(수행중인게 없다고 나와버린다.) ///070723 일본버그리포트 수정
		{
			sprintf(strTemp, " ");
			PutString(3,  strTemp , x,y+16*2);
		}
		if(m_Data0 !=0)
		{
			sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data0),m_Data5);
			PutString(3,  strTemp , x,y+16*2);
		}
		if(m_Data1 !=0)
		{
			sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data1),m_Data6);
			PutString(4,  strTemp , x,y+16*3);
		}

		if(m_Data2 !=0)
		{
			sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data2),m_Data7);
			PutString(5,  strTemp , x,y+16*4);
		}

		if(m_Data3 !=0)
		{
			sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data3),m_Data8);
			PutString(6,  strTemp , x,y+16*5);
		}
		if(m_Data4 !=0)
		{
			sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data4),m_Data9);
			PutString(7,  strTemp , x,y+16*6);
		}
	}
	else if(m_Type ==0)
	{
		// IDS_QUEST_INFO_0 : 킹몬스터를 제외한 저주받은 몬스터들은
		sprintf(strTemp, G_STRING(IDS_QUEST_INFO_0));
		PutString(3,  strTemp , x,y+16*2);


		// IDS_QUEST_INFO_1 : 해당 몬스터를 사냥하다보면 찾으실수
		sprintf(strTemp, G_STRING(IDS_QUEST_INFO_1));
		PutString(4,  strTemp , x,y+16*3);

		// IDS_QUEST_INFO_2 : 있습니다.
		sprintf(strTemp, G_STRING(IDS_QUEST_INFO_2));
		PutString(5,  strTemp , x,y+16*4);
	}
}

void CQuestData::SetNoQuest()
{
	CloseRes();

	for(int i=0; i<QuestData_EndOfEnum; i++)
	{
		if(m_pTextSur[i])
			SAFE_DELETE(m_pTextSur[i]);
	}
	m_Index = -1;
	m_Type = -1;

	m_Name.clear();
	m_String0.clear();
	m_String1.clear();
	m_String2.clear();
	m_String3.clear();
	m_String4.clear();
	m_String5.clear();
	m_Content.clear();



	m_Data0 = 0;
	m_Data1 = 0;
	m_Data2 = 0;
	m_Data3 = 0;
	m_Data4 = 0;
	m_Data5 = 0;
	m_Data6 = 0;
	m_Data7 = 0;
	m_Data8 = 0;
	m_Data9 = 0;

	m_bNoQuest = TRUE;
	SetKillMob0(0);
}

void CQuestData::Reload(int index, int kill_num)
{
	switch(index)
	{
	case 0:
		SetKillMob0(kill_num);
		break;
	case 1:
		SetKillMob1(kill_num);
		break;
	case 2:
		SetKillMob2(kill_num);
		break;
	case 3:
		SetKillMob3(kill_num);
		break;
	case 4:
		SetKillMob4(kill_num);
		break;
	}
}

void CQuestData::SetKillMob0(int num)
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	m_Data5 = num;
	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	if( m_bNoQuest )
		strcpy(strTemp,G_STRING(IDS_QUEST_NO_DATA));
	else
		sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data0),num);

	PutString(3,  strTemp , x,y+16*2);
}

void CQuestData::SetKillMob1(int num)
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	m_Data6 = num;
	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data1),num);
	PutString(4,  strTemp , x,y+16*3);
}

void CQuestData::SetKillMob2(int num)
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	m_Data7 = num;
	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data2),num);
	PutString(5,  strTemp , x,y+16*4);
}

void CQuestData::SetKillMob3(int num)
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	m_Data8 = num;
	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data3),num);
	PutString(6,  strTemp , x,y+16*5);
}

void CQuestData::SetKillMob4(int num)
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	m_Data9 = num;
	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf(strTemp, "%s: %d",(char*)GET_NPC_NAME_VNUM(m_Data4),num);
	PutString(7,  strTemp , x,y+16*6);
}

//////////////

void CQuestData::Init(int MaxListCnt, int ViewCnt)
{
	SetMaxListCnt(MaxListCnt);
	ViewCnt > m_iMaxListCnt ? m_iViewCnt = m_iMaxListCnt :	m_iViewCnt = ViewCnt;
	m_iStartViewCnt = 0;
	m_iEndViewCnt   = m_iStartViewCnt+m_iViewCnt;
	m_bMainActive = TRUE;
}


void CQuestData::Show_List()
{
	if( m_iEndViewCnt > m_iMaxListCnt )
		m_iEndViewCnt = m_iMaxListCnt;

	for(int i=m_iStartViewCnt; i<m_iEndViewCnt; i++)
	{
		if(m_pListSur[i])
		{
			m_pListSur[i]->m_StringPosX = m_pBackSur->Xpos+12;
			m_pListSur[i]->m_StringPosY = m_pBackSur->Ypos+134+((TEXT_SIZE+TEXT_CY_GAP)*(i-m_iStartViewCnt));
			m_pListSur[i]->Draw();
		}
	}
}

void CQuestData::Show_ArrowBtn()
{
	if(m_bScroll)
	{
		m_pArrowUp->Draw();
		m_pArrowDown->Draw();
		g_pDisplay->Blt(m_pBarSur->Xpos, m_pBarSur->Ypos, m_pBarSur);
	}
}


void CQuestData :: SetBarPosToGo (int y)
{
	if (m_iMaxListCnt > m_iViewCnt)
	{
		if (y < m_pArrowUp->PosY+m_pArrowUp->Height+m_pBarSur->GetHeight()/2)
			GoHome ();
		else if (y > m_pArrowDown->PosY-m_pBarSur->GetHeight()/2)
			GoEnd ();
		else
		{
			int HeightCanBeScrolled = m_iListBox_Cy - (2*m_pArrowUp->Height);

			// 한 line당의 pixel수

			int interval = HeightCanBeScrolled / (m_iMaxListCnt-m_iViewCnt);

			m_iStartViewCnt = (y-m_pArrowUp->PosY-m_pArrowUp->Height) / interval;
			m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;

			CalculateBarPos ();
		}
	}
}

void CQuestData::CalculateBarPos ()
{
	if (m_iMaxListCnt > m_iViewCnt)
	{
		int n1 = m_iMaxListCnt - m_iViewCnt;
		int n2 = m_iStartViewCnt;

		if (n1 != n2)
		{
			// 화면상에 스크롤될 수 있는 height pixel 수 (Bar Height 포함하지 않음)
			int HeightCanBeScrolled = m_iListBox_Cy - 2*m_pArrowUp->Height - m_pBarSur->GetHeight();

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

void CQuestData::GoHome ()
{
	m_iStartViewCnt = 0;
	m_pBarSur->Ypos = m_pArrowUp->PosY+m_pArrowUp->Height;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
}

void CQuestData::GoEnd ()
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

//-----------------------------------------------------------------
// name : List_UpScroll
// desc : 스크롤버튼(up)이 눌린경우 보여주는 list의 변화..
//-----------------------------------------------------------------
void CQuestData::List_UpScroll()
{
	if(m_iEndViewCnt >= m_iMaxListCnt) return;
	m_iStartViewCnt++;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
	CalculateBarPos ();
}

//-----------------------------------------------------------------
// name : List_UpScroll
// desc : 스크롤버튼(up)이 눌린경우 보여주는 list의 변화..
//-----------------------------------------------------------------
void CQuestData::List_DownScroll()
{
	if(m_iStartViewCnt <= 0) return;
	m_iStartViewCnt--;
	m_iEndViewCnt = m_iStartViewCnt+m_iViewCnt;
	CalculateBarPos ();
}

int CQuestData::IsInside_ScrollArea(int x, int y)
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

void CQuestData::CreListSurface()
{
	if( !g_pDisplay )
		return;

	int i;

	if(m_pListSur)
	{
		for (i=0; i < m_iMaxListCnt; i++)
		{
			if (m_pListSur[i])
				SAFE_DELETE(m_pListSur[i]);
		}// end for
		SAFE_DELETE_ARRAY(m_pListSur);
	}// end if
	m_pListSur  = new CTextOutBox*[m_iMaxListCnt];

	for(i=0; i<m_iMaxListCnt; i++)
	{
		m_pListSur[i] = new CTextOutBox();
		m_pListSur[i]->Init(TEXT_SIZE, NAME_COLOR, 0, 0);
		m_pListSur[i]->SetString(m_sListStr[i]);
	}
}

void CQuestData::SetMaxListCnt(int cnt)
{
	m_iMaxListCnt = cnt;
	m_sListStr = new char*[m_iMaxListCnt];

	for(int i=0; i<m_iMaxListCnt; i++)
	{
		m_sListStr[i] = new char[200];
		m_sListStr[i][0] = '\0';
	}
}

const int LINE_PER_TEXT = 40;

BOOL CQuestData::QuestMessage() //1203 레벨업시에 나타나는 도움말 창.
{
	CloseRes();
	int tot_line = 0;
	char strTemp[100] = "";

	if(m_bNoQuest)
	{
		SetKillMob0(0);
		return 0;
	}

	if(!g_pNk2DFrame->IsInterfaceVisible(QUEST) || m_bReload)
	{
		m_bReload = FALSE;
		tot_line = GetScrollSize();//25;
		Init(tot_line, 8);	//15

		// List 의 개수가 1개 이상이라면
		if(m_iMaxListCnt>0)
			CreListSurface();

		// 스크롤 기능을 사용하는가??
		if(!m_bNoQuest)
			m_iMaxListCnt - m_iViewCnt > 0 ? m_bScroll = TRUE : m_bScroll = FALSE;

		if(m_bScroll)
		{

			if(m_pArrowUp) SAFE_DELETE(m_pArrowUp);
			m_pArrowUp = new CBasicButton();

			if(m_pArrowDown) SAFE_DELETE(m_pArrowDown);
			m_pArrowDown = new CBasicButton();

			// Arrow Button FileName
			m_pArrowUp->SetFileName("info_arr_up");
			m_pArrowUp->SetSize( TEXT_SIZE, ONE_LINE_CY );
			m_pArrowUp->SetDisable( false );

			m_pArrowDown->SetFileName("info_arr_down");
			m_pArrowDown->SetSize( TEXT_SIZE, ONE_LINE_CY );
			m_pArrowDown->SetDisable( false );

			m_pArrowUp->LoadRes();
			m_pArrowDown->LoadRes();

			m_pArrowUp->PosX = m_iListBox_X+m_iListBox_Cx - m_pArrowUp->Width;
			m_pArrowUp->PosY = m_iListBox_Y-4;
			m_pArrowDown->PosX = m_iListBox_X+m_iListBox_Cx - m_pArrowUp->Width;
			m_pArrowDown->PosY = m_iListBox_Y + (ONE_LINE_CY*(m_iViewCnt-1))-4;

			int a, b;
			a = m_iMaxListCnt - m_iViewCnt;
			b = m_iViewCnt - (a+2);

			if(b<=0) b = 1;

			g_pDisplay->CreateSurfaceFromBitmap (&m_pBarSur, "interface/common/scroll_thumb.bmp");
			//			g_pDisplay->CreateSurfaceFromBitmap (&m_pBarSur, "interface/common/scroll_thumb.bmp", m_pArrowUp->Width, (BAR_CY*b)); // scroll bar surface
			m_pBarSur->SetColorKey(RGB(0, 0, 0));
			m_pBarSur->Xpos = m_pArrowUp->PosX;
			m_pBarSur->Ypos = m_pArrowUp->PosY + m_pArrowUp->Height;
		}
		//17글자가 최고 길이로 맞춰야 함.*/
		strcpy(strTemp,"");

		if(!m_Name.empty())
		{
			SetName();

			AddStringMulti((char*)m_String0.c_str(), LINE_PER_TEXT);
			strcpy(strTemp,"");

			AddStringMulti((char*)m_String1.c_str(), LINE_PER_TEXT);
			strcpy(strTemp,"");

			if(   !( g_dwClientCountry == CTRY_JPN && m_Type == 1 && (m_Sub == 387 || m_Sub == 397) )   )  ///07-05-07 이 퀘스트 들은 내용이 중복되는 스트링이 표시된다. 일본이 그거 싫다고 해서 예외처리해줌.
			{
				if(m_Exp)
				{
					// IDS_QUEST_GET_EXP : 획득 경험치 : %d
					sprintf(strTemp, G_STRING(IDS_QUEST_GET_EXP),m_Exp);
					SetAddStr(strTemp);
					strcpy(strTemp,"");
				}
				if(m_Laim)
				{
					// IDS_QUEST_GET_LAIM : 획득 라임 : %d
					sprintf(strTemp, G_STRING(IDS_QUEST_GET_LAIM),m_Laim);
					SetAddStr(strTemp);
					strcpy(strTemp,"");
				}
			}

			AddStringMulti((char*)m_String2.c_str(), LINE_PER_TEXT);
			strcpy(strTemp,"");

			if(!m_String3.empty())
			{
				AddStringMulti((char*)m_String3.c_str(), LINE_PER_TEXT);
				strcpy(strTemp,"");
			}

			if(!m_String4.empty())
			{
				AddStringMulti((char*)m_String4.c_str(), LINE_PER_TEXT);
				strcpy(strTemp,"");
			}
			if(!m_String5.empty())
			{
				AddStringMulti((char*)m_String5.c_str(), LINE_PER_TEXT);
				strcpy(strTemp,"");
			}

			if(!m_Content.empty())
			{
				AddStringMulti((char*)m_Content.c_str(), LINE_PER_TEXT);
				strcpy(strTemp,"");
			}
		}
		CreListSurface();
	}
	else
		CloseRes();


	return 1;
}

BOOL CQuestData::SetAddStr(char* str)
{
	if(m_iCurListNum >= m_iMaxListCnt)
		return false;

	if(!str) return false;
	strcpy(m_sListStr[m_iCurListNum++], str);
	return true;
}

void CQuestData::CloseRes()
{
	if(m_iMaxListCnt >0)
	{
		for(int i=0; i<m_iMaxListCnt; i++)
		{
			SAFE_DELETE(m_pListSur[i]);
			SAFE_DELETE_ARRAY(m_sListStr[i]); // delete char**
		}
	}
	SAFE_DELETE_ARRAY(m_pListSur);
	SAFE_DELETE_ARRAY(m_sListStr); // delete char**

	if(m_bScroll)
	{
		SAFE_DELETE(m_pArrowUp);
		SAFE_DELETE(m_pArrowDown);
		SAFE_DELETE(m_pBarSur);
		m_bScroll = FALSE;
	}

	m_iCurListNum = 0;
	m_iMaxListCnt = 0;
	m_iViewCnt = 0;
}


void CQuestData::AddStringMulti(char *string, int len)
{
	char descr_buf[102] = "";

	if (!string)
		return;

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

			if( string[pos] == 13 ||  string[pos] == 10 )	//엔터키 문자 Skip!
				pos++;
			else
				descr_buf[idx++] = string[pos++];
		}
		descr_buf[idx] = '\0';
		SetAddStr(descr_buf);
	}
}

void CQuestData::ReadMobNameFile(char *file_name)
{
	return;
	PFILE *fp = NULL;
	int npc_count, len;

	fp = pfopen(file_name, "rb");
	if (fp == NULL)
	{
		MessageBox(NULL, "Failed to load map data", "error!!", MB_OK);
		return;
	}

	pfread(&npc_count, sizeof(int), 1, fp);
	m_nNpcNameNum = npc_count;

	MobNameList = new MOBNAMESTRUCT[m_nNpcNameNum];

	for (int i = 0; i < m_nNpcNameNum; i++)
	{
		pfread(&(MobNameList[i].nVnum), sizeof(int), 1, fp);		// 맙 번호
		pfread(&len, sizeof(int), 1, fp);						// 화일 이름 길이.
		//assert(len > 0);										// 맙 이름은 있어야 한다.
		MobNameList[i].mob_name = new char[len+1];
		pfread(MobNameList[i].mob_name, len, 1, fp);				// 맙 이름 읽는다.
		MobNameList[i].mob_name[len] = NULL;
	}

	pfclose(fp);
}

int CQuestData::GetScrollSize()
{
	int cnt=0,size=0;

	size = m_String0.size();
	if(size != 0)
		cnt += 1 + size/36;
	size = m_String1.size();
	if(size != 0)
		cnt += 1 + size/36;
	size = m_String2.size();
	if(size != 0)
		cnt += 1 + size/36;
	size = m_String3.size();
	if(size != 0)
		cnt += 1 + size/36;
	size = m_String4.size();
	if(size != 0)
		cnt += 1 + size/36;
	size = m_String5.size();
	if(size != 0)
		cnt += 1 + size/36;
	size = m_Content.size();
	if(size != 0)
		cnt += 1 + size/36;


	if(m_Exp!=0)
		cnt++;
	if(m_Laim!=0)
		cnt++;

	return cnt; //1줄은 여분으로.
}

void CQuestData::PopUpMsg(int msg)
{
	switch(msg)
	{
	case -1:
		// IDS_QUEST_NO_MACHINE : 수신기가 없습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_NO_MACHINE), TYPE_NOR_OK, 1 );
		break;
	case -2:
		// IDS_QUEST_CONFIRM_LEVEL : 수신기 레벨이 안맞습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_CONFIRM_LEVEL), TYPE_NOR_OK, 1 );
		break;
	case -3:
		// IDS_QUEST_UNABLE_MACHINE : 다 사용한 수신기입니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_UNABLE_MACHINE), TYPE_NOR_OK, 1 );
		break;
	case -4:
		// IDS_QUEST_NOT_RECEIVE : 퀘스트를 받을수 없습니다.
		g_pNk2DFrame->InsertPopup( (char*)G_STRING(IDS_QUEST_NOT_RECEIVE), TYPE_NOR_OK, 1 );
		break;
	}
}

//===========================================================================
// nate 2004 - 4 : quest
void CQuestData::SetTotalPoint( int nTotalPoint )
{
	m_nQuestTotalPoint = nTotalPoint;

	int x,y;
	char strTemp[100];

	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf( strTemp, G_STRING(IDS_QUESTRUIN_TOTAL), nTotalPoint );
	PutString( 3, strTemp, x, y+16*2 );
}

void CQuestData::SetAddPoint( int nAddPoint )
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf( strTemp, G_STRING(IDS_QUESTRUIN_ADD), nAddPoint );
	PutString( 4, strTemp, x, y+16*3 );
}


void CQuestData::SetRemainPoint( int nRemainPoint )
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf( strTemp, G_STRING(IDS_QUESTRUIN_REM), nRemainPoint );
	PutString( 5, strTemp, x, y +16*4);
}

void CQuestData::SetNextQuest()
{
	int x = 0,y = 0;
	char strTemp[100] = "";

	x = m_pBackSur->Xpos+14;
	y = m_pBackSur->Ypos+280;

	sprintf( strTemp, G_STRING(IDS_QUESTRUIN_CALL) );
	PutString( 5, strTemp, x, y +16*4);
}
//===========================================================================

LRESULT CQuestData::PopupMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, int nMsg)
{
	if (nMsg == 1)
	{
		if (g_pTcpIp)	g_pTcpIp->SendNetMessage("quest cancel\n");
		SetNoQuest();
	}

	return 0L;
}
