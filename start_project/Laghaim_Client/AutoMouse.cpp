#include "stdafx.h"
// AutoMouse.cpp: implementation of the CAutoMouse class.
//
//////////////////////////////////////////////////////////////////////


// nate 2004 - 3
// 숫자 확인 시스템
#include <time.h>

#include "headers.h"
#include "Nk2DFrame.h"
#include "basicbutton.h"
#include "textoutbox.h"
#include "TextBox.h"
#include "tcpipcon.h"
#include "AlphaPopUp.h"
#include "ControlBottom.h"
#include "g_stringmanager.h"

// nate 2004 - 3
// 숫자 확인 시스템
#include "3d_PosPoint.h"
#include "ImageDefine.h"

#include "AutoMouse.h"

// nate 2004 - 3
// 숫자 확인 시스템
#define IsSpace( c )	( c == 0x20 || ( c >= 0x09 && c <= 0x0D ) )

static DWORD back_color = 0x000fff;//0x000000;






//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoMouse::CAutoMouse()
	: m_bCurAct(FALSE)
	, m_CurTextView(0)
{
	memset(m_strhelpinfo, 0, sizeof(m_strhelpinfo));

	// nate 2004 - 3 : 숫자 확인 시스템
	m_pSurfaceNum = NULL;
	m_pBoard = NULL;
	m_byCheckCount = 0;
	ZeroMemory( m_nNum, sizeof( m_nNum ) );
	for( int i = 0 ; i < 4 ; i ++ )
		m_fRandom[ i ] = 0.0f;

	m_pSurfaceBack = NULL;
	m_pQuestionmark = NULL;
	m_pQuestionmark2 = NULL;
	m_pTextIn = NULL;
	m_pMunjaText = NULL;
	m_pBtn1 = NULL;
	m_pCloseBtn = NULL;
	m_pHelpInfo = NULL;
	m_bOnHelp = false;
	m_bActive = false;
	m_ftime = 0.000f;
	m_felapsedtime = 0.000f;
	ZeroMemory(m_dic_word,sizeof(m_dic_word));

	///숫자입력시스템보완
	m_nDecodeBit = 0; // 체크넘버의 암호화 비트 게임 실행시 랜덤하게 구해진다.
	ZeroMemory( m_nRealCheckNumber, sizeof(int)*NUM_OF_CHECKNUMBER ); // 게임 실행시 랜덤하게 구해지는 4자리 체크 넘버.( 위 m_nDecodeBit값으로 부터 암호화 되어 저장된다. )
	ZeroMemory( m_nConfirmNumber, sizeof(BYTE)*NUM_OF_CONFIRMNUMBER ); // 게임 실행시 랜덤하게 구해지는 4자리 체크 넘버.( 위 m_nDecodeBit값으로 부터 암호화 되어 저장된다. )
	m_bOneFailed = FALSE;

	m_Type = AUTOMOUSE_TYPE_NUMBER; ///QnA

	Init();
}

CAutoMouse::~CAutoMouse()
{
	DeleteRes();

	SAFE_DELETE( m_pSurfaceNum );
	SAFE_DELETE( m_pSurfaceBack );
	SAFE_DELETE( m_pQuestionmark );
	SAFE_DELETE( m_pQuestionmark2 );
	SAFE_DELETE( m_pCloseBtn);
	SAFE_DELETE( m_pBtn1);
	SAFE_DELETE( m_pHelpInfo );
	SAFE_DELETE( m_pMunjaText );
	SAFE_DELETE( m_pTextIn );
	SAFE_DELETE_ARRAY(m_pBoard);
}

