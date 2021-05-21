#include "stdafx.h"
#include "CreateCharacterPage.h"
#include "headers.h"
#include "Sheet.h"
#include "SMRender.h"
#include "selectcharacterpage.h"
#include "setvaluepage.h"
#include "DirtSnd.h"
#include "Country.h"
#include "g_stringmanager.h"
#include "ChaAct.h"
#include "main.h"
#include "LHTextChecker.h"
#include "LHUI_Captcha.h"
#include "CommonConfig.h"

#define GOBACK 1
#define GONEXT 2
#define GOMAKE 3

#define BACKGROUND_CREATECHARACTERPAGE		"interface/MakeChar/back_crechapage.bmp"

bool strinc(char *str, const char *inc);
char *ExamSpChar(char *str);
bool ExamNumOnly(char* str);
bool IsTowByte(char str);
char *ExamSpChar_for_jpn(char *str, bool bKata);
char *ExamEscChar_for_jpn(char *str);
char * Check2ByteChar(char * str);
BOOL Str2Str(char * str1, char * str2);

extern BOOL g_bHuman;
extern BOOL g_bAidia;
extern DWORD g_dwClientCountry;
extern DWORD g_dwLangType;




HHOOK g_hHook = NULL;
LRESULT CALLBACK KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(g_hHook, code, wParam, lParam);
	else
	{
		if (wParam == VK_HANJA)	// 한자키 무시
			return -1;
		else
			return CallNextHookEx(g_hHook, code, wParam, lParam);
	}
}

//-----------------------------------------------------------------------------------------
CCreateCharacterPage::CCreateCharacterPage()
{
	m_pSVPage = NULL;
	m_pSheet = NULL;
	m_pBatangSur = NULL;
	m_pszProperList = NULL;
	m_nProperListCount = 0;

	ZeroMemory( m_pInfo, sizeof( CSurface* ) * NUM_RACE );
	m_iOldRaceNum = 0;
	m_MsgPopUp.Init(TYPE_NOR_OK);
	for(int i=0; i<DENY_STRING_NUM; i++)
	{
		m_Deny_String[i] = NULL;
	}

	m_captcha = NULL;
}

CCreateCharacterPage::CCreateCharacterPage( CSheet* pSheet )
{
	m_pSVPage = NULL;
	m_pSheet = NULL;
	m_pBatangSur = NULL;
	m_pszProperList = NULL;
	m_nProperListCount = 0;
	SetSheetPtr( pSheet );

	ZeroMemory( m_pInfo, sizeof( CSurface* ) * NUM_RACE );
	m_iOldRaceNum = 0;
	m_MsgPopUp.Init(TYPE_NOR_OK);
	m_pSVPage = new CSetValuePage(pSheet);
	for(int i=0; i<DENY_STRING_NUM; i++)
	{
		m_Deny_String[i] = NULL;
	}

	m_captcha = NULL;
}

CCreateCharacterPage::~CCreateCharacterPage()
{
	for(int i=0; i<DENY_STRING_NUM; i++)
		SAFE_DELETE_ARRAY(m_Deny_String[i]);

	for( int i = 0 ; i < m_nProperListCount ; i ++ )
		SAFE_DELETE_ARRAY(m_pszProperList[i]);
	SAFE_DELETE_ARRAY(m_pszProperList);

	DeleteRes();  // <-- 이게 먼저 그담에 delete m_pSVPage 이다.
	SAFE_DELETE( m_pSVPage );
}

void CCreateCharacterPage::SetSheetPtr( CSheet* pSheet )
{
	m_pSheet = pSheet;
}

void CCreateCharacterPage::Init()
{
	m_BackBtn.SetFileName( "common/btn_toleft2" );
	m_NextBtn.SetFileName("common/btn_toright2");

	m_BackBtn.SetDisable( false );
	m_NextBtn.SetDisable( false );

	m_NameBox.Init(203, 127, CRECHAR_EDIT_FSIZE,
				   RGB(0, 0, 1), CRECHAR_EDIT_FCOLOR, FALSE, NEW_CHARNAME);
	m_NameBox.SetEnable(TRUE);

	m_SexBox.SetSize(2);
	m_SexBox.SetString(0, (char*)G_STRING(IDS_CRECHA_MALE));
	m_SexBox.SetString(1, (char*)G_STRING(IDS_CRECHA_FEMALE));

	m_HairBox.Init(CRECHAR_HAIR_X, CRECHAR_HAIR_Y, CRECHAR_SPIN_WIDTH, CRECHAR_SPIN_IMGHEIGHT,
				   CRECHAR_SPIN_FSIZE, CRECHAR_SPIN_FCOLOR);
	m_RaceBox.Init(CRECHAR_RACE_X, CRECHAR_RACE_Y, CRECHAR_SPIN_WIDTH, CRECHAR_SPIN_IMGHEIGHT,
				   CRECHAR_SPIN_FSIZE, CRECHAR_SPIN_FCOLOR);
	m_SexBox.Init(CRECHAR_SEX_X, CRECHAR_SEX_Y , CRECHAR_SPIN_WIDTH, CRECHAR_SPIN_IMGHEIGHT,
				  CRECHAR_SPIN_FSIZE, CRECHAR_SPIN_FCOLOR);

	char buf[10][80] = {0,};
	for(int i=0; i<10; i++)
		// IDS_CRECHA_HAIR_01 : Style %d
		sprintf(buf[i], G_STRING(IDS_CRECHA_HAIR_01), i+1);

	m_HairBox.SetSize(10);
	for( int i = 0 ; i < 10 ; i ++ )
		m_HairBox.SetString(i, buf[i]);

	for( int i=0; i<DENY_STRING_NUM; i++)
	{
		m_Deny_String[i] = NULL;
		m_Deny_String[i] = new char[400];
	}
	// IDS_CRECHA_MANAGEMENT :
	strcpy(m_Deny_String[0], G_STRING(IDS_CRECHA_MANAGEMENT));		// 운영
	strcpy(m_Deny_String[1], G_STRING(IDS_CRECHA_LAGHAIM_HAN));		// 라그하임
	strcpy(m_Deny_String[2], G_STRING(IDS_CRECHA_LAGHAIM_ENG));		// laghaim
	strcpy(m_Deny_String[3], G_STRING(IDS_CRECHA_WHISPER));			// *귓속말*
	strcpy(m_Deny_String[4], G_STRING(IDS_CRECHA_GUILD));			// *GUILD*
	strcpy(m_Deny_String[5], G_STRING(IDS_CRECHA_PARTY));			// *Party*
	strcpy(m_Deny_String[6], G_STRING(IDS_CRECHA_MUDORI));			// 머도리
	strcpy(m_Deny_String[7], G_STRING(IDS_CRECHA_SHANGSHANG));		// 생생
	strcpy(m_Deny_String[8], G_STRING(IDS_CRECHA_GURIWORA));		// 그리워라
	strcpy(m_Deny_String[9], "머딜이");
}


