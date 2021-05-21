#include "stdafx.h"
// AutoMouse.cpp: implementation of the CAutoMouse class.
//
//////////////////////////////////////////////////////////////////////


// nate 2004 - 3
// ���� Ȯ�� �ý���
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
// ���� Ȯ�� �ý���
#include "3d_PosPoint.h"
#include "ImageDefine.h"

#include "AutoMouse.h"

// nate 2004 - 3
// ���� Ȯ�� �ý���
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

	// nate 2004 - 3 : ���� Ȯ�� �ý���
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

	///�����Է½ý��ۺ���
	m_nDecodeBit = 0; // üũ�ѹ��� ��ȣȭ ��Ʈ ���� ����� �����ϰ� ��������.
	ZeroMemory( m_nRealCheckNumber, sizeof(int)*NUM_OF_CHECKNUMBER ); // ���� ����� �����ϰ� �������� 4�ڸ� üũ �ѹ�.( �� m_nDecodeBit������ ���� ��ȣȭ �Ǿ� ����ȴ�. )
	ZeroMemory( m_nConfirmNumber, sizeof(BYTE)*NUM_OF_CONFIRMNUMBER ); // ���� ����� �����ϰ� �������� 4�ڸ� üũ �ѹ�.( �� m_nDecodeBit������ ���� ��ȣȭ �Ǿ� ����ȴ�. )
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
	// nate 2004 - 3 : ���� Ȯ�� �ý���
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
			m_pBtn1->SetFileName("dictionary/btn_dic_ok");// Ȯ�ι�ư (���� ��� ����)
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

		// nate 2004 - 3 : ���� Ȯ�� �ý���
		for( int i = 0 ; i < 4 ; i ++ )
		{
			if( m_pBoard )
				m_pBoard[ i ].LoadRes( 9, 9 );
		}

		g_pDisplay->CreateSurfaceFromBitmap(&m_pSurfaceBack, "interface/dictionary/dic_back.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pQuestionmark, "interface/dictionary/dic_questionmark_normal.bmp");
		g_pDisplay->CreateSurfaceFromBitmap(&m_pQuestionmark2, "interface/dictionary/dic_questionmark_over.bmp");

		// Back Surface ��ġ
		if( !m_pSurfaceBack )
			return;

		m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pSurfaceBack->GetWidth()/4 ;
		m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT() - (m_pSurfaceBack->GetHeight()*2) - 15;//0;

		// �������� ������ ���� ��ġ
		if( m_pMunjaText )
			m_pMunjaText->SetPos(m_pSurfaceBack->Xpos+51, m_pSurfaceBack->Ypos+45);
		// ���� �Է� ��ġ
		if( m_pTextIn )
		{
			m_pTextIn->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+63);
		}
		// Questionmark ��ġ
		if( m_pQuestionmark )
		{
			m_pQuestionmark->Xpos = m_pSurfaceBack->Xpos + m_pSurfaceBack->GetWidth() - m_pQuestionmark->GetWidth()-1; // �׽�Ʈ �� ����
			m_pQuestionmark->Ypos = m_pSurfaceBack->Ypos + 1;
		}
		if( m_pQuestionmark2 )
		{
			m_pQuestionmark2->Xpos = m_pQuestionmark->Xpos;
			m_pQuestionmark2->Ypos = m_pQuestionmark->Ypos;
		}

		// Ȯ�ι�ư ��ġ
		if( m_pBtn1 )
			m_pBtn1->SetPosition(m_pQuestionmark->Xpos-23, (m_pQuestionmark->Ypos+m_pQuestionmark->GetHeight())+2);

		MakeCheckNumber(); // ���� üũ �ѹ��� �����ϰ� �̾Ƴ���.(��ȣȭ�� �Ѵ�.) (by ����) ///�����Է½ý��ۺ���
	}
	else if( m_Type == AUTOMOUSE_TYPE_QnA || m_Type == AUTOMOUSE_TYPE_QUIZ )
	{
		SAFE_DELETE(m_pBtn1);
		m_pBtn1 = new CBasicButton();
		if( m_pBtn1 )
		{
			m_pBtn1->SetFileName("dictionary/btn_dic_ok");// Ȯ�ι�ư (���� ��� ����)
			m_pBtn1->LoadRes();
		}

		if( m_Type == AUTOMOUSE_TYPE_QUIZ ) // qna�� �Ժη� ������ �ȵǹǷ� �ݱ� ��ư�� �ε����� �ʴ´�.
		{
			SAFE_DELETE(m_pCloseBtn);
			m_pCloseBtn = new CBasicButton();
			if( m_pCloseBtn )
			{
				m_pCloseBtn->SetFileName("btn_close");// Ȯ�ι�ư (���� ��� ����)
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

		// Back Surface ��ġ
		if( !m_pSurfaceBack )
			return;

		m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH()/2 - m_pSurfaceBack->GetWidth()/2 ;
		m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT()/2 - (m_pSurfaceBack->GetHeight()/2)+50;

		// ���� �Է� ��ġ
		if( m_pTextIn )
			m_pTextIn->SetPos(m_pSurfaceBack->Xpos+31, m_pSurfaceBack->Ypos+193);

		// Ȯ�ι�ư ��ġ
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
		x = m_pSurfaceBack->Xpos; //��üȭ�鿡�� back�� x��ǥ
		y = m_pSurfaceBack->Ypos; //��üȭ�鿡�� back�� y��ǥ

		g_pDisplay->Blt(x, y, m_pSurfaceBack);

		if(m_bOnHelp)
			g_pDisplay->Blt(m_pQuestionmark2->Xpos, m_pQuestionmark2->Ypos, m_pQuestionmark2);
		else
			g_pDisplay->Blt(m_pQuestionmark->Xpos, m_pQuestionmark->Ypos, m_pQuestionmark);

		if( m_pTextIn )
			m_pTextIn->Draw();// ���� �Է�

		if(strlen(m_dic_word) >= 1 )
		{
			if( m_pMunjaText )
				m_pMunjaText->SetString(m_dic_word);

			int add_x = ( timeGetTime() / 300 % 50 ) - 20;

			int i = 0;

			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// ����� ������ ������ �����ϰ�
									EFF_AUTOMOUSE,					// ��� �׸��ΰ�
									1,					// �׸��� ����������
									(m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)/10 // üũ�ϴ� ������ 4��°�ڸ�. (�������� �� ������ 3��° �ڸ����� �ε��� ��� ��ȣȭ�� Ǯ� ����.)
								  );		// ���° ������ �׸�����
			++i;

			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// ����� ������ ������ �����ϰ�
									EFF_AUTOMOUSE,					// ��� �׸��ΰ�
									1,					// �׸��� ����������
									m_nNum[3] // üũ�ϴ� ������ 3��° �ڸ�, �������� �� ���ڸ� ���.
								  );
			++i;


			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// ����� ������ ������ �����ϰ�
									EFF_AUTOMOUSE,					// ��� �׸��ΰ�
									1,					// �׸��� ����������
									(m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)%10 // üũ�ϴ� ������ 2��°�ڸ�. (�������� �� ������ 3��° �ڸ����� �ε��� ��� ��ȣȭ�� Ǯ� ����.)
								  );
			++i;

			m_pBoard[ i ].RenderOj( m_pSurfaceBack->Xpos + 51 + i * 20 + add_x,
									m_pSurfaceBack->Ypos + 50,
									m_fRandom[ i ],		// ����� ������ ������ �����ϰ�
									EFF_AUTOMOUSE,					// ��� �׸��ΰ�
									1,					// �׸��� ����������
									m_nNum[1] // üũ�ϴ� ������ 1��° �ڸ�, �������� �� ���ڸ� ���.
								  );
			++i;

		}

		if( m_pBtn1 )
			m_pBtn1->DrawToBlt(); // Ȯ�ι�ư

		// helpinfo
		if( m_bOnHelp && m_pHelpInfo )
		{
			// HelpInfo ���� ::����  dll�� �̿��ϰų� LoadResString()�Լ��� ����ϱ����
			CMultiText multiText;
			if( m_pHelpInfo->GetIndex() != -100)
			{
				//-- IDS_USER_CHECK1 : ������ ���� ������ �ִ���,
				sprintf(m_strhelpinfo,G_STRING(IDS_USER_CHECK1));
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_USER_CHECK2 : Ȯ���ϱ� ���� �ý����Դϴ�.
				sprintf(m_strhelpinfo,G_STRING(IDS_USER_CHECK2));
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_INSERT_NUM10_1 : 10�� �ȿ� ���ڸ� �Է����ּ���.
				sprintf(m_strhelpinfo,G_STRING(IDS_INSERT_NUM10_1));
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_INSERT_NUM10_2 : �ùٸ� ���ڸ� �Է��ϸ� ���ݷ��� 10�а�
				sprintf(m_strhelpinfo,G_STRING(IDS_INSERT_NUM10_2)); //-- IDS_INSERT_NUM10_2
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));
				//	m_pHelpInfo->PutString(&multiText);
				ZeroMemory(m_strhelpinfo,sizeof(m_strhelpinfo));

				//-- IDS_UP20 : 20 %% ����մϴ�.
				sprintf(m_strhelpinfo,G_STRING(IDS_UP20)); //-- IDS_UP20
				multiText.AddString(m_strhelpinfo, RGB(255, 255, 255));

				m_pHelpInfo->PutString(&multiText);
				m_pHelpInfo->SetIndex(-100);
			}
			m_pHelpInfo->Draw(point.x,point.y); // ��ǥ ����
		}
	}
	else if( m_Type == AUTOMOUSE_TYPE_QnA || m_Type == AUTOMOUSE_TYPE_QUIZ )
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_pDisplay->GetHWnd(), &point);
		int x, y;
		x = m_pSurfaceBack->Xpos; //��üȭ�鿡�� back�� x��ǥ
		y = m_pSurfaceBack->Ypos; //��üȭ�鿡�� back�� y��ǥ

		g_pDisplay->Blt(x, y, m_pSurfaceBack);

		if( m_pHelpInfo && m_pHelpInfo->GetSurface() )
		{
			int half_size = m_pHelpInfo->GetSurWidth() / 2;
			g_pDisplay->Blt( x+126-half_size, y+57, m_pHelpInfo->GetSurface() );
		}

		if( m_pTextIn )
			m_pTextIn->Draw();// ���� �Է�

		if( m_pBtn1 )
			m_pBtn1->DrawToBlt(); // Ȯ�ι�ư

		if( m_pCloseBtn ) // qna�� �ε尡 �ȵǹǷ� �� ������.
			m_pCloseBtn->DrawToBlt(); // �ݱ�.
	}
}