void CAutoMouse::Init()
{
	// nate 2004 - 3 : 숫자 확인 시스템
	m_pBoard = new C3d_PosPoint[ 4 ];
	srand( (unsigned)time( NULL ) );

	SAFE_DELETE(m_pHelpInfo);
	m_pHelpInfo = new CAlphaPopUp;
	if( m_pHelpInfo )
		m_pHelpInfo->Init(14);

	SAFE_DELETE(m_pMunjaText);
	m_pMunjaText = new CTextOutBox();
	if( m_pMunjaText  )
		m_pMunjaText->Init(12, RGB(255, 210, 0),0, 0);

	//	TEXTIN
	SAFE_DELETE(m_pTextIn);
	m_pTextIn = new CTextBox();
	if( m_pTextIn )
	{
		m_pTextIn->Init(0, 0, 40, back_color, RGB(255, 255, 255 ));
	}

}
void CAutoMouse::LoadRes(int Type)
{
	if( !g_pNk2DFrame || !g_pDisplay )
		return;

	m_Type = Type;

	if( m_Type == AUTOMOUSE_TYPE_NUMBER )
	{
		SAFE_DELETE(m_pBtn1);
		m_pBtn1 = new CBasicButton();
		if( m_pBtn1 )
		{
			m_pBtn1->SetFileName("dictionary/btn_dic_ok");// 확인버튼 (추후 경로 수정)
			m_pBtn1->LoadRes();
		}

		if( m_pTextIn )
		{
			m_pTextIn->ClearText();
			m_pTextIn->SetfontSize(10);
			m_pTextIn->SetEnable(false);
			m_pTextIn->SetStringLength(10);
			m_pTextIn->SetText('\0');
		}

		if( m_pHelpInfo )
			m_pHelpInfo->LoadRes(GET_CLIENT_WIDTH(), GET_CLIENT_HEIGHT());

		// nate 2004 - 3 : 숫자 확인 시스템
		for( int i = 0 ; i < 4 ; i ++ )
		{
			if( m_pBoard )
				m_pBoard[ i ].LoadRes( 9, 9 );
		}

		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/dictionary/dic_back.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pQuestionmark, "interface/dictionary/dic_questionmark_normal.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pQuestionmark2, "interface/dictionary/dic_questionmark_over.bmp");

		// Back Surface 위치
		if( !m_pSurfaceBack )
			return;

		m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pSurfaceBack->GetWidth()/4 ;
		m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT() - (m_pSurfaceBack->GetHeight()*2) - 15;//0;

		// 서버에서 보내온 문자 위치
		if( m_pMunjaText )
			m_pMunjaText->SetPos(m_pSurfaceBack->Xpos+51, m_pSurfaceBack->Ypos+45);
		// 문자 입력 위치
		if( m_pTextIn )
		{
			m_pTextIn->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+63);
		}
		// Questionmark 위치
		if( m_pQuestionmark )
		{
			m_pQuestionmark->Xpos = m_pSurfaceBack->Xpos + m_pSurfaceBack->GetWidth() - m_pQuestionmark->GetWidth()-1; // 테스트 후 변경
			m_pQuestionmark->Ypos = m_pSurfaceBack->Ypos + 1;
		}
		if( m_pQuestionmark2 )
		{
			m_pQuestionmark2->Xpos = m_pQuestionmark->Xpos;
			m_pQuestionmark2->Ypos = m_pQuestionmark->Ypos;
		}

		// 확인버튼 위치
		if( m_pBtn1 )
			m_pBtn1->SetPosition(m_pQuestionmark->Xpos-23, (m_pQuestionmark->Ypos+m_pQuestionmark->GetHeight())+2);

		MakeCheckNumber(); // 실제 체크 넘버를 랜덤하게 뽑아낸다.(암호화도 한다.) (by 원석) ///숫자입력시스템보완
	}
	else if( m_Type == AUTOMOUSE_TYPE_QnA || m_Type == AUTOMOUSE_TYPE_QUIZ )
	{
		SAFE_DELETE(m_pBtn1);
		m_pBtn1 = new CBasicButton();
		if( m_pBtn1 )
		{
			m_pBtn1->SetFileName("dictionary/btn_dic_ok");// 확인버튼 (추후 경로 수정)
			m_pBtn1->LoadRes();
		}

		if( m_Type == AUTOMOUSE_TYPE_QUIZ ) // qna는 함부로 닫으면 안되므로 닫기 버튼을 로드하지 않는다.
		{
			SAFE_DELETE(m_pCloseBtn);
			m_pCloseBtn = new CBasicButton();
			if( m_pCloseBtn )
			{
				m_pCloseBtn->SetFileName("btn_close");// 확인버튼 (추후 경로 수정)
				m_pCloseBtn->LoadRes();
			}
		}

		if( m_pTextIn )
		{
			m_pTextIn->ClearText();
			m_pTextIn->SetfontSize(10);
			m_pTextIn->SetEnable(false);
			m_pTextIn->SetStringLength(15);
			m_pTextIn->SetText('\0');
		}

		if( m_pHelpInfo )
			m_pHelpInfo->LoadRes(GET_CLIENT_WIDTH(), GET_CLIENT_HEIGHT());

		if( m_Type == AUTOMOUSE_TYPE_QnA )
			g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/dictionary/qna_back.bmp"); // Back
		else if( m_Type == AUTOMOUSE_TYPE_QUIZ )
			g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/dictionary/QuizEvent_Back.bmp"); // Back

		// Back Surface 위치
		if( !m_pSurfaceBack )
			return;

		m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pSurfaceBack->GetWidth()/2 ;
		m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT()/2 - (m_pSurfaceBack->GetHeight()/2)+50;

		// 문자 입력 위치
		if( m_pTextIn )
			m_pTextIn->SetPos(m_pSurfaceBack->Xpos+31, m_pSurfaceBack->Ypos+193);

		// 확인버튼 위치
		if( m_pBtn1 )
			m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+174, m_pSurfaceBack->Ypos+191 );

		if( m_pCloseBtn )
			m_pCloseBtn->SetPosition(m_pSurfaceBack->Xpos+215, m_pSurfaceBack->Ypos+12 );
	}
}