void CCreateCharacterPage::SetBackground()
{
	SAFE_DELETE( m_pBatangSur );
	if( g_pDisplay )
	{
		g_pDisplay->CreateSurfaceFromBitmap( &m_pBatangSur, BACKGROUND_CREATECHARACTERPAGE, 0, 0);

		if( m_pBatangSur )
			m_pBatangSur->SetColorKey(TRANS_COLOR);
	}
}


void CCreateCharacterPage::LoadRes()
{
	if( g_dwClientCountry == CTRY_KOR || g_dwClientCountry == CTRY_TH )
		ReadProperList("proper.nam");
	SetBackground();
	Init();

	m_BackBtn.LoadRes();
	m_NextBtn.LoadRes();
	m_NameBox.SetfontSize();
	m_RaceBox.LoadRes("common/btn_toright2", "common/btn_toleft2");
	m_SexBox.LoadRes("common/btn_toright2", "common/btn_toleft2");
	m_HairBox.LoadRes("common/btn_toright2", "common/btn_toleft2");

	char szTemp[ 100 ] = "";
	for( int i = 0 ; i < NUM_RACE ; i ++ )
	{
		SAFE_DELETE( m_pInfo[ i ] );
		sprintf( szTemp, "interface/MakeChar/info%d.bmp", i );
		if( g_pDisplay )
			g_pDisplay->CreateSurfaceFromBitmap( &m_pInfo[ i ], szTemp );
		if( m_pInfo[ i ] )
			m_pInfo[ i ]->SetColorKey( RGB( 0, 0, 0 ) );
	}
	m_MsgPopUp.LoadRes();
	if (g_dwClientCountry == 0)
		m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_BADNAME));

	if( m_pSVPage )
	{
		m_pSVPage->LoadRes();
	}

	if( CommonConfig::Instance()->GetCaptcha() == true )
	{
		m_captcha = new LHUI_Captcha(m_pSheet);
		m_captcha->LoadRes();
	}
}

void CCreateCharacterPage::DeleteRes()
{
	m_BackBtn.DeleteRes();
	m_NextBtn.DeleteRes();
	m_RaceBox.DeleteRes();
	m_SexBox.DeleteRes();
	m_HairBox.DeleteRes();
	m_NameBox.DeleteRes();
	SAFE_DELETE( m_pBatangSur );

	for( int i = 0 ; i < NUM_RACE ; i ++ )
	{
		SAFE_DELETE( m_pInfo[ i ] );
	}

	m_MsgPopUp.DeleteRes();
	m_pSVPage->DeleteRes();

	SAFE_DELETE(m_captcha);
}

void CCreateCharacterPage::Draw()
{
	if( !g_pDisplay )
		return;

	if( m_pSheet )
	{
		m_pSheet->RenderBasicCha(290, 135, 184, 254);
	}

	g_pDisplay->Blt( 0, 0, m_pBatangSur, NULL );


	// 에이디아
	if (g_bAidia)
	{
		g_pDisplay->Blt( RACE_INFO_X, RACE_INFO_Y, m_pInfo[m_RaceBox.m_CurSel], NULL );
	}
	else
	{
		if (m_RaceBox.m_CurSel == 2)
			g_pDisplay->Blt( RACE_INFO_X, RACE_INFO_Y, m_pInfo[2], NULL );
		else if (m_RaceBox.m_CurSel == 3)
			g_pDisplay->Blt( RACE_INFO_X, RACE_INFO_Y, m_pInfo[3], NULL );
		else
			g_pDisplay->Blt( RACE_INFO_X, RACE_INFO_Y, m_pInfo[m_RaceBox.m_CurSel], NULL );
	}



	m_BackBtn.Draw( 292, 562 );
	m_NextBtn.Draw( 472, 562 );

	m_NameBox.Draw( 85, 155, 177 );
	m_RaceBox.Draw();
	m_SexBox.Draw();
	m_HairBox.Draw();
	if( m_pSVPage )
		m_pSVPage->Draw();

	m_MsgPopUp.Draw();

	if( m_captcha )
		m_captcha->Draw();
}