LRESULT CAutoMouse::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bDrag = FALSE; ///07-04-12 �����Էµ� �巡�� �����ϰ� ����
	static int DistX = 0, DistY = 0; // �巡�׽� Ŭ���� ���� ���콺 ��ǥ�� ���� ��ġ. ///07-04-12 �����Էµ� �巡�� �����ϰ� ����
	int x = 0, y = 0;
	if (g_pNk2DFrame
			&& g_pNk2DFrame->GetControlBottom()
			&& g_pNk2DFrame->GetControlBottom()->m_InputBox.GetEnable()
			&& m_pTextIn )
		m_pTextIn->SetEnable(FALSE);

	// Ÿ�Ժ� �޼��� ���ν����� �����ϴϱ� if~else ���� �ƿ� �Լ��� ������.
	if( m_Type == AUTOMOUSE_TYPE_QnA || m_Type == AUTOMOUSE_TYPE_QUIZ )
		return QnAMsgProc( hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if(!IsInside(x, y))//������ ���
			return 0;
		else if( !m_pBtn1->IsInside(x,y) && !m_pTextIn->IsInside(x,y) ) // �����ȿ��� �ٿ��̸�...///07-04-12 �����Էµ� �巡�� �����ϰ� ����
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

		// ��� ���� �巡�� �÷��״� Ǯ���ش�. ///07-04-12 �����Էµ� �巡�� �����ϰ� ����
		bDrag = FALSE;

		if(!IsInside(x, y))//������ ���
			return 0;

		if(!m_pBtn1)
			return 0;

		m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn1->GetState() == BTN_ACTION) 			//������ �޽��� ����
		{
			char *buf;
			buf = m_pTextIn->GetString();// or buf = m_pTextIn->GetStrBuf();

//============================================================================================
			// nate 2004 - 3
			// ���� Ȯ�� �ý���
			// ����ڰ� �Է��� ���ڰ����� ���� ����
			int nSize = sizeof( buf ) / sizeof( char );
			char arStr[ 256 ] = {0,};

			int i = 0;
			while( *buf )
			{
				if( !IsSpace( *buf ) )
					arStr[ i++ ] = *buf;

				buf++;
			}

			SendConfirm( arStr ); // ������ ģ ���ڷκ��� ��Ŷ�� ���� ������. ///�����Է½ý��ۺ���
			m_bOneFailed = FALSE; // ���н� �ѹ��� �ٽ� ���������� ����. ///�����Է½ý��ۺ���

			m_pBtn1->SetState(BTN_NORMAL);
			return 1;
		}

		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bDrag ) // �巡�� ���̸�... ///07-04-12 �����Էµ� �巡�� �����ϰ� ����
		{
			m_pSurfaceBack->Xpos = x + DistX;
			m_pSurfaceBack->Ypos = y + DistY;

			// ȭ�� �Ѿ�� ����.
			if( m_pSurfaceBack->Xpos < 0 )
				m_pSurfaceBack->Xpos = 0;
			if( m_pSurfaceBack->Ypos < 0 )
				m_pSurfaceBack->Ypos = 0;
			if( m_pSurfaceBack->Xpos > GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth() )
				m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth();
			if( m_pSurfaceBack->Ypos > GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
				m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();


			// �������� ������ ���� ��ġ
			if( m_pMunjaText )
				m_pMunjaText->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+65);
			// ���� �Է� ��ġ
			if( m_pTextIn )
				m_pTextIn->SetPos(m_pSurfaceBack->Xpos+11, m_pSurfaceBack->Ypos+65);

			// Ȯ�ι�ư ��ġ
			if( m_pBtn1 )
				m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+152, m_pSurfaceBack->Ypos+63 );

			m_pQuestionmark->Xpos = m_pSurfaceBack->Xpos + m_pSurfaceBack->GetWidth() - m_pQuestionmark->GetWidth()-1; // �׽�Ʈ �� ����
			m_pQuestionmark->Ypos = m_pSurfaceBack->Ypos + 1;
			m_pQuestionmark2->Xpos = m_pQuestionmark->Xpos;
			m_pQuestionmark2->Ypos = m_pQuestionmark->Ypos;
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)	// Ȯ�� ��ư
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
			// nate 2004 - 3 : ���� Ȯ�� �ý��� - ����ڰ� �Է��� ���ڰ����� ���� ����
			int nSize = sizeof( buf ) / sizeof( char );
			char arStr[ 256 ] = {0,};

			int i = 0;
			while( *buf )
			{
				if( !IsSpace( *buf ) )
					arStr[ i++ ] = *buf;

				buf++;
			}

			SendConfirm( arStr ); // ������ ģ ���ڷκ��� ��Ŷ�� ���� ������. ///�����Է½ý��ۺ���
			m_bOneFailed = FALSE; // ���н� �ѹ��� �ٽ� ���������� ����. ///�����Է½ý��ۺ���

			return 1;
		}

		if(wParam ==VK_TAB  )//Focus�� ä�� â���� �̵�
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
		if( m_pTextIn && m_pTextIn->GetEnable() ) ///07-04-12 ��Ŀ���� �������� �޵��� ����
			return m_pTextIn->MsgProc(hWnd, msg, wParam, lParam); ///07-04-12 ���ϰ��� ����.(��� �̰� ������ 0�����̶� �� ������ �ִ�)
		break;
	}
	return 0;
}