BOOL CAutoMouse::IsInside(int x, int y)
{
	if( !m_pSurfaceBack )
		return FALSE;

	if (x >= m_pSurfaceBack->Xpos
			&& x < m_pSurfaceBack->Xpos + m_pSurfaceBack->GetWidth()
			&& y >= m_pSurfaceBack->Ypos
			&& y < m_pSurfaceBack->Ypos + m_pSurfaceBack->GetHeight() )
		return TRUE;

	return FALSE;
}

void CAutoMouse::DeleteRes()
{
	SAFE_DELETE( m_pSurfaceNum );
	SAFE_DELETE( m_pSurfaceBack );
	SAFE_DELETE( m_pQuestionmark );
	SAFE_DELETE( m_pQuestionmark2 );
	SAFE_DELETE( m_pSurfaceNum );
	SAFE_DELETE( m_pSurfaceBack );
	SAFE_DELETE( m_pQuestionmark );
	SAFE_DELETE( m_pQuestionmark2 );
	SAFE_DELETE_ARRAY(m_pBoard);
}

void CAutoMouse::Draw()
{
	if( !g_pDisplay || !m_pSurfaceBack )
		return;

	if( m_Type == AUTOMOUSE_TYPE_NUMBER )
	{
		SetRandomNum();

		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);
		int x, y;
		x = m_pSurfaceBack->Xpos; //전체화면에서 back의 x좌표
		y = m_pSurfaceBack->Ypos; //전체화면에서 back의 y좌표

		g_pDisplay->Blt(x, y, m_pSurfaceBack);

		if(m_bOnHelp)
			g_pDisplay->Blt(m_pQuestionmark2->Xpos, m_pQuestionmark2->Ypos, m_pQuestionmark2);
		else
			g_pDisplay->Blt(m_pQuestionmark->Xpos, m_pQuestionmark->Ypos, m_pQuestionmark);

		if( m_pTextIn )
			m_pTextIn->Draw();// 문자 입력

		if(strlen(m_dic_word) >= 1 )
		{
			if( m_pMunjaText )
				m_pMunjaText->SetString(m_dic_word);

			int add_x = ( timeGetTime() / 300 % 50 ) - 20;

			int i = 0;

			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// 출력할 보드의 각도를 랜덤하게
									EFF_AUTOMOUSE,					// 몇번 그림인가
									1,					// 그림을 나눌것인지
									(m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)/10 // 체크하는 숫자의 4번째자리. (서버에서 온 숫자중 3번째 자릿수를 인덱스 삼아 암호화를 풀어서 구함.)
								  );		// 몇번째 나눠진 그림인지
			++i;

			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// 출력할 보드의 각도를 랜덤하게
									EFF_AUTOMOUSE,					// 몇번 그림인가
									1,					// 그림을 나눌것인지
									m_nNum[3] // 체크하는 숫자의 3번째 자리, 서버에서 온 숫자를 사용.
								  );
			++i;


			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// 출력할 보드의 각도를 랜덤하게
									EFF_AUTOMOUSE,					// 몇번 그림인가
									1,					// 그림을 나눌것인지
									(m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)%10 // 체크하는 숫자의 2번째자리. (서버에서 온 숫자중 3번째 자릿수를 인덱스 삼아 암호화를 풀어서 구함.)
								  );
			++i;

			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// 출력할 보드의 각도를 랜덤하게
									EFF_AUTOMOUSE,					// 몇번 그림인가
									1,					// 그림을 나눌것인지
									m_nNum[1] // 체크하는 숫자의 1번째 자리, 서버에서 온 숫자를 사용.
								  );
			++i;

		}

		if( m_pBtn1 )
			m_pBtn1->DrawToBlt(); // 확인버튼

		// helpinfo
		if( m_bOnHelp && m_pHelpInfo )
		{
			// HelpInfo 내용 ::추후  dll을 이용하거나 LoadResString()함수를 사용하기로함
			CMultiText multiText;
			if( m_pHelpInfo->GetIndex() != -100)
			{
				//-- IDS_USER_CHECK1 : 본인이 실제 접속해 있는지,
				sprintf(m_strhelpinfo,G_STRING(IDS_USER_CHECK1));
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_USER_CHECK2 : 확인하기 위한 시스템입니다.
				sprintf(m_strhelpinfo,G_STRING(IDS_USER_CHECK2));
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_INSERT_NUM10_1 : 10분 안에 숫자를 입력해주세요.
				sprintf(m_strhelpinfo,G_STRING(IDS_INSERT_NUM10_1));
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_INSERT_NUM10_2 : 올바른 숫자를 입력하면 공격력이 10분간
				sprintf(m_strhelpinfo,G_STRING(IDS_INSERT_NUM10_2)); //-- IDS_INSERT_NUM10_2
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_UP20 : 20 %% 상승합니다.
				sprintf(m_strhelpinfo,G_STRING(IDS_UP20)); //-- IDS_UP20
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));

				m_pHelpInfo->PutString(&multiText);
				m_pHelpInfo->SetIndex(-100);
			}
			m_pHelpInfo->Draw(point.x,point.y); // 좌표 수정
		}
	}
	else if( m_Type == AUTOMOUSE_TYPE_QnA || m_Type == AUTOMOUSE_TYPE_QUIZ )
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);
		int x, y;
		x = m_pSurfaceBack->Xpos; //전체화면에서 back의 x좌표
		y = m_pSurfaceBack->Ypos; //전체화면에서 back의 y좌표

		g_pDisplay->Blt(x, y, m_pSurfaceBack);

		if( m_pHelpInfo && m_pHelpInfo->GetSurface() )
		{
			int half_size = m_pHelpInfo->GetSurWidth() / 2;
			g_pDisplay->Blt( x+126-half_size, y+57, m_pHelpInfo->GetSurface() );
		}

		if( m_pTextIn )
			m_pTextIn->Draw();// 문자 입력

		if( m_pBtn1 )
			m_pBtn1->DrawToBlt(); // 확인버튼

		if( m_pCloseBtn ) // qna는 로드가 안되므로 안 찍힌다.
			m_pCloseBtn->DrawToBlt(); // 닫기.
	}
}