void CCreateCharacterPage::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	int ret = 0;
	if(m_MsgPopUp.CheckMsgExist() == TRUE)
	{
		if((ret = m_MsgPopUp.MsgProc(hWnd, msg, wParam, lParam))==1)
		{
			g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, NULL, GetCurrentThreadId());// nate 2006-03-15 : 메세지 훅 설정
			m_NameBox.SetEnable(TRUE);
			m_BackBtn.SetState(BTN_NORMAL);
			m_NextBtn.SetState(BTN_NORMAL);

			if (g_pDSound)
				g_pDSound->Play(g_SheetSound[SHEET_SND_CLICK]);
		}
		else if(ret ==-1)
			return;

		return;
	}

	if( m_captcha && !m_captcha->_GetDisalbe() )
	{
		if( m_captcha->MsgProc(hWnd, msg, wParam, lParam) == 2 )
			Action(GOMAKE);

		return;
	}

	switch ( msg )
	{
	case WM_LBUTTONDOWN:

		m_BackBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_NextBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_NameBox.MsgProc( hWnd, msg, wParam, lParam );
		m_RaceBox.MsgProc( hWnd, msg, wParam, lParam );
		m_SexBox.MsgProc( hWnd, msg, wParam, lParam );
		m_HairBox.MsgProc( hWnd, msg, wParam, lParam );
		if( m_pSVPage )
			m_pSVPage->MsgProc( hWnd, msg, wParam, lParam );

		break;

	case WM_LBUTTONUP:

		m_BackBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_NextBtn.MsgProc( hWnd, msg, wParam, lParam );
		if( m_pSVPage )
			m_pSVPage->MsgProc( hWnd, msg, wParam, lParam );

		if ( m_BackBtn.GetState() == BTN_ACTION )
		{
			UnhookWindowsHookEx(g_hHook);// nate 2006-03-15 : 메세지 훅 핸득 제거
			m_BackBtn.m_iBtnFlg = BTN_NORMAL;
			if (g_dwLangType == 1)
				m_NameBox.CloseIMC();

			Action( GOBACK );
		}
		else if ( m_NextBtn.GetState() == BTN_ACTION )
		{
			UnhookWindowsHookEx(g_hHook);
			m_NextBtn.m_iBtnFlg = BTN_NORMAL;

			int perom_count = 0;
			for( int i=0 ; i<SLOT_COUNT ; i++ )
			{
				if( m_pSheet->m_Slot[i] && m_pSheet->m_Slot[i]->m_Race== RACE_PEROM )
					perom_count++;
			}

			if( m_RaceBox.m_CurSel == RACE_PEROM && perom_count != 0 )
			{
				m_MsgPopUp.PutString((char*)G_STRING(IDS_LHSTRING1941));
				return;
			}

			Action( GONEXT );;
		}
		if (m_pSheet && m_RaceBox.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
		{
			if(m_pSheet->m_SelRace !=m_RaceBox.m_CurSel)
				OutoSetValue(m_RaceBox.m_CurSel);

			if (g_bAidia)
				// 에이디아.
				m_pSheet->m_SelRace = m_RaceBox.m_CurSel;
			else
			{
				// 2월 9일 패치 앞두고 수정.
				if (m_RaceBox.m_CurSel == 2)
				{
					m_RaceBox.m_CurSel = 3;
					m_pSheet->m_SelRace = 3;
				}
				else
					m_pSheet->m_SelRace = m_RaceBox.m_CurSel;
			}

			if( m_pSheet )
				m_pSheet->SetBasicCha(m_pSheet->m_SelRace, m_SexBox.m_CurSel, m_HairBox.m_CurSel);
		}
		if (m_pSheet && m_SexBox.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
		{
			m_pSheet->m_SelSex = m_SexBox.m_CurSel;
			m_pSheet->SetBasicCha(m_pSheet->m_SelRace, m_SexBox.m_CurSel, m_HairBox.m_CurSel);
		}
		if (m_pSheet && m_HairBox.MsgProc( hWnd, msg, wParam, lParam ) >= 0)
		{
			m_pSheet->SetBasicCha(m_pSheet->m_SelRace, m_SexBox.m_CurSel, m_HairBox.m_CurSel);

			if(m_pSheet->m_SelRace != m_iOldRaceNum)
			{
				m_HairBox.m_CurSel = 0;
				m_iOldRaceNum = m_pSheet->m_SelRace;
			}
		}

		break;

	case WM_MOUSEMOVE:

		m_BackBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_NextBtn.MsgProc( hWnd, msg, wParam, lParam );
		m_pSVPage->MsgProc( hWnd, msg, wParam, lParam );
		break;

	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:


		// 캐릭터 이름 이상하게 입력 못하도록 컨트롤 키를 막음
		if( GetKeyState(VK_CONTROL) & 0x8000 ) // 컨트롤 키가 눌려있으면...
			return;

		if (wParam == VK_HANJA)	// 한자키 무시
			return;

		m_NameBox.MsgProc( hWnd, msg, wParam, lParam );
		break;
	}
}

void CCreateCharacterPage::Action( int i )
{
	if (g_pDSound)
		g_pDSound->Play(g_SheetSound[SHEET_SND_CLICK]);

	switch ( i )
	{
	case GOBACK:  // 뒤로 버튼이 누렸을때..종족/성별/머리스타일/등의 플레그들을 초기값으로 맞춘다
		{
			m_RaceBox.m_CurSel = 0;
			m_SexBox.m_CurSel = 0;
			m_HairBox.m_CurSel = 0;
			m_NameBox.SetEnable(TRUE);
			m_NameBox.ClearText();
			if( m_pSheet )
				m_pSheet->GoPage( SELCHARPAGE );

			if (g_dwClientCountry == 0 || g_dwLangType == 0)
				m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_BADNAME));

			if( m_pSheet && m_pSheet->m_pSelCharPg )
			{
				m_pSheet->m_pSelCharPg->m_StartBtn.SetDisable( TRUE );
				m_pSheet->m_pSelCharPg->m_NewBtn.SetDisable( FALSE );
				m_pSheet->m_pSelCharPg->m_DeleteBtn.m_iBtnFlg = BTN_DISABLE;
				m_pSheet->m_pSelCharPg->m_DeleteBtn.SetDisable( FALSE );
				m_pSheet->m_pSelCharPg->m_DeleteBtn.SetVisible( TRUE );
				m_pSheet->m_pSelCharPg->SetBoxesVisible( FALSE );
				m_pSheet->m_pSelCharPg->m_bNullSlot = TRUE;
			}

			if( CommonConfig::Instance()->GetCaptcha() == true )
			{
				m_captcha->_SetVisible(false);
				m_captcha->_SetDisable(true);
			}			
		}

		break;

	case GONEXT:

		{
			m_NameBox.SetEnable(FALSE);
			m_BackBtn.SetState(BTN_DISABLE);
			m_NextBtn.SetState(BTN_DISABLE);

			char *found_sp = NULL;
			// 캐릭터 이름입력에 관한 popup message
			if (strlen(m_NameBox.GetString()) == 0)
			{
				// IDS_CRECHA_INPUT_NAME : 이름을 입력하세요
				m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_INPUT_NAME));
				return;
			}
			else if(CheckSpace(m_NameBox.GetString()) == TRUE)
			{
				// IDS_CRECHA_UNABLE_BLANK : 캐릭터 이름은 공백을 허용하지 않습니다
				m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_UNABLE_BLANK));
				return;
			}
			if (strinc(m_NameBox.GetString(), "'"))
			{
				m_MsgPopUp.PutString((char*)G_STRING(IDS_LHSTRING1884));
				return;
			}

			int min_name_cnt = 0;
			if(g_dwLangType == 1) // jpn mode
				min_name_cnt = 4;
			else
				min_name_cnt = 4;

			if (strlen(m_NameBox.GetString()) < min_name_cnt
					|| strlen(m_NameBox.GetString()) > 12 )
			{
				// IDS_CRECHA_NAME_LIMIT : 캐릭터 이름은 12자 이하 입니다
				m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_NAME_LIMIT));
				return;
			}

			if(g_dwLangType == 1 )	 // 일본어일경우 특수문자가 2바이트 1바이트가 공존하기땜에..ㅡㅡ;
			{
				if((found_sp = ExamSpChar_for_jpn(m_NameBox.GetString(), false)) != NULL)
				{
					char msgStr[100] = "";
					// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
					sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), found_sp);
					m_MsgPopUp.PutString(msgStr);
					return;
				}
				if((found_sp = ExamSpChar_for_jpn(m_NameBox.GetString(), true)) != NULL)
				{
					m_MsgPopUp.PutString((char*)G_STRING(IDS_LHSTRING1884));
					return;
				}

				if ((found_sp = ExamEscChar_for_jpn(m_NameBox.GetString())) != NULL)
				{
					m_MsgPopUp.PutString((char*)G_STRING(IDS_LHSTRING1884));
					return;
				}

				char str[24][80] = {0,};

				strcpy(str[0], "support");
				strcpy(str[1], "gammania");
				strcpy(str[2], "gamemaster");
				strcpy(str[3], "laghaim");
				strcpy(str[4], "긖??긣");
				strcpy(str[5], "긊?긦귺");
				strcpy(str[6], "궕귏궸궇");
				strcpy(str[7], "긒???긚?");
				strcpy(str[8], "깋긐긪귽?");
				strcpy(str[9], "긖?긣");
				strcpy(str[10], "궛?귔귏궥궫");
				strcpy(str[11], "긒??긚?");
				strcpy(str[12], "굟굧");
				strcpy(str[13], "괽굃");
				strcpy(str[14], "궛귔귏궥궫");
				strcpy(str[15], "*볙룒쁞*");
				strcpy(str[16], "*GUILD*");
				strcpy(str[17], "*Party*");
				strcpy(str[18], "Gamania");
				strcpy(str[19], "gamania");
				strcpy(str[20], "GAMANIA");
				strcpy(str[21], "eternalchaos");
				strcpy(str[22], "GM");
				strcpy(str[23], "gm");

				for (int i = 0; i < 24; i++)
				{
					if (strinc(m_NameBox.GetString(), str[i]) ) // || !stricmp(m_NameBox.GetString(), str[i]) )
					{
						Sleep(700);
						// IDS_CRECHA_USEING_NAEM : 그 이름은 이미 사용 중 입니다
						m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_USEING_NAEM));
						return;
					}
				}

				for (int i = 0; i < DENY_STRING_NUM; i++)
				{
					if (strinc(m_NameBox.GetString(), m_Deny_String[i]) ) // || stricmp(m_NameBox.GetString(), m_Deny_String[i]) )
					{
						Sleep(700);
						// IDS_CRECHA_USEING_NAEM : 그 이름은 이미 사용 중 입니다
						m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_USEING_NAEM));
						return;
					}
				}
			}
			else if (g_dwClientCountry == CTRY_DEU
					 || g_dwClientCountry == CTRY_DENG
					 || g_dwClientCountry == CTRY_ENG
					 || g_dwClientCountry == CTRY_MENG
					 || g_dwClientCountry == CTRY_FR)
			{
				char *str = NULL;
				str = m_NameBox.GetString();
				int len = strlen(str);

				for (int i=0; i < len; i++)
				{
					if (!isalnum(str[i]))
					{
						char msgStr[100] = "";
						char str2[2] = "";

						str2[0] = str[i];
						str2[1] = NULL;
						// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
						sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), str2);
						m_MsgPopUp.PutString(msgStr);
						return;
					}
				}
			}
			else	// for korea
			{
				char msgStr[100] = "";
				if((found_sp = ExamSpChar(m_NameBox.GetString())) != NULL)
				{
					// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
					sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), found_sp);
					m_MsgPopUp.PutString(msgStr);
					return;
				}

				if((found_sp = ExamSpCharOne(m_NameBox.GetString())) != NULL)
				{
					// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
					sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), found_sp);
					m_MsgPopUp.PutString(msgStr);
					return;
				}

				if( ( found_sp = Check2ByteChar(m_NameBox.GetString()) ) != NULL )
				{
					// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
					sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), found_sp);
					m_MsgPopUp.PutString(msgStr);
					return;
				}

				// nate 2006-03-17 : Proper List - 스트링 체크
				for (int i = 0; i < m_nProperListCount; i++)
				{
					strcpy(msgStr,m_NameBox.GetString() );
					strlwr(msgStr);	// 소문자로 변환

					if( Str2Str( msgStr, m_pszProperList[i] ) )
						//				if( strstr(msgStr,m_pszProperList[i]) )
					{
						Sleep(700);
						// IDS_CRECHA_UNABLE_STRING_02 : 캐릭터 이름에 "%s" 를 허용하지 않습니다
						sprintf(msgStr, G_STRING(IDS_CRECHA_UNABLE_STRING_02), m_pszProperList[i]);
						m_MsgPopUp.PutString(msgStr);
						return;
					}
				}

				if( CommonConfig::Instance()->GetCaptcha() == true )
				{
					m_captcha->_reset();
					m_captcha->_SetVisible(true);
					m_captcha->_SetDisable(false);
					return;
				}
			}

			int ret = 0;
			if( m_pSheet != NULL )
			{
				ret = m_pSheet->ConfirmExist(m_NameBox.GetString());
			}

			if( ret == -1 )
			{
				m_captcha->_SetVisible(false);
				m_captcha->_SetDisable(true);

				m_MsgPopUp.PutString((char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION));
				return;
			}
			else if( ret == 1 )
			{
				SAFE_DELETE_ARRAY(m_pSheet->m_TempSlot.m_Name);

				m_pSheet->m_TempSlot.m_Name = m_NameBox._StrDup();

				// 에이디아.
				if (g_bAidia)
					m_pSheet->m_TempSlot.m_Race = m_RaceBox.m_CurSel;
				else
					// 다른 부분에서 바꾸었으니까 정상적으로.
					m_pSheet->m_TempSlot.m_Race = m_RaceBox.m_CurSel;

				m_pSheet->m_TempSlot.m_Sex  = m_SexBox.m_CurSel;
				m_pSheet->m_TempSlot.m_Hair = m_HairBox.m_CurSel;

				m_NameBox.ClearText();
				m_NameBox.SetEnable(TRUE);

				// 여기가 완료되는 부분이다.
				if( m_pSVPage )
					m_pSVPage->Complete();
			}
			else
			{
				// IDS_CRECHA_USEING_NAEM : 그 이름은 이미 사용 중 입니다
				m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_USEING_NAEM));
				return;
			}
		}
		break;

	case GOMAKE:
		{
			int ret = 0;
			if( m_pSheet != NULL )
			{
				ret = m_pSheet->ConfirmExist(m_NameBox.GetString());
			}

			if( ret == -1 )
			{
				m_captcha->_SetVisible(false);
				m_captcha->_SetDisable(true);

				m_MsgPopUp.PutString((char*)G_STRING(IDS_UIMGR_BREAK_CONNECTION));
				return;
			}
			else if( ret == 1 )
			{
				SAFE_DELETE_ARRAY(m_pSheet->m_TempSlot.m_Name);

				m_pSheet->m_TempSlot.m_Name = m_NameBox._StrDup();

				if (g_bAidia)
					m_pSheet->m_TempSlot.m_Race = m_RaceBox.m_CurSel;
				else
					m_pSheet->m_TempSlot.m_Race = m_RaceBox.m_CurSel;

				m_pSheet->m_TempSlot.m_Sex  = m_SexBox.m_CurSel;
				m_pSheet->m_TempSlot.m_Hair = m_HairBox.m_CurSel;

				m_NameBox.ClearText();
				m_NameBox.SetEnable(TRUE);

				if( m_pSVPage )
					m_pSVPage->Complete();

				m_captcha->_SetVisible(false);
				m_captcha->_SetDisable(true);
			}
			else
			{
				m_captcha->_SetVisible(false);
				m_captcha->_SetDisable(true);

				m_MsgPopUp.PutString((char*)G_STRING(IDS_CRECHA_USEING_NAEM));
				return;
			}
		}
		break;
	}
}