LRESULT CAutoMouse::QnAMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x = 0, y = 0;
	char strTemp[255];
	static BOOL bDrag = FALSE;
	static int DistX = 0, DistY = 0; // �巡�׽� Ŭ���� ���� ���콺 ��ǥ�� ���� ��ġ.

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

		if(!IsInside(x, y))//������ ���
			return 0;
		else if( !m_pBtn1->IsInside(x,y) && !m_pTextIn->IsInside(x,y) ) // �����ȿ��� �ٿ��̸�...
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

		// ��� ���� �巡�� �÷��״� Ǯ���ش�.
		bDrag = FALSE;

		if(!IsInside(x, y))//������ ���
			return 0;

		if(!m_pBtn1)
			return 0;

		m_pBtn1->MsgProc(hWnd, msg, wParam, lParam);

		if(m_pBtn1->GetState() == BTN_ACTION && strlen(m_pTextIn->GetStrBuf()) )			//������ �޽��� ����
		{
			if( m_Type == AUTOMOUSE_TYPE_QnA )
				sprintf( strTemp, "qna %s\n", m_pTextIn->GetStrBuf() );
			else if( m_Type == AUTOMOUSE_TYPE_QUIZ )
				sprintf( strTemp, "quiz %s\n", m_pTextIn->GetStrBuf() );

			if( g_pTcpIp )
				g_pTcpIp->SendNetMessage( strTemp );

			m_pBtn1->SetState(BTN_NORMAL);

			g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // â�� �ݴ´�.

			return 1;
		}

		if( m_pCloseBtn )
		{
			m_pCloseBtn->MsgProc(hWnd, msg, wParam, lParam);

			if( m_pCloseBtn->GetState() == BTN_ACTION )
			{
				g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // â�� �ݴ´�.
			}
		}

		break;

	case WM_MOUSEMOVE:
		x = LOWORD (lParam);
		y = HIWORD (lParam);

		if( bDrag ) // �巡�� ���̸�...
		{
			m_pSurfaceBack->Xpos = x + DistX;
			m_pSurfaceBack->Ypos = y + DistY;

			// ȭ�� �Ѿ�� ����.
			if( m_pSurfaceBack->Xpos < 0 )
				m_pSurfaceBack->Xpos = 0;
			if( m_pSurfaceBack->Ypos < 0 )
				m_pSurfaceBack->Ypos = 0;
			if( m_pSurfaceBack->Xpos > GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth() )
				m_pSurfaceBack->Xpos = GET_CLIENT_WIDTH() - m_pSurfaceBack->GetWidth();
			if( m_pSurfaceBack->Ypos > GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight() )
				m_pSurfaceBack->Ypos = GET_CLIENT_HEIGHT() - m_pSurfaceBack->GetHeight() - g_pNk2DFrame->GetControlBottom()->GetRefHeight();

			// ���� �Է� ��ġ
			if( m_pTextIn )
				m_pTextIn->SetPos(m_pSurfaceBack->Xpos+31, m_pSurfaceBack->Ypos+193);

			// Ȯ�ι�ư ��ġ
			if( m_pBtn1 )
				m_pBtn1->SetPosition(m_pSurfaceBack->Xpos+174, m_pSurfaceBack->Ypos+191 );

			if( m_pCloseBtn )
				m_pCloseBtn->SetPosition(m_pSurfaceBack->Xpos+215, m_pSurfaceBack->Ypos+12 );
		}

		if(m_pBtn1 && m_pBtn1->GetState() != BTN_DISABLE)	// Ȯ�� ��ư
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

			g_pNk2DFrame->ShowAutoMouseWindow( FALSE ); // â�� �ݴ´�.

			return 1;
		}

		if(wParam ==VK_TAB  )//Focus�� ä�� â���� �̵�
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
		if( m_pTextIn && m_pTextIn->GetEnable() ) ///07-04-12 ��Ŀ���� �������� �޵��� ����
			return m_pTextIn->MsgProc(hWnd, msg, wParam, lParam); ///07-04-12 ���ϰ��� ����.(��� �̰� ������ 0�����̶� �� ������ �ִ�)
		break;
	}
	return 0;
}