LRESULT CAutoMouse::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bDrag = FALSE; ///07-04-12 숫자입력도 드래그 가능하게 수정
	static int DistX = 0, DistY = 0; // 드래그시 클릭한 곳과 마우스 좌표의 차이 수치. ///07-04-12 숫자입력도 드래그 가능하게 수정
	int x = 0, y = 0;
	if (g_pNk2DFrame
			&& g_pNk2DFrame->GetControlBottom()
			&& g_pNk2DFrame->GetControlBottom()->m_InputBox.GetEnable()
			&& m_pTextIn )
		m_pTextIn->SetEnable(FALSE);

	// 타입별 메세지 프로시져는 복잡하니까 if~else 말고 아예 함수를 가르자.
	if( m_Type == AUTOMOUSE_TYPE_QnA || m_Type == AUTOMOUSE_TYPE_QUIZ )
		return QnAMsgProc( hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))//영역을 벗어남
			return 0;
		else if( !m_pBtn1->IsInside(x,y) && !m_pTextIn->IsInside(x,y) ) // 영역안에서 다운이면...///07-04-12 숫자입력도 드래그 가능하게 수정
		{
			bDrag = TRUE;

			DistX = m_pSurfaceBack->Xpos - x;
			DistY = m_pSurfaceBack->Ypos - y;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pTextIn )
			m_pTextIn->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pCloseBtn )
			m_pCloseBtn->MsgProc(hWnd, msg, wParam, lParam);

		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 어디서 땟든 드래그 플래그는 풀어준다. ///07-04-12 숫자입력도 드래그 가능하게 수정
		bDrag = FALSE;

		if(!IsInside(x, y))//영역을 벗어남
			return 0;

		if(!m_pBtn1)
			return 0;

		m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn1->GetState() == BTN_ACTION) 			//서버로 메시지 전송
		{
			char *buf;
			buf = m_pTextIn->GetString();// or buf = m_pTextIn->GetStrBuf();

//============================================================================================
			// nate 2004 - 3
			// 숫자 확인 시스템
			// 사용자가 입력한 숫자값에서 공백 제거
			int nSize = sizeof( buf ) / sizeof( char );
			char arStr[ 256 ] = {0,};

			int i = 0;
			while( *buf )
			{
				if( !IsSpace( *buf ) )
					arStr[ i++ ] = *buf;

				buf++;
			}

			SendConfirm( arStr ); // 유저가 친 숫자로부터 패킷을 만들어서 보낸다. ///숫자입력시스템보완
			m_bOneFailed = FALSE; // 실패시 한번만 다시 보내보도록 셋팅. ///숫자입력시스템보완

			m_pBtn1->SetState(BTN_NORMAL);
			return 1;
		}

		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bDrag ) // 드래그 중이면... ///07-04-12 숫자입력도 드래그 가능하게 수정
		{
			m_pSurfaceBack->Xpos = x + DistX;
			m_pSurfaceBack->Ypos = y + DistY;

			// 화면 넘어가면 보정.
			if( m_pSurfaceBack->Xpos < 0 )
				m_pSurfaceBack->Xpos = 0;
			if( m_pSurfaceBack->Ypos < 0 )
				m_pSurfaceBack->Ypos = 0;
			if( m_pSurfaceBack->Xpos > GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth() )
				m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth();
			if( m_pSurfaceBack->Ypos > GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
				m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();


			// 서버에서 보내온 문자 위치
			if( m_pMunjaText )
				m_pMunjaText->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+65);
			// 문자 입력 위치
			if( m_pTextIn )
				m_pTextIn->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+65);

			// 확인버튼 위치
			if( m_pBtn1 )
				m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+152, m_pSurfaceBack->Ypos+63 );

			m_pQuestionmark->Xpos = m_pSurfaceBack->Xpos + m_pSurfaceBack->GetWidth() - m_pQuestionmark->GetWidth()-1; // 테스트 후 변경
			m_pQuestionmark->Ypos = m_pSurfaceBack->Ypos + 1;
			m_pQuestionmark2->Xpos = m_pQuestionmark->Xpos;
			m_pQuestionmark2->Ypos = m_pQuestionmark->Ypos;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)	// 확인 버튼
		{
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn1->IsInside(x, y))
				m_pBtn1->SetState(BTN_ON);
		}

		if((m_pQuestionmark
				&& m_pQuestionmark->Xpos <= x
				&& x <= m_pQuestionmark->Xpos+m_pQuestionmark->GetWidth())
				&& m_pQuestionmark->Ypos <= y
				&& y <= m_pQuestionmark->Ypos+m_pQuestionmark->GetHeight() )
			m_bOnHelp  = true;
		else
			m_bOnHelp = false;

		break;

	case WM_KEYDOWN:

		if(wParam ==VK_RETURN)
		{
			char *buf;
			SetTextInFocus();
			buf = m_pTextIn->GetString();
//============================================================================================
			// nate 2004 - 3 : 숫자 확인 시스템 - 사용자가 입력한 숫자값에서 공백 제거
			int nSize = sizeof( buf ) / sizeof( char );
			char arStr[ 256 ] = {0,};

			int i = 0;
			while( *buf )
			{
				if( !IsSpace( *buf ) )
					arStr[ i++ ] = *buf;

				buf++;
			}

			SendConfirm( arStr ); // 유저가 친 숫자로부터 패킷을 만들어서 보낸다. ///숫자입력시스템보완
			m_bOneFailed = FALSE; // 실패시 한번만 다시 보내보도록 셋팅. ///숫자입력시스템보완

			return 1;
		}

		if(wParam ==VK_TAB  )//Focus를 채팅 창으로 이동
		{
			if(m_pTextIn && m_pTextIn->GetEnable())
			{
				m_pTextIn->SetEnable(false);
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
					g_pNk2DFrame->GetControlBottom()->m_InputBox.SetEnable(TRUE);
				if( pCMyApp )
					pCMyApp->RestoreImmStatus();
			}
			else
				SetTextInFocus();

			return 1;
		}
		break;
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_pTextIn && m_pTextIn->GetEnable() ) ///07-04-12 포커스가 있을때만 받도록 수정
			return m_pTextIn->MsgProc(hWnd, msg, wParam, lParam); ///07-04-12 리턴값도 수정.(사실 이거 무조건 0리턴이라 좀 문제가 있다)
		break;
	}
	return 0;
}