void CCreateCharacterPage::OutoSetValue(int Race)
{
	if( !m_pSVPage )
		return;

	int Str = 0, Int = 0, Dex = 0, Con = 0, Cha = 0;

	// 더이상 못움직에 하는 "막대기들"의 각각의 위치를 담을 변수를 지정
	int StrBarMaxPos = 0, IntBarMaxPos = 0, DexBarMaxPos = 0, ConBarMaxPos = 0, ChaBarMaxPos = 0;

	// 각각의 힘에대한 최대치를 나타내주는 "BAR"의 위치를 담을 변수들
	int StrBarXpos = 0, IntBarXpos = 0, DexBarXpos = 0, ConBarXpos = 0, ChaBarXpos = 0;
	int StartPos	 = m_pSVPage->m_StrDrag.m_PosX;
	int Horizon_Size = m_pSVPage->m_StrDrag.m_Width;

	switch(Race)
	{
	case 0:
		Str = 30, Int = 5, Dex = 5,	Con = 30, Cha = 10;
		StrBarMaxPos = 30, IntBarMaxPos = 10, DexBarMaxPos = 10, ConBarMaxPos = 30, ChaBarMaxPos = 20;

		m_pSVPage->m_StrDrag.SetValues(5, 30, Str, 30);
		m_pSVPage->m_IntDrag.SetValues(5, 30, Int, 10);
		m_pSVPage->m_DexDrag.SetValues(5, 30, Dex, 10);
		m_pSVPage->m_ConDrag.SetValues(5, 30, Con, 30);
		m_pSVPage->m_ChaDrag.SetValues(5, 30, Cha, 20);
		break;

	case 1:
		Str = 15, Int = 30, Dex = 5, Con = 15, Cha = 15;
		StrBarMaxPos = 15, IntBarMaxPos = 30, DexBarMaxPos = 20, ConBarMaxPos = 15, ChaBarMaxPos = 20;

		m_pSVPage->m_StrDrag.SetValues(5, 30, Str, 15);
		m_pSVPage->m_IntDrag.SetValues(5, 30, Int, 30);
		m_pSVPage->m_DexDrag.SetValues(5, 30, Dex, 20);
		m_pSVPage->m_ConDrag.SetValues(5, 30, Con, 15);
		m_pSVPage->m_ChaDrag.SetValues(5, 30, Cha, 20);
		break;

	case 2:
		Str = 15, Int = 30, Dex = 5, Con = 15, Cha = 15;
		StrBarMaxPos = 15, IntBarMaxPos = 30, DexBarMaxPos = 20, ConBarMaxPos = 15, ChaBarMaxPos = 20;

		m_pSVPage->m_StrDrag.SetValues(5, 30, Str, 15);
		m_pSVPage->m_IntDrag.SetValues(5, 30, Int, 30);
		m_pSVPage->m_DexDrag.SetValues(5, 30, Dex, 20);
		m_pSVPage->m_ConDrag.SetValues(5, 30, Con, 15);
		m_pSVPage->m_ChaDrag.SetValues(5, 30, Cha, 20);
		break;

	case 3:
		Str = 15, Int = 5, Dex = 30, Con = 20, Cha = 10;
		StrBarMaxPos = 20, IntBarMaxPos = 20, DexBarMaxPos = 30, ConBarMaxPos = 20, ChaBarMaxPos = 20;

		m_pSVPage->m_StrDrag.SetValues(5, 30, Str, 20);
		m_pSVPage->m_IntDrag.SetValues(5, 30, Int, 20);
		m_pSVPage->m_DexDrag.SetValues(5, 30, Dex, 30);
		m_pSVPage->m_ConDrag.SetValues(5, 30, Con, 20);
		m_pSVPage->m_ChaDrag.SetValues(5, 30, Cha, 20);
		break;
	case 4:
		Str = 30, Int = 10, Dex = 10, Con = 25, Cha = 5;
		StrBarMaxPos = 30, IntBarMaxPos = 15, DexBarMaxPos = 15, ConBarMaxPos = 30, ChaBarMaxPos = 20;

		m_pSVPage->m_StrDrag.SetValues(5, 30, Str, 30);
		m_pSVPage->m_IntDrag.SetValues(5, 30, Int, 15);
		m_pSVPage->m_DexDrag.SetValues(5, 30, Dex, 15);
		m_pSVPage->m_ConDrag.SetValues(5, 30, Con, 30);
		m_pSVPage->m_ChaDrag.SetValues(5, 30, Cha, 20);
		break;

	case 5:
		Str = 30, Int = 10, Dex = 10, Con = 25, Cha = 5;
		StrBarMaxPos = 30, IntBarMaxPos = 15, DexBarMaxPos = 15, ConBarMaxPos = 30, ChaBarMaxPos = 20;

		m_pSVPage->m_StrDrag.SetValues(5, 30, Str, 30);
		m_pSVPage->m_IntDrag.SetValues(5, 30, Int, 15);
		m_pSVPage->m_DexDrag.SetValues(5, 30, Dex, 15);
		m_pSVPage->m_ConDrag.SetValues(5, 30, Con, 30);
		m_pSVPage->m_ChaDrag.SetValues(5, 30, Cha, 20);
		break;
	}

	// 각각의 힘에대한 최대치를 제한하는 막대기가 놓여질 위치를 계산한다
	StrBarXpos =StartPos + (Horizon_Size *(StrBarMaxPos-5)/(30-5)-(8/ 2)+10);
	IntBarXpos =StartPos + (Horizon_Size *(IntBarMaxPos-5)/(30-5)-(8/ 2)+10);
	DexBarXpos =StartPos + (Horizon_Size *(DexBarMaxPos-5)/(30-5)-(8/ 2)+10);
	ConBarXpos =StartPos + (Horizon_Size *(ConBarMaxPos-5)/(30-5)-(8/ 2)+10);
	ChaBarXpos =StartPos + (Horizon_Size *(ChaBarMaxPos-5)/(30-5)-(8/ 2)+10);


	m_pSVPage->SetCharitisics(Str, Int, Dex, Con, Cha);
	m_pSVPage->SetDragBarPos(StrBarXpos, IntBarXpos, DexBarXpos, ConBarXpos, ChaBarXpos);
}