void CAutoMouse::AddWord(char *w1,char* w2,int flag,BOOL bAct)
{
	char a;
	int i;
	// �δܾ� �Է�
	if(strlen(w1)>=1)
	{
		// ������ ���� ���ڸ� ���ڷ� ��ȯ
		for( i = 0 ; i < 2 ; i ++ )
		{
			strncpy( &a, w1 + i, 1 );
			m_nNum[ i ] = atoi( &a );
		}
		strcpy(m_dic_word,w1);
	}

	if(strlen(w2)>=1)
	{
		// ������ ���� ���ڸ� ���ڷ� ��ȯ
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
// ���� Ȯ�� �ý���
void CAutoMouse::SetRandomNum()
{
	for( int i = 0 ; i < 4 ; i ++ )
		m_fRandom[ i ] = ( ( rand() % 10 ) * 0.1f ) - 0.5f;	// ȸ����
}

////////////////////////////////////////////////////////////////////////////////////////
// ������ : �̿���
// ������ : 05-04-08
// �������� : �����Է½ý��� ���� ó��. ///�����Է½ý��ۺ���
void CAutoMouse::MakeCheckNumber() // ���� ���۽� ����. üũ�ѹ��� ��ȣȭ ��Ʈ�� ���������� �����ش�.
{
	m_nDecodeBit = rand(); // üũ�ѹ��� ��ȣȭ ��Ʈ ���� ����� �����ϰ� ��������.

	for( int i = 0 ; i < NUM_OF_CHECKNUMBER ; ++i )
	{
		m_nRealCheckNumber[i] = rand()%99; // ���� ����� �����ϰ� �������� 4�ڸ� üũ �ѹ� �� 1, 3��ġ�� �ΰ� .( �� m_nDecodeBit������ ���� ��ȣȭ �Ǿ� ����ȴ�. )
		m_nRealCheckNumber[i] ^= m_nDecodeBit; // ��ȣȭ ó��.
	}
}

void CAutoMouse::SetConfirmNumber( int Arg1, int Arg2 ) // �����κ��� ���� ��ȣ�κ��� ������ȣ 5���� �����Ѵ�.
{
	m_nConfirmNumber[0] = Arg1 / 100; // 4�ڸ� ���� �� 2�ڸ�
	m_nConfirmNumber[1] = Arg1 % 100; // 4�ڸ� ���� �� 2�ڸ�
	m_nConfirmNumber[2] = Arg2 / 10000; // 6�ڸ� ���� �� 2�ڸ�
	m_nConfirmNumber[3] = (Arg2 % 10000) / 100; // 6�ڸ� ���� ��� 2�ڸ�
	m_nConfirmNumber[4] = Arg2 % 100; // 6�ڸ� ���� ������ 2�ڸ�
}

void CAutoMouse::SendConfirm( char *InputStr ) // ������ȣ�� ����ؼ� ������.
{
	char tmp[200];

	// ������ȣ 5���� �� Ÿ�ֿ̹� Ŭ���̾�Ʈ���� ����ؼ� ���س���.
	m_nConfirmNumber[5] = ( m_nConfirmNumber[0] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[6] = ( m_nConfirmNumber[1] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[7] = ( m_nConfirmNumber[2] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[8] = ( m_nConfirmNumber[3] * (pCMyApp->m_nCurHour+1) ) % 100;
	m_nConfirmNumber[9] = ( m_nConfirmNumber[4] * (pCMyApp->m_nCurHour+1) ) % 100;

	// Ŭ���̾�Ʈ ��ü üũ.
	if( strlen(InputStr)!=4 || (atoi(InputStr)/1000) != (m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)/10 // 4��°�ڸ�.
			|| (atoi(InputStr)%100)/10 != (m_nRealCheckNumber[ m_nNum[2] ] ^ m_nDecodeBit)%10 // 2��° �ڸ�.
	  )
	{
		// ���������� Ʋ�� �����ڵ带 �����ش�.
		sprintf( tmp, "autock %d\n"
				 , ((m_nNum[2]*m_nNum[1])%10*100000000)+(m_nNum[3]*10000000)+(m_nNum[1]*1000000) // �� 4�ڸ� (m_nNum[0]*m_nNum[1] �� ���ڸ�, m_nNum[0], m_nNum[1] �� ���ڸ���)
				 + (m_nConfirmNumber[ ( (g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+2) )%10 ] * ( ((g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+1))%10) * 1000) // ��� 3�ڸ� ( �����ڵ�[�����ε���]*�����ε��� ) ( �����ε��� = (�ڽ��� ĳ���� �ε��� ���ڸ�+(�ð���+1))%10  )
				 + (m_nNum[2]*100)+(m_nNum[1]*10)+(g_pRoh->m_nCharIndex%10) // ������ 3�ڸ� (m_nNum[2], m_nNum[3] �� ���ڸ���, �ڽ��� ĳ���� �ε��� ���ڸ�)
			   );

		if( g_pTcpIp )
			g_pTcpIp->SendNetMessage( tmp );

		return;
	}

	// ������ȣ�� ������ ��Ŀ�� �ִ��� �˾ƺ��� ����� ���� ������ ������� �ʴ´�.
	sprintf( tmp, "autock %d\n"
			 , ( ( (m_nNum[0]*(atoi(InputStr)%10) )%10*100000000)+(m_nNum[0]*10000000)+((atoi(InputStr)%10)*1000000) ) // �� 4�ڸ� (m_nNum[0]*m_nNum[1] �� ���ڸ�, m_nNum[0], m_nNum[1] �� ���ڸ���)
			 + (m_nConfirmNumber[ ( (g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+1) )%10 ] * ( ((g_pRoh->m_nCharIndex%10)+(pCMyApp->m_nCurHour+1))%10) * 1000) // ��� 3�ڸ� ( �����ڵ�[�����ε���]*�����ε��� ) ( �����ε��� = (�ڽ��� ĳ���� �ε��� ���ڸ�+(�ð���+1))%10  )
			 + (m_nNum[2]*100)+(( (atoi(InputStr)%1000)/100 )*10)+(g_pRoh->m_nCharIndex%10) // ������ 3�ڸ� (m_nNum[2], m_nNum[3] �� ���ڸ���, �ڽ��� ĳ���� �ε��� ���ڸ�)
		   );

	if (g_pTcpIp)
		g_pTcpIp->SendNetMessage(tmp);
}