LRESULT CAutoMouse::QnAMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = 0, y = 0;
	char strTemp[255];
	static BOOL bDrag = FALSE;
	static int DistX = 0, DistY = 0; // 드래그시 클릭한 곳과 마우스 좌표의 차이 수치.

	if (g_pNk2DFrame
			&& g_pNk2DFrame->GetControlBottom()
			&& g_pNk2DFrame->GetControlBottom()->m_InputBox.GetEnable()
			&& m_pTextIn )
		m_pTextIn->SetEnable(FALSE);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))//영역을 벗어남
			return 0;
		else if( !m_pBtn1->IsInside(x,y) && !m_pTextIn->IsInside(x,y) ) // 영역안에서 다운이면...
		{
			bDrag = TRUE;

			DistX = m_pSurfaceBack->Xpos - x;
			DistY = m_pSurfaceBack->Ypos - y;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
		if( m_pTextIn )
			m_pTextIn->MsgProc(hWnd, msg, wParam, lParam);
		if(m_pCloseBtn )
			m_pCloseBtn->MsgProc(hWnd, msg, wParam, lParam);

		return 1;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		// 어디서 땟든 드래그 플래그는 풀어준다.
		bDrag = FALSE;

		if(!IsInside(x, y))//영역을 벗어남
			return 0;

		if(!m_pBtn1)
			return 0;

		m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn1->GetState() == BTN_ACTION && strlen(m_pTextIn->GetStrBuf()) )			//서버로 메시지 전송
		{
			if( m_Type == AUTOMOUSE_TYPE_QnA )
				sprintf( strTemp, "qna %s\n", m_pTextIn->GetStrBuf() );
			else if( m_Type == AUTOMOUSE_TYPE_QUIZ )
				sprintf( strTemp, "quiz %s\n", m_pTextIn->GetStrBuf() );

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( strTemp );

			m_pBtn1->SetState(BTN_NORMAL);

			g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // 창을 닫는다.

			return 1;
		}

		if( m_pCloseBtn )
		{
			m_pCloseBtn->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pCloseBtn->GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // 창을 닫는다.
			}
		}

		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bDrag ) // 드래그 중이면...
		{
			m_pSurfaceBack->Xpos = x + DistX;
			m_pSurfaceBack->Ypos = y + DistY;

			// 화면 넘어가면 보정.
			if( m_pSurfaceBack->Xpos < 0 )
				m_pSurfaceBack->Xpos = 0;
			if( m_pSurfaceBack->Ypos < 0 )
				m_pSurfaceBack->Ypos = 0;
			if( m_pSurfaceBack->Xpos > GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth() )
				m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth();
			if( m_pSurfaceBack->Ypos > GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
				m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();

			// 문자 입력 위치
			if( m_pTextIn )
				m_pTextIn->SetPos(m_pSurfaceBack->Xpos+31, m_pSurfaceBack->Ypos+193);

			// 확인버튼 위치
			if( m_pBtn1 )
				m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+174, m_pSurfaceBack->Ypos+191 );

			if( m_pCloseBtn )
				m_pCloseBtn->SetPosition(m_pSurfaceBack->Xpos+215, m_pSurfaceBack->Ypos+12 );
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)	// 확인 버튼
		{
			m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);
			if(m_pBtn1->IsInside(x, y))
				m_pBtn1->SetState(BTN_ON);
		}

		break;

	case WM_KEYDOWN:

		if( wParam ==VK_RETURN && m_pTextIn->GetEnable() && strlen(m_pTextIn->GetStrBuf()) )
		{
			if( m_Type == AUTOMOUSE_TYPE_QnA )
				sprintf( strTemp, "qna %s\n", m_pTextIn->GetStrBuf() );
			else if( m_Type == AUTOMOUSE_TYPE_QUIZ )
				sprintf( strTemp, "quiz %s\n", m_pTextIn->GetStrBuf() );

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( strTemp );

			g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // 창을 닫는다.

			return 1;
		}

		if(wParam ==VK_TAB  )//Focus를 채팅 창으로 이동
		{
			if(m_pTextIn && m_pTextIn->GetEnable())
			{
				m_pTextIn->SetEnable(false);
				if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
					g_pNk2DFrame->GetControlBottom()->m_InputBox.SetEnable(TRUE);
				if( pCMyApp )
					pCMyApp->RestoreImmStatus();
			}
			else
				SetTextInFocus();

			return 1;
		}