//...........................................................

BOOL CCreateCharacterPage::CheckSpace(char* String)
{
	int len = strlen(String);
	for(int i=0; i<len; i++)
	{
		if(is_space(String[i]) == TRUE)
			return TRUE;
	}
	return FALSE;
}

void CCreateCharacterPage::SetRaceBox( BYTE byCanCharCreate )
{
	if( byCanCharCreate == 2 )
	{
		m_RaceBox.SetSize(NUM_RACE);
		m_RaceBox.SetString(0, (char*)G_STRING(IDS_CRECHA_BULKAN));
		m_RaceBox.SetString(1, (char*)G_STRING(IDS_CRECHA_KAI));
		m_RaceBox.SetString(2, (char*)G_STRING(IDS_CRECHA_ADI));
		m_RaceBox.SetString(3, (char*)G_STRING(IDS_CRECHA_HUNMAN));
		m_RaceBox.SetString(4, (char*)G_STRING(IDS_SCORE_FREEK));
		m_RaceBox.SetString(5, (char*)G_STRING(IDS_LHSTRING1893));
	}
	else if( byCanCharCreate == 1)
	{
		m_RaceBox.SetSize(5);
		m_RaceBox.SetString(0, (char*)G_STRING(IDS_CRECHA_BULKAN));
		m_RaceBox.SetString(1, (char*)G_STRING(IDS_CRECHA_KAI));
		m_RaceBox.SetString(2, (char*)G_STRING(IDS_CRECHA_ADI));
		m_RaceBox.SetString(3, (char*)G_STRING(IDS_CRECHA_HUNMAN));
		m_RaceBox.SetString(4, (char*)G_STRING(IDS_SCORE_FREEK));
	}
	else if( byCanCharCreate == 0 )
	{
		m_RaceBox.SetSize(4);
		m_RaceBox.SetString(0, (char*)G_STRING(IDS_CRECHA_BULKAN));
		m_RaceBox.SetString(1, (char*)G_STRING(IDS_CRECHA_KAI));
		m_RaceBox.SetString(2, (char*)G_STRING(IDS_CRECHA_ADI));
		m_RaceBox.SetString(3, (char*)G_STRING(IDS_CRECHA_HUNMAN));
	}
}