//		break;
	case WM_CHAR:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_NOTIFY:
		if( m_pTextIn && m_pTextIn->GetEnable() ) ///07-04-12 포커스가 있을때만 받도록 수정
			return m_pTextIn->MsgProc(hWnd, msg, wParam, lParam); ///07-04-12 리턴값도 수정.(사실 이거 무조건 0리턴이라 좀 문제가 있다)
		break;
	}
	return 0;
}


void CAutoMouse::AddWord(char *w1,char* w2,int flag,BOOL bAct)
{
	char a;
	int i;
	// 두단어 입력
	if(strlen(w1)>=1)
	{
		// 서버로 받은 문자를 숫자로 변환
		for( i = 0 ; i < 2 ; i ++ )
		{
			strncpy( &a, w1 + i, 1 );
			m_nNum[ i ] = atoi( &a );
		}
		strcpy(m_dic_word,w1);
	}

	if(strlen(w2)>=1)
	{
		// 서버로 받은 문자를 숫자로 변환
		for( i = 0 ; i < 2 ; i ++ )
		{
			strncpy( &a, w2 + i, 1 );
			m_nNum[ i + 2 ] = atoi( &a );
		}
		strcat(m_dic_word,w2);
	}

	m_CurTextView = flag;
	m_bCurAct = bAct;
	m_ftime = timeGetTime();
	m_bActive = true;
	SetTextInFocus();
}

void CAutoMouse::SetTextInFocus()
{
	if (m_pTextIn && m_pTextIn->GetEnable() == FALSE)
	{
		if( g_pNk2DFrame && g_pNk2DFrame->GetControlBottom() )
			g_pNk2DFrame->GetControlBottom()->m_InputBox.SetEnable(FALSE);
		m_pTextIn->SetEnable(TRUE);
	}
}

// nate 2004 - 3
// 숫자 확인 시스템
void CAutoMouse::SetRandomNum()
{
	for( int i = 0 ; i < 4 ; i ++ )
		m_fRandom[ i ] = ( ( rand() % 10 ) * 0.1f ) - 0.5f;	// 회전각
}

////////////////////////////////////////////////////////////////////////////////////////
// 수정자 : 이원석
// 수정일 : 05-04-08
// 수정내용 : 숫자입력시스템 보완 처리. ///숫자입력시스템보완
void CAutoMouse::MakeCheckNumber() // 게임 시작시 실행. 체크넘버와 암호화 비트를 랜덤값으로 구해준다.
{
	m_nDecodeBit = rand(); // 체크넘버의 암호화 비트 게임 실행시 랜덤하게 구해진다.

	for( int i = 0 ; i < NUM_OF_CHECKNUMBER ; ++i )
	{
		m_nRealCheckNumber[i] = rand()%99; // 게임 실행시 랜덤하게 구해지는 4자리 체크 넘버 중 1, 3위치의 두개 .( 위 m_nDecodeBit값으로 부터 암호화 되어 저장된다. )
		m_nRealCheckNumber[i] ^= m_nDecodeBit; // 암호화 처리.
	}
}