void CCreateCharacterPage::ReadProperList(char* szFileName)
{
	FILE* fp = NULL;
	int i = 0, j = 0;
	int nLen = 0;
	char szBuf[100] = "";

	fp = fopen(szFileName, "rt");
	if(!fp)
	{
		fp = fopen(szFileName, "rt");
		if(!fp)
		{
			MessageBox(NULL, szFileName, "Warning", MB_OK);
			exit(1);
		}
	}

	fscanf(fp, "%s", szBuf);
	nLen = strlen(szBuf);
	for(i=0; i<nLen; i++)
		szBuf[i] = szBuf[i]-1;

	m_nProperListCount = atoi(szBuf);

	m_pszProperList = new char*[m_nProperListCount];

	for( i = 0 ; i < m_nProperListCount ; i ++ )
	{
		m_pszProperList[i] = new char[16];

		fscanf(fp, "%s", szBuf);
		nLen = strlen(szBuf);

		for(j=0; j<nLen; j++)
			szBuf[j] = szBuf[j]-1;

		strcpy(m_pszProperList[i], szBuf);
	}
	fclose(fp);
}

HRESULT CCreateCharacterPage::Restore()
{
	HRESULT hr = S_OK;

	m_MsgPopUp.RestoreAll();
	m_NameBox.RestoreSurfaces();
	if( m_pSVPage )
		m_pSVPage->Restore();

	return hr;
}