void CAutoMouse::SetConfirmNumber( int Arg1, int Arg2 ) // 서버로부터 받은 번호로부터 인증번호 5개를 산출한다.
{
	m_nConfirmNumber[0] = Arg1 / 100; // 4자리 수중 앞 2자리
	m_nConfirmNumber[1] = Arg1 % 100; // 4자리 수중 뒤 2자리
	m_nConfirmNumber[2] = Arg2 / 10000; // 6자리 수중 앞 2자리
	m_nConfirmNumber[3] = (Arg2 % 10000) / 100; // 6자리 수중 가운데 2자리
	m_nConfirmNumber[4] = Arg2 % 100; // 6자리 수중 마지막 2자리
}

void CAutoMouse::SendConfirm( char *InputStr ) // 인증번호를 계산해서 보낸다.
{
	char tmp[200];

	// 인증번호 5개를 이 타이밍에 클라이언트에서 계산해서 구해낸다.
	m_nConfirmNumber[5] = ( m_nConfirmNumber[0] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[6] = ( m_nConfirmNumber[1] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[7] = ( m_nConfirmNumber[2] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[8] = ( m_nConfirmNumber[3] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[9] = ( m_nConfirmNumber[4] * (pCMyApp->m_nCurHour+1) ) % 100;

	// 클라이언트 자체 체크.
	if( strlen(InputStr)!=4 || (atoi(InputStr)/1000) != (m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)/10 // 4번째자리.
			|| (atoi(InputStr)%100)/10 != (m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)%10 // 2번째 자리.
	  )
	{
		// 실패했으면 틀린 인증코드를 보내준다.
		sprintf( tmp, "autock %d\n"
				 , ((m_nNum[2]*m_nNum[1])%10*100000000)+(m_nNum[3]*10000000)+(m_nNum[1]*1000000) // 앞 4자리 (m_nNum[0]*m_nNum[1] 이 두자리, m_nNum[0], m_nNum[1] 이 한자리씩)
				 + (m_nConfirmNumber[ ( (g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+2) )%10 ] * ( ((g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+1))%10) * 1000) // 가운데 3자리 ( 인증코드[인증인덱스]*인증인덱스 ) ( 인증인덱스 = (자신의 캐릭터 인덱스 끝자리+(시간값+1))%10  )
				 + (m_nNum[2]*100)+(m_nNum[1]*10)+(g_pRoh->m_nCharIndex%10) // 마지막 3자리 (m_nNum[2], m_nNum[3] 이 한자리씩, 자신의 캐릭터 인덱스 끝자리)
			   );

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage( tmp );

		return;
	}

	// 인증번호의 생성은 해커가 최대한 알아보기 힘들게 따로 변수를 사용하지 않는다.
	sprintf( tmp, "autock %d\n"
			 , ( ( (m_nNum[0]*(atoi(InputStr)%10) )%10*100000000)+(m_nNum[0]*10000000)+((atoi(InputStr)%10)*1000000) ) // 앞 4자리 (m_nNum[0]*m_nNum[1] 이 두자리, m_nNum[0], m_nNum[1] 이 한자리씩)
			 + (m_nConfirmNumber[ ( (g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+1) )%10 ] * ( ((g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+1))%10) * 1000) // 가운데 3자리 ( 인증코드[인증인덱스]*인증인덱스 ) ( 인증인덱스 = (자신의 캐릭터 인덱스 끝자리+(시간값+1))%10  )
			 + (m_nNum[2]*100)+(( (atoi(InputStr)%1000)/100 )*10)+(g_pRoh->m_nCharIndex%10) // 마지막 3자리 (m_nNum[2], m_nNum[3] 이 한자리씩, 자신의 캐릭터 인덱스 끝자리)
		   );

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}