char *spChar = "　！＇ㄱ，．／：；？＾＿｀｜￣、。·‥…¨〃­―∥＼∼´～ˇ˘˝˚˙¸˛¡¿ː" // ㄱ
			   "＂（）［］｛｝‘’“”〔〕〈〉《》" // ㄴ
			   "＋－＜＝＞×∠⌒∂≡≪≫∈⊂∪∧∨￢" // ㄷ
			   "＄％￦Ｆ′″Å℉㎕㎖㎗ℓ㎘㏄㎣㎤㎥㎦㎙㎚㎛㎜㎝㎞㎟㎠㎡㎢㏊㎍㎎㎏㏏㎈㎉㏈㎧㎨㎰㎱㎲㎳㎴㎵㎶㎷㎸㎹" // ㄹ
			   "㎀㎁㎂㎃㎄㎺㎻㎼㎽㎾㎿㎐㎑㎒㎓㎔㏀㏁㎊㎋㎌㏖㏅㎭㏛㎩㎪㎫㎬㏝㏐㏓㏃㏉㏜㏆"
			   "＃＆＊＠§" // ㅁ
			   "─│┐└├┬┤┴┼━┃┓┗┣┳┫┻╋┠┯┨┷┿┝┰┥┸╂┒┑┖┕┞┟┡┢┦┧┩┪┭┮┱┲┵┶"	// ㄹ
			   "┹┺┽┾╀╁╃╄╅╆╇╈╉╊"
			   "０１２３４５６７８９ⅰⅱⅲⅳⅴⅵⅶⅷⅸⅹⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩ" // ㅈ
			   "ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"	// ㅍ
			   "ΑΒΕΖΗΘΙΚΛΜΝΟΠΡΤΥΧαβεκμνορστυχω"	// ㅎ
			   "ĿŊıĳĸŀßŉ"	// ㄲ
			   "ぁあぃいいぅうぇえぉおかがきぎくぐけげこごさざしじすせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶ" // ㄸ
			   "ぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん"
			   "ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブ"	// ㅃ
			   "プヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶ";

char * spChar_tw = "죚죛죜죝죞죟죢죣죥????　、。·''""­―∥＼∼‘’“”〔〕〈〉《》「」『』【】±×÷"
				   "≠≤≥∞∴°′″℃Å￠￡￥♂♀∠⊥⌒∂∇≡≒§※☆★○●◎◇◆□■△▲▽▼→←↑↓↔〓≪"
				   "≫√∽∝∵∫∬∈∋⊆⊇⊂⊃∪∩∧∨￢?줐줒줓줔줕줖줗줙줚줛줜줝줞줟줠줡줢줣줤줥줦줧줨줩줪"
				   "줫??????줭줮줯줰줱줲줳줵줶줷줸줹줺줻줼줽줾줿쥀쥁쥂쥃쥄쥅쥆쥇????⇒⇔∀桁鹹合哈"
				   "˘˝˚˙¸˛¡0123456789▷▶♤♠♡♥♧♣⊙◈▣◐◑▒▤▥▨▧▦짷♨ㅚ☎☜☞¶†‡↕↗↙↖"
				   "↘♭♩♪♬㉿㈜№㏇™㏂㏘℡€®????????????????????????쥱쥲쥳쥵쥶"
				   "쥷쥸쥹쥺쥻쥽쥾쥿즀즁즂즃즄즅즆즇즊즋즍즎즏??????즑즒즓즔즕즖즗즚즜즞즟즠즡즢즣즤즥"
				   "즦즧즨즩즪즫즬즭즮????！＂＃＄％＆＇（）＊＋，－．／０１２３４５６７８９：；＜＝＞？"
				   "罕翰閑閒限韓割轄函含咸啣喊檻涵緘艦銜陷鹹合哈盒蛤閤闔陜亢伉姮嫦巷恒抗杭桁?⇒⇔∀航";


char * spChar_chi = "ㄱㄳㄵㄷㄹㄻㄽㄿㅁㅃㄲㄴㄶㄸㄺㄼㄾㅀㅂㅄㆃㅅㅇㅉㅋㅏㅑㅔㅓㅖㅘㆀㅆㅈㅊㅌㅎㅐㅒ"
					"ㅕㅗㅙㅚㅛㅜㅝㅞㅟㅢㅥㅨㅫㆁㅡㅤㅧㅪㅭㅠㅣㅦㅩㅬㅮㅯㅰㅱㅲㅳㅵㅷㅾㆂㅹㅺㅻㅼㅽ"
					"ㅴㅶㅸㅿ??????????｛｝[]⊆∈⊂∵∽±×∬⊇∝√【】『』''"";:←〓°≪"
					"┒┑┚，。／＄→′⊃÷≠≤《》？、∼≪≫√∽∵∫∬∈∋⊇⊂⊃∪∩∧∨￢＃＆＠￦＾"
					"＿￣";

//char * spChar_jpn = "?";
char * spChar_jpn = "`ⅰ∀ㄵ├ŁかЛ?갚껙뉘떱많본섹씩읒쩠컵판훔墾勁卦今錄桐虜妹膊覆?@";
char * only_tw_char = "~!@#$%^&*()_+|`-=\\{}[]:\";'<>?,./"
					  "￠￡€￥?ⓒ®™?§??¶≪≫€œ¿¡"; // 경고! \{는 \\{으로 추정
//char * only_tw_char = "~!@#$%^&*()_+|`-=\{}[]:\";'<>?,./"; // 경고! \{는 \\{으로 추정


char *etcChar(char *str)
{
	static char found_sp[2] = "";

	int len = strlen(str);
	int splen = strlen(only_tw_char);

	for (int pos = 0; pos < len; pos++)
	{
		int dd = (unsigned char) str[pos];

		if(dd> 127)
		{
			pos++;
			continue;
		}
		for(int i=0; i<splen; i++)
		{
			if(dd == (unsigned char) only_tw_char[i])
			{
				found_sp[0] = only_tw_char[i];
				found_sp[1] = '\0';
				return found_sp;
			}
		}
	}
	return NULL;
}

bool ExamNumOnly(char *str)
{
	bool num_only = true;
	int len = strlen(str);
	for (int pos = 0; pos < len; pos++)
	{
		if (str[pos] < '0' || str[pos] >= '9')
		{
			return false;
		}
	}
	return num_only;
}

char OneByteSpChar[] = "`~!@#$%^&*()-_=+\\|[{]};:'\",<.>/?";	// \\| 같은데...??
//char OneByteSpChar[] = "`~!@#$%^&*()-_=+\|[{]};:'\",<.>/?";



char *ExamSpCharOne(char* str)
{
	static char found_sp[2] = "";
	int len = strlen(str);
	int splen = sizeof(OneByteSpChar);
	for (int pos = 0; pos < len; pos++)
	{
		if ((unsigned char) str[pos] <= 127) // 1바이트
		{
			for(int i = 0; i < splen; ++i)
			{
				if (str[pos] == OneByteSpChar[i])
				{
					found_sp[0] = str[pos];
					found_sp[1] = NULL;
					return found_sp;
				}
			}
		}
		else pos++; //2바이트 문자
	}
	return NULL;
}

char *ExamSpChar(char *str)
{
	static char found_sp[3] = "";

	int len = strlen(str);
	int splen;
	if(g_dwLangType == 3 || g_dwClientCountry ==3)
	{
		spChar = spChar_tw;
		splen = strlen(spChar_tw);
	}
	else if(g_dwLangType == 4 || g_dwClientCountry ==4)
	{
		spChar = spChar_chi;
		splen = strlen(spChar_chi);
	}
	else if(g_dwLangType == 1 || g_dwClientCountry ==1)
	{
		spChar = spChar_jpn;
		splen = strlen(spChar_jpn);
		/*	}else if(g_dwLangType == 7 || g_dwClientCountry ==7)
			{
				return etcChar(str);
		*/
	}
	else
	{
		splen = strlen(spChar);
	}

	for (int pos = 0; pos < len; pos++)
	{
		if ((unsigned char) str[pos] > 127)
		{
			for (int sp_pos = 0; sp_pos < splen; sp_pos += 2)
			{
				if (!strncmp(str+pos, spChar+sp_pos, 2))
				{
					found_sp[0] = spChar[sp_pos];
					found_sp[1] = spChar[sp_pos+1];
					found_sp[2] = '\0';

					return found_sp;
				}
			}
			pos++;
		}
	}
	if(g_dwLangType == 3 || g_dwClientCountry ==3)
		return etcChar(str);	// 대만용에서는 ("~!@#$%^&*()_+|`-=\{}[]:\";'<>?,./") <--- 문자열두 사용할 수 엄따.
	else
		return NULL;
}



bool IsTowByte(char str)
{
	bool bTwobyte = false;

	// ASCII ..
	if((unsigned char)str  >= 0x20 && (unsigned char)str  <=0x7E)
		bTwobyte = false;
	// 2BYTE STRING (HANJA first byte / kata(JUN GAK) / hira)
	else if((unsigned char)str  >= 0x81 && (unsigned char)str  <=0x9F)
		bTwobyte = true;
	// 2BYTE STING (HANJA first byte)
	else if((unsigned char)str  >= 0xE0 && (unsigned char)str  <=0xEF)
		bTwobyte = true;
	// 1BYTE STRING
	else
		bTwobyte = false;

	return bTwobyte;
}


char *ExamSpChar_for_jpn(char *str, bool bKata)
{
	static char found_sp[3];
	found_sp[0] = '\0';

	int len = strlen(str);
	int splen = 0;

	spChar = spChar_jpn;
	splen = strlen(spChar_jpn);

	int	iStart, iEnd ;
	if( bKata )
	{
		iStart = 7;
		iEnd = splen;
	}
	else
	{
		iStart = 0;
		iEnd = 7;
	}

	bool input_str_byte = false;
	bool list_str_byte = false;
	int z = 0;
	for (int pos = 0; pos < len; pos++)
	{
		input_str_byte = IsTowByte(str[pos]);

		for (int sp_pos = iStart; sp_pos < iEnd; sp_pos++)
		{
			list_str_byte = IsTowByte(spChar[sp_pos]);

			if(input_str_byte == list_str_byte)
			{
				list_str_byte ? z = 2 : z = 1;

				if (!strncmp(str+pos, spChar+sp_pos, z))
				{
					if(!list_str_byte)
					{
						found_sp[0] = spChar[sp_pos];
						found_sp[1] = '\0';
					}
					else
					{
						found_sp[0] = spChar[sp_pos];
						found_sp[1] = spChar[sp_pos+1];
						found_sp[2] = '\0';
					}

					return found_sp;
				}
			}
			if(list_str_byte)
				sp_pos++;
		}
		if(input_str_byte)
			pos++;
	}
	return NULL;

}

char *ExamEscChar_for_jpn(char *str)
{
	static char found_sp[3];
	found_sp[0] = '\0';

	int len = strlen(str);

	for (int pos = 0; pos < len; pos++)
	{
		if (str[pos] == 0x5c)
			return "e";			// "e" means error
	}

	return NULL;
}

char * Check2ByteChar(char * str)
{
	static char found_sp[3];
	int len = strlen(str);

	for(int i=0; i<len; i++)
	{
		if( str[i] == -92 ) // 와성되지 않은 한
		{
			memcpy( found_sp, &str[i], sizeof(char)*2);
			found_sp[2] = NULL;
			return found_sp;
		}
		else if( str[i] < 0)
			i++;
	}
	return NULL;
}


BOOL Str2Str(char * str1, char * str2) // 이름 그냥 막 지었다 ㅡㅡ;;
{
	static char found_sp[3];
	int len = strlen(str1);

	if( !strncmp( str2, "쌍", 2) || ! strncmp(str2, "썅", 2))
	{
		int a = 0;
	}

	for(int i=0; i<len; i++)
	{
		if( str2[0] >= 0 ) // 영문이거나 1바이트 문자이면
		{
			if( !(str1[i] != -92 && str1[i] < 0))
			{
				if( !strncmp( &str1[i], str2, strlen(str2) ) )
					return TRUE;
			}
		}
		else
		{
			if(str1[i] != -92 && str1[i] < 0 )
			{

				if( !strncmp( &str1[i], str2, strlen(str2) ) )
					return  TRUE;

				i++;
			}
		}
	}
	return FALSE;
}